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

#include <vector>
#include <random>
#include <iomanip>

#include "tidop/core/base/exception.h"
#include "tidop/math/math.h"
#include "tidop/math/base/simd.h"
#include "tidop/math/base/data.h"


namespace tl
{

/*! \addtogroup Vector
 *  \{
 */

 /// \cond

template<typename T>
class VectorBase;

template<typename T, size_t _size>
class Vector;

/// \endcond

/*!
 * \class VectorBase
 * \brief Base class for vector operations.
 *
 * \tparam VectorDerived A template parameter representing the derived vector class.
 * \tparam T The type of elements in the vector.
 * \tparam _size The size of the vector, defaulting to `DynamicData` if not specified.
 *
 * The `VectorBase` class serves as a base for vector classes, defining fundamental
 * operations between vectors and between vectors and scalars. It provides a foundation
 * for creating vector types with customizable behavior.
 *
 * The `_size` parameter allows the class to work with both static and dynamic vectors.
 * If `_size` is set to `DynamicData`, the vector will be dynamic, allowing its size to
 * change at runtime. Otherwise, the vector will be static, with a fixed size defined
 * at compile time.
 */
template<
    template<typename, size_t _size = DynamicData>
class VectorDerived, typename T, size_t _size
>
class VectorBase<VectorDerived<T, _size>>
{

public:

    /*!
     * \enum Properties
     * \brief Enum to define vector properties.
     *
     * - `contiguous_memory`: Indicates the vector is stored in contiguous memory.
     */
    enum class Properties
    {
        contiguous_memory = 0x01
    };

public:

    /*!
     * \brief Flags for vector properties.
     */
    EnumFlags<Properties> properties;

public:

    /*!
     * \brief Default constructor.
     */
    VectorBase();

    /*!
     * \brief Calculates the magnitude (or length) of the vector.
     * \return The magnitude of the vector.
     */
    auto module() const -> double;

    /*!
     * \brief Normalizes the vector, making its magnitude equal to 1.
     */
    auto normalize() -> void;

    /*!
     * \brief Computes the dot product with another vector.
     * \param[in] vector Another vector to compute the dot product with.
     * \tparam VectorDerived2 The type of the other vector.
     * \return The dot product as a double.
     */
    template<typename VectorDerived2>
    auto dotProduct(const VectorDerived2 &vector) const -> double;

    auto sum() const->T;

    /*!
     * \brief Conversion operator to another vector type.
     * \tparam VectorDerived2 The type to convert to.
     * \return A new vector of type `VectorDerived2`.
     */
    template<typename VectorDerived2>
    operator VectorDerived2() const
    {
        VectorDerived2 v;
        v = this->derived();
        return v;
    }

    /* Unary arithmetic operators */

    /*!
     * \brief Unary plus operator.
     * \return A copy of the vector.
     */
    auto operator+() const -> Vector<T, _size>;

    /*!
     * \brief Unary minus operator.
     * \return A negated copy of the vector.
     */
    auto operator-() const -> Vector<T, _size>;

    /* Binary arithmetic operators */

    /*!
     * \brief Adds two vectors.
     * \param[in] vector2 The vector to add.
     * \return The sum of the vectors.
     */
    auto operator+(const VectorDerived<T, _size> &vector2) const -> Vector<T, _size>;

    /*!
     * \brief Subtracts one vector from another.
     * \param[in] vector2 The vector to subtract.
     * \return The difference of the vectors.
     */
    auto operator-(const VectorDerived<T, _size> &vector2) const -> Vector<T, _size>;

    /*!
     * \brief Multiplies two vectors element-wise.
     * \param[in] vector2 The vector to multiply.
     * \return The product of the vectors.
     */
    auto operator*(const VectorDerived<T, _size> &vector2) const -> Vector<T, _size>;

    /*!
     * \brief Divides two vectors element-wise.
     * \param[in] vector2 The vector to divide by.
     * \return The quotient of the vectors.
     */
    auto operator/(const VectorDerived<T, _size> &vector2) const -> Vector<T, _size>;

    /*!
     * \brief Multiplies the vector by a scalar.
     * \param[in] scalar The scalar to multiply by.
     * \return The scaled vector.
     */
    auto operator*(T scalar) const -> Vector<T, _size>;

    /*!
     * \brief Divides the vector by a scalar.
     * \param[in] scalar The scalar to divide by.
     * \return The scaled vector.
     */
    auto operator/(T scalar) const -> Vector<T, _size>;

    /*!
     * \brief Adds another vector to this vector.
     * \param[in] vector The vector to add.
     * \tparam VectorDerived2 The type of the other vector.
     * \return A reference to this vector.
     */
    template<typename VectorDerived2>
    auto operator+=(const VectorDerived2 &vector) -> VectorDerived<T, _size> &;

    /*!
     * \brief Subtracts another vector from this vector.
     * \param[in] vector The vector to subtract.
     * \tparam VectorDerived2 The type of the other vector.
     * \return A reference to this vector.
     */
    template<typename VectorDerived2>
    auto operator-=(const VectorDerived2 &vector) -> VectorDerived<T, _size> &;

