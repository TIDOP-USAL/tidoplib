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

#include "tidop/core/base/defs.h"

#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>

namespace tl
{

/*! \addtogroup FeatureMatching
 *  \{
 */

/*!
 * \brief Base class for performing geometric tests, such as homography and fundamental matrix estimation.
 */
class TL_EXPORT GeometricTest
{

public:

    /*!
     * \brief Struct to hold properties specific to the geometric test.
     */
    struct Properties{};

    /*!
     * \brief Enum representing the type of geometric test.
     */
    enum class Type
    {
        homography_all_points, ///< Homography estimation using all points.
        homography_ransac,     ///< Homography estimation using RANSAC.
        homography_lmeds,      ///< Homography estimation using LMEDS.
        homography_rho,        ///< Homography estimation using RHO.
#if (CV_VERSION_MAJOR > 4 || (CV_VERSION_MAJOR == 4 && CV_VERSION_MINOR > 4))
        homography_usac,       ///< Homography estimation using USAC.
#endif
        fundamental_seven_points, ///< Fundamental matrix estimation using seven points.
        fundamental_eight_points, ///< Fundamental matrix estimation using eight points.
        fundamental_ransac,       ///< Fundamental matrix estimation using RANSAC.
        fundamental_lmeds         ///< Fundamental matrix estimation using LMEDS.
#if (CV_VERSION_MAJOR > 4 || (CV_VERSION_MAJOR == 4 && CV_VERSION_MINOR > 4))
        ,
        fundamental_usac          ///< Fundamental matrix estimation using USAC.
#endif
    };

private:

    Type mType;

public:

    /*!
     * \brief Constructor that initializes the geometric test with a specific type.
     * \param[in] type The type of geometric test.
     */
    GeometricTest(Type type) : mType(type) {}

    /*!
     * \brief Virtual destructor for the GeometricTest class.
     */
    virtual ~GeometricTest() = default;

public:

    /*!
     * \brief Retrieves the properties of the geometric test.
     * \return A pointer to the properties.
     */
    virtual auto properties() const -> const Properties* = 0;

    /*!
     * \brief Sets the properties of the geometric test.
     * \param[in] properties A pointer to the properties to set.
     */
    virtual void setProperties(const Properties *properties) = 0;

    /*!
     * \brief Executes the geometric test on two sets of points.
     * \param[in] points1 The first set of points.
     * \param[in] points2 The second set of points.
     * \return A vector of unsigned char with the inliers.
     */
    virtual auto exec(const std::vector<cv::Point2f> &points1,
                      const std::vector<cv::Point2f> &points2) -> std::vector<unsigned char> = 0;

    /*!
     * \brief Gets the type of the geometric test.
     * \return The type of geometric test.
     */
    auto type() const -> Type 
    {
        return mType;
    }

};



/*!
 * \brief Properties for All Points geometric test.
 */
struct TL_EXPORT AllPointsTestProperties
  : GeometricTest::Properties
{
    double confidence{0.999}; ///< Confidence level for the test.
};

/*!
 * \brief Properties for RANSAC geometric test.
 */
struct TL_EXPORT RANSACTestProperties
  : GeometricTest::Properties
{
    double distance{0.7};     ///< Distance threshold for inlier detection.
    double confidence{0.999}; ///< Confidence level for the test.
    int iterations{2000};     ///< Maximum number of iterations.
};

/*!
 * \brief Properties for LMEDS geometric test.
 */
struct TL_EXPORT LMedsTestProperties
  : GeometricTest::Properties
{
    double confidence{0.999}; ///< Confidence level for the test.
    int iterations{2000};     ///< Maximum number of iterations.
};

/*!
 * \brief Properties for RHO geometric test.
 */
struct TL_EXPORT RHOTestProperties
  : GeometricTest::Properties
{
    double distance{0.7};     ///< Distance threshold for inlier detection.
    double confidence{0.999}; ///< Confidence level for the test.
};

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



/*!
 * \brief Factory class for creating instances of geometric tests.
 */
class TL_EXPORT GeometricTestFactory
{

private:

    GeometricTestFactory() = default;

public:

    /*!
     * \brief Creates a geometric test of the specified type.
     * \param[in] type The type of geometric test to create.
     * \return A shared pointer to the created geometric test.
     */
    static auto create(GeometricTest::Type type) -> std::shared_ptr<GeometricTest>;

    /*!
     * \brief Creates a geometric test of the specified type with given properties.
     * \param[in] type The type of geometric test to create.
     * \param[in] properties The properties to configure the geometric test.
     * \return A shared pointer to the created geometric test.
     */
    static auto create(GeometricTest::Type type,
                       const GeometricTest::Properties *properties) -> std::shared_ptr<GeometricTest>;

};


/*! \} */


} // namespace tl
