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

/*! \file Matrix.h
 * \brief Dense matrix implementation supporting static and dynamic sizes.
 *
 * This file defines the `Matrix` class template, which provides a dense matrix
 * with support for both compile‑time fixed dimensions (when `Rows` and `Cols`
 * are numeric values) and runtime dynamic dimensions (when `Rows` or `Cols`
 * equals `DynamicData`). The class inherits from `MatrixBase` and uses
 * expression templates for lazy evaluation, enabling efficient chaining of
 * operations without unnecessary temporaries.
 *
 * \ingroup Matrix
 * \see tl::MatrixBase, tl::MatrixExpr, tl::Vector
 */

#pragma once

#include "tidop/core/base/exception.h"
#include "tidop/core/concurrency/parallel.h"
#include "tidop/math/math.h"
#include "tidop/math/base/Data.h"
#include "tidop/math/algebra/decomp/LU.h"
#include "tidop/math/algebra/decomp/SVD.h"
#include "tidop/math/algebra/matrix/MatrixBase.h"
#include "tidop/math/algebra/matrix/MatrixBlock.h"
#include "tidop/math/algebra/matrix/MatrixDiagonal.h"
#include "tidop/math/algebra/matrix/MatrixRow.h"
#include "tidop/math/algebra/matrix/MatrixCol.h"
#include "tidop/math/algebra/detail/Evaluator.h"
#include "tidop/math/algebra/matrix/detail/Assign.h"

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


/*!
 * \class Matrix
 * \brief Dense matrix with support for static and dynamic sizes.
 *
 * \tparam T Element type (must be arithmetic, e.g., `float`, `double`, `int`).
 * \tparam Rows Number of rows at compile time, or `DynamicData` for dynamic rows.
 * \tparam Cols Number of columns at compile time, or `DynamicData` for dynamic columns.
 *
 * The `Matrix` class provides a dense, column‑major (or row‑major, depending on
 * implementation) storage of elements. It supports all basic matrix operations
 * (addition, subtraction, multiplication, element‑wise operations, etc.) through
 * expression templates, as well as linear algebra decompositions (LU, SVD, etc.),
 * submatrix views (blocks, rows, columns), and convenient factory methods.
 *
 * ### Example (static size)
 * \code
 * Matrix<double, 3, 3> A = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
 * Matrix<double, 3, 3> B = Matrix<double, 3, 3>::identity();
 * auto C = A * B;          // expression, not evaluated yet
 * auto result = C.eval();  // concrete matrix
 * \endcode
 *
 * ### Example (dynamic size)
 * \code
 * Matrix<double> D(2, 3);  // 2 rows, 3 columns, elements uninitialized
 * D.fill(0.0);
 * D(0, 1) = 5.0;
 * \endcode
 */
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

    /*! \brief Compile‑time total number of elements (size = rows×cols or DynamicData). */
    enum data
    {
        size = DataSize<T, Rows, Cols>::size()
    };

