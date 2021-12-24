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

#ifndef TL_GEOMETRY_TRANSFORM_PROJECTIVE_H
#define TL_GEOMETRY_TRANSFORM_PROJECTIVE_H

#include "config_tl.h"

#include "tidop/geometry/transform/transform.h"
#include "tidop/math/mathutils.h"
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
 * \brief Transformación Projectiva
 *
 * La Transformación Projectiva expresa la relación que existe entre dos planos.
 *
 * <BLOCKQUOTE>
 *    x' = ( a * x + b * y + c ) / ( g * x + h * y + 1 ) <BR>
 *    y' = ( d * x + e * y + f ) / ( g * x + h * y + 1 )
 * </BLOCKQUOTE>
 */
template<typename Point_t>
class Projective
  : public Transform2D<Point_t>
{

public:

  /*!
   * \brief Constructor por defecto
   */
  Projective();

  /*!
   * \brief Constructor
   * \param[in] a
   * \param[in] b
   * \param[in] c
   * \param[in] d
   * \param[in] e
   * \param[in] f
   * \param[in] g
   * \param[in] h
   */
  Projective(double a, double b, 
             double c, double d, 
             double e, double f, 
             double g, double h);

  ~Projective() override = default;

  /*!
   * \brief Calcula la transformación proyectiva entre dos sistemas
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
   * \brief Transforma un conjunto de puntos en otro aplicando una 
   * transformación proyectiva
   *
   * <H3>Ejemplo:</H3>
   * \code
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
   * \brief Aplica una transformación proyectiva a un punto
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
   * \brief Aplica una transformación proyectiva a un punto
   * \param[in] ptIn Punto de entrada
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Punto de salida
   * \see Transform::Order
   */
  Point_t transform(const Point_t &ptIn, 
                    Transform::Order trfOrder = Transform::Order::direct) const override;

  /*!
   * \brief Establece los parámetros
   * \param[in] a 
   * \param[in] b 
   * \param[in] c
   * \param[in] d
   * \param[in] e
   * \param[in] f
   * \param[in] g
   * \param[in] h
   */
  void setParameters(double a, double b, 
                     double c, double d, 
                     double e, double f, 
                     double g, double h);

  void getParameters(double *a, double *b, 
                     double *c, double *d, 
                     double *e, double *f, 
                     double *g, double *h);

  bool isNull() const
  {
    return (a == 1. &&
            b == 0. &&
            c == 0. &&
            d == 0. &&
            e == 1. &&
            f == 0. &&
            g == 0. &&
            h == 0.);
  }

private:

  /*!
   * \brief Actualiza la transformación
   */
  void update();

private:

  /*!
   * \brief Parámetro a
   */
  double a;

  /*!
   * \brief Parámetro b
   */
  double b;

  /*!
   * \brief Parámetro c
   */
  double c;

  /*!
   * \brief Parámetro d
   */
  double d;

  /*!
   * \brief Parámetro e
   */
  double e;

  /*!
   * \brief Parámetro f
   */
  double f;

  /*!
   * \brief Parámetro g
   */
  double g;
  
  /*!
   * \brief Parámetro h
   */
  double h;

  /*!
   * \brief Parámetro 'a' transformación inversa
   */
  double ai;

  /*!
   * \brief Parámetro 'b' transformación inversa
   */
  double bi;

  /*!
   * \brief Parámetro 'c' transformación inversa
   */
  double ci;

  /*!
   * \brief Parámetro 'd' transformación inversa
   */
  double di;

  /*!
   * \brief Parámetro 'e' transformación inversa
   */
  double ei;

  /*!
   * \brief Parámetro 'f' transformación inversa
   */
  double fi;

  /*!
   * \brief Parámetro 'g' transformación inversa
   */
  double gi;
  
  /*!
   * \brief Parámetro 'h' transformación inversa
   */
  double hi;

};


template<typename Point_t> inline
Projective<Point_t>::Projective()
  : Transform2D<Point_t>(Transform::Type::projective, 4),
    a(1),
    b(0),
    c(0),
    d(0),
    e(1),
    f(0),
    g(0),
    h(0)
{
  update();
}

