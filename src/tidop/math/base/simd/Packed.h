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
#include "tidop/core/base/Type.h"
#include "tidop/core/base/Exception.h"
#include "tidop/math/base/Constants.h"
#include "tidop/math/base/Concepts.h"
#include "tidop/math/base/simd/Compare.h"
#include "tidop/math/base/simd/Traits.h"
#include "tidop/math/base/simd/detail/BitwiseAnd.h"
#include "tidop/math/base/simd/detail/BitwiseNot.h"
#include "tidop/math/base/simd/detail/BitwiseOr.h"
#include "tidop/math/base/simd/detail/BitwiseXor.h"
#include "tidop/math/base/simd/detail/ChangeSign.h"
#include "tidop/math/base/simd/detail/HorizontalSum.h"
#include "tidop/math/base/simd/detail/LoadPacked.h"
#include "tidop/math/base/simd/detail/MaskOp.h"
#include "tidop/math/base/simd/detail/Max.h"
#include "tidop/math/base/simd/detail/Min.h"
#include "tidop/math/base/simd/detail/PackedAdd.h"
#include "tidop/math/base/simd/detail/PackedDiv.h"
#include "tidop/math/base/simd/detail/PackedMul.h"
#include "tidop/math/base/simd/detail/PackedSub.h"
#include "tidop/math/base/simd/detail/SetPacked.h"
#include "tidop/math/base/simd/detail/SetZero.h"
#include "tidop/math/base/simd/detail/StorePacked.h"

/// \cond
#ifdef TL_HAVE_SIMD_INTRINSICS
/// \endcond

namespace tl
{


/*! \addtogroup MathBase
 *  \{
 */


/*!
 * \class Packed
 * \brief Encapsulates SIMD operations for different configurations.
 *
 * This class provides an abstraction over SIMD intrinsic types. Depending on the project configuration,
 * different SIMD instructions (AVX, SSE4.2, SSE4.1, SSE3, SSE2) are used. The class supports loading,
 * storing, and basic arithmetic operations on SIMD-packed data.
 *
 * \tparam T The data type of the elements in the packed SIMD type.
 */
template<typename T>
class Packed
{

public:

    /*!
     * \brief The type of each element in the SIMD packed type.
     */
    using value_type = std::remove_cv_t<typename PackedTraits<Packed<T>>::value_type>;

    /*!
     * \brief The SIMD intrinsic type used for packed operations.
     */
    using simd_type = typename PackedTraits<Packed<T>>::simd_type;

public:

    /*!
     * \brief Default constructor.
     */
    Packed() = default;

    /*!
     * \brief Copy constructor.
     * \param[in] packed The Packed object to copy from.
     */
    Packed(const Packed &packed);

    /*!
     * \brief Move constructor.
     * \param[in] packed The Packed object to move from.
     */
    Packed(Packed &&packed) noexcept;

    /*!
     * \brief Constructor from SIMD intrinsic type.
     * \param[in] packed The SIMD intrinsic value to initialize with.
     */
    Packed(simd_type packed);

    /*!
     * \brief Constructor from scalar value.
     * \param[in] scalar The scalar value to initialize all elements of the SIMD packed type.
     */
    Packed(value_type scalar);

    /*!
     * \brief Loads data from the specified source.
     *
     * This method loads data into the `Packed` object from a given memory address.
     * If the memory address is properly aligned, an optimized load is used.
     * Otherwise, an unaligned load is used.
     *
     * \param[in] src Memory address from which to load the data.
     */
    void load(const value_type *src);

    /**
     * \brief Stores data to the specified destination.
     *
     * This method stores the data from the `Packed` object into a given memory address.
     * If the memory address is properly aligned, an optimized store is used.
     * Otherwise, an unaligned store is used.
     *
     * \param[out] dst Memory address where to store the data.
     */
    void store(value_type *dst) const;

    /*!
     * \brief Load values from aligned memory into the SIMD packed type.
     * \param[in] src Pointer to the aligned memory source.
     */
    void loadAligned(const value_type *src);

