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
#include "tidop/math/algebra/vector.h"
#include "tidop/math/base/simd.h"
#include "tidop/math/base/blas.h"
#include "tidop/math/base/cuda.h"
#include "tidop/math/base/data.h"
#include "tidop/math/algebra/decomp/lu.h"
#include "tidop/math/algebra/matrix/row.h"
#include "tidop/math/algebra/matrix/col.h"
#include "tidop/math/algebra/matrix/block.h"
#include "tidop/math/algebra/matrix/base.h"
#include "tidop/geometry/rect.h"

#include <type_traits>
#include <iomanip>

namespace tl
{


//TODO: Expression templates:
//      Por ahora no se implementan las operaciones entre matrices con 
//      expressions templates. En su lugar se emplea cuando es posible
//      el operador de movimiento para minimizar en lo posible la creación 
//      de objetos temporales. 


/*! \addtogroup Matrix
 *  \{
 */

/// \cond
template<typename T>
class MatrixBase;

template<typename T, size_t Rows, size_t Cols>
class Matrix;

template<typename T>
class LuDecomposition;
/// \endcond


class TL_EXPORT MatrixConfig
{

public:

    enum class Product
    {
#ifdef TL_HAVE_CUDA
        CuBLAS,
#endif
#ifdef TL_HAVE_OPENBLAS
        BLAS,
#endif
#ifdef TL_HAVE_SIMD_INTRINSICS
        SIMD,
#endif
        CPP
    };

    Product product =
#ifdef TL_HAVE_SIMD_INTRINSICS
        Product::SIMD;
#else
        Product::CPP;
#endif