template<typename Point_t> inline
Projective<Point_t>::Projective(double a, double b, 
                                double c, double d, 
                                double e, double f, 
                                double g, double h)
  : Transform2D<Point_t>(Transform::Type::projective, 4),
    a(a),
    b(b),
    c(c),
    d(d),
    e(e),
    f(f),
    g(g),
    h(h)
{
  update();
}

template<typename Point_t> inline
Transform::Status Projective<Point_t>::compute(const std::vector<Point_t> &pts1, 
                                               const std::vector<Point_t> &pts2,
                                               std::vector<double> *error,
                                               double *rmse)
{
  int n1 = pts1.size();
  int n2 = pts2.size();

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
  size_t n = 8;

  try {

    math::Matrix<double> A(m, n, 0);
    math::Vector<double> B(m);

    for (size_t i = 0, r = 0; i < n1; i++, r++) {

      A.at(r, 0) = pts1[i].x;
      A.at(r, 1) = pts1[i].y;
      A.at(r, 2) = 1;
      A.at(r, 3)  = 0;
      A.at(r, 4)  = 0;
      A.at(r, 5)  = 0;
      A.at(r, 6)  = -pts1[i].x * pts2[i].x;
      A.at(r, 7)  = -pts2[i].x * pts1[i].y;
      
      B[r] = pts2[i].x;

      r++;

      A.at(r, 0) = 0;
      A.at(r, 1) = 0;
      A.at(r, 2) = 0;
      A.at(r, 3) = pts1[i].x;
      A.at(r, 4) = pts1[i].y;
      A.at(r, 5) = 1;
      A.at(r, 6) = -pts2[i].y * pts1[i].x;
      A.at(r, 7) = -pts2[i].y * pts1[i].y;

      B[r] = pts2[i].y;
    }

    math::SingularValueDecomposition<math::Matrix<double>> svd(A);
    math::Vector<double> C = svd.solve(B);

    a = C[0];
    b = C[1];
    c = C[2];
    d = C[3];
    e = C[4];
    f = C[5];
    g = C[6];
    h = C[7];

    update();

    if (error) {
      if (rmse) *rmse = this->_rootMeanSquareError(pts1, pts2, error);
    }

  } catch (std::exception &e) {
    msgError(e.what());
    status = Transform::Status::failure;
  }

  //double *A = nullptr;
  //double *B = nullptr;
  //double *C = nullptr;

  //try {

  //  A = new double[m * n];
  //  double *pa = A;
  //  B = new double[m];
  //  double *pb = B;
  //  C = new double[n];

  //  for (int i = 0; i < n1; i++) {
  //    *pa++ = pts1[i].x;
  //    *pa++ = pts1[i].y;
  //    *pa++ = 1;
  //    *pa++ = 0;
  //    *pa++ = 0;
  //    *pa++ = 0;
  //    *pa++ = -pts1[i].x * pts2[i].x;
  //    *pa++ = -pts2[i].x * pts1[i].y;
  //    *pb++ = pts2[i].x;
  //    *pa++ = 0;
  //    *pa++ = 0;
  //    *pa++ = 0;
  //    *pa++ = pts1[i].x;
  //    *pa++ = pts1[i].y;
  //    *pa++ = 1;
  //    *pa++ = -pts2[i].y * pts1[i].x;
  //    *pa++ = -pts2[i].y * pts1[i].y;
  //    *pb++ = pts2[i].y;
  //  }

  //  tl::solveSVD(m, n, A, B, C);

  //  a = C[0];
  //  b = C[1];
  //  c = C[2];
  //  d = C[3];
  //  e = C[4];
  //  f = C[5];
  //  g = C[6];
  //  h = C[7];

  //  if (error) {
  //    if (rmse) *rmse = this->_rootMeanSquareError(pts1, pts2, error);
  //  }

  //} catch (std::exception &e) {
  //  msgError(e.what());
  //  status = Transform::Status::failure;
  //}

  //delete[] A;
  //delete[] B;
  //delete[] C;

  return status;
}