    /*!
     * \brief Load values from unaligned memory into the SIMD packed type.
     * \param[in] src Pointer to the unaligned memory source.
     */
    void loadUnaligned(const value_type *src);

    /*!
     * \brief Store the values of the SIMD packed type into aligned memory.
     * \param[out] dst Pointer to the aligned memory destination.
     */
    void storeAligned(value_type *dst) const;

    /*!
     * \brief Store the values of the SIMD packed type into unaligned memory.
     * \param[out] dst Pointer to the unaligned memory destination.
     */
    void storeUnaligned(value_type *dst) const;

    /*!
     * \brief Set all elements of the SIMD packed type to a scalar value.
     * \param[in] value The scalar value to set.
     */
    void setScalar(value_type value);

    /*!
     * \brief Extract the element at a given index.
     * \param[in] index The index of the element to extract (0 <= index < size()).
     * \return The extracted scalar value.
     */
    [[nodiscard]]
    auto extract(size_t index) const -> value_type;

    /*!
     * \brief Insert a scalar value into the packed vector at a given index.
     * \param[in] index The index of the element to modify (0 <= index < size()).
     * \param[in] value The scalar value to insert.
     */
    void insert(size_t index, value_type value);

    /*!
     * \brief Assignment operator for the SIMD intrinsic type.
     * \param[in] packed The SIMD intrinsic value to assign.
     * \return Reference to the updated Packed object.
     */
    auto operator=(simd_type packed) -> Packed<T> &;

    /*!
     * \brief Type cast operator to convert to SIMD intrinsic type.
     * \return The SIMD intrinsic value.
     */
    operator simd_type() const;

    /*!
     * \brief Get the size of the SIMD packed type.
     * \return The number of elements in the SIMD packed type.
     */
    static constexpr size_t size();

    /*!
     * \brief Assignment operator for copy assignment.
     * \param[in] rhs The Packed object to assign from.
     * \return Reference to the updated Packed object.
     */
    auto operator=(const Packed<T> &rhs) -> Packed<T>&;

    /*!
     * \brief Assignment operator for move assignment.
     * \param[in] rhs The Packed object to move from.
     * \return Reference to the updated Packed object.
     */
    auto operator=(Packed<T> &&rhs) noexcept -> Packed<T>&;

    /*!
     * \brief Compound addition assignment operator.
     * \param[in] rhs The Packed object to add.
     * \return Reference to the updated Packed object.
     */
    auto operator+=(const Packed<T> &rhs) -> Packed<T>&;

    /*!
     * \brief Compound subtraction assignment operator.
     * \param[in] rhs The Packed object to subtract.
     * \return Reference to the updated Packed object.
     */
    auto operator-=(const Packed<T> &rhs) -> Packed<T>&;

    /*!
     * \brief Compound multiplication assignment operator.
     * \param[in] rhs The Packed object to multiply.
     * \return Reference to the updated Packed object.
     */
    auto operator*=(const Packed<T> &rhs) -> Packed<T>&;

    /*!
     * \brief Compound division assignment operator.
     * \param[in] rhs The Packed object to divide.
     * \return Reference to the updated Packed object.
     */
    auto operator/=(const Packed<T> &rhs) -> Packed<T>&;

    /*!
     * \brief Compound bitwise AND assignment operator.
     * \param[in] rhs The Packed object to perform bitwise AND with.
     * \return Reference to the updated Packed object.
     */
    auto operator&=(const Packed<T> &rhs) noexcept -> Packed<T>&;

    /*!
     * \brief Compound bitwise OR assignment operator.
     * \param[in] rhs The Packed object to perform bitwise OR with.
     * \return Reference to the updated Packed object.
     */
    auto operator|=(const Packed<T> &rhs) noexcept -> Packed<T>&;

