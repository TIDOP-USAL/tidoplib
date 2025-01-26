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

#include "tidop/core/base/defs.h"

namespace tl
{

/*! \addtogroup Rotations
 *  \{
 */

/*!
 * \brief Orientation Interface
 *
 * This class serves as an interface for various representations of rotations and orientations
 * in 3D space. It provides a common interface for different rotation representations such as
 * axis-angle, Euler angles, rotation matrices, and quaternions. This allows easy manipulation
 * and conversion between different rotation representations.
 */
class TL_EXPORT Orientation
{

public:

    /*!
     * \brief Rotation types
     *
     * This enum defines the different types of rotation representations supported.
     */
    enum class Type
    {
        axis_angle,      /*!< Axial-angular representation */
        euler_angles,    /*!< Euler/Tait-Bryan angles representation */
        rotation_matrix, /*!< Rotation matrix representation */
        quaternion       /*!< Quaternion representation */
    };

public:

    /*!
     * \brief Default constructor
     */
    Orientation() = default;

    /*!
     * \brief Virtual destructor
     * Ensures proper cleanup when derived classes are destructed.
     */
    virtual ~Orientation() = default;

    /*!
     * \brief Rotation type
     *
     * Returns the type of the rotation representation used in the derived class.
     *
     * \see Rotation::Type
     * \return The rotation type as an enumeration value.
     */
    virtual auto type() const->Type = 0;

};

/// \cond

template<typename T, int... P>
class RotationConverter;

/*!
 * \brief Base class for rotations
 */
template<typename T>
class OrientationBase;

/// \endcond


/*!
 * \brief Base class for orientations
 *
 * This class serves as a base class for different orientation representations. It provides
 * a common interface for derived classes that represent various types of orientations such as
 * axis-angle, Euler angles, rotation matrices, and quaternions. It also facilitates type conversion
 * between different derived orientation types.
 *
 * The class is templated to allow various derived types to be used, with the ability to convert
 * between these types using the `operator` template methods.
 *
 * \tparam OrientationDerived The derived class representing a specific orientation type (e.g., AxisAngle, EulerAngles, etc.)
 * \tparam T The scalar type (e.g., float, double) used for the orientation representation.
 * \tparam P Additional template parameters that may be used by the derived classes.
 */
template<
    template<typename, int... P>
    class OrientationDerived, typename T, int... P>
class OrientationBase<OrientationDerived<T, P...>>
    : public Orientation
{

private:

    Type rotationType;

public:

    /*!
     * \brief Constructor
     * \param type The type of rotation representation.
     */
    OrientationBase(Type type);

    /*!
     * \brief Copy constructor
     * \param rotation The rotation object to copy.
     */
    OrientationBase(const OrientationBase &rotation);

    /*!
     * \brief Move constructor
     * \param rotation The rotation object to move.
     */
    OrientationBase(OrientationBase &&rotation) TL_NOEXCEPT;

    /*!
     * \brief Virtual destructor
     */
    ~OrientationBase() override = default;

    /*!
     * \brief Copy assignment operator
     * \param rotation The rotation object to assign.
     * \return A reference to this object.
     */
    auto operator=(const OrientationBase &rotation) -> OrientationBase&;

    /*!
     * \brief Move assignment operator
     * \param rotation The rotation object to move.
     * \return A reference to this object.
     */
    auto operator=(OrientationBase &&rotation) TL_NOEXCEPT -> OrientationBase&;

    /*!
     * \brief Get the rotation type
     *
     * Returns the rotation type used in the derived class.
     *
     * \return The rotation type as an enumeration value.
     */
    auto type() const -> Type override;

    /*!
     * \brief Conversion operator to another orientation type
     *
     * Converts this orientation to a different derived orientation type.
     *
     * \tparam OrientationDerived2 The target derived orientation class to convert to.
     * \return The converted orientation object.
     */
    template<
        template<typename>
        class OrientationDerived2>
    operator OrientationDerived2<T>()
    {
        OrientationDerived2<T> orientation;
        RotationConverter<T, P...>::convert(this->derived(), orientation);
        return orientation;
    }

    /*!
     * \brief Conversion operator to another orientation type with additional template parameters
     *
     * Converts this orientation to a different derived orientation type with additional template parameters.
     *
     * \tparam OrientationDerived2 The target derived orientation class to convert to.
     * \tparam Q Additional template parameters for the target orientation class.
     * \return The converted orientation object.
     */
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

    /*!
     * \brief Access the derived object
     *
     * Returns a reference to the derived object by casting this base class to the derived class.
     *
     * \return A reference to the derived orientation object.
     */
    auto derived() -> OrientationDerived<T, P...> &
    {
        return *static_cast<OrientationDerived<T, P...> *>(this);
    }

    /*!
     * \brief Access the derived object (const version)
     *
     * Returns a reference to the derived object by casting this base class to the derived class.
     *
     * \return A reference to the derived orientation object (const version).
     */
    auto derived() const -> const OrientationDerived<T, P...> &
    {
        return *static_cast<const OrientationDerived<T, P...> *>(this);
    }
};



/* OrientationBase implementation */


template<
  template<typename, int... P>
  class OrientationDerived, typename T, int... P>
OrientationBase<OrientationDerived<T, P...>>::OrientationBase(Orientation::Type type)
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
auto OrientationBase<OrientationDerived<T, P...>>::type() const -> Orientation::Type
{
    return rotationType;
}

/*! \} */

} // End namespace tl
