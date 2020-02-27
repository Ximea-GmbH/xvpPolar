#include "abstractpolarchnbl.h"
#include <ImageData.h>
#include <PicBufAPI.h>


QString CxAbstractPolarChnbl::title() const
{
  return m_title;
}

int CxAbstractPolarChnbl::buffersCountInMemoryPool() const
{
  return 1;
}

bool CxAbstractPolarChnbl::acceptsDataFrom(CxChainable* pPredecessor)
{
  //The object acceps images (coming form CxImageProvider)
  return qobject_cast<CxImageProvider*>(pPredecessor);
}

CxAbstractPolarChnbl::CxAbstractPolarChnbl(const QString& title):
  CxImageProvider(ExChainableFlags(CxChainable::eHasOwnMemoryPool | CxChainable::eCanChangeEnableState)),
  m_title(QString("Polarization: ")+title)
{

}

IxChainData*CxAbstractPolarChnbl::processData(IxChainData* pReceivedData)
{
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

  if(!processBuffers(inputBuffer, outputBuffer)){
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


