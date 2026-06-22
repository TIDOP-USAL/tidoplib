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

#include <map>
#include <string>
#include <memory>

#include "tidop/config.h"
#include "tidop/core/base/Path.h"
//#include "tidop/core/base/macros/SmartPtr.h"
#include "tidop/geometry/spatial/BoundingBox.h"

//#include <proj.h>
//
//#include "tidop/geotools/GeoTools.h"
//#include "PointCloudToolsDefinitions.h"

namespace tl
{

/*! \addtogroup PointCloudToolsIO
 *  \{
 */

class PointCloudReaderBase;

/*!
 * \brief Concrete wrapper class for reading point cloud files.
 *
 * It manages an internal `PointCloudReaderBase` instantiated via `PointCloudReaderFactory`.
 */
class TL_EXPORT PointCloudReader
{

private:

    std::unique_ptr<PointCloudReaderBase> mReader;
	
public:

    PointCloudReader();
    PointCloudReader(tl::Path file);
    ~PointCloudReader();

    void open(const tl::Path &file);
    auto isOpen() const -> bool;
    void close();

    void copcDumpBoundingBoxToCsv(const std::string &fileName, std::string crsId = "");
    void copcGetResolutionByLevel(std::map<int, double> &resolutionByLevel);

    void getBoundingBox(double &x_min, double &y_min, double &z_min,
                        double &x_max, double &y_max, double &z_max, std::string crsId = "");
    auto getBoundingBox(std::string crsId = "") const -> BoundingBox<Point3d>;
    auto getDimensionsNames() const -> std::vector<std::string>;
    bool getIsCopc() const;

    void getPoints(double &x_o, double &y_o, double &z_o,
                   std::vector<std::vector<float> > &coordinates,
                   std::vector<std::string> dimensionsNames,
                   std::vector<std::vector<float> > &dimensionsValues,
                   std::string crsId = "");

    void getPoints(double &x_o, double &y_o, double &z_o,
                   std::vector<std::vector<float> > &coordinates,
                   std::vector<std::string> dimensionsNames,
                   std::vector<std::vector<float> > &dimensionsValues,
                   double x_min, double y_min, double z_min,
                   double x_max, double y_max, double z_max,
                   std::string crsId = "");

    void getPoints(double &x_o, double &y_o, double &z_o,
                   std::vector<std::vector<float> > &coordinates,
                   std::vector<std::string> dimensionsNames,
                   std::vector<std::vector<float> > &dimensionsValues,
                   double resolution,
                   std::string crsId = "");

    void getPoints(double &x_o, double &y_o, double &z_o,
                   std::vector<std::vector<float> > &coordinates,
                   std::vector<std::string> dimensionsNames,
                   std::vector<std::vector<float> > &dimensionsValues,
                   double x_min, double y_min, double z_min,
                   double x_max, double y_max, double z_max,
                   double resolution,
                   std::string crsId = "");

    auto getOffset() const -> Point3<double>;
    auto getCoordinates(int index) const -> Point3<double>;
    auto getField(int index, const std::string &name) const -> double;
    auto hasColors() const -> bool;
    auto hasNormals() const -> bool;

};

/*! \} */

}
