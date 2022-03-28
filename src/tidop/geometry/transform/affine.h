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

#ifndef TL_GEOMETRY_TRANSFORM_AFFINE_H
#define TL_GEOMETRY_TRANSFORM_AFFINE_H

#include "config_tl.h"

#include "tidop/core/messages.h"
#include "tidop/geometry/transform/transform.h"
#include "tidop/math/algebra/rotation_matrix.h"
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
 * \brief Transformación afín
 *
 * La Transformación Afín expresa la relación que existe (o la transformación que es 
 * preciso realizar) entre dos sistemas cartesianos que discrepan en la situación del 
 * origen, en la orientación de los ejes y en la unidad de medida a lo largo de los 
 * mismos de manera que dicha variación en unidad de medida es constante a lo largo 
 * de cada eje pero no entre los dos ejes.
 *
 * \f[ a =  scaleX * cos(rotation)\f]
 * \f[ b = -scaleY * sin(rotation)\f]
 * \f[ c =  scaleX * sin(rotation)\f]
 * \f[ d =  scaleY * cos(rotation)\f]
 *
 * \f[ x' = a * x + b * y + x0\f]
 * \f[ y' = c * x + d * y + y0\f]
 */
template<typename Point_t>
class Affine
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
  Affine();

  /*!
   * \brief Constructor
   * \param[in] tx Traslación en el eje X
   * \param[in] ty Traslación en el eje Y
   * \param[in] scaleX Escala en el eje X
   * \param[in] scaleY Escala en el eje Y
   * \param[in] rotation Rotación
   */
  Affine(double tx, 
         double ty, 
         double scaleX, 
         double scaleY, 
         double rotation);

  /*!
   * \brief Constructor a partir de una matriz con los coeficientes
   * \f[
   *   A=\begin{bmatrix}
   *   a & b & x0 \\
   *   c & d & y0 \\
   * \end{bmatrix}
   * \f]
   *
   * \f[ a =  scaleX * cos(rotation)\f]
   * \f[ b = -scaleY * sin(rotation)\f]
   * \f[ c =  scaleX * sin(rotation)\f]
   * \f[ d =  scaleY * cos(rotation)\f]
   *
   * \param[in] mat Matriz de coficientes
   */
  Affine(const math::Matrix<double, 2, 3> &mat);

  ~Affine() override = default;

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
   * \brief Transforma un conjunto de puntos en otro aplicando una transformación afín
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
   * \brief Aplica una transformación afín a un punto
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
   * \brief Aplica una transformación afín a un punto
   * \param[in] ptIn Punto de entrada
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Punto de salida
   * \see Transform::Order
   */
  Point_t transform(const Point_t &ptIn,
                    Transform::Order trfOrder = Transform::Order::direct) const override;

#ifdef TL_ENABLE_DEPRECATED_METHODS
  /*!
   * \brief Devuelve los coeficientes de la transformación
   * \param[out] a Coeficiente a
   * \param[out] b Coeficiente b
   * \param[out] c Coeficiente c
   * \param[out] d Coeficiente d
   * \deprecated Use 'parameters(double *a, double *b, double *c, double *d)' en su lugar
   */
  TL_DEPRECATED("parameters", "2.0")
  void getParameters(double *a, double *b, double *c, double *d);

  /*!
   * \brief Devuelve el giro
   * \return Ángulo de rotación en radianes
   * \deprecated Use 'rotation()' en su lugar
   */
  TL_DEPRECATED("rotation", "2.0")
  double getRotation() const;

  /*!
   * \brief Devuelve la escala correspondiente al eje X
   * \return Escala eje X
   * \deprecated Use 'scaleX()' en su lugar
   */
  TL_DEPRECATED("scaleX", "2.0")
  double getScaleX() const;

  /*!
   * \brief Devuelve la escala correspondiente al eje Y
   * \return Escala eje Y
   * \deprecated Use 'scaleY()' en su lugar
   */
  TL_DEPRECATED("scaleY", "2.0")
  double getScaleY() const;
#endif // TL_ENABLE_DEPRECATED_METHODS

  /*!
   * \brief Devuelve los coeficientes de la transformación
   * \f[
   * \begin{bmatrix}
   *   a & b & x0 \\
   *   c & d & y0 \\
   * \end{bmatrix}
   * \f]
   */
  math::Matrix<double,2,3> parameters() const;

  /*!
   * \brief Devuelve el giro
   * \return Ángulo de rotación en radianes
   */
  double rotation() const;

  /*!
   * \brief Devuelve la escala correspondiente al eje X
   * \return Escala eje X
   */
  double scaleX() const;

  /*!
   * \brief Devuelve la escala correspondiente al eje Y
   * \return Escala eje Y
   */
  double scaleY() const;

  /*!
   * \brief Establece los parámetros
   * \param[in] tx Traslación en x
   * \param[in] ty Traslación en y
   * \param[in] scaleX Escala en x
   * \param[in] scaleY Escala en y
   * \param[in] rotation Rotación
   */
  void setParameters(double tx, 
                     double ty, 
                     double scaleX, 
                     double scaleY, 
                     double rotation);

  /*!
   * \brief Establece los parámetros
   * \f[ a =  scaleX * cos(rotation)\f]
   * \f[ b = -scaleY * sin(rotation)\f]
   * \f[ c =  scaleX * sin(rotation)\f]
   * \f[ d =  scaleY * cos(rotation)\f]
   * \param[in] a Coeficiente a
   * \param[in] b Coeficiente b
   * \param[in] c Coeficiente c
   * \param[in] d Coeficiente d
   * \param[in] tx Traslación en x
   * \param[in] ty Traslación en y
   */
  void setParameters(double a, 
                     double b, 
                     double c, 
                     double d, 
                     double tx,
                     double ty);

  /*!
   * \brief Establece los parámetros a partir de una matriz con los coeficientes
   * \f[
   *   A=\begin{bmatrix}
   *   a & b & x0 \\
   *   c & d & y0 \\
   * \end{bmatrix}
   * \f]
   *
   * \f[ a =  scaleX * cos(rotation)\f]
   * \f[ b = -scaleY * sin(rotation)\f]
   * \f[ c =  scaleX * sin(rotation)\f]
   * \f[ d =  scaleY * cos(rotation)\f]
   *
   * \param[in] mat Matriz de coficientes
   */
  void setParameters(const math::Matrix<double, 2, 3> &mat);

  /*!
   * \brief Establece la rotación de la transformación
   * \param[in] rotation Ángulo de rotación en radianes
   */
  void setRotation(double rotation);

  /*!
   * \brief Establece la escala de la transformación en el eje X
   * \param[in] scaleX Escala de la transformación
   */
  void setScaleX(double scaleX);

  /*!
   * \brief Establece la escala de la transformación en el eje Y
   * \param[in] scaleY Escala de la transformación
   */
  void setScaleY(double scaleY);

  bool isNull() const
  {
    return (tx == 0. &&
            ty == 0. &&
            mScaleX == 1. &&
            mScaleY == 1. &&
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


  double mScaleX;
  double mScaleY;
  double mRotation;
  double a;
  double b;
  double c;
  double d;
  double ai;
  double bi;
  double ci;
  double di;
  double txi;
  double tyi;

};


template<typename Point_t> inline
Affine<Point_t>::Affine()
  : Transform2D<Point_t>(Transform::Type::affine, 3),
    tx(0.),
    ty(0.),
    mScaleX(1.),
    mScaleY(1.),
    mRotation(0.)
{
  update();
}

template<typename Point_t> inline
Affine<Point_t>::Affine(double tx, 
                        double ty, 
                        double scaleX, 
                        double scaleY, 
                        double rotation)
  : Transform2D<Point_t>(Transform::Type::affine, 3),
    tx(tx),
    ty(ty),
    mScaleX(scaleX),
    mScaleY(scaleY),
    mRotation(rotation)
{
  update();
}

template<typename Point_t> inline
Affine<Point_t>::Affine(const math::Matrix<double, 2, 3> &mat)
  : Transform2D<Point_t>(Transform::Type::affine, 3)
{
  this->setParameters(mat.at(0,0), mat.at(0,1), mat.at(1,0), 
                      mat.at(1,1), mat.at(0,2), mat.at(2,2));
}

template<typename Point_t> inline
Transform::Status Affine<Point_t>::compute(const std::vector<Point_t> &pts1,
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
  size_t n = 6;

  try {

    math::Matrix<double> A(m, n, 0);
    math::Vector<double> B(m);

    for (size_t i = 0, r = 0; i < n1; i++, r++) {
      A.at(r, 0) = pts1[i].x;
      A.at(r, 1) = pts1[i].y;
      //A.at(r, 2) = 0;
      //A.at(r, 3) = 0;
      A.at(r, 4) = 1;
      //A.at(r, 5) = 0;

      B[r] = pts2[i].x;

      r++;

      //A.at(r, 0) = 0;
      //A.at(r, 1) = 0;
      A.at(r, 2) = pts1[i].x;
      A.at(r, 3) = pts1[i].y;
      //A.at(r, 4) = 0;
      A.at(r, 5) = 1;

      B[r] = pts2[i].y;
    }

    math::SingularValueDecomposition<math::Matrix<double>> svd(A);
    math::Vector<double> C = svd.solve(B);

    a = C[0];
    b = C[1];
    c = C[2];
    d = C[3];
    tx = C[4];
    ty = C[5];

    updateInv();

    mRotation = (atan2(c, a) + atan2(-b, d)) / 2.;
    mScaleX = math::module(a, c);
    mScaleY = math::module(b, d);

    if (error) {
      if (rmse) *rmse = this->_rootMeanSquareError(pts1, pts2, error);
    }
  } catch (std::exception &e) {
    msgError(e.what());
    status = Transform::Status::failure;
  }

  return status;
}

template<typename Point_t> inline
Transform::Status Affine<Point_t>::transform(const std::vector<Point_t> &ptsIn, 
                                             std::vector<Point_t> &ptsOut, 
                                             Transform::Order trfOrder) const
{
  Transform::Status r_status = Transform::Status::success;

  this->formatVectorOut(ptsIn, ptsOut);

  for (int i = 0; i < ptsIn.size(); i++) {
    r_status = transform(ptsIn[i], ptsOut[i], trfOrder);
    if ( r_status == Transform::Status::failure ) break;
  }

  return r_status;
}

template<typename Point_t> inline
Transform::Status Affine<Point_t>::transform(const Point_t &ptIn,
                                             Point_t &ptOut, 
                                             Transform::Order trfOrder) const
{
  Transform::Status r_status = Transform::Status::success;

  using sub_type = typename Point_t::value_type;
  sub_type x_aux = ptIn.x;

  try {
    if (trfOrder == Transform::Order::direct){
      ptOut.x = static_cast<sub_type>(a * x_aux + b * ptIn.y + tx);
      ptOut.y = static_cast<sub_type>(c * x_aux + d * ptIn.y + ty);
    } else {
      ptOut.x = static_cast<sub_type>(ai * x_aux + bi * ptIn.y + txi);
      ptOut.y = static_cast<sub_type>(ci * x_aux + di * ptIn.y + tyi);
    }
  } catch (std::exception &e ) {
    msgError("Affine transformation error: %s", e.what());
    r_status = Transform::Status::failure;
  }

  return r_status; 
}

template<typename Point_t> inline
Point_t Affine<Point_t>::transform(const Point_t &ptIn, 
                                   Transform::Order trfOrder) const
{
  Point_t r_pt{};

  using sub_type = typename Point_t::value_type;
  if (trfOrder == Transform::Order::direct){
    r_pt.x = static_cast<sub_type>(a * ptIn.x + b * ptIn.y + tx);
    r_pt.y = static_cast<sub_type>(c * ptIn.x + d * ptIn.y + ty);
  } else {
    r_pt.x = static_cast<sub_type>(ai * ptIn.x + bi * ptIn.y + txi);
    r_pt.y = static_cast<sub_type>(ci * ptIn.x + di * ptIn.y + tyi);
  }

  return r_pt;
}

#ifdef TL_ENABLE_DEPRECATED_METHODS
template<typename Point_t> inline
void Affine<Point_t>::getParameters(double *_a, double *_b, double *_c, double *_d)
{
  *_a = a;
  *_b = b;
  *_c = c;
  *_d = d;
}

template<typename Point_t> inline
double Affine<Point_t>::getRotation() const
{
  return mRotation;
}

template<typename Point_t>
double Affine::getScaleX() const
{
  return mScaleX;
}

template<typename Point_t>
double Affine::getScaleY() const
{
  return mScaleY;
}
#endif // TL_ENABLE_DEPRECATED_METHODS

template<typename Point_t>
math::Matrix<double, 2, 3> Affine<Point_t>::parameters() const
{
  math::Matrix<double, 2, 3> mat;
  mat.at(0, 0) = this->a;
  mat.at(0, 1) = this->b;
  mat.at(0, 2) = this->tx;
  mat.at(1, 0) = this->c;
  mat.at(1, 1) = this->d;
  mat.at(1, 2) = this->ty;
  return mat;
}

template<typename Point_t> inline
double Affine<Point_t>::rotation() const
{
  return mRotation;
}

template<typename Point_t>
double Affine<Point_t>::scaleX() const
{
  return mScaleX;
}

template<typename Point_t>
double Affine<Point_t>::scaleY() const
{
  return mScaleY;
}

template<typename Point_t> inline
void Affine<Point_t>::setParameters(double tx, 
                                    double ty, 
                                    double scaleX, 
                                    double scaleY, 
                                    double rotation)
{
  this->tx = tx;
  this->ty = ty;
  mScaleX = scaleX;
  mScaleY = scaleY;
  mRotation = rotation;

  this->update();
}

template<typename Point_t> inline
void Affine<Point_t>::setParameters(double a, 
                                    double b, 
                                    double c, 
                                    double d, 
                                    double tx, 
                                    double ty)
{
  this->a = a;
  this->b = b;
  this->c = c;
  this->d = d;
  this->tx = tx;
  this->ty = ty;

  mRotation = (atan2(b, a) + atan2(-c, d) ) / 2.;
  mScaleX = sqrt(a*a + b*b);
  mScaleY = sqrt(c*c + d*d);

  this->updateInv();
}

template<typename Point_t>
inline void Affine<Point_t>::setParameters(const math::Matrix<double, 2, 3> &mat)
{
  this->setParameters(mat.at(0,0), mat.at(0,1), 
                      mat.at(1,0), mat.at(1,1), 
                      mat.at(0,2), mat.at(2,2));
}

template<typename Point_t> inline
void Affine<Point_t>::setRotation(double rotation)
{
  mRotation = rotation;
  this->update();
}

template<typename Point_t> inline
void Affine<Point_t>::setScaleX(double scaleX)
{
  mScaleX = scaleX;
  this->update();
}

template<typename Point_t> inline
void Affine<Point_t>::setScaleY(double scaleY)
{
  mScaleY = scaleY;
  this->update();
}

template<typename Point_t> inline
void Affine<Point_t>::update()
{
  a =  mScaleX * cos(mRotation);
  b = -mScaleY * sin(mRotation);
  c =  mScaleX * sin(mRotation);
  d =  mScaleY * cos(mRotation);
  
  this->updateInv();
}

template<typename Point_t> inline
void Affine<Point_t>::updateInv()
{
  // Transformación inversa
  double det = a * d - c * b;
  if (det == 0.) {
    msgError("determinant null");
  } else {
    ai = d / det;
    bi = -b / det;
    ci = -c / det;
    di = a / det;
    TL_DISABLE_WARNING(TL_WARNING_C4244)
    this->txi = (-d * tx + b * ty) / det;
    this->tyi = (-a * ty + c * tx) / det;
    TL_ENABLE_WARNING(TL_WARNING_C4244)
  }
}

/*! \} */ // end of trf2DGroup

/*! \} */ // end of trfGroup

/*! \} */ // end of geometry

} // End namespace tl

#endif // TL_GEOMETRY_TRANSFORM_AFFINE_H
