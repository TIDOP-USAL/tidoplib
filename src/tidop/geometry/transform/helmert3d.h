/************************************************************************
 *                                                                      *
 * Copyright (C) 2020 by Tidop Research Group                           *
 *                                                                      *
 * This file is part of TidopLib                                        *
 *                                                                      *
 * TidopLib is free software: you can redistribute it and/or modify     *
 * it under the terms of the GNU General Public License as published by *
 * the Free Software Foundation, either version 3 of the License, or    *
 * (at your option) any later version.                                  *
 *                                                                      *
 * TidopLib is distributed in the hope that it will be useful,          *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.      *
 *                                                                      *
 * @license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>                *
 *                                                                      *
 ************************************************************************/

#ifndef TL_GEOMETRY_TRANSFORM_HELMERT_3D_H
#define TL_GEOMETRY_TRANSFORM_HELMERT_3D_H

#include "config_tl.h"

#include "tidop/geometry/transform/transform.h"
#include "tidop/math/algebra/rotation_convert.h"
#include "tidop/core/exception.h"

namespace tl
{



/*! \addtogroup trfGroup
 *  \{
 */

/*! \addtogroup trf3DGroup
 *  \{
 */


/*!
 * \brief Helmert 3D
 *
 * El Helmert 3D o Transformación de Semejanza en el espacio expresa la relación 
 * que existe (o la transformación que es preciso realizar) entre dos sistemas 
 * cartesianos que difieren en la situación del origen, en la orientación de los 
 * ejes y en la unidad de medida a lo largo de los mismos (eslala uniforme a lo
 * largo de los tres ejes)  
 *
 * <BLOCKQUOTE>

 * </BLOCKQUOTE>
 */
template<typename Point_t>
class Helmert3D
  : public Transform3D<Point_t>
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

  /*!
   * \brief Traslación en z
   */
  double tz;

public:

  /*!
   * \brief Constructor por defecto
   */
  Helmert3D();

  /*!
   * \brief Constructor
   * \param[in] tx Traslación en X
   * \param[in] ty Traslación en Y
   * \param[in] tz Traslación en Z
   * \param[in] scale Escala
   * \param[in] omega Rotación respecto al eje X
   * \param[in] phi Rotación respecto al eje Y
   * \param[in] kappa Rotación respecto al eje Z
   */
  Helmert3D(double tx, 
            double ty, 
            double tz, 
            double scale, 
            double omega, 
            double phi, 
            double kappa);

  /*!
   * \brief Constructor
   * \param[in] tx Traslación en X
   * \param[in] ty Traslación en Y
   * \param[in] tz Traslación en Z
   * \param[in] scale Escala
   * \param[in] rotation Matriz de rotación
   */
  //Helmert3D(double tx, 
  //          double ty, 
  //          double tz, 
  //          double scale, 
  //          const std::array<std::array<double, 3>, 3> &rotation);
  Helmert3D(double tx, 
            double ty, 
            double tz, 
            double scale, 
            const math::RotationMatrix<double> &rotation);

  /*!
   * \brief Constructor de copia Helmert3D
   * \param[in] trf Objeto Helmert3D que se copia
   */
  Helmert3D(const Helmert3D &trf);


  ~Helmert3D() override = default;

  /*!
   * \brief Calcula la transformación Helmert 3D entre dos sistemas
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
   * \brief Devuelve la matriz de rotación
   */
  //const std::array<std::array<double, 3>, 3> &rotationMatrix() const;
  math::RotationMatrix<double> rotationMatrix() const;

  /*!
   * \brief Transforma un conjunto de puntos en otro aplicando un helmert 3D
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
   * \brief Aplica un helmert 3D a un punto
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
   * \brief Aplica un helmert 3D a un punto
   * \param[in] ptIn Punto de entrada
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Punto de salida
   * \see Transform::Order
   */
  Point_t transform(const Point_t &ptIn, 
                    Transform::Order trfOrder = Transform::Order::direct) const override;

#ifdef TL_ENABLE_DEPRECATED_METHODS
  /*!
   * \brief Devuelve la escala de la transformación
   * \return Escala de la transformación
   */
  TL_DEPRECATED("scale()", "2.0")
  double getScale() const { return mScale; }
#endif // TL_ENABLE_DEPRECATED_METHODS

  /*!
   * \brief Devuelve la escala de la transformación
   * \return Escala de la transformación
   */
  double scale() const { return mScale; }

  /*!
   * \brief Establece la escala de la transformación
   * \param[in] scale Escala de la transformación
   */
  void setScale(double scale);

  double omega() const;
  void setOmega(double omega);

  double phi() const;
  void setPhi(double phi);

