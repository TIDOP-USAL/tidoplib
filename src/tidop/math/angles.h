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

#include "tidop/math/math.h"

namespace tl
{

/*! \addtogroup MathTools
 *  \{
 */

/*!
 * \defgroup angleConversion Angle conversion
 *
 * \brief Functions and classes for converting between different angle units.
 *
 * This group contains classes and functions for handling angles in different units
 * (degrees, radians, and gradians) and converting between these units.
 *
 * \section angleConversionExample Example
 *
 * The following example demonstrates how to convert an angle from degrees to radians using
 * the `AngleConverter` class:
 *
 * ### Example Usage
 * \code{.cpp}
 * Degrees<double> degrees(135, 34, 27);
 * Radians<double> radians = degrees;
 *
 * Message::info("The angle in radians is: {}", radians.value());
 * \endcode
 *
 * The output will be:
 * The angle in radians is: 2.3662161708
 * 
 * \{
 */

/*!
 * \brief Determines if a value is negative.
 * \tparam T The type of the value.
 * \param t The value to check.
 * \return -1 if the value is negative, 1 otherwise.
 */
template<typename T>
auto isNegative(T t) -> int
{
    return t < 0 ? -1 : 1;
}


/*!
 * \brief Interface for angle representation and manipulation.
 *
 * The `Angle` class provides a base interface for representing angles in
 * different units and includes methods for normalizing these angles.
 */
class Angle
{

public:

    /*!
     * \brief Enumeration of supported angle units.
     */
    enum class Unit
    {
        radians, /*!< Angle in radians. */
        degrees, /*!< Angle in degrees. */
        gradians /*!< Angle in gradians. */
    };

public:

    /*!
     * \brief Default constructor.
     */
    Angle() = default;

    /*!
     * \brief Virtual destructor.
     */
    virtual ~Angle() = default;

    /*!
     * \brief Get the unit of the angle.
     * \return The unit of the angle.
     * \see Unit
     */
    virtual auto unit() const->Unit = 0;

    /*!
     * \brief Normalize the angle.
     *
     * This method adjusts the angle to a standard range (typically within
     * \f$-\pi\f$ to \f$\pi\f$ for radians or \f$-180^\circ\f$ to \f$180^\circ\f$ for degrees).
     */
    virtual void normalize() = 0;

