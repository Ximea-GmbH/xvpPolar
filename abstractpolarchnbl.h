#ifndef CXABSTRACTPOLARCHNBL_H
#define CXABSTRACTPOLARCHNBL_H

#include<Chainable.h>

class CxAbstractPolarChnbl: public CxImageProvider
{
public:
  QString title() const;
  int buffersCountInMemoryPool() const;
  bool acceptsDataFrom(CxChainable *pPredecessor);

protected:
  CxAbstractPolarChnbl(const QString& title);
  IxChainData *processData(IxChainData *pReceivedData);
  virtual bool processBuffers(const SxPicBuf& input, SxPicBuf& output) = 0;

private:
  QString m_title;
};


#endif // CXABSTRACTPOLARCHNBL_H
