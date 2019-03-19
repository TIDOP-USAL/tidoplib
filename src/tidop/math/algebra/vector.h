/****************************************************************************
 *                                                                          *
 *  This file is part of TidopLib and can not be copied and/or distributed  *
 *  without the express permision of ITOS3D ENGINEERING S.L                 *
 *                                                                          *
 *  Contact: http://www.itos3d.com                                          *
 *           http://tidop.usal.es                                           *
 *                                                                          *
 *--------------------------------------------------------------------------*
 *                                                                          *
 *  Copyright (C) 2018, ITOS3D ENGINEERING S.L - All rights reserved        *
 *                                                                          *
 ****************************************************************************/

#ifndef TL_MATH_VECTOR_H
#define TL_MATH_VECTOR_H

#include "config_tl.h"

#include "tidop/core/defs.h"

#include <vector>
#include <array>

namespace tl
{

namespace math
{

/*! \addtogroup Math
 *  \{
 */


/*! \addtogroup Algebra
 *  \{
 */

template<size_t _size, typename T = double>
class Vector
{

public:

  /*!
   * \brief value_type&
   */
  typedef typename std::array<T, _size>::reference reference;

  /*!
   * \brief const value_type&
   */
  typedef typename std::array<T, _size>::const_reference const_reference;


protected:

  std::array<T, _size> mVector;
  size_t mSize;

public:
  
  Vector();
  Vector(const Vector &vector);
  Vector(std::initializer_list<T> values);
  ~Vector();

  size_t size();

  /*!
   * \brief Devuelve una referencia al elemento de la posición indicada
   * No se comprueba si el elemento al que se quiere acceder esta dentro de los limites
   * return Referencia constante al elemento
   */
  const_reference operator[](size_t position) const;

  /*!
   * \brief Devuelve una referencia al elemento de la posición indicada
   * No se comprueba si el elemento al que se quiere acceder esta dentro de los limites
   * return Referencia al elemento
   */
  reference operator[](size_t position);

private:

};

template<size_t _size, typename T> inline 
Vector<_size, T>::Vector()
  : mSize(_size)
{
  T ini_val = -std::numeric_limits<T>().max();
  for (size_t i = 0; i < mSize; i++) {
    this->mVector[i] = ini_val;
  }
}

template<size_t _size, typename T> inline
Vector<_size, T>::Vector(const Vector &vector)
  : mVector(vector.mVector),
    mSize(vector.mSize)
{
}

template<size_t _size, typename T> inline
Vector<_size, T>::Vector(std::initializer_list<T> vector)
  : mSize(_size)
{
  size_t n = vector.size();
  if (n == _size){
    std::copy(vector.begin(), vector.end(), mVector.begin());
  } else if (n > _size){
    std::copy(vector.begin(), vector.end(), mVector.begin());
    std::fill(mVector.begin() + n, mVector.end(), T{0});
  } else {
    std::copy(vector.begin(), vector.begin() + n, mVector.begin());
  }
}


template<size_t _size, typename T> inline 
Vector<_size, T>::~Vector()
{
}

template<size_t _size, typename T> inline 
size_t Vector<_size, T>::size()
{
  return mSize;
}

template<size_t _size, typename T> inline
typename Vector<_size, T>::const_reference Vector<_size, T>::operator[](size_t position) const
{
  mVector.at(pos);
  return mVector[position];
}

template<size_t _size, typename T> inline
typename Vector<_size, T>::reference Vector<_size, T>::operator[](size_t position)
{
  return mVector[position];
}


/*! \} */ // end of Algebra

/*! \} */ // end of Math

} // Fin namespace math


} // End namespace tl

#endif // TL_MATH_VECTOR_H
