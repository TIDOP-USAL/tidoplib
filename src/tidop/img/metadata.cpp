/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by Esteban Ruiz de Oña Crespo                       *
 *                                                                        *
 * This file is part of TidopLib                                          *
 *                                                                        *
 * TidopLib is free software: you can redistribute it and/or modify       *
 * it under the terms of the GNU Lesser General Public License as         *
 * published by the Free Software Foundation, either version 3 of the     *
 * License, or (at your option) any later version.                        *
 *                                                                        *
 * TidopLib is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU Lesser General Public License for more details.                    *
 *                                                                        *
 * You should have received a copy of the GNU Lesser General Public       *
 * License along with TidopLib. If not, see <http://www.gnu.org/licenses>.*
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#include "tidop/img/metadata.h"

#include "tidop/core/msg/message.h"

namespace tl
{


MetadataItemBase::MetadataItemBase(std::string name,
                                   std::string defValue)
  : MetadataItem(),
    mName(std::move(name)),
    mDefaultValue(std::move(defValue)),
    bActive(false)
{
}

auto MetadataItemBase::value() const -> std::string
{
    return mValue;
}

void MetadataItemBase::setValue(const std::string &value)
{
    bActive = true;
    mValue = value;
}

auto MetadataItemBase::defaultValue() const -> std::string
{
    return mDefaultValue;
}

void MetadataItemBase::setDefaultValue(const std::string &defValue)
{
    mDefaultValue = defValue;
}

auto MetadataItemBase::isActive() const -> bool
{
    return bActive;
}



MetadataItemNumber::MetadataItemNumber(const std::string &name,
                                       const std::string &defValue)
    : MetadataItemBase(name, defValue)
{
}

void MetadataItemNumber::parseValue(const std::string &value)
{
    size_t pos1 = value.find('(');
    size_t pos2 = value.find(')');

    if (pos1 != std::string::npos && pos2 != std::string::npos) {
        setValue(value.substr(pos1 + 1, pos2 - pos1 + 1));
    }
}



MetadataItemText::MetadataItemText(const std::string &name,
                                   const std::string &defValue)
    : MetadataItemBase(name, defValue)
{
}

void MetadataItemText::parseValue(const std::string &value)
{
    setValue(value);
}





ImageMetadata::ImageMetadata(Format format)
    : mFormat(format)
{
}

ImageMetadata::~ImageMetadata() = default;

auto ImageMetadata::format() const -> Format
{
    return mFormat;
}






class TL_EXPORT ImageMetadataBase
    : public ImageMetadata
{

protected:

    std::map<std::string, std::pair<std::string, bool>> mMetadata;

public:

    ImageMetadataBase(Format format)
        : ImageMetadata(format)
    {
    }

    ~ImageMetadataBase() override = default;

    auto metadata(const std::string& name,
                  bool &active) const -> std::string override;
    void setMetadata(const std::string &name,
                     const std::string &value) override;

    auto metadata() const -> std::map<std::string, std::string> override
    {
        return this->metadata(true);
    }

    auto activeMetadata() const -> std::map<std::string, std::string> override
    {
        return this->metadata(false);
    }

    void reset() override
    {
        this->init();
    }

private:

    virtual void init()
    {
    }

    auto metadata(bool all) const -> std::map<std::string, std::string>;

};


auto ImageMetadataBase::metadata(const std::string& name, bool& active) const -> std::string
{
    std::string value;
    active = false;

    auto metadata = mMetadata.find(name);
    if (metadata != mMetadata.end()) {
        value = metadata->second.first;
        active = metadata->second.second;
    } else {
        Message::warning("Metadata '{}' not supported", name);
    }

    return value;
}

void ImageMetadataBase::setMetadata(const std::string &name, const std::string &value)
{
    auto metadata = mMetadata.find(name);
    if (metadata != mMetadata.end()) {
        metadata->second.first = value;
        metadata->second.second = true;
    } /*else {
      msgWarning("Metadata '%s' not supported", name.c_str());
    }*/
}

auto ImageMetadataBase::metadata(bool all) const -> std::map<std::string, std::string>
{
    std::map<std::string, std::string> metadata;

    for (auto &pair : mMetadata) {
        if (all || pair.second.second == true)
            metadata[pair.first] = pair.second.first;
    }

    return metadata;
}




class TL_EXPORT ExifMetadata
    : public ImageMetadataBase
{

public:

    ExifMetadata(Format format)
        : ImageMetadataBase(format)
    {
        ExifMetadata::init();
    }

    ~ExifMetadata() override
    {
    }

private:

    void init() override;

};


void ExifMetadata::init()
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


