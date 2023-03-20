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

#ifndef TL_MATH_TRANSFORM_H
#define TL_MATH_TRANSFORM_H

#include "tidop/config.h"

#include <vector>
#include <array>

#include "tidop/math/algebra/matrix.h"
#include "tidop/math/algebra/rotation_matrix.h"

namespace tl
{

namespace math
{

/*! \addtogroup Math
 *  \{
 */


 /*! \addtogroup Algebra
  *
  * Algebra
  *
  *  \{
  */


/*!
 * \brief 
 */
template <typename T, size_t dim>
class Transform
  : public Matrix<T, dim + 1, dim + 1>
{

public:
  
  Transform();
  Transform(const Transform<T, dim> &matrix);
  Transform(const Matrix<T, dim + 1, dim + 1> &matrix);
  ~Transform() override = default;

  RotationMatrix<T> rotation() const;
  void setRotation(const RotationMatrix<T> &rotation);
  
  Vector<T, dim> translation() const;
  void setTranslation(const Vector<T, dim> &translation);
  
  T scale() const;
  void setScale(T scale);
  
private:

};


template <typename T, size_t dim> inline
Transform<T, dim>::Transform()
  : Matrix<T, dim + 1, dim + 1>()
{
}

template <typename T, size_t dim> inline
Transform<T, dim>::Transform(const Transform<T, dim> &matrix)
  : Matrix<T, dim + 1, dim + 1>(matrix)
{
}

template <typename T, size_t dim> inline
Transform<T, dim>::Transform(const Matrix<T, dim + 1, dim + 1> &matrix)
  : Matrix<T, dim + 1, dim + 1>(matrix)
{
}

template <typename T, size_t dim> inline
RotationMatrix<T> Transform<T, dim>::rotation() const
{
  return RotationMatrix<T>();
}

template <typename T, size_t dim> inline
void Transform<T, dim>::setRotation(const RotationMatrix<T> &rotation)
{
}

template <typename T, size_t dim> inline
Vector<T, dim> Transform<T, dim>::translation() const
{
  return Vector<T, dim>();
}

template <typename T, size_t dim> inline
T Transform<T, dim>::scale() const
{
  return T();
}

template <typename T, size_t dim> inline
void Transform<T, dim>::setScale(T scale)
{
  
}

  
/*! \} */ // end of Algebra

/*! \} */ // end of Math

} // Fin namespace math

} // End namespace tl

#endif // TL_MATH_TRANSFORM_H
