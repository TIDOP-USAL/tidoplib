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
   * \brief Allocator
   */
  typedef typename std::array<T, _size>::allocator_type allocator_type;

  /*!
   * \brief value_type
   */
  typedef typename std::array<T, _size>::value_type value_type;

  /*!
   * \brief Tipo entero sin signo (por lo general size_t)
   */
  typedef typename std::array<T, _size>::size_type size_type;

    /*!
   * \brief Tipo entero con signo (por lo general ptrdiff_t)
   */
  typedef typename std::array<T, _size>::difference_type difference_type;

  /*!
   * \brief std::allocator_traits<Allocator>::pointer
   */
  typedef typename std::array<T, _size>::pointer pointer;

  /*!
   * \brief std::allocator_traits<Allocator>::const_pointer 
   */
  typedef typename std::array<T, _size>::const_pointer const_pointer;

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
  
  /*!
   * \brief Constructora por defecto
   */
  Vector();

  /*!
   * \brief Constructor de copia
   * \param[in] vector Objeto que se copia
   */
  Vector(const Vector &vector);

  /*!
   * \brief Constructor de movimiento
   * \param[in] vector Objeto que se mueve
   */
  Vector(Vector &&vector) TL_NOEXCEPT;

  /*!
   * \brief Constructora inicializador de lista
   * \param[in] values Listado de valores del vector
   */
  Vector(std::initializer_list<T> values);

  /*!
   * \brief Destructora
   */
  ~Vector() = default;

  /*!
   * \brief Devuelve el tamaño del vector
   * \return Tamaño del vector
   */
  size_t size();

  /*!
   * \brief Devuelve un iterador al inicio del vector
   * \return Iterador al primer elemento del vector
   */
  virtual iterator begin();

  /*!
   * \brief Devuelve un iterador constante al inicio del vector
   * \return Iterador al primer elemento del vector
   */
  virtual const_iterator begin() const;

  /*!
   * \brief Devuelve un iterador al siguiente elemento después del final del vector
   * Este elemento actúa como un marcador de posición, intentar acceder a él resulta en un comportamiento no definido
   * \return Iterador al siguiente elemento después del final del vector
   */
  virtual iterator end();

  /*!
   * \brief Devuelve un iterador constante al siguiente elemento después del final del contenedor
   * Este elemento actúa como un marcador de posición, intentar acceder a él resulta en un comportamiento no definido 
   * \return Iterador al siguiente elemento después del final del contenedor
   */
  virtual const_iterator end() const;

  /*!
   * \brief Devuelve una referencia constante al elemento de la posición indicada
   * return Referencia constante al elemento
   */
  const_reference at(size_type position) const;

  /*!
   * \brief Devuelve una referencia al elemento de la posición indicada
   * return Referencia al elemento
   */
  reference at(size_type position);

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

  /*!
   * \brief Comprueba si el contenedor esta vacio
   * \return true si el contenedor está vacío y false en caso contrario
   */
  bool empty() const;

  /*!
   * \brief Asignación de copia
   */
  Vector &operator=(const Vector &entity);

  /*!
   * \brief Asignación de movimiento
   */
  Vector &operator=(Vector &&entity) TL_NOEXCEPT;

  //T dot() const;
  T lenght() const;
  T normalize() const;

  //inline bool operator==(Vector const& vec) const;
  //inline bool operator!=(Vector const& vec) const;
  //inline bool operator< (Vector const& vec) const;
  //inline bool operator<=(Vector const& vec) const;
  //inline bool operator> (Vector const& vec) const;
  //inline bool operator>=(Vector const& vec) const;

  /*!
   * \brief Construye un vector de ceros
   * \f[ V = [ 0 0 0 ] \f]
   * \return
   */
  static Vector zero();

  //static Vector unit();

private:

};


/* Implementación Matrix */

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
Vector<_size, T>::Vector(Vector &&vector) TL_NOEXCEPT
  : mVector(std::forward<EntityContainer<Entity_t>>(vector.mVector)),
    mSize(std::move(vector.mSize))
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
size_t Vector<_size, T>::size()
{
  return mSize;
}

template<size_t _size, typename T> inline
typename Vector<_size, T>::iterator Vector<_size, T>::begin()
{
  return mVector.begin();
}

template<size_t _size, typename T> inline 
typename Vector<_size, T>::const_iterator Vector<_size, T>::begin() const
{
  return mVector.cbegin();
}

template<size_t _size, typename T> inline
typename Vector<_size, T>::iterator Vector<_size, T>::end()
{
  return mVector.end();
}

template<size_t _size, typename T> inline
typename Vector<_size, T>::const_iterator Vector<_size, T>::end() const
{
  return mVector.cend();
}

template<size_t _size, typename T> inline
typename Vector<_size, T>::const_reference Vector<_size, T>::at(size_type position) const
{
  return mVector.at(position);
}

template<size_t _size, typename T> inline
typename Vector<_size, T>::reference Vector<_size, T>::at(size_type position)
{
  return mVector.at(position);
}

template<size_t _size, typename T> inline
typename Vector<_size, T>::const_reference Vector<_size, T>::operator[](size_t position) const
{
  return mVector[position];
}

template<size_t _size, typename T> inline
typename Vector<_size, T>::reference Vector<_size, T>::operator[](size_t position)
{
  return mVector[position];
}

