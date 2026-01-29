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
#include <variant>
#include <type_traits>

namespace tl
{
	
struct WKTWriter 
{

    template<typename Point_t>
    static void write(std::ostream &os, const Point_t &g, point_tag)
    {
        using Tag = typename point_traits<Point_t>::tag_type;

        static_assert(Tag::is_ogc, "WKT Format Error: Only XY, XYZ, XYM, and XYZM layouts are supported by OGC standard.");

        writePrefix<Tag>(os, "POINT");
        writeCoords(os, g);
        os << ")";
    }

    template<typename LineString_t>
    static void write(std::ostream &os, const LineString_t &g, linestring_tag)
    {
        using Point_t = typename geometry_traits<LineString_t>::point_type;
        using Tag = typename point_traits<Point_t>::tag_type;

        static_assert(Tag::is_ogc, "WKT Format Error: Only XY, XYZ, XYM, and XYZM layouts are supported by OGC standard.");

        writePrefix<Tag>(os, "LINESTRING");
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
        using Tag = typename point_traits<Point_t>::tag_type;
        
        static_assert(Tag::is_ogc, "WKT Format Error: Only XY, XYZ, XYM, and XYZM layouts are supported by OGC standard.");

        writePrefix<Tag>(os, "POLYGON");
        writeRing(os, g.outer());
        for (const auto &inner : g.inners()) {
            os << ", ";
            writeRing(os, inner);
        }
        os << ")";
    }

    template<typename Segment_t>
    static void write(std::ostream &os, const Segment_t &s, segment_tag)
    {
        using Point_t = typename geometry_traits<Segment_t>::point_type;
        using Tag = typename point_traits<Point_t>::tag_type;

        static_assert(Tag::is_ogc, "WKT Format Error: Only XY, XYZ, XYM, and XYZM layouts are supported by OGC standard.");

        writePrefix<Tag>(os, "LINESTRING");
        writeCoords(os, s.pt1());
        os << ", ";
        writeCoords(os, s.pt2());
        os << ")";
    }

    template<typename MultiPoint_t>
    static void write(std::ostream &os, const MultiPoint_t &g, multipoint_tag)
    {
        using Point_t = typename geometry_traits<MultiPoint_t>::point_type;
        using Tag = typename point_traits<Point_t>::tag_type;

        writePrefix<Tag>(os, "MULTIPOINT");
        for (size_t i = 0; i < g.size(); ++i) {
            os << "(";
            writeCoords(os, g[i]);
            os << ")" << (i < g.size() - 1 ? ", " : "");
        }
        os << ")";
    }

    template<typename MultiLineString_t>
    static void write(std::ostream &os, const MultiLineString_t &g, multilinestring_tag)
    {
        using Point_t = typename geometry_traits<MultiLineString_t>::point_type;
        using Tag = typename point_traits<Point_t>::tag_type;

        writePrefix<Tag>(os, "MULTILINESTRING");
        for (size_t i = 0; i < g.size(); ++i) {
            os << "(";
            const auto &line = g[i];
            writeRingCoordinates(os, line);
            os << ")" << (i < g.size() - 1 ? ", " : "");
        }
        os << ")";
    }

    template<typename MultiPolygon_t>
    static void write(std::ostream &os, const MultiPolygon_t &g, multipolygon_tag)
    {
        using Point_t = typename geometry_traits<MultiPolygon_t>::point_type;
        using Tag = typename point_traits<Point_t>::tag_type;

        writePrefix<Tag>(os, "MULTIPOLYGON");

        for (size_t i = 0; i < g.size(); ++i) {
            os << "(";
            const auto &poly = g[i];
            writeRing(os, poly.outer());
            for (const auto &inner : poly.inners()) {
                os << ", ";
                writeRing(os, inner);
            }
            os << ")" << (i < g.size() - 1 ? ", " : "");
        }
        os << ")";
    }

    template<typename Point_t>
    static void write(std::ostream &os, const GeometryCollection<Point_t> &g, collection_tag)
    {
        using Tag = typename point_traits<Point_t>::tag_type;

        writePrefix<Tag>(os, "GEOMETRYCOLLECTION");

        for (size_t i = 0; i < g.size(); ++i) {
            // Usamos std::visit para despachar el GeoID al método write correspondiente
            std::visit([&os](auto &&arg) {
                // arg es una referencia al objeto real (Point_t&, LineString<Point_t>&, etc.)
                // Obtenemos su tag de geometría mediante traits para llamar al overload de WKTWriter
                //using PtrType = std::decay_t<decltype(arg)>;
                //using GeometryType = std::remove_const_t<std::remove_pointer_t<PtrType>>; // Quitamos el * para los traits
                ////using ArgType = std::decay_t<decltype(arg)>;
                //using ArgTag = typename geometry_traits<ArgType>::tag_type; // Ej: point_tag

                //WKTWriter::write(os, *arg, geometry_tag_t<GeometryType>{});

                const auto &geom = arg.get();
                using GeometryType = std::decay_t<decltype(geom)>;

                WKTWriter::write(os, geom, geometry_tag_t<GeometryType>{});
                }, g[i]);

            if (i < g.size() - 1) os << ", ";
        }
        os << ")";
    }

private:

    template<typename Tag>
    static void writePrefix(std::ostream &os, const std::string &typeName)
    {
        os << typeName;

        if constexpr (std::is_same_v<Tag, xyz_tag>)
            os << " Z (";
        else if constexpr (std::is_same_v<Tag, xym_tag>)
            os << " M (";
        else if constexpr (std::is_same_v<Tag, xyzm_tag>)
            os << " ZM (";
        else
            os << " (";
    }

    template<typename Point_t>
    static void writeCoords(std::ostream &os, const Point_t &p)
    {
        static constexpr size_t storage_size = point_traits<Point_t>::storage_size;

        for (size_t i = 0; i < storage_size; ++i) {
            os << p[i] << (i < storage_size - 1 ? " " : "");
        }
    }

    template<typename G>
    static void writeRingCoordinates(std::ostream &os, const G &ring)
    {
        for (size_t i = 0; i < ring.size(); ++i) {
            writeCoords(os, ring[i]);
            if (i < ring.size() - 1) os << ", ";
        }
    }

    template<typename G>
    static void writeRing(std::ostream &os, const G &ring)
    {
        os << "(";
        writeRingCoordinates(os, ring);
        os << ")";
    }
};

} // End namespace tl
