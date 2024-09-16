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

#include "tidop/img/formats.h"

#include "tidop/img/img.h"


namespace tl
{


ImageOptionsBase::ImageOptionsBase(Format format)
  : mFormat(format)
{
}

ImageOptionsBase::~ImageOptionsBase() = default;

auto ImageOptionsBase::format() const -> Format
{
    return mFormat;
}

auto ImageOptionsBase::options() const -> std::map<std::string, std::string>
{
    return this->options(true);
}

auto ImageOptionsBase::activeOptions() const -> std::map<std::string, std::string>
{
    return this->options(false);
}



/* TiffOptions */

TiffOptions::TiffOptions()
  : ImageOptionsBase(Format::tiff)
{
    this->init();
}

TiffOptions::~TiffOptions() = default;

void TiffOptions::reset()
{
    this->init();
}


auto TiffOptions::isEnableTFW() const -> bool
{
    return bTFW.second;
}

void TiffOptions::enableTFW(bool value)
{
    bTFW.second = value;
}

auto TiffOptions::isEnableRPB() const -> bool
{
    return bRPB.second;
}

void TiffOptions::enableRPB(bool value)
{
    bRPB.second = value;
}

auto TiffOptions::isEnableRPCTX() const -> bool
{
#if GDAL_VERSION_MAJOR < 2
    TL_COMPILER_WARNING("Option 'RPCTX' not supported for this version of GDAL");
#endif
    return bRPCTX.second;
}

void TiffOptions::enableRPCTX(bool value)
{
#if GDAL_VERSION_MAJOR < 2
    TL_COMPILER_WARNING("Option 'RPCTX' not supported for this version of GDAL");
#endif
    bRPCTX.second = value;
}

auto TiffOptions::isEnableTiled() const -> bool
{
    return bTiled.second;
}

void TiffOptions::enableTiled(bool value)
{
    bTiled.second = value;
}

auto TiffOptions::blockXSize() const -> int
{
    return mBlockXSize.second;
}

void TiffOptions::setBlockXSize(int blockXSize)
{
    mBlockXSize.second = blockXSize;
}

void TiffOptions::setBlockYSize(int blockYSize)
{
    mBlockYSize.second = blockYSize;
}

auto TiffOptions::blockYSize() const -> int
{
    return mBlockYSize.second;
}

auto TiffOptions::nBits() const -> int
{
    return mNBits.second;
}

void TiffOptions::setNBits(int nBits)
{
    mNBits.second = nBits;
}

auto TiffOptions::jpegQuality() const -> uint8_t
{
    return mJpegQuality.second;
}

void TiffOptions::setJpegQuality(uint8_t jpegQuality)
{
    if (jpegQuality > 100) mJpegQuality.second = 100;
    else mJpegQuality.second = jpegQuality;
}

auto TiffOptions::zLevel() const -> uint8_t
{
    return mZLevel.second;
}

void TiffOptions::setZLevel(uint8_t zLevel)
{
    if (zLevel > 9) mZLevel.second = 9;
    else mZLevel.second = zLevel;
}

auto TiffOptions::bigTiff() const -> BigTiff
{
    return mBigTiff.second;
}

void TiffOptions::setBigTiff(BigTiff bigTiff)
{
    mBigTiff.second = bigTiff;
}

auto TiffOptions::compress() const -> Compress
{
    return mCompress.second;
}

void TiffOptions::setCompress(Compress compress)
{
    mCompress.second = compress;
}

auto TiffOptions::photometric() const -> Photometric
{
    return mPhotometric.second;
}

void TiffOptions::setPhotometric(Photometric photometric)
{
    mPhotometric.second = photometric;
}

auto TiffOptions::alpha() const -> Alpha
{
    return mAlpha.second;
}

void TiffOptions::setAlpha(Alpha alpha)
{
    mAlpha.second = alpha;
}

auto TiffOptions::profile() const -> Profile
{
    return mProfile.second;
}

void TiffOptions::setProfile(Profile profile)
{
    mProfile.second = profile;
}

auto TiffOptions::pixelType() const -> PixelType
{
    return mPixelType.second;
}

void TiffOptions::setPixelType(PixelType pixelType)
{
    mPixelType.second = pixelType;
}

auto TiffOptions::geotiffKeysFlavor() const -> GeotiffKeysFlavor
{
    return mGeotiffKeysFlavor.second;
}

void TiffOptions::setGeotiffKeysFlavor(GeotiffKeysFlavor geotiffKeysFlavor)
{
    mGeotiffKeysFlavor.second = geotiffKeysFlavor;
}

auto TiffOptions::internalMask() const -> bool
{
    return mInternalMask.second;
}

void TiffOptions::setInternalMask(bool internalMask)
{
    mInternalMask.second = internalMask;
}

void TiffOptions::init()
{
    bTFW = std::make_pair(false, false);
    bRPB = std::make_pair(false, false);
    bRPCTX = std::make_pair(false, false);
    bTiled = std::make_pair(false, false);
    mBlockXSize = std::make_pair(256, 256);
    mBlockYSize = std::make_pair(256, 256);
    mNBits = std::make_pair(8, 8);
    mJpegQuality = std::make_pair(75, 75);
    mZLevel = std::make_pair(6, 6);
    mBigTiff = std::make_pair(BigTiff::if_needed, BigTiff::if_needed);
    mCompress = std::make_pair(Compress::none, Compress::none);
    mPhotometric = std::make_pair(Photometric::minisblack, Photometric::minisblack);
    mAlpha = std::make_pair(Alpha::unspecified, Alpha::unspecified);
    mProfile = std::make_pair(Profile::gdal_geotiff, Profile::gdal_geotiff);
    mPixelType = std::make_pair(PixelType::def, PixelType::def);
    mGeotiffKeysFlavor = std::make_pair(GeotiffKeysFlavor::standard, GeotiffKeysFlavor::standard);
}

auto TiffOptions::options(bool all) const -> std::map<std::string, std::string>
{
    std::map<std::string, std::string> options;

    if (all || bTFW.first != bTFW.second) {
        options["TFW"] = bTFW.second ? "YES" : "NO";
    }

    if (all || bRPB.first != bRPB.second) {
        options["RPB"] = bRPB.second ? "YES" : "NO";
    }

    if (all || bRPCTX.first != bRPCTX.second) {
        options["RPCTX"] = bRPCTX.second ? "YES" : "NO";
    }

    if (all || bTiled.first != bTiled.second) {
        options["TILED"] = bTiled.second ? "YES" : "NO";
    }

    if (all || mBlockXSize.first != mBlockXSize.second) {
        options["BLOCKXSIZE"] = std::to_string(mBlockXSize.second);
    }

    if (all || mBlockYSize.first != mBlockYSize.second) {
        options["BLOCKYSIZE"] = std::to_string(mBlockYSize.second);
    }

    if (all || mNBits.first != mNBits.second) {
        options["NBITS"] = std::to_string(mNBits.second);
    }

    if (all || mJpegQuality.first != mJpegQuality.second) {
        options["JPEG_QUALITY"] = std::to_string(mJpegQuality.second);
    }

    if (all || mZLevel.first != mZLevel.second) {
        options["ZLEVEL"] = std::to_string(mZLevel.second);
    }

    if (all || mBigTiff.first != mBigTiff.second) {
        switch (mBigTiff.second) {
        case BigTiff::yes:
            options["BigTiff"] = "YES";
            break;
        case BigTiff::no:
            options["BigTiff"] = "NO";
            break;
        case BigTiff::if_needed:
            options["BigTiff"] = "IF_NEEDED";
            break;
        case BigTiff::if_safer:
            options["BigTiff"] = "IF_SAFER";
            break;
        }
    }

    if (all || mCompress.first != mCompress.second) {

        switch (mCompress.second) {
        case Compress::jpeg:
            options["COMPRESS"] = "JPEG";
            break;
        case Compress::lzw:
            options["COMPRESS"] = "LZW";
            break;
        case Compress::packbits:
            options["COMPRESS"] = "PACKBITS";
            break;
        case Compress::deflate:
            options["COMPRESS"] = "DEFLATE";
            break;
        case Compress::ccittrle:
            options["COMPRESS"] = "CCITTRLE";
            break;
        case Compress::ccittfax3:
            options["COMPRESS"] = "CCITTFAX3";
            break;
        case Compress::ccittfax4:
            options["COMPRESS"] = "CCITTFAX4";
            break;
        case Compress::lzma:
            options["COMPRESS"] = "LZMA";
            break;
#ifdef TL_HAVE_GDAL
#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(2,3,0)
        case Compress::zstd:
            options["COMPRESS"] = "ZSTD";
            break;
#endif
#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(2,4,0)
        case Compress::lerc:
            options["COMPRESS"] = "LERC";
            break;
        case Compress::lerc_deflate:
            options["COMPRESS"] = "LERC_DEFLATE";
            break;
        case Compress::lerc_zstd:
            options["COMPRESS"] = "LERC_ZSTD";
            break;
#endif
#endif // TL_HAVE_GDAL
        case Compress::webp:
            options["COMPRESS"] = "WEBP";
            break;
        case Compress::none:
            options["COMPRESS"] = "NONE";
            break;
        }
    }

    if (all || mPhotometric.first != mPhotometric.second) {

        switch (mPhotometric.second) {
        case Photometric::minisblack:
            options["PHOTOMETRIC"] = "MINISBLACK";
            break;
        case Photometric::miniswhite:
            options["PHOTOMETRIC"] = "MINISWHITE";
            break;
        case Photometric::rgb:
            options["PHOTOMETRIC"] = "RGB";
            break;
        case Photometric::cmyk:
            options["PHOTOMETRIC"] = "CMYK";
            break;
        case Photometric::ycbcr:
            options["PHOTOMETRIC"] = "YCBCR";
            break;
        case Photometric::cielab:
            options["PHOTOMETRIC"] = "CIELAB";
            break;
        case Photometric::icclab:
            options["PHOTOMETRIC"] = "ICCLAB";
            break;
        case Photometric::itulab:
            options["PHOTOMETRIC"] = "ITULAB";
            break;
        }
    }

    if (all || mAlpha.first != mAlpha.second) {

        switch (mAlpha.second) {
        case Alpha::yes:
            options["ALPHA"] = "YES";
            break;
        case Alpha::non_premultiplied:
            options["ALPHA"] = "NON-PREMULTIPLIED";
            break;
        case Alpha::premultiplied:
            options["ALPHA"] = "PREMULTIPLIED";
            break;
        case Alpha::unspecified:
            options["ALPHA"] = "UNSPECIFIED";
            break;
        }
    }

    if (all || mProfile.first != mProfile.second) {
        switch (mProfile.second) {
        case Profile::gdal_geotiff:
            options["PROFILE"] = "GDALGeoTIFF";
            break;
        case Profile::geotiff:
            options["PROFILE"] = "GeoTIFF";
            break;
        case Profile::baseline:
            options["PROFILE"] = "BASELINE";
            break;
        }
    }

    if (all || mPixelType.first != mPixelType.second) {
        switch (mPixelType.second) {
        case PixelType::def:
            options["PIXELTYPE"] = "DEFAULT";
            break;
        case PixelType::signedbyte:
            options["PIXELTYPE"] = "SIGNEDBYTE";
            break;
        }
    }

    if (all || mGeotiffKeysFlavor.first != mGeotiffKeysFlavor.second) {
        switch (mGeotiffKeysFlavor.second) {
        case GeotiffKeysFlavor::standard:
            options["GEOTIFF_KEYS_FLAVOR"] = "STANDARD";
            break;
        case GeotiffKeysFlavor::esri_pe:
            options["GEOTIFF_KEYS_FLAVOR"] = "ESRI_PE";
            break;
        }
    }

    if (all || mInternalMask.first != mInternalMask.second) {
        options["GDAL_TIFF_INTERNAL_MASK"] = std::to_string(mBlockXSize.second);
    }

    return options;
}



/* PngOptions */

PngOptions::PngOptions()
  : ImageOptionsBase(Format::png)
{
    this->init();
}

PngOptions::~PngOptions() = default;

void PngOptions::reset()
{
    this->init();
}

auto PngOptions::isEnableWorldFile() const -> bool
{
    return bWorldFile.second;
}

void PngOptions::setEnableWorldFile(bool enable)
{
    bWorldFile.second = enable;
}

auto PngOptions::zLevel() const -> uint8_t
{
    return mZLevel.second;
}

void PngOptions::setZLevel(uint8_t zLevel)
{
    if (zLevel > 9) mZLevel.second = 9;
    else mZLevel.second = zLevel;
}

#if GDAL_VERSION_MAJOR >= 2

auto PngOptions::title() const -> std::string
{
    return mTitle.second;
}

void PngOptions::setTitle(const std::string &title)
{
    mTitle.second = title;
}

auto PngOptions::description() const -> std::string
{
    return mDescription.second;
}

void PngOptions::setDescription(const std::string &description)
{
    mDescription.second = description;
}

auto PngOptions::copyright() const -> std::string
{
    return mCopyright.second;
}

void PngOptions::setCopyright(const std::string &copyright)
{
    mCopyright.second = copyright;
}

auto PngOptions::comment() const -> std::string
{
    return mComment.second;
}

void PngOptions::setComment(const std::string &comment)
{
    mComment.second = comment;
}

#endif

#ifdef TL_HAVE_GDAL
#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(2,1,0)

auto PngOptions::nBits() const -> int
{
    return mNBits.second;
}

void PngOptions::setNBits(int nBits)
{
    mNBits.second = nBits;
}

#endif
#endif // TL_HAVE_GDAL

void PngOptions::init()
{
    bWorldFile = std::make_pair(false, false);
    mZLevel = std::make_pair(6, 6);
#ifdef TL_HAVE_GDAL
#if GDAL_VERSION_MAJOR >= 2
    mTitle = std::make_pair("", "");
    mDescription = std::make_pair("", "");
    mCopyright = std::make_pair("", "");
    mComment = std::make_pair("", "");
#endif
#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(2,1,0)
    mNBits = std::make_pair(8, 8);
#endif
#endif // TL_HAVE_GDAL
}

auto PngOptions::options(bool all) const -> std::map<std::string, std::string>
{
    std::map<std::string, std::string> options;

    if (all || bWorldFile.first != bWorldFile.second) {
        options["WORLDFILE"] = bWorldFile.second ? "YES" : "NO";
    }

    if (all || mZLevel.first != mZLevel.second) {
        options["ZLEVEL"] = std::to_string(mZLevel.second);
    }

#ifdef TL_HAVE_GDAL
#if GDAL_VERSION_MAJOR >= 2

    if (all || mTitle.first != mTitle.second) {
        options["TITLE"] = mTitle.second;
    }

    if (all || mDescription.first != mDescription.second) {
        options["DESCRIPTION"] = mDescription.second;
    }

    if (all || mCopyright.first != mCopyright.second) {
        options["COPYRIGHT"] = mCopyright.second;
    }

    if (all || mComment.first != mComment.second) {
        options["COMMENT"] = mComment.second;
    }

#endif

#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(2,1,0)

    if (all || mNBits.first != mNBits.second) {
        options["NBITS"] = std::to_string(mNBits.second);
    }

#endif
#endif // TL_HAVE_GDAL

    return options;
}




/* JpegOptions */

JpegOptions::JpegOptions()
  : ImageOptionsBase(Format::jpeg)
{
    this->init();
}

JpegOptions::~JpegOptions() = default;


void JpegOptions::reset()
{
    this->init();
}

auto JpegOptions::isEnableWorldFile() const -> bool
{
    return bWorldFile.second;
}

void JpegOptions::setEnableWorldFile(bool enable)
{
    bWorldFile.second = enable;
}

auto JpegOptions::quality() const -> uint8_t
{
    return mQuality.second;
}

void JpegOptions::setQuality(uint8_t quality)
{
    if (quality < 10) mQuality.second = 10;
    else if (quality > 100) mQuality.second = 100;
    else mQuality.second = quality;
}

auto JpegOptions::internalMask() const -> bool
{
    return bInternalMask.second;
}

void JpegOptions::setInternalMask(bool internalMask)
{
    bInternalMask.second = internalMask;
}

auto JpegOptions::iccProfile() const -> std::string
{
    return mIccProfile.second;
}

void JpegOptions::setIccProfile(const std::string &iccProfile)
{
    mIccProfile.second = iccProfile;
}

auto JpegOptions::comment() const -> std::string
{
    return mComment.second;
}

void JpegOptions::setComment(const std::string &comment)
{
    mComment.second = comment;
}

auto JpegOptions::exifThumbnail() const -> bool
{
    return bExifThumbnail.second;
}

void JpegOptions::setExifThumbnail(bool exifThumbnail)
{
    bExifThumbnail.second = exifThumbnail;
}

auto JpegOptions::thumbnailWidth() const -> int
{
    return mThumbnailWidth.second;
}

void JpegOptions::setThumbnailWidth(int thumbnailWidth)
{
    mThumbnailWidth.second = thumbnailWidth;
}

auto JpegOptions::thumbnailHeight() const -> int
{
    return mThumbnailHeight.second;
}

void JpegOptions::setThumbnailHeight(int thumbnailHeight)
{
    mThumbnailHeight.second = thumbnailHeight;
}

#ifdef TL_HAVE_GDAL
#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(2,3,0)
auto JpegOptions::writeExifMetadata() const -> bool
{
    return mWriteExifMetadata.second;
}

void JpegOptions::setWriteExifMetadata(bool writeExifMetadata)
{
    mWriteExifMetadata.second = writeExifMetadata;
}
#endif
#endif // TL_HAVE_GDAL

void JpegOptions::init()
{
    bWorldFile = std::make_pair(false, false);
    mQuality = std::make_pair(75, 75);
    bInternalMask = std::make_pair(true, true);
    mIccProfile = std::make_pair("", "");
    mComment = std::make_pair("", "");
    bExifThumbnail = std::make_pair(false, false);
    mThumbnailWidth = std::make_pair(128, 128);
    mThumbnailHeight = std::make_pair(128, 128);
    mWriteExifMetadata = std::make_pair(true, true);
}

auto JpegOptions::options(bool all) const -> std::map<std::string, std::string>
{
    std::map<std::string, std::string> options;

    if (all || bWorldFile.first != bWorldFile.second) {
        options["WORLDFILE"] = bWorldFile.second ? "YES" : "NO";
    }

    if (all || mQuality.first != mQuality.second) {
        options["QUALITY"] = std::to_string(mQuality.second);
    }

    if (all || bInternalMask.first != bInternalMask.second) {
        options["INTERNAL_MASK"] = bInternalMask.second ? "YES" : "NO";
    }

    if (all || mIccProfile.first != mIccProfile.second) {
        options["SOURCE_ICC_PROFILE"] = mIccProfile.second;
    }

    if (all || mComment.first != mComment.second) {
        options["COMMENT"] = mComment.second;
    }

    if (all || bExifThumbnail.first != bExifThumbnail.second) {
        options["EXIF_THUMBNAIL"] = bExifThumbnail.second ? "YES" : "NO";
    }

    if (all || mThumbnailWidth.first != mThumbnailWidth.second) {
        options["THUMBNAIL_WIDTH"] = std::to_string(mThumbnailWidth.second);
    }

    if (all || mThumbnailHeight.first != mThumbnailHeight.second) {
        options["THUMBNAIL_HEIGHT"] = std::to_string(mThumbnailHeight.second);
    }

    if (all || mWriteExifMetadata.first != mWriteExifMetadata.second) {
        options["WRITE_EXIF_METADATA"] = mWriteExifMetadata.second ? "YES" : "NO";
    }

    return options;
}



/* BmpOptions */

BmpOptions::BmpOptions()
  : ImageOptionsBase(Format::bmp)
{
    this->init();
}

BmpOptions::~BmpOptions() = default;

void BmpOptions::reset()
{
    this->init();
}

auto BmpOptions::isEnableWorldFile() const -> bool
{
    return bWorldFile.second;
}

void BmpOptions::setEnableWorldFile(bool enable)
{
    bWorldFile.second = enable;
}

void BmpOptions::init()
{
    bWorldFile = std::make_pair(false, false);
}

auto BmpOptions::options(bool all) const -> std::map<std::string, std::string>
{
    std::map<std::string, std::string> options;

    if (all || bWorldFile.first != bWorldFile.second) {
        options["WORLDFILE"] = bWorldFile.second ? "YES" : "NO";
    }

    return options;
}



/* GifOptions */

GifOptions::GifOptions()
  : ImageOptionsBase(Format::bmp)
{
    //mDataTypes = {DataType::TL_8U};
    this->init();
}

GifOptions::~GifOptions() = default;

void GifOptions::reset()
{
}

auto GifOptions::isEnableWorldFile() const -> bool
{
    return mWorldFile.second;
}

void GifOptions::setEnableWorldFile(bool enable)
{
    mWorldFile.second = enable;
}

auto GifOptions::interlacing() const -> bool
{
    return mInterlacing.second;
}

void GifOptions::setInterlacing(bool active)
{
    mInterlacing.second = active;
}

void GifOptions::init()
{
    mWorldFile = std::make_pair(false, false);
    mInterlacing = std::make_pair(false, false);
}

auto GifOptions::options(bool all) const -> std::map<std::string, std::string>
{
    std::map<std::string, std::string> options;

    if (all || mWorldFile.first != mWorldFile.second) {
        options["WORLDFILE"] = mWorldFile.second ? "YES" : "NO";
    }
    if (all || mInterlacing.first != mInterlacing.second) {
        options["INTERLACING"] = mInterlacing.second ? "ON" : "";
    }

    return options;
}

} // namespace tl
