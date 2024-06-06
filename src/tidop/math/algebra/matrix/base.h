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

#include "tidop/math/data.h"


namespace tl
{

/*! \addtogroup math
 *  \{
 */

/*! \addtogroup algebra Algebra
 *  \{
 */
 
template<
  template<typename, size_t Rows = DynamicData, size_t Cols = DynamicData>
  class MatrixDerived, typename T, size_t Rows, size_t Cols
>
class MatrixBase<MatrixDerived<T, Rows, Cols>>
{

public:

    enum class Properties
    {
        contiguous_memory = 0x01
    };

public:

    MatrixBase();
    virtual ~MatrixBase() = default;
    
    /*!
     * \brief Determinant of the matrix
     * \return Determinant
     */
    auto determinant() const -> T;
    
    /* Unary arithmetic operators */
    
    /*!
     * \brief Operator unary plus
     */
    auto operator+() const -> Matrix<T, Rows, Cols>;
    
    /*!
     * \brief Operator unary minus
     *
     * \f[
     * A=\begin{bmatrix}
     * 1 & 2 & 3 \\
     * 4 & 5 & 6 \\
     * 7 & 8 & 9 \\
     * \end{bmatrix}
     *
     * B = -A
     *
     * B=\begin{bmatrix}
     * -1 & -2 & -3 \\
     * -4 & -5 & -6 \\
     * -7 & -8 & -9 \\
     * \end{bmatrix}
     * \f]
     *
     * \return Matrix with all the elements of the input matrix changed sign
     */
    auto operator-() const -> Matrix<T, Rows, Cols>;

    /* Binary arithmetic operators */
    
    /*!
     * \brief Addition or addition of matrices
     *
     * \f[ C = A + B \f]
     *
     * \f[
     * A=\begin{bmatrix}
     * a1 & a2 & a3 \\
     * a4 & a5 & a6 \\
     * a7 & a8 & a9 \\
     * \end{bmatrix}
     *
     * B=\begin{bmatrix}
     * b1 & b2 & b3 \\
     * b4 & b5 & b6 \\
     * b7 & b8 & b9 \\
     * \end{bmatrix}
     *
     * C=\begin{bmatrix}
     * a1+b1 & a2+b2 & a3+b3 \\
     * a4+b4 & a5+b5 & a6+b6 \\
     * a7+b7 & a8+b8 & a9+b9 \\
     * \end{bmatrix}
     * \f]
     *
     * <h4>Example</h4>
     * \code
     * Matrix2x2i A;
     * Matrix2x2i B;
     *
     * Matrix2x2i A{1, 4,
     *              3, 2};
     * Matrix2x2i B{4, 5,
     *              2, 8};
     *
     * Matrix2x2i C = A + B;
     * \endcode
     */
    auto operator +(const MatrixDerived<T, Rows, Cols> &matrix2) const-> Matrix<T, Rows, Cols>;
    
    template<typename MatrixDerived2>
    auto operator +(const MatrixDerived2 &matrix2) const -> MatrixDerived<T, Rows, Cols>;

    /*!
     * \brief Addition to a matrix
     *
     * Addiction from one matrix to another
     *
     * \f[ A += B \f]
     *
     * \f[
     * A=\begin{bmatrix}
     * a1 & a2 & a3 \\
     * a4 & a5 & a6 \\
     * a7 & a8 & a9 \\
     * \end{bmatrix}
     *
     * B=\begin{bmatrix}
     * b1 & b2 & b3 \\
     * b4 & b5 & b6 \\
     * b7 & b8 & b9 \\
     * \end{bmatrix}
     *
     * A=\begin{bmatrix}
     * a1+b1 & a2+b2 & a3+b3 \\
     * a4+b4 & a5+b5 & a6+b6 \\
     * a7+b7 & a8+b8 & a9+b9 \\
     * \end{bmatrix}
     * \f]
     *
     * <h4>Example</h4>
     * \code
     * Matrix2x2i A{1, 4,
     *              3, 2};
     * Matrix2x2i B{4, 5,
     *              2, 8};
     *
     *  A += B;
     *
     * \endcode
     */
    template<typename MatrixDerived2>
    auto operator +=(const MatrixDerived2 &matrix) -> MatrixDerived<T, Rows, Cols> &;

