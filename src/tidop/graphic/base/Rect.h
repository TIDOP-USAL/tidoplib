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

/*! \file Rect.h
 * \brief 2D rectangle class.
 *
 * This file defines the `Rect` class template, which represents an axis-aligned
 * rectangle in 2D space defined by its top-left corner (origin) and its size
 * (width and height). The rectangle provides methods for geometry queries such as
 * containment, corner access, normalization, and conversion to a `BoundingBox`.
 *
 * \ingroup Geometry
 * \see tl::Point, tl::Size, tl::BoundingBox
 */

#pragma once

#include <limits>

#include "tidop/geometry/primitives/Point.h"
#include "tidop/geometry/spatial/BoundingBox.h"
#include "tidop/core/base/Size.h"
#include "tidop/core/base/Concepts.h"

namespace tl
{

/*! \addtogroup Geometry
 *  \{
 */

/*!
 * \class Rect
 * \brief 2D axis-aligned rectangle defined by an origin point and a size.
 *
 * \tparam T Coordinate type (must be arithmetic, e.g., `int`, `float`, `double`).
 *
 * A rectangle is represented by its top-left corner (origin) and its dimensions
 * (width and height). The class provides methods to access corners, test
 * containment, normalize (ensure positive width and height), and convert to
 * a `BoundingBox`. Rectangles can be empty or invalid if width or height
 * is less than or equal to zero.
 *
 * ### Example
 * \code
 * Rect<int> r(10, 20, 100, 50);  // (x=10, y=20, width=100, height=50)
 * auto tl = r.topLeft();         // (10, 20)
 * auto br = r.bottomRight();     // (110, 70)
 * bool contains = r.contains(Point<int>(50, 40)); // true
 * \endcode
 */
template<typename T>
class Rect
{
    static_assert(Arithmetic<T>,
        "Point requires an arithmetic type (integral or floating-point)");

public:

    using value_type = T;

private:

    Point<T> mOrigin;
    Size<T> mSize;

public:

    /*!
     * \brief Default constructor.
     * Constructs a empty Rect object. isValid() returns false
     * and isEmpty() return true.
     */
    constexpr Rect() = default;
    
    /*!
     * \brief Constructs a rectangle with top-left corner (x, y) and width and height
     * \param[in] x Rectangle left coordinate
     * \param[in] y Rectangle top coordinate
     * \param[in] width Rectangle width
     * \param[in] height Rectangle height
     */
    constexpr Rect(T x, T y, T width, T height) noexcept;
    
    /*!
     * \brief Constructs a rectangle with top-left and bottom-right corners
     * \param[in] topLeft Rectangle top-left corner
     * \param[in] bottomRight Rectangle bottom-right corner
     */
    constexpr Rect(const Point<T> &topLeft,
                   const Point<T> &bottomRight) noexcept;
    
    /*!
     * \brief Constructs a rectangle with top-left corner and size (width==height)
     * \param[in] topLeft Rectangle top-left corner
     * \param[in] size Rectangle width and height size
     */
    constexpr Rect(const Point<T> &topLeft,
                   const Size<T> &size) noexcept;
    
    /*!
     * \brief Constructs a rectangle with top-left corner and dimensions (width and height)
     * \param[in] topLeft Rectangle top-left corner
     * \param[in] width Rectangle width
     * \param[in] height Rectangle height
     */
    constexpr Rect(const Point<T> &topLeft,
                   T width, 
                   T height) noexcept;
    
    /*!
     * \brief Copy constructor
     * \param[in] rect Rect object to copy
     */
    constexpr Rect(const Rect &rect) = default;
    
    /*!
     * \brief Move constructor
     * \param[in] rect Rect object to move
     */
    constexpr Rect(Rect &&rect) noexcept = default;

    ~Rect() = default;

    /*!
     * \brief Copy assignment operator
     * \param[in] rect Rect object to copy
     */
    constexpr auto operator = (const Rect &rect) -> Rect & = default;
    
    /*!
     * \brief Move assignment operator
     * \param[in] rect Rect object to move
     */
    constexpr auto operator = (Rect &&rect) noexcept -> Rect& = default;