    mMetadata["XMP_About"] = std::make_pair("", false);
    mMetadata["XMP_AbsoluteAltitude"] = std::make_pair("", false);
    mMetadata["XMP_RelativeAltitude"] = std::make_pair("", false);
    mMetadata["XMP_GpsLatitude"] = std::make_pair("", false);
    mMetadata["XMP_GpsLongitude"] = std::make_pair("", false);
    mMetadata["XMP_GimbalRollDegree"] = std::make_pair("", false);
    mMetadata["XMP_GimbalYawDegree"] = std::make_pair("", false);
    mMetadata["XMP_GimbalPitchDegree"] = std::make_pair("", false);
    mMetadata["XMP_FlightRollDegree"] = std::make_pair("", false);
    mMetadata["XMP_FlightYawDegree"] = std::make_pair("", false);
    mMetadata["XMP_FlightPitchDegree"] = std::make_pair("", false);
    mMetadata["XMP_FlightXSpeed"] = std::make_pair("", false);
    mMetadata["XMP_FlightYSpeed"] = std::make_pair("", false);
    mMetadata["XMP_FlightZSpeed"] = std::make_pair("", false);
    mMetadata["XMP_CamReverse"] = std::make_pair("", false);
    mMetadata["XMP_GimbalReverse"] = std::make_pair("", false);
    mMetadata["XMP_CalibratedFocalLength"] = std::make_pair("", false);
    mMetadata["XMP_CalibratedOpticalCenterX"] = std::make_pair("", false);
    mMetadata["XMP_CalibratedOpticalCenterY"] = std::make_pair("", false);
    mMetadata["XMP_RtkFlag"] = std::make_pair("", false);
    mMetadata["XMP_RtkStdLon"] = std::make_pair("", false);
    mMetadata["XMP_RtkStdLat"] = std::make_pair("", false);
    mMetadata["XMP_RtkStdHgt"] = std::make_pair("", false);
}



class TL_EXPORT JpegMetadata
    : public ExifMetadata
{

public:

    JpegMetadata();
    ~JpegMetadata() override;

private:

    void init() override;

};

JpegMetadata::JpegMetadata()
    : ExifMetadata(Format::jpeg)
{
    this->init();
}

JpegMetadata::~JpegMetadata() = default;