  double kappa() const;
  void setKappa(double kappa);

  /*!
   * \brief Establece los parámetros
   * \param[in] tx Traslación en X
   * \param[in] ty Traslación en Y
   * \param[in] tz Traslación en Z
   * \param[in] scale Escala
   * \param[in] omega Rotación respecto al eje X
   * \param[in] phi Rotación respecto al eje Y
   * \param[in] kappa Rotación respecto al eje Z
   */
  void setParameters(double tx, 
                     double ty, 
                     double tz, 
                     double scale, 
                     double omega, 
                     double phi, 
                     double kappa);

  bool isNull() const
  {
    return (tx == 0. &&
            ty == 0. &&
            tz == 0. &&
            mScale == 1. &&
            mOmega == 0. &&
            mPhi == 0. &&
            mKappa == 0.);
  }

private:

  /*!
   * \brief Actualiza la transformación
   */
  void update();

private:

  /*!
   * \brief Escala
   */
  double mScale;

  /*!
   * \brief Ángulo de rotation respecto al eje x
   */
  double mOmega;

  /*!
   * \brief Ángulo de rotation respecto al eje x
   */
  double mPhi;

  /*!
   * \brief Ángulo de rotation respecto al eje x
   */
  double mKappa;

  /*!
   * \brief Matriz de rotación
   */
  //std::array<std::array<double, 3>, 3> mR;
  math::RotationMatrix<double> mR;

  /*!
   * \brief Matriz de rotación inversa
   */
  //std::array<std::array<double, 3>, 3> mRinv;
  math::RotationMatrix<double> mRinv;
};



template<typename Point_t> inline
Helmert3D<Point_t>::Helmert3D()
  : Transform3D<Point_t>(Transform::Type::helmert_3d, 3),
    tx(0.),
    ty(0.),
    tz(0.),
    mScale(1.),
    mOmega(0.),
    mPhi(0.),
    mKappa(0.)
{
  update();
}

template<typename Point_t> inline
Helmert3D<Point_t>::Helmert3D(double tx, 
                              double ty, 
                              double tz, 
                              double scale,
                              double omega, 
                              double phi,
                              double kappa)
  : Transform3D<Point_t>(Transform::Type::helmert_3d, 3),
    tx(tx),
    ty(ty),
    tz(tz),
    mScale(scale),
    mOmega(omega),
    mPhi(phi),
    mKappa(kappa)
{
  update();
}

//template<typename Point_t> inline
//Helmert3D<Point_t>::Helmert3D(double tx, 
//                              double ty, 
//                              double tz,
//                              double scale,
//                              const std::array<std::array<double, 3>, 3> &rotation)
//  : Transform3D<Point_t>(Transform::Type::helmert_3d, 3),
//    tx(tx),
//    ty(ty),
//    tz(tz),
//    mScale(scale),
//    mR(rotation)
//{
//  eulerAngles(mR, &mOmega, &mPhi, &mKappa);
//  update();
//}

template<typename Point_t> inline
Helmert3D<Point_t>::Helmert3D(double tx, 
                              double ty, 
                              double tz, 
                              double scale, 
                              const math::RotationMatrix<double> &rotation)
  : Transform3D<Point_t>(Transform::Type::helmert_3d, 3),
    tx(tx),
    ty(ty),
    tz(tz),
    mScale(scale),
    mR(rotation)
{
  math::EulerAngles<double> eulerAngles;
  eulerAngles.axes = math::EulerAngles<double>::Axes::xyz;
  math::RotationConverter<double>::convert(rotation, eulerAngles);
  mOmega = eulerAngles.omega;
  mPhi = eulerAngles.phi;
  mKappa = eulerAngles.kappa;
  //eulerAngles(mR, &mOmega, &mPhi, &mKappa);
  update();
}

template<typename Point_t>
Helmert3D<Point_t>::Helmert3D(const Helmert3D &trf)
  : Transform3D<Point_t>(Transform::Type::helmert_3d, 3),
    tx(trf.tx),
    ty(trf.ty),
    tz(trf.tz),
    mScale(trf.mScale),
    mOmega(trf.mOmega),
    mPhi(trf.mPhi),
    mKappa(trf.mKappa),
    mR(trf.mR),
    mRinv(trf.mRinv)
{

}

