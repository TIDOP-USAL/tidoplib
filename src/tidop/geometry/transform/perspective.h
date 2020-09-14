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


#ifndef TL_GEOMETRY_TRANSFORM_PERSPECTIVE_H
#define TL_GEOMETRY_TRANSFORM_PERSPECTIVE_H

#include "config_tl.h"

#include "tidop/core/messages.h"
#include "tidop/geometry/transform/transform.h"


namespace tl
{


/*! \addtogroup trfGroup
 *  \{
 */

/*! \addtogroup trf2DGroup
 *  \{
 */


#ifdef HAVE_OPENCV

/*!
 * \brief Transformación perspectiva
 */
template<typename Point_t>
class TrfPerspective
  : public Transform2D<Point_t>
{

public:

  /*!
   * \brief Constructor
   */
  TrfPerspective();

  /*!
   * \brief Destructora
   */
  ~TrfPerspective() override = default;

  /*!
   * \brief Calcula los parámetros de transformación
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
   * \brief Aplica la transformación a un punto
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
   * \brief Aplica la transformación a un punto
   * \param[in] ptIn Punto de entrada
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Punto de salida
   * \see Transform::Order
   */
  Point_t transform(const Point_t &ptIn, 
                    Transform::Order trfOrder = Transform::Order::direct) const override;

  /*!
   * \brief Aplica la transformación a un conjunto de puntos
   * \param[in] ptsIn Puntos de entrada
   * \param[out] ptsOut Puntos de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Transform::Status
   * \see Transform::Order, Transform::Status
   */
  Transform::Status transform(const std::vector<Point_t> &ptsIn,
                              std::vector<Point_t> &ptsOut,
                              Transform::Order trfOrder = Transform::Order::direct) const override;

private:

  /*!
   * \brief Parámetros de la transformación
   */
  cv::Mat H;
};


template<typename Point_t> inline
TrfPerspective<Point_t>::TrfPerspective()
  : Transform2D<Point_t>(Transform::Type::perspective, 4)
{

}

template<typename Point_t> inline
Transform::Status TrfPerspective<Point_t>::compute(const std::vector<Point_t> &pts1,
                                                   const std::vector<Point_t> &pts2,
                                                   std::vector<double> *error,
                                                   double *rmse)
{
  //if (error) error = NULL; // Habria que poder calcular el error
  //if (rmse) rmse = NULL;

  size_t n1 = pts1.size();
  size_t n2 = pts2.size();

  if (n1 != n2) {
    msgError("Sets of points with different size. Size pts1 = %zu and size pts2 = %zu", n1, n2);
    return Transform::Status::failure;
  }

  //TODO: chapuza...
  std::vector<cv::Point_<sub_type>> in(n1);
  std::vector<cv::Point_<sub_type>> out(n1);
  for (size_t i = 0; i < n1; i++) {
    in[i] = pts1[i];
    out[i] = pts2[i];
  }

  if (!this->isNumberOfPointsValid(n1)) {
    msgError("Invalid number of points: %zu < %i", n1, this->mMinPoint);
    return Transform::Status::failure;
  }

  Transform::Status status = Transform::Status::success;

  try {
    H = cv::findHomography(in, out, cv::RANSAC);
    //cv::Mat H0 = cv::findHomography(pts1, pts2, cv::RANSAC);
    //cv::Mat H1 = cv::findHomography(pts1, pts2, cv::LMEDS);
    //cv::Mat H2 = cv::findHomography(pts1, pts2);
    //... determinar error
    //TL_THROW_ASSERT(!H.empty(), "Error al calcular los parámetros de la transformación");
    if (H.empty()) {
      msgError("Error al calcular los parámetros de la transformación");
      return Transform::Status::failure;
    }
    if (error) {
      if (rmse) *rmse = this->_rootMeanSquareError(pts1, pts2, error);
    }

  } catch (std::exception &e) {
    //msgError(e.what());
    tl::MessageManager::release(e.what(), tl::MessageLevel::msg_error);
    status = Transform::Status::failure;
  }

  return status;
}

template<typename Point_t> inline
Transform::Status TrfPerspective<Point_t>::transform(const Point_t &ptIn,
                                                     Point_t &ptOut,
                                                     Transform::Order trfOrder) const
{
  std::vector<cv::Point_<sub_type>> vIn, vOut;
  vIn.push_back(cv::Point_<sub_type>(ptIn.x, ptIn.y));
  try {
    if (trfOrder == Transform::Order::direct) {
      cv::perspectiveTransform(vIn, vOut, H);
    } else {
      cv::perspectiveTransform(vIn, vOut, H.inv());
    }
    ptOut.x = vOut[0].x;
    ptOut.y = vOut[0].y;
  } catch ( cv::Exception &e ) {
    msgError("Error in perspective transformation: %s", e.what());
    return Transform::Status::failure;
  }
  return Transform::Status::success;
}

template<typename Point_t> inline
Point_t TrfPerspective<Point_t>::transform(const Point_t &ptIn, 
                                           Transform::Order trfOrder) const
{
  Point_t out = ptIn;
  transform(ptIn, out, trfOrder);
  return out;
}

template<typename Point_t> inline
Transform::Status TrfPerspective<Point_t>::transform(const std::vector<Point_t> &ptsIn,
                                                     std::vector<Point_t> &ptsOut,
                                                     Transform::Order trfOrder) const
{
  // TODO: chapuza...
  size_t n = ptsIn.size();
  std::vector<cv::Point_<sub_type>> in(n);
  std::vector<cv::Point_<sub_type>> out;
  for ( size_t i = 0; i < n; i++ ) {
    in[i] = ptsIn[i];
  }

  try {
    if (trfOrder == Transform::Order::direct)
      cv::perspectiveTransform(in, out, H);
    else {
      cv::perspectiveTransform(in, out, H.inv());
    }
  } catch ( cv::Exception &e ) {
    msgError("Error in perspective transformation: %s",e.what());
    return Transform::Status::failure;
  }
  ptsOut.resize(n);
  for (size_t i = 0; i < n; i++) {
    ptsOut[i].x = out[i].x;
    ptsOut[i].y = out[i].y;
  }
  return Transform::Status::success;
}


#endif // HAVE_OPENCV


/*! \} */ // end of trf2DGroup

/*! \} */ // end of trfGroup


} // End namespace tl

#endif // TL_GEOMETRY_TRANSFORM_PERSPECTIVE_H