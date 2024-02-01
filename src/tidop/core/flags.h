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

#include "tidop/config.h"

#include "tidop/core/defs.h"

#include <type_traits>
#include <utility>


namespace tl
{

/*! \addtogroup core
 *  \{
 */


/*!
 * \brief This class allows the use of an enum as a flag.
 *
 * <h4>Example</h4>
 * 
 * \code
 * enum class ePrueba : int8_t {
 *    flag_0 = (0 << 0),
 *    flag_1 = (1 << 0),
 *    flag_2 = (1 << 1),
 *    flag_3 = (1 << 2),
 *    flag_4 = (1 << 3),
 *    flag_5 = (1 << 4),
 *    flag_6 = (1 << 5),
 *    flag_7 = (1 << 6)
 * };
 * ALLOW_BITWISE_FLAG_OPERATIONS(ePrueba)
 * 
 * int main(int argc, char *argv[])
 * {
 *
 *     EnumFlags<ePrueba> flag(ePrueba::flag_1);
 *     
 *     // Check if the flag is active
 *     bool bActive = flag.isEnabled(ePrueba::flag_1);
 *     
 *     // Activate a flag
 *     flag2.enable(ePrueba::flag_3);
 *     
 *     // Deactivate a flag
 *     flag2.disable(ePrueba::flag_1);
 *     
 *     // Switch a flag
 *     flag2.switchFlag(ePrueba::flag_5);
 *     
 *     EnumFlags<ePrueba> flag2;
 *     flag2 = ePrueba::flag_3 | ePrueba::flag_4;
 *     
 *     return 0;
 * }
 *
 * \endcode
 */
template<typename T>
class EnumFlags
{

public:

    using Type = typename std::underlying_type<T>::type;

private:

    Type flag;

public:

    /*!
     * \brief Default constructor
     */
    EnumFlags();
    
    /*!
     * \brief Copy constructor
     * \param[in] flag Flags
     */
    EnumFlags(const EnumFlags<T> &enumFlag);
    
    /*!
     * \brief Move constructor
     * \param[in] flag Flags
     */
    EnumFlags(EnumFlags<T> &&enumFlag) TL_NOEXCEPT;
    
    /*!
     * \brief Constructor
     * \param[in] flag Flags
     */
    EnumFlags(T flag);
    
    /*!
     * \brief Destructor
     */
    ~EnumFlags() = default;
    
    /*!
     * \brief Copy assignment operator
     * \param[in] enumFlag EnumFlags object
     * \return EnumFlags object reference
     */
    auto operator = (const EnumFlags<T> &enumFlag) -> EnumFlags&;
    
    /*!
     * \brief Move assignment operator
     * \param[in] enumFlag EnumFlags object
     * \return EnumFlags object reference
     */
    auto operator = (EnumFlags<T> &&enumFlag) TL_NOEXCEPT -> EnumFlags&;
    
    /*!
     * \brief Enumeration assignment operator
     * \param[in] flag enumeration or union of them
     * \return EnumFlags object reference
     */
    auto operator = (T flag) -> EnumFlags&;
    
    /*!
     * \brief Check if the flag is enabled
     * \param[in] flag Flag to be checked
     * \return True if enabled and false otherwise
     */
    auto isEnabled(T flag) const -> bool;
    
    /*!
     * \brief Check if the flag is disabled
     * \param[in] flag Flag to be checked
     * \return True if disabled and false otherwise
     */
    auto isDisabled(T flag) const -> bool;
    
    /*!
     * \brief Enable a flag
     * \param[in] flag Flag to activate
     */
    void enable(T flag);
    
    /*!
     * \brief Disable a flag
     * \param[in] flag Flag to disable
     */
    void disable(T flag);
    
    /*!
     * \brief Enables or disables a flag
     * \param[in] flag Flag to enable/disable
     * \param[in] active True to activate the flag
     */
    void activeFlag(T flag, bool active);
    
    /*!
     * \brief Switch a flag
     * \param[in] flag Flag to switch
     */
    void switchFlag(T flag);
    
