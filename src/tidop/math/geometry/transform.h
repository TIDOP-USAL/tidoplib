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

#include "tidop/math/algebra/matrix.h"
#include "tidop/math/algebra/vector.h"
#include "tidop/math/algebra/rotations/rotation_convert.h"

namespace tl
{


/*! \addtogroup GeometricTransformations
 *  \{
 */


//template <typename T, size_t Dim>
//class TransformBase
//{
//
//public:
//
//    virtual auto transform(const Point<T> &point) const  -> Point<T> = 0;
//    virtual auto transform(const Point3<T> &point) const  -> Point3<T> = 0;
//
//    virtual auto operator * (const Point<T> &point) const -> Point<T> = 0;
//    virtual auto operator * (const Point3<T> &point) const -> Point3<T> = 0;
//
//    virtual auto operator() (const Point<T> &point) const -> Point<T> = 0;
//    virtual auto operator() (const Point3<T> &point) const -> Point3<T> = 0;
//};


/*!
 * \brief 
 */
template <typename T, size_t Dim>
class Transform
{

public:

    enum
    {
        dimensions = Dim,
        matrix_size
    };

public:

    Transform();
    Transform(const Transform<T, Dim> &matrix);
    Transform(const Matrix<T, matrix_size, matrix_size> &matrix);
    virtual ~Transform() = default;

    //RotationMatrix<T> rotation() const;
    //void setRotation(const RotationMatrix<T> &rotation);
    
    auto translation() const -> Vector<T, Dim>;
    void setTranslation(const Vector<T, Dim> &translation);
    
    auto scale() const -> Vector<T, Dim>;
    void setScale(T scale);

    auto inverse() const -> Transform<T, Dim>;

    auto operator[](size_t position) const -> const internal::MatrixRow<const T>
    {
        return transformMatrix[position];
    }

    auto operator[](size_t position) -> internal::MatrixRow<T>
    {
        return transformMatrix[position];
    }

    template<size_t rows, size_t cols>
    auto transform(const Matrix<T, rows, cols> &src) -> Matrix<T, rows, cols>
    {
        TL_ASSERT(src.cols() == dimensions, "Invalid dimensions");
        return src * transformMatrix;
    }
        
    auto transform(const Vector<T, Dim> &src) -> Vector<T, Dim>
    {
        return transformMatrix * src;
    }

    //auto operator = (const Scaling<T,Dim> &scale) -> Transform<T, Dim>&
    //{
    //    this->transformMatrix = Matrix<T, Dim + 1, Dim + 1>::identity();

    //    for (size_t i = 0; i < Dim; i++) {
    //        TL_TODO("Comprobar si la escala es uniforme");
    //        //if (scale.isUniform()){ 
    //        this->transformMatrix(i, i) = scale;
    //        //} else {
    //        //this->transformMatrix(i, i) = scale[i];
    //        //}
    //    }
    //    return *this;
    //}

    //auto operator = (const Translation<T, Dim> &translate) -> Transform<T, Dim> &
    //{
    //    this->transformMatrix = Matrix<T, Dim + 1, Dim + 1>::identity();

    //    auto col = this->transformMatrix.col(Dim);
    //    for (size_t i = 0; i < Dim; i++)
    //        col[i] = translate[i];

    //    return *this;
    //}

    friend Transform<T, Dim> operator * (const Transform<T, Dim> &transform1,
                                         const Transform<T, Dim> &transform2)
    {
        Transform<T, Dim> transform = transform1.transformMatrix * transform2.transformMatrix;

        return transform;
    }

protected:

    Matrix<T, matrix_size, matrix_size> transformMatrix;
};


template <typename T, size_t Dim>
Transform<T, Dim>::Transform()
  : transformMatrix(Matrix<T, matrix_size, matrix_size>::identity())
{
    static_assert(Dim == 2 || Dim == 3, "Only 2 or 3 dimensions allowed");
}

template <typename T, size_t Dim>
Transform<T, Dim>::Transform(const Transform<T, Dim> &matrix)
  : transformMatrix(matrix.transformMatrix)
{
    static_assert(Dim == 2 || Dim == 3, "Only 2 or 3 dimensions allowed");
}

template <typename T, size_t Dim>
Transform<T, Dim>::Transform(const Matrix<T, matrix_size, matrix_size> &matrix)
  : transformMatrix(matrix)
{
}

//template <typename T, size_t Dim> inline
//RotationMatrix<T> Transform<T, Dim>::rotation() const
//{
//  return RotationMatrix<T>();
//}
//
//template <typename T, size_t Dim> inline
//void Transform<T, Dim>::setRotation(const RotationMatrix<T> &rotation)
//{
//}

template <typename T, size_t Dim>
auto Transform<T, Dim>::translation() const -> Vector<T, Dim>
{
    Vector<T, Dim> translation;
    for (size_t i = 0; i < Dim; i++) translation[i] = transformMatrix(i, Dim);
    return translation;
}

template <typename T, size_t Dim>
void Transform<T, Dim>::setTranslation(const Vector<T, Dim> &translation)
{
    auto col = transformMatrix.col(Dim);
    for (size_t i = 0; i < Dim; i++)
        col[i] = translation[i];
}

template <typename T, size_t Dim>
auto Transform<T, Dim>::scale() const -> Vector<T, Dim>
{
    Vector<T, Dim> _scale;
    for (size_t i = 0; i < Dim; i++) _scale[i] = transformMatrix(i, i);
    return _scale;
}

template <typename T, size_t Dim>
void Transform<T, Dim>::setScale(T scale)
{
    for (size_t i = 0; i < Dim; i++)
      transformMatrix(i, i) = scale;
}

template<typename T, size_t Dim>
auto Transform<T, Dim>::inverse() const -> Transform<T, Dim>
{
    return transformMatrix.inverse();
}


/*! \} */



} // End namespace tl