    /*!
     * \brief Multiplies this vector by another vector element-wise.
     * \param[in] vector The vector to multiply by.
     * \tparam VectorDerived2 The type of the other vector.
     * \return A reference to this vector.
     */
    template<typename VectorDerived2>
    auto operator*=(const VectorDerived2 &vector) -> VectorDerived<T, _size> &;

    /*!
     * \brief Divides this vector by another vector element-wise.
     * \param[in] vector The vector to divide by.
     * \tparam VectorDerived2 The type of the other vector.
     * \return A reference to this vector.
     */
    template<typename VectorDerived2>
    auto operator/=(const VectorDerived2 &vector) -> VectorDerived<T, _size> &;

    /*!
     * \brief Multiplies the vector by a scalar.
     * \param[in] scalar The scalar to multiply by.
     * \return A reference to this vector.
     */
    auto operator*=(T scalar) -> VectorDerived<T, _size> &;

    /*!
     * \brief Divides the vector by a scalar.
     * \param[in] scalar The scalar to divide by.
     * \return A reference to this vector.
     */
    auto operator/=(T scalar) -> VectorDerived<T, _size> &;

protected:

    /*!
     * \brief Sets the values of this vector to match another vector.
     * \param[in] vector The vector to copy values from.
     * \tparam VectorDerived2 The type of the other vector.
     */
    template<typename VectorDerived2>
    void set(const VectorDerived2 &vector);

private:

    auto derived() -> VectorDerived<T, _size> &;
    auto derived() const -> const VectorDerived<T, _size> &;

};



/*!
 * \class Vector
 * \brief Represents a mathematical vector, supporting both static and dynamic sizes.
 *
 * \tparam T The type of elements in the vector.
 * \tparam _size The size of the vector. If not specified, defaults to `DynamicData`, making it a dynamic vector.
 *
 * The `Vector` class provides a robust implementation for vectors of various sizes, including
 * operations for accessing, modifying, and manipulating vector data. It inherits from `VectorBase`
 * and supports both static and dynamic sizing depending on the template parameter `_size`.
 *
 */
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

public:

    /*!
     * \brief Default constructor. Initializes an empty vector.
     */
    Vector();

    /*!
     * \brief Constructs a vector of a given size, initializing all elements to a specified value.
     * \param[in] size The size of the vector.
     * \param[in] value The value to initialize each element to (default is the lowest value of `T`).
     */
    explicit Vector(size_t size, T value = std::numeric_limits<T>().lowest());

    /*!
     * \brief Copy constructor.
     * \param[in] vector The vector to copy.
     */
    Vector(const Vector &vector);

    /*!
     * \brief Move constructor.
     * \param[in] vector The vector to move.
     */
    Vector(Vector &&vector) TL_NOEXCEPT;

    /*!
     * \brief Constructs a vector from an initializer list.
     * \param[in] values The values to initialize the vector with.
     */
    Vector(std::initializer_list<T> values);

    /*!
     * \brief Constructs a vector from a raw pointer and size.
     * \param[in] data Pointer to the data.
     * \param[in] size The size of the vector.
     */
    Vector(T *data, size_t size);

    ~Vector() = default;

    /*!
     * \brief Assignment operator for copying another vector.
     * \param vector The vector to copy from.
     * \return A reference to this vector.
     */
    auto operator=(const Vector &vector) -> Vector &;

    /*!
     * \brief Move assignment operator.
     * \param vector The vector to move from.
     * \return A reference to this vector.
     */
    auto operator=(Vector &&vector) TL_NOEXCEPT -> Vector &;

    /*!
     * \brief Assignment operator for assigning a derived vector.
     * \tparam VectorDerived The type of the derived vector.
     * \param vector The derived vector to assign from.
     * \return A reference to this vector.
     */
    template<typename VectorDerived>
    auto operator=(const VectorDerived &vector) -> Vector &;

public:

    /*!
     * \brief Resizes the vector to a new size.
     * \param[in] size The new size of the vector.
     */
    void resize(size_t size);

    /*!
     * \brief Resizes the vector to a new size and initializes new elements to a specified value.
     * \param[in] size The new size of the vector.
     * \param[in] value The value to initialize new elements with.
     */
    void resize(size_t size, T value);

    /*!
     * \brief Returns the size of the vector.
     * \return The number of elements in the vector.
     */
    auto size() const TL_NOEXCEPT -> size_t;

    /*!
     * \brief Accesses the element at the specified position with bounds checking.
     *
     * \param[in] position The position of the element to access.
     * \return A reference to the element at the specified position.
     * \throws std::out_of_range if the position is out of bounds.
     */
    auto at(size_type position) -> reference;

    /*!
     * \brief Accesses the element at the specified position with bounds checking (const version).
     *
     * \param[in] position The position of the element to access.
     * \return A const reference to the element at the specified position.
     * \throws std::out_of_range if the position is out of bounds.
     */
    auto at(size_type position) const -> const_reference;

