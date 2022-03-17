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

#ifndef TL_MATH_ANGLES_H
#define TL_MATH_ANGLES_H

#include "tidop/math/math.h"

#include <utility>

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
 * \brief Convert from degrees, minutes and seconds to decimal degrees
 * \param[in] degrees Degrees
 * \param[in] minutes Minutes
 * \param[in] seconds Seconds
 * \return Decimal degrees
 */
TL_EXPORT double degreesToDecimalDegrees(int degrees, int minutes, double seconds);


/*!
 * \brief Convert from degrees, minutes and seconds to radians
 * \param[in] degrees Degrees
 * \param[in] minutes Minutes
 * \param[in] seconds Seconds
 * \return radians
 */
TL_EXPORT double degreesToRadians(int degrees, int minutes, double seconds);

/*!
 * \brief Convert from degrees, minutes and seconds to gradians
 * \param[in] degrees Degrees
 * \param[in] minutes Minutes
 * \param[in] seconds Seconds
 * \return Gradians
 */
TL_EXPORT double degreesToGradians(int degrees, int minutes, double seconds);

/*!
 * \brief Convert from decimal degrees to degrees, minutes and seconds
 * \param[in] decimalDegrees Decimal degrees
 * \param[out] degrees Degrees
 * \param[out] minutes Minutes
 * \param[out] seconds Seconds
 *
 *
 * <h4>Example</h4>
 * \code
 * int degrees, minutes, seconds;
 * decimalDegreesToDegrees(55.666, degrees, minutes, seconds);
 * \endcode
 */
TL_EXPORT void decimalDegreesToDegrees(double decimalDegrees, int &degrees, int &minutes, double &seconds);

/*!
 * \brief Convert from decimal degrees to radians
 * \param[in] decimalDegrees Decimal degrees
 * \return Radianes
 */
TL_EXPORT double decimalDegreesToRadians(double decimalDegrees);

/*!
 * \brief Convert from decimal degrees to gradians
 * \param[in] decimalDegrees Decimal degrees
 * \return Gradians
 */
TL_EXPORT double decimalDegreesToGradians(double decimalDegrees);

/*!
 * \brief Convert from  radians to degrees, minutes and seconds
 * \param[in] radians Radians
 * \param[out] degrees Degrees
 * \param[out] minutes Minutes
 * \param[out] seconds Seconds
 */
TL_EXPORT void radiansToDegrees(double radians, int &degrees, int &minutes, double &seconds);

/*!
 * \brief Convert from  radians to Degrees
 * \param[in] radians Radians
 * \return Degrees
 */
TL_EXPORT double radiansToDecimalDegrees(double radians);

/*!
 * \brief Convert from radians to gradians
 * \param[in] radians Radians
 * \return Gradians
 */
TL_EXPORT double radiansToGradians(double radians);

/*!
 * \brief Convert from gradians to degrees, minutes and seconds
 * \param[in] gradians Gradians
 * \param[out] degrees Degrees
 * \param[out] minutes Minutes
 * \param[out] seconds Seconds
 */
TL_EXPORT void gradiansToDegrees(double gradians, int &degrees, int &minutes, double &seconds);

/*!
 * \brief Convert from gradians to Degrees
 * \param[in] gradians Gradians
 * \return Degrees
 */
TL_EXPORT double gradiansToDecimalDegrees(double gradians);

/*!
 * \brief Convert from gradians to radians
 * \param[in] gradians Gradians
 * \return radians
 */
TL_EXPORT double gradiansToRadians(double gradians);




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
   * \brief normalize angle 
   */
  virtual void normalize() = 0;

  /*!
   * \brief normalize angle positive
   */
  virtual void normalizePositive() = 0;
};



/*!
 * \brief Angle Base class
 */
template<typename T>
class AngleBase
  : public Angle 
{

public:
  
  AngleBase(Unit unit);
  AngleBase(Unit unit, T value);
  AngleBase(const AngleBase &angle);
  AngleBase(AngleBase &&angle) TL_NOEXCEPT;
  ~AngleBase() override = default;

  AngleBase &operator=(const AngleBase &angle);
  AngleBase &operator=(AngleBase &&angle) TL_NOEXCEPT;
  //void operator=(T angle);

  virtual T value() const;
  virtual void setValue(T angle);

  Unit unit() const override;

private:

  Unit mUnit;
  T mValue;

};



/*!
 * \brief Radians class
 *
 */
template<typename T>
class Radians
  : public AngleBase<T>
{

public:

  Radians();
  Radians(double angle);
  Radians(const Radians &radians);
  Radians(Radians &&radians) TL_NOEXCEPT;
  ~Radians() override = default;

  Radians &operator=(const Radians &radians);
  Radians &operator=(Radians &&radians) TL_NOEXCEPT;

  void normalize() override;
  void normalizePositive() override;

};



/*!
 * \brief Gradians class
 *
 */
