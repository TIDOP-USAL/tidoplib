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

#ifndef TL_GEOMETRY_TRANSFORM_SCALING_H
#define TL_GEOMETRY_TRANSFORM_SCALING_H

#include "config_tl.h"

#include "tidop/geometry/transform/transform.h"
#include "tidop/geometry/transform/helmert2d.h"
#include "tidop/geometry/transform/affine.h"
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
 * \brief Escala
 *
 * Transformación que aplica un cambio de escala
 */
template<typename Point_t>
class Scaling
  : public Transform2D<Point_t>
{

public:

  /*!
   * \brief Constructora por defecto
   */
  Scaling();

  /*!
   * \brief Constructora
   * \param[in] scale Escala
   */
  Scaling(double scale);

  /*!
   * \brief Constructor de copia
   * \param[in] obj Objeto que se copia
   */
  Scaling(const Scaling &obj);

  /*!
   * \brief Constructor de movimiento
   * \param[in] obj Objeto que se copia
   */
  Scaling(Scaling &&obj) TL_NOEXCEPT;

  /*!
   * \brief Cálculo de la traslación
   *
   * Calcula la traslación entre dos sistemas diferentes a partir
   * de dos conjuntos de puntos en cada sistema
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
   * \brief Aplica una traslación a un punto
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
   * \brief Aplica una traslación a un punto
   * \param[in] ptIn Punto de entrada
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Punto de salida
   * \see Transform::Order
   */
  Point_t transform(const Point_t &ptIn,
                    Transform::Order trfOrder = Transform::Order::direct) const override;

  /*!
   * \brief Transforma un conjunto de puntos en otro aplicando una traslación
   * \param[in] ptsIn Puntos de entrada
   * \param[out] ptsOut Puntos de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Transform::Status
   * \see Transform::Order, Transform::Status
   */
  Transform::Status transform(const std::vector<Point_t> &ptsIn,
                              std::vector<Point_t> &ptsOut,
                              Transform::Order trfOrder = Transform::Order::direct) const override;

  /*!
   * \brief Devuelve la escala de la transformación
   * \return Escala
   */
  double scale() const;

  void setScale(double scale);

  /*!
   * \brief Transformación inversa
   * \return
   */
  Scaling inverse() const;

  /*!
   * \brief Operador de asignación
   * \param[in] obj Objeto Scaling que se copia
   */
  Scaling &operator = (const Scaling<Point_t> &obj);

  /*!
   * \brief Operador de asignación de movimiento
   * \param[in] obj Objeto Scaling que se mueve
   */
  Scaling &operator = (Scaling<Point_t> &&obj) TL_NOEXCEPT;


  // conversión a otras transformaciones
  template <typename Point_t2>
  explicit operator Scaling<Point_t2>() const;

  template <typename Point_t2>
  explicit operator Helmert2D<Point_t2>() const;

  template <typename Point_t2>
  explicit operator Affine<Point_t2>() const;

  bool isNull() const override
  {
    return (mScale == 1.);
  }

protected:

  /*!
   * \brief Escala
   */
  double mScale;

};


/// Implementación Scaling

template<typename Point_t> inline
Scaling<Point_t>::Scaling()
  : Transform2D<Point_t>(Transform::Type::scaling, 1),
    mScale(1.)
{
}

template<typename Point_t> inline
Scaling<Point_t>::Scaling(double scale)
  : Transform2D<Point_t>(Transform::Type::scaling, 1),
    mScale(scale)
{
}

template<typename Point_t> inline
Scaling<Point_t>::Scaling(const Scaling &obj)
  : Transform2D<Point_t>(obj),
    mScale(obj.mScale)
{
}

template<typename Point_t> inline
Scaling<Point_t>::Scaling(Scaling &&obj) TL_NOEXCEPT
  : Transform2D<Point_t>(std::forward<Transform2D<Point_t>>(obj)),
    mScale(obj.mScale)
{
}