    /*!
     * \brief Accesses the element at the specified position without bounds checking.
     *
     * \param[in] position The position of the element to access.
     * \return A reference to the element at the specified position.
     * \note No bounds checking is performed, so accessing an invalid position
     *       may result in undefined behavior.
     */
    auto operator[](size_t position) TL_NOEXCEPT -> reference;

    /*!
     * \brief Accesses the element at the specified position without bounds checking (const version).
     *
     * \param position The position of the element to access.
     * \return A const reference to the element at the specified position.
     * \note No bounds checking is performed, so accessing an invalid position
     *       may result in undefined behavior.
     */
    auto operator[](size_t position) const TL_NOEXCEPT -> const_reference;

    /*!
     * \brief Access the first element in the vector.
     * \return A reference to the first element.
     */
    auto front() TL_NOEXCEPT -> reference;

    /*!
     * \brief Access the first element in the vector (const version).
     * \return A const reference to the first element.
     */
    auto front() const TL_NOEXCEPT -> const_reference;

    /*!
     * \brief Access the last element in the vector.
     * \return A reference to the last element.
     */
    auto back() TL_NOEXCEPT -> reference;

    /*!
     * \brief Access the last element in the vector (const version).
     * \return A const reference to the last element.
     * \throws std::out_of_range if the vector is empty.
     */
    auto back() const TL_NOEXCEPT -> const_reference;

    /*!
     * \brief Returns an iterator to the beginning of the vector.
     * \return An iterator to the beginning of the vector.
     */
    auto begin() TL_NOEXCEPT -> iterator;

    /*!
     * \brief Returns a const iterator to the beginning of the vector.
     * \return A const iterator to the beginning of the vector.
     */
    auto begin() const TL_NOEXCEPT -> const_iterator;

    /*!
     * \brief Returns an iterator to the end of the vector.
     * \return An iterator to the end of the vector.
     */
    auto end() TL_NOEXCEPT -> iterator;

    /*!
     * \brief Returns a const iterator to the end of the vector.
     * \return A const iterator to the end of the vector.
     */
    auto end() const TL_NOEXCEPT -> const_iterator;

    /*!
     * \brief Returns a pointer to the data array of the vector.
     * \return A pointer to the data array.
     */
    auto data() TL_NOEXCEPT -> pointer;

    /*!
     * \brief Returns a const pointer to the data array of the vector.
     * \return A const pointer to the data array.
     */
    auto data() const TL_NOEXCEPT -> const_pointer;

    /*!
     * \brief Access the x-component of the vector.
     * \return A const reference to the x-component.
     * \note Only valid for vectors with size at least 1.
     */
    auto x() const TL_NOEXCEPT -> const_reference;

    /*!
     * \brief Access the y-component of the vector.
     * \return A const reference to the y-component.
     * \note Only valid for vectors with size at least 2.
     */
    auto y() const TL_NOEXCEPT -> const_reference;

    /*!
     * \brief Access the z-component of the vector.
     * \return A const reference to the z-component.
     * \note Only valid for vectors with size at least 3.
     */
    auto z() const TL_NOEXCEPT -> const_reference;

    /*!
     * \brief Access the w-component of the vector.
     * \return A const reference to the w-component.
     * \note Only valid for vectors with size at least 4.
     */
    auto w() const TL_NOEXCEPT -> const_reference;

    /*!
     * \brief Access the x-component of the vector (non-const version).
     * \return A reference to the x-component.
     * \note Only valid for vectors with size at least 1.
     */
    auto x() TL_NOEXCEPT -> reference;

    /*!
     * \brief Access the y-component of the vector (non-const version).
     * \return A reference to the y-component.
     * \note Only valid for vectors with size at least 2.
     */
    auto y() TL_NOEXCEPT -> reference;

    /*!
     * \brief Access the z-component of the vector (non-const version).
     * \return A reference to the z-component.
     * \note Only valid for vectors with size at least 3.
     */
    auto z() TL_NOEXCEPT-> reference;

    /*!
     * \brief Access the w-component of the vector (non-const version).
     * \return A reference to the w-component.
     * \note Only valid for vectors with size at least 4.
     */
    auto w() TL_NOEXCEPT -> reference;

    /*!
     * \brief Equality operator for comparing two vectors.
     * \param vector The vector to compare with.
     * \return True if the vectors are equal, false otherwise.
     */
    bool operator == (const Vector &vector) const;

    /*!
     * \brief Inequality operator for comparing two vectors.
     * \param vector The vector to compare with.
     * \return True if the vectors are not equal, false otherwise.
     */
    bool operator != (const Vector &vector) const;

    /*!
     * \brief Less-than operator for comparing two vectors.
     * \param vector The vector to compare with.
     * \return True if this vector is less than the other, false otherwise.
     */
    bool operator <  (const Vector &vector) const;

