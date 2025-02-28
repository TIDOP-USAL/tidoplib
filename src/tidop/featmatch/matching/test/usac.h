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

#include "tidop/featmatch/matching/test/geomtest.h"

#include <opencv2/calib3d.hpp>

namespace tl
{


/*!
 * \addtogroup GeometricTest
 *  \{
 */


#if (CV_VERSION_MAJOR > 4 || (CV_VERSION_MAJOR == 4 && CV_VERSION_MINOR > 4))
   
/*!
 * \brief Properties for USAC geometric test.
 */
struct TL_EXPORT UsacTestProperties
  : GeometricTest::Properties
{
    double confidence{0.99}; ///< Confidence level for the test.
    bool isParallel{false};  ///< Flag to indicate if the test should run in parallel.
    int	loIterations{5};     ///< Number of iterations for local optimization
    cv::LocalOptimMethod loMethod{cv::LocalOptimMethod::LOCAL_OPTIM_INNER_LO}; ///< Local optimization method.
    int loSampleSize{14};   ///< Sample size for local optimization.
    int maxIterations{5000};  ///< Maximum number of iterations.
    cv::NeighborSearchMethod neighborsSearch{cv::NeighborSearchMethod::NEIGH_GRID}; ///< Neighbor search method.
    int randomGeneratorState{0}; ///< Random generator state.
    cv::SamplingMethod sampler{cv::SamplingMethod::SAMPLING_UNIFORM}; ///< Sampling method.
    cv::ScoreMethod score{cv::ScoreMethod::SCORE_METHOD_MSAC}; ///< Scoring method.
    double threshold{1.5}; ///< Threshold for inlier detection.
};

#endif


/*! \} */


} // namespace tl