template<typename Point_t> inline
Transform::Status Scaling<Point_t>::compute(const std::vector<Point_t> &pts1,
                                            const std::vector<Point_t> &pts2,
                                            std::vector<double> *error,
                                            double *rmse)
{
  size_t n1 = pts1.size();
  size_t n2 = pts2.size();

  if (n1 != n2) {
    msgError("Sets of points with different size. Size pts1 = %zu and size pts2 = %zui", n1, n2);
    return Transform::Status::failure;
  }

  if (!this->isNumberOfPointsValid(n1)) {
    msgError("Invalid number of points: %zu < %i", n1, this->mMinPoint);
    return Transform::Status::failure;
  }

  Transform::Status status = Transform::Status::success;
  size_t m = n1 * static_cast<size_t>(this->mDimensions);
  size_t n = 4;

  try {

    math::Matrix<double> A(m, n, 0);
    math::Vector<double> B(m);

    for (size_t i = 0, r = 0; i < n1; i++, r++) {

      A.at(r, 0) = pts1[i].x;
      //A.at(r, 1) = 0;

      B[r] = pts2[i].x;

      r++;

      //A.at(r, 0) = 0;
      A.at(r, 1) = pts1[i].y;
      
      B[r] = pts2[i].y;

    }

    math::SingularValueDecomposition<math::Matrix<double>> svd(A);
    math::Vector<double> C = svd.solve(B);

    mScale = (C[0] + C[1])/2.;

    if (error) {
      if (rmse) *rmse = this->_rootMeanSquareError(pts1, pts2, error);
    }

  } catch (std::exception &e) {
    MessageManager::release(e.what(), tl::MessageLevel::msg_error);
    status = Transform::Status::failure;
  }

  return status;
}

template<typename Point_t> inline
Transform::Status Scaling<Point_t>::transform(const Point_t &ptIn,
                                              Point_t &ptOut,
                                              Transform::Order trfOrder) const
{
  using sub_type = typename Point_t::value_type;

  Transform::Status r_status = Transform::Status::success;
  try {
    if (trfOrder == Transform::Order::direct){
      ptOut.x = static_cast<sub_type>(mScale * ptIn.x);
      ptOut.y = static_cast<sub_type>(mScale * ptIn.y);
    } else {
      double inv = 1. / mScale;
      ptOut.x = static_cast<sub_type>(inv * ptIn.x);
      ptOut.y = static_cast<sub_type>(inv * ptIn.y);
    }
  } catch (tl::Exception &e ) {
    MessageManager::release(
          MessageManager::Message("Scaling transformation error: %s", e.what()).message(),
          tl::MessageLevel::msg_error);
    r_status = Transform::Status::failure;
  }
  return r_status;
}

template<typename Point_t> inline
Point_t Scaling<Point_t>::transform(const Point_t &ptIn, 
                                    Transform::Order trfOrder) const
{
  Point_t pt_aux;
  Transform::Status r_status = transform(ptIn, pt_aux, trfOrder);
  return r_status == Transform::Status::success ? pt_aux : ptIn;
}

template<typename Point_t> inline
Transform::Status Scaling<Point_t>::transform(const std::vector<Point_t> &ptsIn,
                                              std::vector<Point_t> &ptsOut,
                                              Transform::Order trfOrder) const
{
  this->formatVectorOut(ptsIn, ptsOut);
  Transform::Status r_status = Transform::Status::success;
  for (size_t i = 0; i < ptsIn.size(); i++) {
    r_status = transform(ptsIn[i], ptsOut[i], trfOrder);
    if ( r_status == Transform::Status::failure ) break;
  }
  return r_status;
}

template<typename Point_t>
double Scaling<Point_t>::scale() const
{
  return mScale;
}

template<typename Point_t>
void Scaling<Point_t>::setScale(double scale)
{
  mScale = scale;
}

template<typename Point_t> inline
Scaling<Point_t> Scaling<Point_t>::inverse() const
{
  return Scaling<Point_t>(1./mScale);
}

template<typename Point_t>
Scaling<Point_t> &Scaling<Point_t>::operator =(const Scaling<Point_t> &obj)
{
  if (this != &obj) {
    Transform2D<Point_t>::operator = (obj);
    this->mScale = obj.mScale;
  }
  return *this;
}

template<typename Point_t>
Scaling<Point_t> &Scaling<Point_t>::operator =(Scaling<Point_t> &&obj) TL_NOEXCEPT
{
  if (this != &obj) {
    Transform2D<Point_t>::operator = (std::forward<Transform2D<Point_t>>(obj));
    this->mScale = obj.mScale;
  }
  return *this;
}

template<typename Point_t> template<typename Point_t2> inline
Scaling<Point_t>::operator Scaling<Point_t2>() const
{
  return Scaling<Point_t2>(mScale);
}

template<typename Point_t> template<typename Point_t2> inline
Scaling<Point_t>::operator Helmert2D<Point_t2>() const
{
  return Helmert2D<Point_t2>(0., 0., mScale, 0.);
}

template<typename Point_t> template<typename Point_t2> inline
Scaling<Point_t>::operator Affine<Point_t2>() const
{
  return Affine<Point_t2>(0., 0., mScale, mScale, 0.);
}


/*! \} */ // end of trf2DGroup

/*! \} */ // end of trfGroup

/*! \} */ // end of geometry

} // End namespace tl

#endif // TL_GEOMETRY_TRANSFORM_SCALING_H
