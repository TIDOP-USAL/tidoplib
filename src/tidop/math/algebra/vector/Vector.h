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
#include "tidop/core/base/type_conversions.h"
#include "tidop/math/math.h"
#include "tidop/math/base/data.h"
#include "tidop/math/algebra/vector/VectorBase.h"
#include "tidop/math/algebra/vector/detail/MatVecMul.h"
#include "tidop/math/algebra/vector/detail/Assing.h"
#include "tidop/math/base/simd.h"
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
    Vector(Vector &&vector) noexcept;

    /*!
     * \brief Constructs a vector from an initializer list.
     * \param[in] values The values to initialize the vector with.
     */
    Vector(std::initializer_list<T> values);
    //template<typename... Args, std::enable_if_t<sizeof...(Args) == Size, int> = 0>
    //explicit Vector(Args... args)
    //    : mData{static_cast<T>(args)...}
    //{
    //}

    /*!
     * \brief Constructs a vector from a raw pointer and size.
     * \param[in] data Pointer to the data.
     * \param[in] size The size of the vector.
     */
    Vector(T *data, size_t size);

    //template<typename OtherDerived>
    //Vector(const VectorBase<OtherDerived> &other);
    template<VectorExpr Expr>
    Vector(const Expr& expr) 
        : mData(Data<T, Size>(expr.size()))
    {
        TL_ASSERT(expr.size() == this->mData.size(), "Static vector cannot be resized");

        //*this = expr;
        detail::assign(*this, expr);
    }
	
    //~Vector() = default;

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
     * \brief Assignment operator for assigning a derived vector.
     * \tparam VectorDerived The type of the derived vector.
     * \param vector The derived vector to assign from.
     * \return A reference to this vector.
     */
    //template<typename OtherDerived>
    //auto operator=(const VectorBase<OtherDerived> &vector) -> Vector &;
    template<VectorExpr Expr>
    auto operator=(const Expr &expr) -> Vector&
    {
        return detail::assign(*this, expr);
    }

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
     * \brief Equality operator for comparing two vectors.
     * \param vector The vector to compare with.
     * \return True if the vectors are equal, false otherwise.
     */
    //bool operator == (const Vector &vector) const;

    /*!
     * \brief Inequality operator for comparing two vectors.
     * \param vector The vector to compare with.
     * \return True if the vectors are not equal, false otherwise.
     */
    //bool operator != (const Vector &vector) const;

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

    /*!
     * \brief Convert to dynamic vector.
     * Only works if this vector is static.
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
     * \brief Convert to static vector.
     * For static vectors: same size only (compile-time check).
     * For dynamic vectors: runtime size check.
     */
    template<size_t TargetSize>
    explicit operator Vector<T, TargetSize>() const
    {
        if constexpr (Size != DynamicData) {
            // Este vector es estático
            static_assert(Size == TargetSize,
                "Cannot convert static vector to different static size. "
                "Use toDynamic() first.");

            Vector<T, TargetSize> result;
            for (size_t i = 0; i < TargetSize; ++i) {
                result[i] = (*this)[i];
            }
            return result;
        } else {
            // Este vector es dinámico
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

    auto aliases(const void *ptr) const -> bool
    {
        return static_cast<const void *>(this->data()) == ptr;
    }

    auto packet(size_t i) const noexcept -> Packed<T>
    {
        Packed<T> p;
        p.loadUnaligned(&this->data()[i]);
        return p;
    }

    void fill(T value)
    {
        std::fill(mData.begin(), mData.end(), value);
    }

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

//template<typename T, size_t Size>
//template<typename OtherDerived>
//Vector<T, Size>::Vector(const VectorBase<OtherDerived> &other)
//{
//    this->set(other.derived());
//}

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

//template<typename T, size_t Size>
//template<typename OtherDerived>
//auto Vector<T, Size>::operator=(const VectorBase<OtherDerived> &vector) -> Vector &
//{
//    this->set(vector.derived());
//    return (*this);
//}

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

//template<typename T, size_t Size>
//bool Vector<T, Size>::operator == (const Vector<T, Size> &vector) const
//{
//    return this->mData == vector.mData;
//}
//
//template<typename T, size_t Size>
//bool Vector<T, Size>::operator != (const Vector<T, Size> &vector) const
//{
//    return this->mData != vector.mData;
//}

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
auto Vector<T, Size>::unit(size_t size) -> Vector
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





/* Binary arithmetic operators */

/* Addition */

//template<typename PointDerived, typename VectorDerived,
//    typename = std::enable_if_t<
//    is_point<PointDerived>::value &&
//    is_vector<VectorDerived>::value>>
//auto operator+(const PointDerived &p, const VectorDerived &v) -> PointDerived
//{
//    PointDerived res = p;
//    constexpr std::size_t N = vector_traits<PointDerived>::size;
//
//    for (std::size_t i = 0; i < N; ++i)
//        res[i] += v[i];
//
//    return res;
//}
//
//template<typename T, size_t Size>
//auto operator +(Vector<T, Size>& vector1,
//                Vector<T, Size>&& vector2) -> Vector<T, Size>
//{
//    vector2 += vector1;
//    return vector2;
//}
//
//template<typename T, size_t Size>
//auto operator +(Vector<T, Size>&& vector1,
//                const Vector<T, Size>& vector2) -> Vector<T, Size>
//{
//    vector1 += vector2;
//    return vector1;
//}
//
//template<typename T, size_t Size>
//auto operator +(Vector<T, Size>&& vector1,
//                Vector<T, Size>&& vector2) -> Vector<T, Size>
//{
//    vector1 += vector2;
//    return vector1;
//}


/* Subtraction */

//template<typename T, size_t Size>
//auto operator -(Vector<T, Size>& vector1,
//                Vector<T, Size>&& vector2) -> Vector<T, Size>
//{
//    vector2 -= vector1;
//    return -vector2;
//}
//
//template<typename T, size_t Size>
//auto operator -(Vector<T, Size>&& vector1,
//                const Vector<T, Size>& vector2) -> Vector<T, Size>
//{
//    vector1 -= vector2;
//    return vector1;
//}
//
//template<typename T, size_t Size>
//auto operator -(Vector<T, Size>&& vector1,
//                Vector<T, Size>&& vector2) -> Vector<T, Size>
//{
//    vector1 -= vector2;
//    return vector1;
//}


/* Multiplication */
//
//template<typename T, size_t Size>
//auto operator *(const Vector<T, Size>& vector1,
//                Vector<T, Size>&& vector2) -> Vector<T, Size>
//{
//    vector2 *= vector1;
//    return vector2;
//}
//
//template<typename T, size_t Size>
//auto operator *(Vector<T, Size>&& vector1,
//                const Vector<T, Size>& vector2) -> Vector<T, Size>
//{
//    vector1 *= vector2;
//    return vector1;
//}
//
//template<typename T, size_t Size>
//auto operator *(Vector<T, Size>&& vector1,
//                Vector<T, Size>&& vector2) -> Vector<T, Size>
//{
//    vector1 *= vector2;
//    return vector1;
//}


/* Division */

//template<typename T, size_t Size>
//auto operator /(Vector<T, Size>&& vector1,
//                const Vector<T, Size>& vector2) -> Vector<T, Size>
//{
//    vector1 /= vector2;
//    return vector1;
//}
//
//template<typename T, size_t Size>
//auto operator /(Vector<T, Size>&& vector1,
//                Vector<T, Size>&& vector2) -> Vector<T, Size>
//{
//    vector1 /= vector2;
//    return vector1;
//}


/* Multiplication vector * scalar */

template<typename Scalar, VectorExpr Expr>
    requires std::is_convertible_v<Scalar, typename vector_traits<Expr>::value_type>
auto operator*(const Scalar &scalar, const Expr &expr)
{
    return VecMulScalarExpr(expr, scalar);
}



//template<typename T, size_t Size>
//auto operator *(Vector<T, Size>&& vector, T scalar) -> Vector<T, Size>
//{
//    vector *= scalar;
//    return vector;
//}
//
//template<
//  template<typename, size_t Size = DynamicData>
//  class VectorDerived, typename T, size_t Size>
//auto operator *(T scalar, const VectorDerived<T, Size> &vector) -> Vector<T, Size>
//{
//    Vector<T, Size> _vector = vector;
//    _vector *= scalar;
//    return _vector;
//}
//
//template<typename T, size_t Size>
//auto operator *(T scalar, Vector<T, Size> &&vector) -> Vector<T, Size>
//{
//    vector *= scalar;
//    return vector;
//}


/* Division Vector / scalar */

//template<typename T, size_t Size>
//auto operator /(Vector<T, Size>&& vector, T scalar) -> Vector<T, Size>
//{
//    vector /= scalar;
//    return vector;
//}

//template<typename T, size_t Size>
//inline Vector<T, Size> operator / (const Vector<T, Size> &vector, T scalar)
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
 * \tparam Size The size of the vectors.
 * \param[in] v1 The first vector.
 * \param[in] v2 The second vector.
 * \return The dot product of `v1` and `v2` as a `double`.
 */
//template<typename T, size_t Size> 
//auto dotProduct(const Vector<T, Size>& v1,
//                const Vector<T, Size>& v2) -> double
//{
//    //TL_ASSERT(v1.size() == v2.size(), "Different vector size");
//
//    //double dot = static_cast<double>(v1[0]) * static_cast<double>(v2[0]);
//    //for (size_t i = 1; i < v1.size(); i++) {
//    //    dot += static_cast<double>(v1[i]) * static_cast<double>(v2[i]);
//    //}
//    //return dot;
//    return v1.dotProduct(v2);
//}

/*!
 * \brief Computes the cross product of two 3-dimensional vectors.
 *
 * The cross product of two vectors in three-dimensional space results in a vector that is
 * perpendicular to both of the vectors being multiplied, with a direction given by the right-hand rule.
 * This function assumes that the vectors have three components.
 *
 * \tparam T The type of the elements in the vectors.
 * \tparam Size The size of the vectors, defaulting to 3.
 * \param[in] min The first vector.
 * \param[in] max The second vector.
 * \return The cross product of `pt1` and `pt2` as a `Vector<T, Size>`.
 */
//template<typename T, size_t Size = 3>
//auto crossProduct(const Vector<T, Size>& pt1,
//                  const Vector<T, Size>& pt2) -> Vector<T, Size>
//{
//    return Vector<T, Size>({
//    	pt1.y() * pt2.z() - pt1.z() * pt2.y(),
//        pt1.z() * pt2.x() - pt1.x() * pt2.z(),
//        pt1.x() * pt2.y() - pt1.y() * pt2.x()
//    });
//}
//
//template<typename T, size_t Size>
//auto operator<<(std::ostream& os, const Vector<T, Size>& vector) -> std::ostream&
//{
//    for (size_t i = 0; i < vector.size(); i++) {
//        os << " " << vector[i] << "\n";
//    }
//    os << std::flush;
//    return os;
//}
//
//template<typename T, size_t Size>
//auto operator<<(std::ostream& os, const Vector<T, Size>* vector) -> std::ostream&
//{
//    for (size_t i = 0; i < vector->size(); i++) {
//        os << " " << (*vector)[i] << "\n";
//    }
//    os << std::flush;
//    return os;
//}


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
//template<typename T>
//T vectorAngle(const Vector<T, 2> &v1, const Vector<T, 2> &v2) 
//{
//    if (v1 == v2) return 0.;
//
//    T magV1 = v1.module();
//    T magV2 = v2.module();
//    
//    if (magV1 == 0 || magV2 == 0) {
//        throw std::invalid_argument("The angle is not defined for zero magnitude vectors.");
//    }
//    
//    T cosTheta = dotProduct(v1, v2) / (magV1 * magV2);
//    // Asegurarse de que el valor esté en el rango [-1, 1] debido a posibles errores numéricos
//    cosTheta = tl::clamp(cosTheta, static_cast<T>(-1), static_cast<T>(1));
//    
//    return std::acos(cosTheta);
//}


/*! \} */


} // End namespace tl

