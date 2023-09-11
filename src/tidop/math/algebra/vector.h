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

#include <iterator>
#include <vector>
#include <array>
#include <random>

#include "tidop/core/exception.h"
#include "tidop/math/math.h"
#include "tidop/math/simd.h"
#include "tidop/math/data.h"


namespace tl
{

/*! \addtogroup math
 *  \{
 */


 /*! \addtogroup algebra
  *  \{
  */


template<typename T>
class VectorBase;

template<typename T, size_t _size>
class Vector;

template<
    template<typename, size_t _size = DynamicData>
class VectorDerived, typename T, size_t _size
>
class VectorBase<VectorDerived<T, _size>>
{

public:

    enum class Properties
    {
        contiguous_memory = 0x01
    };

public:

    EnumFlags<Properties> properties;

public:

    VectorBase() { this->properties.enable(Properties::contiguous_memory); }
    ~VectorBase() = default;

    auto module() const -> double;
    auto normalize() -> void;
    auto dotProduct(const VectorDerived<T, _size> &vector) const -> double;

    /* Unary arithmetic operators */

    /*!
     * \brief Operator unary plus
     */
    auto operator+()->Vector<T, _size>;

    /*!
     * \brief Operator unary minus
     */
    auto operator-()->Vector<T, _size>;


    /* Binary arithmetic operators */

    auto operator+(const VectorDerived<T, _size> &vector2)->Vector<T, _size>;
    auto operator-(const VectorDerived<T, _size> &vector2)->Vector<T, _size>;
    auto operator*(const VectorDerived<T, _size> &vector2)->Vector<T, _size>;
    auto operator/(const VectorDerived<T, _size> &vector2)->Vector<T, _size>;
    auto operator*(T scalar)->Vector<T, _size>;
    auto operator/(T scalar)->Vector<T, _size>;
    template<typename VectorDerived2>
    auto operator+=(const VectorDerived2 &vector)->VectorDerived<T, _size> &;
    template<typename VectorDerived2>
    auto operator-=(const VectorDerived2 &vector)->VectorDerived<T, _size> &;
    template<typename VectorDerived2>
    auto operator*=(const VectorDerived2 &vector)->VectorDerived<T, _size> &;
    template<typename VectorDerived2>
    auto operator/=(const VectorDerived2 &vector)->VectorDerived<T, _size> &;
    auto operator*=(T scalar)->VectorDerived<T, _size> &;
    auto operator/=(T scalar)->VectorDerived<T, _size> &;

private:

