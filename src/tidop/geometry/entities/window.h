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
#include <utility>

#include "tidop/core/base/size.h"
#include "tidop/geometry/entities/entity.h"
#include "tidop/geometry/entities/point.h"

namespace tl
{

/*! \addtogroup GeometricEntities
 *  \{
 */


/*!
 * \brief The `Window` class represents a 2D window (bounding box) defined by two points in space.
 *
 * The `Window` class is used to represent a rectangular area in 2D space. The window is defined by two corner points, 
 * typically the bottom-left and top-right corners. This class provides various utility functions for manipulating and querying the window's properties.
 * It also supports conversion between different point types and contains functions for checking point containment within the window.
 */
template<typename Point_t>
class Window
  : public Entity
{

public:

    using value_type = Point_t;
    using scalar = typename Point_t::value_type;

public:

    Point_t pt1;
    Point_t pt2;

public:

    /*!
     * \brief Default constructor
     *
     * Creates an empty window with uninitialized points.
     */
    Window();

    /*!
     * \brief Copy constructor
     * \param[in] window The window to copy.
     *
     * Creates a copy of the provided window.
     */
    Window(const Window &window);

    /*!
     * \brief Move constructor
     * \param[in] window The window to move.
     *
     * Moves the contents of the provided window into the new window.
     */
    Window(Window &&window) TL_NOEXCEPT;

    /*!
     * \brief Constructor using two points
     * \param[in] pt1 One corner of the window.
     * \param[in] pt2 The opposite corner of the window.
     *
     * Initializes the window with the provided points.
     */
    Window(Point_t pt1, Point_t pt2);

    /*!
     * \brief Constructor using a point and dimensions
     * \param[in] point The bottom-left corner of the window.
     * \param[in] width The width of the window.
     * \param[in] height The height of the window.
     *
     * Creates a window defined by a bottom-left corner point and its width and height.
     */
    template<typename T> Window(const Point_t &point, T width, T height);

    /*!
     * \brief Constructor using a point and side length
     * \param[in] pt The bottom-left corner of the window.
     * \param[in] side The length of each side of a square window.
     *
     * Creates a window where both sides are of equal length (a square).
     */
    template<typename T> Window(const Point_t &pt, T side);

    /*!
     * \brief Constructor using a list of points
     * \param[in] vertices The list of points (should represent the vertices of a window).
     *
     * Creates a window by using the list of vertices, typically to define a rectangular area.
     */
    explicit Window(const std::vector<Point_t> &vertices);

    /*!
     * \brief Constructor using a list of points with a different point type
     * \param[in] vertices The list of points (with a different point type) to define the window.
     *
     * Creates a window using a list of points, with a different point type from the template.
     */
    template<typename Point_t2> Window(const std::vector<Point_t2> &vertices);
    
    ~Window() override = default;
    
    /*!
     * \brief Copy assignment operator
     * \param[in] window The window to copy.
     * \return Reference to this window after copying.
     *
     * Copies the contents of the provided window into this window.
     */
    auto operator =(const Window &window)->Window &;

    /*!
     * \brief Move assignment operator
     * \param[in] window The window to move.
     * \return Reference to this window after moving.
     *
     * Moves the contents of the provided window into this window.
     */
    auto operator =(Window &&window) TL_NOEXCEPT->Window &;
    
    /*!
     * \brief Equality operator
     * \param[in] window The window to compare with.
     * \return `true` if both windows are identical, `false` otherwise.
     *
     * Compares two windows for equality by checking if their corner points are the same.
     */
    auto operator ==(const Window &window) const -> bool;

    /*!
     * \brief Type conversion operator
     * \param[in] window The window to convert.
     * \return A new window of a different point type.
     *
     * Converts the current window to one with a different point type.
     */
    template<typename Point_t2> operator Window<Point_t2>() const;
    
    /*!
     * \brief Get the width of the window
     * \return The width of the window.
     *
     * The width is the horizontal distance between `pt1` and `pt2`.
     */
    auto width() const -> scalar;

    /*!
     * \brief Get the height of the window
     * \return The height of the window.
     *
     * The height is the vertical distance between `pt1` and `pt2`.
     */
    auto height() const -> scalar;

    /*!
     * \brief Get the center point of the window
     * \return The center point of the window.
     *
     * The center is calculated as the midpoint between `pt1` and `pt2`.
     */
    auto center() const -> Point_t;

    /*!
     * \brief Check if the window is empty
     * \return `true` if the window is empty, `false` otherwise.
     *
     * A window is considered empty if its width or height is zero.
     */
    auto isEmpty() const -> bool;

    /*!
     * \brief Check if the window is valid
     * \return `true` if the window is valid, `false` otherwise.
     *
     * A window is valid if both width and height are positive values.
     */
    auto isValid() const -> bool;

    /*!
     * \brief Normalize the window
     *
     * Adjusts the window so that `pt1` represents the bottom-left corner
     * and `pt2` represents the top-right corner.
     */
    void normalized();

    /*!
     * \brief Check if the point is inside the window
     * \param[in] pt The point to check.
     * \return `true` if the point is inside the window, `false` otherwise.
     *
     * Determines if a point lies inside the window by comparing its coordinates with `pt1` and `pt2`.
     */
    template<typename Point_t2> auto containsPoint(const Point_t2& pt) const -> bool;

    /*!
     * \brief Check if the window contains another window
     * \param[in] w The window to check.
     * \return `true` if the window contains the other window, `false` otherwise.
     *
     * Determines if the current window fully contains the other window by checking the corners.
     */
    template<typename Point_t2> auto containsWindow(const Window<Point_t2>& w) const -> bool;
 
};



// Definición de métodos

template<typename Point_t>
Window<Point_t>::Window() 
  : Entity(Type::window), 
    pt1(std::numeric_limits<scalar>().max(),
        std::numeric_limits<scalar>().max()), 
    pt2(-std::numeric_limits<scalar>().max(), 
        -std::numeric_limits<scalar>().max()) 
{
}

template<typename Point_t>
Window<Point_t>::Window(const Window &w) 
  : Entity(Type::window), 
    pt1(w.pt1), 
    pt2(w.pt2) 
{
}

template<typename Point_t>
Window<Point_t>::Window(Window &&window) TL_NOEXCEPT
  : Entity(std::forward<Entity>(window)), 
    pt1(std::move(window.pt1)), 
    pt2(std::move(window.pt2)) 
{
}

template<typename Point_t>
Window<Point_t>::Window(Point_t pt1,
                        Point_t pt2)
  : Entity(Type::window), 
    pt1(std::move(pt1)), 
    pt2(std::move(pt2)) 
{
}
    
template<typename Point_t> template<typename T>
Window<Point_t>::Window(const Point_t &pt,
                        T width, T height)
  : Entity(Type::window)
{
    auto half_width = width / consts::two<scalar>;
    auto half_height = height / consts::two<scalar>;

#if (CPP_VERSION >= 17)
    if constexpr (std::is_integral<scalar>::value) {
#else
    if (std::is_integral<scalar>::value) {
#endif

        int dx = static_cast<int>(width) % 2;
        int dy = static_cast<int>(height) % 2;

        this->pt1 = Point_t(pt.x - half_width,
                            pt.y - half_height);
        this->pt2 = Point_t(pt.x + half_width + dx,
                            pt.y + half_height + dy);
    } else {
        this->pt1 = Point_t(pt.x - half_width,
                            pt.y - half_height);
        this->pt2 = Point_t(pt.x + half_width,
                            pt.y + half_height);
    }
}

template<typename Point_t> template<typename T>
Window<Point_t>::Window(const Point_t &pt,
                        T side)
  : Entity(Type::window)
{
    auto half_side = side / consts::two<scalar>;

#if (CPP_VERSION >= 17)
    if constexpr (std::is_integral<scalar>::value) {
#else
    if (std::is_integral<scalar>::value) {
#endif
        int dxy = static_cast<int>(side) % 2;
        this->pt1 = Point_t(pt.x - half_side,
                            pt.y - half_side);
        this->pt2 = Point_t(pt.x + half_side + dxy,
                            pt.y + half_side + dxy);
    } else {
        this->pt1 = Point<T>(pt.x - half_side,
                             pt.y - half_side);
        this->pt2 = Point<T>(pt.x + half_side,
                             pt.y + half_side);
    }
}

template<typename Point_t>
Window<Point_t>::Window(const std::vector<Point_t> &vertices)
  : Entity(Type::window),
    pt1(std::numeric_limits<scalar>().max(),
        std::numeric_limits<scalar>().max()),
    pt2(-std::numeric_limits<scalar>().max(),
        -std::numeric_limits<scalar>().max())
{
    if (vertices.size() >= 2) {
        for (size_t i = 0; i < vertices.size(); i++) {
            if (pt1.x > vertices[i].x) pt1.x = vertices[i].x;
            if (pt1.y > vertices[i].y) pt1.y = vertices[i].y;
            if (pt2.x < vertices[i].x) pt2.x = vertices[i].x;
            if (pt2.y < vertices[i].y) pt2.y = vertices[i].y;
        }
    }
}

template<typename Point_t> template<typename Point_t2>
Window<Point_t>::Window(const std::vector<Point_t2> &vertices)
  : Entity(Type::window),
    pt1(std::numeric_limits<scalar>().max(),
        std::numeric_limits<scalar>().max()),
    pt2(-std::numeric_limits<scalar>().max(),
        -std::numeric_limits<scalar>().max())
{
    if (vertices.size() >= 2) {
        for (size_t i = 0; i < vertices.size(); i++) {
            if (pt1.x > vertices[i].x) pt1.x = numberCast<scalar>(vertices[i].x);
            if (pt1.y > vertices[i].y) pt1.y = numberCast<scalar>(vertices[i].y);
            if (pt2.x < vertices[i].x) pt2.x = numberCast<scalar>(vertices[i].x);
            if (pt2.y < vertices[i].y) pt2.y = numberCast<scalar>(vertices[i].y);
        }
    }
}

template<typename Point_t>
auto Window<Point_t>::operator = (const Window &window) -> Window<Point_t> &
{
    if (this != &window) {
        Entity::operator = (window);
        this->pt1 = window.pt1;
        this->pt2 = window.pt2;
    }

    return *this;
}

template<typename Point_t>
auto Window<Point_t>::operator = (Window &&window) TL_NOEXCEPT -> Window<Point_t> &
{
    if (this != &window) {
        Entity::operator = (std::forward<Entity>(window));
        this->pt1 = std::move(window.pt1);
        this->pt2 = std::move(window.pt2);
    }

    return *this;
}

template<typename Point_t>
auto Window<Point_t>::operator == (const Window &window) const -> bool
{
    return (pt1 == window.pt1 && pt2 == window.pt2);
}

template<typename Point_t> template<typename Point_t2>
Window<Point_t>::operator Window<Point_t2>() const
{
    using scalar2 = typename Point_t2::value_type;
    Window<Point_t2> w; 
    w.pt1.x = numberCast<scalar2>(pt1.x);
    w.pt1.y = numberCast<scalar2>(pt1.y);
    w.pt2.x = numberCast<scalar2>(pt2.x);
    w.pt2.y = numberCast<scalar2>(pt2.y);

    return w;
}

template<typename Point_t>
auto Window<Point_t>::width() const -> scalar
{
    return this->isEmpty() ? consts::zero<scalar> : pt2.x - pt1.x;
}

template<typename Point_t>
auto Window<Point_t>::height() const -> scalar
{
    return this->isEmpty() ? consts::zero<scalar> : pt2.y - pt1.y;
}

template<typename Point_t>
auto Window<Point_t>::center() const -> Point_t
{
    return Point_t((pt1.x + pt2.x) / consts::two<scalar>,
                   (pt1.y + pt2.y) / consts::two<scalar>);
}

template<typename Point_t>
auto Window<Point_t>::isEmpty() const -> bool
{
    return (pt1.x == std::numeric_limits<scalar>().max() &&
            pt1.y == std::numeric_limits<scalar>().max() &&
            pt2.x == -std::numeric_limits<scalar>().max() &&
            pt2.y == -std::numeric_limits<scalar>().max());
}

template<typename Point_t>
auto Window<Point_t>::isValid() const -> bool
{
    return this->width() > consts::zero<scalar> &&
           this->height() > consts::zero<scalar>;
}

template<typename Point_t>
void Window<Point_t>::normalized()
{
    if (!this->isValid()) {
        if (this->pt1.x > this->pt2.x) std::swap(this->pt1.x, this->pt2.x);
        if (this->pt1.y > this->pt2.y) std::swap(this->pt1.y, this->pt2.y);
    }
}

template<typename Point_t> template<typename Point_t2>
auto Window<Point_t>::containsPoint(const Point_t2 &pt) const -> bool
{
    Point_t _pt(pt.x, pt.y);
    return ((pt2.x >= _pt.x) && (pt2.y >= _pt.y) &&
            (pt1.x <= _pt.x) && (pt1.y <= _pt.y));
}

template<typename Point_t> template<typename Point_t2>
auto Window<Point_t>::containsWindow(const Window<Point_t2> &w) const -> bool
{
    Window<Point_t> w2 = w;
    return pt1.x <= w2.pt1.x &&
        pt1.y <= w2.pt1.y &&
        pt2.x >= w2.pt2.x &&
        pt2.y >= w2.pt2.y;
}

using WindowI = Window<Point<int> >;
using WindowD = Window<Point<double> >;
using WindowF = Window<Point<float> >;





/*!
 * \brief Checks if two windows intersect.
 * \param[in] w1 The first window.
 * \param[in] w2 The second window.
 * \return `true` if the two windows intersect, `false` otherwise.
 *
 * This function checks if two windows overlap. It does so by comparing their corner points (`pt1` and `pt2`) to see if the windows' areas intersect.
 */
template<typename T1, typename T2>
auto intersectWindows(const T1 &w1, const T2 &w2) -> bool
{
    return (w1.pt2.x >= w2.pt1.x &&
            w1.pt2.y >= w2.pt1.y &&
            w1.pt1.x <= w2.pt2.x &&
            w1.pt1.y <= w2.pt2.y);
}

/*!
 * \brief Computes the intersection of two windows.
 * \param[in] w1 The first window.
 * \param[in] w2 The second window.
 * \return The intersection of the two windows as a new window.
 *
 * This function calculates the intersection of two windows, returning a new window that represents the overlapping area.
 * If the windows do not intersect, the returned window will be empty.
 */
template<typename T>
auto windowIntersection(const T &w1, const T &w2) -> T
{
    T w;
    if (intersectWindows(w1, w2)) {
        w.pt1.x = std::max(w1.pt1.x, w2.pt1.x);
        w.pt1.y = std::max(w1.pt1.y, w2.pt1.y);
        w.pt2.x = std::min(w1.pt2.x, w2.pt2.x);
        w.pt2.y = std::min(w1.pt2.y, w2.pt2.y);
    }
    return w;
}

/*!
 * \brief Computes the union of two windows.
 * \param[in] w1 The first window.
 * \param[in] w2 The second window.
 * \return The union of the two windows as a new window.
 *
 * This function calculates the union of two windows, creating a new window that represents the smallest bounding window
 * that contains both input windows.
 */
template<typename T>
auto joinWindow(const T &w1, const T &w2) -> T
{
    T w;
    w.pt1.x = std::min(w1.pt1.x, w2.pt1.x);
    w.pt1.y = std::min(w1.pt1.y, w2.pt1.y);
    w.pt2.x = std::max(w1.pt2.x, w2.pt2.x);
    w.pt2.y = std::max(w1.pt2.y, w2.pt2.y);
    return w;
}

/*!
 * \brief Expands a window by specified amounts in the x and y directions.
 * \param[in] w The window to be expanded.
 * \param[in] szx The amount by which to expand in the x-direction.
 * \param[in] szy The amount by which to expand in the y-direction.
 * \return The expanded window.
 *
 * This function expands a window by the given amounts in both x and y directions. The expansion is applied to both the
 * bottom-left corner (`pt1`) and top-right corner (`pt2`).
 */
template<typename T1, typename T2>
auto expandWindow(const T1 &w, T2 szx, T2 szy) -> T1
{
    T1 _w;
    _w.pt1.x = w.pt1.x - szx;
    _w.pt1.y = w.pt1.y - szy;
    _w.pt2.x = w.pt2.x + szx;
    _w.pt2.y = w.pt2.y + szx;
    return _w;
}


/*!
 * \brief Expands a window by a specified amount.
 * \param[in] w The window to be expanded.
 * \param[in] sz The amount by which to expand in both x and y directions.
 * \return The expanded window.
 *
 * This function expands a window by a single value in both the x and y directions (i.e., the window is expanded
 * symmetrically in both dimensions).
 */
template<typename T1, typename T2>
auto expandWindow(const T1 &w, T2 sz) -> T1
{
    return expandWindow(w, sz, sz);
}

/*!
 * \brief Moves a window by specified distances in the x and y directions.
 * \param[in] w The window to be moved.
 * \param[in] dx The distance to move the window in the x-direction.
 * \param[in] dy The distance to move the window in the y-direction.
 * \return A new window, moved by the specified amounts.
 *
 * This function moves the window by the specified amounts along the x and y axes, effectively translating the window's
 * position by the given offsets.
 */
template<typename Point_t, typename T>
auto moveWindow(const Window<Point_t> &w, T dx, T dy) -> Window<Point_t>
{
    Window<Point_t> w_return = w;
    Point<T> t(dx, dy);
    w_return.pt1 += t;
    w_return.pt2 += t;
    return w_return;
}

template<typename Point_t> static inline
auto operator == (const Window<Point_t> &window1, const Window<Point_t> &window2) -> bool
{
    return (window1.pt1 == window2.pt1 &&
            window1.pt2 == window2.pt2);
}

template<typename Point_t> static inline
auto operator != (const Window<Point_t> &window1, const Window<Point_t> &window2) -> bool
{
    return (window1.pt1 != window2.pt1 ||
            window1.pt2 != window2.pt2);
}

/*!
 * \brief Computes the bounding window for a range of points.
 * \param[in] begin The iterator to the beginning of the range.
 * \param[in] end The iterator to the end of the range.
 * \return The bounding window that contains all the points in the range.
 *
 * This function calculates the smallest window that contains all points in the specified range. The resulting window
 * is the one that encloses all the points between the `begin` and `end` iterators.
 */
template<typename It>
auto boundingWindow(It begin, It end) -> Window<iteratorValueType<It>>
{
    using Point_t = iteratorValueType<It>;
    Window<Point_t> window;

    while (begin != end) {
        if (window.pt1.x > begin->x) window.pt1.x = begin->x;
        if (window.pt1.y > begin->y) window.pt1.y = begin->y;
        if (window.pt2.x < begin->x) window.pt2.x = begin->x;
        if (window.pt2.y < begin->y) window.pt2.y = begin->y;
        ++begin;
    }

    return window;
}

/*! \} */ // end of geometry

} // End namespace TL
