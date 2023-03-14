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

#ifndef TL_CORE_FLAGS_H
#define TL_CORE_FLAGS_H

#include "config_tl.h"

#include "tidop/core/defs.h"

#include <type_traits>
#include <utility>


namespace tl
{

/*! \addtogroup core
 *  \{
 */


/*!
 * \brief Clase Flag de enums
 *
 * Esta clase permite utilizar un enum como flag en el caso de declarar la
 * enumeración como 'enum class'.
 *
 * Ejemplo de uso:
 * \code
 * enum class ePrueba : int8_t {
 *  flag_0 = (0 << 0),
 *  flag_1 = (1 << 0),
 *  flag_2 = (1 << 1),
 *  flag_3 = (1 << 2),
 *  flag_4 = (1 << 3),
 *  flag_5 = (1 << 4),
 *  flag_6 = (1 << 5),
 *  flag_7 = (1 << 6)
 * };
 *
 * int main(int argc, char *argv[])
 * {
 *
 *   EnumFlags<ePrueba> flag(ePrueba::flag_1);
 *
 *   // Comprueba si el flag esta activo
 *   bool bActive = flag.isEnabled(ePrueba::flag_1);
 *
 *   // Activa un flag
 *   flag2.enable(ePrueba::flag_3);
 *
 *   // Desactiva un flag
 *   flag2.disable(ePrueba::flag_1);
 *
 *   // Invierte un flag
 *   flag2.switchFlag(ePrueba::flag_5);
 *
 *   // Pueden combinarse los enums
 *   EnumFlags<ePrueba> flag2;
 *   flag2 = ePrueba::flag_3 | ePrueba::flag_4;
 *
 *   return 0;
 * }
 *
 * \endcode
 */
template<typename T>
class EnumFlags
{

public:

  /*!
   * \brief Tipo de la enumeración
   */
  using Type = typename std::underlying_type<T>::type; 

public:

  /*!
   * \brief Constructora por defecto
   */
  EnumFlags();

  /*!
   * \brief Constructora de copia
   * \param[in] flag
   */
  EnumFlags(const EnumFlags<T> &flag);
 
  /*!
   * \brief Constructora de movimiento
   * \param[in] flag
   */
  EnumFlags(EnumFlags<T> &&flag) TL_NOEXCEPT;

  /*!
   * \brief Constructora
   * \param[in] flag
   */
  EnumFlags(T flag);

  /*!
   * \brief Destructora
   */
  ~EnumFlags() = default;

  /*!
   * \brief Operador asignación
   * \param[in] flag Objeto EnumFlags
   * \return Referencia al objeto EnumFlags
   */
  EnumFlags<T> &operator = (const EnumFlags<T> &flag);

  /*!
   * \brief Operador asignación de movimiento
   * \param[in] flag Objeto EnumFlags
   * \return Referencia al objeto EnumFlags
   */
  EnumFlags<T> &operator = (EnumFlags<T> &&flag) TL_NOEXCEPT;

  /*!
   * \brief Operador asignación enumeración
   * \param[in] flag enumeracion o unión de ellas
   * \return Referencia al objeto EnumFlags
   */
  EnumFlags<T> &operator = (T flag);

  /*!
   * \brief Comprueba si el flag esta desactivado
   * \param[in] flag Flag que se comprueba
   * \return Verdadero si esta desactivado y falso en caso contrario.
   */
  bool isEnabled(T flag) const;

  /*!
   * \brief Comprueba si el flag esta desactivado
   * \param[in] flag Flag que se comprueba
   * \return Verdadero si esta desactivado y falso en caso contrario.
   */
  bool isDisabled(T flag) const;

  /*!
   * \brief Activa un flag
   * \param[in] flag Flag que se activa
   */
  void enable(T flag);

  /*!
   * \brief Desactiva un flag
   * \param[in] flag Flag que se desactiva
   */
  void disable(T flag);

  /*!
   * \brief Activa o desactiva un flag
   * \param[in] flag Flag que se desactiva
   * \param[in] active Verdadero para activar el flag
   */
  void activeFlag(T flag, bool active);

