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

#include "tidop/geotools/GeoTools.h"
#include "tidop/geotools/impl/CRSsTools.h"

using namespace tl;


GeoTools::~GeoTools()
{
    clear();
}

void GeoTools::clear()
{
    if (mPtrCRSsTools) {
        delete mPtrCRSsTools;
        mPtrCRSsTools = nullptr;
    }
}

CRSsTools* GeoTools::ptrCRSsTools()
{
    if (mPtrCRSsTools == NULL)
    {
        initializeCRSsTools();
    }
    //return(dynamic_cast<CRSsTools*>(mPtrCRSsTools));
    return(mPtrCRSsTools);
}

//void GeoTools::getCRSsInfo(std::vector<CRSInfo>&)
//{
//    if (mPtrCRSsTools == NULL)
//    {
//        mPtrCRSsTools = new CRSsToolsImp();
//
//    }
//}

void GeoTools::initializeCRSsTools(bool ignoreDeprecatedCRSs)
{
    if (mPtrCRSsTools != NULL)
    {
        delete(mPtrCRSsTools);
        mPtrCRSsTools = NULL;
    }
    mPtrCRSsTools = new CRSsToolsImpl(ignoreDeprecatedCRSs);
}
