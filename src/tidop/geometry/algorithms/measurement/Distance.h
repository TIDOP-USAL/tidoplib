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

#include "tidop/geometry/base/Traits.h"
#include "tidop/geometry/base/Concepts.h"
#include "tidop/geometry/algorithms/projection.h"
#include "tidop/geometry/algorithms/analysis/Intersects.h"

namespace tl
{
	
/*!
 * \brief Base class for distance functors
 */
template<typename Point_t>
struct DistanceBase
{
    virtual ~DistanceBase() = default;

    virtual auto operator()(const Point_t &a, const Point_t &b) const -> double = 0;
};

/*!
 * \brief Squared Euclidean distance
 *
 * \tparam Point_t Point type
 */
template<typename Point_t>
struct SquaredEuclideanDistance
  : public DistanceBase<Point_t>
{

    auto operator()(const Point_t &a, const Point_t &b) const -> double override
    {
        return (b - a).squaredNorm();
    }
};

/*!
 * \brief Euclidean distance
 * 
 * \tparam Point_t Point type
 */
template<typename Point_t>
struct EuclideanDistance
  : public SquaredEuclideanDistance<Point_t>
{
    auto operator()(const Point_t &a, const Point_t &b) const -> double override
    {
        return (b - a).norm();
    }
};

/*!
 * \brief Manhattan distance (L1)
 */
template<typename Point_t>
struct ManhattanDistance
  : public DistanceBase<Point_t>
{

    auto operator()(const Point_t &a, const Point_t &b) const -> double override
    {
        double sum = 0.0;

        //if constexpr (HasDimensions<PointType>::value) {
        //    constexpr size_t dim = PointType::dimensions;
        //    for (size_t i = 0; i < dim; ++i) {
        //        sum += std::abs(this->get_coordinate(a, i) -
        //            this->get_coordinate(b, i));
        //    }
        //} else {
        //    size_t dim = 0;
        //    if constexpr (HasSubscriptOperator<PointType>::value) {
        //        if constexpr (requires { a.size(); }) {
        //            dim = a.size();
        //        }
        //    }

        //    for (size_t i = 0; i < dim; ++i) {
        //        sum += std::abs(this->get_coordinate(a, i) -
        //            this->get_coordinate(b, i));
        //    }
        //}

        return sum;
    }
};

/*!
 * \brief Chebyshev distance (L∞)
 */
template<typename Point_t>
struct ChebyshevDistance
  : public DistanceBase<Point_t>
{

    auto operator()(const Point_t &a, const Point_t &b) const -> double override 
    {
        double max_diff = 0.0;

        //if constexpr (HasDimensions<PointType>::value) {
        //    constexpr size_t dim = PointType::dimensions;
        //    for (size_t i = 0; i < dim; ++i) {
        //        double diff = std::abs(this->get_coordinate(a, i) -
        //            this->get_coordinate(b, i));
        //        if (diff > max_diff) max_diff = diff;
        //    }
        //} else {
        //    size_t dim = 0;
        //    if constexpr (HasSubscriptOperator<PointType>::value) {
        //        if constexpr (requires { a.size(); }) {
        //            dim = a.size();
        //        }
        //    }

        //    for (size_t i = 0; i < dim; ++i) {
        //        double diff = std::abs(this->get_coordinate(a, i) -
        //            this->get_coordinate(b, i));
        //        if (diff > max_diff) max_diff = diff;
        //    }
        //}

        return max_diff;
    }
};

/*!
 * \brief Minkowski distance (Lp)
 */
//template<typename Point_t, double p = 2.0>
//struct MinkowskiDistance
//  : public DistanceBase<Point_t> 
//{
//    static_assert(p >= 1.0, "Minkowski distance requires p >= 1");
//
//    auto operator()(const PointType &a, const PointType &b) const -> double override 
//    {
//        double sum = 0.0;
//
//        //if constexpr (HasDimensions<PointType>::value) {
//        //    constexpr size_t dim = PointType::dimensions;
//        //    for (size_t i = 0; i < dim; ++i) {
//        //        double diff = std::abs(this->get_coordinate(a, i) -
//        //            this->get_coordinate(b, i));
//        //        sum += std::pow(diff, p);
//        //    }
//        //} else {
//        //    size_t dim = 0;
//        //    if constexpr (HasSubscriptOperator<PointType>::value) {
//        //        if constexpr (requires { a.size(); }) {
//        //            dim = a.size();
//        //        }
//        //    }
//
//        //    for (size_t i = 0; i < dim; ++i) {
//        //        double diff = std::abs(this->get_coordinate(a, i) -
//        //            this->get_coordinate(b, i));
//        //        sum += std::pow(diff, p);
//        //    }
//        //}
//
//        return std::pow(sum, 1.0 / p);
//    }
//};



/*! \addtogroup Measurements
 *  \{
 */

template<GeometryConcept G1, GeometryConcept G2>
    requires SameSpatialDimension<G1, G2>
[[nodiscard]]
auto distance(const G1 &g1, const G2 &g2) -> double;

/*! \} */ 

} // End namespace tl

#include "Distance.impl.h"
