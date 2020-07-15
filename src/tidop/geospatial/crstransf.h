#ifndef TL_GEOSPATIAL_CRS_TRANSFORM_H
#define TL_GEOSPATIAL_CRS_TRANSFORM_H

#include "tidop/core/defs.h"

#include <memory>

#ifdef HAVE_GDAL
TL_SUPPRESS_WARNINGS
#include "ogr_spatialref.h"
#include "ogr_p.h"
#include "ogr_api.h"
TL_DEFAULT_WARNINGS
#endif

#include "tidop/geospatial/crs.h"
#include "tidop/core/messages.h"
#include "tidop/core/exception.h"
#include "tidop/geometry/transform.h"


namespace tl
{

template<typename Point_t> class Transform3D;


namespace geospatial
{


#if defined HAVE_GDAL && defined HAVE_PROJ4

///TODO: controlar cuando es altura elipsoidal y ortométrica

/*!
 * \brief transformación entre sistemas de referencia
 */
template<typename Point_t>
class CrsTransform 
  : public Transform3D<Point_t>
{

public:

  /*!
   * \brief Constructor
   */
  CrsTransform(const std::shared_ptr<Crs> &epsgIn, const std::shared_ptr<Crs> &epsgOut);
  
  //CrsTransform(const char *epsgIn, const char *epsgOut);

  ~CrsTransform();

  /*!
   * \brief Operación no soportada para CrsTransform
   * \param[in] pts1 Conjunto de puntos en el primero de los sistemas
   * \param[in] pts2 Conjunto de puntos en el segundo de los sistemas
   * \param[out] error Vector con los errores para cada punto
   * \param[out] rmse Root Mean Square Error
   * \return transform_status
   * \see transform_status
   */
  transform_status compute(const std::vector<Point_t> &pts1, 
                           const std::vector<Point_t> &pts2, 
                           std::vector<double> *error = nullptr,
                           double *rmse = nullptr) override;

  /*!
   * \brief Transforma un conjunto de puntos a otro sistema de referencia
   * \param[in] ptsIn Puntos de entrada
   * \param[out] ptsOut Puntos de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \see transform_order
   */
  transform_status transform(const std::vector<Point_t> &ptsIn, std::vector<Point_t> *ptsOut, 
                 transform_order trfOrder = transform_order::direct) const override;

  /*!
   * \brief Transforma un punto a otro sistema de referencia
   * \param[in] ptIn Punto de entrada
   * \param[out] ptOut Punto de salida
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \see transform_order
   */
  transform_status transform(const Point_t &ptIn, Point_t *ptOut, transform_order trfOrder = transform_order::direct) const override;

  /*!
   * \brief Transforma un punto a otro sistema de referencia
   * \param[in] ptIn Punto de entrada
   * \param[in] trfOrder Transformación directa (por defecto) o inversa
   * \return Punto de salida
   * \see transform_order
   */
  Point_t transform(const Point_t &ptIn, transform_order trfOrder = transform_order::direct) const override;

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

  OGRCoordinateTransformation *pCoordinateTransformation;
  OGRCoordinateTransformation *pCoordinateTransformationInv;

};


template<typename Point_t> inline
CrsTransform<Point_t>::CrsTransform(const std::shared_ptr<Crs> &epsgIn, 
                                    const std::shared_ptr<Crs> &epsgOut) 
  : Transform3D<Point_t>(transform_type::crs),
    mEpsgIn(epsgIn), 
    mEpsgOut(epsgOut), 
    pCoordinateTransformation(nullptr),
    pCoordinateTransformationInv(nullptr)
{
  init();
}

//template<typename Point_t> inline
//CrsTransform<Point_t>::CrsTransform(const char *epsgIn, const char *epsgOut) 
//  : Transform3D<Point_t>(transform_type::CRS), 
//  mEpsgIn(std::make_shared<Crs>(epsgIn)), 
//  mEpsgOut(std::make_shared<Crs>(epsgOut)), 
//  pCoordinateTransformation(0), 
//  pCoordinateTransformationInv(0) 
//{
//  init();
//}

template<typename Point_t> inline
CrsTransform<Point_t>::~CrsTransform() 
{
  if (pCoordinateTransformation)
    OGRCoordinateTransformation::DestroyCT(pCoordinateTransformation), pCoordinateTransformation = nullptr;
  if (pCoordinateTransformationInv)
    OGRCoordinateTransformation::DestroyCT(pCoordinateTransformationInv), pCoordinateTransformationInv = nullptr;
  OSRCleanup();
}

TL_DISABLE_WARNING(TL_UNREFERENCED_FORMAL_PARAMETER)
template<typename Point_t> inline
transform_status CrsTransform<Point_t>::compute(const std::vector<Point_t> &pts1, 
                                                const std::vector<Point_t> &pts2, 
                                                std::vector<double> *error,
                                                double *rmse)
{
  msgError("'compute' is not supported for CrsTransform");
  //TL_COMPILER_WARNING("'compute' is not supported for CrsTransform");
  return transform_status::failure;
}
TL_ENABLE_WARNING(TL_UNREFERENCED_FORMAL_PARAMETER)

template<typename Point_t> inline
transform_status CrsTransform<Point_t>::transform(const std::vector<Point_t> &ptsIn, 
                                                  std::vector<Point_t> *ptsOut, 
                                                  transform_order trfOrder) const
{
  this->formatVectorOut(ptsIn, ptsOut);
  for (int i = 0; i < ptsIn.size(); i++) {
    // TODO: Debería ser mas rapido hacer:
    // size_t n = ptsIn.size();
    // double *x = new double[n], *p_x = x;
    // double *y = new double[n], *p_y = y;
    // double *z = new double[n], *p_z = z;
    // for (int i = 0; i < n; i++) {
    //  *p_x++ = ptsIn[i].x;
    //  *p_y++ = ptsIn[i].y;
    //  *p_z++ = ptsIn[i].z;
    //}
    // if (trfOrder == transform_order::DIRECT){
    //   pCoordinateTransformation->Transform(n, &ptOut->x, &ptOut->y, &ptOut->z);
    // } else {
    //   pCoordinateTransformationInv->Transform(n, &ptOut->x, &ptOut->y, &ptOut->z);
    //}
    transform(ptsIn[i], &(*ptsOut)[i], trfOrder);
  }
  return transform_status::success;
}


template<typename Point_t> inline
transform_status CrsTransform<Point_t>::transform(const Point_t &ptIn, 
                                                  Point_t *ptOut, 
                                                  transform_order trfOrder) const
{
  *ptOut = ptIn;
  try {
    if (trfOrder == transform_order::direct){
      if (pCoordinateTransformation)
        pCoordinateTransformation->Transform(1, &ptOut->x, &ptOut->y, &ptOut->z);
      else
        msgError("GDAL ERROR (%i): %s", CPLGetLastErrorNo(), CPLGetLastErrorMsg());
    } else {
      if (pCoordinateTransformationInv) 
        pCoordinateTransformationInv->Transform(1, &ptOut->x, &ptOut->y, &ptOut->z);
      else
        msgError("GDAL ERROR (%i): %s", CPLGetLastErrorNo(), CPLGetLastErrorMsg());
    }
  } catch (...) {
    throw;
  }
  return transform_status::success;
}


template<typename Point_t> inline
Point_t CrsTransform<Point_t>::transform(const Point_t &ptIn, transform_order trfOrder) const
{
  Point_t r_pt = ptIn;
  try{
    if (trfOrder == transform_order::direct){
      pCoordinateTransformation->Transform(1, &r_pt.x, &r_pt.y, &r_pt.z);
    } else {
      pCoordinateTransformationInv->Transform(1, &r_pt.x, &r_pt.y, &r_pt.z);
    }
  } catch (std::exception &e) {
    throw std::runtime_error( e.what() );
  }
  return r_pt;
}

template<typename Point_t> inline
void CrsTransform<Point_t>::init()
{
  OGRSpatialReference *spatialReferenceIn = mEpsgIn->getOGRSpatialReference();
  OGRSpatialReference *spatialReferenceOut = mEpsgOut->getOGRSpatialReference();
  pCoordinateTransformation = OGRCreateCoordinateTransformation(spatialReferenceIn,
                                                                spatialReferenceOut);
  pCoordinateTransformationInv = OGRCreateCoordinateTransformation(spatialReferenceOut,
                                                                   spatialReferenceIn);
  OSRCleanup();
}

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


#endif // HAVE_GDAL

} // End namespace geospatial

} // End namespace tl

#endif // TL_GEOSPATIAL_CRS_TRANSFORM_H
