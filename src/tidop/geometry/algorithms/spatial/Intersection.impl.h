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

#include <cmath>

#include "tidop/geometry/spatial/BoundingBox.h"

namespace tl
{

namespace detail
{

//TODO: Esto se tiene que mover a Intersects

//template <typename Point_t>
//auto intersect_lines(const Segment<Point_t> &ln1, 
//                     const Segment<Point_t> &ln2,
//                     Point_t &out_pt) -> bool
//{
//    using T = typename Point_t::value_type;
//    auto vs1 = ln1.vector();
//    auto vs2 = ln2.vector();
//
//    double cp = vs1.cross(vs2);
//    
//
//    // Si el producto vectorial es 0, las rectas son paralelas o coincidentes
//    if (std::abs(cp) < 1e-10) return false;
//
//    auto v11_12 = ln2.min() - ln1.min();
//    double t = crossProduct(v11_12, vs2) / cp;
//
//    out_pt.x() = numberCast<T>(ln1.min().x() + t * vs1.x());
//    out_pt.y() = numberCast<T>(ln1.min().y() + t * vs1.y());
//
//    return true;
//}
//
//template <typename Point_t>
//bool intersect_segments(const Segment<Point_t> &ln1, 
//                        const Segment<Point_t> &ln2, 
//                        Point_t &out_pt) 
//{
//    using T = typename Point_t::value_type;
//    auto vs1 = ln1.vector();
//    auto vs2 = ln2.vector();
//
//    double cp = crossProduct(vs1, vs2);
//    if (std::abs(cp) < 1e-10) return false;
//
//    Point_t v11_12 = ln2.min - ln1.min;
//    double t = crossProduct(v11_12, vs2) / cp;
//    double u = crossProduct(v11_12, vs1) / cp;
//
//    // Los parámetros t y u deben estar en el rango [0, 1] para que el punto esté en los segmentos
//    if (t >= 0.0 && t <= 1.0 && u >= 0.0 && u <= 1.0) {
//        out_pt.x = numberCast<T>(ln1.min.x() + t * vs1.x);
//        out_pt.y = numberCast<T>(ln1.min.y() + t * vs1.y);
//        return true;
//    }
//
//    return false;
//}


template<typename BBox_t>
auto intersection_impl(const BBox_t &b1, const BBox_t &b2, bbox_tag, bbox_tag) -> BBox_t
{
    using Point_t = typename geometry_traits<BBox_t>::point_type;
    constexpr size_t Dim = dimension_value(geometry_traits<BBox_t>::dimension);

    Point_t new_min, new_max;

    for (size_t i = 0; i < Dim; ++i) {
        new_min[i] = std::max(b1.min()[i], b2.min()[i]);
        new_max[i] = std::min(b1.max()[i], b2.max()[i]);

        // Si en alguna dimensión no solapan, el BBox resultante es inválido
        if (new_min[i] > new_max[i]) {
            return BBox_t(); // Retorna BBox vacío (min > max)
        }
    }

    return BBox_t(new_min, new_max);
}


} // namespace detail


template<typename G1, typename G2>
auto intersection(const G1 &g1, const G2 &g2)
{
    using tag1 = geometry_tag_t<G1>;
    using tag2 = geometry_tag_t<G2>;
    return detail::intersection_impl(g1, g2, tag1{}, tag2{});
}

} // namespace tl