#include "polartohsvchnbl.h"
#include <ImageData.h>
#include <PicBufAPI.h>
#include <PicBufMacros.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

// Flags: Own memory pool is needed because of different format of output images
Q_INVOKABLE CxPolarToHSVChnbl::CxPolarToHSVChnbl():CxImageProvider(ExChainableFlags(CxChainable::eHasOwnMemoryPool | CxChainable::eCanChangeEnableState))
{

}

QString CxPolarToHSVChnbl::title() const {
    return tr("Polarization: Map to HSV");
}

bool CxPolarToHSVChnbl::acceptsDataFrom(CxChainable *pPrecedessor) {
    //The object accepts images (coming from CxImageProvider).
    return qobject_cast<CxImageProvider*>(pPrecedessor) != NULL;
}

int CxPolarToHSVChnbl::buffersCountInMemoryPool() const {
    // The buffer for the output image is allocated from Memory Pool
    return 1;
}

CxChainable* CxPolarToHSVChnbl::clone() {
    // Object can be cloned trivially
    return new CxPolarToHSVChnbl();
}

IxChainData* CxPolarToHSVChnbl::processData(IxChainData *pReceivedData) {
    // Received data should be an image
    CxImageData* inputImage = qobject_cast<CxImageData*>(pReceivedData);
    // If there was no  or invalid input, there shall be no output
    if(inputImage == NULL){
        setErrorMessage("Invalid input image!");
        return NULL;
    }
    // Get buffer from input image
    SxPicBuf &inputBuffer = inputImage->picBuf();
    CxImageMetadata inputMetadata = inputImage->imageMetadata();
    // New buffer for output image
    SxPicBuf outputBuffer;
    CxImageMetadata outputMetadata;

    // Calculate output image format and metadata
    if(!queryOutputImageInfo(inputBuffer, outputBuffer, &inputMetadata, &outputMetadata)){
        setErrorMessage("Could not query output image info");
        return NULL;
    }

    // Allocate buffer for output image from memory pool
    if(!CxPicBufAPI::AllocPicBufFromPool(m_hMemoryPool, this, outputBuffer, outputBuffer)){
        setErrorMessage("Could not allocate buffer!");
        return NULL;
    }

    // Main conversion algorithm populates outputBuffer based on inputBuffer
    switch(inputBuffer.m_eDataType){
      case extypeUInt8:
        convertPolarToHSV<unsigned char,8>(inputBuffer, outputBuffer);
        break;
      case extypeUInt16:
        switch(inputBuffer.m_uiBpc){ // Not passing bpc directly to allow for compile time optimizations
          case 8:
            convertPolarToHSV<unsigned short,8>(inputBuffer, outputBuffer);
            break;
          case 10:
            convertPolarToHSV<unsigned short, 10>(inputBuffer, outputBuffer);
            break;
          case 12:
            convertPolarToHSV<unsigned short, 12>(inputBuffer, outputBuffer);
            break;
          case 14:
            convertPolarToHSV<unsigned short, 14>(inputBuffer, outputBuffer);
            break;
          case 16:
            convertPolarToHSV<unsigned short, 16>(inputBuffer, outputBuffer);
            break;
          default:
            setErrorMessage("Unsupported input bit depth!");
            return NULL;
        }
        break;
      default:
        setErrorMessage("Unsupported input bit depth!");
        return NULL;
    }


    //Create output image
    CxImageData* result = new CxImageData();
    result->setPicBuf(outputBuffer);
    result->setImageMetadata(outputMetadata);

    // If we have made it this far, we can now delete the input image
    delete pReceivedData;

    return result;
}

template <typename T, int bpc>
bool CxPolarToHSVChnbl::convertPolarToHSV(const SxPicBuf &input, SxPicBuf &output)
{
#pragma omp parallel for
    // Loop over input image two rows at a time
    for(unsigned int row = 0; row < input.m_uiHeight; row+=2){
        // Pointers because performance
        const T* upper = ROW(T, input, row);
        const T* lower = ROW(T, input, row+1);
        PolarPixelRGB32* dst = ROW(PolarPixelRGB32, output, row/2);
        size_t column = 0;
        constexpr size_t stepsize = 4;
        while (column < input.m_uiWidth){
          size_t step = calculatePolarization<T, stepsize, bpc>(upper, lower, dst);
          upper += 2*step;
          lower += 2*step;
          dst += step;
          column += 2*step;
        }
    }
    return true;
}

