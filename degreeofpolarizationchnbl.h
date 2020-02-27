#ifndef CXDEGREEOFPOLARIZATIONCHNBL_H
#define CXDEGREEOFPOLARIZATIONCHNBL_H

#include "abstractpolarchnbl.h"

class CxDegreeOfPolarizationChnbl : public CxAbstractPolarChnbl
{
  Q_OBJECT
  Q_CLASSINFO("CustomName", "Polarization: Degree of polarization")
public:
  Q_INVOKABLE CxDegreeOfPolarizationChnbl();

  CxChainable *clone();
  bool queryOutputImageInfo(const SxPicBufInfo &picInfoInput, SxPicBufInfo &picInfoOutput, const CxImageMetadata *pMetadataInput, CxImageMetadata *pMetadataOutput);

protected:
  bool processBuffers(const SxPicBuf &input, SxPicBuf &output);

private:
  template <typename T, int bpc>
  bool convertToDOP(const SxPicBuf& input, SxPicBuf& output);

  template <typename T, int vecWidth, int bpc>
  int calculateDOP(const T* upper, const T* lower, T* dst);

};

#endif // CXDEGREEOFPOLARIZATIONCHNBL_H
