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

#include <optional>

#include "tidop/geometry/base/Traits.h"
#include "tidop/geometry/base/Concepts.h"
#include "tidop/geometry/spatial/Line.h"
#include "tidop/math/algebra/decomp/Eigen.h"
#include "tidop/math/algebra/matrix/Matrix.h"
#include "tidop/math/algebra/vector/Vector.h"


namespace tl
{
	
// se podría unificar fitLine3D y fitLine2D

template<typename PointContainer>
    requires Point3DConcept<typename PointContainer::value_type>
auto fitLine3D(const PointContainer &points, double tolerance = 1e-8) -> std::optional<Line<typename point_traits<typename PointContainer::value_type>::value_type>>
{
    using point_type = typename PointContainer::value_type;
    using T = typename point_traits<point_type>::value_type;

    if (points.size() < 2) return std::nullopt;

    // Centroide
    auto centroid = Vector<T, 3>::zeros();
    for (const auto &point : points) {
        centroid += point.vector();
    }
    centroid /= points.size();

    // Matriz de covarianza
    auto cov = Matrix<T, 3, 3>::zeros();
    for (const auto &point : points) {
        Matrix<T, 3, 1> v;
        v.col(0) = point.vector() - centroid;
        cov += v * v.transpose();
    }

    EigenDecomposition<Matrix<T, 3, 3>> eig(cov);
    auto eigen_values = eig.eigenvaluesReal();
    auto eigen_vectors = eig.eigenvectors();

    if (eigen_values[2] < tolerance) return std::nullopt;

    Vector<T, 3> direction = eigen_vectors.col(2);
    direction.normalize();

    return Line<T>(centroid, direction);
}

                        
template<typename PointContainer>
    requires Point2DConcept<typename PointContainer::value_type>
auto fitLine2D(const PointContainer &points, double tolerance = 1e-8) -> std::optional<Line<typename point_traits<typename PointContainer::value_type>::value_type, 2>>
{
    using point_type = typename PointContainer::value_type;
    using T = typename point_traits<point_type>::value_type;

    if (points.size() < 2) return std::nullopt;

    // Centroide
    auto centroid = Vector<T, 2>::zeros();
    for (const auto &point : points) {
        centroid += point.vector();
    }
    centroid /= points.size();
    
    // Matriz de covarianza
    auto cov = Matrix<T, 2, 2>::zeros();
    for (const auto &point : points) {
        Matrix<T, 2, 1> v;
        v.col(0) = point.vector() - centroid;
        cov += v * v.transpose();
    }

    EigenDecomposition<Matrix<T, 2, 2>> eig(cov);
    auto eigen_values = eig.eigenvaluesReal();
    auto eigen_vectors = eig.eigenvectors();

    if (eigen_values[1] < tolerance) return std::nullopt;

    Vector<T, 2> direction = eigen_vectors.col(1);
    direction.normalize();

    return Line<T, 2>(centroid, direction);
}

template<typename PointContainer>
    requires requires { typename PointContainer::value_type; } &&
                        Point2DConcept<typename PointContainer::value_type>
auto fitLine2DSVD(const PointContainer &points, double tolerance = 1e-8) -> std::optional<Line<typename point_traits<typename PointContainer::value_type>::value_type, 2>>
{
    using point_type = typename PointContainer::value_type;
    using T = typename point_traits<point_type>::value_type;

    size_t n = points.size();
    if (n < 2) return std::nullopt;

    auto centroid = Vector<T, 2>::zeros();
    for (const auto &point : points) {
        centroid += point.vector();
    }
    centroid /= points.size();

    Matrix<T> A(n, 2);
    for (size_t i = 0; i < n; ++i) {
        //A(i, 0) = points[i].x() - cx;
        //A(i, 1) = points[i].y() - cy;
        A[i] = points[i].vector() - centroid;
    }

    auto svd = SingularValueDecomposition<Matrix<T>>(A);
    auto V = svd.v();
    auto S = svd.w();

    if (S[0] < tolerance) return std::nullopt;

    Vector<T, 2> direction = V.col(0);
    direction.normalize();
    //T dx = V(0, idx);
    //T dy = V(1, idx);
    //T norm = std::sqrt(dx * dx + dy * dy);
    //dx /= norm; dy /= norm;

    return Line<T, 2>(centroid, direction);
}

// TODO: marcar como deprecated

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

} // End namespace tl