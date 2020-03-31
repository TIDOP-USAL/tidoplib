#ifndef TL_GRAPHIC_IMG_METADATA_H
#define TL_GRAPHIC_IMG_METADATA_H

#include "config_tl.h"
#include "tidop/core/defs.h"
#include "tidop/core/utils.h"

//namespace TL
//{
//
//template<typename T>
//class Data
//{
//private:
//
//  std::string mName;
//  T mValue;
//  T mDefValue;
//  typedef T type;
//
//public:
//
//  /*!
//   * \brief Constructora por defecto
//   */
//  Data(const std::string &name, const T defValue);
//  Data(const std::string &name, const T &value, const T defValue);
//
//  std::string name() const;
//
//  T value() const;
//  
//  void setValue(const T &value);
//  void reset();
//  Data<T> &operator = (const T &value);
//  operator const T&() const;
////  T &operator()();
////  T const &operator()() const;
////  bool active() const;
////  bool required() const;
//
//  std::string string() const;
//};
//
//template<typename T> inline
//Data<T>::Data(const std::string &name, const T defValue)
//  : mName(name),
//    mValue(defValue),
//    mDefValue(defValue)
//{
//}
//
//template<typename T> inline
//Data<T>::Data(const std::string &name, const T &value, const T defValue)
//  : mName(name),
//    mValue(value),
//    mDefValue(defValue)
//{
//}
//
//template<typename T> inline
//std::string Data<T>::name() const
//{
//  return mName;
//}
//
//template<typename T> inline
//T Data<T>::value() const
//{
//  return mValue;
//}
//
//template<typename T> inline
//void Data<T>::setValue(const T &value)
//{
//  mValue = value;
//}
//
//template<typename T>
//void Data<T>::reset()
//{
//  mValue = mDefValue;
//}
//
//template<typename T>
//Data<T> &Data<T>::operator =(const T &value)
//{
//  mValue = value;
//}
//
//template<typename T>
//T &Data::operator()()
//{
//  return mValue;
//}

//template<typename T>
//const TL::T &Data::operator()() const
//{
//  return mValue;
//}

//template<typename T>
//Data<T>::operator const T &() const
//{
//  return mValue;
//}

//template<typename T> inline
//bool Data<T>::active() const
//{
//  return bActive;
//}

//template<typename T> inline
//bool Data<T>::required() const
//{
//  return bRequired;
//}


