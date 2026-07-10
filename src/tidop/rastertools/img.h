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
#include <limits>

#include "tidop/core/base/Defs.h"
#include "tidop/core/base/Type.h"
#include "tidop/rastertools/DataType.h"

namespace tl
{

template<typename T> class EnumFlags;

/*! \addtogroup RasterTools
 *  \{
 */

/// \cond

template <typename T, typename Enable = void>
struct NoDataValue
{
    static constexpr T value = static_cast<T>(-9999);
};

template <typename T>
struct NoDataValue<T, typename enableIfFloating<T, T>::type>
{
    static constexpr T value = static_cast<T>(-9999.0L);
};

template <typename T>
struct NoDataValue<T, typename enableIfUnsigned<T, T>::type>
{
    static constexpr T value = static_cast<T>(0);
};

template <>
struct NoDataValue<signed char, void>
{
    static constexpr signed char value = std::numeric_limits<signed char>::min();
};

/// \endcond

/*!
 * \brief Provides a type-specific constant value to mark invalid or missing data (NoData).
 *
 * This constant variable template safely retrieves the most appropriate NoData value
 * based on the underlying type T by accessing the specialized auxiliary struct 'NoDataValue<T>'.
 *
 * This approach ensures that the NoData marker is physically valid for the type:
 *
 * - **Floating-Point Types (float, double):** The value is typically set to -9999.0.
 * - **Unsigned Integral Types (unsigned short, unsigned int):** The value is typically set to 0.
 * - **Signed Integral Types (int, short):** The value is typically set to -9999.
 * - **Signed Char (signed char):** Due to the limited range (-128 to 127), the value is set to **-128** to prevent overflow.
 *
 * \tparam T The type for which the NoData constant is requested (e.g., float, unsigned short).
 * \return The constexpr NoData value of type T.
 */
template<typename T>
constexpr T NoData = NoDataValue<T>::value;


TL_EXPORT int dataTypeToOpenCVDataType(DataType dataType);
TL_EXPORT DataType openCVDataTypeToDataType(int dataType);


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