    /*!
     * \brief Compound bitwise XOR assignment operator.
     * \param[in] rhs The Packed object to perform bitwise XOR with.
     * \return Reference to the updated Packed object.
     */
    auto operator^=(const Packed<T> &rhs) noexcept -> Packed<T> &;

    /*!
     * \brief Post-increment operator.
     * \return The Packed object before incrementing.
     */
    auto operator++(int) -> Packed<T>;

    /*!
     * \brief Pre-increment operator.
     * \return Reference to the updated Packed object.
     */
    auto operator++() -> Packed<T>&;

    /*!
     * \brief Post-decrement operator.
     * \return The Packed object before decrementing.
     */
    auto operator--(int) -> Packed<T>;

    /*!
     * \brief Pre-decrement operator.
     * \return Reference to the updated Packed object.
     */
    auto operator--() -> Packed<T>&;

    /*!
     * \brief Sum of the elements of the SIMD packed type.
     * \return The sum of the elements.
     */
    TL_DEPRECATED("reduceSum()", "4.0")
    [[nodiscard]]
    auto sum() const -> T;

    /*!
     * \brief Horizontal sum reduction.
     * \return Sum of all SIMD lanes.
     */
    [[nodiscard]]
    auto reduceSum() const -> T;

    /*!
     * \brief Create a Packed object initialized with zero.
     * \return A Packed object with all elements set to zero.
     */
    static auto zero() -> Packed;

private:

    static bool isAligned(const void *ptr, size_t alignment)
    {
        return (reinterpret_cast<uintptr_t>(ptr) % alignment) == 0;
    }

private:

