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

#include "tidop/core/base/Defs.h"

//#include <vector>

//#ifdef TL_HAVE_GDAL
//TL_DISABLE_WARNINGS
//#include "gdal_priv.h"
//TL_DEFAULT_WARNINGS
//#endif // TL_HAVE_GDAL

#include <map>
#include <optional>
#include <string>
#include <string_view>

namespace tl
{





/*! \addtogroup VectorIO
 *  \{
 */


namespace shape
{

// Layer creation options

// Override the type of shapefile created. Can be one of NULL for a simple .dbf 
// file with no .shp file, POINT, ARC, POLYGON or MULTIPOINT for 2D; POINTZ, ARCZ, 
// POLYGONZ, MULTIPOINTZ or MULTIPATCH for 3D; POINTM, ARCM, POLYGONM or MULTIPOINTM 
// for measured geometries; and POINTZM, ARCZM, POLYGONZM or MULTIPOINTZM for 
// 3D measured geometries.
constexpr std::string_view shpt = "SHPT";
constexpr std::string_view encoding = "ENCODING";
// Defaults to NO. Set to YES to resize fields to their optimal size. See above "Field sizes" section.
constexpr std::string_view resize = "RESIZE";
// Defaults to NO. Set to YES to enforce the 2GB file size for .SHP or .DBF files.
constexpr std::string_view limit_2gb = "2GB_LIMIT";
// Defaults to NO.Set to YES to create a spatial index(.qix).
constexpr std::string_view spatial_index = "SPATIAL_INDEX";
constexpr std::string_view bbf_date_last_update = "DBF_DATE_LAST_UPDATE";
constexpr std::string_view auto_repack = "AUTO_REPACK";
constexpr std::string_view dbf_eof_char = "DBF_EOF_CHAR";

}


/*!
 * \brief Format options
 */
class TL_EXPORT VectorOptions
{

public:

    using container_type = std::map<std::string, std::string, std::less<>>;
    using iterator = std::map<std::string, std::string>::iterator;
    using const_iterator = std::map<std::string, std::string>::const_iterator;

protected:

    container_type mOptions;

public:

    VectorOptions() = default;
    virtual ~VectorOptions() = default;

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


/*! \} */ // end of vector

}