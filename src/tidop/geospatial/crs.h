#ifndef TL_GEOSPATIAL_CRS_H
#define TL_GEOSPATIAL_CRS_H

#include "tidop/core/defs.h"

#ifdef HAVE_GDAL
TL_SUPPRESS_WARNINGS
#include "ogr_spatialref.h"
TL_DEFAULT_WARNINGS
#endif


namespace tl
{

namespace geospatial
{

#if defined HAVE_GDAL && defined HAVE_PROJ4

template<typename Point_t> class CrsTransform;

/*!
 * \brief Clase sistema de referencia
 */
class TL_EXPORT Crs
{

public:

  /*!
   * \brief Constructor
   * \param[in] epsg Sistema de referencia como código EPSG
   * \param[in] grid Rejilla de transformación de sistema de coordenadas
   * \param[in] geoid Fichero de ondulación del geoide
   */
  Crs(const std::string &epsg, 
      const std::string &grid = "", 
      const std::string &geoid = "");

  ~Crs();

  /*!
   * \brief Devuelve el código EPSG del sistema de referencia
   * \return
   */
  std::string epsgCode() const;

  bool isGeocentric() const;

  bool isGeographic() const;

  std::string exportToProj() const;
  std::string exportToWkt() const;

protected:

  OGRSpatialReference *getOGRSpatialReference( );

  template<typename Point_t> friend class CrsTransform;

private:

  /*!
   * \brief inicializador de la clase
   */
  void init();
  void initGrid();
  void initGeoid();

private:
  
  /*!
   * \brief Código EPSG del sistema de referencia
   */
  std::string mEpsg;

  /*!
   * \brief Rejilla de transformación de sistema de coordenadas
   */
  std::string mGrid;

  /*!
   * \brief Geoide
   */
  std::string mGeoid;

  // TODO: como puntero no hay manera. No hay manera de destruirlo sin que de un error...
  //OGRSpatialReference *pCrs;

  /*!
   * \brief Objeto OGRSpatialReference de Gdal
   */
  OGRSpatialReference mCrs;

};

inline std::string Crs::epsgCode() const
{
  return mEpsg;
}

inline bool Crs::isGeocentric() const
{
  return mCrs.IsGeocentric() != 0;
}

inline bool Crs::isGeographic() const
{
  return mCrs.IsGeographic() != 0;
}

inline OGRSpatialReference *Crs::getOGRSpatialReference()
{
  return &mCrs;
}


/* ---------------------------------------------------------------------------------- */

///TODO: Carga de un listado de sistemas de referencia soportados.
///      Deberian mostrarse los códigos EPSG, tipo de proyección, permitir busqueda por nombre y por zona


/* ---------------------------------------------------------------------------------- */

#endif // HAVE_GDAL

} // End namespace geospatial

} // End namespace tl


#endif // TL_GEOSPATIAL_CRS_H