//template<size_t _size, typename T> inline 
//T Vector<_size, T>::dot() const
//{
//  T dot = v0[0] * v1[0];
//  for (int i = 1; i < N; ++i)
//  {
//    dot += v0[i] * v1[i];
//  }
//  return dot;
//}

template<size_t _size, typename T> inline
bool Vector<_size, T>::empty() const
{
  return mVector.empty();
}

template<size_t _size, typename T> inline
Vector<_size, T> &Vector<_size, T>::operator=(const Vector<_size, T> &vector)
{
  if (this != &vector) {
    this->mVector = vector.mVector;
  }
  return (*this);
}

template<size_t _size, typename T> inline
Vector<_size, T> &Vector<_size, T>::operator=(const Vector<_size, T> &entity)
{
  if (this != &vector) {
    this->mVector = std::forward<EntityContainer<Entity_t>>(vector.mVector);
  }
  return (*this);
}

template<size_t _size, typename T> inline
T Vector<_size, T>::lenght() const
{
  return T();
}

template<size_t _size, typename T>
inline T Vector<_size, T>::normalize() const
{
  return T();
}

template<size_t _size, typename T> inline
Vector<_size, T> Vector<_size, T>::zero()
{
  Vector<_size, T> vector;
  for (size_t i = 0; i < _size; i++) {
    vector[i] = T{0};
  }
  return vector;
}

/* Operaciones unarias */

template<size_t _size, typename T>  static
Vector<_size, T> operator + (const Vector<_size, T> &vector)
{
  return matrix;
}

template<size_t _size, typename T> static
Vector<_size, T> operator - (const Vector<_size, T> &vector)
{
  Vector<_size, T> v;
  for (size_t i = 0; i < _size; i++) {
    v[i] = -vector[i];
  }
  return v;
}

/* Operaciones binarias */

template<size_t _size, typename T>
Vector<_size, T> operator + (const Vector<_size, T> &v0,
                             const Vector<_size, T> &v1)
{
  Vector<_size, T> v = v0;
  return v += v1;
}

template<size_t _size, typename T>
Vector<_size, T> &operator += (Vector<_size, T> &v0, 
                               const Vector<_size, T> &v1)
{
  for (size_t i = 0; i < _size; i++) {
    v0[i] += v1[i];
  }
  return v0;
}

template<size_t _size, typename T>
Vector<_size, T> operator - (const Vector<_size, T> &v0,
                             const Vector<_size, T> &v1)
{
  Vector<_size, T> v = v0;
  return v -= v1;
}

template<size_t _size, typename T>
Vector<_size, T> &operator -= (Vector<_size, T> &v0, 
                               const Vector<_size, T> &v1)
{
  for (size_t i = 0; i < _size; i++) {
    v0[i] -= v1[i];
  }
  return v0;
}

template<size_t _size, typename T>
Vector<_size, T> operator*(Vector<_size, T> const& v0,
                          Vector<_size, T> const& v1)
{
  Vector<_size, T> result = v0;
  return result *= v1;
}

template<size_t _size, typename T>
Vector<_size, T> &operator *= (Vector<_size, T> &v0, 
                               const Vector<_size, T> &v1)
{
  for (size_t i = 0; i < _size; i++) {
    v0[i] *= v1[i];
  }
  return v0;
}

template<size_t _size, typename T>
Vector<_size, T> operator / (const Vector<_size, T> &v0,
                             const Vector<_size, T> &v1)
{
  Vector<_size, T> result = v0;
  return result /= v1;
}

template<size_t _size, typename T>
Vector<_size, T> &operator /= (Vector<_size, T> &v0, 
                               const Vector<_size, T> &v1)
{
  for (size_t i = 0; i < _size; i++) {
    v0[i] /= v1[i];
  }
  return v0;
}

template<size_t _size, typename T>
Vector<_size, T> operator * (const Vector<_size, T> &vector, 
                             T scalar)
{
  Vector<_size, T> v = vector;
  return v *= scalar;
}

template<size_t _size, typename T>
Vector<_size, T> operator * (T scalar, 
                             const Vector<_size, T> &vector)
{
  Vector<_size, T> v = vector;
  return v *= scalar;
}

template<size_t _size, typename T>
Vector<_size, T> &operator *= (Vector<_size, T> &vector, 
                               T scalar)
{
  for (size_t i = 0; i < _size; i++) {
    vector[i] *= scalar;
  }
  return vector;
}

template<size_t _size, typename T>
Vector<_size, T> operator / (const Vector<_size, T> &vector, 
                             T scalar)
{
  Vector<_size, T> v = vector;
  return v /= scalar;
}

template<size_t _size, typename T>
Vector<_size, T> &operator /= (Vector<_size, T> &vector,
                               T scalar)
{
  if (scalar != static_cast<T>(0)) {
    for (size_t i = 0; i < _size; i++) {
      vector[i] /= scalar;
    }
  } else {
    for (size_t i = 0; i < _size; i++) {
      vector[i] = static_cast<T>(0);
    }
  }
  return vector;
}

/*! \} */ // end of Algebra

/*! \} */ // end of Math

} // Fin namespace math


} // End namespace tl

#endif // TL_MATH_VECTOR_H
