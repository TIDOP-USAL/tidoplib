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

/*!
 * \file Flags.h
 * \brief Bitwise flag operations for enums and integer types
 *
 * This module provides two complementary classes for managing flag operations:
 * - EnumFlags for type-safe enum-based flags
 * - Flags for integer-based bit flags
 *
 * Both classes support checking, enabling, disabling, and toggling individual flags
 * with an intuitive API. EnumFlags works with enum class types using the
 * ALLOW_BITWISE_FLAG_OPERATIONS macro to enable bitwise operators.
 *
 * ### Classes
 *
 * - \ref EnumFlags - Type-safe flag management for enum class types
 * - \ref Flags - Bit flag management for integer types (8, 16, 32, 64 bit)
 *
 * ### Macros
 *
 * - \ref ALLOW_BITWISE_FLAG_OPERATIONS - Enable bitwise operators for enum class
 *
 * ### Example Usage - Enum Flags
 *
 * \code{.cpp}
 * enum class Permission : uint8_t {
 *     read   = (1 << 0),
 *     write  = (1 << 1),
 *     execute = (1 << 2)
 * };
 * ALLOW_BITWISE_FLAG_OPERATIONS(Permission)
 *
 * EnumFlags<Permission> perms(Permission::read);
 * perms.enable(Permission::write);
 * if (perms.isEnabled(Permission::read)) {
 *     // User can read
 * }
 * \endcode
 *
 * ### Example Usage - Integer Flags
 *
 * \code{.cpp}
 * Flags_8 flags{0, 2, 5};  // Enable bits 0, 2, 5
 * if (flags.isEnabled(0)) {
 *     // Bit 0 is set
 * }
 * flags.switchFlag(3);  // Toggle bit 3
 * \endcode
 *
 * \see tl::EnumFlags, tl::Flags, tl::ALLOW_BITWISE_FLAG_OPERATIONS
 */

#pragma once

#include "tidop/config.h"

#include <type_traits>
#include <utility>


namespace tl
{

/*! \addtogroup Base
 *  \{
 */


/*!
 * \brief Type-safe flag management for enum class types.
 *
 * Provides an intuitive interface for managing flags defined as enum class.
 * The underlying type of the enum is automatically detected and used for
 * bitwise operations. Use the ALLOW_BITWISE_FLAG_OPERATIONS macro to enable
 * bitwise operators for your enum type.
 *
 * ### Example Usage
 * \code{.cpp}
 *
 * \code{.cpp}
 * enum class ePrueba : int8_t {
 *     flag_0 = (1 << 0),
 *     flag_1 = (1 << 1),
 *     flag_2 = (1 << 2),
 *     flag_3 = (1 << 3),
 *     flag_4 = (1 << 4),
 *     flag_5 = (1 << 5),
 *     flag_6 = (1 << 6),
 *     flag_7 = (1 << 7)
 * };
 * ALLOW_BITWISE_FLAG_OPERATIONS(ePrueba)
 * 
 * EnumFlags<ePrueba> flags(ePrueba::flag_1);
 * 
 * // Check if the flag is active
 * bool bActive = flags.isEnabled(ePrueba::flag_1);  // true
 * 
 * // Activate a flag
 * flags.enable(ePrueba::flag_3);
 * 
 * // Deactivate a flag
 * flags.disable(ePrueba::flag_1);
 * 
 * // Switch a flag
 * flags.switchFlag(ePrueba::flag_5);
 * 
 * // Combine flags using bitwise operations
 * EnumFlags<ePrueba> flags2 = ePrueba::flag_3 | ePrueba::flag_4;
 * \endcode
 *
 * ### Notes
 *
 * - T must be an enum class type
 * - Use ALLOW_BITWISE_FLAG_OPERATIONS(T) after enum declaration to enable operator support
 * - Flags should be defined with power-of-two values for individual bit representation
 *
 * \see ALLOW_BITWISE_FLAG_OPERATIONS, Flags
 */
template<typename T>
class EnumFlags
{

public:

    using Type = typename std::underlying_type<T>::type;

private:

    Type flag = static_cast<Type>(0);

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes EnumFlags with no flags set (all bits cleared).
     */
    constexpr EnumFlags() = default;

    /*!
     * \brief Copy constructor.
     * 
     * Creates a copy of another EnumFlags object.
     *
     * \param[in] enumFlag The EnumFlags object to copy
     */
    constexpr EnumFlags(const EnumFlags<T> &enumFlag) = default;
    
    /*!
     * \brief Move constructor.
     * 
     * Transfers ownership of flags from another EnumFlags object.
     *
     * \param[in] enumFlag The EnumFlags object to move from
     */
    constexpr EnumFlags(EnumFlags<T> &&enumFlag) noexcept = default;
    