    /*!
     * \brief Subtraction of matrices
     *
     * \f[ C = A - B \f]
     *
     * \f[
     * A=\begin{bmatrix}
     * a1 & a2 & a3 \\
     * a4 & a5 & a6 \\
     * a7 & a8 & a9 \\
     * \end{bmatrix}
     *
     * B=\begin{bmatrix}
     * b1 & b2 & b3 \\
     * b4 & b5 & b6 \\
     * b7 & b8 & b9 \\
     * \end{bmatrix}
     *
     * C=\begin{bmatrix}
     * a1-b1 & a2-b2 & a3-b3 \\
     * a4-b4 & a5-b5 & a6-b6 \\
     * a7-b7 & a8-b8 & a9-b9 \\
     * \end{bmatrix}
     * \f]
     *
     * <h4>Example</h4>
     * \code
     * Matrix2x2i A;
     * Matrix2x2i B;
     *
     * Matrix2x2i A{1, 4,
     *              3, 2};
     * Matrix2x2i B{4, 5,
     *              2, 8};
     *
     * Matrix2x2i C = A - B;
     * \endcode
     */
    auto operator -(const MatrixDerived<T, Rows, Cols> &matrix2) const -> Matrix<T, Rows, Cols>;
    
    template<typename MatrixDerived2>
    auto operator -(const MatrixDerived2 &matrix2) const -> MatrixDerived<T, Rows, Cols>;

    /*!
     * \brief Subtraction of one matrix by another
     *
     * \f[ A -= B \f]
     *
     * \f[
     * A=\begin{bmatrix}
     * a1 & a2 & a3 \\
     * a4 & a5 & a6 \\
     * a7 & a8 & a9 \\
     * \end{bmatrix}
     *
     * B=\begin{bmatrix}
     * b1 & b2 & b3 \\
     * b4 & b5 & b6 \\
     * b7 & b8 & b9 \\
     * \end{bmatrix}
     * \f]
     *
     * <h4>Example</h4>
     * \code
     * Matrix2x2i A;
     * Matrix2x2i B;
     *
     * Matrix2x2i A{1, 4,
     *              3, 2};
     * Matrix2x2i B{4, 5,
     *              2, 8};
     *
     * Matrix2x2i A -= B;
     * \endcode
     */
    template<typename MatrixDerived2>
    auto operator -=(const MatrixDerived2 &matrix) -> MatrixDerived<T, Rows, Cols>&;

    /*!
     * \brief Multiplication of a matrix by a scalar
     *
     * \f[ C = A * s \f]
     *
     * \f[
     * A=\begin{bmatrix}
     * a1 & a2 & a3 \\
     * a4 & a5 & a6 \\
     * a7 & a8 & a9 \\
     * \end{bmatrix}
     *
     * C=\begin{bmatrix}
     * a1*s & a2*s & a3*s \\
     * a4*s & a5*s & a6*s \\
     * a7*s & a8*s & a9*s \\
     * \end{bmatrix}
     * \f]
     *
     * <h4>Example</h4>
     * \code
     * Matrix2x2i A{1, 4,
     *              3, 2};
     *
     * int s = 2;
     * Matrix2x2i C = A * s;
     * \endcode
     */
    auto operator *(T scalar) const -> Matrix<T, Rows, Cols>;

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
     * int s = 2;
     * A *= s;
     * \endcode
     */
    auto operator *=(T scalar) -> MatrixDerived<T, Rows, Cols>&;

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
    auto operator /(T scalar) const -> Matrix<T, Rows, Cols>;

    /*!
     * \brief Division of a scalar by a matrix
     *
     * \f[ C = s / A \f]
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
     * int s = 2;
     * A *= s;
     * \endcode
     */
    auto operator /=(T scalar) -> MatrixDerived<T, Rows, Cols> &;

    /// Tendria que ser una clase Vector al igual que MatrixCol y MatrixRow
    /// Por ahora construyo un vector
    auto diagonal() const -> Vector<T>; 

protected:

    auto derived() -> MatrixDerived<T, Rows, Cols> &;
    auto derived() const -> const MatrixDerived<T, Rows, Cols> &;

protected:

    template<typename MatrixDerived2>
    void set(const MatrixDerived2 &matrix);

