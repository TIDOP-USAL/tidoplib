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
class Affine
  : public Matrix<T, dim + 1, dim + 1>
{

public:

    Affine();
    Affine(const Transform<T, dim> &matrix);
    Affine(const Matrix<T, dim + 1, dim + 1> &matrix);
    ~Affine() override = default;

private:

    Transform<T, dim> _transform;
};


template <typename T, size_t dim> inline
Affine<T, dim>::Affine()
  : _transform(Matrix<T, dim + 1, dim + 1>::indentity())
{
    static_assert(dim == 2 || dim == 3, "Only 2 or 3 dimensions allowed");
}

template <typename T, size_t dim> inline
Affine<T, dim>::Affine(const Transform<T, dim> &matrix)
  : _transform(matrix)
{
    static_assert(dim == 2 || dim == 3, "Only 2 or 3 dimensions allowed");
}

template <typename T, size_t dim> inline
Affine<T, dim>::Affine(const Matrix<T, dim + 1, dim + 1> &matrix)
  : _transform(matrix)
{
}

  
/*! \} */ // end of Algebra

/*! \} */ // end of Math

} // Fin namespace math

} // End namespace tl