    /*!
     * \brief Less-than-or-equal operator for comparing two vectors.
     * \param vector The vector to compare with.
     * \return True if this vector is less than or equal to the other, false otherwise.
     */
    bool operator <= (const Vector &vector) const;

    /*!
     * \brief Greater-than operator for comparing two vectors.
     * \param vector The vector to compare with.
     * \return True if this vector is greater than the other, false otherwise.
     */
    bool operator >  (const Vector &vector) const;

    /*!
     * \brief Greater-than-or-equal operator for comparing two vectors.
     * \param vector The vector to compare with.
     * \return True if this vector is greater than or equal to the other, false otherwise.
     */
    bool operator >= (const Vector &vector) const;

    /*!
     * \brief Creates a zero vector of a specified size.
     * \param size The size of the vector.
     * \return A vector with all elements initialized to zero.
     */
    static auto zero(size_t size = 0) -> Vector;

    /*!
     * \brief Creates a unit vector of a specified size.
     * \param size The size of the vector.
     * \return A unit vector.
     */
    static auto unit(size_t size = 0) -> Vector;

    /*!
     * \brief Creates a random vector of a specified size.
     * \param size The size of the vector.
     * \return A vector with randomly initialized elements.
     */
    static auto randon(size_t size = 0) -> Vector;

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
VectorBase<VectorDerived<T, _size>>::VectorBase()
{
    this->properties.enable(Properties::contiguous_memory);
}

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
auto VectorBase<VectorDerived<T, _size>>::sum() const -> T
{
    T summation{};

    auto &derived = this->derived();
    size_t i = 0;

#ifdef TL_HAVE_SIMD_INTRINSICS
    using Scalar = std::remove_cv_t<T>;
    Packed<Scalar> packed_a;
    constexpr size_t packed_size = packed_a.size();
    size_t max_vector = (derived.size() / packed_size) * packed_size;
    
    if (this->properties.isEnabled(VectorDerived<T, _size>::Properties::contiguous_memory)) {

        for (; i < max_vector; i += packed_size) {
            packed_a.loadUnaligned(&derived[i]);
            summation += packed_a.sum();
        }

    }

#endif

    for (; i < derived.size(); ++i) {
        summation += derived[i];
    }

    return summation;
}

template<
    template<typename, size_t _size = DynamicData>
class VectorDerived, typename T, size_t _size>
template<typename VectorDerived2>
auto VectorBase<VectorDerived<T, _size>>::dotProduct(const VectorDerived2 &vector) const -> double
{
    auto &derived = this->derived();

    TL_ASSERT(derived.size() == vector.size(), "Different vector size");

    double dot = 0.0;
    size_t i = 0;

#ifdef TL_HAVE_SIMD_INTRINSICS
    using Scalar = std::remove_cv_t<T>;
    Packed<Scalar> packed_a;
    Packed<Scalar> packed_b;
    Packed<Scalar> packed_result(0);

    constexpr size_t packed_size = packed_a.size();
    size_t max_vector = (derived.size() / packed_size) * packed_size;

    if (this->properties.isEnabled(VectorDerived<T, _size>::Properties::contiguous_memory) &&
        vector.properties.isEnabled(VectorDerived2::Properties::contiguous_memory)) {

        for (; i < max_vector; i += packed_size) {

            packed_a.loadUnaligned(&derived[i]);
            packed_b.loadUnaligned(&vector[i]);

            packed_result += packed_a * packed_b;

        }

        dot += static_cast<double>(packed_result.sum());
    }
#endif

    for (; i < derived.size(); ++i) {
        dot += static_cast<double>(derived[i]) * static_cast<double>(vector[i]);
    }

    return dot;
}

/* Unary arithmetic operators */

template<
    template<typename, size_t _size = DynamicData>
class VectorDerived, typename T, size_t _size>
auto VectorBase<VectorDerived<T, _size>>::operator+() const -> Vector<T, _size>
{
    return this->derived();
}

template<
    template<typename, size_t _size = DynamicData>
class VectorDerived, typename T, size_t _size>
auto VectorBase<VectorDerived<T, _size>>::operator-() const -> Vector<T, _size>
{
    static_assert(std::is_signed<T>::value, "Requires signed type");

    using Scalar = std::remove_cv_t<T>;

    Vector<Scalar, _size> result = this->derived();
    size_t i = 0;

#ifdef TL_HAVE_SIMD_INTRINSICS
    Packed<Scalar> packed_value;
    constexpr size_t packed_size = packed_value.size();
    size_t max_vector = (result.size() / packed_size) * packed_size;

    if (this->properties.isEnabled(VectorDerived<T, _size>::Properties::contiguous_memory)) {
        for (; i < max_vector; i += packed_size) {
            packed_value.loadUnaligned(&result[i]);
            packed_value = - packed_value;
            packed_value.storeUnaligned(&result[i]);
        }
    }
#endif

    // Procesa los elementos restantes de manera escalar
    for (; i < result.size(); ++i) {
        result[i] = -result[i];
    }

    return result;
}

/* Binary arithmetic operators */

template<
    template<typename, size_t _size = DynamicData>
class VectorDerived, typename T, size_t _size>
auto VectorBase<VectorDerived<T, _size>>::operator+(const VectorDerived<T, _size> &vector2) const -> Vector<T, _size>
{
    Vector<T, _size> vector = this->derived();
    vector += vector2;
    return vector;
}

template<
    template<typename, size_t _size = DynamicData>
class VectorDerived, typename T, size_t _size>
auto VectorBase<VectorDerived<T, _size>>::operator-(const VectorDerived<T, _size> &vector2) const -> Vector<T, _size>
{
    Vector<T, _size> vector = this->derived();
    vector -= vector2;
    return vector;
}

template<
    template<typename, size_t _size = DynamicData>
class VectorDerived, typename T, size_t _size>
auto VectorBase<VectorDerived<T, _size>>::operator*(const VectorDerived<T, _size> &vector2) const -> Vector<T, _size>
{
    Vector<T, _size> vector = this->derived();
    vector *= vector2;
    return vector;
}

template<
    template<typename, size_t _size = DynamicData>
class VectorDerived, typename T, size_t _size>
auto VectorBase<VectorDerived<T, _size>>::operator/(const VectorDerived<T, _size> &vector2) const -> Vector<T, _size>
{
    Vector<T, _size> vector = this->derived();
    vector /= vector2;
    return vector;
}

template<
    template<typename, size_t _size = DynamicData>
class VectorDerived, typename T, size_t _size>
auto VectorBase<VectorDerived<T, _size>>::operator*(T scalar) const -> Vector<T, _size>
{
    Vector<T, _size> vector = this->derived();
    vector *= scalar;
    return vector;
}

template<
    template<typename, size_t _size = DynamicData>
class VectorDerived, typename T, size_t _size>
auto VectorBase<VectorDerived<T, _size>>::operator/(T scalar) const -> Vector<T, _size>
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
auto VectorBase<VectorDerived<T, _size>>::operator-=(const VectorDerived2 &vector) -> VectorDerived<T, _size> &
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
auto VectorBase<VectorDerived<T, _size>>::operator*=(const VectorDerived2 &vector) -> VectorDerived<T, _size> &
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
auto VectorBase<VectorDerived<T, _size>>::operator/=(const VectorDerived2 &vector) -> VectorDerived<T, _size> &
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
template<typename VectorDerived2>
void VectorBase<VectorDerived<T, _size>>::set(const VectorDerived2 &vector)
{
    auto &derived = this->derived();

    if(_size == DynamicData) {
        derived = VectorDerived<T, _size>(vector.size());
    }

    TL_ASSERT(derived.size() == vector.size(), "Static vector cannot be resized");

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
            packed_a = packed_b;
            packed_a.storeUnaligned(&derived[i]);

        }
    }

