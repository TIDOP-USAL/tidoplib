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

namespace tl
{

/*! \addtogroup GeometricTest
 *  \{
 */

/*!
 * \brief Base class for performing geometric tests, such as homography and fundamental matrix estimation.
 * 
 *
 * This class provides an interface for performing geometric tests between two sets of matched points.
 * It supports various types of tests, including homography and fundamental matrix estimation,
 * using different algorithms such as RANSAC, LMEDS, and USAC.
 * 
 * #### Example Usage
 * \code{.cpp}
 * // Create a geometric test using RANSAC-based homography estimation
 * auto geometricTest = GeometricTestFactory::create(GeometricTest::Type::homography_ransac);
 *
 * // Define corresponding points from two images
 *   size_t size = matches.size();
 *   std::vector<cv::Point2f> points1(size);
 *   std::vector<cv::Point2f> points2(size);
 *   for (size_t i = 0; i < size; i++) {
 *       points1[i] = keypoints1[static_cast<size_t>(matches[i].queryIdx)].pt;
 *       points2[i] = keypoints2[static_cast<size_t>(matches[i].trainIdx)].pt;
 *   }
 *
 * // Execute the test to determine inliers
 * std::vector<unsigned char> inliers = geometricTest->exec(points1, points2);
 * std::vector<cv::DMatch> filter_matches;
 * for (size_t i = 0; i < nPoints; i++) {
 *     if (inliers[i])
 *         filter_matches.push_back(matches[i]);
 * }
 * \endcode
 */
class TL_EXPORT GeometricTest
{

public:

    /*!
     * \brief Struct to hold properties specific to the geometric test.
     * 
     * This struct can be extended to include algorithm-specific parameters.
     */
    struct Properties{};

    /*!
     * \brief Enum representing the type of geometric test.
     *
     * Defines different algorithms used for estimating geometric transformations.
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
    GeometricTest(Type type);

    /*!
     * \brief Virtual destructor for the GeometricTest class.
     */
    virtual ~GeometricTest() = default;

public:

    /*!
     * \brief Retrieves the properties of the geometric test.
     * \return A pointer to the properties.
     *
     * Derived classes should return a pointer to their specific `Properties` implementation.
     */
    virtual auto properties() const -> const Properties* = 0;

    /*!
     * \brief Sets the properties of the geometric test.
     * \param[in] properties A pointer to the properties to set.
     *
     * This method allows modifying test-specific parameters, such as RANSAC thresholds.
     */
    virtual void setProperties(const Properties *properties) = 0;

    /*!
     * \brief Executes the geometric test on two sets of corresponding points.
     *
     * This method applies the selected geometric transformation estimation method (e.g., homography, fundamental matrix)
     * and returns a vector indicating which points are inliers.
     *
     * \return A vector of unsigned char with the inliers (1 for inliers, 0 for outliers).
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
 * \brief Factory class for creating instances of geometric tests.
 *
 * The `GeometricTestFactory` provides a simple interface for instantiating different
 * types of geometric tests. This ensures modularity and ease of configuration.
 */
class TL_EXPORT GeometricTestFactory
{

private:

    GeometricTestFactory() = default;

public:

    /*!
     * \brief Creates a geometric test of the specified type.
     * 
     * This function initializes the appropriate geometric test algorithm based on the provided type.
     * 
     * \param[in] type The type of geometric test to create.
     * \return A shared pointer to the created geometric test.
     */
    static auto create(GeometricTest::Type type) -> std::shared_ptr<GeometricTest>;

    /*!
     * \brief Creates a geometric test of the specified type with given properties.
     * 
     * This function allows creating a geometric test with custom parameters.
     * 
     * \param[in] type The type of geometric test to create.
     * \param[in] properties The properties to configure the geometric test.
     * \return A shared pointer to the created geometric test.
     */
    static auto create(GeometricTest::Type type,
                       const GeometricTest::Properties *properties) -> std::shared_ptr<GeometricTest>;

};


/*! \} */


} // namespace tl
