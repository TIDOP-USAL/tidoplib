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

#include "tidop/math/base/data.h"
#include "tidop/graphic/Rect.h"
#include "tidop/math/base/Concepts.h"
#include "tidop/math/algebra/matrix/detail/Assing.h"

// Mover a Assing.h
//#include "tidop/math/algebra/eval/MatrixBlockEval.h"

namespace tl
{

template<typename T>
class MatrixBase;

template<typename T, size_t Rows, size_t Cols>
class MatrixBlock;

namespace detail
{

//template<typename Block, typename Expr>
//void assign_block(Block &dst, const Expr &expr)
//{
//    if (expr.aliases(&dst(0, 0))) {
//        Matrix<typename Block::value_type> tmp(expr);
//        assign_block(dst, tmp);
//        return;
//    }
//
//    Evaluator<std::remove_cvref_t<Expr>> eval(expr);
//
//    for (size_t r = 0; r < dst.rows(); ++r) {
//        for (size_t c = 0; c < dst.cols(); ++c) {
//            dst(r, c) = eval.coeff(r, c);
//        }
//    }
//}

//template<typename Block, typename Expr>
//void assign_block(Block &dst, const Expr &expr)
//{
//    using CleanExpr = std::remove_cvref_t<Expr>;
//
//    // Alias
//    if (expr.aliases(&dst(0, 0))) {
//        Matrix<typename Block::value_type> tmp(expr);
//        assign_block(dst, tmp);
//        return;
//    }
//
//    if constexpr (is_matrix_product_v<CleanExpr>) {
//
//        const auto &lhs = expr.lhs();
//        const auto &rhs = expr.rhs();
//
//        // TODO: Aqui no siempre se necesita un temporal, solo cuando hay alias
//        Matrix<typename matrix_traits<Block>::value_type> tmp(lhs.rows(), rhs.cols());
//
//        detail::mulmat(lhs, rhs, tmp);
//
//        assign_block(dst, tmp);
//        return;
//
//    } else {
//
//        Evaluator<CleanExpr> eval(expr);
//
//        for (size_t r = 0; r < dst.rows(); ++r) {
//            for (size_t c = 0; c < dst.cols(); ++c) {
//                dst(r, c) = eval.coeff(r, c);
//            }
//        }
//    }
//}

} // namespace detail


template<typename T, size_t Rows = DynamicData, size_t Cols = DynamicData>
class MatrixBlock
  : public MatrixBase<MatrixBlock<T, Rows, Cols>>
{

public:

    using value_type = std::remove_cv_t<T>;
    using size_type = size_t;
    using reference = T &;
    using const_reference = const T &;

private:

    T *mData;             // Puntero a la memoria de la matriz ORIGINAL
    size_t mParentCols;   // Columnas de la matriz original (necesario para el salto/stride)

    size_t mRows;         // Filas de este bloque
    size_t mCols;         // Columnas de este bloque
    size_t mIniRow;       // Fila de inicio en la original
    size_t mIniCol;       // Columna de inicio en la original
  
public:

    MatrixBlock(T *data, 
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

    template<MatrixExpr Expr>
    auto operator=(const Expr &expr) && -> MatrixBlock &
    {
        TL_ASSERT(expr.rows() == mRows && expr.cols() == mCols, "Block size mismatch in assignment");

        detail::assign_block(*this, expr);

        return *this;

        //TL_ASSERT(expr.rows() == mRows && expr.cols() == mCols, "Block size mismatch in assignment");

        //if (expr.aliases(mData)) {

        //    Matrix<T> temp_eval(expr);

        //    for (size_t r = 0; r < mRows; ++r) {
        //        for (size_t c = 0; c < mCols; ++c) {
        //            (*this)(r, c) = temp_eval(r, c);
        //        }
        //    }

        //} else {

        //    for (size_t r = 0; r < mRows; ++r) {
        //        for (size_t c = 0; c < mCols; ++c) {
        //            (*this)(r, c) = expr(r, c);
        //        }
        //    }
        //}

        //return *this;
    }

    auto operator=(const MatrixBlock &other) && -> MatrixBlock &
    {
        return std::move(*this).template operator=<MatrixBlock>(other);
    }

    /*!
     * \brief Reference to the element at position (row, col)
     * \param[in] row Row of the matrix
     * \param[in] col Column of the matrix
     * \return Value of the matrix at the specified row and column position
     * <h4>Example</h4>
     * \code
     * Matrix<double,3,3> matrix;
     * matrix.at(0, 0) = 1.5;
     * double value = matrix.at(0, 0);
     * \endcode
     */
    auto at(size_t row, size_t col) -> reference
    {
        TL_ASSERT(row < mRows && col < mCols, "Matrix block out of range");

        return (*this)(row, col);
    }
    
    /*!
     * \brief Constant reference to the element at position (row, col)
     * \param[in] row Row
     * \param[in] col Column
     * \return Value of the matrix at the specified row and column position
     * <h4>Example</h4>
     * \code
     * double value = matrix.at(0, 0);
     * \endcode
     */
    auto at(size_t row, size_t col) const -> const_reference
    {
        TL_ASSERT(row < mRows && col < mCols, "Matrix block out of range");

        return (*this)(row, col);
    }

    /*!
     * \brief Reference to the element at position (row, col)
     * \param[in] row Row of the matrix
     * \param[in] col Column of the matrix
     * \return Value of the matrix at the specified row and column position
     * <h4>Example</h4>
     * \code
     * Matrix<double,3,3> matrix;
     * matrix(0, 0) = 1.5;
     * double value = matrix(0, 0);
     * \endcode
     */
    auto operator()(size_t row, size_t col) -> reference
    {
        return mData[(mIniRow + row) * mParentCols + (mIniCol + col)];
    }

    /*!
     * \brief Constant reference to the element at position (row, col)
     * \param[in] row Row
     * \param[in] col Column
     * \return Value of the matrix at the specified row and column position
     * <h4>Example</h4>
     * \code
     * double value = matrix(0, 0);
     * \endcode
     */
    auto operator()(size_t row, size_t col) const -> const_reference
    {
        return mData[(mIniRow + row) * mParentCols + (mIniCol + col)];
    }
    
    /*!
     * \brief Reference to the element
     * The position of the element is determined as:
     *   r * this->cols() + c
     * \param[in] position Position of the matrix element
     * \return Value of the matrix at that position
     * <h4>Example</h4>
     * \code
     * Matrix<double,3,3> matrix;
     * matrix(4) = 1.5;
     * double value = matrix(4); // value == 1.5
     * \endcode
     */
    auto operator()(size_t position) -> reference
    {
        size_t r = position / mCols;
        size_t c = position % mCols;
        return (*this)(r, c);
    }

    /*!
     * \brief Constant reference to the element
     * The position of the element is determined as:
     *   r * this->cols() + c
     * \param[in] position Position of the matrix element
     * \return Value of the matrix at that position
     * <h4>Example</h4>
     * \code
     * Matrix<double,3,3> matrix;
     * matrix(4) = 1.5;
     * double value = matrix(4); // value == 1.5
     * \endcode
     */
    auto operator()(size_t position) const -> const_reference
    {
        size_t r = position / mCols;
        size_t c = position % mCols;
        return (*this)(r, c);
    }

    auto operator[](size_t position) -> reference
    {
        return (*this)(position);
    }

    auto operator[](size_t position) const -> const_reference
    {
        return (*this)(position);
    }

    auto rows() const noexcept -> size_t { return mRows; }
    auto cols() const noexcept -> size_t { return mCols; }

    //auto aliases(const void *ptr) const -> bool 
    //{
    //    return mData == ptr;
    //}
    auto aliases(const void *ptr) const -> bool
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
};




/*------------------------------------------------------------------------*/
/* MatrixBlock implementation                                             */
/*------------------------------------------------------------------------*/

//template<typename T, size_t Rows, size_t Cols>
//MatrixBlock<T, Rows, Cols>::MatrixBlock(T *data,
//                                        size_t rows,
//                                        size_t cols,
//                                        size_t iniRow,
//                                        size_t endRow,
//                                        size_t iniCol,
//                                        size_t endCol)
//  : matrixData(data),
//    matrixRows(rows),
//    matrixCols(cols),
//    matrixIniRow(iniRow),
//    matrixEndRow(endRow),
//    matrixIniCol(iniCol),
//    matrixEndCol(endCol)
//{
//    this->properties.disable(MatrixBlock<T, Rows, Cols>::Properties::contiguous_memory);
//}
//
//template<typename T, size_t Rows, size_t Cols>
//auto MatrixBlock<T, Rows, Cols>::operator=(const MatrixBlock &block) -> MatrixBlock&
//{
//    size_t rows = this->rows();
//    size_t cols = this->cols();
//    size_t rows2 = block.rows();
//    size_t cols2 = block.cols();
//
//    TL_ASSERT(rows == rows2 && cols == cols2, "A size != B size");
//
//    Rect<size_t> rect1(this->matrixIniCol, cols, this->cols(), rows);
//    Rect<size_t> rect2(block.matrixIniCol, block.matrixIniRow, cols2, rows2);
//    Rect<size_t> intersect = tl::intersect(rect1, rect2);
//
//    if(this->matrixData == block.matrixData && intersect.isValid()) {
//
//        Matrix<T, Rows, Cols> mat = block;
//
//        for(size_t r = 0; r < this->rows(); r++) {
//            for(size_t c = 0; c < this->cols(); c++) {
//                (*this)(r, c) = mat(r, c);
//            }
//        }
//
//    } else {
//
//        for(size_t r = 0; r < this->rows(); r++) {
//            for(size_t c = 0; c < this->cols(); c++) {
//                (*this)(r, c) = block(r, c);
//            }
//        }
//
//    }
//
//    return *this;
//}
//
//template<typename T, size_t Rows, size_t Cols>
//template<typename T2, size_t _rows2, size_t _cols2>
//auto MatrixBlock<T, Rows, Cols>::operator=(const Matrix<T2, _rows2, _cols2> &matrix) -> MatrixBlock&
//{
//    size_t rows = this->rows();
//    size_t cols = this->cols();
//    size_t rows2 = matrix.rows();
//    size_t cols2 = matrix.cols();
//
//    TL_ASSERT(rows == rows2 && cols == cols2, "A size != B size");
//
//    for(size_t r = 0; r < this->rows(); r++) {
//        for(size_t c = 0; c < this->cols(); c++) {
//            (*this)(r, c) = static_cast<T>(matrix(r, c));
//        }
//    }
//
//    return *this;
//}
//
//template<typename T, size_t Rows, size_t Cols>
//auto MatrixBlock<T, Rows, Cols>::at(size_t row, size_t col) -> reference
//{
//    if(matrixEndRow - matrixIniRow < row || matrixEndCol - matrixIniCol < col) throw std::out_of_range("Matrix block out of range");
//
//    return (*this)(row, col);
//}
//
//template<typename T, size_t Rows, size_t Cols>
//auto MatrixBlock<T, Rows, Cols>::at(size_t row, size_t col) const -> const_reference
//{
//    if(matrixEndRow - matrixIniRow < row || matrixEndCol - matrixIniCol < col) throw std::out_of_range("Matrix block out of range");
//
//    return (*this)(row, col);
//}
//
//template<typename T, size_t Rows, size_t Cols>
//auto MatrixBlock<T, Rows, Cols>::operator()(size_t row, size_t col) -> reference
//{
//    return matrixData[(matrixIniRow + row) * matrixCols + col + matrixIniCol];
//}
//
//template<typename T, size_t Rows, size_t Cols>
//auto MatrixBlock<T, Rows, Cols>::operator()(size_t row, size_t col) const -> const_reference
//{
//    return matrixData[(matrixIniRow + row) * matrixCols + col + matrixIniCol];
//}
//
//template<typename T, size_t Rows, size_t Cols>
//auto MatrixBlock<T, Rows, Cols>::operator()(size_t position) -> reference
//{
//    size_t col = position % cols();
//    size_t row = position / cols();
//
//    return (*this)(row, col);
//}
//
//template<typename T, size_t Rows, size_t Cols>
//auto MatrixBlock<T, Rows, Cols>::operator()(size_t position) const -> const_reference
//{
//    size_t col = position % cols();
//    size_t row = position / cols();
//
//    return (*this)(row, col);
//}
//
//template<typename T, size_t Rows, size_t Cols>
//auto MatrixBlock<T, Rows, Cols>::rows() const -> size_t
//{
//    return matrixEndRow - matrixIniRow + 1;
//}
//
//template<typename T, size_t Rows, size_t Cols>
//auto MatrixBlock<T, Rows, Cols>::cols() const -> size_t
//{
//    return matrixEndCol - matrixIniCol + 1;
//}
//
//template<typename T, size_t Rows, size_t Cols>
//MatrixBlock<T, Rows, Cols>::operator Matrix<T, DynamicData, DynamicData>()
//{
//    Matrix<T, DynamicData, DynamicData> matrix(this->rows(), this->cols());
//
//    for(size_t r = 0; r < this->rows(); r++) {
//        for(size_t c = 0; c < this->cols(); c++) {
//            matrix(r, c) = (*this)(r, c);
//        }
//    }
//
//    return matrix;
//}

} // End namespace tl