template<typename Point_t> inline
Transform::Status Projective<Point_t>::transform(const std::vector<Point_t> &ptsIn, 
                                                 std::vector<Point_t> &ptsOut, 
                                                 Transform::Order trfOrder) const
{
  Transform::Status r_status;
  this->formatVectorOut(ptsIn, ptsOut);
  for (int i = 0; i < ptsIn.size(); i++) {
    r_status = transform(ptsIn[i], ptsOut[i], trfOrder);
    if ( r_status == Transform::Status::failure ) break;
  }
  return r_status;
}

template<typename Point_t> inline
Transform::Status Projective<Point_t>::transform(const Point_t &ptIn, 
                                                 Point_t &ptOut, 
                                                 Transform::Order trfOrder) const
{
  Transform::Status r_status = Transform::Status::success;

  using sub_type = typename Point_t::value_type;
  Point_t pt_aux = ptIn;
  try {
    if ( trfOrder == Transform::Order::direct ) {
      ptOut.x = static_cast<sub_type>((a * ptIn.x + b * ptIn.y + c)
                                      / (g * ptIn.x + h * ptIn.y + 1));
      ptOut.y = static_cast<sub_type>((d * ptIn.x + e * ptIn.y + f)
                                      / (g * ptIn.x + h * ptIn.y + 1));
    } else {
      ptOut.x = static_cast<sub_type>((ai * ptIn.x + bi * ptIn.y + ci)
                                      / (g * ptIn.x + h * ptIn.y + 1));
      ptOut.y = static_cast<sub_type>((di * ptIn.x + ei * ptIn.y + fi)
                                      / (g * ptIn.x + h * ptIn.y + 1));
    }
  } catch (std::exception &e ) {
    msgError("Error in projective transformation: %s", e.what());
    r_status = Transform::Status::failure;
  }
  return r_status; 
}

template<typename Point_t> inline
Point_t Projective<Point_t>::transform(const Point_t &ptIn,
                                       Transform::Order trfOrder) const
{
  Point_t r_pt{};

  using sub_type = typename Point_t::value_type;
  if (trfOrder == Transform::Order::direct) {
    r_pt.x = static_cast<sub_type>((a * ptIn.x + b * ptIn.y + c)
                                   / (g * ptIn.x + h * ptIn.y + 1));
    r_pt.y = static_cast<sub_type>((d * ptIn.x + e * ptIn.y + f)
                                   / (g * ptIn.x + h * ptIn.y + 1));
  } else {
    r_pt.x = static_cast<sub_type>((ai * ptIn.x + bi * ptIn.y + ci)
                                   / (g * ptIn.x + h * ptIn.y + 1));
    r_pt.y = static_cast<sub_type>((di * ptIn.x + ei * ptIn.y + fi)
                                   / (g * ptIn.x + h * ptIn.y + 1));
  }
  return r_pt;
}

template<typename Point_t> inline
void Projective<Point_t>::setParameters(double _a, double _b, 
                                        double _c, double _d, 
                                        double _e, double _f, 
                                        double _g, double _h)
{
  a = _a;
  b = _b;
  c = _c;
  d = _d;
  e = _e;
  f = _f;
  g = _g;
  h = _h;
  update();
}

template<typename Point_t> inline
void Projective<Point_t>::getParameters(double *_a, double *_b, 
                                        double *_c, double *_d, 
                                        double *_e, double *_f, 
                                        double *_g, double *_h)
{
  *_a = a;
  *_b = b;
  *_c = c;
  *_d = d;
  *_e = e;
  *_f = f;
  *_g = g;
  *_h = h;
}

template<typename Point_t> inline
void Projective<Point_t>::update()
{
  // Transformación inversa
  double aux = a * e - b * d;
  if (aux == 0.) {
    msgError("Division by zero");
  } else {
    ai = (e - f * h) / aux;
    bi = (c * h - b) / aux;
    ci = (b * f - c * e) / aux;
    di = (f * g - d) / aux;
    ei = (a - c * g) / aux;
    fi = (c * d - a * f) / aux;
    gi = (d * h - e * g) / aux;
    hi = (b * g - a * h) / aux;
  }

}


/*! \} */ // end of trf2DGroup

/*! \} */ // end of trfGroup

/*! \} */ // end of geometry

} // End namespace tl

#endif // TL_GEOMETRY_TRANSFORM_PROJECTIVE_H