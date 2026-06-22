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

#include <map>
#include <optional>
#include <string>
#include <string_view>


namespace tl
{

/*! \addtogroup RasterIO
 *  \{
 */

namespace tiff
{
/*!
 * \brief ESRI world file (.tfw) creation.
 * \note Supported values: "YES", "NO".
 */
constexpr std::string_view TFW = "TFW";

/*!
 * \brief .RPB file for RPC coefficients.
 * \note Supported values: "YES", "NO".
 */
constexpr std::string_view RPB = "RPB";

/*!
 * \brief _RPC.TXT file for RPC coefficients.
 * \note Supported values: "YES", "NO".
 */
constexpr std::string_view RPCTX = "RPCTX";

/*!
 * \brief Enable tiled TIFF instead of striped.
 * \note Supported values: "YES", "NO". Default is "NO" (striped).
 */
constexpr std::string_view Tiled = "TILED";

/*!
 * \brief Tile width (or strip height) in pixels.
 * \note Integer value. Default is 256. Must be divisible by 16 when TILED="YES".
 */
constexpr std::string_view BlockXSize = "BLOCKXSIZE";

/*!
 * \brief Tile height (or strip height) in pixels.
 * \note Integer value. Default is 256 for tiles; for strips it is auto‑computed. Must be divisible by 16 when TILED="YES".
 */
constexpr std::string_view BlockYSize = "BLOCKYSIZE";

/*!
 * \brief Number of bits per sample (per band).
 * \note Integer between 1 and 31. The pixel type is deduced from the range.
 */
constexpr std::string_view NBits = "NBITS";

/*!
 * \brief JPEG quality when COMPRESS="JPEG".
 * \note Integer between 1 and 100. Default is 75.
 */
constexpr std::string_view JpegQuality = "JPEG_QUALITY";

/*!
 * \brief Compression level for DEFLATE or ZSTD.
 * \note Integer. For DEFLATE: 1–9 (or 1–12 with libdeflate). Default is 6.
 */
constexpr std::string_view ZLevel = "ZLEVEL";

/*!
 * \brief BigTIFF usage (for files larger than 4 GB).
 * \note Supported values: "YES", "NO", "IF_NEEDED", "IF_SAFER". Default is "IF_SAFER".
 */
constexpr std::string_view BigTiff = "BigTiff";

/*!
 * \brief Compression method.
 * \note Supported values: "JPEG", "LZW", "PACKBITS", "DEFLATE", "CCITTRLE",
 *       "CCITTFAX3", "CCITTFAX4", "LZMA", "ZSTD" (GDAL≥2.3), "LERC" (GDAL≥2.4),
 *       "LERC_DEFLATE" (GDAL≥2.4), "LERC_ZSTD" (GDAL≥2.4), "WEBP", "NONE".
 */
constexpr std::string_view Compress = "COMPRESS";

/*!
 * \brief Photometric interpretation (color space).
 * \note Supported values: "MINISBLACK", "MINISWHITE", "RGB", "CMYK", "YCBCR",
 *       "CIELAB", "ICCLAB", "ITULAB".
 */
constexpr std::string_view Photometric = "PHOTOMETRIC";

/*!
 * \brief Alpha channel handling.
 * \note Supported values: "YES", "NON-PREMULTIPLIED", "PREMULTIPLIED", "UNSPECIFIED".
 */
constexpr std::string_view Alpha = "ALPHA";

/*!
 * \brief Profile for TIFF tags (control of non‑baseline extensions).
 * \note Supported values: "GDALGeoTIFF" (default), "GeoTIFF", "BASELINE".
 */
constexpr std::string_view Profile = "PROFILE";

/*!
 * \brief Forced pixel type.
 * \note Supported values: "DEFAULT", "SIGNEDBYTE" (deprecated since GDAL 3.7, use Int8).
 */
constexpr std::string_view PixelType = "PIXELTYPE";

/*!
 * \brief Flavor of GeoTIFF keys for spatial reference.
 * \note Supported values: "STANDARD" (default), "ESRI_PE" (ArcGIS compatibility).
 */
constexpr std::string_view GeotiffKeysFlavor = "GEOTIFF_KEYS_FLAVOR";

/*!
 * \brief Create internal mask for transparency data.
 * \note Supported values: "YES", "NO". Default is "NO".
 */
constexpr std::string_view InternalMask = "GDAL_TIFF_INTERNAL_MASK";

} // namespace tiff



namespace png
{

/*!
 * \brief Enable or disable generation of an associated ESRI world file.
 * \note Supported values: "YES", "NO".
 */
constexpr std::string_view WorldFile = "WORLDFILE";

/*!
 * \brief Compression level for the PNG file (zlib compression).
 * \note Integer value. Typical range 1–9, where 1 is fastest (least compression)
 *       and 9 is slowest (best compression). Default is 6.
 */
constexpr std::string_view ZLevel = "ZLEVEL";

/*!
 * \brief Title text stored in the PNG file (tEXt chunk).
 * \note Free text string. Supported since GDAL 2.0.
 */
constexpr std::string_view Title = "TITLE";

/*!
 * \brief Description text stored in the PNG file (tEXt chunk).
 * \note Free text string. Supported since GDAL 2.0.
 */
constexpr std::string_view Description = "DESCRIPTION";

/*!
 * \brief Copyright notice stored in the PNG file (tEXt chunk).
 * \note Free text string. Supported since GDAL 2.0.
 */
constexpr std::string_view Copyright = "COPYRIGHT";

/*!
 * \brief Comment text stored in the PNG file (tEXt chunk).
 * \note Free text string. Supported since GDAL 2.0.
 */
constexpr std::string_view Comment = "COMMENT";

/*!
 * \brief Number of bits per sample (bit depth).
 * \note Integer value. Supported values depend on the pixel type and color type.
 *       Typical values: 1, 2, 4, 8, 16. Requires GDAL 2.1 or later.
 */
constexpr std::string_view NBits = "NBITS";

} // namespace png

namespace jpeg
{

/*!
 * \brief Enable or disable generation of an associated ESRI world file.
 * \note Supported values: "YES", "NO".
 */
constexpr std::string_view WorldFile = "WORLDFILE";

/*!
 * \brief JPEG compression quality.
 * \note Integer value from 1 to 100. Higher values give better quality but larger file size. Default is 75.
 */
constexpr std::string_view Quality = "QUALITY";

/*!
 * \brief Enable or disable writing an internal mask (for transparency).
 * \note Supported values: "YES", "NO".
 */
constexpr std::string_view InternalMask = "INTERNAL_MASK";

/*!
 * \brief ICC profile embedded in the JPEG, encoded as Base64.
 * \note String value representing the profile.
 */
constexpr std::string_view SourceIccProfile = "SOURCE_ICC_PROFILE";

/*!
 * \brief Comment embedded in the JPEG (COM marker).
 * \note String value, free text.
 */
constexpr std::string_view Comment = "COMMENT";

/*!
 * \brief Enable or disable generation of an EXIF thumbnail.
 * \note Supported values: "YES", "NO".
 */
constexpr std::string_view ExifThumbnail = "EXIF_THUMBNAIL";

/*!
 * \brief Width of the EXIF thumbnail in pixels.
 * \note Integer value.
 */
constexpr std::string_view ThumbnailWidth = "THUMBNAIL_WIDTH";

/*!
 * \brief Height of the EXIF thumbnail in pixels.
 * \note Integer value.
 */
constexpr std::string_view ThumbnailHeight = "THUMBNAIL_HEIGHT";

/*!
 * \brief Whether to write EXIF metadata items in an EXIF segment.
 * \note Supported values: "YES", "NO". Default is "YES". Requires GDAL 2.3 or later.
 */
constexpr std::string_view WriteExifMetadata = "WRITE_EXIF_METADATA";

} // namespace jpeg


namespace bmp
{

/*!
 * \brief Enable or disable generation of an associated ESRI world file.
 * \note Supported values: "YES", "NO".
 */
constexpr std::string_view WorldFile = "WORLDFILE";

} // namespace bmp


namespace gif
{

/*!
 * \brief Enable or disable generation of an associated ESRI world file.
 * \note Supported values: "YES", "NO".
 */
constexpr std::string_view WorldFile = "WORLDFILE";

/*!
 * \brief Enable or disable interlacing for the GIF image.
 * \note Supported values: "YES", "NO". (Original GDAL uses "ON" to enable, empty to disable.)
 */
constexpr std::string_view Interlacing = "INTERLACING";

} // namespace gif


/*!
 * \brief Unified container for image format options.
 *
 * This class provides a generic key-value store for format-specific options
 * (e.g., compression, tiling, world file generation). Keys and values are
 * strings, and the container supports iteration, querying, and modification.
 *
 * Typical usage:
 * \code
 * ImageOptions options;
 * options.set(tiff::Compress, "LZW")
 *         .set(tiff::Tiled, "YES");
 *
 * if (auto val = options.value(tiff::Compress)) {
 *     // use *val
 * }
 * \endcode
 */
class ImageOptions
{

public:

    using container_type = std::map<std::string, std::string, std::less<>>;
    using iterator = typename std::map<std::string, std::string>::iterator;
    using const_iterator = typename std::map<std::string, std::string>::const_iterator;


private:

    container_type mOptions;

public:

    /*!
     * \brief Default constructor – creates an empty options container.
     */
    ImageOptions() = default;

    /*!
     * \brief Set or overwrite an option value.
     *
     * \param key   Option name (e.g., tiff::Compress, png::ZLevel).
     * \param value Option value as a string (e.g., "LZW", "YES", "75").
     * \return Reference to this ImageOptions object (for chaining).
     */
    auto &set(std::string_view key, std::string_view value);

    /*!
     * \brief Retrieve an option value.
     *
     * \param key Option name.
     * \return An optional containing the value as string_view if the key exists,
     *         or std::nullopt otherwise.
     */
    [[nodiscard]]
    auto value(std::string_view key) const -> std::optional<std::string_view>;

    /*!
     * \brief Iterator to the first option.
     * \return Mutable iterator.
     */
    auto begin() noexcept -> iterator
    {
        return mOptions.begin();
    }

    /*!
     * \brief Const iterator to the first option.
     * \return Constant iterator.
     */
    auto begin() const noexcept -> const_iterator
    {
        return mOptions.begin();
    }

    /*!
     * \brief Iterator to the end of the options.
     * \return Mutable end iterator.
     */
    auto end() noexcept -> iterator
    {
        return mOptions.end();
    }

    /*!
     * \brief Const iterator to the end of the options.
     * \return Constant end iterator.
     */
    auto end() const noexcept -> const_iterator
    {
        return mOptions.end();
    }

    /*!
     * \brief Check whether the container is empty.
     * \return true if no options are stored, false otherwise.
     */
    [[nodiscard]]
    auto empty() const noexcept -> bool { return mOptions.empty(); }
    
    /*!
     * \brief Get the number of stored options.
     * \return Number of key‑value pairs.
     */
    [[nodiscard]]
    auto size() const noexcept -> size_t { return mOptions.size(); }

    /*!
     * \brief Remove all options from the container.
     */
    void clear() { mOptions.clear(); }

};

/*! \} */

} // namespace tl
