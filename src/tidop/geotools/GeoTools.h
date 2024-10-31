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

#ifndef GEOTOOLS_GEOTOOLS_INTERFACE_H
#define GEOTOOLS_GEOTOOLS_INTERFACE_H

#include <vector>

#include "tidop/core/defs.h"
#include "GeoToolsDefinitions.h"

namespace tl{

class CRSsTools;

/*!
 * \brief GeoTools class
 */
class TL_EXPORT GeoTools{
public:
    static inline GeoTools* getInstance(void )
    {
        if (mInstance==0) mInstance = new GeoTools;
        return mInstance;
    };
    ~GeoTools();
public:
    void initializeCRSsTools(bool ignoreDeprecatedCRSs = true);
    CRSsTools* ptrCRSsTools();
protected:
    inline GeoTools(){ mPtrCRSsTools =NULL;};
    //void getCRSsInfo(std::vector<CRSInfo>&);
private:
    void clear();
    static GeoTools* mInstance;
	CRSsTools* mPtrCRSsTools;
};

}
#endif