#endif

    for (; i < derived.size(); ++i) {
        derived[i] = vector[i];
    }
}

template<
    template<typename, size_t _size = DynamicData>
class VectorDerived, typename T, size_t _size>
auto VectorBase<VectorDerived<T, _size>>::operator*=(T scalar) -> VectorDerived<T, _size> &
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
auto VectorBase<VectorDerived<T, _size>>::operator/=(T scalar) -> VectorDerived<T, _size> &
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
auto VectorBase<VectorDerived<T, _size>>::derived() -> VectorDerived<T, _size> &
{
    return *static_cast<VectorDerived<T, _size> *>(this);
}

template<
    template<typename, size_t _size = DynamicData>
class VectorDerived, typename T, size_t _size>
auto VectorBase<VectorDerived<T, _size>>::derived() const -> const VectorDerived<T, _size> &
{
    return *static_cast<const VectorDerived<T, _size> *>(this);
}




/* Vector implementation */

template<typename T, size_t _size>
Vector<T, _size>::Vector()
  : _data(Data<T, _size>())
{
}

template<typename T, size_t _size>
Vector<T, _size>::Vector(size_t size, T value)
  : _data(Data<T, _size>(size, value))
{
}

template<typename T, size_t _size>
Vector<T, _size>::Vector(const Vector &vector)
  : _data(vector._data)
{
}

template<typename T, size_t _size>
Vector<T, _size>::Vector(Vector &&vector) TL_NOEXCEPT
  : _data(std::move(vector._data))
{
}

template<typename T, size_t _size>
Vector<T, _size>::Vector(std::initializer_list<T> values)
  : _data(Data<T, _size>(values))
{
}

template<typename T, size_t _size>
Vector<T, _size>::Vector(T *data, size_t size)
    : _data(Data<T, _size>(data, size))
{
}

template<typename T, size_t _size>
auto Vector<T, _size>::operator=(const Vector &vector) -> Vector &
{
    if (this != &vector) {
        this->_data = vector._data;
    }

    return (*this);
}

template<typename T, size_t _size>
auto Vector<T, _size>::operator=(Vector &&vector) TL_NOEXCEPT -> Vector &
{
    if (this != &vector) {
        this->_data = std::forward<Data<T, _size>>(vector._data);
    }

    return (*this);
}

