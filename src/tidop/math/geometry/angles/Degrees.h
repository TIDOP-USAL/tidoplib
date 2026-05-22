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
#include "tidop/math/geometry/angles/AngleBase.h"
#include "tidop/math/geometry/angles/detail/Utils.h"

namespace tl
{


/*!
 * \brief Class representing an angle in degrees.
 *
 * The `Degrees` class provides functionality for handling angles in degrees,
 * including methods for normalization and setting/getting the degree, minute,
 * and second components.
 *
 * \tparam T The type used for the angle value, typically a floating-point type like `float` or `double`.
 */
template<Floating T>
class Degrees
  : public AngleBase<Degrees<T>, T>
{

public:

    using value_type = T;
    using AngleBase<Degrees<T>, T>::AngleBase;

public:

    /*!
     * \brief Default constructor.
     *
     * Constructs a `Degrees` object with a default angle value of zero.
     */
    constexpr Degrees()
      : AngleBase<Degrees<T>, T>(AngleUnit::degrees)
    {}

    /*!
     * \brief Constructor with an initial angle value.
     * \param value The initial angle value in degrees.
     */
    constexpr Degrees(T value)
      : AngleBase<Degrees<T>, T>(AngleUnit::degrees, value) 
    {}

    /*!
     * \brief Copy constructor.
     * \param degrees The `Degrees` object to copy from.
     */
    //Degrees(const Degrees &degrees);

    /*!
     * \brief Move constructor.
     * \param degrees The `Degrees` object to move from.
     */
    //Degrees(Degrees &&degrees) noexcept;

    /*!
     * \brief Constructor with degrees, minutes, and seconds components.
     * \param degrees The degree component.
     * \param minutes The minute component.
     * \param seconds The second component.
     */
    constexpr Degrees(int degrees, int minutes, T seconds)
      : AngleBase<Degrees<T>, T>(AngleUnit::degrees)
    {
        this->setValue(detail::isNegative(degrees) *
            (std::abs(degrees) +
                minutes / static_cast<T>(consts::degrees_to_minutes) +
                seconds / static_cast<T>(consts::degrees_to_seconds)));
    }

    /*!
     * \brief Destructor.
     */
    ~Degrees() = default;

    /*!
     * \brief Normalize the angle to the range \f$[-180, 180]\f$.
     *
     * Adjusts the angle value to ensure it is within the standard range for degrees.
     */
    constexpr void normalize()
    {
        T val = std::fmod(this->value(), static_cast<T>(360));
        if (val > 180) val -= 360;
        else if (val <= -180) val += 360;
        this->setValue(val);
    }

    /*!
     * \brief Normalize the angle to the range \f$[0, 360]\f$.
     *
     * Adjusts the angle value to be positive and within a full circle in degrees.
     */
    constexpr void normalizePositive()
    {
        T val = std::fmod(this->value(), static_cast<T>(360));
        if (val < 0) val += 360;
        this->setValue(val);
    }

    /*!
     * \brief Get the degree component of the angle.
     * \return The degree component, as an integer.
     */
    //auto degrees() const -> int;
    [[nodiscard]] constexpr auto degrees() const -> int
    {
        return static_cast<int>(this->value());
    }

    /*!
     * \brief Set the degree component of the angle.
     * \param degrees The degree value to set.
     */
    //void setDegrees(int degrees);
    constexpr void setDegrees(int degrees)
    {
        *this = Degrees(degrees, minutes(), seconds());
    }

    /*!
     * \brief Get the minute component of the angle.
     * \return The minute component, as an integer.
     */
    //auto minutes() const -> int;
    [[nodiscard]] constexpr auto minutes() const -> int
    {
        int seconds = static_cast<int>(std::round(this->value() * consts::degrees_to_seconds));
        seconds = std::abs(seconds % consts::degrees_to_seconds);
        return seconds / consts::minutes_to_seconds;
    }

    /*!
     * \brief Set the minute component of the angle.
     * \param minutes The minute value to set.
     */
    constexpr void setMinutes(int minutes)
    {
        T value = detail::isNegative(this->degrees()) * (std::abs(this->degrees()) +
            minutes / static_cast<T>(consts::degrees_to_minutes) +
            this->seconds() / static_cast<T>(consts::degrees_to_seconds));

        this->setValue(value);
    }

    /*!
     * \brief Get the second component of the angle.
     * \return The second component, as a value of type `T`.
     */
    //auto seconds() const->T;
    [[nodiscard]] constexpr auto seconds() const -> T
    {
        //T total_seconds = std::abs(this->value() * static_cast<T>(3600));
        //return std::fmod(total_seconds, static_cast<T>(60));
        double min = fabs(this->value() - this->degrees()) * consts::degrees_to_minutes;
        return fabs(min - this->minutes()) * consts::minutes_to_seconds;
    }

    /*!
     * \brief Set the second component of the angle.
     * \param seconds The second value to set.
     */
    constexpr void setSeconds(T seconds)
    {
        T value = detail::isNegative(this->degrees()) *
            (std::abs(this->degrees()) +
                std::abs(this->value() - this->degrees()) +
                seconds / static_cast<T>(consts::degrees_to_seconds));
        this->setValue(value);
    }

};



/* Degrees implementation */

//template<typename T>
//Degrees<T>::Degrees(const Degrees &degrees)
//  : AngleBase<Degrees<T>>(degrees)
//{
//}
//
//template<typename T>
//Degrees<T>::Degrees(Degrees &&degrees) noexcept
//  : AngleBase<Degrees<T>>(std::forward<AngleBase<Degrees<T>>>(degrees))
//{
//}

//template<typename T>
//Degrees<T>::Degrees(int degrees, int minutes, T seconds)
//  : AngleBase<Degrees<T>>(Angle::Unit::degrees)
//{
//    this->setValue(detail::isNegative(degrees) *
//                   (std::abs(degrees) +
//                   minutes / static_cast<T>(consts::degrees_to_minutes) +
//                   seconds / static_cast<T>(consts::degrees_to_seconds)));
//}

//template<typename T>
//auto Degrees<T>::operator=(const Degrees &degrees) -> Degrees<T>&
//{
//    if (this != &degrees) {
//        AngleBase<Degrees<T>>::operator=(degrees);
//    }
//
//    return (*this);
//}
//
//template<typename T>
//auto Degrees<T>::operator=(Degrees &&degrees) noexcept -> Degrees<T>&
//{
//    if (this != &degrees) {
//        AngleBase<Degrees<T>>::operator=(std::forward<AngleBase<Degrees<T>>>(degrees));
//    }
//
//    return (*this);
//}

//template<typename T>
//void Degrees<T>::normalize()
//{
//    T value = this->value();
//
//    if (value <= -static_cast<T>(consts::half_circle_deg) ||
//        value > static_cast<T>(consts::half_circle_deg)) {
//
//        value = fmod(value + static_cast<T>(consts::half_circle_deg),
//                     static_cast<T>(consts::full_circle_deg));
//
//        if (value <= consts::zero<T>)
//            value += static_cast<T>(consts::half_circle_deg);
//        else
//            value -= static_cast<T>(consts::half_circle_deg);
//
//        this->setValue(value);
//    }
//}

//template<typename T>
//void Degrees<T>::normalizePositive()
//{
//    T value = this->value();
//
//    if (value < consts::zero<T> ||
//        value >= static_cast<T>(consts::full_circle_deg)) {
//
//        value = fmod(value, static_cast<T>(consts::full_circle_deg));
//
//        if (value < consts::zero<T>)
//            value += static_cast<T>(consts::full_circle_deg);
//
//        this->setValue(value);
//    }
//}

//template<typename T>
//auto Degrees<T>::degrees() const -> int
//{
//    return static_cast<int>(this->value());
//}

//template<typename T>
//void Degrees<T>::setDegrees(int degrees)
//{
//    T value = detail::isNegative(degrees) * (std::abs(degrees) +
//              this->minutes() / static_cast<T>(consts::degrees_to_minutes) +
//              this->seconds() / static_cast<T>(consts::degrees_to_seconds));
//    this->setValue(value);
//}

//template<typename T>
//auto Degrees<T>::minutes() const -> int
//{
//    int seconds = static_cast<int>(std::round(this->value() * consts::degrees_to_seconds));
//    seconds = std::abs(seconds % consts::degrees_to_seconds);
//    return seconds / consts::minutes_to_seconds;
//}

//template<typename T>
//void Degrees<T>::setMinutes(int minutes)
//{
//    T value = detail::isNegative(this->degrees()) * (std::abs(this->degrees()) +
//                                             minutes / static_cast<T>(consts::degrees_to_minutes) +
//                                             this->seconds() / static_cast<T>(consts::degrees_to_seconds));
//
//    this->setValue(value);
//}

//template<typename T>
//auto Degrees<T>::seconds() const -> T
//{
//    double min = fabs(this->value() - this->degrees()) * consts::degrees_to_minutes;
//    return fabs(min - this->minutes()) * consts::minutes_to_seconds;
//}

//template<typename T>
//void Degrees<T>::setSeconds(T seconds)
//{
//    T value = detail::isNegative(this->degrees()) *
//              (std::abs(this->degrees()) +
//              std::abs(this->value() - this->degrees()) +
//              seconds / static_cast<T>(consts::degrees_to_seconds));
//    this->setValue(value);
//}

namespace literals
{

constexpr auto operator"" _deg(long double val)
{
    return Degrees<double>(static_cast<double>(val));
}

constexpr auto operator"" _deg(unsigned long long val)
{
    return Degrees<double>(static_cast<double>(val));
}

}


/*! \} */

} // namespace tl
