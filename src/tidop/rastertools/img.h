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

#include <string>
#include <vector>

#include "tidop/core/base/defs.h"
#include "tidop/rastertools/DataType.h"

namespace tl
{

template<typename T> class EnumFlags;

/*! \addtogroup RasterTools
 *  \{
 */


//TL_EXPORT std::vector<std::string> gdalValidExtensions();
//TL_EXPORT bool gdalValidExtensions(const std::string &extension);
//TL_EXPORT EnumFlags<DataType> gdalValidDataTypes(const std::string &format);
//
///*!
// * \brief Returns the GDAL driver name corresponding to a file extension.
// * If the extension does not match an available driver, it returns nullptr.
// * \param[in] extension File extension
// * \return GDAL driver name
// */
//TL_EXPORT std::string gdalDriverFromExtension(const std::string &extension);

TL_EXPORT int dataTypeToOpenCVDataType(DataType dataType);
TL_EXPORT DataType openCVDataTypeToDataType(int dataType);

//TL_EXPORT std::vector<int> gdalBandOrder(int channels);



#ifdef TL_HAVE_EDSDK

/*!
 * \brief Clase singleton para registrar la API de canon
 *
 */
class TL_EXPORT RegisterEDSDK
{

private:

    /*!
     * \brief Constructor privado
     */
    RegisterEDSDK();

public:

    ~RegisterEDSDK();

    RegisterEDSDK(RegisterEDSDK const &) = delete;
    void operator=(RegisterEDSDK const &) = delete;

    /*!
     * \brief Inicio de la API EDSDK
     */
    static void init();

private:

    static std::unique_ptr<RegisterEDSDK> sRegisterEDSDK;
    static std::mutex sMutex;
};

#endif // TL_HAVE_EDSDK

/*! \} */


} // End namespace tl
