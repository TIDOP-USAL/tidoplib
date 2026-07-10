/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by Esteban Ruiz de O�a Crespo                       *
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

#include "tidop/rastertools/img.h"

#include "tidop/core/base/Flags.h"
#include "tidop/core/base/StringUtils.h"

#ifdef TL_HAVE_EDSDK
#include "EDSDK.h"
#ifndef HAVE_RAW
#  define HAVE_RAW
#endif
#endif // TL_HAVE_EDSDK

#ifdef TL_HAVE_OPENCV
#include "opencv2/core/core.hpp"
#endif // TL_HAVE_OPENCV


namespace tl
{


#ifdef TL_HAVE_OPENCV

int dataTypeToOpenCVDataType(DataType dataType)
{
    int ret;
    switch (dataType) {
    case DataType::TL_8U:
        ret = CV_8U;
        break;
    case DataType::TL_8S:
        ret = CV_8S;
        break;
    case DataType::TL_16U:
        ret = CV_16U;
        break;
    case DataType::TL_16S:
        ret = CV_16S;
        break;
    case DataType::TL_32U:
        ret = CV_32S;
        break;
    case DataType::TL_32S:
        ret = CV_32S;
        break;
    case DataType::TL_32F:
        ret = CV_32F;
        break;
    case DataType::TL_64F:
        ret = CV_64F;
        break;
    default:
        ret = -1;
        break;
    }
    return ret;
}

DataType openCVDataTypeToDataType(int dataType)
{
    DataType data_type;

    switch (dataType) {
    case CV_8U:
        data_type = DataType::TL_8U;
        break;
    case CV_8S:
        data_type = DataType::TL_8S;
        break;
    case CV_16U:
        data_type = DataType::TL_16U;
        break;
    case CV_16S:
        data_type = DataType::TL_16S;
        break;
    case CV_32S:
        data_type = DataType::TL_32S;
        break;
    case CV_32F:
        data_type = DataType::TL_32F;
        break;
    case CV_64F:
        data_type = DataType::TL_64F;
        break;
    default:
        data_type = DataType::TL_8U;
        break;
    }

    return data_type;
}

#endif // TL_HAVE_OPENCV


#ifdef TL_HAVE_EDSDK

std::unique_ptr<RegisterEDSDK> RegisterEDSDK::sRegisterEDSDK;
std::mutex RegisterEDSDK::sMutex;

RegisterEDSDK::RegisterEDSDK() 
{
    EdsInitializeSDK();
}

RegisterEDSDK::~RegisterEDSDK() 
{
    EdsTerminateSDK();
}

void RegisterEDSDK::init()
{
    if (sRegisterEDSDK.get() == nullptr) {
        std::lock_guard<std::mutex> lck(RegisterEDSDK::sMutex);
        if (sRegisterEDSDK.get() == nullptr) {
            sRegisterEDSDK.reset(new RegisterEDSDK());
        }
    }
}

#endif // TL_HAVE_EDSDK

} // End namespace tl
