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


#include <vector>
#include <array>

#include "tidop/math/algebra/matrix.h"

namespace tl
{

/*!
 * \addtogroup MathTools
 *
 * \{
 */


/* ---------------------------------------------------------------------------------- */
/*           AJUSTES DE PUNTOS A GEOMETRIAS (LINEAS, PLANOS, ...)                     */
/* ---------------------------------------------------------------------------------- */


/*!
 * \brief Linear regression line of Y on X
 * The linear regression line of Y on X is used to estimate the values of Y from those of X.
 * The slope of the line is the ratio of the covariance to the variance of the X variable.
 *
 * \f$ y = m * x + b\f$<BR>
 *
 * \param[in] pts Points
 * \param[out] m Slope of the line
 * \param[out] b Intercept
 * \return Correlation coefficient. Value between -1 (negative slope) and 1 (positive slope). Values close to zero indicate a poor fit.
 *
 * ### Example Usage
 * \code{.cpp}
 * double m = 0.;
 * double b = 0.;
 * double corr = regressionLinearYX(pts, &m, &b);
 * \endcode
 */
template<typename Point_t> 
auto regressionLinearYX(const std::vector<Point_t> &pts, double *m, double *b) -> double
{
    double corr = 0.0;
    double sx = 0.;
    double sy = 0.;
    double sx2 = 0.;
    double sy2 = 0.;
    double sxy = 0.;
    size_t n = pts.size();
    if (n >= 2) {
        for (size_t i = 0; i < n; i++) {
            sx += pts[i].x;
            sy += pts[i].y;
            sx2 += pts[i].x * pts[i].x;
            sy2 += pts[i].y * pts[i].y;
            sxy += pts[i].x * pts[i].y;
        }
        double den = (n * sx2 - sx * sx);
        if (den != 0.) {
            // Línea no vertical
            *m = (n * sxy - sy * sx) / (n * sx2 - sx * sx);
            *b = (sy - *m * sx) / n;
            corr = sqrt(*m * (sxy - sx * sy / n) / (sy2 - sy * sy / n));
        }
    }
    return(corr);
}

/*!
 * \brief Linear regression line of X on Y
 * The linear regression line of X on Y is used to estimate the values of X from those of Y.
 * The slope of the line is the ratio of the covariance to the variance of the Y variable.
 *
 * \f$ x = m*y + b\f$<BR>
 *
 * \param[in] pts Points
 * \param[out] m Slope of the line
 * \param[out] b Intercept
 * \return Correlation coefficient. Value between -1 (negative slope) and 1 (positive slope). Values close to zero indicate a poor fit.
 *
 * ### Example Usage
 * \code{.cpp}
 * double m = 0.;
 * double b = 0.;
 * double corr = regressionLinearXY(pts, &m, &b);
 * \endcode
 */
template<typename Point_t> 
auto regressionLinearXY(const std::vector<Point_t> &pts, double *m, double *b) -> double
{
    double corr = 0.0;
    double sx = 0.;
    double sy = 0.;
    double sx2 = 0.;
    double sy2 = 0.;
    double sxy = 0.;
    size_t n = pts.size();
    if (n >= 2) {
        for (size_t i = 0; i < n; i++) {
            sx += pts[i].x;
            sy += pts[i].y;
            sx2 += pts[i].x * pts[i].x;
            sy2 += pts[i].y * pts[i].y;
            sxy += pts[i].x * pts[i].y;
        }
        double den = (n * sy2 - sy * sy);
        if (den != 0.) {
            *m = (n * sxy - sy * sx) / (n * sy2 - sy * sy);
            *b = (sx - *m * sy) / n;
            corr = sqrt(*m * (sxy - sx * sy / n) / (sx2 - sx * sx / n));
        }
    }
    return(corr);
}




/*!
 * \brief Obtains the equation of a plane passing through three points
 *
 * \f$ A*x + B*y + C*z + D = 0\f$
 *
 * \param[in] points Points defining the plane
 * \param[out] plane Parameters of the general equation of the plane (A, B, C, D)
 * \param[in] normalize If true, normalizes the plane equation
 * \return Normal to the plane
 */
template<typename T> 
auto threePointsPlane(const std::array<T, 3>& points, 
                      std::array<double, 4>& plane,
                      bool normalize = false) -> double
{
    T v1 = points[1] - points[0];
    T v2 = points[2] - points[0];
    plane[0] = v1.y * v2.z - v1.z * v2.y;
    plane[1] = v1.z * v2.x - v1.x * v2.z;
    plane[2] = v1.x * v2.y - v2.x * v1.y;
    plane[3] = -plane[0] * points[0].x - plane[1] * points[0].y - plane[2] * points[0].z;
    double N = sqrt(plane[0] * plane[0] + plane[1] * plane[1] + plane[2] * plane[2]);
    if (N != 0. && normalize) {
        plane[0] /= N;
        plane[1] /= N;
        plane[2] /= N;
        plane[3] /= N;
    }
    return N;
}


/*! \} */

} // End namespace tl

