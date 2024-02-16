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

#include "tidop/core/defs.h"
#include "tidop/math/statistic/algorithm.h"

namespace tl
{

/*! \addtogroup math
 *  \{
 */


/*! \addtogroup statistics Statistics
 *  \{
 */


enum class SkewnessMethod
{
    fisher_pearson, //adjusted Fisher - Pearson standardized moment coefficient
    pearson_mode,
    bowley,
    momental
};


template<typename T>
class Skewness
  : public internal::Algorithm<T>
{

public:

    Skewness()
    {
    }
    ~Skewness() override
    {
    }

};



template<typename T>
class SkewnessFactory
{

private:

    SkewnessFactory() = default;

public:

    /*!
     * \brief
     * \param[in] method
     * \see SkewnessMethod
     * \return
    */
    static auto create(SkewnessMethod method) -> std::shared_ptr<Skewness<T>>;
};

#include "tidop/math/statistic/skewness.impl.h"

/*! \} */ // end of statistic

/*! \} */ // end of math

} // End namespace tl
