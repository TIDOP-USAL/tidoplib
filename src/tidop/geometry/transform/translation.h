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
#include "tidop/geometry/transform/helmert2d.h"
#include "tidop/geometry/transform/affine.h"
#include "tidop/math/algebra/matrix.h"
#include "tidop/math/algebra/vector.h"
#include "tidop/math/algebra/svd.h"

namespace tl
{
namespace geom
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
 * \brief Traslación
 *
 * Transformación que aplica una traslación en el plano a un conjunto de puntos
 */
template<typename Point_t>
class TL_DEPRECATED("Translation<T, 2>", "3.0") Translation
    : public Transform2D<Point_t>
{

public:

    /*!
     * \brief Traslación en X
     */
    double tx;

    /*!
     * \brief Traslación en Y
     */
    double ty;

public:

    /*!
     * \brief Constructora por defecto
     */
    Translation();

    /*!
     * \brief Constructora
     * \param[in] tx Traslación en el eje X
     * \param[in] ty Traslación en el eje Y
     */
    Translation(double tx, double ty);

    /*!
     * \brief Constructor de copia
     * \param[in] translation Objeto que se copia
     */
    Translation(const Translation &translation);

    /*!
     * \brief Constructor de movimiento
     * \param[in] translation Objeto que se copia
     */
    Translation(Translation &&translation) TL_NOEXCEPT;


    Transform::Status compute(const std::vector<Point_t> &pts1,
                              const std::vector<Point_t> &pts2,
                              std::vector<double> *error = nullptr,
                              double *rmse = nullptr) override;
    Transform::Status transform(const Point_t &ptIn,
                                Point_t &ptOut,
                                Transform::Order trfOrder = Transform::Order::direct) const override;
    Point_t transform(const Point_t &ptIn,
                      Transform::Order trfOrder = Transform::Order::direct) const override;
    Transform::Status transform(const std::vector<Point_t> &ptsIn,
                                std::vector<Point_t> &ptsOut,
                                Transform::Order trfOrder = Transform::Order::direct) const override;

    /*!
     * \brief Transformación inversa
     * \return
     */
    Translation inverse() const;

    /*!
     * \brief Operador de asignación
     * \param[in] translation Objeto Translation que se copia
     */
    Translation &operator = (const Translation<Point_t> &translation);

    /*!
     * \brief Operador de asignación de movimiento
     * \param[in] translation Objeto Translation que se mueve
     */
    Translation &operator = (Translation<Point_t> &&translation) TL_NOEXCEPT;


    // conversión a otras transformaciones
    template <typename Point_t2>
    explicit operator Translation<Point_t2>() const;

    template <typename Point_t2>
    explicit operator Helmert2D<Point_t2>() const;

    template <typename Point_t2>
    explicit operator Affine<Point_t2>() const;

    bool isNull() const override
    {
        return (tx == 0. &&
                ty == 0.);
    }

};


/// Implementación Translate

template<typename Point_t> inline
Translation<Point_t>::Translation()
  : Transform2D<Point_t>(Transform::Type::translation, 1),
    tx(0.),
    ty(0.)
{
}

template<typename Point_t> inline
Translation<Point_t>::Translation(double tx, double ty)
  : Transform2D<Point_t>(Transform::Type::translation, 1),
    tx(tx),
    ty(ty)
{
}

template<typename Point_t> inline
Translation<Point_t>::Translation(const Translation &translation)
  : Transform2D<Point_t>(translation),
    tx(translation.tx),
    ty(translation.ty)
{
}

template<typename Point_t> inline
Translation<Point_t>::Translation(Translation &&translation) TL_NOEXCEPT
  : Transform2D<Point_t>(std::forward<Transform2D<Point_t>>(translation)),
    tx(translation.tx),
    ty(translation.ty)
{
}

template<typename Point_t> inline
Transform::Status Translation<Point_t>::compute(const std::vector<Point_t> &pts1,
                                                const std::vector<Point_t> &pts2,
                                                std::vector<double> *error,
                                                double *rmse)
{
    size_t n1 = pts1.size();
    size_t n2 = pts2.size();

    if (n1 != n2) {
        //msgError("Sets of points with different size. Size pts1 = %zu and size pts2 = %zu", n1, n2);
        return Transform::Status::failure;
    }

    if (!this->isNumberOfPointsValid(n1)) {
        //msgError("Invalid number of points: %zu < %i", n1, this->mMinPoint);
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
            //A.at(r, 1) = 0;
            A.at(r, 2) = 1;
            //A.at(r, 3) = 0;

            B[r] = pts2[i].x;

            r++;

            //A.at(r, 0) = 0;
            A.at(r, 1) = pts1[i].y;
            //A.at(r, 2) = 0;
            A.at(r, 3) = 1;

            B[r] = pts2[i].y;

        }

        SingularValueDecomposition<Matrix<double>> svd(A);
        Vector<double> C = svd.solve(B);

        tx = C[2];
        ty = C[3];

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
Transform::Status Translation<Point_t>::transform(const Point_t &ptIn,
                                                  Point_t &ptOut,
                                                  Transform::Order trfOrder) const
{
    using sub_type = typename Point_t::value_type;

    Transform::Status r_status = Transform::Status::success;
    Point_t pt_aux;
    if (std::is_integral<sub_type>::value) {
        pt_aux.x = static_cast<sub_type>(round(tx));
        pt_aux.y = static_cast<sub_type>(round(ty));
    } else {
        pt_aux.x = static_cast<sub_type>(tx);
        pt_aux.y = static_cast<sub_type>(ty);
    }
    ptOut = (trfOrder == Transform::Order::direct) ? ptIn + pt_aux : ptIn - pt_aux;
    return r_status;
}

template<typename Point_t> inline
Point_t Translation<Point_t>::transform(const Point_t &ptIn,
                                        Transform::Order trfOrder) const
{
    Point_t pt_aux;
    Transform::Status r_status = transform(ptIn, pt_aux, trfOrder);
    return r_status == Transform::Status::success ? pt_aux : ptIn;
}

template<typename Point_t> inline
Transform::Status Translation<Point_t>::transform(const std::vector<Point_t> &ptsIn,
                                                  std::vector<Point_t> &ptsOut,
                                                  Transform::Order trfOrder) const
{
    this->formatVectorOut(ptsIn, ptsOut);
    Transform::Status r_status = Transform::Status::success;
    for (size_t i = 0; i < ptsIn.size(); i++) {
        r_status = transform(ptsIn[i], ptsOut[i], trfOrder);
        if (r_status == Transform::Status::failure) break;
    }

    return r_status;
}

template<typename Point_t> inline
Translation<Point_t> Translation<Point_t>::inverse() const
{
    return Translation<Point_t>(-this->tx, -this->ty);
}

template<typename Point_t>
Translation<Point_t> &Translation<Point_t>::operator =(const Translation<Point_t> &translation)
{
    if (this != &translation) {
        Transform2D<Point_t>::operator = (translation);
        this->tx = translation.tx;
        this->ty = translation.ty;
    }

    return *this;
}

template<typename Point_t>
Translation<Point_t> &Translation<Point_t>::operator =(Translation<Point_t> &&translation) TL_NOEXCEPT
{
    if (this != &translation) {
        Transform2D<Point_t>::operator = (std::forward<Transform2D<Point_t>>(translation));
        this->tx = translation.tx;
        this->ty = translation.ty;
    }
    return *this;
}

template<typename Point_t> template<typename Point_t2> inline
Translation<Point_t>::operator Translation<Point_t2>() const
{
    return Translation<Point_t2>(this->tx, this->ty);
}

template<typename Point_t> template<typename Point_t2> inline
Translation<Point_t>::operator Helmert2D<Point_t2>() const
{
    return Helmert2D<Point_t2>(this->tx, this->ty, 1., 0.);
}
  
template<typename Point_t> template<typename Point_t2> inline
Translation<Point_t>::operator Affine<Point_t2>() const
{
    return Affine<Point_t2>(this->tx, this->ty, 1., 1., 0.);
}



/*! \} */ // end of trf2DGroup

/*! \} */ // end of trfGroup

/*! \} */ // end of geometry

} // End namespace geom
} // End namespace tl

