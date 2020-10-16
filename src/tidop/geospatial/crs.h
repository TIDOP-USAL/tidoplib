#ifndef TL_GEOSPATIAL_CRS_H
#define TL_GEOSPATIAL_CRS_H

#include "tidop/core/defs.h"

#include <string>

class OGRSpatialReference;

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

  Crs();

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
  void setEpsgCode(const std::string &epsg);
  bool isGeocentric() const;
  bool isGeographic() const;
  std::string exportToProj() const;
  std::string exportToWkt() const;
  bool isValid() const;

protected:

  OGRSpatialReference *getOGRSpatialReference( );

  template<typename Point_t> friend class CrsTransform;

private:

  /*!
   * \brief inicializador de la clase
   */
  void initFromEpsg();
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

  /*!
   * \brief Objeto OGRSpatialReference de Gdal
   */
  OGRSpatialReference *mCrs;

};


/* ---------------------------------------------------------------------------------- */

///TODO: Carga de un listado de sistemas de referencia soportados.
///      Deberian mostrarse los códigos EPSG, tipo de proyección, permitir busqueda por nombre y por zona


/* ---------------------------------------------------------------------------------- */

#endif // HAVE_GDAL

} // End namespace geospatial

} // End namespace tl


#endif // TL_GEOSPATIAL_CRS_H