  /*!
   * \brief Invierte un flag
   * \param[in] flag Flag que se invierte
   */
  void switchFlag(T flag);

  /*!
   * \brief Pone a cero todos los flags
   */
  void clear();

  /*!
   * \brief Devuelve los flags
   * \return
   */
  T flags() const;

#ifdef TL_ENABLE_DEPRECATED_METHODS

  /*!
   * \brief Comprueba si el flag esta activo
   * \param[in] flag Flag que se comprueba
   * \return Verdadero si esta activo y falso en caso contrario.
   * \deprecated Use 'flags()' en su lugar 
   */
  TL_DEPRECATED("isEnabled()", 2.2)
  bool isActive(T flag) const;

  /*!
   * \brief Activa un flag
   * \param[in] flag Flag que se activa
   */
  TL_DEPRECATED("enable()", 2.2)
  void flagOn(T flag);

  /*!
   * \brief Desactiva un flag
   * \param[in] flag Flag que se desactiva
   */
  TL_DEPRECATED("disable()", 2.2)
  void flagOff(T flag);


#endif // TL_ENABLE_DEPRECATED_METHODS

private:

  /*!
   * \brief mFlag
   */
  Type mFlag;

};

template<typename T>
inline EnumFlags<T>::EnumFlags() 
  : mFlag(0) 
{
}

template<typename T>
inline EnumFlags<T>::EnumFlags(const EnumFlags<T> &flag)
  : mFlag(flag.mFlag) 
{

}

template<typename T>
inline EnumFlags<T>::EnumFlags(EnumFlags<T> &&flag) TL_NOEXCEPT
  : mFlag(std::move(flag.mFlag)) 
{

}


template<typename T>
inline EnumFlags<T>::EnumFlags(T flag) 
  : mFlag(static_cast<Type>(flag))
{
}

template<typename T>
inline EnumFlags<T> &EnumFlags<T>::operator = (const EnumFlags<T> &flag)
{
  if (this != &flag) {
    this->mFlag = flag.mFlag;
  }
  return *this;
}

template<typename T>
inline EnumFlags<T> &EnumFlags<T>::operator = (EnumFlags<T> &&flag) TL_NOEXCEPT
{
  if (this != &flag) {
    this->mFlag = std::move(flag.mFlag);
  }
  return *this;
}

template<typename T>
inline EnumFlags<T> &EnumFlags<T>::operator = (T flag)
{
  mFlag = static_cast<Type>(flag);
  return *this;
}

template<typename T>
inline bool EnumFlags<T>::isEnabled(T flag) const
{
  return 0 != (mFlag & static_cast<Type>(flag));
}

template<typename T>
inline bool EnumFlags<T>::isDisabled(T flag) const
{
  return 0 == (mFlag & static_cast<Type>(flag));
}

template<typename T>
inline void tl::EnumFlags<T>::enable(T flag)
{
  mFlag |= static_cast<Type>(flag);
}

template<typename T>
inline void EnumFlags<T>::disable(T flag)
{
  mFlag &= ~static_cast<Type>(flag);
}

template<typename T>
inline void EnumFlags<T>::activeFlag(T flag, bool active)
{
  if (active) 
    enable(flag);
  else 
    disable(flag);
}

template<typename T>
inline void EnumFlags<T>::switchFlag(T flag)
{
  if (isEnabled(flag)) 
    disable(flag);
  else 
    enable(flag);
}

template<typename T>
inline void EnumFlags<T>::clear()
{
  mFlag = static_cast<Type>(0);
}

template<typename T>
inline T EnumFlags<T>::flags() const
{
  return static_cast<T>(mFlag);
}

#ifdef TL_ENABLE_DEPRECATED_METHODS

template<typename T> inline
bool EnumFlags<T>::isActive(T flag) const
{
  return 0 != (mFlag & static_cast<Type>(flag));
}

template<typename T> inline
void EnumFlags<T>::flagOn(T flag)
{
  mFlag |= static_cast<Type>(flag);
}

template<typename T> inline
void EnumFlags<T>::flagOff(T flag)
{
  mFlag &= ~static_cast<Type>(flag);
}

#endif // TL_ENABLE_DEPRECATED_METHODS




/*!
 * \brief Permite operaciones a nivel de bit con un 'enum class'
 *
 * Debe añadirse debajo de la declaración del enum
 *
 * \code
 * enum class ePrueba : int8_t {
 *  flag01 = (1 << 0),
 *  flag02 = (1 << 1),
 *  flag03 = (1 << 2),
 *  flag04 = (1 << 3),
 *  flag05 = (1 << 4),
 *  flag06 = (1 << 5),
 *  flag07 = (1 << 6)
 * };
 *
 * ALLOW_BITWISE_FLAG_OPERATIONS(ePrueba)
 *
 * \endcode
 */
#define ALLOW_BITWISE_FLAG_OPERATIONS(T_FLAG)                       \
inline T_FLAG operator | (T_FLAG flag1, T_FLAG flag2)               \
{                                                                   \
  return static_cast<T_FLAG> (                                      \
    static_cast<std::underlying_type<T_FLAG>::type>(flag1) |        \
    static_cast<std::underlying_type<T_FLAG>::type>(flag2)          \
  );                                                                \
}                                                                   \
                                                                    \
inline T_FLAG operator & (T_FLAG flag1, T_FLAG flag2)               \
{                                                                   \
  return static_cast<T_FLAG> (                                      \
    static_cast<std::underlying_type<T_FLAG>::type>(flag1) &        \
    static_cast<std::underlying_type<T_FLAG>::type>(flag2)          \
  );                                                                \
}                                                                   \
                                                                    \
inline T_FLAG operator ^ (T_FLAG flag1, T_FLAG flag2)               \
{                                                                   \
  return static_cast<T_FLAG> (                                      \
    static_cast<std::underlying_type<T_FLAG>::type>(flag1) ^        \
    static_cast<std::underlying_type<T_FLAG>::type>(flag2)          \
  );                                                                \
}                                                                   \
                                                                    \
inline T_FLAG operator ~ (T_FLAG flag)                              \
{                                                                   \
  return static_cast<T_FLAG> (                                      \
    ~static_cast<std::underlying_type<T_FLAG>::type>(flag)          \
  );                                                                \
}                                                                   \



template<typename T>
class Flags
{

public:

  /*!
   * \brief Tipo del flag
   */
  using Type = T; 

public:

  /*!
   * \brief Constructora por defecto
   */
  Flags();

  /*!
   * \brief Constructora de copia
   */
  Flags(const Flags &flag);

  Flags(Flags &&flag) TL_NOEXCEPT;

  /*!
   * \brief Constructora de lista
   * \param[in] flags listado de flags activos
   */
  Flags(std::initializer_list<T> flags);

  /*!
   * \brief Destructora
   */
  ~Flags() = default;

  /*!
   * \brief Operador asignación
   * \param flag enumeracion o unión de ellas
   * \return Referencia al objeto EnumFlags
   */
  Flags &operator = (const Flags<T> &flag);

  /*!
   * \brief Operador asignación de movimiento
   * \param flag enumeracion o unión de ellas
   * \return Referencia al objeto EnumFlags
   */
  Flags &operator = (Flags<T> &&flag) TL_NOEXCEPT;

  /*!
   * \brief Comprueba si el flag esta desactivado
   * \param[in] flag Flag que se comprueba
   * \return Verdadero si esta desactivado y falso en caso contrario.
   */
  bool isEnabled(T flag) const;

  /*!
   * \brief Comprueba si el flag esta desactivado
   * \param[in] flag Flag que se comprueba
   * \return Verdadero si esta desactivado y falso en caso contrario.
   */
  bool isDisabled(T flag) const;

  /*!
   * \brief Activa un flag
   * \param flag Flag que se activa
   */
  void enable(T flag);

  /*!
   * \brief Desactiva un flag
   * \param flag Flag que se desactiva
   */
  void disable(T flag);

  /*!
   * \brief Invierte un flag
   * \param flag Flag que se invierte
   */
  void switchFlag(T flag);

