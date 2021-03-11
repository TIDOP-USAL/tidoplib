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

#ifndef TL_GEOMETRY_TRANSFORM_ROTATION_H
#define TL_GEOMETRY_TRANSFORM_ROTATION_H

#include "config_tl.h"

#include "tidop/geometry/transform/transform.h"
#include "tidop/geometry/transform/translation.h"
#include "tidop/geometry/transform/helmert2d.h"
#include "tidop/geometry/transform/affine.h"

namespace tl
{



/*! \addtogroup trfGroup
 *  \{
 */

/*! \addtogroup trf2DGroup
 *  \{
 */



/*!
 * \brief Rotación
 *
 * Transformación que aplica una rotación en el plano a un conjunto de puntos 
 */
template<typename Point_t> 
class Rotation
  : public Transform2D<Point_t>
{

public:

  /*!
   * \brief Constructora por defecto
   */
  Rotation();

  /*!
   * \brief Constructora
   * \param[in] angle Ángulo en radianes
   */
  Rotation(double angle);

  /*!
   * \brief Constructor de copia
   * \param[in] rotation Objeto Rotation que se copia
   */
  Rotation(const Rotation &rotation);

  /*!
   * \brief Constructor de movimiento
   * \param[in] rotation Objeto Rotation que se copia
   */
  Rotation(Rotation &&rotation) TL_NOEXCEPT;

  /*!
   * \brief Calculo del ángulo de rotación
   *
   * Calcula la rotación entre dos sistemas de coordenadas a partir de dos 
   * conjuntos de puntos
   * 
   * <H3>Ejemplo:</H3>
   * \code
   * Rotation<cv::Point2d> rot;
   * std::vector<cv::Point2d> pts_in{ cv::Point2d(12.3, 34.3), cv::Point2d(10.6, 34.345), cv::Point2d(34.76, 54.26) };
   * std::vector<cv::Point2d> pts_out{ cv::Point2d(-7.869, 35.578), cv::Point2d(-9.33, 34.71), cv::Point2d(0.499, 64.43) };
   * rot.compute(pts_in, pts_out);
   * \endcode
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
   * \brief Devuelve el ángulo de la rotación
   * \return Ángulo en radianes
   */
  double angle() const;

  /*!
   * \brief Establece en ángulo de la rotación
   * \param[in] angle Ángulo en radianes
   */
  void setAngle(double angle);

  /*!
   * \brief Aplica una rotación a un punto
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
   * \brief Aplica una rotación a un punto
   * \param[in] ptIn Punto de entrada
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Punto de salida
   * \see Transform::Order
   */
  Point_t transform(const Point_t &ptIn,
                    Transform::Order trfOrder = Transform::Order::direct) const override;

  /*!
   * \brief Aplica una rotación a un conjunto de puntos
   *
   * Transforma un conjunto de puntos en un sistema cartesiano a otro
   * sistema aplicando un giro.
   *
   * <H3>Ejemplo:</H3>
   * \code
   * std::vector<PointD> pts_in{ 
   *   PointD(12.3, 34.3), 
   *   PointD(10.6, 34.345),
   *   PointD(34.76, 54.26) };
   *
   * std::vector<PointD> pts_out;
   *
   * Rotation<PointD> rot(0.562);
   * rot.transform(pts_in, &pts_out);
   * \endcode
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
   * \brief Transformación inversa
   * \return
   */
  Rotation inverse() const;

  /*!
   * \brief Operador de asignación
   * \param[in] rotation Objeto Rotation que se copia
   */
  Rotation &operator = (const Rotation<Point_t> &rotation);

  /*!
   * \brief Operador de asignación de movimiento
   * \param[in] rotation Objeto Rotation que se mueve
   */
  Rotation &operator = (Rotation<Point_t> &&rotation) TL_NOEXCEPT;


  template<typename Point_t2>
  explicit operator Rotation<Point_t2>() const;

  template<typename Point_t2>
  explicit operator Helmert2D<Point_t2>() const;

  template<typename Point_t2>
  explicit operator Affine<Point_t2>() const;

  bool isNull() const
  {
    return (mAngle == 0.);
  }

private:

  void update();

private:

  /*!
   * \brief Ángulo de rotación
   */
  double mAngle;

  /*!
   * \brief r1 = cos(angle);
   */
  double r1;

  /*!
   * \brief r2 = sin(angle);
   */
  double r2;

};

template<typename Point_t> inline
Rotation<Point_t>::Rotation()
  : Transform2D<Point_t>(Transform::Type::rotation, 1),
    mAngle(0.)
{
  update();
}

template<typename Point_t> inline
Rotation<Point_t>::Rotation(double angle)
  : Transform2D<Point_t>(Transform::Type::rotation, 1),
    mAngle(angle)
{
  update();
}

template<typename Point_t> inline
Rotation<Point_t>::Rotation(const Rotation &rotation)
  : Transform2D<Point_t>(rotation),
    mAngle(rotation.mAngle),
    r1(rotation.r1),
    r2(rotation.r2)
{
}

template<typename Point_t> inline
Rotation<Point_t>::Rotation(Rotation &&rotation) TL_NOEXCEPT
  : Transform2D<Point_t>(std::forward<Transform2D<Point_t>>(rotation)),
    mAngle(rotation.mAngle),
    r1(rotation.r1),
    r2(rotation.r2)
{
}

template<typename Point_t> inline
Transform::Status Rotation<Point_t>::compute(const std::vector<Point_t> &pts1, 
                                             const std::vector<Point_t> &pts2,
                                             std::vector<double> *error, 
                                             double *rmse)
{
  size_t n1 = pts1.size();
  size_t n2 = pts2.size();
  
  if (n1 != n2) {
    msgError("Sets of points with different size. Size pts1 = %zu and size pts2 = %zu", n1, n2);
    return Transform::Status::failure;
  } 

  if (!this->isNumberOfPointsValid(n1)) {
    msgError("Invalid number of points: %zu < %i", n1, this->mMinPoint);
    return Transform::Status::failure;
  }

  Transform::Status status = Transform::Status::success;
  
  size_t m = n1 * static_cast<size_t>(this->mDimensions);
  size_t n = 2;
  double *a = nullptr;
  double *b = nullptr;
  double *c = nullptr;

  try {

    a = new double[m * n];
    double *pa = a;
    b = new double[m];
    double *pb = b;
    c = new double[n];

    for (int i = 0; i < n1; i++) {
      *pa++ = pts1[i].x;
      *pa++ = -pts1[i].y;
      *pb++ = pts2[i].x;
      *pa++ = pts1[i].y;
      *pa++ = pts1[i].x;
      *pb++ = pts2[i].y;
    }
    
    solveSVD(m, n, a, b, c);
    r1 = c[0];
    r2 = c[1];
    mAngle = acos(r1);

    if (error) {
      if (rmse) *rmse = this->_rootMeanSquareError(pts1, pts2, error);
    }
  } catch (std::exception &e) {
    MessageManager::release(e.what(), tl::MessageLevel::msg_error);
    status = Transform::Status::failure;
  }

  delete[] a;
  delete[] b;
  delete[] c;

  return status;
}

#ifdef TL_ENABLE_DEPRECATED_METHODS
template<typename Point_t> inline
double Rotation<Point_t>::getAngle() const 
{ 
  return mAngle; 
}
#endif // TL_ENABLE_DEPRECATED_METHODS

template<typename Point_t> inline
double Rotation<Point_t>::angle() const
{
  return mAngle;
}

template<typename Point_t> inline
void Rotation<Point_t>::setAngle(double angle)
{ 
  mAngle = angle;
  update(); 
}

template<typename Point_t> inline
Transform::Status Rotation<Point_t>::transform(const Point_t &ptsIn,
                                               Point_t &ptsOut,
                                               Transform::Order trfOrder) const
{
  using sub_type = typename Point_t::value_type;

  Transform::Status r_status = Transform::Status::success;

  sub_type x_aux = ptsIn.x;
  if (trfOrder == Transform::Order::direct) {
    ptsOut.x = static_cast<sub_type>(x_aux * r1 - ptsIn.y * r2);
    ptsOut.y = static_cast<sub_type>(x_aux * r2 + ptsIn.y * r1);
  } else {
    ptsOut.x = static_cast<sub_type>(x_aux * r1 + ptsIn.y * r2);
    ptsOut.y = static_cast<sub_type>(ptsIn.y * r1 - x_aux * r2);
  }
  return r_status;
}

template<typename Point_t> inline
Point_t Rotation<Point_t>::transform(const Point_t &ptsIn, 
                                     Transform::Order trfOrder) const
{
  using sub_type = typename Point_t::value_type;

  Point_t out;
  if (trfOrder == Transform::Order::direct) {
    out.x = static_cast<sub_type>(ptsIn.x*r1 - ptsIn.y*r2);
    out.y = static_cast<sub_type>(ptsIn.x*r2 + ptsIn.y*r1);
  } else {
    out.x = static_cast<sub_type>(ptsIn.x*r1 + ptsIn.y*r2);
    out.y = static_cast<sub_type>(ptsIn.y*r1 - ptsIn.x*r2);
  }
  return out;
}

template<typename Point_t> inline
Transform::Status Rotation<Point_t>::transform(const std::vector<Point_t> &in,
                                               std::vector<Point_t> &out,
                                               Transform::Order trfOrder) const
{
  this->formatVectorOut(in, out);
  Transform::Status r_status = Transform::Status::success;
  for (int i = 0; i < in.size(); i++) {
    r_status = transform(in[i], out[i], trfOrder);
    if ( r_status == Transform::Status::failure ) break;
  }
  return r_status;
}

template<typename Point_t>
Rotation<Point_t> &Rotation<Point_t>::operator =(const Rotation<Point_t> &rotation)
{
  if (this != &rotation) {
    Transform2D<Point_t>::operator = (rotation);
    this->mAngle = rotation.mAngle;
    this->r1 = rotation.r1;
    this->r2 = rotation.r2;
  }
  return *this;
}

template<typename Point_t>
Rotation<Point_t> &Rotation<Point_t>::operator =(Rotation<Point_t> &&rotation) TL_NOEXCEPT
{
  if (this != &rotation) {
    Transform2D<Point_t>::operator = (std::forward<Transform2D<Point_t>>(rotation));
    this->mAngle = rotation.mAngle;
    this->r1 = rotation.r1;
    this->r2 = rotation.r2;
  }
  return *this;
}

template<typename Point_t> inline
Rotation<Point_t> Rotation<Point_t>::inverse() const
{
  return Rotation<Point_t>(-this->angle());
}

template<typename Point_t> template<typename Point_t2> inline
Rotation<Point_t>::operator Rotation<Point_t2>() const
{
  return Rotation<Point_t2>(mAngle);
}

template<typename Point_t> template<typename Point_t2> inline
Rotation<Point_t>::operator Helmert2D<Point_t2>() const
{
  return Helmert2D<Point_t2>(0., 0., 1., mAngle);
}
  
template<typename Point_t> template<typename Point_t2>  inline
Rotation<Point_t>::operator Affine<Point_t2>() const
{
  return Affine<Point_t2>(0., 0., 1., 1., mAngle);
}

template<typename Point_t> inline
void Rotation<Point_t>::update()
{
  r1 = cos(mAngle);
  r2 = sin(mAngle);
}


/*! \} */ // end of trfGroup

/*! \} */ // end of trf2DGroup


} // End namespace tl

#endif // TL_GEOMETRY_TRANSFORM_ROTATION_H
