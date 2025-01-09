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

#ifndef GEOTOOLS_CRSTOOLS_INTERFACE_H
#define GEOTOOLS_CRSTOOLS_INTERFACE_H

#pragma once

#include <vector>
#include <map>
#include "tidop/core/base/defs.h"
#include "tidop/geometry/entities/point.h"

#include "GeoToolsDefinitions.h"

#if defined TL_HAVE_GDAL && (defined TL_HAVE_PROJ4 || defined TL_HAVE_PROJ)

#include <string>

namespace tl{

/*!
 * \brief CRSsTools class
 */
class TL_EXPORT CRSsTools{
public:
    inline CRSsTools(){};
    ~CRSsTools() {};
    virtual void crsOperation(std::string crsSourceId, std::string crsTargetId,
        double& fc, double&sc, double& tc) = 0;
    virtual Point3d crsOperation(const std::string &crsSourceId, const std::string &crsTargetId, const Point3d &point) = 0;
    virtual void crsOperation(std::string crsSourceId, std::string crsTargetId,
        std::vector<std::vector<double> >& points, bool byPoint=false) = 0;
    virtual void crsOperation(std::string crsSourceId, std::string crsTargetId,
        std::vector<Point3d> &points, bool byPoint = false) = 0;
    virtual void crsOperation(std::string crsSourceId, std::string crsTargetId,
        std::map<std::string, std::vector<double> >& points, bool byPoint = false) = 0;
    virtual void dumpCRSsInfoToFile(std::string fileName) = 0;
    virtual std::string getCRSEnu(std::string crsId, double fc, double sc, double tc) = 0;
    virtual std::string getCRSIdEllipsoidHeightsForPDAL(std::string crsId) = 0;
    virtual void getCRSsInfo(std::map<std::string, CRSInfo>&) = 0;
    virtual void getCRSPrecision(std::string crsId, int& crsPrecision, int& crsVerticalPrecision) = 0;
    virtual void getCRSsFor2dApplications(std::map<std::string, CRSInfo>&) = 0;
    virtual void getCRSsVertical(std::string crsId, std::map<std::string, CRSInfo>&) = 0;
    virtual bool getIsCRSEnu(std::string crsId) = 0;
    virtual bool getIsCRSGeographic(std::string crsId) = 0;
    virtual bool getIsCRSValid(std::string crsId) = 0;
    virtual void setCRSFromWkt(std::string wkt, std::string& crsId) = 0;

};

}
#endif 
#endif // GEOTOOLS_CRSTOOLS_INTERFACE_H
