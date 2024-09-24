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
{

public:

    enum class Format
    {
        shp,
        dxf,
        dgn,
        geojson
    };

    using option_iterator = std::map<std::string, std::string>::iterator;
    using option_const_iterator = std::map<std::string, std::string>::const_iterator;

protected:

    Format mFormat;

public:

    VectorOptions() = default;
    virtual ~VectorOptions() = default;

    virtual auto format() const -> Format = 0;

    virtual auto options() const -> std::map<std::string, std::string> = 0;
    virtual auto activeOptions() const -> std::map<std::string, std::string> = 0;
    virtual void reset() = 0;

};


class TL_EXPORT VectorOptionsBase
  : public VectorOptions
{

private:

    Format mFormat;

public:

    VectorOptionsBase(Format format);
    ~VectorOptionsBase() override;

    auto format() const -> Format override;
    auto options() const -> std::map<std::string, std::string> override;
    auto activeOptions() const -> std::map<std::string, std::string> override;

protected:

    virtual auto options(bool all) const -> std::map<std::string, std::string> = 0;

};


/*!
 * \brief Class that manages the options of the Shape format
 */
//class TL_EXPORT ShapeOptions
//  : public VectorOptionsBase
//{
//
//public:
//
//    /*!
//     * \brief Geometry adjustment modes
//     */
//    enum class AdjustGeomType : uint8_t
//    {
//        no,
//        first_shape,
//        all_shapes
//    };
//
//protected:
//
//    /*!
//     * \brief Encoding
//     */
//    std::pair<std::string, std::string> mEncoding;
//
//    /*!
//     * \brief Modification date to write in the DBF header with the format year-month-day.
//     * If not specified, the current date is used.
//     */
//    std::pair<std::string, std::string> mDbfDateLastUpdate;
//
//    std::pair<bool, bool> bAdjustType;
//
//    /*!
//     * \brief Adjustment of geometry type
//     * Defines how the layer geometry type is calculated, particularly to distinguish shapefiles
//     * that have shapes with meaningful values in the M dimension from those where M values are
//     * set to nodata.
//     * The default value is FIRST_SHAPE.
//     * \see AdjustGeomType
//     */
//    std::pair<AdjustGeomType, AdjustGeomType> mAdjustGeomType;
//
//    std::pair<bool, bool> bAutoRepack;
//    std::pair<bool, bool> bDbfEofChar;
//
//public:
//
//    ShapeOptions();
//    ~ShapeOptions() override;
//
//    void reset() override;
//
//    void enableAdjustType(bool value = true);
//    void enableAutoRepac(bool value = true);
//    void enableDbfEofChar(bool value = true);
//
//    auto encoding() const -> std::string;
//    void setEncoding(const std::string &encoding);
//
//    auto dbfDateLastUpdate() const -> std::string;
//    void setDbfDateLastUpdate(const std::string &date);
//
//    auto adjustGeomType() const -> AdjustGeomType;
//    void setAdjustGeomType(AdjustGeomType type);
//
//private:
//
//    void init();
//    auto options(bool all) const -> std::map<std::string, std::string> override;
//
//};




/*! \} */ // end of vector

}