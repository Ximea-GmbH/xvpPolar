#ifndef POLARTOHSVCHNBL_H
#define POLARTOHSVCHNBL_H

#include<Chainable.h>

struct PolarPixel{
  float angle_deg;
  float degree_of_polarization;
  float value;
};

struct PolarPixelRGB32{
  inline PolarPixelRGB32(const float& angle_rad, const float& degree_of_polarization, const float& value);
  unsigned char red;
  unsigned char green;
  unsigned char blue;
  unsigned char alpha = 0; // Not in use
};

class CxPolarToHSVChnbl: public CxImageProvider
{
    Q_OBJECT
    Q_CLASSINFO("CustomName", "Polar To HSV")
public:
    Q_INVOKABLE CxPolarToHSVChnbl();

    // CxChainable interface
public:
    virtual QString title() const;
    virtual bool acceptsDataFrom(CxChainable *pPrecedessor);
    virtual int buffersCountInMemoryPool() const;
    virtual CxChainable *clone();

protected:
    virtual IxChainData *processData(IxChainData *pReceivedData);

private:
    template <typename T, int bpc>
    bool convertPolarToHSV(const SxPicBuf& input, SxPicBuf& output);

    template<typename T, int vecWidth, int bpc>
    inline size_t calculatePolarization(const T* upper, const T*lower, PolarPixelRGB32* dst);

    // CxImageProvider interface
public:
    virtual bool queryOutputImageInfo(const SxPicBufInfo &picInfoInput, SxPicBufInfo &picInfoOutput, const CxImageMetadata *pMetadataInput, CxImageMetadata *pMetadataOutput);
};



#endif // POLARTOHSVCHNBL_H
