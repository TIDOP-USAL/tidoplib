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

#include "tidop/math/algebra/matrix.h"
#include "tidop/math/algebra/rotation_matrix.h"
#include "tidop/math/algebra/svd.h"
#include <tidop/math/statistics.h>
#include <tidop/math/algebra/vector.h>
#include "tidop/math/algebra/rotation_convert.h"

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


template <typename T, size_t Dim> inline
Transform<T, Dim>::Transform()
  : transformMatrix(Matrix<T, matrix_size, matrix_size>::identity())
{
    static_assert(Dim == 2 || Dim == 3, "Only 2 or 3 dimensions allowed");
}

template <typename T, size_t Dim> inline
Transform<T, Dim>::Transform(const Transform<T, Dim> &matrix)
  : transformMatrix(matrix.transformMatrix)
{
    static_assert(Dim == 2 || Dim == 3, "Only 2 or 3 dimensions allowed");
}

template <typename T, size_t Dim> inline
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

template <typename T, size_t Dim> inline
auto Transform<T, Dim>::translation() const -> Vector<T, Dim>
{
    Vector<T, Dim> translation;
    for (size_t i = 0; i < Dim; i++) translation[i] = transformMatrix(i, Dim);
    return translation;
}

template <typename T, size_t Dim> inline
void Transform<T, Dim>::setTranslation(const Vector<T, Dim> &translation)
{
    auto col = transformMatrix.col(Dim);
    for (size_t i = 0; i < Dim; i++)
        col[i] = translation[i];
}

template <typename T, size_t Dim> inline
auto Transform<T, Dim>::scale() const -> Vector<T, Dim>
{
    Vector<T, Dim> _scale;
    for (size_t i = 0; i < Dim; i++) _scale[i] = transformMatrix(i, i);
    return _scale;
}

template <typename T, size_t Dim> inline
void Transform<T, Dim>::setScale(T scale)
{
    for (size_t i = 0; i < Dim; i++)
      transformMatrix(i, i) = scale;
}

template<typename T, size_t Dim>
inline auto Transform<T, Dim>::inverse() const -> Transform<T, Dim>
{
    return transformMatrix.inverse();
}



/* Rotation */

template <typename T, size_t Dim>
class Rotation
  : public Transform<T, Dim> 
{

public:

    Rotation() : Transform<T, Dim>() {}
    
    Rotation(T angle) : Transform<T, Dim>() 
    {
        static_assert(Dim == 2, "Constructor for 2D Rotation. Use the 3D Rotation constructor.");

        this->transformMatrix[0][0] = cos(angle);
        this->transformMatrix[0][1] = -sin(angle);
        this->transformMatrix[1][0] = sin(angle);
        this->transformMatrix[1][1] = cos(angle);
    }
    
    Rotation(const RotationMatrix<double> &rotation) : Transform<T, Dim>() 
    {
        static_assert(Dim == 2, "Constructor for 3D Rotation. Use the 2D Rotation constructor.");

        auto block = this->transformMatrix.block(0, 2, 0, 2);
        block = rotation;
    }

    ~Rotation() override = default;

private:

};






