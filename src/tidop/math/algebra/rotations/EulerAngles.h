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

#include <utility>

#include "tidop/math/base/Concepts.h"
#include "tidop/math/algebra/rotations/Rotations.h"
#include "tidop/math/algebra/rotations/EulerAnglesAxis.h"
#include "tidop/math/algebra/vector/Vector.h"


namespace tl
{


/*! \addtogroup Rotations
 *  \{
 */


/*!
 * \brief Euler Angles representation class.
 *
 * The `EulerAngles` class represents the orientation of an object using
 * Euler angles, which are defined by three angles corresponding to rotations
 * about the axes. The class allows different conventions (or axis orders)
 * to be used, specified by the template parameter `_axes`.
 *
 * \tparam T The data type of the Euler angles (e.g., `float`, `double`).
 * \tparam _axes The specific rotation sequence (default is `Axes::xyz`).
 */
template<typename T, int _axes = Axes::xyz>
class EulerAngles
    : public OrientationBase<EulerAngles<T, _axes>>
{

    static_assert(Floating<T>, "Integral type not supported");

public:

    T x;  /*!< The first Euler angle (corresponding to the first axis in the rotation sequence). */
    T y;  /*!< The second Euler angle (corresponding to the second axis in the rotation sequence). */
    T z;  /*!< The third Euler angle (corresponding to the third axis in the rotation sequence). */
    Axes axes; /*!< The rotation axes convention being used (e.g., `Axes::xyz`). */

public:

    /*!
     * \brief Default constructor.
     * Initializes the Euler angles to zero.
     */
    EulerAngles();

    /*!
     * \brief Constructor with specific Euler angles.
     * \param[in] x The first Euler angle.
     * \param[in] y The second Euler angle.
     * \param[in] z The third Euler angle.
     */
    EulerAngles(T x, T y, T z);

    /*!
     * \brief Constructor with a vector of Euler angles.
     * \param[in] angles A vector containing the three Euler angles.
     */
    EulerAngles(const Vector<T, 3> &angles);

    /*!
     * \brief Copy constructor.
     * \param[in] eulerAngles The `EulerAngles` instance to copy.
     */
    EulerAngles(const EulerAngles<T, _axes> &eulerAngles);

    /*!
     * \brief Move constructor.
     * \param[in] eulerAngles The `EulerAngles` instance to move.
     */
    EulerAngles(EulerAngles<T, _axes> &&eulerAngles) noexcept;

    ~EulerAngles() = default;

    /*!
     * \brief Copy assignment operator.
     * \param[in] eulerAngles The `EulerAngles` instance to copy.
     * \return A reference to the current instance.
     */
    auto operator=(const EulerAngles<T, _axes> &eulerAngles) -> EulerAngles&;

    /*!
     * \brief Move assignment operator.
     * \param[in] eulerAngles The `EulerAngles` instance to move.
     * \return A reference to the current instance.
     */
    auto operator=(EulerAngles<T, _axes> &&eulerAngles) noexcept -> EulerAngles&;

    /*!
     * \brief Unary plus operator.
     * Returns a copy of the `EulerAngles` object.
     * \return A copy of the current `EulerAngles` instance.
     */
    [[nodiscard]] 
    auto operator+() const -> EulerAngles<T, _axes>;

    /*!
     * \brief Unary minus operator.
     * Negates the Euler angles.
     * \return A new `EulerAngles` instance with negated angles.
     */
    [[nodiscard]] 
    auto operator-() const -> EulerAngles<T, _axes>;
};


template<typename T, int _axes>
EulerAngles<T, _axes>::EulerAngles()
  : x{0},
    y{0},
    z{0},
    axes(static_cast<Axes>(_axes))
{
}

template<typename T, int _axes>
EulerAngles<T, _axes>::EulerAngles(T x, T y, T z)
  : x(x),
    y(y),
    z(z),
    axes(static_cast<Axes>(_axes))
{
}

template<typename T, int _axes>
EulerAngles<T, _axes>::EulerAngles(const Vector<T, 3> &angles)
  : x(angles[0]),
    y(angles[1]),
    z(angles[2]),
    axes(static_cast<Axes>(_axes))
{
}

template<typename T, int _axes>
EulerAngles<T, _axes>::EulerAngles(const EulerAngles<T, _axes> &eulerAngles)
  : x(eulerAngles.x),
    y(eulerAngles.y),
    z(eulerAngles.z),
    axes(static_cast<Axes>(_axes))
{
}

template<typename T, int _axes>
EulerAngles<T, _axes>::EulerAngles(EulerAngles<T, _axes> &&eulerAngles) noexcept
  : x(std::exchange(eulerAngles.x, 0)),
    y(std::exchange(eulerAngles.y, 0)),
    z(std::exchange(eulerAngles.z, 0)),
    axes(static_cast<Axes>(_axes))
{
}

template<typename T, int _axes>
auto EulerAngles<T, _axes>::operator = (const EulerAngles<T, _axes> &eulerAngles) -> EulerAngles<T, _axes>&
{
    if (this != &eulerAngles) {
        x = eulerAngles.x;
        y = eulerAngles.y;
        z = eulerAngles.z;
    }
    return *this;
}

template<typename T, int _axes>
auto EulerAngles<T, _axes>::operator = (EulerAngles<T, _axes> &&eulerAngles) noexcept -> EulerAngles<T, _axes>&
{
    if (this != &eulerAngles) {
        x = std::exchange(eulerAngles.x, 0);
        y = std::exchange(eulerAngles.y, 0);
        z = std::exchange(eulerAngles.z, 0);
    }

    return *this;
}

/* Operaciones unarias */

template<typename T, int _axes>
auto EulerAngles<T, _axes>::operator+() const -> EulerAngles<T, _axes>
{
    return *this;
}

template <typename T, int _axes>
auto EulerAngles<T, _axes>::operator-() const -> EulerAngles<T, _axes>
{
    return EulerAngles<T, _axes>(-this->x,-this->y,-this->z);
}


/*! \} */

} // End namespace tl

