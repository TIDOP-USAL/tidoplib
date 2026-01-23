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

#include "tidop/geometry/base/Traits.h"
#include <ostream>

namespace tl
{
	
struct WKTWriter 
{
    
    template<typename Point_t>
    static void writeCoords(std::ostream &os, const Point_t &p)
    {
        constexpr std::size_t dim = dimension_value(geometry_traits<Point_t>::dimension);
        for (std::size_t d = 0; d < dim; ++d) {
            os << p[d] << (d < dim - 1 ? " " : "");
        }
    }

    template<typename Point_t>
    static void write(std::ostream &os, const Point_t &g, point_tag)
    {
        constexpr auto dim = geometry_traits<Point_t>::dimension;
        os << (dim == Dimension::dim3 ? "POINT Z (" : "POINT (");
        writeCoords(os, g);
        os << ")";
    }

    template<typename LineString_t>
    static void write(std::ostream &os, const LineString_t &g, linestring_tag)
    {
        using Point_t = typename geometry_traits<LineString_t>::point_type;
        os << (geometry_traits<Point_t>::dimension == Dimension::dim3 ? "LINESTRING Z (" : "LINESTRING (");
        for (std::size_t i = 0; i < g.size(); ++i) {
            writeCoords(os, g[i]);
            if (i < g.size() - 1) os << ", ";
        }
        os << ")";
    }

    template<typename Polygon_t>
    static void write(std::ostream &os, const Polygon_t &g, polygon_tag)
    {
        using Point_t = typename geometry_traits<Polygon_t>::point_type;
        os << (geometry_traits<Point_t>::dimension == Dimension::dim3 ? "POLYGON Z (" : "POLYGON (");
        
        auto write_ring = [&](const auto &ring) {
            os << "(";
            for (std::size_t i = 0; i < ring.size(); ++i) {
                writeCoords(os, ring[i]);
                if (i < ring.size() - 1) os << ", ";
            }
            os << ")";
        };

        write_ring(g.outer());
        for (const auto &inner : g.inners()) {
            os << ", ";
            write_ring(inner);
        }
        os << ")";
    }

    template<typename Segment_t>
    static void write(std::ostream &os, const Segment_t &s, segment_tag)
    {
        os << (geometry_traits<Geometry>::dimension == Dimension::dim3 ? "LINESTRING Z (" : "LINESTRING (");
        writeCoords(os, s.first());
        os << ", ";
        writeCoords(os, s.second());
        os << ")";
    }
};

} // End namespace tl