template <typename T, size_t Dim>
class Umeyama
  : public Transform<T, Dim> 
{

public:

    Umeyama() : Transform<T, Dim>() {}
    ~Umeyama() override = default;

    //template<size_t rows, size_t cols>
    //void compute(const Matrix<T, rows, cols> &src,
    //             const Matrix<T, rows, cols> &dst)
    //{
    //    size_t size = src.rows();
    //    Vector<double> mean_src(Dim);
    //    Vector<double> mean_dst(Dim);

    //    for (size_t c = 0; c < Dim; c++) {
    //        auto src_col = src.col(c);
    //        mean_src[c] = mean(src_col.begin(), src_col.end());
    //        auto dst_col = dst.col(c);
    //        mean_dst[c] = mean(dst_col.begin(), dst_col.end());
    //    }

    //    Matrix<double> src_demean = src;
    //    Matrix<double> dst_demean = dst;
    //    for (size_t c = 0; c < Dim; c++) {
    //        for (size_t r = 0; r < size; r++) {
    //            src_demean[r][c] -= mean_src[c];
    //            dst_demean[r][c] -= mean_dst[c];
    //        }
    //    }

    //    auto sigma = dst_demean.transpose() * src_demean / static_cast<double>(size);
    //    SingularValueDecomposition<Matrix<double>> svd(sigma);
    //    //transformMatrix = Matrix<double>::identity(Dim + 1, Dim + 1);
    //    Matrix<double, Dim, Dim> S = Matrix<double, Dim, Dim>::identity();
    //    if (sigma.determinant() < 0) {
    //        S[Dim - 1][Dim - 1] = -1;
    //    }

    //    if (sigma.rank() == Dim - 1) {
    //        if (svd.u().determinant() * svd.v().determinant() < 0)
    //            S[Dim - 1][Dim - 1] = -1;
    //    }

    //    auto block = this->transformMatrix.block(0, Dim - 1, 0, Dim - 1);
    //    block = svd.u() * S * svd.v().transpose();

    //    double src_var{};
    //    double module{};
    //    for (size_t c = 0; c < src_demean.cols(); c++) {
    //        auto vector = src_demean.col(c);
    //        module = vector.module();
    //        src_var += module * module;
    //    }

    //    src_var /= size;

    //    double scale = 1 / src_var * svd.w().dotProduct(S.diagonal());

    //    this->transformMatrix.col(Dim)[0] = mean_dst[0];
    //    this->transformMatrix.col(Dim)[1] = mean_dst[1];
    //    this->transformMatrix.col(Dim)[2] = mean_dst[2];

    //    this->transformMatrix.block(0, Dim - 1, 0, Dim - 1) *= scale;
    //    auto aux = this->transformMatrix.block(0, Dim - 1, 0, Dim - 1) * mean_src;
    //    this->transformMatrix.col(Dim)[0] -= aux[0];
    //    this->transformMatrix.col(Dim)[1] -= aux[1];
    //    this->transformMatrix.col(Dim)[2] -= aux[2];

    //}

    template<size_t rows, size_t cols>
    static Matrix<double, Dim + 1, Dim + 1> compute(const Matrix<T, rows, cols> &src,
                                                    const Matrix<T, rows, cols> &dst)
    {
        TL_ASSERT(src.cols() == Dim, "Invalid matrix columns size");
        TL_ASSERT(dst.cols() == Dim, "Invalid matrix columns size");
        TL_ASSERT(src.rows() == dst.rows(), "Different matrix sizes");

        size_t size = src.rows();
        Vector<double> mean_src(Dim);
        Vector<double> mean_dst(Dim);

        for (size_t c = 0; c < Dim; c++) {
            auto src_col = src.col(c);
            mean_src[c] = mean(src_col.begin(), src_col.end());
            auto dst_col = dst.col(c);
            mean_dst[c] = mean(dst_col.begin(), dst_col.end());
        }

        Matrix<double> src_demean = src;
        Matrix<double> dst_demean = dst;
        for (size_t c = 0; c < Dim; c++) {
            for (size_t r = 0; r < size; r++) {
                src_demean[r][c] -= mean_src[c];
                dst_demean[r][c] -= mean_dst[c];
            }
        }

        auto sigma = dst_demean.transpose() * src_demean / static_cast<double>(size);
        SingularValueDecomposition<Matrix<double>> svd(sigma);
        Matrix<double, Dim + 1, Dim + 1> transformMatrix = Matrix<double>::identity(Dim + 1, Dim + 1);
        Matrix<double, Dim, Dim> S = Matrix<double, Dim, Dim>::identity();
        if (sigma.determinant() < 0) {
            S[Dim - 1][Dim - 1] = -1;
        }

        if (sigma.rank() == Dim - 1) {
            if (svd.u().determinant() * svd.v().determinant() < 0)
                S[Dim - 1][Dim - 1] = -1;
        }

        auto block = transformMatrix.block(0, Dim - 1, 0, Dim - 1);
        block = svd.u() * S * svd.v().transpose();

        double src_var{};
        double module{};
        for (size_t c = 0; c < src_demean.cols(); c++) {
            auto vector = src_demean.col(c);
            module = vector.module();
            src_var += module * module;
        }

        src_var /= size;

        double scale = 1 / src_var * svd.w().dotProduct(S.diagonal());

        transformMatrix.col(Dim)[0] = mean_dst[0];
        transformMatrix.col(Dim)[1] = mean_dst[1];
        transformMatrix.col(Dim)[2] = mean_dst[2];

        transformMatrix.block(0, Dim - 1, 0, Dim - 1) *= scale;
        auto aux = transformMatrix.block(0, Dim - 1, 0, Dim - 1) * mean_src;
        transformMatrix.col(Dim)[0] -= aux[0];
        transformMatrix.col(Dim)[1] -= aux[1];
        transformMatrix.col(Dim)[2] -= aux[2];

        return transformMatrix;
    }
};

