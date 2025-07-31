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
#include <string>

#include "tidop/core/base/defs.h"
#include "PointCloudToolsDefinitions.h"

namespace tl
{

class GeoTools;

/*! \addtogroup PointCloudTools
 *  \{
 */

/*!
 * \brief Static utility class for point cloud CRS handling and format conversion.
 *
 * The PointCloudTools class provides a set of tools for:
 * - Assigning and transforming coordinate reference systems (CRS).
 * - Converting point cloud files to COPC format.
 * - Extracting COPC spatial hierarchy information.
 * - Reading CRS identifiers embedded in point cloud files.
 *
 * This class is designed as a singleton and accessed via \ref getInstance().
 *
 * ### Example: Convert a LAS file to COPC with CRS assignment
 * \code
 * auto pctools = tl::PointCloudTools::getInstance();
 * pctools->formatFileConversionToCOPC("input.las", "output.copc.laz", "EPSG:25830");
 * \endcode
 */
class TL_EXPORT PointCloudTools
{

public:

    /*!
     * \brief Singleton accessor.
     * \return Pointer to the PointCloudTools instance.
     */
    static inline PointCloudTools* getInstance()
    {
        static PointCloudTools instance;
        return &instance;
    };
    ~PointCloudTools();

    /*!
     * \brief Assigns or transforms a CRS in a LAS/LAZ file and writes the output.
     *
     * If the input file lacks CRS, or if \p ignoreInternalCrs is true, the specified CRS
     * will be applied. If the file already contains CRS and transformation is needed,
     * a reprojection is performed using PDAL.
     *
     * \param fileName Input LAS/LAZ file path.
     * \param crsI CRS identifier to assign (e.g., "EPSG:25830").
     * \param outputFileName Path for the output file.
     * \param ignoreInternalCrs If true, any embedded CRS in the file is ignored.
     * \return True if the operation was successful.
     */
    bool assignCRS(std::string fileName, std::string crsI, std::string outputFileName, 
                   bool ignoreInternalCrs=true);

    /*!
     * \brief Extracts the COPC spatial hierarchy from a file.
     *
     * Reads the 4D index of each node (D, X, Y, Z) in the COPC structure and outputs the
     * bounding box, resolution, and point count for each node.
     *
     * \param[out] fileName Path to a COPC (.laz) file.
     * \param[out] x_min_by_node_keys Map of minimum X values by node key.
     * \param[out] x_max_by_node_keys Map of maximum X values by node key.
     * \param[out] y_min_by_node_keys Map of minimum Y values by node key.
     * \param[out] y_max_by_node_keys Map of maximum Y values by node key.
     * \param[out] resolution_by_node_keys Resolution by node key.
     * \param[out] number_of_points_by_node_keys Number of points per node.
     */
    void getCOPCStruct(std::string fileName,
        std::map<int, std::map<int, std::map<int, std::map<int, double > > > >& x_min_by_node_keys,
        std::map<int, std::map<int, std::map<int, std::map<int, double > > > >& x_max_by_node_keys,
        std::map<int, std::map<int, std::map<int, std::map<int, double > > > >& y_min_by_node_keys,
        std::map<int, std::map<int, std::map<int, std::map<int, double > > > >& y_max_by_node_keys, 
        std::map<int, std::map<int, std::map<int, std::map<int, double > > > >& resolution_by_node_keys,
        std::map<int, std::map<int, std::map<int, std::map<int, int > > > >& number_of_points_by_node_keys);

    /*!
     * \brief Reads the CRS identifier embedded in a LAS/LAZ or COPC file.
     *
     * The CRS is returned as a string identifier in EPSG format (e.g., "EPSG:25830+5783").
     *
     * \param[in] fileName Path to the input point cloud file.
     * \return CRS identifier, or empty string if not found.
     */
    std::string getCRSId(std::string fileName);

    /*!
     * \brief Converts a point cloud to COPC format, with optional CRS transformation.
     *
     * Supported input formats: LAS, LAZ, PLY.
     *
     * - If both CRS IDs are provided and different, a reprojection is applied.
     * - If only source CRS is provided, it is assigned to the input.
     * - If the source CRS is ENU, an intermediate conversion to geographic/UTM is performed.
     *
     * \param[in] fileName Input point cloud file path.
     * \param[in] outputFileName Output COPC (.laz) file path.
     * \param[in] targetCrsId Target CRS (e.g., "EPSG:25830").
     * \param[in] sourceCrsId Optional source CRS. If empty, it will be detected or assumed.
     */
    void formatFileConversionToCOPC(std::string fileName, std::string outputFileName, 
                                    std::string targetCrsId, std::string sourceCrsId ="");

    //bool formatFileConversionToCOPC(std::string fileName, std::string outputFileName, 
    //    std::string targetCrsId ="");
    // public:
    // PointCloudFileManager* ptrPointCloudFileManager();

protected:

    PointCloudTools();

private:

    void clear();

private:

    GeoTools* mPtrGeoTools;
};

/*! \} */

}
