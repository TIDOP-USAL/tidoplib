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

#include <iterator>
#include <vector>
#include <array>
#include <valarray>

#include "tidop/core/exception.h"

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
   * \brief value_type
   */
  typedef T value_type;

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

  /*!
   * \brief Iterador de acceso aleatorio
   */
  typedef typename std::array<T, _size>::iterator iterator;

  /*!
   * \brief Iterador constante de acceso aleatorio
   */
  typedef typename std::array<T, _size>::const_iterator const_iterator;

  /*!
   * \brief Iterador de acceso aleatorio
   */
  typedef typename std::array<T, _size>::reverse_iterator reverse_iterator;

  /*!
   * \brief Iterador constante de acceso aleatorio
   */
  typedef typename std::array<T, _size>::const_reverse_iterator const_reverse_iterator;

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
  size_t size() const TL_NOEXCEPT;

  reference front();
  const_reference front() const;
  reference back();
  const_reference back() const;

  /*!
   * \brief Devuelve un iterador al inicio del vector
   * \return Iterador al primer elemento del vector
   */
  iterator begin() TL_NOEXCEPT;

  /*!
   * \brief Devuelve un iterador constante al inicio del vector
   * \return Iterador al primer elemento del vector
   */
  const_iterator begin() const TL_NOEXCEPT;

  /*!
   * \brief Devuelve un iterador al siguiente elemento después del final del vector
   * Este elemento actúa como un marcador de posición, intentar acceder a él resulta en un comportamiento no definido
   * \return Iterador al siguiente elemento después del final del vector
   */
  iterator end() TL_NOEXCEPT;

  /*!
   * \brief Devuelve un iterador constante al siguiente elemento después del final del contenedor
   * Este elemento actúa como un marcador de posición, intentar acceder a él resulta en un comportamiento no definido 
   * \return Iterador al siguiente elemento después del final del contenedor
   */
  const_iterator end() const TL_NOEXCEPT;

  reverse_iterator rbegin() TL_NOEXCEPT;
  const_reverse_iterator rbegin() const TL_NOEXCEPT;

  reverse_iterator rend() TL_NOEXCEPT;
  const_reverse_iterator rend() const TL_NOEXCEPT;

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
  //bool empty() const;

  void fill(T value);

  /*!
   * \brief Asignación de copia
   */
  Vector &operator=(const Vector &entity);

  /*!
   * \brief Asignación de movimiento
   */
  Vector &operator=(Vector &&entity) TL_NOEXCEPT;

  double module() const;

  void normalize();
  //Vector normalize() const;

  bool operator == (const Vector &vector) const;
  bool operator != (const Vector &vector) const;
  bool operator <  (const Vector &vector) const;
  bool operator <= (const Vector &vector) const;
  bool operator >  (const Vector &vector) const;
  bool operator >= (const Vector &vector) const;

  /*!
   * \brief Construye un vector de ceros
   * \f[ V = [ 0 0 0 ] \f]
   * \return
   */
  static Vector zero();

  //static Vector unit();

protected:

  std::array<T, _size> mVector;

};



/* Definición de alias Vector */


typedef Vector<2, int>    Vector2i;
typedef Vector<2, double> Vector2d;
typedef Vector<2, float>  Vector2f;
typedef Vector<3, int>    Vector3i;
typedef Vector<3, double> Vector3d;
typedef Vector<3, float>  Vector3f;


/* Implementación Vector */

template<size_t _size, typename T> inline 
Vector<_size, T>::Vector()
{
  T ini_val = -std::numeric_limits<T>().max();
  for (size_t i = 0; i < _size; i++) {
    this->mVector[i] = ini_val;
  }
}

template<size_t _size, typename T> inline
Vector<_size, T>::Vector(const Vector &vector)
  : mVector(vector.mVector)
{
}

template<size_t _size, typename T> inline
Vector<_size, T>::Vector(Vector &&vector) TL_NOEXCEPT
  : mVector(std::forward<std::array<T, _size>>(vector.mVector))
{
}

template<size_t _size, typename T> inline
Vector<_size, T>::Vector(std::initializer_list<T> vector)
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
size_t Vector<_size, T>::size() const TL_NOEXCEPT
{
  return _size;
}

template<size_t _size, typename T> inline
typename Vector<_size, T>::reference Vector<_size, T>::front()
{
  return mVector.front();
}

template<size_t _size, typename T> inline
typename Vector<_size, T>::const_reference Vector<_size, T>::front() const
{
  return mVector.front();
}

template<size_t _size, typename T> inline
typename Vector<_size, T>::reference Vector<_size, T>::back()
{
  return mVector.back();
}

template<size_t _size, typename T> inline
typename Vector<_size, T>::const_reference Vector<_size, T>::back() const
{
  return mVector.back();
}

