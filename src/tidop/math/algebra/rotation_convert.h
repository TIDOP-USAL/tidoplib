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

#include "tidop/math/math.h"
#include "tidop/math/algebra/quaternion.h"
#include "tidop/math/algebra/euler_angles.h"
#include "tidop/math/algebra/rotation_matrix.h"
#include "tidop/math/algebra/axis_angle.h"

namespace tl
{

/*! \addtogroup math
 *  \{
 */

/*! \addtogroup algebra
 *  \{
 */

/*! \defgroup rotations Rotations
 *  \{
 */

/*!
 * \brief Clase para convertir entre diferentes sistemas de rotación
 */
template <typename T>
class RotationConverter
{

private:

    RotationConverter() = default;

public:

    /*!
     * \brief Convierte una matriz de rotación a cuaterniones
     * \f[ x^2 = \frac{+r_{00} - r_{11} - r_{22} + 1}{4}  \f]
     * \f[ y^2 = \frac{-r_{00} + r_{11} - r_{22} + 1}{4}  \f]
     * \f[ z^2 = \frac{-r_{00} - r_{11} + r_{22} + 1}{4}  \f]
     * \f[ w^2 = \frac{+r_{00} + r_{11} + r_{22} + 1}{4}  \f]
     * \f[ x^2 + y^2 = \frac{1 - r_{22}}{2} \f]
     * \f[ z^2 + w^2 = \frac{1 + r_{22}}{2} \f]
     * \f[ y^2 - x^2 = \frac{r_{11} - r_{00}}{2} \f]
     * \f[ w^2 - z^2 = \frac{r_{11} + r_{00}}{2} \f]
     * \f[ x*y = \frac{r_{01} + r_{10}}{4} \f]
     * \f[ x*z = \frac{r_{02} + r_{20}}{4} \f]
     * \f[ y*z = \frac{r_{12} + r_{21}}{4} \f]
     * \f[ x*w = \frac{r_{21} - r_{12}}{4} \f]
     * \f[ y*w = \frac{r_{02} - r_{20}}{4} \f]
     * \f[ z*w = \frac{r_{10} - r_{01}}{4} \f]
     *
     * <h4>Ejemplo</h4>
     * \code
     *   RotationMatrix<double> rotationMatrix;
     *   rot[0][0] = -0.8888889;
     *   rot[0][1] = 0.4444444;
     *   rot[0][2] = -0.1111111;
     *   rot[1][0] = -0.1111111;
     *   rot[1][1] = -0.4444444;
     *   rot[1][2] = -0.8888889;
     *   rot[2][0] = -0.4444444;
     *   rot[2][1] = -0.7777778;
     *   rot[2][2] = 0.4444444;
     *   Quaterniond quaternion;
     *   RotationConverter<double>::convert(rotationMatrix, quaternion);
     * \endcode
     *
     * \param[in] rotationMatrix Matriz de rotación
     * \param[out] quaternion Rotación como cuaterniones
     */
    static void convert(const RotationMatrix<T> &rotationMatrix,
                        Quaternion<T> &quaternion);

    /*!
     * \brief Convierte una rotación como cuaterniones a matriz de rotación
     *
     * \f[ r_{00} = 1 - 2 * y² - 2 * z \f]
     * \f[ r_{01} = 2 * x * y - 2 * z * w  \f]
     * \f[ r_{02} = 2 * x * z + 2 * y * w \f]
     * \f[ r_{10} = 2 * x * y + 2 * z * w \f]
     * \f[ r_{11} = 1 - 2 * x² - 2 * z² \f]
     * \f[ r_{12} = 2 * y * z - 2 * x * w \f]
     * \f[ r_{20} = 2 * x * z - 2 * y * w \f]
     * \f[ r_{21} = 2 * y * z + 2 * x * w \f]
     * \f[ r_{22} = 1 - 2 * x² - 2 * y² \f]
     *
     * <h4>Ejemplo</h4>
     * \code
     *   Quaterniond quaternion(0., 1., -3., 2.);
     *   RotationMatrix<double> rotationMatrix;
     *   RotationConverter<double>::convert(quaternion, rotationMatrix);
     * \endcode
     *
     * \param[in] quaternion Rotación como cuaterniones
     * \param[out] rotationMatrix Matriz de rotación
     */
    static void convert(const Quaternion<T> &quaternion,
                        RotationMatrix<T> &rotationMatrix);

