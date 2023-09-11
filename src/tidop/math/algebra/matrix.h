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

#include <iomanip>

#include "tidop/math/math.h"
#include "tidop/math/algebra/vector.h"
#include "tidop/core/exception.h"
#include "tidop/core/utils.h"
#include "tidop/core/concurrency.h"
#include "tidop/math/simd.h"
#include "tidop/math/blas.h"
#include "tidop/math/data.h"
#include "tidop/math/algebra/lu.h"
#include "tidop/geometry/rect.h"



namespace tl
{


//TODO: Expression templates:
//      Por ahora no se implementan las operaciones entre matrices con 
//      expressions templates. En su lugar se emplea cuando es posible
//      el operador de movimiento para minimizar en lo posible la creación 
//      de objetos temporales. 


/*! \addtogroup math
 *  \{
 */

/*! \defgroup algebra Algebra
 *  
 * Algebra
 *
 *  \{
 */

template<typename T>
class MatrixBase;

template<typename T, size_t _rows, size_t _cols>
class Matrix;

template<typename T>
class LuDecomposition;

/// \cond

namespace internal
{

 /* Iterators */


template<typename T>
class IteratorRows
{

public:

    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T *;
    using reference = T &;

private:

    pointer rowPtr;

public:

    IteratorRows(pointer ptr);
    ~IteratorRows() = default;

    auto operator*() const -> reference;
    auto operator->() -> pointer;
    auto operator++() -> IteratorRows&;
    auto operator++(int) -> IteratorRows;

    bool operator== (const IteratorRows &itRow);
    bool operator!= (const IteratorRows &itRow);

};


template<typename T>
class IteratorCols
{

public:

    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T *;
    using reference = T &;

private:

    pointer colPtr;
    size_t colSize;

public:

    IteratorCols(pointer ptr, size_t colSize);
    ~IteratorCols() = default;

    auto operator*() const -> reference;
    auto operator->() -> pointer;
    auto operator++() -> IteratorCols&;
    auto operator++(int) -> IteratorCols;

    bool operator == (const IteratorCols &itCol);
    bool operator != (const IteratorCols &itCol);

};




template<typename T, size_t _size_ = DynamicData>
class MatrixRow
    : public VectorBase<MatrixRow<T, _size_>>
{
private:

    T *matrixData;
    size_t matrixRow;
    size_t matrixCols;

public:

    using value_type = T;
    using size_type = size_t;
    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;

    using iterator = IteratorRows<T>;
    using const_iterator = IteratorRows<const T>;

public:

    MatrixRow(T *data, size_t row, size_t cols);
    ~MatrixRow() = default;

    auto begin() TL_NOEXCEPT->iterator;
    auto begin() const TL_NOEXCEPT -> const_iterator;
    auto end() TL_NOEXCEPT->iterator;
    auto end() const TL_NOEXCEPT -> const_iterator;
    auto size() const TL_NOEXCEPT -> size_t;

    auto operator[](size_t column) const -> const_reference;
    auto operator[](size_t column) -> reference;
    auto operator=(T value) -> void;
    auto operator=(const Vector<T> &vector) -> MatrixRow&;
    template<typename T2, size_t _size2>
    auto operator = (const Vector<T2, _size2> &vector) -> MatrixRow&;

    operator Vector<T>() const;

};


template<typename T, size_t _size_ = DynamicData>
class MatrixCol
  : public VectorBase<MatrixCol<T, _size_>>
{

private:

    T *matrixData;
    size_t matrixCol;
    size_t matrixRows;
    size_t matrixCols;

public:

    using value_type = T;
    using size_type = size_t;
    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;
    
    using iterator = IteratorCols<T>;
    using const_iterator = IteratorCols<const T>;

public:

    MatrixCol(T *data, size_t col, size_t rows, size_t cols);
    ~MatrixCol() = default;
    
    auto begin() TL_NOEXCEPT -> iterator;
    auto begin() const TL_NOEXCEPT -> const_iterator;
    auto end() TL_NOEXCEPT -> iterator;
    auto end() const TL_NOEXCEPT -> const_iterator;
    auto size() const TL_NOEXCEPT -> size_t;
    
    auto operator[](size_t row) const -> const_reference;
    auto operator[](size_t row) -> reference;
    auto operator=(T value) -> void;
    auto operator=(const Vector<T> &vector) -> MatrixCol&;
    template<typename T2, size_t _size2>
    auto operator = (const Vector<T2, _size2> &vector) -> MatrixCol&;   
    
    operator Vector<T>() const;

};


template<typename T, size_t _rows, size_t _cols>
class MatrixBlock;


template<typename T, size_t _rows = DynamicData, size_t _cols = DynamicData>
class MatrixBlock
  : public MatrixBase<MatrixBlock<T, _rows, _cols>>
{

public:

    using value_type = T;
    using size_type = size_t;
    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;

private:

    T *matrixData;
    size_t matrixRows;
    size_t matrixCols;
    size_t matrixIniRow;
    size_t matrixEndRow;
    size_t matrixIniCol;
    size_t matrixEndCol;
  
public:

    MatrixBlock(T *data,
                size_t rows,
                size_t cols,
                size_t iniRow,
                size_t endRow,
                size_t iniCol,
                size_t endCol);
    ~MatrixBlock() = default;
    
    auto operator=(const MatrixBlock &block) -> MatrixBlock&;
    template<typename T2, size_t _rows2, size_t _cols2>
    auto operator=(const Matrix<T2, _rows2, _cols2> &matrix) -> MatrixBlock&;
    
    /*!
     * \brief Referencia al elemento en la posición fila (r) y columna (c)
     * \param[in] r Fila de la matriz
     * \param[in] c Columna de la matriz
     * \return Valor de la matriz en la posición fila y columna
     * <h4>Ejemplo</h4>
     * \code
     * Matrix<double,3,3> matrix;
     * matrix.at(0, 0) = 1.5;
     * double value = matrix.at(0, 0);
     * \endcode
     */
    auto at(size_t r, size_t c) -> reference;
    
    /*!
     * \brief Referencia constante al elemento en la posición fila (r) y columna (c)
     * \param[in] r Fila
     * \param[in] c Columna
     * \return Valor de la matriz en la posición fila y columna
     * <h4>Ejemplo</h4>
     * \code
     * double value = matrix.at(0, 0);
     * \endcode
     */
    auto at(size_t r, size_t c) const -> const_reference;
    
    /*!
     * \brief Referencia al elemento en la posición fila (r) y columna (c)
     * \param[in] r Fila de la matriz
     * \param[in] c Columna de la matriz
     * \return Valor de la matriz en la posición fila y columna
     * <h4>Ejemplo</h4>
     * \code
     * Matrix<double,3,3> matrix;
     * matrix(0, 0) = 1.5;
     * double value = matrix(0, 0);
     * \endcode
     */
    auto operator()(size_t row, size_t col) -> reference;
    
    /*!
     * \brief Referencia constante al elemento en la posición fila (r) y columna (c)
     * \param[in] r Fila
     * \param[in] c Columna
     * \return Valor de la matriz en la posición fila y columna
     * <h4>Ejemplo</h4>
     * \code
     * double value = matrix(0, 0);
     * \endcode
     */
    auto operator()(size_t row, size_t col) const -> const_reference;
    
    /*!
     * \brief Referencia al elemento
     * La posición del elemento se determina como:
     *   r * this->cols() + c
     * \param[in] position Posición del elemento de la matriz
     * \return Valor de la matriz en dicha posición
     * <h4>Ejemplo</h4>
     * \code
     * Matrix<double,3,3> matrix;
     * matrix(4) = 1.5;
     * double value = matrix(4); // value == 1.5
     * \endcode
     */
    auto operator()(size_t position) -> reference;
    
    /*!
     * \brief Referencia constante al elemento
     * La posición del elemento se determina como:
     *   r * this->cols() + c
     * \param[in] position Posición del elemento de la matriz
     * \return Valor de la matriz en dicha posición
     * <h4>Ejemplo</h4>
     * \code
     * Matrix<double,3,3> matrix;
     * matrix(4) = 1.5;
     * double value = matrix(4); // value == 1.5
     * \endcode
     */
    auto operator()(size_t position) const -> const_reference;
    
    auto rows() const -> size_t;
    auto cols() const -> size_t;
    
    operator Matrix<T, DynamicData, DynamicData>() const;

};


} // namespace internal

/// \endcond



template<
  template<typename, size_t _rows, size_t _cols = DynamicData>
  class MatrixDerived, typename T, size_t _rows, size_t _cols
>
class MatrixBase<MatrixDerived<T, _rows, _cols>>
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
     * \brief Determinante de la matriz
     * \return Determinante
     */
    auto determinant() const -> T;
    
    /* Unary arithmetic operators */
    
    /*!
     * \brief Operator unary plus
     */
    auto operator+() -> Matrix<T, _rows, _cols>;
    
    /*!
     * \brief Operator unary minus
     *
     * \f[
     * A=\begin{bmatrix}
     * 1 & 2 & 3 \\
     * 4 & 5 & 6 \\
     * 7 & 8 & 9 \\
     * \end{bmatrix}
     * \f]
     *
     * \f[ B = -A \f]
     *
     * B=\begin{bmatrix}
     * -1 & -2 & -3 \\
     * -4 & -5 & -6 \\
     * -7 & -8 & -9 \\
     * \end{bmatrix}
     * \f]
     *
     * \return Matriz con todos los elementos de la matriz de entrada cambiados de signo
     */
    auto operator-() -> Matrix<T, _rows, _cols>;

    /* Binary arithmetic operators */
    
    /*!
     * \brief Suma o adición de matrices
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
     * <h4>Ejemplo</h4>
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
    auto operator +(const MatrixDerived<T, _rows, _cols> &matrix2) -> Matrix<T, _rows, _cols>;
    
    template<typename MatrixDerived2>
    auto operator +(const MatrixDerived2 &matrix2) -> MatrixDerived<T, _rows, _cols>;

    /*!
     * \brief Adición a una matriz
     *
     * Adicción de una matriz a otra
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
     * <h4>Ejemplo</h4>
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
    auto operator +=(const MatrixDerived2 &matrix) -> MatrixDerived<T, _rows, _cols> &;

    /*!
     * \brief Resta de matrices
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
     * <h4>Ejemplo</h4>
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
    auto operator -(const MatrixDerived<T, _rows, _cols> &matrix2) -> Matrix<T, _rows, _cols>;
    
    template<typename MatrixDerived2>
    auto operator -(const MatrixDerived2 &matrix2) -> MatrixDerived<T, _rows, _cols>;

    /*!
     * \brief Resta de una matriz por otra
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
     * <h4>Ejemplo</h4>
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
    auto operator -=(const MatrixDerived2 &matrix) -> MatrixDerived<T, _rows, _cols>&;

    /*!
     * \brief Multiplicación de una matriz por un escalar
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
     * <h4>Ejemplo</h4>
     * \code
     * Matrix2x2i A{1, 4,
     *              3, 2};
     *
     * int s = 2;
     * Matrix2x2i C = A * s;
     * \endcode
     */
    auto operator *(T scalar) -> Matrix<T, _rows, _cols>;