template<typename T>
class Gradians
  : public AngleBase<T>
{

public:

  Gradians();
  Gradians(T value);
  Gradians(const Gradians &gradians);
  Gradians(Gradians &&gradians) TL_NOEXCEPT;
  ~Gradians() override = default;

  Gradians &operator=(const Gradians &gradians);
  Gradians &operator=(Gradians &&gradians) TL_NOEXCEPT;

  void normalize() override;
  void normalizePositive() override;

  int degrees() const;
  void setDegrees(int degrees);
  int minutes() const;
  void setMinutes(int minutes);
  T seconds() const;
  void setSeconds(T seconds);
};



/*!
 * \brief Degrees class
 *
 */
template<typename T>
class Degrees
  : public AngleBase<T>
{

public:

  Degrees();
  Degrees(T value);
  Degrees(const Degrees &degrees);
  Degrees(Degrees &&degrees) TL_NOEXCEPT;
  Degrees(int degrees, int minutes, T seconds);
  ~Degrees() override = default;

  Degrees &operator=(const Degrees &degrees);
  Degrees &operator=(Degrees &&degrees) TL_NOEXCEPT;

  void normalize() override;
  void normalizePositive() override;

  int degrees() const;
  void setDegrees(int degrees);
  int minutes() const;
  void setMinutes(int minutes);
  T seconds() const;
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

template<typename T>
AngleBase<T>::AngleBase(Unit unit)
  : mUnit(unit),
    mValue(0)
{
  static_assert(std::is_floating_point<T>::value, "Integral type not supported");
}

template<typename T>
AngleBase<T>::AngleBase(Unit unit, T value)
  : mUnit(unit),
    mValue(value)
{
  static_assert(std::is_floating_point<T>::value, "Integral type not supported");
}

template<typename T> inline
AngleBase<T>::AngleBase(const AngleBase &angle)
  : mUnit(angle.mUnit),
    mValue(angle.mValue)
{
  static_assert(std::is_floating_point<T>::value, "Integral type not supported");
}

template<typename T> inline
AngleBase<T>::AngleBase(AngleBase &&angle) TL_NOEXCEPT
  : mUnit(angle.mUnit),
    mValue(angle.mValue)
{
  static_assert(std::is_floating_point<T>::value, "Integral type not supported");
}

template<typename T> inline
AngleBase<T> &AngleBase<T>::operator=(const AngleBase &angle)
{
  if (this != &angle) {
    mUnit = angle.mUnit;
    mValue = angle.mValue;
  }
  return (*this);
}

template<typename T> inline
AngleBase<T> &AngleBase<T>::operator=(AngleBase &&angle) TL_NOEXCEPT
{
  if (this != &angle) {
    mUnit = angle.mUnit;
    mValue = angle.mValue;
  }
  return (*this);
}

//template<typename T> inline
//void AngleBase<T>::operator=(T angle)
//{
//  mValue = angle;
//}

template<typename T> inline
T AngleBase<T>::value() const
{
  return mValue;
}

template<typename T> inline
void AngleBase<T>::setValue(T angle)
{
  mValue = angle;
}

template<typename T> inline
Angle::Unit AngleBase<T>::unit() const
{
  return mUnit;
}



/* Radians implementation */

template<typename T> inline
Radians<T>::Radians()
  : AngleBase<T>(Angle::Unit::radians)
{
}

template<typename T> inline
Radians<T>::Radians(double angle)
  : AngleBase<T>(Angle::Unit::radians, angle)
{
}

template<typename T> inline
Radians<T>::Radians(const Radians &radians)
  : AngleBase<T>(radians)
{
}

template<typename T> inline
Radians<T>::Radians(Radians &&radians) TL_NOEXCEPT
  : AngleBase<T>(std::forward<AngleBase<T>>(radians))
{
}

template<typename T> inline
Radians<T> &Radians<T>::operator=(const Radians &radians)
{
  if (this != &radians) {
    AngleBase<T>::operator=(radians);
  }
  return (*this);
}

template<typename T> inline
Radians<T> &Radians<T>::operator=(Radians &&radians) TL_NOEXCEPT
{
  if (this != &radians) {
    AngleBase<T>::operator=(std::forward<AngleBase<T>>(radians));
  }
  return (*this);
}

template<typename T> inline
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

template<typename T> inline
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

template<typename T> inline
Gradians<T>::Gradians()
  : AngleBase<T>(Angle::Unit::gradians)
{
}

template<typename T> inline
Gradians<T>::Gradians(T value)
  : AngleBase<T>(Angle::Unit::gradians, value)
{
}

template<typename T> inline
Gradians<T>::Gradians(const Gradians &gradians)
  : AngleBase<T>(gradians)
{
}

template<typename T> inline
Gradians<T>::Gradians(Gradians &&gradians) TL_NOEXCEPT
  : AngleBase<T>(std::forward<AngleBase<T>>(gradians))
{
}

template<typename T> inline
Gradians<T> &Gradians<T>::operator=(const Gradians &gradians)
{
  if (this != &gradians) {
    AngleBase<T>::operator=(gradians);
  }
  return (*this);
}

template<typename T> inline
Gradians<T> &Gradians<T>::operator=(Gradians &&gradians) TL_NOEXCEPT
{
  if (this != &gradians) {
    AngleBase<T>::operator=(std::forward<AngleBase<T>>(gradians));
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

template<typename T> inline
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

template<typename T> inline
int Gradians<T>::degrees() const
{
  return static_cast<int>(this->value());
}

template<typename T> inline
void tl::math::Gradians<T>::setDegrees(int degrees)
{
  T value = isNegative(degrees) * 
            (static_cast<T>(abs(degrees)) +
            ((this->seconds() / consts::quarter_circle_grad) + this->minutes()) / 
            consts::quarter_circle_grad);
  this->setValue(value); 
}

template<typename T> inline
int Gradians<T>::minutes() const
{
  return static_cast<int>(fabs(this->value() - this->degrees()) * consts::quarter_circle_grad);
}

template<typename T> inline
 void tl::math::Gradians<T>::setMinutes(int minutes)
{
  int degrees = this->degrees();
  T value = isNegative(degrees) *
            (static_cast<T>(abs(degrees)) + 
             ((this->seconds() / static_cast<T>(consts::quarter_circle_grad)) +
              static_cast<T>(minutes)) / static_cast<T>(consts::quarter_circle_grad));
  this->setValue(value);
}

template<typename T> inline
T Gradians<T>::seconds() const
{
  double min = fabs(this->value() - this->degrees()) * static_cast<T>(consts::quarter_circle_grad);
  return fabs(min - this->minutes()) * static_cast<T>(consts::quarter_circle_grad);
}

template<typename T> inline
void tl::math::Gradians<T>::setSeconds(T seconds)
{
  int degrees = this->degrees();
  T value = isNegative(degrees) * 
            (static_cast<T>(abs(this->degrees())) +
             ((seconds / consts::quarter_circle_grad) +
              static_cast<T>(this->minutes())) / consts::quarter_circle_grad);
  this->setValue(value);
}



/* Degrees implementation */

template<typename T> inline
Degrees<T>::Degrees()
  : AngleBase<T>(Angle::Unit::degrees)
{
}

template<typename T> inline
Degrees<T>::Degrees(T value)
  : AngleBase<T>(Angle::Unit::degrees, value)
{
}

template<typename T> inline
Degrees<T>::Degrees(const Degrees &degrees)
  : AngleBase<T>(degrees)
{

}

template<typename T> inline
Degrees<T>::Degrees(Degrees &&degrees) TL_NOEXCEPT
  : AngleBase<T>(std::forward<AngleBase<T>>(degrees))
{
}

template<typename T> inline
Degrees<T>::Degrees(int degrees, int minutes, T seconds)
  : AngleBase<T>(Angle::Unit::degrees)
{
  this->setValue(isNegative(degrees) *
                 (abs(degrees) +
                  minutes / static_cast<T>(consts::degrees_to_minutes) +
                  seconds / static_cast<T>(consts::degrees_to_seconds)));
}

template<typename T> inline
Degrees<T> &Degrees<T>::operator=(const Degrees &degrees)
{
  if (this != &degrees) {
    AngleBase<T>::operator=(degrees);
  }
  return (*this);
}

template<typename T> inline
Degrees<T> &Degrees<T>::operator=(Degrees &&degrees) TL_NOEXCEPT
{
  if (this != &degrees) {
    AngleBase<T>::operator=(std::forward<AngleBase<T>>(degrees));
  }
  return (*this);
}

template<typename T> inline
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

template<typename T> inline
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

template<typename T> inline
int Degrees<T>::degrees() const
{
  return static_cast<int>(this->value());
}

template<typename T> inline
void tl::math::Degrees<T>::setDegrees(int degrees)
{
  T value = isNegative(degrees) * (abs(degrees) +
             this->minutes() / static_cast<T>(consts::degrees_to_minutes) +
             this->seconds() / static_cast<T>(consts::degrees_to_seconds));
  this->setValue(value);
}

template<typename T> inline
int Degrees<T>::minutes() const
{
  int seconds = static_cast<int>(std::round(this->value() * consts::degrees_to_seconds));
  seconds = std::abs(seconds % 3600);
  return seconds / 60;
}

template<typename T> inline
void tl::math::Degrees<T>::setMinutes(int minutes)
{
  T value = isNegative(this->degrees()) * (abs(this->degrees()) +
                                           minutes / static_cast<T>(consts::degrees_to_minutes) +
                                           this->seconds() / static_cast<T>(consts::degrees_to_seconds));

  this->setValue(value);
}

template<typename T> inline
T Degrees<T>::seconds() const
{
  double min = fabs(this->value() - this->degrees()) * consts::degrees_to_minutes;
  return fabs(min - this->minutes()) * consts::minutes_to_seconds;
}

template<typename T> inline
void tl::math::Degrees<T>::setSeconds(T seconds)
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


/*! \} */ // end of angleConversion  
  
/*! \} */ // end of Math



} // Fin namespace math

} // Fin namespace tl

#endif // TL_MATH_ANGLES_H 