template<typename Point_t> inline
Transform::Status Helmert3D<Point_t>::compute(const std::vector<Point_t> &pts1, 
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
  size_t n = 7;

  double *A = nullptr;
  double *L = nullptr;
  double *C = nullptr;

  try {

    A = new double[m * n];
    double *pa = A;
    L = new double[m];
    double *pl = L;
    C = new double[n];

    for (int i = 0; i < n1; i++) {
      *pa++ = pts1[i].x;
      *pa++ = 0;
      *pa++ = -pts1[i].z;
      *pa++ = pts1[i].y;
      *pa++ = 1;
      *pa++ = 0;
      *pa++ = 0;
      *pl++ = pts2[i].x;
      *pa++ = pts1[i].y;
      *pa++ = pts1[i].z;
      *pa++ = 0;
      *pa++ = -pts1[i].x;
      *pa++ = 0;
      *pa++ = 1;
      *pa++ = 0;
      *pl++ = pts2[i].y;
      *pa++ = pts1[i].z;
      *pa++ = -pts1[i].y;
      *pa++ = pts1[i].x;
      *pa++ = 0;
      *pa++ = 0;
      *pa++ = 0;
      *pa++ = 1;
      *pl++ = pts2[i].z;
    }

    solveSVD(m, n, A, L, C);

    mScale = C[0];
    mOmega = C[1];
    mPhi = C[2];
    mKappa = C[3];
    tx = C[4];
    ty = C[5];
    tz = C[6];

    if (error) {
      ///TODO: este metodo sólo contempla puntos en 2d....
      if (rmse) *rmse = this->_rootMeanSquareError(pts1, pts2, error);
    }
    
    //rmse = _rootMeanSquareError(pts1, pts2, error);
  } catch (std::exception &e) {
    msgError(e.what());
    status = Transform::Status::failure;
  }

  delete[] A;
  delete[] L;
  delete[] C;

  return status;
}
//
//template<typename Point_t> inline
//const std::array<std::array<double, 3>, 3> &Helmert3D<Point_t>::rotationMatrix() const 
//{
//  return mR;
//}

template<typename Point_t> inline
math::RotationMatrix<double> Helmert3D<Point_t>::rotationMatrix() const
{
  return mR;
}