    /*!
     * \brief Multiplicación de un escalar por una matriz
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
     * <h4>Ejemplo</h4>
     * \code
     * Matrix2x2i A{1, 4,
     *              3, 2};
     * int s = 2;
     * A *= s;
     * \endcode
     */
    auto operator *=(T scalar) -> MatrixDerived<T, _rows, _cols>&;

    /*!
     * \brief División de una matriz por un escalar
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
     * <h4>Ejemplo</h4>
     * \code
     * Matrix2x2i A{1.f, 4.f,
     *              3.f, 2.f};
     *
     * float s = 2.f;
     * Matrix2x2f C = A / s;
     * \endcode
     */
    auto operator /(T scalar) -> Matrix<T, _rows, _cols>;

    /*!
     * \brief División de un escalar por una matriz
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
     * <h4>Ejemplo</h4>
     * \code
     * Matrix2x2i A{1, 4,
     *              3, 2};
     * int s = 2;
     * A *= s;
     * \endcode
     */
    auto operator /=(T scalar) -> MatrixDerived<T, _rows, _cols> &;

    /// Tendria que ser una clase Vector al igual que MatrixCol y MatrixRow
    /// Por ahora construyo un vector
    auto diagonal() const -> Vector<T>; 

protected:

    MatrixDerived<T, _rows, _cols> &derived();
    const MatrixDerived<T, _rows, _cols> &derived() const;

protected:

    template<typename MatrixDerived2>
    void set(const MatrixDerived2 &matrix);

    T determinant2x2() const;
    T determinant3x3() const;
    T determinant4x4() const;
    T determinantnxn() const;
 
public:

    EnumFlags<Properties> properties;

};




template<typename T, size_t _rows = DynamicData, size_t _cols = DynamicData>
class Matrix
  : public MatrixBase<Matrix<T, _rows, _cols>>
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
        size = DataSize<T, _rows, _cols>::size()
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
     * \param[in] value Matrix columns
     */
    Matrix(size_t rows, size_t cols, T value);

    /*!
     * \brief Constructora de copia
     * \param[in] mat Objeto Matrix que se copia
     */
    Matrix(const Matrix &mat);

    template<typename MatrixDerived>
    Matrix(const MatrixDerived &matrix);
    /*!
     * \brief Constructor de movimiento
     * \param[in] mat Objeto Matrix que se mueve
     */
    Matrix(Matrix &&mat) TL_NOEXCEPT;

    Matrix(std::initializer_list<T> values);
    Matrix(std::initializer_list<std::initializer_list<T>> values);
    Matrix(const T *data, size_t rows, size_t cols);

    /*!
     * \brief destructora
     */
    ~Matrix() override = default;

    /*!
     * \brief Operador de asignación de copia
     * \param[in] matrix Objeto que se copia
     */
    auto operator = (const Matrix &matrix)->Matrix &;

    /*!
     * \brief Operador de asignación de movimiento
     * \param[in] matrix Objeto que se mueve
     */
    auto operator = (Matrix &&matrix) TL_NOEXCEPT->Matrix &;

    operator Matrix<T, DynamicData, DynamicData>() const;

    /*!
     * \brief Número de filas de la matriz
     * \return Número de filas
     */
    size_t rows() const
    {
        return mRows;
    }

    /*!
     * \brief Número de columnas de la matriz
     * \return Número de columnas
     */
    size_t cols() const
    {
        return mCols;
    }

    /*!
     * \brief Matriz inversa
     * Una matriz cuadrada e invertible A tiene una matriz inversa \f[ A^{-1} \f]
     * \param[out] invertibility Comprueba si la matriz es invertible
     * \return Matriz inversa
     * <h4>Ejemplo</h4>
     * \code
     * Matrix<double, 2, 2> mat_2x2{2., 3.
     *                              1., 4.};
     * bool invertible;
     * Matrix<double, 2, 2> inv_mat = mat_2x2.inverse(&invertible);
     * \endcode
     */
    auto inverse(bool *invertibility = nullptr) const->Matrix;

    /*!
     * \brief Calcula la matriz transpuesta
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
     * <h4>Ejemplo</h4>
    * \code
     * Matrix<double, 2, 2> mat_2x2{2., 3.
     *                              1., 4.};
     * Matrix<double, 2, 2> transpose_mat = mat_2x2.transpose();
     * std::cout << transpose_mat << std::endl;
     *
     * \endcode
     *
     * \return Matriz transpuesta
     */
    auto transpose() const->Matrix<T, _cols, _rows>;


    /*!
     * \brief Calcula la matriz de adjuntos
     * \f[ adj(A) = C^T \f]
     * \return Matriz de adjuntos
     */
    auto adjugate() const->Matrix;

    /*!
     * \brief Calcula la matriz cofactor
     * \return Matriz cofactor
     */
    auto cofactorMatrix() const->Matrix;

    /*!
     * \brief Traza de una matriz cuadrada
     * Suma de los elementos de la diagonal principal de una matriz cuadrada
     * \f[ tr(A) = a_{11} + ... +  a_{nn} \f]
     */
    auto trace() const->T;

    /*!
     * \brief Comprueba si la matrix es invertible
     * \return Verdadero si la matriz es invertible
     */
    auto invertible() -> bool;

    /*!
     * \brief Comprueba si la matrix es singular
     * Una matriz cuadrada que no tiene inversa es singular. El
     * determinante de una matriz singular es 0
     * \return Verdadero si la matriz es singular
     */
    auto singular() -> bool;

    /*!
     * \brief Cofactor
     * El determinante obtenido al eliminar la fila y la columna de un elemento dado de una matriz o determinante.
     * El cofactor está precedido por un signo + o - dependiendo de si el elemento está en una posición + o -.
     * \f[ (-)^{r+j} \f]
     * \return cofactor
     */
    auto cofactor(size_t r, size_t c) const->T;

    /*!
     * \brief Primer menor
     * Un menor de una matriz cuadrada A es el determinante de alguna de las
     * submatrices obtenidas a partir de la eliminación de una filas y una columna.
     * Se utilizan para el cálculo de la matriz de cofactores.
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
     * \return Primer menor
     */
    auto firstMinor(size_t row, size_t col) const->T;

    /*!
     * \brief Forma escalonada de fila
     */
    auto rowEchelonForm() const->Matrix;

    /*!
     * \brief Forma escalonada de fila reducida
     */
    auto reducedRowEchelonForm() const->Matrix;

    /*!
     * \brief Rango de una matriz
     */
    auto rank() const -> int;

    /*!
     * \brief Intercambia dos filas
     * \param[in] i Primera fila a intercambiar
     * \param[in] j Segunda fila a intercambiar
     */
    auto swapRows(size_t i, size_t j) -> void;

    /*!
     * \brief Referencia al elemento en la posición fila (r) y columna (c)
     * \param[in] r Fila de la matriz
     * \param[in] c Columna de la matriz
     * \return Valor de la matriz en la posición fila y columna
     * <h4>Ejemplo</h4>
     * \code
     * Matrix<double,3,3> matrix;
     * matrix.at(0, 0) = 1.5;
     * double value = matrix.at(0, 0);
     * \endcode
     */
    auto at(size_t r, size_t c) -> reference;

    /*!
     * \brief Referencia constante al elemento en la posición fila (r) y columna (c)
     * \param[in] r Fila
     * \param[in] c Columna
     * \return Valor de la matriz en la posición fila y columna
     * <h4>Ejemplo</h4>
     * \code
     * double value = matrix.at(0, 0);
     * \endcode
     */
    auto at(size_t r, size_t c) const->const_reference;

    /*!
     * \brief Referencia al elemento en la posición fila (r) y columna (c)
     * \param[in] r Fila de la matriz
     * \param[in] c Columna de la matriz
     * \return Valor de la matriz en la posición fila y columna
     * <h4>Ejemplo</h4>
     * \code
     * Matrix<double,3,3> matrix;
     * matrix(0, 0) = 1.5;
     * double value = matrix(0, 0);
     * \endcode
     */
    auto operator()(size_t r, size_t c)->reference;

    /*!
     * \brief Referencia constante al elemento en la posición fila (r) y columna (c)
     * \param[in] r Fila
     * \param[in] c Columna
     * \return Valor de la matriz en la posición fila y columna
     * <h4>Ejemplo</h4>
     * \code
     * double value = matrix(0, 0);
     * \endcode
     */
    auto operator()(size_t r, size_t c) const->const_reference;

    /*!
     * \brief Referencia al elemento
     * La posición del elemento se determina como:
     *   r * this->cols() + c
     * \param[in] position Posición del elemento de la matriz
     * \return Valor de la matriz en dicha posición
     * <h4>Ejemplo</h4>
     * \code
     * Matrix<double,3,3> matrix;
     * matrix(4) = 1.5;
     * double value = matrix(4); // value == 1.5
     * \endcode
     */
    auto operator()(size_t position)->reference;

    /*!
     * \brief Referencia constante al elemento
     * La posición del elemento se determina como:
     *   r * this->cols() + c
     * \param[in] position Posición del elemento de la matriz
     * \return Valor de la matriz en dicha posición
     * <h4>Ejemplo</h4>
     * \code
     * Matrix<double,3,3> matrix;
     * matrix(4) = 1.5;
     * double value = matrix(4); // value == 1.5
     * \endcode
     */
    auto operator()(size_t position) const->const_reference;

    auto operator[](size_t position) const -> const internal::MatrixRow<const T>;
    auto operator[](size_t position)->internal::MatrixRow<T>;

    auto row(size_t row) const -> const internal::MatrixRow<const T>;
    auto row(size_t row) -> internal::MatrixRow<T>;

    auto col(size_t col) const -> const internal::MatrixCol<const T>;
    auto col(size_t col) -> internal::MatrixCol<T>;

    auto block(size_t iniRow, size_t endRow, size_t iniCol, size_t endCol) const -> const internal::MatrixBlock<const T, DynamicData, DynamicData>;
    auto block(size_t iniRow, size_t endRow, size_t iniCol, size_t endCol) -> internal::MatrixBlock<T>;

    auto rowBlock(size_t iniRow, size_t endRow) const -> const internal::MatrixBlock<const T, DynamicData, DynamicData>;
    auto rowBlock(size_t iniRow, size_t endRow) -> internal::MatrixBlock<T, DynamicData, DynamicData>;
    auto colBlock(size_t iniCol, size_t endCol) const -> const internal::MatrixBlock<const T, DynamicData, DynamicData>;
    auto colBlock(size_t iniCol, size_t endCol) -> internal::MatrixBlock<T, DynamicData, DynamicData>;

    /*!
     * \brief Construye una matriz de ceros
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
     * \brief Construye una matriz de 'unos'
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
     * \brief Construye la matriz identidad
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
     * \brief Construye una matriz con valores aleatorios
     */
    static auto randon() -> Matrix;
    static auto randon(size_t rows, size_t cols) -> Matrix;

    auto data() -> pointer;
    auto data() const->const_pointer;

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

    Data<T, size> _data;
    size_t mRows{_rows};
    size_t mCols{_cols};
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
template<typename T, size_t _rows1, size_t _col1, size_t _rows2, size_t _cols2, size_t _rows3, size_t _cols3> inline
void mulmat_simd(const Matrix<T, _rows1, _col1> &matrix1,
                 const Matrix<T, _rows2, _cols2> &matrix2,
                 Matrix<T, _rows3, _cols3> &matrix)
{
    TL_ASSERT(matrix1.cols() == matrix2.rows(), "A columns != B rows");
    TL_ASSERT(matrix1.rows() == matrix.rows(), "C rows != A rows");
    TL_ASSERT(matrix2.cols() == matrix.cols(), "B columns != C columns");

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

                packed_b.loadUnaligned(&matrix2(i, c));

                packed_c.loadUnaligned(&matrix(r, c));
                packed_c += packed_a1 * packed_b;
                packed_c.storeUnaligned(&matrix(r, c));

                packed_c.loadUnaligned(&matrix(r + 1, c));
                packed_c += packed_a2 * packed_b;
                packed_c.storeUnaligned(&matrix(r + 1, c));

                packed_c.loadUnaligned(&matrix(r + 2, c));
                packed_c += packed_a3 * packed_b;
                packed_c.storeUnaligned(&matrix(r + 2, c));

                packed_c.loadUnaligned(&matrix(r + 3, c));
                packed_c += packed_a4 * packed_b;
                packed_c.storeUnaligned(&matrix(r + 3, c));

                packed_c.loadUnaligned(&matrix(r + 4, c));
                packed_c += packed_a5 * packed_b;
                packed_c.storeUnaligned(&matrix(r + 4, c));

                packed_c.loadUnaligned(&matrix(r + 5, c));
                packed_c += packed_a6 * packed_b;
                packed_c.storeUnaligned(&matrix(r + 5, c));

                packed_c.loadUnaligned(&matrix(r + 6, c));
                packed_c += packed_a7 * packed_b;
                packed_c.storeUnaligned(&matrix(r + 6, c));

                packed_c.loadUnaligned(&matrix(r + 7, c));
                packed_c += packed_a8 * packed_b;
                packed_c.storeUnaligned(&matrix(r + 7, c));

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

                packed_b.loadUnaligned(&matrix2(i, c));

                packed_c.loadUnaligned(&matrix(r, c));
                packed_c += packed_a * packed_b;
                packed_c.storeUnaligned(&matrix(r, c));
            }

            for (size_t c = max_vector; c < cols; c++) {
                matrix(r, c) += a * matrix2(i, c);
            }

        }
    }
}