    /*!
     * \brief Constructor from enum value.
     *
     * Initializes EnumFlags with a specific flag or combination of flags.
     *
     * \param[in] flag Initial flag value (can be a single flag or combination via bitwise OR)
     *
     * ### Example
     * \code{.cpp}
     * EnumFlags<Permission> perms(Permission::read | Permission::write);
     * \endcode
     */
    constexpr explicit EnumFlags(T flag) noexcept;
    
    /*!
     * \brief Destructor
     */
    ~EnumFlags() = default;
    
    /*!
     * \brief Copy assignment operator.
     * 
     * \param[in] enumFlag EnumFlags object to copy from
     * \return Reference to this object
     */
    constexpr auto operator = (const EnumFlags<T> &enumFlag) -> EnumFlags& = default;
    
    /*!
     * \brief Move assignment operator.
     * 
     * \param[in] enumFlag EnumFlags object to move from
     * \return Reference to this object
     */
    constexpr auto operator = (EnumFlags<T> &&enumFlag) noexcept -> EnumFlags& = default;
    
    /*!
     * \brief Enum assignment operator.
     *
     * Replaces all flags with the new enum value(s).
     *
     * \param[in] flag New flag value (replaces existing flags)
     * \return Reference to this object
     *
     * ### Example
     * \code{.cpp}
     * EnumFlags<Permission> perms;
     * perms = Permission::read;  // Set only read flag
     * \endcode
     */
    constexpr auto operator=(T flag) noexcept -> EnumFlags &;

    /*!
     * \brief Check if the flag is enabled
     * \param[in] flag Flag to be checked
     * \return True if enabled and false otherwise
     */
    [[nodiscard]] 
    constexpr auto isEnabled(T flag) const noexcept -> bool;
    
    /*!
     * \brief Check if the flag is disabled
     * \param[in] flag Flag to be checked
     * \return True if disabled and false otherwise
     */
    [[nodiscard]] 
    constexpr auto isDisabled(T flag) const noexcept -> bool;
    
    /*!
     * \brief Enable a flag
     * \param[in] flag Flag to activate
     */
    constexpr void enable(T flag) noexcept;
    
    /*!
     * \brief Disable a flag
     * \param[in] flag Flag to disable
     */
    constexpr void disable(T flag) noexcept;
    
    /*!
     * \brief Enables or disables a flag
     * \param[in] flag Flag to enable/disable
     * \param[in] active True to activate the flag
     */
    constexpr void activeFlag(T flag, bool active) noexcept;
    
    /*!
     * \brief Switch a flag
     * \param[in] flag Flag to switch
     */
    constexpr void switchFlag(T flag) noexcept;
    
    /*!
     * \brief Deactivate all flags
     */
    constexpr void clear() noexcept;
    
