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

#ifndef PCTOOLS_POINTCLOUDREADER_H
#define PCTOOLS_POINTCLOUDREADER_H

#include <map>
#include <string>
#include <memory>

#include "tidop/config.h"
#include "tidop/core/defs.h"
#include "tidop/core/path.h"
#include "tidop/core/ptr.h"
#include "tidop/geometry/entities/bbox.h"

#include <proj.h>

#include "tidop/geotools/GeoTools.h"
#include "PointCloudToolsDefinitions.h"

namespace tl
{
    /*!
     * \brief Class for reading different image formats
     */
    class TL_EXPORT PointCloudReader
    {
        GENERATE_UNIQUE_PTR(PointCloudReader)

    public:

        PointCloudReader(tl::Path file);
        virtual ~PointCloudReader() = default;

        /*!
         * \brief Open the file
         */
        virtual void open() = 0;

        /*!
         * \brief Check if the file has been loaded correctly.
         */
        virtual auto isOpen() const -> bool = 0;

        /*!
         * \brief Close the file
         */
        virtual void close() = 0;
        /*!
         * \brief Reference system in WKT format
         */
        //virtual auto crsWkt() const->std::string = 0;

        virtual void copcDumpBoundingBoxToCsv(std::string fileName, std::string crsId = "") = 0;
        virtual void copcGetResolutionByLevel(std::map<int,double>& resolutionByLevel) = 0;

        auto file() const->tl::Path;

        virtual void getBoundingBox(double& x_min, double& y_min, double& z_min,
            double& x_max, double& y_max, double& z_max, std::string crsId="") = 0;
        virtual BoundingBoxd getBoundingBox(std::string crsId="") = 0;

        virtual void getDimensionsNames(std::vector<std::string>& values) = 0;

        virtual bool getIsCopc() = 0;

        virtual void getPoints(double& x_o, double& y_o, double& z_o,
            std::vector<std::vector<float> >& coordinates,
            std::vector<std::string> dimensionsNames,
            std::vector<std::vector<float> >& dimensionsValues,
            std::string crsId = "") = 0;

        virtual void getPoints(double& x_o, double& y_o, double& z_o, 
            std::vector<std::vector<float> >& coordinates,
            std::vector<std::string> dimensionsNames,
            std::vector<std::vector<float> >& dimensionsValues,
            double x_min, double y_min, double z_min,
            double x_max, double y_max, double z_max,
            std::string crsId = "") = 0;

        virtual void getPoints(double& x_o, double& y_o, double& z_o, 
            std::vector<std::vector<float> >& coordinates,
            std::vector<std::string> dimensionsNames,
            std::vector<std::vector<float> >& dimensionsValues,
            double resolution,
            std::string crsId = "") = 0;

        virtual void getPoints(double& x_o, double& y_o, double& z_o, 
            std::vector<std::vector<float> >& coordinates,
            std::vector<std::string> dimensionsNames,
            std::vector<std::vector<float> >& dimensionsValues,
            double x_min, double y_min, double z_min,
            double x_max, double y_max, double z_max,
            double resolution,
            std::string crsId = "") = 0;

    protected:
        GeoTools* mPtrGeoTools;

    private:
        Path mFile;
    };

    /*!
 * \brief Class factory for reading image formats
 */
    class TL_EXPORT PointCloudReaderFactory
    {

    private:

        PointCloudReaderFactory() = default;

    public:

        static auto create(const Path& file)->PointCloudReader::Ptr;
    };

}

#endif