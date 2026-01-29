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

/*! \file wkt_proxy.hpp
 * \brief WKT (Well-Known Text) serialization proxy.
 *
 * This module provides a non-intrusive way to serialize and deserialize 
 * geometries to/from the OGC WKT standard. It uses a proxy-based approach
 * to handle precision and streaming without memory overhead.
 * * ### Classes
 * - \ref tl::WKTProxy : Wrapper for geometry serialization.
 * * ### Functions
 * - \ref tl::to_wkt : Helper to wrap a geometry for output.
 * - \ref tl::read_wkt : Helper to wrap a geometry for input.
 * * \see tl::WKTWriter
 */

#pragma once

#include "tidop/geometry/base/Traits.h"
#include "tidop/geometry/io/wkt/Writer.h"

namespace tl
{

/*! \addtogroup GeometryIO
 *  \{
 */

/*! 
 * \brief Proxy class for WKT (Well-Known Text) serialization of geometries.
 * 
 * This class acts as a lightweight wrapper around a geometry object,
 * providing methods to control WKT output formatting, such as precision.
 *
 * * ### Classes
 * - \ref tl::WKTProxy : Wrapper for geometry serialization.
 * * ### Functions
 * - \ref tl::wkt : Helper to wrap a geometry for output.
 * * \see tl::WKTWriter
 * 
 * \tparam Geometry Type of the geometry being serialized.
 */
template<typename Geometry_t>
struct WKTProxy 
{
    const Geometry_t &geometry;
    int precision = -1;

    /*! \brief Define la precisión decimal de las coordenadas. */
    auto withPrecision(int p) -> WKTProxy &
    { 
        precision = p;
        return *this;
    }

    /*! 
     * \brief Convierte la geometría a string.
     * \warning En geometrías masivas, este método puede consumir mucha memoria RAM.
     * Se recomienda usar el operador << directo a stream en su lugar.
     */
    auto str() const -> std::string
    {
        std::stringstream ss;
        ss << *this;
        return ss.str();
    }

    /*! \brief Operador para inyección en streams (Consumo de memoria O(1)). */
    friend std::ostream& operator<<(std::ostream& os, const WKTProxy& proxy)
    {
        // Preservar estado original
        std::ios_base::fmtflags old_flags(os.flags());
        auto old_precision = os.precision();

        if (proxy.precision >= 0) {
            os << std::fixed << std::setprecision(proxy.precision);
        }

        // Despacho por Tag Dispatching
        WKTWriter::write(os, proxy.geometry, geometry_tag_t<Geometry_t>{});

        // Restaurar estado
        os.flags(old_flags);
        os.precision(old_precision);
        return os;
    }
};


/*!
 * \brief Helper function to create a WKTProxy for a geometry.
 *
 * This function simplifies the creation of a WKTProxy object,
 * allowing for easy serialization of geometries to WKT format.
 *
 * ### Usage Example
 * 
 * \code{.cpp}
 * auto p = tl::Point2d(10.5123, 20.0);
 * // 1. Simple usage with streams
 * std::cout << tl::wkt(p) << std::endl; // Output: POINT (10.5123 20)
 * // 2. Controlling precision
 * std::cout << tl::wkt(p).with_precision(2) << std::endl; // Output: POINT (10.51 20.00)
 * // 3. Usage with std::format (C++20)
 * std::string s = std::format("Geometry: {:.3f}", tl::wkt(p));
 * \endcode
 * 
 * \tparam Geometry Type of the geometry being serialized.
 * \param g The geometry instance to be wrapped.
 * \return A WKTProxy wrapping the provided geometry.
 */
template<typename Geometry>
auto wkt(const Geometry &g) 
{
    return WKTProxy<Geometry>{g};
}

/*! \} */ 

} // End namespace tl

// Soporte para std::format / fmt
#if TL_CPP_VERSION >= 20 || defined(TL_HAVE_FMT)
template <typename Geometry>
struct FORMAT_NAMESPACE formatter<tl::WKTProxy<Geometry>> {

    int precision = -1;

    constexpr auto parse(FORMAT_NAMESPACE format_parse_context &ctx) 
    {
        auto it = ctx.begin();
        if (it == ctx.end() || *it == '}') return it;

        // Parseo manual simple de precisión: {:.3f}
        if (*it == ':') ++it;
        if (*it == '.') {
            ++it;
            precision = 0;
            while (it != ctx.end() && *it >= '0' && *it <= '9') {
                precision = precision * 10 + (*it - '0');
                ++it;
            }
        }
        // Saltamos hasta el final del especificador
        while (it != ctx.end() && *it != '}') ++it;
        return it;
    }

    auto format(const tl::WKTProxy<Geometry> &proxy, FORMAT_NAMESPACE format_context &ctx) const
    {
        // Creamos una copia del proxy para no modificar el original
        auto p = proxy;
        // Si el formato de string dio una precisión, esa manda
        if (this->precision != -1) p.withPrecision(this->precision);

        std::stringstream ss;
        ss << p;
        return FORMAT_NAMESPACE format_to(ctx.out(), "{}", ss.str());
    }
};
#endif
