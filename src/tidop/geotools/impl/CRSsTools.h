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

#ifndef GEOTOOLS_CRSTOOLS_H
#define GEOTOOLS_CRSTOOLS_H

#include <map>

#include <proj.h>

#include "../CRSsTools.h"

class OGRCoordinateTransformation;

namespace tl {
  
class CRS;
class CRSsToolsImpl: public CRSsTools{
public:
    CRSsToolsImpl(bool oamsTraditionalGisOrder=true,
        bool ignoreDeprecated=true);
    ~CRSsToolsImpl();
    void crsOperation(std::string crsSourceId, std::string crsTargetId,
        double& fc, double& sc, double& tc) override;
    void crsOperation(std::string crsSourceId, std::string crsTargetId,
        std::map<std::string, std::vector<double> >& points, bool byPoint = false) override;
    void crsOperation(std::string crsSourceId, std::string crsTargetId,
        std::vector<std::vector<double> >& points, bool byPoint = false) override;
    void dumpCRSsInfoToFile(std::string fileName) override;
    std::string getCRSEnu(std::string crsId, double fc, double sc, double tc) override;
    void getCRSsInfo(std::map<std::string, CRSInfo>&) override;
    void getCRSPrecision(std::string crsId, int& crsPrecision, int& crsVerticalPrecision) override;
    void getCRSsFor2dApplications(std::map<std::string, CRSInfo>&) override;
    void getCRSsVertical(std::string crsId, std::map<std::string, CRSInfo>&) override;

private:
    void clear();
    CRS* getCRS(std::string crsId);
    OGRCoordinateTransformation* getCRSOperation(std::string sourceCrsId, std::string targetCrsId);
    PJ* getCRSOperationEcefToEnu(std::string crsEnuId);
    void initialize();
    //std::string pjTypeToString(PJ_TYPE type);
    bool mIsInitialized=false;
    bool mIgnoreDeprecated;
    bool mOamsTraditionalGisOrder;
    std::map<std::string, CRSInfo> mCRSsInfo;
    std::map<std::string, CRS*> mPtrCRSs;
    //std::map<std::string, CRS*> mPtrTopocentricCRSs;
    std::map<std::string, std::map<std::string, OGRCoordinateTransformation*> > mPtrCRSsOperations;
    std::map<std::string, PJ*> mPtrCRSsOperationsEcefToEnuByCRSEnuId;
    std::map<std::string, std::string> mCRSGeo3dIdByCRSEnuId;
    //std::map<std::string, std::string> mCRSBaseIdByCRSId;
    std::map<std::string, std::vector<std::string> > mCRSsVerticalIdByBaseCRSId;
    PJ_CONTEXT* mProjContext;
};
}

#endif