    simd_type mValue;
};


/*!
 * \brief Element‑wise minimum of two SIMD packets.
 * \tparam P A type satisfying `PackedConcept`.
 * \param[in] packed1 First packet.
 * \param[in] packed2 Second packet.
 * \return A packet containing the element‑wise minimums.
 *
 * This function uses the native SIMD min instruction for the given type
 * (e.g., `_mm256_min_ps` for `float`, `_mm_min_epi32` for integers).
 *
 * ### Example
 * \code
 * Packed<float> a = ...;
 * Packed<float> b = ...;
 * Packed<float> min_vals = min(a, b);
 * \endcode
 */
template<PackedConcept P>
[[nodiscard]]
auto min(const P &packed1, const P &packed2) -> P
{
    return detail::min_impl(packed1, packed2);
}

/*!
 * \brief Element‑wise maximum of two SIMD packets.
 * \tparam P A type satisfying `PackedConcept`.
 * \param[in] packed1 First packet.
 * \param[in] packed2 Second packet.
 * \return A packet containing the element‑wise maximums.
 *
 * This function uses the native SIMD max instruction.
 */
template<PackedConcept P>
[[nodiscard]]
auto max(const P &packed1, const P &packed2) -> P
{
    return detail::max_impl(packed1, packed2);
}

/*!
 * \brief Checks whether all elements of a mask packet are true (non‑zero).
 * \tparam T Arithmetic type (integral or floating‑point).
 * \param[in] mask A `Packed<T>` interpreted as a mask (non‑zero = true, zero = false).
 * \return `true` if every element in the packet is non‑zero; `false` otherwise.
 *
 * The implementation uses the appropriate `movemask` intrinsic for the type
 * (e.g., `_mm256_movemask_ps` for `float`, `_mm256_movemask_epi8` for integers).
 */
template<Arithmetic T>
[[nodiscard]]
auto all(const Packed<T> &mask) -> bool
{
    return detail::all_impl(mask);
}

/*!
 * \brief Checks whether no element of a mask packet is true (all zero).
 * \tparam T Arithmetic type.
 * \param[in] mask A `Packed<T>` interpreted as a mask.
 * \return `true` if all elements are zero; `false` otherwise.
 */
template<Arithmetic T>
[[nodiscard]]
auto none(const Packed<T> &mask) -> bool
{
    return detail::none_impl(mask);
}

/*!
 * \brief Checks whether at least one element of a mask packet is true.
 * \tparam T Arithmetic type.
 * \param[in] mask A `Packed<T>` interpreted as a mask.
 * \return `true` if any element is non‑zero; `false` otherwise.
 *
 * Equivalent to `!none(mask)`.
 */
template<Arithmetic T>
[[nodiscard]]
bool any(const Packed<T> &mask)
{
    return !none(mask);
}


// compare

/// \cond

namespace internal
{

template<typename T>
[[nodiscard]]
auto extractElement(const Packed<T> &packed, size_t index) -> typename Packed<T>::value_type
{
    using Traits = PackedTraits<Packed<T>>;

    TL_ASSERT(index < Traits::size, "Out of range");

    alignas(Traits::alignment)
    T values[Traits::size];

    packed.storeAligned(values);

    return values[index];
}

template<typename T>
auto insertElement(const Packed<T> &v, size_t index, T value) -> Packed<T>
{
    using Traits = PackedTraits<Packed<T>>;

    alignas(Traits::alignment)
    T tmp[Traits::size];

    v.storeAligned(tmp);
    tmp[index] = value;

    Packed<T> p;
    p.loadAligned(tmp);
    return p;
}

} // namespace internal 

/// \endcond


/* Packed Implementation */

template<typename T>
Packed<T>::Packed(const Packed &packed)
  : mValue(packed.mValue)
{
}

template<typename T>
Packed<T>::Packed(Packed &&packed) noexcept
  : mValue(std::move(packed.mValue))
{
}

template<typename T>
Packed<T>::Packed(simd_type packed)
  : mValue(packed)
{
}

template<typename T>
Packed<T>::Packed(value_type scalar)
    : mValue(detail::set(scalar))
{
}

template<typename T>
void Packed<T>::load(const value_type *src)
{
    constexpr size_t alignment = alignof(simd_type);
    if (isAligned(src, alignment)) {
        loadAligned(src);
    } else {
        loadUnaligned(src);
    }
}

template<typename T>
void Packed<T>::loadAligned(const value_type *src)
{
    mValue = detail::loadPackedAligned(src);
}

template<typename T>
void Packed<T>::loadUnaligned(const value_type *src)
{
    mValue = detail::loadPackedUnaligned(src);
}

template<typename T>
void Packed<T>::store(value_type *dst) const
{
    constexpr size_t alignment = alignof(simd_type);
    if (isAligned(dst, alignment)) {
        storeAligned(dst);
    } else {
        storeUnaligned(dst);
    }
}

template<typename T>
void Packed<T>::storeAligned(value_type *dst) const
{
    detail::storePackedAligned(dst, mValue);
}

template<typename T>
void Packed<T>::storeUnaligned(value_type *dst) const
{
    detail::storePackedUnaligned(dst, mValue);
}

template<typename T>
void Packed<T>::setScalar(value_type value)
{
    mValue = detail::set(value);
}

template<typename T>
auto Packed<T>::extract(size_t index) const -> value_type
{
    return internal::extractElement(*this, static_cast<int>(index));
}

template<typename T>
void Packed<T>::insert(size_t index, value_type value)
{
    *this = internal::insertElement(*this, static_cast<int>(index), value);
}

template<typename T>
auto Packed<T>::operator=(simd_type packed) -> Packed<T>&
{
    mValue = packed;
    return *this;
}

template<typename T>
constexpr auto Packed<T>::size() -> size_t
{
    return PackedTraits<Packed<T>>::size;
}

template<typename T>
Packed<T>::operator simd_type() const
{
    return mValue;
}

template<typename T>
auto Packed<T>::operator=(const Packed<T> &rhs) -> Packed<T>&
{
    if (this != &rhs) {
        mValue = rhs;
    }

    return *this;
}

template<typename T>
auto Packed<T>::operator=(Packed<T> &&rhs) noexcept -> Packed<T>&
{
    if (this != &rhs) {
        mValue = std::move(rhs);
    }

    return *this;
}

template<typename T>
auto Packed<T>::operator+=(const Packed<T> &rhs) -> Packed<T>&
{
    *this = *this + rhs;
    return *this;
}

template<typename T>
auto Packed<T>::operator-=(const Packed<T> &rhs) -> Packed<T>&
{
    *this = *this - rhs;
    return *this;
}

template<typename T>
auto Packed<T>::operator*=(const Packed<T> &rhs) -> Packed<T>&
{
    *this = *this * rhs;
    return *this;
}

template<typename T>
auto Packed<T>::operator/=(const Packed<T> &rhs) -> Packed<T>&
{
    *this = *this / rhs;
    return *this;
}

template<typename T>
auto Packed<T>::operator&=(const Packed<T> &rhs) noexcept -> Packed<T> &
{
    *this = *this & rhs;
    return *this;
}

template<typename T>
auto Packed<T>::operator|=(const Packed<T> &rhs) noexcept -> Packed<T> &
{
    *this = *this | rhs;
    return *this;
}

template<typename T>
auto Packed<T>::operator^=(const Packed<T> &rhs) noexcept -> Packed<T> &
{
    *this = *this ^ rhs;
    return *this;
}


template<typename T>
auto Packed<T>::operator++(int) -> Packed<T>
{
    Packed<T> packet = *this;
    *this += consts::one<T>;
    return *packet;
}

template<typename T>
auto Packed<T>::operator++() -> Packed<T>&
{
    *this += consts::one<T>;
    return *this;
}

template<typename T>
auto Packed<T>::operator--(int) -> Packed<T>
{
    Packed<T> packet = *this;
    *this -= consts::one<T>;
    return *packet;
}

template<typename T>
auto Packed<T>::operator--() -> Packed<T>&
{
    *this -= consts::one<T>;
    return *this;
}

template<typename T>
auto Packed<T>::sum() const -> T
{
    return detail::horizontal_sum(*this);
}

template<typename T>
auto Packed<T>::reduceSum() const -> T
{
    return detail::horizontal_sum(*this);
}

template<typename T>
auto Packed<T>::zero() -> Packed
{
    return detail::setZero<T>();
}


/* Packed overload operators */

template<typename T>
auto operator -(const Packed<T> &packet) -> Packed<T>
{
    return detail::changeSign(packet);
}

template<typename T>
auto operator+(const Packed<T> &packed1,
    const Packed<T> &packed2) -> Packed<T>
{
    return detail::add(packed1, packed2);
}

template<typename T>
auto operator+(const Packed<T> &packed, T scalar) -> Packed<T>
{
    return packed + Packed<T>(scalar);
}

template<typename T>
auto operator+(T scalar, const Packed<T> &packed) -> Packed<T>
{
    return Packed<T>(scalar) + packed;
}

template<typename T>
auto operator-(const Packed<T> &packed1,
    const Packed<T> &packed2) -> Packed<T>
{
    return detail::sub(packed1, packed2);
}

template<typename T>
auto operator-(const Packed<T> &packed, T scalar) -> Packed<T>
{
    return packed - Packed<T>(scalar);
}

template<typename T>
auto operator-(T scalar, const Packed<T> &packed) -> Packed<T>
{
    return Packed<T>(scalar) - packed;
}

template<typename T>
auto operator*(const Packed<T> &packed1,
    const Packed<T> &packed2) -> Packed<T>
{
    return detail::mul(packed1, packed2);
}

template<typename T>
auto operator*(const Packed<T> &packed, T scalar) -> Packed<T>
{
    return packed * Packed<T>(scalar);
}

template<typename T>
auto operator*(T scalar, const Packed<T> &packed) -> Packed<T>
{
    return Packed<T>(scalar) * packed;
}

template<typename T>
auto operator/(const Packed<T> &packed1,
    const Packed<T> &packed2) -> Packed<T>
{
    return detail::div(packed1, packed2);
}

template<typename T>
auto operator/(const Packed<T> &packed, T scalar) -> Packed<T>
{
    return packed / Packed<T>(scalar);
}

template<typename T>
auto operator/(T scalar, const Packed<T> &packed) -> Packed<T>
{
    return Packed<T>(scalar) / packed;
}

/* Comparison Operators */

template<typename T>
static auto operator ==(const Packed<T> &packed1,
    const Packed<T> &packed2) -> Packed<T>
{
    return compare<CompareOp::eq>(packed1, packed2);
}

template<typename T>
static auto operator !=(const Packed<T> &packed1,
    const Packed<T> &packed2) -> Packed<T>
{
    return compare<CompareOp::ne>(packed1, packed2);
}

template<typename T>
static auto operator > (const Packed<T> &packed1,
    const Packed<T> &packed2) -> Packed<T>
{
    return compare<CompareOp::gt>(packed1, packed2);
}

template<typename T>
static auto operator < (const Packed<T> &packed1,
    const Packed<T> &packed2) -> Packed<T>
{
    return compare<CompareOp::lt>(packed1, packed2);
}

template<typename T>
static auto operator >= (const Packed<T> &packed1,
    const Packed<T> &packed2) -> Packed<T>
{
    return compare<CompareOp::ge>(packed1, packed2);
}

template<typename T>
static auto operator <= (const Packed<T> &packed1,
    const Packed<T> &packed2) -> Packed<T>
{
    return compare<CompareOp::le>(packed1, packed2);
}


/* Bitwise Operators */

template<typename T>
auto operator~(const Packed<T> &a) -> Packed<T>
{
    return detail::bitwiseNot(a);
}

template<typename T>
auto operator&(const Packed<T> &a, const Packed<T> &b) -> Packed<T>
{
    return detail::bitwiseAnd(a, b);
}

template<typename T>
auto operator|(const Packed<T> &a, const Packed<T> &b) -> Packed<T>
{
    return detail::bitwiseOr(a, b);
}

template<typename T>
auto operator^(const Packed<T> &a, const Packed<T> &b) -> Packed<T>
{
    return detail::bitwiseXor(a, b);
}

/* Transpose */

template<typename T>
auto transposeMatrix4x4(Packed<T> &r1,
                        Packed<T> &r2,
                        Packed<T> &r3,
                        Packed<T> &r4) -> std::enable_if_t<
    std::is_same<float, std::remove_cv_t<T>>::value,
    void>
{
    __m128 tmp1 = _mm_unpacklo_ps(r1, r2);
    __m128 tmp2 = _mm_unpackhi_ps(r1, r2);
    __m128 tmp3 = _mm_unpacklo_ps(r3, r4);
    __m128 tmp4 = _mm_unpackhi_ps(r3, r4);

    r1 = _mm_movelh_ps(tmp1, tmp3);
    r2 = _mm_movehl_ps(tmp3, tmp1);
    r3 = _mm_movelh_ps(tmp2, tmp4);
    r4 = _mm_movehl_ps(tmp4, tmp2);


/// _MM_TRANSPOSE4_PS lo hace:
//    __m128 _Tmp3, _Tmp2, _Tmp1, _Tmp0;                          
//        
//        _Tmp0   = _mm_shuffle_ps((row0), (row1), 0x44);          
//        _Tmp2   = _mm_shuffle_ps((row0), (row1), 0xEE);          
//        _Tmp1   = _mm_shuffle_ps((row2), (row3), 0x44);          
//        _Tmp3   = _mm_shuffle_ps((row2), (row3), 0xEE);          
//        
//        (row0) = _mm_shuffle_ps(_Tmp0, _Tmp1, 0x88);              
//        (row1) = _mm_shuffle_ps(_Tmp0, _Tmp1, 0xDD);              
//        (row2) = _mm_shuffle_ps(_Tmp2, _Tmp3, 0x88);              
//        (row3) = _mm_shuffle_ps(_Tmp2, _Tmp3, 0xDD); 
}


/*! \} */

} // End namespace tl

/// \cond
#endif // TL_HAVE_SIMD_INTRINSICS
/// \endcond