    /*!
     * \brief Returns the origin (top-left corner).
     * \return Const reference to the origin point.
     */
    [[nodiscard]]
    constexpr auto origin() const noexcept -> const Point<T> &;

    /*!
     * \brief Returns the size of the rectangle.
     * \return Size object containing width and height.
     */
    [[nodiscard]]
    constexpr auto size() const noexcept -> Size<T>;

    /*!
     * \brief Returns the X-coordinate of the top-left corner.
     */
    [[nodiscard]] 
    constexpr auto x() const noexcept -> T;

    /*!
     * \brief Returns the Y-coordinate of the top-left corner.
     */
    [[nodiscard]] 
    constexpr auto y() const noexcept -> T;

    /*!
     * \brief Returns the width of the rectangle.
     */
    [[nodiscard]] 
    constexpr auto width() const noexcept -> T;


    /*!
     * \brief Returns the height of the rectangle.
     */
    [[nodiscard]] 
    constexpr auto height() const noexcept -> T;

    /*!
     * \brief Return top-left corner
     */
    [[nodiscard]]
    constexpr auto topLeft() const noexcept -> Point<T>;
    
    /*!
     * \brief Return top-right corner
     */
    [[nodiscard]]
    constexpr auto topRight() const noexcept -> Point<T>;
    
    /*!
     * \brief Return bottom-right corner
     */
    [[nodiscard]] 
    constexpr auto bottomRight() const noexcept -> Point<T>;
    
    /*!
     * \brief Return bottom-left corner
     */
    [[nodiscard]]
    constexpr auto bottomLeft() const noexcept -> Point<T>;
    
    /*!
     * \brief Check if Rect object is empty. 
     * \return Returns true if the rectangle is empty, otherwise returns false. 
     */
    [[nodiscard]]
    constexpr auto isEmpty() const noexcept -> bool;
    
    /*!
     * \brief Check if Rect object is valid.
     * \return Returns true if the rectangle is valid, otherwise returns false.
     */
    [[nodiscard]]
    constexpr auto isValid() const noexcept -> bool;
    
    /*!
     * \brief Check if a point is contained in the rectangle.
     * \param[in] pt Point to check if is contained.
     * \return Returns true if the rectangle contains the point.
    */
    [[nodiscard]]
    constexpr auto contains(const Point<T> &pt) const noexcept -> bool;
    
    /*!
     * \brief Converts the rectangle to a `BoundingBox`.
     * \return A `BoundingBox<Point<T>>` spanning the rectangle area.
     */
    auto boundingBox() const -> BoundingBox<Point<T>>;
    
    /*!
     * \brief Normalizes the rectangle in-place.
     * Ensures that width and height are positive by swapping the origin
     * and bottom-right corner if necessary.
     */
    constexpr void normalize() noexcept;

    /*!
     * \brief Returns a normalized copy of the rectangle.
     * \return A new `Rect` with positive width and height.
     */
    [[nodiscard]] 
    constexpr auto normalized() const noexcept -> Rect;

    /*!
     * \brief Type conversion
     */
    template<typename T2> operator Rect<T2>() const;

    template<typename T>
    [[nodiscard]]
    constexpr auto operator == (const Rect<T> &other) -> bool
    {
        return (mOrigin == other.mOrigin &&
                mSize == other.mSize);
    }

