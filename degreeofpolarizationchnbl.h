#ifndef CXDEGREEOFPOLARIZATIONCHNBL_H
#define CXDEGREEOFPOLARIZATIONCHNBL_H

#include<Chainable.h>

class CxDegreeOfPolarizationChnbl : public CxImageProvider
{
  Q_OBJECT
  Q_CLASSINFO("CustomName", "Polarization: Degree of polarization")
public:
  Q_INVOKABLE CxDegreeOfPolarizationChnbl();

  virtual QString title() const;
  virtual bool acceptsDataFrom(CxChainable *pPrecedessor);
  virtual int buffersCountInMemoryPool() const;
  virtual CxChainable *clone();
  virtual bool queryOutputImageInfo(const SxPicBufInfo &picInfoInput, SxPicBufInfo &picInfoOutput, const CxImageMetadata *pMetadataInput, CxImageMetadata *pMetadataOutput);

protected:
  virtual IxChainData *processData(IxChainData *pReceivedData);

private:
  template <typename T, int bpc>
  bool convertToDOP(const SxPicBuf& input, SxPicBuf& output);

  template <typename T, int vecWidth, int bpc>
  int calculateDOP(const T* upper, const T* lower, T* dst);

};

#endif // CXDEGREEOFPOLARIZATIONCHNBL_H
