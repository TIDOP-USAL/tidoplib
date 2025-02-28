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

#include <cmath>

namespace tl
{

/*! \addtogroup Statistics
 *  \{
 */

/*!
 * \enum KurtosisMethod
 * \brief Enum representing different methods for calculating kurtosis.
 *
 * The KurtosisMethod enum defines various approaches for calculating kurtosis. 
 * These methods describe the shape of the distribution's tails and peak.
 * Available methods include:
 * - \c pearson: The traditional Pearson method based on the fourth central moment.
 * - \c moors: The Moors method, which uses quantiles (octiles) for a robust measure of kurtosis.
 */
enum class KurtosisMethod
{
    pearson, ///< Pearson's method for kurtosis based on moments.
    moors    ///< Moors method based on quantiles (octiles).
};


/*!
 * \class Kurtosis
 * \brief Abstract base class for calculating kurtosis.
 *
 * This class serves as a base class for different algorithms that calculate kurtosis 
 * for a given dataset. The specific method used depends on the chosen \c KurtosisMethod.
 * 
 * \tparam T The data type for which the kurtosis is calculated (e.g., double, int).
 */
template<typename T>
class Kurtosis
  : public internal::Algorithm<T>
{

public:

    Kurtosis() = default;
    ~Kurtosis() override = default;

};


/*!
 * \class KurtosisFactory
 * \brief Factory class for creating instances of the Kurtosis algorithms.
 *
 * The KurtosisFactory class is responsible for creating and returning instances 
 * of different kurtosis calculation methods based on the selected \c KurtosisMethod.
 */
template<typename T>
class KurtosisFactory
{

private:

    KurtosisFactory() = default;

public:

    /*!
     * \brief Creates a Kurtosis object based on the specified method.
     * \param[in] method The method to use for kurtosis calculation, as defined in KurtosisMethod.
     * \see KurtosisMethod
     * \return A shared pointer to the selected Kurtosis algorithm.
     *
     * This method returns a Kurtosis object that uses the specified method to calculate kurtosis for the given data type.
     */
    static auto create(KurtosisMethod method) -> std::shared_ptr<Kurtosis<T>>;
};

#include "tidop/math/statistic/kurtosis.impl.h"

/*! \} */ 

} // End namespace tl
