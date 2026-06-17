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

#pragma once

#include <cmath>
#include <algorithm>
#include <limits>

#include "tidop/math/numeric/Arithmetic.h"
#include "tidop/math/algebra/vector/Vector.h"
#include "tidop/math/algebra/rotations/EulerAnglesAxis.h"

namespace tl
{

template<typename T> class Quaternion;
template<typename T> class RotationMatrix;
template<typename T> class AxisAngle;
template<typename T, Axes Order> class EulerAngles;

namespace detail
{

template<typename T>
void convert(const RotationMatrix<T> &rotationMatrix, Quaternion<T> &quaternion)
{
    const T four{4};

    T r22 = rotationMatrix[2][2];
    if (r22 <= consts::zero<T>) {

        T r11_r00 = rotationMatrix[1][1] - rotationMatrix[0][0];
        if (r11_r00 <= consts::zero<T>) {
            quaternion.x() = sqrt((consts::one<T> -r22 - r11_r00) / four);
            T qx4 = quaternion.x() * four;
            quaternion.y() = (rotationMatrix[0][1] + rotationMatrix[1][0]) / qx4;
            quaternion.z() = (rotationMatrix[0][2] + rotationMatrix[2][0]) / qx4;
            quaternion.w() = (rotationMatrix[2][1] - rotationMatrix[1][2]) / qx4;
        } else {
            quaternion.y() = sqrt((consts::one<T>  -r22 + r11_r00) / four);
            T qy4 = quaternion.y() * four;
            quaternion.x() = (rotationMatrix[0][1] + rotationMatrix[1][0]) / qy4;
            quaternion.z() = (rotationMatrix[1][2] + rotationMatrix[2][1]) / qy4;
            quaternion.w() = (rotationMatrix[0][2] - rotationMatrix[2][0]) / qy4;
        }

    } else {

        T r11_r00 = rotationMatrix[1][1] + rotationMatrix[0][0];
        if (r11_r00 <= consts::zero<T>) {
            quaternion.z() = sqrt((consts::one<T>  +r22 - r11_r00) / four);
            T qz4 = quaternion.z() * four;
            quaternion.x() = (rotationMatrix[0][2] + rotationMatrix[2][0]) / qz4;
            quaternion.y() = (rotationMatrix[1][2] + rotationMatrix[2][1]) / qz4;
            quaternion.w() = (rotationMatrix[1][0] - rotationMatrix[0][1]) / qz4;
        } else {
            quaternion.w() = sqrt((consts::one<T>  +r22 + r11_r00) / four);
            T qw4 = quaternion.w() * four;
            quaternion.x() = (rotationMatrix[2][1] - rotationMatrix[1][2]) / qw4;
            quaternion.y() = (rotationMatrix[0][2] - rotationMatrix[2][0]) / qw4;
            quaternion.z() = (rotationMatrix[1][0] - rotationMatrix[0][1]) / qw4;
        }

    }
}

template<typename T>
void convert(const Quaternion<T> &quaternion, RotationMatrix<T> &rotationMatrix)
{
    T _2x = consts::two<T> *quaternion.x();
    T _2y = consts::two<T> *quaternion.y();
    T _2z = consts::two<T> *quaternion.z();
    T _2xx = _2x * quaternion.x();
    T _2xy = _2x * quaternion.y();
    T _2xz = _2x * quaternion.z();
    T _2xw = _2x * quaternion.w();
    T _2yy = _2y * quaternion.y();
    T _2yz = _2y * quaternion.z();
    T _2yw = _2y * quaternion.w();
    T _2zz = _2z * quaternion.z();
    T _2zw = _2z * quaternion.w();

    rotationMatrix[0][0] = consts::one<T> -_2yy - _2zz;
    rotationMatrix[0][1] = _2xy - _2zw;
    rotationMatrix[0][2] = _2xz + _2yw;
    rotationMatrix[1][0] = _2xy + _2zw;
    rotationMatrix[1][1] = consts::one<T> -_2xx - _2zz;
    rotationMatrix[1][2] = _2yz - _2xw;
    rotationMatrix[2][0] = _2xz - _2yw;
    rotationMatrix[2][1] = _2yz + _2xw;
    rotationMatrix[2][2] = consts::one<T> -_2xx - _2yy;
}

template<typename T>
void convert(const Quaternion<T> &quaternion, AxisAngle<T> &axisAngle)
{
    auto v = quaternion.imag();
    double m = v.module();
    if (m > std::numeric_limits<T>::epsilon()) {
        axisAngle.setVector(v / m);
        T angle = consts::two<T> *std::acos(tl::clamp(quaternion.w(), -consts::one<T>, consts::one<T>));
        axisAngle.setAngle(angle);
    } else {
        axisAngle.setVector({consts::one<T>, consts::zero<T>, consts::zero<T>});
        axisAngle.setAngle(consts::zero<T>);
    }
}

template<typename T>
void convert(const AxisAngle<T> &axisAngle, Quaternion<T> &quaternion)
{
    T a_2 = axisAngle.angle() * consts::one_half<T>;
    T sin_a_2 = std::sin(a_2);
    quaternion.imag() = sin_a_2 * axisAngle.vector();
    quaternion.w() = std::cos(a_2);
}

template<typename T>
void convert(const RotationMatrix<T> &rotationMatrix, AxisAngle<T> &axisAngle)
{
    Quaternion<T> quaternion;
    convert(rotationMatrix, quaternion);
    convert(quaternion, axisAngle);
}

template<typename T>
void convert(const AxisAngle<T> &axisAngle, RotationMatrix<T> &rotationMatrix)
{
    const auto &axis = axisAngle.vector();
    T axis_0 = axis[0];
    T axis_1 = axis[1];
    T axis_2 = axis[2];

    T ca = std::cos(axisAngle.angle());
    T sa = std::sin(axisAngle.angle());
    T _1mca = consts::one<T> -ca;
    T xy1mca = axis_0 * axis_1 * _1mca;
    T xz1mca = axis_0 * axis_2 * _1mca;
    T yz1mca = axis_1 * axis_2 * _1mca;
    T xsa = axis_0 * sa;
    T ysa = axis_1 * sa;
    T zsa = axis_2 * sa;

    rotationMatrix[0][0] = axis_0 * axis_0 * _1mca + ca;
    rotationMatrix[0][1] = xy1mca - zsa;
    rotationMatrix[0][2] = xz1mca + ysa;
    rotationMatrix[1][0] = xy1mca + zsa;
    rotationMatrix[1][1] = axis_1 * axis_1 * _1mca + ca;
    rotationMatrix[1][2] = yz1mca - xsa;
    rotationMatrix[2][0] = xz1mca - ysa;
    rotationMatrix[2][1] = yz1mca + xsa;
    rotationMatrix[2][2] = axis_2 * axis_2 * _1mca + ca;
}

template<typename T, Axes Order>
void convert(const RotationMatrix<T> &rotationMatrix, EulerAngles<T, Order> &eulerAngles)
{
    T m00 = rotationMatrix[0][0];
    T m01 = rotationMatrix[0][1];
    T m02 = rotationMatrix[0][2];
    T m10 = rotationMatrix[1][0];
    T m11 = rotationMatrix[1][1];
    T m12 = rotationMatrix[1][2];
    T m20 = rotationMatrix[2][0];
    T m21 = rotationMatrix[2][1];
    T m22 = rotationMatrix[2][2];

    // Tait-Bryan angles
    if constexpr (Order == Axes::xyz) {

        eulerAngles.y() = asin(tl::clamp(m02, -consts::one<T>, consts::one<T>));
        if (std::abs(m02) < consts::one<T>) {
            eulerAngles.x() = atan2(-m12, m22);
            eulerAngles.z() = atan2(-m01, m00);
        } else {
            eulerAngles.x() = atan2(m21, m11);
            eulerAngles.z() = consts::zero<T>;
        }

    } else if constexpr (Order == Axes::yxz) {

        eulerAngles.y() = asin(-tl::clamp(m12, -consts::one<T>, consts::one<T>));
        if (std::abs(m12) < consts::one<T>) {
            eulerAngles.x() = atan2(m02, m22);
            eulerAngles.z() = atan2(m10, m11);
        } else {
            eulerAngles.x() = atan2(-m20, m00);
            eulerAngles.z() = consts::zero<T>;
        }

    } else if constexpr (Order == Axes::zxy) {

        eulerAngles.y() = asin(clamp(m21, -consts::one<T>, consts::one<T>));

        if (std::abs(m21) < consts::one<T>) {

            eulerAngles.x() = atan2(-m01, m11);
            eulerAngles.z() = atan2(-m20, m22);

        } else {

            eulerAngles.x() = atan2(m10, m00);
            eulerAngles.z() = consts::zero<T>;

        }

    } else if constexpr (Order == Axes::zyx) {

        eulerAngles.y() = asin(-tl::clamp(m20, -consts::one<T>, consts::one<T>));

        if (std::abs(m20) < consts::one<T>) {

            eulerAngles.x() = atan2(m10, m00);
            eulerAngles.z() = atan2(m21, m22);

        } else {

            eulerAngles.x() = atan2(-m01, m11);
            eulerAngles.z() = consts::zero<T>;

        }

    } else if constexpr (Order == Axes::yzx) {

        eulerAngles.y() = asin(tl::clamp(m10, -consts::one<T>, consts::one<T>));

        if (std::abs(m10) < consts::one<T>) {

            eulerAngles.x() = atan2(-m20, m00);
            eulerAngles.z() = atan2(-m12, m11);

        } else {

            eulerAngles.x() = atan2(m02, m22);
            eulerAngles.z() = consts::zero<T>;

        }

    } else if constexpr (Order == Axes::xzy) {

        eulerAngles.y() = asin(-clamp(m01, -consts::one<T>, consts::one<T>));

        if (std::abs(m01) < consts::one<T>) {

            eulerAngles.x() = atan2(m21, m11);
            eulerAngles.z() = atan2(m02, m00);

        } else {

            eulerAngles.x() = atan2(-m12, m22);
            eulerAngles.z() = consts::zero<T>;

        }

    } else if constexpr (Order == Axes::xyx) {

        eulerAngles.y() = std::acos(tl::clamp(m00, -consts::one<T>, consts::one<T>));

        if (std::abs(m00) < consts::one<T>) {

            eulerAngles.x() = atan2(m10, -m20);
            eulerAngles.z() = atan2(m01, m02);

        } else {

            eulerAngles.x() = consts::zero<T>;
            eulerAngles.z() = atan2(-m12, m11);

        }

    } else if constexpr (Order == Axes::xzx) {

        eulerAngles.y() = std::acos(tl::clamp(m00, -consts::one<T>, consts::one<T>));

        if (std::abs(m00) < consts::one<T>) {

            eulerAngles.x() = atan2(m20, m10);
            eulerAngles.z() = atan2(m02, -m01);

        } else {

            eulerAngles.x() = consts::zero<T>;
            eulerAngles.z() = atan2(m21, m22);

        }

    } else if constexpr (Order == Axes::yxy) {

        eulerAngles.y() = std::acos(tl::clamp(m11, -consts::one<T>, consts::one<T>));

        if (std::abs(m11) < consts::one<T>) {
            eulerAngles.x() = atan2(m01, m21);
            eulerAngles.z() = atan2(m10, -m12);
        } else {

            eulerAngles.x() = consts::zero<T>;
            eulerAngles.z() = atan2(m02, m00);

        }

    } else if constexpr (Order == Axes::yzy) {

        eulerAngles.y() = std::acos(tl::clamp(m11, -consts::one<T>, consts::one<T>));

        if (std::abs(m11) < consts::one<T>) {

            eulerAngles.x() = atan2(m21, -m01);
            eulerAngles.z() = atan2(m12, m10);

        } else {

            eulerAngles.x() = consts::zero<T>;
            eulerAngles.z() = atan2(-m20, m22);

        }

    } else if constexpr (Order == Axes::zxz) {

        eulerAngles.y() = std::acos(tl::clamp(m22, -consts::one<T>, consts::one<T>));

        if (std::abs(m22) < consts::one<T>) {

            eulerAngles.x() = atan2(m02, -m12);
            eulerAngles.z() = atan2(m20, m21);

        } else {

            eulerAngles.x() = consts::zero<T>;
            eulerAngles.z() = std::atan2(-m01, m00);

        }

    } else if constexpr (Order == Axes::zyz) {

        eulerAngles.y() = std::acos(tl::clamp(m22, -consts::one<T>, consts::one<T>));

        if (std::abs(m22) < consts::one<T>) {

            eulerAngles.x() = atan2(m12, m02);
            eulerAngles.z() = atan2(m21, -m20);

        } else {

            eulerAngles.x() = consts::zero<T>;
            eulerAngles.z() = std::atan2(m10, m11);

        }

    }
}

template<typename T, Axes Order>
void convert(const EulerAngles<T, Order> &eulerAngles, RotationMatrix<T> &rotationMatrix)
{
    T c1 = cos(eulerAngles.x());
    T c2 = cos(eulerAngles.y());
    T c3 = cos(eulerAngles.z());

    T s1 = sin(eulerAngles.x());
    T s2 = sin(eulerAngles.y());
    T s3 = sin(eulerAngles.z());

    // Tait-Bryan angles
    if constexpr (Order == Axes::xyz) {

        rotationMatrix[0][0] = c2 * c3;
        rotationMatrix[0][1] = -c2 * s3;
        rotationMatrix[0][2] = s2;
        rotationMatrix[1][0] = c1 * s3 + c3 * s1 * s2;
        rotationMatrix[1][1] = c1 * c3 - s1 * s2 * s3;
        rotationMatrix[1][2] = -c2 * s1;
        rotationMatrix[2][0] = s1 * s3 - c1 * c3 * s2;
        rotationMatrix[2][1] = c3 * s1 + c1 * s2 * s3;
        rotationMatrix[2][2] = c1 * c2;

    } else if constexpr (Order == Axes::yxz) {

        rotationMatrix[0][0] = c1 * c3 + s1 * s2 * s3;
        rotationMatrix[0][1] = c3 * s1 * s2 - c1 * s3;
        rotationMatrix[0][2] = c2 * s1;
        rotationMatrix[1][0] = c2 * s3;
        rotationMatrix[1][1] = c2 * c3;
        rotationMatrix[1][2] = -s2;
        rotationMatrix[2][0] = c1 * s2 * s3 - c3 * s1;
        rotationMatrix[2][1] = c1 * c3 * s2 + s1 * s3;
        rotationMatrix[2][2] = c1 * c2;

    } else if constexpr (Order == Axes::zxy) {

        rotationMatrix[0][0] = c1 * c3 - s1 * s2 * s3;
        rotationMatrix[0][1] = -s1 * c2;
        rotationMatrix[0][2] = c1 * s3 + s1 * s2 * c3;
        rotationMatrix[1][0] = s1 * c3 + c1 * s2 * s3;
        rotationMatrix[1][1] = c1 * c2;
        rotationMatrix[1][2] = s1 * s3 - c1 * s2 * c3;
        rotationMatrix[2][0] = -c2 * s3;
        rotationMatrix[2][1] = s2;
        rotationMatrix[2][2] = c2 * c3;

    } else if constexpr (Order == Axes::zyx) {

        rotationMatrix[0][0] = c1 * c2;
        rotationMatrix[0][1] = c1 * s2 * s3 - c3 * s1;
        rotationMatrix[0][2] = s1 * s3 + c1 * c3 * s2;
        rotationMatrix[1][0] = c2 * s1;
        rotationMatrix[1][1] = c1 * c3 + s1 * s2 * s3;
        rotationMatrix[1][2] = c3 * s1 * s2 - c1 * s3;
        rotationMatrix[2][0] = -s2;
        rotationMatrix[2][1] = c2 * s3;
        rotationMatrix[2][2] = c2 * c3;

    } else if constexpr (Order == Axes::yzx) {

        rotationMatrix[0][0] = c1 * c2;
        rotationMatrix[0][1] = s1 * s3 - c1 * s2 * c3;
        rotationMatrix[0][2] = c1 * s2 * s3 + s1 * c3;
        rotationMatrix[1][0] = s2;
        rotationMatrix[1][1] = c2 * c3;
        rotationMatrix[1][2] = -c2 * s3;
        rotationMatrix[2][0] = -s1 * c2;
        rotationMatrix[2][1] = c1 * s3 + s1 * s2 * c3;
        rotationMatrix[2][2] = c1 * c3 - s1 * s2 * s3;

    } else if constexpr (Order == Axes::xzy) {

        rotationMatrix[0][0] = c2 * c3;
        rotationMatrix[0][1] = -s2;
        rotationMatrix[0][2] = c2 * s3;
        rotationMatrix[1][0] = c1 * s2 * c3 + s1 * s3;
        rotationMatrix[1][1] = c1 * c2;
        rotationMatrix[1][2] = c1 * s2 * s3 - s1 * c3;
        rotationMatrix[2][0] = s1 * s2 * c3 - c1 * s3;
        rotationMatrix[2][1] = s1 * c2;
        rotationMatrix[2][2] = s1 * s2 * s3 + c1 * c3;

    }
    // Euler angles
    else if constexpr (Order == Axes::xyx) {

        rotationMatrix[0][0] = c2;
        rotationMatrix[0][1] = s2 * s3;
        rotationMatrix[0][2] = s2 * c3;
        rotationMatrix[1][0] = s1 * s2;
        rotationMatrix[1][1] = c1 * c3 - s1 * c2 * s3;
        rotationMatrix[1][2] = -c1 * s3 - s1 * c2 * c3;
        rotationMatrix[2][0] = -c1 * s2;
        rotationMatrix[2][1] = s1 * c3 + c1 * c2 * s3;
        rotationMatrix[2][2] = c1 * c2 * c3 - s1 * s3;

    } else if constexpr (Order == Axes::xzx) {

        rotationMatrix[0][0] = c2;
        rotationMatrix[0][1] = -c3 * s2;
        rotationMatrix[0][2] = s2 * s3;
        rotationMatrix[1][0] = c1 * s2;
        rotationMatrix[1][1] = c1 * c2 * c3 - s1 * s3;
        rotationMatrix[1][2] = -c3 * s1 - c1 * c2 * s3;
        rotationMatrix[2][0] = s1 * s2;
        rotationMatrix[2][1] = c1 * s3 + c2 * c3 * s1;
        rotationMatrix[2][2] = c1 * c3 - c2 * s1 * s3;

    } else if constexpr (Order == Axes::yxy) {

        rotationMatrix[0][0] = c1 * c3 - c2 * s1 * s3;
        rotationMatrix[0][1] = s1 * s2;
        rotationMatrix[0][2] = c1 * s3 + c2 * c3 * s1;
        rotationMatrix[1][0] = s2 * s3;
        rotationMatrix[1][1] = c2;
        rotationMatrix[1][2] = -c3 * s2;
        rotationMatrix[2][0] = -c3 * s1 - c1 * c2 * s3;
        rotationMatrix[2][1] = c1 * s2;
        rotationMatrix[2][2] = c1 * c2 * c3 - s1 * s3;

    } else if constexpr (Order == Axes::yzy) {

        rotationMatrix[0][0] = c1 * c2 * c3 - s1 * s3;
        rotationMatrix[0][1] = -c1 * s2;
        rotationMatrix[0][2] = c3 * s1 + c1 * c2 * s3;
        rotationMatrix[1][0] = c3 * s2;
        rotationMatrix[1][1] = c2;
        rotationMatrix[1][2] = s2 * s3;
        rotationMatrix[2][0] = -c1 * s3 - c2 * c3 * s1;
        rotationMatrix[2][1] = s1 * s2;
        rotationMatrix[2][2] = c1 * c3 - c2 * s1 * s3;

    } else if constexpr (Order == Axes::zxz) {

        rotationMatrix[0][0] = c1 * c3 - c2 * s1 * s3;
        rotationMatrix[0][1] = -c1 * s3 - c2 * c3 * s1;
        rotationMatrix[0][2] = s1 * s2;
        rotationMatrix[1][0] = c3 * s1 + c1 * c2 * s3;
        rotationMatrix[1][1] = c1 * c2 * c3 - s1 * s3;
        rotationMatrix[1][2] = -c1 * s2;
        rotationMatrix[2][0] = s2 * s3;
        rotationMatrix[2][1] = c3 * s2;
        rotationMatrix[2][2] = c2;

    } else if constexpr (Order == Axes::zyz) {

        rotationMatrix[0][0] = c1 * c2 * c3 - s1 * s3;
        rotationMatrix[0][1] = -c3 * s1 - c1 * c2 * s3;
        rotationMatrix[0][2] = c1 * s2;
        rotationMatrix[1][0] = c1 * s3 + c2 * c3 * s1;
        rotationMatrix[1][1] = c1 * c3 - c2 * s1 * s3;
        rotationMatrix[1][2] = s1 * s2;
        rotationMatrix[2][0] = -c3 * s2;
        rotationMatrix[2][1] = s2 * s3;
        rotationMatrix[2][2] = c2;

    }
}

template<typename T, Axes Order>
void convert(const Quaternion<T> &quaternion, EulerAngles<T, Order> &eulerAngles)
{
    RotationMatrix<T> rotationMatrix;
    convert(quaternion, rotationMatrix);
    convert(rotationMatrix, eulerAngles);
}

template<typename T, Axes Order>
void convert(const EulerAngles<T, Order> &eulerAngles, Quaternion<T> &quaternion)
{
    T x = eulerAngles.x();
    T y = eulerAngles.y();
    T z = eulerAngles.z();

    T x_half = x * consts::one_half<T>;
    T y_half = y * consts::one_half<T>;
    T z_half = z * consts::one_half<T>;

    T c1 = cos(x_half);
    T c2 = cos(y_half);
    T c3 = cos(z_half);

    T s1 = sin(x_half);
    T s2 = sin(y_half);
    T s3 = sin(z_half);

    // Tait-Bryan angles
    if constexpr (Order == Axes::xyz) {

        quaternion.x() = s1 * c2 * c3 + c1 * s2 * s3;
        quaternion.y() = c1 * s2 * c3 - s1 * c2 * s3;
        quaternion.z() = c1 * c2 * s3 + s1 * s2 * c3;
        quaternion.w() = c1 * c2 * c3 - s1 * s2 * s3;

    } else if constexpr (Order == Axes::yxz) {

        quaternion.x() = s1 * s3 * c2 + s2 * c1 * c3;
        quaternion.y() = s1 * c2 * c3 - s2 * s3 * c1;
        quaternion.z() = c1 * c2 * s3 - s1 * s2 * c3;
        quaternion.w() = c1 * c2 * c3 + s1 * s2 * s3;

    } else if constexpr (Order == Axes::zxy) {

        quaternion.x() = s2 * c1 * c3 - s1 * s3 * c2;
        quaternion.y() = s1 * s2 * c3 + s3 * c1 * c2;
        quaternion.z() = s1 * c2 * c3 + s2 * s3 * c1;
        quaternion.w() = c1 * c2 * c3 - s1 * s2 * s3;

    } else if constexpr (Order == Axes::zyx) {

        quaternion.x() = s3 * c1 * c2 - s1 * s2 * c3;
        quaternion.y() = s1 * s3 * c2 + s2 * c1 * c3;
        quaternion.z() = s1 * c2 * c3 - s2 * s3 * c1;
        quaternion.w() = c1 * c2 * c3 + s1 * s2 * s3;

    } else if constexpr (Order == Axes::yzx) {

        quaternion.x() = s1 * s2 * c3 + s3 * c1 * c2;
        quaternion.y() = s1 * c2 * c3 + s2 * s3 * c1;
        quaternion.z() = s2 * c1 * c3 - s1 * s3 * c2;
        quaternion.w() = c1 * c2 * c3 - s1 * s2 * s3;

    } else if constexpr (Order == Axes::xzy) {

        quaternion.x() = s1 * c2 * c3 - s2 * s3 * c1;
        quaternion.y() = s3 * c1 * c2 - s1 * s2 * c3;
        quaternion.z() = s1 * s3 * c2 + s2 * c1 * c3;
        quaternion.w() = s1 * s2 * s3 + c1 * c2 * c3;

    }
    // Euler angles
    else if constexpr (Order == Axes::xyx) {

        quaternion.x() = c2 * sin((x + z) * consts::one_half<T>);
        quaternion.y() = s2 * cos((x - z) * consts::one_half<T>);
        quaternion.z() = s2 * sin((x - z) * consts::one_half<T>);
        quaternion.w() = c2 * cos((x + z) * consts::one_half<T>);

    } else if constexpr (Order == Axes::xzx) {

        quaternion.x() = c2 * sin((x + z) * consts::one_half<T>);
        quaternion.y() = -s2 * sin((x - z) * consts::one_half<T>);
        quaternion.z() = s2 * cos((x - z) * consts::one_half<T>);
        quaternion.w() = c2 * cos((x + z) * consts::one_half<T>);

    } else if constexpr (Order == Axes::yxy) {

        quaternion.x() = s2 * cos((x - z) * consts::one_half<T>);
        quaternion.y() = c2 * sin((x + z) * consts::one_half<T>);
        quaternion.z() = -s2 * sin((x - z) * consts::one_half<T>);
        quaternion.w() = c2 * cos((x + z) * consts::one_half<T>);

    } else if constexpr (Order == Axes::yzy) {

        quaternion.x() = s2 * sin((x - z) * consts::one_half<T>);
        quaternion.y() = c2 * sin((x + z) * consts::one_half<T>);
        quaternion.z() = s2 * cos((x - z) * consts::one_half<T>);
        quaternion.w() = c2 * cos((x + z) * consts::one_half<T>);

    } else if constexpr (Order == Axes::zxz) {

        quaternion.x() = s2 * cos((x - z) * consts::one_half<T>);
        quaternion.y() = s2 * sin((x - z) * consts::one_half<T>);
        quaternion.z() = c2 * sin((x + z) * consts::one_half<T>);
        quaternion.w() = c2 * cos((x + z) * consts::one_half<T>);

    } else if constexpr (Order == Axes::zyz) {

        quaternion.x() = -s2 * sin((x - z) * consts::one_half<T>);
        quaternion.y() = s2 * cos((x - z) * consts::one_half<T>);
        quaternion.z() = c2 * sin((x + z) * consts::one_half<T>);
        quaternion.w() = c2 * cos((x + z) * consts::one_half<T>);

    }
}

template<typename T, Axes Order>
void convert(const EulerAngles<T, Order> &eulerAngles, AxisAngle<T> &axisAngle)
{
    Quaternion<T> quaternion;
    convert(eulerAngles, quaternion);
    convert(quaternion, axisAngle);
}

template<typename T, Axes Order>
void convert(const AxisAngle<T> &axisAngle, EulerAngles<T, Order> &eulerAngles)
{
    RotationMatrix<T> rotationMatrix;
    convert(axisAngle, rotationMatrix);
    convert(rotationMatrix, eulerAngles);
}

template<typename T, Axes Order1, Axes Order2>
void convert(const EulerAngles<T, Order1> &src, EulerAngles<T, Order2> &dst)
{
    if constexpr (Order1 == Order2) {
        dst = src;
    } else {
        RotationMatrix<T> m;
        convert(src, m);
        convert(m, dst);
    }
}

} // namespace detail

} // namespace tl
