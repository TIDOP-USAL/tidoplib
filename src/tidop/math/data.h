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

#ifndef TL_MATH_DATA_STORAGE_H
#define TL_MATH_DATA_STORAGE_H

#include "config_tl.h"

#include <vector>
#include <array>


namespace tl
{

namespace math
{

constexpr auto DynamicDataStorage = std::numeric_limits<size_t>::max();

/*! \addtogroup math
 *  \{
 */
 
template<typename T, size_t _rows, size_t _cols = 1>
class DataSize
{

public:

  static constexpr size_t size() 
  {
    return _rows == DynamicDataStorage ? DynamicDataStorage : _rows * _cols;
  }

};


/*!
 * \brief Static data storage
 */
template<typename T, size_t _size/*_rows, size_t _cols*/>
class Data
{

public:

  using value_type = T;
  using size_type = size_t;
  using pointer = T *;
  using const_pointer = const T *;
  using reference = T &;
  using const_reference = const T &;
  
  using iterator = typename std::array<T, _size/*_rows *_cols*/>::iterator;
  using const_iterator = typename std::array<T, _size/*_rows *_cols*/>::const_iterator;

public:

  Data();
  Data(size_t size/*rows, size_t cols*/);
  Data(size_t size/*rows, size_t cols*/, T value);
  Data(const Data &matrix);
  Data(Data &&matrix) TL_NOEXCEPT;
  //Data(std::initializer_list<T> values);
  //Data(std::initializer_list<std::initializer_list<T>> values);
  Data(T *data, size_t size = 0/*rows = 0, size_t cols = 0*/);
  virtual ~Data() = default;

  Data &operator = (const Data &matrix);
  Data &operator = (Data &&matrix) TL_NOEXCEPT;

  reference at(size_t position);
  const_reference at(size_t position) const;
  reference operator[](size_t position);
  const_reference operator[](size_t position) const;

  /*!
   * \brief Asigna el mismo valor a toda la matriz
   * \param[in] value Valor escalar que se asigna a todos los elementos de la matriz
   *
   * <h4>Ejemplo</h4>
   * \code
   * Matrix<double,3,3> matrix;
   * matrix = 1.5;
   * \endcode
   */
  void operator=(T value);

  /*!
   * \brief Número de filas de la matriz
   * \return Número de filas
   */
  //size_t rows() const;

  /*!
   * \brief Número de columnas de la matriz
   * \return Número de columnas
   */
  //size_t cols() const;

  /*!
   * \brief Intercambia dos filas
   * \param[in] i Primera fila a intercambiar
   * \param[in] j Segunda fila a intercambiar
   */
  //void swapRows(size_t i, size_t j);

  iterator begin() TL_NOEXCEPT
  {
    return mData.begin();
  }
  
  const_iterator begin() const TL_NOEXCEPT
  {
    return mData.begin();
  }

  iterator end() TL_NOEXCEPT
  {
    return mData.end();
  }

  const_iterator end() const TL_NOEXCEPT
  {
    return mData.end();
  }

  pointer data();
  const_pointer data() const;

private:

  std::array<T, _size/*_rows *_cols*/> mData;
};



/*------------------------------------------------------------------------*/


/*!
 * \brief Dynamic data storage
 */
template<typename T>
class Data<T, DynamicDataStorage/*, DynamicDataStorage*/>
{

public:

  using value_type = T;
  using size_type = size_t;
  using pointer = T *;
  using const_pointer = const T *;
  using reference = T &;
  using const_reference = const T &;

  using iterator = typename std::vector<T>::iterator;
  using const_iterator = typename std::vector<T>::const_iterator;

public:

  Data() = default;
  Data(size_t size/*rows, size_t cols*/);
  Data(size_t size/*rows, size_t cols*/, T val);
  Data(const Data &matrix);
  Data(Data &&matrix) TL_NOEXCEPT;
  //Data(std::initializer_list<T> values);
  Data(const T *data, size_t size/*rows, size_t cols*/);
  virtual ~Data() = default;

  Data &operator = (const Data &matrix);
  Data &operator = (Data &&matrix) TL_NOEXCEPT;

  reference at(size_t position);
  const_reference at(size_t position) const;
  reference operator[](size_t position);
  const_reference operator[](size_t position) const;

  /*!
   * \brief Asigna el mismo valor a toda el conjunto de datos
   * \param[in] value Valor escalar que se asigna
   *
   * <h4>Ejemplo</h4>
   * \code
   * Matrix<double,3,3> matrix;
   * matrix = 1.5;
   * \endcode
   */
  void operator=(T value)
  {
    std::fill(mData.begin(), mData.end(), value);
  }

