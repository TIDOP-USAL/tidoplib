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

#include <algorithm>

#include "tidop/geometry/base/Geometry.h"
#include "tidop/geometry/primitives/Point.h"

namespace tl
{

namespace geometry
{

/*! \addtogroup Geometry
 *  \{
 */

template<typename Point_t>
class BoundingBox final
  : public GeometryBase<BoundingBox<Point_t>>
{

public:

    using T = typename Point_t::value_type;
    static constexpr size_t _size = VectorTraits<Point_t>::size;

private:

    std::array<Point_t, 2> mPoints;

public:

    BoundingBox() 
    {
        //mPoints[0].fill(std::numeric_limits<T>::max());
        //mPoints[1].fill(std::numeric_limits<T>::lowest());
        for (size_t i = 0; i < _size; ++i) {
            mPoints[0][i] = std::numeric_limits<T>::max();
            mPoints[1][i] = std::numeric_limits<T>::lowest();
        }
    }

    /*!
     * \brief Constructor that defines the bounding box using two corner points.
     * \param[in] pt1 First corner point.
     * \param[in] pt2 Second corner point.
     */
    BoundingBox(const Point_t &pt1, const Point_t &pt2) 
      : mPoints{pt1, pt2}
    {
        normalized();
    }

    /*!
     * \brief Constructor that defines the bounding box using a central point and dimensions.
     * \param[in] pt Center of the bounding box.
     * \param[in] width Width of the bounding box.
     * \param[in] height Height of the bounding box.
     */
    template<typename U>
    BoundingBox(const Point_t &pt, U width, U height);

    /*!
     * \brief Constructor that defines the bounding box using a central point and dimensions.
     * \param[in] pt Center of the bounding box.
     * \param[in] width Width of the bounding box.
     * \param[in] depth Depth of the bounding box.
     * \param[in] height Height of the bounding box.
     */
    template<typename U>
    BoundingBox(const Point_t &pt, U width, U depth, U height);

    /*!
     * \brief Constructor that defines a cubic bounding box using a central point and side length.
     * \param[in] pt Center of the bounding box.
     * \param[in] side Length of each side (width = depth = height).
     */
    template<typename U>
    BoundingBox(const Point_t &pt, U side);

    /*!
     * \brief Constructor that creates a bounding box from a set of 3D points.
     * \param[in] vertices Vector of points used to compute the bounding box.
     */
    explicit BoundingBox(const std::vector<Point_t> &vertices)
      : BoundingBox()
    {
        for (const auto &v : vertices) {
            add(v);
        }
    }

    /*!
     * \brief Constructor de conversión/copia para diferentes tipos de punto.
     * \tparam OtherPoint_t El tipo de punto del BoundingBox origen.
     */
    template<typename OtherPoint_t>
    explicit BoundingBox(const BoundingBox<OtherPoint_t>&other)
    {
        static_assert(geometry_traits<Point_t>::dimension == geometry_traits<OtherPoint_t>::dimension, "BoundingBoxes must have the same dimension for conversion.");

        mPoints[0] = static_cast<Point_t>(other.pt1());
        mPoints[1] = static_cast<Point_t>(other.pt2());
    }

    auto pt1() noexcept -> Point_t &{ return mPoints[0]; }
    auto pt1() const noexcept -> const Point_t &{ return mPoints[0]; }

    auto pt2() noexcept -> Point_t &{ return mPoints[1]; }
    auto pt2() const noexcept -> const Point_t &{ return mPoints[1]; }

    auto start() noexcept -> Point_t &{ return mPoints[0]; }
    auto start() const noexcept -> const Point_t &{ return mPoints[0]; }

    auto end() noexcept -> Point_t &{ return mPoints[1]; }
    auto end() const noexcept -> const Point_t &{ return mPoints[1]; }

    /*!
     * \brief Retrieves the width of the bounding box.
     * \return The width.
     */
    auto width() const -> T;

    /*!
     * \brief Retrieves the height of the bounding box.
     * \return The height.
     */
    auto height() const -> T;

    /*!
     * \brief Retrieves the depth of the bounding box.
     * \return The depth.
     */
    auto depth() const -> T;

    // Método para expandir la caja (Crucial para algoritmos)
    void add(const Point_t &pt)
    {
        for (size_t i = 0; i < _size; ++i) {
            if (pt[i] < mPoints[0][i]) mPoints[0][i] = pt[i];
            if (pt[i] > mPoints[1][i]) mPoints[1][i] = pt[i];
        }
    }

    // Normalización N-Dimensional
    void normalized() 
    {
        for (size_t i = 0; i < _size; ++i) {
            if (mPoints[0][i] > mPoints[1][i]) std::swap(mPoints[0][i], mPoints[1][i]);
        }
    }

    // Contención Genérica
    //auto containsPoint(const Point_t &pt) const -> bool
    //{
    //    for (size_t i = 0; i < _size; ++i) {
    //        if (pt[i] < mPoints[0][i] || pt[i] > mPoints[1][i]) return false;
    //    }
    //    return true;
    //}

    // Propiedades geométricas
    auto center() const -> Point_t
    {
        Point_t center{};
        if (!this->isEmpty()) {
            auto vector = (mPoints[1] - mPoints[0]) / consts::two<T>;
            for (size_t i = 0; i < _size; ++i) {
                center[i] = mPoints[0][i] + vector[i];
                //center[i] = (mPoints[0][i] + mPoints[1][i]) / consts::two<T>;
            }
        }

        return center;
    }

    auto extent(size_t dim) const -> T 
    {
        return (isEmpty()) ? 0 : mPoints[1][dim] - mPoints[0][dim];
    }

    auto isEmpty() const -> bool 
    {
        return mPoints[0][0] == std::numeric_limits<T>::max();
    }

    auto isValid() const -> bool
    {
        if constexpr (is_3d_v<BoundingBox<Point_t>>) {
            return this->width() > consts::zero<T> &&
                   this->height() > consts::zero<T> &&
                   this->depth() > consts::zero<T>;
        } else {
            return this->width() > consts::zero<T> &&
                   this->height() > consts::zero<T>;
        }
    }

    // Implementación del Visitor
    //void accept(GeometryVisitor &v) override { v.visit(*this); }
};

template<typename Point_t>
template<typename U>
BoundingBox<Point_t>::BoundingBox(const Point_t &pt, U width, U height)
{
    static_assert(_size == 2, "This constructor requires a 2D Point.");

    auto half_width = width / consts::two<T>;
    auto half_height = height / consts::two<T>;

    mPoints[0] = Point_t(pt.x() - half_width, pt.y() - half_height);
    mPoints[1] = Point_t(pt.x() + half_width, pt.y() + half_height);

    if constexpr (std::is_integral_v<T>) {
        mPoints[1].x() += static_cast<T>(width) % 2;
        mPoints[1].y() += static_cast<T>(height) % 2;
    }
}

template<typename Point_t>
template<typename U>
BoundingBox<Point_t>::BoundingBox(const Point_t &pt, U width, U depth, U height)
{
    static_assert(_size == 3, "This constructor requires a 3D Point.");

    auto half_width = width / consts::two<T>;
    auto half_depth = depth / consts::two<T>;
    auto half_height = height / consts::two<T>;

    mPoints[0] = Point_t(pt.x() - half_width, pt.y() - half_depth, pt.z() - half_height);
    mPoints[1] = Point_t(pt.x() + half_width, pt.y() + half_depth, pt.z() + half_height);

    if constexpr (std::is_integral_v<T>) {
        mPoints[1].x() += static_cast<T>(width) % 2;
        mPoints[1].y() += static_cast<T>(depth) % 2;
        mPoints[1].z() += static_cast<T>(height) % 2; 
    }
}

template<typename Point_t>
template<typename U>
BoundingBox<Point_t>::BoundingBox(const Point_t &pt, U side)
{
    T half_side = static_cast<T>(side) / 2;
    T extra = (std::is_integral_v<T> && (static_cast<int>(side) % 2 != 0)) ? 1 : 0;

    for (size_t i = 0; i < _size; ++i) {
        mPoints[0][i] = pt[i] - half_side;
        mPoints[1][i] = pt[i] + half_side + extra;
    }
}


template<typename Point_t>
auto BoundingBox<Point_t>::width() const -> T
{
    return this->isEmpty() ? consts::zero<T> : mPoints[1].x() - mPoints[0].x();
}

template<typename Point_t>
auto BoundingBox<Point_t>::height() const -> T
{
    return this->isEmpty() ? consts::zero<T> : mPoints[1].y() - mPoints[0].y();
}

template<typename Point_t>
auto BoundingBox<Point_t>::depth() const -> T
{
    static_assert(!is_2d_v<BoundingBox<Point_t>>, "Method only supported for 3 or 4 dimensions");
    return this->isEmpty() ? consts::zero<T> : mPoints[1].z() - mPoints[0].z();
}

/*! \} */

}
}

