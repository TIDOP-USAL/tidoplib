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

#ifndef TL_GEOSPATIAL_CRS_TRANSFORM_H
#define TL_GEOSPATIAL_CRS_TRANSFORM_H

#include "tidop/core/defs.h"

#include <memory>

#include "tidop/geospatial/crs.h"
#include "tidop/core/messages.h"
#include "tidop/core/exception.h"
#include "tidop/geometry/entities/point.h"
#include "tidop/geometry/transform/transform.h"
#include "tidop/geometry/entities/point.h"
#include "tidop/math/algebra/rotation_matrix.h"

namespace tl
{

template<typename Point_t> class Transform3D;

namespace geospatial
{

namespace internal
{
class CoordinateTransformation;
}

#if defined TL_HAVE_GDAL && defined TL_HAVE_PROJ4

///TODO: controlar cuando es altura elipsoidal y ortométrica

/*!
 * \brief transformación entre sistemas de referencia
 */
class TL_EXPORT CrsTransform
  : public Transform3D<Point3<double>>
{

public:

  /*!
   * \brief Constructor
   */
  CrsTransform(const std::shared_ptr<Crs> &epsgIn, 
               const std::shared_ptr<Crs> &epsgOut);
  
  //CrsTransform(const char *epsgIn, const char *epsgOut);

  ~CrsTransform();

  /*!
   * \brief Operación no soportada para CrsTransform
   * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
   * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
   * \param[out] error Vector con los errores para cada punto
   * \param[out] rmse Root Mean Square Error
   * \return Transform::Status
   * \see Transform::Status
   */
  Transform::Status compute(const std::vector<Point3<double>> &pts1,
                            const std::vector<Point3<double>> &pts2,
                            std::vector<double> *error = nullptr,
                            double *rmse = nullptr) override;

  /*!
   * \brief Transforma un conjunto de puntos a otro sistema de referencia
   * \param[in] ptsIn Puntos de entrada
   * \param[out] ptsOut Puntos de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \see Transform::Order
   */
  Transform::Status transform(const std::vector<Point3<double>> &ptsIn,
                              std::vector<Point3<double>> &ptsOut,
                              Transform::Order trfOrder = Transform::Order::direct) const override;

  /*!
   * \brief Transforma un punto a otro sistema de referencia
   * \param[in] ptIn Punto de entrada
   * \param[out] ptOut Punto de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \see Transform::Order
   */
  Transform::Status transform(const Point3<double> &ptIn,
                              Point3<double> &ptOut,
                              Transform::Order trfOrder = Transform::Order::direct) const override;

  /*!
   * \brief Transforma un punto a otro sistema de referencia
   * \param[in] ptIn Punto de entrada
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Punto de salida
   * \see Transform::Order
   */
  Point3<double> transform(const Point3<double> &ptIn,
                           Transform::Order trfOrder = Transform::Order::direct) const override;

  bool isNull() const override;

private:

  void init();

protected:
  
  /*!
   * \brief Sistema de referencia de entrada
   */
  std::shared_ptr<Crs> mEpsgIn;

  /*!
   * \brief Sistema de referencia de salida
   */
  std::shared_ptr<Crs> mEpsgOut;

private:

  internal::CoordinateTransformation *mCoordinateTransformation;
  internal::CoordinateTransformation *mCoordinateTransformationInv;

  //OGRCoordinateTransformation *pCoordinateTransformation;
  //OGRCoordinateTransformation *pCoordinateTransformationInv;

};


//template<typename Point_t>
//class CrsTransformCache
//{
//public:
//
//  /*!
//   * \brief Allocator
//   */
//  typedef std::vector<std::shared_ptr<CrsTransform<Point_t>>>::allocator_type allocator_type;
//
//  /*!
//   * \brief value_type
//   */
//  typedef std::vector<std::shared_ptr<CrsTransform<Point_t>>>::value_type value_type;
//
//  /*!
//   * \brief Tipo entero sin signo (por lo general size_t) 
//   */
//  typedef std::vector<std::shared_ptr<CrsTransform<Point_t>>>::size_type size_type;
//
//  /*!
//   * \brief Tipo entero con signo (por lo general ptrdiff_t)
//   */
//  typedef std::vector<std::shared_ptr<CrsTransform<Point_t>>>::difference_type difference_type;
//
//  /*!
//   * \brief std::allocator_traits<Allocator>::pointer
//   */
//  typedef std::vector<std::shared_ptr<CrsTransform<Point_t>>>::pointer pointer;
//
//  /*!
//   * \brief std::allocator_traits<Allocator>::const_pointer 
//   */
//  typedef std::vector<std::shared_ptr<CrsTransform<Point_t>>>::const_pointer const_pointer;
//
//  /*!
//   * \brief value_type&
//   */
//  typedef std::vector<std::shared_ptr<CrsTransform<Point_t>>>::reference reference;
//
//  /*!
//   * \brief const value_type&
//   */
//  typedef std::vector<std::shared_ptr<CrsTransform<Point_t>>>::const_reference const_reference;
//
//  /*!
//   * \brief Iterador de acceso aleatorio
//   */
//  typedef std::vector<std::shared_ptr<CrsTransform<Point_t>>>::iterator iterator;
//
//  /*!
//   * \brief Iterador constante de acceso aleatorio
//   */
//  typedef std::vector<std::shared_ptr<CrsTransform<Point_t>>>::const_iterator const_iterator;
//
//private:
//  
//  static std::unique_ptr<CrsTransformCache> sCrsTransformCache;
//  static std::mutex sMutex;
//
//  std::vector<std::shared_ptr<CrsTransform<Point_t>>> mCrsTransforms;
//
//  size_t mCacheIdx;
//
//private:
//
//  CrsTransformCache();
//
//public:
//
//  ~CrsTransformCache();
//
//  // Se impide la copia y asignación
//  CrsTransformCache(CrsTransformCache const&) = delete;
//  void operator=(CrsTransformCache const&) = delete;
//  
//  static CrsTransformCache<Point_t> &getInstance();
//
//private:
//
//};
//
//template<typename Point_t> 
//std::unique_ptr<CrsTransformCache<Point_t>> CrsTransformCache<Point_t>::sCrsTransformCache;
//
//template<typename Point_t> 
//std::mutex CrsTransformCache<Point_t>::sMutex;
//
//template<typename Point_t> inline
//CrsTransformCache<Point_t>::CrsTransformCache()
//{
//}
//
//template<typename Point_t> inline
//CrsTransformCache<Point_t>::~CrsTransformCache()
//{
//}
//
//template<typename Point_t> inline
//CrsTransformCache<Point_t> &CrsTransformCache<Point_t>::getInstance()
//{
//  if (sCrsTransformCache.get() == nullptr) {
//    std::lock_guard<std::mutex> lck(CrsTransformCache<Point_t>::sMutex);
//    if (sCrsTransformCache.get() == nullptr) {
//      sCrsTransformCache.reset(new CrsTransformCache<Point_t>());
//    }
//  }
//  return *sCrsCache;
//}

#endif // TL_HAVE_GDAL


class EcefToEnu
{

public:

  EcefToEnu(const Point3<double> &center)
    : mCenter(center)
  {
  }

  ~EcefToEnu()
  {
  }

  Point3<double> direct(const Point3<double> &ecef,
                        double longitude,
                        double latitude);
  Point3<double> inverse(const Point3<double> &enu,
                         double longitude,
                         double latitude);

private:

  math::RotationMatrix<double> rotationMatrixToEnu(double longitude,
                                                   double latitude);

private:

  Point3<double> mCenter;

};


} // End namespace geospatial

} // End namespace tl

#endif // TL_GEOSPATIAL_CRS_TRANSFORM_H
