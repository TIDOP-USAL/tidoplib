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
  * \brief Abstract base class for image format options.
  *
  * The `ImageOptions` class provides an interface for managing options related to different image formats.
  * It allows accessing and resetting options and retrieving both all and active options.
  */
class TL_EXPORT ImageOptions
{

public:

    /*!
     * \brief Enum representing supported image formats.
     */
    enum class Format
    {
        tiff,   /*!< TIFF format */
        jpeg,   /*!< JPEG format */
        jp2000, /*!< JPEG 2000 format */
        png,    /*!< PNG format */
        bmp,    /*!< BMP format */
        gif     /*!< GIF format */
    };

    using option_iterator = std::map<std::string, std::string>::iterator;
    using option_const_iterator = std::map<std::string, std::string>::const_iterator;

public:

    /*!
     * \brief Default constructor.
     */
    ImageOptions() = default;

    virtual ~ImageOptions() = default;

    /*!
     * \brief Returns the format of the image.
     * \return Image format.
     */
    virtual auto format() const -> Format = 0;

    /*!
     * \brief Retrieves all options for the image format.
     * \return Map of option names and values.
     */
    virtual auto options() const -> std::map<std::string, std::string> = 0;

    /*!
     * \brief Retrieves only the active options for the image format.
     * \return Map of active option names and values.
     */
    virtual auto activeOptions() const -> std::map<std::string, std::string> = 0;

    /*!
     * \brief Resets all options to their default values.
     */
    virtual void reset() = 0;

};


/*!
 * \brief Base class for image format options with a specific format.
 *
 * The `ImageOptionsBase` class extends `ImageOptions` to handle options for a specific image format.
 * It provides concrete implementations for common functionality and leaves the retrieval of options
 * (all or active) to be implemented by derived classes.
 */
class TL_EXPORT ImageOptionsBase
  : public ImageOptions
{

private:

    Format mFormat;

public:

    /*!
     * \brief Constructor to initialize options with a specific format.
     * \param[in] format Format of the image.
     */
    ImageOptionsBase(Format format);
    ~ImageOptionsBase() override;

    /*!
     * \brief Returns the format of the image.
     * \return Image format.
     */
    auto format() const -> Format override;

    /*!
     * \brief Retrieves all options for the image format.
     * \return Map of option names and values.
     */
    auto options() const -> std::map<std::string, std::string> override;

    /*!
     * \brief Retrieves only the active options for the image format.
     * \return Map of active option names and values.
     */
    auto activeOptions() const -> std::map<std::string, std::string> override;

protected:

    /*!
     * \brief Retrieves options for the image format.
     * \param[in] all If true, retrieves all options; otherwise, retrieves only active options.
     * \return Map of option names and values.
     */
    virtual auto options(bool all) const -> std::map<std::string, std::string> = 0;

};



/*!
 * \brief Options for TIFF format images.
 *
 * The `TiffOptions` class provides configurable options for saving images in the TIFF format.
 * It includes various settings such as compression type, photometric interpretation, tile size,
 * and additional TIFF-specific properties.
 */