    /*!
     * \brief Get the current flags value.
     *
     * \return The combined flags as the enum type
     */  
    [[nodiscard]]
    constexpr auto flags() const noexcept -> T;

};




template<typename T>
constexpr EnumFlags<T>::EnumFlags(T flag) noexcept
  : flag(static_cast<Type>(flag))
{
}

template<typename T>
constexpr auto EnumFlags<T>::operator = (T flag) noexcept -> EnumFlags&
{
    this->flag = static_cast<Type>(flag);
    return *this;
}

template<typename T>
constexpr auto EnumFlags<T>::isEnabled(T flag) const noexcept -> bool
{
    return 0 != (this->flag & static_cast<Type>(flag));
}

template<typename T>
constexpr auto EnumFlags<T>::isDisabled(T flag) const noexcept -> bool
{
    return 0 == (this->flag & static_cast<Type>(flag));
}

template<typename T>
constexpr void EnumFlags<T>::enable(T flag) noexcept
{
    this->flag |= static_cast<Type>(flag);
}

template<typename T>
constexpr void EnumFlags<T>::disable(T flag) noexcept
{
    this->flag &= ~static_cast<Type>(flag);
}

template<typename T>
constexpr void EnumFlags<T>::activeFlag(T flag, bool active) noexcept
{
    active ? enable(flag) : disable(flag);
}

template<typename T>
constexpr void EnumFlags<T>::switchFlag(T flag) noexcept
{
    isEnabled(flag) ? disable(flag) : enable(flag);
}

template<typename T>
constexpr void EnumFlags<T>::clear() noexcept
{
    this->flag = static_cast<Type>(0);
}

template<typename T>
constexpr auto EnumFlags<T>::flags() const noexcept -> T
{
    return static_cast<T>(this->flag);
}



/*!
 * \brief Enables bitwise operators for enum class types.
 *
 * This macro generates overloads for bitwise operators (&, |, ^, ~, |=) to work
 * with a specific enum class type. This allows natural bitwise operations on flags
 * defined as enum values.
 *
 * Must be invoked in the same namespace as the enum, typically immediately after
 * the enum class definition.
 *
 * ### Generated Operators
 *
 * - `operator|` - Bitwise OR (combine flags)
 * - `operator&` - Bitwise AND (intersection of flags)
 * - `operator^` - Bitwise XOR (toggle flags)
 * - `operator~` - Bitwise NOT (flip all bits)
 * - `operator|=` - Bitwise OR-assign (combine and assign)
 *
 * ### Example Usage
 *
 * \code{.cpp}
 * enum class Permission : uint8_t {
 *     read    = (1 << 0),  // 0x01
 *     write   = (1 << 1),  // 0x02
 *     execute = (1 << 2)   // 0x04
 * };
 * ALLOW_BITWISE_FLAG_OPERATIONS(Permission)
 *
 * // Now you can use bitwise operators:
 * Permission user_perms = Permission::read | Permission::execute;
 * Permission admin_perms = user_perms | Permission::write;
 * \endcode
 *
 * ### Notes
 *
 * - Requires std::underlying_type to determine the enum's base type
 * - All generated operators are inline for performance
 * - The macro does NOT affect the global scope in unexpected ways
 * - Multiple invocations with different enum types are safe
 *
 * \see EnumFlags
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
                                                                    \
inline T_FLAG& operator |= (T_FLAG &flag1, T_FLAG flag2)            \
{                                                                   \
  flag1 = flag1 | flag2;                                            \
  return flag1;                                                     \
}                                                                   \



/*!
 * \brief Bit flag management for integer types.
 *
 * Provides an intuitive interface for managing individual bits in an integer value.
 * Unlike EnumFlags which works with enum types, Flags works with raw bit positions
 * (0 through sizeof(T)*8-1). Bits are enabled by their position index.
 *
 * ### Design
 *
 * - Generic: Works with any integral type (8, 16, 32, 64 bit)
 * - Efficient: Direct bitwise operations on integer values
 * - Simple: Bits are identified by index (0 = LSB, 7 = MSB for uint8_t)
 *
 * ### Example Usage
 *
 * \code{.cpp}
 * int main()
 * {
 *     // Initialize with specific bits enabled (0, 3, 7, 4)
 *     Flags_8 flag_list{0, 3, 7, 4};
 *     
 *     // Check if a bit is active
 *     bool active = flag_list.isEnabled(0);  // Return true
 *     active = flag_list.isEnabled(1);       // Return false
 * 
 *     // Activate a bit
 *     flag_list.enable(5);
 *     
 *     // Deactivate a bit
 *     flag_list.disable(1);
 *     
 *     // Toggle a bit
 *     flag_list.switchFlag(5);
 *     
 *     // Get all flags
 *     uint8_t value = flag_list.flags();  // Binary: 10111001
 *     
 *     return 0;
 * }
 * \endcode
 *
 * ### Predefined Types
 *
 * - Flags_8 - 8-bit flags (uint8_t)
 * - Flags_16 - 16-bit flags (uint16_t)
 * - Flags_32 - 32-bit flags (uint32_t)
 * - Flags_64 - 64-bit flags (uint64_t)
 *
 * ### Notes
 *
 * - T must be an integral type (use std::is_integral trait)
 * - Bit indices start at 0 (least significant bit)
 * - Maximum bit index depends on type size (7 for uint8_t, 15 for uint16_t, etc.)
 * - Constructor with initializer_list activates specified bit positions
 *
 * \see Flags_8, Flags_16, Flags_32, Flags_64
 */
template<typename T>
class Flags
{

    static_assert(std::is_integral<T>::value, "Type not supported. Flags only supports integer types");

public:

    using Type = T; 

private:

    Type _flags = 0;

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes Flags with all bits cleared (value = 0).
     * Also enforces compile-time check that T is an integral type.
     */
    constexpr Flags() = default;

    /*!
     * \brief Copy constructor.
     * 
     * Creates a copy of another Flags object.
     *
     * \param[in] flags The Flags object to copy
     */    
    constexpr Flags(const Flags &flags) = default;

    /*!
     * \brief Move constructor.
     * 
     * Transfers ownership from another Flags object.
     *
     * \param[in] flags The Flags object to move from
     */    
    constexpr Flags(Flags &&flags) noexcept = default;

