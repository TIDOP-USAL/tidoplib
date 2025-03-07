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

#include "tidop/math/statistic/algorithm.h"

namespace tl
{

/*! \addtogroup Statistics
 *  \{
 */


/*!
 * \enum SkewnessMethod
 * \brief Enum representing the methods used to calculate skewness.
 *
 * The SkewnessMethod enum defines different approaches to calculating skewness, 
 * which measures the asymmetry of a distribution. The available methods are:
 *
 * - \c fisher_pearson: The adjusted Fisher-Pearson standardized moment coefficient.
 * - \c pearson_mode: The Pearson's mode skewness, calculated using the mode of the distribution.
 * - \c bowley: Skewness calculated using Bowley's method, which relies on quartiles.
 * - \c momental: Skewness based on the moment-based method, calculated using raw moments of the distribution.
 */
enum class SkewnessMethod
{
    fisher_pearson, ///< Adjusted Fisher-Pearson standardized moment coefficient
    pearson_mode,   ///< Pearson's mode skewness
    bowley,         ///< Skewness using Bowley's method
    momental        ///< Moment-based skewness method
};

/*!
 * \class Skewness
 * \brief Class representing a skewness algorithm for a given type of data.
 *
 * This class serves as a base class for calculating skewness in a dataset. 
 * It is templated to support various data types. Derived classes would implement 
 * specific methods for calculating skewness based on the chosen method (e.g., 
 * Fisher-Pearson, Pearson Mode, etc.).
 *
 * \tparam T The data type for which the skewness is calculated (e.g., double, int).
 */
template<typename T>
class Skewness
  : public internal::Algorithm<T>
{

public:

    Skewness() = default;
    ~Skewness() override = default;

};


/*!
 * \class SkewnessFactory
 * \brief Factory class for creating Skewness objects based on the selected method.
 *
 * This factory class is used to instantiate Skewness objects with different methods 
 * for calculating skewness. Depending on the method provided, the appropriate skewness 
 * calculation algorithm will be selected and returned.
 */
template<typename T>
class SkewnessFactory
{

private:

    SkewnessFactory() = default;

public:

    /*!
     * \brief Creates a Skewness object based on the specified method.
     * \param[in] method The method to use for skewness calculation, as defined in SkewnessMethod.
     * \see SkewnessMethod
     * \return A shared pointer to the selected Skewness algorithm.
     *
     * This method returns a Skewness object that uses the specified method to calculate skewness for the given data type.
     */
    static auto create(SkewnessMethod method) -> std::shared_ptr<Skewness<T>>;
};

#include "tidop/math/statistic/skewness.impl.h"

/*! \} */

} // End namespace tl