class TL_EXPORT TiffOptions
  : public ImageOptionsBase
{

public:

    /*!
     * \brief Enum for BigTIFF options.
     *
     * Specifies whether to use BigTIFF based on the file size or other criteria.
     */
    enum class BigTiff : uint8_t
    {
        yes,            /*!< Always use BigTIFF format. */
        no,             /*!< Never use BigTIFF format. */
        if_needed,      /*!< Use BigTIFF only if necessary. */
        if_safer        /*!< Use BigTIFF if it is safer for the file size. */
    };

    /*!
     * \brief Enum for compression types.
     *
     * Specifies the compression method to be used for the TIFF file.
     */
    enum class Compress : uint8_t
    {
        jpeg,          /*!< JPEG compression. */
        lzw,           /*!< LZW compression. */
        packbits,      /*!< PackBits compression. */
        deflate,       /*!< Deflate compression. */
        ccittrle,      /*!< CCITT RLE compression. */
        ccittfax3,     /*!< CCITT Group 3 fax compression. */
        ccittfax4,     /*!< CCITT Group 4 fax compression. */
        lzma,          /*!< LZMA compression. */
/// \cond
#ifdef TL_HAVE_GDAL
#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(2,3,0)
/// \endcond
        zstd,          /*!< ZSTD compression (requires GDAL 2.3+). */
/// \cond
#endif
#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(2,4,0)
/// \endcond
        lerc,          /*!< LERC compression (requires GDAL 2.4+). */
        lerc_deflate,  /*!< LERC with deflate compression. */
        lerc_zstd,     /*!< LERC with ZSTD compression. */
/// \cond
#endif
#endif // TL_HAVE_GDAL
/// \endcond
        webp,          /*!< WebP compression. */
        none           /*!< No compression. */
    };

    /*!
     * \brief Enum for photometric interpretation.
     *
     * Specifies the color space and how pixel data is stored in the image.
     * The default value is `minisblack`, but for input images with 3 or 4 bands
     * of Byte type, the default will be `rgb`. This default can be overridden using
     * this enumeration.
     */
    enum class Photometric : uint8_t
    {
        /*!
         * \brief Monochrome: black is zero.
         *
         * This is the default for single-band images, where darker values represent
         * lower intensity (closer to black).
         */
        minisblack,

        /*!
         * \brief Monochrome: white is zero.
         *
         * An inverse of `MINISBLACK`, where darker values represent higher intensity
         * (closer to white).
         */
        miniswhite,

        /*!
         * \brief RGB color model.
         *
         * Used for standard color images. This becomes the default for images with
         * 3 or 4 bands of Byte type.
         */
        rgb,

        /*!
         * \brief CMYK color model.
         *
         * Used for images in the CMYK color space, commonly used in printing.
         */
        cmyk,

        /*!
         * \brief YCbCr color model.
         *
         * Commonly used in video and image compression formats, separating
         * the luminance (Y) from the chrominance (Cb and Cr).
         */
        ycbcr,

        /*!
         * \brief CIE L*a*b* color model.
         *
         * A color space that represents colors more closely to human perception,
         * used for high-fidelity color applications.
         */
        cielab,

        /*!
         * \brief ICC L*a*b* color model.
         *
         * A variant of the CIE L*a*b* color space, defined by the International
         * Color Consortium (ICC).
         */
        icclab,

        /*!
         * \brief ITU L*a*b* color model.
         *
         * Defined by the International Telecommunication Union (ITU) for specific
         * broadcast and telecommunication uses.
         */
        itulab
    };

    /*!
     * \brief Enum for alpha channel interpretation.
     *
     * Specifies how the alpha (transparency) channel is treated.
     */
    enum class Alpha : uint8_t
    {
        yes,                 /*!< Alpha channel is present. */
        non_premultiplied,   /*!< Non-premultiplied alpha. */
        premultiplied,       /*!< Premultiplied alpha. */
        unspecified          /*!< Alpha channel is unspecified. */
    };

    /*!
     * \brief Enum for specifying the profile type used in a TIFF image.
     *
     * Controls what non-baseline tags are emitted by GDAL when writing TIFF files.
     */
    enum class Profile : uint8_t
    {
        /*!
         * \brief GDAL GeoTIFF profile.
         *
         * This is the default option. Various GDAL custom tags may be written in addition to
         * the standard GeoTIFF tags, providing extended functionality and metadata specific
         * to GDAL.
         */
        gdal_geotiff,

        /*!
         * \brief GeoTIFF profile.
         *
         * Only standard GeoTIFF tags are added to the baseline. This profile ensures compatibility
         * with software that expects only standard GeoTIFF metadata without any additional custom tags.
         */
        geotiff,

        /*!
         * \brief Baseline profile.
         *
         * No GDAL or GeoTIFF tags are written. This option is useful for creating files that
         * need to be read by applications that are intolerant of unrecognized tags, ensuring
         * maximum compatibility.
         */
        baseline
    };

    /*!
     * \brief Enum for specifying the pixel type in a TIFF image.
     *
     * Determines how pixel data is interpreted when creating or writing TIFF files.
     */
    enum class PixelType : uint8_t
    {
        /*!
         * \brief Default pixel type.
         *
         * Uses the default pixel type for the image data.
         */
        def,

        /*!
         * \brief Signed byte pixel type.
         *
         * Forces the image data to be written as signed byte. This option is deprecated
         * starting with GDAL 3.7, where `Int8` should be used instead for similar functionality.
         */
        signedbyte
    };

    /*!
     * \brief Enum for GeoTIFF keys flavor.
     *
     * Specifies the flavor of GeoTIFF keys used for writing spatial reference system (SRS) information.
     */
    enum class GeotiffKeysFlavor : uint8_t
    {
        /*!
         * \brief Standard GeoTIFF keys.
         *
         * Uses the generally accepted formulations of GeoTIFF keys, including
         * extensions for new EPSG codes. This is the default choice.
         */
        standard,

        /*!
         * \brief ESRI PE GeoTIFF keys.
         *
         * Writes GeoTIFF keys in a way that is more compatible with ArcGIS,
         * particularly affecting the EPSG:3857 (Web Mercator) spatial reference system.
         * For other SRS, the standard keys are used, with an additional ESRI PE WKT string
         * as the value of the `PCSCitationGeoKey`.
         */
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

    /*!
     * \brief Reset all TIFF options to their default values.
     */
    void reset() override;

    /*!
     * \brief Check if the creation of an associated ESRI world file (.tfw) is enabled.
     * \return True if the creation of the .tfw file is enabled, false otherwise.
     */
    auto isEnableTFW() const -> bool;

    /*!
     * \brief Enable or disable the creation of an associated ESRI world file (.tfw).
     * \param value True to enable the creation of the .tfw file, false to disable it.
     */
    void enableTFW(bool value = true);

    /*!
     * \brief Check if the creation of an associated .RPB file for RPC (Rational Polynomial Coefficients) is enabled.
     * \return True if the creation of the .RPB file is enabled, false otherwise.
     */
    auto isEnableRPB() const -> bool;

    /*!
     * \brief Enable or disable the creation of an associated .RPB file for RPC (Rational Polynomial Coefficients).
     * \param value True to enable the creation of the .RPB file, false to disable it.
     * \note If not specified, the .RPB file is automatically generated if RPC information is available and the profile is not the default GDALGeoTIFF.
     */
    void enableRPB(bool value = true);

    /*!
     * \brief Check if the creation of an associated _RPC.TXT file for RPC (Rational Polynomial Coefficients) is enabled.
     * \return True if the creation of the _RPC.TXT file is enabled, false otherwise.
     */
    auto isEnableRPCTX() const -> bool;

    /*!
     * \brief Enable or disable the creation of an associated _RPC.TXT file for RPC (Rational Polynomial Coefficients).
     * \param value True to enable the creation of the _RPC.TXT file, false to disable it.
     */
    void enableRPCTX(bool value = true);

    /*!
     * \brief Check if the creation of tiled TIFF files is enabled.
     * \return True if tiled TIFF creation is enabled, false otherwise.
     * \note Defaults to NO, creating striped TIFF files by default.
     */
    auto isEnableTiled() const -> bool;

    /*!
     * \brief Enable or disable the creation of tiled TIFF files.
     * \param value True to enable tiled TIFF creation, false to create striped TIFF files.
     * \note Defaults to NO. This option can be used to force the creation of tiled TIFF files.
     */
    void enableTiled(bool value = true);

    /*!
     * \brief Get the tile width (BLOCKXSIZE).
     * \return Tile width in pixels.
     * \note Defaults to 256. Must be divisible by 16 when TILED=YES.
     */
    auto blockXSize() const -> int;

    /*!
     * \brief Set the tile width (BLOCKXSIZE).
     * \param blockXSize Tile width in pixels.
     * \note Defaults to 256. Must be divisible by 16 when TILED=YES.
     */
    void setBlockXSize(int blockXSize);

    /*!
     * \brief Get the tile or strip height (BLOCKYSIZE).
     * \return Tile or strip height in pixels.
     * \note Defaults to 256 for tiles. For strips, defaults to a value such that one strip is 8K or less. Must be divisible by 16 when TILED=YES.
     */
    auto blockYSize() const -> int;

    /*!
     * \brief Set the tile or strip height (BLOCKYSIZE).
     * \param blockYSize Tile or strip height in pixels.
     * \note Must be divisible by 16 when TILED=YES.
     */
    void setBlockYSize(int blockYSize);

    /*!
     * \brief Get the number of bits per sample (NBITS).
     * \return Number of bits per sample.
     * \note Supports values from 1 to 31. For values less than 8, the pixel type should be Byte. Values from 9 to 15 correspond to UInt16, and 17 to 31 to UInt32. From GDAL 2.2, 16 is accepted for Float32 to generate half-precision floating point values.
     */
    auto nBits() const -> int;

    /*!
     * \brief Set the number of bits per sample (NBITS).
     * \param nBits Number of bits per sample.
     * \note Supports values from 1 to 31 with restrictions on the pixel type. See detailed notes for valid ranges and types.
     */
    void setNBits(int nBits);

    /*!
     * \brief Get the JPEG quality setting.
     * \return JPEG quality value (1-100).
     * \note Defaults to 75. Higher values result in better image quality but larger file sizes, while lower values produce higher compression ratios at the cost of image quality, leading to visible blocking artifacts. Values above 95 do not significantly improve quality but can substantially increase file size.
     */
    auto jpegQuality() const->uint8_t;

    /*!
     * \brief Set the JPEG quality setting.
     * \param jpegQuality JPEG quality value (1-100).
     * \note Low values lead to higher compression with visible artifacts, while higher values improve quality but increase file size. Defaults to 75.
     */
    void setJpegQuality(uint8_t jpegQuality);

    /*!
     * \brief Get the compression level for DEFLATE compression.
     * \return Compression level (1-9 or 1-12).
     * \note Defaults to 6. Higher values provide better compression but at a slower speed. A value of 9 (or 12 with `libdeflate`) is the best but slowest compression, while a value of 1 is the fastest but provides the least compression.
     */
    auto zLevel() const -> uint8_t;

    /*!
     * \brief Set the compression level for DEFLATE compression.
     * \param zLevel Compression level (1-9 or 1-12).
     * \note A value of 1 results in faster compression with less reduction in file size, while higher values (up to 9 with zlib, or up to 12 with libdeflate) result in slower compression with better compression ratios.
     */
    void setZLevel(uint8_t zLevel);

    /*!
     * \brief Get the BigTIFF option.
     * \return The current BigTIFF option.
     * \see BigTiff
     */
    auto bigTiff() const->BigTiff;

    /*!
     * \brief Set the BigTIFF option.
     * \param bigTiff The BigTIFF option.
     * \see BigTiff
     */
    void setBigTiff(BigTiff bigTiff);

    /*!
     * \brief Get the compression method.
     * \return The current compression method.
     * \see Compress
     */
    auto compress() const -> Compress;

    /*!
     * \brief Set the compression method.
     * \param compress The compression method.
     * \see Compress
     */
    void setCompress(Compress compress);

    /*!
     * \brief Get the photometric interpretation.
     * \return The current photometric interpretation.
     * \see Photometric
     */
    auto photometric() const -> Photometric;

    /*!
     * \brief Set the photometric interpretation.
     * \param photometric The photometric interpretation.
     * \see Photometric
     */
    void setPhotometric(Photometric photometric);

    /*!
 * \brief Get the alpha channel handling option.
 * \return The current alpha option.
 */
    auto alpha() const -> Alpha;

    /*!
     * \brief Set the alpha channel handling option.
     * \param alpha The alpha option.
     */
    void setAlpha(Alpha alpha);

    /*!
     * \brief Get the profile for TIFF tags.
     * \return The current profile.
     * \see Profile
     */
    auto profile() const -> Profile;

    /*!
     * \brief Set the profile for TIFF tags.
     * \param profile The profile to use.
     * \see Profile
     */
    void setProfile(Profile profile);

    /*!
     * \brief Get the pixel type setting.
     * \return The current pixel type setting.
     * \see PixelType
     */
    auto pixelType() const -> PixelType;

    /*!
     * \brief Set the pixel type setting.
     * \param pixelType The pixel type setting.
     * \see PixelType
     */
    void setPixelType(PixelType pixelType);

    /*!
     * \brief Get the flavor of GeoTIFF keys.
     * \return The current GeoTIFF keys flavor.
     * \see GeotiffKeysFlavor
     */
    auto geotiffKeysFlavor() const -> GeotiffKeysFlavor;

    /*!
     * \brief Set the flavor of GeoTIFF keys.
     * \param geotiffKeysFlavor The GeoTIFF keys flavor.
     * \see GeotiffKeysFlavor
     */
    void setGeotiffKeysFlavor(GeotiffKeysFlavor geotiffKeysFlavor);

    /*!
     * \brief Check if internal mask creation is enabled.
     * \return True if internal mask is enabled, false otherwise.
     */
    auto internalMask() const -> bool;

    /*!
     * \brief Enable or disable internal mask creation.
     * \param internalMask True to enable, false to disable.
     */
    void setInternalMask(bool internalMask);

private:

    void init();
    auto options(bool all) const -> std::map<std::string, std::string> override;

};



class TL_EXPORT PngOptions
  : public ImageOptionsBase
{

private:

    std::pair<bool, bool> bWorldFile;
    std::pair<uint8_t, uint8_t> mZLevel;
    std::pair<std::string, std::string> mTitle;
    std::pair<std::string, std::string> mDescription;
    std::pair<std::string, std::string> mCopyright;
    std::pair<std::string, std::string> mComment;
#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(2,1,0)
    std::pair<int, int> mNBits;
#endif

public:

    PngOptions();
    ~PngOptions() override;

    void reset() override;

    /*!
     * \brief Check if the generation of an associated ESRI world file is enabled.
     * \return True if enabled, false otherwise.
     */
    auto isEnableWorldFile() const -> bool;

    /*!
     * \brief Enable or disable the generation of an associated ESRI world file.
     * \param enable True to enable, false to disable.
     */
    void setEnableWorldFile(bool enable);

    /*!
     * \brief Get the compression level for the PNG file.
     * \return The compression level.
     */
    auto zLevel() const -> uint8_t;

    /*!
     * \brief Set the compression level for the PNG file.
     * \param zLevel The compression level to set.
     */
    void setZLevel(uint8_t zLevel);

    /*!
     * \brief Get the title metadata for the PNG file.
     * \return The title.
     */
    auto title() const->std::string;

    /*!
     * \brief Set the title metadata for the PNG file.
     * \param title The title to set.
     */
    void setTitle(const std::string &title);

    /*!
     * \brief Get the description metadata for the PNG file.
     * \return The description.
     */
    auto description() const -> std::string;

    /*!
     * \brief Set the description metadata for the PNG file.
     * \param description The description to set.
     */
    void setDescription(const std::string &description);

    /*!
     * \brief Get the copyright metadata for the PNG file.
     * \return The copyright.
     */
    auto copyright() const -> std::string;

    /*!
     * \brief Set the copyright metadata for the PNG file.
     * \param copyright The copyright to set.
     */
    void setCopyright(const std::string &copyright);

    /*!
     * \brief Get the comment metadata for the PNG file.
     * \return The comment.
     */
    auto comment() const -> std::string;

    /*!
     * \brief Set the comment metadata for the PNG file.
     * \param comment The comment to set.
     */
    void setComment(const std::string &comment);

/// \cond
#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(2,1,0)
/// \endcond

    /*!
     * \brief Get the number of bits per sample in the PNG file (requires GDAL 2.1+).
     * \return The number of bits.
     */
    auto nBits() const -> int;

    /*!
     * \brief Set the number of bits per sample in the PNG file (requires GDAL 2.1+).
     * \param nBits The number of bits to set.
     */
    void setNBits(int nBits);

/// \cond
#endif
/// \endcond

private:

    void init();
    auto options(bool all) const -> std::map<std::string, std::string> override;

};



/*!
 * \brief Class that manages the JPEG format options.
 *
 * This class provides methods to configure various options for creating JPEG images,
 * including quality, internal mask, ICC profile, EXIF thumbnail, and metadata settings.
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

    /*!
     * \brief Constructor.
     */
    JpegOptions();

    /*!
     * \brief Destructor.
     */
    ~JpegOptions() override;

    /*!
     * \brief Reset all options to their default values.
     */
    void reset() override;

    /*!
     * \brief Check if the generation of an associated ESRI world file is enabled.
     * \return True if enabled, false otherwise.
     */
    auto isEnableWorldFile() const -> bool;

    /*!
     * \brief Enable or disable the generation of an associated ESRI world file.
     * \param enable True to enable, false to disable.
     */
    void setEnableWorldFile(bool enable);

    /*!
     * \brief Get the JPEG quality setting.
     * \return Quality level (1-100). Defaults to 75.
     */
    auto quality() const -> uint8_t;

    /*!
     * \brief Set the JPEG quality.
     * \param quality Quality level (1-100). Higher values provide better quality but larger file sizes.
     */
    void setQuality(uint8_t quality);

    /*!
     * \brief Check if an internal mask is enabled.
     * \return True if enabled, false otherwise.
     */
    auto internalMask() const -> bool;

    /*!
     * \brief Enable or disable the writing of an internal mask.
     * \param internalMask True to enable, false to disable.
     */
    void setInternalMask(bool internalMask);

    /*!
     * \brief Get the ICC profile.
     * \return ICC profile encoded in Base64.
     */
    auto iccProfile() const -> std::string;

    /*!
     * \brief Set the ICC profile.
     * \param iccProfile ICC profile encoded in Base64.
     */
    void setIccProfile(const std::string &iccProfile);

    /*!
     * \brief Get the comment embedded in the JPEG.
     * \return Comment string.
     */
    auto comment() const -> std::string;

    /*!
     * \brief Set the comment to embed in the JPEG.
     * \param comment Comment string.
     */
    void setComment(const std::string &comment);

    /*!
     * \brief Check if EXIF thumbnail generation is enabled.
     * \return True if enabled, false otherwise.
     */
    auto exifThumbnail() const -> bool;

    /*!
     * \brief Enable or disable the generation of an EXIF thumbnail.
     * \param exifThumbnail True to enable, false to disable.
     */
    void setExifThumbnail(bool exifThumbnail);

    /*!
     * \brief Get the width of the EXIF thumbnail.
     * \return Thumbnail width.
     */
    auto thumbnailWidth() const -> int;

    /*!
     * \brief Set the width of the EXIF thumbnail.
     * \param thumbnailWidth Thumbnail width.
     */
    void setThumbnailWidth(int thumbnailWidth);

    /*!
     * \brief Get the height of the EXIF thumbnail.
     * \return Thumbnail height.
     */
    auto thumbnailHeight() const -> int;

    /*!
     * \brief Set the height of the EXIF thumbnail.
     * \param thumbnailHeight Thumbnail height.
     */
    void setThumbnailHeight(int thumbnailHeight);

    //(Starting with GDAL 2.3). 
    //Whether to write EXIF_xxxx metadata items in a EXIF segment. 
    //Default to YES.

/// \cond
#ifdef TL_HAVE_GDAL
#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(2,3,0)
/// \endcond
    /*!
     * \brief Check if writing EXIF metadata is enabled. (requires GDAL 2.3+)
     * \return True if enabled, false otherwise.
     */
    auto writeExifMetadata() const -> bool;

    /*!
     * \brief Enable or disable writing EXIF metadata. (requires GDAL 2.3+)
     * \param writeExifMetadata True to enable, false to disable.
     */
    void setWriteExifMetadata(bool writeExifMetadata);
/// \cond
#endif
#endif // TL_HAVE_GDAL
/// \endcond

private:

    void init();
    std::map<std::string, std::string> options(bool all) const override;

};



