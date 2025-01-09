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

#pragma once

#include "tidop/config.h"
#include "tidop/core/base/defs.h"

#include <map>

#ifdef TL_HAVE_GDAL
TL_DISABLE_WARNINGS
#include "gdal_priv.h"
TL_DEFAULT_WARNINGS
#endif // TL_HAVE_GDAL

#include "tidop/core/base/flags.h"


namespace tl
{

/*! \addtogroup RasterIO
 *  \{
 */

/*!
 * \brief Format options
 */
class TL_EXPORT ImageOptions
{

public:

    enum class Format
    {
        tiff,
        jpeg,
        jp2000,
        png,
        bmp,
        gif
    };

    using option_iterator = std::map<std::string, std::string>::iterator;
    using option_const_iterator = std::map<std::string, std::string>::const_iterator;

public:

    ImageOptions() = default;
    virtual ~ImageOptions() = default;

    virtual auto format() const -> Format = 0;

    virtual auto options() const -> std::map<std::string, std::string> = 0;
    virtual auto activeOptions() const -> std::map<std::string, std::string> = 0;
    virtual void reset() = 0;

};


class TL_EXPORT ImageOptionsBase
  : public ImageOptions
{

private:

    Format mFormat;

public:

    ImageOptionsBase(Format format);
    ~ImageOptionsBase() override;

    auto format() const -> Format override;
    auto options() const -> std::map<std::string, std::string> override;
    auto activeOptions() const -> std::map<std::string, std::string> override;

protected:

    virtual auto options(bool all) const -> std::map<std::string, std::string> = 0;

};


/*!
 * \brief TIFF format options
 */
class TL_EXPORT TiffOptions
  : public ImageOptionsBase
{

public:

    /*!
     * \brief BigTIFF properties
     */
    enum class BigTiff : uint8_t
    {
        yes,            /*!< Force BigTiff. */
        no,             /*!< Force normal TIFF. */
        if_needed,      /*!< BigTiff if needed. */
        if_safer        /*!< . */
    };

    /*!
     * \brief Compression type
     */
    enum class Compress : uint8_t
    {
        jpeg,
        lzw,
        packbits,
        deflate,
        ccittrle,
        ccittfax3,
        ccittfax4,
        lzma,
#ifdef TL_HAVE_GDAL
#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(2,3,0)
        zstd,
#endif
#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(2,4,0)
        lerc,
        lerc_deflate,
        lerc_zstd,
#endif
#endif // TL_HAVE_GDAL
        webp,
        none
    };

    enum class Photometric : uint8_t
    {
        minisblack,
        miniswhite,
        rgb,
        cmyk,
        ycbcr,
        cielab,
        icclab,
        itulab
    };

    enum class Alpha : uint8_t
    {
        yes,
        non_premultiplied,
        premultiplied,
        unspecified
    };

    enum class Profile : uint8_t
    {
        gdal_geotiff,
        geotiff,
        baseline
    };

    enum class PixelType : uint8_t
    {
        def, //default
        signedbyte
    };

    enum class GeotiffKeysFlavor : uint8_t
    {
        standard,
        esri_pe
    };

private:

    std::pair<bool, bool> bTFW;
    std::pair<bool, bool> bRPB;
    std::pair<bool, bool> bRPCTX;
    std::pair<bool, bool> bTiled;
    std::pair<int, int> mBlockXSize;
    std::pair<int, int> mBlockYSize;
    std::pair<int, int> mNBits;
    std::pair<uint8_t, uint8_t> mJpegQuality;
    std::pair<uint8_t, uint8_t> mZLevel;
    std::pair<BigTiff, BigTiff> mBigTiff;
    std::pair<Compress, Compress>  mCompress;
    std::pair<Photometric, Photometric> mPhotometric;
    std::pair<Alpha, Alpha> mAlpha;
    std::pair<Profile, Profile> mProfile;
    std::pair<PixelType, PixelType> mPixelType;
    std::pair<GeotiffKeysFlavor, GeotiffKeysFlavor> mGeotiffKeysFlavor;
    std::pair<bool, bool> mInternalMask;

public:

    TiffOptions();
    ~TiffOptions() override;

    void reset() override;

    auto isEnableTFW() const -> bool;
    void enableTFW(bool value = true);

    auto isEnableRPB() const -> bool;
    void enableRPB(bool value = true);

    auto isEnableRPCTX() const -> bool;
    void enableRPCTX(bool value = true);

    auto isEnableTiled() const -> bool;
    void enableTiled(bool value = true);

    auto blockXSize() const -> int;
    void setBlockXSize(int blockXSize);

    auto blockYSize() const -> int;
    void setBlockYSize(int blockYSize);

    /*!
     * \brief Number of bits
     */
    auto nBits() const -> int;
    void setNBits(int nBits);

    auto jpegQuality() const -> uint8_t;
    void setJpegQuality(uint8_t jpegQuality);

    auto zLevel() const -> uint8_t;
    void setZLevel(uint8_t zLevel);

    auto bigTiff() const -> BigTiff;
    void setBigTiff(BigTiff bigTiff);

    auto compress() const -> Compress;
    void setCompress(Compress compress);

    auto photometric() const -> Photometric;
    void setPhotometric(Photometric photometric);

    auto alpha() const -> Alpha;
    void setAlpha(Alpha alpha);

    auto profile() const -> Profile;
    void setProfile(Profile profile);

    auto pixelType() const -> PixelType;
    void setPixelType(PixelType pixelType);

    auto geotiffKeysFlavor() const -> GeotiffKeysFlavor;
    void setGeotiffKeysFlavor(GeotiffKeysFlavor geotiffKeysFlavor);

    auto internalMask() const -> bool;
    void setInternalMask(bool internalMask);

private:

    void init();
    auto options(bool all) const -> std::map<std::string, std::string> override;

};


/*!
 * \brief  Class that manages PNG format options
 */
class TL_EXPORT PngOptions
  : public ImageOptionsBase
{

private:

    std::pair<bool, bool> bWorldFile;
    std::pair<uint8_t, uint8_t> mZLevel;
#ifdef TL_HAVE_GDAL
#if GDAL_VERSION_MAJOR >= 2
    std::pair<std::string, std::string> mTitle;
    std::pair<std::string, std::string> mDescription;
    std::pair<std::string, std::string> mCopyright;
    std::pair<std::string, std::string> mComment;
#endif
#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(2,1,0)
    std::pair<int, int> mNBits;
#endif
#endif // TL_HAVE_GDAL

public:

    PngOptions();
    ~PngOptions() override;

    void reset() override;

    auto isEnableWorldFile() const -> bool;
    void setEnableWorldFile(bool enable);

    auto zLevel() const -> uint8_t;
    void setZLevel(uint8_t zLevel);

#ifdef TL_HAVE_GDAL
#if GDAL_VERSION_MAJOR >= 2

    auto title() const -> std::string;
    void setTitle(const std::string &title);

    auto description() const -> std::string;
    void setDescription(const std::string &description);

    auto copyright() const -> std::string;
    void setCopyright(const std::string &copyright);

    auto comment() const -> std::string;
    void setComment(const std::string &comment);

#endif

#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(2,1,0)

    auto nBits() const -> int;
    void setNBits(int nBits);

#endif
#endif // TL_HAVE_GDAL

private:

    void init();
    auto options(bool all) const -> std::map<std::string, std::string> override;

};



/*!
 * \brief Class that manages the Jpeg format options
 */
class TL_EXPORT JpegOptions
    : public ImageOptionsBase
{

private:

    std::pair<bool, bool> bWorldFile;
    std::pair<uint8_t, uint8_t> mQuality;
    std::pair<bool, bool> bInternalMask;
    std::pair<std::string, std::string> mIccProfile;
    std::pair<std::string, std::string> mComment;
    std::pair<bool, bool> bExifThumbnail;
    std::pair<int, int> mThumbnailWidth;
    std::pair<int, int> mThumbnailHeight;
    std::pair<bool, bool> mWriteExifMetadata;

public:

    JpegOptions();
    ~JpegOptions() override;

    void reset() override;

    bool isEnableWorldFile() const;
    void setEnableWorldFile(bool enable);

    uint8_t quality() const;
    void setQuality(uint8_t quality);

    bool internalMask() const;
    void setInternalMask(bool internalMask);

    //ICC profile encoded in Base64.
    std::string iccProfile() const;
    void setIccProfile(const std::string &iccProfile);

    std::string comment() const;
    void setComment(const std::string &comment);

    //Whether to generate an EXIF thumbnail(overview), itself JPEG 
    //compressed. Defaults to NO. If enabled, the maximum dimension 
    //of the thumbnail will be 128, if neither THUMBNAIL_WIDTH nor 
    //THUMBNAIL_HEIGHT are specified.
    bool exifThumbnail() const;
    void setExifThumbnail(bool exifThumbnail);

    //THUMBNAIL_WIDTH=n: Width of thumbnail. Only taken into account if EXIF_THUMBNAIL=YES.
    int thumbnailWidth() const;
    void setThumbnailWidth(int thumbnailWidth);

    //THUMBNAIL_HEIGHT=n: Height of thumbnail. Only taken into account if EXIF_THUMBNAIL=YES.
    int thumbnailHeight() const;
    void setThumbnailHeight(int thumbnailHeight);

    //(Starting with GDAL 2.3). 
    //Whether to write EXIF_xxxx metadata items in a EXIF segment. 
    //Default to YES.

#ifdef TL_HAVE_GDAL
#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(2,3,0)
    bool writeExifMetadata() const;
    void setWriteExifMetadata(bool writeExifMetadata);
#endif
#endif // TL_HAVE_GDAL

private:

    void init();
    std::map<std::string, std::string> options(bool all) const override;

};



/*!
 * \brief Class that manages the bmp format options.
 */
class TL_EXPORT BmpOptions
    : public ImageOptionsBase
{

private:

    std::pair<bool, bool> bWorldFile;

public:

    BmpOptions();
    ~BmpOptions() override;

    void reset() override;

    bool isEnableWorldFile() const;
    void setEnableWorldFile(bool enable);

private:

    void init();
    std::map<std::string, std::string> options(bool all) const override;

};


/*!
 * \brief Class handling the gif format options
 */
class TL_EXPORT GifOptions
  : public ImageOptionsBase
{

private:

    std::pair<bool, bool> mWorldFile;
    std::pair<bool, bool> mInterlacing;

public:

    GifOptions();
    ~GifOptions() override;

    void reset() override;

    bool isEnableWorldFile() const;
    void setEnableWorldFile(bool enable);
    bool interlacing() const;
    void setInterlacing(bool active);

private:

    void init();
    std::map<std::string, std::string> options(bool all) const override;

};


/*! \} */


} // namespace tl