    auto derived() -> VectorDerived<T, _size> &;
    auto derived() const -> const VectorDerived<T, _size> &;

};


template<typename T, size_t _size = DynamicData>
class Vector
    : public VectorBase<Vector<T, _size>>
{

public:

    using value_type = T;
    using size_type = size_t;
    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;

    using iterator = typename Data<T, _size>::iterator;
    using const_iterator = typename Data<T, _size>::const_iterator;
    //using reverse_iterator       = typename Data<T, _size>::reverse_iterator;
    //using const_reverse_iterator = typename Data<T, _size>::const_reverse_iterator;

public:

    Vector();
    Vector(size_t size, T value = std::numeric_limits<T>().lowest());
    Vector(const Vector &vector);
    Vector(Vector &&vector) TL_NOEXCEPT;
    Vector(std::initializer_list<T> values);
    Vector(T *data, size_t size);
    ~Vector() = default;

    auto operator=(const Vector &vector)->Vector &;
    auto operator=(Vector &&vector) TL_NOEXCEPT -> Vector &;

public:

    void resize(size_t size);
    void resize(size_t size, T value);

    auto size() const TL_NOEXCEPT -> size_t;

    auto at(size_type position) -> reference;
    auto at(size_type position) const->const_reference;

    auto operator[](size_t position)->reference;
    auto operator[](size_t position) const->const_reference;

    auto front() -> reference;
    auto front() const->const_reference;
    auto back() -> reference;
    auto back() const->const_reference;
    auto begin() TL_NOEXCEPT->iterator;
    auto begin() const TL_NOEXCEPT->const_iterator;
    auto end() TL_NOEXCEPT->iterator;
    auto end() const TL_NOEXCEPT->const_iterator;

    auto data() -> pointer;
    auto data() const->const_pointer;

    auto x() const->T;
    auto y() const->T;
    auto z() const->T;
    auto w() const->T;

    auto x() -> T &;
    auto y() -> T &;
    auto z() -> T &;
    auto w() -> T &;

    bool operator == (const Vector &vector) const;
    bool operator != (const Vector &vector) const;
    bool operator <  (const Vector &vector) const;
    bool operator <= (const Vector &vector) const;
    bool operator >  (const Vector &vector) const;
    bool operator >= (const Vector &vector) const;

    static auto zero() -> Vector;
    static auto zero(size_t size) -> Vector;
    static auto unit() -> Vector;
    static auto unit(size_t size) -> Vector;
    static auto randon() -> Vector;
    static auto randon(size_t size) -> Vector;

private:

    Data<T, _size> _data;

};



/* Definición de alias Vector */

using Vector2i = Vector<int, 2>;
using Vector2d = Vector<double, 2>;
using Vector2f = Vector<float, 2>;
using Vector3i = Vector<int, 3>;
using Vector3d = Vector<double, 3>;
using Vector3f = Vector<float, 3>;
using Vector4i = Vector<int, 4>;
using Vector4d = Vector<double, 4>;
using Vector4f = Vector<float, 4>;




/*------------------------------------------------------------------------*/
/* VectorBase implementation                                              */
/*------------------------------------------------------------------------*/


template<
    template<typename, size_t _size = DynamicData>
class VectorDerived, typename T, size_t _size>
auto VectorBase<VectorDerived<T, _size>>::module() const -> double
{
    return sqrt(dotProduct(this->derived()));
}

template<
    template<typename, size_t _size = DynamicData>
class VectorDerived, typename T, size_t _size>
auto VectorBase<VectorDerived<T, _size>>::normalize() -> void
{
    *this /= static_cast<T>(this->module());
}

template<
    template<typename, size_t _size = DynamicData>
class VectorDerived, typename T, size_t _size>
auto VectorBase<VectorDerived<T, _size>>::dotProduct(const VectorDerived<T, _size> &vector) const -> double
{
    auto &derived = this->derived();

    TL_ASSERT(derived.size() == vector.size(), "Different vector size");

    double dot = static_cast<double>(derived[0]) * static_cast<double>(vector[0]);
    for (size_t i = 1; i < derived.size(); i++) {
        dot += static_cast<double>(derived[i]) * static_cast<double>(vector[i]);
    }

    return dot;
}

/* Unary arithmetic operators */

template<
    template<typename, size_t _size = DynamicData>
class VectorDerived, typename T, size_t _size>
auto VectorBase<VectorDerived<T, _size>>::operator+() -> Vector<T, _size>
{
    return this->derived();
}

template<
    template<typename, size_t _size = DynamicData>
class VectorDerived, typename T, size_t _size>
auto VectorBase<VectorDerived<T, _size>>::operator-() -> Vector<T, _size>
{
    static_assert(std::is_signed<T>::value, "Requires signed type");

    Vector<T, _size> vector = this->derived();

    for (size_t i = 0; i < vector.size(); i++) {
        vector[i] = -vector[i];
    }

    return vector;
}


/* Binary arithmetic operators */

template<
    template<typename, size_t _size = DynamicData>
class VectorDerived, typename T, size_t _size>
inline auto VectorBase<VectorDerived<T, _size>>::operator+(const VectorDerived<T, _size> &vector2) -> Vector<T, _size>
{
    Vector<T, _size> vector = this->derived();
    vector += vector2;
    return vector;
}

template<
    template<typename, size_t _size = DynamicData>
class VectorDerived, typename T, size_t _size>
inline auto VectorBase<VectorDerived<T, _size>>::operator-(const VectorDerived<T, _size> &vector2) -> Vector<T, _size>
{
    Vector<T, _size> vector = this->derived();
    vector -= vector2;
    return vector;
}

template<
    template<typename, size_t _size = DynamicData>
class VectorDerived, typename T, size_t _size>
inline auto VectorBase<VectorDerived<T, _size>>::operator*(const VectorDerived<T, _size> &vector2) -> Vector<T, _size>
{
    Vector<T, _size> vector = this->derived();
    vector *= vector2;
    return vector;
}

template<
    template<typename, size_t _size = DynamicData>
class VectorDerived, typename T, size_t _size>
inline auto VectorBase<VectorDerived<T, _size>>::operator/(const VectorDerived<T, _size> &vector2) -> Vector<T, _size>
{
    Vector<T, _size> vector = this->derived();
    vector /= vector2;
    return vector;
}

template<
    template<typename, size_t _size = DynamicData>
class VectorDerived, typename T, size_t _size>
inline auto VectorBase<VectorDerived<T, _size>>::operator*(T scalar) -> Vector<T, _size>
{
    Vector<T, _size> vector = this->derived();
    vector *= scalar;
    return vector;
}

template<
    template<typename, size_t _size = DynamicData>
class VectorDerived, typename T, size_t _size>
auto VectorBase<VectorDerived<T, _size>>::operator/(T scalar) -> Vector<T, _size>
{
    Vector<T, _size> vector = this->derived();
    vector /= scalar;
    return vector;
}

template<
    template<typename, size_t _size = DynamicData>
class VectorDerived, typename T, size_t _size>
template<typename VectorDerived2>
auto VectorBase<VectorDerived<T, _size>>::operator+=(const VectorDerived2 &vector) -> VectorDerived<T, _size> &
{
    auto &derived = this->derived();

    TL_ASSERT(derived.size() == vector.size(), "Different size vectors");

    size_t i{0};

#ifdef TL_HAVE_SIMD_INTRINSICS

    Packed<T> packed_a;
    Packed<T> packed_b;

    constexpr size_t packed_size = packed_a.size();
    size_t max_vector = (derived.size() / packed_size) * packed_size;

    if (this->properties.isEnabled(VectorDerived<T, _size>::Properties::contiguous_memory) &&
        vector.properties.isEnabled(VectorDerived2::Properties::contiguous_memory)) {

        for (; i < max_vector; i += packed_size) {

            packed_a.loadUnaligned(&derived[i]);
            packed_b.loadUnaligned(&vector[i]);

            packed_a += packed_b;
            packed_a.storeUnaligned(&derived[i]);

        }
    }

#endif

    for (; i < derived.size(); ++i) {
        derived[i] += vector[i];
    }

    return derived;
}

template<
    template<typename, size_t _size = DynamicData>
class VectorDerived, typename T, size_t _size>
template<typename VectorDerived2>
inline auto VectorBase<VectorDerived<T, _size>>::operator-=(const VectorDerived2 &vector) -> VectorDerived<T, _size> &
{
    auto &derived = this->derived();

    TL_ASSERT(derived.size() == vector.size(), "");

    size_t i{0};

#ifdef TL_HAVE_SIMD_INTRINSICS

    Packed<T> packed_a;
    Packed<T> packed_b;

    constexpr size_t packed_size = packed_a.size();

    size_t max_vector = (derived.size() / packed_size) * packed_size;

    if (this->properties.isEnabled(VectorDerived<T, _size>::Properties::contiguous_memory) &&
        vector.properties.isEnabled(VectorDerived2::Properties::contiguous_memory)) {

        for (; i < max_vector; i += packed_size) {

            packed_a.loadUnaligned(&derived[i]);
            packed_b.loadUnaligned(&vector[i]);
            packed_a -= packed_b;
            packed_a.storeUnaligned(&derived[i]);

        }
    }

#endif

    for (; i < derived.size(); ++i) {
        derived[i] -= vector[i];
    }

    return derived;
}

template<
    template<typename, size_t _size = DynamicData>
class VectorDerived, typename T, size_t _size>
template<typename VectorDerived2>
inline auto VectorBase<VectorDerived<T, _size>>::operator*=(const VectorDerived2 &vector) -> VectorDerived<T, _size> &
{
    auto &derived = this->derived();

    TL_ASSERT(derived.size() == vector.size(), "");

    size_t i{0};

#ifdef TL_HAVE_SIMD_INTRINSICS

    Packed<T> packed_a;
    Packed<T> packed_b;

    constexpr size_t packed_size = packed_a.size();

    if (this->properties.isEnabled(VectorDerived<T, _size>::Properties::contiguous_memory) &&
        vector.properties.isEnabled(VectorDerived2::Properties::contiguous_memory)) {

        size_t max_vector = (derived.size() / packed_size) * packed_size;
        for (; i < max_vector; i += packed_size) {

            packed_a.loadUnaligned(&derived[i]);
            packed_b.loadUnaligned(&vector[i]);
            packed_a *= packed_b;
            packed_a.storeUnaligned(&derived[i]);

        }
    }

#endif

    for (; i < derived.size(); ++i) {
        derived[i] *= vector[i];
    }



    return derived;
}

template<
    template<typename, size_t _size = DynamicData>
class VectorDerived, typename T, size_t _size>
template<typename VectorDerived2>
inline auto VectorBase<VectorDerived<T, _size>>::operator/=(const VectorDerived2 &vector) -> VectorDerived<T, _size> &
{
    auto &derived = this->derived();

    TL_ASSERT(derived.size() == vector.size(), "");

    size_t i{0};

#ifdef TL_HAVE_SIMD_INTRINSICS

    Packed<T> packed_a;
    Packed<T> packed_b;

    constexpr size_t packed_size = packed_a.size();
    size_t max_vector = (derived.size() / packed_size) * packed_size;

    if (this->properties.isEnabled(Properties::contiguous_memory) &&
        vector.properties.isEnabled(VectorDerived2::Properties::contiguous_memory)) {

        for (; i < max_vector; i += packed_size) {

            packed_a.loadUnaligned(&derived[i]);
            packed_b.loadUnaligned(&vector[i]);
            packed_a /= packed_b;
            packed_a.storeUnaligned(&derived[i]);

        }
    }

#endif

    for (; i < derived.size(); ++i) {
        derived[i] /= vector[i];
    }

    return derived;
}

template<
    template<typename, size_t _size = DynamicData>
class VectorDerived, typename T, size_t _size>
inline auto VectorBase<VectorDerived<T, _size>>::operator*=(T scalar) -> VectorDerived<T, _size> &
{
    auto &derived = this->derived();

    size_t i{0};

#ifdef TL_HAVE_SIMD_INTRINSICS

    Packed<T> packed_a;
    Packed<T> packed_b(scalar);

    constexpr size_t packed_size = packed_a.size();
    size_t max_vector = (derived.size() / packed_size) * packed_size;

    if (this->properties.isEnabled(Properties::contiguous_memory)) {
        for (; i < max_vector; i += packed_size) {

            packed_a.loadUnaligned(&derived[i]);
            packed_a *= packed_b;
            packed_a.storeUnaligned(&derived[i]);

        }
    }

#endif

    for (; i < derived.size(); i++) {
        derived[i] *= scalar;
    }

    return derived;
}

template<
    template<typename, size_t _size = DynamicData>
class VectorDerived, typename T, size_t _size>
inline auto VectorBase<VectorDerived<T, _size>>::operator/=(T scalar) -> VectorDerived<T, _size> &
{
    auto &derived = this->derived();

    TL_ASSERT(scalar != consts::zero<T>, "Division by zero");

    size_t i{0};

#ifdef TL_HAVE_SIMD_INTRINSICS

    Packed<T> packed_a;
    Packed<T> packed_b(scalar);

    constexpr size_t packed_size = packed_a.size();
    size_t max_vector = (derived.size() / packed_size) * packed_size;

    if (this->properties.isEnabled(Properties::contiguous_memory)) {
        for (; i < max_vector; i += packed_size) {

            packed_a.loadUnaligned(&derived[i]);
            packed_a /= packed_b;
            packed_a.storeUnaligned(&derived[i]);

        }
    }

#endif

    for (; i < derived.size(); ++i) {
        derived[i] /= scalar;
    }

    return derived;
}

template<
    template<typename, size_t _size = DynamicData>
class VectorDerived, typename T, size_t _size>
inline auto VectorBase<VectorDerived<T, _size>>::derived() -> VectorDerived<T, _size> &
{
    return *static_cast<VectorDerived<T, _size> *>(this);
}

template<
    template<typename, size_t _size = DynamicData>
class VectorDerived, typename T, size_t _size>
inline auto VectorBase<VectorDerived<T, _size>>::derived() const -> const VectorDerived<T, _size> &
{
    return *static_cast<const VectorDerived<T, _size> *>(this);
}




/*------------------------------------------------------------------------*/
/* Vector implementation                                                  */
/*------------------------------------------------------------------------*/


template<typename T, size_t _size>
inline Vector<T, _size>::Vector()
  : _data(Data<T, _size>())
{
}

template<typename T, size_t _size>
inline Vector<T, _size>::Vector(size_t size, T value)
  : _data(Data<T, _size>(size, value))
{
}

template<typename T, size_t _size>
inline Vector<T, _size>::Vector(const Vector &vector)
  : _data(vector._data)
{
}

template<typename T, size_t _size>
inline Vector<T, _size>::Vector(Vector &&vector) TL_NOEXCEPT
  : _data(std::move(vector._data))
{
}

template<typename T, size_t _size>
inline Vector<T, _size>::Vector(std::initializer_list<T> values)
  : _data(Data<T, _size>(values))
{
}

template<typename T, size_t _size>
inline Vector<T, _size>::Vector(T *data, size_t size)
    : _data(Data<T, _size>(data, size))
{
}

template<typename T, size_t _size>
inline auto Vector<T, _size>::operator=(const Vector &vector) -> Vector &
{
    if (this != &vector) {
        this->_data = vector._data;
    }

    return (*this);
}

template<typename T, size_t _size>
inline auto Vector<T, _size>::operator=(Vector &&vector) TL_NOEXCEPT -> Vector &
{
    if (this != &vector) {
        this->_data = std::forward<Data<T, _size>>(vector._data);
    }

    return (*this);
}

template<typename T, size_t _size>
inline void Vector<T, _size>::resize(size_t size)
{
    static_assert(_size == DynamicData, "Fixed-size vector not support resize");

    _data.resize(size);
}

template<typename T, size_t _size>
inline void Vector<T, _size>::resize(size_t size, T value)
{
    static_assert(_size == DynamicData, "Fixed-size vector not support resize");

    _data.resize(size, value);
}

template<typename T, size_t _size>
inline auto Vector<T, _size>::size() const TL_NOEXCEPT -> size_t
{
    return _data.size();
}

template<typename T, size_t _size>
inline auto Vector<T, _size>::at(size_type position) -> reference
{
    return _data.at(position);
}

template<typename T, size_t _size>
inline auto Vector<T, _size>::at(size_type position) const -> const_reference
{
    return _data.at(position);
}

template<typename T, size_t _size>
inline auto Vector<T, _size>::operator[](size_t position) -> reference
{
    return _data[position];
}

template<typename T, size_t _size>
inline auto Vector<T, _size>::operator[](size_t position) const -> const_reference
{
    return _data[position];
}

template<typename T, size_t _size>
inline auto Vector<T, _size>::front() -> reference
{
    return _data.front();
}

template<typename T, size_t _size>
inline auto Vector<T, _size>::front() const -> const_reference
{
    return _data.front();
}

template<typename T, size_t _size>
inline auto Vector<T, _size>::back() -> reference
{
    return _data.back();
}

template<typename T, size_t _size>
inline auto Vector<T, _size>::back() const -> const_reference
{
    return _data.back();
}

template<typename T, size_t _size>
inline auto Vector<T, _size>::begin() TL_NOEXCEPT -> iterator
{
    return _data.begin();
}

template<typename T, size_t _size>
inline auto Vector<T, _size>::begin() const TL_NOEXCEPT -> const_iterator
{
    return _data.begin();
}

template<typename T, size_t _size>
inline auto Vector<T, _size>::end() TL_NOEXCEPT -> iterator
{
    return _data.end();
}

template<typename T, size_t _size>
inline auto Vector<T, _size>::end() const TL_NOEXCEPT -> const_iterator
{
    return _data.end();
}

template<typename T, size_t _size>
inline auto Vector<T, _size>::data() -> pointer
{
    return _data.data();
}

template<typename T, size_t _size>
inline auto Vector<T, _size>::data() const -> const_pointer
{
    return _data.data();
}

template<typename T, size_t _size>
inline auto Vector<T, _size>::x() const -> T
{
    static_assert(_size != DynamicData, "Unsupported method for dynamic vectors");
    static_assert(_size >= 2 && _size <= 4, "Method only supported for 2, 3 or 4 element vectors");
    return _data[0];
}

template<typename T, size_t _size>
inline auto Vector<T, _size>::y() const -> T
{
    static_assert(_size != DynamicData, "Unsupported method for dynamic vectors");
    static_assert(_size >= 2 && _size <= 4, "Method only supported for 2, 3 or 4 element vectors");
    return _data[1];
}

template<typename T, size_t _size>
inline auto Vector<T, _size>::z() const -> T
{
    static_assert(_size != DynamicData, "Unsupported method for dynamic vectors");
    static_assert(_size >= 2 && _size <= 4, "Method only supported for 2, 3 or 4 element vectors");
    return _data[2];
}

template<typename T, size_t _size>
inline auto Vector<T, _size>::w() const -> T
{
    static_assert(_size != DynamicData, "Unsupported method for dynamic vectors");
    static_assert(_size >= 2 && _size <= 4, "Method only supported for 2, 3 or 4 element vectors");
    return _data[3];
}

template<typename T, size_t _size>
inline auto Vector<T, _size>::x() -> T &
{
    static_assert(_size != DynamicData, "Unsupported method for dynamic vectors");
    static_assert(_size >= 2 && _size <= 4, "Method only supported for 2, 3 or 4 element vectors");
    return _data[0];
}

template<typename T, size_t _size>
inline auto Vector<T, _size>::y() -> T &
{
    static_assert(_size != DynamicData, "Unsupported method for dynamic vectors");
    static_assert(_size >= 2 && _size <= 4, "Method only supported for 2, 3 or 4 element vectors");
    return _data[1];
}

template<typename T, size_t _size>
inline auto Vector<T, _size>::z() -> T &
{
    static_assert(_size != DynamicData, "Unsupported method for dynamic vectors");
    static_assert(_size >= 2 && _size <= 4, "Method only supported for 2, 3 or 4 element vectors");
    return _data[2];
}

template<typename T, size_t _size>
inline auto Vector<T, _size>::w() -> T &
{
    static_assert(_size != DynamicData, "Unsupported method for dynamic vectors");
    static_assert(_size >= 2 && _size <= 4, "Method only supported for 2, 3 or 4 element vectors");
    return _data[3];
}

template<typename T, size_t _size> inline
bool Vector<T, _size>::operator == (const Vector<T, _size> &vector) const
{
    return this->_data == vector.mData;
}

template<typename T, size_t _size> inline
bool Vector<T, _size>::operator != (const Vector<T, _size> &vector) const
{
    return this->_data != vector.mData;
}

template<typename T, size_t _size> inline
bool Vector<T, _size>::operator <  (const Vector<T, _size> &vector) const
{
    return this->_data < vector.mData;
}

template<typename T, size_t _size> inline
bool Vector<T, _size>::operator <= (const Vector<T, _size> &vector) const
{
    return this->_data <= vector.mData;
}

template<typename T, size_t _size> inline
bool Vector<T, _size>::operator >  (const Vector<T, _size> &vector) const
{
    return this->_data > vector.mData;
}

template<typename T, size_t _size> inline
bool Vector<T, _size>::operator >= (const Vector<T, _size> &vector) const
{
    return this->_data > vector.mData;
}

template<typename T, size_t _size> inline
auto Vector<T, _size>::zero() -> Vector
{
    Vector<T, _size> vector;
    for (size_t i = 0; i < vector.size(); i++) {
        vector[i] = consts::zero<T>;
    }
    return vector;
}

template<typename T, size_t _size> inline
auto Vector<T, _size>::zero(size_t size) -> Vector
{
    static_assert(_size == DynamicData, "Fixed-size vector not support resize");
    return Vector<T>(size, consts::zero<T>);
}

template<typename T, size_t _size> inline
auto Vector<T, _size>::unit() -> Vector
{
    Vector<T, _size> vector;

    for (size_t i = 0; i < vector.size(); i++) {
        vector[i] = consts::one<T>;
    }

    return vector;
}

template<typename T, size_t _size> inline
auto Vector<T, _size>::unit(size_t size) -> Vector
{
    static_assert(_size == DynamicData, "Fixed-size vector not support resize");
    return Vector<T>(size, consts::one<T>);
}

template<typename T, size_t _size> inline
auto Vector<T, _size>::randon() -> Vector
{
    Vector<T, _size> vector;

    std::random_device rd;
    std::mt19937 random_number_engine(rd());
    std::uniform_real_distribution<> distribution(0.0, 99.0);

    for (size_t i = 0; i < vector.size(); i++) {
        vector[i] = static_cast<T>(distribution(random_number_engine));
    }

    return vector;
}

template<typename T, size_t _size> inline
auto Vector<T, _size>::randon(size_t size) -> Vector
{
    static_assert(_size == DynamicData, "Fixed-size vector not support resize");

    Vector<T, _size> vector(size);

    std::random_device rd;
    std::mt19937 random_number_engine(rd());
    std::uniform_real_distribution<> distribution(0.0, 99.0);

    for (size_t i = 0; i < vector.size(); i++) {
        vector[i] = static_cast<T>(distribution(random_number_engine));
    }

    return vector;
}

/*------------------------------------------------------------------------*/
/* End Vector implementation                                              */
/*------------------------------------------------------------------------*/





/* Binary arithmetic operators */

/* Addition */

template<typename T, size_t _size> 
inline Vector<T, _size> operator +(Vector<T, _size> &vector1,
                                   Vector<T, _size> &&vector2)
{
    vector2 += vector1;
    return vector2;
}

template<typename T, size_t _size>
inline Vector<T, _size> operator +(Vector<T, _size> &&vector1,
                                   const Vector<T, _size> &vector2)
{
    vector1 += vector2;
    return vector1;
}

template<typename T, size_t _size>
inline Vector<T, _size> operator +(Vector<T, _size> &&vector1,
                                   Vector<T, _size> &&vector2)
{
    vector1 += vector2;
    return vector1;
}


/* Subtraction */

template<typename T, size_t _size>
inline Vector<T, _size> operator -(Vector<T, _size> &vector1,
                                   Vector<T, _size> &&vector2)
{
    vector2 -= vector1;
    return -vector2;
}

template<typename T, size_t _size>
inline Vector<T, _size> operator -(Vector<T, _size> &&vector1,
                                   const Vector<T, _size> &vector2)
{
    vector1 -= vector2;
    return vector1;
}

template<typename T, size_t _size>
inline Vector<T, _size> operator -(Vector<T, _size> &&vector1,
                                   Vector<T, _size> &&vector2)
{
    vector1 -= vector2;
    return vector1;
}


/* Multiplication */

template<typename T, size_t _size>
inline Vector<T, _size> operator *(const Vector<T, _size> &vector1,
                                   Vector<T, _size> &&vector2)
{
    vector2 *= vector1;
    return vector2;
}

template<typename T, size_t _size>
inline Vector<T, _size> operator *(Vector<T, _size> &&vector1,
                                   const Vector<T, _size> &vector2)
{
    vector1 *= vector2;
    return vector1;
}

template<typename T, size_t _size>
inline Vector<T, _size> operator *(Vector<T, _size> &&vector1,
                                   Vector<T, _size> &&vector2)
{
    vector1 *= vector2;
    return vector1;
}


/* Division */

template<typename T, size_t _size>
inline Vector<T, _size> operator /(Vector<T, _size> &&vector1,
                                   const Vector<T, _size> &vector2)
{
    vector1 /= vector2;
    return vector1;
}

template<typename T, size_t _size>
inline Vector<T, _size> operator /(Vector<T, _size> &&vector1,
                                   Vector<T, _size> &&vector2)
{
    vector1 /= vector2;
    return vector1;
}


/* Multiplication vector * scalar */

template<typename T, size_t _size>
inline Vector<T, _size> operator *(Vector<T, _size> &&vector, T scalar)
{
    vector *= scalar;
    return vector;
}

template<
  template<typename, size_t _size = DynamicData>
  class VectorDerived, typename T, size_t _size>
Vector<T, _size> operator *(T scalar, const VectorDerived<T, _size> &vector)
{
    Vector<T, _size> _vector = vector;
    _vector *= scalar;
    return _vector;
}

template<typename T, size_t _size>
inline Vector<T, _size> operator * (T scalar, Vector<T, _size> &&vector)
{
    vector *= scalar;
    return vector;
}


/* Division Vector / scalar */

template<typename T, size_t _size>
inline Vector<T, _size> operator / (Vector<T, _size> &&vector, T scalar)
{
    vector /= scalar;
    return vector;
}




template<typename T, size_t _size> 
double dotProduct(const Vector<T, _size> &v1,
                  const Vector<T, _size> &v2)
{
    TL_ASSERT(v1.size() == v2.size(), "Different vector size");

    double dot = static_cast<double>(v1[0]) * static_cast<double>(v2[0]);
    for (size_t i = 1; i < v1.size(); i++) {
        dot += static_cast<double>(v1[i]) * static_cast<double>(v2[i]);
    }
    return dot;
}




template<typename T, size_t _size>
std::ostream &operator<< (std::ostream &os, const Vector<T, _size> &vector)
{
    for (size_t i = 0; i < vector.size(); i++) {
        os << " " << vector[i] << "\n";
    }
    os << std::flush;
    return os;
}

template<typename T, size_t _size>
std::ostream &operator<< (std::ostream &os, const Vector<T, _size> *vector)
{
    for (size_t i = 0; i < vector->size(); i++) {
        os << " " << (*vector)[i] << "\n";
    }
    os << std::flush;
    return os;
}

/*! \} */ // end of algebra

/*! \} */ // end of math

} // End namespace tl

