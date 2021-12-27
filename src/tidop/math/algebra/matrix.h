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
 * License along with Foobar. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#ifndef TL_MATH_MATRIX_H
#define TL_MATH_MATRIX_H

#include "config_tl.h"

#include <vector>
#include <array>
#ifdef MATRIX_STD_VALARRAY
#include <valarray>
#endif

#ifdef HAVE_OPENBLAS
#include <lapacke.h>
#endif // HAVE_OPENBLAS

#include "tidop/math/math.h"
#include "tidop/math/algebra/vector.h"
#include "tidop/core/exception.h"
#include "tidop/core/utils.h"
#include "tidop/math/simd.h"

namespace tl
{

namespace math
{

constexpr auto DynamicMatrix = std::numeric_limits<size_t>::max();

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
class MatrixRow;

template<typename T>
class MatrixCol;

template<typename T>
class MatrixBlock;

template<typename T, size_t _rows, size_t _cols>
class MatrixBase;

template<typename T, size_t _rows, size_t _cols>
class MatrixBase
{

public:

  using value_type      = T;
  using size_type       = size_t;
  using pointer         = T*;
  using const_pointer   = const T*;
  using reference       = T&;
  using const_reference = const T&;

public:

  MatrixBase();
  MatrixBase(size_t rows, size_t cols);
  MatrixBase(size_t rows, size_t cols, T value);
  MatrixBase(const MatrixBase &matrix);
  MatrixBase(MatrixBase &&matrix) TL_NOEXCEPT;
  MatrixBase(std::initializer_list<T> values);
  MatrixBase(std::initializer_list<std::initializer_list<T>> values);
  MatrixBase(T *data, size_t rows = 0, size_t cols = 0);
  virtual ~MatrixBase() = default;

  MatrixBase &operator = (const MatrixBase &matrix);
  MatrixBase &operator = (MatrixBase &&matrix) TL_NOEXCEPT;

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
  reference at(size_t r, size_t c);

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
  const_reference at(size_t r, size_t c) const;

  //const MatrixRow<const T> operator[](size_t position) const;
  //MatrixRow<T> operator[](size_t position);

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
  reference operator()(size_t r, size_t c);

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
  const_reference operator()(size_t r, size_t c) const;

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
  reference operator()(size_t position);
  const_reference operator()(size_t position) const;

  void operator=(T value)
  {
    mData.fill(value);
  }

  /*!
   * \brief Número de filas de la matriz
   * \return Número de filas
   */
  size_t rows() const;

  /*!
   * \brief Número de columnas de la matriz
   * \return Número de columnas
   */
  size_t cols() const;

  void swapRows(size_t i, size_t j);

  pointer data()
  {
    return mData.data();
  }
    
  const_pointer data() const
  {
    return mData.data();
  }

private:

  std::array<T, _rows * _cols> mData;
};


template<typename T, size_t _rows, size_t _cols> inline 
MatrixBase<T, _rows, _cols>::MatrixBase()
  : mData()
{
  this->mData.fill(-std::numeric_limits<T>().max());
}

template<typename T, size_t _rows, size_t _cols> inline
MatrixBase<T, _rows, _cols>::MatrixBase(size_t  /*rows*/, size_t  /*cols*/)
  : mData()
{
  this->mData.fill(-std::numeric_limits<T>().max());
}

template<typename T, size_t _rows, size_t _cols> inline 
MatrixBase<T, _rows, _cols>::MatrixBase(size_t  /*rows*/, size_t  /*cols*/, T val)
  : mData()
{
  this->mData.fill(val);
}

template<typename T, size_t _rows, size_t _cols> inline 
MatrixBase<T, _rows, _cols>::MatrixBase(const MatrixBase &matrix)
  : mData(std::move(matrix.mData))
{
}

template<typename T, size_t _rows, size_t _cols> inline 
MatrixBase<T, _rows, _cols>::MatrixBase(std::initializer_list<T> values)
{
  size_t n = values.size();
  if (n == _rows * _cols){
    std::copy(values.begin(), values.end(), mData.begin());
  } else if (n < _rows * _cols){
    std::copy(values.begin(), values.end(), mData.begin());
    std::fill(mData.begin() + n, mData.end(), consts::zero<T>);
  } else {
    std::copy(values.begin(), values.begin() + _rows * _cols, mData.begin());
  }
}

template<typename T, size_t _rows, size_t _cols> inline
MatrixBase<T, _rows, _cols>::MatrixBase(std::initializer_list<std::initializer_list<T>> values)
{
  auto it_data = mData.begin();
  size_t rows_counter = 0;
  for (auto it = values.begin(); it != values.end(); it++) {
    if (rows_counter < _rows) {
      size_t n = it->size();
      if (n == _cols) {
        std::copy(it->begin(), it->end(), it_data);
      } else if (n < _cols) {
        std::copy(it->begin(), it->end(), it_data);
        std::fill(it_data + n, mData.end(), consts::zero<T>);
      } else {
        std::copy(it->begin(), it->end() + _cols, it_data);
      }
      it_data += _cols;
      rows_counter++;
    }
  }

  if (it_data != mData.end()) {
    std::fill(it_data, mData.end(), consts::zero<T>);
  }
}

template<typename T, size_t _rows, size_t _cols>
inline MatrixBase<T, _rows, _cols>::MatrixBase(T *data, size_t  /*rows*/, size_t  /*cols*/)
  : mData(data)
{
}

template<typename T, size_t _rows, size_t _cols> inline 
MatrixBase<T, _rows, _cols>::MatrixBase(MatrixBase &&matrix) TL_NOEXCEPT
  : mData(std::move(matrix.mData))
{
}

template<typename T, size_t _rows, size_t _cols> inline 
MatrixBase<T, _rows, _cols> &MatrixBase<T, _rows, _cols>::operator = (const MatrixBase &matrix)
{
  if (this != &matrix) {
    this->mData = matrix.mData;
  }
  return *this;
}

template<typename T, size_t _rows, size_t _cols> inline 
MatrixBase<T, _rows, _cols> &MatrixBase<T, _rows, _cols>::operator = (MatrixBase &&matrix) TL_NOEXCEPT
{
  if (this != &matrix) {
    this->mData = std::move(matrix.mData);
  }
  return *this;
}

template<typename T, size_t _rows, size_t _cols> inline 
T &MatrixBase<T, _rows, _cols>::at(size_t r, size_t c)
{
  return mData.at(r * _cols + c);
}

template<typename T, size_t _rows, size_t _cols> inline 
const T &MatrixBase<T, _rows, _cols>::at(size_t r, size_t c) const
{
  return mData.at(r * _cols + c);
}

template<typename T, size_t _rows, size_t _cols> inline 
T &MatrixBase<T, _rows, _cols>::operator()(size_t r, size_t c)
{
  return mData[r * this->cols() + c];
}

template<typename T, size_t _rows, size_t _cols> inline 
const T &MatrixBase<T, _rows, _cols>::operator()(size_t r, size_t c) const
{
  return mData[r * this->cols() + c];
}

template<typename T, size_t _rows, size_t _cols> inline
T &MatrixBase<T, _rows, _cols>::operator()(size_t position)
{
  return mData[position];
}

template<typename T, size_t _rows, size_t _cols> inline
const T &MatrixBase<T, _rows, _cols>::operator()(size_t position) const
{
  return mData[position];
}

template<typename T, size_t _rows, size_t _cols> inline 
size_t MatrixBase<T, _rows, _cols>::rows() const
{
  return _rows;
}

template<typename T, size_t _rows, size_t _cols> inline 
size_t MatrixBase<T, _rows, _cols>::cols() const
{
  return _cols;
}

template<typename T, size_t _rows, size_t _cols> inline 
void MatrixBase<T, _rows, _cols>::swapRows(size_t i, size_t j)
{
  for (size_t c = 0; c < _cols; c++) {
    std::swap(mData[i* _cols + c], mData[j * _cols + c]);
  }
}



#define MATRIX_STD_VECTOR 1
//#define MATRIX_STD_VALARRAY 1

#ifdef MATRIX_STD_VECTOR

template<typename T>
class MatrixBase<T, DynamicMatrix, DynamicMatrix>
{

public:

  using value_type      = T;
  using size_type       = size_t;
  using pointer         = T*;
  using const_pointer   = const T*;
  using reference       = T&;
  using const_reference = const T&;

public:

  MatrixBase()= default;
  MatrixBase(size_t rows, size_t cols);
  MatrixBase(size_t rows, size_t cols, T val);
  MatrixBase(const MatrixBase &matrix);
  MatrixBase(MatrixBase &&matrix) TL_NOEXCEPT;
  MatrixBase(std::initializer_list<T> values);
  MatrixBase(std::initializer_list<std::initializer_list<T>> values);
  MatrixBase(const T *data, size_t rows, size_t cols);
  virtual ~MatrixBase() = default;

  MatrixBase &operator = (const MatrixBase &matrix);
  MatrixBase &operator = (MatrixBase &&matrix) TL_NOEXCEPT;

  reference at(size_t r, size_t c);
  const_reference at(size_t r, size_t c) const;

  //const MatrixRow<const T> operator[](size_t position) const;
  //MatrixRow<T> operator[](size_t position);

  reference operator()(size_t r, size_t c);
  const_reference operator()(size_t r, size_t c) const;

  reference operator()(size_t position);
  const_reference operator()(size_t position) const;

  void operator=(T value)
  {
    std::fill(mData.begin(), mData.end(), value);
  }

  size_t rows() const;
  size_t cols() const;

  void swapRows(size_t i, size_t j);

  pointer data()
  {
    return mData.data();
  }
  
  const_pointer data() const
  {
    return mData.data();
  }

private:

  //template<typename T> friend class MatrixRow;
  //template<typename T> friend class MatrixCol;
  std::vector<T> mData;
  size_t mRows{0};
  size_t mCols{0};
};


template<typename T> inline
MatrixBase<T, DynamicMatrix, DynamicMatrix>::MatrixBase(size_t rows, size_t cols)
  : mData(rows * cols, -std::numeric_limits<T>().max()),
    mRows(rows),
    mCols(cols)
{
}

template<typename T> inline
MatrixBase<T, DynamicMatrix, DynamicMatrix>::MatrixBase(size_t rows, size_t cols, T val)
  : mData(rows * cols, val),
    mRows(rows),
    mCols(cols)
{
}

template<typename T> inline 
MatrixBase<T, DynamicMatrix, DynamicMatrix>::MatrixBase(const MatrixBase &matrix)
  : mData(matrix.mData), 
    mRows(matrix.mRows),
    mCols(matrix.mCols)
{
}

template<typename T> inline 
MatrixBase<T, DynamicMatrix, DynamicMatrix>::MatrixBase(MatrixBase &&matrix) TL_NOEXCEPT
  : mData(std::move(matrix.mData)), 
    mRows(matrix.mRows),
    mCols(matrix.mCols)
{
}

template<typename T> inline 
MatrixBase<T, DynamicMatrix, DynamicMatrix>::MatrixBase(std::initializer_list<T> values)
{
  this->mRows = 1;
  this->mCols = values.size();

  std::copy(values.begin(), values.end(), mData.begin());
}

template<typename T> inline
MatrixBase<T, DynamicMatrix, DynamicMatrix>::MatrixBase(std::initializer_list<std::initializer_list<T>> values)
{
  this->mRows = values.size();
  auto it = values.begin();
  this->mCols = it->size();
  mData.resize(this->mRows * this->mCols);

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
}

template<typename T> inline
MatrixBase<T, DynamicMatrix, DynamicMatrix>::MatrixBase(const T *data, size_t rows, size_t cols)
  : mData(rows *cols, -std::numeric_limits<T>().max()),
    mRows(rows),
    mCols(cols)
{
  mData.assign(data, data + mData.size());
}

template<typename T> inline 
MatrixBase<T, DynamicMatrix, DynamicMatrix> 
&MatrixBase<T, DynamicMatrix, DynamicMatrix>::operator = (const MatrixBase<T, DynamicMatrix, DynamicMatrix> &matrix)
{
  if (this != &matrix) {
    this->mData = matrix.mData;
    this->mRows = matrix.mRows;
    this->mCols = matrix.mCols;
  }
  return *this;
}

template<typename T> inline 
MatrixBase<T, DynamicMatrix, DynamicMatrix> 
&MatrixBase<T, DynamicMatrix, DynamicMatrix>::operator = (MatrixBase<T, DynamicMatrix, DynamicMatrix> &&matrix) TL_NOEXCEPT
{
  if (this != &matrix) {
    this->mData = std::move(matrix.mData);
    this->mRows = matrix.mRows;
    this->mCols = matrix.mCols;
  }
  return *this;
}

template<typename T> inline 
T &MatrixBase<T, DynamicMatrix, DynamicMatrix>::at(size_t r, size_t c)
{
  return mData.at(r * mCols + c);
}

template<typename T> inline 
const T &MatrixBase<T, DynamicMatrix, DynamicMatrix>::at(size_t r, size_t c) const
{
  return mData.at(r * mCols + c);
}

//template<typename T> inline
//const MatrixRow<const T> MatrixBase<T, DynamicMatrix, DynamicMatrix>::operator[](size_t position) const
//{
//  return MatrixRow<const T>(this->data(), position, cols());
//}
//
//template<typename T> inline
//MatrixRow<T> MatrixBase<T, DynamicMatrix, DynamicMatrix>::operator[](size_t position)
//{
//  return MatrixRow<T>(this->data(), position, cols());
//}

template<typename T> inline
T &MatrixBase<T, DynamicMatrix, DynamicMatrix>::operator()(size_t r, size_t c) 
{
  return mData[r * mCols + c];
}

template<typename T> inline
const T &MatrixBase<T, DynamicMatrix, DynamicMatrix>::operator()(size_t r, size_t c) const
{
  return mData[r * mCols + c];
}

template<typename T> inline
T &MatrixBase<T, DynamicMatrix, DynamicMatrix>::operator()(size_t position)
{
  return mData[position];
}

template<typename T> inline
const T &MatrixBase<T, DynamicMatrix, DynamicMatrix>::operator()(size_t position) const
{
  return mData[position];
}

template<typename T> inline 
size_t MatrixBase<T, DynamicMatrix, DynamicMatrix>::rows() const
{
  return mRows;
}

template<typename T> inline 
size_t MatrixBase<T, DynamicMatrix, DynamicMatrix>::cols() const
{
  return mCols;
}

template<typename T> inline 
void MatrixBase<T, DynamicMatrix, DynamicMatrix>::swapRows(size_t i, size_t j)
{
  for (size_t c = 0; c < mCols; c++) {
    std::swap(mData[i* mCols + c], mData[j * mCols + c]);
  }
}



////////////////////////////////////////////////////////////////////////////////////////////////



#elif MATRIX_STD_VALARRAY

template<typename T>
class MatrixBase<T, DynamicMatrix, DynamicMatrix>
{

public:

  using value_type      = T;
  using size_type       = size_t;
  using pointer         = T*;
  using const_pointer   = const T*;
  using reference       = T&;
  using const_reference = const T&;

public:

  MatrixBase();
  MatrixBase(size_t rows, size_t cols);
  MatrixBase(size_t rows, size_t cols, T val);
  MatrixBase(const MatrixBase &matrix);
  MatrixBase(MatrixBase &&matrix) TL_NOEXCEPT;
  MatrixBase(std::initializer_list<T> values);
  MatrixBase(T *data, size_t rows, size_t cols);
  virtual ~MatrixBase() = default;

  MatrixBase &operator = (const MatrixBase &matrix);
  MatrixBase &operator = (MatrixBase &&matrix) TL_NOEXCEPT;

  reference at(size_t r, size_t c);
  const_reference at(size_t r, size_t c) const;

  size_t rows() const;
  size_t cols() const;

  void swapRows(size_t i, size_t j);

  pointer data()
  {
    return mData;
  }

private:

  std::valarray<T> mData;
  size_t mRows;
  size_t mCols;

};


template<typename T> inline 
MatrixBase<T, DynamicMatrix, DynamicMatrix>::MatrixBase()
  : mRows(0),
    mCols(0)
{
}

template<typename T> inline
MatrixBase<T, DynamicMatrix, DynamicMatrix>::MatrixBase(size_t rows, size_t cols)
  : mData(-std::numeric_limits<T>().max(), rows *cols),
    mRows(rows),
    mCols(cols)
{
}

template<typename T> inline
MatrixBase<T, DynamicMatrix, DynamicMatrix>::MatrixBase(size_t rows, size_t cols, T val)
  : mData(val, rows * cols), 
    mRows(rows),
    mCols(cols)
{
}

template<typename T> inline 
MatrixBase<T, DynamicMatrix, DynamicMatrix>::MatrixBase(const MatrixBase &matrix)
  : mData(matrix.mData), 
    mRows(matrix.mRows),
    mCols(matrix.mCols)
{
}

template<typename T> inline 
MatrixBase<T, DynamicMatrix, DynamicMatrix>::MatrixBase(MatrixBase &&matrix) TL_NOEXCEPT
  : mData(std::move(matrix.mData)), 
    mRows(matrix.mRows),
    mCols(matrix.mCols)
    
{
}

template<typename T> inline 
MatrixBase<T, DynamicMatrix, DynamicMatrix>::MatrixBase(std::initializer_list<T> values)
{
  this->mRows = 1;
  this->mCols = values.size();

  std::copy(values.begin(), values.end(), mData.begin());
}

template<typename T>
inline MatrixBase<T, DynamicMatrix, DynamicMatrix>::MatrixBase(T *data, size_t rows, size_t cols)
  : mData(-std::numeric_limits<T>().max(), rows *cols),
    mRows(rows),
    mCols(cols)
{
  for (size_t i = 0; i < mData.size(); i++){
    mData[i] = data[i];
  }
}

template<typename T> inline 
MatrixBase<T, DynamicMatrix, DynamicMatrix> &MatrixBase<T, DynamicMatrix, DynamicMatrix>::operator = (const MatrixBase<T, DynamicMatrix, DynamicMatrix> &matrix)
{
  if (this != &matrix) {
    this->mData = matrix.mData;
    this->mRows = matrix.mRows;
    this->mCols = matrix.mCols;
  }
  return *this;
}

template<typename T> inline 
MatrixBase<T, DynamicMatrix, DynamicMatrix> &MatrixBase<T, DynamicMatrix, DynamicMatrix>::operator = (MatrixBase<T, DynamicMatrix, DynamicMatrix> &&matrix) TL_NOEXCEPT
{
  if (this != &matrix) {
    this->mData = std::move(matrix.mData);
    this->mRows = matrix.mRows;
    this->mCols = matrix.mCols;
  }
  return *this;
}

template<typename T> inline 
T &MatrixBase<T, DynamicMatrix, DynamicMatrix>::at(size_t r, size_t c)
{
  return mData[r * mCols + c];
}

template<typename T> inline 
const T &MatrixBase<T, DynamicMatrix, DynamicMatrix>::at(size_t r, size_t c) const
{
  return mData[r * mCols + c];
}

template<typename T> inline 
size_t MatrixBase<T, DynamicMatrix, DynamicMatrix>::rows() const
{
  return mRows;
}

template<typename T> inline 
size_t MatrixBase<T, DynamicMatrix, DynamicMatrix>::cols() const
{
  return mCols;
}

template<typename T> inline 
void MatrixBase<T, std::numeric_limits<size_t>().max(), DynamicMatrix>::swapRows(size_t i, size_t j)
{
  for (size_t c = 0; c < mCols; c++) {
    std::swap(mData[i* mCols + c], mData[j * mCols + c]);
  }
}



//////////////////////////////////////////////////////////////////////////////////////////////

#else

template<typename T>
class MatrixBase<T, DynamicMatrix, DynamicMatrix>
{

public:

  using value_type      = T;
  using size_type       = size_t;
  using pointer         = T*;
  using const_pointer   = const T*;
  using reference       = T&;
  using const_reference = const T&;

public:

  MatrixBase();
  MatrixBase(size_t rows, size_t cols);
  MatrixBase(size_t rows, size_t cols, T val);
  MatrixBase(const MatrixBase &matrix);
  MatrixBase(MatrixBase &&matrix) TL_NOEXCEPT;
  MatrixBase(std::initializer_list<T> values);
  MatrixBase(T *data, size_t rows = 0, size_t cols = 0);
  virtual ~MatrixBase() 
  {
    if (mData) {
      delete[] mData;

    }
  }

  MatrixBase &operator = (const MatrixBase &matrix);
  MatrixBase &operator = (MatrixBase &&matrix) TL_NOEXCEPT;

  reference at(size_t r, size_t c);
  const_reference at(size_t r, size_t c) const;

  size_t rows() const;
  size_t cols() const;

  void swapRows(size_t i, size_t j);

  pointer data()
  {
    return mData;
  }

private:

  //std::vector<std::vector<T>> mData;
  //std::valarray<T> mData;
  T *mData;
  size_t mRows;
  size_t mCols;

};


template<typename T> inline 
MatrixBase<T, DynamicMatrix, DynamicMatrix>::MatrixBase()
  : mData(nullptr),
    mRows(0),
    mCols(0)
{
}

template<typename T> inline
MatrixBase<T, DynamicMatrix, DynamicMatrix>::MatrixBase(size_t rows, size_t cols)
  : mData(new T[rows * cols]),
    mRows(rows),
    mCols(cols)
{
}

template<typename T> inline
MatrixBase<T, DynamicMatrix, DynamicMatrix>::MatrixBase(size_t rows, size_t cols, T val)
  : mRows(rows),
    mCols(cols)
{
  mData = new T[rows * cols];
  T ini_val = -std::numeric_limits<T>().max();
  for (size_t r = 0; r < rows * cols; r++) {
    this->mData[r] = ini_val;
  }
}

template<typename T> inline 
MatrixBase<T, DynamicMatrix, DynamicMatrix>::MatrixBase(const MatrixBase &matrix)
  : mRows(matrix.mRows),
    mCols(matrix.mCols)
{
  mData = new T[matrix.mRows * matrix.mCols];
  *mData = *matrix.mData;
  //for (size_t r = 0; r < _rows * _cols; r++) {
  //  this->mData[r] = matrix.at(r);
  //}
}

template<typename T> inline 
MatrixBase<T, DynamicMatrix, DynamicMatrix>::MatrixBase(MatrixBase &&matrix) TL_NOEXCEPT
  : mRows(matrix.mRows),
    mCols(matrix.mCols)
    
{
  mData = new T[matrix.mRows * matrix.mCols];
  *this->mData = std::move(*matrix.mData);
}

template<typename T> inline 
MatrixBase<T, DynamicMatrix, DynamicMatrix> &MatrixBase<T, DynamicMatrix, DynamicMatrix>::operator = (const MatrixBase<T, DynamicMatrix, DynamicMatrix> &matrix)
{
  if (this != &matrix) {
    mData = new T[matrix.mRows * matrix.mCols];
    *this->mData = *matrix.mData;
    this->mRows = matrix.mRows;
    this->mCols = matrix.mCols;
  }
  return *this;
}

template<typename T> inline 
MatrixBase<T, DynamicMatrix, DynamicMatrix> &MatrixBase<T, DynamicMatrix, DynamicMatrix>::operator = (MatrixBase<T, DynamicMatrix, DynamicMatrix> &&matrix) TL_NOEXCEPT
{
  if (this != &matrix) {
    mData = new T[matrix.mRows * matrix.mCols];
    *mData = *matrix.mData;
    //this->mData = std::move(matrix.mData);
    this->mRows = matrix.mRows;
    this->mCols = matrix.mCols;
  }
  return *this;
}

template<typename T> inline 
T &MatrixBase<T, DynamicMatrix, DynamicMatrix>::at(size_t r, size_t c)
{
  return mData[r * mCols + c];
}

template<typename T> inline 
const T &MatrixBase<T, DynamicMatrix, DynamicMatrix>::at(size_t r, size_t c) const
{
  return mData[r * mCols + c];
}

template<typename T> inline 
size_t MatrixBase<T, DynamicMatrix, DynamicMatrix>::rows() const
{
  return mRows;
}

template<typename T> inline 
size_t MatrixBase<T, DynamicMatrix, DynamicMatrix>::cols() const
{
  return mCols;
}

template<typename T> inline 
void MatrixBase<T, std::numeric_limits<size_t>().max(), DynamicMatrix>::swapRows(size_t i, size_t j)
{
  for (size_t c = 0; c < mCols; c++) {
    std::swap(mData[i* mCols + c], mData[j * mCols + c]);
  }
}

#endif

//////////////////////////////////////////////////////////////////////////////////////////////




/*!
 * \brief Clase matriz 
 *
 */
template<typename T, size_t _rows = DynamicMatrix, size_t _cols = DynamicMatrix>
class Matrix
  : public MatrixBase<T, _rows, _cols>
{

public:

  /*!
   * \brief Constructora por defecto
   */
  Matrix() = default;

  /*!
   * \brief Constructora filas-columnas
   */
  Matrix(size_t rows, size_t cols);

  /*!
   * \brief Constructora filas, columnas y valor
   */
  Matrix(size_t rows, size_t cols, T val);

  /*!
   * \brief Constructora de copia
   * \param[in] mat Objeto Matrix que se copia
   */
  Matrix(const Matrix &mat);

  /*!
   * \brief Constructor de movimiento
   * \param[in] mat Objeto Matrix que se mueve
   */
  Matrix(Matrix &&mat) TL_NOEXCEPT;

  Matrix(std::initializer_list<T> values);
  Matrix(std::initializer_list<std::initializer_list<T>> values);
  Matrix(T *data, size_t rows, size_t cols);

  /*!
   * \brief destructora
   */
  ~Matrix() override = default;

  /*!
   * \brief Operador de asignación de copia
   * \param[in] mat Objeto que se copia
   */
  Matrix &operator = (const Matrix &mat);
  
  /*!
   * \brief Operador de asignación de movimiento
   * \param[in] mat Objeto que se mueve
   */
  Matrix &operator = (Matrix &&mat) TL_NOEXCEPT;

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
  Matrix inverse(bool *invertibility = nullptr) const;

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
  Matrix<T, _cols, _rows> transpose() const;

  /*!
   * \brief Calcula la matriz de adjuntos
   * \f[ adj(A) = C^T \f]
   * \return Matriz de adjuntos
   */
  Matrix adjugate() const;

  /*!
   * \brief Calcula la matriz cofactor
   * \return Matriz cofactor
   */
  Matrix cofactorMatrix() const;

  const MatrixRow<const T> operator[](size_t position) const;
  MatrixRow<T> operator[](size_t position);
  const MatrixRow<const T> row(size_t row) const;
  MatrixRow<T> row(size_t row);
  const MatrixCol<const T> col(size_t col) const;
  MatrixCol<T> col(size_t col);

  const MatrixBlock<const T> block(size_t iniRow,
                                   size_t endRow,
                                   size_t iniCol,
                                   size_t endCol) const;
  MatrixBlock<T> block(size_t iniRow,
                       size_t endRow,
                       size_t iniCol,
                       size_t endCol);
  const MatrixBlock<const T> rowBlock(size_t iniRow, size_t endRow) const;
  MatrixBlock<T> rowBlock(size_t iniRow, size_t endRow);
  const MatrixBlock<const T> colBlock(size_t iniCol, size_t endCol) const;
  MatrixBlock<T> colBlock(size_t iniCol, size_t endCol);

  /*!
   * \brief Determinante de la matriz
   * \return Determinante
   */
  T determinant() const;

  /*!
   * \brief Traza de una matriz cuadrada
   * Suma de los elementos de la diagonal principal de una matriz cuadrada
   * \f[ tr(A) = a_{11} + ... +  a_{nn} \f]
   */
  T trace() const;

  /*!
   * \brief Comprueba si la matrix es invertible
   * \return Verdadero si la matriz es invertible
   */
  bool invertible();

  /*!
   * \brief Comprueba si la matrix es singular
   * Una matriz cuadrada que no tiene inversa es singular. El 
   * determinante de una matriz singular es 0
   * \return Verdadero si la matriz es singular
   */
  bool singular();

  /*!
   * \brief Cofactor
   * El determinante obtenido al eliminar la fila y la columna de un elemento dado de una matriz o determinante. 
   * El cofactor está precedido por un signo + o - dependiendo de si el elemento está en una posición + o -.
   * \f[ (-)^{r+j} \f]
   * \return cofactor
   */
  T cofactor(size_t r, size_t c) const;

  /*!
   * \brief Primero menor
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
   * \return Primero menor
   */
  T firstMinor(size_t r, size_t c) const;

  /*!
   * \brief Forma escalonada de fila
   */
  Matrix rowEchelonForm() const;

  /*!
   * \brief Forma escalonada de fila reducida
   */
  Matrix reducedRowEchelonForm() const;

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
  static Matrix zero();
  static Matrix zero(size_t rows, size_t cols);

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
  static Matrix ones();
  static Matrix ones(size_t rows, size_t cols);

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
  static Matrix identity();
  static Matrix identity(size_t rows, size_t cols);

  static Matrix randon();
  static Matrix randon(size_t rows, size_t cols);

  //static Matrix transpose(const Matrix &matrix);

private:


  T determinant2x2() const;
  T determinant3x3() const;
  T determinant4x4() const;
  T determinantnxn() const;
  Matrix inverse2x2(bool *invertibility) const;
  Matrix inverse3x3(bool *invertibility) const;
  Matrix inverse4x4(bool *invertibility) const;
  Matrix inversenxn(bool *invertibility) const;
  Matrix adjoint2x2() const;
  Matrix adjoint3x3() const;
  Matrix adjoint4x4() const;
  Matrix adjointnxn() const;

  /// por referencia
  void adjoint2x2(Matrix<T, _rows, _cols> &matrix) const;
  void adjoint3x3(Matrix<T, _rows, _cols> &matrix) const;
  void adjoint4x4(Matrix<T, _rows, _cols> &matrix) const;
  void adjointnxn(Matrix<T, _rows, _cols> &matrix) const;
  void cofactorMatrix(Matrix<T, _rows, _cols> &matrix) const;

};


/* Definición de alias Matrix */

using Matrix2x2i = Matrix<int, 2, 2>;
using Matrix2x2f = Matrix<float, 2, 2>;
using Matrix2x2d = Matrix<double, 2, 2>;
using Matrix3x3i = Matrix<int, 3, 3>;
using Matrix3x3f = Matrix<float, 3, 3>;
using Matrix3x3d = Matrix<double, 3, 3>;
using Matrix4x4i = Matrix<int, 4, 4>;
using Matrix4x4f = Matrix<float, 4, 4>;
using Matrix4x4d = Matrix<double, 4, 4>;



/* Implementación Matrix */


template<typename T, size_t _rows, size_t _cols> inline
Matrix<T, _rows, _cols>::Matrix(size_t rows, size_t cols)
  : MatrixBase<T, _rows, _cols>(rows, cols)
{
}

template<typename T, size_t _rows, size_t _cols> inline
Matrix<T, _rows, _cols>::Matrix(size_t rows, size_t cols, T val)
  : MatrixBase<T, _rows, _cols>(rows, cols, val)
{
}

template<typename T, size_t _rows, size_t _cols> inline
Matrix<T, _rows, _cols>::Matrix(const Matrix &matrix)
  : MatrixBase<T, _rows, _cols>(matrix)
{
}

template<typename T, size_t _rows, size_t _cols> inline
Matrix<T, _rows, _cols>::Matrix(std::initializer_list<T> values)
  : MatrixBase<T, _rows, _cols>(values)
{
}

template<typename T, size_t _rows, size_t _cols> inline
Matrix<T, _rows, _cols>::Matrix(std::initializer_list<std::initializer_list<T>> values)
  : MatrixBase<T, _rows, _cols>(values)
{

}

template<typename T, size_t _rows, size_t _cols> inline
Matrix<T, _rows, _cols>::Matrix(T *data, size_t rows, size_t cols)
  : MatrixBase<T, _rows, _cols>(data, rows, cols)
{
}

template<typename T, size_t _rows, size_t _cols> inline
Matrix<T, _rows, _cols>::Matrix(Matrix &&matrix) TL_NOEXCEPT
  : MatrixBase<T, _rows, _cols>(std::forward<MatrixBase<T, _rows, _cols>>(matrix))
{
}

template<typename T, size_t _rows, size_t _cols> inline
Matrix<T, _rows, _cols> &Matrix<T, _rows, _cols>::operator = (const Matrix &matrix)
{
  if (this != &matrix) {
    MatrixBase<T, _rows, _cols>::operator=(matrix);
  }
  return *this;
}

template<typename T, size_t _rows, size_t _cols> inline
Matrix<T, _rows, _cols> &Matrix<T, _rows, _cols>::operator = (Matrix &&matrix) TL_NOEXCEPT
{
  if (this != &matrix) {
    MatrixBase<T, _rows, _cols>::operator=(std::forward<MatrixBase<T, _rows, _cols>>(matrix));
  }
  return *this;
}

template<typename T, size_t _rows, size_t _cols> inline
Matrix<T, _rows, _cols> Matrix<T, _rows, _cols>::inverse(bool *invertibility) const
{
  static_assert(_rows == _cols, "Non-Square Matrix");
  static_assert(std::is_floating_point<T>::value, "Integral type not supported");

  Matrix<T, _rows, _cols> matrix;
  
  size_t rows = this->rows();

  if (rows == 2) {
    matrix = inverse2x2(invertibility);
  } else if (rows == 3) {
    matrix = inverse3x3(invertibility);
  } else if (rows == 4) {
    matrix = inverse4x4(invertibility);
  } else {
    matrix = inversenxn(invertibility);
  }

  return matrix;
}

template<typename T, size_t _rows, size_t _cols> inline
Matrix<T, _rows, _cols> Matrix<T, _rows, _cols>::inverse2x2(bool *invertibility) const
{
  Matrix<T, _rows, _cols> matrix(*this);

  T det = determinant2x2();
  if (det != consts::zero<T>) {
    matrix[0][0] =  this->operator()(1,1) / det;
    matrix[0][1] = -this->operator()(0,1) / det;
    matrix[1][0] = -this->operator()(1,0) / det;
    matrix[1][1] =  this->operator()(0,0) / det;
    if (invertibility) *invertibility = true;
  } else {
    if (invertibility) *invertibility = false;
  }

  return matrix;
}

template<typename T, size_t _rows, size_t _cols> inline
Matrix<T, _rows, _cols> Matrix<T, _rows, _cols>::inverse3x3(bool *invertibility) const
{
  Matrix<T, _rows, _cols> matrix(*this);

  Matrix<T, _rows, _cols> adj = this->adjoint3x3();
  T det = this->operator()(0,0) * adj.operator()(0, 0) 
        + this->operator()(0,1) * adj.operator()(1, 0) 
        + this->operator()(0,2) * adj.operator()(2, 0);

  if (det != consts::zero<T>) {

    matrix = adj / det;

    if (invertibility) *invertibility = true;
  } else {
    if (invertibility) *invertibility = false;
  }

  return matrix;
}

template<typename T, size_t _rows, size_t _cols> inline 
Matrix<T, _rows, _cols> Matrix<T, _rows, _cols>::inverse4x4(bool *invertibility) const
{
  Matrix<T, _rows, _cols> matrix(*this);

  T m00 = this->operator()(0,0);
  T m01 = this->operator()(0,1);
  T m02 = this->operator()(0,2);
  T m03 = this->operator()(0,3);
  T m10 = this->operator()(1,0);
  T m11 = this->operator()(1,1);
  T m12 = this->operator()(1,2);
  T m13 = this->operator()(1,3);
  T m20 = this->operator()(2,0);
  T m21 = this->operator()(2,1);
  T m22 = this->operator()(2,2);
  T m23 = this->operator()(2,3);
  T m30 = this->operator()(3,0);
  T m31 = this->operator()(3,1);
  T m32 = this->operator()(3,2);
  T m33 = this->operator()(3,3);

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

  if (det != consts::zero<T>) {
    
    matrix[0][0] = ( m11 * b5 - m12 * b4 + m13 * b3) / det;
    matrix[0][1] = (-m01 * b5 + m02 * b4 - m03 * b3) / det;
    matrix[0][2] = ( m31 * a5 - m32 * a4 + m33 * a3) / det;
    matrix[0][3] = (-m21 * a5 + m22 * a4 - m23 * a3) / det;
    matrix[1][0] = (-m10 * b5 + m12 * b2 - m13 * b1) / det;
    matrix[1][1] = ( m00 * b5 - m02 * b2 + m03 * b1) / det;
    matrix[1][2] = (-m30 * a5 + m32 * a2 - m33 * a1) / det;
    matrix[1][3] = ( m20 * a5 - m22 * a2 + m23 * a1) / det;
    matrix[2][0] = ( m10 * b4 - m11 * b2 + m13 * b0) / det;
    matrix[2][1] = (-m00 * b4 + m01 * b2 - m03 * b0) / det;
    matrix[2][2] = ( m30 * a4 - m31 * a2 + m33 * a0) / det;
    matrix[2][3] = (-m20 * a4 + m21 * a2 - m23 * a0) / det;
    matrix[3][0] = (-m10 * b3 + m11 * b1 - m12 * b0) / det;
    matrix[3][1] = ( m00 * b3 - m01 * b1 + m02 * b0) / det;
    matrix[3][2] = (-m30 * a3 + m31 * a1 - m32 * a0) / det;
    matrix[3][3] = ( m20 * a3 - m21 * a1 + m22 * a0) / det;

    if (invertibility) *invertibility = true;
  } else {
    if (invertibility) *invertibility = false;
  }

  return matrix;
}

template<typename T, size_t _rows, size_t _cols> inline 
Matrix<T, _rows, _cols> Matrix<T, _rows, _cols>::inversenxn(bool *invertibility) const
{
  Matrix<T, _rows, _cols> matrix(*this);

//#ifdef TL_INVERSE_ADJUGATE_DIV_DETERMINANT
  T det = determinantnxn();
  if (det != consts::zero<T>) {
    matrix = this->adjugate();
    matrix /= det;
    if (invertibility != nullptr) *invertibility = true;
  } else {
    if (invertibility != nullptr) *invertibility = false;
  }
//#else
//  LuDecomposition<Matrix<T, _rows, _cols>> lu(*this);
//  matrix = lu.inverse();
//#endif 

  return matrix;
}

template<typename T, size_t _rows, size_t _cols> inline 
Matrix<T, _cols, _rows> Matrix<T, _rows, _cols>::transpose() const
{
  size_t rows = this->rows();
  size_t cols = this->cols();
  Matrix<T, _cols, _rows> matrix(cols, rows);

  for (size_t r = 0; r < rows; r++) {
    for (size_t c = 0; c < cols; c++) {
      matrix[c][r] = this->operator()(r, c);
    }
  }

  return matrix;
}

template<typename T, size_t _rows, size_t _cols> inline
Matrix<T, _rows, _cols> Matrix<T, _rows, _cols>::adjugate() const
{
  static_assert(_rows == _cols, "Non-Square Matrix");
  size_t rows = this->rows();
  size_t cols = this->cols();
  TL_ASSERT(rows == cols, "Non-Square Matrix")

  Matrix<T, _rows, _cols> matrix(*this);

  if (rows == 2) {
    adjoint2x2(matrix);
  } else if (rows == 3) {
    adjoint3x3(matrix);
  } else if (rows == 4) {
    adjoint4x4(matrix);
  } else {
    adjointnxn(matrix);
  }

  return matrix;
}

template<typename T, size_t _rows, size_t _cols> inline
Matrix<T, _rows, _cols> Matrix<T, _rows, _cols>::adjoint2x2() const
{
  Matrix<T, _rows, _cols> matrix(*this);

  std::swap(matrix(0, 0), matrix(1, 1));
  matrix(0, 1) = -matrix(0, 1);
  matrix(1, 0) = -matrix(1, 0);

  return matrix;
}

template<typename T, size_t _rows, size_t _cols> inline
Matrix<T, _rows, _cols> Matrix<T, _rows, _cols>::adjoint3x3() const
{
  Matrix<T, _rows, _cols> matrix(*this);

  T m00 = this->operator()(0, 0);
  T m01 = this->operator()(0, 1);
  T m02 = this->operator()(0, 2);
  T m10 = this->operator()(1, 0);
  T m11 = this->operator()(1, 1);
  T m12 = this->operator()(1, 2);
  T m20 = this->operator()(2, 0);
  T m21 = this->operator()(2, 1);
  T m22 = this->operator()(2, 2);

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

template<typename T, size_t _rows, size_t _cols> inline
Matrix<T, _rows, _cols> Matrix<T, _rows, _cols>::adjoint4x4() const
{
  Matrix<T, _rows, _cols> matrix(*this);

  T m00 = this->operator()(0, 0);
  T m01 = this->operator()(0, 1);
  T m02 = this->operator()(0, 2);
  T m03 = this->operator()(0, 3);
  T m10 = this->operator()(1, 0);
  T m11 = this->operator()(1, 1);
  T m12 = this->operator()(1, 2);
  T m13 = this->operator()(1, 3);
  T m20 = this->operator()(2, 0);
  T m21 = this->operator()(2, 1);
  T m22 = this->operator()(2, 2);
  T m23 = this->operator()(2, 3);
  T m30 = this->operator()(3, 0);
  T m31 = this->operator()(3, 1);
  T m32 = this->operator()(3, 2);
  T m33 = this->operator()(3, 3);

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

  matrix(0, 0) =  m11 * b5 - m12 * b4 + m13 * b3;
  matrix(0, 1) = -m01 * b5 + m02 * b4 - m03 * b3;
  matrix(0, 2) =  m31 * a5 - m32 * a4 + m33 * a3;
  matrix(0, 3) = -m21 * a5 + m22 * a4 - m23 * a3;
  matrix(1, 0) = -m10 * b5 + m12 * b2 - m13 * b1;
  matrix(1, 1) =  m00 * b5 - m02 * b2 + m03 * b1;
  matrix(1, 2) = -m30 * a5 + m32 * a2 - m33 * a1;
  matrix(1, 3) =  m20 * a5 - m22 * a2 + m23 * a1;
  matrix(2, 0) =  m10 * b4 - m11 * b2 + m13 * b0;
  matrix(2, 1) = -m00 * b4 + m01 * b2 - m03 * b0;
  matrix(2, 2) =  m30 * a4 - m31 * a2 + m33 * a0;
  matrix(2, 3) = -m20 * a4 + m21 * a2 - m23 * a0;
  matrix(3, 0) = -m10 * b3 + m11 * b1 - m12 * b0;
  matrix(3, 1) =  m00 * b3 - m01 * b1 + m02 * b0;
  matrix(3, 2) = -m30 * a3 + m31 * a1 - m32 * a0;
  matrix(3, 3) =  m20 * a3 - m21 * a1 + m22 * a0;

  return matrix;
}

template<typename T, size_t _rows, size_t _cols> inline
Matrix<T, _rows, _cols> Matrix<T, _rows, _cols>::adjointnxn() const
{
  Matrix<T, _rows, _cols> matrix = this->cofactorMatrix();
  return matrix.transpose();
}

template<typename T, size_t _rows, size_t _cols> inline
void Matrix<T, _rows, _cols>::adjoint2x2(Matrix<T, _rows, _cols> &matrix) const
{
  std::swap(matrix(0, 0), matrix(1, 1));
  matrix(0, 1) = -matrix(0, 1);
  matrix(1, 0) = -matrix(1, 0);
}

template<typename T, size_t _rows, size_t _cols> inline
void Matrix<T, _rows, _cols>::adjoint3x3(Matrix<T, _rows, _cols> &matrix) const
{
  T m00 = this->operator()(0,0);
  T m01 = this->operator()(0,1);
  T m02 = this->operator()(0,2);
  T m10 = this->operator()(1,0);
  T m11 = this->operator()(1,1);
  T m12 = this->operator()(1,2);
  T m20 = this->operator()(2,0);
  T m21 = this->operator()(2,1);
  T m22 = this->operator()(2,2);

  matrix(0, 0) = m11 * m22 - m12 * m21;
  matrix(0, 1) = m02 * m21 - m01 * m22;
  matrix(0, 2) = m01 * m12 - m02 * m11;
  matrix(1, 0) = m12 * m20 - m10 * m22;
  matrix(1, 1) = m00 * m22 - m02 * m20;
  matrix(1, 2) = m02 * m10 - m00 * m12;
  matrix(2, 0) = m10 * m21 - m11 * m20;
  matrix(2, 1) = m01 * m20 - m00 * m21;
  matrix(2, 2) = m00 * m11 - m01 * m10;
}

template<typename T, size_t _rows, size_t _cols> inline
void Matrix<T, _rows, _cols>::adjoint4x4(Matrix<T, _rows, _cols> &matrix) const
{
  T m00 = this->operator()(0,0);
  T m01 = this->operator()(0,1);
  T m02 = this->operator()(0,2);
  T m03 = this->operator()(0,3);
  T m10 = this->operator()(1,0);
  T m11 = this->operator()(1,1);
  T m12 = this->operator()(1,2);
  T m13 = this->operator()(1,3);
  T m20 = this->operator()(2,0);
  T m21 = this->operator()(2,1);
  T m22 = this->operator()(2,2);
  T m23 = this->operator()(2,3);
  T m30 = this->operator()(3,0);
  T m31 = this->operator()(3,1);
  T m32 = this->operator()(3,2);
  T m33 = this->operator()(3,3);

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

  matrix[0][0] =  m11 * b5 - m12 * b4 + m13 * b3;
  matrix[0][1] = -m01 * b5 + m02 * b4 - m03 * b3;
  matrix[0][2] =  m31 * a5 - m32 * a4 + m33 * a3;
  matrix[0][3] = -m21 * a5 + m22 * a4 - m23 * a3;
  matrix[1][0] = -m10 * b5 + m12 * b2 - m13 * b1;
  matrix[1][1] =  m00 * b5 - m02 * b2 + m03 * b1;
  matrix[1][2] = -m30 * a5 + m32 * a2 - m33 * a1;
  matrix[1][3] =  m20 * a5 - m22 * a2 + m23 * a1;
  matrix[2][0] =  m10 * b4 - m11 * b2 + m13 * b0;
  matrix[2][1] = -m00 * b4 + m01 * b2 - m03 * b0;
  matrix[2][2] =  m30 * a4 - m31 * a2 + m33 * a0;
  matrix[2][3] = -m20 * a4 + m21 * a2 - m23 * a0;
  matrix[3][0] = -m10 * b3 + m11 * b1 - m12 * b0;
  matrix[3][1] =  m00 * b3 - m01 * b1 + m02 * b0;
  matrix[3][2] = -m30 * a3 + m31 * a1 - m32 * a0;
  matrix[3][3] =  m20 * a3 - m21 * a1 + m22 * a0;
}

template<typename T, size_t _rows, size_t _cols> inline 
void Matrix<T, _rows, _cols>::adjointnxn(Matrix<T, _rows, _cols> &matrix) const
{
  this->cofactorMatrix(matrix);
  matrix = matrix.transpose();
}

template<typename T, size_t _rows, size_t _cols> inline
Matrix<T, _rows, _cols> Matrix<T, _rows, _cols>::cofactorMatrix() const
{
  static_assert(_rows == _cols, "Non-Square Matrix");

  size_t rows = this->rows();
  size_t cols = this->cols();

  TL_ASSERT(rows == cols, "Non-Square Matrix")

  Matrix<T, _rows, _cols> matrix(*this);
  for (size_t r = 0; r < rows; r++) {
    for (size_t c = 0; c < cols; c++) {
      matrix[r][c] = cofactor(r, c);
    }
  }

  return matrix;
}

template<typename T, size_t _rows, size_t _cols> inline
const MatrixRow<const T> Matrix<T, _rows, _cols>::operator[](size_t position) const
{
  return MatrixRow<const T>(this->data(), position, this->cols());
}

template<typename T, size_t _rows, size_t _cols> inline
MatrixRow<T> Matrix<T, _rows, _cols>::operator[](size_t position)
{
  return MatrixRow<T>(this->data(), position, this->cols());
}

template<typename T, size_t _rows, size_t _cols> inline
const MatrixRow<const T> Matrix<T, _rows, _cols>::row(size_t row) const
{
  return MatrixRow<const T>(this, row);
}

template<typename T, size_t _rows, size_t _cols> inline
MatrixRow<T> Matrix<T, _rows, _cols>::row(size_t row)
{
  return MatrixRow<T>(this->data(), row, this->cols());
}

template<typename T, size_t _rows, size_t _cols> inline
const MatrixCol<const T> Matrix<T, _rows, _cols>::col(size_t col) const
{
  return MatrixCol<const T>(this->data(), col, this->rows(), this->cols());
}

template<typename T, size_t _rows, size_t _cols> inline
MatrixCol<T> Matrix<T, _rows, _cols>::col(size_t col)
{
  return MatrixCol<T>(this->data(), col, this->rows(), this->cols());
}

template<typename T, size_t _rows, size_t _cols> inline
const MatrixBlock<const T> Matrix<T, _rows, _cols>::block(size_t iniRow,
                                                          size_t endRow,
                                                          size_t iniCol,
                                                          size_t endCol) const
{
  return MatrixBlock<const T>(this->data(), this->rows(), this->cols(), iniRow, endRow, iniCol, endCol);
}

template<typename T, size_t _rows, size_t _cols> inline
MatrixBlock<T> Matrix<T, _rows, _cols>::block(size_t iniRow,
                                              size_t endRow,
                                              size_t iniCol,
                                              size_t endCol)
{
  return MatrixBlock<T>(this->data(), this->rows(), this->cols(), iniRow, endRow, iniCol, endCol);
}

template<typename T, size_t _rows, size_t _cols> inline
const MatrixBlock<const T> Matrix<T, _rows, _cols>::rowBlock(size_t iniRow, size_t endRow) const
{
  return block(iniRow, endRow, 0, this->cols() - 1);
}

template<typename T, size_t _rows, size_t _cols> inline
MatrixBlock<T> Matrix<T, _rows, _cols>::rowBlock(size_t iniRow, size_t endRow)
{
  return block(iniRow, endRow, 0, this->cols() - 1);
}

template<typename T, size_t _rows, size_t _cols> inline
const MatrixBlock<const T> Matrix<T, _rows, _cols>::colBlock(size_t iniCol, size_t endCol) const
{
  return block(0, this->rows() - 1, iniCol, endCol);
}

template<typename T, size_t _rows, size_t _cols> inline
MatrixBlock<T> Matrix<T, _rows, _cols>::colBlock(size_t iniCol, size_t endCol)
{
  return block(0, this->rows() - 1, iniCol, endCol);
}

template<typename T, size_t _rows, size_t _cols> inline
void Matrix<T, _rows, _cols>::cofactorMatrix(Matrix<T, _rows, _cols> &matrix) const
{
  static_assert(_rows == _cols, "Non-Square Matrix");

  size_t rows = this->rows();
  size_t cols = this->cols();
  TL_ASSERT(rows == cols, "Non-Square Matrix")

  for (size_t r = 0; r < rows; r++) {
    for (size_t c = 0; c < cols; c++) {
      matrix[r][c] = cofactor(r, c);
    }
  }
}


template<typename T, size_t _rows, size_t _cols> inline
Matrix<T, _rows, _cols> Matrix<T, _rows, _cols>::rowEchelonForm() const
{
  size_t rows = this->rows();
  size_t cols = this->cols();

  Matrix<T, _rows, _cols> matrix(*this);

  for (size_t i = 0; i < rows; ++i) {
    T pivotElement = matrix[i][i];
    size_t pivotRow = i;
    for (size_t r = i + 1; r < rows; ++r) {
      if (std::abs(matrix[r][i]) > std::abs(pivotElement)) {
        pivotElement = matrix[r][i];
        pivotRow = r;
      }
    }

    if (pivotElement == consts::zero<T>) {
      break;
    }

    if (pivotRow != i) {
      matrix.swapRows(i, pivotRow);
    }

    matrix[i] /= pivotElement;
    //matrix[i] = matrix[i] / pivotElement;

    for (size_t r = i + 1; r < rows; ++r) {
      T scale = matrix[r][i];
      for (size_t c = i; c < cols; ++c) {
        matrix[r][c] = matrix[r][c] - scale * matrix[i][c];
      }
    }
  }

  return matrix;
}

template<typename T, size_t _rows, size_t _cols> inline
Matrix<T, _rows, _cols> Matrix<T, _rows, _cols>::reducedRowEchelonForm() const
{
  Matrix<T, _rows, _cols> matrix = this->rowEchelonForm();

  size_t rows = this->rows();
  size_t cols = this->cols();

  for (size_t i = rows; i > 0; --i) {

    for (size_t r = i-1; r > 0; --r) {

      T scale = matrix[r-1][i-1];

      for (size_t c = i-1; c < cols; ++c) {
        matrix[r-1][c] = matrix[r-1][c] - scale * matrix[i-1][c];
      }
    }

  }

  return matrix;
}

template<typename T, size_t _rows, size_t _cols> inline
T Matrix<T, _rows, _cols>::determinant() const
{
  static_assert(_rows == _cols, "Non-Square Matrix");

  T d = consts::one<T>;
  size_t rows = this->rows();

  if (rows == 2) {
    d = determinant2x2();
  } else if (rows == 3) {
    d = determinant3x3();
  } else if (rows == 4) {
    d = determinant4x4();
  } else {
    d = determinantnxn();
  }

  return d;
}

template<typename T, size_t _rows, size_t _cols> inline
T Matrix<T, _rows, _cols>::determinant2x2() const
{
  T det = this->operator()(0,0) * this->operator()(1,1) - this->operator()(0,1) * this->operator()(1,0);
  return det;
}

template<typename T, size_t _rows, size_t _cols> inline
T Matrix<T, _rows, _cols>::determinant3x3() const
{
  T m00 = this->operator()(0, 0);
  T m01 = this->operator()(0, 1);
  T m02 = this->operator()(0, 2);
  T m10 = this->operator()(1, 0);
  T m11 = this->operator()(1, 1);
  T m12 = this->operator()(1, 2);
  T m20 = this->operator()(2, 0);
  T m21 = this->operator()(2, 1);
  T m22 = this->operator()(2, 2);

  T c00 = m11 * m22 - m12 * m21;
  T c10 = m12 * m20 - m10 * m22;
  T c20 = m10 * m21 - m11 * m20;

  T det = m00 * c00 + m01 * c10 + m02 * c20;

  return det;
}

template<typename T, size_t _rows, size_t _cols> inline
T Matrix<T, _rows, _cols>::determinant4x4() const
{
  TL_TODO("codigo repetido con adjoint4x4")
  T m00 = this->operator()(0,0);
  T m01 = this->operator()(0,1);
  T m02 = this->operator()(0,2);
  T m03 = this->operator()(0,3);
  T m10 = this->operator()(1,0);
  T m11 = this->operator()(1,1);
  T m12 = this->operator()(1,2);
  T m13 = this->operator()(1,3);
  T m20 = this->operator()(2,0);
  T m21 = this->operator()(2,1);
  T m22 = this->operator()(2,2);
  T m23 = this->operator()(2,3);
  T m30 = this->operator()(3,0);
  T m31 = this->operator()(3,1);
  T m32 = this->operator()(3,2);
  T m33 = this->operator()(3,3);

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

template<typename T, size_t _rows, size_t _cols> inline
T Matrix<T, _rows, _cols>::determinantnxn() const
{
  T d = consts::one<T>;
  size_t rows = this->rows();
  size_t cols = this->cols();

  Matrix<T, _rows, _cols> matrix(*this);

  for (size_t i = 0; i < rows; ++i) {
    T pivotElement = matrix[i][i];
    size_t pivotRow = i;
    for (size_t r = i + 1; r < rows; ++r) {
      if (std::abs(matrix[r][i]) > std::abs(pivotElement)) {
        pivotElement = matrix[r][i];
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
        matrix[r][c] -= matrix[r][i] * matrix[i][c] / pivotElement;
      }
    }
  }

  return d;
}

template<typename T, size_t _rows, size_t _cols> inline
T Matrix<T, _rows, _cols>::trace() const
{
  static_assert(_rows == _cols, "Non-Square Matrix");

  size_t rows = this->rows();
  size_t cols = this->cols();
  
  TL_ASSERT(rows == cols, "Non-Square Matrix")

  T trace = consts::zero<T>;
  for (size_t r = 0; r < rows; r++) {
    trace += this->operator()(r, r);
  }
  return trace;
}

template<typename T, size_t _rows, size_t _cols> inline
bool Matrix<T, _rows, _cols>::invertible()
{
  T det = determinant();
  return (det != consts::zero<T>);
}

template<typename T, size_t _rows, size_t _cols> inline
bool Matrix<T, _rows, _cols>::singular()
{
  T det = determinant();
  return (det == consts::zero<T>);
}

template<typename T, size_t _rows, size_t _cols> inline 
T Matrix<T, _rows, _cols>::cofactor(size_t r, size_t c) const
{
  static_assert(_rows == _cols, "Non-Square Matrix");

  T sign = ((r + c) % 2 == 0) ? consts::one<T> : -consts::one<T>;
  return sign * this->firstMinor(r, c);
}

template<typename T, size_t _rows, size_t _cols> inline 
T Matrix<T, _rows, _cols>::firstMinor(size_t r, size_t c) const
{
  static_assert(_rows == _cols, "Non-Square Matrix");

  size_t rows = this->rows();
  size_t cols = this->cols();
  
  TL_ASSERT(rows == cols, "Non-Square Matrix")

  T determinant = consts::zero<T>;

  size_t i = 0;
  size_t j = 0;

  constexpr static size_t _r = _rows == DynamicMatrix ? DynamicMatrix : _rows - 1;
  constexpr static size_t _c = _cols == DynamicMatrix ? DynamicMatrix : _cols - 1;

  if (_rows == DynamicMatrix && _cols == DynamicMatrix) {
    Matrix<T> matrix(rows-1, cols-1);
    for (size_t row = 0; row < rows; row++) {
      for (size_t col = 0; col < cols; col++) {
        if (row != r && col != c) {
          matrix[i][j++] = this->operator()(row,col);
          if (j == rows - 1) {
            j = 0;
            i++;
          }
        }
      }
    }
    determinant = matrix.determinant();
  } else {
    Matrix<T, _r, _c> matrix;
    for (size_t row = 0; row < rows; row++) {
      for (size_t col = 0; col < cols; col++) {
        if (row != r && col != c) {
          matrix[i][j++] = this->operator()(row, col);
          if (j == rows - 1) {
            j = 0;
            i++;
          }
        }
      }
    }
    determinant = matrix.determinant();
  }

  return determinant;
}

template<typename T, size_t _rows, size_t _cols> inline 
Matrix<T, _rows, _cols> Matrix<T, _rows, _cols>::zero()
{
  Matrix<T, _rows, _cols> matrix;

  for (size_t r = 0; r < matrix.rows(); r++) {
    for (size_t c = 0; c < matrix.cols(); c++) {
      matrix(r, c) = consts::zero<T>;
    }
  }

  return matrix;
}

template<typename T, size_t _rows, size_t _cols> inline 
Matrix<T, _rows, _cols> Matrix<T, _rows, _cols>::zero(size_t rows, size_t cols)
{
  static_assert(_rows == DynamicMatrix || _cols == DynamicMatrix, "Dynamic Matrix not support resize");

  return Matrix<T>(rows, cols, consts::zero<T>);
}

template<typename T, size_t _rows, size_t _cols> inline 
Matrix<T, _rows, _cols> Matrix<T, _rows, _cols>::ones()
{
  Matrix<T, _rows, _cols> matrix;

  for (size_t r = 0; r < matrix.rows(); r++) {
    for (size_t c = 0; c < matrix.cols(); c++) {
      matrix(r, c) = consts::one<T>;
    }
  }

  return matrix;
}

template<typename T, size_t _rows, size_t _cols> inline 
Matrix<T, _rows, _cols> Matrix<T, _rows, _cols>::ones(size_t rows, size_t cols)
{
  static_assert(_rows == DynamicMatrix || _cols == DynamicMatrix, "Dynamic Matrix not support resize");

  return Matrix<T>(rows, cols, consts::one<T>);
}

template<typename T, size_t _rows, size_t _cols> inline 
Matrix<T, _rows, _cols> Matrix<T, _rows, _cols>::identity()
{
  Matrix<T, _rows, _cols> matrix;

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

template<typename T, size_t _rows, size_t _cols> inline 
Matrix<T, _rows, _cols> Matrix<T, _rows, _cols>::identity(size_t rows, size_t cols)
{
  static_assert(_rows == DynamicMatrix || _cols == DynamicMatrix, "Dynamic Matrix not support resize");

  Matrix<T> matrix(rows, cols);

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

template<typename T, size_t _rows, size_t _cols> inline
Matrix<T, _rows, _cols> Matrix<T, _rows, _cols>::randon()
{
  Matrix<T, _rows, _cols> matrix;

  std::random_device rd;
  std::mt19937 random_number_engine(rd());
  std::uniform_real_distribution<> distribution(0.0, 99.0);

  for (size_t r = 0; r < matrix.rows(); r++) {
    for (size_t c = 0; c < matrix.cols(); c++) {
      matrix(r, c) = distribution(random_number_engine);
    }
  }

  return matrix;
}

template<typename T, size_t _rows, size_t _cols> inline
Matrix<T, _rows, _cols> Matrix<T, _rows, _cols>::randon(size_t rows, size_t cols)
{
  static_assert(_rows == DynamicMatrix || _cols == DynamicMatrix, "Dynamic Matrix not support resize");

  Matrix<T> matrix(rows, cols);

  std::random_device rd;
  std::mt19937 random_number_engine(rd());
  std::uniform_real_distribution<> distribution(0.0, 99.0);

  for (size_t r = 0; r < matrix.rows(); r++) {
    for (size_t c = 0; c < matrix.cols(); c++) {
      matrix(r, c) = distribution(random_number_engine);
    }
  }

  return matrix;
}



/* Operaciones unarias */

template<typename T, size_t _rows, size_t _cols> inline  static
Matrix<T, _rows, _cols> operator + (const Matrix<T, _rows, _cols> &matrix)
{
  return matrix;
}

/*!
 * \brief Cambia el signo de una matriz
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
template<typename T, size_t _rows, size_t _cols> inline static
Matrix<T, _rows, _cols> operator - (const Matrix<T, _rows, _cols> &matrix)
{
  static_assert(std::is_signed<T>::value, "Requires signed type");

  Matrix<T, _rows, _cols> _m(matrix);

  for (size_t r = 0; r < matrix.rows(); r++) {
    for (size_t c = 0; c < matrix.cols(); c++) {
      _m(r, c) = -_m(r, c);
    }
  }

  return _m;
}


/* Operaciones binarias entre matrices */


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
template<typename T, size_t _rows, size_t _cols> inline static
Matrix<T, _rows, _cols> operator + (const Matrix<T, _rows, _cols> &matrix1,
                                    const Matrix<T, _rows, _cols> &matrix2)
{
  Matrix<T, _rows, _cols> matrix = matrix1;
  return matrix += matrix2;
}

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
template<typename T, size_t _rows, size_t _cols> inline static
Matrix<T, _rows, _cols> &operator += (Matrix<T, _rows, _cols> &matrix1,
                                      const Matrix<T, _rows, _cols> &matrix2)
{
  size_t rows1 = matrix1.rows();
  size_t cols1 = matrix1.cols();
  size_t rows2 = matrix2.rows();
  size_t cols2 = matrix2.cols();

  TL_ASSERT(rows1 == rows2 && cols1 == cols2, "A size != B size")

  for (size_t r = 0; r < rows1; r++) {
    for (size_t c = 0; c < cols1; c++) {
      matrix1(r, c) += matrix2(r, c);
    }
  }

  return matrix1;
}

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
template<typename T, size_t _rows, size_t _cols> inline static
Matrix<T, _rows, _cols> operator - (const Matrix<T, _rows, _cols> &matrix1,
                                    const Matrix<T, _rows, _cols> &matrix2)
{
  Matrix<T, _rows, _cols> matrix = matrix1;
  return matrix -= matrix2;
}

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
template<typename T, size_t _rows, size_t _cols> inline static
Matrix<T, _rows, _cols> &operator -= (Matrix<T, _rows, _cols> &matrix1,
                                      const Matrix<T, _rows, _cols> &matrix2)
{
  size_t rows1 = matrix1.rows();
  size_t cols1 = matrix1.cols();
  size_t rows2 = matrix2.rows();
  size_t cols2 = matrix2.cols();

  TL_ASSERT(rows1 == rows2 && cols1 == cols2, "A size != B size")


  for (size_t r = 0; r < matrix1.rows(); r++) {
    for (size_t c = 0; c < matrix1.cols(); c++) {
      matrix1(r, c) -= matrix2(r, c);
    }
  }
  

  return matrix1;
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

  T temp = math::consts::zero<T>;
  for (size_t r = 0; r < _rows; r++) {
    for (size_t i = 0; i < _dim; i++) {
      T a = matrix1(r, i);
      for (size_t c = 0; c < _cols; c++) {
        matrix(r, c) += a * matrix2(i, c);
      }
    }
  }

  return matrix;
}

template<typename T> inline
Matrix<T> operator * (const Matrix<T> &matrix1,
                      const Matrix<T> &matrix2)
{
  size_t rows = matrix1.rows();
  size_t dim1 = matrix1.cols();
  size_t cols = matrix2.cols();
  size_t dim2 = matrix2.rows();

  TL_ASSERT(dim1 == dim2, "A columns != B rows")

  Matrix<T> matrix = Matrix<T>::zero(rows, cols);

  for (size_t r = 0; r < rows; r++) {
    for (size_t i = 0; i < dim1; i++) {
      T a = matrix1(r, i);
      for (size_t c = 0; c < cols; c++) {
        matrix(r, c) += a * matrix2(i, c);
      }
    }
  }
  
  return matrix;
}


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
template<typename T, size_t _rows, size_t _cols> inline static
Matrix<T, _rows, _cols> operator * (const Matrix<T, _rows, _cols> &matrix,
                                     T scalar)
{
  Matrix<T, _rows, _cols> _matrix = matrix;
  return _matrix *= scalar;
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
template<typename T, size_t _rows, size_t _cols> inline static
Matrix<T, _rows, _cols> operator * (T scalar, const Matrix<T, _rows, _cols> &matrix)
{
  Matrix<T, _rows, _cols> _matrix = matrix;
  return _matrix *= scalar;
}

template<typename T, size_t _rows, size_t _cols> inline static
Matrix<T, _rows, _cols> &operator *= (Matrix<T, _rows, _cols> &matrix, T scalar)
{
  for (size_t r = 0; r < matrix.rows(); r++) {
    for (size_t c = 0; c < matrix.cols(); c++) {
      matrix(r, c) *= scalar;
    }
  }

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
Matrix<T, _rows, _cols> operator / (const Matrix<T, _rows, _cols> &matrix, T scalar)
{
  Matrix<T, _rows, _cols> _matrix = matrix;
  return _matrix /= scalar;
}

template<typename T, size_t _rows, size_t _cols> inline static
Matrix<T, _rows, _cols> &operator /= (Matrix<T, _rows, _cols> &matrix, T scalar)
{
  if (scalar != consts::zero<T>) {
    for (size_t r = 0; r < matrix.rows(); r++) {
      for (size_t c = 0; c < matrix.cols(); c++) {
        matrix(r, c) /= scalar;
      }
    }
  } else {
    matrix = Matrix<T, _rows, _cols>::zero();
  }

  return matrix;
}

template<typename T> inline static
Matrix<T> &operator /= (Matrix<T> &matrix, T scalar)
{
  if (scalar != consts::zero<T>) {
    for (size_t r = 0; r < matrix.rows(); r++) {
      for (size_t c = 0; c < matrix.cols(); c++) {
        matrix(r, c) /= scalar;
      }
    }
  } else {
    matrix = Matrix<T>::zero(matrix.rows(), matrix.cols());
  }
  return matrix;
}




template<typename T, size_t _rows, size_t _cols>
std::ostream &operator<< (std::ostream &os, const Matrix<T, _rows, _cols> &matrix) 
{
  for (size_t r = 0; r < matrix.rows(); r++) {
    for (size_t c = 0; c < matrix.cols(); c++) {
      os << " " << matrix(r, c);
    }
    os << "\n";
  }
  os << std::flush;
  return os;
}

template<typename T, size_t _rows, size_t _cols>
std::ostream &operator<< (std::ostream &os, const Matrix<T, _rows, _cols> *matrix)
{
  for (size_t r = 0; r < matrix->rows(); r++) {
    for (size_t c = 0; c < matrix->cols(); c++) {
      os << " " << matrix(r, c);
    }
    os << "\n";
  }
  os << std::flush;
  return os;
}




template<typename T, size_t _rows, size_t _dim> inline  static
Vector<T, _rows> operator * (const Matrix<T, _rows, _dim> &matrix,
                             const Vector<T, _dim> &vector)
{
  Vector<T, _rows> vect = Vector<T, _rows>::zero();

  for (size_t r = 0; r < _rows; r++) {
    for (size_t c = 0; c < _dim; c++) {
        vect[r] += matrix(r, c) * vector[c];
    }
  }

  return vect;
}

template<typename T> inline  
static Vector<T> operator * (const Matrix<T> &matrix,
                             const Vector<T> &vector)
{
  size_t rows = matrix.rows();
  size_t dim1 = matrix.cols();
  size_t dim2 = vector.size();

  TL_ASSERT(dim1 == dim2, "Matrix columns != Vector size")

  Vector<T> vect = Vector<T>::zero(rows);

  for (size_t r = 0; r < rows; r++) {
    for (size_t c = 0; c < dim1; c++) {
      vect[r] += matrix(r, c) * vector[c];
    }
  }

  return vect;
}


template<typename T, size_t _rows, size_t _cols> inline static
bool operator == (const Matrix<T, _rows, _cols> &matrix1,
                  const Matrix<T, _rows, _cols> &matrix2)
{
  size_t rows1 = matrix1.rows();
  size_t cols1 = matrix1.cols();
  size_t rows2 = matrix2.rows();
  size_t cols2 = matrix2.cols();
  if (rows1 != rows2 || cols1 != cols2) return false;

  for (size_t r = 0; r < matrix1.rows(); r++) {
    for (size_t c = 0; c < matrix1.cols(); c++) {
      if (matrix1(r, c) != matrix2(r, c)) return false;
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
  if (rows1 != rows2 || cols1 != cols2) return true;

  for (size_t r = 0; r < matrix1.rows(); r++) {
    for (size_t c = 0; c < matrix1.cols(); c++) {
      if (matrix1(r,c) != matrix2(r,c)) return true;
    }
  }

  return false;
}


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

  pointer mPtr;

public:

  IteratorRows(pointer ptr)
    : mPtr(ptr)
  {
  }

  reference operator*() const
  {
    return *mPtr;
  }

  pointer operator->()
  {
    return mPtr;
  }

  IteratorRows &operator++()
  {
    mPtr++;
    return *this;
  }

  IteratorRows operator++(int)
  {
    IteratorRows it = *this;
    ++(*this);
    return it;
  }

  friend bool operator== (const IteratorRows &a, const IteratorRows &b)
  {
    return a.mPtr == b.mPtr;
  }

  friend bool operator!= (const IteratorRows &a, const IteratorRows &b)
  {
    return a.mPtr != b.mPtr;
  }

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

  pointer mPtr;
  size_t mColSize;

public:

  IteratorCols(pointer ptr, size_t colSize)
    : mPtr(ptr),
      mColSize(colSize)
  {
  }

  reference operator*() const
  {
    return *mPtr;
  }

  pointer operator->()
  {
    return mPtr;
  }

  IteratorCols &operator++()
  {
    mPtr += mColSize;
    return *this;
  }

  IteratorCols operator++(int)
  {
    IteratorCols it = *this;
    ++(*this);
    return it;
  }

  friend bool operator== (const IteratorCols &a, const IteratorCols &b)
  {
    return a.mPtr == b.mPtr;
  }

  friend bool operator!= (const IteratorCols &a, const IteratorCols &b)
  {
    return a.mPtr != b.mPtr;
  }

};


/// Clase MatrixRow

/// ¿Heredar MatrixRow y MatrixCol de Vector<T> para no duplicar las operaciones entre vectores??

template<typename T>
class MatrixRow
  //: public Vector<T>
{

public:

  using value_type = T;
  using size_type = size_t;
  using pointer = T *;
  using const_pointer = const T *;
  using reference = T &;
  using const_reference = const T &;

  using iterator = IteratorRows<T>;
  using const_iterator = IteratorRows<const T>;
  //using reverse_iterator = std::reverse_iterator<iterator>;
  //using const_reverse_iterator = std::const_reverse_iterator<const_iterator>;

public:

  MatrixRow(T *data, size_t row, size_t cols)
    : mData(data),
      mRow(row),
      mCols(cols)
  {}
  MatrixRow(const MatrixRow &matrixRow)
    : mData(matrixRow.mData),
      mRow(matrixRow.mRow),
      mCols(matrixRow.mCols)
  {}
  ~MatrixRow(){}

  const_reference operator[](size_t column) const
  {
    return mData[mRow * mCols + column];
  }

  reference operator[](size_t column)
  {
    return mData[mRow * mCols + column];
  }

  iterator begin() TL_NOEXCEPT
  {
    return iterator(&mData[mRow * mCols]);
  }

  iterator end() TL_NOEXCEPT
  {
    return iterator(&mData[mRow * mCols]  + mCols);
  }

  size_t size() const
  {
    return mCols;
  }

  void operator=(T value)
  {
    std::fill(begin(), end(), value);
  }

  MatrixRow<T> &operator += (const MatrixRow<T> &matrixRow)
  {
    TL_ASSERT(this->size() == matrixRow.size(), "")

    for (size_t i = 0; i < this->size(); i++) {
      (*this)[i] += matrixRow[i];
    }

    return *this;
  }

  MatrixRow<T> &operator -= (const MatrixRow<T> &matrixRow)
  {
    TL_ASSERT(this->size() == matrixRow.size(), "")

    for (size_t i = 0; i < this->size(); i++) {
      (*this)[i] -= matrixRow[i];
    }

    return *this;
  }

  MatrixRow<T> &operator *= (const MatrixRow<T> &matrixRow)
  {
    TL_ASSERT(this->size() == matrixRow.size(), "")

    for (size_t i = 0; i < this->size(); i++) {
      (*this)[i] *= matrixRow[i];
    }

    return *this;
  }

  MatrixRow<T> &operator /= (const MatrixRow<T> &matrixRow)
  {
    TL_ASSERT(this->size() == matrixRow.size(), "")

    for (size_t i = 0; i < this->size(); i++) {
      (*this)[i] /= matrixRow[i];
    }

    return *this;
  }

  MatrixRow<T> &operator *= (T scalar)
  {
    for (size_t i = 0; i < this->size(); i++) {
      (*this)[i] *= scalar;
    }
    return *this;
  }

  MatrixRow<T> &operator /= (T scalar)
  {
    if (scalar != consts::zero<T>) {
      for (size_t i = 0; i < this->size(); i++) {
        (*this)[i] /= scalar;
      }
    } else {
      for (size_t i = 0; i < this->size(); i++) {
        (*this)[i] = static_cast<T>(0);
      }
    }
    return *this;
  }

private:

  T *mData;
  size_t mRow;
  size_t mCols;
};


/// Clase MatrixCol

template<typename T>
class MatrixCol
{

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

  MatrixCol(T *data, size_t col, size_t rows, size_t cols)
    : mData(data),
      mCol(col),
      mRows(rows),
      mCols(cols)
  { }
  ~MatrixCol(){ }

  const_reference operator[](size_t row) const
  {
    return mData[row * mCols + mCol];
  }

  reference operator[](size_t row)
  {
    return mData[row * mCols + mCol];
  }

  iterator begin() TL_NOEXCEPT
  {
    return iterator(&mData[mCol], mCols);
  }

  iterator end() TL_NOEXCEPT
  {
    return iterator(&mData[mCol] + mRows * mCols, mCols);
  }

  size_t size() const
  {
    return mRows;
  }

  void operator=(T value)
  {
    std::fill(begin(), end(), value);
  }

  MatrixCol<T> &operator += (const MatrixCol<T> &matrixCol)
  {
    TL_ASSERT(this->size() == matrixCol.size(), "")

    for (size_t i = 0; i < this->size(); i++) {
      (*this)[i] += matrixCol[i];
    }

    return *this;
  }

  MatrixCol<T> &operator -= (const MatrixCol<T> &matrixCol)
  {
    TL_ASSERT(this->size() == matrixCol.size(), "")

    for (size_t i = 0; i < this->size(); i++) {
      (*this)[i] -= matrixCol[i];
    }

    return *this;
  }

  MatrixCol<T> &operator *= (const MatrixCol<T> &matrixCol)
  {
    TL_ASSERT(this->size() == matrixCol.size(), "")

    for (size_t i = 0; i < this->size(); i++) {
      (*this)[i] *= matrixCol[i];
    }

    return *this;
  }

  MatrixCol<T> &operator /= (const MatrixCol<T> &matrixCol)
  {
    TL_ASSERT(this->size() == matrixCol.size(), "")

      for (size_t i = 0; i < this->size(); i++) {
        (*this)[i] /= matrixCol[i];
      }

    return *this;
  }

  MatrixCol<T> &operator *= (T scalar)
  {
    for (size_t i = 0; i < this->size(); i++) {
      (*this)[i] *= scalar;
    }
    return *this;
  }

  MatrixCol<T> &operator /= (T scalar)
  {
    if (scalar != consts::zero<T>) {
      for (size_t i = 0; i < this->size(); i++) {
        (*this)[i] /= scalar;
      }
    } else {
      for (size_t i = 0; i < this->size(); i++) {
        (*this)[i] = static_cast<T>(0);
      }
    }
    return *this;
  }


private:

  T *mData;
  size_t mCol;
  size_t mRows;
  size_t mCols;
};


/* Operaciones unarias para filas */

template<typename T>
MatrixRow<T> operator + (const MatrixRow<T> &row)
{
  return row;
}

template<typename T>
MatrixRow<T> operator - (const MatrixRow<T> &row)
{
  static_assert(std::is_signed<T>::value, "Requires signed type");

  MatrixRow<T> r = row;
  for (size_t i = 0; i < row.size(); i++) {
    r[i] = -row[i];
  }

  return r;
}

/* Operaciones binarias para filas */

template<typename T>
MatrixRow<T> operator + (const MatrixRow<T> &r0,
                         const MatrixRow<T> &r1)
{
  MatrixRow<T> r = r0;
  return r += r1;
}

template<typename T>
MatrixRow<T> operator - (const MatrixRow<T> &r0,
                         const MatrixRow<T> &r1)
{
  MatrixRow<T> r = r0;
  return r -= r1;
}

template<typename T>
MatrixRow<T> operator*(const MatrixRow<T> &r0,
                       const MatrixRow<T> &r1)
{
  MatrixRow<T> result = r0;
  return result *= r1;
}

template<typename T>
MatrixRow<T> operator / (const MatrixRow<T> &r0,
                         const MatrixRow<T> &r1)
{
  MatrixRow<T> result = r0;
  return result /= r1;
}

template<typename T>
MatrixRow<T> operator * (const MatrixRow<T> &row,
                         T scalar)
{
  MatrixRow<T> r = row;
  return r *= scalar;
}

template<typename T>
MatrixRow<T> operator * (T scalar,
                         const MatrixRow<T> &row)
{
  MatrixRow<T> r = row;
  return r *= scalar;
}

template<typename T>
MatrixRow<T> operator / (const MatrixRow<T> &row,
                         T scalar)
{
  MatrixRow<T> r = row;
  return r /= scalar;
}


/* Operaciones unarias para columnas */

template<typename T>
MatrixCol<T> operator + (const MatrixCol<T> &col)
{
  return col;
}

template<typename T>
MatrixCol<T> operator - (const MatrixCol<T> &col)
{
  static_assert(std::is_signed<T>::value, "Requires signed type");

  MatrixCol<T> c = col;
  for (size_t i = 0; i < col.size(); i++) {
    c[i] = -col[i];
  }

  return c;
}

/* Operaciones binarias para columnas */

template<typename T>
MatrixCol<T> operator + (const MatrixCol<T> &c0,
                         const MatrixCol<T> &c1)
{
  MatrixCol<T> c = c0;
  return c += c1;
}

template<typename T>
MatrixCol<T> operator - (const MatrixCol<T> &c0,
                         const MatrixCol<T> &c1)
{
  MatrixCol<T> c = c0;
  return c -= c1;
}

template<typename T>
MatrixCol<T> operator*(const MatrixCol<T> &c0,
                       const MatrixCol<T> &c1)
{
  MatrixCol<T> result = c0;
  return result *= c1;
}

template<typename T>
MatrixCol<T> operator / (const MatrixCol<T> &c0,
                         const MatrixCol<T> &c1)
{
  MatrixCol<T> result = c0;
  return result /= c1;
}

template<typename T>
MatrixCol<T> operator * (const MatrixCol<T> &col,
                         T scalar)
{
  MatrixCol<T> c = col;
  return c *= scalar;
}

template<typename T>
MatrixCol<T> operator * (T scalar,
                         const MatrixCol<T> &col)
{
  MatrixCol<T> c = col;
  return c *= scalar;
}

template<typename T>
MatrixCol<T> operator / (const MatrixCol<T> &col,
                         T scalar)
{
  MatrixCol<T> c = col;
  return c /= scalar;
}



/* Matrix Block */

/// ¿Heredar de Matrix<T> para no duplicar las operaciones entre matrices??


template<typename T>
class MatrixBlock
{

public:

  using value_type = T;
  using size_type = size_t;
  using pointer = T *;
  using const_pointer = const T *;
  using reference = T &;
  using const_reference = const T &;

public:

  MatrixBlock(T *data,
              size_t rows, 
              size_t cols, 
              size_t iniRow, 
              size_t endRow,
              size_t iniCol,
              size_t endCol)
    : mData(data), 
      mRows(rows),
      mCols(cols),
      mIniRow(iniRow),
      mEndRow(endRow),
      mIniCol(iniCol),
      mEndCol(endCol)
  { }
  ~MatrixBlock(){ }

  MatrixBlock &operator = (const MatrixBlock &block)
  {
    // Mismo bloque....
    if (this != &block) {

      if (this->mData != &block.mData) {

      } else {

      }

    }

    return *this;
  }

  //const_reference operator[](size_t column) const
  //{
  //  return mData[mRow * mCols + column];
  //}

  //reference operator[](size_t column)
  //{
  //  return mData[mRow * mCols + column];
  //}
  
  reference operator()(size_t row, size_t col)
  {
    return mData[(mIniRow + row) * mCols + col + mIniCol];
  }

  const_reference operator()(size_t row, size_t col) const
  {
    return mData[(mIniRow + row) * mCols + col + mIniCol];
  }

  size_t rows()
  {
    return mEndRow - mIniRow + 1;
  }

  size_t cols()
  {
    return mEndCol - mIniCol + 1;
  }

private:

  T *mData;
  size_t mRows;
  size_t mCols;
  size_t mIniRow;
  size_t mEndRow;
  size_t mIniCol;
  size_t mEndCol;
};


/* Operaciones unarias para un bloque */

template<typename T>
MatrixBlock<T> operator + (const MatrixBlock<T> &block)
{
  return block;
}

template<typename T>
MatrixBlock<T> operator - (const MatrixBlock<T> &block)
{
  static_assert(std::is_signed<T>::value, "Requires signed type");

  MatrixBlock<T> b = block;
  for (size_t i = 0; i < block.width(); i++) {
    for (size_t j = 0; j < block.height(); j++) {
      b[i] = -block(i, j);
    }
  }

  return b;
}

/*! \} */ // end of algebra

/*! \} */ // end of math

} // Fin namespace math


} // End namespace tl

#endif // TL_MATH_MATRIX_H