public:

    /*!
     * \brief Default constructor.
     * For static matrices, elements are default‑initialized.
     * For dynamic matrices, creates an empty matrix (0×0).
     */
    Matrix();

    /*!
     * \brief Constructs a matrix with given number of rows and columns.
     * \param[in] rows Number of rows (ignored for static matrices).
     * \param[in] cols Number of columns (ignored for static matrices).
     * Elements are default‑initialized (no values set).
     */
    Matrix(size_t rows, size_t cols);

    /*!
     * \brief Constructs a matrix with given dimensions and initializes all elements to a value.
     * \param[in] rows Number of rows (ignored for static matrices).
     * \param[in] cols Number of columns (ignored for static matrices).
     * \param[in] value Value to fill the matrix with.
     */
    Matrix(size_t rows, size_t cols, T value);

    /*!
     * \brief Copy constructor.
     * \param[in] mat Matrix to copy.
     */
    Matrix(const Matrix &mat);

    /*!
     * \brief Move constructor.
     * \param[in] mat Matrix to move.
     */
    Matrix(Matrix &&mat) noexcept;

    /*!
     * \brief Constructs a matrix from a flat initializer list (row‑major order).
     * \param[in] values List of values in row‑major order.
     * For static matrices, the number of values must equal `Rows * Cols`.
     * For dynamic matrices, the size is deduced as `values.size()`, and the matrix is
     * a column vector (number of rows = size, columns = 1) unless the shape is
     * provided otherwise (use the 2D initializer list overload).
     */
    Matrix(std::initializer_list<T> values);

    /*!
     * \brief Constructs a matrix from a 2D initializer list.
     * \param[in] values List of rows, each row being a list of values.
     * Example: `{{1,2},{3,4}}` creates a 2×2 matrix.
     */
    Matrix(std::initializer_list<std::initializer_list<T>> values);

    /*!
     * \brief Constructs a matrix from raw data.
     * \param[in] data Pointer to the raw data (row‑major order).
     * \param[in] rows Number of rows.
     * \param[in] cols Number of columns.
     */
    Matrix(const T *data, size_t rows, size_t cols);

    /*!
     * \brief Constructs a matrix from a matrix expression.
     * \tparam Expr A type satisfying `MatrixExpr`.
     * \param[in] expr The expression to evaluate.
     */
    template<MatrixExpr Expr>
    Matrix(const Expr &expr);
	
    /*!
     * \brief Copy assignment operator
     * \param[in] matrix Object being copied
     * \return Reference to the assigned object
     */
    auto operator=(const Matrix &matrix) -> Matrix &;

    /*!
     * \brief Move assignment operator
     * \param[in] matrix Object being moved
     * \return Reference to the assigned object
     */
    auto operator=(Matrix &&matrix) noexcept -> Matrix &;

    /*!
     * \brief Assignment from a matrix expression.
     * \tparam Expr A type satisfying `MatrixExpr`.
     * \param[in] expr The expression to evaluate and assign.
     * \return Reference to `*this`.
     */
    template<MatrixExpr Expr>
    auto operator=(const Expr &expr) -> Matrix& 
    {
        return detail::assign(*this, expr);
    }	

    /*!
     * \brief Number of rows.
     * \return Number of rows (compile‑time constant for static matrices).
     */
    constexpr auto rows() const noexcept -> size_t { return mRows; }

    /*!
     * \brief Number of columns.
     * \return Number of columns (compile‑time constant for static matrices).
     */
    constexpr auto cols() const noexcept -> size_t { return mCols; }

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
    auto at(size_t r, size_t c) -> reference { return mData.at(r * mCols + c); }

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
    auto at(size_t r, size_t c) const -> const_reference { return mData.at(r * mCols + c); }

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
    auto operator()(size_t r, size_t c) noexcept -> reference { return mData[r * mCols + c]; }

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
    auto operator()(size_t r, size_t c) const noexcept -> const_reference { return mData[r * mCols + c]; }

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
    auto operator()(size_t position) noexcept -> reference { return mData[position]; }

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
    auto operator()(size_t position) const noexcept -> const_reference { return mData[position]; }

    /*!
     * \brief Row access using subscript operator (returns a row view).
     * \param[in] r Row index.
     * \return `MatrixRow` object (mutable).
     */
    auto operator[](size_t r) -> MatrixRow<T> { return row(r); }

    /*!
     * \brief Const row access using subscript operator.
     * \param[in] r Row index.
     * \return Const `MatrixRow` object.
     */
    auto operator[](size_t r) const -> const MatrixRow<const T> { return row(r); }

    /*!
     * \brief Returns a mutable block (submatrix) view.
     * \param[in] startRow Starting row index.
     * \param[in] startCol Starting column index.
     * \param[in] numRows Number of rows in the block.
     * \param[in] numCols Number of columns in the block.
     * \return `MatrixBlock` object representing the submatrix.
     */
    auto block(size_t startRow, size_t startCol, size_t numRows, size_t numCols) -> MatrixBlock<T>;

    /*!
     * \brief Returns a const block view.
     * \param[in] startRow Starting row index.
     * \param[in] startCol Starting column index.
     * \param[in] numRows Number of rows in the block.
     * \param[in] numCols Number of columns in the block.
     * \return Const `MatrixBlock` object.
     */
    auto block(size_t startRow, size_t startCol, size_t numRows, size_t numCols) const -> const MatrixBlock<const T>;

    /*!
     * \brief Returns a mutable row view.
     * \param[in] r Row index.
     * \return `MatrixRow` object representing the row (as a vector).
     */
    auto row(size_t r) -> MatrixRow<T>;

    /*!
     * \brief Returns a const row view.
     * \param[in] r Row index.
     * \return Const `MatrixRow` object.
     */
    auto row(size_t r) const -> const MatrixRow<const T>;

    /*!
     * \brief Returns a mutable column view.
     * \param[in] c Column index.
     * \return `MatrixCol` object representing the column (as a vector).
     */
    auto col(size_t c) -> MatrixCol<T>;

    /*!
     * \brief Returns a const column view.
     * \param[in] c Column index.
     * \return Const `MatrixCol` object.
     */
    auto col(size_t c) const -> const MatrixCol<const T>;

    /*!
     * \brief Returns a mutable view of the matrix diagonal.
     *
     * Provides a `MatrixDiagonal<T>` view that allows reading and writing the
     * diagonal elements of the matrix. The size of the diagonal is `min(rows(), cols())`.
     * The view behaves like a vector and can be used in vector expressions and assignments.
     * Any modification to the diagonal view is reflected directly in the original matrix.
     *
     * \return A `MatrixDiagonal<T>` object representing the diagonal.
     *
     * \note The diagonal elements are accessed with stride = columns + 1 in row‑major storage.
     * \note For a square matrix, the diagonal size equals the number of rows/columns.
     *       For a non‑square matrix, the diagonal is truncated to the smaller dimension.
     *
     * ### Example
     * \code
     * Matrix<double, 4, 4> A;
     * // Fill the diagonal with 1.0
     * A.diagonal().fill(1.0);
     *
     * // Assign a vector to the diagonal
     * Vector<double, 4> v = {1, 2, 3, 4};
     * A.diagonal() = v;
     *
     * // Multiply the diagonal by 2 (using expression)
     * A.diagonal() *= 2;
     * \endcode
     */
    auto diagonal() -> MatrixDiagonal<T>;

    /*!
     * \brief Returns a const view of the matrix diagonal.
     *
     * Provides a read‑only `MatrixDiagonal<const T>` view of the diagonal elements.
     * The size of the diagonal is `min(rows(), cols())`. The view can be used in
     * vector expressions but cannot modify the original matrix.
     *
     * \return A `MatrixDiagonal<const T>` object representing the diagonal.
     *
     * ### Example
     * \code
     * const Matrix<double, 3, 3> A = ...;
     * auto diag = A.diagonal();
     * double d0 = diag[0];       // read first diagonal element
     * // diag[0] = 5.0;          // error: read‑only view
     * \endcode
     */
    auto diagonal() const -> MatrixDiagonal<const T>;

    /*!
     * \brief Swap two rows
     * \param[in] i First row to swap
     * \param[in] j Second row to swap
     */
    void swapRows(size_t i, size_t j);

    /*!
     *\brief Swap two cols
     *\param[in] i First cols to swap
     *\param[in] j Second cols to swap
     */
    void swapCols(size_t i, size_t j);

    /*!
     * \brief Fills all elements with a given value.
     * \param[in] value Value to fill.
     */
    void fill(T value);

    /* Factory methods */

    /*!
     * \brief Creates a matrix of zeros.
     * 
     * \f[
     * A=\begin{bmatrix}
     * 0 & 0 & 0 \\
     * 0 & 0 & 0 \\
     * 0 & 0 & 0 \\
     * \end{bmatrix}
     * \f]
     * 
     * \param[in] rows Number of rows (for dynamic matrices).
     * \param[in] cols Number of columns (for dynamic matrices).
     * \return Zero matrix.
     */
    TL_DEPRECATED("zeros(size_t rows = 0, size_t cols = 0)", "4.0")
    static auto zero(size_t rows = 0, size_t cols = 0) -> Matrix;
    static auto zeros(size_t rows = 0, size_t cols = 0) -> Matrix;

    /*!
     * \brief Creates a matrix of ones.
     * 
     * \f[
     * A=\begin{bmatrix}
     * 1 & 1 & 1 \\
     * 1 & 1 & 1 \\
     * 1 & 1 & 1 \\
     * \end{bmatrix}
     * \f]
     * 
     * \param[in] rows Number of rows.
     * \param[in] cols Number of columns.
     * \return Matrix with all elements set to 1.
     */
    static auto ones(size_t rows = 0, size_t cols = 0) -> Matrix;

    /*!
     * \brief Creates an identity matrix.
     * 
     * \f[
     * A=\begin{bmatrix}
     * 1 & 0 & 0 \\
     * 0 & 1 & 0 \\
     * 0 & 0 & 1 \\
     * \end{bmatrix}
     * \f]
     * \param[in] rows Number of rows.
     * \param[in] cols Number of columns (if different from rows, the matrix is not square,
     *                 but still has ones on the main diagonal up to min(rows, cols)).
     * \return Identity matrix.
     */
    static auto identity(size_t rows = 0, size_t cols = 0) -> Matrix;

    /*!
     * \brief Creates a matrix with random values uniformly distributed in [0,1].
     * \param[in] rows Number of rows.
     * \param[in] cols Number of columns.
     * \return Random matrix.
     */
    static auto randon(size_t rows = 0, size_t cols = 0) -> Matrix;

    /*!
     * \brief Returns a pointer to the underlying data.
     * \return Mutable pointer.
     */
    auto data() noexcept -> pointer { return mData.data(); }

    /*!
     * \brief Returns a const pointer to the underlying data.
     * \return Const pointer.
     */
    auto data() const noexcept -> const_pointer { return mData.data(); }

    /*!
     * \brief Checks if the matrix’s data aliases a given memory address.
     * \param[in] ptr Pointer to test.
     * \return `true` if internal storage uses that address.
     */
	auto aliases(const void* ptr) const -> bool;

