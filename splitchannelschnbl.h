#ifndef CXSPLITCHANNELSCHNBL_H
#define CXSPLITCHANNELSCHNBL_H

#include "abstractpolarchnbl.h"

class CxSplitChannelsChnbl : public CxAbstractPolarChnbl
{
  Q_OBJECT
  Q_CLASSINFO("CustomName", "Polarization: Split Channels")
public:
  Q_INVOKABLE CxSplitChannelsChnbl();
  CxChainable *clone();
  bool queryOutputImageInfo(const SxPicBufInfo &picInfoInput, SxPicBufInfo &picInfoOutput, const CxImageMetadata *pMetadataInput, CxImageMetadata *pMetadataOutput);

protected:
  bool processBuffers(const SxPicBuf &input, SxPicBuf &output);

private:
  template<typename T>
  bool splitChannels(const SxPicBuf &input, SxPicBuf &output);

};

#endif // CXSPLITCHANNELSCHNBL_H