template<typename Point_t> inline
Transform::Status Helmert3D<Point_t>::transform(const std::vector<Point_t> &ptsIn, 
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
Transform::Status Helmert3D<Point_t>::transform(const Point_t &ptIn,
                                                Point_t &ptOut, 
                                                Transform::Order trfOrder) const
{
  Transform::Status r_status = Transform::Status::success;
  Point_t ptAux = ptIn;
  try {
    if (trfOrder == Transform::Order::direct){
      ptOut.x = static_cast<sub_type>(mScale * (ptAux.x * mR.at(0,0) + ptAux.y * mR.at(0,1) + ptAux.z * mR.at(0,2)) + tx);
      ptOut.y = static_cast<sub_type>(mScale * (ptAux.x * mR.at(1,0) + ptAux.y * mR.at(1,1) + ptAux.z * mR.at(1,2)) + ty);
      ptOut.z = static_cast<sub_type>(mScale * (ptAux.x * mR.at(2,0) + ptAux.y * mR.at(2,1) + ptAux.z * mR.at(2,2)) + tz);
    } else {
      sub_type dx = ptIn.x - tx; 
      sub_type dy = ptIn.y - ty; 
      sub_type dz = ptIn.z - tz;
      ptOut.x = static_cast<sub_type>(mScale * (dx * mRinv.at(0,0) + dy * mRinv.at(0,1) + dz * mRinv.at(0,2)));
      ptOut.y = static_cast<sub_type>(mScale * (dx * mRinv.at(1,0) + dy * mRinv.at(1,1) + dz * mRinv.at(1,2)));
      ptOut.z = static_cast<sub_type>(mScale * (dx * mRinv.at(2,0) + dy * mRinv.at(2,1) + dz * mRinv.at(2,2)));
    }
  } catch (std::exception &e ) {
    msgError("Error in Helmert 3D transformation: %s", e.what());
    r_status = Transform::Status::failure;
  }
  return r_status; 
}

template<typename Point_t> inline
Point_t Helmert3D<Point_t>::transform(const Point_t &ptIn,
                                      Transform::Order trfOrder) const
{
  Point_t r_pt;
  if (trfOrder == Transform::Order::direct){
    r_pt.x = static_cast<sub_type>(mScale * (ptIn.x * mR.at(0,0) + ptIn.y * mR.at(0,1) + ptIn.z * mR.at(0,2)) + tx);
    r_pt.y = static_cast<sub_type>(mScale * (ptIn.x * mR.at(1,0) + ptIn.y * mR.at(1,1) + ptIn.z * mR.at(1,2)) + ty);
    r_pt.z = static_cast<sub_type>(mScale * (ptIn.x * mR.at(2,0) + ptIn.y * mR.at(2,1) + ptIn.z * mR.at(2,2)) + tz);
  } else {
    sub_type dx = ptIn.x - tx; 
    sub_type dy = ptIn.y - ty; 
    sub_type dz = ptIn.z - tz;
    r_pt.x = static_cast<sub_type>(mScale * (dx * mRinv.at(0,0) + dy * mRinv.at(0,1) + dz * mRinv.at(0,2)));
    r_pt.y = static_cast<sub_type>(mScale * (dx * mRinv.at(1,0) + dy * mRinv.at(1,1) + dz * mRinv.at(1,2)));
    r_pt.z = static_cast<sub_type>(mScale * (dx * mRinv.at(2,0) + dy * mRinv.at(2,1) + dz * mRinv.at(2,2)));
  }
  return r_pt;
}

template<typename Point_t> inline
void Helmert3D<Point_t>::setParameters(double tx, 
                                       double ty, 
                                       double tz, 
                                       double scale, 
                                       double omega, 
                                       double phi, 
                                       double kappa)
{
  this->tx = tx;
  this->ty = ty;
  this->tz = tz;
  mScale = scale;
  mOmega = omega;
  mPhi = phi;
  mKappa = kappa;

  update();
}

template<typename Point_t> inline
void Helmert3D<Point_t>::setScale(double scale)
{
  mScale = scale;
}

template<typename Point_t> inline
double Helmert3D<Point_t>::omega() const
{
return mOmega;
}

template<typename Point_t> inline
void Helmert3D<Point_t>::setOmega(double omega)
{
  mOmega = omega;
  update();
}

template<typename Point_t> inline
double Helmert3D<Point_t>::phi() const
{
return mPhi;
}

template<typename Point_t> inline
void Helmert3D<Point_t>::setPhi(double phi)
{
  mPhi = phi;
  update();
}

template<typename Point_t> inline
double Helmert3D<Point_t>::kappa() const
{
return mKappa;
}

template<typename Point_t> inline
void Helmert3D<Point_t>::setKappa(double kappa)
{
  mKappa = kappa;
  update();
}

template<typename Point_t> inline
void Helmert3D<Point_t>::update()
{
  math::EulerAngles<double> eulerAngles(mOmega, mPhi, mKappa, 
                                        math::EulerAngles<double>::Axes::xyz);
  math::RotationConverter<double>::convert(eulerAngles, mR);
  mRinv = mR.inverse();
//  rotationMatrix(mOmega, mPhi, mKappa, &mR);
//#ifdef HAVE_OPENCV
//  cv::Mat inv = cv::Mat(3, 3, CV_64F, mR.data()).inv();
//  for (int i = 0; i < 3; i++) {
//    for (int j = 0; j < 3; j++) {
//      mRinv[i][j] = inv.at<double>(i, j);
//    }
//  }
//#elif HAVE_EIGEN
//  Eigen::Map<Matrix<double, 3, 3, RowMajor> > mat(mR.data());
//  mat = mat.inverse();
//#endif
}

template<typename Point_t> inline
Helmert3D<Point_t> operator*(Helmert3D<Point_t> &trf1, 
                             Helmert3D<Point_t> &trf2)
{
  //const std::array<std::array<double, 3>, 3> R1 = trf1.getRotationMatrix();
  //const std::array<std::array<double, 3>, 3> R2 = trf2.getRotationMatrix();

  //cv::Mat _r1 = cv::Mat(3, 3, CV_32F, R1);

  //std::array<std::array<double, 3>, 3> R;
  //cv::Mat matRot1 = cv::Mat(3, 3, CV_64F, R1.data());
  //cv::Mat matRot2 = cv::Mat(3, 3, CV_64F, R2.data());
  //cv::Mat rot = matRot1 * matRot2;

  //Point_t r_pt;
  //r_pt.x = static_cast<sub_type>( trf2.getScale() * (trf2.x*R1[0][0] + trf2.y*R1[0][1] + trf2.z*R1[0][2]) + trf2.x0 );
  //r_pt.y = static_cast<sub_type>( trf2.getScale() * (trf2.x*R1[1][0] + trf2.y*R1[1][1] + trf2.z*R1[1][2]) + trf2.y0 );
  //r_pt.z = static_cast<sub_type>( trf2.getScale() * (trf2.x*R1[2][0] + trf2.y*R1[2][1] + trf2.z*R1[2][2]) + trf2.z0 );

  //Helmert3D<Point_t> trf(r_pt.x, r_pt.y, r_pt.z, trf1.getScale(), rot);

}


/*! \} */ // end of trf3DGroup


/*! \} */ // end of trfGroup


} // End namespace tl

#endif // TL_GEOMETRY_TRANSFORM_HELMERT_3D_H