    /*!
     * \brief Deactivate all flags
     */
    void clear();
    
    auto flags() const -> T;

};

template<typename T>
inline EnumFlags<T>::EnumFlags() 
  : flag(0) 
{
}

template<typename T>
inline EnumFlags<T>::EnumFlags(const EnumFlags<T> &enumFlag)
  : flag(enumFlag.flag) 
{

}

template<typename T>
inline EnumFlags<T>::EnumFlags(EnumFlags<T> &&enumFlag) TL_NOEXCEPT
  : flag(std::move(enumFlag.flag)) 
{

}


template<typename T>
inline EnumFlags<T>::EnumFlags(T flag) 
  : flag(static_cast<Type>(flag))
{
}

template<typename T>
inline auto EnumFlags<T>::operator = (const EnumFlags<T> &enumFlag) -> EnumFlags&
{
    if (this != &enumFlag) {
        this->flag = enumFlag.flag;
    }

    return *this;
}

template<typename T>
inline auto EnumFlags<T>::operator = (EnumFlags<T> &&enumFlag) TL_NOEXCEPT -> EnumFlags&
{
    if (this != &enumFlag) {
        this->flag = std::move(enumFlag.flag);
    }

    return *this;
}

template<typename T>
inline auto EnumFlags<T>::operator = (T flag) -> EnumFlags&
{
    this->flag = static_cast<Type>(flag);
    return *this;
}

template<typename T>
inline auto EnumFlags<T>::isEnabled(T flag) const -> bool
{
    return 0 != (this->flag & static_cast<Type>(flag));
}

template<typename T>
inline auto EnumFlags<T>::isDisabled(T flag) const -> bool
{
    return 0 == (this->flag & static_cast<Type>(flag));
}

template<typename T>
inline void tl::EnumFlags<T>::enable(T flag)
{
    this->flag |= static_cast<Type>(flag);
}

template<typename T>
inline void EnumFlags<T>::disable(T flag)
{
    this->flag &= ~static_cast<Type>(flag);
}

template<typename T>
inline void EnumFlags<T>::activeFlag(T flag, bool active)
{
    active ? enable(flag) : disable(flag);
}

template<typename T>
inline void EnumFlags<T>::switchFlag(T flag)
{
    isEnabled(flag) ? disable(flag) : enable(flag);
}

template<typename T>
inline void EnumFlags<T>::clear()
{
    this->flag = static_cast<Type>(0);
}

template<typename T>
inline auto EnumFlags<T>::flags() const -> T
{
    return static_cast<T>(this->flag);
}



/*!
 * \brief Allows bit-level operations with an 'enum class'
 *
 * This macro must be added below the enum declaration.
 *
 * <h4>Example</h4>
 * 
 * \code
 * enum class FlagTest : int8_t {
 *    flag01 = (1 << 0),
 *    flag02 = (1 << 1),
 *    flag03 = (1 << 2),
 *    flag04 = (1 << 3),
 *    flag05 = (1 << 4),
 *    flag06 = (1 << 5),
 *    flag07 = (1 << 6)
 * };
 *
 * ALLOW_BITWISE_FLAG_OPERATIONS(FlagTest)
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



/*!
 * \brief This class allows the use of an integer type as a flag
 *
 * <h4>Example</h4>
 * 
 * \code
 *
 * int main(int argc, char *argv[])
 * {
 *     Flags_8 flag_list{ 0, 3, 7, 4 }; // Enable 0, 3, 7 and 4
 *     
 *     // Check if the flag is active
 *     bool active = flag_list.isEnabled(0); // Return true
 *     active = flag_list.isEnabled(1); // Return false
 * 
 *     // Activate a flag
 *     flag_list.enable(5);
 *     
 *     // Deactivate a flag
 *     flag_list.disable(1);
 *     
 *     // Switch a flag
 *     flag_list.switchFlag(5);
 *     
 *     return 0;
 * }
 * \endcode
 */
template<typename T>
class Flags
{

public:

    using Type = T; 

private:

