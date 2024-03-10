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

#include "tidop/math/algebra/vector.h"
#include "tidop/math/algebra/rotations.h"


namespace tl
{

/*! \addtogroup math
 *  \{
 */


/*! \addtogroup algebra
 *  \{
 */

/*! \addtogroup rotations
*  \{
*/

/*!
* \brief Notación axial-angular
*/
template<typename T>
class AxisAngle
  : public OrientationBase<AxisAngle<T>>
{

private:

    T mAngle;
    Vector<T, 3> mAxis;

public:

    AxisAngle();
    AxisAngle(const AxisAngle &axisAngle);
    AxisAngle(AxisAngle &&axisAngle) TL_NOEXCEPT;
    AxisAngle(T angle, const Vector<T, 3> &axis);
    ~AxisAngle() override = default;

    auto operator = (const AxisAngle &axisAngle) -> AxisAngle &;
    auto operator = (AxisAngle &&axisAngle) TL_NOEXCEPT -> AxisAngle &;

    /*!
     * \brief Angle
     * \return
     */
    auto angle() const -> T;

    /*!
     * \brief Sets the angle
     * \param[in] angle
     * \return
     */
    void setAngle(T angle);

    /*!
     * \brief Axis
     * \return
     */
    auto axis() const -> Vector<T, 3>;

    /*!
     * \brief Sets the Axis
     * \param[in] i Index
     * \return
     */
    auto axis(size_t i) const -> T;

    /*!
     * \brief
     * \param[in] axis
     * \return
     */
    auto setAxis(const Vector<T, 3> &axis) -> void;

};


/* AxisAngle implementation */

template<typename T>
AxisAngle<T>::AxisAngle()
  : OrientationBase<AxisAngle<T>>(Orientation::Type::axis_angle),
    mAngle(0),
    mAxis{1,0,0}
{
    static_assert(std::is_floating_point<T>::value, "Integral type not supported");
}

template<typename T>
AxisAngle<T>::AxisAngle(const AxisAngle &axisAngle)
  : OrientationBase<AxisAngle<T>>(Orientation::Type::axis_angle),
    mAngle(axisAngle.mAngle),
    mAxis(axisAngle.mAxis)
{
    static_assert(std::is_floating_point<T>::value, "Integral type not supported");
}

template<typename T>
AxisAngle<T>::AxisAngle(AxisAngle &&axisAngle) TL_NOEXCEPT
  : OrientationBase<AxisAngle<T>>(std::forward<OrientationBase<AxisAngle<T>>>(axisAngle)),
    mAngle(axisAngle.mAngle),
    mAxis(std::forward<Vector<T, 3>>(axisAngle.mAxis))
{
    static_assert(std::is_floating_point<T>::value, "Integral type not supported");
}

template<typename T>
AxisAngle<T>::AxisAngle(T angle, const Vector<T, 3> &axis)
  : OrientationBase<AxisAngle<T>>(Orientation::Type::axis_angle),
    mAngle(angle),
    mAxis(axis)
{
    static_assert(std::is_floating_point<T>::value, "Integral type not supported");

    mAxis.normalize();
}

template <typename T>
auto AxisAngle<T>::operator = (const AxisAngle<T> &axisAngle) -> AxisAngle &
{
    if (this != &axisAngle) {
        mAngle = axisAngle.mAngle;
        mAxis = axisAngle.mAxis;
    }

    return *this;
}

template <typename T>
auto AxisAngle<T>::operator = (AxisAngle &&axisAngle) TL_NOEXCEPT -> AxisAngle &
{
    if (this != &axisAngle) {
        mAngle = axisAngle.mAngle;
        mAxis = std::forward<Vector<T, 3>>(axisAngle.mAxis);
    }

    return *this;
}

template<typename T>
auto AxisAngle<T>::angle() const -> T
{
    return mAngle;
}

template<typename T>
auto AxisAngle<T>::setAngle(T angle) -> void
{
    mAngle = angle;
}

template<typename T>
auto AxisAngle<T>::axis() const -> Vector<T, 3>
{
    return mAxis;
}

template<typename T>
auto AxisAngle<T>::axis(size_t i) const -> T
{
    TL_ASSERT(i < 3, "");
    return mAxis.at(i);
}

template<typename T>
void AxisAngle<T>::setAxis(const Vector<T, 3> &axis)
{
    mAxis = axis;
    mAxis.normalize();
}

template<typename T>
static inline bool operator == (const AxisAngle<T> &axisAngle1,
                                const AxisAngle<T> &axisAngle2)
{
    return axisAngle1.mAxis == axisAngle2.mAxis &&
           axisAngle1.mAngle == axisAngle2.mAngle;
}

template<typename T>
static inline bool operator != (const AxisAngle<T> &axisAngle1,
                                const AxisAngle<T> &axisAngle2)
{
    return axisAngle1.mAxis != axisAngle2.mAxis &&
           axisAngle1.mAngle != axisAngle2.mAngle;
}

/*! \} */ // end of rotations

/*! \} */ // end of algebra

/*! \} */ // end of math

} // End namespace tl