    static auto instance() -> MatrixConfig &
    {
        static MatrixConfig _config;
        return _config;
    }
};


template<typename T, size_t Rows = DynamicData, size_t Cols = DynamicData>
class Matrix
  : public MatrixBase<Matrix<T, Rows, Cols>>
{

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

    template<typename MatrixDerived>
    Matrix(const MatrixDerived &matrix);

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

    ~Matrix() override = default;

    /*!
     * \brief Copy assignment operator
     * \param[in] matrix Object being copied
     * \return Reference to the assigned object
     */
    auto operator=(const Matrix &matrix)->Matrix &;

    /*!
     * \brief Move assignment operator
     * \param[in] matrix Object being moved
     * \return Reference to the assigned object
     */
    auto operator=(Matrix &&matrix) TL_NOEXCEPT-> Matrix &;

    operator Matrix<T, DynamicData, DynamicData>();

    /*!
     * \brief Number of rows in the matrix
     * \return Number of rows
     */
    auto rows() const TL_NOEXCEPT -> size_t;

    /*!
     * \brief Number of columns in the matrix
     * \return Number of columns
     */
    auto cols() const TL_NOEXCEPT -> size_t;

    /*!
     * \brief Inverse matrix
     * A square and invertible matrix A has an inverse matrix \f[ A^{-1} \f]
     * \param[out] invertibility Checks if the matrix is invertible
     * \return The inverse matrix
     * <h4>Example</h4>
     * \code
     * Matrix<double, 2, 2> mat_2x2{2., 3.,
     *                               1., 4.};
     * bool invertible;
     * Matrix<double, 2, 2> inv_mat = mat_2x2.inverse(&invertible);
     * \endcode
     */
    auto inverse(bool *invertibility = nullptr) const -> Matrix;

    /*!
     * \brief Calculates the transpose matrix
     *
     * \f[
     * A=\begin{bmatrix}
     * 1 & 2 & 3 \\
     * 4 & 5 & 6 \\
     * 7 & 8 & 9 \\
     * \end{bmatrix}
     * \f]
     *
     * \f[
     * A^{T}=\begin{bmatrix}
     * 1 & 4 & 7 \\
     * 2 & 5 & 8 \\
     * 3 & 6 & 9 \\
     * \end{bmatrix}
     * \f]
     *
     * <h4>Example</h4>
     * \code
     * Matrix<double, 2, 2> mat_2x2{2., 3.,
     *                               1., 4.};
     * Matrix<double, 2, 2> transpose_mat = mat_2x2.transpose();
     * std::cout << transpose_mat << std::endl;
     * \endcode
     *
     * \return The transpose matrix
     */
    auto transpose() const->Matrix<T, Cols, Rows>;

    /*!
     * \brief Calculates the adjugate matrix
     * \f[ adj(A) = C^T \f]
     * \return The adjugate matrix
     */
    auto adjugate() const->Matrix;

    /*!
     * \brief Calculates the cofactor matrix
     * \return The cofactor matrix
     */
    auto cofactorMatrix() const->Matrix;

    /*!
     * \brief Trace of a square matrix
     * Sum of the elements on the main diagonal of a square matrix
     * \f[ tr(A) = a_{11} + ... +  a_{nn} \f]
     * \return The trace of the matrix
     */
    auto trace() const -> T;

    /*!
     * \brief Checks if the matrix is invertible
     * \return True if the matrix is invertible
     */
    auto invertible() -> bool;

    /*!
     * \brief Checks if the matrix is singular
     * A square matrix that has no inverse is singular. The
     * determinant of a singular matrix is 0
     * \return True if the matrix is singular
     */
    auto singular() -> bool;

    /*!
     * \brief Cofactor
     * The determinant obtained by removing the row and column of a given element from a matrix or determinant.
     * The cofactor is preceded by a sign + or - depending on whether the element is in a + or - position.
     * \f[ (-)^{r+j} \f]
     * \return The cofactor
     */
    auto cofactor(size_t r, size_t c) const -> T;

    /*!
     * \brief First minor
     * A minor of a square matrix A is the determinant of some of the
     * submatrices obtained from the elimination of a row and a column.
     * They are used for calculating the cofactor matrix.
     *
     * \f[
     * A=\begin{bmatrix}
     * a1 & a2 & a3 \\
     * a4 & a5 & a6 \\
     * a7 & a8 & a9 \\
     * \end{bmatrix}
     * \f]
     *
     * \f[ M_{23} = a8*a1-a2*a7 \f]
     *
     * \return The first minor
     */
    auto firstMinor(size_t row, size_t col) const -> T;

    /*!
     * \brief Row echelon form
     */
    auto rowEchelonForm() const -> Matrix;

    /*!
     * \brief Reduced row echelon form
     */
    auto reducedRowEchelonForm() const -> Matrix;

    /*!
     * \brief Rank of a matrix
     */
    auto rank() const -> int;

    /*!
     * \brief Swap two rows
     * \param[in] i First row to swap
     * \param[in] j Second row to swap
     */
    auto swapRows(size_t i, size_t j) -> void;

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
    auto at(size_t r, size_t c) -> reference;

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
    auto at(size_t r, size_t c) const -> const_reference;

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
    auto operator()(size_t r, size_t c) TL_NOEXCEPT -> reference;

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
    auto operator()(size_t r, size_t c) const TL_NOEXCEPT -> const_reference;

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
    auto operator()(size_t position) TL_NOEXCEPT -> reference;

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
    auto operator()(size_t position) const TL_NOEXCEPT-> const_reference;

    auto operator[](size_t position) const -> const internal::MatrixRow<const T>;
    auto operator[](size_t position) -> internal::MatrixRow<T>;

    auto row(size_t row) const -> const internal::MatrixRow<const T>;
    auto row(size_t row) -> internal::MatrixRow<T>;

    auto col(size_t col) const -> const internal::MatrixCol<const T>;
    auto col(size_t col) -> internal::MatrixCol<T>;

    auto block(size_t iniRow, size_t endRow, size_t iniCol, size_t endCol) const -> const internal::MatrixBlock<const T>;
    auto block(size_t iniRow, size_t endRow, size_t iniCol, size_t endCol) -> internal::MatrixBlock<T>;

    auto rowBlock(size_t iniRow, size_t endRow) const -> const internal::MatrixBlock<const T, DynamicData, DynamicData>;
    auto rowBlock(size_t iniRow, size_t endRow) -> internal::MatrixBlock<T, DynamicData, DynamicData>;
    auto colBlock(size_t iniCol, size_t endCol) const -> const internal::MatrixBlock<const T, DynamicData, DynamicData>;
    auto colBlock(size_t iniCol, size_t endCol) -> internal::MatrixBlock<T, DynamicData, DynamicData>;

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
    static auto zero() -> Matrix;
    static auto zero(size_t rows, size_t cols) -> Matrix;

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
    static auto ones() -> Matrix;
    static auto ones(size_t rows, size_t cols) -> Matrix;

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
    static auto identity() -> Matrix;
    static auto identity(size_t rows, size_t cols) -> Matrix;

    /*!
     * \brief Constructs a matrix with random values
     */
    static auto randon() -> Matrix;
    static auto randon(size_t rows, size_t cols) -> Matrix;

    auto data() TL_NOEXCEPT -> pointer;
    auto data() const TL_NOEXCEPT -> const_pointer;

private:

    auto inverse2x2(bool *invertibility) const->Matrix;
    auto inverse3x3(bool *invertibility) const->Matrix;
    auto inverse4x4(bool *invertibility) const->Matrix;
    auto inversenxn(bool *invertibility) const->Matrix;

    auto adjoint2x2() const->Matrix;
    auto adjoint3x3() const->Matrix;
    auto adjoint4x4() const->Matrix;
    auto adjointnxn() const->Matrix;

private:

    Data<T, size> mData;
    size_t mRows{Rows};
    size_t mCols{Cols};
};




using Matrix2x2i = Matrix<int, 2, 2>;
using Matrix2x2f = Matrix<float, 2, 2>;
using Matrix2x2d = Matrix<double, 2, 2>;
using Matrix3x3i = Matrix<int, 3, 3>;
using Matrix3x3f = Matrix<float, 3, 3>;
using Matrix3x3d = Matrix<double, 3, 3>;
using Matrix4x4i = Matrix<int, 4, 4>;
using Matrix4x4f = Matrix<float, 4, 4>;
using Matrix4x4d = Matrix<double, 4, 4>;




/// \cond

namespace internal
{

#ifdef TL_HAVE_SIMD_INTRINSICS
template<typename T, size_t _rows1, size_t _col1, size_t _rows2, size_t _cols2, size_t _rows3, size_t _cols3>
void mulmat_simd(const Matrix<T, _rows1, _col1> &matrix1,
                 const Matrix<T, _rows2, _cols2> &matrix2,
                 Matrix<T, _rows3, _cols3> &matrix)
{

    size_t rows = matrix1.rows();
    size_t dim = matrix1.cols();
    size_t cols = matrix2.cols();

    Packed<T> packed_a;
    Packed<T> packed_b;
    Packed<T> packed_c;
    Packed<T> packed_a1;
    Packed<T> packed_a2;
    Packed<T> packed_a3;
    Packed<T> packed_a4;
    Packed<T> packed_a5;
    Packed<T> packed_a6;
    Packed<T> packed_a7;
    Packed<T> packed_a8;

    constexpr size_t packed_size = packed_a.size();
    size_t max_vector = cols - cols % packed_size;
    size_t iter = rows - rows % 8;

    T b{};

    for (size_t r = 0; r < iter; r += 8) {
        for (size_t i = 0; i < dim; i++) {

            packed_a1.setScalar(matrix1(r, i));
            packed_a2.setScalar(matrix1(r + 1, i));
            packed_a3.setScalar(matrix1(r + 2, i));
            packed_a4.setScalar(matrix1(r + 3, i));
            packed_a5.setScalar(matrix1(r + 4, i));
            packed_a6.setScalar(matrix1(r + 5, i));
            packed_a7.setScalar(matrix1(r + 6, i));
            packed_a8.setScalar(matrix1(r + 7, i));

            for (size_t c = 0; c < max_vector; c += packed_size) {

                packed_b.loadAligned(&matrix2(i, c));

                packed_c.loadAligned(&matrix(r, c));
                packed_c += packed_a1 * packed_b;
                packed_c.storeAligned(&matrix(r, c));

                packed_c.loadAligned(&matrix(r + 1, c));
                packed_c += packed_a2 * packed_b;
                packed_c.storeAligned(&matrix(r + 1, c));

                packed_c.loadAligned(&matrix(r + 2, c));
                packed_c += packed_a3 * packed_b;
                packed_c.storeAligned(&matrix(r + 2, c));

                packed_c.loadAligned(&matrix(r + 3, c));
                packed_c += packed_a4 * packed_b;
                packed_c.storeAligned(&matrix(r + 3, c));

                packed_c.loadAligned(&matrix(r + 4, c));
                packed_c += packed_a5 * packed_b;
                packed_c.storeAligned(&matrix(r + 4, c));

                packed_c.loadAligned(&matrix(r + 5, c));
                packed_c += packed_a6 * packed_b;
                packed_c.storeAligned(&matrix(r + 5, c));

                packed_c.loadAligned(&matrix(r + 6, c));
                packed_c += packed_a7 * packed_b;
                packed_c.storeAligned(&matrix(r + 6, c));

                packed_c.loadAligned(&matrix(r + 7, c));
                packed_c += packed_a8 * packed_b;
                packed_c.storeAligned(&matrix(r + 7, c));

            }

            for (size_t c = max_vector; c < cols; c++) {

                b = matrix2(i, c);
                matrix(r, c) += matrix1(r, i) * b;
                matrix(r + 1, c) += matrix1(r + 1, i) * b;
                matrix(r + 2, c) += matrix1(r + 2, i) * b;
                matrix(r + 3, c) += matrix1(r + 3, i) * b;
                matrix(r + 4, c) += matrix1(r + 4, i) * b;
                matrix(r + 5, c) += matrix1(r + 5, i) * b;
                matrix(r + 6, c) += matrix1(r + 6, i) * b;
                matrix(r + 7, c) += matrix1(r + 7, i) * b;
            }

        }
    }

    for (size_t r = iter; r < rows; r++) {
        for (size_t i = 0; i < dim; i++) {

            T a = matrix1(r, i);
            packed_a.setScalar(a);

            for (size_t c = 0; c < max_vector; c += packed_size) {

                packed_b.loadAligned(&matrix2(i, c));

                packed_c.loadAligned(&matrix(r, c));
                packed_c += packed_a * packed_b;
                packed_c.storeAligned(&matrix(r, c));
            }

            for (size_t c = max_vector; c < cols; c++) {
                matrix(r, c) += a * matrix2(i, c);
            }

        }
    }
}

template<typename T, size_t _rows1, size_t _col1, size_t _rows2, size_t _cols2, size_t _rows3, size_t _cols3>
void mulmat_simd_parallel(const Matrix<T, _rows1, _col1> &matrix1,
                          const Matrix<T, _rows2, _cols2> &matrix2,
                          Matrix<T, _rows3, _cols3> &matrix)
{
    size_t rows = matrix1.rows();
    size_t dim = matrix1.cols();
    size_t cols = matrix2.cols();

    Packed<T> packed_a;
    Packed<T> packed_b;
    Packed<T> packed_c;
    Packed<T> packed_a1, packed_a2, packed_a3, packed_a4;
    Packed<T> packed_a5, packed_a6, packed_a7, packed_a8;

    constexpr size_t packed_size = packed_a.size();
    size_t max_vector = cols - cols % packed_size;
    size_t iter = rows - rows % 8;

    T b{};

    // Paralelizamos el bucle externo
    #pragma omp parallel for collapse(2) private(packed_a1, packed_a2, packed_a3, packed_a4, \
                                                 packed_a5, packed_a6, packed_a7, packed_a8, \
                                                 packed_a, packed_b, packed_c, b)
    for (int r = 0; r < iter; r += 8) {
        for (int i = 0; i < dim; i++) {

            packed_a1.setScalar(matrix1(r, i));
            packed_a2.setScalar(matrix1(r + 1, i));
            packed_a3.setScalar(matrix1(r + 2, i));
            packed_a4.setScalar(matrix1(r + 3, i));
            packed_a5.setScalar(matrix1(r + 4, i));
            packed_a6.setScalar(matrix1(r + 5, i));
            packed_a7.setScalar(matrix1(r + 6, i));
            packed_a8.setScalar(matrix1(r + 7, i));

            for (int c = 0; c < max_vector; c += packed_size) {
                packed_b.loadAligned(&matrix2(i, c));

                packed_c.loadAligned(&matrix(r, c));
                packed_c += packed_a1 * packed_b;
                packed_c.storeAligned(&matrix(r, c));

                packed_c.loadAligned(&matrix(r + 1, c));
                packed_c += packed_a2 * packed_b;
                packed_c.storeAligned(&matrix(r + 1, c));

                packed_c.loadAligned(&matrix(r + 2, c));
                packed_c += packed_a3 * packed_b;
                packed_c.storeAligned(&matrix(r + 2, c));

                packed_c.loadAligned(&matrix(r + 3, c));
                packed_c += packed_a4 * packed_b;
                packed_c.storeAligned(&matrix(r + 3, c));

                packed_c.loadAligned(&matrix(r + 4, c));
                packed_c += packed_a5 * packed_b;
                packed_c.storeAligned(&matrix(r + 4, c));

                packed_c.loadAligned(&matrix(r + 5, c));
                packed_c += packed_a6 * packed_b;
                packed_c.storeAligned(&matrix(r + 5, c));

                packed_c.loadAligned(&matrix(r + 6, c));
                packed_c += packed_a7 * packed_b;
                packed_c.storeAligned(&matrix(r + 6, c));

                packed_c.loadAligned(&matrix(r + 7, c));
                packed_c += packed_a8 * packed_b;
                packed_c.storeAligned(&matrix(r + 7, c));
            }

            for (size_t c = max_vector; c < cols; c++) {
                b = matrix2(i, c);
                matrix(r, c) += matrix1(r, i) * b;
                matrix(r + 1, c) += matrix1(r + 1, i) * b;
                matrix(r + 2, c) += matrix1(r + 2, i) * b;
                matrix(r + 3, c) += matrix1(r + 3, i) * b;
                matrix(r + 4, c) += matrix1(r + 4, i) * b;
                matrix(r + 5, c) += matrix1(r + 5, i) * b;
                matrix(r + 6, c) += matrix1(r + 6, i) * b;
                matrix(r + 7, c) += matrix1(r + 7, i) * b;
            }
        }
    }

