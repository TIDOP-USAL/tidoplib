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

#include "tidop/core/base/defs.h"
#include "GeoToolsDefinitions.h"

namespace tl
{

class CRSsTools;

/*! \addtogroup GeoToolsGroup
 *  \{
 */

/*!
 * \class GeoTools
 * \brief Singleton interface for accessing geospatial tools and CRS utilities.
 *
 * The GeoTools class provides centralized access to coordinate reference system (CRS) transformation tools 
 * through the CRSsTools interface. It is implemented as a singleton and manages the lifecycle of the 
 * CRS tools internally. This class offers a convenient entry point for applications that require CRS 
 * transformations or metadata queries.
 */
class TL_EXPORT GeoTools
{

public:

    /*!
     * \brief Retrieve the singleton instance of GeoTools.
     * \return Pointer to the singleton instance.
     */
    static GeoTools* getInstance(void )
    {
        static GeoTools instance;
        return &instance;
    }

    /*!
     * \brief Destructor.
     *
     * Automatically cleans up any allocated CRS tools.
     */
    ~GeoTools();

public:

    /*!
     * \brief Initialize the internal CRSsTools implementation.
     * \param ignoreDeprecatedCRSs If true, deprecated CRSs will be ignored during initialization.
     */
    void initializeCRSsTools(bool ignoreDeprecatedCRSs = true);

    /*!
     * \brief Get a pointer to the internal CRSsTools implementation.
     * \return Pointer to CRSsTools.
     */
    CRSsTools* ptrCRSsTools();

    TL_DISABLE_COPY(GeoTools)
    TL_DISABLE_MOVE(GeoTools)

protected:

    GeoTools(){ mPtrCRSsTools = nullptr; };

private:

    void clear();

private:

	CRSsTools* mPtrCRSsTools;
};

/*! \} */

}