  /*!
   * \brief Pone a cero todos los flags
   */
  void clear();
   
  /*!
   * \brief Devuelve los flags
   * \return
   */
  T flags() const;

#ifdef TL_ENABLE_DEPRECATED_METHODS

  /*!
   * \brief Comprueba si el flag esta activo
   * \param flag Flag que se comprueba
   * \return Verdadero si esta activo y falso en caso contrario.
   */
  TL_DEPRECATED("isEnabled()", 2.2)
  bool isActive(T flag) const;
  
  /*!
   * \brief Activa un flag
   * \param flag Flag que se activa
   */
  TL_DEPRECATED("enable()", 2.2)
  void flagOn(T flag);

  /*!
   * \brief Desactiva un flag
   * \param flag Flag que se desactiva
   */
  TL_DEPRECATED("disable()", 2.2)
  void flagOff(T flag);

#endif // TL_ENABLE_DEPRECATED_METHODS


private:

  /*!
   * \brief mFlag
   */
  Type mFlag;

};

/* Definición de tipos */

using Flags_8 = Flags<uint8_t>;
using Flags_16 = Flags<uint16_t>;
using Flags_32 = Flags<uint32_t>;
using Flags_64 = Flags<uint64_t>;


template<typename T>
inline Flags<T>::Flags()
  : mFlag(0)
{
  static_assert(std::is_integral<T>::value, "Type not supported. Flags only supports integer types");
}

template<typename T> 
inline Flags<T>::Flags(const Flags &flag) 
  : mFlag(flag.mFlag)
{
  static_assert(std::is_integral<T>::value, "Type not supported. Flags only supports integer types");
}

template<typename T>
inline Flags<T>::Flags(Flags &&flag) TL_NOEXCEPT
  : mFlag(flag.mFlag)
{
  static_assert(std::is_integral<T>::value, "Type not supported. Flags only supports integer types");
}

template<typename T> 
inline Flags<T>::Flags(std::initializer_list<T> flags)
  : mFlag(0)
{
  static_assert(std::is_integral<T>::value, "Float point type not supported");
  for (auto flg : flags) {
    this->enable(flg);
  }
}

template<typename T>
inline Flags<T> &Flags<T>::operator = (const Flags<T> &flag)
{
  if (this != &flag) {
    mFlag = flag.mFlag;
  }
  return *this;
}

template<typename T>
inline Flags<T> &Flags<T>::operator = (Flags<T> &&flag) TL_NOEXCEPT
{
  if (this != &flag) {
    mFlag = flag.mFlag;
  }
  return *this;
}

template<typename T>
inline bool Flags<T>::isEnabled(T flag) const
{
  return 0 != (mFlag & T{1} << flag);
}

template<typename T>
inline bool Flags<T>::isDisabled(T flag) const
{
  return 0 == (mFlag & T{1} << flag);
}

template<typename T>
inline void Flags<T>::enable(T flag)
{
  mFlag |= (T{1} << flag);
}

template<typename T> 
inline void Flags<T>::disable(T flag)
{
  mFlag &= ~(T{1} << flag);
}

template<typename T> 
inline void Flags<T>::switchFlag(T flag)
{
  if (isEnabled(flag)) 
    disable(flag);
  else 
    enable(flag);
}

template<typename T> 
inline void Flags<T>::clear()
{
  mFlag = T{0};
}

template<typename T> 
inline T Flags<T>::flags() const
{
  return mFlag;
}

#ifdef TL_ENABLE_DEPRECATED_METHODS

template<typename T> inline
bool Flags<T>::isActive(T flag) const
{
  return 0 != (mFlag & T{1} << flag);
}

template<typename T> inline
void Flags<T>::flagOn(T flag)
{
  mFlag |= (T{1} << flag);
}

template<typename T> inline
void Flags<T>::flagOff(T flag)
{
  mFlag &= ~(T{1} << flag);
}

#endif // TL_ENABLE_DEPRECATED_METHODS

/*! \} */ // end of core

} // End namespace tl


#endif // TL_CORE_FLAGS_H
