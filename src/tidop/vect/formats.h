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

#include "tidop/core/defs.h"

#include <vector>

#ifdef TL_HAVE_GDAL
TL_DISABLE_WARNINGS
#include "gdal_priv.h"
TL_DEFAULT_WARNINGS
#endif // TL_HAVE_GDAL

#include "tidop/core/utils.h"

namespace tl
{

/*! \addtogroup vector
 *  \{
 */

/*!
 * \brief Format options
 */
class TL_EXPORT VectorOptions
  : public FileOptions
{

public:

    enum class Format
    {
        shp,
        dxf,
        dgn
    };

protected:

    Format mFormat;

public:

    VectorOptions(Format format);
    ~VectorOptions() override;

    auto format() const -> Format;

    //auto options() -> const char* override = 0;
};

/*!
 * \brief Class that manages the options of the Shape format
 */
class TL_EXPORT ShapeOptions
  : public VectorOptions
{

public:

    /*!
     * \brief Geometry adjustment modes
     */
    enum class AdjustGeomType : uint8_t
    {
        no,
        first_shape,
        all_shapes
    };

protected:

    /*!
     * \brief Encoding
     */
    std::string mEncoding;

    /*!
     * \brief Modification date to write in the DBF header with the format year-month-day.
     * If not specified, the current date is used.
     */
    std::string mDbfDateLastUpdate;

    bool bAdjustType;

    /*!
     * \brief Adjustment of geometry type
     * Defines how the layer geometry type is calculated, particularly to distinguish shapefiles
     * that have shapes with meaningful values in the M dimension from those where M values are
     * set to nodata.
     * The default value is FIRST_SHAPE.
     * \see AdjustGeomType
     */
    AdjustGeomType mAdjustGeomType;

    bool bAutoRepack;
    bool bDbfEofChar;

public:

    ShapeOptions();
    ~ShapeOptions() override;

    auto options() const -> const char* override;

    void enableAdjustType(bool value = true);
    void enableAutoRepac(bool value = true);
    void enableDbfEofChar(bool value = true);

    auto encoding() const -> std::string;
    void setEncoding(const std::string &encoding);

    auto dbfDateLastUpdate() const -> std::string;
    void setDbfDateLastUpdate(const std::string &date);

    auto adjustGeomType() const -> AdjustGeomType;
    void setAdjustGeomType(AdjustGeomType type);

};

/*! \} */ // end of vector

}