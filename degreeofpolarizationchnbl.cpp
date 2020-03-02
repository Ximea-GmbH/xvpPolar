#include "degreeofpolarizationchnbl.h"
#include <ImageData.h>
#include <PicBufAPI.h>
#include <PicBufMacros.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

// Flags: Own memory pool need because of different format of output images
CxDegreeOfPolarizationChnbl::CxDegreeOfPolarizationChnbl(): CxAbstractPolarChnbl("Degree of Polarization")
{

}

CxChainable* CxDegreeOfPolarizationChnbl::clone() {
    // Object can be cloned trivially
  return new CxDegreeOfPolarizationChnbl();
}


bool CxDegreeOfPolarizationChnbl::processBuffers(const SxPicBuf &input, SxPicBuf &output){
  switch(input.m_eDataType){
    case extypeUInt8:
      convertToDOP<unsigned char, 8>(input, output);
      break;
    case extypeUInt16:
      switch(input.m_uiBpc){ // Not passing bpc directly to allow for compile time optimizations
        case 8:
          convertToDOP<unsigned short, 8>(input, output);
          break;
        case 10:
          convertToDOP<unsigned short, 10>(input, output);
          break;
        case 12:
          convertToDOP<unsigned short, 12>(input, output);
          break;
        case 14:
          convertToDOP<unsigned short, 14>(input, output);
          break;
        case 16:
          convertToDOP<unsigned short, 16>(input, output);
          break;
        default:
          setErrorMessage("Unsupported input bit depth!");
          return false;
      }
      break;
    default:
      setErrorMessage("Unsupported input image bit depth!");
      return false;
  }
  return true;
}


bool CxDegreeOfPolarizationChnbl::queryOutputImageInfo(const SxPicBufInfo& picInfoInput, SxPicBufInfo& picInfoOutput, const CxImageMetadata* pMetadataInput, CxImageMetadata* pMetadataOutput){
   // Initialize output with input
   picInfoOutput = picInfoInput;

   // One 2x2 pattern in the input image becomes one pixel in the output image
   picInfoOutput.m_uiWidth /= 2;
   picInfoOutput.m_uiHeight /= 2;

   // Recalculate stride
   picInfoOutput.m_uiStride = XICORE_ALIGN_IMG_STRIDE(picInfoOutput.m_uiWidth*picInfoOutput.bytesPerPixel());

   if (pMetadataOutput != NULL && pMetadataInput != NULL){
       *pMetadataOutput = *pMetadataInput;
   }

   // TODO: Find out how the color version reports its CFA
   return true;

}

template<typename T, int bpc>
bool CxDegreeOfPolarizationChnbl::convertToDOP(const SxPicBuf& input, SxPicBuf& output){
#pragma omp parallel for
  // Loop over input image two rows at a time
  for(unsigned int row = 0; row < input.m_uiHeight; row+=2){
    // Pointers because performance
    const T* upper = ROW(T, input, row);
    const T* lower = ROW(T, input, row+1);
    T* dst = ROW(T, output, row/2);
    size_t column = 0;
    constexpr size_t stepsize = 4;
    while (column+2*stepsize <= input.m_uiWidth){
      size_t step = calculateDOP<T, stepsize, bpc>(upper, lower, dst);
      upper += 2*step;
      lower += 2*step;
      dst += step;
      column += 2*step;
    }
  }
  return true;
}

template<typename T, int vecWidth, int bpc>
int CxDegreeOfPolarizationChnbl::calculateDOP(const T* upper, const T* lower, T* dst){
  constexpr float kPi = (float)M_PI;
  constexpr float kPi4 = (float) M_PI_4;
  float measured_angles[vecWidth*2];
  constexpr float maxValue = (float) (1<<bpc);

#pragma loop(ivdep)
#pragma loop GCC ivdep
  for (int i = 0; i<vecWidth*2; i++){
    //Load data from pointer to vector register as float
    float pix_up = (float) upper[i];
    // Shuffle the lower row (0,1,2,3)=>(1,0,3,2)
    float pix_low = ((i & 1) * (float) lower[i-1]) + (((i+1) & 1) * (float) lower[i+1]);
    // Performance critical line: Divide upper by the sum of upper and lower, Sqrt and acos, then store
    measured_angles[i] = std::acos(std::sqrt(pix_up/(pix_up+pix_low)));
  }

  // Next part is more branched, so it will not be vectorized
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
    dst[i]= (1.0f - (angle_difference/kPi4)) * maxValue;
  }

  return vecWidth;
}
