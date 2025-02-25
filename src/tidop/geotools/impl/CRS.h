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

#include <ogrsf_frmts.h>

#include "CRSsToolsDefinitions.h"

namespace tl {
    struct CRSInfo;
    class CRS : public OGRSpatialReference
    {
    public:
        CRS(struct CRSInfo&,
            bool oamsTraditionalGisOrder = true);
        CRS(std::string strEpsgCodes,
            std::map<std::string, CRSInfo>& crssInfo,
            bool oamsTraditionalGisOrder = true);
        CRS(std::string projString,
            bool oamsTraditionalGisOrder = true);
        ~CRS();
        std::string getBaseCrsId(PJ_CONTEXT* projContext);
        std::string getEllipsoidString() { return(mEllipsoidString); };
        int getEpsgCode() { return(mEpsgCode); };
        std::string getId() { return(mId); };
        int getPrecision() { return(mPrecision); };
        int getVerticalEpsgCode() { return(mVerticalEpsgCode); };
        int getVerticalPrecision() { return(mVerticalPrecision); };
        void setBaseCrsId(std::string value) { mBaseCrsId = value; };
    private:
        std::string mId; // like "EPSG:4326", "ENU:4937;-4.495021180808;36.756413127079;142.1590"
        int mEpsgCode = CRS_EPSG_CODE_NULL;
        int mVerticalEpsgCode = CRS_EPSG_CODE_NULL;
        bool mIsENU = false;
        int mPrecision = CRSSTOOLS_COORDINATE_LINEAR_PRECISION;
        int mVerticalPrecision = CRSSTOOLS_COORDINATE_LINEAR_PRECISION;
        std::string mEllipsoidString;
        bool mOamsTraditionalGisOrder;
        std::string mBaseCrsId;
    };
}