template<typename T, size_t _rows, size_t _dim, size_t _cols> inline
void mulmat_simd_parallel(const Matrix<T, _rows, _dim> &matrix1,
                          const Matrix<T, _dim, _cols> &matrix2,
                          Matrix<T, _rows, _cols> &matrix)
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

  auto f_aux = [&](size_t ini, size_t end) {

    T b{};

    for (size_t r = ini; r < end; r += 8) {
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

          packed_b.loadUnaligned(&matrix2(i, c));

          packed_c.loadUnaligned(&matrix(r, c));
          packed_c += packed_a1 * packed_b;
          packed_c.storeUnaligned(&matrix(r, c));

          packed_c.loadUnaligned(&matrix(r + 1, c));
          packed_c += packed_a2 * packed_b;
          packed_c.storeUnaligned(&matrix(r + 1, c));

          packed_c.loadUnaligned(&matrix(r + 2, c));
          packed_c += packed_a3 * packed_b;
          packed_c.storeUnaligned(&matrix(r + 2, c));

          packed_c.loadUnaligned(&matrix(r + 3, c));
          packed_c += packed_a4 * packed_b;
          packed_c.storeUnaligned(&matrix(r + 3, c));

          packed_c.loadUnaligned(&matrix(r + 4, c));
          packed_c += packed_a5 * packed_b;
          packed_c.storeUnaligned(&matrix(r + 4, c));

          packed_c.loadUnaligned(&matrix(r + 5, c));
          packed_c += packed_a6 * packed_b;
          packed_c.storeUnaligned(&matrix(r + 5, c));

          packed_c.loadUnaligned(&matrix(r + 6, c));
          packed_c += packed_a7 * packed_b;
          packed_c.storeUnaligned(&matrix(r + 6, c));

          packed_c.loadUnaligned(&matrix(r + 7, c));
          packed_c += packed_a8 * packed_b;
          packed_c.storeUnaligned(&matrix(r + 7, c));

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
  };

  /// iter es el número ideal de hilos
  size_t one_thread_per_block = iter / 8;
  size_t num_threads = optimalNumberOfThreads();
  size_t block_size = one_thread_per_block / num_threads;
  if (block_size > 0) {
    block_size *= 8;
  } else {
    num_threads = one_thread_per_block;
  }

  std::vector<std::thread> threads(num_threads);

  // El tamaño del bloque tiene que ser siempre multiplo de 8

  size_t block_ini = 0;
  size_t block_end = 0;

  for (size_t i = 0; i < num_threads; i++) {

    if (i == num_threads - 1) block_end = iter;
    else block_end = block_ini + block_size;

    threads[i] = std::thread(f_aux, block_ini, block_end);

    block_ini = block_end;
  }

  /// Lo puedo sacar fuera e incluir en otro hilo el proceso sin SIMD
  for (auto &_thread : threads) {
    if (_thread.joinable())
      _thread.join();
  }

  //T b{};

  //for (size_t r = 0; r < iter; r += 8) {
  //  for (size_t i = 0; i < dim; i++) {

  //    packed_a1.setScalar(matrix1(r, i));
  //    packed_a2.setScalar(matrix1(r + 1, i));
  //    packed_a3.setScalar(matrix1(r + 2, i));
  //    packed_a4.setScalar(matrix1(r + 3, i));
  //    packed_a5.setScalar(matrix1(r + 4, i));
  //    packed_a6.setScalar(matrix1(r + 5, i));
  //    packed_a7.setScalar(matrix1(r + 6, i));
  //    packed_a8.setScalar(matrix1(r + 7, i));

  //    for (size_t c = 0; c < max_vector; c += packed_size) {

  //      packed_b.loadUnaligned(&matrix2(i, c));

  //      packed_c.loadUnaligned(&matrix(r, c));
  //      packed_c += packed_a1 * packed_b;
  //      packed_c.storeUnaligned(&matrix(r, c));

  //      packed_c.loadUnaligned(&matrix(r + 1, c));
  //      packed_c += packed_a2 * packed_b;
  //      packed_c.storeUnaligned(&matrix(r + 1, c));

  //      packed_c.loadUnaligned(&matrix(r + 2, c));
  //      packed_c += packed_a3 * packed_b;
  //      packed_c.storeUnaligned(&matrix(r + 2, c));

  //      packed_c.loadUnaligned(&matrix(r + 3, c));
  //      packed_c += packed_a4 * packed_b;
  //      packed_c.storeUnaligned(&matrix(r + 3, c));

  //      packed_c.loadUnaligned(&matrix(r + 4, c));
  //      packed_c += packed_a5 * packed_b;
  //      packed_c.storeUnaligned(&matrix(r + 4, c));

  //      packed_c.loadUnaligned(&matrix(r + 5, c));
  //      packed_c += packed_a6 * packed_b;
  //      packed_c.storeUnaligned(&matrix(r + 5, c));

  //      packed_c.loadUnaligned(&matrix(r + 6, c));
  //      packed_c += packed_a7 * packed_b;
  //      packed_c.storeUnaligned(&matrix(r + 6, c));

  //      packed_c.loadUnaligned(&matrix(r + 7, c));
  //      packed_c += packed_a8 * packed_b;
  //      packed_c.storeUnaligned(&matrix(r + 7, c));

  //    }

  //    for (size_t c = max_vector; c < cols; c++) {

  //      b = matrix2(i, c);
  //      matrix(r, c) += matrix1(r, i) * b;
  //      matrix(r + 1, c) += matrix1(r + 1, i) * b;
  //      matrix(r + 2, c) += matrix1(r + 2, i) * b;
  //      matrix(r + 3, c) += matrix1(r + 3, i) * b;
  //      matrix(r + 4, c) += matrix1(r + 4, i) * b;
  //      matrix(r + 5, c) += matrix1(r + 5, i) * b;
  //      matrix(r + 6, c) += matrix1(r + 6, i) * b;
  //      matrix(r + 7, c) += matrix1(r + 7, i) * b;
  //    }

  //  }
  //}

  for (size_t r = iter; r < rows; r++) {
    for (size_t i = 0; i < dim; i++) {

      T a = matrix1(r, i);
      packed_a.setScalar(a);

      for (size_t c = 0; c < max_vector; c += packed_size) {

        packed_b.loadUnaligned(&matrix2(i, c));

        packed_c.loadUnaligned(&matrix(r, c));
        packed_c += packed_a * packed_b;
        packed_c.storeUnaligned(&matrix(r, c));
      }

      for (size_t c = max_vector; c < cols; c++) {
        matrix(r, c) += a * matrix2(i, c);
      }

    }
  }
}

#endif // TL_HAVE_SIMD_INTRINSICS

template<typename T, size_t _rows1, size_t _col1, size_t _rows2, size_t _cols2, size_t _rows3, size_t _cols3> inline
void mulmat_cpp(const Matrix<T, _rows1, _col1> &matrix1,
                const Matrix<T, _rows2, _cols2> &matrix2,
                Matrix<T, _rows3, _cols3> &matrix)
{
    TL_ASSERT(matrix1.cols() == matrix2.rows(), "A columns != B rows");
    TL_ASSERT(matrix1.rows() == matrix.rows(), "C rows != A rows");
    TL_ASSERT(matrix2.cols() == matrix.cols(), "B columns != C columns");

    for (size_t r = 0; r < matrix1.rows(); r++) {
        for (size_t i = 0; i < matrix1.cols(); i++) {
            T a = matrix1(r, i);
            for (size_t c = 0; c < matrix2.cols(); c++) {
                matrix(r, c) += a * matrix2(i, c);
            }
        }
    }
}

//template<typename T, size_t _rows, size_t _dim, size_t _cols> inline
template<typename T, size_t _rows1, size_t _col1, size_t _rows2, size_t _cols2, size_t _rows3, size_t _cols3> inline

typename std::enable_if <
  std::is_integral<T>::value,
  void>::type
mulmat(const Matrix<T, _rows1, _col1> &matrix1,
       const Matrix<T, _rows2, _cols2> &matrix2,
       Matrix<T, _rows3, _cols3> &matrix)
{
#if defined TL_HAVE_SIMD_INTRINSICS

    mulmat_simd(matrix1, matrix2, matrix);
    //mulmat_simd_parallel(matrix1, matrix2, matrix);

#else 

    mulmat_cpp(matrix1, matrix2, matrix);

#endif
}

