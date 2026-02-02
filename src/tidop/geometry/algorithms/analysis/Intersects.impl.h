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


namespace tl
{

namespace detail
{

template<typename Point_t>
auto intersects_impl(const Point_t &p1, 
                     const Point_t &p2, 
                     point_tag, 
                     point_tag) -> bool
{
    return p1 == p2; // Podrías usar una tolerancia
}

template<typename Point_t>
auto intersects_impl(const Point_t &point, 
                     const Segment<Point_t> &segment,
                     point_tag, 
                     segment_tag) -> bool
{
    auto proj = project(point, segment);
    return !proj.isBeforeStart() && !proj.isAfterEnd() &&
        distance(point, proj.closestPoint) < std::numeric_limits<double>::epsilon();
}

/* Checks if two segments intersect */
template<typename Point_t>
auto intersects_impl(const Segment<Point_t> &s1, 
                     const Segment<Point_t> &s2,
                     segment_tag, 
                     segment_tag) -> bool
{
    // Solo para 2D por ahora
    static_assert(point_traits<Point_t>::spatial_dims == 2, "Segment intersection currently only supports 2D");

    const auto &p1 = s1.pt1();
    const auto &p2 = s1.pt2();
    const auto &q1 = s2.pt1();
    const auto &q2 = s2.pt2();

    // Función de orientación
    auto orientation = [](const Point_t &a, const Point_t &b, const Point_t &c) -> int {
        double val = (b.y() - a.y()) * (c.x() - b.x()) - (b.x() - a.x()) * (c.y() - b.y());
        if (std::abs(val) < std::numeric_limits<double>::epsilon()) return 0; // colineal
        return (val > 0) ? 1 : 2; // horario o antihorario
        };

    auto onSegment = [](const Point_t &p, const Point_t &q, const Point_t &r) -> bool {
        return q.x() <= std::max(p.x(), r.x()) && q.x() >= std::min(p.x(), r.x()) &&
            q.y() <= std::max(p.y(), r.y()) && q.y() >= std::min(p.y(), r.y());
        };

    int o1 = orientation(p1, p2, q1);
    int o2 = orientation(p1, p2, q2);
    int o3 = orientation(q1, q2, p1);
    int o4 = orientation(q1, q2, p2);

    // Caso general
    if (o1 != o2 && o3 != o4) return true;

    // Casos especiales (colinealidad)
    if (o1 == 0 && onSegment(p1, q1, p2)) return true;
    if (o2 == 0 && onSegment(p1, q2, p2)) return true;
    if (o3 == 0 && onSegment(q1, p1, q2)) return true;
    if (o4 == 0 && onSegment(q1, p2, q2)) return true;

    return false;
}

/* Fast bounding box intersection check. */
template<typename Point_t>
auto intersects_impl(const BoundingBox<Point_t> &bbox1,
                     const BoundingBox<Point_t> &bbox2,
                     bbox_tag, 
                     bbox_tag) -> bool
{
    if (bbox1.isEmpty() || bbox2.isEmpty()) return false;

    for (size_t i = 0; i < point_traits<Point_t>::spatial_dims; ++i) {
        if (bbox1.max()[i] < bbox2.min()[i] || bbox1.min()[i] > bbox2.max()[i]) {
            return false;
        }
    }
    return true;
}

} // namespace detail

template<typename G1, typename G2>
auto intersects(const G1 &g1, const G2 &g2) -> bool
{
    static_assert(is_geometry_v<G1>, "First argument must be a geometry");
    static_assert(is_geometry_v<G2>, "Second argument must be a geometry");

    // Chequeo rápido con bounding boxes
    // Si G1 y G2 son BoundingBox esto no tiene sentido
    auto bbox1 = envelope(g1);
    auto bbox2 = envelope(g2);

    if (!detail::intersects_impl(bbox1, bbox2), bbox_tag, bbox_tag) {
        return false;
    }

    using tag1 = geometry_tag_t<G1>;
    using tag2 = geometry_tag_t<G2>;

    return detail::intersects_impl(g1, g2, tag1{}, tag2{});
}

} // namespace tl