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

namespace math
{

/*! \addtogroup math
 *  \{
 */

/*! \addtogroup angleConversion
 * \{
 */

template<typename T>
inline int isNegative(T t)
{
    return t < 0 ? -1 : 1;
}


/*!
 * \brief Angle interface
 */
class Angle
{

public:

    /*!
     * \brief Angle units
     */
    enum class Unit
    {
        radians,
        degrees,
        gradians
    };

public:

    Angle() = default;
    virtual ~Angle() = default;

    /*!
     * \brief Angle unit
     * \see Unit
     */
    virtual Unit unit() const = 0;

    /*!
     * \brief Normalize angle
     */
    virtual void normalize() = 0;

    /*!
     * \brief normalize angle positive
     */
    virtual void normalizePositive() = 0;
};



template<typename T>
class AngleConverter;

template<typename T>
class AngleBase;


template<
  template<typename>
  class AngleDerived, typename T>
class AngleBase<AngleDerived<T>>
    : public Angle
{

private:

    Unit mUnit;
    T mValue;

public:

    AngleBase(Unit unit);
    AngleBase(Unit unit, T value);
    AngleBase(const AngleBase &angle);
    AngleBase(AngleBase &&angle) TL_NOEXCEPT;
    ~AngleBase() override = default;

    auto operator=(const AngleBase &angle) -> AngleBase&;
    auto operator=(AngleBase &&angle) TL_NOEXCEPT -> AngleBase&;

    virtual auto value() const -> T;
    auto unit() const -> Unit override;

    template<
        template<typename>
    class AngleDerived2, typename T2>
    operator AngleDerived2<T2>()
    {
        AngleDerived2<T> angle;
        AngleConverter<T>::convert(derived(), angle.derived());
        return AngleDerived2<T2>(angle.value());
    }

public:

    auto derived() -> AngleDerived<T> &
    {
        return *static_cast<AngleDerived<T> *>(this);
    }

    auto derived() const -> const AngleDerived<T> &
    {
        return *static_cast<const AngleDerived<T> *>(this);
    }

protected:

    virtual void setValue(T angle);

};



/*!
 * \brief Radians class
 *
 */
template<typename T>
class Radians
    : public AngleBase<Radians<T>>
{

public:

    Radians();
    Radians(double angle);
    Radians(const Radians &radians);
    Radians(Radians &&radians) TL_NOEXCEPT;
    ~Radians() override = default;

    auto operator=(const Radians &radians) -> Radians&;
    auto operator=(Radians &&radians) TL_NOEXCEPT -> Radians &;

    void normalize() override;
    void normalizePositive() override;

};



/*!
 * \brief Gradians class
 *
 */
template<typename T>
class Gradians
    : public AngleBase<Gradians<T>>
{

public:

    Gradians();
    Gradians(T value);
    Gradians(const Gradians &gradians);
    Gradians(Gradians &&gradians) TL_NOEXCEPT;
    ~Gradians() override = default;

    auto operator=(const Gradians &gradians) -> Gradians&;
    auto operator=(Gradians &&gradians) TL_NOEXCEPT -> Gradians&;

    void normalize() override;
    void normalizePositive() override;

    auto degrees() const -> int;
    void setDegrees(int degrees);
    auto minutes() const -> int;
    void setMinutes(int minutes);
    auto seconds() const -> T;
    void setSeconds(T seconds);

};



/*!
 * \brief Degrees class
 *
 */
template<typename T>
class Degrees
    : public AngleBase<Degrees<T>>
{

public:

    Degrees();
    Degrees(T value);
    Degrees(const Degrees &degrees);
    Degrees(Degrees &&degrees) TL_NOEXCEPT;
    Degrees(int degrees, int minutes, T seconds);
    ~Degrees() override = default;

    auto operator=(const Degrees &degrees) -> Degrees&;
    auto operator=(Degrees &&degrees) TL_NOEXCEPT -> Degrees&;

    void normalize() override;
    void normalizePositive() override;

    auto degrees() const -> int;
    void setDegrees(int degrees);
    auto minutes() const -> int;
    void setMinutes(int minutes);
    auto seconds() const -> T;
    void setSeconds(T seconds);

};



/*!
 * \brief AngleConverter class
 *
 */
template <typename T>
class AngleConverter
{

private:

    AngleConverter() = default;

public:

    /*!
     * \brief Convert from degrees to radians
     * \param[in] degrees Degrees
     * \param[ou] radians Radians
     */
    static void convert(const Degrees<T> &degrees,
                        Radians<T> &radians);

    /*!
     * \brief Convert from degrees to gradians
     * \param[in] degrees Degrees
     * \param[ou] gradians Gradians
     */
    static void convert(const Degrees<T> &degrees,
                        Gradians<T> &gradians);

    /*!
     * \brief Convert from gradians to radians
     * \param[in] gradians Gradians
     * \param[ou] radians Radians
     */
    static void convert(const Gradians<T> &gradians,
                        Radians<T> &radians);

