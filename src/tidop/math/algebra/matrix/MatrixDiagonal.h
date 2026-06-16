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

/*! \file MatrixDiagonal.h
 * \brief Diagonal view of a matrix (non‑owning).
 *
 * This file defines the `MatrixDiagonal` class template, which provides a view
 * (reference) into the diagonal elements of a matrix. It does not copy data;
 * instead, it stores a pointer to the parent matrix's data and the stride
 * between diagonal elements. The diagonal behaves like a `Vector` (it inherits
 * from `VectorBase`) and can be used in vector expressions and assignments.
 * Assignments to the diagonal write back directly into the parent matrix.
 *
 * Diagonal views are typically obtained via `Matrix::diagonal()` (once
 * implemented) and are intended to be used as temporaries.
 *
 * \ingroup Matrix
 * \see tl::Matrix, tl::MatrixRow, tl::MatrixCol, tl::VectorBase
 */

#pragma once

#include "tidop/math/algebra/vector/Vector.h"
#include "tidop/math/algebra/vector/detail/Assign.h"

namespace tl
{
	
/*!
 * \class IteratorDiagonal
 * \brief Forward iterator for traversing the diagonal of a matrix.
 *
 * \tparam T Element type (may be const or non‑const).
 *
 * This iterator is used by `MatrixDiagonal` to provide STL‑compatible iteration
 * over the diagonal elements. It steps through memory using a fixed stride
 * (number of columns + 1) to move from one diagonal element to the next.
 */
template<typename T>
class IteratorDiagonal
{

public:

    using iterator_category = std::forward_iterator_tag; /*!< Forward iterator. */
    using value_type = T;                                /*!< Element type. */
    using difference_type = std::ptrdiff_t;              /*!< Pointer difference. */
    using pointer = T *;                                 /*!< Pointer to element. */
    using reference = T &;                               /*!< Reference to element. */

private:

    pointer mPtr = nullptr;      /*!< Current position in the diagonal. */
    size_t mStride = 0;    /*!< Stride (number of columns + 1) to advance to next diagonal element. */

public:

    constexpr IteratorDiagonal() = default;

    /*!
     * \brief Constructs an iterator pointing to a given position on the diagonal.
     * \param[in] ptr    Pointer to the current diagonal element.
     * \param[in] stride Number of columns of the parent matrix (plus one) to move to the next diagonal element.
     */
    constexpr IteratorDiagonal(pointer ptr, size_t stride);
    ~IteratorDiagonal() = default;

    /*!
     * \brief Dereferences the iterator.
     * \return Reference to the current diagonal element.
     */
    constexpr auto operator*() const -> reference;

    /*!
     * \brief Arrow operator.
     * \return Pointer to the current diagonal element.
     */
    constexpr auto operator->() const -> pointer;

    /*!
     * \brief Pre‑increment (move to next diagonal element).
     * \return Reference to the incremented iterator.
     */
    constexpr auto operator++() -> IteratorDiagonal &;

    /*!
     * \brief Post‑increment.
     * \return Copy of the iterator before increment.
     */
    constexpr auto operator++(int) -> IteratorDiagonal;

    [[nodiscard]]
    constexpr auto operator== (const IteratorDiagonal &other) const noexcept -> bool;

