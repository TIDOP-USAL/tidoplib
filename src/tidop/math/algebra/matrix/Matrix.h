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

#include "tidop/core/base/exception.h"
#include "tidop/core/concurrency/parallel.h"
#include "tidop/math/math.h"
#include "tidop/math/base/data.h"
#include "tidop/math/algebra/decomp/lu.h"
#include "tidop/math/algebra/decomp/svd.h"
#include "tidop/math/algebra/matrix/MatrixBase.h"
#include "tidop/math/algebra/matrix/MatrixBlock.h"
#include "tidop/math/algebra/matrix/MatrixRow.h"
#include "tidop/math/algebra/matrix/MatrixCol.h"
#include "tidop/math/algebra/matrix/Kernel.h"
#include "tidop/math/algebra/detail/Evaluator.h"
#include "tidop/math/algebra/matrix/detail/Assing.h"

#include <type_traits>
#include <iomanip>
#include <concepts>

namespace tl
{

/*! \addtogroup Matrix
 *  \{
 */

template<typename T, size_t Rows, size_t Cols>
class Matrix;


template<typename T, size_t Rows = DynamicData, size_t Cols = DynamicData>
class Matrix
  : public MatrixBase<Matrix<T, Rows, Cols>>
{

    static_assert(Arithmetic<T>, "Matrix requires an arithmetic type (integral or floating-point)");
    static_assert(!std::is_const_v<T>, "Matrix cannot hold const types. Use 'const Matrix<T>' instead.");

public:

    using value_type = T;
    using size_type = size_t;
    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;

    enum data
    {
        size = DataSize<T, Rows, Cols>::size()
    };

public:

    /*!
     * \brief Default constructor
     */
    Matrix();

    /*!
     * \brief Row-column constructor
     * \param[in] rows Matrix rows
     * \param[in] cols Matrix columns
     */
    Matrix(size_t rows, size_t cols);

    /*!
     * \brief Rows, columns and value constructor
     * \param[in] rows Matrix rows
     * \param[in] cols Matrix columns
     * \param[in] value Matrix value
     */
    Matrix(size_t rows, size_t cols, T value);

    /*!
     * \brief Copy constructor
     * \param[in] mat Matrix object being copied
     */
    Matrix(const Matrix &mat);

    //template<typename MatrixDerived>
    //Matrix(const MatrixDerived &matrix);

    /*!
     * \brief Move constructor
     * \param[in] mat Matrix object being moved
     */
    Matrix(Matrix &&mat) TL_NOEXCEPT;

    /*!
     * \brief Constructor using initializer list for one-dimensional matrix
     * \param[in] values Initializer list containing values for the matrix
     */
    Matrix(std::initializer_list<T> values);

    /*!
     * \brief Constructor using initializer list for two-dimensional matrix
     * \param[in] values Initializer list containing lists of values for the matrix
     */
    Matrix(std::initializer_list<std::initializer_list<T>> values);

    /*!
     * \brief Constructor using raw data for matrix initialization
     * \param[in] data Pointer to the raw data
     * \param[in] rows Number of rows in the matrix
     * \param[in] cols Number of columns in the matrix
     */
    Matrix(const T *data, size_t rows, size_t cols);

    template<MatrixExpr Expr>
    Matrix(const Expr& expr) 
        : mData(Data<T, data::size>(expr.rows() * expr.cols())),
          mRows(Rows == DynamicData ? expr.rows() : Rows),
          mCols(Cols == DynamicData ? expr.cols() : Cols)
    {

        TL_ASSERT(expr.rows() == mRows &&
                  expr.cols() == mCols, "Static matrix cannot be resized");

        //*this = expr;
        detail::assign(*this, expr);
    }
	
    /*!
     * \brief Copy assignment operator
     * \param[in] matrix Object being copied
     * \return Reference to the assigned object
     */
    auto operator=(const Matrix &matrix) -> Matrix &
    {
        if (this != &matrix) {
            this->mData = matrix.mData;
            this->mRows = matrix.mRows;
            this->mCols = matrix.mCols;
        }

        return *this;
    }

    /*!
     * \brief Move assignment operator
     * \param[in] matrix Object being moved
     * \return Reference to the assigned object
     */
    auto operator=(Matrix &&matrix) noexcept -> Matrix &
    {
        if (this != &matrix) {
            this->mData = std::move(matrix.mData);
            this->mRows = std::move(matrix.mRows);
            this->mCols = std::move(matrix.mCols);
        }

        return *this;
    }

    template<MatrixExpr Expr>
    auto operator=(const Expr &expr) -> Matrix& 
    {
        return detail::assign(*this, expr);
    }	

    /*!
     * \brief Number of rows in the matrix
     * \return Number of rows
     */
    constexpr auto rows() const noexcept -> size_t
    {
        return mRows;
    }

    /*!
     * \brief Number of columns in the matrix
     * \return Number of columns
     */
    constexpr auto cols() const noexcept -> size_t
    {
        return mCols;
    }

    /*!
     * \brief Swap two rows
     * \param[in] i First row to swap
     * \param[in] j Second row to swap
     */
    void swapRows(size_t i, size_t j)
    {
        for (size_t c = 0; c < mCols; c++) {
            std::swap(mData[i * mCols + c], mData[j * mCols + c]);
        }
    }

    /*!
     *\brief Swap two cols
     *\param[in] i First cols to swap
     *\param[in] j Second cols to swap
     */
    void swapCols(size_t i, size_t j)
    {
        for (size_t r = 0; r < mRows; r++) {
            std::swap(mData[r * mCols + i], mData[r * mCols + j]);
        }
    }

    /*!
     * \brief Reference to the element at position (r, c)
     * \param[in] r Row of the matrix
     * \param[in] c Column of the matrix
     * \return Value of the matrix at the specified row and column position
     * <h4>Example</h4>
     * \code
     * Matrix<double, 3, 3> matrix;
     * matrix.at(0, 0) = 1.5;
     * double value = matrix.at(0, 0);
     * \endcode
     */
    auto at(size_t r, size_t c) -> reference
    {
        return mData.at(r * mCols + c);
    }

    /*!
     * \brief Constant reference to the element at position (r, c)
     * \param[in] r Row
     * \param[in] c Column
     * \return Value of the matrix at the specified row and column position
     * <h4>Example</h4>
     * \code
     * double value = matrix.at(0, 0);
     * \endcode
     */
    auto at(size_t r, size_t c) const -> const_reference
    {
        return mData.at(r * mCols + c);
    }

    /*!
     * \brief Reference to the element at position (r, c)
     * \param[in] r Row of the matrix
     * \param[in] c Column of the matrix
     * \return Value of the matrix at the specified row and column position
     * <h4>Example</h4>
     * \code
     * Matrix<double, 3, 3> matrix;
     * matrix(0, 0) = 1.5;
     * double value = matrix(0, 0);
     * \endcode
     */
    auto operator()(size_t r, size_t c) noexcept -> reference
    {
        return mData[r * mCols + c];
    }

    /*!
     * \brief Constant reference to the element at position (r, c)
     * \param[in] r Row
     * \param[in] c Column
     * \return Value of the matrix at the specified row and column position
     * <h4>Example</h4>
     * \code
     * double value = matrix(0, 0);
     * \endcode
     */
    auto operator()(size_t r, size_t c) const noexcept -> const_reference
    {
        return mData[r * mCols + c];
    }

    /*!
     * \brief Reference to the element
     * The position of the element is determined as:
     *   r * this->cols() + c
     * \param[in] position Position of the matrix element
     * \return Value of the matrix at the specified position
     * <h4>Example</h4>
     * \code
     * Matrix<double, 3, 3> matrix;
     * matrix(4) = 1.5;
     * double value = matrix(4); // value == 1.5
     * \endcode
     */
    auto operator()(size_t position) noexcept -> reference
    {
        return mData[position];
    }

    /*!
     * \brief Constant reference to the element
     * The position of the element is determined as:
     *   r * this->cols() + c
     * \param[in] position Position of the matrix element
     * \return Value of the matrix at the specified position
     * <h4>Example</h4>
     * \code
     * Matrix<double, 3, 3> matrix;
     * matrix(4) = 1.5;
     * double value = matrix(4); // value == 1.5
     * \endcode
     */
    auto operator()(size_t position) const noexcept -> const_reference
    {
        return mData[position];
    }

    auto block(size_t startRow, size_t startCol, size_t numRows, size_t numCols) -> MatrixBlock<T, DynamicData, DynamicData>
    {
        TL_ASSERT(startRow + numRows <= this->rows() &&
                  startCol + numCols <= this->cols(), "Matrix block out of range");

        // Ya no hay que calcular nada, los parámetros van directos al constructor
        return MatrixBlock<T, DynamicData, DynamicData>(this->data(), this->cols(), startRow, startCol, numRows, numCols);
    }

    auto block(size_t startRow, size_t startCol, size_t numRows, size_t numCols) const -> const MatrixBlock<const T, DynamicData, DynamicData>
    {
        TL_ASSERT(startRow + numRows <= this->rows() &&
                  startCol + numCols <= this->cols(), "Matrix block out of range");

        return MatrixBlock<const T, DynamicData, DynamicData>(this->data(), this->cols(), startRow, startCol, numRows, numCols);
    }

    auto row(size_t r) -> MatrixRow<T> { return MatrixRow<T>(this->data(), r, this->cols()); }
    auto row(size_t r) const -> const MatrixRow<const T> { return MatrixRow<const T>(this->data(), r, this->cols()); }

    auto col(size_t c) -> MatrixCol<T> { return MatrixCol<T>(this->data(), c, this->rows(), this->cols()); }
    auto col(size_t c) const -> const MatrixCol<const T> { return MatrixCol<const T>(this->data(), c, this->rows(), this->cols()); }

    //auto rowBlock(size_t iniRow, size_t endRow) -> auto { return block(iniRow, endRow, 0, this->cols() - 1); }
    //auto rowBlock(size_t iniRow, size_t endRow) const -> auto { return block(iniRow, endRow, 0, this->cols() - 1); }

    //auto colBlock(size_t iniCol, size_t endCol) -> auto { return block(0, this->rows() - 1, iniCol, endCol); }
    //auto colBlock(size_t iniCol, size_t endCol) const -> auto { return block(0, this->rows() - 1, iniCol, endCol); }

    auto operator[](size_t r) -> MatrixRow<T> { return row(r); }
    auto operator[](size_t r) const -> const MatrixRow<const T> { return row(r); }

    /*!
     * \brief Constructs a matrix of zeros
     * \f[
     * A=\begin{bmatrix}
     * 0 & 0 & 0 \\
     * 0 & 0 & 0 \\
     * 0 & 0 & 0 \\
     * \end{bmatrix}
     * \f]
     * \return
     */
    static auto zero(size_t rows = 0, size_t cols = 0) -> Matrix
    {
        return Matrix<T, Rows, Cols>(rows == 0 ? Rows : rows, cols == 0 ? Cols : cols, consts::zero<T>);
    }

    /*!
     * \brief Constructs a matrix of 'ones'
     * \f[
     * A=\begin{bmatrix}
     * 1 & 1 & 1 \\
     * 1 & 1 & 1 \\
     * 1 & 1 & 1 \\
     * \end{bmatrix}
     * \f]
     * \return
     */
    static auto ones(size_t rows = 0, size_t cols = 0) -> Matrix
    {
        return Matrix<T, Rows, Cols>(rows == 0 ? Rows : rows, cols == 0 ? Cols : cols, consts::one<T>);
    }

    /*!
     * \brief Constructs the identity matrix
     * \f[
     * A=\begin{bmatrix}
     * 1 & 0 & 0 \\
     * 0 & 1 & 0 \\
     * 0 & 0 & 1 \\
     * \end{bmatrix}
     * \f]
     * \return
     */
    static auto identity(size_t rows = 0, size_t cols = 0) -> Matrix
    {
        Matrix<T, Rows, Cols> matrix(rows, cols);

        for (size_t r = 0; r < matrix.rows(); r++) {
            for (size_t c = 0; c < matrix.cols(); c++) {
                if (r == c) {
                    matrix(r, c) = consts::one<T>;
                } else {
                    matrix(r, c) = consts::zero<T>;
                }
            }
        }

        return matrix;
    }

    /*!
     * \brief Constructs a matrix with random values
     */
    static auto randon(size_t rows = 0, size_t cols = 0) -> Matrix
    {
        Matrix<T, Rows, Cols> matrix(rows, cols);

        static thread_local std::mt19937 random_number_engine(std::random_device{}());
        std::uniform_real_distribution<> distribution(0.0, 99.0);

        size_t size = matrix.rows() * matrix.cols();

        for (size_t i = 0; i < size; ++i) {
            matrix(i) = static_cast<T>(distribution(random_number_engine));
        }

        return matrix;
    }

    auto data() noexcept -> pointer
    {
        return mData.data();
    }

    auto data() const noexcept -> const_pointer
    {
        return mData.data();
    }

	auto aliases(const void* ptr) const -> bool 
    {
        return static_cast<const void*>(this->data()) == ptr;
    }

    auto packet(size_t i) const noexcept -> Packed<T>
    {
        Packed<T> p;
        p.loadUnaligned(&this->data()[i]);
        return p;
    }

    void resize(size_t rows, size_t cols)
        requires (Rows == DynamicData || Cols == DynamicData)
    {
        if (mRows == rows && mCols == cols) {
            return;
        }

        mRows = rows;
        mCols = cols;

        mData.resize(mRows * mCols);
    }

    void fill(T value)
    {
        std::fill(mData.begin(), mData.end(), value);
    }

private:

    Data<T, size> mData;
    size_t mRows{Rows};
    size_t mCols{Cols};
};


/* Matrix alias */

using Matrix2x2i = Matrix<int, 2, 2>;
using Matrix2x2f = Matrix<float, 2, 2>;
using Matrix2x2d = Matrix<double, 2, 2>;
using Matrix3x3i = Matrix<int, 3, 3>;
using Matrix3x3f = Matrix<float, 3, 3>;
using Matrix3x3d = Matrix<double, 3, 3>;
using Matrix4x4i = Matrix<int, 4, 4>;
using Matrix4x4f = Matrix<float, 4, 4>;
using Matrix4x4d = Matrix<double, 4, 4>;




/* Matrix implementation */

template<typename T, size_t Rows, size_t Cols>
Matrix<T, Rows, Cols>::Matrix()
  : mData(Data<T, data::size>()),
    mRows(Rows == DynamicData ? 0 : Rows),
    mCols(Cols == DynamicData ? 0 : Cols)
{

}

template<typename T, size_t Rows, size_t Cols>
Matrix<T, Rows, Cols>::Matrix(size_t rows, size_t cols)
  : mData(Data<T, data::size>(rows *cols)),
    mRows(Rows == DynamicData ? rows : Rows),
    mCols(Cols == DynamicData ? cols : Cols)
{
}

template<typename T, size_t Rows, size_t Cols>
Matrix<T, Rows, Cols>::Matrix(size_t rows, size_t cols, T value)
  : mData(Data<T, data::size>(rows *cols, value)),
    mRows(Rows == DynamicData ? rows : Rows),
    mCols(Cols == DynamicData ? cols : Cols)
{
}

template<typename T, size_t Rows, size_t Cols>
Matrix<T, Rows, Cols>::Matrix(const Matrix &mat)
  : mData(mat.mData),
    mRows(mat.mRows),
    mCols(mat.mCols)
{
}

template<typename T, size_t Rows, size_t Cols>
Matrix<T, Rows, Cols>::Matrix(Matrix &&mat) noexcept
  : mData(std::move(mat.mData)),
    mRows(std::move(mat.mRows)),
    mCols(std::move(mat.mCols))
{
}

template<typename T, size_t Rows, size_t Cols>
Matrix<T, Rows, Cols>::Matrix(std::initializer_list<T> values)
{
    if (Rows == DynamicData && Cols == DynamicData) {

        this->mRows = 1;
        this->mCols = values.size();
        mData = Data<T, data::size>(values.size());
        std::copy(values.begin(), values.end(), mData.begin());

    } else {

        mData = Data<T, data::size>();
        this->mRows = Rows;
        this->mCols = Cols;

        size_t n = values.size();
        if (n == data::size) {
            std::copy(values.begin(), values.end(), mData.begin());
        } else if (n < data::size) {
            std::copy(values.begin(), values.end(), mData.begin());
            std::fill(mData.begin() + n, mData.end(), consts::zero<T>);
        } else {
            std::copy(values.begin(), values.begin() + data::size, mData.begin());
        }
    }
}

template<typename T, size_t Rows, size_t Cols>
Matrix<T, Rows, Cols>::Matrix(std::initializer_list<std::initializer_list<T>> values)
{
    if (Rows == DynamicData && Cols == DynamicData) {

        this->mRows = values.size();
        auto it = values.begin();
        this->mCols = it->size();
        mData = Data<T, data::size>(this->mRows * this->mCols);

        auto it_data = mData.begin();
        for (auto it = values.begin(); it != values.end(); it++) {
            size_t n = it->size();
            if (n == mCols) {
                std::copy(it->begin(), it->end(), it_data);
            } else if (n < mCols) {
                std::copy(it->begin(), it->end(), it_data);
                std::fill(it_data + n, mData.end(), consts::zero<T>);
            } else {
                std::copy(it->begin(), it->end() + mCols, it_data);
            }
            it_data += mCols;
        }

    } else {

        auto it_data = mData.begin();
        size_t rows_counter = 0;
        for (auto it = values.begin(); it != values.end(); it++) {
            if (rows_counter < Rows) {
                size_t n = it->size();
                if (n == Cols) {
                    std::copy(it->begin(), it->end(), it_data);
                } else if (n < Cols) {
                    std::copy(it->begin(), it->end(), it_data);
                    std::fill(it_data + n, mData.end(), consts::zero<T>);
                } else {
                    std::copy(it->begin(), it->end() + Cols, it_data);
                }

                it_data += Cols;
                rows_counter++;
            }
        }

        if (it_data != mData.end()) {
            std::fill(it_data, mData.end(), consts::zero<T>);
        }
    }
}

template<typename T, size_t Rows, size_t Cols>
Matrix<T, Rows, Cols>::Matrix(const T *data, size_t rows, size_t cols)
  : mData(Data<T, data::size>(data, rows *cols)),
    mRows(Rows == DynamicData ? rows : Rows),
    mCols(Cols == DynamicData ? cols : Cols)
{
}



/*! \} */

} // namespace tl


