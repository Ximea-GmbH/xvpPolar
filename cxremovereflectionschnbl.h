#ifndef CXREMOVEREFLECTIONSCHNBL_H
#define CXREMOVEREFLECTIONSCHNBL_H

#include "abstractpolarchnbl.h"

class CxRemoveReflectionsChnbl : public CxAbstractPolarChnbl
{
  Q_OBJECT
  Q_CLASSINFO("CustomName", "Polarization: Remove Reflection")
public:
  Q_INVOKABLE CxRemoveReflectionsChnbl();
  CxChainable*clone();
  bool queryOutputImageInfo(const SxPicBufInfo& picInfoInput, SxPicBufInfo& picInfoOutput, const CxImageMetadata* pMetadataInput, CxImageMetadata* pMetadataOutput);

protected:
  bool processBuffers(const SxPicBuf& input, SxPicBuf& output);

private:
  template<typename T>
  bool convertToMinimumImage(const SxPicBuf& input, SxPicBuf& output);
};

#endif // CXREMOVEREFLECTIONSCHNBL_H