template<typename T, size_t _rows1, size_t _col1, size_t _rows2, size_t _cols2, size_t _rows3, size_t _cols3> inline
typename std::enable_if <
  std::is_floating_point<T>::value,
  void>::type
mulmat(const Matrix<T, _rows1, _col1> &matrix1,
       const Matrix<T, _rows2, _cols2> &matrix2,
       Matrix<T, _rows3, _cols3> &matrix)
{

#ifdef TL_HAVE_OPENBLAS

    blas::gemm(static_cast<int>(matrix1.rows()),
               static_cast<int>(matrix1.cols()),
               static_cast<int>(matrix1.cols()),
               matrix1.data(), matrix2.data(), matrix.data());

#elif defined TL_HAVE_SIMD_INTRINSICS

    mulmat_simd(matrix1, matrix2, matrix);
    //mulmat_simd_parallel(matrix1, matrix2, matrix);

#else 

    mulmat_cpp(matrix1, matrix2, matrix);

#endif
}




/*------------------------------------------------------------------------*/
/* Implementation                                                         */
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------*/
/* IteratorRows implementation                                            */
/*------------------------------------------------------------------------*/

template<typename T>
inline IteratorRows<T>::IteratorRows(pointer ptr)
  : rowPtr(ptr)
{
}

template<typename T>
inline auto IteratorRows<T>::operator*() const -> reference
{
    return *rowPtr;
}

template<typename T>
inline auto IteratorRows<T>::operator->() -> pointer
{
    return rowPtr;
}

template<typename T>
inline auto IteratorRows<T>::operator++() -> IteratorRows&
{
    rowPtr++;
    return *this;
}

template<typename T>
inline auto IteratorRows<T>::operator++(int) -> IteratorRows
{
    IteratorRows it = *this;
    ++(*this);
    return it;
}

template<typename T>
inline bool IteratorRows<T>::operator == (const IteratorRows<T> &itRow)
{
    return this->rowPtr == itRow.rowPtr;
}

template<typename T>
inline bool IteratorRows<T>::operator != (const IteratorRows<T> &itRow)
{
    return this->rowPtr != itRow.rowPtr;
}


/*------------------------------------------------------------------------*/
/* IteratorRows implementation                                            */
/*------------------------------------------------------------------------*/



template<typename T>
inline IteratorCols<T>::IteratorCols(pointer ptr, size_t colSize)
  : colPtr(ptr),
    colSize(colSize)
{
}

template<typename T>
inline auto IteratorCols<T>::operator*() const -> reference
{
    return *colPtr;
}

template<typename T>
inline auto IteratorCols<T>::operator->() -> pointer
{
    return colPtr;
}

template<typename T>
inline auto IteratorCols<T>::operator++() -> IteratorCols&
{
    colPtr += colSize;
    return *this;
}

template<typename T>
inline auto IteratorCols<T>::operator++(int) -> IteratorCols
{
    IteratorCols it = *this;
    ++(*this);
    return it;
}

template<typename T>
inline bool IteratorCols<T>::operator == (const IteratorCols<T> &itCol)
{
    return this->colPtr == itCol.colPtr;
}

template<typename T>
inline bool IteratorCols<T>::operator != (const IteratorCols<T> &itCol)
{
    return this->colPtr != itCol.colPtr;
}



/*------------------------------------------------------------------------*/
/* MatrixRow implementation                                               */
/*------------------------------------------------------------------------*/

template<typename T, size_t _size_>
MatrixRow<T, _size_>::MatrixRow(T *data, size_t row, size_t cols)
  : matrixData(data),
    matrixRow(row),
    matrixCols(cols)
{
    this->properties.enable(MatrixRow<T, _size_>::Properties::contiguous_memory);
}

template<typename T, size_t _size_>
inline auto MatrixRow<T, _size_>::begin() TL_NOEXCEPT -> iterator
{
    return iterator(&matrixData[matrixRow * matrixCols]);
}

template<typename T, size_t _size_>
inline auto MatrixRow<T, _size_>::begin() const TL_NOEXCEPT -> const_iterator
{
    return iterator(&matrixData[matrixRow * matrixCols]);
}

template<typename T, size_t _size_>
inline auto MatrixRow<T, _size_>::end() TL_NOEXCEPT -> iterator
{
    return iterator(&matrixData[matrixRow * matrixCols] + matrixCols);
}

template<typename T, size_t _size_>
inline auto MatrixRow<T, _size_>::end() const TL_NOEXCEPT -> const_iterator
{
  return iterator(&matrixData[matrixRow * matrixCols] + matrixCols);
}

template<typename T, size_t _size_>
inline auto MatrixRow<T, _size_>::size() const TL_NOEXCEPT -> size_t
{
    return matrixCols;
}

template<typename T, size_t _size_>
inline auto MatrixRow<T, _size_>::operator[](size_t column) const -> const_reference
{
    return matrixData[matrixRow * matrixCols + column];
}

template<typename T, size_t _size_>
inline auto MatrixRow<T, _size_>::operator[](size_t column) -> reference
{
    return matrixData[matrixRow * matrixCols + column];
}

template<typename T, size_t _size_>
inline auto MatrixRow<T, _size_>::operator=(T value) -> void
{
    std::fill(begin(), end(), value);
}

template<typename T, size_t _size_>
inline auto MatrixRow<T, _size_>::operator=(const Vector<T> &vector) -> MatrixRow&
{
    TL_ASSERT(vector.size() == size(), "Invalid vector size");
    
    for(size_t i = 0; i < size(); i++)
        (*this)[i] = vector[i];
    
    return *this;
}

template<typename T, size_t _size_>
template<typename T2, size_t _size2>
inline auto MatrixRow<T, _size_>::operator=(const Vector<T2, _size2> &vector) -> MatrixRow &
{
    TL_ASSERT(this->size() == vector.size(), "A size != B size");

    for(size_t i = 0; i < this->size(); i++) {
        (*this)(i) = static_cast<T>(vector(i));
    }

    return *this;
}

template<typename T, size_t _size_>
inline MatrixRow<T, _size_>::operator Vector<T>() const
{
    Vector<T> vector(this->size());

    for(size_t i = 0; i < this->size(); i++) {
        vector[i] = (*this)[i];
    }

    return vector;
}




/*------------------------------------------------------------------------*/
/* MatrixCol implementation                                               */
/*------------------------------------------------------------------------*/

template<typename T, size_t _size_>
MatrixCol<T, _size_>::MatrixCol(T *data, size_t col, size_t rows, size_t cols)
  : matrixData(data),
    matrixCol(col),
    matrixRows(rows),
    matrixCols(cols)
{
    this->properties.disable(MatrixCol<T, _size_>::Properties::contiguous_memory);
}

template<typename T, size_t _size_>
inline auto MatrixCol<T, _size_>::begin() TL_NOEXCEPT -> iterator
{
    return iterator(&matrixData[matrixCol], matrixCols);
}

template<typename T, size_t _size_>
inline auto MatrixCol<T, _size_>::begin() const TL_NOEXCEPT -> const_iterator
{
    return iterator(&matrixData[matrixCol], matrixCols);
}

template<typename T, size_t _size_>
inline auto MatrixCol<T, _size_>::end() TL_NOEXCEPT -> iterator
{
    return iterator(&matrixData[matrixCol] + matrixRows * matrixCols, matrixCols);
}

template<typename T, size_t _size_>
inline auto MatrixCol<T, _size_>::end() const TL_NOEXCEPT -> const_iterator
{
    return iterator(&matrixData[matrixCol] + matrixRows * matrixCols, matrixCols);
}

template<typename T, size_t _size_>
inline auto MatrixCol<T, _size_>::size() const TL_NOEXCEPT -> size_t
{
    return matrixRows;
}

template<typename T, size_t _size_>
inline auto MatrixCol<T, _size_>::operator[](size_t row) const -> const_reference
{
    return matrixData[row * matrixCols + matrixCol];
}

template<typename T, size_t _size_>
inline auto MatrixCol<T, _size_>::operator[](size_t row) -> reference
{
    return matrixData[row * matrixCols + matrixCol];
}

template<typename T, size_t _size_>
inline auto MatrixCol<T, _size_>::operator=(T value) -> void
{
    std::fill(begin(), end(), value);
}

template<typename T, size_t _size_>
inline auto MatrixCol<T, _size_>::operator=(const Vector<T> &vector) -> MatrixCol&
{
    TL_ASSERT(vector.size() == size(), "Invalid vector size");

    for(size_t i = 0; i < size(); i++)
        (*this)[i] = vector[i];

    return *this;
}

template<typename T, size_t _size_>
template<typename T2, size_t _size2>
inline auto MatrixCol<T, _size_>::operator = (const Vector<T2, _size2> &vector) -> MatrixCol&
{
    TL_ASSERT(this->size() == vector.size(), "A size != B size");

    for(size_t i = 0; i < this->size(); i++) {
        (*this)(i) = static_cast<T>(vector(i));
    }

    return *this;
}

template<typename T, size_t _size_>
inline MatrixCol<T, _size_>::operator Vector<T>() const
{
    Vector<T> vector(this->size());

    for(size_t i = 0; i < this->size(); i++) {
        vector[i] = (*this)[i];
    }

    return vector;
}



/*------------------------------------------------------------------------*/
/* MatrixBlock implementation                                             */
/*------------------------------------------------------------------------*/

template<typename T, size_t _rows, size_t _cols>
inline MatrixBlock<T, _rows, _cols>::MatrixBlock(T *data,
                                                 size_t rows,
                                                 size_t cols,
                                                 size_t iniRow,
                                                 size_t endRow,
                                                 size_t iniCol,
                                                 size_t endCol)
  : matrixData(data),
    matrixRows(rows),
    matrixCols(cols),
    matrixIniRow(iniRow),
    matrixEndRow(endRow),
    matrixIniCol(iniCol),
    matrixEndCol(endCol)
{
    this->properties.disable(MatrixBlock<T, _rows, _cols>::Properties::contiguous_memory);
}

template<typename T, size_t _rows, size_t _cols>
auto MatrixBlock<T, _rows, _cols>::operator=(const MatrixBlock &block) -> MatrixBlock&
{
    size_t rows = this->rows();
    size_t cols = this->cols();
    size_t rows2 = block.rows();
    size_t cols2 = block.cols();

    TL_ASSERT(rows == rows2 && cols == cols2, "A size != B size");

    Rect<int> rect1(this->matrixIniCol, cols, this->cols(), rows);
    Rect<int> rect2(block.matrixIniCol, block.matrixIniRow, cols2, rows2);
    Rect<int> intersect = tl::intersect(rect1, rect2);

    if(this->matrixData == block.matrixData && intersect.isValid()) {

        Matrix<T> mat = block;

        for(size_t r = 0; r < this->rows(); r++) {
            for(size_t c = 0; c < this->cols(); c++) {
                (*this)(r, c) = mat(r, c);
            }
        }

    } else {

        for(size_t r = 0; r < this->rows(); r++) {
            for(size_t c = 0; c < this->cols(); c++) {
                (*this)(r, c) = block(r, c);
            }
        }

    }

    return *this;
}

