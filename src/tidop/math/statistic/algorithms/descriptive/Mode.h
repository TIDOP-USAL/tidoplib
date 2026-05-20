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

#include <ranges>
#include <map>
#include <algorithm>
#include "tidop/math/base/Concepts.h"

namespace tl
{

/*! \addtogroup Statistics
 * \{
 */

template<NumericRange R>
auto mode(R &&range) -> std::ranges::range_value_t<R>
{
    using T = std::remove_cv_t<std::ranges::range_value_t<R>>;

    std::map<T, int> h;
    for (auto &&value : range) {
        h[value]++;
    }

    auto max = std::max_element(h.begin(), h.end(),
                                [](const std::pair<T, int> &p1,
                                const std::pair<T, int> &p2) {
                                    return p1.second < p2.second;
                                });

    return max->first;
}

template<typename It>
auto mode(It first, It last)
{
    return mode(std::ranges::subrange(first, last));
}

/*! \} */

} // End namespace tl
