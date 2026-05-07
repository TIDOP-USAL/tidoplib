/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by David Hernandez Lopez                            *
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

#include <string>

namespace tl 
{

/*! \addtogroup GeoToolsGroup
 *  \{
 */

 /*!
  * \struct CRSInfo
  * \brief Metadata container for a Coordinate Reference System (CRS).
  *
  * This struct holds descriptive information about a coordinate reference system,
  * including its authority, name, code, type, applicable area, and projection method.
  * It also indicates whether the CRS is deprecated.
  */
struct CRSInfo
{
    /*!
     * \brief Name of the authority defining the CRS (e.g., "EPSG").
     */
    std::string auth_name;

    /*!
     * \brief Human-readable name of the CRS.
     */
    std::string name;

    /*!
     * \brief Code of the CRS within the authority (e.g., "4326").
     */
    std::string code;

    /*!
     * \brief Type of the CRS (e.g., "geographic", "projected", "vertical").
     */
    std::string type;

    /*!
     * \brief Name of the geographical area where the CRS is valid.
     */
    std::string area_name;

    /*!
     * \brief Name of the projection method used (if applicable).
     */
    std::string projection_method_name;

    /*!
     * \brief Indicates whether the CRS is deprecated.
     */
    bool deprecated = false;
};

/*! \} */

}