template<typename T, size_t _size>
template<typename VectorDerived>
auto Vector<T, _size>::operator=(const VectorDerived &vector) -> Vector &
{
    VectorBase<Vector<T, _size>>::set(vector);
    return (*this);
}

template<typename T, size_t _size>
void Vector<T, _size>::resize(size_t size)
{
    static_assert(_size == DynamicData, "Fixed-size vector not support resize");

    _data.resize(size);
}

template<typename T, size_t _size>
void Vector<T, _size>::resize(size_t size, T value)
{
    static_assert(_size == DynamicData, "Fixed-size vector not support resize");

    _data.resize(size, value);
}

template<typename T, size_t _size>
auto Vector<T, _size>::size() const TL_NOEXCEPT -> size_t
{
    return _data.size();
}

template<typename T, size_t _size>
auto Vector<T, _size>::at(size_type position) -> reference
{
    return _data.at(position);
}

template<typename T, size_t _size>
auto Vector<T, _size>::at(size_type position) const -> const_reference
{
    return _data.at(position);
}

template<typename T, size_t _size>
auto Vector<T, _size>::operator[](size_t position) TL_NOEXCEPT -> reference
{
    return _data[position];
}

template<typename T, size_t _size>
auto Vector<T, _size>::operator[](size_t position) const TL_NOEXCEPT -> const_reference
{
    return _data[position];
}

template<typename T, size_t _size>
auto Vector<T, _size>::front() TL_NOEXCEPT -> reference
{
    return _data.front();
}

template<typename T, size_t _size>
auto Vector<T, _size>::front() const TL_NOEXCEPT -> const_reference
{
    return _data.front();
}

template<typename T, size_t _size>
auto Vector<T, _size>::back() TL_NOEXCEPT -> reference
{
    return _data.back();
}

template<typename T, size_t _size>
auto Vector<T, _size>::back() const TL_NOEXCEPT -> const_reference
{
    return _data.back();
}

template<typename T, size_t _size>
auto Vector<T, _size>::begin() TL_NOEXCEPT -> iterator
{
    return _data.begin();
}

template<typename T, size_t _size>
auto Vector<T, _size>::begin() const TL_NOEXCEPT -> const_iterator
{
    return _data.begin();
}

template<typename T, size_t _size>
auto Vector<T, _size>::end() TL_NOEXCEPT -> iterator
{
    return _data.end();
}

template<typename T, size_t _size>
auto Vector<T, _size>::end() const TL_NOEXCEPT -> const_iterator
{
    return _data.end();
}

template<typename T, size_t _size>
auto Vector<T, _size>::data() TL_NOEXCEPT -> pointer
{
    return _data.data();
}

template<typename T, size_t _size>
auto Vector<T, _size>::data() const TL_NOEXCEPT -> const_pointer
{
    return _data.data();
}

template<typename T, size_t _size>
auto Vector<T, _size>::x() const TL_NOEXCEPT -> const_reference
{
    static_assert(_size != DynamicData, "Unsupported method for dynamic vectors");
    static_assert(_size >= 2 && _size <= 4, "Method only supported for 2, 3 or 4 element vectors");
    return _data[0];
}

template<typename T, size_t _size>
auto Vector<T, _size>::y() const TL_NOEXCEPT -> const_reference
{
    static_assert(_size != DynamicData, "Unsupported method for dynamic vectors");
    static_assert(_size >= 2 && _size <= 4, "Method only supported for 2, 3 or 4 element vectors");
    return _data[1];
}

template<typename T, size_t _size>
auto Vector<T, _size>::z() const TL_NOEXCEPT -> const_reference
{
    static_assert(_size != DynamicData, "Unsupported method for dynamic vectors");
    static_assert(_size >= 2 && _size <= 4, "Method only supported for 2, 3 or 4 element vectors");
    return _data[2];
}

template<typename T, size_t _size>
auto Vector<T, _size>::w() const TL_NOEXCEPT -> const_reference
{
    static_assert(_size != DynamicData, "Unsupported method for dynamic vectors");
    static_assert(_size >= 2 && _size <= 4, "Method only supported for 2, 3 or 4 element vectors");
    return _data[3];
}

template<typename T, size_t _size>
auto Vector<T, _size>::x() TL_NOEXCEPT -> reference
{
    static_assert(_size != DynamicData, "Unsupported method for dynamic vectors");
    static_assert(_size >= 2 && _size <= 4, "Method only supported for 2, 3 or 4 element vectors");
    return _data[0];
}

template<typename T, size_t _size>
auto Vector<T, _size>::y() TL_NOEXCEPT -> reference
{
    static_assert(_size != DynamicData, "Unsupported method for dynamic vectors");
    static_assert(_size >= 2 && _size <= 4, "Method only supported for 2, 3 or 4 element vectors");
    return _data[1];
}

