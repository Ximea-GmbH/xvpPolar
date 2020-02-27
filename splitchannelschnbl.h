#ifndef CXSPLITCHANNELSCHNBL_H
#define CXSPLITCHANNELSCHNBL_H

#include<Chainable.h>

class CxSplitChannelsChnbl : public CxImageProvider
{
  Q_OBJECT
  Q_CLASSINFO("CustomName", "Polarization: Split Channels")
public:
  Q_INVOKABLE CxSplitChannelsChnbl();
  virtual QString title() const;
  virtual bool acceptsDataFrom(CxChainable *pPrecedessor);
  virtual int buffersCountInMemoryPool() const;
  virtual CxChainable *clone();
  virtual bool queryOutputImageInfo(const SxPicBufInfo &picInfoInput, SxPicBufInfo &picInfoOutput, const CxImageMetadata *pMetadataInput, CxImageMetadata *pMetadataOutput);

protected:
  virtual IxChainData *processData(IxChainData *pReceivedData);

private:
  template<typename T>
  bool splitChannels(const SxPicBuf &input, SxPicBuf &output);

};

#endif // CXSPLITCHANNELSCHNBL_H