    /*!
     * \brief Normalize the angle to a positive range.
     *
     * This method adjusts the angle to a positive range (typically within
     * \f$0\f$ to \f$2\pi\f$ for radians or \f$0^\circ\f$ to \f$360^\circ\f$ for degrees).
     */
    virtual void normalizePositive() = 0;
};



template<typename T>
class AngleConverter;

template<typename T>
class AngleBase;


/*!
 * \brief Template class for base angle representation and conversion.
 *
 * This class provides a common base for angle representation in various units
 * and allows for conversions between different derived angle types.
 *
 * \tparam AngleDerived The derived angle class template.
 * \tparam T The type used for the angle value (e.g., `double` or `float`).
 */
template<
  template<typename>
  class AngleDerived, typename T>
class AngleBase<AngleDerived<T>>
  : public Angle
{

private:

    Unit mUnit; /*!< The unit of the angle (e.g., radians, degrees). */
    T mValue; /*!< The value of the angle. */

public:

    /*!
     * \brief Construct an angle with a specific unit.
     * \param[in] unit The unit of the angle.
     */
    explicit AngleBase(Unit unit);

    /*!
     * \brief Construct an angle with a specific unit and value.
     * \param[in] unit The unit of the angle.
     * \param[in] value The value of the angle.
     */
    AngleBase(Unit unit, T value);

    /*!
     * \brief Copy constructor.
     * \param[in] angle The angle to copy.
     */
    AngleBase(const AngleBase &angle);

    /*!
     * \brief Move constructor.
     * \param[in] angle The angle to move.
     */
    AngleBase(AngleBase &&angle) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~AngleBase() override = default;

    /*!
     * \brief Copy assignment operator.
     * \param[in] angle The angle to copy.
     * \return A reference to the updated object.
     */
    auto operator=(const AngleBase &angle) -> AngleBase&;

    /*!
     * \brief Move assignment operator.
     * \param[in] angle The angle to move.
     * \return A reference to the updated object.
     */
    auto operator=(AngleBase &&angle) TL_NOEXCEPT -> AngleBase&;

    /*!
     * \brief Get the value of the angle.
     * \return The value of the angle.
     */
    virtual auto value() const->T;

    /*!
     * \brief Get the unit of the angle.
     * \return The unit of the angle.
     */
    auto unit() const->Unit override;

    /*!
     * \brief Conversion operator to another derived angle type.
     *
     * Converts the current angle to a different derived angle type.
     * \tparam AngleDerived2 The target derived angle class template.
     * \tparam T2 The target value type.
     * \return An instance of the target angle type.
     */
    template<
        template<typename>
    class AngleDerived2, typename T2>
    operator AngleDerived2<T2>()
    {
        AngleDerived2<T> angle;
        AngleConverter<T>::convert(derived(), angle);
        return AngleDerived2<T2>(angle.value());
    }

private:

    /*!
     * \brief Get a reference to the derived class instance.
     * \return A reference to the derived class instance.
     */
    auto derived() -> AngleDerived<T> &
    {
        return *static_cast<AngleDerived<T> *>(this);
    }

    /*!
     * \brief Get a constant reference to the derived class instance.
     * \return A constant reference to the derived class instance.
     */
    auto derived() const -> const AngleDerived<T> &
    {
        return *static_cast<const AngleDerived<T> *>(this);
    }

protected:

    virtual void setValue(T angle);

};



/*!
 * \brief Class representing an angle in radians.
 *
 * The `Radians` class inherits from `AngleBase` and provides specific
 * implementations for handling angles in radians.
 *
 * \tparam T The type used for the angle value (e.g., `double` or `float`).
 */
template<typename T>
class Radians
    : public AngleBase<Radians<T>>
{

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes the angle to zero radians.
     */
    Radians();

    /*!
     * \brief Constructor with an initial angle value.
     * \param[in] angle The initial angle value in radians.
     */
    Radians(T angle);

    /*!
     * \brief Copy constructor.
     * \param[in] radians The `Radians` object to copy.
     */
    Radians(const Radians &radians);

    /*!
     * \brief Move constructor.
     * \param[in] radians The `Radians` object to move.
     */
    Radians(Radians &&radians) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~Radians() override = default;

    /*!
     * \brief Copy assignment operator.
     * \param[in] radians The `Radians` object to copy.
     * \return A reference to the updated object.
     */
    auto operator=(const Radians &radians)->Radians &;

    /*!
     * \brief Move assignment operator.
     * \param[in] radians The `Radians` object to move.
     * \return A reference to the updated object.
     */
    auto operator=(Radians &&radians) TL_NOEXCEPT->Radians &;

    /*!
     * \brief Normalize the angle to the range \f$[-\pi, \pi]\f$.
     *
     * Ensures that the angle value is within the standard range for radians.
     */
    void normalize() override;

    /*!
     * \brief Normalize the angle to the range \f$[0, 2\pi]\f$.
     *
     * Ensures that the angle value is positive and within the range of one full circle.
     */
    void normalizePositive() override;

};



/*!
 * \brief Class representing an angle in gradians.
 *
 * The `Gradians` class provides functionality for handling angles in gradians,
 * allowing for normalization and conversion to and from degrees, minutes, and seconds.
 *
 * \tparam T The type used for the angle value, typically a floating-point type like `float` or `double`.
 */
template<typename T>
class Gradians
    : public AngleBase<Gradians<T>>
{

public:

    /*!
     * \brief Default constructor.
     *
     * Constructs a `Gradians` object with a default angle value of zero.
     */
    Gradians();

    /*!
     * \brief Constructor with an initial angle value.
     * \param[in] value The initial angle value in gradians.
     */
    Gradians(T value);

    /*!
     * \brief Copy constructor.
     * \param[in] gradians The `Gradians` object to copy from.
     */
    Gradians(const Gradians &gradians);

    /*!
     * \brief Move constructor.
     * \param[in] gradians The `Gradians` object to move from.
     */
    Gradians(Gradians &&gradians) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~Gradians() override = default;

    /*!
     * \brief Copy assignment operator.
     * \param[in] gradians The `Gradians` object to copy from.
     * \return A reference to this object.
     */
    auto operator=(const Gradians &gradians)->Gradians &;

    /*!
     * \brief Move assignment operator.
     * \param[in] gradians The `Gradians` object to move from.
     * \return A reference to this object.
     */
    auto operator=(Gradians &&gradians) TL_NOEXCEPT->Gradians &;

    /*!
     * \brief Normalize the angle to the range \f$[-200, 200]\f$.
     *
     * Adjusts the angle value to ensure it is within the standard range for gradians.
     */
    void normalize() override;

    /*!
     * \brief Normalize the angle to the range \f$[0, 400]\f$.
     *
     * Adjusts the angle value to be positive and within a full circle in gradians.
     */
    void normalizePositive() override;

    /*!
     * \brief Get the degree component of the angle.
     * \return The degree component, as an integer.
     */
    auto degrees() const -> int;

    /*!
     * \brief Set the degree component of the angle.
     * \param[in] degrees The degree value to set.
     */
    void setDegrees(int degrees);

    /*!
     * \brief Get the minute component of the angle.
     * \return The minute component, as an integer.
     */
    auto minutes() const -> int;

    /*!
     * \brief Set the minute component of the angle.
     * \param minutes The minute value to set.
     */
    void setMinutes(int minutes);

    /*!
     * \brief Get the second component of the angle.
     * \return The second component, as a value of type `T`.
     */
    auto seconds() const->T;

    /*!
     * \brief Set the second component of the angle.
     * \param seconds The second value to set.
     */
    void setSeconds(T seconds);

};



/*!
 * \brief Class representing an angle in degrees.
 *
 * The `Degrees` class provides functionality for handling angles in degrees,
 * including methods for normalization and setting/getting the degree, minute,
 * and second components.
 *
 * \tparam T The type used for the angle value, typically a floating-point type like `float` or `double`.
 */
template<typename T>
class Degrees
    : public AngleBase<Degrees<T>>
{

public:

    /*!
     * \brief Default constructor.
     *
     * Constructs a `Degrees` object with a default angle value of zero.
     */
    Degrees();

    /*!
     * \brief Constructor with an initial angle value.
     * \param value The initial angle value in degrees.
     */
    Degrees(T value);

    /*!
     * \brief Copy constructor.
     * \param degrees The `Degrees` object to copy from.
     */
    Degrees(const Degrees &degrees);

    /*!
     * \brief Move constructor.
     * \param degrees The `Degrees` object to move from.
     */
    Degrees(Degrees &&degrees) TL_NOEXCEPT;

    /*!
     * \brief Constructor with degrees, minutes, and seconds components.
     * \param degrees The degree component.
     * \param minutes The minute component.
     * \param seconds The second component.
     */
    Degrees(int degrees, int minutes, T seconds);

    /*!
     * \brief Destructor.
     */
    ~Degrees() override = default;

    /*!
     * \brief Copy assignment operator.
     * \param[in] degrees The `Degrees` object to copy from.
     * \return A reference to this object.
     */
    auto operator=(const Degrees &degrees)->Degrees &;

    /*!
     * \brief Move assignment operator.
     * \param[in] degrees The `Degrees` object to move from.
     * \return A reference to this object.
     */
    auto operator=(Degrees &&degrees) TL_NOEXCEPT->Degrees &;

    /*!
     * \brief Normalize the angle to the range \f$[-180, 180]\f$.
     *
     * Adjusts the angle value to ensure it is within the standard range for degrees.
     */
    void normalize() override;

    /*!
     * \brief Normalize the angle to the range \f$[0, 360]\f$.
     *
     * Adjusts the angle value to be positive and within a full circle in degrees.
     */
    void normalizePositive() override;

    /*!
     * \brief Get the degree component of the angle.
     * \return The degree component, as an integer.
     */
    auto degrees() const -> int;

    /*!
     * \brief Set the degree component of the angle.
     * \param degrees The degree value to set.
     */
    void setDegrees(int degrees);

    /*!
     * \brief Get the minute component of the angle.
     * \return The minute component, as an integer.
     */
    auto minutes() const -> int;

    /*!
     * \brief Set the minute component of the angle.
     * \param minutes The minute value to set.
     */
    void setMinutes(int minutes);

    /*!
     * \brief Get the second component of the angle.
     * \return The second component, as a value of type `T`.
     */
    auto seconds() const->T;

    /*!
     * \brief Set the second component of the angle.
     * \param seconds The second value to set.
     */
    void setSeconds(T seconds);

};



/*!
 * \brief Class for converting between different angle units.
 *
 * The `AngleConverter` class provides static methods to convert angles
 * between degrees, radians, and gradians.
 *
 * \tparam T The type used for the angle values, typically a floating-point type like `float` or `double`.
 */
template <typename T>
class AngleConverter
{

private:

    AngleConverter() = default;

public:

    /*!
     * \brief Convert an angle from degrees to radians.
     * \param[in] degrees The angle in degrees.
     * \param[out] radians The converted angle in radians.
     */
    static void convert(const Degrees<T> &degrees,
                        Radians<T> &radians);

    /*!
     * \brief Convert an angle from degrees to gradians.
     * \param[in] degrees The angle in degrees.
     * \param[out] gradians The converted angle in gradians.
     */
    static void convert(const Degrees<T> &degrees,
                        Gradians<T> &gradians);

    /*!
     * \brief Convert an angle from gradians to radians.
     * \param[in] gradians The angle in gradians.
     * \param[out] radians The converted angle in radians.
     */
    static void convert(const Gradians<T> &gradians,
                        Radians<T> &radians);

    /*!
     * \brief Convert an angle from gradians to degrees.
     * \param[in] gradians The angle in gradians.
     * \param[out] degrees The converted angle in degrees.
     */
    static void convert(const Gradians<T> &gradians,
                        Degrees<T> &degrees);

    /*!
     * \brief Convert an angle from radians to degrees.
     * \param[in] radians The angle in radians.
     * \param[out] degrees The converted angle in degrees.
     */
    static void convert(const Radians<T> &radians,
                        Degrees<T> &degrees);

