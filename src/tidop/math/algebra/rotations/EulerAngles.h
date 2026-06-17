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
 * to be used, specified by the template parameter `Order`.
 *
 * \tparam T The data type of the Euler angles (e.g., `float`, `double`).
 * \tparam Order The specific rotation sequence (default is `Axes::xyz`).
 */
template<typename T, Axes Order = Axes::xyz>
class EulerAngles
    : public RotationBase<EulerAngles<T, Order>>
{

    static_assert(Floating<T>, "Integral type not supported");

public:

    using value_type = T;
    using reference = T &;
    using const_reference = const T &;

private:

    Vector<T, 3> mAngles; /*!< The vector containing the three Euler angles. */

public:

    static constexpr Axes axes = Order; /*!< The rotation axes convention being used. */

public:

    /*!
     * \brief Default constructor.
     * Initializes the Euler angles to zero.
     */
    constexpr EulerAngles();

    /*!
     * \brief Constructor with specific Euler angles.
     * \param[in] x The first Euler angle.
     * \param[in] y The second Euler angle.
     * \param[in] z The third Euler angle.
     */
    constexpr EulerAngles(T x, T y, T z);

    /*!
     * \brief Constructor with a vector of Euler angles.
     * \param[in] angles A vector containing the three Euler angles.
     */
    constexpr EulerAngles(Vector<T, 3> angles);

    /*!
     * \brief Copy constructor.
     * \param[in] eulerAngles The `EulerAngles` instance to copy.
     */
    constexpr EulerAngles(const EulerAngles<T, Order> &eulerAngles) = default;

    /*!
     * \brief Move constructor.
     * \param[in] eulerAngles The `EulerAngles` instance to move.
     */
    constexpr EulerAngles(EulerAngles<T, Order> &&eulerAngles) noexcept = default;

    ~EulerAngles() = default;

    /*!
     * \brief Copy assignment operator.
     * \param[in] eulerAngles The `EulerAngles` instance to copy.
     * \return A reference to the current instance.
     */
    constexpr auto operator=(const EulerAngles<T, Order> &eulerAngles) -> EulerAngles& = default;

    /*!
     * \brief Move assignment operator.
     * \param[in] eulerAngles The `EulerAngles` instance to move.
     * \return A reference to the current instance.
     */
    constexpr auto operator=(EulerAngles<T, Order> &&eulerAngles) noexcept -> EulerAngles& = default;

    /*!
     * \brief Access the first Euler angle (corresponding to the first axis in the rotation sequence).
     * \return A reference to the angle.
     */
    [[nodiscard]]
    constexpr auto x() const noexcept -> const_reference { return mAngles[0]; }

    /*!
     * \brief Access the first Euler angle (non-const version).
     * \return A reference to the angle.
     */
    [[nodiscard]]
    constexpr auto x() noexcept -> reference { return mAngles[0]; }

    /*!
     * \brief Access the second Euler angle (corresponding to the second axis in the rotation sequence).
     * \return A reference to the angle.
     */
    [[nodiscard]]
    constexpr auto y() const noexcept -> const_reference { return mAngles[1]; }

    /*!
     * \brief Access the second Euler angle (non-const version).
     * \return A reference to the angle.
     */
    [[nodiscard]]
    constexpr auto y() noexcept -> reference { return mAngles[1]; }

    /*!
     * \brief Access the third Euler angle (corresponding to the third axis in the rotation sequence).
     * \return A reference to the angle.
     */
    [[nodiscard]]
    constexpr auto z() const noexcept -> const_reference { return mAngles[2]; }

    /*!
     * \brief Access the third Euler angle (non-const version).
     * \return A reference to the angle.
     */
    [[nodiscard]]
    constexpr auto z() noexcept -> reference { return mAngles[2]; }

    /*!
     * \brief Gets the underlying vector of angles.
     * \return Reference to the vector of angles.
     */
    [[nodiscard]]
    constexpr auto vector() const noexcept -> const Vector<T, 3>& { return mAngles; }

    /*!
     * \brief Gets the underlying vector of angles (non-const version).
     * \return Reference to the vector of angles.
     */
    [[nodiscard]]
    constexpr auto vector() noexcept -> Vector<T, 3>& { return mAngles; }

    /*!
     * \brief Unary plus operator.
     * Returns a copy of the `EulerAngles` object.
     * \return A copy of the current `EulerAngles` instance.
     */
    [[nodiscard]] 
    auto operator+() const -> EulerAngles<T, Order>;

    /*!
     * \brief Unary minus operator.
     * Negates the Euler angles.
     * \return A new `EulerAngles` instance with negated angles.
     */
    [[nodiscard]] 
    auto operator-() const -> EulerAngles<T, Order>;
};


template<typename T, Axes Order>
constexpr EulerAngles<T, Order>::EulerAngles()
  : mAngles{0, 0, 0}
{
}

template<typename T, Axes Order>
constexpr EulerAngles<T, Order>::EulerAngles(T x, T y, T z)
  : mAngles{x, y, z}
{
}

template<typename T, Axes Order>
constexpr EulerAngles<T, Order>::EulerAngles(Vector<T, 3> angles)
  : mAngles(std::move(angles))
{
}

/* Operaciones unarias */

template<typename T, Axes Order>
auto EulerAngles<T, Order>::operator+() const -> EulerAngles<T, Order>
{
    return *this;
}

template <typename T, Axes Order>
auto EulerAngles<T, Order>::operator-() const -> EulerAngles<T, Order>
{
    return EulerAngles<T, Order>(-this->x(), -this->y(), -this->z());
}


template<typename T, Axes OrderL, Axes OrderR>
[[nodiscard]]
constexpr auto operator == (const EulerAngles<T, OrderL> &lhs,
                            const EulerAngles<T, OrderR> &rhs) -> bool
{
    return OrderL == OrderR && lhs.vector() == rhs.vector();
}

template<typename T, Axes OrderL, Axes OrderR>
[[nodiscard]]
constexpr auto operator != (const EulerAngles<T, OrderL> &lhs,
                            const EulerAngles<T, OrderR> &rhs) -> bool
{
    return !(lhs == rhs);
}

/*! \} */

} // End namespace tl
