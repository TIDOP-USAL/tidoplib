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


#include "tidop/core/defs.h"

#if defined TL_HAVE_GDAL && (defined TL_HAVE_PROJ4 || defined TL_HAVE_PROJ)

#include <string>

class OGRSpatialReference;

namespace tl
{

/*!
 * \addtogroup geospatial
 *
 * \{
 */


class CrsTransform;

/*!
 * \brief Clase sistema de referencia
 */
class TL_EXPORT Crs
{

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
    auto epsgCode() const -> std::string;
    void setEpsgCode(const std::string &epsg);
    auto toProjFormat() const -> std::string;
    void fromProjFormat(const std::string &proj);
    auto toWktFormat() const -> std::string;
    void fromWktFormat(const std::string &wkt);

    auto isGeocentric() const -> bool;
    auto isGeographic() const -> bool;
    auto isValid() const -> bool;

protected:

    OGRSpatialReference *getOGRSpatialReference();
    friend class CrsTransform;

private:

    /*!
     * \brief inicializador de la clase
     */
    void initFromEpsg();
    void initGrid();
    void initGeoid();

};


/* ---------------------------------------------------------------------------------- */

///TODO: Carga de un listado de sistemas de referencia soportados.
///      Deberian mostrarse los códigos EPSG, tipo de proyección, permitir busqueda por nombre y por zona



/*! \} */ // end of geospatial

} // End namespace tl

#endif // TL_HAVE_GDAL && defined TL_HAVE_PROJ
