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
  const std::size_t offset_right = output.m_uiWidth/2;
  const std::size_t offset_low = output.m_uiHeight/2;
  //Channels are split in two steps: First all even rows, then all odd rows
  for(std::size_t i = 0; i <= 1; i++){
#pragma omp parallel for
    for(quint32 row = i; row < input.m_uiHeight; row+=2u){
      const T* src = ROW(T, input, row);
      T* dst_left = ROW(T, output, row/2 + offset_low * i);
      T* dst_right = dst_left+offset_right;
      for(std::size_t column = 0; column < input.m_uiWidth; column += 2){
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

   //HACK: There is no info about the cfa in the image data from polarized cameras, so we use the modelId to deduce it here
   if(pMetadataInput->m_iCameraModelId == 182){
     picInfoOutput.m_eColorFilterArray = excfaBG;
   }

   if (pMetadataOutput != nullptr && pMetadataInput != nullptr){
       *pMetadataOutput = *pMetadataInput;
        pMetadataOutput->updateFromPicBufInfo(&picInfoOutput);
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