template<typename T, size_t _rows, size_t _cols>
template<typename T2, size_t _rows2, size_t _cols2>
auto MatrixBlock<T, _rows, _cols>::operator=(const Matrix<T2, _rows2, _cols2> &matrix) -> MatrixBlock&
{
    size_t rows = this->rows();
    size_t cols = this->cols();
    size_t rows2 = matrix.rows();
    size_t cols2 = matrix.cols();

    TL_ASSERT(rows == rows2 && cols == cols2, "A size != B size");

    for(size_t r = 0; r < this->rows(); r++) {
        for(size_t c = 0; c < this->cols(); c++) {
            (*this)(r, c) = static_cast<T>(matrix(r, c));
        }
    }

    return *this;
}

template<typename T, size_t _rows, size_t _cols>
inline auto MatrixBlock<T, _rows, _cols>::at(size_t r, size_t c) -> reference
{
    if(matrixEndRow - matrixIniRow < r || matrixEndCol - matrixIniCol < c) throw std::out_of_range("Matrix block out of range");

    return (*this)(r, c);
}

template<typename T, size_t _rows, size_t _cols>
inline auto MatrixBlock<T, _rows, _cols>::at(size_t r, size_t c) const -> const_reference
{
    if(matrixEndRow - matrixIniRow < r || matrixEndCol - matrixIniCol < c) throw std::out_of_range("Matrix block out of range");

    return (*this)(r, c);
}

template<typename T, size_t _rows, size_t _cols>
inline auto MatrixBlock<T, _rows, _cols>::operator()(size_t row, size_t col) -> reference
{
    return matrixData[(matrixIniRow + row) * matrixCols + col + matrixIniCol];
}

template<typename T, size_t _rows, size_t _cols>
inline auto MatrixBlock<T, _rows, _cols>::operator()(size_t row, size_t col) const -> const_reference
{
    return matrixData[(matrixIniRow + row) * matrixCols + col + matrixIniCol];
}

template<typename T, size_t _rows, size_t _cols>
inline auto MatrixBlock<T, _rows, _cols>::operator()(size_t position) -> reference
{
    size_t col = position % cols();
    size_t row = position / cols();

    return (*this)(row, col);
}

template<typename T, size_t _rows, size_t _cols>
inline auto MatrixBlock<T, _rows, _cols>::operator()(size_t position) const -> const_reference
{
    size_t col = position % cols();
    size_t row = position / cols();

    return (*this)(row, col);
}

template<typename T, size_t _rows, size_t _cols>
inline auto MatrixBlock<T, _rows, _cols>::rows() const -> size_t
{
    return matrixEndRow - matrixIniRow + 1;
}

template<typename T, size_t _rows, size_t _cols>
inline auto MatrixBlock<T, _rows, _cols>::cols() const -> size_t
{
    return matrixEndCol - matrixIniCol + 1;
}

template<typename T, size_t _rows, size_t _cols>
MatrixBlock<T, _rows, _cols>::operator Matrix<T>() const
{
    Matrix<T> matrix(this->rows(), this->cols());

    for(size_t r = 0; r < this->rows(); r++) {
        for(size_t c = 0; c < this->cols(); c++) {
            matrix(r, c) = (*this)(r, c);
        }
    }

    return matrix;
}



} // namespace internal 

/// \endcond


/*------------------------------------------------------------------------*/
/* MatrixBase implementation                                              */
/*------------------------------------------------------------------------*/


template<
  template<typename, size_t _rows = DynamicData, size_t _cols = DynamicData>
  class MatrixDerived, typename T, size_t _rows, size_t _cols>
MatrixBase<MatrixDerived<T, _rows, _cols>>::MatrixBase()
{
    this->properties.enable(Properties::contiguous_memory);
}

template<
  template<typename, size_t _rows = DynamicData, size_t _cols = DynamicData>
  class MatrixDerived, typename T, size_t _rows, size_t _cols>
