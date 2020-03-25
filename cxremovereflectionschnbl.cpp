#include "cxremovereflectionschnbl.h"
#include <ImageData.h>
#include <PicBufAPI.h>
#include <PicBufMacros.h>

CxRemoveReflectionsChnbl::CxRemoveReflectionsChnbl() : CxAbstractPolarChnbl("Remove Reflection")
{

}

CxChainable* CxRemoveReflectionsChnbl::clone(){
  return new CxRemoveReflectionsChnbl();
}

bool CxRemoveReflectionsChnbl::queryOutputImageInfo(const SxPicBufInfo &picInfoInput, SxPicBufInfo &picInfoOutput, const CxImageMetadata *pMetadataInput, CxImageMetadata *pMetadataOutput){
   // Initialize output with input
   picInfoOutput = picInfoInput;

   // One 2x2 pattern in the input image becomes one pixel in the output image
   picInfoOutput.m_uiWidth /= 2;
   picInfoOutput.m_uiHeight /= 2;

   picInfoOutput.m_uiStride = XICORE_ALIGN_IMG_STRIDE(picInfoOutput.m_uiWidth*picInfoOutput.bytesPerPixel());

   if (pMetadataOutput != NULL && pMetadataInput != NULL){
       *pMetadataOutput = *pMetadataInput;
   }
   return true;
}

bool CxRemoveReflectionsChnbl::processBuffers(const SxPicBuf &input, SxPicBuf &output){
  switch(input.m_eDataType){
    case extypeUInt8:
      return convertToMinimumImage<unsigned char>(input,output);
    case extypeUInt16:
      return convertToMinimumImage<unsigned short>(input,output);
    default:
      setErrorMessage("Unsupported data type!");
      return false;
  }
}

template<typename T>
bool CxRemoveReflectionsChnbl::convertToMinimumImage(const SxPicBuf& input, SxPicBuf& output)
{
#pragma omp parallel for
    // Loop over input image two rows at a time
    for(int row = 0; row < input.m_uiHeight; row+=2){
        const T* upper = ROW(T, input, row);
        const T* lower = ROW(T, input, row+1);
        T* dst = ROW(T, output, row/2);
        size_t column = 0;
        while (column < input.m_uiWidth){
          T upper_temp = *upper < *(upper+1) ? *upper : *(upper+1);
          T lower_temp =  *lower < *(lower+1) ? *lower : *(lower+1);
          *dst = upper_temp < lower_temp ? upper_temp : lower_temp;
          upper += 2;
          lower += 2;
          dst++;
          column += 2;
        }
    }
    return true;
}
