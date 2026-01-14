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

#include "tidop/config.h"

#include <cstdint>
#include <type_traits>

namespace tl
{
	
/*! \addtogroup GeometricEntities
 *  \{
 */

enum class Dimension : std::uint8_t
{
    dim2 = 2,
    dim3 = 3,
    dim4 = 4,
    dynamic = 255  // Para geometrías con dimensión variable
};

template<std::size_t N>
struct dimension_constant
{
    static constexpr std::size_t value = N;
    static constexpr Dimension enum_value = static_cast<Dimension>(N);

    // Conversión a/desde enum
    static constexpr dimension_constant from_enum(Dimension d) {
        switch (d) {
            case Dimension::dim2: 
                return dimension_constant<2>{};
            case Dimension::dim3: 
                return dimension_constant<3>{};
            case Dimension::dim4: 
                return dimension_constant<4>{};
        default: return dimension_constant<0>{};
        }
    }
};

// Aliases comunes
using dim2 = dimension_constant<2>;
using dim3 = dimension_constant<3>;
using dim4 = dimension_constant<4>;

// Helper functions
constexpr auto is_valid_dimension(Dimension d) -> bool
{
    return d == Dimension::dim2 ||
           d == Dimension::dim3 ||
           d == Dimension::dim4;
}

constexpr auto dimension_value(Dimension d) -> size_t
{
    return d == Dimension::dynamic ? 0 : static_cast<size_t>(d);
}

// Traits para obtener dimensión de tipos
template<typename T>
struct dimension_of
{
    static constexpr Dimension value = Dimension::dynamic;
};

/*! \} */ 

}