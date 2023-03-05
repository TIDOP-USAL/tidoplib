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
template<typename T, size_t _size>
class Data
{

public:

  using value_type = T;
  using size_type = size_t;
  using pointer = T *;
  using const_pointer = const T *;
  using reference = T &;
  using const_reference = const T &;
  
  using iterator = typename std::array<T, _size>::iterator;
  using const_iterator = typename std::array<T, _size>::const_iterator;

public:

  Data();
  Data(size_t size);
  Data(size_t size, T value);
  Data(const Data &matrix);
  Data(Data &&matrix) TL_NOEXCEPT;
  Data(std::initializer_list<T> values);
  Data(const T *data, size_t size = 0);
  virtual ~Data() = default;

  Data &operator = (const Data &matrix);
  Data &operator = (Data &&matrix) TL_NOEXCEPT;

  reference at(size_t position);
  const_reference at(size_t position) const;
  reference operator[](size_t position);
  const_reference operator[](size_t position) const;

  void operator=(T value);

  reference front()
  {
    return mData.front();
  }

  const_reference front() const
  {
    return mData.front();
  }

  reference back()
  {
    return mData.back();
  }

  const_reference back() const
  {
    return mData.back();
  }

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

  size_t size() const { return mData.size(); }

private:

  std::array<T, _size> mData;
};



/*------------------------------------------------------------------------*/


/*!
 * \brief Dynamic data storage
 */
template<typename T>
class Data<T, DynamicDataStorage>
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
  Data(size_t size);
  Data(size_t size, T val);
  Data(const Data &matrix);
  Data(Data &&matrix) TL_NOEXCEPT;
  Data(std::initializer_list<T> values);
  Data(const T *data, size_t size);
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
   */
  void operator=(T value)
  {
    std::fill(mData.begin(), mData.end(), value);
  }

  reference front()
  {
    return mData.front();
  }

  const_reference front() const
  {
    return mData.front();
  }

  reference back()
  {
    return mData.back();
  }

  const_reference back() const
  {
    return mData.back();
  }

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

  size_t size() const { return mData.size(); }

private:

  std::vector<T> mData;

};



/*------------------------------------------------------------------------*/
/* Implementation                                                         */
/*------------------------------------------------------------------------*/


/* Static Data implementation */

template<typename T, size_t _size> inline
Data<T, _size>::Data()
  : mData()
{
  this->mData.fill(-std::numeric_limits<T>().max());
}

template<typename T, size_t _size> inline
Data<T, _size>::Data(size_t  /*size*/)
  : mData()
{
  this->mData.fill(-std::numeric_limits<T>().max());
}

template<typename T, size_t _size> inline
Data<T, _size>::Data(size_t  /*size*/, T val)
  : mData()
{
  this->mData.fill(val);
}

template<typename T, size_t _size> inline
Data<T, _size>::Data(const Data &matrix)
  : mData(std::move(matrix.mData))
{
}

// ¿El de movimiento???

template<typename T, size_t _size> inline
Data<T, _size>::Data(std::initializer_list<T> values)
{
  size_t n = values.size();
  if (n == _size) {
    std::copy(values.begin(), values.end(), mData.begin());
  } else if (n < _size) {
    std::copy(values.begin(), values.end(), mData.begin());
    std::fill(mData.begin() + n, mData.end(), consts::zero<T>);
  } else {
    std::copy(values.begin(), values.begin() + _size, mData.begin());
  }
}

template<typename T, size_t _size>
inline Data<T, _size>::Data(const T *data,
                            size_t /*size*/)
  : mData(data)
{
}

template<typename T, size_t _size> inline
Data<T, _size>::Data(Data &&matrix) TL_NOEXCEPT
  : mData(std::move(matrix.mData))
{
}

template<typename T, size_t _size> inline
Data<T, _size> &Data<T, _size>::operator = (const Data &matrix)
{
  if (this != &matrix) {
    this->mData = matrix.mData;
  }
  return *this;
}

template<typename T, size_t _size> inline
Data<T, _size> &Data<T, _size>::operator = (Data &&matrix) TL_NOEXCEPT
{
  if (this != &matrix) {
    this->mData = std::move(matrix.mData);
  }
  return *this;
}

template<typename T, size_t _size> inline
T &Data<T, _size>::at(size_t position)
{
  return mData.at(position);
}

template<typename T, size_t _size> inline
const T &Data<T, _size>::at(size_t position) const
{
  return mData.at(position);
}

template<typename T, size_t _size> inline
T &Data<T, _size>::operator[](size_t position)
{
  return mData[position];
}

template<typename T, size_t _size> inline
const T &Data<T, _size>::operator[](size_t position) const
{
  return mData[position];
}

template<typename T, size_t _size> inline
void Data<T, _size>::operator=(T value)
{
  mData.fill(value);
}

template<typename T, size_t _size> inline
T *Data<T, _size>::data()
{
  return mData.data();
}

template<typename T, size_t _size> inline
const T *Data<T, _size>::data() const
{
  return mData.data();
}



/* Dynamic Data implementation */

template<typename T> inline
Data<T, DynamicDataStorage>::Data(size_t size)
  : mData(size, -std::numeric_limits<T>().max())
{
}

template<typename T> inline
Data<T, DynamicDataStorage>::Data(size_t size, T val)
  : mData(size, val)
{
}

template<typename T> inline
Data<T, DynamicDataStorage>::Data(const Data &matrix)
  : mData(matrix.mData)
{
}

template<typename T> inline
Data<T, DynamicDataStorage>::Data(Data &&matrix) TL_NOEXCEPT
  : mData(std::move(matrix.mData))
{
}

template<typename T> inline
Data<T, DynamicDataStorage>::Data(std::initializer_list<T> values)
{
  mData.resize(values.size());
  std::copy(values.begin(), values.end(), mData.begin());
}

template<typename T> inline
Data<T, DynamicDataStorage>::Data(const T *data, size_t size)
  : mData(size, -std::numeric_limits<T>().max())
{
  mData.assign(data, data + mData.size());
}

template<typename T> inline
Data<T, DynamicDataStorage>
&Data<T, DynamicDataStorage>::operator = (const Data<T, DynamicDataStorage> &matrix)
{
  if (this != &matrix) {
    this->mData = matrix.mData;
  }
  return *this;
}

template<typename T> inline
Data<T, DynamicDataStorage>
&Data<T, DynamicDataStorage>::operator = (Data<T, DynamicDataStorage> &&matrix) TL_NOEXCEPT
{
  if (this != &matrix) {
    this->mData = std::move(matrix.mData);
  }
  return *this;
}

template<typename T> inline
T &Data<T, DynamicDataStorage>::at(size_t position)
{
  return mData.at(position);
}

template<typename T> inline
const T &Data<T, DynamicDataStorage>::at(size_t position) const
{
  return mData.at(position);
}

template<typename T> inline
T &Data<T, DynamicDataStorage>::operator[](size_t position)
{
  return mData[position];
}

template<typename T> inline
const T &Data<T, DynamicDataStorage>::operator[](size_t position) const
{
  return mData[position];
}

template<typename T> inline
T *Data<T, DynamicDataStorage>::data()
{
  return mData.data();
}

template<typename T> inline
const T *Data<T, DynamicDataStorage>::data() const
{
  return mData.data();
}


/*! \} */ // end of math

} // Fin namespace math

} // End namespace tl

#endif // TL_MATH_DATA_STORAGE_H