template<typename T, size_t _size>
auto Vector<T, _size>::z() TL_NOEXCEPT -> reference
{
    static_assert(_size != DynamicData, "Unsupported method for dynamic vectors");
    static_assert(_size >= 2 && _size <= 4, "Method only supported for 2, 3 or 4 element vectors");
    return _data[2];
}

template<typename T, size_t _size>
auto Vector<T, _size>::w() TL_NOEXCEPT -> reference
{
    static_assert(_size != DynamicData, "Unsupported method for dynamic vectors");
    static_assert(_size >= 2 && _size <= 4, "Method only supported for 2, 3 or 4 element vectors");
    return _data[3];
}

template<typename T, size_t _size>
bool Vector<T, _size>::operator == (const Vector<T, _size> &vector) const
{
    return this->_data == vector._data;
}

template<typename T, size_t _size>
bool Vector<T, _size>::operator != (const Vector<T, _size> &vector) const
{
    return this->_data != vector._data;
}

template<typename T, size_t _size>
bool Vector<T, _size>::operator <  (const Vector<T, _size> &vector) const
{
    return this->_data < vector._data;
}

template<typename T, size_t _size>
bool Vector<T, _size>::operator <= (const Vector<T, _size> &vector) const
{
    return this->_data <= vector._data;
}

template<typename T, size_t _size>
bool Vector<T, _size>::operator >  (const Vector<T, _size> &vector) const
{
    return this->_data > vector._data;
}

template<typename T, size_t _size>
bool Vector<T, _size>::operator >= (const Vector<T, _size> &vector) const
{
    return this->_data >= vector._data;
}

template<typename T, size_t _size>
auto Vector<T, _size>::zero(size_t size) -> Vector
{
    return Vector<T, _size>(size, consts::zero<T>);
}

template<typename T, size_t _size>
auto Vector<T, _size>::unit(size_t size) -> Vector
{
    return Vector<T, _size>(size, consts::one<T>);
}

template<typename T, size_t _size>
auto Vector<T, _size>::randon(size_t size) -> Vector
{
    Vector<T, _size> vector(size);

    std::random_device rd;
    std::mt19937 random_number_engine(rd());
    std::uniform_real_distribution<> distribution(0.0, 99.0);

    for (size_t i = 0; i < vector.size(); i++) {
        vector[i] = static_cast<T>(distribution(random_number_engine));
    }

    return vector;
}





/* Binary arithmetic operators */

/* Addition */

template<typename T, size_t _size>
auto operator +(Vector<T, _size>& vector1,
                Vector<T, _size>&& vector2) -> Vector<T, _size>
{
    vector2 += vector1;
    return vector2;
}

template<typename T, size_t _size>
auto operator +(Vector<T, _size>&& vector1,
                const Vector<T, _size>& vector2) -> Vector<T, _size>
{
    vector1 += vector2;
    return vector1;
}

template<typename T, size_t _size>
auto operator +(Vector<T, _size>&& vector1,
                Vector<T, _size>&& vector2) -> Vector<T, _size>
{
    vector1 += vector2;
    return vector1;
}


/* Subtraction */

template<typename T, size_t _size>
auto operator -(Vector<T, _size>& vector1,
                Vector<T, _size>&& vector2) -> Vector<T, _size>
{
    vector2 -= vector1;
    return -vector2;
}

template<typename T, size_t _size>
auto operator -(Vector<T, _size>&& vector1,
                const Vector<T, _size>& vector2) -> Vector<T, _size>
{
    vector1 -= vector2;
    return vector1;
}

template<typename T, size_t _size>
auto operator -(Vector<T, _size>&& vector1,
                Vector<T, _size>&& vector2) -> Vector<T, _size>
{
    vector1 -= vector2;
    return vector1;
}


/* Multiplication */

template<typename T, size_t _size>
auto operator *(const Vector<T, _size>& vector1,
                Vector<T, _size>&& vector2) -> Vector<T, _size>
{
    vector2 *= vector1;
    return vector2;
}

template<typename T, size_t _size>
auto operator *(Vector<T, _size>&& vector1,
                const Vector<T, _size>& vector2) -> Vector<T, _size>
{
    vector1 *= vector2;
    return vector1;
}

template<typename T, size_t _size>
auto operator *(Vector<T, _size>&& vector1,
                Vector<T, _size>&& vector2) -> Vector<T, _size>
{
    vector1 *= vector2;
    return vector1;
}


/* Division */

template<typename T, size_t _size>
auto operator /(Vector<T, _size>&& vector1,
                const Vector<T, _size>& vector2) -> Vector<T, _size>
{
    vector1 /= vector2;
    return vector1;
}

template<typename T, size_t _size>
auto operator /(Vector<T, _size>&& vector1,
                Vector<T, _size>&& vector2) -> Vector<T, _size>
{
    vector1 /= vector2;
    return vector1;
}


/* Multiplication vector * scalar */

template<typename T, size_t _size>
auto operator *(Vector<T, _size>&& vector, T scalar) -> Vector<T, _size>
{
    vector *= scalar;
    return vector;
}

