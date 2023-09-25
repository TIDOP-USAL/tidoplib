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

#pragma once

#include "tidop/config.h"

#include <vector>
#include <array>

#include "tidop/math/algebra/transform.h"
#include "tidop/math/algebra/translation.h"
#include "tidop/math/algebra/scaling.h"


namespace tl
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
template <typename T, size_t Dim>
class Affine
{

public:

    enum
    {
        dimensions = Dim
    };

    using value_type = T;
    using size_type = size_t;
    using reference = T &;
    using const_reference = const T &;

public:

    Affine();
    //Affine(const Transform<T, Dim> &matrix);
    Affine(const Matrix<T, Dim, Dim + 1> &matrix);
    Affine(T sx, T sy, T tx, T ty, T angle);
    Affine(T sx, T sy, T sz, T tx, T ty, T tz, T omega, T phi, T kappa);
    Affine(const Point<T> &translation, const Vector<T, 2> &scale, T angle);
    Affine(const Point3<T> &translation, const Vector<T, 3> &scale, const RotationMatrix<double> &rotation);
    //Affine(const Translation<T, Dim> &translation, const Scaling<T, Dim> &scale, const Rotation<T, Dim> &rotation);
    ~Affine() override = default;

private:

    Matrix<T, Dim, Dim + 1> _transform;
};





template <typename T, size_t Dim> inline
Affine<T, Dim>::Affine()
  : _transform(Matrix<T, Dim, Dim + 1>::indentity())
{
    static_assert(dimensions == 2 || dimensions == 3, "Only 2 or 3 dimensions allowed");
}

//template <typename T, size_t dim> inline
//Affine<T, dim>::Affine(const Transform<T, dim> &matrix)
//  : _transform(matrix)
//{
//    static_assert(dim == 2 || dim == 3, "Only 2 or 3 dimensions allowed");
//}

template <typename T, size_t Dim> inline
Affine<T, Dim>::Affine(const Matrix<T, Dim, Dim + 1> &matrix)
  : _transform(matrix)
{
    static_assert(dimensions == 2 || dimensions == 3, "Only 2 or 3 dimensions allowed");
}

template<typename T, size_t Dim>
inline Affine<T, Dim>::Affine(T sx, T sy, T tx, T ty, T angle)
{
    static_assert(dimensions == 2, "Constructor for 2D Affine. Use the 3D Affine constructor: Affine(T sx, T sy, T sz, T tx, T ty, T tz, T omega, T phi, T kappa).");

    _transform[0][0] = sx * cos(angle);
    _transform[0][1] = -sy * sin(angle);
    _transform[0][2] = tx;
    _transform[1][0] = sx * sin(angle);
    _transform[1][1] = sy * cos(angle);
    _transform[1][2] = ty;
}

template<typename T, size_t Dim>
inline Affine<T, Dim>::Affine(T sx, T sy, T sz, T tx, T ty, T tz, T omega, T phi, T kappa)
{
    static_assert(dimensions == 3, "Constructor for 3D Affine. Use the 2D Affine constructor: Affine(T sx, T sy, T tx, T ty, T angle).");

    EulerAngles<T> eulerAngles(omega, phi, kappa);
    auto block = _transform.block(0, dimensions - 1, 0, dimensions - 1);
    block = eulerAngles;
    block *= Vector<T>{sx, sy, sz};
    _transform[0][3] = tx;
    _transform[1][3] = ty;
    _transform[2][3] = tz;
}

template<typename T, size_t Dim>
inline Affine<T, Dim>::Affine(const Point<T> &translation, const Vector<T, 2> &scale, T angle)
{
}

template<typename T, size_t Dim>
inline Affine<T, Dim>::Affine(const Point3<T> &translation, 
                              const Vector<T, 3> &scale,
                              const RotationMatrix<double> &rotation)
{
}

  
/*! \} */ // end of Algebra

/*! \} */ // end of Math

} // End namespace tl

