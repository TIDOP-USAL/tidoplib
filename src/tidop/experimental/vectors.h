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
 
#ifndef TL_EXPERIMENTAL_VECTORS_H
#define TL_EXPERIMENTAL_VECTORS_H

#include <mutex>

namespace tl
{

namespace experimental
{

/* ---------------------------------------------------------------------------------- */
/*                              Operaciones con vectores                              */
/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Ordena un vector de menor a mayor
 * \param[in] v Vector
 */
//template<typename T> inline 
//void sortVector(std::vector<T> *v)
//{
//  std::sort(v->begin(), v->end());
//}
//
///*!
// * \brief Ordena un vector de mayor a menor
// * \param[in] v Vector
// */
//template<typename T> inline 
//void sortVectorInv(std::vector<T> *v)
//{
//  std::sort(v->rbegin(),v->rend());
//}
//
///*!
// * \brief Determinar el número de elementos iguales a un número.
// * Sobrecarga del operador == para determinar el número de elementos de un
// * vector que son iguales al valor pasado como parámetro.
// * \param[in] v Vector
// * \param[in] t Valor
// * \return Número de elementos que cumplen la condición
// */
//template<typename T> inline 
//int operator==(const std::vector<T> &v, const T t)
//{
//  sortVector(v);
//  std::pair<typename std::vector<T>::iterator, typename std::vector<T>::iterator> bounds;
//  bounds = std::equal_range(v.begin(), v.end(), t);
//  int ini = bounds.first - v.begin();
//  int end = bounds.second - v.begin();
//  return ( end - ini );
//}
//
///*!
// * \brief Determina el número de elementos distintos a un número.
// * Sobrecarga del operador != para determinar el número de elementos de un
// * vector que son distintos al valor pasado como parámetro.
// * \param[in] v Vector
// * \param[in] t Valor
// * \return Número de elementos que cumplen la condición
// */
//template<typename T> inline 
//int operator!=(const std::vector<T> &v, const T t)
//{
//  sortVector(v);
//  std::pair<typename std::vector<T>::iterator, typename std::vector<T>::iterator> bounds;
//  bounds = std::equal_range(v.begin(), v.end(), t);
//  int r1 = bounds.first - v.begin();
//  int r2 = v.end() - bounds.second;
//  return (r1 + r2 );
//}
//
///*!
// * \brief operator >=
// * \param[in] v
// * \param[in] t
// * \return
// */
//template<typename T> inline 
//int operator>=(const std::vector<T> &v, const T t)
//{
//  sortVector(v);
//  typename std::vector<T>::iterator upOrEq;
//  upOrEq = std::lower_bound(v.begin(), v.end(), t);
//  return (v.end() - upOrEq);
//}
//
//template<typename T> inline 
//int operator<=(const std::vector<T> &v, const T t)
//{
//  sortVector(v);
//  typename std::vector<T>::iterator lowOrEq;
//  lowOrEq = std::upper_bound(v.begin(), v.end(), t);
//  return (lowOrEq - v.begin());
//}
//
//template<typename T> inline 
//int operator> (const std::vector<T> &v, const T t)
//{
//  sortVector(v);
//  typename std::vector<T>::iterator up;
//  up = std::upper_bound(v.begin(), v.end(), t);
//  return (v.end() - up);
//}
//
//template<typename T> inline 
//int operator< (const std::vector<T> &v, const T t)
//{
//  sortVector(v);
//  typename std::vector<T>::iterator low;
//  low = std::lower_bound(v.begin(), v.end(), t);
//  return (low - v.begin());
//}

/* ---------------------------------------------------------------------------------- */

} // End namespace experimental

} // End namespace tl

#endif // TL_EXPERIMENTAL_VECTORS_H
