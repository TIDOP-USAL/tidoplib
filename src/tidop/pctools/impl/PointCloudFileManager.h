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

#ifndef PCTOOLS_POINTCLOUDFILEMANAGER_INTERFACE_H
#define PCTOOLS_POINTCLOUDFILEMANAGER_INTERFACE_H

#include <map>

#include <proj.h>

#include "tidop/geotools/GeoTools.h"

#include "../PointCloudFileManager.h"


namespace tl {
  
class PointClooudFile;
class PointCloudFileManagerImpl: public PointCloudFileManager{
public:
    PointCloudFileManagerImpl();
    ~PointCloudFileManagerImpl();
    void addPointCloudFile(std::string fileName) override;

private:
    void clear();
    void initialize();
    GeoTools* mPtrGeoTools;
};
}

#endif