    #pragma omp parallel for private(packed_a, packed_b, packed_c, b)
    for (int r = iter; r < rows; r++) {
        for (int i = 0; i < dim; i++) {

            T a = matrix1(r, i);
            packed_a.setScalar(a);

            for (int c = 0; c < max_vector; c += packed_size) {
                packed_b.loadAligned(&matrix2(i, c));
                packed_c.loadAligned(&matrix(r, c));
                packed_c += packed_a * packed_b;
                packed_c.storeAligned(&matrix(r, c));
            }

            for (int c = max_vector; c < cols; c++) {
                matrix(r, c) += a * matrix2(i, c);
            }
        }
    }
}

//template<typename T, size_t Rows, size_t _dim, size_t Cols>
//void mulmat_simd_parallel(const Matrix<T, Rows, _dim> &matrix1,
//                          const Matrix<T, _dim, Cols> &matrix2,
//                          Matrix<T, Rows, Cols> &matrix)
//{
//
//    size_t rows = matrix1.rows();
//    size_t dim = matrix1.cols();
//    size_t cols = matrix2.cols();
//
//    Packed<T> packed_a;
//    Packed<T> packed_b;
//    Packed<T> packed_c;
//    Packed<T> packed_a1;
//    Packed<T> packed_a2;
//    Packed<T> packed_a3;
//    Packed<T> packed_a4;
//    Packed<T> packed_a5;
//    Packed<T> packed_a6;
//    Packed<T> packed_a7;
//    Packed<T> packed_a8;
//
//    constexpr size_t packed_size = packed_a.size();
//    size_t max_vector = cols - cols % packed_size;
//    size_t iter = rows - rows % 8;
//
//    auto f_aux = [&](size_t ini, size_t end) {
//
//        T b{};
//
//        for (size_t r = ini; r < end; r += 8) {
//            for (size_t i = 0; i < dim; i++) {
//
//                packed_a1.setScalar(matrix1(r, i));
//                packed_a2.setScalar(matrix1(r + 1, i));
//                packed_a3.setScalar(matrix1(r + 2, i));
//                packed_a4.setScalar(matrix1(r + 3, i));
//                packed_a5.setScalar(matrix1(r + 4, i));
//                packed_a6.setScalar(matrix1(r + 5, i));
//                packed_a7.setScalar(matrix1(r + 6, i));
//                packed_a8.setScalar(matrix1(r + 7, i));
//
//                for (size_t c = 0; c < max_vector; c += packed_size) {
//
//                    packed_b.loadAligned(&matrix2(i, c));
//
//                    packed_c.loadAligned(&matrix(r, c));
//                    packed_c += packed_a1 * packed_b;
//                    packed_c.storeAligned(&matrix(r, c));
//
//                    packed_c.loadAligned(&matrix(r + 1, c));
//                    packed_c += packed_a2 * packed_b;
//                    packed_c.storeAligned(&matrix(r + 1, c));
//
//                    packed_c.loadAligned(&matrix(r + 2, c));
//                    packed_c += packed_a3 * packed_b;
//                    packed_c.storeAligned(&matrix(r + 2, c));
//
//                    packed_c.loadAligned(&matrix(r + 3, c));
//                    packed_c += packed_a4 * packed_b;
//                    packed_c.storeAligned(&matrix(r + 3, c));
//
//                    packed_c.loadAligned(&matrix(r + 4, c));
//                    packed_c += packed_a5 * packed_b;
//                    packed_c.storeAligned(&matrix(r + 4, c));
//
//                    packed_c.loadAligned(&matrix(r + 5, c));
//                    packed_c += packed_a6 * packed_b;
//                    packed_c.storeAligned(&matrix(r + 5, c));
//
//                    packed_c.loadAligned(&matrix(r + 6, c));
//                    packed_c += packed_a7 * packed_b;
//                    packed_c.storeAligned(&matrix(r + 6, c));
//
//                    packed_c.loadAligned(&matrix(r + 7, c));
//                    packed_c += packed_a8 * packed_b;
//                    packed_c.storeAligned(&matrix(r + 7, c));
//
//                }
//
//                for (size_t c = max_vector; c < cols; c++) {
//
//                    b = matrix2(i, c);
//                    matrix(r, c) += matrix1(r, i) * b;
//                    matrix(r + 1, c) += matrix1(r + 1, i) * b;
//                    matrix(r + 2, c) += matrix1(r + 2, i) * b;
//                    matrix(r + 3, c) += matrix1(r + 3, i) * b;
//                    matrix(r + 4, c) += matrix1(r + 4, i) * b;
//                    matrix(r + 5, c) += matrix1(r + 5, i) * b;
//                    matrix(r + 6, c) += matrix1(r + 6, i) * b;
//                    matrix(r + 7, c) += matrix1(r + 7, i) * b;
//                }
//
//            }
//        }
//    };
//
//    /// iter es el número ideal de hilos
//    size_t one_thread_per_block = iter / 8;
//    size_t num_threads = optimalNumberOfThreads();
//    size_t block_size = one_thread_per_block / num_threads;
//    if (block_size > 0) {
//        block_size *= 8;
//    } else {
//        num_threads = one_thread_per_block;
//    }
//
//    std::vector<std::thread> threads(num_threads);
//
//    // El tamaño del bloque tiene que ser siempre multiplo de 8
//
//    size_t block_ini = 0;
//    size_t block_end = 0;
//
//    for (size_t i = 0; i < num_threads; i++) {
//
//        if (i == num_threads - 1) block_end = iter;
//        else block_end = block_ini + block_size;
//
//        threads[i] = std::thread(f_aux, block_ini, block_end);
//
//        block_ini = block_end;
//    }
//
//    /// Lo puedo sacar fuera e incluir en otro hilo el proceso sin SIMD
//    for (auto &_thread : threads) {
//        if (_thread.joinable())
//            _thread.join();
//    }
//
//    //T b{};
//
//    //for (size_t r = 0; r < iter; r += 8) {
//    //  for (size_t i = 0; i < dim; i++) {
//
//    //    packed_a1.setScalar(matrix1(r, i));
//    //    packed_a2.setScalar(matrix1(r + 1, i));
//    //    packed_a3.setScalar(matrix1(r + 2, i));
//    //    packed_a4.setScalar(matrix1(r + 3, i));
//    //    packed_a5.setScalar(matrix1(r + 4, i));
//    //    packed_a6.setScalar(matrix1(r + 5, i));
//    //    packed_a7.setScalar(matrix1(r + 6, i));
//    //    packed_a8.setScalar(matrix1(r + 7, i));
//
//    //    for (size_t c = 0; c < max_vector; c += packed_size) {
//
//    //      packed_b.loadAligned(&matrix2(i, c));
//
//    //      packed_c.loadAligned(&matrix(r, c));
//    //      packed_c += packed_a1 * packed_b;
//    //      packed_c.storeAligned(&matrix(r, c));
//
//    //      packed_c.loadAligned(&matrix(r + 1, c));
//    //      packed_c += packed_a2 * packed_b;
//    //      packed_c.storeAligned(&matrix(r + 1, c));
//
//    //      packed_c.loadAligned(&matrix(r + 2, c));
//    //      packed_c += packed_a3 * packed_b;
//    //      packed_c.storeAligned(&matrix(r + 2, c));
//
//    //      packed_c.loadAligned(&matrix(r + 3, c));
//    //      packed_c += packed_a4 * packed_b;
//    //      packed_c.storeAligned(&matrix(r + 3, c));
//
//    //      packed_c.loadAligned(&matrix(r + 4, c));
//    //      packed_c += packed_a5 * packed_b;
//    //      packed_c.storeAligned(&matrix(r + 4, c));
//
//    //      packed_c.loadAligned(&matrix(r + 5, c));
//    //      packed_c += packed_a6 * packed_b;
//    //      packed_c.storeAligned(&matrix(r + 5, c));
//
//    //      packed_c.loadAligned(&matrix(r + 6, c));
//    //      packed_c += packed_a7 * packed_b;
//    //      packed_c.storeAligned(&matrix(r + 6, c));
//
//    //      packed_c.loadAligned(&matrix(r + 7, c));
//    //      packed_c += packed_a8 * packed_b;
//    //      packed_c.storeAligned(&matrix(r + 7, c));
//
//    //    }
//
//    //    for (size_t c = max_vector; c < cols; c++) {
//
//    //      b = matrix2(i, c);
//    //      matrix(r, c) += matrix1(r, i) * b;
//    //      matrix(r + 1, c) += matrix1(r + 1, i) * b;
//    //      matrix(r + 2, c) += matrix1(r + 2, i) * b;
//    //      matrix(r + 3, c) += matrix1(r + 3, i) * b;
//    //      matrix(r + 4, c) += matrix1(r + 4, i) * b;
//    //      matrix(r + 5, c) += matrix1(r + 5, i) * b;
//    //      matrix(r + 6, c) += matrix1(r + 6, i) * b;
//    //      matrix(r + 7, c) += matrix1(r + 7, i) * b;
//    //    }
//
//    //  }
//    //}
//
//    for (size_t r = iter; r < rows; r++) {
//        for (size_t i = 0; i < dim; i++) {
//
//            T a = matrix1(r, i);
//            packed_a.setScalar(a);
//
//            for (size_t c = 0; c < max_vector; c += packed_size) {
//
//                packed_b.loadAligned(&matrix2(i, c));
//
//                packed_c.loadAligned(&matrix(r, c));
//                packed_c += packed_a * packed_b;
//                packed_c.storeAligned(&matrix(r, c));
//            }
//
//            for (size_t c = max_vector; c < cols; c++) {
//                matrix(r, c) += a * matrix2(i, c);
//            }
//
//        }
//    }
//}

#endif // TL_HAVE_SIMD_INTRINSICS

template<typename T, size_t _rows1, size_t _col1, size_t _rows2, size_t _cols2, size_t _rows3, size_t _cols3>
void mulmat_cpp(const Matrix<T, _rows1, _col1> &matrix1,
                const Matrix<T, _rows2, _cols2> &matrix2,
                Matrix<T, _rows3, _cols3> &matrix)
{
    for (size_t r = 0; r < matrix1.rows(); r++) {
        for (size_t i = 0; i < matrix1.cols(); i++) {
            T a = matrix1(r, i);
            for (size_t c = 0; c < matrix2.cols(); c++) {
                matrix(r, c) += a * matrix2(i, c);
            }
        }
    }
}

template<typename T, size_t _rows1, size_t _col1, size_t _rows2, size_t _cols2, size_t _rows3, size_t _cols3>
auto mulmat(const Matrix<T, _rows1, _col1> &matrix1,
            const Matrix<T, _rows2, _cols2> &matrix2,
            Matrix<T, _rows3, _cols3> &matrix) -> std::enable_if_t<std::is_integral<T>::value, void>
{
//#if defined TL_HAVE_SIMD_INTRINSICS
//
//    mulmat_simd(matrix1, matrix2, matrix);
//    //mulmat_simd_parallel(matrix1, matrix2, matrix);
//
//#else 
//
//    mulmat_cpp(matrix1, matrix2, matrix);
//
//#endif
    TL_ASSERT(matrix1.cols() == matrix2.rows(), "A columns != B rows");
    TL_ASSERT(matrix1.rows() == matrix.rows(), "C rows != A rows");
    TL_ASSERT(matrix2.cols() == matrix.cols(), "B columns != C columns");

    switch (MatrixConfig::instance().product) {
#ifdef TL_HAVE_SIMD_INTRINSICS
    case tl::MatrixConfig::Product::SIMD:
        mulmat_simd(matrix1, matrix2, matrix);
        //mulmat_simd_parallel(matrix1, matrix2, matrix);
        break;
#endif
    case tl::MatrixConfig::Product::CPP:
    default:
        mulmat_cpp(matrix1, matrix2, matrix);
        break;
    }
}

template<typename T, size_t _rows1, size_t _cols1, size_t _rows2, size_t _cols2, size_t _rows3, size_t _cols3>
auto mulmat(const Matrix<T, _rows1, _cols1> &matrix1,
            const Matrix<T, _rows2, _cols2> &matrix2,
            Matrix<T, _rows3, _cols3> &matrix) -> std::enable_if_t<std::is_floating_point<T>::value, void>
{

    switch (MatrixConfig::instance().product) {
#ifdef TL_HAVE_CUDA
    case tl::MatrixConfig::Product::CuBLAS:
        cuda::gemm(matrix1.rows(),
                   matrix2.cols(),
                   matrix1.cols(),
                   matrix1.data(), 
                   matrix2.data(), 
                   matrix.data());
        break;
#endif
#ifdef TL_HAVE_OPENBLAS
    case tl::MatrixConfig::Product::BLAS:
        blas::gemm(matrix1.rows(),
                   matrix2.cols(),
                   matrix1.cols(),
                   matrix1.data(), 
                   matrix2.data(), 
                   matrix.data());
        break;
#endif
#ifdef TL_HAVE_SIMD_INTRINSICS
    case tl::MatrixConfig::Product::SIMD:
        mulmat_simd(matrix1, matrix2, matrix);
        break;
#endif
    case tl::MatrixConfig::Product::CPP:
        mulmat_cpp(matrix1, matrix2, matrix);
        break;
    }
}


#ifdef TL_HAVE_SIMD_INTRINSICS
template<typename T, size_t Rows, size_t Cols, size_t _size>
void matrix_per_vector_simd(const Matrix<T, Rows, Cols> &matrix,
                            const Vector<T, _size> &vector,
                            Vector<T, Rows> &vectorOut)
{
    size_t rows = matrix.rows();
    size_t cols = matrix.cols();

    Packed<T> packed_a;
    Packed<T> packed_b;
    Packed<T> packed_c;

    constexpr size_t packed_size = packed_a.size();
    size_t max_vector = cols - cols % packed_size;

    for(size_t r = 0; r < rows; r++) {
        for(size_t i = 0; i < max_vector; i += packed_size) {

            packed_a.loadAligned(&vector[i]);
            packed_b.loadAligned(&matrix(r, i));
            packed_c = packed_a * packed_b;
            vectorOut[r] += packed_c.sum();
        }

        for(size_t i = max_vector; i < cols; i++) {
            vectorOut[r] += matrix(r, i) * vector[i];
        }
    }
}
#endif // TL_HAVE_SIMD_INTRINSICS

template<typename T, size_t Rows, size_t Cols, size_t _size>
void matrix_per_vector_cpp(const Matrix<T, Rows, Cols> &matrix, 
                           const Vector<T, _size> &vector, 
                           Vector<T, Rows> &vectorOut)
{
    size_t rows = matrix.rows();
    size_t cols = matrix.cols();

    for(size_t r = 0; r < rows; r++) {
        for(size_t c = 0; c < cols; c++) {
            vectorOut[r] += matrix(r, c) * vector[c];
        }
    }
}


template<typename T, size_t Rows, size_t Cols, size_t _size>
auto matrix_per_vector(const Matrix<T, Rows, Cols> &matrix, 
                       const Vector<T, _size> &vector, 
                       Vector<T, Rows> &vectorOut) -> std::enable_if_t<std::is_integral<T>::value, void>
{
    TL_ASSERT(matrix.cols() == vector.size(), "Matrix columns != Vector size");

//#ifndef TL_HAVE_SIMD_INTRINSICS
//
//    matrix_per_vector_cpp(matrix, vector, vectorOut);
//
//#else
//
//    matrix_per_vector_simd(matrix, vector, vectorOut);
//
//#endif

    switch (MatrixConfig::instance().product) {
#ifdef TL_HAVE_SIMD_INTRINSICS
    case tl::MatrixConfig::Product::SIMD:
        matrix_per_vector_simd(matrix, vector, vectorOut);
        break;
#endif
    case tl::MatrixConfig::Product::CPP:
        //matrix_per_vector_cpp(matrix, vector, vectorOut);
        //break;
    default:
        matrix_per_vector_cpp(matrix, vector, vectorOut);
        break;
    }
}


template<typename T, size_t Rows, size_t Cols, size_t _size>
auto matrix_per_vector(const Matrix<T, Rows, Cols> &matrix, 
                       const Vector<T, _size> &vector, 
                       Vector<T, Rows> &vectorOut) -> std::enable_if_t<std::is_floating_point<T>::value, void>
{       
    TL_ASSERT(matrix.cols() == vector.size(), "Matrix columns != Vector size");

    switch (MatrixConfig::instance().product) {
#ifdef TL_HAVE_CUDA
    case tl::MatrixConfig::Product::CuBLAS:
        cuda::gemv(matrix.rows(),
                   matrix.cols(),
                   matrix.data(), 
                   vector.data(), 
                   vectorOut.data());
        break;
#endif
#ifdef TL_HAVE_OPENBLAS
    case tl::MatrixConfig::Product::BLAS:
        blas::gemv(matrix.rows(),
                   matrix.cols(),
                   matrix.data(), 
                   vector.data(), 
                   vectorOut.data());
        break;
#endif
#ifdef TL_HAVE_SIMD_INTRINSICS
    case tl::MatrixConfig::Product::SIMD:
        matrix_per_vector_simd(matrix, vector, vectorOut);
        break;
#endif
    case tl::MatrixConfig::Product::CPP:
        //matrix_per_vector_cpp(matrix, vector, vectorOut);
        //break;
    default:
        matrix_per_vector_cpp(matrix, vector, vectorOut);
        break;
    }
}


} // namespace internal 