    auto determinant2x2() const -> T;
    auto determinant3x3() const -> T;
    auto determinant4x4() const -> T;
    auto determinantnxn() const -> T;
 
public:

    EnumFlags<Properties> properties;

};



/* MatrixBase implementation */

template<
  template<typename, size_t Rows = DynamicData, size_t Cols = DynamicData>
  class MatrixDerived, typename T, size_t Rows, size_t Cols>
MatrixBase<MatrixDerived<T, Rows, Cols>>::MatrixBase()
{
    this->properties.enable(Properties::contiguous_memory);
}

template<
  template<typename, size_t Rows = DynamicData, size_t Cols = DynamicData>
  class MatrixDerived, typename T, size_t Rows, size_t Cols>
auto MatrixBase<MatrixDerived<T, Rows, Cols>>::determinant() const -> T
{
    static_assert(Rows == Cols, "Non-Square Matrix");

    auto &derived = this->derived();

    size_t rows = derived.rows();
    size_t cols = derived.cols();
    TL_ASSERT(rows == cols, "Non-Square Matrix");

    T d = consts::one<T>;

    if(rows == 2)
        d = determinant2x2();
    else if(rows == 3)
        d = determinant3x3();
    else if(rows == 4)
        d = determinant4x4();
    else
        d = determinantnxn();

    return d;
}

template<
  template<typename, size_t Rows = DynamicData, size_t Cols = DynamicData>
  class MatrixDerived, typename T, size_t Rows, size_t Cols>
auto MatrixBase<MatrixDerived<T, Rows, Cols>>::operator+() const -> Matrix<T, Rows, Cols>
{
    return this->derived();
}

template<
  template<typename, size_t Rows = DynamicData, size_t Cols = DynamicData>
  class MatrixDerived, typename T, size_t Rows, size_t Cols>
auto MatrixBase<MatrixDerived<T, Rows, Cols>>::operator-() const -> Matrix<T, Rows, Cols>
{
    static_assert(std::is_signed<T>::value, "Requires signed type");

    Matrix<T, Rows, Cols> matrix = this->derived();

    size_t size = matrix.rows() * matrix.cols();
    size_t i{0};

#ifdef TL_HAVE_SIMD_INTRINSICS

    Packed<T> packed_a;

    constexpr size_t packed_size = packed_a.size();
    size_t max_size = size - size % packed_size;

    if(this->properties.isEnabled(Properties::contiguous_memory)) {

        for(; i < max_size; i += packed_size) {
            packed_a.loadUnaligned(&matrix(i));
            packed_a = -packed_a;
            packed_a.storeUnaligned(&matrix(i));
        }
    }
#endif

    for(; i < size; i++)
        matrix(i) = -matrix(i);

    return matrix;
}

template<
  template<typename, size_t Rows = DynamicData, size_t Cols = DynamicData>
  class MatrixDerived, typename T, size_t Rows, size_t Cols>
auto MatrixBase<MatrixDerived<T, Rows, Cols>>::operator +(const MatrixDerived<T, Rows, Cols> &matrix2) const -> Matrix<T, Rows, Cols>
{
    Matrix<T, Rows, Cols> matrix = this->derived();
    matrix += matrix2;
    return matrix;
}

template<
  template<typename, size_t Rows = DynamicData, size_t Cols = DynamicData>
  class MatrixDerived, typename T, size_t Rows, size_t Cols>
template<typename MatrixDerived2>
auto MatrixBase<MatrixDerived<T, Rows, Cols>>::operator +(const MatrixDerived2 &matrix2) const -> MatrixDerived<T, Rows, Cols>
{
    MatrixDerived<T, Rows, Cols> matrix = this->derived();
    matrix += matrix2;
    return matrix;
}

template<
  template<typename, size_t Rows = DynamicData, size_t Cols = DynamicData>
  class MatrixDerived, typename T, size_t Rows, size_t Cols>
template<typename MatrixDerived2>
auto MatrixBase<MatrixDerived<T, Rows, Cols>>::operator +=(const MatrixDerived2 &matrix) -> MatrixDerived<T, Rows, Cols> &
{
    auto &derived = this->derived();

    size_t rows = derived.rows();
    size_t cols = derived.cols();
    size_t rows2 = matrix.rows();
    size_t cols2 = matrix.cols();

    TL_ASSERT(rows == rows2 && cols == cols2, "Different size matrices");

    size_t size = rows * cols;
    size_t i{0};

#ifdef TL_HAVE_SIMD_INTRINSICS

    Packed<T> packed_a;
    Packed<T> packed_b;

    constexpr size_t packed_size = packed_a.size();
    size_t max_size = size - size % packed_size;

    if(this->properties.isEnabled(MatrixDerived<T, Rows, Cols>::Properties::contiguous_memory) &&
       matrix.properties.isEnabled(MatrixDerived2::Properties::contiguous_memory)) {

        for(; i < max_size; i += packed_size) {
            packed_a.loadUnaligned(&derived(i));
            packed_b.loadUnaligned(&matrix(i));
            packed_a += packed_b;
            packed_a.storeUnaligned(&derived(i));
        }
    }
#endif

    for(; i < size; i++) {
        derived(i) += matrix(i);
    }

    return derived;
}

template<
  template<typename, size_t Rows = DynamicData, size_t Cols = DynamicData>
  class MatrixDerived, typename T, size_t Rows, size_t Cols>
auto MatrixBase<MatrixDerived<T, Rows, Cols>>::operator -(const MatrixDerived<T, Rows, Cols> &matrix2) const -> Matrix<T, Rows, Cols>
{
    Matrix<T, Rows, Cols> matrix = this->derived();
    matrix -= matrix2;
    return matrix;
}

template<
  template<typename, size_t Rows = DynamicData, size_t Cols = DynamicData>
  class MatrixDerived, typename T, size_t Rows, size_t Cols>
template<typename MatrixDerived2>
auto MatrixBase<MatrixDerived<T, Rows, Cols>>::operator -(const MatrixDerived2 &matrix2) const -> MatrixDerived<T, Rows, Cols>
{
    MatrixDerived<T, Rows, Cols> matrix = this->derived();
    matrix -= matrix2;
    return matrix;
}

template<
  template<typename, size_t Rows = DynamicData, size_t Cols = DynamicData>
  class MatrixDerived, typename T, size_t Rows, size_t Cols>
template<typename MatrixDerived2>
auto MatrixBase<MatrixDerived<T, Rows, Cols>>::operator -=(const MatrixDerived2 &matrix) -> MatrixDerived<T, Rows, Cols> &
{
    auto &derived = this->derived();

    size_t rows = derived.rows();
    size_t cols = derived.cols();
    size_t rows2 = matrix.rows();
    size_t cols2 = matrix.cols();

    TL_ASSERT(rows == rows2 && cols == cols2, "Different size matrices");

    size_t size = rows * cols;
    size_t i{0};

#ifdef TL_HAVE_SIMD_INTRINSICS

    Packed<T> packed_a;
    Packed<T> packed_b;

    constexpr size_t packed_size = packed_a.size();
    size_t max_size = size - size % packed_size;

    if(this->properties.isEnabled(Properties::contiguous_memory) &&
        matrix.properties.isEnabled(MatrixDerived2::Properties::contiguous_memory)) {
        for(; i < max_size; i += packed_size) {

            packed_a.loadUnaligned(&derived(i));
            packed_b.loadUnaligned(&matrix(i));
            packed_a -= packed_b;
            packed_a.storeUnaligned(&derived(i));
        }
    }

#endif

    for(; i < size; i++) {
        derived(i) -= matrix(i);
    }

    return  derived;
}

template<
  template<typename, size_t Rows = DynamicData, size_t Cols = DynamicData>
  class MatrixDerived, typename T, size_t Rows, size_t Cols>
auto MatrixBase<MatrixDerived<T, Rows, Cols>>::operator *(T scalar) const -> Matrix<T, Rows, Cols>
{
    Matrix<T, Rows, Cols> matrix = this->derived();
    matrix *= scalar;
    return matrix;
}

template<
  template<typename, size_t Rows = DynamicData, size_t Cols = DynamicData>
  class MatrixDerived, typename T, size_t Rows, size_t Cols>
auto MatrixBase<MatrixDerived<T, Rows, Cols>>::operator *=(T scalar) -> MatrixDerived<T, Rows, Cols> &
{
    auto &derived = this->derived();
    size_t size = derived.rows() * derived.cols();
    size_t i{0};

#ifdef TL_HAVE_SIMD_INTRINSICS

    Packed<T> packed_a;
    Packed<T> packed_b(scalar);

    constexpr size_t packed_size = packed_a.size();
    size_t max_size = size - size % packed_size;

    if(this->properties.isEnabled(Properties::contiguous_memory)) {

        for(; i < max_size; i += packed_size) {
            packed_a.loadUnaligned(&derived(i));
            packed_a *= packed_b;
            packed_a.storeUnaligned(&derived(i));
        }
    }

#endif

    for(; i < size; i++) {
        derived(i) *= scalar;
    }

    return derived;
}

template<
  template<typename, size_t Rows = DynamicData, size_t Cols = DynamicData>
  class MatrixDerived, typename T, size_t Rows, size_t Cols>
auto MatrixBase<MatrixDerived<T, Rows, Cols>>::operator /(T scalar) const -> Matrix<T, Rows, Cols>
{
    Matrix<T, Rows, Cols> _matrix = this->derived();
    _matrix /= scalar;
    return _matrix;
}

template<
  template<typename, size_t Rows = DynamicData, size_t Cols = DynamicData>
  class MatrixDerived, typename T, size_t Rows, size_t Cols>
auto MatrixBase<MatrixDerived<T, Rows, Cols>>::operator /=(T scalar) -> MatrixDerived<T, Rows, Cols> &
{
    auto &derived = this->derived();
    size_t size = derived.rows() * derived.cols();
    size_t i{0};

    T s = consts::one<T> / scalar;

#ifdef TL_HAVE_SIMD_INTRINSICS

    Packed<T> packed_a;
    Packed<T> packed_b(s);

    constexpr size_t packed_size = packed_a.size();
    size_t max_size = size - size % packed_size;

    if(this->properties.isEnabled(Properties::contiguous_memory)) {

        for(; i < max_size; i += packed_size) {
            packed_a.loadUnaligned(&derived(i));
            packed_a *= packed_b;
            packed_a.storeUnaligned(&derived(i));
        }
    }

#endif

    for(; i < size; i++) {
        derived(i) *= s;
    }

    return derived;
}

template<
  template<typename, size_t Rows = DynamicData, size_t Cols = DynamicData>
  class MatrixDerived, typename T, size_t Rows, size_t Cols>
auto MatrixBase<MatrixDerived<T, Rows, Cols>>::diagonal() const -> Vector<T> 
{

    auto &derived = this->derived();
    size_t size = std::min(derived.rows(), derived.cols());
    Vector<T> diag_vector(size);

    for(size_t i = 0; i < size; ++i) {
        diag_vector[i] = derived(i, i);
    }

    return diag_vector;
}

template<
  template<typename, size_t Rows = DynamicData, size_t Cols = DynamicData>
  class MatrixDerived, typename T, size_t Rows, size_t Cols>
auto MatrixBase<MatrixDerived<T, Rows, Cols>>::derived() -> MatrixDerived<T, Rows, Cols> &
{
    return *static_cast<MatrixDerived<T, Rows, Cols> *>(this);
}

template<
  template<typename, size_t Rows = DynamicData, size_t Cols = DynamicData>
  class MatrixDerived, typename T, size_t Rows, size_t Cols>
auto MatrixBase<MatrixDerived<T, Rows, Cols>>::derived() const -> const MatrixDerived<T, Rows, Cols> &
{
    return *static_cast<const MatrixDerived<T, Rows, Cols> *>(this);
}

template<
  template<typename, size_t Rows = DynamicData, size_t Cols = DynamicData>
  class MatrixDerived, typename T, size_t Rows, size_t Cols>
template<typename MatrixDerived2>
void MatrixBase<MatrixDerived<T, Rows, Cols>>::set(const MatrixDerived2 &matrix)
{
    auto &derived = this->derived();

    size_t rows = derived.rows();
    size_t cols = derived.cols();

    TL_ASSERT(rows == matrix.rows() && cols == matrix.cols(), "Different size matrices");

    size_t size = rows * cols;
    size_t i{0};

#ifdef TL_HAVE_SIMD_INTRINSICS

    Packed<T> packed_a;
    Packed<T> packed_b;

    constexpr size_t packed_size = packed_a.size();
    size_t max_size = size - size % packed_size;

    if(this->properties.isEnabled(Properties::contiguous_memory) &&
        matrix.properties.isEnabled(MatrixDerived2::Properties::contiguous_memory)) {

        for(; i < max_size; i += packed_size) {
            packed_b.loadUnaligned(&matrix(i));
            packed_b.storeUnaligned(&derived(i));
        }

    }

#endif

    for(; i < size; i++) {
        derived(i) = matrix(i);
    }

}

template<
  template<typename, size_t Rows = DynamicData, size_t Cols = DynamicData>
  class MatrixDerived, typename T, size_t Rows, size_t Cols>
auto MatrixBase<MatrixDerived<T, Rows, Cols>>::determinant2x2() const -> T
{
    auto &derived = this->derived();
    
    T det = derived(0, 0) * derived(1, 1) - derived(0, 1) * derived(1, 0);
    
    return det;
}

template<
  template<typename, size_t Rows = DynamicData, size_t Cols = DynamicData>
  class MatrixDerived, typename T, size_t Rows, size_t Cols>
auto MatrixBase<MatrixDerived<T, Rows, Cols>>::determinant3x3() const -> T
{
    auto &derived = this->derived();
    
    T m00 = derived(0, 0);
    T m01 = derived(0, 1);
    T m02 = derived(0, 2);
    T m10 = derived(1, 0);
    T m11 = derived(1, 1);
    T m12 = derived(1, 2);
    T m20 = derived(2, 0);
    T m21 = derived(2, 1);
    T m22 = derived(2, 2);
    
    T c00 = m11 * m22 - m12 * m21;
    T c10 = m12 * m20 - m10 * m22;
    T c20 = m10 * m21 - m11 * m20;
    
    T det = m00 * c00 + m01 * c10 + m02 * c20;
    
    return det;
}

template<
  template<typename, size_t Rows = DynamicData, size_t Cols = DynamicData>
  class MatrixDerived, typename T, size_t Rows, size_t Cols>
auto MatrixBase<MatrixDerived<T, Rows, Cols>>::determinant4x4() const -> T
{
    auto &derived = this->derived();

    T m00 = derived(0, 0);
    T m01 = derived(0, 1);
    T m02 = derived(0, 2);
    T m03 = derived(0, 3);
    T m10 = derived(1, 0);
    T m11 = derived(1, 1);
    T m12 = derived(1, 2);
    T m13 = derived(1, 3);
    T m20 = derived(2, 0);
    T m21 = derived(2, 1);
    T m22 = derived(2, 2);
    T m23 = derived(2, 3);
    T m30 = derived(3, 0);
    T m31 = derived(3, 1);
    T m32 = derived(3, 2);
    T m33 = derived(3, 3);

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

    return det;
}

template<
  template<typename, size_t Rows = DynamicData, size_t Cols = DynamicData>
  class MatrixDerived, typename T, size_t Rows, size_t Cols>
auto MatrixBase<MatrixDerived<T, Rows, Cols>>::determinantnxn() const -> T
{
    auto &derived = this->derived();

    T d = consts::one<T>;
    size_t rows = derived.rows();
    size_t cols = derived.cols();

    Matrix<T, DynamicData, DynamicData> matrix = derived;

    for (size_t i = 0; i < rows; ++i) {
        T pivotElement = matrix(i, i);
        size_t pivotRow = i;
        for (size_t r = i + 1; r < rows; ++r) {
            if (std::abs(matrix(r, i)) > std::abs(pivotElement)) {
                pivotElement = matrix(r, i);
                pivotRow = r;
            }
        }

        if (pivotElement == consts::zero<T>) {
            d = consts::zero<T>;
            break;
        }

        if (pivotRow != i) {
            matrix.swapRows(i, pivotRow);
            d = -d;
        }

        d *= pivotElement;

        for (size_t r = i + 1; r < rows; ++r) {
            for (size_t c = i + 1; c < cols; ++c) {
                matrix(r, c) -= matrix(r, i) * matrix(i, c) / pivotElement;
            }
        }
    }

    return d;
}


/*! \} */ // end of algebra

/*! \} */ // end of math

} // End namespace tl