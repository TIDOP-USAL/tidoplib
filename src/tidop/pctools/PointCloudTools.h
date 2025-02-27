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

#include "tidop/core/defs.h"
#include "PointCloudToolsDefinitions.h"

namespace tl{
    class GeoTools;
class TL_EXPORT PointCloudTools{
public:
    static inline PointCloudTools* getInstance()
    {
        static PointCloudTools instance;
        return &instance;
    };
    ~PointCloudTools();
    bool assignCRS(std::string fileName, std::string crsI, std::string outputFileName, 
        bool ignoreInternalCrs=true);
    std::string getCRSId(std::string fileName);
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
    GeoTools* mPtrGeoTools;
};

}