    /*!
     * \brief Convierte una rotación como coordendas a notación axial-angular
     *
     * <h4>Ejemplo</h4>
     * \code
     *   Quaterniond quaternion(0., 1., -3., 2.);
     *   AxisAngle<double> axis_angle;
     *   RotationConverter<double>::convert(quaternion, axis_angle);
     * \endcode
     *
     * \param[in] quaternion Rotación como cuaterniones
     * \param[out] axisAngle Rotación en notación axial-angular
     */
    static void convert(const Quaternion<T> &quaternion,
                        AxisAngle<T> &axisAngle);

    /*!
     * \brief Convierte una rotación en notación axial-angular a quaterniones
     *
     * <h4>Ejemplo</h4>
     * \code
     *   AxisAngle<double> axis_angle(3.4764888, {-0.1690308, -0.5070926, 0.8451542});
     *   Quaterniond quaternion;
     *   RotationConverter<double>::convert(axis_angle, quaternion);
     * \endcode
     *
     * \param[in] axisAngle Rotación en notación axial-angular
     * \param[out] quaternion Rotación como cuaterniones
     */
    static void convert(const AxisAngle<T> &axisAngle,
                        Quaternion<T> &quaternion);

    /*!
     * Convierte una matriz de rotacion a notación axial-angular
     *
     * <h4>Ejemplo</h4>
     * \code
     *   RotationMatrix<double> rotationMatrix;
     *   rot[0][0] = -0.8888889;
     *   rot[0][1] = 0.4444444;
     *   rot[0][2] = -0.1111111;
     *   rot[1][0] = -0.1111111;
     *   rot[1][1] = -0.4444444;
     *   rot[1][2] = -0.8888889;
     *   rot[2][0] = -0.4444444;
     *   rot[2][1] = -0.7777778;
     *   rot[2][2] = 0.4444444;
     *   AxisAngle<double> axis_angle;
     *   RotationConverter<double>::convert(rotationMatrix, axis_angle);
     * \endcode
     *
     * \param[in] rotationMatrix Matriz de rotación
     * \param[out] axisAngle Rotación en notación axial-angular
     */
    static void convert(const RotationMatrix<T> &rotationMatrix,
                        AxisAngle<T> &axisAngle);

    /*!
     * \brief Convierte una rotación en notación axial-angular a una matriz de rotación
     *
     * <h4>Ejemplo</h4>
     * \code
     *   AxisAngle<double> axis_angle(3.4764888, {-0.1690308, -0.5070926, 0.8451542});
     *   RotationMatrix<double> rotationMatrix;
     *   RotationConverter<double>::convert(axis_angle, rotationMatrix);
     * \endcode
     *
     * \param[in] axisAngle Rotación en notación axial-angular
     * \param[out] rotationMatrix Matriz de rotación
     */
    static void convert(const AxisAngle<T> &axisAngle,
                        RotationMatrix<T> &rotationMatrix);

    /*!
     * Convierte una matriz de rotacion a ángulos de Euler
     *
     * <h4>Ejemplo</h4>
     * \code
     *   RotationMatrix<double> rotationMatrix;
     *   rot[0][0] = -0.8888889;
     *   rot[0][1] = 0.4444444;
     *   rot[0][2] = -0.1111111;
     *   rot[1][0] = -0.1111111;
     *   rot[1][1] = -0.4444444;
     *   rot[1][2] = -0.8888889;
     *   rot[2][0] = -0.4444444;
     *   rot[2][1] = -0.7777778;
     *   rot[2][2] = 0.4444444;
     *   EulerAngles<double> eulerAngles;
     *   RotationConverter<double>::convert(rotationMatrix, eulerAngles);
     * \endcode
     *
     * \param[in] rotationMatrix Matriz de rotación
     * \param[out] axisAngle Rotación en notación axial-angular
     */
    static void convert(const RotationMatrix<T> &rotationMatrix,
                        EulerAngles<T> &eulerAngles);

    /*!
     * \brief Convierte una rotación como ángulos de Euler a matriz de rotación
     * https://ntrs.nasa.gov/archive/nasa/casi.ntrs.nasa.gov/19770024290.pdf
     * <h4>Ejemplo</h4>
     * \code
     * EulerAngles<double> eulerAngles(2.356194490192345,
     *                                 2.5261129449194057,
     *                                 0.7853981633974483,
     *                                 EulerAngles<double>::Axes::xyz);
     * RotationMatrix<double> rotationMatrix;
     * RotationConverter<double>::convert(eulerAngles, rotationMatrix);
     * \endcode
     * \param[in] eulerAngles Rotación como ángulos de euler
     * \param[out] rotationMatrix Matriz de rotación
     */
    static void convert(const EulerAngles<T> &eulerAngles,
                        RotationMatrix<T> &rotationMatrix);