    /*!
     * \brief Inequality comparison.
     * \param[in] other Other iterator.
     * \return `true` if they point to different positions.
     */
    [[nodiscard]]
    constexpr auto operator!= (const IteratorDiagonal &other) const noexcept -> bool;

}; 


/*!
 * \class MatrixDiagonal
 * \brief Non‑owning view of the diagonal of a matrix.
 *
 * \tparam T Element type (may be const or non‑const).
 *
 * This class provides a diagonal view of a matrix. It inherits from `VectorBase`
 * and can be used in vector expressions and assignments. Modifying elements of
 * the diagonal modifies the original matrix. The diagonal is stored compactly;
 * the view does not copy the data.
 *
 * ### Example
 * \code
 * Matrix<double, 4, 4> A;
 * // Fill the diagonal with 1.0
 * A.diagonal().fill(1.0);
 *
 * // Assign a vector to the diagonal
 * Vector<double, 4> v = {1,2,3,4};
 * A.diagonal() = v;
 * \endcode
 *
 * \note This class is typically obtained via `Matrix::diagonal()`.
 */
template<typename T>
class MatrixDiagonal
  : public VectorBase<MatrixDiagonal <T>>
{

private:

    T *matrixData;      /*!< Pointer to the start of the parent matrix data. */
    size_t mRows;       /*!< Number of rows of the parent matrix. */
    size_t mCols;       /*!< Number of columns of the parent matrix (stride). */

public:

    using value_type = std::remove_cv_t<T>;
    using size_type = size_t;
    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;

    using iterator = IteratorDiagonal<T>;
    using const_iterator = IteratorDiagonal<const T>;

public:

    /*!
     * \brief Constructs a diagonal view.
     * \param[in] data  Pointer to the parent matrix data.
     * \param[in] rows  Number of rows of the parent matrix.
     * \param[in] cols  Number of columns of the parent matrix (stride).
     */
    constexpr MatrixDiagonal(T *data, size_t row, size_t cols);

    /*!
     * \brief Assigns a vector expression to this diagonal (writes back to parent matrix).
     * \tparam Expr A type satisfying `VectorExpr`.
     * \param[in] expr The expression to assign.
     * \return Reference to this diagonal.
     */
    template<VectorExpr Expr>
    constexpr auto operator=(const Expr &expr) -> MatrixDiagonal &
    {
        detail::assign_diagonal(*this, expr);
        return *this;
    }

    /*!
     * \brief Returns an iterator to the first diagonal element.
     */
    [[nodiscard]]
    constexpr auto begin() noexcept -> iterator;

    /*!
     * \brief Returns a const iterator to the first diagonal element.
     */
    [[nodiscard]]
    constexpr auto begin() const noexcept -> const_iterator;

    /*!
     * \brief Returns an iterator to one past the last diagonal element.
     */
    [[nodiscard]]
    constexpr auto end() noexcept -> iterator;

    /*!
     * \brief Returns a const iterator to one past the last diagonal element.
     */
    [[nodiscard]]
    constexpr auto end() const noexcept -> const_iterator;

    /*!
     * \brief Returns the number of diagonal elements (min(rows, cols)).
     */
    [[nodiscard]]
    constexpr auto size() const noexcept -> size_t;

    /*!
     * \brief Fills the entire diagonal with a given value.
     * \param[in] value Value to assign to each diagonal element.
     */
    constexpr void fill(T value);

    /*!
     * \brief Accesses the element at a given diagonal index (const version).
     * \param[in] index Index along the diagonal (0‑based).
     * \return Const reference to the element.
     */
    [[nodiscard]]
    constexpr auto operator[](size_t index) const -> const_reference;

    /*!
     * \brief Accesses the element at a given diagonal index (non‑const version).
     * \param[in] index Index along the diagonal.
     * \return Reference to the element.
     */
    [[nodiscard]]
    constexpr auto operator[](size_t index) -> reference;

    /*!
     * \brief Checks if the diagonal’s data aliases a given memory address.
     * \param[in] ptr Pointer to test.
     * \return `true` if the diagonal’s underlying data starts at that address.
     */
    [[nodiscard]]
    constexpr auto aliases(const void *ptr) const -> bool;

};



/* IteratorDiagonal implementation */

template<typename T>
constexpr IteratorDiagonal<T>::IteratorDiagonal(pointer ptr, size_t stride)
  : mPtr(ptr), 
    mStride(stride)
{
}

template<typename T>
constexpr auto IteratorDiagonal<T>::operator*() const -> reference
{
    return *mPtr;
}

template<typename T>
constexpr auto IteratorDiagonal<T>::operator->() const -> pointer
{
    return mPtr;
}

template<typename T>
constexpr auto IteratorDiagonal<T>::operator++() -> IteratorDiagonal&
{
    mPtr += mStride;
    return *this;
}

template<typename T>
constexpr auto IteratorDiagonal<T>::operator++(int) -> IteratorDiagonal
{
    IteratorDiagonal it = *this;
    ++(*this);
    return it;
}

template<typename T>
constexpr auto IteratorDiagonal<T>::operator == (const IteratorDiagonal<T> &other) const noexcept -> bool
{
    return this->mPtr == other.mPtr;
}

template<typename T>
constexpr auto IteratorDiagonal<T>::operator != (const IteratorDiagonal<T> &other) const noexcept -> bool
{
    return this->mPtr != other.mPtr;
}


/* MatrixDiagonal implementation */

template<typename T>
constexpr MatrixDiagonal<T>::MatrixDiagonal(T *data, size_t row, size_t cols)
  : matrixData(data),
    mRows(row),
    mCols(cols)
{
}

template<typename T>
constexpr auto MatrixDiagonal<T>::begin() noexcept -> iterator
{
    return iterator(matrixData, mCols + 1);
}

template<typename T>
constexpr auto MatrixDiagonal<T>::begin() const noexcept -> const_iterator
{
    return iterator(matrixData, mCols + 1);
}

template<typename T>
constexpr auto MatrixDiagonal<T>::end() noexcept -> iterator
{
    return iterator(matrixData + size() * (mCols + 1), mCols + 1);
}

template<typename T>
constexpr auto MatrixDiagonal<T>::end() const noexcept -> const_iterator
{
  return iterator(matrixData + size() * (mCols + 1), mCols + 1);
}

template<typename T>
constexpr auto MatrixDiagonal<T>::size() const noexcept -> size_t
{
    return std::min(mRows, mCols);
}

template<typename T>
constexpr void MatrixDiagonal<T>::fill(T value)
{
    std::fill(begin(), end(), value);
}

template<typename T>
constexpr auto MatrixDiagonal<T>::operator[](size_t i) const -> const_reference
{
    return matrixData[i * (mCols + 1)];
}

template<typename T>
constexpr auto MatrixDiagonal<T>::operator[](size_t i) -> reference
{
    return matrixData[i * (mCols + 1)];
}

/*!
* \brief Checks if the diagonal’s data aliases a given memory address.
* \param[in] ptr Pointer to test.
* \return `true` if the diagonal’s underlying data starts at that address.
*/
template<typename T>
constexpr auto MatrixDiagonal<T>::aliases(const void *ptr) const -> bool
{
    return matrixData == ptr;
}


} // End namespace tl