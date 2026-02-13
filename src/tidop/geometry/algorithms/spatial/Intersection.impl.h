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
#include "Intersection.h"

namespace tl
{

namespace detail
{


template<typename P1, typename P2>
auto intersection_impl(const P1 &p1, 
                       const P2 &p2, 
                       point_tag, 
                       point_tag) -> std::optional<common_point_without_measure_t<P1, P2>>
{
    using Point_t = common_point_without_measure_t<P1, P2>;

    if (equals(p1, p2)) {
        return static_cast<Point_t>(p1);
    }

    return std::nullopt;
}

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

template<typename G1, typename G2>
auto intersection_impl(const G1 &, const G2 &, ...)
{
    static_assert(false, "intersection not implemented for these geometry types");
}


} // namespace detail



template<typename G1, typename G2>
auto intersection(const G1 &g1, const G2 &g2)
{
    using tag1 = geometry_tag_t<G1>;
    using tag2 = geometry_tag_t<G2>;

    return detail::intersection_impl(g1, g2, tag1{}, tag2{});
}


//template<PointConcept P1, PointConcept P2>
//    requires SameSpatialDimension<P1, P2>
//[[nodiscard]]
//auto intersection(const P1 &p1, const P2 &p2) -> std::optional<common_point_without_measure_t<P1, P2>>
//{
//    if (equals(p1, p2)) {
//
//        // Convertir a punto sin medida
//        using Point_t = common_point_without_measure_t<P1, P2>;
//        Point_t result = static_cast<Point_t>(p1);
//
//        // Copiar solo dimensiones espaciales
//        //constexpr size_t spatial_dims = point_traits<P1>::spatial_dims;
//        //for (size_t i = 0; i < spatial_dims; ++i) {
//        //    result[i] = static_cast<typename Point_t::value_type>(p1[i]);
//        //}
//
//        return result;
//    }
//
//    return std::nullopt;
//}



} // namespace tl