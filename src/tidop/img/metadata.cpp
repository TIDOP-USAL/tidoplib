#include "metadata.h"

//#include "tidop/img/imghandler.h"

namespace tl
{
//
//
//EXIFMetadata::EXIFMetadata()
//  : mImageHandler(nullptr)
//{}
//
//EXIFMetadata::EXIFMetadata(ImageHandler *imageHandler)
//  : mImageHandler(imageHandler)
//{
//}
//
//EXIFMetadata::~EXIFMetadata()
//{
//}
//
//void EXIFMetadata::setImageHandler(ImageHandler *imageHandler)
//{
//  mImageHandler = imageHandler;
//}
//
//std::string EXIFMetadata::exifDocumentName() const
//{
//  std::string exifDocumentName;
//  if (mImageHandler) {
//    exifDocumentName = mImageHandler->metadata("EXIF_Document_Name");
//  }
//  return exifDocumentName;
//}
//
////void EXIFMetadata::setExifDocumentName(const std::string &exifDocumentName)
////{
////  if (mImageHandler) {
////    mImageHandler->setMetadata("EXIF_Document_Name", exifDocumentName);
////  }
////}
//
//std::string EXIFMetadata::exifImageDescription() const
//{
//  std::string exifImageDescription;
//  if (mImageHandler) {
//    exifImageDescription = mImageHandler->metadata("EXIF_ImageDescription");
//  }
//  return exifImageDescription;
//}
//
//void EXIFMetadata::setExifImageDescription(const std::string &exifImageDescription)
//{
//  if (mImageHandler) {
//    mImageHandler->setMetadata("EXIF_ImageDescription", exifImageDescription);
//  }
//}
//
//std::string EXIFMetadata::exifMake() const
//{
//  std::string exifMake;
//  if (mImageHandler) {
//    exifMake = mImageHandler->metadata("EXIF_Make");
//  }
//  return exifMake;
//}
//
////void EXIFMetadata::setExifMake(const std::string &exifMake)
////{
////  if (mImageHandler) {
////    mImageHandler->setMetadata("EXIF_Make", exifMake);
////  }
////}
//
//std::string EXIFMetadata::exifModel() const
//{
//  std::string exifModel;
//  if (mImageHandler) {
//    exifModel = mImageHandler->metadata("EXIF_Model");
//  }
//  return exifModel;
//}
//
////void EXIFMetadata::setExifModel(const std::string &exifModel)
////{
////  if (mImageHandler) {
////    mImageHandler->setMetadata("EXIF_Model", exifModel);
////  }
////}
//
//uint16_t EXIFMetadata::exifOrientation() const
//{
//  uint16_t exifOrientation;
//  if (mImageHandler) {
//    std::string s_orientation = mImageHandler->metadata("EXIF_Orientation");
//    exifOrientation = static_cast<uint16_t>(std::stoi(s_orientation));
//  }
//  return exifOrientation;
//}
//
//void EXIFMetadata::setExifOrientation(uint16_t exifOrientation)
//{
//  if (mImageHandler) {
//    mImageHandler->setMetadata("EXIF_Orientation", std::to_string(exifOrientation));
//  }
//}
//
//double EXIFMetadata::exifXResolution() const
//{
//  double exifXResolution;
//  if (mImageHandler) {
//    std::string sXResolution = mImageHandler->metadata("EXIF_XResolution");
//    exifXResolution = std::stod(sXResolution);
//  }
//  return exifXResolution;
//}
//
//void EXIFMetadata::setExifXResolution(double exifXResolution)
//{
//  if (mImageHandler) {
//    mImageHandler->setMetadata("EXIF_XResolution", std::to_string(exifXResolution));
//  }
//}
//
//double EXIFMetadata::exifYResolution() const
//{
//  double exifYResolution;
//  if (mImageHandler) {
//    std::string sYResolution = mImageHandler->metadata("EXIF_YResolution");
//    exifYResolution = std::stod(sYResolution);
//  }
//  return exifYResolution;
//}
//
//void EXIFMetadata::setExifYResolution(double exifYResolution)
//{
//  if (mImageHandler) {
//    mImageHandler->setMetadata("EXIF_YResolution", std::to_string(exifYResolution));
//  }
//}
//
//uint16_t EXIFMetadata::exifResolutionUnit() const
//{
//  uint16_t exifResolutionUnit;
//  if (mImageHandler) {
//    std::string sResolutionUnit = mImageHandler->metadata("EXIF_ResolutionUnit");
//    exifResolutionUnit = static_cast<uint16_t>(std::stoi(sResolutionUnit));
//  }
//  return exifResolutionUnit;
//}
//
//void EXIFMetadata::setExifResolutionUnit(uint16_t exifResolutionUnit)
//{
//  if (mImageHandler) {
//    mImageHandler->setMetadata("EXIF_ResolutionUnit", std::to_string(exifResolutionUnit));
//  }
//}
//
//uint16_t EXIFMetadata::exifTransferFunction() const
//{
//  uint16_t exifTransferFunction;
//  if (mImageHandler) {
//    std::string sTransferFunction = mImageHandler->metadata("EXIF_TransferFunction");
//    exifTransferFunction = static_cast<uint16_t>(std::stoi(sTransferFunction));
//  }
//  return exifTransferFunction;
//}
//
////void EXIFMetadata::setExifTransferFunction(uint16_t exifTransferFunction)
////{
////  if (mImageHandler) {
////    mImageHandler->setMetadata("EXIF_TransferFunction", std::to_string(exifTransferFunction));
////  }
////}
//
//std::string EXIFMetadata::exifSoftware() const
//{
//  std::string exifSoftware;
//  if (mImageHandler) {
//    exifSoftware = mImageHandler->metadata("EXIF_Software");
//  }
//  return exifSoftware;
//}
//
//void EXIFMetadata::setExifSoftware(const std::string &exifSoftware)
//{
//  if (mImageHandler) {
//    mImageHandler->setMetadata("EXIF_Software", exifSoftware);
//  }
//}
//
//std::string EXIFMetadata::exifDateTime() const
//{
//  std::string exifDateTime;
//  if (mImageHandler) {
//    exifDateTime = mImageHandler->metadata("EXIF_DateTime");
//  }
//  return exifDateTime;
//}
//
//void EXIFMetadata::setExifDateTime(const std::string &exifDateTime)
//{
//  if (mImageHandler) {
//    mImageHandler->setMetadata("EXIF_DateTime", exifDateTime);
//  }
//}
//
//std::string EXIFMetadata::exifArtist() const
//{
//  std::string exifArtist;
//  if (mImageHandler) {
//    exifArtist = mImageHandler->metadata("EXIF_Artist");
//  }
//  return exifArtist;
//}
//
////void EXIFMetadata::setExifArtist(const std::string &exifArtist)
////{
////  if (mImageHandler) {
////    mImageHandler->setMetadata("EXIF_Artist", exifArtist);
////  }
////}
//
//uint16_t EXIFMetadata::exifYCbCrPositioning() const
//{
//  uint16_t exifYCbCrPositioning;
//  if (mImageHandler) {
//    std::string sYCbCrPositioning = mImageHandler->metadata("EXIF_YCbCrPositioning");
//    exifYCbCrPositioning = static_cast<uint16_t>(std::stoi(sYCbCrPositioning));
//  }
//  return exifYCbCrPositioning;
//}
//
////void EXIFMetadata::setExifYCbCrPositioning(uint16_t exifYCbCrPositioning)
////{
////  if (mImageHandler) {
////    mImageHandler->setMetadata("EXIF_YCbCrPositioning", std::to_string(exifYCbCrPositioning));
////  }
////}
//
//std::string EXIFMetadata::exifCopyright() const
//{
//  std::string exifCopyright;
//  if (mImageHandler) {
//    exifCopyright = mImageHandler->metadata("EXIF_Copyright");
//  }
//  return exifCopyright;
//}
//
////void EXIFMetadata::setExifCopyright(const std::string &exifCopyright)
////{
////  if (mImageHandler) {
////    mImageHandler->setMetadata("EXIF_Copyright", exifCopyright);
////  }
////}
//
//double EXIFMetadata::exifExposureTime() const
//{
//  double exifExposureTime;
//  if (mImageHandler) {
//    std::string sExposureTime = mImageHandler->metadata("EXIF_ExposureTime");
//    exifExposureTime = std::stod(sExposureTime);
//  }
//  return exifExposureTime;
//}
//
//void EXIFMetadata::setExifExposureTime(double exifExposureTime)
//{
//  if (mImageHandler) {
//    mImageHandler->setMetadata("EXIF_ExposureTime", std::to_string(exifExposureTime));
//  }
//}
//
//double EXIFMetadata::exifFNumber() const
//{
//  double exifFNumber;
//  if (mImageHandler) {
//    std::string sFNumber = mImageHandler->metadata("EXIF_FNumber");
//    exifFNumber = std::stod(sFNumber);
//  }
//  return exifFNumber;
//}
//
//void EXIFMetadata::setExifFNumber(double exifFNumber)
//{
//  if (mImageHandler) {
//    mImageHandler->setMetadata("EXIF_FNumber", std::to_string(exifFNumber));
//  }
//}
//
//uint16_t EXIFMetadata::exifExposureProgram() const
//{
//  uint16_t exifExposureProgram;
//  if (mImageHandler) {
//    std::string sExposureProgram = mImageHandler->metadata("EXIF_ExposureProgram");
//    exifExposureProgram = static_cast<uint16_t>(std::stoi(sExposureProgram));
//  }
//  return exifExposureProgram;
//}
//
//void EXIFMetadata::setExifExposureProgram(uint16_t exifExposureProgram)
//{
//  if (mImageHandler) {
//    mImageHandler->setMetadata("EXIF_ExposureProgram", std::to_string(exifExposureProgram));
//  }
//}
//
//std::string EXIFMetadata::exifSpectralSensitivity() const
//{
//  std::string exifSpectralSensitivity;
//  if (mImageHandler) {
//    exifSpectralSensitivity = mImageHandler->metadata("EXIF_SpectralSensitivity");
//  }
//  return exifSpectralSensitivity;
//}
//
////void EXIFMetadata::setExifSpectralSensitivity(const std::string &exifSpectralSensitivity)
////{
////  if (mImageHandler) {
////    mImageHandler->setMetadata("EXIF_SpectralSensitivity", exifSpectralSensitivity);
////  }
////}
//
//uint16_t EXIFMetadata::exifISOSpeedRatings() const
//{
//  uint16_t exifISOSpeedRatings;
//  if (mImageHandler) {
//    std::string sISOSpeedRatings = mImageHandler->metadata("EXIF_ISOSpeedRatings");
//    exifISOSpeedRatings = static_cast<uint16_t>(std::stoi(sISOSpeedRatings));
//  }
//  return exifISOSpeedRatings;
//}
//
//void EXIFMetadata::setExifISOSpeedRatings(uint16_t exifISOSpeedRatings)
//{
//  if (mImageHandler) {
//    mImageHandler->setMetadata("EXIF_ISOSpeedRatings", std::to_string(exifISOSpeedRatings));
//  }
//}
//
//uint16_t EXIFMetadata::exifSensitivityType() const
//{
//  uint16_t exifSensitivityType;
//  if (mImageHandler) {
//    std::string sSensitivityType = mImageHandler->metadata("EXIF_SensitivityType");
//    exifSensitivityType = static_cast<uint16_t>(std::stoi(sSensitivityType));
//  }
//  return exifSensitivityType;
//}
//
////void EXIFMetadata::setExifSensitivityType(uint16_t exifSensitivityType)
////{
////}
//
//long EXIFMetadata::exifStandardOutputSensitivity() const
//{
//  long exifStandardOutputSensitivity;
//  if (mImageHandler) {
//    std::string sStandardOutputSensitivity = mImageHandler->metadata("EXIF_StandardOutputSensitivity");
//    exifStandardOutputSensitivity = std::stol(sStandardOutputSensitivity);
//  }
//  return exifStandardOutputSensitivity;
//}
//
//long EXIFMetadata::exifRecommendedExposureIndex() const
//{
//  long exifRecommendedExposureIndex;
//  if (mImageHandler) {
//    std::string sRecommendedExposureIndex = mImageHandler->metadata("EXIF_RecommendedExposureIndex");
//    exifRecommendedExposureIndex = std::stol(sRecommendedExposureIndex);
//  }
//  return exifRecommendedExposureIndex;
//}
//
//long EXIFMetadata::exifISOSpeed() const
//{
//  long exifISOSpeed;
//  if (mImageHandler) {
//    std::string sISOSpeed = mImageHandler->metadata("EXIF_ISOSpeed");
//    exifISOSpeed = std::stol(sISOSpeed);
//  }
//  return exifISOSpeed;
//}
//
//long EXIFMetadata::exifISOSpeedLatitudeyyy() const
//{
//  long exifISOSpeedLatitudeyyy;
//  if (mImageHandler) {
//    std::string sISOSpeedLatitudeyyy = mImageHandler->metadata("EXIF_ISOSpeedLatitudeyyy");
//    exifISOSpeedLatitudeyyy = std::stol(sISOSpeedLatitudeyyy);
//  }
//  return exifISOSpeedLatitudeyyy;
//}
//
//long EXIFMetadata::exifISOSpeedLatitudezzz() const
//{
//  long exifISOSpeedLatitudezzz;
//  if (mImageHandler) {
//    std::string sISOSpeedLatitudezzz = mImageHandler->metadata("EXIF_ISOSpeedLatitudezzz");
//    exifISOSpeedLatitudezzz = std::stol(sISOSpeedLatitudezzz);
//  }
//  return exifISOSpeedLatitudezzz;
//}
//
//std::string EXIFMetadata::exifVersion() const
//{
//  std::string exifVersion;
//  if (mImageHandler) {
//    exifVersion = mImageHandler->metadata("EXIF_ExifVersion");
//  }
//  return exifVersion;
//}
//
//std::string EXIFMetadata::exifDateTimeOriginal() const
//{
//  std::string exifDateTimeOriginal;
//  if (mImageHandler) {
//    exifDateTimeOriginal = mImageHandler->metadata("EXIF_DateTimeOriginal");
//  }
//  return exifDateTimeOriginal;
//}
//
//void EXIFMetadata::setExifDateTimeOriginal(const std::string &exifDateTimeOriginal)
//{
//  if (mImageHandler) {
//    mImageHandler->setMetadata("EXIF_DateTimeOriginal", exifDateTimeOriginal);
//  }
//}
//
//std::string EXIFMetadata::exifDateTimeDigitized() const
//{
//  std::string exifDateTimeDigitized;
//  if (mImageHandler) {
//    exifDateTimeDigitized = mImageHandler->metadata("EXIF_DateTimeDigitized");
//  }
//  return exifDateTimeDigitized;
//}
//
//void EXIFMetadata::setExifDateTimeDigitized(const std::string &exifDateTimeDigitized)
//{
//  if (mImageHandler) {
//    mImageHandler->setMetadata("EXIF_DateTimeDigitized", exifDateTimeDigitized);
//  }
//}
//
//std::string EXIFMetadata::exifOffsetTime() const
//{
//  std::string exifOffsetTime;
//  if (mImageHandler) {
//    exifOffsetTime = mImageHandler->metadata("EXIF_OffsetTime");
//  }
//  return exifOffsetTime;
//}
//
//std::string EXIFMetadata::exifOffsetTimeOriginal() const
//{
//  std::string exifOffsetTimeOriginal;
//  if (mImageHandler) {
//    exifOffsetTimeOriginal = mImageHandler->metadata("EXIF_OffsetTimeOriginal");
//  }
//  return exifOffsetTimeOriginal;
//}
//
//std::string EXIFMetadata::exifOffsetTimeDigitized() const
//{
//  std::string exifOffsetTimeDigitized;
//  if (mImageHandler) {
//    exifOffsetTimeDigitized = mImageHandler->metadata("EXIF_OffsetTimeDigitized");
//  }
//  return exifOffsetTimeDigitized;
//}
//
//double EXIFMetadata::exifCompressedBitsPerPixel() const
//{
//  double exifCompressedBitsPerPixel;
//  if (mImageHandler) {
//    std::string sCompressedBitsPerPixel = mImageHandler->metadata("EXIF_CompressedBitsPerPixel");
//    exifCompressedBitsPerPixel = std::stod(sCompressedBitsPerPixel);
//  }
//  return exifCompressedBitsPerPixel;
//}
//
//double EXIFMetadata::exifShutterSpeedValue() const
//{
//  double exifShutterSpeedValue;
//  if (mImageHandler) {
//    std::string sShutterSpeedValue = mImageHandler->metadata("EXIF_ShutterSpeedValue");
//    exifShutterSpeedValue = std::stod(sShutterSpeedValue);
//  }
//  return exifShutterSpeedValue;
//}
//
//void EXIFMetadata::setExifShutterSpeedValue(double exifShutterSpeedValue)
//{
//  if (mImageHandler) {
//    mImageHandler->setMetadata("EXIF_ShutterSpeedValue", std::to_string(exifShutterSpeedValue));
//  }
//}
//
//double EXIFMetadata::exifApertureValue() const
//{
//  double exifApertureValue;
//  if (mImageHandler) {
//    std::string sApertureValue = mImageHandler->metadata("EXIF_ApertureValue");
//    exifApertureValue = std::stod(sApertureValue);
//  }
//  return exifApertureValue;
//}
//
//void EXIFMetadata::setExifApertureValue(double exifApertureValue)
//{
//  if (mImageHandler) {
//    mImageHandler->setMetadata("EXIF_ApertureValue", std::to_string(exifApertureValue));
//  }
//}
//
//double EXIFMetadata::exifExposureBiasValue() const
//{
//  double exifExposureBiasValue;
//  if (mImageHandler) {
//    std::string sExposureBiasValue = mImageHandler->metadata("EXIF_ExposureBiasValue");
//    exifExposureBiasValue = std::stod(sExposureBiasValue);
//  }
//  return exifExposureBiasValue;
//}
//
//double EXIFMetadata::exifMaxApertureValue() const
//{
//  double exifMaxApertureValue;
//  if (mImageHandler) {
//    std::string sMaxApertureValue = mImageHandler->metadata("EXIF_MaxApertureValue");
//    exifMaxApertureValue = std::stod(sMaxApertureValue);
//  }
//  return exifMaxApertureValue;
//}
//
//void EXIFMetadata::setExifMaxApertureValue(double exifMaxApertureValue)
//{
//  if (mImageHandler) {
//    mImageHandler->setMetadata("EXIF_MaxApertureValue", std::to_string(exifMaxApertureValue));
//  }
//}
//
//double EXIFMetadata::exifSubjectDistance() const
//{
//  double exifSubjectDistance;
//  if (mImageHandler) {
//    std::string sSubjectDistance = mImageHandler->metadata("EXIF_SubjectDistance");
//    exifSubjectDistance = std::stod(sSubjectDistance);
//  }
//  return exifSubjectDistance;
//}
//
//void EXIFMetadata::setExifSubjectDistance(double exifSubjectDistance)
//{
//  if (mImageHandler) {
//    mImageHandler->setMetadata("EXIF_SubjectDistance", std::to_string(exifSubjectDistance));
//  }
//}
//
//uint16_t EXIFMetadata::exifMeteringMode() const
//{
//  uint16_t exifMeteringMode;
//  if (mImageHandler) {
//    std::string sMeteringMode = mImageHandler->metadata("EXIF_MeteringMode");
//    exifMeteringMode = static_cast<uint16_t>(std::stoi(sMeteringMode));
//  }
//  return exifMeteringMode;
//}
//
//void EXIFMetadata::setExifMeteringMode(uint16_t exifMeteringMode)
//{
//  if (mImageHandler) {
//    mImageHandler->setMetadata("EXIF_MeteringMode", std::to_string(exifMeteringMode));
//  }
//}
//
//uint16_t EXIFMetadata::exifFlash() const
//{
//  uint16_t exifFlash;
//  if (mImageHandler) {
//    std::string sFlash = mImageHandler->metadata("EXIF_Flash");
//    exifFlash = static_cast<uint16_t>(std::stoi(sFlash));
//  }
//  return exifFlash;
//}
//
//void EXIFMetadata::setExifFlash(uint16_t exifFlash)
//{
//  if (mImageHandler) {
//    mImageHandler->setMetadata("EXIF_Flash", std::to_string(exifFlash));
//  }
//}
//
//double EXIFMetadata::exifFocalLength() const
//{
//  double exifFocalLength;
//  if (mImageHandler) {
//    std::string sFocalLength = mImageHandler->metadata("EXIF_FocalLength");
//    exifFocalLength = std::stod(sFocalLength);
//  }
//  return exifFocalLength;
//}
//
//void EXIFMetadata::setExifFocalLength(double exifFocalLength)
//{
//  if (mImageHandler) {
//    mImageHandler->setMetadata("EXIF_FocalLength", std::to_string(exifFocalLength));
//  }
//}
//
//uint16_t EXIFMetadata::exifColorSpace() const
//{
//  uint16_t exifColorSpace;
//  if (mImageHandler) {
//    std::string sColorSpace = mImageHandler->metadata("EXIF_ColorSpace");
//    exifColorSpace = static_cast<uint16_t>(std::stoi(sColorSpace));
//  }
//  return exifColorSpace;
//}
//
//void EXIFMetadata::setExifColorSpace(uint16_t exifColorSpace)
//{
//  if (mImageHandler) {
//    mImageHandler->setMetadata("EXIF_ColorSpace", std::to_string(exifColorSpace));
//  }
//}
//
//long EXIFMetadata::exifPixelXDimension() const
//{
//  long exifPixelXDimension;
//  if (mImageHandler) {
//    std::string sPixelXDimension = mImageHandler->metadata("EXIF_PixelXDimension");
//    exifPixelXDimension = std::stol(sPixelXDimension);
//  }
//  return exifPixelXDimension;
//}
//
//long EXIFMetadata::exifPixelYDimension() const
//{
//  long exifPixelYDimension;
//  if (mImageHandler) {
//    std::string sPixelYDimension = mImageHandler->metadata("EXIF_PixelYDimension");
//    exifPixelYDimension = std::stol(sPixelYDimension);
//  }
//  return exifPixelYDimension;
//}
//
//double EXIFMetadata::exifFocalPlaneXResolution() const
//{
//  double exifFocalPlaneXResolution;
//  if (mImageHandler) {
//    std::string sFocalPlaneXResolution = mImageHandler->metadata("EXIF_FocalPlaneXResolution");
//    exifFocalPlaneXResolution = std::stod(sFocalPlaneXResolution);
//  }
//  return exifFocalPlaneXResolution;
//}
//
//void EXIFMetadata::setExifFocalPlaneXResolution(double exifFocalPlaneXResolution)
//{
//  if (mImageHandler) {
//    mImageHandler->setMetadata("EXIF_FocalPlaneXResolution", std::to_string(exifFocalPlaneXResolution));
//  }
//}
//
//double EXIFMetadata::exifFocalPlaneYResolution() const
//{
//  double exifFocalPlaneYResolution;
//  if (mImageHandler) {
//    std::string sFocalPlaneYResolution = mImageHandler->metadata("EXIF_FocalPlaneYResolution");
//    exifFocalPlaneYResolution = std::stod(sFocalPlaneYResolution);
//  }
//  return exifFocalPlaneYResolution;
//}
//
//void EXIFMetadata::setExifFocalPlaneYResolution(double exifFocalPlaneYResolution)
//{
//  if (mImageHandler) {
//    mImageHandler->setMetadata("EXIF_FocalPlaneYResolution", std::to_string(exifFocalPlaneYResolution));
//  }
//}
//
//uint16_t EXIFMetadata::exifFocalPlaneResolutionUnit() const
//{
//  uint16_t exifFocalPlaneResolutionUnit;
//  if (mImageHandler) {
//    std::string sFocalPlaneResolutionUnit = mImageHandler->metadata("EXIF_FocalPlaneResolutionUnit");
//    exifFocalPlaneResolutionUnit = static_cast<uint16_t>(std::stoi(sFocalPlaneResolutionUnit));
//  }
//  return exifFocalPlaneResolutionUnit;
//}
//
//void EXIFMetadata::setExifFocalPlaneResolutionUnit(uint16_t exifFocalPlaneResolutionUnit)
//{
//  if (mImageHandler) {
//    mImageHandler->setMetadata("EXIF_FocalPlaneResolutionUnit", std::to_string(exifFocalPlaneResolutionUnit));
//  }
//}
//
//double EXIFMetadata::exifFocalLengthIn35mmFilm() const
//{
//  double exifFocalLengthIn35mmFilm;
//  if (mImageHandler) {
//    std::string sFocalLengthIn35mmFilm = mImageHandler->metadata("EXIF_FocalLengthIn35mmFilm");
//    exifFocalLengthIn35mmFilm = std::stod(sFocalLengthIn35mmFilm);
//  }
//  return exifFocalLengthIn35mmFilm;
//}
//
//void EXIFMetadata::setExifFocalLengthIn35mmFilm(double exifFocalLengthIn35mmFilm)
//{
//  if (mImageHandler) {
//    mImageHandler->setMetadata("EXIF_FocalLengthIn35mmFilm", std::to_string(exifFocalLengthIn35mmFilm));
//  }
//}
//
//double EXIFMetadata::exifSubjectDistanceRange() const
//{
//  double exifSubjectDistanceRange;
//  if (mImageHandler) {
//    std::string sSubjectDistanceRange = mImageHandler->metadata("EXIF_SubjectDistanceRange");
//    exifSubjectDistanceRange = std::stod(sSubjectDistanceRange);
//  }
//  return exifSubjectDistanceRange;
//}
//
//std::string EXIFMetadata::exifGPSLatitudeRef() const
//{
//  std::string exifGPSLatitudeRef;
//  if (mImageHandler) {
//    exifGPSLatitudeRef = mImageHandler->metadata("EXIF_GPSLatitudeRef");
//  }
//  return exifGPSLatitudeRef;
//}
//
//double EXIFMetadata::exifGPSLatitude() const
//{
//  double exifGPSLatitude;
//  if (mImageHandler) {
//    std::string sGPSLatitude = mImageHandler->metadata("EXIF_GPSLatitude");
//    exifGPSLatitude = std::stod(sGPSLatitude);
//  }
//  return exifGPSLatitude;
//}
//
//std::string EXIFMetadata::exifGPSLongitudeRef() const
//{
//  std::string exifGPSLongitudeRef;
//  if (mImageHandler) {
//    exifGPSLongitudeRef = mImageHandler->metadata("EXIF_GPSLongitudeRef");
//  }
//  return exifGPSLongitudeRef;
//}
//
//double EXIFMetadata::exifGPSLongitude() const
//{
//  double exifGPSLongitude;
//  if (mImageHandler) {
//    std::string sGPSLongitude = mImageHandler->metadata("EXIF_GPSLongitude");
//    exifGPSLongitude = std::stod(sGPSLongitude);
//  }
//  return exifGPSLongitude;
//}
//
//double EXIFMetadata::exifGPSAltitude() const
//{
//  double exifGPSAltitude;
//  if (mImageHandler) {
//    std::string sGPSAltitude = mImageHandler->metadata("EXIF_GPSAltitude");
//    exifGPSAltitude = std::stod(sGPSAltitude);
//  }
//  return exifGPSAltitude;
//}
//
//double EXIFMetadata::exifGPSTimeStamp() const
//{
//  double exifGPSLatitude;
//  if (mImageHandler) {
//    std::string sGPSLatitude = mImageHandler->metadata("EXIF_GPSLatitude");
//    exifGPSLatitude = std::stod(sGPSLatitude);
//  }
//  return exifGPSLatitude;
//}
//
//std::string EXIFMetadata::exifGPSSatellites() const
//{
//  std::string exifGPSSatellites;
//  if (mImageHandler) {
//    exifGPSSatellites = mImageHandler->metadata("EXIF_GPSSatellites");
//  }
//  return exifGPSSatellites;
//}
//
//std::string EXIFMetadata::exifGPSStatus() const
//{
//  std::string exifGPSStatus;
//  if (mImageHandler) {
//    exifGPSStatus = mImageHandler->metadata("EXIF_GPSStatus");
//  }
//  return exifGPSStatus;
//}
//
//std::string EXIFMetadata::exifGPSMeasureMode() const
//{
//  std::string exifGPSMeasureMode;
//  if (mImageHandler) {
//    exifGPSMeasureMode = mImageHandler->metadata("EXIF_GPSMeasureMode");
//  }
//  return exifGPSMeasureMode;
//}
//
//double EXIFMetadata::exifGPSDOP() const
//{
//  double exifGPSDOP;
//  if (mImageHandler) {
//    std::string sGPSDOP = mImageHandler->metadata("EXIF_GPSDOP");
//    exifGPSDOP = std::stod(sGPSDOP);
//  }
//  return exifGPSDOP;
//}
//
//std::string EXIFMetadata::exifGPSSpeedRef() const
//{
//  std::string exifGPSSpeedRef;
//  if (mImageHandler) {
//    exifGPSSpeedRef = mImageHandler->metadata("EXIF_GPSSpeedRef");
//  }
//  return exifGPSSpeedRef;
//}
//
//double EXIFMetadata::exifGPSSpeed() const
//{
//  double exifGPSSpeed;
//  if (mImageHandler) {
//    std::string sGPSSpeed = mImageHandler->metadata("EXIF_GPSSpeed");
//    exifGPSSpeed = std::stod(sGPSSpeed);
//  }
//  return exifGPSSpeed;
//}
//
//std::string EXIFMetadata::exifGPSTrackRef() const
//{
//  std::string exifGPSTrackRef;
//  if (mImageHandler) {
//    exifGPSTrackRef = mImageHandler->metadata("EXIF_GPSTrackRef");
//  }
//  return exifGPSTrackRef;
//}
//
//double EXIFMetadata::exifGPSTrack() const
//{
//  double exifGPSTrack;
//  if (mImageHandler) {
//    std::string sGPSTrack = mImageHandler->metadata("EXIF_GPSTrack");
//    exifGPSTrack = std::stod(sGPSTrack);
//  }
//  return exifGPSTrack;
//}
//
//std::string EXIFMetadata::exifGPSImgDirectionRef() const
//{
//  std::string exifGPSImgDirectionRef;
//  if (mImageHandler) {
//    exifGPSImgDirectionRef = mImageHandler->metadata("EXIF_GPSImgDirectionRef");
//  }
//  return exifGPSImgDirectionRef;
//}
//
//double EXIFMetadata::exifGPSImgDirection() const
//{
//  double exifGPSImgDirection;
//  if (mImageHandler) {
//    std::string sGPSImgDirection = mImageHandler->metadata("EXIF_GPSImgDirection");
//    exifGPSImgDirection = std::stod(sGPSImgDirection);
//  }
//  return exifGPSImgDirection;
//}
//
//std::string EXIFMetadata::exifGPSMapDatum() const
//{
//  std::string exifGPSMapDatum;
//  if (mImageHandler) {
//    exifGPSMapDatum = mImageHandler->metadata("EXIF_GPSMapDatum");
//  }
//  return exifGPSMapDatum;
//}
//
//std::string EXIFMetadata::exifGPSDestLatitudeRef() const
//{
//  std::string exifGPSDestLatitudeRef;
//  if (mImageHandler) {
//    exifGPSDestLatitudeRef = mImageHandler->metadata("EXIF_GPSDestLatitudeRef");
//  }
//  return exifGPSDestLatitudeRef;
//}
//
//double EXIFMetadata::exifGPSDestLatitude() const
//{
//  double exifGPSDestLatitude;
//  if (mImageHandler) {
//    std::string sGPSDestLatitude = mImageHandler->metadata("EXIF_GPSDestLatitude");
//    exifGPSDestLatitude = std::stod(sGPSDestLatitude);
//  }
//  return exifGPSDestLatitude;
//}
//
//std::string EXIFMetadata::exifGPSDestLongitudeRef() const
//{
//  std::string exifGPSDestLongitudeRef;
//  if (mImageHandler) {
//    exifGPSDestLongitudeRef = mImageHandler->metadata("EXIF_GPSDestLongitudeRef");
//  }
//  return exifGPSDestLongitudeRef;
//}
//
//double EXIFMetadata::exifGPSDestLongitude() const
//{
//  double exifGPSDestLongitude;
//  if (mImageHandler) {
//    std::string sGPSDestLongitude = mImageHandler->metadata("EXIF_GPSDestLongitude");
//    exifGPSDestLongitude = std::stod(sGPSDestLongitude);
//  }
//  return exifGPSDestLongitude;
//}
//
//std::string EXIFMetadata::exifGPSDestBearingRef() const
//{
//  std::string exifGPSDestBearingRef;
//  if (mImageHandler) {
//    exifGPSDestBearingRef = mImageHandler->metadata("EXIF_GPSDestBearingRef");
//  }
//  return exifGPSDestBearingRef;
//}
//
//double EXIFMetadata::exifGPSDestBearing() const
//{
//  double exifGPSDestBearing;
//  if (mImageHandler) {
//    std::string sGPSDestBearing = mImageHandler->metadata("EXIF_GPSDestBearing");
//    exifGPSDestBearing = std::stod(sGPSDestBearing);
//  }
//  return exifGPSDestBearing;
//}
//
//std::string EXIFMetadata::exifGPSDestDistanceRef() const
//{
//  std::string exifGPSDestDistanceRef;
//  if (mImageHandler) {
//    exifGPSDestDistanceRef = mImageHandler->metadata("EXIF_GPSDestDistanceRef");
//  }
//  return exifGPSDestDistanceRef;
//}
//
//double EXIFMetadata::exifGPSDestDistance() const
//{
//  double exifGPSDestDistance;
//  if (mImageHandler) {
//    std::string sGPSDestDistance = mImageHandler->metadata("EXIF_GPSDestDistance");
//    exifGPSDestDistance = std::stod(sGPSDestDistance);
//  }
//  return exifGPSDestDistance;
//}
//
//std::string EXIFMetadata::exifGPSDateStamp() const
//{
//  std::string exifGPSDateStamp;
//  if (mImageHandler) {
//    exifGPSDateStamp = mImageHandler->metadata("EXIF_GPSDateStamp");
//  }
//  return exifGPSDateStamp;
//}
//
//uint16_t EXIFMetadata::exifGPSDifferential() const
//{
//  uint16_t exifGPSDifferential;
//  if (mImageHandler) {
//    std::string s_exifGPSDifferential = mImageHandler->metadata("EXIF_GPSDifferential");
//    exifGPSDifferential = static_cast<uint16_t>(std::stoi(s_exifGPSDifferential));
//  }
//  return exifGPSDifferential;
//}
//
//double EXIFMetadata::exifGPSHPositioningError() const
//{
//  double exifGPSHPositioningError;
//  if (mImageHandler) {
//    std::string sGPSHPositioningError = mImageHandler->metadata("EXIF_GPSHPositioningError");
//    exifGPSHPositioningError = std::stod(sGPSHPositioningError);
//  }
//  return exifGPSHPositioningError;
//}
//
///*--------------------------------------------------------------------*/
//
//
//JpegMetadata::JpegMetadata()
//  : ImgMetadata(Format::jpeg),
//    EXIFMetadata(nullptr),
//    mImageHandler(nullptr)
//{}
//
//JpegMetadata::JpegMetadata(ImageHandler *imageHandler)
//  : ImgMetadata(Format::jpeg),
//    EXIFMetadata(imageHandler),
//    mImageHandler(imageHandler)
//{
//}
//
//JpegMetadata::~JpegMetadata()
//{}
//
//void JpegMetadata::setImageHandler(ImageHandler *imageHandler)
//{
//  EXIFMetadata::setImageHandler(imageHandler);
//  mImageHandler = imageHandler;
//}
//
////std::string JpegMetadata::exifDocumentName() const
////{
////  std::string exifDocumentName;
////  if (mImageHandler) {
////    exifDocumentName = mImageHandler->metadata("EXIF_Document_Name");
////  }
////  return exifDocumentName;
////}
////
////std::string JpegMetadata::exifImageDescription() const
////{
////  std::string exifImageDescription;
////  if (mImageHandler) {
////    exifImageDescription = mImageHandler->metadata("EXIF_ImageDescription");
////  }
////  return exifImageDescription;
////}
////
////std::string JpegMetadata::exifMake() const
////{
////  std::string exifMake;
////  if (mImageHandler) {
////    exifMake = mImageHandler->metadata("EXIF_Make");
////  }
////  return exifMake;
////}
////
////std::string JpegMetadata::exifModel() const
////{
////  std::string exifModel;
////  if (mImageHandler) {
////    exifModel = mImageHandler->metadata("EXIF_Model");
////  }
////  return exifModel;
////}
////
////uint16_t JpegMetadata::exifOrientation() const
////{
////  uint16_t exifOrientation;
////  if (mImageHandler) {
////    std::string s_orientation = mImageHandler->metadata("EXIF_Orientation");
////    exifOrientation = static_cast<uint16_t>(std::stoi(s_orientation));
////  }
////  return exifOrientation;
////}
////
////double JpegMetadata::exifXResolution() const
////{
////  double exifXResolution;
////  if (mImageHandler) {
////    std::string sXResolution = mImageHandler->metadata("EXIF_XResolution");
////    exifXResolution = std::stod(sXResolution);
////  }
////  return exifXResolution;
////}
////
////double JpegMetadata::exifYResolution() const
////{
////  double exifYResolution;
////  if (mImageHandler) {
////    std::string sYResolution = mImageHandler->metadata("EXIF_YResolution");
////    exifYResolution = std::stod(sYResolution);
////  }
////  return exifYResolution;
////}
////
////uint16_t JpegMetadata::exifResolutionUnit() const
////{
////  uint16_t exifResolutionUnit;
////  if (mImageHandler) {
////    std::string sResolutionUnit = mImageHandler->metadata("EXIF_ResolutionUnit");
////    exifResolutionUnit = static_cast<uint16_t>(std::stoi(sResolutionUnit));
////  }
////  return exifResolutionUnit;
////}
////
////uint16_t JpegMetadata::exifTransferFunction() const
////{
////  uint16_t exifTransferFunction;
////  if (mImageHandler) {
////    std::string sTransferFunction = mImageHandler->metadata("EXIF_TransferFunction");
////    exifTransferFunction = static_cast<uint16_t>(std::stoi(sTransferFunction));
////  }
////  return exifTransferFunction;
////}
////
////std::string JpegMetadata::exifSoftware() const
////{
////  std::string exifSoftware;
////  if (mImageHandler) {
////    exifSoftware = mImageHandler->metadata("EXIF_Software");
////  }
////  return exifSoftware;
////}
////
////std::string JpegMetadata::exifDateTime() const
////{
////  std::string exifDateTime;
////  if (mImageHandler) {
////    exifDateTime = mImageHandler->metadata("EXIF_DateTime");
////  }
////  return exifDateTime;
////}
////
////std::string JpegMetadata::exifArtist() const
////{
////  std::string exifArtist;
////  if (mImageHandler) {
////    exifArtist = mImageHandler->metadata("EXIF_Artist");
////  }
////  return exifArtist;
////}
////
////uint16_t JpegMetadata::exifYCbCrPositioning() const
////{
////  uint16_t exifYCbCrPositioning;
////  if (mImageHandler) {
////    std::string sYCbCrPositioning = mImageHandler->metadata("EXIF_YCbCrPositioning");
////    exifYCbCrPositioning = static_cast<uint16_t>(std::stoi(sYCbCrPositioning));
////  }
////  return exifYCbCrPositioning;
////}
////
////std::string JpegMetadata::exifCopyright() const
////{
////  std::string exifCopyright;
////  if (mImageHandler) {
////    exifCopyright = mImageHandler->metadata("EXIF_Copyright");
////  }
////  return exifCopyright;
////}
////
////double JpegMetadata::exifExposureTime() const
////{
////  double exifExposureTime;
////  if (mImageHandler) {
////    std::string sExposureTime = mImageHandler->metadata("EXIF_ExposureTime");
////    exifExposureTime = std::stod(sExposureTime);
////  }
////  return exifExposureTime;
////}
////
////double JpegMetadata::exifFNumber() const
////{
////  double exifFNumber;
////  if (mImageHandler) {
////    std::string sFNumber = mImageHandler->metadata("EXIF_FNumber");
////    exifFNumber = std::stod(sFNumber);
////  }
////  return exifFNumber;
////}
////
////uint16_t JpegMetadata::exifExposureProgram() const
////{
////  uint16_t exifExposureProgram;
////  if (mImageHandler) {
////    std::string sExposureProgram = mImageHandler->metadata("EXIF_ExposureProgram");
////    exifExposureProgram = static_cast<uint16_t>(std::stoi(sExposureProgram));
////  }
////  return exifExposureProgram;
////}
////
////std::string JpegMetadata::exifSpectralSensitivity() const
////{
////  std::string exifSpectralSensitivity;
////  if (mImageHandler) {
////    exifSpectralSensitivity = mImageHandler->metadata("EXIF_SpectralSensitivity");
////  }
////  return exifSpectralSensitivity;
////}
////
////uint16_t JpegMetadata::exifISOSpeedRatings() const
////{
////  uint16_t exifISOSpeedRatings;
////  if (mImageHandler) {
////    std::string sISOSpeedRatings = mImageHandler->metadata("EXIF_ISOSpeedRatings");
////    exifISOSpeedRatings = static_cast<uint16_t>(std::stoi(sISOSpeedRatings));
////  }
////  return exifISOSpeedRatings;
////}
////
////uint16_t JpegMetadata::exifSensitivityType() const
////{
////  uint16_t exifSensitivityType;
////  if (mImageHandler) {
////    std::string sSensitivityType = mImageHandler->metadata("EXIF_SensitivityType");
////    exifSensitivityType = static_cast<uint16_t>(std::stoi(sSensitivityType));
////  }
////  return exifSensitivityType;
////}
////
////long JpegMetadata::exifStandardOutputSensitivity() const
////{
////  long exifStandardOutputSensitivity;
////  if (mImageHandler) {
////    std::string sStandardOutputSensitivity = mImageHandler->metadata("EXIF_StandardOutputSensitivity");
////    exifStandardOutputSensitivity = std::stol(sStandardOutputSensitivity);
////  }
////  return exifStandardOutputSensitivity;
////}
////
////long JpegMetadata::exifRecommendedExposureIndex() const
////{
////  long exifRecommendedExposureIndex;
////  if (mImageHandler) {
////    std::string sRecommendedExposureIndex = mImageHandler->metadata("EXIF_RecommendedExposureIndex");
////    exifRecommendedExposureIndex = std::stol(sRecommendedExposureIndex);
////  }
////  return exifRecommendedExposureIndex;
////}
////
////long JpegMetadata::exifISOSpeed() const
////{
////  long exifISOSpeed;
////  if (mImageHandler) {
////    std::string sISOSpeed = mImageHandler->metadata("EXIF_ISOSpeed");
////    exifISOSpeed = std::stol(sISOSpeed);
////  }
////  return exifISOSpeed;
////}
////
////long JpegMetadata::exifISOSpeedLatitudeyyy() const
////{
////  long exifISOSpeedLatitudeyyy;
////  if (mImageHandler) {
////    std::string sISOSpeedLatitudeyyy = mImageHandler->metadata("EXIF_ISOSpeedLatitudeyyy");
////    exifISOSpeedLatitudeyyy = std::stol(sISOSpeedLatitudeyyy);
////  }
////  return exifISOSpeedLatitudeyyy;
////}
////
////long JpegMetadata::exifISOSpeedLatitudezzz() const
////{
////  long exifISOSpeedLatitudezzz;
////  if (mImageHandler) {
////    std::string sISOSpeedLatitudezzz = mImageHandler->metadata("EXIF_ISOSpeedLatitudezzz");
////    exifISOSpeedLatitudezzz = std::stol(sISOSpeedLatitudezzz);
////  }
////  return exifISOSpeedLatitudezzz;
////}
////
////std::string JpegMetadata::exifDateTimeOriginal() const
////{
////  std::string exifDateTimeOriginal;
////  if (mImageHandler) {
////    exifDateTimeOriginal = mImageHandler->metadata("EXIF_DateTimeOriginal");
////  }
////  return exifDateTimeOriginal;
////}
////
////std::string JpegMetadata::exifDateTimeDigitized() const
////{
////  std::string exifDateTimeDigitized;
////  if (mImageHandler) {
////    exifDateTimeDigitized = mImageHandler->metadata("EXIF_DateTimeDigitized");
////  }
////  return exifDateTimeDigitized;
////}
////
////std::string JpegMetadata::exifOffsetTime() const
////{
////  std::string exifOffsetTime;
////  if (mImageHandler) {
////    exifOffsetTime = mImageHandler->metadata("EXIF_OffsetTime");
////  }
////  return exifOffsetTime;
////}
////
////std::string JpegMetadata::exifOffsetTimeOriginal() const
////{
////  std::string exifOffsetTimeOriginal;
////  if (mImageHandler) {
////    exifOffsetTimeOriginal = mImageHandler->metadata("EXIF_OffsetTimeOriginal");
////  }
////  return exifOffsetTimeOriginal;
////}
////
////std::string JpegMetadata::exifOffsetTimeDigitized() const
////{
////  std::string exifOffsetTimeDigitized;
////  if (mImageHandler) {
////    exifOffsetTimeDigitized = mImageHandler->metadata("EXIF_OffsetTimeDigitized");
////  }
////  return exifOffsetTimeDigitized;
////}
////
////double JpegMetadata::exifCompressedBitsPerPixel() const
////{
////  double exifCompressedBitsPerPixel;
////  if (mImageHandler) {
////    std::string sCompressedBitsPerPixel = mImageHandler->metadata("EXIF_CompressedBitsPerPixel");
////    exifCompressedBitsPerPixel = std::stod(sCompressedBitsPerPixel);
////  }
////  return exifCompressedBitsPerPixel;
////}
////
////double JpegMetadata::exifShutterSpeedValue() const
////{
////  double exifShutterSpeedValue;
////  if (mImageHandler) {
////    std::string sShutterSpeedValue = mImageHandler->metadata("EXIF_ShutterSpeedValue");
////    exifShutterSpeedValue = std::stod(sShutterSpeedValue);
////  }
////  return exifShutterSpeedValue;
////}
////
////double JpegMetadata::exifApertureValue() const
////{
////  double exifApertureValue;
////  if (mImageHandler) {
////    std::string sApertureValue = mImageHandler->metadata("EXIF_ApertureValue");
////    exifApertureValue = std::stod(sApertureValue);
////  }
////  return exifApertureValue;
////}
////
////double JpegMetadata::exifExposureBiasValue() const
////{
////  double exifExposureBiasValue;
////  if (mImageHandler) {
////    std::string sExposureBiasValue = mImageHandler->metadata("EXIF_ExposureBiasValue");
////    exifExposureBiasValue = std::stod(sExposureBiasValue);
////  }
////  return exifExposureBiasValue;
////}
////
////double JpegMetadata::exifMaxApertureValue() const
////{
////  double exifMaxApertureValue;
////  if (mImageHandler) {
////    std::string sMaxApertureValue = mImageHandler->metadata("EXIF_MaxApertureValue");
////    exifMaxApertureValue = std::stod(sMaxApertureValue);
////  }
////  return exifMaxApertureValue;
////}
////
////double JpegMetadata::exifFocalLength() const
////{
////  double exifFocalLength;
////  if (mImageHandler) {
////    std::string sFocalLength = mImageHandler->metadata("EXIF_FocalLength");
////    exifFocalLength = std::stod(sFocalLength);
////  }
////  return exifFocalLength;
////}
////
////double JpegMetadata::exifFocalLengthIn35mmFilm() const
////{
////  double exifFocalLengthIn35mmFilm;
////  if (mImageHandler) {
////    std::string sFocalLengthIn35mmFilm = mImageHandler->metadata("EXIF_FocalLengthIn35mmFilm");
////    exifFocalLengthIn35mmFilm = std::stod(sFocalLengthIn35mmFilm);
////  }
////  return exifFocalLengthIn35mmFilm;
////}
////
////double JpegMetadata::exifSubjectDistanceRange() const
////{
////  double exifSubjectDistanceRange;
////  if (mImageHandler) {
////    std::string sSubjectDistanceRange = mImageHandler->metadata("EXIF_SubjectDistanceRange");
////    exifSubjectDistanceRange = std::stod(sSubjectDistanceRange);
////  }
////  return exifSubjectDistanceRange;
////}
////
////std::string JpegMetadata::exifGPSLatitudeRef() const
////{
////  std::string exifGPSLatitudeRef;
////  if (mImageHandler) {
////    exifGPSLatitudeRef = mImageHandler->metadata("EXIF_GPSLatitudeRef");
////  }
////  return exifGPSLatitudeRef;
////}
////
////double JpegMetadata::exifGPSLatitude() const
////{
////  double exifGPSLatitude;
////  if (mImageHandler) {
////    std::string sGPSLatitude = mImageHandler->metadata("EXIF_GPSLatitude");
////    exifGPSLatitude = std::stod(sGPSLatitude);
////  }
////  return exifGPSLatitude;
////}
////
////std::string JpegMetadata::exifGPSLongitudeRef() const
////{
////  std::string exifGPSLongitudeRef;
////  if (mImageHandler) {
////    exifGPSLongitudeRef = mImageHandler->metadata("EXIF_GPSLongitudeRef");
////  }
////  return exifGPSLongitudeRef;
////}
////
////double JpegMetadata::exifGPSLongitude() const
////{
////  double exifGPSLongitude;
////  if (mImageHandler) {
////    std::string sGPSLongitude = mImageHandler->metadata("EXIF_GPSLongitude");
////    exifGPSLongitude = std::stod(sGPSLongitude);
////  }
////  return exifGPSLongitude;
////}
////
////double JpegMetadata::exifGPSAltitude() const
////{
////  double exifGPSAltitude;
////  if (mImageHandler) {
////    std::string sGPSAltitude = mImageHandler->metadata("EXIF_GPSAltitude");
////    exifGPSAltitude = std::stod(sGPSAltitude);
////  }
////  return exifGPSAltitude;
////}
////
////double JpegMetadata::exifGPSTimeStamp() const
////{
////  double exifGPSLatitude;
////  if (mImageHandler) {
////    std::string sGPSLatitude = mImageHandler->metadata("EXIF_GPSLatitude");
////    exifGPSLatitude = std::stod(sGPSLatitude);
////  }
////  return exifGPSLatitude;
////}
////
////std::string JpegMetadata::exifGPSSatellites() const
////{
////  std::string exifGPSSatellites;
////  if (mImageHandler) {
////    exifGPSSatellites = mImageHandler->metadata("EXIF_GPSSatellites");
////  }
////  return exifGPSSatellites;
////}
////
////std::string JpegMetadata::exifGPSStatus() const
////{
////  std::string exifGPSStatus;
////  if (mImageHandler) {
////    exifGPSStatus = mImageHandler->metadata("EXIF_GPSStatus");
////  }
////  return exifGPSStatus;
////}
////
////std::string JpegMetadata::exifGPSMeasureMode() const
////{
////  std::string exifGPSMeasureMode;
////  if (mImageHandler) {
////    exifGPSMeasureMode = mImageHandler->metadata("EXIF_GPSMeasureMode");
////  }
////  return exifGPSMeasureMode;
////}
////
////double JpegMetadata::exifGPSDOP() const
////{
////  double exifGPSDOP;
////  if (mImageHandler) {
////    std::string sGPSDOP = mImageHandler->metadata("EXIF_GPSDOP");
////    exifGPSDOP = std::stod(sGPSDOP);
////  }
////  return exifGPSDOP;
////}
////
////std::string JpegMetadata::exifGPSSpeedRef() const
////{
////  std::string exifGPSSpeedRef;
////  if (mImageHandler) {
////    exifGPSSpeedRef = mImageHandler->metadata("EXIF_GPSSpeedRef");
////  }
////  return exifGPSSpeedRef;
////}
////
////double JpegMetadata::exifGPSSpeed() const
////{
////  double exifGPSSpeed;
////  if (mImageHandler) {
////    std::string sGPSSpeed = mImageHandler->metadata("EXIF_GPSSpeed");
////    exifGPSSpeed = std::stod(sGPSSpeed);
////  }
////  return exifGPSSpeed;
////}
////
////std::string JpegMetadata::exifGPSTrackRef() const
////{
////  std::string exifGPSTrackRef;
////  if (mImageHandler) {
////    exifGPSTrackRef = mImageHandler->metadata("EXIF_GPSTrackRef");
////  }
////  return exifGPSTrackRef;
////}
////
////double JpegMetadata::exifGPSTrack() const
////{
////  double exifGPSTrack;
////  if (mImageHandler) {
////    std::string sGPSTrack = mImageHandler->metadata("EXIF_GPSTrack");
////    exifGPSTrack = std::stod(sGPSTrack);
////  }
////  return exifGPSTrack;
////}
////
////std::string JpegMetadata::exifGPSImgDirectionRef() const
////{
////  std::string exifGPSImgDirectionRef;
////  if (mImageHandler) {
////    exifGPSImgDirectionRef = mImageHandler->metadata("EXIF_GPSImgDirectionRef");
////  }
////  return exifGPSImgDirectionRef;
////}
////
////double JpegMetadata::exifGPSImgDirection() const
////{
////  double exifGPSImgDirection;
////  if (mImageHandler) {
////    std::string sGPSImgDirection = mImageHandler->metadata("EXIF_GPSImgDirection");
////    exifGPSImgDirection = std::stod(sGPSImgDirection);
////  }
////  return exifGPSImgDirection;
////}
////
////std::string JpegMetadata::exifGPSMapDatum() const
////{
////  std::string exifGPSMapDatum;
////  if (mImageHandler) {
////    exifGPSMapDatum = mImageHandler->metadata("EXIF_GPSMapDatum");
////  }
////  return exifGPSMapDatum;
////}
////
////std::string JpegMetadata::exifGPSDestLatitudeRef() const
////{
////  std::string exifGPSDestLatitudeRef;
////  if (mImageHandler) {
////    exifGPSDestLatitudeRef = mImageHandler->metadata("EXIF_GPSDestLatitudeRef");
////  }
////  return exifGPSDestLatitudeRef;
////}
////
////double JpegMetadata::exifGPSDestLatitude() const
////{
////  double exifGPSDestLatitude;
////  if (mImageHandler) {
////    std::string sGPSDestLatitude = mImageHandler->metadata("EXIF_GPSDestLatitude");
////    exifGPSDestLatitude = std::stod(sGPSDestLatitude);
////  }
////  return exifGPSDestLatitude;
////}
////
////std::string JpegMetadata::exifGPSDestLongitudeRef() const
////{
////  std::string exifGPSDestLongitudeRef;
////  if (mImageHandler) {
////    exifGPSDestLongitudeRef = mImageHandler->metadata("EXIF_GPSDestLongitudeRef");
////  }
////  return exifGPSDestLongitudeRef;
////}
////
////double JpegMetadata::exifGPSDestLongitude() const
////{
////  double exifGPSDestLongitude;
////  if (mImageHandler) {
////    std::string sGPSDestLongitude = mImageHandler->metadata("EXIF_GPSDestLongitude");
////    exifGPSDestLongitude = std::stod(sGPSDestLongitude);
////  }
////  return exifGPSDestLongitude;
////}
////
////std::string JpegMetadata::exifGPSDestBearingRef() const
////{
////  std::string exifGPSDestBearingRef;
////  if (mImageHandler) {
////    exifGPSDestBearingRef = mImageHandler->metadata("EXIF_GPSDestBearingRef");
////  }
////  return exifGPSDestBearingRef;
////}
////
////double JpegMetadata::exifGPSDestBearing() const
////{
////  double exifGPSDestBearing;
////  if (mImageHandler) {
////    std::string sGPSDestBearing = mImageHandler->metadata("EXIF_GPSDestBearing");
////    exifGPSDestBearing = std::stod(sGPSDestBearing);
////  }
////  return exifGPSDestBearing;
////}
////
////std::string JpegMetadata::exifGPSDestDistanceRef() const
////{
////  std::string exifGPSDestDistanceRef;
////  if (mImageHandler) {
////    exifGPSDestDistanceRef = mImageHandler->metadata("EXIF_GPSDestDistanceRef");
////  }
////  return exifGPSDestDistanceRef;
////}
////
////double JpegMetadata::exifGPSDestDistance() const
////{
////  double exifGPSDestDistance;
////  if (mImageHandler) {
////    std::string sGPSDestDistance = mImageHandler->metadata("EXIF_GPSDestDistance");
////    exifGPSDestDistance = std::stod(sGPSDestDistance);
////  }
////  return exifGPSDestDistance;
////}
////
////std::string JpegMetadata::exifGPSDateStamp() const
////{
////  std::string exifGPSDateStamp;
////  if (mImageHandler) {
////    exifGPSDateStamp = mImageHandler->metadata("EXIF_GPSDateStamp");
////  }
////  return exifGPSDateStamp;
////}
////
////uint16_t JpegMetadata::exifGPSDifferential() const
////{
////  uint16_t exifGPSDifferential;
////  if (mImageHandler) {
////    std::string s_exifGPSDifferential = mImageHandler->metadata("EXIF_GPSDifferential");
////    exifGPSDifferential = static_cast<uint16_t>(std::stoi(s_exifGPSDifferential));
////  }
////  return exifGPSDifferential;
////}
////
////double JpegMetadata::exifGPSHPositioningError() const
////{
////  double exifGPSHPositioningError;
////  if (mImageHandler) {
////    std::string sGPSHPositioningError = mImageHandler->metadata("EXIF_GPSHPositioningError");
////    exifGPSHPositioningError = std::stod(sGPSHPositioningError);
////  }
////  return exifGPSHPositioningError;
////}
//
//
///*--------------------------------------------------------------------*/
//
//
//TiffMetadata::TiffMetadata()
//  : ImgMetadata(Format::tiff),
//    EXIFMetadata(nullptr),
//    mImageHandler(nullptr)
//{
//}
//
//TiffMetadata::TiffMetadata(ImageHandler *imageHandler)
//  : ImgMetadata(Format::tiff),
//    EXIFMetadata(imageHandler),
//    mImageHandler(imageHandler)
//{
//}
//
//TiffMetadata::~TiffMetadata()
//{
//}
//
//void TiffMetadata::setImageHandler(ImageHandler *imageHandler)
//{
//  EXIFMetadata::setImageHandler(imageHandler);
//  mImageHandler = imageHandler;
//}
//
//std::string TiffMetadata::documentName() const
//{
//  std::string document_name;
//  if (mImageHandler) {
//    document_name = mImageHandler->metadata("TIFFTAG_DOCUMENTNAME");
//  }
//  return document_name;
//}
//
//void TiffMetadata::setDocumentName(const std::string &documentName)
//{
//  if (mImageHandler) {
//     mImageHandler->setMetadata("TIFFTAG_DOCUMENTNAME", documentName);
//  }
//}
//
//std::string TiffMetadata::imageDescription() const
//{
//  std::string image_description;
//  if (mImageHandler) {
//    image_description = mImageHandler->metadata("TIFFTAG_IMAGEDESCRIPTION");
//  }
//  return image_description;
//}
//
//void TiffMetadata::setImageDescription(const std::string &imageDescription)
//{
//  if (mImageHandler) {
//    mImageHandler->setMetadata("TIFFTAG_IMAGEDESCRIPTION", imageDescription);
//  }
//}
//
//std::string TiffMetadata::software() const
//{
//  std::string software;
//  if (mImageHandler) {
//    software = mImageHandler->metadata("TIFFTAG_SOFTWARE");
//  }
//  return software;
//}
//
//void TiffMetadata::setSoftware(const std::string &software)
//{
//  if (mImageHandler) {
//    mImageHandler->setMetadata("TIFFTAG_SOFTWARE", software);
//  }
//}
//
//std::string TiffMetadata::datetime() const
//{
//  std::string datetime;
//  if (mImageHandler) {
//    datetime = mImageHandler->metadata("TIFFTAG_DATETIME");
//  }
//  return datetime;
//}
//
//void TiffMetadata::setDatetime(const std::string &datetime)
//{
//  if (mImageHandler) {
//    mImageHandler->setMetadata("TIFFTAG_DATETIME", datetime);
//  }
//}
//
//std::string TiffMetadata::artist() const
//{
//  std::string artist;
//  if (mImageHandler) {
//    artist = mImageHandler->metadata("TIFFTAG_ARTIST");
//  }
//  return artist;
//}
//
//void TiffMetadata::setArtist(const std::string &artist)
//{
//  if (mImageHandler) {
//    mImageHandler->setMetadata("TIFFTAG_ARTIST", artist);
//  }
//}
//
//std::string TiffMetadata::hostComputer() const
//{
//  std::string hostComputer;
//  if (mImageHandler) {
//    hostComputer = mImageHandler->metadata("TIFFTAG_HOSTCOMPUTER");
//  }
//  return hostComputer;
//}
//
//void TiffMetadata::setHostComputer(const std::string &hostComputer)
//{
//  if (mImageHandler) {
//    mImageHandler->setMetadata("TIFFTAG_HOSTCOMPUTER", hostComputer);
//  }
//}
//
//std::string TiffMetadata::copyright() const
//{
//  std::string copyright;
//  if (mImageHandler) {
//    copyright = mImageHandler->metadata("TIFFTAG_COPYRIGHT");
//  }
//  return copyright;
//}
//
//void TiffMetadata::setCopyright(const std::string &copyright)
//{
//  if (mImageHandler) {
//    mImageHandler->setMetadata("TIFFTAG_COPYRIGHT", copyright);
//  }
//}
//
//std::string TiffMetadata::xResolution() const
//{
//  std::string xResolution;
//  if (mImageHandler) {
//    xResolution = mImageHandler->metadata("TIFFTAG_XRESOLUTION");
//  }
//  return xResolution;
//}
//
//void TiffMetadata::setXResolution(const std::string &xResolution)
//{
//  if (mImageHandler) {
//    mImageHandler->setMetadata("TIFFTAG_XRESOLUTION", xResolution);
//  }
//}
//
//std::string TiffMetadata::yResolution() const
//{
//  std::string yResolution;
//  if (mImageHandler) {
//    yResolution = mImageHandler->metadata("TIFFTAG_YRESOLUTION");
//  }
//  return yResolution;
//}
//
//void TiffMetadata::setYResolution(const std::string &yResolution)
//{
//  if (mImageHandler) {
//    mImageHandler->setMetadata("TIFFTAG_YRESOLUTION", yResolution);
//  }
//}
//
//std::string TiffMetadata::resolutionUnit() const
//{
//  std::string resolutionUnit;
//  if (mImageHandler) {
//    resolutionUnit = mImageHandler->metadata("TIFFTAG_RESOLUTIONUNIT");
//  }
//  return resolutionUnit;
//}
//
//void TiffMetadata::setResolutionUnit(const std::string &resolutionUnit)
//{
//  if (mImageHandler) {
//    mImageHandler->setMetadata("TIFFTAG_RESOLUTIONUNIT", resolutionUnit);
//  }
//}
//
//std::string TiffMetadata::minSampleValue() const
//{
//  std::string minSampleValue;
//  if (mImageHandler) {
//    minSampleValue = mImageHandler->metadata("TIFFTAG_MINSAMPLEVALUE");
//  }
//  return minSampleValue;
//}
//
//std::string TiffMetadata::maxSampleValue() const
//{
//  std::string maxSampleValue;
//  if (mImageHandler) {
//    maxSampleValue = mImageHandler->metadata("TIFFTAG_MAXSAMPLEVALUE");
//  }
//  return maxSampleValue;
//}
//
//
///*--------------------------------------------------------------------*/
//
//
//JP2000Metadata::JP2000Metadata()
//  : ImgMetadata(Format::jp2000),
//    mImageHandler(nullptr)
//{
//}
//
//JP2000Metadata::JP2000Metadata(ImageHandler *imageHandler)
//  : ImgMetadata(Format::tiff),
//    mImageHandler(imageHandler)
//{
//}
//
//JP2000Metadata::~JP2000Metadata()
//{
//}
//
//void JP2000Metadata::setImageHandler(ImageHandler *imageHandler)
//{
//  mImageHandler = imageHandler;
//}



} // End namespace tl
