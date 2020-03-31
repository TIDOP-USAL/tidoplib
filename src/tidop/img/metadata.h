#ifndef TL_IMAGE_METADATA_H
#define TL_IMAGE_METADATA_H

#include "config_tl.h"
#include "tidop/core/defs.h"
#include "tidop/core/utils.h"


namespace tl
{

class ImageHandler;

class TL_EXPORT ImgMetadata
{

public:

  enum class Format
  {
    tiff,
    jpeg,
    jp2000,
    png,
    bmp
  };

public:

  ImgMetadata(Format format){}
  virtual ~ImgMetadata(){}

  virtual void setImageHandler(ImageHandler *imageHandler) = 0;

};


class TL_EXPORT EXIFMetadata 
  //: public ImgMetadata
{

public:

  EXIFMetadata();
  EXIFMetadata(ImageHandler *imageHandler);
  ~EXIFMetadata();

  void setImageHandler(ImageHandler *imageHandler);

  std::string exifDocumentName() const;
  //void setExifDocumentName(const std::string &exifDocumentName);

  std::string exifImageDescription() const;
  void setExifImageDescription(const std::string &exifImageDescription);

  std::string exifMake() const;
  //void setExifMake(const std::string &exifMake);

  std::string exifModel() const;
  //void setExifModel(const std::string &exifModel);

  /*!
   * 0: Unspecified
   * 1: upper left of image
   * 3: lower right of image
   * 6: upper right of image
   * 8: lower left of image
   * 9: undefined
   */
  uint16_t exifOrientation() const;
  void setExifOrientation(uint16_t exifOrientation);

  double exifXResolution() const;
  void setExifXResolution(double exifXResolution);

  double exifYResolution() const;
  void setExifYResolution(double exifYResolution);

  uint16_t exifResolutionUnit() const;
  void setExifResolutionUnit(uint16_t exifResolutionUnit);

  uint16_t exifTransferFunction() const;
  //void setExifTransferFunction(uint16_t exifTransferFunction);

  std::string exifSoftware() const;
  void setExifSoftware(const std::string &exifSoftware);

  std::string exifDateTime() const;
  void setExifDateTime(const std::string &exifDateTime);

  std::string exifArtist() const;
  //void setExifArtist(const std::string &exifArtist);

  //EXIF_WhitePoint	0x013E	RATIONAL	2	Optional
  //EXIF_PrimaryChromaticities	0x013F	RATIONAL	6	Optional
  //EXIF_YCbCrCoefficients	0x0211	RATIONAL	3	Optional
 
  uint16_t exifYCbCrPositioning() const;
  //void setExifYCbCrPositioning(uint16_t exifYCbCrPositioning);

  //EXIF_ReferenceBlackWhite	0x0214	RATIONAL	6	Optional
  
  std::string exifCopyright() const;
  //void setExifCopyright(const std::string &exifCopyright);

  double exifExposureTime() const;
  void setExifExposureTime(double exifExposureTime);

  double exifFNumber() const;
  void setExifFNumber(double exifFNumber);

  /*!
   * 0: Not defined
   * 1: Manual
   * 2: Normal program
   * 3: Aperture priority
   * 4: Shutter priority
   * 5: Creative program
   * 6: Action program
   * 7: Portrait mode
   * 8: Landscape mode
   */
  uint16_t exifExposureProgram() const;
  void setExifExposureProgram(uint16_t exifExposureProgram);

  std::string exifSpectralSensitivity() const;
  //void setExifSpectralSensitivity(const std::string &exifSpectralSensitivity);

  /*!
   * \brief Velocidad ISO
   */
  uint16_t exifISOSpeedRatings() const;
  void setExifISOSpeedRatings(uint16_t exifISOSpeedRatings);

  uint16_t exifSensitivityType() const;
  //void setExifSensitivityType(uint16_t exifSensitivityType);

  long exifStandardOutputSensitivity() const;
  long exifRecommendedExposureIndex() const;
  long exifISOSpeed() const;
  long exifISOSpeedLatitudeyyy() const;
  long exifISOSpeedLatitudezzz() const;
  std::string exifVersion() const;

  std::string exifDateTimeOriginal() const;
  void setExifDateTimeOriginal(const std::string &exifDateTimeOriginal);

  std::string exifDateTimeDigitized() const;
  void setExifDateTimeDigitized(const std::string &exifDateTimeDigitized);
  
  std::string exifOffsetTime() const;
  std::string exifOffsetTimeOriginal() const;
  std::string exifOffsetTimeDigitized() const;
  double exifCompressedBitsPerPixel() const;

  /*!
   * \brief Shutter speed (reciprocal of exposure time)
   */
  double exifShutterSpeedValue() const;
  void setExifShutterSpeedValue(double exifShutterSpeedValue);

  double exifApertureValue() const;
  void setExifApertureValue(double exifApertureValue);

//  //exifBrightnessValue	0x9203	SRATIONAL	1	Optional

  /*!
   * \brief Exposure bias value in EV
   * Optional
   */
  double exifExposureBiasValue() const;
  
  double exifMaxApertureValue() const;
  void setExifMaxApertureValue(double exifMaxApertureValue);

  /*!
   * \brief Distancia al objeto
   */
  double exifSubjectDistance() const;
  void setExifSubjectDistance(double exifSubjectDistance);

  uint16_t exifMeteringMode() const;
  void setExifMeteringMode(uint16_t exifMeteringMode);

////  //EXIF_LightSource	0x9208	SHORT	1	Optional

  //0 = no flash, 1 = flash used
  uint16_t exifFlash() const;
  void setExifFlash(uint16_t exifFlash);
  
  /*!
   * \brief Distancia focal de la lente en milímetros
   * Opcional
   */
  double exifFocalLength() const;
  void setExifFocalLength(double exifFocalLength);

//  //EXIF_SubjectArea	0x9214	SHORT	variable	Optional
//  //EXIF_MakerNote	0x927C	UNDEFINED	variable	Optional
//  //EXIF_UserComment	0x9286	UNDEFINED	variable	Optional
//  //EXIF_SubSecTime	0x9290	ASCII	variable	Optional
//  //EXIF_SubSecTime_Original	0x9291	ASCII	variable	Optional
//  //EXIF_SubSecTime_Digitized	0x9292	ASCII	variable	Optional
//  //EXIF_FlashpixVersion	0xA000	UNDEFINED	4	Mandatory

  uint16_t exifColorSpace() const;
  void setExifColorSpace(uint16_t exifColorSpace);

  long exifPixelXDimension() const;
  long exifPixelYDimension() const;


//  //EXIF_RelatedSoundFile	0xA004	ASCII	13	Optional
//  //EXIF_FlashEnergy	0xA20B	RATIONAL	1	Optional
//  //EXIF_SpatialFrequencyResponse	0xA20C	UNDEFINED	variable	Optional
  
  double exifFocalPlaneXResolution() const;
  void setExifFocalPlaneXResolution(double exifFocalPlaneXResolution);

  double exifFocalPlaneYResolution() const;
  void setExifFocalPlaneYResolution(double exifFocalPlaneYResolution);

  uint16_t exifFocalPlaneResolutionUnit() const;
  void setExifFocalPlaneResolutionUnit(uint16_t exifFocalPlaneResolutionUnit);

  //  //EXIF_SubjectLocation	0xA214	SHORT	2	Optional
//  //EXIF_ExposureIndex	0xA215	RATIONAL	1	Optional
//  //EXIF_SensingMethod	0xA217	SHORT	1	Optional
//  //EXIF_FileSource	0xA300	UNDEFINED	1	Optional
//  //EXIF_SceneType	0xA301	UNDEFINED	1	Optional
//  //EXIF_CFAPattern	0xA302	UNDEFINED	variable	Optional
//  //EXIF_CustomRendered	0xA401	SHORT	1	Optional
//  //EXIF_ExposureMode	0xA402	SHORT	1	Recommended
//  //EXIF_WhiteBalance	0xA403	SHORT	1	Recommended
//  //EXIF_DigitalZoomRatio	0xA404	RATIONAL	1	Optional
   
  /*!
   * \brief Distancia focal en película de 35 mm
   */
  double exifFocalLengthIn35mmFilm() const;
  void setExifFocalLengthIn35mmFilm(double exifFocalLengthIn35mmFilm);

  //EXIF_SceneCaptureType	0xA406	SHORT	1	Recommended
  //EXIF_GainControl	0xA407	RATIONAL	1	Optional
  //EXIF_Contrast	0xA408	SHORT	1	Optional
  //EXIF_Saturation	0xA409	SHORT	1	Optional
  //EXIF_Sharpness	0xA40A	SHORT	1	Optional
  //EXIF_DeviceSettingDescription	0xA40B	UNDEFINED	variable	Optional
    
  /*!
   * \brief Distancia al punto de enfoque en metros
   * Opcional
   */
  double exifSubjectDistanceRange() const;

  //EXIF_ImageUniqueID	0xA420	ASCII	33	Optional
  //EXIF_CameraOwnerName	0xA430	ASCII	variable	Optional
  //EXIF_BodySerialNumber	0xA431	ASCII	variable	Optional
  //EXIF_LensSpecification	0xA432	RATIONAL	4	Optional
  //EXIF_LensMake	0xA433	ASCII	variable	Optional
  //EXIF_LensModel	0xA434	ASCII	variable	Optional
  //EXIF_LensSerialNumber	0xA435	ASCII	variable	Optiona

  //exifGPSVersionID	0x0000	BYTE	4	Optional
  std::string exifGPSLatitudeRef() const;
  double exifGPSLatitude() const;
  std::string  exifGPSLongitudeRef() const;
  double exifGPSLongitude() const;
  //exifGPSAltitudeRef	0x0005	BYTE	1	Optional
  double exifGPSAltitude() const;
  double exifGPSTimeStamp() const;
  std::string  exifGPSSatellites() const;
  std::string  exifGPSStatus() const;
  std::string  exifGPSMeasureMode() const;
  double exifGPSDOP() const;
  std::string  exifGPSSpeedRef() const;
  double exifGPSSpeed() const;
  std::string  exifGPSTrackRef() const;
  double exifGPSTrack() const;
  std::string  exifGPSImgDirectionRef() const;
  double exifGPSImgDirection() const;
  std::string  exifGPSMapDatum() const;
  std::string exifGPSDestLatitudeRef() const;
  double exifGPSDestLatitude() const;
  std::string exifGPSDestLongitudeRef() const;
  double exifGPSDestLongitude() const;
  std::string  exifGPSDestBearingRef() const;
  double exifGPSDestBearing() const;
  std::string  exifGPSDestDistanceRef() const;
  double exifGPSDestDistance() const;
  //exifGPSProcessingMethod	0x001B	UNDEFINED	variable	Optional
  //exifGPSAreaInformation	0x001C	UNDEFINED	variable	Optional
  std::string  exifGPSDateStamp() const;
  uint16_t exifGPSDifferential() const;
  double exifGPSHPositioningError() const;

private:

  ImageHandler *mImageHandler;

};


class TL_EXPORT JpegMetadata 
  : public ImgMetadata,
    public EXIFMetadata
{

public:

  JpegMetadata();
  JpegMetadata(ImageHandler *imageHandler);
  ~JpegMetadata();

  void setImageHandler(ImageHandler *imageHandler) override;

protected:

  ImageHandler *mImageHandler;

};


class TL_EXPORT TiffMetadata
  : public ImgMetadata,
    public EXIFMetadata
{

public:

  TiffMetadata();
  TiffMetadata(ImageHandler *imageHandler);
  ~TiffMetadata();

  void setImageHandler(ImageHandler *imageHandler) override;

  std::string documentName() const;
  void setDocumentName(const std::string &documentName);

  std::string imageDescription() const;
  void setImageDescription(const std::string &imageDescription);

  std::string software() const;
  void setSoftware(const std::string &software);

  std::string datetime() const;
  void setDatetime(const std::string &datetime);
    
  std::string artist() const;
  void setArtist(const std::string &artist);

  std::string hostComputer() const;
  void setHostComputer(const std::string &hostComputer);

  std::string copyright() const;
  void setCopyright(const std::string &copyright);

  std::string xResolution() const;
  void setXResolution(const std::string &xResolution);

  std::string yResolution() const;
  void setYResolution(const std::string &yResolution);

  std::string resolutionUnit() const;
  void setResolutionUnit(const std::string &resolutionUnit);

  std::string minSampleValue() const;
  std::string maxSampleValue() const;

protected:

  ImageHandler *mImageHandler;

};



class TL_EXPORT JP2000Metadata
  : public ImgMetadata
{

public:

  JP2000Metadata();
  JP2000Metadata(ImageHandler *imageHandler);
  ~JP2000Metadata();

  void setImageHandler(ImageHandler *imageHandler) override;

protected:

  ImageHandler *mImageHandler;

};


}  // End namespace tl

#endif // TL_IMAGE_METADATA_H
