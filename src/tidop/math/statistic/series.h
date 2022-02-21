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

#ifndef TL_MATH_STATISTIC_SERIES_H
#define TL_MATH_STATISTIC_SERIES_H

#include <vector> 
#include <string> 
#include <tuple>
#include <iomanip>

#include <tidop/core/defs.h>
#include <tidop/core/messages.h>

namespace tl
{

namespace math
{
	
/*! \addtogroup math
 *  \{
 */


/*! \defgroup statistics Statistics
 *  \{
 */
 
template<typename T>
class Series
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
  using reverse_iterator = typename std::vector<T>::reverse_iterator;
  using const_reverse_iterator = typename std::vector<T>::const_reverse_iterator;

public:

  Series() = default;

  /*!
   * \brief Series
   * \param data
   */
  Series(std::initializer_list<T> data);

  /*!
   * \brief Series
   * \param data
   */
  Series(std::initializer_list<std::pair<std::string, T>> data);

  /*!
   * \brief Series
   * \param data
   */
  Series(std::initializer_list<std::pair<size_t, T>> data);

  ~Series();

  TL_TODO("Añadir copia y asignación")

  size_t size() const;

  T operator[](size_t idx);
  T operator[](const std::string &idx);

  //void setData(std::initializer_list<T> data);
  //void setData(std::initializer_list<std::pair<std::string, T>> data);
  //void setData(std::initializer_list<std::pair<size_t, T>> data);

  reference front();
  const_reference front() const;
  reference back();
  const_reference back() const;
  iterator begin() TL_NOEXCEPT;
  const_iterator begin() const TL_NOEXCEPT;
  iterator end() TL_NOEXCEPT;
  const_iterator end() const TL_NOEXCEPT;
  reverse_iterator rbegin() TL_NOEXCEPT;
  const_reverse_iterator rbegin() const TL_NOEXCEPT;
  reverse_iterator rend() TL_NOEXCEPT;
  const_reverse_iterator rend() const TL_NOEXCEPT;

private:

private:

#ifdef DATA_USE_VAL_ARRAY
  std::valarray<size_t> mIndex;
  std::valarray<T> mData;
#else
  std::vector<size_t> mIndex;
  std::vector <std::string> mStringIndex;
  std::vector<T> mData;
#endif

};


template<typename T> inline
Series<T>::Series(std::initializer_list<T> data)
  : mData(data)
{
}

template<typename T> inline 
Series<T>::Series(std::initializer_list<std::pair<std::string, T>> data)
{
  TL_TODO("En c++ 17 se puede utilizar tuple")

  size_t n = data.size();
  mStringIndex.reserve(n);
  mData.reserve(n);
  for (auto it = data.begin(); it != data.end(); it++) {
    mStringIndex.push_back(it->first);
    mData.push_back(it->second);
  }
}

template<typename T> inline
Series<T>::Series(std::initializer_list<std::pair<size_t, T>> data)
{
  TL_TODO("En c++ 17 se puede utilizar tuple")

  size_t n = data.size();
  mIndex.reserve(n);
  mData.reserve(n);
  for (auto it = data.begin(); it != data.end(); it++) {
    mIndex.push_back(it->first);
    mData.push_back(it->second);
  }
}

template<typename T> inline
Series<T>::~Series()
{
}

template<typename T> inline
size_t Series<T>::size() const
{
  return mData.size();
}

template<typename T> 
T Series<T>::operator[](size_t idx)
{
  T value{};

  if (mIndex.empty()) {
    value = mData[idx];
  }
  else {

    for (size_t i = 0; i < mIndex.size(); i++) {
      if (idx == mIndex[i]) {
        value = mData[i];
        break;
      }
    }
    TL_TODO("Devolver excepción si no se encuentra el indice??")
  }

  return value;
}

template<typename T>
T Series<T>::operator[](const std::string &idx)
{
  T value{};

  if (mStringIndex.empty()) {

    try {

      size_t integer_idx = std::stoull(idx);
      if (mIndex.empty()) {
        value = mData[integer_idx];
      }
      else {
        for (size_t i = 0; i < mIndex.size(); i++) {
          if (integer_idx == mIndex[i]) {
            value = mData[i];
            break;
          }
        }
      }

    } catch (const std::invalid_argument &e) {
      msgError("Invalid index", e.what());
    }

  }
  else {

    for (size_t i = 0; i < mStringIndex.size(); i++) {
      if (mStringIndex[i] == idx) {
        value = mData[i];
        break;
      }
    }
    TL_TODO("Devolver excepción si no se encuentra el indice??")
  }

  return value;
}

//template<typename T> inline
//void Series<T>::setData(std::initializer_list<T> data)
//{
//  mData = data;
//}
//
//template<typename T> inline
//void Series<T>::setData(std::initializer_list<std::pair<std::string, T>> data)
//{ 
//  size_t n = data.size();
//  mStringIndex.reserve(n);
//  mData.reserve(n);
//  for (auto it = data.begin(); it != data.end(); it++) {
//    mStringIndex.push_back(it->first);
//    mData.push_back(it->second);
//  }
//}
//
//template<typename T> inline
//void Series<T>::setData(std::initializer_list<std::pair<size_t, T>> data)
//{
//  size_t n = data.size();
//  mIndex.reserve(n);
//  mData.reserve(n);
//  for (auto it = data.begin(); it != data.end(); it++) {
//    mIndex.push_back(it->first);
//    mData.push_back(it->second);
//  }
//}

template<typename T> inline
typename Series<T>::reference Series<T>::front()
{
  return mData.front();
}

template<typename T> inline
typename Series<T>::const_reference Series<T>::front() const
{
  return mData.front();
}

template<typename T> inline
typename Series<T>::reference Series<T>::back()
{
  return mData.back();
}

template<typename T> inline
typename Series<T>::const_reference Series<T>::back() const
{
  return mData.back();
}

template<typename T> inline
typename Series<T>::iterator Series<T>::begin() TL_NOEXCEPT
{
  return mData.begin();
}

template<typename T> inline
typename Series<T>::const_iterator Series<T>::begin() const TL_NOEXCEPT
{
  return mData.begin();
}

template<typename T> inline
typename Series<T>::iterator Series<T>::end() TL_NOEXCEPT
{
  return mData.end();
}

template<typename T> inline
typename Series<T>::const_iterator Series<T>::end() const TL_NOEXCEPT
{
  return mData.end();
}

template<typename T> inline
typename Series<T>::reverse_iterator Series<T>::rbegin() TL_NOEXCEPT
{
  return mData.rbegin();
}

template<typename T> inline
typename Series<T>::const_reverse_iterator Series<T>::rbegin() const TL_NOEXCEPT
{
  return mData.rbegin();
}

template<typename T> inline
typename Series<T>::reverse_iterator Series<T>::rend() TL_NOEXCEPT
{
  return mData.rend();
}

template<typename T> inline
typename Series<T>::const_reverse_iterator Series<T>::rend() const TL_NOEXCEPT
{
  return mData.rend();
}


/* Print */

template<typename T>
std::ostream &operator<< (std::ostream &os, const Series<T> &serie)
{
  os << std::flush;
  return os;
}

/*! \} */ // end of statistic

/*! \} */ // end of math

} // End namespace math

} // End namespace tl

#endif TL_MATH_STATISTIC_SERIES_H