bool CxPolarToHSVChnbl::queryOutputImageInfo(const SxPicBufInfo &picInfoInput, SxPicBufInfo &picInfoOutput, const CxImageMetadata *pMetadataInput, CxImageMetadata *pMetadataOutput)
{
   // Initialize output with input
   picInfoOutput = picInfoInput;
   // Output image is RGB32
   SET_PIC_RGB32(picInfoOutput)

   // One 2x2 pattern in the input image becomes one pixel in the output image
   picInfoOutput.m_uiWidth /= 2;
   picInfoOutput.m_uiHeight /= 2;

   // Recalculate stride
   picInfoOutput.m_uiStride = XICORE_ALIGN_IMG_STRIDE(picInfoOutput.m_uiWidth*picInfoOutput.bytesPerPixel());

   if (pMetadataOutput != NULL && pMetadataInput != NULL){
       *pMetadataOutput = *pMetadataInput;
       // RGB images do not have color filter arrays
       pMetadataOutput->m_eColorFilterArray = excfaNone;
   }
   return true;
}

// Adapted form wikipedia article on HSV (HSV to RGB conversion)
PolarPixelRGB32::PolarPixelRGB32(const float &angle_rad, const float &degree_of_polarization, const float &value){
  constexpr float kPi3 = (float) 3.0f / M_PI;
  const float chroma = degree_of_polarization * value;
  const float scaled_hue = angle_rad * kPi3;
  const float x = chroma * ( 1.0f - std::abs(scaled_hue - (std::floor(scaled_hue/2.0f)*2.0f)-1.0f));

#define SET_COLOR(R, G, B) {\
  red = (unsigned char) ((R - chroma + value)*256.0f);\
  green = (unsigned char) ((G - chroma + value)*256.0f);\
  blue = (unsigned char) ((B - chroma + value)*256.0f);\
}


  const int int_hue = (int) scaled_hue; //This may perform a little better
  switch (int_hue) {
        case 0: SET_COLOR(chroma, x, 0.0f) break;
        case 1: SET_COLOR(x, chroma, 0.0f) break;
        case 2: SET_COLOR(0.0f, chroma, x) break;
        case 3: SET_COLOR(0.0f, x, chroma) break;
        case 4: SET_COLOR(x, 0.0f, chroma) break;
        case 5: SET_COLOR(chroma, 0.0f, x) break;
        default:SET_COLOR(0.0f, 0.0f, 0.0f) break;
  }

#undef SET_COLOR
}

template<typename T, int vecWidth, int bpc>
size_t CxPolarToHSVChnbl::calculatePolarization(const T* upper, const T* lower, PolarPixelRGB32* dst)
{
  constexpr float kPi = (float)M_PI;
  constexpr float kPi4 = (float)M_PI_4;
  float measured_angles[vecWidth*2];
  float sums[vecWidth*2];

  // Inner loop: Can be completely vectorized in MSVC and partly in GCC
#pragma loop(ivdep)
#pragma GCC ivdep
  for(int i =0; i<vecWidth*2; i++){
    // Load data from pointer in vector register as float
    float pix_up = (float) upper[i];
    // Shuffle the lower roe (0,1,2,3) => (1,0,3,2)
    float pix_low = ((i & 1) * (float) lower[i-1]) + (((i+1) & 1) * (float) lower[i+1]);
    // Performance critical line: Divide upper by the sum of upper and lower, Sqrt and acos, then store
    measured_angles[i] = std::acos(std::sqrt(pix_up/(pix_up+pix_low)));
    sums[i] = pix_up + pix_low; // Only half of these values are actually used, but it does not matter for performance
  }

  //Next part is more branched, so it will not be vectorized
  for (int i = 0; i < vecWidth; i++){
    const float result1 = (measured_angles[(2*i)+1] > kPi4) ? -measured_angles[2*i] : measured_angles[2*i];
    float result2;
    if (measured_angles[2*i] < kPi4){
      result2 = kPi4 - measured_angles[(2*i)+1];
    }else if (measured_angles[(2*i)+1] <= kPi4){
      result2 = kPi4 + measured_angles[(2*i)+1];
    }else{
      result2 = kPi4 + measured_angles[(2*i)+1] - kPi;
    }
    float angle_difference = std::abs(result1-result2);
    const float degree_of_polarization = 1.0f - (angle_difference/kPi4);
    const float polarization_angle = ((result1+result2)) + kPi; // Result in rad shifted to avoid negative values
    constexpr float maxsum = (float) (1<<bpc) *2.0f; // Maximum value of two pixels added to each other at this bit depth
    dst[i] = PolarPixelRGB32(polarization_angle, degree_of_polarization, sums[2*i]/maxsum);
  }
  return vecWidth;
}