    /*!
     * \brief Convierte una rotación como cuaterniones a ángulos de Euler
     * <h4>Ejemplo</h4>
     * \code
     * Quaterniond quaternion(0., 1., -3., 2.);
     * EulerAngles<double> eulerAngles;
     * RotationConverter<double>::convert(quaternion, eulerAngles);
     * \endcode
     * \param[in] quaternion Rotación como cuaterniones
     * \param[out] eulerAngles Rotación como ángulos de Euler
     */
    static void convert(const Quaternion<T> &quaternion,
                        EulerAngles<T> &eulerAngles);

    /*!
     * \brief Convierte una rotación como ángulos de Euler a cuaterniones
     * https://ntrs.nasa.gov/archive/nasa/casi.ntrs.nasa.gov/19770024290.pdf
     * <h4>Ejemplo</h4>
     * \code
     * EulerAngles<double> eulerAngles(2.356194490192345,
     *                                 2.5261129449194057,
     *                                 0.7853981633974483,
     *                                 EulerAngles<double>::Axes::xyz);
     * Quaterniond quaternion;
     * RotationConverter<double>::convert(eulerAngles, quaternion);
     * \endcode
     * \param[in] eulerAngles Rotación como ángulos de euler
     * \param[out] quaternion Rotación como cuaterniones
     */
    static void convert(const EulerAngles<T> &eulerAngles,
                        Quaternion<T> &quaternion);

    /*!
     * \brief Convierte una rotación como ángulos de Euler a notación axial-angular
     *
     * <h4>Ejemplo</h4>
     * \code
     * EulerAngles<double> eulerAngles(2.356194490192345,
     *                                 2.5261129449194057,
     *                                 0.7853981633974483,
     *                                 EulerAngles<double>::Axes::xyz);
     * AxisAngle<double> axisAngle;
     * RotationConverter<double>::convert(eulerAngles, axisAngle);
     * \endcode
     * \param[in] eulerAngles Rotación como ángulos de euler
     * \param[out] axisAngle Rotación en notación axial-angular
     */
    static void convert(const EulerAngles<T> &eulerAngles,
                        AxisAngle<T> &axisAngle);