inline auto MatrixBase<MatrixDerived<T, _rows, _cols>>::determinant() const -> T
{
    static_assert(_rows == _cols, "Non-Square Matrix");

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
  template<typename, size_t _rows = DynamicData, size_t _cols = DynamicData>
  class MatrixDerived, typename T, size_t _rows, size_t _cols>
inline auto MatrixBase<MatrixDerived<T, _rows, _cols>>::operator+() -> Matrix<T, _rows, _cols>
{
    return this->derived();
}

template<
  template<typename, size_t _rows = DynamicData, size_t _cols = DynamicData>
  class MatrixDerived, typename T, size_t _rows, size_t _cols>
auto MatrixBase<MatrixDerived<T, _rows, _cols>>::operator-() -> Matrix<T, _rows, _cols>
{
    static_assert(std::is_signed<T>::value, "Requires signed type");

    Matrix<T, _rows, _cols> matrix = this->derived();

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
  template<typename, size_t _rows = DynamicData, size_t _cols = DynamicData>
  class MatrixDerived, typename T, size_t _rows, size_t _cols>
inline auto MatrixBase<MatrixDerived<T, _rows, _cols>>::operator +(const MatrixDerived<T, _rows, _cols> &matrix2) -> Matrix<T, _rows, _cols>
{
    Matrix<T, _rows, _cols> matrix = this->derived();
    matrix += matrix2;
    return matrix;
}

template<
  template<typename, size_t _rows = DynamicData, size_t _cols = DynamicData>
  class MatrixDerived, typename T, size_t _rows, size_t _cols>
template<typename MatrixDerived2>
inline auto MatrixBase<MatrixDerived<T, _rows, _cols>>::operator +(const MatrixDerived2 &matrix2) -> MatrixDerived<T, _rows, _cols>
{
    MatrixDerived<T, _rows, _cols> matrix = this->derived();
    matrix += matrix2;
    return matrix;
}

template<
  template<typename, size_t _rows = DynamicData, size_t _cols = DynamicData>
  class MatrixDerived, typename T, size_t _rows, size_t _cols>
template<typename MatrixDerived2>
auto MatrixBase<MatrixDerived<T, _rows, _cols>>::operator +=(const MatrixDerived2 &matrix) -> MatrixDerived<T, _rows, _cols> &
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

    if(this->properties.isEnabled(MatrixDerived<T, _rows, _cols>::Properties::contiguous_memory) &&
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
  template<typename, size_t _rows = DynamicData, size_t _cols = DynamicData>
  class MatrixDerived, typename T, size_t _rows, size_t _cols>
auto MatrixBase<MatrixDerived<T, _rows, _cols>>::operator -(const MatrixDerived<T, _rows, _cols> &matrix2) -> Matrix<T, _rows, _cols>
{
    Matrix<T, _rows, _cols> matrix = this->derived();
    matrix -= matrix2;
    return matrix;
}

template<
  template<typename, size_t _rows = DynamicData, size_t _cols = DynamicData>
  class MatrixDerived, typename T, size_t _rows, size_t _cols>
template<typename MatrixDerived2>
auto MatrixBase<MatrixDerived<T, _rows, _cols>>::operator -(const MatrixDerived2 &matrix2) -> MatrixDerived<T, _rows, _cols>
{
    MatrixDerived<T, _rows, _cols> matrix = this->derived();
    matrix -= matrix2;
    return matrix;
}

template<
  template<typename, size_t _rows = DynamicData, size_t _cols = DynamicData>
  class MatrixDerived, typename T, size_t _rows, size_t _cols>
template<typename MatrixDerived2>
auto MatrixBase<MatrixDerived<T, _rows, _cols>>::operator -=(const MatrixDerived2 &matrix) -> MatrixDerived<T, _rows, _cols> &
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
  template<typename, size_t _rows = DynamicData, size_t _cols = DynamicData>
  class MatrixDerived, typename T, size_t _rows, size_t _cols>
inline auto MatrixBase<MatrixDerived<T, _rows, _cols>>::operator *(T scalar) -> Matrix<T, _rows, _cols>
{
    Matrix<T, _rows, _cols> matrix = this->derived();
    matrix *= scalar;
    return matrix;
}

template<
  template<typename, size_t _rows = DynamicData, size_t _cols = DynamicData>
  class MatrixDerived, typename T, size_t _rows, size_t _cols>
auto MatrixBase<MatrixDerived<T, _rows, _cols>>::operator *=(T scalar) -> MatrixDerived<T, _rows, _cols> &
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
  template<typename, size_t _rows = DynamicData, size_t _cols = DynamicData>
  class MatrixDerived, typename T, size_t _rows, size_t _cols>
auto MatrixBase<MatrixDerived<T, _rows, _cols>>::operator /(T scalar) -> Matrix<T, _rows, _cols>
{
    Matrix<T, _rows, _cols> _matrix = this->derived();
    _matrix /= scalar;
    return _matrix;
}

template<
  template<typename, size_t _rows = DynamicData, size_t _cols = DynamicData>
  class MatrixDerived, typename T, size_t _rows, size_t _cols>
auto MatrixBase<MatrixDerived<T, _rows, _cols>>::operator /=(T scalar) -> MatrixDerived<T, _rows, _cols> &
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
  template<typename, size_t _rows = DynamicData, size_t _cols = DynamicData>
  class MatrixDerived, typename T, size_t _rows, size_t _cols>
auto MatrixBase<MatrixDerived<T, _rows, _cols>>::diagonal() const -> Vector<T> 
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
  template<typename, size_t _rows = DynamicData, size_t _cols = DynamicData>
  class MatrixDerived, typename T, size_t _rows, size_t _cols>
MatrixDerived<T, _rows, _cols> &MatrixBase<MatrixDerived<T, _rows, _cols>>::derived()
{
    return *static_cast<MatrixDerived<T, _rows, _cols> *>(this);
}

template<
  template<typename, size_t _rows = DynamicData, size_t _cols = DynamicData>
  class MatrixDerived, typename T, size_t _rows, size_t _cols>
const MatrixDerived<T, _rows, _cols> &MatrixBase<MatrixDerived<T, _rows, _cols>>::derived() const
{
    return *static_cast<const MatrixDerived<T, _rows, _cols> *>(this);
}

template<
  template<typename, size_t _rows = DynamicData, size_t _cols = DynamicData>
  class MatrixDerived, typename T, size_t _rows, size_t _cols>
template<typename MatrixDerived2>
void MatrixBase<MatrixDerived<T, _rows, _cols>>::set(const MatrixDerived2 &matrix)
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
  template<typename, size_t _rows = DynamicData, size_t _cols = DynamicData>
  class MatrixDerived, typename T, size_t _rows, size_t _cols>
T MatrixBase<MatrixDerived<T, _rows, _cols>>::determinant2x2() const
{
    auto &derived = this->derived();
    
    T det = derived(0, 0) * derived(1, 1) - derived(0, 1) * derived(1, 0);
    
    return det;
}

template<
  template<typename, size_t _rows = DynamicData, size_t _cols = DynamicData>
  class MatrixDerived, typename T, size_t _rows, size_t _cols>
T MatrixBase<MatrixDerived<T, _rows, _cols>>::determinant3x3() const
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
  template<typename, size_t _rows = DynamicData, size_t _cols = DynamicData>
  class MatrixDerived, typename T, size_t _rows, size_t _cols>
T MatrixBase<MatrixDerived<T, _rows, _cols>>::determinant4x4() const
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
  template<typename, size_t _rows = DynamicData, size_t _cols = DynamicData>
  class MatrixDerived, typename T, size_t _rows, size_t _cols>
T MatrixBase<MatrixDerived<T, _rows, _cols>>::determinantnxn() const
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




/*------------------------------------------------------------------------*/
/* Matrix implementation                                                  */
/*------------------------------------------------------------------------*/


template<typename T, size_t _rows, size_t _cols>
Matrix<T, _rows, _cols>::Matrix()
  : _data(Data<T, data::size>()),
    mRows(_rows == DynamicData ? 0 : _rows),
    mCols(_cols == DynamicData ? 0 : _cols)
{

}

template<typename T, size_t _rows, size_t _cols>
Matrix<T, _rows, _cols>::Matrix(size_t rows, size_t cols)
  : _data(Data<T, data::size>(rows *cols)),
    mRows(_rows == DynamicData ? rows : _rows),
    mCols(_cols == DynamicData ? cols : _cols)
{
}

template<typename T, size_t _rows, size_t _cols>
Matrix<T, _rows, _cols>::Matrix(size_t rows, size_t cols, T value)
  : _data(Data<T, data::size>(rows * cols, value)),
    mRows(_rows == DynamicData ? rows : _rows),
    mCols(_cols == DynamicData ? cols : _cols)
{
}

template<typename T, size_t _rows, size_t _cols>
Matrix<T, _rows, _cols>::Matrix(const Matrix &mat)
  : _data(mat._data),
    mRows(mat.mRows),
    mCols(mat.mCols)
{
}

template<typename T, size_t _rows, size_t _cols>
template<typename MatrixDerived>
Matrix<T, _rows, _cols>::Matrix(const MatrixDerived &matrix)
{
    if(_rows == DynamicData && _cols == DynamicData) {
        _data = Data<T, data::size>(matrix.rows() * matrix.cols());
        mRows = matrix.rows();
        mCols = matrix.cols();
    }

    TL_ASSERT(matrix.rows() == this->rows() &&
              matrix.cols() == this->cols(), "Static matrix cannot be resized");

    if(matrix.rows() == this->rows() &&
       matrix.cols() == this->cols()) {
        MatrixBase<Matrix<T, _rows, _cols>>::set(matrix);
    }

}

template<typename T, size_t _rows, size_t _cols>
Matrix<T, _rows, _cols>::Matrix(Matrix &&mat) TL_NOEXCEPT
  : _data(std::move(mat._data)),
    mRows(std::move(mat.mRows)),
    mCols(std::move(mat.mCols))
{
}

template<typename T, size_t _rows, size_t _cols>
Matrix<T, _rows, _cols>::Matrix(std::initializer_list<T> values)
{
    if(_rows == DynamicData && _cols == DynamicData) {
        this->mRows = 1;
        this->mCols = values.size();
        _data = Data<T, data::size>(values.size());
        std::copy(values.begin(), values.end(), _data.begin());

    } else {

        _data = Data<T, data::size>();
        this->mRows = _rows;
        this->mCols = _cols;

        size_t n = values.size();
        if(n == data::size) {
            std::copy(values.begin(), values.end(), _data.begin());
        } else if(n < data::size) {
            std::copy(values.begin(), values.end(), _data.begin());
            std::fill(_data.begin() + n, _data.end(), consts::zero<T>);
        } else {
            std::copy(values.begin(), values.begin() + data::size, _data.begin());
        }
    }
}

template<typename T, size_t _rows, size_t _cols>
Matrix<T, _rows, _cols>::Matrix(std::initializer_list<std::initializer_list<T>> values)
{
    if(_rows == DynamicData && _cols == DynamicData) {

        this->mRows = values.size();
        auto it = values.begin();
        this->mCols = it->size();
        _data = Data<T, data::size>(this->mRows * this->mCols);

        auto it_data = _data.begin();
        for(auto it = values.begin(); it != values.end(); it++) {
            size_t n = it->size();
            if(n == mCols) {
                std::copy(it->begin(), it->end(), it_data);
            } else if(n < mCols) {
                std::copy(it->begin(), it->end(), it_data);
                std::fill(it_data + n, _data.end(), consts::zero<T>);
            } else {
                std::copy(it->begin(), it->end() + mCols, it_data);
            }
            it_data += mCols;
        }

    } else {

        auto it_data = _data.begin();
        size_t rows_counter = 0;
        for(auto it = values.begin(); it != values.end(); it++) {
            if(rows_counter < _rows) {
                size_t n = it->size();
                if(n == _cols) {
                    std::copy(it->begin(), it->end(), it_data);
                } else if(n < _cols) {
                    std::copy(it->begin(), it->end(), it_data);
                    std::fill(it_data + n, _data.end(), consts::zero<T>);
                } else {
                    std::copy(it->begin(), it->end() + _cols, it_data);
                }

                it_data += _cols;
                rows_counter++;
            }
        }

        if(it_data != _data.end()) {
            std::fill(it_data, _data.end(), consts::zero<T>);
        }
    }
}

template<typename T, size_t _rows, size_t _cols>
Matrix<T, _rows, _cols>::Matrix(const T *data, size_t rows, size_t cols)
  : _data(Data<T, data::size>(data, rows *cols)),
    mRows(_rows == DynamicData ? rows : _rows),
    mCols(_cols == DynamicData ? cols : _cols)
{
}

template<typename T, size_t _rows, size_t _cols>
auto Matrix<T, _rows, _cols>::operator = (const Matrix &matrix) -> Matrix&
{
    if(this != &matrix) {
        this->_data = matrix._data;
        this->mRows = matrix.mRows;
        this->mCols = matrix.mCols;
    }

    return *this;
}

template<typename T, size_t _rows, size_t _cols>
auto Matrix<T, _rows, _cols>::operator = (Matrix &&matrix) TL_NOEXCEPT -> Matrix &
{
    if(this != &matrix) {
        this->_data = std::move(matrix._data);
        this->mRows = std::move(matrix.mRows);
        this->mCols = std::move(matrix.mCols);
    }

    return *this;
}

template<typename T, size_t _rows, size_t _cols>
Matrix<T, _rows, _cols>::operator Matrix<T, DynamicData, DynamicData>() const
{
    Matrix<T, DynamicData, DynamicData> matrix(this->data(), this->rows(), this->cols());
    return matrix;
}



template<typename T, size_t _rows, size_t _cols>
auto Matrix<T, _rows, _cols>::inverse(bool *invertibility) const -> Matrix
{
    static_assert(_rows == _cols, "Non-Square Matrix");
    static_assert(std::is_floating_point<T>::value, "Integral type not supported");

    size_t rows = this->rows();
    size_t cols = this->cols();
    TL_ASSERT(rows == cols, "Non-Square Matrix");

    Matrix<T, _rows, _cols> matrix;

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

template<typename T, size_t _rows, size_t _cols>
auto Matrix<T, _rows, _cols>::inverse2x2(bool *invertibility) const -> Matrix
{
    Matrix<T, _rows, _cols> matrix(*this);

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

template<typename T, size_t _rows, size_t _cols>
auto Matrix<T, _rows, _cols>::inverse3x3(bool *invertibility) const -> Matrix
{
    Matrix<T, _rows, _cols> matrix(*this);

    Matrix<T, _rows, _cols> adj = this->adjoint3x3();
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

template<typename T, size_t _rows, size_t _cols>
auto Matrix<T, _rows, _cols>::inverse4x4(bool *invertibility) const -> Matrix
{
    Matrix<T, _rows, _cols> matrix(*this);

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

template<typename T, size_t _rows, size_t _cols>
auto Matrix<T, _rows, _cols>::inversenxn(bool *invertibility) const -> Matrix
{
    Matrix<T, _rows, _cols> matrix(*this);

    LuDecomposition<Matrix<T, _rows, _cols>> lu(*this);

    T det = lu.determinant();//this->determinantnxn();
    if(det != consts::zero<T>) {

        //  matrix = this->adjugate();
        //  matrix /= det;

        Matrix<T, _rows, _cols> indentity(this->rows(), this->cols(), 0);
        for(size_t r = 0; r < this->rows(); r++)
            indentity(r, r) = consts::one<T>;
        matrix = lu.solve(indentity);

        if(invertibility != nullptr) *invertibility = true;

    } else if(invertibility != nullptr) {
        *invertibility = false;
    }

    return matrix;
}

template<typename T, size_t _rows, size_t _cols>
auto Matrix<T, _rows, _cols>::transpose() const -> Matrix<T, _cols, _rows>
{
    size_t rows = this->rows();
    size_t cols = this->cols();

    Matrix<T, _cols, _rows> matrix(cols, rows);

    for(size_t r = 0; r < rows; r++) {
        for(size_t c = 0; c < cols; c++) {
            matrix[c][r] = (*this)(r, c);
        }
    }

#if defined TL_HAVE_SIMD_INTRINSICS



#endif

    return matrix;
}

template<typename T, size_t _rows, size_t _cols>
inline auto Matrix<T, _rows, _cols>::adjugate() const -> Matrix
{
    static_assert(_rows == _cols, "Non-Square Matrix");
    size_t rows = this->rows();
    size_t cols = this->cols();
    TL_ASSERT(rows == cols, "Non-Square Matrix");

    Matrix<T, _rows, _cols> matrix(*this);

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

template<typename T, size_t _rows, size_t _cols>
inline auto Matrix<T, _rows, _cols>::adjoint2x2() const -> Matrix
{
    Matrix<T, _rows, _cols> matrix(*this);

    std::swap(matrix(0, 0), matrix(1, 1));
    matrix(0, 1) = -matrix(0, 1);
    matrix(1, 0) = -matrix(1, 0);

    return matrix;
}

template<typename T, size_t _rows, size_t _cols>
auto Matrix<T, _rows, _cols>::adjoint3x3() const -> Matrix
{
    Matrix<T, _rows, _cols> matrix(*this);

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

template<typename T, size_t _rows, size_t _cols>
auto Matrix<T, _rows, _cols>::adjoint4x4() const -> Matrix
{
    Matrix<T, _rows, _cols> matrix(*this);

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

template<typename T, size_t _rows, size_t _cols>
inline auto Matrix<T, _rows, _cols>::adjointnxn() const -> Matrix
{
    Matrix<T, _rows, _cols> matrix = this->cofactorMatrix();
    return matrix.transpose();
}

template<typename T, size_t _rows, size_t _cols>
auto Matrix<T, _rows, _cols>::cofactorMatrix() const -> Matrix
{
    static_assert(_rows == _cols, "Non-Square Matrix");

    size_t rows = this->rows();
    size_t cols = this->cols();

    TL_ASSERT(rows == cols, "Non-Square Matrix");

    Matrix<T, _rows, _cols> matrix(*this);
    for(size_t r = 0; r < rows; r++) {
        for(size_t c = 0; c < cols; c++) {
            matrix[r][c] = cofactor(r, c);
        }
    }

    return matrix;
}

template<typename T, size_t _rows, size_t _cols>
auto Matrix<T, _rows, _cols>::trace() const -> T
{
    static_assert(_rows == _cols, "Non-Square Matrix");

    size_t rows = this->rows();
    size_t cols = this->cols();

    TL_ASSERT(rows == cols, "Non-Square Matrix");

    T trace = consts::zero<T>;
    for(size_t r = 0; r < rows; r++) {
        trace += (*this)(r, r);
    }

    return trace;
}

template<typename T, size_t _rows, size_t _cols>
inline auto Matrix<T, _rows, _cols>::invertible() -> bool
{
    T det = this->determinant();
    return (det != consts::zero<T>);
}

template<typename T, size_t _rows, size_t _cols>
inline auto Matrix<T, _rows, _cols>::singular() -> bool
{
    T det = this->determinant();
    return (det == consts::zero<T>);
}

template<typename T, size_t _rows, size_t _cols>
auto Matrix<T, _rows, _cols>::cofactor(size_t r, size_t c) const -> T
{
    static_assert(_rows == _cols, "Non-Square Matrix");
    size_t rows = this->rows();
    size_t cols = this->cols();
    TL_ASSERT(rows == cols, "Non-Square Matrix");

    T sign = ((r + c) % 2 == 0) ? consts::one<T> : -consts::one<T>;
    return sign * this->firstMinor(r, c);
}

template<typename T, size_t _rows, size_t _cols>
auto Matrix<T, _rows, _cols>::firstMinor(size_t row, size_t col) const -> T
{
    static_assert(_rows == _cols, "Non-Square Matrix");
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

template<typename T, size_t _rows, size_t _cols>
auto Matrix<T, _rows, _cols>::rowEchelonForm() const -> Matrix
{
    size_t rows = this->rows();
    size_t cols = this->cols();

    Matrix<T, _rows, _cols> matrix(*this);

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

template<typename T, size_t _rows, size_t _cols>
auto Matrix<T, _rows, _cols>::reducedRowEchelonForm() const -> Matrix
{
    size_t rows = this->rows();
    size_t cols = this->cols();

    Matrix<T, _rows, _cols> matrix(*this);

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

template<typename T, size_t _rows, size_t _cols>
auto Matrix<T, _rows, _cols>::rank() const -> int
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

template<typename T, size_t _rows, size_t _cols>
auto Matrix<T, _rows, _cols>::swapRows(size_t i, size_t j) -> void
{
    for(size_t c = 0; c < mCols; c++) {
        std::swap(_data[i * mCols + c], _data[j * mCols + c]);
    }
}

template<typename T, size_t _rows, size_t _cols>
inline auto Matrix<T, _rows, _cols>::at(size_t r, size_t c) -> reference
{
    return _data.at(r * mCols + c);
}

template<typename T, size_t _rows, size_t _cols>
inline auto Matrix<T, _rows, _cols>::at(size_t r, size_t c) const -> const_reference
{
    return _data.at(r * mCols + c);
}

template<typename T, size_t _rows, size_t _cols>
inline auto Matrix<T, _rows, _cols>::operator()(size_t r, size_t c) -> reference
{
    return _data[r * mCols + c];
}

template<typename T, size_t _rows, size_t _cols>
inline auto Matrix<T, _rows, _cols>::operator()(size_t r, size_t c) const -> const_reference
{
    return _data[r * mCols + c];
}

template<typename T, size_t _rows, size_t _cols>
inline auto Matrix<T, _rows, _cols>::operator()(size_t position) -> reference
{
    return _data[position];
}

template<typename T, size_t _rows, size_t _cols>
inline auto Matrix<T, _rows, _cols>::operator()(size_t position) const -> const_reference
{
    return _data[position];
}

template<typename T, size_t _rows, size_t _cols>
inline auto Matrix<T, _rows, _cols>::operator[](size_t position) const -> const internal::MatrixRow<const T>
{
    return internal::MatrixRow<const T>(this->data(), position, this->cols());
}

template<typename T, size_t _rows, size_t _cols>
inline auto Matrix<T, _rows, _cols>::operator[](size_t position)  -> internal::MatrixRow<T>
{
    return internal::MatrixRow<T>(this->data(), position, this->cols());
}

template<typename T, size_t _rows, size_t _cols>
inline  auto Matrix<T, _rows, _cols>::row(size_t row) const -> const internal::MatrixRow<const T>
{
    return internal::MatrixRow<const T>(this, row);
}

template<typename T, size_t _rows, size_t _cols>
inline  auto Matrix<T, _rows, _cols>::row(size_t row) -> internal::MatrixRow<T>
{
    return internal::MatrixRow<T>(this->data(), row, this->cols());
}

template<typename T, size_t _rows, size_t _cols>
inline  auto Matrix<T, _rows, _cols>::col(size_t col) const -> const internal::MatrixCol<const T>
{
    return internal::MatrixCol<const T>(this->data(), col, this->rows(), this->cols());
}

template<typename T, size_t _rows, size_t _cols>
inline  auto Matrix<T, _rows, _cols>::block(size_t iniRow, 
                                            size_t endRow,
                                            size_t iniCol, 
                                            size_t endCol) const -> const internal::MatrixBlock<const T, DynamicData, DynamicData>
{
    TL_ASSERT(iniRow >= 0 && endRow > iniRow && endRow < this->rows() &&
              iniCol >= 0 && endCol > iniCol && endCol < this->cols(), "Matrix block out of range");

    return internal::MatrixBlock<const T, DynamicData, DynamicData>(this->data(),
                                                          this->rows(),
                                                          this->cols(),
                                                           iniRow,
                                                           endRow,
                                                           iniCol,
                                                           endCol);
}

template<typename T, size_t _rows, size_t _cols>
inline  auto Matrix<T, _rows, _cols>::block(size_t iniRow, size_t endRow, size_t iniCol, size_t endCol) -> internal::MatrixBlock<T>
{
    TL_ASSERT(iniRow >= 0 && endRow >= iniRow && endRow < this->rows() &&
              iniCol >= 0 && endCol >= iniCol && endCol < this->cols(), "Matrix block out of range");

    return internal::MatrixBlock<T>(this->data(), this->rows(), this->cols(),
                                    iniRow, endRow, iniCol, endCol);
}

template<typename T, size_t _rows, size_t _cols>
inline  auto Matrix<T, _rows, _cols>::rowBlock(size_t iniRow, size_t endRow) const -> const internal::MatrixBlock<const T, DynamicData, DynamicData>
{
    return block(iniRow, endRow, 0, this->cols() - 1);
}

template<typename T, size_t _rows, size_t _cols>
inline  auto Matrix<T, _rows, _cols>::rowBlock(size_t iniRow, size_t endRow) -> internal::MatrixBlock<T, DynamicData, DynamicData>
{
    return block(iniRow, endRow, 0, this->cols() - 1);
}

template<typename T, size_t _rows, size_t _cols>
inline  auto Matrix<T, _rows, _cols>::colBlock(size_t iniCol, size_t endCol) const -> const internal::MatrixBlock<const T, DynamicData, DynamicData>
{
    return block(0, this->rows() - 1, iniCol, endCol);
}

template<typename T, size_t _rows, size_t _cols>
inline  auto Matrix<T, _rows, _cols>::colBlock(size_t iniCol, size_t endCol) -> internal::MatrixBlock<T, DynamicData, DynamicData>
{
    return block(0, this->rows() - 1, iniCol, endCol);
}

template<typename T, size_t _rows, size_t _cols>
inline  auto Matrix<T, _rows, _cols>::col(size_t col) -> internal::MatrixCol<T>
{
    return internal::MatrixCol<T>(this->data(), col, this->rows(), this->cols());
}

template<typename T, size_t _rows, size_t _cols>
inline auto Matrix<T, _rows, _cols>::zero() -> Matrix
{
    static_assert(_rows != DynamicData || _cols != DynamicData, "Not supported for dinamic matrix");

    return Matrix<T, _rows, _cols>(_rows, _cols, consts::zero<T>);
}

template<typename T, size_t _rows, size_t _cols>
inline auto Matrix<T, _rows, _cols>::zero(size_t rows, size_t cols) -> Matrix
{
    static_assert(_rows == DynamicData || _cols == DynamicData, "Not supported for static matrix");

    return Matrix<T>(rows, cols, consts::zero<T>);
}

template<typename T, size_t _rows, size_t _cols>
inline auto Matrix<T, _rows, _cols>::ones() -> Matrix
{
    static_assert(_rows != DynamicData || _cols != DynamicData, "Not supported for dinamic matrix");

    return Matrix<T, _rows, _cols>(_rows, _cols, consts::one<T>);
}

template<typename T, size_t _rows, size_t _cols>
inline auto Matrix<T, _rows, _cols>::ones(size_t rows, size_t cols) -> Matrix
{
    static_assert(_rows == DynamicData || _cols == DynamicData, "Not supported for static matrix");

    return Matrix<T>(rows, cols, consts::one<T>);
}

template<typename T, size_t _rows, size_t _cols>
auto Matrix<T, _rows, _cols>::identity() -> Matrix
{
    static_assert(_rows != DynamicData || _cols != DynamicData, "Not supported for dinamic matrix");

    Matrix<T, _rows, _cols> matrix;

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

template<typename T, size_t _rows, size_t _cols>
auto Matrix<T, _rows, _cols>::identity(size_t rows, size_t cols) -> Matrix
{
    static_assert(_rows == DynamicData || _cols == DynamicData, "Not supported for static matrix");

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

template<typename T, size_t _rows, size_t _cols>
auto Matrix<T, _rows, _cols>::randon() -> Matrix
{
    static_assert(_rows != DynamicData || _cols != DynamicData, "Not supported for dinamic matrix");

    Matrix<T, _rows, _cols> matrix;

    std::random_device rd;
    std::mt19937 random_number_engine(rd());
    std::uniform_real_distribution<> distribution(0.0, 99.0);

    constexpr size_t size = _rows * _cols;

    for(size_t i = 0; i < size; ++i) {
        matrix(i) = static_cast<T>(distribution(random_number_engine));
    }

    return matrix;
}

template<typename T, size_t _rows, size_t _cols>
auto Matrix<T, _rows, _cols>::randon(size_t rows, size_t cols) -> Matrix
{
    static_assert(_rows == DynamicData || _cols == DynamicData, "Not supported for static matrix");

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

template<typename T, size_t _rows, size_t _cols>
inline auto Matrix<T, _rows, _cols>::data() -> pointer
{
    return _data.data();
}

template<typename T, size_t _rows, size_t _cols>
inline auto Matrix<T, _rows, _cols>::data() const -> const_pointer
{
    return _data.data();
}





/* Binary arithmetic operators */

/* Casos especiales de suma */

template<typename T, size_t _rows, size_t _cols> inline static
Matrix<T, _rows, _cols> operator + (Matrix<T, _rows, _cols> &&matrix1,
                                    const Matrix<T, _rows, _cols> &matrix2)
{
    matrix1 += matrix2;
    return matrix1;
}

template<typename T, size_t _rows, size_t _cols> inline static
Matrix<T, _rows, _cols> operator + (const Matrix<T, _rows, _cols> &matrix1,
                                    Matrix<T, _rows, _cols> &&matrix2)
{
    matrix2 += matrix1;
    return matrix2;
}

template<typename T, size_t _rows, size_t _cols> inline static
Matrix<T, _rows, _cols> operator + (Matrix<T, _rows, _cols> &&matrix1,
                                    Matrix<T, _rows, _cols> &&matrix2)
{
    matrix1 += matrix2;
    return matrix1;
}

template<typename T, size_t _rows, size_t _cols> inline static
Matrix<T, _rows, _cols> operator + (const internal::MatrixBlock<T> &matrix1,
                                    const Matrix<T, _rows, _cols> &matrix2)
{
    Matrix<T, _rows, _cols> matrix = matrix2;
    matrix += matrix1;
    return matrix;
}

/* Casos especiales de resta de matrices */

template<typename T, size_t _rows, size_t _cols> inline static
Matrix<T, _rows, _cols> operator - (Matrix<T, _rows, _cols> &&matrix1,
                                    const Matrix<T, _rows, _cols> &matrix2)
{
    matrix1 -= matrix2;
    return matrix1;
}

template<typename T, size_t _rows, size_t _cols> inline static
Matrix<T, _rows, _cols> operator - (const Matrix<T, _rows, _cols> &matrix1,
                                    Matrix<T, _rows, _cols> &&matrix2)
{
    matrix2 -= matrix1;
    return -matrix2;
}

template<typename T, size_t _rows, size_t _cols> inline static
Matrix<T, _rows, _cols> operator - (Matrix<T, _rows, _cols> &&matrix1,
                                    Matrix<T, _rows, _cols> &&matrix2)
{
    matrix1 -= matrix2;
    return matrix1;
}

template<typename T, size_t _rows, size_t _cols> inline static
Matrix<T, _rows, _cols> operator - (const internal::MatrixBlock<T> &matrix1,
                                    const Matrix<T, _rows, _cols> &matrix2)
{
    Matrix<T, _rows, _cols> matrix = matrix2;
    matrix -= matrix1;
    return -matrix;
}


/*!
 * \brief Multiplicación de matrices
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
 * <h4>Ejemplo</h4>
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
template<typename T, size_t _rows, size_t _dim, size_t _cols> inline
Matrix<T, _rows, _cols> operator * (const Matrix<T, _rows, _dim> &matrix1,
                                    const Matrix<T, _dim, _cols> &matrix2)
{
    Matrix<T, _rows, _cols> matrix = Matrix<T, _rows, _cols>::zero();

    internal::mulmat(matrix1, matrix2, matrix);

    return matrix;
}

template<typename T> inline
Matrix<T> operator * (const Matrix<T> &matrix1,
                      const Matrix<T> &matrix2)
{
    TL_ASSERT(matrix1.cols() == matrix2.rows(), "A columns != B rows");

    Matrix<T> matrix = Matrix<T>::zero(matrix1.rows(), matrix2.cols());
    internal::mulmat(matrix1, matrix2, matrix);

    return matrix;
}

template<typename T, size_t _rows, size_t _cols> inline
Matrix<T> operator * (const Matrix<T, _rows, _cols> &matrix1,
                      const Matrix<T> &matrix2)
{
    TL_ASSERT(matrix1.cols() == matrix2.rows(), "A columns != B rows");

    Matrix<T> matrix = Matrix<T>::zero(matrix1.rows(), matrix2.cols());
    internal::mulmat(matrix1, matrix2, matrix);

    return matrix;
}

template<typename T, size_t _rows, size_t _cols> inline
Matrix<T> operator * (const Matrix<T> &matrix1,
                      const Matrix<T, _rows, _cols> &matrix2)
{
    TL_ASSERT(matrix1.cols() == matrix2.rows(), "A columns != B rows");

    Matrix<T> matrix = Matrix<T>::zero(matrix1.rows(), matrix2.cols());
    internal::mulmat(matrix1, matrix2, matrix);

    return matrix;
}

template<typename T, size_t _rows, size_t _cols> inline static
Matrix<T, _rows, _cols> operator * (Matrix<T, _rows, _cols> &&matrix,
                                    T scalar)
{
    matrix *= scalar;
    return matrix;
}


/*!
 * \brief Multiplicación de un escalar por una matriz
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
 * <h4>Ejemplo</h4>
 * \code
 * Matrix2x2i A{1, 4,
 *              3, 2};
 *
 * int s = 2;
 * Matrix2x2i C = s * A;
 * \endcode
 */
template<
  template<typename, size_t _rows = DynamicData, size_t _cols = DynamicData>
class MatrixDerived, typename T, size_t _rows, size_t _cols
>
inline static Matrix<T, _rows, _cols> operator * (T scalar, const MatrixDerived<T, _rows, _cols> &matrix)
{
    Matrix<T, _rows, _cols> _matrix = matrix;
    _matrix *= scalar;
    return _matrix;
}

template<typename T, size_t _rows, size_t _cols>
inline static Matrix<T, _rows, _cols> operator * (T scalar, Matrix<T, _rows, _cols> &&matrix)
{
    matrix *= scalar;
    return matrix;
}

/*!
 * \brief División de una matriz por un escalar
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
 * <h4>Ejemplo</h4>
 * \code
 * Matrix2x2i A{1.f, 4.f,
 *              3.f, 2.f};
 *
 * float s = 2.f;
 * Matrix2x2f C = A / s;
 * \endcode
 */
template<typename T, size_t _rows, size_t _cols> inline static
Matrix<T, _rows, _cols> operator / (Matrix<T, _rows, _cols> &&matrix, T scalar)
{
    matrix /= scalar;
    return matrix;
}

template<typename T, size_t _rows, size_t _cols> inline static
bool operator == (const Matrix<T, _rows, _cols> &matrix1,
                  const Matrix<T, _rows, _cols> &matrix2)
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

template<typename T, size_t _rows, size_t _cols> inline static
bool operator != (const Matrix<T, _rows, _cols> &matrix1,
                  const Matrix<T, _rows, _cols> &matrix2)
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


template<typename T, size_t _rows, size_t _cols>
std::ostream &operator<< (std::ostream &os, const Matrix<T, _rows, _cols> &matrix)
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

template<typename T, size_t _rows, size_t _cols>
std::ostream &operator<< (std::ostream &os, const Matrix<T, _rows, _cols> *matrix)
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


template<typename T, size_t _rows, size_t _cols>
std::ostream &operator<< (std::ostream &os, const internal::MatrixBlock<T, _rows, _cols> &matrix)
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

template<typename T, size_t _rows, size_t _cols>
std::ostream &operator<< (std::ostream &os, const internal::MatrixBlock<T, _rows, _cols> *matrix)
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



template<typename T, size_t _rows, size_t _dim> inline  static
Vector<T, _rows> operator * (const Matrix<T, _rows, _dim> &matrix,
                             const Vector<T, _dim> &vector)
{
    Vector<T, _rows> vect = Vector<T, _rows>::zero();

#ifndef TL_HAVE_SIMD_INTRINSICS

    for(size_t r = 0; r < _rows; r++) {
        for(size_t c = 0; c < _dim; c++) {
            vect[r] += matrix(r, c) * vector[c];
        }
    }

#else

    Packed<T> packed_a;
    Packed<T> packed_b;
    Packed<T> packed_c;

    constexpr size_t packed_size = packed_a.size();
    constexpr size_t max_vector = _dim - _dim % packed_size;

    for(size_t r = 0; r < _rows; r++) {
        for(size_t i = 0; i < max_vector; i += packed_size) {

            packed_a.loadUnaligned(&vector[i]);
            packed_b.loadUnaligned(&matrix(r, i));
            packed_c = packed_a * packed_b;
            vect[r] += packed_c.sum();
        }

        for(size_t i = max_vector; i < _dim; i++) {
            vect[r] += matrix(r, i) * vector[i];
        }
    }

#endif

    return vect;
}

template<typename T> inline
static Vector<T> operator * (const Matrix<T> &matrix,
                             const Vector<T> &vector)
{
    size_t rows = matrix.rows();
    size_t dim1 = matrix.cols();
    size_t dim2 = vector.size();

    TL_ASSERT(dim1 == dim2, "Matrix columns != Vector size");

    Vector<T> vect = Vector<T>::zero(rows);

#ifndef TL_HAVE_SIMD_INTRINSICS

    for(size_t r = 0; r < rows; r++) {
        for(size_t c = 0; c < dim1; c++) {
            vect[r] += matrix(r, c) * vector[c];
        }
    }

#else

    Packed<T> packed_a;
    Packed<T> packed_b;
    Packed<T> packed_c;

    constexpr size_t packed_size = packed_a.size();
    size_t max_vector = dim1 - dim1 % packed_size;

    for(size_t r = 0; r < rows; r++) {
        for(size_t i = 0; i < max_vector; i += packed_size) {

            packed_a.loadUnaligned(&vector[i]);
            packed_b.loadUnaligned(&matrix(r, i));
            packed_c = packed_a * packed_b;
            vect[r] += packed_c.sum();
        }

        for(size_t i = max_vector; i < dim1; i++) {
            vect[r] += matrix(r, i) * vector[i];
        }
    }

#endif

    return vect;
}

//template<
//  template<typename, size_t _rows = DynamicData, size_t _cols = DynamicData>
//  class MatrixDerived, typename T, size_t _rows, size_t _cols,
//>
//static Vector<T, _cols> operator * (const MatrixDerived<T, _rows, _cols> &matrix,
//                                    const Vector<T, _cols> &vector)
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

template<typename T> inline
static Vector<T> operator * (const internal::MatrixBlock<T> &matrix,
                             const Vector<T> &vector)
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


template<typename T, size_t _rows, size_t _dim> 
inline  static Vector<T, _rows> operator * (const Vector<T, _dim> &vector,
                                            const Matrix<T, _rows, _dim> &matrix)
{
    return matrix * vector;
}

/*! \} */ // end of algebra

/*! \} */ // end of math


} // End namespace tl


