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

#include "tidop/core/base/Concepts.h"
#include "tidop/math/base/Concepts.h"
#include "tidop/math/geometry/angles/detail/AngleConverter.h"

namespace tl
{

/*! \addtogroup AngleConversion
 *  \{
 */


/*!
 * \brief Template class for base angle representation and conversion.
 *
 * This class provides a common base for angle representation in various units
 * and allows for conversions between different derived angle types.
 *
 * \tparam Derived The derived angle class template.
 * \tparam T The type used for the angle value (e.g., `double` or `float`).
 */
template<typename Derived, Floating T>
class AngleBase
{

public:

    using value_type = T;

private:

    T mValue;        /*!< The value of the angle. */

public:

    constexpr AngleBase();

    /*!
     * \brief Construct an angle with a value.
     * \param[in] value The value of the angle.
     */
    explicit constexpr AngleBase(T value);

    /*!
     * \brief Copy constructor.
     * \param[in] angle The angle to copy.
     */
    constexpr AngleBase(const AngleBase &) = default;

    /*!
     * \brief Move constructor.
     * \param[in] angle The angle to move.
     */
    constexpr AngleBase(AngleBase &&) noexcept = default;

    /*!
     * \brief Destructor.
     */
    ~AngleBase() = default;

    /*!
     * \brief Copy assignment operator.
     * \param[in] angle The angle to copy.
     * \return A reference to the updated object.
     */
    constexpr auto operator=(const AngleBase &) -> AngleBase & = default;
    
    /*!
     * \brief Move assignment operator.
     * \param[in] angle The angle to move.
     * \return A reference to the updated object.
     */
    constexpr auto operator=(AngleBase &&) noexcept -> AngleBase & = default;

    /*!
     * \brief Get the value of the angle.
     * \return The value of the angle.
     */
    [[nodiscard]] 
    constexpr auto value() const noexcept -> T { return mValue; }

    /*!
     * \brief Conversion operator to another derived angle type.
     *
     * Converts the current angle to a different derived angle type.
     * \tparam AngleDerived2 The target derived angle class template.
     * \tparam T2 The target value type.
     * \return An instance of the target angle type.
     */
    template<typename TargetAngle>
        requires IsAngle<TargetAngle>
    constexpr operator TargetAngle() const
    {
        using value_type = typename TargetAngle::value_type;

        if constexpr (std::is_same_v<Derived, TargetAngle>) {
            return TargetAngle(static_cast<value_type>(mValue));
        } else {
            TargetAngle converted;
            detail::AngleConverter<T>::convert(derived(), converted);
            return TargetAngle(static_cast<value_type>(converted.value()));
        }
    }

private:

    /*!
     * \brief Get a reference to the derived class instance.
     * \return A reference to the derived class instance.
     */
    [[nodiscard]] 
    constexpr auto derived() -> Derived &;

    /*!
     * \brief Get a constant reference to the derived class instance.
     * \return A constant reference to the derived class instance.
     */
    [[nodiscard]] 
    constexpr auto derived() const -> const Derived &;

    friend class detail::AngleConverter<T>;

protected:

    constexpr void setValue(T angle) noexcept
    {
        mValue = angle;
    }
};




/* AngleBase implementation */

template<typename Derived, Floating T>
constexpr AngleBase<Derived, T>::AngleBase()
    : mValue(static_cast<T>(0))
{
}

template<typename Derived, Floating T>
constexpr AngleBase<Derived, T>::AngleBase(T value)
    : mValue(value)
{
}

template<typename Derived, Floating T>
constexpr auto AngleBase<Derived, T>::derived() -> Derived &
{
    static_assert(IsAngle<Derived>, "Derived class must fulfill the IsAngle concept");
    return *static_cast<Derived *>(this);
}

template<typename Derived, Floating T>
constexpr auto AngleBase<Derived, T>::derived() const -> const Derived &
{
    static_assert(IsAngle<Derived>, "Derived class must fulfill the IsAngle concept");
    return *static_cast<const Derived *>(this);
}

/*! \} */

} // namespace tl