template <typename T>
class Affine2D
  : public Transform<T, 2>
{

public:

    Affine2D() {}

    Affine2D(T tx, T ty, T scaleX, T scaleY, T rotation)
    {
        this->transformMatrix(0, 0) = scaleX * cos(rotation);
        this->transformMatrix(0, 1) = -scaleX * sin(rotation);
        this->transformMatrix(1, 0) = scaleX * sin(rotation);
        this->transformMatrix(1, 0) = scaleX * cos(rotation);
        this->transformMatrix(0, 2) = tx;
        this->transformMatrix(1, 2) = ty;
    }

    Affine2D(const Matrix<double, 3, 3> &matrix) 
      : Transform<T, 2>(matrix)
    {
    }

    template<size_t rows, size_t cols>
    static Matrix<double, 3, 3> compute(const Matrix<T, rows, cols> &src,
                                        const Matrix<T, rows, cols> &dst)
    {
        TL_ASSERT(src.cols() == 2, "Invalid matrix columns size");
        TL_ASSERT(dst.cols() == 2, "Invalid matrix columns size");
        TL_ASSERT(src.rows() == dst.rows(), "Different matrix sizes");
        TL_ASSERT(src.rows() >= 3 , "Invalid number of points: {} < {}", src.rows(), 3);

        size_t size = src.rows() * 2;

        Matrix<double> A(size, 6, 0);
        Vector<double> B(size);

        for (size_t i = 0, r = 0; i < src.rows(); i++, r++) {
            A(r, 0) = src(i, 0);
            A(r, 1) = src(i, 1);
            A(r, 4) = 1;
            B[r] = dst(i,0);

            r++;
            
            A(r, 2) = src(i, 0);
            A(r, 3) = src(i, 1);
            A(r, 5) = 1;

            B[r] = dst(i,1);
        }

        SingularValueDecomposition<Matrix<double>> svd(A);
        Vector<double> C = svd.solve(B);

        Matrix<double, 3, 3> transformMatrix = Matrix<double>::identity(3, 3);
        transformMatrix(0, 0) = C[0];
        transformMatrix(0, 1) = C[1];
        transformMatrix(0, 2) = C[4];
        transformMatrix(1, 0) = C[2];
        transformMatrix(1, 1) = C[3];
        transformMatrix(1, 2) = C[5];

        return transformMatrix;
    }
};



template <typename T>
class Affine3D
  : public Transform<T, 3>
{

public:

    Affine3D() { }
    Affine3D(T tx, T ty, T tz, T scale, T omega, T phi, T kappa)
    { 
        EulerAngles<T> eulerAngles(omega, phi, kappa, EulerAngles<T>::Axes::xyz);
        RotationMatrix<T> rotation_matrix;
        RotationConverter<T>::convert(eulerAngles, rotation_matrix);
        this->transformMatrix.block(0, 2, 0, 2) = rotation_matrix * scale;
        this->transformMatrix(0, 3) = tx;
        this->transformMatrix(1, 3) = ty;
        this->transformMatrix(2, 3) = tz;
    }

    Affine3D(T tx, T ty, T tz, T scale, const RotationMatrix<T> &rotation)
    {
        this->transformMatrix.block(0, 2, 0, 2) = rotation * scale;
        this->transformMatrix(0, 3) = tx;
        this->transformMatrix(1, 3) = ty;
        this->transformMatrix(2, 3) = tz;
    }

    Affine3D(const Matrix<double, 4, 4> &matrix) 
      : Transform<T, 3>(matrix)
    {
    }

};


/*! \} */ // end of Algebra

/*! \} */ // end of Math

} // Fin namespace math

} // End namespace tl