    /*!
     * \brief Constructor with initializer list of bit positions.
     *
     * Enables specific bit positions provided in the initializer list.
     * Useful for concisely setting multiple bits at once.
     *
     * \param[in] flags Initializer list of bit positions to enable (0-based indices)
     *
     * ### Example
     * \code{.cpp}
     * Flags_8 flags{0, 2, 5};     // Enables bits 0, 2, and 5
     * Flags_32 config{1, 3, 7, 15};  // Enables bits 1, 3, 7, and 15
     * \endcode
     *
     * ### Notes
     * - Bit indices must be valid for the type size
     * - Duplicate indices in the list result in the bit being enabled once
     * - Also enforces compile-time check that T is an integral type
     */    
    constexpr Flags(std::initializer_list<T> flags);

    /*!
     * \brief Destructor.
     */
    ~Flags() = default;

    /*!
     * \brief Copy assignment operator.
     * 
     * \param[in] flags Flags object to copy from
     * \return Reference to this object
     */    
    constexpr auto operator = (const Flags<T> &flags) -> Flags& = default;

    /*!
     * \brief Move assignment operator.
     * 
     * \param[in] flags Flags object to move from
     * \return Reference to this object
     */    
    constexpr auto operator = (Flags<T> &&flags) noexcept -> Flags& = default;
    
    /*!
     * \brief Check if the flag is enabled
     * \param[in] flag Flag to be checked
     * \return True if enabled and false otherwise
     */
    [[nodiscard]]
    constexpr auto isEnabled(T flag) const noexcept  -> bool;
    
    /*!
     * \brief Check if the flag is disabled
     * \param[in] flag Flag to be checked
     * \return True if disabled and false otherwise
     */
    [[nodiscard]]
    constexpr auto isDisabled(T flag) const noexcept -> bool;
    
    /*!
     * \brief Enable a flag
     * \param flag Flag to activate
     */
    constexpr void enable(T flag) noexcept;
    
    /*!
     * \brief Disable a flag
     * \param flag Flag to disable
     */
    constexpr void disable(T flag) noexcept;
    
    /*!
     * \brief Enables or disables a flag based on a boolean condition.
     *
     * Convenience method for conditionally enabling or disabling a flag.
     *
     * \param[in] flag The flag to modify
     * \param[in] active True to enable the flag, false to disable it
     *
     * ### Example
     * \code{.cpp}
     * EnumFlags<Permission> perms;
     * perms.activeFlag(Permission::write, user_is_admin);  // Enable write only if admin
     * \endcode
     *
     * \see enable(), disable(), switchFlag()
     */
    constexpr void activeFlag(T flag, bool active) noexcept;
    
    /*!
     * \brief Switch a flag
     * \param[in] flag Flag to switch
     */
    constexpr void switchFlag(T flag) noexcept;
    
    /*!
     * \brief Deactivate all flags
     */
    constexpr void clear() noexcept;

    /*!
     * \brief Get the current flags value.
     *
     * \return The integer value containing all enabled bits
     */    
    [[nodiscard]]
    constexpr auto flags() const noexcept -> T;

};


/*!
 * \brief Predefined Flags type for 8-bit unsigned integer.
 * \see Flags
 */
using Flags_8 = Flags<uint8_t>;

/*!
 * \brief Predefined Flags type for 16-bit unsigned integer.
 * \see Flags
 */
using Flags_16 = Flags<uint16_t>;

/*!
 * \brief Predefined Flags type for 32-bit unsigned integer.
 * \see Flags
 */
using Flags_32 = Flags<uint32_t>;

/*!
 * \brief Predefined Flags type for 64-bit unsigned integer.
 * \see Flags
 */
using Flags_64 = Flags<uint64_t>;



template<typename T> 
constexpr Flags<T>::Flags(std::initializer_list<T> flags)
    : _flags(0)
{
    for (auto flg : flags) {
        this->enable(flg);
    }
}

template<typename T>
constexpr auto Flags<T>::isEnabled(T flag) const noexcept -> bool
{
    return 0 != (this->_flags & T{1} << flag);
}

template<typename T>
constexpr auto Flags<T>::isDisabled(T flag) const noexcept -> bool
{
    return 0 == (this->_flags & T{1} << flag);
}

template<typename T>
constexpr void Flags<T>::enable(T flag) noexcept
{
    this->_flags |= (T{1} << flag);
}

template<typename T> 
constexpr void Flags<T>::disable(T flag) noexcept
{
    this->_flags &= ~(T{1} << flag);
}

template<typename T> 
constexpr void Flags<T>::activeFlag(T flag, bool active) noexcept
{
    active ? enable(flag) : disable(flag);
}

template<typename T> 
constexpr void Flags<T>::switchFlag(T flag) noexcept
{
    isEnabled(flag) ? disable(flag) : enable(flag);
}

template<typename T> 
constexpr void Flags<T>::clear() noexcept
{
    this->_flags = T{0};
}

template<typename T> 
constexpr auto Flags<T>::flags() const noexcept -> T
{
    return this->_flags;
}

/*! \} */

} // End namespace tl
