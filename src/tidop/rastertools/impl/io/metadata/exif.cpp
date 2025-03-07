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

#include "tidop/rastertools/impl/io/metadata/exif.h"

namespace tl
{


ExifMetadata::ExifMetadata(Format format)
  : ImageMetadataBase(format)
{
    ExifMetadata::init();
}

ExifMetadata::~ExifMetadata()
{
}

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


} // End namespace tl