/// \endcond



/*------------------------------------------------------------------------*/
/* Matrix implementation                                                  */
/*------------------------------------------------------------------------*/


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
  : mData(Data<T, data::size>(rows * cols, value)),
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
template<typename MatrixDerived>
Matrix<T, Rows, Cols>::Matrix(const MatrixDerived &matrix)
{
    if(Rows == DynamicData && Cols == DynamicData) {
        mData = Data<T, data::size>(matrix.rows() * matrix.cols());
        mRows = matrix.rows();
        mCols = matrix.cols();
    }

    TL_ASSERT(matrix.rows() == this->rows() &&
              matrix.cols() == this->cols(), "Static matrix cannot be resized");

    MatrixBase<Matrix<T, Rows, Cols>>::set(matrix);
}

template<typename T, size_t Rows, size_t Cols>
Matrix<T, Rows, Cols>::Matrix(Matrix &&mat) TL_NOEXCEPT
  : mData(std::move(mat.mData)),
    mRows(std::move(mat.mRows)),
    mCols(std::move(mat.mCols))
{
}

template<typename T, size_t Rows, size_t Cols>
Matrix<T, Rows, Cols>::Matrix(std::initializer_list<T> values)
{
    if(Rows == DynamicData && Cols == DynamicData) {
        this->mRows = 1;
        this->mCols = values.size();
        mData = Data<T, data::size>(values.size());
        std::copy(values.begin(), values.end(), mData.begin());

    } else {

        mData = Data<T, data::size>();
        this->mRows = Rows;
        this->mCols = Cols;

        size_t n = values.size();
        if(n == data::size) {
            std::copy(values.begin(), values.end(), mData.begin());
        } else if(n < data::size) {
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
    if(Rows == DynamicData && Cols == DynamicData) {

        this->mRows = values.size();
        auto it = values.begin();
        this->mCols = it->size();
        mData = Data<T, data::size>(this->mRows * this->mCols);

        auto it_data = mData.begin();
        for(auto it = values.begin(); it != values.end(); it++) {
            size_t n = it->size();
            if(n == mCols) {
                std::copy(it->begin(), it->end(), it_data);
            } else if(n < mCols) {
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
        for(auto it = values.begin(); it != values.end(); it++) {
            if(rows_counter < Rows) {
                size_t n = it->size();
                if(n == Cols) {
                    std::copy(it->begin(), it->end(), it_data);
                } else if(n < Cols) {
                    std::copy(it->begin(), it->end(), it_data);
                    std::fill(it_data + n, mData.end(), consts::zero<T>);
                } else {
                    std::copy(it->begin(), it->end() + Cols, it_data);
                }

                it_data += Cols;
                rows_counter++;
            }
        }

        if(it_data != mData.end()) {
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
auto Matrix<T, Rows, Cols>::operator = (const Matrix &matrix) -> Matrix&
{
    if(this != &matrix) {
        this->mData = matrix.mData;
        this->mRows = matrix.mRows;
        this->mCols = matrix.mCols;
    }

    return *this;
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::operator = (Matrix &&matrix) TL_NOEXCEPT -> Matrix &
{
    if(this != &matrix) {
        this->mData = std::move(matrix.mData);
        this->mRows = std::move(matrix.mRows);
        this->mCols = std::move(matrix.mCols);
    }

    return *this;
}

template<typename T, size_t Rows, size_t Cols>
Matrix<T, Rows, Cols>::operator Matrix<T, DynamicData, DynamicData>()
{
    Matrix<T, DynamicData, DynamicData> matrix(this->data(), this->rows(), this->cols());
    return matrix;
}

template <typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::rows() const TL_NOEXCEPT -> size_t
{
    return mRows;
}

template <typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::cols() const TL_NOEXCEPT -> size_t
{
    return mCols;
}


template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::inverse(bool *invertibility) const -> Matrix
{
    static_assert(Rows == Cols, "Non-Square Matrix");
    static_assert(std::is_floating_point<T>::value, "Integral type not supported");

    size_t rows = this->rows();
    size_t cols = this->cols();
    TL_ASSERT(rows == cols, "Non-Square Matrix");

    Matrix<T, Rows, Cols> matrix;

    if(rows == 2)
        matrix = inverse2x2(invertibility);
    else if(rows == 3)
        matrix = inverse3x3(invertibility);
    else if(rows == 4)
        matrix = inverse4x4(invertibility);
    else
        matrix = inversenxn(invertibility);

    return matrix;
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::inverse2x2(bool *invertibility) const -> Matrix
{
    Matrix<T, Rows, Cols> matrix(*this);

    T det = this->determinant2x2();
    if(det != consts::zero<T>) {
        matrix[0][0] = (*this)(1, 1) / det;
        matrix[0][1] = -(*this)(0, 1) / det;
        matrix[1][0] = -(*this)(1, 0) / det;
        matrix[1][1] = (*this)(0, 0) / det;
        if(invertibility) *invertibility = true;
    } else {
        if(invertibility) *invertibility = false;
    }

    return matrix;
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::inverse3x3(bool *invertibility) const -> Matrix
{
    Matrix<T, Rows, Cols> matrix(*this);

    Matrix<T, Rows, Cols> adj = this->adjoint3x3();
    T det = (*this)(0, 0) * adj(0, 0)
        + (*this)(0, 1) * adj(1, 0)
        + (*this)(0, 2) * adj(2, 0);

    if(det != consts::zero<T>) {

        matrix = adj / det;

        if(invertibility) *invertibility = true;
    } else {
        if(invertibility) *invertibility = false;
    }

    return matrix;
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::inverse4x4(bool *invertibility) const -> Matrix
{
    Matrix<T, Rows, Cols> matrix(*this);

    T m00 = (*this)(0, 0);
    T m01 = (*this)(0, 1);
    T m02 = (*this)(0, 2);
    T m03 = (*this)(0, 3);
    T m10 = (*this)(1, 0);
    T m11 = (*this)(1, 1);
    T m12 = (*this)(1, 2);
    T m13 = (*this)(1, 3);
    T m20 = (*this)(2, 0);
    T m21 = (*this)(2, 1);
    T m22 = (*this)(2, 2);
    T m23 = (*this)(2, 3);
    T m30 = (*this)(3, 0);
    T m31 = (*this)(3, 1);
    T m32 = (*this)(3, 2);
    T m33 = (*this)(3, 3);

    T a0 = m00 * m11 - m01 * m10;
    T a1 = m00 * m12 - m02 * m10;
    T a2 = m00 * m13 - m03 * m10;
    T a3 = m01 * m12 - m02 * m11;
    T a4 = m01 * m13 - m03 * m11;
    T a5 = m02 * m13 - m03 * m12;
    T b0 = m20 * m31 - m21 * m30;
    T b1 = m20 * m32 - m22 * m30;
    T b2 = m20 * m33 - m23 * m30;
    T b3 = m21 * m32 - m22 * m31;
    T b4 = m21 * m33 - m23 * m31;
    T b5 = m22 * m33 - m23 * m32;
    T det = a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;

    if(det != consts::zero<T>) {

        matrix[0][0] = (m11 * b5 - m12 * b4 + m13 * b3) / det;
        matrix[0][1] = (-m01 * b5 + m02 * b4 - m03 * b3) / det;
        matrix[0][2] = (m31 * a5 - m32 * a4 + m33 * a3) / det;
        matrix[0][3] = (-m21 * a5 + m22 * a4 - m23 * a3) / det;
        matrix[1][0] = (-m10 * b5 + m12 * b2 - m13 * b1) / det;
        matrix[1][1] = (m00 * b5 - m02 * b2 + m03 * b1) / det;
        matrix[1][2] = (-m30 * a5 + m32 * a2 - m33 * a1) / det;
        matrix[1][3] = (m20 * a5 - m22 * a2 + m23 * a1) / det;
        matrix[2][0] = (m10 * b4 - m11 * b2 + m13 * b0) / det;
        matrix[2][1] = (-m00 * b4 + m01 * b2 - m03 * b0) / det;
        matrix[2][2] = (m30 * a4 - m31 * a2 + m33 * a0) / det;
        matrix[2][3] = (-m20 * a4 + m21 * a2 - m23 * a0) / det;
        matrix[3][0] = (-m10 * b3 + m11 * b1 - m12 * b0) / det;
        matrix[3][1] = (m00 * b3 - m01 * b1 + m02 * b0) / det;
        matrix[3][2] = (-m30 * a3 + m31 * a1 - m32 * a0) / det;
        matrix[3][3] = (m20 * a3 - m21 * a1 + m22 * a0) / det;

        if(invertibility) *invertibility = true;

    } else if(invertibility) {
        *invertibility = false;
    }

    return matrix;
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::inversenxn(bool *invertibility) const -> Matrix
{
    Matrix<T, Rows, Cols> matrix(*this);

    LuDecomposition<Matrix<T, Rows, Cols>> lu(*this);

    T det = lu.determinant();//this->determinantnxn();
    if(det != consts::zero<T>) {

        //  matrix = this->adjugate();
        //  matrix /= det;

        Matrix<T, Rows, Cols> indentity(this->rows(), this->cols(), 0);
        for(size_t r = 0; r < this->rows(); r++)
            indentity(r, r) = consts::one<T>;
        matrix = lu.solve(indentity);

        if(invertibility != nullptr) *invertibility = true;

    } else if(invertibility != nullptr) {
        *invertibility = false;
    }

    return matrix;
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::transpose() const -> Matrix<T, Cols, Rows>
{
    size_t rows = this->rows();
    size_t cols = this->cols();

    Matrix<T, Cols, Rows> matrix(cols, rows);

    for(size_t r = 0; r < rows; r++) {
        for(size_t c = 0; c < cols; c++) {
            matrix[c][r] = (*this)(r, c);
        }
    }

#if defined TL_HAVE_SIMD_INTRINSICS



#endif

    return matrix;
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::adjugate() const -> Matrix
{
    static_assert(Rows == Cols, "Non-Square Matrix");
    size_t rows = this->rows();
    size_t cols = this->cols();
    TL_ASSERT(rows == cols, "Non-Square Matrix");

    Matrix<T, Rows, Cols> matrix(*this);

    if(rows == 2)
        matrix = adjoint2x2();
    else if(rows == 3)
        matrix = adjoint3x3();
    else if(rows == 4)
        matrix = adjoint4x4();
    else
        matrix = adjointnxn();

    return matrix;
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::adjoint2x2() const -> Matrix
{
    Matrix<T, Rows, Cols> matrix(*this);

    std::swap(matrix(0, 0), matrix(1, 1));
    matrix(0, 1) = -matrix(0, 1);
    matrix(1, 0) = -matrix(1, 0);

    return matrix;
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::adjoint3x3() const -> Matrix
{
    Matrix<T, Rows, Cols> matrix(*this);

    T m00 = (*this)(0, 0);
    T m01 = (*this)(0, 1);
    T m02 = (*this)(0, 2);
    T m10 = (*this)(1, 0);
    T m11 = (*this)(1, 1);
    T m12 = (*this)(1, 2);
    T m20 = (*this)(2, 0);
    T m21 = (*this)(2, 1);
    T m22 = (*this)(2, 2);

    matrix(0, 0) = m11 * m22 - m12 * m21;
    matrix(0, 1) = m02 * m21 - m01 * m22;
    matrix(0, 2) = m01 * m12 - m02 * m11;
    matrix(1, 0) = m12 * m20 - m10 * m22;
    matrix(1, 1) = m00 * m22 - m02 * m20;
    matrix(1, 2) = m02 * m10 - m00 * m12;
    matrix(2, 0) = m10 * m21 - m11 * m20;
    matrix(2, 1) = m01 * m20 - m00 * m21;
    matrix(2, 2) = m00 * m11 - m01 * m10;

    return matrix;
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::adjoint4x4() const -> Matrix
{
    Matrix<T, Rows, Cols> matrix(*this);

    T m00 = (*this)(0, 0);
    T m01 = (*this)(0, 1);
    T m02 = (*this)(0, 2);
    T m03 = (*this)(0, 3);
    T m10 = (*this)(1, 0);
    T m11 = (*this)(1, 1);
    T m12 = (*this)(1, 2);
    T m13 = (*this)(1, 3);
    T m20 = (*this)(2, 0);
    T m21 = (*this)(2, 1);
    T m22 = (*this)(2, 2);
    T m23 = (*this)(2, 3);
    T m30 = (*this)(3, 0);
    T m31 = (*this)(3, 1);
    T m32 = (*this)(3, 2);
    T m33 = (*this)(3, 3);

    T a0 = m00 * m11 - m01 * m10;
    T a1 = m00 * m12 - m02 * m10;
    T a2 = m00 * m13 - m03 * m10;
    T a3 = m01 * m12 - m02 * m11;
    T a4 = m01 * m13 - m03 * m11;
    T a5 = m02 * m13 - m03 * m12;
    T b0 = m20 * m31 - m21 * m30;
    T b1 = m20 * m32 - m22 * m30;
    T b2 = m20 * m33 - m23 * m30;
    T b3 = m21 * m32 - m22 * m31;
    T b4 = m21 * m33 - m23 * m31;
    T b5 = m22 * m33 - m23 * m32;

    matrix(0, 0) = m11 * b5 - m12 * b4 + m13 * b3;
    matrix(0, 1) = -m01 * b5 + m02 * b4 - m03 * b3;
    matrix(0, 2) = m31 * a5 - m32 * a4 + m33 * a3;
    matrix(0, 3) = -m21 * a5 + m22 * a4 - m23 * a3;
    matrix(1, 0) = -m10 * b5 + m12 * b2 - m13 * b1;
    matrix(1, 1) = m00 * b5 - m02 * b2 + m03 * b1;
    matrix(1, 2) = -m30 * a5 + m32 * a2 - m33 * a1;
    matrix(1, 3) = m20 * a5 - m22 * a2 + m23 * a1;
    matrix(2, 0) = m10 * b4 - m11 * b2 + m13 * b0;
    matrix(2, 1) = -m00 * b4 + m01 * b2 - m03 * b0;
    matrix(2, 2) = m30 * a4 - m31 * a2 + m33 * a0;
    matrix(2, 3) = -m20 * a4 + m21 * a2 - m23 * a0;
    matrix(3, 0) = -m10 * b3 + m11 * b1 - m12 * b0;
    matrix(3, 1) = m00 * b3 - m01 * b1 + m02 * b0;
    matrix(3, 2) = -m30 * a3 + m31 * a1 - m32 * a0;
    matrix(3, 3) = m20 * a3 - m21 * a1 + m22 * a0;

    return matrix;
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::adjointnxn() const -> Matrix
{
    Matrix<T, Rows, Cols> matrix = this->cofactorMatrix();
    return matrix.transpose();
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::cofactorMatrix() const -> Matrix
{
    static_assert(Rows == Cols, "Non-Square Matrix");

    size_t rows = this->rows();
    size_t cols = this->cols();

    TL_ASSERT(rows == cols, "Non-Square Matrix");

    Matrix<T, Rows, Cols> matrix(*this);
    for(size_t r = 0; r < rows; r++) {
        for(size_t c = 0; c < cols; c++) {
            matrix[r][c] = cofactor(r, c);
        }
    }

    return matrix;
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::trace() const -> T
{
    static_assert(Rows == Cols, "Non-Square Matrix");

    size_t rows = this->rows();
    size_t cols = this->cols();

    TL_ASSERT(rows == cols, "Non-Square Matrix");

    T trace = consts::zero<T>;
    for(size_t r = 0; r < rows; r++) {
        trace += (*this)(r, r);
    }

    return trace;
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::invertible() -> bool
{
    T det = this->determinant();
    return (det != consts::zero<T>);
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::singular() -> bool
{
    T det = this->determinant();
    return (det == consts::zero<T>);
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::cofactor(size_t r, size_t c) const -> T
{
    static_assert(Rows == Cols, "Non-Square Matrix");
    size_t rows = this->rows();
    size_t cols = this->cols();
    TL_ASSERT(rows == cols, "Non-Square Matrix");

    T sign = ((r + c) % 2 == 0) ? consts::one<T> : -consts::one<T>;
    return sign * this->firstMinor(r, c);
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::firstMinor(size_t row, size_t col) const -> T
{
    static_assert(Rows == Cols, "Non-Square Matrix");
    size_t rows = this->rows();
    size_t cols = this->cols();
    TL_ASSERT(rows == cols, "Non-Square Matrix");


    size_t i = 0;
    size_t j = 0;

    Matrix<T> matrix(rows - 1, cols - 1);
    for(size_t r = 0; r < rows; r++) {
        for(size_t c = 0; c < cols; c++) {
            if(row != r && col != c) {
                matrix[i][j++] = (*this)(r, c);
                if(j == rows - 1) {
                    j = 0;
                    i++;
                }
            }
        }
    }

    return matrix.determinant();
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::rowEchelonForm() const -> Matrix
{
    size_t rows = this->rows();
    size_t cols = this->cols();

    Matrix<T, Rows, Cols> matrix(*this);

    for(size_t i = 0; i < rows; ++i) {

        T pivotElement{};
        size_t pivotRow{};
        size_t colPivotElement{};

        for(size_t c = i; c < cols; ++c) {

            pivotElement = matrix[i][c];
            pivotRow = i;

            for(size_t r = i + 1; r < rows; ++r) {
                if(std::abs(matrix[r][c]) > std::abs(pivotElement)) {
                    pivotElement = matrix[r][c];
                    pivotRow = r;
                }
            }

            if(pivotElement != consts::zero<T>) {
                colPivotElement = c;
                break;
            }
        }

        if(pivotRow != i) {
            matrix.swapRows(i, pivotRow);
        }

        if(pivotElement != consts::zero<T>) {
            matrix[i] /= pivotElement;

            for(size_t r = i + 1; r < rows; ++r) {
                T scale = matrix[r][colPivotElement];
                if(scale != consts::zero<T>) {
                    for(size_t c = i; c < cols; ++c) {
                        matrix[r][c] -= scale * matrix[i][c];
                    }
                }

            }
        }
    }

    return matrix;
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::reducedRowEchelonForm() const -> Matrix
{
    size_t rows = this->rows();
    size_t cols = this->cols();

    Matrix<T, Rows, Cols> matrix(*this);

    int lead = 0; // índice de la columna principal

    for(int r = 0; r < rows; r++) { // para cada fila
        if(lead >= cols) { // si hemos llegado al final de las columnas
            return matrix;
        }
        int i = r;
        while(abs(matrix[i][lead]) < std::numeric_limits<T>::epsilon()) { // si el valor es cercano a cero, buscar en la siguiente fila
            i++;
            if(i == rows) {
                i = r;
                lead++;
                if(lead == cols) { // si hemos llegado al final de las columnas
                    return matrix;
                }
            }
        }

        // intercambiar las filas
        if(r != i)
            matrix.swapRows(i, r);

        T lv = consts::one<T> / matrix[r][lead]; // elemento diagonal principal

        matrix[r] *= lv;

        for(int i = 0; i < rows; i++) { // para cada fila
            if(i != r) {
                T lv2 = matrix[i][lead];
                for(int c = 0; c < cols; c++) { // para cada columna
                    matrix[i][c] -= lv2 * matrix[r][c]; // restar la fila escalada
                }
            }
        }

        lead++; // avanzar a la siguiente columna principal
    }

    return matrix;
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::rank() const -> int
{
    auto matrix = rowEchelonForm();
    int rank{};

    for(size_t r = 0; r < matrix.rows(); r++) {
        for(size_t c = r; c < matrix.cols(); c++) {
            if(matrix[r][c] != 0) {
                rank++;
                break;
            }
        }
    }

    return rank;
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::swapRows(size_t i, size_t j) -> void
{
    for(size_t c = 0; c < mCols; c++) {
        std::swap(mData[i * mCols + c], mData[j * mCols + c]);
    }
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::at(size_t r, size_t c) -> reference
{
    return mData.at(r * mCols + c);
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::at(size_t r, size_t c) const -> const_reference
{
    return mData.at(r * mCols + c);
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::operator()(size_t r, size_t c) TL_NOEXCEPT -> reference
{
    return mData[r * mCols + c];
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::operator()(size_t r, size_t c) const TL_NOEXCEPT -> const_reference
{
    return mData[r * mCols + c];
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::operator()(size_t position) TL_NOEXCEPT -> reference
{
    return mData[position];
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::operator()(size_t position) const TL_NOEXCEPT -> const_reference
{
    return mData[position];
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::operator[](size_t position) const -> const internal::MatrixRow<const T>
{
    return internal::MatrixRow<const T>(this->data(), position, this->cols());
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::operator[](size_t position)  -> internal::MatrixRow<T>
{
    return internal::MatrixRow<T>(this->data(), position, this->cols());
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::row(size_t row) const -> const internal::MatrixRow<const T>
{
    return internal::MatrixRow<const T>(this->data(), row);
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::row(size_t row) -> internal::MatrixRow<T>
{
    return internal::MatrixRow<T>(this->data(), row, this->cols());
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::col(size_t col) const -> const internal::MatrixCol<const T>
{
    return internal::MatrixCol<const T>(this->data(), col, this->rows(), this->cols());
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::col(size_t col) -> internal::MatrixCol<T>
{
    return internal::MatrixCol<T>(this->data(), col, this->rows(), this->cols());
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::block(size_t iniRow, 
                                  size_t endRow,
                                  size_t iniCol, 
                                  size_t endCol) const -> const internal::MatrixBlock<const T>
{
    TL_ASSERT(iniRow >= 0 && endRow > iniRow && endRow < this->rows() &&
              iniCol >= 0 && endCol > iniCol && endCol < this->cols(), "Matrix block out of range");

    return internal::MatrixBlock<const T>(this->data(),
                                          this->rows(),
                                          this->cols(),
                                          iniRow,
                                          endRow,
                                          iniCol,
                                          endCol);
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::block(size_t iniRow, size_t endRow, size_t iniCol, size_t endCol) -> internal::MatrixBlock<T>
{
    TL_ASSERT(iniRow >= 0 && endRow >= iniRow && endRow < this->rows() &&
              iniCol >= 0 && endCol >= iniCol && endCol < this->cols(), "Matrix block out of range");

    return internal::MatrixBlock<T>(this->data(), this->rows(), this->cols(),
                                    iniRow, endRow, iniCol, endCol);
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::rowBlock(size_t iniRow, size_t endRow) const -> const internal::MatrixBlock<const T, DynamicData, DynamicData>
{
    return block(iniRow, endRow, 0, this->cols() - 1);
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::rowBlock(size_t iniRow, size_t endRow) -> internal::MatrixBlock<T, DynamicData, DynamicData>
{
    return block(iniRow, endRow, 0, this->cols() - 1);
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::colBlock(size_t iniCol, size_t endCol) const -> const internal::MatrixBlock<const T, DynamicData, DynamicData>
{
    return block(0, this->rows() - 1, iniCol, endCol);
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::colBlock(size_t iniCol, size_t endCol) -> internal::MatrixBlock<T, DynamicData, DynamicData>
{
    return block(0, this->rows() - 1, iniCol, endCol);
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::zero() -> Matrix
{
    static_assert(Rows != DynamicData || Cols != DynamicData, "Not supported for dinamic matrix");

    return Matrix<T, Rows, Cols>(Rows, Cols, consts::zero<T>);
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::zero(size_t rows, size_t cols) -> Matrix
{
    static_assert(Rows == DynamicData || Cols == DynamicData, "Not supported for static matrix");

    return Matrix<T>(rows, cols, consts::zero<T>);
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::ones() -> Matrix
{
    static_assert(Rows != DynamicData || Cols != DynamicData, "Not supported for dinamic matrix");

    return Matrix<T, Rows, Cols>(Rows, Cols, consts::one<T>);
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::ones(size_t rows, size_t cols) -> Matrix
{
    static_assert(Rows == DynamicData || Cols == DynamicData, "Not supported for static matrix");

    return Matrix<T>(rows, cols, consts::one<T>);
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::identity() -> Matrix
{
    static_assert(Rows != DynamicData || Cols != DynamicData, "Not supported for dinamic matrix");

    Matrix<T, Rows, Cols> matrix;

    for(size_t r = 0; r < matrix.rows(); r++) {
        for(size_t c = 0; c < matrix.cols(); c++) {
            if(r == c) {
                matrix(r, c) = consts::one<T>;
            } else {
                matrix(r, c) = consts::zero<T>;
            }
        }
    }

    return matrix;
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::identity(size_t rows, size_t cols) -> Matrix
{
    static_assert(Rows == DynamicData || Cols == DynamicData, "Not supported for static matrix");

    Matrix<T> matrix(rows, cols);

    for(size_t r = 0; r < matrix.rows(); r++) {
        for(size_t c = 0; c < matrix.cols(); c++) {
            if(r == c) {
                matrix(r, c) = consts::one<T>;
            } else {
                matrix(r, c) = consts::zero<T>;
            }
        }
    }

    return matrix;
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::randon() -> Matrix
{
    static_assert(Rows != DynamicData || Cols != DynamicData, "Not supported for dinamic matrix");

    Matrix<T, Rows, Cols> matrix;

    std::random_device rd;
    std::mt19937 random_number_engine(rd());
    std::uniform_real_distribution<> distribution(0.0, 99.0);

    constexpr size_t size = Rows * Cols;

    for(size_t i = 0; i < size; ++i) {
        matrix(i) = static_cast<T>(distribution(random_number_engine));
    }

    return matrix;
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::randon(size_t rows, size_t cols) -> Matrix
{
    static_assert(Rows == DynamicData || Cols == DynamicData, "Not supported for static matrix");

    Matrix<T> matrix(rows, cols);

    std::random_device rd;
    std::mt19937 random_number_engine(rd());
    std::uniform_real_distribution<> distribution(0.0, 99.0);

    size_t size = matrix.rows() * matrix.cols();

    for(size_t i = 0; i < size; ++i) {
        matrix(i) = static_cast<T>(distribution(random_number_engine));
    }

    return matrix;
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::data() TL_NOEXCEPT -> pointer
{
    return mData.data();
}

template<typename T, size_t Rows, size_t Cols>
auto Matrix<T, Rows, Cols>::data() const TL_NOEXCEPT -> const_pointer
{
    return mData.data();
}





/* Binary arithmetic operators */

/* Special cases of addition */

template<typename T, size_t Rows, size_t Cols>
auto operator +(Matrix<T, Rows, Cols>&& matrix1,
                const Matrix<T, Rows, Cols>& matrix2) -> Matrix<T, Rows, Cols>
{
    matrix1 += matrix2;
    return matrix1;
}

template<typename T, size_t Rows, size_t Cols>
auto operator +(const Matrix<T, Rows, Cols>& matrix1,
                Matrix<T, Rows, Cols>&& matrix2) -> Matrix<T, Rows, Cols>
{
    matrix2 += matrix1;
    return matrix2;
}

template<typename T, size_t Rows, size_t Cols>
auto operator +(Matrix<T, Rows, Cols>&& matrix1,
                Matrix<T, Rows, Cols>&& matrix2) -> Matrix<T, Rows, Cols>
{
    matrix1 += matrix2;
    return matrix1;
}

template<typename T, size_t Rows, size_t Cols>
Matrix<T, Rows, Cols> operator + (const internal::MatrixBlock<T> &matrix1,
                                    const Matrix<T, Rows, Cols> &matrix2)
{
    Matrix<T, Rows, Cols> matrix = matrix2;
    matrix += matrix1;
    return matrix;
}

/* Casos especiales de resta de matrices */

template<typename T, size_t Rows, size_t Cols>
auto operator -(Matrix<T, Rows, Cols>&& matrix1,
                const Matrix<T, Rows, Cols>& matrix2) -> Matrix<T, Rows, Cols>
{
    matrix1 -= matrix2;
    return matrix1;
}

template<typename T, size_t Rows, size_t Cols>
auto operator -(const Matrix<T, Rows, Cols>& matrix1,
                Matrix<T, Rows, Cols>&& matrix2) -> Matrix<T, Rows, Cols>
{
    matrix2 -= matrix1;
    return -matrix2;
}

template<typename T, size_t Rows, size_t Cols>
auto operator -(Matrix<T, Rows, Cols>&& matrix1,
                Matrix<T, Rows, Cols>&& matrix2) -> Matrix<T, Rows, Cols>
{
    matrix1 -= matrix2;
    return matrix1;
}

template<typename T, size_t Rows, size_t Cols>
auto operator -(const internal::MatrixBlock<T>& matrix1,
                const Matrix<T, Rows, Cols>& matrix2) -> Matrix<T, Rows, Cols>
{
    Matrix<T, Rows, Cols> matrix = matrix2;
    matrix -= matrix1;
    return -matrix;
}


/*!
 * \brief Matrix multiplication
 *
 * \f[ C = A * B \f]
 *
 * \f[
 * A=\begin{bmatrix}
 * a1 & a2  \\
 * a3 & a4  \\
 * \end{bmatrix}
 *
 * B=\begin{bmatrix}
 * b1 & b2 & b3 \\
 * b4 & b5 & b6 \\
 * \end{bmatrix}
 *
 * C=\begin{bmatrix}
 * a1*b1+a2*b4 & a1*b2+a2*b5 & a1*b3+a2*b6 \\
 * a3*b1+a4*b4 & a3*b2+a4*b5 & a3*b3+a4*b6 \\
 * \end{bmatrix}
 * \f]
 *
 * <h4>Example</h4>
 * \code
 * Matrix2x2i A;
 * Matrix2x2i B;
 *
 * A.at(0, 0) = 1;
 * A.at(0, 1) = 4;
 * A.at(1, 0) = 3;
 * A.at(1, 1) = 2;
 *
 * B.at(0, 0) = 4;
 * B.at(0, 1) = 5;
 * B.at(1, 0) = 2;
 * B.at(1, 1) = 8;
 *
 * Matrix2x2i C = A * B;
 * \endcode
 */
template<typename T, size_t Rows, size_t _dim, size_t Cols>
auto operator *(const Matrix<T, Rows, _dim>& matrix1,
                const Matrix<T, _dim, Cols>& matrix2) -> Matrix<T, Rows, Cols>
{
    Matrix<T, Rows, Cols> matrix = Matrix<T, Rows, Cols>::zero();

    internal::mulmat(matrix1, matrix2, matrix);

    return matrix;
}

template<typename T>
auto operator *(const Matrix<T>& matrix1,
                const Matrix<T>& matrix2) -> Matrix<T>
{
    TL_ASSERT(matrix1.cols() == matrix2.rows(), "A columns != B rows");

    Matrix<T> matrix = Matrix<T>::zero(matrix1.rows(), matrix2.cols());
    internal::mulmat(matrix1, matrix2, matrix);

    return matrix;
}

template<typename T, size_t Rows, size_t Cols>
auto operator *(const Matrix<T, Rows, Cols>& matrix1,
                const Matrix<T>& matrix2) -> Matrix<T>
{
    TL_ASSERT(matrix1.cols() == matrix2.rows(), "A columns != B rows");

    Matrix<T> matrix = Matrix<T>::zero(matrix1.rows(), matrix2.cols());
    internal::mulmat(matrix1, matrix2, matrix);

    return matrix;
}

template<typename T, size_t Rows, size_t Cols>
auto operator *(const Matrix<T>& matrix1,
                const Matrix<T, Rows, Cols>& matrix2) -> Matrix<T>
{
    TL_ASSERT(matrix1.cols() == matrix2.rows(), "A columns != B rows");

    Matrix<T> matrix = Matrix<T>::zero(matrix1.rows(), matrix2.cols());
    internal::mulmat(matrix1, matrix2, matrix);

    return matrix;
}

template<typename T, size_t Rows, size_t Cols>
auto operator *(Matrix<T, Rows, Cols>&& matrix,
                T scalar) -> Matrix<T, Rows, Cols>
{
    matrix *= scalar;
    return matrix;
}


/*!
 * \brief Multiplication of a scalar by a matrix
 *
 * \f[ C = s * A \f]
 *
 * \f[
 * A=\begin{bmatrix}
 * a1 & a2 & a3 \\
 * a4 & a5 & a6 \\
 * a7 & a8 & a9 \\
 * \end{bmatrix}
 *
 * C=\begin{bmatrix}
 * s*a1 & s*a2 & s*a3 \\
 * s*a4 & s*a5 & s*a6 \\
 * s*a7 & s*a8 & s*a9 \\
 * \end{bmatrix}
 * \f]
 *
 * <h4>Example</h4>
 * \code
 * Matrix2x2i A{1, 4,
 *              3, 2};
 *
 * int s = 2;
 * Matrix2x2i C = s * A;
 * \endcode
 */
template<
  template<typename, size_t Rows = DynamicData, size_t Cols = DynamicData>
class MatrixDerived, typename T, size_t Rows, size_t Cols
>
auto operator *(T scalar, const MatrixDerived<T, Rows, Cols>& matrix) -> Matrix<T, Rows, Cols>
{
    Matrix<T, Rows, Cols> _matrix = matrix;
    _matrix *= scalar;
    return _matrix;
}

template<typename T, size_t Rows, size_t Cols>
auto operator *(T scalar, Matrix<T, Rows, Cols>&& matrix) -> Matrix<T, Rows, Cols>
{
    matrix *= scalar;
    return matrix;
}

/*!
 * \brief Division of a matrix by a scalar
 *
 * \f[ C = A / s \f]
 *
 * \f[
 * A=\begin{bmatrix}
 * a1 & a2 & a3 \\
 * a4 & a5 & a6 \\
 * a7 & a8 & a9 \\
 * \end{bmatrix}
 *
 * C=\begin{bmatrix}
 * a1/s & a2/s & a3/s \\
 * a4/s & a5/s & a6/s \\
 * a7/s & a8/s & a9/s \\
 * \end{bmatrix}
 * \f]
 *
 * <h4>Example</h4>
 * \code
 * Matrix2x2i A{1.f, 4.f,
 *              3.f, 2.f};
 *
 * float s = 2.f;
 * Matrix2x2f C = A / s;
 * \endcode
 */
template<typename T, size_t Rows, size_t Cols>
auto operator /(Matrix<T, Rows, Cols>&& matrix, T scalar) -> Matrix<T, Rows, Cols>
{
    matrix /= scalar;
    return matrix;
}

template<typename T, size_t Rows, size_t Cols>
auto operator ==(const Matrix<T, Rows, Cols>& matrix1,
                 const Matrix<T, Rows, Cols>& matrix2) -> bool
{
    size_t rows1 = matrix1.rows();
    size_t cols1 = matrix1.cols();
    size_t rows2 = matrix2.rows();
    size_t cols2 = matrix2.cols();
    if(rows1 != rows2 || cols1 != cols2) return false;

    for(size_t r = 0; r < matrix1.rows(); r++) {
        for(size_t c = 0; c < matrix1.cols(); c++) {
            if(matrix1(r, c) != matrix2(r, c)) return false;
        }
    }

    return true;
}

template<typename T, size_t Rows, size_t Cols>
auto operator !=(const Matrix<T, Rows, Cols>& matrix1,
                 const Matrix<T, Rows, Cols>& matrix2) -> bool
{
    size_t rows1 = matrix1.rows();
    size_t cols1 = matrix1.cols();
    size_t rows2 = matrix2.rows();
    size_t cols2 = matrix2.cols();
    if(rows1 != rows2 || cols1 != cols2) return true;

    for(size_t r = 0; r < matrix1.rows(); r++) {
        for(size_t c = 0; c < matrix1.cols(); c++) {
            if(matrix1(r, c) != matrix2(r, c)) return true;
        }
    }

    return false;
}



/*------------------------------------------------------------------------*/


template<typename T, size_t Rows, size_t Cols>
auto operator<<(std::ostream& os, const Matrix<T, Rows, Cols>& matrix) -> std::ostream&
{
    for(size_t r = 0; r < matrix.rows(); r++) {
        for(size_t c = 0; c < matrix.cols(); c++) {
            os << std::left << std::setw(12) << matrix(r, c) << " ";
        }
        os << "\n";
    }
    os << std::flush;
    return os;
}

template<typename T, size_t Rows, size_t Cols>
auto operator<<(std::ostream& os, const Matrix<T, Rows, Cols>* matrix) -> std::ostream&
{
    for(size_t r = 0; r < matrix->rows(); r++) {
        for(size_t c = 0; c < matrix->cols(); c++) {
            os << std::left << std::setw(12) << (*matrix)(r, c) << " ";
        }
        os << "\n";
    }
    os << std::flush;
    return os;
}


template<typename T, size_t Rows, size_t Cols>
auto operator<<(std::ostream& os, const internal::MatrixBlock<T, Rows, Cols>& matrix) -> std::ostream&
{
    for(size_t r = 0; r < matrix.rows(); r++) {
        for(size_t c = 0; c < matrix.cols(); c++) {
            os << std::left << std::setw(12) << matrix(r, c) << " ";
        }
        os << "\n";
    }
    os << std::flush;
    return os;
}

template<typename T, size_t Rows, size_t Cols>
auto operator<<(std::ostream& os, const internal::MatrixBlock<T, Rows, Cols>* matrix) -> std::ostream&
{
    for(size_t r = 0; r < matrix->rows(); r++) {
        for(size_t c = 0; c < matrix->cols(); c++) {
            os << std::left << std::setw(12) << (*matrix)(r, c) << " ";
        }
        os << "\n";
    }
    os << std::flush;
    return os;
}


/*------------------------------------------------------------------------*/



template<typename T, size_t Rows, size_t Cols>
auto operator *(const Matrix<T, Rows, Cols>& matrix,
                const Vector<T, Cols>& vector) -> Vector<T, Rows>
{
    Vector<T, Rows> vector_out = Vector<T, Rows>::zero();
    internal::matrix_per_vector(matrix, vector, vector_out);
    return vector_out;
}

template<typename T>
auto operator *(const Matrix<T>& matrix,
                const Vector<T>& vector) -> Vector<T>
{
    Vector<T> vector_out = Vector<T>::zero(matrix.rows());
    internal::matrix_per_vector(matrix, vector, vector_out);
    return vector_out;
}

template<typename T, size_t Rows, size_t Cols>
auto operator *(const Matrix<T, Rows, Cols>& matrix,
                const Vector<T>& vector) -> Vector<T, Rows>
{
    Vector<T, Rows> vector_out = Vector<T, Rows>::zero();
    internal::matrix_per_vector(matrix, vector, vector_out);
    return vector_out;
}

template<typename T, size_t _dim>
auto operator *(const Matrix<T>& matrix,
                const Vector<T, _dim>& vector) -> Vector<T>
{
    Vector<T> vector_out = Vector<T>::zero(matrix.rows());
    internal::matrix_per_vector(matrix, vector, vector_out);
    return vector_out;
}

//template<
//  template<typename, size_t Rows = DynamicData, size_t Cols = DynamicData>
//  class MatrixDerived, typename T, size_t Rows, size_t Cols,
//>
//static Vector<T, Cols> operator * (const MatrixDerived<T, Rows, Cols> &matrix,
//                                    const Vector<T, Cols> &vector)
//{
//  size_t rows = matrix.rows();
//  size_t dim1 = matrix.cols();
//  size_t dim2 = vector.size();
//
//  TL_ASSERT(dim1 == dim2, "Matrix columns != Vector size");
//
//  Vector<T, _col> vect = Vector<T>::zero(rows);
//
//  for (size_t r = 0; r < rows; r++) {
//    for (size_t c = 0; c < dim1; c++) {
//      vect[r] += matrix(r, c) * vector[c];
//    }
//  }
//
//  return vect;
//}

template<typename T, size_t _dim>
auto operator *(const internal::MatrixBlock<T>& matrix,
                const Vector<T, _dim>& vector) -> Vector<T>
{
    size_t rows = matrix.rows();
    size_t dim1 = matrix.cols();
    size_t dim2 = vector.size();

    TL_ASSERT(dim1 == dim2, "Matrix columns != Vector size");

    Vector<T> vect = Vector<T>::zero(rows);

    for(size_t r = 0; r < rows; r++) {
        for(size_t c = 0; c < dim1; c++) {
            vect[r] += matrix(r, c) * vector[c];
        }
    }

    return vect;
}


template<typename T, size_t Rows, size_t _dim> 
auto operator *(const Vector<T, _dim>& vector,
                const Matrix<T, Rows, _dim>& matrix) -> Vector<T, Rows>
{
    return matrix * vector;
}

/*! \} */

} // End namespace tl