    /*!
     * \brief Convert an angle from radians to gradians.
     * \param[in] radians The angle in radians.
     * \param[out] gradians The converted angle in gradians.
     */
    static void convert(const Radians<T> &radians,
                        Gradians<T> &gradians);

};




/* AngleBase implementation */

template<
  template<typename>
  class AngleDerived, typename T>
AngleBase<AngleDerived<T>>::AngleBase(Unit unit)
  : mUnit(unit),
    mValue(0)
{
    static_assert(std::is_floating_point<T>::value, "Integral type not supported");
}

template<
  template<typename>
  class AngleDerived, typename T>
AngleBase<AngleDerived<T>>::AngleBase(Unit unit, T value)
  : mUnit(unit),
    mValue(value)
{
    static_assert(std::is_floating_point<T>::value, "Integral type not supported");
}

template<
  template<typename>
  class AngleDerived, typename T>
AngleBase<AngleDerived<T>>::AngleBase(const AngleBase &angle)
  : mUnit(angle.mUnit),
    mValue(angle.mValue)
{
    static_assert(std::is_floating_point<T>::value, "Integral type not supported");
}

template<
  template<typename>
  class AngleDerived, typename T>
AngleBase<AngleDerived<T>>::AngleBase(AngleBase &&angle) TL_NOEXCEPT
  : mUnit(angle.mUnit),
    mValue(angle.mValue)
{
    static_assert(std::is_floating_point<T>::value, "Integral type not supported");
}

template<
  template<typename>
  class AngleDerived, typename T>
auto AngleBase<AngleDerived<T>>::operator=(const AngleBase& angle) -> AngleBase<AngleDerived<T>>&
{
  if (this != &angle) {
    mUnit = angle.mUnit;
    mValue = angle.mValue;
  }

  return (*this);
}

template<
  template<typename>
  class AngleDerived, typename T>
auto AngleBase<AngleDerived<T>>::operator=(AngleBase&& angle) TL_NOEXCEPT -> AngleBase<AngleDerived<T>>&
{
    if (this != &angle) {
        mUnit = angle.mUnit;
        mValue = angle.mValue;
    }

    return (*this);
}

template<
  template<typename>
  class AngleDerived, typename T>
auto AngleBase<AngleDerived<T>>::value() const -> T
{
    return mValue;
}

template<
  template<typename>
  class AngleDerived, typename T>
void AngleBase<AngleDerived<T>>::setValue(T angle)
{
    mValue = angle;
}

template<
  template<typename>
  class AngleDerived, typename T>
auto AngleBase<AngleDerived<T>>::unit() const -> Unit 
{
    return mUnit;
}


/* Radians implementation */

template<typename T>
Radians<T>::Radians()
  : AngleBase<Radians<T>>(Angle::Unit::radians)
{
}

template<typename T>
Radians<T>::Radians(T angle)
  : AngleBase<Radians<T>>(Angle::Unit::radians, angle)
{
}

template<typename T>
Radians<T>::Radians(const Radians &radians)
  : AngleBase<Radians<T>>(radians)
{
}

template<typename T>
Radians<T>::Radians(Radians &&radians) TL_NOEXCEPT
  : AngleBase<Radians<T>>(std::forward<AngleBase<Radians<T>>>(radians))
{
}

template<typename T>
auto Radians<T>::operator=(const Radians &radians) -> Radians<T>&
{
    if (this != &radians) {
        AngleBase<Radians<T>>::operator=(radians);
    }

    return (*this);
}

template<typename T>
auto Radians<T>::operator=(Radians &&radians) TL_NOEXCEPT  -> Radians<T> &
{
    if (this != &radians) {
        AngleBase<Radians<T>>::operator=(std::forward<AngleBase<Radians<T>>>(radians));
    }

    return (*this);
}

template<typename T>
void Radians<T>::normalize()
{
    T value = this->value();

    if (value <= -consts::pi<T> || value > consts::pi<T>) {
        value = fmod(value + consts::pi<T>, consts::two_pi<T>);

        if (value <= consts::zero<T>)
            value += consts::pi<T>;
        else
            value -= consts::pi<T>;

        this->setValue(value);
    }
}

template<typename T>
void Radians<T>::normalizePositive()
{
    T value = this->value();

    if (value < consts::zero<T> || value >= consts::two_pi<T>) {
        value = fmod(value, consts::two_pi<T>);

        if (value < consts::zero<T>)
            value += consts::two_pi<T>;

        this->setValue(value);
    }
}



/* Gradians implementation */

template<typename T>
Gradians<T>::Gradians()
  : AngleBase<Gradians<T>>(Angle::Unit::gradians)
{
}

template<typename T>
Gradians<T>::Gradians(T value)
  : AngleBase<Gradians<T>>(Angle::Unit::gradians, value)
{
}

template<typename T>
Gradians<T>::Gradians(const Gradians &gradians)
  : AngleBase<Gradians<T>>(gradians)
{
}

template<typename T>
Gradians<T>::Gradians(Gradians &&gradians) TL_NOEXCEPT
  : AngleBase<Gradians<T>>(std::forward<AngleBase<Gradians<T>>>(gradians))
{
}

template<typename T>
auto Gradians<T>::operator=(const Gradians &gradians) -> Gradians<T>&
{
  if (this != &gradians) {
    AngleBase<Gradians<T>>::operator=(gradians);
  }

  return (*this);
}

template<typename T>
auto Gradians<T>::operator=(Gradians &&gradians) TL_NOEXCEPT -> Gradians<T>&
{
    if (this != &gradians) {
        AngleBase<Gradians<T>>::operator=(std::forward<AngleBase<Gradians<T>>>(gradians));
    }

    return (*this);
}

template<typename T>
void Gradians<T>::normalize()
{
    T value = this->value();

    if (value <= -static_cast<T>(consts::half_circle_grad) ||
        value > static_cast<T>(consts::half_circle_grad)) {

        value = fmod(value + consts::half_circle_grad, consts::full_circle_grad);

        if (value <= consts::zero<T>)
            value += static_cast<T>(consts::half_circle_grad);
        else
            value -= static_cast<T>(consts::half_circle_grad);

        this->setValue(value);
    }
}

template<typename T>
void Gradians<T>::normalizePositive()
{
    T value = this->value();

    if (value < consts::zero<T> ||
        value >= static_cast<T>(consts::full_circle_grad)) {

        value = fmod(value, static_cast<T>(consts::full_circle_grad));

        if (value < consts::zero<T>)
            value += static_cast<T>(consts::full_circle_grad);

        this->setValue(value);
    }
}

template<typename T>
auto Gradians<T>::degrees() const -> int
{
    return static_cast<int>(this->value());
}

template<typename T>
void Gradians<T>::setDegrees(int degrees)
{
    T value = isNegative(degrees) *
              (static_cast<T>(std::abs(degrees)) +
              ((this->seconds() / consts::quarter_circle_grad) + this->minutes()) /
              consts::quarter_circle_grad);
    this->setValue(value);
}

template<typename T>
auto Gradians<T>::minutes() const -> int
{
    return static_cast<int>(fabs(this->value() - this->degrees()) * consts::quarter_circle_grad);
}

template<typename T>
void Gradians<T>::setMinutes(int minutes)
{
    int degrees = this->degrees();
    T value = isNegative(degrees) *
              (static_cast<T>(std::abs(degrees)) +
              ((this->seconds() / static_cast<T>(consts::quarter_circle_grad)) +
              static_cast<T>(minutes)) / static_cast<T>(consts::quarter_circle_grad));
    this->setValue(value);
}

template<typename T>
auto Gradians<T>::seconds() const -> T
{
    double min = fabs(this->value() - this->degrees()) * static_cast<T>(consts::quarter_circle_grad);
    return fabs(min - this->minutes()) * static_cast<T>(consts::quarter_circle_grad);
}

template<typename T>
void Gradians<T>::setSeconds(T seconds)
{
    int degrees = this->degrees();
    T value = isNegative(degrees) *
              (static_cast<T>(std::abs(this->degrees())) +
              ((seconds / consts::quarter_circle_grad) +
              static_cast<T>(this->minutes())) / consts::quarter_circle_grad);
    this->setValue(value);
}



/* Degrees implementation */

template<typename T>
Degrees<T>::Degrees()
  : AngleBase<Degrees<T>>(Angle::Unit::degrees)
{
}

template<typename T>
Degrees<T>::Degrees(T value)
  : AngleBase<Degrees<T>>(Angle::Unit::degrees, value)
{
}

template<typename T>
Degrees<T>::Degrees(const Degrees &degrees)
  : AngleBase<Degrees<T>>(degrees)
{
}

template<typename T>
Degrees<T>::Degrees(Degrees &&degrees) TL_NOEXCEPT
  : AngleBase<Degrees<T>>(std::forward<AngleBase<Degrees<T>>>(degrees))
{
}

template<typename T>
Degrees<T>::Degrees(int degrees, int minutes, T seconds)
  : AngleBase<Degrees<T>>(Angle::Unit::degrees)
{
    this->setValue(isNegative(degrees) *
                   (std::abs(degrees) +
                   minutes / static_cast<T>(consts::degrees_to_minutes) +
                   seconds / static_cast<T>(consts::degrees_to_seconds)));
}

template<typename T>
auto Degrees<T>::operator=(const Degrees &degrees) -> Degrees<T>&
{
    if (this != &degrees) {
        AngleBase<Degrees<T>>::operator=(degrees);
    }

    return (*this);
}

template<typename T>
auto Degrees<T>::operator=(Degrees &&degrees) TL_NOEXCEPT -> Degrees<T>&
{
    if (this != &degrees) {
        AngleBase<Degrees<T>>::operator=(std::forward<AngleBase<Degrees<T>>>(degrees));
    }

    return (*this);
}

template<typename T>
void Degrees<T>::normalize()
{
    T value = this->value();

    if (value <= -static_cast<T>(consts::half_circle_deg) ||
        value > static_cast<T>(consts::half_circle_deg)) {

        value = fmod(value + static_cast<T>(consts::half_circle_deg),
                     static_cast<T>(consts::full_circle_deg));

        if (value <= consts::zero<T>)
            value += static_cast<T>(consts::half_circle_deg);
        else
            value -= static_cast<T>(consts::half_circle_deg);

        this->setValue(value);
    }
}

template<typename T>
void Degrees<T>::normalizePositive()
{
    T value = this->value();

    if (value < consts::zero<T> ||
        value >= static_cast<T>(consts::full_circle_deg)) {

        value = fmod(value, static_cast<T>(consts::full_circle_deg));

        if (value < consts::zero<T>)
            value += static_cast<T>(consts::full_circle_deg);

        this->setValue(value);
    }
}

template<typename T>
auto Degrees<T>::degrees() const -> int
{
    return static_cast<int>(this->value());
}

template<typename T>
void Degrees<T>::setDegrees(int degrees)
{
    T value = isNegative(degrees) * (std::abs(degrees) +
                                     this->minutes() / static_cast<T>(consts::degrees_to_minutes) +
                                     this->seconds() / static_cast<T>(consts::degrees_to_seconds));
    this->setValue(value);
}

template<typename T>
auto Degrees<T>::minutes() const -> int
{
    int seconds = static_cast<int>(std::round(this->value() * consts::degrees_to_seconds));
    seconds = std::abs(seconds % consts::degrees_to_seconds);
    return seconds / consts::minutes_to_seconds;
}

template<typename T>
void Degrees<T>::setMinutes(int minutes)
{
    T value = isNegative(this->degrees()) * (std::abs(this->degrees()) +
                                             minutes / static_cast<T>(consts::degrees_to_minutes) +
                                             this->seconds() / static_cast<T>(consts::degrees_to_seconds));

    this->setValue(value);
}

template<typename T>
auto Degrees<T>::seconds() const -> T
{
    double min = fabs(this->value() - this->degrees()) * consts::degrees_to_minutes;
    return fabs(min - this->minutes()) * consts::minutes_to_seconds;
}

template<typename T>
void Degrees<T>::setSeconds(T seconds)
{
    T value = isNegative(this->degrees()) *
              (std::abs(this->degrees()) +
              std::abs(this->value() - this->degrees()) +
              seconds / static_cast<T>(consts::degrees_to_seconds));
    this->setValue(value);
}



/* Angle Converter */

template<typename T>
void AngleConverter<T>::convert(const Degrees<T> &degrees,
                                Radians<T> &radians)
{
    radians = degrees.value() * consts::deg_to_rad<T>;
}

template<typename T>
void AngleConverter<T>::convert(const Degrees<T> &degrees,
                                Gradians<T> &gradians)
{
    gradians = degrees.value() * consts::deg_to_grad<T>;
}

template<typename T>
void AngleConverter<T>::convert(const Gradians<T> &gradians,
                                Radians<T> &radians)
{
    radians = gradians.value() * consts::grad_to_rad<T>;
}

template<typename T>
void AngleConverter<T>::convert(const Gradians<T> &gradians, 
                                Degrees<T> &degrees)
{
  degrees = gradians.value() * consts::grad_to_deg<T>;
}

template<typename T>
void AngleConverter<T>::convert(const Radians<T> &radians, 
                                Degrees<T> &degrees)
{
  degrees = radians.value() * consts::rad_to_deg<T>;
}

template<typename T>
void AngleConverter<T>::convert(const Radians<T> &radians, 
                                Gradians<T> &gradians)
{
  gradians = radians.value() * consts::rad_to_grad<T>;
}


//Radians<double> operator""_rad(long double radians)
//{
//    return Radians<double>(radians);
//}
//
//Gradians<double> operator""_grad(long double gradians)
//{
//    return Gradians<double>(gradians);
//}
//
//Degrees<double> operator""_deg(long double degrees)
//{
//    return Degrees<double>(degrees);
//}

/*! \} */
  
/*! \} */


} // Fin namespace tl
