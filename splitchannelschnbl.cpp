#include "splitchannelschnbl.h"
#include <ImageData.h>
#include <PicBufAPI.h>
#include <PicBufMacros.h>

Q_INVOKABLE CxSplitChannelsChnbl::CxSplitChannelsChnbl(): CxAbstractPolarChnbl("Split Channels"){
}

CxChainable* CxSplitChannelsChnbl::clone() {
  return new CxSplitChannelsChnbl();
}

template<typename T>
bool CxSplitChannelsChnbl::splitChannels(const SxPicBuf& input, SxPicBuf& output)
{
  const size_t offset_right = output.m_uiWidth/2;
  const size_t offset_low = output.m_uiHeight/2;
  //Channels are split in two steps: First all even rows, then all odd rows
  for(size_t i = 0; i <= 1; i++){
#pragma omp parallel for
    for(unsigned int row = i; row < input.m_uiHeight; row+=2){
      const T* src = ROW(T, input, row);
      T* dst_left = ROW(T, output, row/2 + offset_low * i);
      T* dst_right = dst_left+offset_right;
      for(size_t column = 0; column < input.m_uiWidth; column += 2){
        *(dst_left++) = *(src++);
        *(dst_right++) = *(src++);
      }
    }

  }
  return true;
}


bool CxSplitChannelsChnbl::queryOutputImageInfo(const SxPicBufInfo &picInfoInput, SxPicBufInfo &picInfoOutput, const CxImageMetadata *pMetadataInput, CxImageMetadata *pMetadataOutput){

   // Initialize output with input
   picInfoOutput = picInfoInput;

   if (pMetadataOutput != NULL && pMetadataInput != NULL){
       *pMetadataOutput = *pMetadataInput;
   }
   return true;
}

bool CxSplitChannelsChnbl::processBuffers(const SxPicBuf& input, SxPicBuf& output)
{
  switch(input.m_eDataType){
    case extypeUInt8:
      splitChannels<unsigned char>(input, output);
      break;
    case extypeUInt16:
      splitChannels<unsigned short>(input, output);
      break;
    default:
      setErrorMessage("Unsupported input data type!");
      return false;
  }
  return true;
}