//class TL_EXPORT ImgMetadata
//{
//
//public:
//
//  enum class Format
//  {
//    TIFF,
//    JPEG,
//    JP2000,
//    PNG,
//    BMP
//  };
//
//private:
//
//public:
//
//  ImgMetadata(){}
//  virtual ~ImgMetadata(){}
//
////  void read(const RasterGraphics &image);
////  void save(RasterGraphics &image);
//
//private:
//
//};
//
//
//class TL_EXPORT JpegMetadata : public ImgMetadata
//{
//public:
//
//  /*!
//   * \brief identificador del metadato
//   */
//  enum class ID
//  {
//    ExifDocumentName,
//    ExifImageDescription,
//    ExifMake,
//    ExifModel, 
//    ExifOrientation,
//    ExifXResolution,
//    ExifYResolution,
//    ExifResolutionUnit,
//    ExifTransferFunction,
//    ExifSoftware,
//    ExifDateTime,
//    ExifArtist,
//    ExifWhitePoint,
//    ExifPrimaryChromaticities,
//    ExifYCbCrCoefficients,
//    ExifYCbCrPositioning,
//    ExifReferenceBlackWhite,
//    ExifCopyright,
//    ExifExposureTime,
//    ExifFNumber,
//    ExifSpectralSensitivity,
//    ExifISOSpeedRatings,
//    ExifOECF,
//    ExifSensitivityType,
//    ExifStandardOutputSensitivity,
//    ExifRecommendedExposureIndex,
//    ExifISOSpeed,
//    ExifISOSpeedLatitudeyyy,
//    ExifISOSpeedLatitudezzz,
//    ExifExifVersion,
//    ExifDateTimeOriginal,
//    ExifDateTimeDigitized,
//    ExifOffsetTime,
//    ExifOffsetTimeOriginal,
//    ExifOffsetTimeDigitized,
//    ExifComponentsConfiguration,
//    ExifCompressedBitsPerPixel,
//    ExifShutterSpeedValue,
//    ExifApertureValue,
//    ExifBrightnessValue,
//    ExifExposureBiasValue,
//    ExifMaxApertureValue,
//    ExifSubjectDistance,
//    ExifMeteringMode,
//    ExifLightSource,
//    ExifFlash,
//    ExifFocalLength,
//    ExifSubjectArea,
//    ExifMakerNote,
//    ExifUserComment,
//    ExifSubSecTime,
//    ExifSubSecTime_Original,
//    ExifSubSecTime_Digitized,
//    ExifFlashpixVersion,
//    ExifColorSpace,
//    ExifPixelXDimension,
//    ExifPixelYDimension,
//    ExifRelatedSoundFile,
//    ExifFlashEnergy,
//    ExifSpatialFrequencyResponse,
//    ExifFocalPlaneXResolution,
//    ExifFocalPlaneYResolution,
//    ExifFocalPlaneResolutionUnit,
//    ExifSubjectLocation,
//    ExifExposureIndex,
//    ExifSensingMethod,
//    ExifFileSource,
//    ExifSceneType,
//    ExifCFAPattern,
//    ExifCustomRendered,
//    ExifExposureMode,
//    ExifWhiteBalance,
//    ExifDigitalZoomRatio,
//    ExifFocalLengthIn35mmFilm,
//    ExifSceneCaptureType,
//    ExifGainControl,
//    ExifContrast,
//    ExifSaturation,
//    ExifSharpness,
//    ExifDeviceSettingDescription,
//    ExifSubjectDistanceRange,
//    ExifImageUniqueID,
//    ExifCameraOwnerName,
//    ExifBodySerialNumber,
//    ExifLensSpecification,
//    ExifLensMake,
//    ExifLensModel,
//    ExifLensSerialNumber
//  };
//
//private:
//
////  std::string mExifDocumentName;
////  std::string mExifImageDescription;
////  std::string mExifMake;
////  std::string mExifModel;
////  unsigned short mExifOrientation;
////  double mExifXResolution;
////  double mExifYResolution;
////  unsigned short mExifResolutionUnit;
////  unsigned short mExifTransferFunction;
////  std::string mEXIF_Software;
////  std::string mEXIF_DateTime;
////  std::string mExifArtist;
//
////  //EXIF_WhitePoint	0x013E	RATIONAL	2	Optional
////  //EXIF_PrimaryChromaticities	0x013F	RATIONAL	6	Optional
////  //EXIF_YCbCrCoefficients	0x0211	RATIONAL	3	Optional
////  //EXIF_YCbCrPositioning	0x0213	SHORT	1	Mandatory
////  //EXIF_ReferenceBlackWhite	0x0214	RATIONAL	6	Optional
////  std::string mExifCopyright;
////  double mEXIF_ExposureTime;
////  double mEXIF_FNumber;
//
////  /*!
////   * Optional
////   * 0: Not defined
////   * 1: Manual
////   * 2: Normal program
////   * 3: Aperture priority
////   * 4: Shutter priority
////   * 5: Creative program
////   * 6: Action program
////   * 7: Portrait mode
////   * 8: Landscape mode
////   */
////  unsigned short mExifExposureProgram;
//
//  //EXIF_SpectralSensitivity	0x8824	ASCII	variable	Optional
//  
//  /*!
//   * \brief Velocidad ISO
//   * Optional
//   */
//  unsigned short mExifISOSpeedRatings;
//
//  //EXIF_OECF	0x8828	UNDEFINED	variable	Optional
//  //EXIF_SensitivityType	0x8830	SHORT	1	Optional
//  //EXIF_StandardOutputSensitivity	0x8831	LONG	1	Optional
//  //EXIF_RecommendedExposureIndex	0x8832	LONG	1	Optional
//  //EXIF_ISOSpeed	0x8833	LONG	1	Optional
//  //EXIF_ISOSpeedLatitudeyyy	0x8834	LONG	1	Optional
//  //EXIF_ISOSpeedLatitudezzz	0x8835	LONG	1	Optional
//  //EXIF_ExifVersion	0x9000	UNDEFINED	4	Mandatory
//  std::string mEXIF_DateTimeOriginal;
//  std::string mEXIF_DateTimeDigitized;
//  //EXIF_OffsetTime	0x9010	ASCII	7	Optional
//  //EXIF_OffsetTimeOriginal	0x9011	ASCII	7	Optional
//  //EXIF_OffsetTimeDigitized	0x9012	ASCII	7	Optional
//  //EXIF_ComponentsConfiguration	0x9101	UNDEFINED	4	Mandatory
//  //EXIF_CompressedBitsPerPixel	0x9102	RATIONAL	1	Optional
//  
//  /*!
//   * \brief Shutter speed (reciprocal of exposure time)
//   * Optional
//   */
//  double mExifShutterSpeedValue;
//
//  //EXIF_ApertureValue	0x9202	RATIONAL	1	Optional
//  //EXIF_BrightnessValue	0x9203	SRATIONAL	1	Optional
//
//  /*!
//   * \brief Exposure bias value in EV
//   * Optional
//   */
//  double mExifExposureBiasValue;
//  
////  //EXIF_MaxApertureValue	0x9205	RATIONAL	1	Optional
////  std::string mEXIF_SubjectDistance; //	0x9206	RATIONAL	1	Optional
////  //EXIF_MeteringMode	0x9207	SHORT	1	Optional
////  //EXIF_LightSource	0x9208	SHORT	1	Optional
////  
////  //0 = no flash, 1 = flash used
////  char mEXIF_Flash;
////  
////  /*!
////   * \brief Distancia focal de la lente en milímetros
////   * Opcional
////   */
////  double mExifFocalLength;
////
////  //EXIF_SubjectArea	0x9214	SHORT	variable	Optional
////  //EXIF_MakerNote	0x927C	UNDEFINED	variable	Optional
////  //EXIF_UserComment	0x9286	UNDEFINED	variable	Optional
////  //EXIF_SubSecTime	0x9290	ASCII	variable	Optional
////  //EXIF_SubSecTime_Original	0x9291	ASCII	variable	Optional
////  //EXIF_SubSecTime_Digitized	0x9292	ASCII	variable	Optional
////  //EXIF_FlashpixVersion	0xA000	UNDEFINED	4	Mandatory
////  //EXIF_ColorSpace	0xA001	SHORT	1	Mandatory
////  //EXIF_PixelXDimension	0xA002	LONG	1	Mandatory
////  //EXIF_PixelYDimension	0xA003	LONG	1	Mandatory
////  //EXIF_RelatedSoundFile	0xA004	ASCII	13	Optional
////  //EXIF_FlashEnergy	0xA20B	RATIONAL	1	Optional
////  //EXIF_SpatialFrequencyResponse	0xA20C	UNDEFINED	variable	Optional
////  //EXIF_FocalPlaneXResolution	0xA20E	RATIONAL	1	Optional
////  //EXIF_FocalPlaneYResolution	0xA20F	RATIONAL	1	Optional
////  //EXIF_FocalPlaneResolutionUnit	0xA210	SHORT	1	Optional
////  //EXIF_SubjectLocation	0xA214	SHORT	2	Optional
////  //EXIF_ExposureIndex	0xA215	RATIONAL	1	Optional
////  //EXIF_SensingMethod	0xA217	SHORT	1	Optional
////  //EXIF_FileSource	0xA300	UNDEFINED	1	Optional
////  //EXIF_SceneType	0xA301	UNDEFINED	1	Optional
////  //EXIF_CFAPattern	0xA302	UNDEFINED	variable	Optional
////  //EXIF_CustomRendered	0xA401	SHORT	1	Optional
////  //EXIF_ExposureMode	0xA402	SHORT	1	Recommended
////  //EXIF_WhiteBalance	0xA403	SHORT	1	Recommended
////  //EXIF_DigitalZoomRatio	0xA404	RATIONAL	1	Optional
////   
////  /*!
////   * \brief Distancia focal en película de 35 mm
////   * Opcional
////   */
////  double mExifFocalLengthIn35mmFilm;
////
////  //EXIF_SceneCaptureType	0xA406	SHORT	1	Recommended
////  //EXIF_GainControl	0xA407	RATIONAL	1	Optional
////  //EXIF_Contrast	0xA408	SHORT	1	Optional
////  //EXIF_Saturation	0xA409	SHORT	1	Optional
////  //EXIF_Sharpness	0xA40A	SHORT	1	Optional
////  //EXIF_DeviceSettingDescription	0xA40B	UNDEFINED	variable	Optional
////    
////  /*!
////   * \brief Distancia al punto de enfoque en metros
////   * Opcional
////   */
////  double mExifSubjectDistanceRange;
////
////  //EXIF_ImageUniqueID	0xA420	ASCII	33	Optional
////  //EXIF_CameraOwnerName	0xA430	ASCII	variable	Optional
////  //EXIF_BodySerialNumber	0xA431	ASCII	variable	Optional
////  //EXIF_LensSpecification	0xA432	RATIONAL	4	Optional
////  //EXIF_LensMake	0xA433	ASCII	variable	Optional
////  //EXIF_LensModel	0xA434	ASCII	variable	Optional
////  //EXIF_LensSerialNumber	0xA435	ASCII	variable	Optional
//
//public:
//
//  JpegMetadata();
//  ~JpegMetadata();
//
//  Data<std::string> ExifDocumentName;
//  Data<std::string> ExifImageDescription;
//  Data<std::string> ExifMake;
//  Data<std::string> ExifModel;
//  Data<unsigned short> ExifOrientation;
//  Data<double> ExifXResolution;
//  Data<double> ExifYResolution;
//  Data<unsigned short> ExifResolutionUnit;
//  Data<unsigned short> ExifTransferFunction;
//  Data<std::string> ExifSoftware;
//  Data<std::string> ExifDateTime;
//  Data<std::string> ExifArtist;
//  //  //EXIF_WhitePoint	0x013E	RATIONAL	2	Optional
//  //  //EXIF_PrimaryChromaticities	0x013F	RATIONAL	6	Optional
//  //  //EXIF_YCbCrCoefficients	0x0211	RATIONAL	3	Optional
//  //  //EXIF_YCbCrPositioning	0x0213	SHORT	1	Mandatory
//  //  //EXIF_ReferenceBlackWhite	0x0214	RATIONAL	6	Optional
//  Data<std::string> ExifCopyright;
//  Data<double> ExifExposureTime;
//  Data<double> ExifFNumber;
//
//  /*!
//   * Optional
//   * 0: Not defined
//   * 1: Manual
//   * 2: Normal program
//   * 3: Aperture priority
//   * 4: Shutter priority
//   * 5: Creative program
//   * 6: Action program
//   * 7: Portrait mode
//   * 8: Landscape mode
//   */
//  Data<unsigned short> ExifExposureProgram;
//
//private:
//
//};





//}

#endif // TL_GRAPHIC_IMG_METADATA_H
