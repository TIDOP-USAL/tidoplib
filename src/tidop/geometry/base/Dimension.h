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

/*! \file Dimensions.h
 * \brief Dimension types and utilities for geometric entities.
 *
 * This file defines the Dimension enumeration and related utilities for
 * working with dimensions in geometric calculations. It provides compile-time
 * and runtime dimension representations.
 * ### Enumerations
 * - \ref tl::Dimension : Dimension enumeration for geometric entities.
 * ### Classes
 * - \ref tl::dimension_constant : Compile-time dimension representation.
 * ### Type Aliases
 * - \ref tl::dim2, \ref tl::dim3, \ref tl::dim4 : Common dimension constants.
 * ### Functions
 * - \ref tl::is_valid_dimension : Validates dimension values.
 * - \ref tl::dimension_value : Converts dimension enum to numeric value.
 * - \ref tl::dimension_of : Traits class for dimension extraction.
 */

#pragma once

#include "tidop/config.h"

#include <cstdint>
#include <type_traits>

namespace tl
{
	
/*! \addtogroup Geometry
 *  \{
 */

/*!
 * \enum Dimension
 * \brief Enumeration representing the dimension of geometric entities.
 */
enum class Dimension : std::uint8_t
{
    dim2 = 2,       /*!< Two-dimensional geometry. */
    dim3 = 3,       /*!< Three-dimensional geometry. */
    dim4 = 4,       /*!< Four-dimensional geometry. */
    dynamic = 255   /*!< Dynamic dimension (size determined at runtime). */
};

/*!
 * \struct dimension_constant
 * \brief Compile-time dimension representation as a type.
 *
 * \tparam N Dimension value (2, 3, or 4).
 */
template<std::size_t N>
struct dimension_constant
{
    static constexpr std::size_t value = N;
    static constexpr Dimension enum_value = static_cast<Dimension>(N);

    // Conversión a/desde enum
    static constexpr dimension_constant from_enum(Dimension d) 
    {
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

// ALIASES

/*! \brief Alias for 2D dimension constant. */
using dim2 = dimension_constant<2>;

/*! \brief Alias for 3D dimension constant. */
using dim3 = dimension_constant<3>;

/*! \brief Alias for 4D dimension constant. */
using dim4 = dimension_constant<4>;


// HELPER FUNCTIONS

/*!
 * \brief Checks if a dimension enum represents a valid fixed dimension.
 * \param[in] d Dimension to check.
 * \return true if dimension is 2, 3, or 4; false otherwise.
 */
constexpr auto is_valid_dimension(Dimension d) -> bool
{
    return d == Dimension::dim2 ||
           d == Dimension::dim3 ||
           d == Dimension::dim4;
}

/*!
 * \brief Converts a dimension enum to its numeric value.
 * \param[in] d Dimension to convert.
 * \return Numeric dimension value (0 for dynamic).
 */
constexpr auto dimension_value(Dimension d) -> size_t
{
    return d == Dimension::dynamic ? 0 : static_cast<size_t>(d);
}

/*!
 * \struct dimension_of
 * \brief Traits class for extracting dimension from types.
 *
 * \tparam T Type to extract dimension from.
 * Specializations should define `static constexpr Dimension value`.
 */
template<typename T>
struct dimension_of
{
    static constexpr Dimension value = Dimension::dynamic;
};

/*! \} */ 

}