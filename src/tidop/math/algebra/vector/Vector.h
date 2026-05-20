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

/*! \file Vector.h
 * \brief Mathematical vector implementation supporting static and dynamic sizes.
 *
 * This file defines the `Vector` class template, which provides a flexible and efficient
 * implementation of mathematical vectors. It supports both compile-time fixed-size vectors
 * (using a template parameter `Size`) and runtime dynamic-size vectors (when `Size = DynamicData`).
 * The class inherits from `VectorBase` and provides a wide range of vector operations,
 * including element access, iterators, and conversions.
 *
 * \ingroup Vector
 * \see tl::VectorBase, tl::Data
 */

#pragma once

#include <vector>
#include <random>
#include <iomanip>

#include "tidop/core/base/Exception.h"
#include "tidop/core/base/TypeConversions.h"
#include "tidop/math/math.h"
#include "tidop/math/base/Data.h"
#include "tidop/math/algebra/vector/VectorBase.h"
#include "tidop/math/algebra/vector/detail/MatVecMul.h"
#include "tidop/math/algebra/vector/detail/Assign.h"
#include "tidop/math/base/Simd.h"
#include "tidop/math/base/Concepts.h"
#include "tidop/math/base/Traits.h"

namespace tl
{

/*! \addtogroup Vector
 *  \{
 */


/*!
 * \class Vector
 * \brief Represents a mathematical vector, supporting both static and dynamic sizes.
 *
 * \tparam T The type of elements in the vector.
 * \tparam Size The size of the vector. If not specified, defaults to `DynamicData`, making it a dynamic vector.
 *
 * The `Vector` class provides a robust implementation for vectors of various sizes, including
 * operations for accessing, modifying, and manipulating vector data. It inherits from `VectorBase`
 * and supports both static and dynamic sizing depending on the template parameter `Size`.
 *
 * ### Example
 * \code
 * // Static 3D vector of doubles
 * Vector<double, 3> v1 = {1.0, 2.0, 3.0};
 *
 * // Dynamic vector of integers
 * Vector<int> v2(5, 10);   // size 5, all elements 10
 *
 * // Access and modify
 * v1.x() = 5.0;
 * double norm = v1.norm();
 * \endcode
 *
 * \note The vector elements are stored in a contiguous memory block, and the class provides
 *       STL-compatible iterators for easy integration with standard algorithms.
 */
template<typename T, size_t Size = DynamicData>
class Vector
  : public VectorBase<Vector<T, Size>>
{

    static_assert(Arithmetic<T>, "Vector requires an arithmetic type (integral or floating-point)");
    static_assert(!std::is_const_v<T>, "Vector cannot hold const types. Use 'const Vector<T>' instead.");

public:

    using value_type = T;
    using size_type = size_t;
    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;

    using iterator = typename Data<T, Size>::iterator;
    using const_iterator = typename Data<T, Size>::const_iterator;

public:

    /*!
     * \brief Default constructor. Initializes an empty vector.
     */
    Vector();

    /*!
     * \brief Constructs a vector of a given size, initializing all elements to a specified value.
     * \param[in] size The size of the vector (ignored for static vectors, must match if provided).
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
    Vector(Vector &&vector) noexcept;

    /*!
     * \brief Constructs a vector from an initializer list.
     * \param[in] values The values to initialize the vector with.
     * \note The number of values must match the vector's size (static) or determines the size (dynamic).
     */
    Vector(std::initializer_list<T> values);

    /*!
     * \brief Constructs a vector from a raw pointer and size.
     * \param[in] data Pointer to the data.
     * \param[in] size The size of the vector.
     */
    Vector(T *data, size_t size);

    /*!
     * \brief Constructs a vector from a vector expression.
     * \tparam Expr A type satisfying the VectorExpr concept.
     * \param[in] expr The expression to evaluate.
     * 
     * ### Example
     * \code
     * Vector<double, 3> v1 = {1.0, 2.0, 3.0};
     * Vector<double, 3> v2 = {4.0, 5.0, 6.0};
     * 
     * // The expression v1 + v2 is evaluated in the constructor
     * Vector<double, 3> v = v1 + v2;
     * \endcode
     */
    template<VectorExpr Expr>
    Vector(const Expr &expr);

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
    auto operator=(Vector &&vector) noexcept -> Vector &;

    /*!
     * \brief Assignment operator for a vector expression.
     * \tparam Expr A type satisfying the VectorExpr concept.
     * \param[in] expr The expression to evaluate and assign.
     * \return Reference to this vector.
     *
     * ### Example
     * \code
     * Vector<double, 3> v1 = {1.0, 2.0, 3.0};
     * Vector<double, 3> v2 = {4.0, 5.0, 6.0};
     *
     * // The expression v1 + v2 is evaluated in the constructor
     * Vector<double, 3> v;
     * v = v1 + v2;
     * \endcode
     */
    template<VectorExpr Expr>
    auto operator=(const Expr &expr) -> Vector &;

    /*!
     * \brief Resizes the vector to a new size.
     * \param[in] size The new size of the vector.
     * \note Only valid for dynamic vectors. For static vectors, a compile-time error occurs.
     */
    void resize(size_t size);

    /*!
     * \brief Resizes the vector to a new size and initializes new elements to a specified value.
     * \param[in] size The new size of the vector.
     * \param[in] value The value to initialize new elements with.
     * \note Only valid for dynamic vectors. For static vectors, a compile-time error occurs.
     */
    void resize(size_t size, T value);

    /*!
     * \brief Returns the size of the vector.
     * \return The number of elements in the vector (compile-time constant for static vectors).
     */
    constexpr auto size() const noexcept -> size_t;

    /*!
     * \brief Accesses the element at the specified position with bounds checking.
     *
     * \param[in] position The position of the element to access.
     * \return A reference to the element at the specified position.
     * \throws std::out_of_range if the position is out of bounds.
     */
    auto at(size_t position) -> reference;

    /*!
     * \brief Accesses the element at the specified position with bounds checking (const version).
     *
     * \param[in] position The position of the element to access.
     * \return A const reference to the element at the specified position.
     * \throws std::out_of_range if the position is out of bounds.
     */
    auto at(size_t position) const -> const_reference;

    /*!
     * \brief Accesses the element at the specified position with bounds checking.
     *
     * \param[in] position The position of the element to access.
     * \return A reference to the element at the specified position.
     * \throws std::out_of_range if the position is out of bounds.
     */
    auto operator()(size_t position) -> reference;

    /*!
     * \brief Accesses the element at the specified position with bounds checking (const version).
     *
     * \param[in] position The position of the element to access.
     * \return A const reference to the element at the specified position.
     * \throws std::out_of_range if the position is out of bounds.
     */
    auto operator()(size_t position) const -> const_reference;

    /*!
     * \brief Accesses the element at the specified position without bounds checking.
     *
     * \param[in] position The position of the element to access.
     * \return A reference to the element at the specified position.
     * \note No bounds checking is performed, so accessing an invalid position
     *       may result in undefined behavior.
     */
    auto operator[](size_t position) noexcept -> reference;

    /*!
     * \brief Accesses the element at the specified position without bounds checking (const version).
     *
     * \param position The position of the element to access.
     * \return A const reference to the element at the specified position.
     * \note No bounds checking is performed, so accessing an invalid position
     *       may result in undefined behavior.
     */
    auto operator[](size_t position) const noexcept -> const_reference;

    /*!
     * \brief Access the first element in the vector.
     * \return A reference to the first element.
     */
    auto front() noexcept -> reference;

    /*!
     * \brief Access the first element in the vector (const version).
     * \return A const reference to the first element.
     */
    auto front() const noexcept -> const_reference;

    /*!
     * \brief Access the last element in the vector.
     * \return A reference to the last element.
     */
    auto back() noexcept -> reference;

    /*!
     * \brief Access the last element in the vector (const version).
     * \return A const reference to the last element.
     * \throws std::out_of_range if the vector is empty.
     */
    auto back() const noexcept -> const_reference;

    /*!
     * \brief Returns an iterator to the beginning of the vector.
     * \return An iterator to the beginning of the vector.
     */
    auto begin() noexcept -> iterator;

    /*!
     * \brief Returns a const iterator to the beginning of the vector.
     * \return A const iterator to the beginning of the vector.
     */
    auto begin() const noexcept -> const_iterator;

    /*!
     * \brief Returns an iterator to the end of the vector.
     * \return An iterator to the end of the vector.
     */
    auto end() noexcept -> iterator;

    /*!
     * \brief Returns a const iterator to the end of the vector.
     * \return A const iterator to the end of the vector.
     */
    auto end() const noexcept -> const_iterator;

    /*!
     * \brief Returns a pointer to the data array of the vector.
     * \return A pointer to the data array.
     */
    auto data() noexcept -> pointer;

    /*!
     * \brief Returns a const pointer to the data array of the vector.
     * \return A const pointer to the data array.
     */
    auto data() const noexcept -> const_pointer;

    /*!
     * \brief Access the x-component of the vector.
     * \return A const reference to the x-component.
     * \note Only valid for vectors with size at least 1.
     */
    auto x() const noexcept -> const_reference;

    /*!
     * \brief Access the y-component of the vector.
     * \return A const reference to the y-component.
     * \note Only valid for vectors with size at least 2.
     */
    auto y() const noexcept -> const_reference;

    /*!
     * \brief Access the z-component of the vector.
     * \return A const reference to the z-component.
     * \note Only valid for vectors with size at least 3.
     */
    auto z() const noexcept -> const_reference;

    /*!
     * \brief Access the w-component of the vector.
     * \return A const reference to the w-component.
     * \note Only valid for vectors with size at least 4.
     */
    auto w() const noexcept -> const_reference;

    /*!
     * \brief Access the x-component of the vector (non-const version).
     * \return A reference to the x-component.
     * \note Only valid for vectors with size at least 1.
     */
    auto x() noexcept -> reference;

    /*!
     * \brief Access the y-component of the vector (non-const version).
     * \return A reference to the y-component.
     * \note Only valid for vectors with size at least 2.
     */
    auto y() noexcept -> reference;

    /*!
     * \brief Access the z-component of the vector (non-const version).
     * \return A reference to the z-component.
     * \note Only valid for vectors with size at least 3.
     */
    auto z() noexcept-> reference;

    /*!
     * \brief Access the w-component of the vector (non-const version).
     * \return A reference to the w-component.
     * \note Only valid for vectors with size at least 4.
     */
    auto w() noexcept -> reference;

    /*!
     * \brief Less-than operator for comparing two vectors.
     * \param vector The vector to compare with.
     * \return True if this vector is less than the other, false otherwise.
     */
    bool operator < (const Vector &vector) const;

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
    bool operator > (const Vector &vector) const;

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
    TL_DEPRECATED("zeros(size_t size = 0)", "4.0")
    static auto zero(size_t size = 0) -> Vector;
    static auto zeros(size_t size = 0) -> Vector;

    /*!
     * \brief Creates a unit vector of a specified size.
     * \param size The size of the vector.
     * \return A unit vector.
     */
    TL_DEPRECATED("ones(size_t size = 0)", "4.0")
    static auto unit(size_t size = 0) -> Vector;
    static auto ones(size_t size = 0) -> Vector;

    /*!
     * \brief Creates a random vector of a specified size.
     * \param size The size of the vector.
     * \return A vector with randomly initialized elements.
     */
    static auto randon(size_t size = 0) -> Vector;

    /*!
     * \brief Convert to dynamic vector.
     * Only works if this vector is static.
     * \note This operator is only available when the current vector is static
     */
    explicit operator Vector<T, DynamicData>() const
    {
        static_assert(Size != DynamicData,
            "Cannot convert dynamic vector to dynamic vector. "
            "It's already dynamic.");

        Vector<T, DynamicData> result(Size);
        for (size_t i = 0; i < Size; ++i) {
            result[i] = (*this)[i];
        }
        return result;
    }

    /*!
     * \brief Convert to a static vector of a given size.
     * \tparam TargetSize The target compile-time size.
     * \return A static vector of the specified size.
     * \throws std::invalid_argument if this is a dynamic vector and its size does not match `TargetSize`.
     * \note For static-to-static conversion, the sizes must match at compile time.
     *       For dynamic-to-static, a runtime check is performed.
     */
    template<size_t TargetSize>
    explicit operator Vector<T, TargetSize>() const
    {
        if constexpr (Size != DynamicData) {

            static_assert(Size == TargetSize,
                "Cannot convert static vector to different static size. "
                "Use toDynamic() first.");

            Vector<T, TargetSize> result;
            for (size_t i = 0; i < TargetSize; ++i) {
                result[i] = (*this)[i];
            }

            return result;

        } else {

            static_assert(TargetSize != DynamicData,
                "Cannot convert dynamic vector to dynamic vector.");

            if (this->size() != TargetSize) {
                throw std::invalid_argument("Size mismatch");
            }

            Vector<T, TargetSize> result;
            for (size_t i = 0; i < TargetSize; ++i) {
                result[i] = (*this)[i];
            }

            return result;
        }
    }

    /*!
     * \brief Checks if the vector's data aliases a given memory address.
     * \param[in] ptr Pointer to the memory address.
     * \return True if the vector's internal data is stored at that address; false otherwise.
     */
    auto aliases(const void *ptr) const -> bool;

#ifdef TL_HAVE_SIMD_INTRINSICS
    /*!
     * \brief Returns a SIMD-friendly packet of elements starting at index `i`.
     * \param[in] i Starting index.
     * \return A Packed<T> object containing the elements.
     * \note This method is used internally for vectorization, but is publicly available.
     */
    auto packet(size_t i) const noexcept -> Packed<T>;
#endif

    /*!
     * \brief Fills the entire vector with a given value.
     * \param[in] value The value to set all elements to.
     */
    void fill(T value);

private:

    Data<T, Size> mData;

};



/* Vector alias */

using Vector2i = Vector<int, 2>;
using Vector2d = Vector<double, 2>;
using Vector2f = Vector<float, 2>;
using Vector3i = Vector<int, 3>;
using Vector3d = Vector<double, 3>;
using Vector3f = Vector<float, 3>;
using Vector4i = Vector<int, 4>;
using Vector4d = Vector<double, 4>;
using Vector4f = Vector<float, 4>;




/* Vector implementation */

template<typename T, size_t Size>
Vector<T, Size>::Vector()
  : mData(Data<T, Size>())
{
}

template<typename T, size_t Size>
Vector<T, Size>::Vector(size_t size, T value)
  : mData(Data<T, Size>(size, value))
{
}

template<typename T, size_t Size>
Vector<T, Size>::Vector(const Vector &vector)
  : mData(vector.mData)
{
}

template<typename T, size_t Size>
Vector<T, Size>::Vector(Vector &&vector) noexcept
  : mData(std::move(vector.mData))
{
}

template<typename T, size_t Size>
Vector<T, Size>::Vector(std::initializer_list<T> values)
  : mData(Data<T, Size>(values))
{
}

template<typename T, size_t Size>
Vector<T, Size>::Vector(T *data, size_t size)
    : mData(Data<T, Size>(data, size))
{
}

template<typename T, size_t Size>
template<VectorExpr Expr>
Vector<T, Size>::Vector(const Expr &expr)
  : mData(Data<T, Size>(expr.size()))
{
    TL_ASSERT(expr.size() == this->mData.size(), "Static vector cannot be resized");

    detail::assign(*this, expr);
}

template<typename T, size_t Size>
auto Vector<T, Size>::operator=(const Vector &vector) -> Vector &
{
    if (this != &vector) {
        this->mData = vector.mData;
    }

    return (*this);
}

template<typename T, size_t Size>
auto Vector<T, Size>::operator=(Vector &&vector) noexcept -> Vector &
{
    if (this != &vector) {
        this->mData = std::forward<Data<T, Size>>(vector.mData);
    }

    return (*this);
}

template<typename T, size_t Size>
template<VectorExpr Expr>
auto Vector<T, Size>::operator=(const Expr &expr) -> Vector &
{
    return detail::assign(*this, expr);
}

template<typename T, size_t Size>
void Vector<T, Size>::resize(size_t size)
{
    static_assert(Size == DynamicData, "Fixed-size vector not support resize");

    mData.resize(size);
}

template<typename T, size_t Size>
void Vector<T, Size>::resize(size_t size, T value)
{
    static_assert(Size == DynamicData, "Fixed-size vector not support resize");

    mData.resize(size, value);
}

template<typename T, size_t Size>
constexpr auto Vector<T, Size>::size() const noexcept -> size_t
{
    return mData.size();
}

template<typename T, size_t Size>
auto Vector<T, Size>::at(size_type position) -> reference
{
    return mData.at(position);
}

template<typename T, size_t Size>
auto Vector<T, Size>::at(size_type position) const -> const_reference
{
    return mData.at(position);
}

template<typename T, size_t Size>
auto Vector<T, Size>::operator()(size_type position) -> reference
{
    return mData.at(position);
}

template<typename T, size_t Size>
auto Vector<T, Size>::operator()(size_type position) const -> const_reference
{
    return mData.at(position);
}

template<typename T, size_t Size>
auto Vector<T, Size>::operator[](size_t position) noexcept -> reference
{
    return mData[position];
}

template<typename T, size_t Size>
auto Vector<T, Size>::operator[](size_t position) const noexcept -> const_reference
{
    return mData[position];
}

template<typename T, size_t Size>
auto Vector<T, Size>::front() noexcept -> reference
{
    return mData.front();
}

template<typename T, size_t Size>
auto Vector<T, Size>::front() const noexcept -> const_reference
{
    return mData.front();
}

template<typename T, size_t Size>
auto Vector<T, Size>::back() noexcept -> reference
{
    return mData.back();
}

template<typename T, size_t Size>
auto Vector<T, Size>::back() const noexcept -> const_reference
{
    return mData.back();
}

template<typename T, size_t Size>
auto Vector<T, Size>::begin() noexcept -> iterator
{
    return mData.begin();
}

template<typename T, size_t Size>
auto Vector<T, Size>::begin() const noexcept -> const_iterator
{
    return mData.begin();
}

template<typename T, size_t Size>
auto Vector<T, Size>::end() noexcept -> iterator
{
    return mData.end();
}

template<typename T, size_t Size>
auto Vector<T, Size>::end() const noexcept -> const_iterator
{
    return mData.end();
}

template<typename T, size_t Size>
auto Vector<T, Size>::data() noexcept -> pointer
{
    return mData.data();
}

template<typename T, size_t Size>
auto Vector<T, Size>::data() const noexcept -> const_pointer
{
    return mData.data();
}

template<typename T, size_t Size>
auto Vector<T, Size>::x() const noexcept -> const_reference
{
    static_assert(Size != DynamicData, "Unsupported method for dynamic vectors");
    static_assert(Size >= 2 && Size <= 4, "Method only supported for 2, 3 or 4 element vectors");
    return mData[0];
}

template<typename T, size_t Size>
auto Vector<T, Size>::y() const noexcept -> const_reference
{
    static_assert(Size != DynamicData, "Unsupported method for dynamic vectors");
    static_assert(Size >= 2 && Size <= 4, "Method only supported for 2, 3 or 4 element vectors");
    return mData[1];
}

template<typename T, size_t Size>
auto Vector<T, Size>::z() const noexcept -> const_reference
{
    static_assert(Size != DynamicData, "Unsupported method for dynamic vectors");
    static_assert(Size > 2 && Size <= 4, "Method only supported for 3 or 4 element vectors");
    return mData[2];
}

template<typename T, size_t Size>
auto Vector<T, Size>::w() const noexcept -> const_reference
{
    static_assert(Size != DynamicData, "Unsupported method for dynamic vectors");
    static_assert(Size == 4, "Method only supported for 4 element vectors");
    return mData[3];
}

template<typename T, size_t Size>
auto Vector<T, Size>::x() noexcept -> reference
{
    static_assert(Size != DynamicData, "Unsupported method for dynamic vectors");
    static_assert(Size >= 2 && Size <= 4, "Method only supported for 2, 3 or 4 element vectors");
    return mData[0];
}

template<typename T, size_t Size>
auto Vector<T, Size>::y() noexcept -> reference
{
    static_assert(Size != DynamicData, "Unsupported method for dynamic vectors");
    static_assert(Size >= 2 && Size <= 4, "Method only supported for 2, 3 or 4 element vectors");
    return mData[1];
}

template<typename T, size_t Size>
auto Vector<T, Size>::z() noexcept -> reference
{
    static_assert(Size != DynamicData, "Unsupported method for dynamic vectors");
    static_assert(Size > 2 && Size <= 4, "Method only supported for 3 or 4 element vectors");
    return mData[2];
}

template<typename T, size_t Size>
auto Vector<T, Size>::w() noexcept -> reference
{
    static_assert(Size != DynamicData, "Unsupported method for dynamic vectors");
    static_assert(Size == 4, "Method only supported for 4 element vectors");
    return mData[3];
}

template<typename T, size_t Size>
bool Vector<T, Size>::operator <  (const Vector<T, Size> &vector) const
{
    return this->mData < vector.mData;
}

template<typename T, size_t Size>
bool Vector<T, Size>::operator <= (const Vector<T, Size> &vector) const
{
    return this->mData <= vector.mData;
}

template<typename T, size_t Size>
bool Vector<T, Size>::operator >  (const Vector<T, Size> &vector) const
{
    return this->mData > vector.mData;
}

template<typename T, size_t Size>
bool Vector<T, Size>::operator >= (const Vector<T, Size> &vector) const
{
    return this->mData >= vector.mData;
}

template<typename T, size_t Size>
auto Vector<T, Size>::zero(size_t size) -> Vector
{
    return Vector<T, Size>(size, consts::zero<T>);
}

template<typename T, size_t Size>
auto Vector<T, Size>::zeros(size_t size) -> Vector
{
    return Vector<T, Size>(size, consts::zero<T>);
}

template<typename T, size_t Size>
auto Vector<T, Size>::unit(size_t size) -> Vector
{
    return Vector<T, Size>(size, consts::one<T>);
}

template<typename T, size_t Size>
auto Vector<T, Size>::ones(size_t size) -> Vector
{
    return Vector<T, Size>(size, consts::one<T>);
}

template<typename T, size_t Size>
auto Vector<T, Size>::randon(size_t size) -> Vector
{
    Vector<T, Size> vector(size);

    static thread_local std::mt19937 random_number_engine(std::random_device{}());
    std::uniform_real_distribution<> distribution(0.0, 99.0);

    for (size_t i = 0; i < vector.size(); i++) {
        vector[i] = static_cast<T>(distribution(random_number_engine));
    }

    return vector;
}

template<typename T, size_t Size>
auto Vector<T, Size>::aliases(const void *ptr) const -> bool
{
    return static_cast<const void *>(this->data()) == ptr;
}

#ifdef TL_HAVE_SIMD_INTRINSICS
template<typename T, size_t Size>
auto Vector<T, Size>::packet(size_t i) const noexcept -> Packed<T>
{
    Packed<T> p;
    p.loadUnaligned(&this->data()[i]);
    return p;
}
#endif 

template<typename T, size_t Size>
void Vector<T, Size>::fill(T value)
{
    std::fill(mData.begin(), mData.end(), value);
}




/* Scalar-vector multiplication */

template<typename Scalar, VectorExpr Expr>
    requires std::is_convertible_v<Scalar, typename vector_traits<Expr>::value_type>
auto operator*(const Scalar &scalar, const Expr &expr)
{
    return expr * scalar;
}


/*! \} */


} // End namespace tl

