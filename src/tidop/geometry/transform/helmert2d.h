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


#include "tidop/geometry/transform/transform.h"
#include "tidop/geometry/transform/translation.h"
#include "tidop/geometry/transform/affine.h"
#include "tidop/geometry/transform/rotation.h"
#include "tidop/geometry/transform/scale.h"
#include "tidop/core/exception.h"
#include "tidop/math/algebra/matrix.h"
#include "tidop/math/algebra/vector.h"
#include "tidop/math/algebra/svd.h"

namespace tl
{

/*! \addtogroup geometry
 *  \{
 */

/*! \addtogroup trfGroup
 *  \{
 */

/*! \addtogroup trf2DGroup
 *  \{
 */



/*!
 * \brief Tranformación Helmert 2D o transformación de semejanza
 *
 * Esta transformación expresa la relación que existe (o transformación que es 
 * preciso realizar)  entre dos sistemas cartesianos que discrepan en la situación 
 * del origen, en la orientación de los ejes y en la unidad de medida a lo largo 
 * de los mismos pero de manera que dicha variación en unidad de medida es constante 
 * a lo largo de cada eje y entre los dos ejes  
 *
 * \f$ a = scale * cos(rotation)\f$<BR>
 * \f$ b = scale * sin(rotation)\f$<BR>
 *
 * \f$ x' = a * x + b * y + X0\f$<BR>
 * \f$ y' = a * y - b * x + Y0\f$
 */
template<typename Point_t>
class Helmert2D
  : public Transform2D<Point_t>
{

public:

    /*!
     * \brief Traslación en x
     */
    double tx;
    
    /*!
     * \brief Traslación en y
     */
    double ty;

public:

    /*!
     * \brief Constructor por defecto
     */
    Helmert2D();

    /*!
     * \brief Constructor
     * \param[in] tx Traslación en x
     * \param[in] ty Traslación en y
     * \param[in] scale Escala
     * \param[in] rotation Rotación
     */
    Helmert2D(double tx, 
              double ty, 
              double scale, 
              double rotation);
    
    /*!
     * \brief Constructor de copia
     * \param[in] helmert2D Objeto que se copia
     */
    Helmert2D(const Helmert2D &helmert2D);
    
    /*!
     * \brief Constructor de movimiento
     * \param[in] helmert2D Objeto que se copia
     */
    Helmert2D(Helmert2D &&helmert2D) TL_NOEXCEPT;
    
    ~Helmert2D() override = default;
    
    /*!
     * \brief Calcula la transformación Helmert 2D entre dos sistemas
     * diferentes a partir de dos conjuntos de puntos en cada sistema
     * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
     * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
     * \param[out] error Vector con los errores para cada punto
     * \param[out] rmse Root Mean Square Error
     * \return Transform::Status
     * \see Transform::Status
     */
    Transform::Status compute(const std::vector<Point_t> &pts1, 
                              const std::vector<Point_t> &pts2, 
                              std::vector<double> *error = nullptr,
                              double *rmse = nullptr) override;
    
    /*!
     * \brief Transforma un conjunto de puntos en otro aplicando un helmert 2D
     * \param[in] ptsIn Puntos de entrada
     * \param[out] ptsOut Puntos de salida
     * \param[in] trfOrder Transformación directa (por defecto) o inversa
     * \return Transform::Status
     * \see Transform::Order, Transform::Status
     *  
     * <H3>Ejemplo:</H3>
     * \code
     * Helmert2D<Point<double>> h2d(x0,y0, scale, rotation);
     * std::vector<Point<double>> pts_in{ Point<double>(4157222.543, 664789.307),
     *     Point<double>(4149043.336, 688836.443), Point<double>(4172803.511, 690340.078),
     *     Point<double>(4177148.376, 642997.635), Point<double>(4137012.190, 671808.029), 
     *     Point<double>(4146292.729, 666952.887), Point<double>(4138759.902, 702670.738) };
     * std::vector<Point<double>> pts_out;
     * h2d.transform(pts_in, pts_out);
     * \endcode
     */
    Transform::Status transform(const std::vector<Point_t> &ptsIn,
                                std::vector<Point_t> &ptsOut,
                                Transform::Order trfOrder = Transform::Order::direct) const override;

    /*!
     * \brief Aplica un helmert 2D a un punto
     * \param[in] ptIn Punto de entrada
     * \param[out] ptOut Punto de salida
     * \param[in] trfOrder Transformación directa (por defecto) o inversa
     * \return Transform::Status
     * \see Transform::Order, Transform::Status
     */
    Transform::Status transform(const Point_t &ptIn,
                                Point_t &ptOut,
                                Transform::Order trfOrder = Transform::Order::direct) const override;
    
    
    /*!
     * \brief Aplica un helmert 2D a un punto
     * \param[in] ptIn Punto de entrada
     * \param[in] trfOrder Transformación directa (por defecto) o inversa
     * \return Punto de salida
     * \see Transform::Order
     */
    Point_t transform(const Point_t &ptIn,
                      Transform::Order trfOrder = Transform::Order::direct) const override;

#ifdef TL_ENABLE_DEPRECATED_METHODS
    /*!
     * \brief Devuelve el giro
     * \return[in] Ángulo de rotación en radianes
     */
    TL_DEPRECATED("rotation", "2.0")
    double getRotation() const;
    
    
    /*!
     * \brief Devuelve la escala de la transformación
     * \return Escala de la transformación
     */
    TL_DEPRECATED("scale", "2.0")
    double getScale() const;
#endif

    /*!
     * \brief Devuelve el giro
     * \return[in] Ángulo de rotación en radianes
     */
    double rotation() const;
    
    
    /*!
     * \brief Devuelve la escala de la transformación
     * \return Escala de la transformación
     */
    double scale() const;
    
    /*!
     * \brief Establece los parámetros
     * \param[in] tx Traslación en x
     * \param[in] ty Traslación en y
     * \param[in] scale Escala
     * \param[in] rotation Rotación
     */
    void setParameters(double tx, 
                       double ty, 
                       double scale, 
                       double rotation);
    
    /*!
     * \brief Establece la rotación de la transformación
     * \param[in] rotation Ángulo de rotación en radianes
     */
    void setRotation(double rotation);
    
    /*!
     * \brief Establece la escala de la transformación
     * \param[in] scale Escala de la transformación
     */
    void setScale(double scale);
    
    /*!
     * \brief Transformación inversa
     * \return
     */
    Helmert2D inverse() const;
    
    /*!
     * \brief Operador de asignación
     * \param[in] helmert Objeto Helmert2D que se copia
     */
    Helmert2D &operator = (const Helmert2D<Point_t> &helmert);
    
    /*!
     * \brief Operador de asignación de movimiento
     * \param[in] helmert Objeto Helmert2D que se mueve
     */
    Helmert2D &operator = (Helmert2D<Point_t> &&helmert) TL_NOEXCEPT;
    
    // conversión a otras transformaciones
    
    template <typename Point_t2>
    explicit operator Helmert2D<Point_t2>() const;
    
    template <typename Point_t2>
    explicit operator Affine<Point_t2>() const;
    
    template <typename Point_t2>
    explicit operator Translation<Point_t2>() const;
    
    template <typename Point_t2>
    explicit operator Rotation<Point_t2>() const;
    
    bool isNull() const
    {
        return (tx == 0. &&
                ty == 0. &&
                mScale == 1. &&
                mRotation == 0.);
    }

private:

    /*!
     * \brief Actualiza la transformación
     */
    void update();
    
    /*!
     * \brief Actualiza los parámetros de la transformacion inversa
     */
    void updateInv();

private:

    /*!
     * \brief Escala
     */
    double mScale;
    
    /*!
     * \brief Ángulo de rotation
     */
    double mRotation;
    
    /*!
     * \brief a = scale * cos(rotation);
     */
    double a;
    
    /*!
     * \brief b = scale * sin(rotation);
     */
    double b;
    
    double ai;
    double bi;
    double txi;
    double tyi;
};



template<typename Point_t> inline
Helmert2D<Point_t>::Helmert2D()
  : Transform2D<Point_t>(Transform::Type::helmert_2d, 2),
    tx(0),
    ty(0),
    mScale(1.),
    mRotation(0.)
{
    update();
}

template<typename Point_t>
Helmert2D<Point_t>::Helmert2D(double tx, 
                              double ty, 
                              double scale, 
                              double rotation)
  : Transform2D<Point_t>(Transform::Type::helmert_2d, 2),
    tx(tx),
    ty(ty),
    mScale(scale),
    mRotation(rotation)
{
    update();
}

template<typename Point_t> inline
Helmert2D<Point_t>::Helmert2D(const Helmert2D &helmert2D)
  : Transform2D<Point_t>(helmert2D),
    tx(helmert2D.tx),
    ty(helmert2D.ty),
    mScale(helmert2D.mScale),
    mRotation(helmert2D.mRotation),
    a(helmert2D.a),
    b(helmert2D.b),
    ai(helmert2D.ai),
    bi(helmert2D.bi),
    txi(helmert2D.txi),
    tyi(helmert2D.tyi)
{
}

template<typename Point_t> inline
Helmert2D<Point_t>::Helmert2D(Helmert2D &&helmert2D) TL_NOEXCEPT
  : Transform2D<Point_t>(std::forward<Transform2D<Point_t>>(helmert2D)),
    tx(helmert2D.tx),
    ty(helmert2D.ty),
    mScale(helmert2D.mScale),
    mRotation(helmert2D.mRotation),
    a(helmert2D.a),
    b(helmert2D.b),
    ai(helmert2D.ai),
    bi(helmert2D.bi),
    txi(helmert2D.txi),
    tyi(helmert2D.tyi)
{
}


template<typename Point_t> inline
Transform::Status Helmert2D<Point_t>::compute(const std::vector<Point_t> &pts1,
                                              const std::vector<Point_t> &pts2,
                                              std::vector<double> *error,
                                              double *rmse)
{
    size_t n1 = pts1.size();
    size_t n2 = pts2.size();

    if (n1 != n2) {
        Message::error("Sets of points with different size. Size pts1 = {} and size pts2 = {}", n1, n2);
        return Transform::Status::failure;
    }

    if (!this->isNumberOfPointsValid(n1)) {
        Message::error("Invalid number of points: {} < {}", n1, this->mMinPoint);
        return Transform::Status::failure;
    }

    Transform::Status status = Transform::Status::success;

    size_t m = n1 * static_cast<size_t>(this->mDimensions);
    size_t n = 4;

    try {

        Matrix<double> A(m, n, 0);
        Vector<double> B(m);

        for (size_t i = 0, r = 0; i < n1; i++, r++) {

            A.at(r, 0) = pts1[i].x;
            A.at(r, 1) = -pts1[i].y;
            A.at(r, 2) = 1;
            //A.at(r, 3) = 0;

            B[r] = pts2[i].x;

            r++;

            A.at(r, 0) = pts1[i].y;
            A.at(r, 1) = pts1[i].x;
            //A.at(r, 2) = 0;
            A.at(r, 3) = 1;

            B[r] = pts2[i].y;
        }

        SingularValueDecomposition<Matrix<double>> svd(A);
        Vector<double> C = svd.solve(B);

        a = C[0];
        b = C[1];
        tx = C[2];
        ty = C[3];

        updateInv();

        mRotation = atan2(b, a);
        mScale = module(a, b);

        if (error) {
            if (rmse) *rmse = this->_rootMeanSquareError(pts1, pts2, error);
        }

    } catch (std::exception &e) {
        printException(e);
        status = Transform::Status::failure;
    }

    return status;
}

template<typename Point_t> inline
Transform::Status Helmert2D<Point_t>::transform(const Point_t &ptIn,
                                                Point_t &ptOut,
                                                Transform::Order trfOrder) const
{
    Transform::Status r_status = Transform::Status::success;

    using sub_type = typename Point_t::value_type;
    sub_type x_aux = ptIn.x;

    try {
        if (trfOrder == Transform::Order::direct) {
            ptOut.x = static_cast<sub_type>(a * x_aux - b * ptIn.y + tx);
            ptOut.y = static_cast<sub_type>(b * x_aux + a * ptIn.y + ty);
        } else {
            ptOut.x = static_cast<sub_type>(ai * x_aux - bi * ptIn.y + txi);
            ptOut.y = static_cast<sub_type>(bi * x_aux + ai * ptIn.y + tyi);
        }
    } catch (tl::Exception &e) {
        printException(e);
        r_status = Transform::Status::failure;
    }

    return r_status;
}

template<typename Point_t> inline
Point_t Helmert2D<Point_t>::transform(const Point_t &ptIn,
                                      Transform::Order trfOrder) const
{
    using sub_type = typename Point_t::value_type;

    Point_t r_pt;
    if (trfOrder == Transform::Order::direct) {
        r_pt.x = static_cast<sub_type>(a * ptIn.x - b * ptIn.y + tx);
        r_pt.y = static_cast<sub_type>(b * ptIn.x + a * ptIn.y + ty);
    } else {
        r_pt.x = static_cast<sub_type>(ai * ptIn.x - bi * ptIn.y + txi);
        r_pt.y = static_cast<sub_type>(bi * ptIn.x + ai * ptIn.y + tyi);
    }

    return r_pt;
}

template<typename Point_t> inline
Transform::Status Helmert2D<Point_t>::transform(const std::vector<Point_t> &ptsIn,
                                                std::vector<Point_t> &ptsOut,
                                                Transform::Order trfOrder) const
{
    Transform::Status r_status = Transform::Status::success;

    this->formatVectorOut(ptsIn, ptsOut);
    for (size_t i = 0; i < ptsIn.size(); i++) {
        r_status = transform(ptsIn[i], ptsOut[i], trfOrder);
        if (r_status == Transform::Status::failure) break;
    }

    return r_status;
}

#ifdef TL_ENABLE_DEPRECATED_METHODS
template<typename Point_t> inline
double Helmert2D<Point_t>::getRotation() const
{
    return mRotation;
}

template<typename Point_t>
double Helmert2D<Point_t>::getScale() const
{
    return mScale;
}
#endif

template<typename Point_t> inline
double Helmert2D<Point_t>::rotation() const
{
    return mRotation;
}

template<typename Point_t>
double Helmert2D<Point_t>::scale() const
{
    return mScale;
}

template<typename Point_t> inline
void Helmert2D<Point_t>::setParameters(double tx,
                                       double ty,
                                       double scale,
                                       double rotation)
{
    this->tx = tx;
    this->ty = ty;
    mScale = scale;
    mRotation = rotation;
    update();
}

template<typename Point_t> inline
void Helmert2D<Point_t>::setRotation(double rotation) 
{
    mRotation = rotation;
    update();
}

template<typename Point_t> inline
void Helmert2D<Point_t>::setScale(double scale)
{
    mScale = scale;
    update();
}

template<typename Point_t> inline
Helmert2D<Point_t> Helmert2D<Point_t>::inverse() const
{
    return Helmert2D<Point_t>(txi, tyi, sqrt(ai*ai + bi*bi), atan2(bi, ai));
}

template<typename Point_t> inline
Helmert2D<Point_t> &Helmert2D<Point_t>::operator = (const Helmert2D<Point_t> &helmert)
{
    if (this != &helmert) {
        Transform2D<Point_t>::operator = (helmert);
        this->tx = helmert.tx;
        this->ty = helmert.ty;
        this->mRotation = helmert.mRotation;
        this->mScale = helmert.mScale;
        this->a = helmert.a;
        this->b = helmert.b;
        this->ai = helmert.ai;
        this->bi = helmert.bi;
        this->txi = helmert.txi;
        this->tyi = helmert.tyi;
    }

    return *this;
}

template<typename Point_t> inline
Helmert2D<Point_t> &Helmert2D<Point_t>::operator = (Helmert2D<Point_t> &&helmert) TL_NOEXCEPT
{
    if (this != &helmert) {
        Transform2D<Point_t>::operator = (std::forward<Transform2D<Point_t>>(helmert));
        this->tx = helmert.tx;
        this->ty = helmert.ty;
        this->mRotation = helmert.mRotation;
        this->mScale = helmert.mScale;
        this->a = helmert.a;
        this->b = helmert.b;
        this->ai = helmert.ai;
        this->bi = helmert.bi;
        this->txi = helmert.txi;
        this->tyi = helmert.tyi;
    }

    return *this;
}

template<typename Point_t> template <typename Point_t2> inline
Helmert2D<Point_t>::operator Helmert2D<Point_t2>() const
{
    return Helmert2D<Point_t2>(this->tx,
                               this->ty,
                               this->mScale,
                               this->mRotation);
}

template<typename Point_t> template<typename Point_t2> inline
Helmert2D<Point_t>::operator Affine<Point_t2>() const
{
    return Affine<Point_t2>(this->tx,
                            this->ty,
                            this->mScale,
                            this->mScale,
                            this->mRotation);
}

template<typename Point_t>  template <typename Point_t2> inline
Helmert2D<Point_t>::operator Translation<Point_t2>() const
{
    return Translation<Point_t2>(this->tx, this->ty);
}

template<typename Point_t>  template <typename Point_t2> inline
Helmert2D<Point_t>::operator Rotation<Point_t2>() const
{
    return Rotation<Point_t2>(this->mRotation);
}

template<typename Point_t> inline
void Helmert2D<Point_t>::update()
{
    a = mScale * cos(mRotation);
    b = mScale * sin(mRotation);

    updateInv();
}

template<typename Point_t>
void Helmert2D<Point_t>::updateInv()
{
    // Transformación inversa
    //double det = a * d - c * b;
    double det = a * a + b * b;
    if (det == 0.) {
        Message::error("determinant null");
    } else {
        ai = a / det;
        bi = -b / det;
        txi = (-a * tx - b * ty) / det;
        tyi = (-a * ty + b * tx) / det;
    }
}


/*! \} */ // end of trf2DGroup

/*! \} */ // end of trfGroup

/*! \} */ // end of geometry

} // End namespace tl
