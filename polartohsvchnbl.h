#ifndef POLARTOHSVCHNBL_H
#define POLARTOHSVCHNBL_H

#include "abstractpolarchnbl.h"

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

class CxPolarToHSVChnbl: public CxAbstractPolarChnbl
{
    Q_OBJECT
    Q_CLASSINFO("CustomName", "Polarization: Map to HSV")
public:
    Q_INVOKABLE CxPolarToHSVChnbl();
    CxChainable *clone();
    bool queryOutputImageInfo(const SxPicBufInfo &picInfoInput, SxPicBufInfo &picInfoOutput, const CxImageMetadata *pMetadataInput, CxImageMetadata *pMetadataOutput);

protected:
    bool processBuffers(const SxPicBuf &input, SxPicBuf &output);

private:
    template <typename T, int bpc>
    bool convertPolarToHSV(const SxPicBuf& input, SxPicBuf& output);

    template<typename T, int vecWidth, int bpc>
    inline std::size_t calculatePolarization(const T* upper, const T*lower, PolarPixelRGB32* dst);

};



#endif // POLARTOHSVCHNBL_H