template<size_t _size, typename T> inline
typename Vector<_size, T>::iterator Vector<_size, T>::begin() TL_NOEXCEPT
{
  return mVector.begin();
}

template<size_t _size, typename T> inline 
typename Vector<_size, T>::const_iterator Vector<_size, T>::begin() const TL_NOEXCEPT
{
  return mVector.cbegin();
}

template<size_t _size, typename T> inline
typename Vector<_size, T>::iterator Vector<_size, T>::end() TL_NOEXCEPT
{
  return mVector.end();
}

template<size_t _size, typename T> inline
typename Vector<_size, T>::const_iterator Vector<_size, T>::end() const TL_NOEXCEPT
{
  return mVector.cend();
}

template<size_t _size, typename T> inline
typename Vector<_size, T>::reverse_iterator Vector<_size, T>::rbegin() TL_NOEXCEPT
{
  return mVector.rbegin();
}

template<size_t _size, typename T> inline
typename Vector<_size, T>::const_reverse_iterator Vector<_size, T>::rbegin() const TL_NOEXCEPT
{
  return mVector.rbegin();
}

template<size_t _size, typename T> inline
typename Vector<_size, T>::reverse_iterator Vector<_size, T>::rend() TL_NOEXCEPT
{
  return mVector.rend();
}