    /*!
     * \brief Convierte una rotación en notación axial-angular a ángulos de Euler
     *
     * <h4>Ejemplo</h4>
     * \code
     *   AxisAngle<double> axis_angle(3.4764888, {-0.1690308, -0.5070926, 0.8451542});
     *   EulerAngles<double> eulerAngles;
     *   RotationConverter<double>::convert(axis_angle, eulerAngles);
     * \endcode
     *
     * \param[in] axisAngle Rotación en notación axial-angular
     * \param[out] eulerAngles Rotación como ángulos de euler
     */
    static void convert(const AxisAngle<T> &axisAngle,
                        EulerAngles<T> &eulerAngles);

};




/// Implementación RotationConverter


template<typename T>
void RotationConverter<T>::convert(const RotationMatrix<T> &rotationMatrix,
                                   Quaternion<T> &quaternion)
{
    const T four{4};

    T r22 = rotationMatrix[2][2];
    if (r22 <= consts::zero<T>) {

        T r11_r00 = rotationMatrix[1][1] - rotationMatrix[0][0];
        if (r11_r00 <= consts::zero<T>) {
            quaternion.x = sqrt((consts::one<T> -r22 - r11_r00) / four);
            T qx4 = quaternion.x * four;
            quaternion.y = (rotationMatrix[0][1] + rotationMatrix[1][0]) / qx4;
            quaternion.z = (rotationMatrix[0][2] + rotationMatrix[2][0]) / qx4;
            quaternion.w = (rotationMatrix[2][1] - rotationMatrix[1][2]) / qx4;
        } else {
            quaternion.y = sqrt((consts::one<T>  -r22 + r11_r00) / four);
            T qy4 = quaternion.y * four;
            quaternion.x = (rotationMatrix[0][1] + rotationMatrix[1][0]) / qy4;
            quaternion.z = (rotationMatrix[1][2] + rotationMatrix[2][1]) / qy4;
            quaternion.w = (rotationMatrix[0][2] - rotationMatrix[2][0]) / qy4;
        }

    } else {

        T r11_r00 = rotationMatrix[1][1] + rotationMatrix[0][0];
        if (r11_r00 <= consts::zero<T>) {
            quaternion.z = sqrt((consts::one<T>  +r22 - r11_r00) / four);
            T qz4 = quaternion.z * four;
            quaternion.x = (rotationMatrix[0][2] + rotationMatrix[2][0]) / qz4;
            quaternion.y = (rotationMatrix[1][2] + rotationMatrix[2][1]) / qz4;
            quaternion.w = (rotationMatrix[1][0] - rotationMatrix[0][1]) / qz4;
        } else {
            quaternion.w = sqrt((consts::one<T>  +r22 + r11_r00) / four);
            T qw4 = quaternion.w * four;
            quaternion.x = (rotationMatrix[2][1] - rotationMatrix[1][2]) / qw4;
            quaternion.y = (rotationMatrix[0][2] - rotationMatrix[2][0]) / qw4;
            quaternion.z = (rotationMatrix[1][0] - rotationMatrix[0][1]) / qw4;
        }

    }
}


template<typename T>
void RotationConverter<T>::convert(const Quaternion<T> &quaternion,
                                   RotationMatrix<T> &rotationMatrix)
{
    rotationMatrix = RotationMatrix<T>::identity();

    T _2x = consts::two<T> *quaternion.x;
    T _2y = consts::two<T> *quaternion.y;
    T _2z = consts::two<T> *quaternion.z;
    T _2xx = _2x * quaternion.x;
    T _2xy = _2x * quaternion.y;
    T _2xz = _2x * quaternion.z;
    T _2xw = _2x * quaternion.w;
    T _2yy = _2y * quaternion.y;
    T _2yz = _2y * quaternion.z;
    T _2yw = _2y * quaternion.w;
    T _2zz = _2z * quaternion.z;
    T _2zw = _2z * quaternion.w;

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
void RotationConverter<T>::convert(const Quaternion<T> &quaternion,
                                   AxisAngle<T> &axisAngle)
{
    T n2 = std::sqrt(quaternion.x * quaternion.x + quaternion.y * quaternion.y + quaternion.z * quaternion.z);
    if (n2 > consts::zero<T>) {
        Vector<T, 3> axis;
        axis[0] = quaternion.x / n2;
        axis[1] = quaternion.y / n2;
        axis[2] = quaternion.z / n2;
        axisAngle.setAxis(axis);
        T angle = consts::two<T> *std::acos(tl::clamp(quaternion.w, -consts::one<T>, consts::one<T>));
        axisAngle.setAngle(angle);
    } else {
        axisAngle.setAxis({consts::one<T>, consts::zero<T>, consts::zero<T>});
        axisAngle.setAngle(consts::zero<T>);
    }
}

template<typename T>
void RotationConverter<T>::convert(const AxisAngle<T> &axisAngle,
                                   Quaternion<T> &quaternion)
{
    T a_2 = axisAngle.angle() * consts::one_half<T>;
    T sin_a_2 = std::sin(a_2);
    quaternion.x = sin_a_2 * axisAngle.axis(0);
    quaternion.y = sin_a_2 * axisAngle.axis(1);
    quaternion.z = sin_a_2 * axisAngle.axis(2);
    quaternion.w = std::cos(a_2);
}

template<typename T>
void RotationConverter<T>::convert(const RotationMatrix<T> &rotationMatrix,
                                   AxisAngle<T> &axisAngle)
{
    Quaternion<T> quaternion;
    convert(rotationMatrix, quaternion);
    convert(quaternion, axisAngle);
}

template<typename T>
void RotationConverter<T>::convert(const AxisAngle<T> &axisAngle,
                                   RotationMatrix<T> &rotationMatrix)
{
    rotationMatrix = RotationMatrix<T>::identity();

    Vector<T, 3> axis = axisAngle.axis();
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



template<typename T>
void RotationConverter<T>::convert(const RotationMatrix<T> &rotationMatrix,
                                   EulerAngles<T> &eulerAngles)
{

    typename EulerAngles<T>::Axes axes = eulerAngles.axes;

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
    if (axes == EulerAngles<T>::Axes::xyz) {

        eulerAngles.y = asin(tl::clamp(m02, -consts::one<T>, consts::one<T>));
        if (std::abs(m02) < consts::one<T>) {
            eulerAngles.x = atan2(-m12, m22);
            eulerAngles.z = atan2(-m01, m00);
        } else {
            eulerAngles.x = atan2(m21, m11);
            eulerAngles.z = consts::zero<T>;
        }

    } else if (axes == EulerAngles<T>::Axes::yxz) {

        eulerAngles.y = asin(-tl::clamp(m12, -consts::one<T>, consts::one<T>));
        if (std::abs(m12) < consts::one<T>) {
            eulerAngles.x = atan2(m02, m22);
            eulerAngles.z = atan2(m10, m11);
        } else {
            eulerAngles.x = atan2(-m20, m00);
            eulerAngles.z = consts::zero<T>;
        }

    } else if (axes == EulerAngles<T>::Axes::zxy) {


        eulerAngles.y = asin(clamp(m21, -consts::one<T>, consts::one<T>));

        if (std::abs(m21) < consts::one<T>) {

            eulerAngles.x = atan2(-m01, m11);
            eulerAngles.z = atan2(-m20, m22);

        } else {

            eulerAngles.x = atan2(m10, m00);
            eulerAngles.z = consts::zero<T>;

        }

    } else if (axes == EulerAngles<T>::Axes::zyx) {

        eulerAngles.y = asin(-tl::clamp(m20, -consts::one<T>, consts::one<T>));

        if (std::abs(m20) < consts::one<T>) {

            eulerAngles.x = atan2(m10, m00);
            eulerAngles.z = atan2(m21, m22);

        } else {

            eulerAngles.x = atan2(-m01, m11);
            eulerAngles.z = consts::zero<T>;

        }

    } else if (axes == EulerAngles<T>::Axes::yzx) {

        eulerAngles.y = asin(tl::clamp(m10, -consts::one<T>, consts::one<T>));

        if (std::abs(m10) < consts::one<T>) {

            eulerAngles.x = atan2(-m20, m00);
            eulerAngles.z = atan2(-m12, m11);

        } else {

            eulerAngles.x = atan2(m02, m22);
            eulerAngles.z = consts::zero<T>;

        }

    } else if (axes == EulerAngles<T>::Axes::xzy) {

        eulerAngles.y = asin(-clamp(m01, -consts::one<T>, consts::one<T>));

        if (std::abs(m01) < consts::one<T>) {

            eulerAngles.x = atan2(m21, m11);
            eulerAngles.z = atan2(m02, m00);

        } else {

            eulerAngles.x = atan2(-m12, m22);
            eulerAngles.z = consts::zero<T>;

        }

    } else if (axes == EulerAngles<T>::Axes::xyx) {

        eulerAngles.y = std::acos(tl::clamp(m00, -consts::one<T>, consts::one<T>));

        if (std::abs(m00) < consts::one<T>) {

            eulerAngles.x = atan2(m10, -m20);
            eulerAngles.z = atan2(m01, m02);

        } else {

            eulerAngles.x = consts::zero<T>;
            eulerAngles.z = atan2(-m12, m11);

        }

    } else if (axes == EulerAngles<T>::Axes::xzx) {

        eulerAngles.y = std::acos(tl::clamp(m00, -consts::one<T>, consts::one<T>));

        if (std::abs(m00) < consts::one<T>) {

            eulerAngles.x = atan2(m20, m10);
            eulerAngles.z = atan2(m02, -m01);

        } else {

            eulerAngles.x = consts::zero<T>;
            eulerAngles.z = atan2(m21, m22);

        }

    } else if (axes == EulerAngles<T>::Axes::yxy) {

        eulerAngles.y = std::acos(tl::clamp(m11, -consts::one<T>, consts::one<T>));

        if (std::abs(m11) < consts::one<T>) {
            eulerAngles.x = atan2(m01, m21);
            eulerAngles.z = atan2(m10, -m12);
        } else {

            eulerAngles.x = consts::zero<T>;
            eulerAngles.z = atan2(m02, m00);

        }

    } else if (axes == EulerAngles<T>::Axes::yzy) {

        eulerAngles.y = std::acos(tl::clamp(m11, -consts::one<T>, consts::one<T>));

        if (std::abs(m11) < consts::one<T>) {

            eulerAngles.x = atan2(m21, -m01);
            eulerAngles.z = atan2(m12, m10);

        } else {

            eulerAngles.x = consts::zero<T>;
            eulerAngles.z = atan2(-m20, m22);

        }

    } else if (axes == EulerAngles<T>::Axes::zxz) {

        eulerAngles.y = std::acos(tl::clamp(m22, -consts::one<T>, consts::one<T>));

        if (std::abs(m22) < consts::one<T>) {

            eulerAngles.x = atan2(m02, -m12);
            eulerAngles.z = atan2(m20, m21);

        } else {

            eulerAngles.x = consts::zero<T>;
            eulerAngles.z = std::atan2(-m01, m00);

        }

    } else if (axes == EulerAngles<T>::Axes::zyz) {

        eulerAngles.y = std::acos(tl::clamp(m22, -consts::one<T>, consts::one<T>));

        if (std::abs(m22) < consts::one<T>) {

            eulerAngles.x = atan2(m12, m02);
            eulerAngles.z = atan2(m21, -m20);

        } else {

            eulerAngles.x = consts::zero<T>;
            eulerAngles.z = std::atan2(m10, m11);

        }

    }
}

template<typename T>
void RotationConverter<T>::convert(const EulerAngles<T> &eulerAngles,
                                   RotationMatrix<T> &rotationMatrix)
{
    typename EulerAngles<T>::Axes axes = eulerAngles.axes;

    T c1 = cos(eulerAngles.x);
    T c2 = cos(eulerAngles.y);
    T c3 = cos(eulerAngles.z);

    T s1 = sin(eulerAngles.x);
    T s2 = sin(eulerAngles.y);
    T s3 = sin(eulerAngles.z);

    // Tait-Bryan angles
    if (axes == EulerAngles<T>::Axes::xyz) {

        rotationMatrix[0][0] = c2 * c3;
        rotationMatrix[0][1] = -c2 * s3;
        rotationMatrix[0][2] = s2;
        rotationMatrix[1][0] = c1 * s3 + c3 * s1 * s2;
        rotationMatrix[1][1] = c1 * c3 - s1 * s2 * s3;
        rotationMatrix[1][2] = -c2 * s1;
        rotationMatrix[2][0] = s1 * s3 - c1 * c3 * s2;
        rotationMatrix[2][1] = c3 * s1 + c1 * s2 * s3;
        rotationMatrix[2][2] = c1 * c2;

    } else if (axes == EulerAngles<T>::Axes::yxz) {

        rotationMatrix[0][0] = c1 * c3 + s1 * s2 * s3;
        rotationMatrix[0][1] = c3 * s1 * s2 - c1 * s3;
        rotationMatrix[0][2] = c2 * s1;
        rotationMatrix[1][0] = c2 * s3;
        rotationMatrix[1][1] = c2 * c3;
        rotationMatrix[1][2] = -s2;
        rotationMatrix[2][0] = c1 * s2 * s3 - c3 * s1;
        rotationMatrix[2][1] = c1 * c3 * s2 + s1 * s3;
        rotationMatrix[2][2] = c1 * c2;

    } else if (axes == EulerAngles<T>::Axes::zxy) {

        rotationMatrix[0][0] = c1 * c3 - s1 * s2 * s3;
        rotationMatrix[0][1] = -s1 * c2;
        rotationMatrix[0][2] = c1 * s3 + s1 * s2 * c3;
        rotationMatrix[1][0] = s1 * c3 + c1 * s2 * s3;
        rotationMatrix[1][1] = c1 * c2;
        rotationMatrix[1][2] = s1 * s3 - c1 * s2 * c3;
        rotationMatrix[2][0] = -c2 * s3;
        rotationMatrix[2][1] = s2;
        rotationMatrix[2][2] = c2 * c3;

    } else if (axes == EulerAngles<T>::Axes::zyx) {

        rotationMatrix[0][0] = c1 * c2;
        rotationMatrix[0][1] = c1 * s2 * s3 - c3 * s1;
        rotationMatrix[0][2] = s1 * s3 + c1 * c3 * s2;
        rotationMatrix[1][0] = c2 * s1;
        rotationMatrix[1][1] = c1 * c3 + s1 * s2 * s3;
        rotationMatrix[1][2] = c3 * s1 * s2 - c1 * s3;
        rotationMatrix[2][0] = -s2;
        rotationMatrix[2][1] = c2 * s3;
        rotationMatrix[2][2] = c2 * c3;

    } else if (axes == EulerAngles<T>::Axes::yzx) {

        rotationMatrix[0][0] = c1 * c2;
        rotationMatrix[0][1] = s1 * s3 - c1 * s2 * c3;
        rotationMatrix[0][2] = c1 * s2 * s3 + s1 * c3;
        rotationMatrix[1][0] = s2;
        rotationMatrix[1][1] = c2 * c3;
        rotationMatrix[1][2] = -c2 * s3;
        rotationMatrix[2][0] = -s1 * c2;
        rotationMatrix[2][1] = c1 * s3 + s1 * s2 * c3;
        rotationMatrix[2][2] = c1 * c3 - s1 * s2 * s3;

    } else if (axes == EulerAngles<T>::Axes::xzy) {

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
    else if (axes == EulerAngles<T>::Axes::xyx) {

        rotationMatrix[0][0] = c2;
        rotationMatrix[0][1] = s2 * s3;
        rotationMatrix[0][2] = s2 * c3;
        rotationMatrix[1][0] = s1 * s2;
        rotationMatrix[1][1] = c1 * c3 - s1 * c2 * s3;
        rotationMatrix[1][2] = -c1 * s3 - s1 * c2 * c3;
        rotationMatrix[2][0] = -c1 * s2;
        rotationMatrix[2][1] = s1 * c3 + c1 * c2 * s3;
        rotationMatrix[2][2] = c1 * c2 * c3 - s1 * s3;

    } else if (axes == EulerAngles<T>::Axes::xzx) {

        rotationMatrix[0][0] = c2;
        rotationMatrix[0][1] = -c3 * s2;
        rotationMatrix[0][2] = s2 * s3;
        rotationMatrix[1][0] = c1 * s2;
        rotationMatrix[1][1] = c1 * c2 * c3 - s1 * s3;
        rotationMatrix[1][2] = -c3 * s1 - c1 * c2 * s3;
        rotationMatrix[2][0] = s1 * s2;
        rotationMatrix[2][1] = c1 * s3 + c2 * c3 * s1;
        rotationMatrix[2][2] = c1 * c3 - c2 * s1 * s3;

    } else if (axes == EulerAngles<T>::Axes::yxy) {

        rotationMatrix[0][0] = c1 * c3 - c2 * s1 * s3;
        rotationMatrix[0][1] = s1 * s2;
        rotationMatrix[0][2] = c1 * s3 + c2 * c3 * s1;
        rotationMatrix[1][0] = s2 * s3;
        rotationMatrix[1][1] = c2;
        rotationMatrix[1][2] = -c3 * s2;
        rotationMatrix[2][0] = -c3 * s1 - c1 * c2 * s3;
        rotationMatrix[2][1] = c1 * s2;
        rotationMatrix[2][2] = c1 * c2 * c3 - s1 * s3;

    } else if (axes == EulerAngles<T>::Axes::yzy) {

        rotationMatrix[0][0] = c1 * c2 * c3 - s1 * s3;
        rotationMatrix[0][1] = -c1 * s2;
        rotationMatrix[0][2] = c3 * s1 + c1 * c2 * s3;
        rotationMatrix[1][0] = c3 * s2;
        rotationMatrix[1][1] = c2;
        rotationMatrix[1][2] = s2 * s3;
        rotationMatrix[2][0] = -c1 * s3 - c2 * c3 * s1;
        rotationMatrix[2][1] = s1 * s2;
        rotationMatrix[2][2] = c1 * c3 - c2 * s1 * s3;

    } else if (axes == EulerAngles<T>::Axes::zxz) {

        rotationMatrix[0][0] = c1 * c3 - c2 * s1 * s3;
        rotationMatrix[0][1] = -c1 * s3 - c2 * c3 * s1;
        rotationMatrix[0][2] = s1 * s2;
        rotationMatrix[1][0] = c3 * s1 + c1 * c2 * s3;
        rotationMatrix[1][1] = c1 * c2 * c3 - s1 * s3;
        rotationMatrix[1][2] = -c1 * s2;
        rotationMatrix[2][0] = s2 * s3;
        rotationMatrix[2][1] = c3 * s2;
        rotationMatrix[2][2] = c2;

    } else if (axes == EulerAngles<T>::Axes::zyz) {

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


template<typename T>
void RotationConverter<T>::convert(const Quaternion<T> &quaternion,
                                   EulerAngles<T> &eulerAngles)
{
    RotationMatrix<T> rotationMatrix;
    convert(quaternion, rotationMatrix);
    convert(rotationMatrix, eulerAngles);
}

template<typename T>
void RotationConverter<T>::convert(const EulerAngles<T> &eulerAngles,
                                   Quaternion<T> &quaternion)
{
    typename EulerAngles<T>::Axes axes = eulerAngles.axes;

    T x = eulerAngles.x;
    T y = eulerAngles.y;
    T z = eulerAngles.z;

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
    if (axes == EulerAngles<T>::Axes::xyz) {

        quaternion.x = s1 * c2 * c3 + c1 * s2 * s3;
        quaternion.y = c1 * s2 * c3 - s1 * c2 * s3;
        quaternion.z = c1 * c2 * s3 + s1 * s2 * c3;
        quaternion.w = c1 * c2 * c3 - s1 * s2 * s3;

    } else if (axes == EulerAngles<T>::Axes::yxz) {

        quaternion.x = s1 * s3 * c2 + s2 * c1 * c3;
        quaternion.y = s1 * c2 * c3 - s2 * s3 * c1;
        quaternion.z = c1 * c2 * s3 - s1 * s2 * c3;
        quaternion.w = c1 * c2 * c3 + s1 * s2 * s3;

    } else if (axes == EulerAngles<T>::Axes::zxy) {

        quaternion.x = s2 * c1 * c3 - s1 * s3 * c2;
        quaternion.y = s1 * s2 * c3 + s3 * c1 * c2;
        quaternion.z = s1 * c2 * c3 + s2 * s3 * c1;
        quaternion.w = c1 * c2 * c3 - s1 * s2 * s3;

    } else if (axes == EulerAngles<T>::Axes::zyx) {

        quaternion.x = s3 * c1 * c2 - s1 * s2 * c3;
        quaternion.y = s1 * s3 * c2 + s2 * c1 * c3;
        quaternion.z = s1 * c2 * c3 - s2 * s3 * c1;
        quaternion.w = c1 * c2 * c3 + s1 * s2 * s3;

    } else if (axes == EulerAngles<T>::Axes::yzx) {

        quaternion.x = s1 * s2 * c3 + s3 * c1 * c2;
        quaternion.y = s1 * c2 * c3 + s2 * s3 * c1;
        quaternion.z = s2 * c1 * c3 - s1 * s3 * c2;
        quaternion.w = c1 * c2 * c3 - s1 * s2 * s3;

    } else if (axes == EulerAngles<T>::Axes::xzy) {

        quaternion.x = s1 * c2 * c3 - s2 * s3 * c1;
        quaternion.y = s3 * c1 * c2 - s1 * s2 * c3;
        quaternion.z = s1 * s3 * c2 + s2 * c1 * c3;
        quaternion.w = s1 * s2 * s3 + c1 * c2 * c3;

    }
    // Euler angles
    else if (axes == EulerAngles<T>::Axes::xyx) {

        quaternion.x = c2 * sin((x + z) * consts::one_half<T>);
        quaternion.y = s2 * cos((x - z) * consts::one_half<T>);
        quaternion.z = s2 * sin((x - z) * consts::one_half<T>);
        quaternion.w = c2 * cos((x + z) * consts::one_half<T>);

    } else if (axes == EulerAngles<T>::Axes::xzx) {

        quaternion.x = c2 * sin((x + z) * consts::one_half<T>);
        quaternion.y = -s2 * sin((x - z) * consts::one_half<T>);
        quaternion.z = s2 * cos((x - z) * consts::one_half<T>);
        quaternion.w = c2 * cos((x + z) * consts::one_half<T>);

    } else if (axes == EulerAngles<T>::Axes::yxy) {

        quaternion.x = s2 * cos((x - z) * consts::one_half<T>);
        quaternion.y = c2 * sin((x + z) * consts::one_half<T>);
        quaternion.z = -s2 * sin((x - z) * consts::one_half<T>);
        quaternion.w = c2 * cos((x + z) * consts::one_half<T>);

    } else if (axes == EulerAngles<T>::Axes::yzy) {

        quaternion.x = s2 * sin((x - z) * consts::one_half<T>);
        quaternion.y = c2 * sin((x + z) * consts::one_half<T>);
        quaternion.z = s2 * cos((x - z) * consts::one_half<T>);
        quaternion.w = c2 * cos((x + z) * consts::one_half<T>);

    } else if (axes == EulerAngles<T>::Axes::zxz) {

        quaternion.x = s2 * cos((x - z) * consts::one_half<T>);
        quaternion.y = s2 * sin((x - z) * consts::one_half<T>);
        quaternion.z = c2 * sin((x + z) * consts::one_half<T>);
        quaternion.w = c2 * cos((x + z) * consts::one_half<T>);

    } else if (axes == EulerAngles<T>::Axes::zyz) {

        quaternion.x = -s2 * sin((x - z) * consts::one_half<T>);
        quaternion.y = s2 * cos((x - z) * consts::one_half<T>);
        quaternion.z = c2 * sin((x + z) * consts::one_half<T>);
        quaternion.w = c2 * cos((x + z) * consts::one_half<T>);

    }
}

template<typename T>
void RotationConverter<T>::convert(const EulerAngles<T> &eulerAngles,
                                   AxisAngle<T> &axisAngle)
{
    Quaternion<T> quaternion;
    convert(eulerAngles, quaternion);
    convert(quaternion, axisAngle);
}

template<typename T>
void RotationConverter<T>::convert(const AxisAngle<T> &axisAngle,
                                   EulerAngles<T> &eulerAngles)
{
    RotationMatrix<T> rotationMatrix;
    convert(axisAngle, rotationMatrix);
    convert(rotationMatrix, eulerAngles);
}

/*! \} */ // end of rotation

/*! \} */ // end of algebra

/*! \} */ // end of math

} // End namespace tl
