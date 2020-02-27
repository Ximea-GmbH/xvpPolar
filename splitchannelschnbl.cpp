#include "splitchannelschnbl.h"
#include <ImageData.h>
#include <PicBufAPI.h>
#include <PicBufMacros.h>

Q_INVOKABLE CxSplitChannelsChnbl::CxSplitChannelsChnbl(): CxImageProvider(ExChainableFlags(CxChainable::eHasOwnMemoryPool | CxChainable::eCanChangeEnableState))
{

}

QString CxSplitChannelsChnbl::title() const
{
  return tr("Polarization: Split Channels");
}

bool CxSplitChannelsChnbl::acceptsDataFrom(CxChainable *pPrecedessor){
  //The object acceps images (coming form CxImageProvider)
  return qobject_cast<CxImageProvider*>(pPrecedessor);
  // I am only now realizing that I spelled pedecessor wrong everywhere
  // TODO: Introduce some proper english
}


CxChainable* CxSplitChannelsChnbl::clone() {
  return new CxSplitChannelsChnbl();
}

int CxSplitChannelsChnbl::buffersCountInMemoryPool() const{
  return 1;
}

IxChainData* CxSplitChannelsChnbl::processData(IxChainData *pReceivedData){
  CxImageData* inputImage = qobject_cast<CxImageData*>(pReceivedData);
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
  switch(inputBuffer.m_eDataType){
    case extypeUInt8:
      splitChannels<unsigned char>(inputBuffer, outputBuffer);
      break;
    case extypeUInt16:
      splitChannels<unsigned short>(inputBuffer, outputBuffer);
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
