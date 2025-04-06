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

#include "tidop/config.h"
#include "tidop/core/defs.h"
#include "tidop/core/path.h"
#include "tidop/core/ptr.h"
#include "tidop/pctools/PointCloudReader.h"

#include <pdal/io/PlyReader.hpp>
#include <pdal/PointView.hpp>
#include <pdal/PointTable.hpp>
#include <pdal/Options.hpp>
#include <stdexcept>
#include <vector>
#include <string>

#define TL_HAVE_PDAL 1

namespace tl
{

class Ply;

class PlyReader 
  : public PointCloudReader 
{
    GENERATE_UNIQUE_PTR(PlyReader)

public:

    PlyReader(tl::Path file);

    void open() override;
    bool isOpen() const override;
    void close() override;

    auto getBoundingBox(std::string crsId = "") const -> BoundingBoxd override;
    auto getDimensionsNames() const -> std::vector<std::string> override;
    auto getOffset() const -> Point3<double> override;
    auto getCoordinates(int index) const -> Point3<double> override;
    auto getField(int index, const std::string &name) const -> double override;
    auto hasColors() const -> bool override;
    auto hasNormals() const -> bool override;

    void copcDumpBoundingBoxToCsv(const std::string &fileName, std::string crsId = "") override {}
    void copcGetResolutionByLevel(std::map<int, double> &resolutionByLevel) override {}
    void getBoundingBox(double &x_min, double &y_min, double &z_min,
        double &x_max, double &y_max, double &z_max, std::string crsId = "") {}
    bool getIsCopc() const { return false; }

    void getPoints(double &x_o, double &y_o, double &z_o,
        std::vector<std::vector<float> > &coordinates,
        std::vector<std::string> dimensionsNames,
        std::vector<std::vector<float> > &dimensionsValues,
        std::string crsId = "") {
    }

    void getPoints(double &x_o, double &y_o, double &z_o,
        std::vector<std::vector<float> > &coordinates,
        std::vector<std::string> dimensionsNames,
        std::vector<std::vector<float> > &dimensionsValues,
        double x_min, double y_min, double z_min,
        double x_max, double y_max, double z_max,
        std::string crsId = "") {
    }

    void getPoints(double &x_o, double &y_o, double &z_o,
        std::vector<std::vector<float> > &coordinates,
        std::vector<std::string> dimensionsNames,
        std::vector<std::vector<float> > &dimensionsValues,
        double resolution,
        std::string crsId = "") {
    }

    void getPoints(double &x_o, double &y_o, double &z_o,
        std::vector<std::vector<float> > &coordinates,
        std::vector<std::string> dimensionsNames,
        std::vector<std::vector<float> > &dimensionsValues,
        double x_min, double y_min, double z_min,
        double x_max, double y_max, double z_max,
        double resolution,
        std::string crsId = "") {
    }

private:

#ifdef TL_HAVE_PDAL
    pdal::PlyReader mReader;
    pdal::PointTable mTable;
    pdal::PointViewSet mViewSet;
    pdal::PointViewPtr mView;
#else
    std::unique_ptr<Ply> mPly;
#endif // TL_HAVE_PDAL
    bool mIsOpen;
};

}