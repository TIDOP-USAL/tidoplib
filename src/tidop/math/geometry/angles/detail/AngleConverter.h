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

#include "tidop/core/base/Concepts.h"

namespace tl
{

template<Floating T> class Degrees;
template<Floating T> class Gradians;
template<Floating T> class Radians;

/*! \addtogroup AngleConversion
 *  \{
 */

namespace detail 
{

/*!
 * \brief Class for converting between different angle units.
 *
 * The `AngleConverter` class provides static methods to convert angles
 * between degrees, radians, and gradians.
 *
 * \tparam T The type used for the angle values, typically a floating-point type like `float` or `double`.
 */
template <Floating T>
class AngleConverter
{

public:

    AngleConverter() = delete;

    /*!
     * \brief Convert an angle from degrees to radians.
     * \param[in] degrees The angle in degrees.
     * \param[out] radians The converted angle in radians.
     */
    static constexpr void convert(const Degrees<T> &degrees,
                                  Radians<T> &radians) noexcept
    {
        radians = degrees.value() * consts::deg_to_rad<T>;
    }

    /*!
     * \brief Convert an angle from degrees to gradians.
     * \param[in] degrees The angle in degrees.
     * \param[out] gradians The converted angle in gradians.
     */
    static constexpr void convert(const Degrees<T> &degrees,
                                  Gradians<T> &gradians) noexcept
    {
        gradians = degrees.value() * consts::deg_to_grad<T>;
    }

    /*!
     * \brief Convert an angle from gradians to radians.
     * \param[in] gradians The angle in gradians.
     * \param[out] radians The converted angle in radians.
     */
    static constexpr void convert(const Gradians<T> &gradians,
                                  Radians<T> &radians) noexcept
    {
        radians = gradians.value() * consts::grad_to_rad<T>;
    }

    /*!
     * \brief Convert an angle from gradians to degrees.
     * \param[in] gradians The angle in gradians.
     * \param[out] degrees The converted angle in degrees.
     */
    static constexpr void convert(const Gradians<T> &gradians,
                                  Degrees<T> &degrees) noexcept
    {
        degrees = gradians.value() * consts::grad_to_deg<T>;
    }

    /*!
     * \brief Convert an angle from radians to degrees.
     * \param[in] radians The angle in radians.
     * \param[out] degrees The converted angle in degrees.
     */
    static constexpr void convert(const Radians<T> &radians,
                                  Degrees<T> &degrees) noexcept
    {
        degrees = radians.value() * consts::rad_to_deg<T>;
    }

    /*!
     * \brief Convert an angle from radians to gradians.
     * \param[in] radians The angle in radians.
     * \param[out] gradians The converted angle in gradians.
     */
    static constexpr void convert(const Radians<T> &radians,
                                  Gradians<T> &gradians) noexcept
    {
        gradians = radians.value() * consts::rad_to_grad<T>;
    }

};

} // namespace detail

/*! \} */

} // namespace tl