void JpegMetadata::init()
{
    //mMetadata["EXIF_Document_Name"] = std::make_pair("", false);
    //mMetadata["EXIF_ImageDescription"] = std::make_pair("", false);
    //mMetadata["EXIF_Make"] = std::make_pair("", false);
    //mMetadata["EXIF_Model"] = std::make_pair("", false);
    //mMetadata["EXIF_Orientation"] = std::make_pair("1", false);
    //mMetadata["EXIF_XResolution"] = std::make_pair("72", false);
    //mMetadata["EXIF_YResolution"] = std::make_pair("72", false);
    //mMetadata["EXIF_ResolutionUnit"] = std::make_pair("2", false);
    //mMetadata["EXIF_Software"] = std::make_pair("", false);
    //mMetadata["EXIF_DateTime"] = std::make_pair("", false);
    //mMetadata["EXIF_Artist"] = std::make_pair("", false);
    //mMetadata["EXIF_WhitePoint"] = std::make_pair("", false);
    //mMetadata["EXIF_PrimaryChromaticities"] = std::make_pair("", false);
    //mMetadata["EXIF_YCbCrCoefficients"] = std::make_pair("", false);
    //mMetadata["EXIF_YCbCrPositioning"] = std::make_pair("1", false);
    //mMetadata["EXIF_ReferenceBlackWhite"] = std::make_pair("", false);
    //mMetadata["EXIF_Copyright"] = std::make_pair("", false);
    //mMetadata["EXIF_ExposureTime"] = std::make_pair("", false);
    //mMetadata["EXIF_FNumber"] = std::make_pair("", false);
    //mMetadata["EXIF_ExposureProgram"] = std::make_pair("0", false);
    //mMetadata["EXIF_SpectralSensitivity"] = std::make_pair("", false);
    //mMetadata["EXIF_ISOSpeedRatings"] = std::make_pair("", false);
    //mMetadata["EXIF_OECF"] = std::make_pair("", false);
    //mMetadata["EXIF_SensitivityType"] = std::make_pair("", false);
    //mMetadata["EXIF_StandardOutputSensitivity"] = std::make_pair("", false);
    //mMetadata["EXIF_RecommendedExposureIndex"] = std::make_pair("", false);
    //mMetadata["EXIF_ISOSpeed"] = std::make_pair("", false);
    //mMetadata["EXIF_ISOSpeedLatitudeyyy"] = std::make_pair("", false);
    //mMetadata["EXIF_ISOSpeedLatitudezzz"] = std::make_pair("", false);
    //mMetadata["EXIF_ExifVersion"] = std::make_pair("0230", false);
    //mMetadata["EXIF_DateTimeOriginal"] = std::make_pair("", false);
    //mMetadata["EXIF_DateTimeDigitized"] = std::make_pair("", false);
    //mMetadata["EXIF_OffsetTime"] = std::make_pair("", false);
    //mMetadata["EXIF_OffsetTimeOriginal"] = std::make_pair("", false);
    //mMetadata["EXIF_OffsetTimeDigitized"] = std::make_pair("", false);
    //mMetadata["EXIF_ComponentsConfiguration"] = std::make_pair("", false);
    //mMetadata["EXIF_CompressedBitsPerPixel"] = std::make_pair("", false);
    //mMetadata["EXIF_ShutterSpeedValue"] = std::make_pair("", false);
    //mMetadata["EXIF_ApertureValue"] = std::make_pair("", false);
    //mMetadata["EXIF_BrightnessValue"] = std::make_pair("", false);
    //mMetadata["EXIF_ExposureBiasValue"] = std::make_pair("", false);
    //mMetadata["EXIF_MaxApertureValue"] = std::make_pair("", false);
    //mMetadata["EXIF_SubjectDistance"] = std::make_pair("", false);
    //mMetadata["EXIF_MeteringMode"] = std::make_pair("", false);
    //mMetadata["EXIF_LightSource"] = std::make_pair("0", false);
    //mMetadata["EXIF_Flash"] = std::make_pair("", false);
    //mMetadata["EXIF_FocalLength"] = std::make_pair("", false);
    //mMetadata["EXIF_SubjectArea"] = std::make_pair("", false);
    //mMetadata["EXIF_MakerNote"] = std::make_pair("", false);
    //mMetadata["EXIF_UserComment"] = std::make_pair("", false);
    //mMetadata["EXIF_SubSecTime"] = std::make_pair("", false);
    //mMetadata["EXIF_SubSecTime_Original"] = std::make_pair("", false);
    //mMetadata["EXIF_SubSecTime_Digitized"] = std::make_pair("", false);
    //mMetadata["EXIF_FlashpixVersion"] = std::make_pair("0100", false);
    //mMetadata["EXIF_ColorSpace"] = std::make_pair("", false);
    //mMetadata["EXIF_PixelXDimension"] = std::make_pair("", false);
    //mMetadata["EXIF_PixelYDimension"] = std::make_pair("", false);
    //mMetadata["EXIF_RelatedSoundFile"] = std::make_pair("", false);
    //mMetadata["EXIF_FlashEnergy"] = std::make_pair("", false);
    //mMetadata["EXIF_SpatialFrequencyResponse"] = std::make_pair("", false);
    //mMetadata["EXIF_FocalPlaneXResolution"] = std::make_pair("", false);
    //mMetadata["EXIF_FocalPlaneYResolution"] = std::make_pair("", false);
    //mMetadata["EXIF_FocalPlaneResolutionUnit"] = std::make_pair("2", false);
    //mMetadata["EXIF_SubjectLocation"] = std::make_pair("", false);
    //mMetadata["EXIF_ExposureIndex"] = std::make_pair("", false);
    //mMetadata["EXIF_SensingMethod"] = std::make_pair("", false);
    //mMetadata["EXIF_FileSource"] = std::make_pair("3 (Digital Still Camera)", false);
    //mMetadata["EXIF_SceneType"] = std::make_pair("	1 (Directly photographed image)", false);
    //mMetadata["EXIF_CFAPattern"] = std::make_pair("", false);
    //mMetadata["EXIF_CustomRendered"] = std::make_pair("", false);
    //mMetadata["EXIF_ExposureMode"] = std::make_pair("", false);
    //mMetadata["EXIF_WhiteBalance"] = std::make_pair("", false);
    //mMetadata["EXIF_DigitalZoomRatio"] = std::make_pair("", false);
    //mMetadata["EXIF_FocalLengthIn35mmFilm"] = std::make_pair("", false);
    //mMetadata["EXIF_SceneCaptureType"] = std::make_pair("0", false);
    //mMetadata["EXIF_GainControl"] = std::make_pair("", false);
    //mMetadata["EXIF_Contrast"] = std::make_pair("0", false);
    //mMetadata["EXIF_Saturation"] = std::make_pair("0", false);
    //mMetadata["EXIF_Sharpness"] = std::make_pair("0", false);
    //mMetadata["EXIF_DeviceSettingDescription"] = std::make_pair("", false);
    //mMetadata["EXIF_SubjectDistanceRange"] = std::make_pair("0", false);
    //mMetadata["EXIF_ImageUniqueID"] = std::make_pair("", false);
    //mMetadata["EXIF_CameraOwnerName"] = std::make_pair("", false);
    //mMetadata["EXIF_BodySerialNumber"] = std::make_pair("", false);
    //mMetadata["EXIF_LensSpecification"] = std::make_pair("", false);
    //mMetadata["EXIF_LensMake"] = std::make_pair("", false);
    //mMetadata["EXIF_LensModel"] = std::make_pair("", false);
    //mMetadata["EXIF_LensSerialNumber"] = std::make_pair("", false);
    //mMetadata["EXIF_GPSVersionID"] = std::make_pair("2.3.0.0", false);
    //mMetadata["EXIF_GPSLatitudeRef"] = std::make_pair("", false);
    //mMetadata["EXIF_GPSLatitude"] = std::make_pair("", false);
    //mMetadata["EXIF_GPSLongitudeRef"] = std::make_pair("", false);
    //mMetadata["EXIF_GPSLongitude"] = std::make_pair("", false);
    //mMetadata["EXIF_GPSAltitudeRef"] = std::make_pair("0", false);
    //mMetadata["EXIF_GPSAltitude"] = std::make_pair("", false);
    //mMetadata["EXIF_GPSTimeStamp"] = std::make_pair("", false);
    //mMetadata["EXIF_GPSSatellites"] = std::make_pair("", false);
    //mMetadata["EXIF_GPSStatus"] = std::make_pair("", false);
    //mMetadata["EXIF_GPSMeasureMode"] = std::make_pair("", false);
    //mMetadata["EXIF_GPSDOP"] = std::make_pair("", false);
    //mMetadata["EXIF_GPSSpeedRef"] = std::make_pair("K", false);
    //mMetadata["EXIF_GPSSpeed"] = std::make_pair("", false);
    //mMetadata["EXIF_GPSTrackRef"] = std::make_pair("T", false);
    //mMetadata["EXIF_GPSTrack"] = std::make_pair("", false);
    //mMetadata["EXIF_GPSImgDirectionRef"] = std::make_pair("T", false);
    //mMetadata["EXIF_GPSImgDirection"] = std::make_pair("", false);
    //mMetadata["EXIF_GPSMapDatum"] = std::make_pair("", false);
    //mMetadata["EXIF_GPSDestLatitudeRef"] = std::make_pair("", false);
    //mMetadata["EXIF_GPSDestLatitude"] = std::make_pair("", false);
    //mMetadata["EXIF_GPSDestLongitudeRef"] = std::make_pair("", false);
    //mMetadata["EXIF_GPSDestLongitude"] = std::make_pair("", false);
    //mMetadata["EXIF_GPSDestBearingRef"] = std::make_pair("T", false);
    //mMetadata["EXIF_GPSDestBearing"] = std::make_pair("", false);
    //mMetadata["EXIF_GPSDestDistanceRef"] = std::make_pair("K", false);
    //mMetadata["EXIF_GPSDestDistance"] = std::make_pair("", false);
    //mMetadata["EXIF_GPSProcessingMethod"] = std::make_pair("", false);
    //mMetadata["EXIF_GPSAreaInformation"] = std::make_pair("", false);
    //mMetadata["EXIF_GPSDateStamp"] = std::make_pair("", false);
    //mMetadata["EXIF_GPSDifferential"] = std::make_pair("", false);
    //mMetadata["EXIF_GPSHPositioningError"] = std::make_pair("", false);
}




class TiffMetadata
    : public ExifMetadata
{
public:

    TiffMetadata()
        : ExifMetadata(Format::tiff)
    {
        TiffMetadata::init();
    }

    ~TiffMetadata() override = default;

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

    ~PngMetadata() override = default;

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

    ~BmpMetadata() override = default;

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

    ~GifMetadata() override = default;

private:

    void init() override {}

};






std::shared_ptr<ImageMetadata> ImageMetadataFactory::create(const std::string &format)
{
    std::shared_ptr<ImageMetadata> imageMetadata;

    if (format == "JPEG") {
        imageMetadata = std::make_shared<JpegMetadata>();
    } else if (format == "GTiff") {
        imageMetadata = std::make_shared<TiffMetadata>();
    } else if (format == "PNG") {
        imageMetadata = std::make_shared<PngMetadata>();
    } else if (format == "BMP") {
        imageMetadata = std::make_shared<BmpMetadata>();
    } else if (format == "GIF") {
        imageMetadata = std::make_shared<GifMetadata>();
    } else {
        throw std::runtime_error("Invalid Image Format");
    }

    return imageMetadata;
}


} // End namespace tl