#ifdef TL_HAVE_SIMD_INTRINSICS
    /*!
     * \brief Returns a SIMD packet of elements starting at index `i`.
     * \param[in] i Linear index.
     * \return `Packed<T>` containing the elements, if SIMD enabled.
     */
    auto packet(size_t i) const noexcept -> Packed<T>;
#endif

    /*!
     * \brief Resizes the matrix (only for dynamic matrices).
     * \param[in] rows New number of rows.
     * \param[in] cols New number of columns.
     * \note Existing data is lost; elements are default‑initialized.
     */
    void resize(size_t rows, size_t cols)
        requires (Rows == DynamicData || Cols == DynamicData);

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
  : mData(Data<T, data::size>(rows * cols)),
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

        this->mRows = values.size();
        this->mCols = 1;
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

template<typename T, size_t Rows, size_t Cols>
template<MatrixExpr Expr>
Matrix<T, Rows, Cols>::Matrix(const Expr &expr)
  : mData(Data<T, data::size>(expr.rows() *expr.cols())),
    mRows(Rows == DynamicData ? expr.rows() : Rows),
    mCols(Cols == DynamicData ? expr.cols() : Cols)
{

    TL_ASSERT(expr.rows() == mRows &&
        expr.cols() == mCols, "Static matrix cannot be resized");

    detail::assign(*this, expr);
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::operator=(const Matrix &matrix) -> Matrix &
{
    if (this != &matrix) {
        this->mData = matrix.mData;
        this->mRows = matrix.mRows;
        this->mCols = matrix.mCols;
    }

    return *this;
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::operator=(Matrix &&matrix) noexcept -> Matrix &
{
    if (this != &matrix) {
        this->mData = std::move(matrix.mData);
        this->mRows = std::move(matrix.mRows);
        this->mCols = std::move(matrix.mCols);
    }

    return *this;
}

template<typename T, size_t Rows, size_t Cols>
void Matrix<T, Rows, Cols>::swapRows(size_t i, size_t j)
{
    for (size_t c = 0; c < mCols; c++) {
        std::swap(mData[i * mCols + c], mData[j * mCols + c]);
    }
}

template<typename T, size_t Rows, size_t Cols>
void Matrix<T, Rows, Cols>::swapCols(size_t i, size_t j)
{
    for (size_t r = 0; r < mRows; r++) {
        std::swap(mData[r * mCols + i], mData[r * mCols + j]);
    }
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::block(size_t startRow, size_t startCol, size_t numRows, size_t numCols) -> MatrixBlock<T>
{
    TL_ASSERT(startRow + numRows <= this->rows() &&
        startCol + numCols <= this->cols(), "Matrix block out of range");

    return MatrixBlock<T>(this->data(), this->cols(), startRow, startCol, numRows, numCols);
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::block(size_t startRow, size_t startCol, size_t numRows, size_t numCols) const -> const MatrixBlock<const T>
{
    TL_ASSERT(startRow + numRows <= this->rows() &&
        startCol + numCols <= this->cols(), "Matrix block out of range");

    return MatrixBlock<const T>(this->data(), this->cols(), startRow, startCol, numRows, numCols);
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::row(size_t r) -> MatrixRow<T>
{ 
    return MatrixRow<T>(this->data(), r, this->cols()); 
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::row(size_t r) const -> const MatrixRow<const T>
{
    return MatrixRow<const T>(this->data(), r, this->cols()); 
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::col(size_t c) -> MatrixCol<T> 
{ 
    return MatrixCol<T>(this->data(), c, this->rows(), this->cols()); 
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::col(size_t c) const -> const MatrixCol<const T> 
{
    return MatrixCol<const T>(this->data(), c, this->rows(), this->cols());
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::diagonal() -> MatrixDiagonal<T>
{
    return MatrixDiagonal<T>(this->data(), this->rows(), this->cols());
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::diagonal() const -> MatrixDiagonal<const T>
{
    return MatrixDiagonal<const T>(this->data(), this->rows(), this->cols());
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::zero(size_t rows, size_t cols) -> Matrix
{
    return Matrix<T, Rows, Cols>(rows == 0 ? Rows : rows, cols == 0 ? Cols : cols, consts::zero<T>);
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::zeros(size_t rows, size_t cols) -> Matrix
{
    return Matrix<T, Rows, Cols>(rows == 0 ? Rows : rows, cols == 0 ? Cols : cols, consts::zero<T>);
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::ones(size_t rows, size_t cols) -> Matrix
{
    return Matrix<T, Rows, Cols>(rows == 0 ? Rows : rows, cols == 0 ? Cols : cols, consts::one<T>);
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::identity(size_t rows, size_t cols) -> Matrix
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

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::randon(size_t rows, size_t cols) -> Matrix
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

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::aliases(const void *ptr) const -> bool
{
    return static_cast<const void *>(this->data()) == ptr;
}

#ifdef TL_HAVE_SIMD_INTRINSICS
template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::packet(size_t i) const noexcept -> Packed<T>
{
    Packed<T> p;
    p.loadUnaligned(&this->data()[i]);
    return p;
}
#endif

template<typename T, size_t Rows, size_t Cols>
void Matrix<T, Rows, Cols>::resize(size_t rows, size_t cols)
    requires (Rows == DynamicData || Cols == DynamicData)
{
    if (mRows == rows && mCols == cols) {
        return;
    }

    mRows = rows;
    mCols = cols;

    mData.resize(mRows * mCols);
}

template<typename T, size_t Rows, size_t Cols>
void Matrix<T, Rows, Cols>::fill(T value)
{
    std::fill(mData.begin(), mData.end(), value);
}

/*! \} */

} // namespace tl


