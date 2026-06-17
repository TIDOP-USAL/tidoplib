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

#include <type_traits>
#include <utility>

#include "tidop/math/base/Concepts.h"
#include "tidop/math/algebra/vector/Vector.h"
#include "tidop/math/algebra/rotations/Rotations.h"


namespace tl
{

/*! \addtogroup Rotations
 *  \{
 */

/*!
 * \brief Axis-Angle notation for rotations.
 *
 * The `AxisAngle` class represents a rotation in three-dimensional space
 * using an angle of rotation around a specific axis.
 *
 * \tparam T The data type of the angle and axis components (e.g., `float` or `double`).
 */
template<typename T>
class AxisAngle
  : public RotationBase<AxisAngle<T>>
{

    static_assert(Floating<T>, "Integral type not supported");

public:

    using value_type = T;
    using reference = T &;
    using const_reference = const T &;

private:

    T mAngle;           /*!< The rotation angle in radians. */
    Vector<T, 3> mAxis; /*!< The axis of rotation, represented as a 3D vector. */

public:

    /*!
     * \brief Default constructor.
     */
    constexpr AxisAngle();

    /*!
     * \brief Copy constructor.
     * \param[in] axisAngle The `AxisAngle` instance to copy.
     */
    constexpr AxisAngle(const AxisAngle &axisAngle) = default;

    /*!
     * \brief Move constructor.
     * \param[in] axisAngle The `AxisAngle` instance to move.
     */
    constexpr AxisAngle(AxisAngle &&axisAngle) noexcept = default;

    /*!
     * \brief Constructor with angle and axis.
     * \param[in] angle The rotation angle in radians.
     * \param[in] axis The axis of rotation as a 3D vector.
     */
    constexpr AxisAngle(T angle, Vector<T, 3> axis);

    ~AxisAngle() = default;

    /*!
     * \brief Copy assignment operator.
     * \param[in] axisAngle The `AxisAngle` instance to copy.
     * \return A reference to the current instance.
     */
    constexpr auto operator = (const AxisAngle &axisAngle) -> AxisAngle & = default;

    /*!
     * \brief Move assignment operator.
     * \param[in] axisAngle The `AxisAngle` instance to move.
     * \return A reference to the current instance.
     */
    constexpr auto operator = (AxisAngle &&axisAngle) noexcept -> AxisAngle & = default;

    /*!
     * \brief Gets the rotation angle.
     * \return The rotation angle in radians.
     */
    [[nodiscard]]
    constexpr auto angle() const noexcept -> T;

    /*!
     * \brief Sets the rotation angle.
     * \param[in] angle The new rotation angle in radians.
     */
    void setAngle(T angle);

    /*!
     * \brief Gets the x-component of the rotation axis.
     * \return A const reference to the x-component.
     */
    [[nodiscard]]
    constexpr auto x() const noexcept -> const_reference { return mAxis[0]; }

    /*!
     * \brief Gets the x-component of the rotation axis (non-const version).
     * \return A reference to the x-component.
     */
    [[nodiscard]]
    constexpr auto x() noexcept -> reference { return mAxis[0]; }

    /*!
     * \brief Gets the y-component of the rotation axis.
     * \return A const reference to the y-component.
     */
    [[nodiscard]]
    constexpr auto y() const noexcept -> const_reference { return mAxis[1]; }

    /*!
     * \brief Gets the y-component of the rotation axis (non-const version).
     * \return A reference to the y-component.
     */
    [[nodiscard]]
    constexpr auto y() noexcept -> reference { return mAxis[1]; }

    /*!
     * \brief Gets the z-component of the rotation axis.
     * \return A const reference to the z-component.
     */
    [[nodiscard]]
    constexpr auto z() const noexcept -> const_reference { return mAxis[2]; }

    /*!
     * \brief Gets the z-component of the rotation axis (non-const version).
     * \return A reference to the z-component.
     */
    [[nodiscard]]
    constexpr auto z() noexcept -> reference { return mAxis[2]; }

    /*!
     * \brief Gets the rotation axis as a 3D vector.
     * \return A const reference to the axis vector.
     */
    [[nodiscard]]
    constexpr auto vector() const noexcept -> const Vector<T, 3> & { return mAxis; }

    /*!
     * \brief Gets the rotation axis as a 3D vector (non-const version).
     * \return A reference to the axis vector.
     */
    [[nodiscard]]
    constexpr auto vector() noexcept -> Vector<T, 3> & { return mAxis; }

    /*!
     * \brief Sets the rotation axis.
     *
     * The provided vector is normalized before being stored.
     * \param[in] axis The new axis of rotation as a 3D vector.
     */
    void setVector(Vector<T, 3> axis);

};


/* AxisAngle implementation */

template<typename T>
constexpr AxisAngle<T>::AxisAngle()
  : mAngle(0),
    mAxis{1,0,0}
{
}

template<typename T>
constexpr AxisAngle<T>::AxisAngle(T angle, Vector<T, 3> axis)
  : mAngle(angle),
    mAxis(std::move(axis))
{
    mAxis.normalize();
}

template<typename T>
constexpr auto AxisAngle<T>::angle() const noexcept -> T
{
    return mAngle;
}

template<typename T>
auto AxisAngle<T>::setAngle(T angle) -> void
{
    mAngle = angle;
}

template<typename T>
void AxisAngle<T>::setVector(Vector<T, 3> axis)
{
    mAxis = std::move(axis);
    mAxis.normalize();
}

template<typename T>
[[nodiscard]]
constexpr auto operator == (const AxisAngle<T> &lhs,
                            const AxisAngle<T> &rhs) -> bool
{
    return lhs.vector() == rhs.vector() &&
           lhs.angle() == rhs.angle();
}

template<typename T>
[[nodiscard]]
constexpr auto operator != (const AxisAngle<T> &lhs,
                            const AxisAngle<T> &rhs) -> bool
{
    return !(lhs == rhs);
}

/*! \} */

} // End namespace tl