  /*!
   * \brief Número de filas de la matriz
   * \return Número de filas
   */
  //size_t rows() const;

  /*!
   * \brief Número de columnas de la matriz
   * \return Número de columnas
   */
  //size_t cols() const;

  /*!
   * \brief Intercambia dos filas
   * \param[in] i Primera fila a intercambiar
   * \param[in] j Segunda fila a intercambiar
   */
  //void swapRows(size_t i, size_t j);

  iterator begin() TL_NOEXCEPT
  {
    return mData.begin();
  }

  const_iterator begin() const TL_NOEXCEPT
  {
    return mData.begin();
  }

  iterator end() TL_NOEXCEPT
  {
    return mData.end();
  }

  const_iterator end() const TL_NOEXCEPT
  {
    return mData.end();
  }

  pointer data();
  const_pointer data() const;

private:

  std::vector<T> mData;
  //size_t mRows{0};
  //size_t mCols{0};
};





/*------------------------------------------------------------------------*/
/* Implementation                                                         */
/*------------------------------------------------------------------------*/


/* Static Data implementation */

template<typename T, size_t _size/*_rows, size_t _cols*/> inline
Data<T, _size/*_rows, _cols*/>::Data()
  : mData()
{
  this->mData.fill(-std::numeric_limits<T>().max());
}

template<typename T, size_t _size/*_rows, size_t _cols*/> inline
Data<T, _size/*_rows, _cols*/>::Data(size_t  /*rows*//*, size_t*/  /*cols*/)
  : mData()
{
  this->mData.fill(-std::numeric_limits<T>().max());
}

template<typename T, size_t _size/*_rows, size_t _cols*/> inline
Data<T, _size/*_rows, _cols*/>::Data(size_t  /*rows*//*, size_t*/  /*cols*/, T val)
  : mData()
{
  this->mData.fill(val);
}

template<typename T, size_t _size/*_rows, size_t _cols*/> inline
Data<T, _size/*_rows, _cols*/>::Data(const Data &matrix)
  : mData(std::move(matrix.mData))
{
}

// ¿El de movimiento???

//template<typename T, size_t _size/*_rows, size_t _cols*/> inline
//Data<T, _size/*_rows, _cols*/>::Data(std::initializer_list<T> values)
//{
//  size_t n = values.size();
//  if (n == _rows * _cols) {
//    std::copy(values.begin(), values.end(), mData.begin());
//  } else if (n < _rows * _cols) {
//    std::copy(values.begin(), values.end(), mData.begin());
//    std::fill(mData.begin() + n, mData.end(), consts::zero<T>);
//  } else {
//    std::copy(values.begin(), values.begin() + _rows * _cols, mData.begin());
//  }
//}

//template<typename T, size_t _rows, size_t _cols> inline
//Data<T, _rows, _cols>::Data(std::initializer_list<std::initializer_list<T>> values)
//{
//  auto it_data = mData.begin();
//  size_t rows_counter = 0;
//  for (auto it = values.begin(); it != values.end(); it++) {
//    if (rows_counter < _rows) {
//      size_t n = it->size();
//      if (n == _cols) {
//        std::copy(it->begin(), it->end(), it_data);
//      } else if (n < _cols) {
//        std::copy(it->begin(), it->end(), it_data);
//        std::fill(it_data + n, mData.end(), consts::zero<T>);
//      } else {
//        std::copy(it->begin(), it->end() + _cols, it_data);
//      }
//      it_data += _cols;
//      rows_counter++;
//    }
//  }
//
//  if (it_data != mData.end()) {
//    std::fill(it_data, mData.end(), consts::zero<T>);
//  }
//}

template<typename T, size_t _size/*_rows, size_t _cols*/>
inline Data<T, _size/*_rows, _cols*/>::Data(T *data,
                                   size_t  /*rows*//*,
                                   size_t*/  /*cols*/)
  : mData(data)
{
}

template<typename T, size_t _size/*_rows, size_t _cols*/> inline
Data<T, _size/*_rows, _cols*/>::Data(Data &&matrix) TL_NOEXCEPT
  : mData(std::move(matrix.mData))
{
}

template<typename T, size_t _size/*_rows, size_t _cols*/> inline
Data<T, _size/*_rows, _cols*/> &Data<T, _size/*_rows, _cols*/>::operator = (const Data &matrix)
{
  if (this != &matrix) {
    this->mData = matrix.mData;
  }
  return *this;
}

template<typename T, size_t _size/*_rows, size_t _cols*/> inline
Data<T, _size/*_rows, _cols*/> &Data<T, _size/*_rows, _cols*/>::operator = (Data &&matrix) TL_NOEXCEPT
{
  if (this != &matrix) {
    this->mData = std::move(matrix.mData);
  }
  return *this;
}

template<typename T, size_t _size/*_rows, size_t _cols*/> inline
T &Data<T, _size/*_rows, _cols*/>::at(size_t position)
{
  return mData.at(position);
}

template<typename T, size_t _size/*_rows, size_t _cols*/> inline
const T &Data<T, _size/*_rows, _cols*/>::at(size_t position) const
{
  return mData.at(position);
}
 
//template<typename T, size_t _rows, size_t _cols> inline
//T &Data<T, _rows, _cols>::at(size_t r, size_t c)
//{
//  return mData.at(r * _cols + c);
//}
//
//template<typename T, size_t _rows, size_t _cols> inline
//const T &Data<T, _rows, _cols>::at(size_t r, size_t c) const
//{
//  return mData.at(r * _cols + c);
//}

//template<typename T, size_t _rows, size_t _cols> inline
//T &Data<T, _rows, _cols>::operator()(size_t r, size_t c)
//{
//  return mData[r * _cols + c];
//}
//
//template<typename T, size_t _rows, size_t _cols> inline
//const T &Data<T, _rows, _cols>::operator()(size_t r, size_t c) const
//{
//  return mData[r * _cols + c];
//}

template<typename T, size_t _size/*_rows, size_t _cols*/> inline
T &Data<T, _size/*_rows, _cols*/>::operator[](size_t position)
{
  return mData[position];
}

template<typename T, size_t _size/*_rows, size_t _cols*/> inline
const T &Data<T, _size/*_rows, _cols*/>::operator[](size_t position) const
{
  return mData[position];
}

template<typename T, size_t _size/*_rows, size_t _cols*/> inline
void Data<T, _size/* _rows, _cols*/>::operator=(T value)
{
  mData.fill(value);
}

//template<typename T, size_t _rows, size_t _cols> inline
//size_t Data<T, _rows, _cols>::rows() const
//{
//  return _rows;
//}
//
//template<typename T, size_t _rows, size_t _cols> inline
//size_t Data<T, _rows, _cols>::cols() const
//{
//  return _cols;
//}

//template<typename T, size_t _rows, size_t _cols> inline
//void Data<T, _rows, _cols>::swapRows(size_t i, size_t j)
//{
//  for (size_t c = 0; c < _cols; c++) {
//    std::swap(mData[i * _cols + c], mData[j * _cols + c]);
//  }
//}

template<typename T, size_t _size/*_rows, size_t _cols*/> inline
T *Data<T, _size/*_rows, _cols*/>::data()
{
  return mData.data();
}

template<typename T, size_t _size/*_rows, size_t _cols*/> inline
const T *Data<T, _size/*_rows, _cols*/>::data() const
{
  return mData.data();
}



/* Dynamic Data implementation */

template<typename T> inline
Data<T, DynamicDataStorage/*, DynamicDataStorage*/>::Data(size_t size/*rows,
                                                      size_t cols*/)
  : mData(size/*rows *cols*/, -std::numeric_limits<T>().max())/*,
    mRows(rows),
    mCols(cols)*/
{
}

template<typename T> inline
Data<T, DynamicDataStorage/*, DynamicDataStorage*/>::Data(size_t size/*rows,
                                                      size_t cols*/,
                                                      T val)
  : mData(size/*rows *cols*/, val)/*,
    mRows(rows),
    mCols(cols)*/
{
}

template<typename T> inline
Data<T, DynamicDataStorage/*, DynamicDataStorage*/>::Data(const Data &matrix)
  : mData(matrix.mData)/*,
    mRows(matrix.mRows),
    mCols(matrix.mCols)*/
{
}

template<typename T> inline
Data<T, DynamicDataStorage/*, DynamicDataStorage*/>::Data(Data &&matrix) TL_NOEXCEPT
  : mData(std::move(matrix.mData))/*,
    mRows(matrix.mRows),
    mCols(matrix.mCols)*/
{
}

//template<typename T> inline
//Data<T, DynamicDataStorage/*, DynamicDataStorage*/>::Data(std::initializer_list<T> values)
//{
//  this->mRows = 1;
//  this->mCols = values.size();
//
//  std::copy(values.begin(), values.end(), mData.begin());
//}

//template<typename T> inline
//Data<T, DynamicDataStorage, DynamicDataStorage>::Data(std::initializer_list<std::initializer_list<T>> values)
//{
//  this->mRows = values.size();
//  auto it = values.begin();
//  this->mCols = it->size();
//  mData.resize(this->mRows * this->mCols);
//
//  auto it_data = mData.begin();
//  for (auto it = values.begin(); it != values.end(); it++) {
//    size_t n = it->size();
//    if (n == mCols) {
//      std::copy(it->begin(), it->end(), it_data);
//    } else if (n < mCols) {
//      std::copy(it->begin(), it->end(), it_data);
//      std::fill(it_data + n, mData.end(), consts::zero<T>);
//    } else {
//      std::copy(it->begin(), it->end() + mCols, it_data);
//    }
//    it_data += mCols;
//  }
//}

template<typename T> inline
Data<T, DynamicDataStorage/*, DynamicDataStorage*/>::Data(const T *data,
                                                      size_t size/*rows,
                                                      size_t cols*/)
  : mData(size/*rows *cols*/, -std::numeric_limits<T>().max())/*,
  mRows(rows),
  mCols(cols)*/
{
  mData.assign(data, data + mData.size());
}

template<typename T> inline
Data<T, DynamicDataStorage/*, DynamicDataStorage*/>
&Data<T, DynamicDataStorage/*, DynamicDataStorage*/>::operator = (const Data<T, DynamicDataStorage/*, DynamicDataStorage*/> &matrix)
{
  if (this != &matrix) {
    this->mData = matrix.mData;
    //this->mRows = matrix.mRows;
    //this->mCols = matrix.mCols;
  }
  return *this;
}

template<typename T> inline
Data<T, DynamicDataStorage/*, DynamicDataStorage*/>
&Data<T, DynamicDataStorage/*, DynamicDataStorage*/>::operator = (Data<T, DynamicDataStorage/*, DynamicDataStorage*/> &&matrix) TL_NOEXCEPT
{
  if (this != &matrix) {
    this->mData = std::move(matrix.mData);
    //this->mRows = matrix.mRows;
    //this->mCols = matrix.mCols;
  }
  return *this;
}

//template<typename T> inline
//T &Data<T, DynamicDataStorage, DynamicDataStorage>::at(size_t r,
//                                                       size_t c)
//{
//  return mData.at(r * mCols + c);
//}
//
//template<typename T> inline
//const T &Data<T, DynamicDataStorage, DynamicDataStorage>::at(size_t r,
//                                                             size_t c) const
//{
//  return mData.at(r * mCols + c);
//}

template<typename T> inline
T &Data<T, DynamicDataStorage/*, DynamicDataStorage*/>::at(size_t position)
{
  return mData.at(position);
}

template<typename T> inline
const T &Data<T, DynamicDataStorage/*, DynamicDataStorage*/>::at(size_t position) const
{
  return mData.at(position);
}

//template<typename T> inline
//T &Data<T, DynamicDataStorage, DynamicDataStorage>::operator()(size_t r,
//                                                               size_t c)
//{
//  return mData[r * mCols + c];
//}
//
//template<typename T> inline
//const T &Data<T, DynamicDataStorage, DynamicDataStorage>::operator()(size_t r,
//                                                                     size_t c) const
//{
//  return mData[r * mCols + c];
//}

template<typename T> inline
T &Data<T, DynamicDataStorage/*, DynamicDataStorage*/>::operator[](size_t position)
{
  return mData[position];
}

template<typename T> inline
const T &Data<T, DynamicDataStorage/*, DynamicDataStorage*/>::operator[](size_t position) const
{
  return mData[position];
}

//template<typename T> inline
//size_t Data<T, DynamicDataStorage, DynamicDataStorage>::rows() const
//{
//  return mRows;
//}
//
//template<typename T> inline
//size_t Data<T, DynamicDataStorage, DynamicDataStorage>::cols() const
//{
//  return mCols;
//}

//template<typename T> inline
//void Data<T, DynamicDataStorage, DynamicDataStorage>::swapRows(size_t i,
//                                                               size_t j)
//{
//  for (size_t c = 0; c < mCols; c++) {
//    std::swap(mData[i * mCols + c], mData[j * mCols + c]);
//  }
//}

template<typename T> inline
T *Data<T, DynamicDataStorage/*, DynamicDataStorage*/>::data()
{
  return mData.data();
}

template<typename T> inline
const T *Data<T, DynamicDataStorage/*, DynamicDataStorage*/>::data() const
{
  return mData.data();
}


/*! \} */ // end of math

} // Fin namespace math

} // End namespace tl

#endif // TL_MATH_DATA_STORAGE_H