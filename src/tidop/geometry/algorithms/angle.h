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

#include "tidop/geometry/entities/point.h"
#include "tidop/geometry/algorithms/vector.h"
#include "tidop/math/math.h"

namespace tl
{

/*! \addtogroup geometry
 *  \{
 */

/*! \addtogroup geometry_algorithms
 *  \{
 */

/*!
 * \brief Returns the angle between two vectors
 * \param[in] v1 Vector 1
 * \param[in] v2 Vector 2
 * \return Angle in radians
 */
template<typename Point_t> inline
double vectorAngle(const Point_t &v1, const Point_t &v2)
{
    if (v1 == Point_t() || v2 == Point_t())
        return 0.0;
    return std::acos((v1.x * v2.x + v1.y * v2.y) / (module(v1.x, v1.y) * module(v2.x, v2.y)));
}

template<typename Point_t> inline
double vectorAngleOX(const Point_t &v)
{
    double angle = 0.0;
    if (!(v.x == 0 && v.y == 0))
        angle = atan2(v.y, v.x);
    return angle;
}

template<typename Point_t> inline
double vectorAngleOY(const Point_t &v)
{
    double angle = 0.0;
    if (!(v.x == 0 && v.y == 0))
        angle = atan2(v.x, v.y);
    return angle;
}

/*!
 * \brief Angle between line and plane
 * \param[in] line Line
 * \param[in] plane Plane defined by its normal
 * \return Angle in radians
 * http://www.vitutor.com/geometry/distance/line_plane.html
 */
template<typename Point3_t> inline
double angleBetweenLineAndPlane(const Segment3D<Point3_t> &line, const Point3_t &plane)
{
    Point3_t v1 = line.vector();
    double dot = dotProduct3D(v1, plane);
    double det = module3D(v1) * module3D(plane);
    return det != 0. ? asin(dot / det) : 0.;
}

/*!
 * \brief Angle between two planes
 * \param[in] plane1 Equation of the first plane
 * \param[in] plane2 Equation of the second plane
 * \return Angle in radians
 * http://www.vitutor.com/geometry/distance/angle_planes.html
 */
template<typename T> inline
double angleBetweenPlanes(const std::array<T, 4> &plane1, const std::array<T, 4> &plane2)
{
    return angleBetweenPlanes(Point3<T>(plane1[0], plane1[1], plane1[2]), Point3<T>(plane2[0], plane2[1], plane2[2]));
}

/*!
 * \brief Angle between two planes
 * \param[in] plane1 Vector normal to the first plane
 * \param[in] plane2 Vector normal to the second plane
 * \return Angle in radians
 * http://www.vitutor.com/geometry/distance/angle_planes.html
 */
template<typename Vect_t> inline
double angleBetweenPlanes(const Vect_t &plane1, const Vect_t &plane2)
{
    double dot = dotProduct3D(plane1, plane2);
    double det = module3D(plane1) * module3D(plane2);
    return det != 0. ? acos(dot / det) : 0.;
}

template<typename Point_t> inline
double azimut(const Point_t &pt1, const Point_t &pt2)
{
    double azimut = 0.;
    Point_t v{};
    v.x = pt2.x - pt1.x;
    v.y = pt2.y - pt1.y;
    if (v.x == 0 && v.y == 0) return azimut;
    azimut = atan2(v.x, v.y);
    if (azimut < 0.) azimut += consts::two_pi<double>;
    return azimut;
}


/*! \} */ // end of geometry_algorithms

/*! \} */ // end of geometry

} // End namespace tl

