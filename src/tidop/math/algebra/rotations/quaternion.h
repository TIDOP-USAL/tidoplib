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


#ifdef TL_HAVE_FMT
#include <fmt/format.h>
#else
#include <format>
#endif

#include "tidop/math/math.h"
#include "tidop/math/algebra/rotations/rotations.h"
#include "tidop/geometry/entities/point.h"

namespace tl
{

/*! \addtogroup Rotations
 *  \{
 */



 /*!
  * \brief Quaternion class for representing orientations and rotations in space.
  *
  * Unit quaternions provide a mathematical notation for representing the
  * orientations and rotations of objects in three dimensions. Compared to
  * Euler angles, they are easier to compose and avoid the gimbal lock issue.
  * Compared to rotation matrices, they are more efficient and numerically stable.
  *
  * A quaternion is represented as:
  * \f[ w + xi + yj + zk \f]
  * where \(w, x, y, z\) are the components of the quaternion.
  */
template<typename T>
class Quaternion
  : public OrientationBase<Quaternion<T>>
{

public:

    T x; /*!< The x coefficient of the quaternion. */
    T y; /*!< The y coefficient of the quaternion. */
    T z; /*!< The z coefficient of the quaternion. */
    T w; /*!< The scalar part (w) of the quaternion. */

public:

    /*!
     * \brief Default constructor
     * Initializes the quaternion to zero.
     */
    Quaternion();

    /*!
     * \brief Constructor
     * \param[in] x Coefficient for the x-axis component.
     * \param[in] y Coefficient for the y-axis component.
     * \param[in] z Coefficient for the z-axis component.
     * \param[in] w Scalar part of the quaternion.
     */
    Quaternion(T x, T y, T z, T w);

    /*!
     * \brief Copy constructor
     * \param[in] quaternion The quaternion object to copy.
     */
    Quaternion(const Quaternion<T> &quaternion);

    /*!
     * \brief Move constructor
     * \param[in] quaternion The quaternion object to move.
     */
    Quaternion(Quaternion<T> &&quaternion) TL_NOEXCEPT;

    /*!
     * \brief Destructor
     */
    ~Quaternion() override;

    /*!
     * \brief Assignment operator
     * \param[in] quaternion The quaternion object to copy.
     * \return A reference to the current quaternion object.
     */
    auto operator=(const Quaternion<T> &quaternion) -> Quaternion &;

    /*!
     * \brief Move assignment operator
     * \param[in] quaternion The quaternion object to move.
     * \return A reference to the current quaternion object.
     */
    auto operator=(Quaternion<T> &&quaternion) TL_NOEXCEPT -> Quaternion &;

    /*!
     * \brief Quaternion conjugate
     * The conjugate of a quaternion reverses the sign of the "added" components:
     * \f[ q = w - xi - yj - zk \f]
     * \return The conjugated quaternion.
     */
    auto conjugate() const->Quaternion<T>;

    /*!
     * \brief Norm
     * The norm of a quaternion is defined as the square root of the sum of the squares of its components:
     * \f[ n(q) = \sqrt{w^2 + x^2 + y^2 + z^2} \f]
     * \return The norm (magnitude) of the quaternion.
     */
    auto norm() const -> T;

    /*!
     * \brief Normalizes the quaternion.
     * Converts the quaternion into a unit quaternion (magnitude 1).
     * \return A reference to the normalized quaternion.
     */
    auto normalize() -> Quaternion&;

    /*!
     * \brief Inverse of the quaternion
     * The inverse of a non-zero quaternion is given by:
     * \f[ q^{-1} = \frac{-xi - yj - zk + w}{|q|^2} \f]
     * When the quaternion is zero, the function returns zero.
     * \return The inverse of the quaternion.
     */
    auto inverse() const -> Quaternion;

    /*!
     * \brief Quaternion multiplication assignment
     * Multiplies the current quaternion by another quaternion.
     * \param[in] quaternion The quaternion to multiply with.
     * \return A reference to the current quaternion.
     */
    auto operator *=(const Quaternion &quaternion) -> Quaternion&;

    /*!
     * \brief Quaternion addition assignment
     * Adds another quaternion to the current quaternion.
     * \param[in] quaternion The quaternion to add.
     * \return A reference to the current quaternion.
     */
    auto operator+=(const Quaternion &quaternion) -> Quaternion &;

    /*!
     * \brief Quaternion subtraction assignment
     * Subtracts another quaternion from the current quaternion.
     * \param[in] quaternion The quaternion to subtract.
     * \return A reference to the current quaternion.
     */
    auto operator-=(const Quaternion &quaternion) -> Quaternion &;

    /*!
     * \brief Scalar multiplication assignment
     * Multiplies the current quaternion by a scalar.
     * \param[in] scalar The scalar to multiply by.
     * \return A reference to the current quaternion.
     */
    auto operator*=(T scalar) -> Quaternion &;

    /*!
     * \brief Scalar division assignment
     * Divides the current quaternion by a scalar.
     * \param[in] scalar The scalar to divide by.
     * \return A reference to the current quaternion.
     */
    auto operator /=(T scalar) -> Quaternion&;

    /* Factory methods */

    /*!
     * \brief Returns the zero quaternion
     * \f[ z = 0*i + 0*j + 0*k + 0 \f]
     * \return The zero quaternion.
     */
    static auto zero() -> Quaternion;

    /*!
     * \brief Returns the quaternion representing the unit vector along the i-axis.
     * \f[ i = 1*i + 0*j + 0*k + 0 \f]
     * \return The quaternion representing the i-axis.
     */
    static auto i() -> Quaternion;

    /*!
     * \brief Returns the quaternion representing the unit vector along the j-axis.
     * \f[ j = 0*i + 1*j + 0*k + 0 \f]
     * \return The quaternion representing the j-axis.
     */
    static auto j() -> Quaternion;

    /*!
     * \brief Returns the quaternion representing the unit vector along the k-axis.
     * \f[ k = 0*i + 0*j + 1*k + 0 \f]
     * \return The quaternion representing the k-axis.
     */
    static auto k() -> Quaternion;

    /*!
     * \brief Returns the identity quaternion
     * \f[ 1 = 0*i + 0*j + 0*k + 1 \f]
     * \return The identity quaternion (1).
     */
    static auto identity() -> Quaternion;

    /*!
     * \brief Normalize a quaternion
     * \param[in] quaternion The quaternion to normalize.
     * \return The normalized quaternion.
     */
    static auto normalize(const Quaternion<T> &quaternion) -> Quaternion;

};



using Quaternionf = Quaternion<float>;
using Quaterniond = Quaternion<double>;




template<typename T>
Quaternion<T>::Quaternion()
  : OrientationBase<Quaternion<T>>(Orientation::Type::quaternion),
    x(-std::numeric_limits<T>().max()),
    y(-std::numeric_limits<T>().max()),
    z(-std::numeric_limits<T>().max()),
    w(-std::numeric_limits<T>().max())
{
}

template<typename T>
Quaternion<T>::Quaternion(T x, T y, T z, T w)
  : OrientationBase<Quaternion<T>>(Orientation::Type::quaternion),
    x(x),
    y(y),
    z(z),
    w(w)
{
}

template<typename T>
Quaternion<T>::Quaternion(const Quaternion<T> &quaternion)
  : OrientationBase<Quaternion<T>>(Orientation::Type::quaternion),
    x(quaternion.x),
    y(quaternion.y),
    z(quaternion.z),
    w(quaternion.w)
{
}

template<typename T>
Quaternion<T>::Quaternion(Quaternion<T> &&quaternion) TL_NOEXCEPT
  : OrientationBase<Quaternion<T>>(Orientation::Type::quaternion),
    x(std::exchange(quaternion.x, -std::numeric_limits<T>().max())),
    y(std::exchange(quaternion.y, -std::numeric_limits<T>().max())),
    z(std::exchange(quaternion.z, -std::numeric_limits<T>().max())),
    w(std::exchange(quaternion.w, -std::numeric_limits<T>().max()))
{
}

template<typename T>
Quaternion<T>::~Quaternion()
{
}

template<typename T>
auto Quaternion<T>::operator = (const Quaternion &quaternion) -> Quaternion<T> &
{
    if (this != &quaternion) {
        this->x = quaternion.x;
        this->y = quaternion.y;
        this->z = quaternion.z;
        this->w = quaternion.w;
    }

    return *this;
}

template<typename T>
auto Quaternion<T>::operator = (Quaternion &&quaternion) TL_NOEXCEPT -> Quaternion<T> &
{
    if (this != &quaternion) {
        this->x = std::exchange(quaternion.x, -std::numeric_limits<T>().max());
        this->y = std::exchange(quaternion.y, -std::numeric_limits<T>().max());
        this->z = std::exchange(quaternion.z, -std::numeric_limits<T>().max());
        this->w = std::exchange(quaternion.w, -std::numeric_limits<T>().max());
    }

    return *this;
}

template<typename T>
auto Quaternion<T>::conjugate() const -> Quaternion<T>
{
    return Quaternion<T>(-x, -y, -z, w);
}

template<typename T>
auto Quaternion<T>::norm() const -> T
{
    return sqrt(x * x + y * y + z * z + w * w);
}

template<typename T>
auto Quaternion<T>::normalize() -> Quaternion<T>&
{
    T length = this->norm();
    *this /= length;
    return *this;
}

template <typename T>
auto Quaternion<T>::inverse() const -> Quaternion<T>
{
    T _dot = dot(*this, *this);
    Quaternion<T> inverse = this->conjugate() / _dot;
    return inverse;
}

template<typename T>
auto Quaternion<T>::operator *= (const Quaternion<T> &quaternion) -> Quaternion &
{
    Quaternion<T> copy = *this;

    this->x = copy.x * quaternion.w + copy.y * quaternion.z - copy.z * quaternion.y + copy.w * quaternion.x;
    this->y = -copy.x * quaternion.z + copy.y * quaternion.w + copy.z * quaternion.x + copy.w * quaternion.y;
    this->z = copy.x * quaternion.y - copy.y * quaternion.x + copy.z * quaternion.w + copy.w * quaternion.z;
    this->w = -copy.x * quaternion.x - copy.y * quaternion.y - copy.z * quaternion.z + copy.w * quaternion.w;

    return *this;
}

template<typename T>
auto Quaternion<T>::operator += (const Quaternion<T> &quaternion) -> Quaternion &
{
    this->x += quaternion.x;
    this->y += quaternion.y;
    this->z += quaternion.z;
    this->w += quaternion.w;

    return *this;
}

template<typename T>
auto Quaternion<T>::operator -= (const Quaternion<T> &quaternion) -> Quaternion &
{
    this->x -= quaternion.x;
    this->y -= quaternion.y;
    this->z -= quaternion.z;
    this->w -= quaternion.w;

    return *this;
}

template<typename T>
auto Quaternion<T>::operator *= (T scalar) -> Quaternion &
{
    this->x *= scalar;
    this->y *= scalar;
    this->z *= scalar;
    this->w *= scalar;

    return *this;
}

template<typename T>
auto Quaternion<T>::operator /= (T scalar) -> Quaternion &
{
    if (scalar != consts::zero<T>) {
        
        this->operator*=(consts::one<T> / scalar);

    } else {
        //¿Devolver excepción?
        this->x = consts::zero<T>;
        this->y = consts::zero<T>;
        this->z = consts::zero<T>;
        this->w = consts::zero<T>;
    }

    return *this;
}

template<typename T>
auto Quaternion<T>::zero() -> Quaternion<T>
{
    return Quaternion(consts::zero<T>,
                      consts::zero<T>,
                      consts::zero<T>,
                      consts::zero<T>);
}

template<typename T>
auto Quaternion<T>::i() -> Quaternion<T>
{
    return Quaternion(consts::one<T>,
                      consts::zero<T>,
                      consts::zero<T>,
                      consts::zero<T>);
}

template<typename T>
auto Quaternion<T>::j() ->Quaternion<T>
{
    return Quaternion(consts::zero<T>,
                      consts::one<T>,
                      consts::zero<T>,
                      consts::zero<T>);
}

template<typename T>
auto Quaternion<T>::k() -> Quaternion<T>
{
    return Quaternion(consts::zero<T>,
                      consts::zero<T>,
                      consts::one<T>,
                      consts::zero<T>);
}

template<typename T>
auto Quaternion<T>::identity() -> Quaternion<T>
{
    return Quaternion(consts::zero<T>,
                      consts::zero<T>,
                      consts::zero<T>,
                      consts::one<T>);
}

template<typename T> inline
auto Quaternion<T>::normalize(const Quaternion<T> &quaternion) -> Quaternion<T>
{
    Quaternion<T> _quaternion(quaternion);
    _quaternion.normalize();
    return _quaternion;
}


/* Operaciones unarias */

template <typename T>
auto operator -(const Quaternion<T>& quaternion) -> Quaternion<T>
{
    return Quaternion<T>(-quaternion.x,
                         -quaternion.y,
                         -quaternion.z,
                         -quaternion.w);
}

template <typename T>
auto operator -(Quaternion<T>&& quaternion) -> Quaternion<T>
{
    quaternion.x = -quaternion.x;
    quaternion.y = -quaternion.y;
    quaternion.z = -quaternion.z;
    quaternion.w = -quaternion.w;

    return quaternion;
}

/* Operations between quaternions */

/*!
 * \brief Quaternion multiplication
 * \f[ q_1 = w_1 + x_1 \cdot i + y_1 \cdot j + z_1 \cdot k \f]
 * \f[ q_2 = w_2 + x_2 \cdot i + y_2 \cdot j + z_2 \cdot k \f]
 * \f[ q_1 \cdot q_2 = \f]
 */
template<typename T>
auto operator *(const Quaternion<T>& quat1,
                const Quaternion<T>& quat2) -> Quaternion<T>
{
    Quaternion<T> quaternion = quat1;
    quaternion *= quat2;
    return quaternion;
}

template<typename T>
auto operator *(Quaternion<T> &&quat1,
                const Quaternion<T> &quat2) -> Quaternion<T>
{
    quat1 *= quat2;
    return quat1;
}

template<typename T>
auto operator *(Quaternion<T> &&quat1,
                Quaternion<T> &&quat2) -> Quaternion<T>
{
    quat1 *= quat2;
    return quat1;
}

/*!
 * \brief Quaternion addition
 * Quaternion addition is performed term by term
 * \f[ q_1 = w_1 + x_1 \cdot i + y_1 \cdot j + z_1 \cdot k \f]
 * \f[ q_2 = w_2 + x_2 \cdot i + y_2 \cdot j + z_2 \cdot k \f]
 * \f[ q = q_1 + q_2 \f]
 * \f[ q = (w_1 + w_2) + (x_1 + x_2) \cdot i + (y_1 + y_2) \cdot j + (z_1 + z_2) \cdot k \f]
 */
template<typename T>
auto operator +(const Quaternion<T>& quat1,
                const Quaternion<T>& quat2) -> Quaternion<T>
{
    Quaternion<T> q = quat1;
    return q += quat2;
}

template<typename T>
auto operator +(Quaternion<T>&& quat1,
                const Quaternion<T>& quat2) -> Quaternion<T>
{
    return quat1 += quat2;
}

template<typename T>
auto operator +(const Quaternion<T>& quat1,
                Quaternion<T>&& quat2) -> Quaternion<T>
{
    return quat2 += quat1;
}

template<typename T>
auto operator +(Quaternion<T>&& quat1,
                Quaternion<T>&& quat2) -> Quaternion<T>
{
    return quat1 += quat2;
}


/*!
 * \brief Quaternion subtraction
 * Quaternion subtraction is performed term by term
 * \f[ q_1 = w_1 + x_1 \cdot i + y_1 \cdot j + z_1 \cdot k \f]
 * \f[ q_2 = w_2 + x_2 \cdot i + y_2 \cdot j + z_2 \cdot k \f]
 * \f[ q = q_1 - q_2 \f]
 * \f[ q = (w_1 - w_2) + (x_1 - x_2) \cdot i + (y_1 - y_2) \cdot j + (z_1 - z_2) \cdot k \f]
 */
template<typename T>
auto operator -(const Quaternion<T>& quat1,
                const Quaternion<T>& quat2) -> Quaternion<T>
{
    Quaternion<T> q = quat1;
    return q -= quat2;
}

template<typename T>
auto operator -(Quaternion<T>&& quat1,
                const Quaternion<T>& quat2) -> Quaternion<T>
{
    return quat1 -= quat2;
}

template<typename T>
auto operator -(const Quaternion<T>& quat1,
                Quaternion<T>&& quat2) -> Quaternion<T>
{
    return -(quat2 -= quat1);
}

template<typename T>
auto operator -(Quaternion<T>&& quat1,
                Quaternion<T>&& quat2) -> Quaternion<T>
{
    return quat1 -= quat2;
}

template<typename T>
auto operator *(const Quaternion<T>& quaternion, T scalar) -> Quaternion<T>
{
    Quaternion<T> q = quaternion;
    return q *= scalar;
}

template<typename T>
auto operator *(Quaternion<T>&& quaternion, T scalar) -> Quaternion<T>
{
    quaternion *= scalar;
    return quaternion;
}

template<typename T>
auto operator *(T scalar, const Quaternion<T>& quaternion) -> Quaternion<T>
{
    Quaternion<T> q = quaternion;
    return q *= scalar;
}

template<typename T>
auto operator *(T scalar, Quaternion<T>&& quaternion) -> Quaternion<T>
{
    quaternion *= scalar;
    return quaternion;
}

template<typename T>
auto operator /(const Quaternion<T>& quaternion, T scalar) -> Quaternion<T>
{
    Quaternion<T> q = quaternion;
    return q /= scalar;
}

template<typename T>
auto operator /(Quaternion<T>&& quaternion, T scalar) -> Quaternion<T>
{
    quaternion /= scalar;
    return quaternion;
}

template<typename T>
auto operator /(T scalar, const Quaternion<T>& quaternion) -> Quaternion<T>
{
    Quaternion<T> q = quaternion;
    return q /= scalar;
}

template<typename T>
auto operator /(T scalar, Quaternion<T>&& quaternion) -> Quaternion<T>
{
    quaternion /= scalar;
    return quaternion;
}

template<typename T>
auto dot(const Quaternion<T>& quat1, const Quaternion<T>& quat2) -> T
{
    return quat1.x * quat2.x + quat1.y * quat2.y + quat1.z * quat2.z + quat1.w * quat2.w;
}

template<typename T> 
auto operator ==(const Quaternion<T>& q1, const Quaternion<T>& q2) -> bool
{
    return q1.x == q2.x && q1.y == q2.y && q1.z == q2.z && q1.w == q2.w;
}

template<typename T> 
auto operator !=(const Quaternion<T>& q1, const Quaternion<T>& q2) -> bool
{
    return q1.x != q2.x || q1.y != q2.y || q1.z != q2.z || q1.w != q2.w;
}

template<typename T>
auto operator *(const Quaternion<T> &quaternion, const Point3<T> &point) -> Point3<T>
{
    Quaternion<T> q1 = quaternion;
    q1.normalize();
    auto q3 = q1 * Quaternion<T>(point.x, point.y, point.z, consts::zero<T>) * q1.conjugate();
    return {q3.x, q3.y, q3.z};
}

template<typename T>
auto operator *(Quaternion<T> &&quaternion, const Point3<T> &point) -> Point3<T>
{
    quaternion.normalize();
    auto q2 = quaternion * Quaternion<T>(point.x, point.y, point.z, consts::zero<T>) * quaternion.conjugate();
    return {q2.x, q2.y, q2.z};
}

template<typename T>
auto operator *(const Quaternion<T> &quaternion, const Vector<T, 3> &point) -> Vector<T, 3>
{
    Quaternion<T> q1 = quaternion;
    q1.normalize();
    auto q3 = q1 * Quaternion<T>(point.x(), point.y(), point.z(), consts::zero<T>) * q1.conjugate();
    return {q3.x, q3.y, q3.z};
}

template<typename T>
auto operator *(Quaternion<T> &&quaternion, const Vector<T, 3> &point) -> Vector<T, 3>
{
    quaternion.normalize();
    auto q2 = quaternion * Quaternion<T>(point.x(), point.y(), point.z(), consts::zero<T>) * quaternion.conjugate();
    return {q2.x, q2.y, q2.z};
}

template<typename T>
auto operator<<(std::ostream& os, const Quaternion<T>& q) -> std::ostream&
{
    os << "[w=" << q.w << ", x=" << q.x << ", y=" << q.y << ", z=" << q.z << "]";
    os << std::endl;

    return os;
}

template<typename T>
auto operator<<(std::ostream& os, const Quaternion<T>* q) -> std::ostream&
{
    os << "[w=" << q->w << ", x=" << q->x << ", y=" << q->y << ", z=" << q->z << "]";
    os << std::endl;

    return os;
}



/*! \} */

} // End namespace tl

#if CPP_VERSION >= 20 || defined(TL_HAVE_FMT)

template <typename T>
struct FORMAT_NAMESPACE formatter<tl::Quaternion<T>> 
{

    constexpr auto parse(FORMAT_NAMESPACE format_parse_context &context) 
    {
        return context.begin();
    }

    auto format(const tl::Quaternion<T> &q, FORMAT_NAMESPACE format_context &ctx) 
    {
        return FORMAT_NAMESPACE format_to(ctx.out(), "[w:{}, x:{}, y:{}, z:{}]", q.w, q.x, q.y, q.z);
    }
};

#endif