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

#include "tidop/core/defs.h"
#include "tidop/core/exception.h"




namespace tl
{

/*! \addtogroup math
 *  \{
 */


/*! \addtogroup algebra
 *  \{
 */

/*! \defgroup orientation Orientation
 *  \{
 */



/*!
 * \brief Orientation Interface
 */
class TL_EXPORT Orientation
{

public:

    /*!
     * \brief Rotacion types
     */
    enum class Type
    {
        axis_angle,      /*!< Axial-angular */
        euler_angles,    /*!< Euler/TaitBryan angles*/
        rotation_matrix, /*!< Rotation matriz */
        quaternion       /*!< Quaternions */
    };

public:

    Orientation() = default;
    virtual ~Orientation() = default;

    /*!
     * \brief Rotation type
     * \see Rotation::Type
     */
    virtual Type type() const = 0;

};


template<typename T, int... P>
class RotationConverter;

/*!
 * \brief Base class for rotations
 */
template<typename T>
class OrientationBase;

template<
    template<typename, int... P>
    class OrientationDerived, typename T, int... P>
class OrientationBase<OrientationDerived<T, P...>>
    : public Orientation
{

private:

    Type rotationType;

public:

    OrientationBase(Type type);
    OrientationBase(const OrientationBase &rotation);
    OrientationBase(OrientationBase &&rotation) TL_NOEXCEPT;
    ~OrientationBase() override = default;

    auto operator=(const OrientationBase &rotation) -> OrientationBase&;
    auto operator=(OrientationBase &&rotation) TL_NOEXCEPT -> OrientationBase&;

    auto type() const -> Type override;

    template<
        template<typename>
        class OrientationDerived2>
    operator OrientationDerived2<T>()
    {
        OrientationDerived2<T> orientation;
        RotationConverter<T, P...>::convert(this->derived(), orientation);
        return orientation;
    }
        
    template<
        template<typename, int... Q>
        class OrientationDerived2, int...Q>
    operator OrientationDerived2<T, Q...>()
    {
        OrientationDerived2<T, Q...> orientation;
        RotationConverter<T, Q...>::convert(this->derived(), orientation);
        return orientation;
    }
private:

    auto derived() -> OrientationDerived<T, P...> &
    {
        return *static_cast<OrientationDerived<T, P...> *>(this);
    }

    auto derived() const -> const OrientationDerived<T, P...> &
    {
        return *static_cast<const OrientationDerived<T, P...> *>(this);
    }
};



/* OrientationBase implementation */


template<
  template<typename, int... P>
  class OrientationDerived, typename T, int... P>
OrientationBase<OrientationDerived<T, P...>>::OrientationBase(Type type)
  : rotationType(type)
{
}

template<
  template<typename, int... P>
  class OrientationDerived, typename T, int... P>
OrientationBase<OrientationDerived<T, P...>>::OrientationBase(const OrientationBase &rotation)
  : rotationType(rotation.rotationType)
{
}

template<
  template<typename, int... P>
  class OrientationDerived, typename T, int... P>
OrientationBase<OrientationDerived<T, P...>>::OrientationBase(OrientationBase &&rotation) TL_NOEXCEPT
  : rotationType(rotation.rotationType)
{

}

template<
  template<typename, int... P>
  class OrientationDerived, typename T, int... P>
auto OrientationBase<OrientationDerived<T, P...>>::operator=(const OrientationBase &rotation) -> OrientationBase &
{
    if (this != &rotation) {
        this->rotationType = rotation.rotationType;
    }

    return (*this);
}

template<
  template<typename, int... P>
  class OrientationDerived, typename T, int... P>
auto OrientationBase<OrientationDerived<T, P...>>::operator=(OrientationBase &&rotation) TL_NOEXCEPT -> OrientationBase &
{
    if (this != &rotation) {
        this->rotationType = rotation.rotationType;
    }

    return (*this);
}


template<
  template<typename, int... P>
  class OrientationDerived, typename T, int... P>
inline auto OrientationBase<OrientationDerived<T, P...>>::type() const -> Type
{
    return rotationType;
}

/*! \} */ // end of rotation

/*! \} */ // end of algebra

/*! \} */ // end of math

} // End namespace tl
