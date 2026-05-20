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
 * MERCHANTABILITY or FITNESS FOR A2 PARTICULAR PURPOSE.  See the         *
 * GNU Lesser General Public License for more details.                    *
 *                                                                        *
 * You should have received a copy of the GNU Lesser General Public       *
 * License along with TidopLib. If not, see <http://www.gnu.org/licenses> *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#pragma once

#include <ranges>
#include "tidop/math/statistic/algorithms/descriptive/Quantile.h"
#include "tidop/math/base/Concepts.h"

namespace tl
{

/*! \addtogroup Statistics
 *  \{
 */

template<NumericRange R>
auto median(R &&range)
{
    return quantile(std::forward<R>(range), 0.5);
}

template<typename It>
auto median(It first, It last)
{
    return median(std::ranges::subrange(first, last));
}

/*! \} */

} // End namespace tl
