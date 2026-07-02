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

#include "tidop/geometry/shapes/Shape.h"
#include "tidop/geometry/primitives/Point.h"
#include "tidop/geometry/algorithms/measurement/Distance.h"
#include "tidop/graphic/base/Rect.h"
#include "tidop/math/numeric/Arithmetic.h"

namespace tl
{

/*! \addtogroup Shapes
 *  \{
 */


/*!
 * \brief Triangle class
 *
 * Represents a 2D triangle defined by three points.
 */
template<typename T>
class Triangle
  : public Shape
{

public:

    using value_type = T;

protected:

    Point<T> mPt1;
    Point<T> mPt2;
    Point<T> mPt3;

public:

    /*!
     * \brief Default constructor
     */
    constexpr Triangle() = default;

    /*!
     * \brief Constructor with three points
     * \param[in] min First vertex
     * \param[in] max Second vertex
     * \param[in] pt3 Third vertex
     */
    explicit constexpr Triangle(const Point<T> &pt1, const Point<T> &pt2, const Point<T> &pt3);

    /*!
     * \brief Copy constructor
     * \param[in] triangle Triangle object to copy
     */
    constexpr Triangle(const Triangle<T> &triangle) = default;

    /*!
     * \brief Move constructor
     * \param[in] triangle Triangle object to move
     */
    constexpr Triangle(Triangle<T> &&triangle) noexcept = default;

    /*!
     * \brief Destructor
     */
    ~Triangle() override = default;

    /*!
     * \brief Copy assignment operator
     * \param[in] triangle Triangle object to copy
     * \return Reference to the assigned object
     */
    constexpr auto operator=(const Triangle<T> &triangle)->Triangle<T> & = default;

    /*!
     * \brief Move assignment operator
     * \param[in] triangle Triangle object to move
     * \return Reference to the assigned object
     */
    constexpr auto operator=(Triangle<T> &&triangle)  noexcept -> Triangle<T> & = default;

    /*!
     * \brief Computes the area of the triangle using the determinant formula
     * \return The computed area
     */
    [[nodiscard]]
    constexpr auto area() const noexcept -> double override;

    /*!
     * \brief Computes the perimeter of the triangle
     * \return The sum of the three side lengths
     */
    [[nodiscard]]
    constexpr auto perimeter() const noexcept -> double;

    /*!
     * \brief Checks if the triangle is valid (not degenerate)
     * \return True if the triangle is valid, false otherwise
     */
    [[nodiscard]]
    constexpr auto isValid() const noexcept -> bool;

    /*!
     * \brief Computes the centroid (barycenter) of the triangle
     * \return The centroid as a `Point<T>`
     */
    [[nodiscard]]
    constexpr auto centroid() const -> Point<T>;

    /*!
     * \brief Equality comparison operator
     * \param[in] other The triangle to compare with
     * \return True if triangles are equal, false otherwise
     */
    [[nodiscard]]
    constexpr auto operator==(const Triangle &other) const -> bool;

    [[nodiscard]]
    constexpr auto pt1() const -> Point<T> { return mPt1; }
    [[nodiscard]]
    constexpr auto pt2() const -> Point<T> { return mPt2; }
    [[nodiscard]]
    constexpr auto pt3() const -> Point<T> { return mPt3; }
};



//template<typename T>
//Triangle<T>::Triangle()
//  : mPt1(),
//    mPt2(),
//    mPt3()
//{
//}

template<typename T>
constexpr Triangle<T>::Triangle(const Point<T> &pt1,
                                const Point<T> &pt2, 
                                const Point<T> &pt3)
  : mPt1(pt1),
    mPt2(pt2),
    mPt3(pt3)
{
}

//template<typename T>
//Triangle<T>::Triangle(const Triangle<T> &triangle)
//  : mPt1(triangle.mPt1),
//    mPt2(triangle.mPt2),
//    mPt3(triangle.mPt3)
//{
//}
//
//template<typename T>
//Triangle<T>::Triangle(Triangle<T> &&triangle) TL_NOEXCEPT
//  : mPt1(std::move(triangle.mPt1)),
//    mPt2(std::move(triangle.mPt2)),
//    mPt3(std::move(triangle.mPt3))
//{
//}
//
//template<typename T>
//auto Triangle<T>::operator = (const Triangle &triangle) -> Triangle<T> &
//{
//    if (this != &triangle) {
//        this->mPt1 = triangle.mPt1;
//        this->mPt2 = triangle.mPt2;
//        this->mPt3 = triangle.mPt3;
//    }
//
//    return *this;
//}
//
//template<typename T>
//auto Triangle<T>::operator = (Triangle &&triangle) TL_NOEXCEPT -> Triangle<T> &
//{
//    if (this != &triangle) {
//        this->mPt1 = std::move(triangle.mPt1);
//        this->mPt2 = std::move(triangle.mPt2);
//        this->mPt3 = std::move(triangle.mPt3);
//    }
//
//    return *this;
//}

template<typename T>
constexpr auto Triangle<T>::area() const noexcept-> double
{
    return 0.5 * tl::abs(
        mPt1.x() * (mPt2.y() - mPt3.y()) +
        mPt2.x() * (mPt3.y() - mPt1.y()) +
        mPt3.x() * (mPt1.y() - mPt2.y()));
}

template<typename T>
constexpr auto Triangle<T>::perimeter() const noexcept-> double
{
    double side1 = distance(mPt1, mPt2);
    double side2 = distance(mPt2, mPt3);
    double side3 = distance(mPt3, mPt1);
    return side1 + side2 + side3;
}

template<typename T>
constexpr auto Triangle<T>::isValid() const noexcept -> bool
{
    double side1 = distance(mPt1, mPt2);
    double side2 = distance(mPt2, mPt3);
    double side3 = distance(mPt3, mPt1);
    return (side1 + side2 > side3) &&
           (side2 + side3 > side1) &&
           (side3 + side1 > side2);
}

template<typename T>
constexpr auto Triangle<T>::centroid() const -> Point<T>
{
    return Point<T>((mPt1.x() + mPt2.x() + mPt3.x()) / 3.0, (mPt1.y() + mPt2.y() + mPt3.y()) / 3.0);
}

template<typename T>
constexpr auto Triangle<T>::operator==(const Triangle &other) const -> bool
{
    return mPt1 == other.mPt1 && mPt2 == other.mPt2 && mPt3 == other.mPt3;
}


/*! \} */ 

}