template<size_t _size, typename T> inline
typename Vector<_size, T>::const_reverse_iterator Vector<_size, T>::rend() const TL_NOEXCEPT
{
  return mVector.rend();
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
//bool Vector<_size, T>::empty() const
//{
//  return mVector.empty();
//}

template<size_t _size, typename T> inline
void Vector<_size, T>::fill(T value)
{
  return mVector.fill(value);
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
Vector<_size, T> &Vector<_size, T>::operator=(Vector<_size, T> &&vector) TL_NOEXCEPT
{
  if (this != &vector) {
    this->mVector = std::forward<std::array<T, _size>>(vector.mVector);
  }
  return (*this);
}

template<size_t _size, typename T> inline
double Vector<_size, T>::module() const
{
  return sqrt(dotProduct(*this, *this));
}

template<size_t _size, typename T> inline
void Vector<_size, T>::normalize()
{
  T length = static_cast<T>(this->module());
  if (length > static_cast<T>(0)) {
    *this /= length;
  } else {
    for (size_t i = 0; i < _size; i++) {
      this->mVector[i] = static_cast<T>(0);
    }
  }
}

//template<size_t _size, typename T> inline
//Vector<_size, T> Vector<_size, T>::normalize() const
//{
//  Vector<_size, T> v = *this;
//  T length = static_cast<T>(v.module());
//  if (length > static_cast<T>(0)) {
//    v /= length;
//  } else {
//    for (size_t i = 0; i < _size; i++) {
//      v.at(i) = static_cast<T>(0);
//    }
//  }
//  return v;
//}

template<size_t _size, typename T> inline
bool Vector<_size, T>::operator == (const Vector &vector) const
{
  return this->mVector == vector.mVector;
}

template<size_t _size, typename T> inline
bool Vector<_size, T>::operator != (const Vector &vector) const
{
  return this->mVector != vector.mVector;
}

template<size_t _size, typename T> inline
bool Vector<_size, T>::operator <  (const Vector &vector) const
{
  return this->mVector < vector.mVector;
}

template<size_t _size, typename T> inline
bool Vector<_size, T>::operator <= (const Vector &vector) const
{
  return this->mVector <= vector.mVector;
}

template<size_t _size, typename T> inline
bool Vector<_size, T>::operator >  (const Vector &vector) const
{
  return this->mVector > vector.mVector;
}

template<size_t _size, typename T> inline
bool Vector<_size, T>::operator >= (const Vector &vector) const
{
  return this->mVector > vector.mVector;
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
  return vector;
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


template<size_t _size, typename T> inline 
double dotProduct(const Vector<_size, T> &v1,
                  const Vector<_size, T> &v2)
{
  double dot = static_cast<double>(v1.at(0)) * static_cast<double>(v2[0]);
  for (size_t i = 1; i < _size; i++) {
    dot += static_cast<double>(v1[i]) * static_cast<double>(v2[i]);
  }
  return dot;
}



/////////////////////////////////// 



template<typename T = double>
class VectorDyn
{

public:

  /*!
   * \brief value_type
   */
  typedef T value_type;

  /*!
   * \brief Tipo entero sin signo (por lo general size_t)
   */
  typedef typename std::vector<T>::size_type size_type;

  /*!
   * \brief Tipo entero con signo (por lo general ptrdiff_t)
   */
  typedef typename std::vector<T>::difference_type difference_type;

  /*!
   * \brief std::allocator_traits<Allocator>::pointer
   */
  typedef typename std::vector<T>::pointer pointer;

  /*!
   * \brief std::allocator_traits<Allocator>::const_pointer 
   */
  typedef typename std::vector<T>::const_pointer const_pointer;

  /*!
   * \brief value_type&
   */
  typedef typename std::vector<T>::reference reference;

  /*!
   * \brief const value_type&
   */
  typedef typename std::vector<T>::const_reference const_reference;

  /*!
   * \brief Iterador de acceso aleatorio
   */
  typedef typename std::vector<T>::iterator iterator;

  /*!
   * \brief Iterador constante de acceso aleatorio
   */
  typedef typename std::vector<T>::const_iterator const_iterator;

  /*!
   * \brief Iterador de acceso aleatorio
   */
  typedef typename std::vector<T>::reverse_iterator reverse_iterator;

  /*!
   * \brief Iterador constante de acceso aleatorio
   */
  typedef typename std::vector<T>::const_reverse_iterator const_reverse_iterator;

public:
  
  /*!
   * \brief Constructora por defecto
   */
  VectorDyn();

  VectorDyn(size_t size, 
            T val = -std::numeric_limits<T>().max());

  /*!
   * \brief Constructor de copia
   * \param[in] vector Objeto que se copia
   */
  VectorDyn(const VectorDyn &vector);

  /*!
   * \brief Constructor de movimiento
   * \param[in] vector Objeto que se mueve
   */
  VectorDyn(VectorDyn &&vector) TL_NOEXCEPT;

  /*!
   * \brief Constructora inicializador de lista
   * \param[in] values Listado de valores del vector
   */
  VectorDyn(std::initializer_list<T> values);

  /*!
   * \brief Destructora
   */
  ~VectorDyn() = default;

  /*!
   * \brief Devuelve el tamaño del vector
   * \return Tamaño del vector
   */
  size_t size() TL_NOEXCEPT;

  void resize(size_t size);
  void resize(size_t size, T value);

  reference front();
  const_reference front() const;
  reference back();
  const_reference back() const;

  /*!
   * \brief Devuelve un iterador al inicio del vector
   * \return Iterador al primer elemento del vector
   */
  iterator begin() TL_NOEXCEPT;

  /*!
   * \brief Devuelve un iterador constante al inicio del vector
   * \return Iterador al primer elemento del vector
   */
  const_iterator begin() const TL_NOEXCEPT;

  /*!
   * \brief Devuelve un iterador al siguiente elemento después del final del vector
   * Este elemento actúa como un marcador de posición, intentar acceder a él resulta en un comportamiento no definido
   * \return Iterador al siguiente elemento después del final del vector
   */
  iterator end() TL_NOEXCEPT;

  /*!
   * \brief Devuelve un iterador constante al siguiente elemento después del final del contenedor
   * Este elemento actúa como un marcador de posición, intentar acceder a él resulta en un comportamiento no definido 
   * \return Iterador al siguiente elemento después del final del contenedor
   */
  const_iterator end() const TL_NOEXCEPT;

  reverse_iterator rbegin() TL_NOEXCEPT;
  const_reverse_iterator rbegin() const TL_NOEXCEPT;

  reverse_iterator rend() TL_NOEXCEPT;
  const_reverse_iterator rend() const TL_NOEXCEPT;

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
  //bool empty() const;

  void fill(T value);

  /*!
   * \brief Asignación de copia
   */
  VectorDyn &operator=(const VectorDyn &entity);

  /*!
   * \brief Asignación de movimiento
   */
  VectorDyn &operator=(VectorDyn &&entity) TL_NOEXCEPT;

  double module() const;

  void normalize();
  //Vector normalize() const;

  bool operator == (const VectorDyn &vector) const;
  bool operator != (const VectorDyn &vector) const;
  bool operator <  (const VectorDyn &vector) const;
  bool operator <= (const VectorDyn &vector) const;
  bool operator >  (const VectorDyn &vector) const;
  bool operator >= (const VectorDyn &vector) const;

  /*!
   * \brief Construye un vector de ceros
   * \f[ V = [ 0 0 0 ] \f]
   * \return
   */
  static VectorDyn zero(size_t size);

  //static Vector unit();

private:

  std::vector<T> mVector;

};


/* Definición de alias Vector */


typedef VectorDyn<int>    Vectorni;
typedef VectorDyn<double> Vectornd;
typedef VectorDyn<float>  Vectornf;


/* Implementación Vector */

template<typename T> inline 
VectorDyn<T>::VectorDyn()
{
  //T ini_val = -std::numeric_limits<T>().max();
  //for (size_t i = 0; i < _size; i++) {
  //  this->mVector[i] = ini_val;
  //}
}

template<typename T> inline 
VectorDyn<T>::VectorDyn(size_t size, T val)
{
  this->mVector.resize(size, val);
}

template<typename T> inline
VectorDyn<T>::VectorDyn(const VectorDyn &vector)
  : mVector(vector.mVector)
{
}

template<typename T> inline
VectorDyn<T>::VectorDyn(VectorDyn &&vector) TL_NOEXCEPT
  : mVector(std::move(vector.mVector))
{
}

template<typename T> inline
VectorDyn<T>::VectorDyn(std::initializer_list<T> vector)
{
  mVector = vector;
}

template<typename T> inline 
size_t VectorDyn<T>::size() TL_NOEXCEPT
{
  return mVector.size();
}

template<typename T> inline
void VectorDyn<T>::resize(size_t size)
{
  mVector.resize(size);
}

template<typename T> inline
void VectorDyn<T>::resize(size_t size, T value)
{
  mVector.resize(size, value);
}

template<typename T> inline
typename VectorDyn<T>::reference VectorDyn<T>::front()
{
  return mVector.front();
}

template<typename T> inline
typename VectorDyn<T>::const_reference VectorDyn<T>::front() const
{
  return mVector.front();
}

template<typename T> inline
typename VectorDyn<T>::reference VectorDyn<T>::back()
{
  return mVector.back();
}

template<typename T> inline
typename VectorDyn<T>::const_reference VectorDyn<T>::back() const
{
  return mVector.back();
}

template<typename T> inline
typename VectorDyn<T>::iterator VectorDyn<T>::begin() TL_NOEXCEPT
{
  return mVector.begin();
}

template<typename T> inline 
typename VectorDyn<T>::const_iterator VectorDyn<T>::begin() const TL_NOEXCEPT
{
  return mVector.cbegin();
}

template<typename T> inline
typename VectorDyn<T>::iterator VectorDyn<T>::end() TL_NOEXCEPT
{
  return mVector.end();
}

template<typename T> inline
typename VectorDyn<T>::const_iterator VectorDyn<T>::end() const TL_NOEXCEPT
{
  return mVector.cend();
}

template<typename T> inline
typename VectorDyn<T>::reverse_iterator VectorDyn<T>::rbegin() TL_NOEXCEPT
{
  return mVector.rbegin();
}

template<typename T> inline
typename VectorDyn<T>::const_reverse_iterator VectorDyn<T>::rbegin() const TL_NOEXCEPT
{
  return mVector.rbegin();
}

template<typename T> inline
typename VectorDyn<T>::reverse_iterator VectorDyn<T>::rend() TL_NOEXCEPT
{
  return mVector.rend();
}

template<typename T> inline
typename VectorDyn<T>::const_reverse_iterator VectorDyn<T>::rend() const TL_NOEXCEPT
{
  return mVector.rend();
}

template<typename T> inline
typename VectorDyn<T>::const_reference VectorDyn<T>::at(size_type position) const
{
  return mVector.at(position);
}

template<typename T> inline
typename VectorDyn<T>::reference VectorDyn<T>::at(size_type position)
{
  return mVector.at(position);
}

template<typename T> inline
typename VectorDyn<T>::const_reference VectorDyn<T>::operator[](size_t position) const
{
  return mVector[position];
}

template<typename T> inline
typename VectorDyn<T>::reference VectorDyn<T>::operator[](size_t position)
{
  return mVector[position];
}

//template<typename T> inline
//bool VectorDyn<T>::empty() const
//{
//  return mVector.empty();
//}

template<typename T> inline
void VectorDyn<T>::fill(T value)
{
  return mVector.fill(value);
}

template<typename T> inline
VectorDyn<T> &VectorDyn<T>::operator=(const VectorDyn<T> &vector)
{
  if (this != &vector) {
    this->mVector = vector.mVector;
  }
  return (*this);
}

template<typename T> inline
VectorDyn<T> &VectorDyn<T>::operator=(VectorDyn<T> &&vector) TL_NOEXCEPT
{
  if (this != &vector) {
    this->mVector = std::forward<std::vector<T>>(vector.mVector);
  }
  return (*this);
}

template<typename T> inline
double VectorDyn<T>::module() const
{
  return sqrt(dotProduct(*this, *this));
}

template<typename T> inline
void VectorDyn<T>::normalize()
{
  T length = static_cast<T>(this->module());
  if (length > static_cast<T>(0)) {
    *this /= length;
  } else {
    for (size_t i = 0; i < mVector.size(); i++) {
      this->mVector[i] = static_cast<T>(0);
    }
  }
}

template<typename T> inline
bool VectorDyn<T>::operator == (const VectorDyn &vector) const
{
  return this->mVector == vector.mVector;
}

template<typename T> inline
bool VectorDyn<T>::operator != (const VectorDyn &vector) const
{
  return this->mVector != vector.mVector;
}

template<typename T> inline
bool VectorDyn<T>::operator <  (const VectorDyn &vector) const
{
  return this->mVector < vector.mVector;
}

template<typename T> inline
bool VectorDyn<T>::operator <= (const VectorDyn &vector) const
{
  return this->mVector <= vector.mVector;
}

template<typename T> inline
bool VectorDyn<T>::operator >  (const VectorDyn &vector) const
{
  return this->mVector > vector.mVector;
}

template<typename T> inline
bool VectorDyn<T>::operator >= (const VectorDyn &vector) const
{
  return this->mVector > vector.mVector;
}

template<typename T> inline
VectorDyn<T> VectorDyn<T>::zero(size_t size)
{
  return VectorDyn<T>(size, 0);
}

/* Operaciones unarias */

template<typename T>  static
VectorDyn<T> operator + (const VectorDyn<T> &vector)
{
  return vector;
}

template<typename T> static
VectorDyn<T> operator - (const VectorDyn<T> &vector)
{
  VectorDyn<T> v(vector);
  for (size_t i = 0; i < v.size(); i++) {
    v[i] = -v[i];
  }
  return v;
}

/* Operaciones binarias */

template<typename T>
VectorDyn<T> operator + (const VectorDyn<T> &v0,
                         const VectorDyn<T> &v1)
{
  VectorDyn<T> v = v0;
  return v += v1;
}

template<typename T>
VectorDyn<T> &operator += (VectorDyn<T> &v0, 
                           const VectorDyn<T> &v1)
{
  //assert(v0.size() == v1.size(), "");
  for (size_t i = 0; i < v0.size(); i++) {
    v0[i] += v1[i];
  }
  return v0;
}

template<typename T>
VectorDyn<T> operator - (const VectorDyn<T> &v0,
                         const VectorDyn<T> &v1)
{
  VectorDyn<T> v = v0;
  return v -= v1;
}

template<typename T>
VectorDyn<T> &operator -= (VectorDyn<T> &v0, 
                           const VectorDyn<T> &v1)
{
  //assert(v0.size() == v1.size(), "");
  for (size_t i = 0; i < v0.size(); i++) {
    v0[i] -= v1[i];
  }
  return v0;
}


template<typename T>
VectorDyn<T> operator*(const VectorDyn<T> &v0,
                       const VectorDyn<T> &v1)
{
  VectorDyn<T> result = v0;
  return result *= v1;
}

template<typename T>
VectorDyn<T> &operator *= (VectorDyn<T> &v0, 
                           const VectorDyn<T> &v1)
{
  //TL_ASSERT(v0.size() == v1.size(), "");

  for (size_t i = 0; i < v0.size(); i++) {
    v0[i] *= v1[i];
  }
  return v0;
}

template<typename T>
VectorDyn<T> operator / (const VectorDyn<T> &v0,
                         const VectorDyn<T> &v1)
{
  VectorDyn<T> result = v0;
  return result /= v1;
}

template<typename T>
VectorDyn<T> &operator /= (VectorDyn<T> &v0, 
                           const VectorDyn<T> &v1)
{
  //TL_ASSERT(v0.size() == v1.size(), "");

  for (size_t i = 0; i < v0.size(); i++) {
    v0[i] /= v1[i];
  }
  return v0;
}

template<typename T>
VectorDyn<T> operator * (const VectorDyn<T> &vector, 
                         T scalar)
{
  VectorDyn<T> v = vector;
  return v *= scalar;
}

template<typename T>
VectorDyn<T> operator * (T scalar, 
                         const VectorDyn<T> &vector)
{
  VectorDyn<T> v = vector;
  return v *= scalar;
}

template<typename T>
VectorDyn<T> &operator *= (VectorDyn<T> &vector, 
                           T scalar)
{
  for (size_t i = 0; i < vector.size(); i++) {
    vector[i] *= scalar;
  }
  return vector;
}

template<typename T>
VectorDyn<T> operator / (const VectorDyn<T> &vector, 
                         T scalar)
{
  VectorDyn<T> v = vector;
  return v /= scalar;
}

template<typename T>
VectorDyn<T> &operator /= (VectorDyn<T> &vector,
                           T scalar)
{
  if (scalar != static_cast<T>(0)) {
    for (size_t i = 0; i < vector.size(); i++) {
      vector[i] /= scalar;
    }
  } else {
    for (size_t i = 0; i < vector.size(); i++) {
      vector[i] = static_cast<T>(0);
    }
  }
  return vector;
}


template<size_t _size, typename T> inline 
double dotProduct(const VectorDyn<T> &v1,
                  const VectorDyn<T> &v2)
{
  //TL_ASSERT(v1.size() == v2.size(), "");

  double dot = static_cast<double>(v1.at(0)) * static_cast<double>(v2[0]);
  for (size_t i = 1; i < v1.size(); i++) {
    dot += static_cast<double>(v1[i]) * static_cast<double>(v2[i]);
  }

  return dot;
}




//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////


template<typename T, size_t _size>
class VectorBase;

template<typename T, size_t _size>
class VectorBase
{

public:

  using value_type      = T;
  using size_type       = size_t;
  using pointer         = T*;
  using const_pointer   = const T*;
  using reference       = T&;
  using const_reference = const T&;

public:

  VectorBase()
  {
    T ini_val = -std::numeric_limits<T>().max();
    for (size_t i = 0; i < _size; i++) {
      this->mData[i] = ini_val;
    }
  }

  VectorBase(size_t size,
             T val)
  {    
    static_assert(_size == -1, "Dynamic Matrix not support resize");
  }

  VectorBase(const VectorBase &vector)
    : mData(vector.mData)
  {
  }

  VectorBase(VectorBase &&vector) TL_NOEXCEPT
    : mData(std::move(vector.mData))
  {
  }
    
  VectorBase(std::initializer_list<T> values)
  {
    size_t n = values.size();
    if (n == _size){
      std::copy(values.begin(), values.end(), mData.begin());
    } else if (n > _size){
      std::copy(values.begin(), values.end(), mData.begin());
      std::fill(mData.begin() + n, mData.end(), T{0});
    } else {
      std::copy(values.begin(), values.begin() + n, mData.begin());
    }
  }

  void resize(size_t size)
  {
    static_assert(_size == -1, "Dynamic Vector not support resize");
  }
  
  void resize(size_t size, T value)
  {
    static_assert(_size == -1, "Dynamic Vector not support resize");
  }

  size_t size() const TL_NOEXCEPT
  {
    return _size;
  }
    
  const_reference at(size_type position) const
  { 
    return mData.at(position);
  }

  reference at(size_type position)
  {
    return mData.at(position);
  }

  const_reference operator[](size_t position) const
  {
    return mData[position];
  }

  reference operator[](size_t position)
  {
    return mData[position];
  }

  //void fill(T value)
  //{
  //  mData.fill(value);
  //}

  bool operator == (const VectorBase &vector) const;
  bool operator != (const VectorBase &vector) const;
  bool operator <  (const VectorBase &vector) const;
  bool operator <= (const VectorBase &vector) const;
  bool operator >  (const VectorBase &vector) const;
  bool operator >= (const VectorBase &vector) const;

protected:

  std::array<T, _size> mData;

};

template<typename T, size_t _size> inline
bool VectorBase<T, _size>::operator == (const VectorBase<T, _size> &vector) const
{
  return this->mData == vector.mData;
}

template<typename T, size_t _size> inline
bool VectorBase<T, _size>::operator != (const VectorBase<T, _size> &vector) const
{
  return this->mData != vector.mData;
}

template<typename T, size_t _size> inline
bool VectorBase<T, _size>::operator <  (const VectorBase<T, _size> &vector) const
{
  return this->mData < vector.mData;
}

template<typename T, size_t _size> inline
bool VectorBase<T, _size>::operator <= (const VectorBase<T, _size> &vector) const
{
  return this->mData <= vector.mData;
}

template<typename T, size_t _size> inline
bool VectorBase<T, _size>::operator >  (const VectorBase<T, _size> &vector) const
{
  return this->mData > vector.mData;
}

template<typename T, size_t _size> inline
bool VectorBase<T, _size>::operator >= (const VectorBase<T, _size> &vector) const
{
  return this->mData > vector.mData;
}




template<typename T>
class VectorBase<T, -1>
{

public:

  using value_type      = T;
  using size_type       = size_t;
  using pointer         = T*;
  using const_pointer   = const T*;
  using reference       = T&;
  using const_reference = const T&;

public:

  VectorBase()
  {
  }
  
  VectorBase(size_t size,
             T val)
  {
    mData.resize(size, val);
  }

  VectorBase(const VectorBase &vector)
    : mData(vector.mData)
  {
  }
    
  VectorBase(VectorBase &&vector) TL_NOEXCEPT
    : mData(std::move(vector.mData))
  {
  }
 
  VectorBase(std::initializer_list<T> values)
    : mData(values)
  {
  }

  void resize(size_t size)
  {
    mData.resize(size);
  }
  
  void resize(size_t size, T value)
  {
    mData.resize(size, value);
  }

  size_t size() const TL_NOEXCEPT
  {
    return mData.size();
  }

  const_reference at(size_type position) const
  { 
    return mData.at(position);
  }

  reference at(size_type position)
  {
    return mData.at(position);
  }

  const_reference operator[](size_t position) const
  {
    return mData[position];
  }

  reference operator[](size_t position)
  {
    return mData[position];
  }

  //void fill(T value)
  //{
  //  mData.fill(value);
  //}

  bool operator == (const VectorBase &vector) const;
  bool operator != (const VectorBase &vector) const;
  bool operator <  (const VectorBase &vector) const;
  bool operator <= (const VectorBase &vector) const;
  bool operator >  (const VectorBase &vector) const;
  bool operator >= (const VectorBase &vector) const;

private:

  TL_TODO("¿Probar con std::valarray?")
  std::vector<T> mData;

};


template<typename T> inline
bool VectorBase<T, -1>::operator == (const VectorBase<T, -1> &vector) const
{
  return this->mData == vector.mData;
}

template<typename T> inline
bool VectorBase<T, -1>::operator != (const VectorBase<T, -1> &vector) const
{
  return this->mData != vector.mData;
}

template<typename T> inline
bool VectorBase<T, -1>::operator <  (const VectorBase<T, -1> &vector) const
{
  return this->mData < vector.mData;
}

template<typename T> inline
bool VectorBase<T, -1>::operator <= (const VectorBase<T, -1> &vector) const
{
  return this->mData <= vector.mData;
}

template<typename T> inline
bool VectorBase<T, -1>::operator >  (const VectorBase<T, -1> &vector) const
{
  return this->mData > vector.mData;
}

template<typename T> inline
bool VectorBase<T, -1>::operator >= (const VectorBase<T, -1> &vector) const
{
  return this->mData > vector.mData;
}





template<typename T, size_t _size = -1>
class Vector2
  : public VectorBase<T, _size>
{

public:
  
  Vector2();
  Vector2(size_t size, T val = -std::numeric_limits<T>().max());
  Vector2(const Vector2 &vector);
  Vector2(Vector2 &&vector) TL_NOEXCEPT;
  Vector2(std::initializer_list<T> values);
  ~Vector2() = default;

  Vector2 &operator=(const Vector2 &vector);
  Vector2 &operator=(Vector2 &&vector) TL_NOEXCEPT;

  double module() const;
  void normalize();

  static Vector2 zero();
  static Vector2 zero(size_t size);
  static Vector2 unit();
  static Vector2 unit(size_t size);
};


/* Definición de alias Vector */


//typedef Vector2<int, 2>    Vector2i;
//typedef Vector2<double, 2> Vector2d;
//typedef Vector2<float, 2>  Vector2f;
//typedef Vector2<int, 3>    Vector3i;
//typedef Vector2<double, 3> Vector3d;
//typedef Vector2<float, 3>  Vector3f;



/* Implementación Vector */

template<typename T, size_t _size> inline
Vector2<T, _size>::Vector2()
{
}
  
template<typename T, size_t _size> inline
Vector2<T, _size>::Vector2(size_t size, T val)
  : VectorBase(size, val)
{
}
 
template<typename T, size_t _size> inline
Vector2<T, _size>::Vector2(const Vector2 &vector)
  : VectorBase(vector)
{
}

template<typename T, size_t _size> inline
Vector2<T, _size>::Vector2(Vector2 &&vector) TL_NOEXCEPT
  : VectorBase(std::forward<VectorBase<T, _size>>(vector))
{
}

template<typename T, size_t _size> inline
Vector2<T, _size>::Vector2(std::initializer_list<T> values)
  : VectorBase(values)
{
}

template<typename T, size_t _size> inline
Vector2<T, _size> &Vector2<T, _size>::operator=(const Vector2<T, _size> &vector)
{
  if (this != &vector) {
    //this->mData = vector.mData;
    VectorBase::operator=(vector);
  }
  return (*this);
}

template<typename T, size_t _size> inline
Vector2<T, _size> &Vector2<T, _size>::operator=(Vector2<T, _size> &&vector) TL_NOEXCEPT
{
  if (this != &vector) {
    VectorBase::operator=(std::forward<VectorBase<T, _size>>(vector));
    //this->mData = std::move(vector.mData);
  }
  return (*this);
}

template<typename T, size_t _size> inline
double Vector2<T, _size>::module() const
{
  return sqrt(dotProduct(*this, *this));
}

template<typename T, size_t _size> inline
void Vector2<T, _size>::normalize()
{
  T length = static_cast<T>(this->module());
  if (length > static_cast<T>(0)) {
    *this /= length;
  } else {
    for (size_t i = 0; i < mVector.size(); i++) {
      this->mVector[i] = static_cast<T>(0);
    }
  }
}

template<typename T, size_t _size> inline
Vector2<T, _size> Vector2<T, _size>::zero()
{ 
  Vector2<T, _size> vector;
  for (size_t i = 0; i < vector.size(); i++) {
    vector[i] = static_cast<size_t>(0);
  }
  return vector;
}

template<typename T, size_t _size> inline
Vector2<T, _size> Vector2<T, _size>::zero(size_t size)
{ 
  static_assert(_size == -1, "Dynamic Vector not support resize");
  return Vector2<T>(size, static_cast<size_t>(0));
}

template<typename T, size_t _size> inline
Vector2<T, _size> Vector2<T, _size>::unit()
{
  Vector2<T, _size> vector;
  for (size_t i = 0; i < vector.size(); i++) {
    vector[i] = static_cast<size_t>(1);
  }
  return vector;
}

template<typename T, size_t _size> inline
Vector2<T, _size> Vector2<T, _size>::unit(size_t size)
{
  static_assert(_size == -1, "Dynamic Matrix not support resize");
  return Vector2<T>(size, static_cast<size_t>(1));
}

/* Operaciones unarias */

template<typename T, size_t _size>  static
Vector2<T, _size> operator + (const Vector2<T, _size> &vector)
{
  return vector;
}

template<typename T, size_t _size> static
Vector2<T, _size> operator - (const Vector2<T, _size> &vector)
{
  Vector2<T, _size> v = vector;
  for (size_t i = 0; i < vector.size(); i++) {
    v[i] = -vector[i];
  }
  return v;
}

/* Operaciones binarias */

template<typename T, size_t _size>
Vector2<T, _size> operator + (const Vector2<T, _size> &v0,
                              const Vector2<T, _size> &v1)
{
  Vector2<T, _size> v = v0;
  return v += v1;
}

template<typename T, size_t _size>
Vector2<T, _size> &operator += (Vector2<T, _size> &v0, 
                                const Vector2<T, _size> &v1)
{
  TL_ASSERT(v0.size() == v1.size(), "");

  for (size_t i = 0; i < v0.size(); i++) {
    v0[i] += v1[i];
  }
  return v0;
}

template<typename T, size_t _size>
Vector2<T, _size> operator - (const Vector2<T, _size> &v0,
                              const Vector2<T, _size> &v1)
{
  Vector2<T, _size> v = v0;
  return v -= v1;
}

template<typename T, size_t _size>
Vector2<T, _size> &operator -= (Vector2<T, _size> &v0, 
                                const Vector2<T, _size> &v1)
{
  TL_ASSERT(v0.size() == v1.size(), "");

  for (size_t i = 0; i < v0.size(); i++) {
    v0[i] -= v1[i];
  }
  return v0;
}

template<typename T, size_t _size>
Vector2<T, _size> operator*(Vector2<T, _size> const& v0,
                            Vector2<T, _size> const& v1)
{
  Vector2<T, _size> result = v0;
  return result *= v1;
}

template<typename T, size_t _size>
Vector2<T, _size> &operator *= (Vector2<T, _size> &v0, 
                                const Vector2<T, _size> &v1)
{
  TL_ASSERT(v0.size() == v1.size(), "");

  for (size_t i = 0; i < v0.size(); i++) {
    v0[i] *= v1[i];
  }
  return v0;
}

template<typename T, size_t _size>
Vector2<T, _size> operator / (const Vector2<T, _size> &v0,
                              const Vector2<T, _size> &v1)
{
  Vector2<T, _size> result = v0;
  return result /= v1;
}

template<typename T, size_t _size>
Vector2<T, _size> &operator /= (Vector2<T, _size> &v0, 
                                const Vector2<T, _size> &v1)
{
  TL_ASSERT(v0.size() == v1.size(), "");

  for (size_t i = 0; i < v0.size(); i++) {
    v0[i] /= v1[i];
  }
  return v0;
}

template<typename T, size_t _size>
Vector2<T, _size> operator * (const Vector2<T, _size> &vector, 
                              T scalar)
{
  Vector2<T, _size> v = vector;
  return v *= scalar;
}

template<typename T, size_t _size>
Vector2<T, _size> operator * (T scalar, 
                              const Vector2<T, _size> &vector)
{
  Vector2<T, _size> v = vector;
  return v *= scalar;
}

template<typename T, size_t _size>
Vector2<T, _size> &operator *= (Vector2<T, _size> &vector, 
                                T scalar)
{
  for (size_t i = 0; i < vector.size(); i++) {
    vector[i] *= scalar;
  }
  return vector;
}

template<typename T, size_t _size>
Vector2<T, _size> operator / (const Vector2<T, _size> &vector, 
                              T scalar)
{
  Vector2<T, _size> v = vector;
  return v /= scalar;
}

template<typename T, size_t _size>
Vector2<T, _size> &operator /= (Vector2<T, _size> &vector,
                                T scalar)
{
  if (scalar != static_cast<T>(0)) {
    for (size_t i = 0; i < vector.size(); i++) {
      vector[i] /= scalar;
    }
  } else {
    for (size_t i = 0; i < vector.size(); i++) {
      vector[i] = static_cast<T>(0);
    }
  }
  return vector;
}

template<typename T, size_t _size> 
double dotProduct(const Vector2<T, _size> &v1,
                  const Vector2<T, _size> &v2)
{
  TL_ASSERT(v1.size() == v2.size(), "");

  double dot = static_cast<double>(v1.at(0)) * static_cast<double>(v2[0]);
  for (size_t i = 1; i < v1.size(); i++) {
    dot += static_cast<double>(v1[i]) * static_cast<double>(v2[i]);
  }
  return dot;
}

/*! \} */ // end of Algebra

/*! \} */ // end of Math

} // Fin namespace math


} // End namespace tl

#endif // TL_MATH_VECTOR_H

