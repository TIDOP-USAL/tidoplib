/****************************************************************************
 *                                                                          *
 *  This file is part of TidopLib and can not be copied and/or distributed  *
 *  without the express permision of ITOS3D ENGINEERING S.L                 *
 *                                                                          *
 *  Contact: http://www.itos3d.com                                          *
 *           http://tidop.usal.es                                           *
 *                                                                          *
 *--------------------------------------------------------------------------*
 *                                                                          *
 *  Copyright (C) 2018, ITOS3D ENGINEERING S.L - All rights reserved        *
 *                                                                          *
 ****************************************************************************/


#ifndef TL_GEOMETRY_TRANSFORM_POLYNOMIAL_H
#define TL_GEOMETRY_TRANSFORM_POLYNOMIAL_H

#include "config_tl.h"

#include "tidop/geometry/transform/transform.h"


namespace tl
{



/*! \addtogroup trfGroup
 *  \{
 */

/*! \addtogroup trf2DGroup
 *  \{
 */

/*!
 * \brief Tranformación polinómica
 *
 */
template<typename Point_t>
class PolynomialTransform
  : public Transform2D<Point_t>
{

public:

  /*!
   * \brief Constructor por defecto
   */
  PolynomialTransform()
    : Transform2D<Point_t>(Transform::Type::polynomial, 2)
  {
  }

  ~PolynomialTransform() override = default;

  /*!
   * \brief Calcula la transformación polinómica entre dos sistemas
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
   * \brief Transforma un conjunto de puntos en otro aplicando una transformación polinómica
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
  * \brief Aplica una transformación polinómica a un punto
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
  * \brief Aplica una transformación polinómica a un punto
   * \param[in] ptIn Punto de entrada
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Punto de salida
   * \see Transform::Order
   */
  Point_t transform(const Point_t &ptIn,
                    Transform::Order trfOrder = Transform::Order::direct) const override;

};

template<typename Point_t> inline
Transform::Status PolynomialTransform<Point_t>::compute(const std::vector<Point_t> &pts1, 
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

  double xmin = TL_DOUBLE_MAX;
  double ymin = TL_DOUBLE_MAX;
  double xmax = TL_DOUBLE_MIN;
  double ymax = TL_DOUBLE_MIN;
  for( int ipt = 0; ipt < n1; ipt++ ) {
    Point_t &ptoi = pts1[ipt];
    if ( ptoi.x > xmax ) xmax = ptoi.x;
    if ( ptoi.x < xmin ) xmin = ptoi.x;
    if ( ptoi.y > ymax ) ymax = ptoi.y;
    if ( ptoi.y < ymin ) ymin = ptoi.y;
  }
  double xc = (xmax + xmin) / 2.;
  double yc = (ymax + ymin) / 2.;

  //...
  

  if (error) {
    if (rmse) *rmse = this->_rootMeanSquareError(pts1, pts2, error);
  }  
  
  return status;
}

template<typename Point_t> inline
Transform::Status PolynomialTransform<Point_t>::transform(const std::vector<Point_t> &ptsIn, 
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
Transform::Status PolynomialTransform<Point_t>::transform(const Point_t &ptIn, 
                                                          Point_t &out, 
                                                          Transform::Order trfOrder) const
{
  Transform::Status r_status = Transform::Status::success;
  sub_type x_aux = ptIn.x;
  try {
    //...
  } catch (std::exception &e ) {
    msgError("Polynomial transform error: %s", e.what());
    r_status = Transform::Status::failure;
  }
  return r_status; 
}

template<typename Point_t> inline
Point_t PolynomialTransform<Point_t>::transform(const Point_t &ptIn,
                                                Transform::Order trfOrder) const
{
  Point_t r_pt;
  
  //...
  
  return r_pt;
}



/*! \} */ // end of trf2DGroup

/*! \} */ // end of trfGroup


} // End namespace tl

#endif // TL_GEOMETRY_TRANSFORM_POLYNOMIAL_H