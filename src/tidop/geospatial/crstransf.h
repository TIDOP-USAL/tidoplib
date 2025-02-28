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


#include "tidop/core/base/defs.h"

#include <memory>

#include "tidop/geospatial/crs.h"
#include "tidop/core/base/exception.h"
#include "tidop/geometry/entities/point.h"
#include "tidop/math/algebra/rotations/rotation_matrix.h"

namespace tl
{

namespace geom
{
template<typename Point_t> class Transform3D;
}

/// \cond

namespace internal
{
class CoordinateTransformation;
}

/// \endcond


/*!
 * \addtogroup geospatial
 *
 * \{
 */

#if defined TL_HAVE_GDAL && (defined TL_HAVE_PROJ4 || defined TL_HAVE_PROJ)

///TODO: controlar cuando es altura elipsoidal y ortométrica

/*!
 * \brief Transformation between coordinate reference systems
 */
class TL_EXPORT CrsTransform
{

public:

    enum class Order
    {
        direct,   /*!< Direct transformation. */
        inverse,  /*!< Inverse transformation. */
    };

protected:

    /*!
     * \brief Input coordinate reference system
     */
    std::shared_ptr<Crs> mEpsgIn;

    /*!
     * \brief Output coordinate reference system
     */
    std::shared_ptr<Crs> mEpsgOut;

private:

    internal::CoordinateTransformation *mCoordinateTransformation;
    internal::CoordinateTransformation *mCoordinateTransformationInv;
    mutable std::mutex mMutex;

public:

    /*!
     * \brief Constructor
     */
    CrsTransform(const std::shared_ptr<Crs> &epsgIn,
                 const std::shared_ptr<Crs> &epsgOut);

    ~CrsTransform();

    TL_DISABLE_COPY(CrsTransform)
    TL_DISABLE_MOVE(CrsTransform)

    /*!
     * \brief Transforms a set of points to another reference system
     * \param[in] ptsIn Input points
     * \param[out] ptsOut Output points
     * \param[in] trfOrder Transformation order, direct (by default) or inverse
     * \see Order
     */
    void transform(const std::vector<Point3<double>> &ptsIn,
                   std::vector<Point3<double>> &ptsOut,
                   Order trfOrder = Order::direct) const;

    /*!
     * \brief Transforms coordinates to another reference system
     * \param[in] ptIn Input coordinates
     * \param[out] ptOut Output coordinates
     * \param[in] trfOrder Transformation order, direct (by default) or inverse
     * \see Order
     */
    void transform(const Point3<double> &ptIn,
                   Point3<double> &ptOut,
                   Order trfOrder = Order::direct) const;

    /*!
     * \brief Transforma un punto a otro sistema de referencia
     * \param[in] ptIn Punto de entrada
     * \param[in] trfOrder Transformación directa (por defecto) o inversa
     * \return Punto de salida
     * \see Transform::Order
     */
    auto transform(const Point3<double> &ptIn,
                   Order trfOrder = Order::direct) const -> Point3<double>;

    auto isNull() const -> bool;

private:

    void init();

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

private:

    Point3<double> mCenter;
    RotationMatrix<double> mRotation;

public:

    EcefToEnu(const Point3<double> &center,
              const RotationMatrix<double> &rotation);

    ~EcefToEnu() = default;

    auto direct(const Point3<double> &ecef) const -> Point3<double>;
    auto inverse(const Point3<double> &enu) const -> Point3<double>;

    auto center() const -> Point3<double>
    {
        return mCenter;
    }

    auto rotation() const -> RotationMatrix<double>
    {
        return mRotation;
    }
};

auto rotationEnuToEcef(double longitude, double latitude) -> RotationMatrix<double>;


/*! \} */

} // End namespace tl


