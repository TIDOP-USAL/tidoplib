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

namespace tl
{

namespace detail 
{

/* Point - Point */

template<Point2DConcept P1, Point2DConcept P2, PrecisionPolicyConcept Policy>
constexpr auto overlaps_impl(const P1 &,
                             const P2 &,
                             const Policy &,
                             point_tag,
                             point_tag) -> bool
{
    return false; // Puntos no pueden superponerse según OGC
}


/* Segment – Segment */

template<Segment2DConcept S1, Segment2DConcept S2, PrecisionPolicyConcept Policy>
[[nodiscard]]
constexpr auto overlaps_impl(const S1 &segment1,
                             const S2 &segment2,
                             const Policy &policy,
                             segment_tag,
                             segment_tag) -> bool
{
    const auto p1 = policy.toKernelPoint<Dimension::dim2>(segment1.pt1());
    const auto p2 = policy.toKernelPoint<Dimension::dim2>(segment1.pt2());
    const auto q1 = policy.toKernelPoint<Dimension::dim2>(segment2.pt1());
    const auto q2 = policy.toKernelPoint<Dimension::dim2>(segment2.pt2());

    if (TopologyKernel::equals(p1, p2) ||
        TopologyKernel::equals(q1, q2)) {
        return false;
    }

    auto type = TopologyKernel::intersectionType(p1, p2, q1, q2);
    if (type != TopologyKernel::IntersectionType::Overlapping) return false;

    bool q_in_p = TopologyKernel::isBetween(p1, p2, q1) &&
                  TopologyKernel::isBetween(p1, p2, q2);
    bool p_in_q = TopologyKernel::isBetween(q1, q2, p1) &&
                  TopologyKernel::isBetween(q1, q2, p2);

    return !(q_in_p || p_in_q);
}


/* LineString – LineString */

template<LinearGeometry2DConcept LS1, LinearGeometry2DConcept LS2, PrecisionPolicyConcept Policy>
[[nodiscard]]
auto overlaps_impl(const LS1 &ls1,
                   const LS2 &ls2,
                   const Policy &policy,
                   linestring_tag,
                   linestring_tag) -> bool
{
    auto k1 = policy.toKernelLineString<Dimension::dim2>(ls1);
    auto k2 = policy.toKernelLineString<Dimension::dim2>(ls2);

    bool has_linear_overlap = false;
    bool has_endpoint = false;
    bool is_contains = false;

    for (size_t i = 0; i + 1 < k1.size(); ++i) {

        const auto p1 = k1[i];
        const auto p2 = k1[i + 1];

        for (size_t j = 0; j + 1 < k2.size(); ++j) {

            const auto q1 = k2[j];
            const auto q2 = k2[j + 1];

            auto type = TopologyKernel::intersectionType(p1, p2, q1, q2);

            // cualquier cruce invalida overlaps
            if (type == TopologyKernel::IntersectionType::Proper)
                return false;

            if (type == TopologyKernel::IntersectionType::Overlapping){

                has_linear_overlap = true;

                if (!has_endpoint) {
                    if ((TopologyKernel::equals(p1, q1) &&
                         TopologyKernel::equals(p2, q2)) ||
                        (TopologyKernel::equals(p2, q1) &&
                         TopologyKernel::equals(p1, q2))) {
                        is_contains = true;
                    } else {

                        bool q_in_p = TopologyKernel::isBetween(p1, p2, q1) &&
                                      TopologyKernel::isBetween(p1, p2, q2);
                        bool p_in_q = TopologyKernel::isBetween(q1, q2, p1) &&
                                      TopologyKernel::isBetween(q1, q2, p2);

                        if (q_in_p || p_in_q)
                            is_contains = true;
                    }
                }

            } else if (type == TopologyKernel::IntersectionType::EndpointInterior) {
                has_endpoint = true;
                is_contains = false; // Ya no puede ser contains
            }
        }
    }

    if (is_contains) return false;

    if (!has_linear_overlap)
        return false;

    return true;
}


/* Polygon - Polygon */

//template<Polygon2DConcept Poly1, Polygon2DConcept Poly2, PrecisionPolicyConcept Policy>
//[[nodiscard]]
//auto overlaps_impl(const Poly1 &a,
//                   const Poly2 &b,
//                   const Policy &policy,
//                   polygon_tag,
//                   polygon_tag) -> bool
//{
//    // 0. Polígonos vacíos o degenerados nunca se solapan
//    if (a.isEmpty() || b.isEmpty()) return false;
//
//    // 1. Si uno contiene al otro, no se solapan (son within/contains)
//    if (contains(a, b, policy) || contains(b, a, policy))
//        return false;
//
//    // 2. Verificar si algún vértice de a está estrictamente dentro de b
//    for (const auto &v : a.outer()) {
//        if (locatePointInPolygon(b, v, policy) == Location::Interior)
//            return true;
//    }
//    for (const auto &hole : a.inners()) {
//        for (const auto &v : hole) {
//            if (locatePointInPolygon(b, v, policy) == Location::Interior)
//                return true;
//        }
//    }
//
//    // 3. Verificar si algún vértice de b está estrictamente dentro de a
//    for (const auto &v : b.outer()) {
//        if (locatePointInPolygon(a, v, policy) == Location::Interior)
//            return true;
//    }
//    for (const auto &hole : b.inners()) {
//        for (const auto &v : hole) {
//            if (locatePointInPolygon(a, v, policy) == Location::Interior)
//                return true;
//        }
//    }
//
//    // 4. Recorrer todos los pares de bordes (exteriores e interiores) para detectar
//    //    cruces o solapamientos que impliquen interior común.
//    auto for_each_edge = [&](const auto &poly, auto &&callback) {
//        // Anillo exterior
//        const auto &outer = poly.outer();
//        for (size_t i = 0; i + 1 < outer.size(); ++i)
//            callback(outer[i], outer[i + 1]);
//        // Huecos
//        for (const auto &hole : poly.inners()) {
//            for (size_t i = 0; i + 1 < hole.size(); ++i)
//                callback(hole[i], hole[i + 1]);
//        }
//        };
//
//    // Convertir puntos al kernel una sola vez (para cada polígono)
//    auto to_kernel = [&](const auto &p) {
//        return policy.template toKernelPoint<Dimension::dim2>(p);
//        };
//
//    std::vector<std::pair<Point2d, Point2d>> edges_a, edges_b;
//    for_each_edge(a, [&](const auto &p1, const auto &p2) {
//        edges_a.emplace_back(p1, p2);
//        });
//    for_each_edge(b, [&](const auto &p1, const auto &p2) {
//        edges_b.emplace_back(p1, p2);
//        });
//
//    // Comprobar cada par de aristas
//    for (const auto &[p1_raw, p2_raw] : edges_a) {
//        const auto p1 = to_kernel(p1_raw);
//        const auto p2 = to_kernel(p2_raw);
//        for (const auto &[q1_raw, q2_raw] : edges_b) {
//            const auto q1 = to_kernel(q1_raw);
//            const auto q2 = to_kernel(q2_raw);
//
//            const auto type = TopologyKernel::intersectionType(p1, p2, q1, q2);
//
//            // a) Cruce propio (Proper) → punto de intersección interior a ambos?
//            if (type == TopologyKernel::IntersectionType::Proper) {
//                auto inter = TopologyKernel::intersectionPoint(p1, p2, q1, q2);
//                if (inter) {
//                    // Convertir el punto kernel a punto original (asumiendo misma representación)
//                    Point2d pt(static_cast<double>(inter->x()), static_cast<double>(inter->y()));
//                    if (locatePointInPolygon(a, pt, policy) == Location::Interior &&
//                        locatePointInPolygon(b, pt, policy) == Location::Interior) {
//                        return true;
//                    }
//                }
//            }
//            // b) Solapamiento colineal (Overlapping) → tomar el punto medio del solapamiento
//            else if (type == TopologyKernel::IntersectionType::Overlapping) {
//                // Obtener los dos puntos extremos del solapamiento (esto puede requerir ordenar)
//                // Usamos el hecho de que en caso de solapamiento, los cuatro puntos son colineales.
//                // Calculamos el segmento de intersección como el intervalo común.
//                auto [r1, r2] = TopologyKernel::overlapSegment(p1, p2, q1, q2);
//                // Punto medio
//                auto mid = TopologyKernel::midPoint(r1, r2);
//                Point2d pt(static_cast<double>(mid.x()), static_cast<double>(mid.y()));
//                if (locatePointInPolygon(a, pt, policy) == Location::Interior &&
//                    locatePointInPolygon(b, pt, policy) == Location::Interior) {
//                    return true;
//                }
//            }
//        }
//    }
//
//    // 5. No se encontró solapamiento válido
//    return false;
//}


} // namespace detail



template<Geometry2DConcept G1, Geometry2DConcept G2>
[[nodiscard]]
constexpr auto overlaps(const G1 &geom1, 
                        const G2 &geom2) -> bool
{
    using P = typename geometry_traits<G1>::point_type;
    using Scalar = typename point_traits<P>::value_type;

    PrecisionPolicy<Scalar, PrecisionModel::Native> policy;

    return overlaps(geom1, geom2, policy);
}

template<Geometry2DConcept G1, Geometry2DConcept G2, PrecisionPolicyConcept Policy>
[[nodiscard]]
constexpr auto overlaps(const G1 &geom1,
                        const G2 &geom2,
                        const Policy &policy) -> bool
{
    using P1 = typename geometry_traits<G1>::point_type;
    using P2 = typename geometry_traits<G2>::point_type;
    using Scalar1 = typename point_traits<P1>::value_type;
    using Scalar2 = typename point_traits<P2>::value_type;

    static_assert(std::is_same_v<Scalar1, Scalar2>, "Points must have same coordinate type");

    if (geom1.isEmpty() || geom2.isEmpty())
        return false;

    return detail::overlaps_impl(geom1, geom2, policy, geometry_tag_t<G1>{}, geometry_tag_t<G2>{});
}

} // namespace tl