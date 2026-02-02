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

//#include "tidop/geometry/primitives/Point.h"
//#include "tidop/geometry/algorithms/vector.h"

namespace tl
{

//template<typename T> class Segment;
//template<typename T> class Segment3D;
//
//template<typename Point_t>
//int projectPointInSegment(const Segment<Point_t> &ln, const Point_t &pt, Point_t *ptp)
//{
//    int iret = 0;
//    if (pt == ln.min || pt == ln.max) {
//        *ptp = pt;
//        return 2;
//    }
//    Point_t v1 = pt - ln.min;
//    Point_t v2 = ln.vector();
//    double daux = dotProduct(v1, v2);
//    double r = daux / (v2.x * v2.x + v2.y * v2.y);
//
//    if (typeid(typename Point_t::value_type) == typeid(int)) {
//        ptp->x = ln.min.x + roundToInteger(v2.x * r);
//        ptp->y = ln.min.y + roundToInteger(v2.y * r);
//    } else {
//        ptp->x = ln.min.x + static_cast<typename Point_t::value_type>(v2.x * r);
//        ptp->y = ln.min.y + static_cast<typename Point_t::value_type>(v2.y * r);
//    }
//
//    if (daux <= 0) iret = -1;
//    else if (daux >= (v2.x * v2.x + v2.y * v2.y)) iret = 1;
//    else if (daux == 0.) iret = 2; // Esta en la línea
//    return iret;
//}
//
//template<typename Point_t>
//int projectPointInSegment(const Segment3D<Point_t> &ln, const Point_t &pt, Point_t *ptp)
//{
//    int iret = 0;
//    if (pt == ln.min || pt == ln.max) {
//        *ptp = pt;
//        return 2;
//    }
//    Point3<double> v1 = pt - ln.min;
//    Point3<double> v2 = ln.vector();
//    double daux = dotProduct3D(v1, v2);
//    double r = daux / (v2.x * v2.x + v2.y * v2.y + v2.z * v2.z);
//
//    if (typeid(typename Point_t::value_type) == typeid(int)) {
//        ptp->x = ln.min.x + roundToInteger(v2.x * r);
//        ptp->y = ln.min.y + roundToInteger(v2.y * r);
//        ptp->z = ln.min.z + roundToInteger(v2.z * r);
//    } else {
//        ptp->x = ln.min.x + static_cast<typename Point_t::value_type>(v2.x * r);
//        ptp->y = ln.min.y + static_cast<typename Point_t::value_type>(v2.y * r);
//        ptp->z = ln.min.z + static_cast<typename Point_t::value_type>(v2.z * r);
//    }
//
//    if (daux <= 0) iret = -1;
//    else if (daux >= (v2.x * v2.x + v2.y * v2.y + v2.z * v2.z)) iret = 1;
//    else if (daux == 0.) iret = 2; // Esta en la línea
//    return iret;
//}

template<typename Point_t> class Segment;

template<typename Point_t>
struct ProjectionResult
{
    Point_t closestPoint;
    double t; // 0.0 es P1, 1.0 es P2

    bool isBeforeStart() const { return t < 0.0; }
    bool isAfterEnd() const { return t > 1.0; }
    bool isInsideSegment() const { return t >= 0.0 && t <= 1.0; }
};

template<typename Point_t>
auto project(const Point_t &pt, const Segment<Point_t> &seg) -> ProjectionResult<Point_t>
{
    auto v = seg.pt2() - seg.pt1();      // Vector del segmento
    auto w = pt - seg.pt1();            // Vector desde el inicio al punto

    double l2 = v.dotProduct(v);    // |v|^2

    if (l2 == 0.0) { // El segmento es en realidad un punto
        return {seg.pt1(), 0.0};
    }

    // t es la proyección escalar de w sobre v, normalizada
    double t = w.dotProduct(v) / l2;

    // Punto proyectado sobre la recta infinita
    Point_t proj_pt = seg.pt1() + v * t;

    return {proj_pt, t};
}

} // End namespace tl

