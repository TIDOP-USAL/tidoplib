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

/*! \file MatrixBlock.h
 * \brief Non‑owning view of a rectangular submatrix (block).
 *
 * This file defines the `MatrixBlock` class template, which provides a view
 * (reference) into a sub‑region of a parent matrix. It does not copy data;
 * instead, it stores a pointer to the parent’s data and the offsets and strides
 * needed to access the block elements. The block behaves like a regular matrix
 * (it inherits from `MatrixBase`) and can be used in expressions and assignments.
 *
 * Blocks are typically obtained via `Matrix::block()`, `Matrix::row()`, or
 * `Matrix::col()` and are intended to be used as temporaries. Assignments to
 * a block write back directly into the parent matrix.
 *
 * \ingroup Matrix
 * \see tl::Matrix, tl::MatrixBase
 */

#pragma once

#include "tidop/math/base/Data.h"
#include "tidop/graphic/Rect.h"
#include "tidop/math/base/Concepts.h"
#include "tidop/math/algebra/matrix/detail/Assign.h"

namespace tl
{

template<typename T>
class MatrixBase;

/*! \addtogroup Matrix
 *  \{
 */

/*!
 * \class MatrixBlock
 * \brief View of a rectangular submatrix (block) of a parent matrix.
 *
 * \tparam T Element type (may be const or non‑const). When `T` is `const`,
 *           the block is read‑only.
 *
 * `MatrixBlock` is a lightweight object that refers to a contiguous sub‑region
 * of a parent matrix. It does not own the data and is returned by
 * method `Matrix::block()`.
 *
 * Because the block does not own its data, it is only valid as long as the
 * parent matrix lives. Assignments to a block (via the special rvalue‑qualified
 * assignment operators) write back to the parent matrix.
 *
 * ### Example
 * \code
 * Matrix<double, 4, 4> A;
 * A.fill(0.0);
 * // Assign to a 2x2 block starting at (1,1)
 * A.block(1, 1, 2, 2) = Matrix<double, 2, 2>::identity();
 *
 * // Blocks can be used in expressions (they are automatically evaluated)
 * Matrix<double, 2, 2> B = A.block(0, 0, 2, 2) * 2.0;
 * \endcode
 */
template<typename T>
class MatrixBlock
  : public MatrixBase<MatrixBlock<T>>
{

public:

    using value_type = std::remove_cv_t<T>;
    using size_type = size_t;
    using reference = T &;
    using const_reference = const T &;

private:

    T *mData;             /*!< Pointer to the start of the parent matrix data. */
    size_t mParentCols;   /*!< Number of columns of the parent matrix (stride). */
    size_t mRows;         /*!< Number of rows of this block. */
    size_t mCols;         /*!< Number of columns of this block. */
    size_t mIniRow;       /*!< Starting row index in the parent matrix. */
    size_t mIniCol;       /*!< Starting column index in the parent matrix. */
  
public:

    /*!
     * \brief Constructs a block view from raw parameters.
     * \param[in] data       Pointer to the parent matrix data (must be non‑null).
     * \param[in] parentCols Number of columns of the parent matrix (stride).
     * \param[in] iniRow     Starting row index in the parent matrix.
     * \param[in] iniCol     Starting column index in the parent matrix.
     * \param[in] blockRows  Number of rows of the block.
     * \param[in] blockCols  Number of columns of the block.
     */
    constexpr MatrixBlock(T *data, 
                          size_t parentCols, 
                          size_t iniRow, 
                          size_t iniCol, 
                          size_t blockRows, 
                          size_t blockCols);

    /*!
     * \brief Assigns a matrix expression to this block (rvalue‑only).
     *
     * This operator is only callable on rvalue block objects (temporaries),
     * which is the typical usage when a block is returned by a method.
     *
     * \tparam Expr A type satisfying `MatrixExpr`.
     * \param[in] expr The expression to assign.
     * \return Reference to this block.
     *
     * \throws tl::Exception if the expression dimensions do not match the block.
     */
    template<MatrixExpr Expr>
    constexpr auto operator=(const Expr &expr) && -> MatrixBlock &;

    /*!
     * \brief Assigns another block to this block (rvalue‑only).
     * \param[in] other The source block.
     * \return Reference to this block.
     */
    constexpr auto operator=(const MatrixBlock &other) && ->MatrixBlock &;

    /*!
     * \brief Accesses the element at (row, col) with bounds checking.
     * \param[in] row Row index (0‑based, relative to the block).
     * \param[in] col Column index (0‑based, relative to the block).
     * \return Reference to the element.
     * \throws std::out_of_range if indices are invalid.
     */
    [[nodiscard]]
    constexpr auto at(size_t row, size_t col) -> reference;
    
    /*!
     * \brief Accesses the element at (row, col) with bounds checking (const version).
     * \param[in] row Row index.
     * \param[in] col Column index.
     * \return Const reference to the element.
     * \throws std::out_of_range if indices are invalid.
     */
    [[nodiscard]]
    constexpr auto at(size_t row, size_t col) const -> const_reference;

    /*!
     * \brief Accesses the element at (row, col) without bounds checking.
     * \param[in] row Row index.
     * \param[in] col Column index.
     * \return Reference to the element.
     */
    [[nodiscard]]
    constexpr auto operator()(size_t row, size_t col) -> reference;

    /*!
     * \brief Accesses the element at (row, col) without bounds checking (const version).
     * \param[in] row Row index.
     * \param[in] col Column index.
     * \return Const reference to the element.
     */
    [[nodiscard]]
    constexpr auto operator()(size_t row, size_t col) const -> const_reference;
    
    /*!
     * \brief Accesses the element at a linear index (row‑major order) without bounds checking.
     * \param[in] position Linear index = row * cols + col.
     * \return Reference to the element.
     */
    [[nodiscard]]
    constexpr auto operator()(size_t position) -> reference;

    /*!
     * \brief Accesses the element at a linear index (const version).
     * \param[in] position Linear index.
     * \return Const reference to the element.
     */
    [[nodiscard]]
    constexpr auto operator()(size_t position) const -> const_reference;

    /*!
     * \brief Subscript operator (linear index, no bounds checking).
     * \param[in] position Linear index.
     * \return Reference to the element.
     */
    [[nodiscard]]
    constexpr auto operator[](size_t position) -> reference;

    /*!
     * \brief Subscript operator (const version).
     * \param[in] position Linear index.
     * \return Const reference to the element.
     */
    [[nodiscard]]
    constexpr auto operator[](size_t position) const -> const_reference;

    /*!
     * \brief Returns the number of rows of the block.
     */
    [[nodiscard]]
    constexpr auto rows() const noexcept -> size_t { return mRows; }

    /*!
     * \brief Returns the number of columns of the block.
     */
    [[nodiscard]]
    constexpr auto cols() const noexcept -> size_t { return mCols; }

    /*!
     * \brief Checks whether the block’s data aliases a given memory address.
     * \param[in] ptr The pointer to test.
     * \return `true` if the data of this block overlaps the address `ptr`.
     */
    [[nodiscard]]
    constexpr auto aliases(const void *ptr) const -> bool;

    /*!
     * \brief Fills the entire block with a given value.
     * \tparam Scalar Type of the value (must be convertible to `value_type`).
     * \param[in] value The value to set.
     */
    constexpr void fill(value_type value);

};



/* MatrixBlock implementation */


template<typename T>
constexpr MatrixBlock<T>::MatrixBlock(T *data,
                                      size_t parentCols, 
                                      size_t iniRow, 
                                      size_t iniCol, 
                                      size_t blockRows, 
                                      size_t blockCols)
  : mData(data),
    mParentCols(parentCols),
    mIniRow(iniRow),
    mIniCol(iniCol),
    mRows(blockRows),
    mCols(blockCols)
{
}

template<typename T>
template<MatrixExpr Expr>
constexpr auto MatrixBlock<T>::operator=(const Expr &expr) && -> MatrixBlock &
{
    TL_ASSERT(expr.rows() == mRows && expr.cols() == mCols, "Block size mismatch in assignment");

    detail::assign_block(*this, expr);

    return *this;
}

template<typename T>
constexpr auto MatrixBlock<T>::operator=(const MatrixBlock &other) && -> MatrixBlock &
{
    return std::move(*this).template operator=<MatrixBlock>(other);
}

template<typename T>
constexpr auto MatrixBlock<T>::at(size_t row, size_t col) -> reference
{
    TL_ASSERT(row < mRows && col < mCols, "Matrix block out of range");

    return (*this)(row, col);
}

template<typename T>
constexpr auto MatrixBlock<T>::at(size_t row, size_t col) const -> const_reference
{
    TL_ASSERT(row < mRows && col < mCols, "Matrix block out of range");

    return (*this)(row, col);
}

template<typename T>
constexpr auto MatrixBlock<T>::operator()(size_t row, size_t col) -> reference
{
    return mData[(mIniRow + row) * mParentCols + (mIniCol + col)];
}

template<typename T>
constexpr auto MatrixBlock<T>::operator()(size_t row, size_t col) const -> const_reference
{
    return mData[(mIniRow + row) * mParentCols + (mIniCol + col)];
}

template<typename T>
constexpr auto MatrixBlock<T>::operator()(size_t position) -> reference
{
    size_t r = position / mCols;
    size_t c = position % mCols;
    return (*this)(r, c);
}

template<typename T>
constexpr auto MatrixBlock<T>::operator()(size_t position) const -> const_reference
{
    size_t r = position / mCols;
    size_t c = position % mCols;
    return (*this)(r, c);
}

template<typename T>
constexpr auto MatrixBlock<T>::operator[](size_t position) -> reference
{
    return (*this)(position);
}

template<typename T>
constexpr auto MatrixBlock<T>::operator[](size_t position) const -> const_reference
{
    return (*this)(position);
}

template<typename T>
constexpr auto MatrixBlock<T>::aliases(const void *ptr) const -> bool
{
    const T *p = static_cast<const T *>(ptr);

    for (size_t r = 0; r < mRows; ++r) {
        const T *row_start = mData + (mIniRow + r) * mParentCols + mIniCol;
        const T *row_end = row_start + mCols;

        if (p >= row_start && p < row_end)
            return true;
    }

    return false;
}

template<typename T>
constexpr void MatrixBlock<T>::fill(value_type value)
{
    size_t size = mRows * mCols;
    size_t i{0};

    // Para futura optimización en casos de memoria contigua, aunque en bloques no suele ser el caso
//#ifdef TL_HAVE_SIMD_INTRINSICS
//
//        if (matrix_traits<Derived>::has_contiguous_memory) {
//            Packed<value_type> packed_val(value);
//            constexpr size_t packed_size = packed_val.size();
//            size_t max_size = size - size % packed_size;
//
//            for (; i < max_size; i += packed_size) {
//                packed_val.storeUnaligned(&derived(i));
//            }
//        }
//#endif

    for (; i < size; i++) {
        (*this)(i) = value;
    }
}

/*! \} */

} // namespace tl