template<
  template<typename, size_t _size = DynamicData>
  class VectorDerived, typename T, size_t _size>
auto operator *(T scalar, const VectorDerived<T, _size>& vector) -> Vector<T, _size>
{
    Vector<T, _size> _vector = vector;
    _vector *= scalar;
    return _vector;
}

template<typename T, size_t _size>
auto operator *(T scalar, Vector<T, _size>&& vector) -> Vector<T, _size>
{
    vector *= scalar;
    return vector;
}


/* Division Vector / scalar */

template<typename T, size_t _size>
auto operator /(Vector<T, _size>&& vector, T scalar) -> Vector<T, _size>
{
    vector /= scalar;
    return vector;
}

//template<typename T, size_t _size>
//inline Vector<T, _size> operator / (const Vector<T, _size> &vector, T scalar)
//{
//    vector /= scalar;
//    return vector;
//}


/*!
 * \brief Computes the dot product of two vectors.
 *
 * The dot product of two vectors is the sum of the products of their corresponding components.
 * This function asserts that both vectors have the same size.
 *
 * \tparam T The type of the elements in the vectors.
 * \tparam _size The size of the vectors.
 * \param[in] v1 The first vector.
 * \param[in] v2 The second vector.
 * \return The dot product of `v1` and `v2` as a `double`.
 */
template<typename T, size_t _size> 
auto dotProduct(const Vector<T, _size>& v1,
                const Vector<T, _size>& v2) -> double
{
    TL_ASSERT(v1.size() == v2.size(), "Different vector size");

    double dot = static_cast<double>(v1[0]) * static_cast<double>(v2[0]);
    for (size_t i = 1; i < v1.size(); i++) {
        dot += static_cast<double>(v1[i]) * static_cast<double>(v2[i]);
    }
    return dot;
}

/*!
 * \brief Computes the cross product of two 3-dimensional vectors.
 *
 * The cross product of two vectors in three-dimensional space results in a vector that is
 * perpendicular to both of the vectors being multiplied, with a direction given by the right-hand rule.
 * This function assumes that the vectors have three components.
 *
 * \tparam T The type of the elements in the vectors.
 * \tparam _size The size of the vectors, defaulting to 3.
 * \param[in] pt1 The first vector.
 * \param[in] pt2 The second vector.
 * \return The cross product of `pt1` and `pt2` as a `Vector<T, _size>`.
 */
template<typename T, size_t _size = 3>
auto crossProduct(const Vector<T, _size>& pt1,
                  const Vector<T, _size>& pt2) -> Vector<T, _size>
{
    return Vector<T, _size>({
    	pt1.y() * pt2.z() - pt1.z() * pt2.y(),
        pt1.z() * pt2.x() - pt1.x() * pt2.z(),
        pt1.x() * pt2.y() - pt1.y() * pt2.x()
    });
}

template<typename T, size_t _size>
auto operator<<(std::ostream& os, const Vector<T, _size>& vector) -> std::ostream&
{
    for (size_t i = 0; i < vector.size(); i++) {
        os << " " << vector[i] << "\n";
    }
    os << std::flush;
    return os;
}

template<typename T, size_t _size>
auto operator<<(std::ostream& os, const Vector<T, _size>* vector) -> std::ostream&
{
    for (size_t i = 0; i < vector->size(); i++) {
        os << " " << (*vector)[i] << "\n";
    }
    os << std::flush;
    return os;
}


/*!
 * \brief Computes the angle between two 2-dimensional vectors.
 *
 * This function calculates the angle in radians between two vectors \f$v1\f$ and \f$v2\f$.
 * If the vectors are equal, the angle is 0. If either vector has a magnitude of zero, an exception is thrown.
 * The angle is computed using the dot product and the magnitudes of the vectors, ensuring that the cosine of the angle
 * is within the range \f$[-1, 1]\f$ to account for numerical precision errors.
 *
 * \tparam T The type of the elements in the vectors.
 * \param[in] v1 The first 2-dimensional vector.
 * \param[in] v2 The second 2-dimensional vector.
 * \return The angle between `v1` and `v2` in radians.
 * \throws std::invalid_argument if either vector has a magnitude of zero.
 */
template<typename T>
T vectorAngle(const Vector<T, 2> &v1, const Vector<T, 2> &v2) 
{
    if (v1 == v2) return 0.;

    T magV1 = v1.module();
    T magV2 = v2.module();
    
    if (magV1 == 0 || magV2 == 0) {
        throw std::invalid_argument("The angle is not defined for zero magnitude vectors.");
    }
    
    T cosTheta = dotProduct(v1, v2) / (magV1 * magV2);
    // Asegurarse de que el valor esté en el rango [-1, 1] debido a posibles errores numéricos
    cosTheta = tl::clamp(cosTheta, static_cast<T>(-1), static_cast<T>(1));
    
    return std::acos(cosTheta);
}


/*! \} */

} // End namespace tl

