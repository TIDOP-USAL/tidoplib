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

#include <vector>
#include <map>
#include "tidop/core/base/defs.h"
#include "tidop/geometry/primitives/Point.h"
#include "tidop/geotools/GeoToolsDefinitions.h"

namespace tl
{

/*! \addtogroup GeoToolsGroup
 *  \{
 */

/*!
 * \class CRSsTools
 * \brief Interface for operations related to Coordinate Reference Systems (CRSs).
 *
 * This abstract class defines the interface for CRS transformations, queries, and utility functions 
 * for coordinate reference systems. It includes methods for transforming coordinates between CRSs,
 * retrieving CRS metadata, and handling various CRS formats and queries.
 *
 * Implementations of this class should provide specific logic for interacting with underlying 
 * geospatial libraries (e.g., GDAL/PROJ).
 */
class TL_EXPORT CRSsTools
{

public:

    CRSsTools() {};
    virtual ~CRSsTools() {};

    /*!
     * \brief Transform coordinates from one CRS to another.
     * \param[in] crsSourceId Source CRS identifier.
     * \param[in] crsTargetId Target CRS identifier.
     * \param[out] fc Reference to first coordinate (in/out).
     * \param[out] sc Reference to second coordinate (in/out).
     * \param[out] tc Reference to third coordinate (in/out).
     */
    virtual void crsOperation(std::string crsSourceId, std::string crsTargetId,
                              double& fc, double&sc, double& tc) = 0;

    /*!
     * \brief Transform a single 3D point between CRSs.
     * \param[in] crsSourceId Source CRS identifier.
     * \param[in] crsTargetId Target CRS identifier.
     * \param[in] point Input point in source CRS.
     * \return Transformed point in target CRS.
     */
    virtual Point3d crsOperation(const std::string &crsSourceId, 
                                 const std::string &crsTargetId, 
                                 const Point3d &point) = 0;

    /*!
     * \brief Transform a list of 3D points represented as vectors.
     * \param[in] crsSourceId Source CRS identifier.
     * \param[in] crsTargetId Target CRS identifier.
     * \param[out] points Input/output points in source/target CRS.
     * \param[in] byPoint If true, transform each point individually.
     */
    virtual void crsOperation(std::string crsSourceId, std::string crsTargetId,
                              std::vector<std::vector<double>> &points, 
                              bool byPoint = false) = 0;

    /*!
     * \brief Transform a list of 3D Points between CRSs.
     * \param[in] crsSourceId Source CRS identifier.
     * \param[in] crsTargetId Target CRS identifier.
     * \param[in] points Input points in source CRS.
     * \param[in] byPoint If true, transform each point individually.
     * \return Transformed points in target CRS.
     */
    virtual std::vector<Point3d> crsOperation(std::string crsSourceId, std::string crsTargetId,
                                              const std::vector<Point3d> &points, 
                                              bool byPoint = false) = 0;

    /*!
     * \brief Transform a map of 3D points between CRSs.
     * \param[in] crsSourceId Source CRS identifier.
     * \param[in] crsTargetId Target CRS identifier.
     * \param[out] points Map of point identifiers and coordinates (in/out).
     * \param[in] byPoint If true, transform each point individually.
     */
    virtual void crsOperation(std::string crsSourceId, std::string crsTargetId,
                              std::map<std::string, std::vector<double>> &points, 
                              bool byPoint = false) = 0;

    /*!
     * \brief Dump all known CRS metadata to a file.
     * \param fileName Path to output file.
     */
    virtual void dumpCRSsInfoToFile(const std::string &fileName) = 0;

    /*!
     * \brief Get the ENU (East-North-Up) CRS identifier from a base CRS and origin.
     * \param[in] crsId Base CRS identifier.
     * \param[in] fc First coordinate (x).
     * \param[in] sc Second coordinate (y).
     * \param[in] tc Third coordinate (z).
     * \return ENU CRS identifier.
     */
    virtual std::string getCRSEnu(std::string crsId, double fc, double sc, double tc) = 0;

    /*!
     * \brief Get a modified CRS ID with ellipsoidal heights for use with PDAL.
     * \param[in] crsId Input CRS identifier.
     * \return CRS ID adapted for ellipsoidal height handling.
     */
    virtual std::string getCRSIdEllipsoidHeightsForPDAL(std::string crsId) = 0;

    /*!
     * \brief Get metadata for all supported CRSs.
     * \return Map of CRS identifiers to their metadata.
     */
    virtual std::map<std::string, CRSInfo> getCRSsInfo() const = 0;

    /*!
     * \brief Get the precision of a given CRS.
     * \param[in] crsId CRS identifier.
     * \param[out] crsPrecision Output horizontal precision.
     * \param[out] crsVerticalPrecision Output vertical precision.
     */
    virtual void getCRSPrecision(std::string crsId, int &crsPrecision, int &crsVerticalPrecision) = 0;


    /*!
     * \brief Get all CRSs suitable for 2D applications.
     * \return Map of CRS identifiers to their metadata.
     */
    virtual std::map<std::string, CRSInfo> getCRSsFor2dApplications() const = 0;

    /*!
     * \brief Get vertical CRSs related to a base CRS.
     * \param[in] crsId Base CRS identifier.
     * \param[out] crssInfo Map of vertical CRS identifiers to their metadata.
     */
    virtual void getCRSsVertical(std::string crsId, std::map<std::string, CRSInfo> &crssInfo) = 0;

    /*!
     * \brief Check if a CRS is an ENU system.
     * \param[in] crsId CRS identifier.
     * \return True if CRS is ENU.
     */
    virtual bool getIsCRSEnu(const std::string &crsId) const = 0;

    /*!
     * \brief Check if a CRS is geographic (lat/lon).
     * \param[in] crsId CRS identifier.
     * \return True if CRS is geographic.
     */
    virtual bool getIsCRSGeographic(const std::string &crsId) = 0;

    /*!
     * \brief Check if a CRS identifier is valid and supported.
     * \param crsId CRS identifier.
     * \return True if CRS is valid.
     */
    virtual bool getIsCRSValid(const std::string &crsId) = 0;

    /*!
     * \brief Set a CRS identifier from a WKT definition.
     * \param[in] wkt Input Well-Known Text definition.
     * \param[out] crsId Output CRS identifier.
     */
    virtual void setCRSFromWkt(std::string wkt, std::string &crsId) = 0;

};

/*! \} */

}