    template<typename T>
    [[nodiscard]]
    constexpr auto operator != (const Rect<T> &other) -> bool
    {
        return !(*this == other);
    }
};


using Recti = Rect<int>;
using Rectf = Rect<float>;
using Rectd = Rect<double>;

template<typename T> 
constexpr Rect<T>::Rect(T x, T y, T width, T height) noexcept
  : mOrigin{x, y},
    mSize{width, height}
{
}

template<typename T> 
constexpr Rect<T>::Rect(const Point<T> &topLeft,
                        const Point<T> &bottomRight) noexcept
  : mOrigin(topLeft),
    mSize{bottomRight.x() - topLeft.x(), bottomRight.y() - topLeft.y()}
{
}

template<typename T> 
constexpr Rect<T>::Rect(const Point<T> &topLeft,
                        const Size<T> &size) noexcept
  : mOrigin(topLeft),
    mSize(size)
{
}

template<typename T> 
constexpr Rect<T>::Rect(const Point<T> &topLeft,
                        T width, T height) noexcept
  : mOrigin(topLeft),
    mSize{width, height}
{
}

template<typename T>
constexpr auto Rect<T>::origin() const noexcept -> const Point<T> &
{
    return mOrigin;
}

template<typename T>
constexpr auto Rect<T>::size() const noexcept -> Size<T>
{
    return mSize;
}

template<typename T>
constexpr auto Rect<T>::x() const noexcept -> T
{ 
    return mOrigin.x();
}

template<typename T>
constexpr auto Rect<T>::y() const noexcept -> T
{
    return mOrigin.y(); 
}

template<typename T>
constexpr auto Rect<T>::width() const noexcept -> T
{ 
    return mSize.width();
}

template<typename T>
constexpr auto Rect<T>::height() const noexcept -> T 
{
    return mSize.height(); 
}

template<typename T>
constexpr auto Rect<T>::topLeft() const noexcept -> Point<T>
{
    return mOrigin;
}

template<typename T>
constexpr auto Rect<T>::topRight() const noexcept -> Point<T>
{
    return Point<T>(mOrigin.x() + mSize.width(), mOrigin.y());
}

template<typename T>
constexpr auto Rect<T>::bottomRight() const noexcept -> Point<T>
{
    return Point<T>(mOrigin.x() + mSize.width(), 
                    mOrigin.y() + mSize.height());
}

template<typename T>
constexpr auto Rect<T>::bottomLeft() const noexcept -> Point<T>
{
    return Point<T>(mOrigin.x(), mOrigin.y() + mSize.height());
}

template<typename T>
constexpr auto Rect<T>::isEmpty() const noexcept -> bool
{
    return mSize.isEmpty();
}

template<typename T>
constexpr auto Rect<T>::isValid() const noexcept -> bool
{
    return mSize.isValid();
}

template<typename T>
constexpr auto Rect<T>::contains(const Point<T> &pt) const noexcept -> bool
{
    return (this->x() <= pt.x() &&
            pt.x() < this->x() + this->width() &&
            this->y() <= pt.y() &&
            pt.y() < this->y() + this->height());
}

template<typename T>
auto Rect<T>::boundingBox() const -> BoundingBox<Point<T>>
{
    return BoundingBox<Point<T>>(mOrigin, bottomRight());
}

template<typename T>
constexpr void Rect<T>::normalize() noexcept
{
    if (mSize.width() < static_cast<T>(0)) {
        mOrigin.x() += mSize.width();
        mSize.setWidth(-mSize.width());
    }
    if (mSize.height() < static_cast<T>(0)) {
        mOrigin.y() += mSize.height();
        mSize.setHeight(-mSize.height());
    }
}

template<typename T>
constexpr auto Rect<T>::normalized() const noexcept -> Rect
{
    Rect r = *this;
    r.normalize();
    return r;
}

template<typename T> template<typename T2>
Rect<T>::operator Rect<T2>() const
{
    Rect<T2> rect(static_cast<Point<T2>>(this->mOrigin),
                  static_cast<Size<T2>>(this->mSize));
    return rect;
}

template<typename T> 
[[nodiscard]] 
constexpr auto intersect(const Rect<T> &rect1, const Rect<T> &rect2) noexcept -> Rect<T>
{
    const T x1 = std::max(rect1.x(), rect2.x());
    const T y1 = std::max(rect1.y(), rect2.y());

    const T x2 = std::min(rect1.x() + rect1.width(), rect2.x() + rect2.width());
    const T y2 = std::min(rect1.y() + rect1.height(), rect2.y() + rect2.height());

    if (x2 <= x1 || y2 <= y1) {
        return Rect<T>{};
    }

    return Rect<T>{x1, y1, x2 - x1, y2 - y1};
}

/*! \} */ 

}
