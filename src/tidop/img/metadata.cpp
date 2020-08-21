#include "metadata.h"

#include "tidop/core/messages.h"

namespace tl
{
ImageMetadata::ImageMetadata(Format format)
  : mFormat(format)
{
}

ImageMetadata::~ImageMetadata()
{
}

ImageMetadata::Format ImageMetadata::format()
{
  return mFormat;
}






class ImageMetadataBase
  : public ImageMetadata
{

public:

  ImageMetadataBase(Format format)
    : ImageMetadata(format)
  {
  }
  
  ~ImageMetadataBase() override
  {

  }
    
  std::string metadata(const std::string &name, bool &active) const override;
  void setMetadata(const std::string &name, const std::string &value) override;

  std::map<std::string, std::string> metadata() const override
  {
    return this->metadata(true);
  }

  std::map<std::string, std::string> activeMetadata() const override
  {
    return this->metadata(false);
  }

  virtual void reset() override
  {
    this->init();
  }

private:

  virtual void init()
  { }
  
  std::map<std::string, std::string> metadata(bool all) const;

protected:

  std::map<std::string, std::pair<std::string, bool>> mMetadata;

};


std::string ImageMetadataBase::metadata(const std::string &name, bool &active) const
{
  std::string value;
  active = false;

  auto metadata = mMetadata.find(name);
  if (metadata != mMetadata.end()) {
    value = metadata->second.first;
    active = metadata->second.second;
  } else {
    msgWarning("Metadata '%s' not supported", name.c_str());
  }

  return value;
}

void ImageMetadataBase::setMetadata(const std::string &name, const std::string &value)
{
  auto metadata = mMetadata.find(name);
  if (metadata != mMetadata.end()) {
    metadata->second.first = value;
    metadata->second.second = true;
  } else {
    msgWarning("Metadata '%s' not supported", name.c_str());
  }
}

std::map<std::string, std::string> ImageMetadataBase::ImageMetadataBase::metadata(bool all) const
{
  std::map<std::string, std::string> metadata;

  for (auto it = mMetadata.begin(); it != mMetadata.end(); it++) {
    if (all || it->second.second == true)
      metadata[it->first] = it->second.first;
  }

  return metadata;
}





// ExposureProgram:
// 0 = Not defined (default)
// 1 = Manual
// 2 = Normal program
// 3 = Aperture priority
// 4 = Shutter priority
// 5 = Creative program (biased toward depth of field)
// 6 = Action program (biased toward fast shutter speed)
// 7 = Portrait mode (for closeup photos with the background out of focus)
// 8 = Landscape mode (for landscape photos with the background in focus)
//
// ComponentsConfiguration:
// 
// Default		4,5,6,0 if RGB uncompressed; 1,2,3,0 otherwise
//
// 0 = does not exist
// 1 = Y
// 2 = Cb
// 3 = Cr
// 4 = R
// 5 = G
// 6 = B
// Other = reserved
//
// MeteringMode
//
// 0 = Unknown (default)
// 1 = Average
// 2 = CenterWeightedAverage
// 3 = Spot
// 4 = MultiSpot
// 5 = Pattern
// 6 = Partial
// 255 = other
//
// LightSource
//
// 0 = Unknown (default)
// 1 = Daylight
// 2 = Fluorescent
// 3 = Tungsten (incandescent light)
// 4 = Flash
// 9 = Fine weather
// 10 = Cloudy weather
// 11 = Shade
// 12 = Daylight fluorescent (D 5700 - 7100K)
// 13 = Day white fluorescent (N 4600 - 5400K)
// 14 = Cool white fluorescent (W 3900 - 4500K)
// 15 = White fluorescent (WW 3200 - 3700K)
// 17 = Standard light A
// 18 = Standard light B
// 19 = Standard light C
// 20 = D55
// 21 = D65
// 22 = D75
// 23 = D50
// 24 = ISO studio tungsten
// 255 = Other light source

// SensingMethod
// 
// 1 = Not defined
// 2 = One-chip color area sensor
// 3 = Two-chip color area sensor
// 4 = Three-chip color area sensor
// 5 = Color sequential area sensor
// 7 = Trilinear sensor
// 8 = Color sequential linear sensor


class TL_EXPORT JpegMetadata
  : public ImageMetadataBase
{

public:
  
  JpegMetadata();
  ~JpegMetadata() override;
  
  //std::string metadata(const std::string &name, bool &active) const override;
  //void setMetadata(const std::string &name, const std::string &value) override;
  //std::map<std::string, std::string> metadata() const override;
  //std::map<std::string, std::string> activeMetadata() const override;
  //void reset() override;

private:

  void init() override;
  //std::map<std::string, std::string> metadata(bool all) const;

};

JpegMetadata::JpegMetadata()
  : ImageMetadataBase(Format::jpeg)
{
  this->init();
}

JpegMetadata::~JpegMetadata()
{
}

//std::string JpegMetadata::metadata(const std::string &name, bool &active) const
//{
//  std::string value;
//  active = false;
//
//  auto metadata = mMetadata.find(name);
//  if (metadata != mMetadata.end()) {
//    value = metadata->second.first;
//    active = metadata->second.second;
//  } else {
//    msgWarning("Metadata '%s' not supported", name.c_str());
//  }
//
//  return value;
//}
//
//void JpegMetadata::setMetadata(const std::string &name, const std::string &value)
//{
//  auto metadata = mMetadata.find(name);
//  if (metadata != mMetadata.end()) {
//    metadata->second.first = value;
//    metadata->second.second = true;
//  } else {
//    msgWarning("Metadata '%s' not supported", name.c_str());
//  }
//}

//std::map<std::string, std::string> JpegMetadata::metadata() const
//{
//  return this->metadata(true);
//}
//
//std::map<std::string, std::string> JpegMetadata::activeMetadata() const
//{
//  return this->metadata(false);
//}
//
//void JpegMetadata::reset()
//{
//  this->init();
//}

void JpegMetadata::init()
{
  mMetadata["EXIF_Document_Name"] = std::make_pair("", false);
  mMetadata["EXIF_ImageDescription"] = std::make_pair("", false);
  mMetadata["EXIF_Make"] = std::make_pair("", false);
  mMetadata["EXIF_Model"] = std::make_pair("", false);
  mMetadata["EXIF_Orientation"] = std::make_pair("1", false);
  mMetadata["EXIF_XResolution"] = std::make_pair("72", false);
  mMetadata["EXIF_YResolution"] = std::make_pair("72", false);
  mMetadata["EXIF_ResolutionUnit"] = std::make_pair("2", false);
  mMetadata["EXIF_Software"] = std::make_pair("", false);
  mMetadata["EXIF_DateTime"] = std::make_pair("", false);
  mMetadata["EXIF_Artist"] = std::make_pair("", false);
  mMetadata["EXIF_WhitePoint"] = std::make_pair("", false);
  mMetadata["EXIF_PrimaryChromaticities"] = std::make_pair("", false);
  mMetadata["EXIF_YCbCrCoefficients"] = std::make_pair("", false);
  mMetadata["EXIF_YCbCrPositioning"] = std::make_pair("1", false);
  mMetadata["EXIF_ReferenceBlackWhite"] = std::make_pair("", false);
  mMetadata["EXIF_Copyright"] = std::make_pair("", false);
  mMetadata["EXIF_ExposureTime"] = std::make_pair("", false);
  mMetadata["EXIF_FNumber"] = std::make_pair("", false);
  mMetadata["EXIF_ExposureProgram"] = std::make_pair("0", false);
  mMetadata["EXIF_SpectralSensitivity"] = std::make_pair("", false);
  mMetadata["EXIF_ISOSpeedRatings"] = std::make_pair("", false);
  mMetadata["EXIF_OECF"] = std::make_pair("", false);
  mMetadata["EXIF_SensitivityType"] = std::make_pair("", false);
  mMetadata["EXIF_StandardOutputSensitivity"] = std::make_pair("", false);
  mMetadata["EXIF_RecommendedExposureIndex"] = std::make_pair("", false);
  mMetadata["EXIF_ISOSpeed"] = std::make_pair("", false);
  mMetadata["EXIF_ISOSpeedLatitudeyyy"] = std::make_pair("", false);
  mMetadata["EXIF_ISOSpeedLatitudezzz"] = std::make_pair("", false);
  mMetadata["EXIF_ExifVersion"] = std::make_pair("0230", false);
  mMetadata["EXIF_DateTimeOriginal"] = std::make_pair("", false);
  mMetadata["EXIF_DateTimeDigitized"] = std::make_pair("", false);
  mMetadata["EXIF_OffsetTime"] = std::make_pair("", false);
  mMetadata["EXIF_OffsetTimeOriginal"] = std::make_pair("", false);
  mMetadata["EXIF_OffsetTimeDigitized"] = std::make_pair("", false);
  mMetadata["EXIF_ComponentsConfiguration"] = std::make_pair("", false);
  mMetadata["EXIF_CompressedBitsPerPixel"] = std::make_pair("", false);
  mMetadata["EXIF_ShutterSpeedValue"] = std::make_pair("", false);
  mMetadata["EXIF_ApertureValue"] = std::make_pair("", false);
  mMetadata["EXIF_BrightnessValue"] = std::make_pair("", false);
  mMetadata["EXIF_ExposureBiasValue"] = std::make_pair("", false);
  mMetadata["EXIF_MaxApertureValue"] = std::make_pair("", false);
  mMetadata["EXIF_SubjectDistance"] = std::make_pair("", false);
  mMetadata["EXIF_MeteringMode"] = std::make_pair("", false);
  mMetadata["EXIF_LightSource"] = std::make_pair("0", false);
  mMetadata["EXIF_Flash"] = std::make_pair("", false);
  mMetadata["EXIF_FocalLength"] = std::make_pair("", false);
  mMetadata["EXIF_SubjectArea"] = std::make_pair("", false);
  mMetadata["EXIF_MakerNote"] = std::make_pair("", false);
  mMetadata["EXIF_UserComment"] = std::make_pair("", false);
  mMetadata["EXIF_SubSecTime"] = std::make_pair("", false);
  mMetadata["EXIF_SubSecTime_Original"] = std::make_pair("", false);
  mMetadata["EXIF_SubSecTime_Digitized"] = std::make_pair("", false);
  mMetadata["EXIF_FlashpixVersion"] = std::make_pair("0100", false);
  mMetadata["EXIF_ColorSpace"] = std::make_pair("", false);
  mMetadata["EXIF_PixelXDimension"] = std::make_pair("", false);
  mMetadata["EXIF_PixelYDimension"] = std::make_pair("", false);
  mMetadata["EXIF_RelatedSoundFile"] = std::make_pair("", false);
  mMetadata["EXIF_FlashEnergy"] = std::make_pair("", false);
  mMetadata["EXIF_SpatialFrequencyResponse"] = std::make_pair("", false);
  mMetadata["EXIF_FocalPlaneXResolution"] = std::make_pair("", false);
  mMetadata["EXIF_FocalPlaneYResolution"] = std::make_pair("", false);
  mMetadata["EXIF_FocalPlaneResolutionUnit"] = std::make_pair("2", false);
  mMetadata["EXIF_SubjectLocation"] = std::make_pair("", false);
  mMetadata["EXIF_ExposureIndex"] = std::make_pair("", false);
  mMetadata["EXIF_SensingMethod"] = std::make_pair("", false);
  mMetadata["EXIF_FileSource"] = std::make_pair("3 (Digital Still Camera)", false);
  mMetadata["EXIF_SceneType"] = std::make_pair("	1 (Directly photographed image)", false);
  mMetadata["EXIF_CFAPattern"] = std::make_pair("", false);
  mMetadata["EXIF_CustomRendered"] = std::make_pair("", false);
  mMetadata["EXIF_ExposureMode"] = std::make_pair("", false);
  mMetadata["EXIF_WhiteBalance"] = std::make_pair("", false);
  mMetadata["EXIF_DigitalZoomRatio"] = std::make_pair("", false);
  mMetadata["EXIF_FocalLengthIn35mmFilm"] = std::make_pair("", false);
  mMetadata["EXIF_SceneCaptureType"] = std::make_pair("0", false);
  mMetadata["EXIF_GainControl"] = std::make_pair("", false);
  mMetadata["EXIF_Contrast"] = std::make_pair("0", false);
  mMetadata["EXIF_Saturation"] = std::make_pair("0", false);
  mMetadata["EXIF_Sharpness"] = std::make_pair("0", false);
  mMetadata["EXIF_DeviceSettingDescription"] = std::make_pair("", false);
  mMetadata["EXIF_SubjectDistanceRange"] = std::make_pair("0", false);
  mMetadata["EXIF_ImageUniqueID"] = std::make_pair("", false);
  mMetadata["EXIF_CameraOwnerName"] = std::make_pair("", false);
  mMetadata["EXIF_BodySerialNumber"] = std::make_pair("", false);
  mMetadata["EXIF_LensSpecification"] = std::make_pair("", false);
  mMetadata["EXIF_LensMake"] = std::make_pair("", false);
  mMetadata["EXIF_LensModel"] = std::make_pair("", false);
  mMetadata["EXIF_LensSerialNumber"] = std::make_pair("", false);
  mMetadata["EXIF_GPSVersionID"] = std::make_pair("2.3.0.0", false);
  mMetadata["EXIF_GPSLatitudeRef"] = std::make_pair("", false);
  mMetadata["EXIF_GPSLatitude"] = std::make_pair("", false);
  mMetadata["EXIF_GPSLongitudeRef"] = std::make_pair("", false);
  mMetadata["EXIF_GPSLongitude"] = std::make_pair("", false);
  mMetadata["EXIF_GPSAltitudeRef"] = std::make_pair("0", false);
  mMetadata["EXIF_GPSAltitude"] = std::make_pair("", false);
  mMetadata["EXIF_GPSTimeStamp"] = std::make_pair("", false);
  mMetadata["EXIF_GPSSatellites"] = std::make_pair("", false);
  mMetadata["EXIF_GPSStatus"] = std::make_pair("", false);
  mMetadata["EXIF_GPSMeasureMode"] = std::make_pair("", false);
  mMetadata["EXIF_GPSDOP"] = std::make_pair("", false);
  mMetadata["EXIF_GPSSpeedRef"] = std::make_pair("K", false);
  mMetadata["EXIF_GPSSpeed"] = std::make_pair("", false);
  mMetadata["EXIF_GPSTrackRef"] = std::make_pair("T", false);
  mMetadata["EXIF_GPSTrack"] = std::make_pair("", false);
  mMetadata["EXIF_GPSImgDirectionRef"] = std::make_pair("T", false);
  mMetadata["EXIF_GPSImgDirection"] = std::make_pair("", false);
  mMetadata["EXIF_GPSMapDatum"] = std::make_pair("", false);
  mMetadata["EXIF_GPSDestLatitudeRef"] = std::make_pair("", false);
  mMetadata["EXIF_GPSDestLatitude"] = std::make_pair("", false);
  mMetadata["EXIF_GPSDestLongitudeRef"] = std::make_pair("", false);
  mMetadata["EXIF_GPSDestLongitude"] = std::make_pair("", false);
  mMetadata["EXIF_GPSDestBearingRef"] = std::make_pair("T", false);
  mMetadata["EXIF_GPSDestBearing"] = std::make_pair("", false);
  mMetadata["EXIF_GPSDestDistanceRef"] = std::make_pair("K", false);
  mMetadata["EXIF_GPSDestDistance"] = std::make_pair("", false);
  mMetadata["EXIF_GPSProcessingMethod"] = std::make_pair("", false);
  mMetadata["EXIF_GPSAreaInformation"] = std::make_pair("", false);
  mMetadata["EXIF_GPSDateStamp"] = std::make_pair("", false);
  mMetadata["EXIF_GPSDifferential"] = std::make_pair("", false);
  mMetadata["EXIF_GPSHPositioningError"] = std::make_pair("", false);
}

//std::map<std::string, std::string> JpegMetadata::metadata(bool all) const
//{
//  std::map<std::string, std::string> metadata;
//
//  for (auto it = mMetadata.begin(); it != mMetadata.end(); it++) {
//    if (all || it->second.second == true)
//      metadata[it->first] = it->second.first;
//  }
//
//  return metadata;
//}





class TiffMetadata
  : public ImageMetadataBase
{
public:
  
  TiffMetadata()
  : ImageMetadataBase(Format::tiff)
  {
  }
  
  ~TiffMetadata()
  {
  }

private:

  void init() override;

};

void TiffMetadata::init()
{
  mMetadata["TIFFTAG_DOCUMENTNAME"] = std::make_pair("", false);
  mMetadata["TIFFTAG_IMAGEDESCRIPTION"] = std::make_pair("", false);
  mMetadata["TIFFTAG_SOFTWARE"] = std::make_pair("", false);
  mMetadata["TIFFTAG_DATETIME"] = std::make_pair("", false);
  mMetadata["TIFFTAG_ARTIST"] = std::make_pair("", false);
  mMetadata["TIFFTAG_HOSTCOMPUTER"] = std::make_pair("", false);
  mMetadata["TIFFTAG_COPYRIGHT"] = std::make_pair("", false);
  mMetadata["TIFFTAG_XRESOLUTION"] = std::make_pair("", false);
  mMetadata["TIFFTAG_YRESOLUTION"] = std::make_pair("", false);
  mMetadata["TIFFTAG_RESOLUTIONUNIT"] = std::make_pair("", false);
  mMetadata["TIFFTAG_MINSAMPLEVALUE"] = std::make_pair("", false);
  mMetadata["TIFFTAG_MAXSAMPLEVALUE"] = std::make_pair("", false);
  mMetadata["GEO_METADATA"] = std::make_pair("", false);
  mMetadata["TIFF_RSID"] = std::make_pair("", false);
}





class PngMetadata
  : public ImageMetadataBase
{
public:
  
  PngMetadata()
  : ImageMetadataBase(Format::png)
  {
  }
  
  ~PngMetadata()
  {
  }

private:

  void init() override;

};

void PngMetadata::init()
{
  mMetadata["SOURCE_ICC_PROFILE"] = std::make_pair("", false);
  mMetadata["SOURCE_ICC_PROFILE_NAME"] = std::make_pair("", false);
  mMetadata["SOURCE_PRIMARIES_RED"] = std::make_pair("", false);
  mMetadata["SOURCE_PRIMARIES_GREEN"] = std::make_pair("", false);
  mMetadata["SOURCE_PRIMARIES_BLUE"] = std::make_pair("", false);
  mMetadata["SOURCE_WHITEPOINT"] = std::make_pair("", false);
  mMetadata["PNG_GAMMA"] = std::make_pair("", false);
}





class BmpMetadata
  : public ImageMetadataBase
{
public:
  
  BmpMetadata()
  : ImageMetadataBase(Format::bmp)
  {
  }
  
  ~BmpMetadata()
  {
  }

private:

  void init() override {}

};




class GifMetadata
  : public ImageMetadataBase
{
public:
  
  GifMetadata()
  : ImageMetadataBase(Format::gif)
  {
  }
  
  ~GifMetadata()
  {
  }

private:

  void init() override {}

};






std::shared_ptr<ImageMetadata> ImageMetadataFactory::create(const std::string &format)
{
  std::shared_ptr<ImageMetadata> imageMetadata;
    
  if (format.compare("JPEG") == 0) {
    imageMetadata = std::make_shared<JpegMetadata>();
  } else if (format.compare("TIFF") == 0) {
    imageMetadata = std::make_shared<TiffMetadata>();
  } else if (format.compare("PNG") == 0) {
    imageMetadata = std::make_shared<PngMetadata>();
  } else if (format.compare("BMP") == 0) {
    imageMetadata = std::make_shared<BmpMetadata>();
  } else if (format.compare("GIF") == 0) {
    imageMetadata = std::make_shared<GifMetadata>();
  } else {
    throw std::runtime_error("Invalid Image Format");
  }

  return imageMetadata;
}


} // End namespace tl