    /*!
     * \brief Convert from gradians to degrees
     * \param[in] gradians Gradians
     * \param[ou] degrees Degrees
     */
    static void convert(const Gradians<T> &gradians,
                        Degrees<T> &degrees);

    /*!
     * \brief Convert from radians to degrees
     * \param[in] radians Radians
     * \param[ou] Degrees degrees
     */
    static void convert(const Radians<T> &radians,
                        Degrees<T> &degrees);

    /*!
     * \brief Convert from radians to gradians
     * \param[in] radians Radians
     * \param[ou] gradians Gradians
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
AngleBase<AngleDerived<T>> &AngleBase<AngleDerived<T>>::operator=(const AngleBase &angle)
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
AngleBase<AngleDerived<T>> &AngleBase<AngleDerived<T>>::operator=(AngleBase &&angle) TL_NOEXCEPT
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
inline auto AngleBase<AngleDerived<T>>::value() const -> T
{
    return mValue;
}

template<
  template<typename>
  class AngleDerived, typename T>
inline void AngleBase<AngleDerived<T>>::setValue(T angle)
{
    mValue = angle;
}

template<
  template<typename>
  class AngleDerived, typename T>
inline auto AngleBase<AngleDerived<T>>::unit() const -> Unit 
{
    return mUnit;
}


/* Radians implementation */

template<typename T> 
inline Radians<T>::Radians()
  : AngleBase<Radians<T>>(Angle::Unit::radians)
{
}

template<typename T> 
inline Radians<T>::Radians(double angle)
  : AngleBase<Radians<T>>(Angle::Unit::radians, angle)
{
}

template<typename T> inline
Radians<T>::Radians(const Radians &radians)
  : AngleBase<Radians<T>>(radians)
{
}

template<typename T> 
inline Radians<T>::Radians(Radians &&radians) TL_NOEXCEPT
  : AngleBase<Radians<T>>(std::forward<AngleBase<Radians<T>>>(radians))
{
}

template<typename T> 
inline auto Radians<T>::operator=(const Radians &radians) -> Radians<T>&
{
    if (this != &radians) {
        AngleBase<Radians<T>>::operator=(radians);
    }

    return (*this);
}

template<typename T> 
inline auto Radians<T>::operator=(Radians &&radians) TL_NOEXCEPT  -> Radians<T> &
{
    if (this != &radians) {
        AngleBase<Radians<T>>::operator=(std::forward<AngleBase<Radians<T>>>(radians));
    }

    return (*this);
}

template<typename T> 
inline void Radians<T>::normalize()
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
inline void Radians<T>::normalizePositive()
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
inline Gradians<T>::Gradians()
  : AngleBase<Gradians<T>>(Angle::Unit::gradians)
{
}

template<typename T> 
inline Gradians<T>::Gradians(T value)
  : AngleBase<Gradians<T>>(Angle::Unit::gradians, value)
{
}

template<typename T> inline
Gradians<T>::Gradians(const Gradians &gradians)
  : AngleBase<Gradians<T>>(gradians)
{
}

template<typename T> 
inline Gradians<T>::Gradians(Gradians &&gradians) TL_NOEXCEPT
  : AngleBase<Gradians<T>>(std::forward<AngleBase<Gradians<T>>>(gradians))
{
}

template<typename T> 
inline auto Gradians<T>::operator=(const Gradians &gradians) -> Gradians<T>&
{
  if (this != &gradians) {
    AngleBase<Gradians<T>>::operator=(gradians);
  }

  return (*this);
}

template<typename T> 
inline auto Gradians<T>::operator=(Gradians &&gradians) TL_NOEXCEPT -> Gradians<T>&
{
    if (this != &gradians) {
        AngleBase<Gradians<T>>::operator=(std::forward<AngleBase<Gradians<T>>>(gradians));
    }

    return (*this);
}

template<typename T>
inline void Gradians<T>::normalize()
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
inline void Gradians<T>::normalizePositive()
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
inline auto Gradians<T>::degrees() const -> int
{
    return static_cast<int>(this->value());
}

template<typename T> inline
void Gradians<T>::setDegrees(int degrees)
{
    T value = isNegative(degrees) *
              (static_cast<T>(std::abs(degrees)) +
              ((this->seconds() / consts::quarter_circle_grad) + this->minutes()) /
              consts::quarter_circle_grad);
    this->setValue(value);
}

template<typename T> 
inline auto Gradians<T>::minutes() const -> int
{
    return static_cast<int>(fabs(this->value() - this->degrees()) * consts::quarter_circle_grad);
}

template<typename T> 
inline void Gradians<T>::setMinutes(int minutes)
{
    int degrees = this->degrees();
    T value = isNegative(degrees) *
              (static_cast<T>(std::abs(degrees)) +
              ((this->seconds() / static_cast<T>(consts::quarter_circle_grad)) +
              static_cast<T>(minutes)) / static_cast<T>(consts::quarter_circle_grad));
    this->setValue(value);
}

template<typename T> 
inline auto Gradians<T>::seconds() const -> T
{
    double min = fabs(this->value() - this->degrees()) * static_cast<T>(consts::quarter_circle_grad);
    return fabs(min - this->minutes()) * static_cast<T>(consts::quarter_circle_grad);
}

template<typename T>
inline void Gradians<T>::setSeconds(T seconds)
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
inline Degrees<T>::Degrees()
  : AngleBase<Degrees<T>>(Angle::Unit::degrees)
{
}

template<typename T> 
inline Degrees<T>::Degrees(T value)
  : AngleBase<Degrees<T>>(Angle::Unit::degrees, value)
{
}

template<typename T> 
inline Degrees<T>::Degrees(const Degrees &degrees)
  : AngleBase<Degrees<T>>(degrees)
{
}

template<typename T>
inline Degrees<T>::Degrees(Degrees &&degrees) TL_NOEXCEPT
  : AngleBase<Degrees<T>>(std::forward<AngleBase<Degrees<T>>>(degrees))
{
}

template<typename T> 
inline Degrees<T>::Degrees(int degrees, int minutes, T seconds)
  : AngleBase<Degrees<T>>(Angle::Unit::degrees)
{
    this->setValue(isNegative(degrees) *
                   (std::abs(degrees) +
                   minutes / static_cast<T>(consts::degrees_to_minutes) +
                   seconds / static_cast<T>(consts::degrees_to_seconds)));
}

template<typename T>
inline auto Degrees<T>::operator=(const Degrees &degrees) -> Degrees<T>&
{
    if (this != &degrees) {
        AngleBase<Degrees<T>>::operator=(degrees);
    }

    return (*this);
}

template<typename T>
inline auto Degrees<T>::operator=(Degrees &&degrees) TL_NOEXCEPT -> Degrees<T>&
{
    if (this != &degrees) {
        AngleBase<Degrees<T>>::operator=(std::forward<AngleBase<Degrees<T>>>(degrees));
    }

    return (*this);
}

template<typename T> 
inline void Degrees<T>::normalize()
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
inline void Degrees<T>::normalizePositive()
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
inline auto Degrees<T>::degrees() const -> int
{
    return static_cast<int>(this->value());
}

template<typename T> 
inline void Degrees<T>::setDegrees(int degrees)
{
    T value = isNegative(degrees) * (std::abs(degrees) +
                                     this->minutes() / static_cast<T>(consts::degrees_to_minutes) +
                                     this->seconds() / static_cast<T>(consts::degrees_to_seconds));
    this->setValue(value);
}

template<typename T> 
inline auto Degrees<T>::minutes() const -> int
{
    int seconds = static_cast<int>(std::round(this->value() * consts::degrees_to_seconds));
    seconds = std::abs(seconds % consts::degrees_to_seconds);
    return seconds / consts::minutes_to_seconds;
}

template<typename T>
inline void Degrees<T>::setMinutes(int minutes)
{
    T value = isNegative(this->degrees()) * (std::abs(this->degrees()) +
                                             minutes / static_cast<T>(consts::degrees_to_minutes) +
                                             this->seconds() / static_cast<T>(consts::degrees_to_seconds));

    this->setValue(value);
}

template<typename T> inline
auto Degrees<T>::seconds() const -> T
{
    double min = fabs(this->value() - this->degrees()) * consts::degrees_to_minutes;
    return fabs(min - this->minutes()) * consts::minutes_to_seconds;
}

template<typename T> inline
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
inline void AngleConverter<T>::convert(const Degrees<T> &degrees,
                                       Radians<T> &radians)
{
    radians = degrees.value() * consts::deg_to_rad<T>;
}

template<typename T>
inline void AngleConverter<T>::convert(const Degrees<T> &degrees,
                                       Gradians<T> &gradians)
{
    gradians = degrees.value() * consts::deg_to_grad<T>;
}

template<typename T>
inline void AngleConverter<T>::convert(const Gradians<T> &gradians,
                                       Radians<T> &radians)
{
    radians = gradians.value() * consts::grad_to_rad<T>;
}

template<typename T>
inline void AngleConverter<T>::convert(const Gradians<T> &gradians, 
                                       Degrees<T> &degrees)
{
  degrees = gradians.value() * consts::grad_to_deg<T>;
}

template<typename T>
inline void AngleConverter<T>::convert(const Radians<T> &radians, 
                                       Degrees<T> &degrees)
{
  degrees = radians.value() * consts::rad_to_deg<T>;
}

template<typename T>
inline void AngleConverter<T>::convert(const Radians<T> &radians, 
                                       Gradians<T> &gradians)
{
  gradians = radians.value() * consts::rad_to_grad<T>;
}


Radians<double> operator""_rad(long double radians)
{
    return Radians<double>(radians);
}

Gradians<double> operator""_grad(long double gradians)
{
    return Gradians<double>(gradians);
}

Degrees<double> operator""_deg(long double degrees)
{
    return Degrees<double>(degrees);
}

/*! \} */ // end of angleConversion  
  
/*! \} */ // end of Math



} // Fin namespace math

} // Fin namespace tl
