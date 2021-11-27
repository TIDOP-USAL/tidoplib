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

  Crs(const Crs &crs);

  ~Crs();

  /*!
   * \brief Devuelve el código EPSG del sistema de referencia
   * \return
   */
  std::string epsgCode() const;
  void setEpsgCode(const std::string &epsg);
  std::string toProjFormat() const;
  void fromProjFormat(const std::string &proj);
  std::string toWktFormat() const;
  void fromWktFormat(const std::string &wkt);

  bool isGeocentric() const;
  bool isGeographic() const;
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
