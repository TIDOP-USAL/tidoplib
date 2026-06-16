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

/*! \file MatrixRow.h
 * \brief Row view of a matrix (non‑owning).
 *
 * This file defines the `MatrixRow` class template, which provides a view (reference)
 * into one row of a parent matrix. It does not copy data; instead, it stores a pointer
 * to the parent’s data and the row index and stride. The row behaves like a `Vector`
 * (it inherits from `VectorBase`) and can be used in vector expressions and assignments.
 * Assignments to a row write back directly into the parent matrix.
 *
 * Row views are typically obtained via `Matrix::row()` and are intended to be used as
 * temporaries.
 *
 * \ingroup Matrix
 * \see tl::Matrix, tl::MatrixCol, tl::VectorBase
 */

#pragma once

#include "tidop/math/algebra/vector/Vector.h"
#include "tidop/math/algebra/vector/detail/Assign.h"

namespace tl
{

/*! \addtogroup Matrix
 *  \{
 */


/*!
 * \class MatrixRow
 * \brief Non‑owning view of a single row of a matrix.
 *
 * \tparam T Element type (may be const or non‑const).
 *
 * This class provides a row view of a matrix. It inherits from `VectorBase`
 * and can be used in vector expressions and assignments. Modifying elements of the
 * row modifies the original matrix. Because matrix rows are stored contiguously,
 * the view is efficient and supports SIMD operations when available.
 *
 * ### Example
 * \code
 * Matrix<double, 4, 4> A;
 * // Fill the second row (index 1) with 7.0
 * A.row(1).fill(7.0);
 *
 * // Assign a vector to a row
 * Vector<double, 4> v = {1,2,3,4};
 * A[0] = v;
 * \endcode
 */
template<typename T>
class MatrixRow
  : public VectorBase<MatrixRow<T>>
{
private:

    T *matrixData;
    size_t matrixRow;
    size_t matrixCols;

public:

    using value_type = std::remove_cv_t<T>;
    using size_type = size_t;
    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;

    using iterator = T *;
    using const_iterator = const T *;

public:

    /*!
     * \brief Constructs a row view.
     * \param[in] data  Pointer to the parent matrix data.
     * \param[in] row   Row index (0‑based).
     * \param[in] cols  Number of columns of the parent matrix.
     */
    constexpr MatrixRow(T *data, size_t row, size_t cols);

    /*!
     * \brief Assigns a vector expression to this row (writes back to parent matrix).
     * \tparam Expr A type satisfying `VectorExpr`.
     * \param[in] expr The expression to assign.
     * \return Reference to this row.
     */
    template<VectorExpr Expr>
    constexpr auto operator=(const Expr &expr) -> MatrixRow &;

    /*!
     * \brief Returns an iterator to the first element of the row.
     */
    [[nodiscard]]
    constexpr auto begin() noexcept -> iterator;

    /*!
     * \brief Returns a const iterator to the first element.
     */
    [[nodiscard]]
    constexpr auto begin() const noexcept -> const_iterator;

    /*!
     * \brief Returns an iterator to one past the last element.
     */
    [[nodiscard]]
    constexpr auto end() noexcept -> iterator;

    /*!
     * \brief Returns a const iterator to one past the last element.
     */
    [[nodiscard]]
    constexpr auto end() const noexcept -> const_iterator;

    /*!
     * \brief Returns the number of elements in the row (i.e., the number of columns).
     */
    [[nodiscard]]
    constexpr auto size() const noexcept -> size_t;

    /*!
     * \brief Fills the entire row with a given value.
     * \param[in] value Value to assign to each element.
     */
    constexpr void fill(T value);

    /*!
     * \brief Accesses the element at a given column (const version).
     * \param[in] column Column index.
     * \return Const reference to the element.
     */
    [[nodiscard]]
    constexpr auto operator[](size_t column) const -> const_reference;

    /*!
     * \brief Accesses the element at a given column (non‑const version).
     * \param[in] column Column index.
     * \return Reference to the element.
     */
    [[nodiscard]]
    constexpr auto operator[](size_t column) -> reference;

    /*!
     * \brief Checks if the row’s data aliases a given memory address.
     * \param[in] ptr Pointer to test.
     * \return `true` if the row’s underlying data starts at that address.
     */
    [[nodiscard]]
    constexpr auto aliases(const void *ptr) const -> bool;

#ifdef TL_HAVE_SIMD_INTRINSICS
    /*!
     * \brief Returns a SIMD packet of elements starting at index `i`.
     * \param[in] i Index (column offset) within the row.
     * \return `Packed<T>` containing the elements (if SIMD is enabled).
     */
    auto packet(size_t i) const noexcept -> Packed<value_type>;
#endif
};


/* MatrixRow implementation */

template<typename T>
constexpr MatrixRow<T>::MatrixRow(T *data, size_t row, size_t cols)
  : matrixData(data),
    matrixRow(row),
    matrixCols(cols)
{
}

template<typename T>
template<VectorExpr Expr>
constexpr auto MatrixRow<T>::operator=(const Expr &expr) -> MatrixRow &
{
    detail::assign_row(*this, expr);
    return *this;
}

template<typename T>
constexpr auto MatrixRow<T>::begin() noexcept -> iterator
{
    return &matrixData[matrixRow * matrixCols];
}

template<typename T>
constexpr auto MatrixRow<T>::begin() const noexcept -> const_iterator
{
    return &matrixData[matrixRow * matrixCols];
}

template<typename T>
constexpr auto MatrixRow<T>::end() noexcept -> iterator
{
    return &matrixData[matrixRow * matrixCols] + matrixCols;
}

template<typename T>
constexpr auto MatrixRow<T>::end() const noexcept -> const_iterator
{
  return &matrixData[matrixRow * matrixCols] + matrixCols;
}

template<typename T>
constexpr auto MatrixRow<T>::size() const noexcept -> size_t
{
    return matrixCols;
}

template<typename T>
constexpr void MatrixRow<T>::fill(T value)
{
    std::fill(begin(), end(), value);
}

template<typename T>
constexpr auto MatrixRow<T>::operator[](size_t column) const -> const_reference
{
    return matrixData[matrixRow * matrixCols + column];
}

template<typename T>
constexpr auto MatrixRow<T>::operator[](size_t column) -> reference
{
    return matrixData[matrixRow * matrixCols + column];
}

template<typename T>
constexpr auto MatrixRow<T>::aliases(const void *ptr) const -> bool
{
    return matrixData == ptr;
}

#ifdef TL_HAVE_SIMD_INTRINSICS
template<typename T>
auto MatrixRow<T>::packet(size_t i) const noexcept -> Packed<value_type>
{
    Packed<value_type> p;
    p.loadUnaligned(&matrixData[matrixRow * matrixCols + i]);
    return p;
}
#endif

/*! \} */

} // End namespace tl