/*!
 * \brief Class that manages the BMP format options.
 *
 * This class provides methods to configure options for creating BMP images,
 * including the option to generate an associated ESRI world file.
 */
class TL_EXPORT BmpOptions
    : public ImageOptionsBase
{

private:

    std::pair<bool, bool> bWorldFile;

public:

    /*!
     * \brief Constructor.
     */
    BmpOptions();

    /*!
     * \brief Destructor.
     */
    ~BmpOptions() override;

    /*!
     * \brief Reset all options to their default values.
     */
    void reset() override;

    /*!
     * \brief Check if the generation of an associated ESRI world file is enabled.
     * \return True if enabled, false otherwise.
     */
    auto isEnableWorldFile() const -> bool;

    /*!
     * \brief Enable or disable the generation of an associated ESRI world file.
     * \param enable True to enable, false to disable.
     */
    void setEnableWorldFile(bool enable);

private:

    void init();
    std::map<std::string, std::string> options(bool all) const override;

};


/*!
 * \brief Class handling the GIF format options.
 *
 * This class provides methods to configure options for creating GIF images,
 * including options for generating an associated ESRI world file and enabling interlacing.
 */
class TL_EXPORT GifOptions
  : public ImageOptionsBase
{

private:

    std::pair<bool, bool> mWorldFile;
    std::pair<bool, bool> mInterlacing;

public:

    /*!
     * \brief Constructor.
     */
    GifOptions();

    /*!
     * \brief Destructor.
     */
    ~GifOptions() override;

    /*!
     * \brief Reset all options to their default values.
     */
    void reset() override;

    /*!
     * \brief Check if the generation of an associated ESRI world file is enabled.
     * \return True if enabled, false otherwise.
     */
    auto isEnableWorldFile() const -> bool;

    /*!
     * \brief Enable or disable the generation of an associated ESRI world file.
     * \param enable True to enable, false to disable.
     */
    void setEnableWorldFile(bool enable);

    /*!
     * \brief Check if interlacing is enabled for the GIF image.
     * \return True if interlacing is enabled, false otherwise.
     */
    auto interlacing() const -> bool;

    /*!
     * \brief Enable or disable interlacing for the GIF image.
     * \param active True to enable interlacing, false to disable.
     */
    void setInterlacing(bool active);

private:

    void init();
    std::map<std::string, std::string> options(bool all) const override;

};


/*! \} */


} // namespace tl