  Type _flags;

public:

    Flags();
    Flags(const Flags &flags);
    Flags(Flags &&flags) TL_NOEXCEPT;
    Flags(std::initializer_list<T> flags);
    ~Flags() = default;
    
    auto operator = (const Flags<T> &flags) -> Flags&;
    auto operator = (Flags<T> &&flags) TL_NOEXCEPT -> Flags&;
    
    /*!
     * \brief Check if the flag is enabled
     * \param[in] flag Flag to be checked
     * \return True if enabled and false otherwise
     */
    auto isEnabled(T flag) const -> bool;
    
    /*!
     * \brief Check if the flag is disabled
     * \param[in] flag Flag to be checked
     * \return True if disabled and false otherwise
     */
    auto isDisabled(T flag) const -> bool;
    
    /*!
     * \brief Enable a flag
     * \param flag Flag to activate
     */
    void enable(T flag);
    
    /*!
     * \brief Disable a flag
     * \param flag Flag to disable
     */
    void disable(T flag);
    
    /*!
     * \brief Enables or disables a flag
     * \param[in] flag Flag to enable/disable
     * \param[in] active True to activate the flag
     */
    void activeFlag(T flag, bool active);
    
    /*!
     * \brief Switch a flag
     * \param[in] flag Flag to switch
     */
    void switchFlag(T flag);
    
    /*!
     * \brief Deactivate all flags
     */
    void clear();
     
    auto flags() const -> T;

};



using Flags_8 = Flags<uint8_t>;
using Flags_16 = Flags<uint16_t>;
using Flags_32 = Flags<uint32_t>;
using Flags_64 = Flags<uint64_t>;


template<typename T>
inline Flags<T>::Flags()
  : _flags(0)
{
    static_assert(std::is_integral<T>::value, "Type not supported. Flags only supports integer types");
}

template<typename T> 
inline Flags<T>::Flags(const Flags &flags) 
  : _flags(flags._flags)
{
    static_assert(std::is_integral<T>::value, "Type not supported. Flags only supports integer types");
}

template<typename T>
inline Flags<T>::Flags(Flags &&flags) TL_NOEXCEPT
  : _flags(flags._flags)
{
    static_assert(std::is_integral<T>::value, "Type not supported. Flags only supports integer types");
}

template<typename T> 
inline Flags<T>::Flags(std::initializer_list<T> flags)
    : _flags(0)
{
    static_assert(std::is_integral<T>::value, "Float point type not supported");
    for (auto flg : flags) {
        this->enable(flg);
    }
}

template<typename T>
inline auto Flags<T>::operator = (const Flags<T> &flags) -> Flags&
{
    if (this != &flags) {
        this->_flags = flags._flags;
    }

    return *this;
}

template<typename T>
inline auto Flags<T>::operator = (Flags<T> &&flags) TL_NOEXCEPT -> Flags&
{
    if (this != &flags) {
        this->_flags = flags._flags;
    }

    return *this;
}

template<typename T>
inline auto Flags<T>::isEnabled(T flag) const -> bool
{
    return 0 != (this->_flags & T{1} << flag);
}

template<typename T>
inline auto Flags<T>::isDisabled(T flag) const -> bool
{
    return 0 == (this->_flags & T{1} << flag);
}

template<typename T>
inline void Flags<T>::enable(T flag)
{
    this->_flags |= (T{1} << flag);
}

template<typename T> 
inline void Flags<T>::disable(T flag)
{
    this->_flags &= ~(T{1} << flag);
}

template<typename T> 
inline void Flags<T>::activeFlag(T flag, bool active)
{
    active ? enable(flag) : disable(flag);
}

template<typename T> 
inline void Flags<T>::switchFlag(T flag)
{
    isEnabled(flag) ? disable(flag) : enable(flag);
}

template<typename T> 
inline void Flags<T>::clear()
{
    this->_flags = T{0};
}

template<typename T> 
inline auto Flags<T>::flags() const -> T
{
    return this->_flags;
}

/*! \} */ // end of core

} // End namespace tl
