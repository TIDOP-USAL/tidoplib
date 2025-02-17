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

#include "tidop/config.h"

#include <memory>

#include <opencv2/features2d.hpp>
#ifdef HAVE_OPENCV_XFEATURES2D 
#include <opencv2/xfeatures2d.hpp>
#endif // HAVE_OPENCV_XFEATURES2D
#ifdef HAVE_OPENCV_CUDAFEATURES2D
#include <opencv2/cudafeatures2d.hpp>
#endif // HAVE_OPENCV_CUDAFEATURES2D

#include "tidop/core/base/flags.h"
#include "tidop/core/base/property.h"

namespace tl
{

class GeometricTest;


/*! \addtogroup FeatureMatching
 * 
 *  \{
 */

/*!
 * \brief Base class for feature matching methods.
 *
 * This class defines the structure for different feature matching methods,
 * such as FLANN-based and brute-force matching.
 * It provides an interface to set and retrieve the matching method type
 * and to reset the matching parameters to their default values.
 */
class TL_EXPORT MatchingMethod
  : public Properties
{

public:

    /*!
     * \enum Type
     * \brief Defines the available feature matching methods.
     */
    enum class Type
    {
        flann,       /*!< FLANN-based matching (Fast Library for Approximate Nearest Neighbors) */
        brute_force  /*!< Brute-force matching */
    };

private:

    tl::EnumFlags<Type> mMatchType;

public:

    /*!
     * \brief Constructor.
     * \param[in] name Name of the matching method.
     * \param[in] type Type of matching method (FLANN or brute-force).
     */
    MatchingMethod(std::string name, Type type) : Properties(std::move(name)), mMatchType(type) {}
    
    virtual ~MatchingMethod() = default;

    /*!
     * \brief Reset the matching parameters to their default values.
     * \note This function must be implemented in derived classes.
     */
    virtual void reset() = 0;

    /*!
     * \brief Get the type of the matching method.
     * \return The matching method type (FLANN or brute-force).
     */
    virtual auto type() const -> Type
    {
        return mMatchType.flags();
    }

};
ALLOW_BITWISE_FLAG_OPERATIONS(MatchingMethod::Type)



/*!
 * \brief Abstract class for feature descriptor matching.
 *
 * This class provides an interface for descriptor matching between a set of query descriptors
 * and a set of train descriptors.
 * It defines pure virtual functions for performing:
 * - One-to-one descriptor matching (finding the best match for each query descriptor).
 * - One-to-many descriptor matching (finding the `k` best matches for each query descriptor).
 */
class TL_EXPORT DescriptorMatcher
{

public:

    DescriptorMatcher() {}
    virtual ~DescriptorMatcher() = default;

public:

    /*!
     * \brief Compute one-to-one descriptor matching.
     *
     * Matches descriptors from the query set to the most similar descriptor in the train set.
     *
     * \param[in] queryDescriptors Set of descriptors from the query image.
     * \param[in] trainDescriptors Set of descriptors from the train image.
     * \param[out] matches Vector of best matches between query and train descriptors.
     * \param[in] mask Optional mask specifying valid descriptor pairs.
     */
    virtual void match(const cv::Mat &queryDescriptors,
                       const cv::Mat &trainDescriptors,
                       std::vector<cv::DMatch> &matches,
                       const cv::Mat mask = cv::Mat()) = 0;

    /*!
     * \brief Compute one-to-many descriptor matching (KNN matching).
     *
     * Each descriptor in the query set is matched to the `k` most similar descriptors in the train set.
     * The default value for `k` is 2, which is commonly used in ratio tests for filtering matches.
     *
     * \param[in] queryDescriptors Set of descriptors from the query image.
     * \param[in] trainDescriptors Set of descriptors from the train image.
     * \param[out] matches Vector of best matches, each query descriptor may have multiple matches.
     * \param[in] k Number of matches for each query descriptor.
     * \param[in] mask Optional mask specifying valid descriptor pairs.
     */
    virtual void match(const cv::Mat &queryDescriptors,
                       const cv::Mat &trainDescriptors,
                       std::vector<std::vector<cv::DMatch>> &matches,
                       int k = 2,
                       const cv::Mat mask = cv::Mat()) = 0;

};




/*!
 * \class MatchingStrategy
 * \brief Abstract base class for different matching strategies.
 *
 * A matching strategy defines how feature matching is performed, considering
 * techniques like robust matching and Grid-based Motion Statistics (GMS).
 */
class TL_EXPORT MatchingStrategy
  : public Properties
{

public:

    /*!
     * \enum Strategy
     * \brief Defines available feature matching strategies.
     */
    enum class Strategy
    {
        robust_matching, /*!< Robust matching with filtering mechanisms (e.g., RANSAC). */
        gms              /*!< Grid-based Motion Statistics (GMS) for adaptive matching. */
    };

protected:

    tl::EnumFlags<Strategy> mStrategy;

public:

    /*!
     * \brief Constructor.
     * \param[in] name Name of the strategy.
     * \param[in] strategy Type of matching strategy.
     */
    MatchingStrategy(std::string name, Strategy strategy) : Properties(std::move(name)), mStrategy(strategy) {}

    virtual ~MatchingStrategy() = default;

    /*!
     * \brief Reset the matching strategy parameters to their default values.
     */
    virtual void reset() = 0;

    /*!
     * \brief Get the selected matching strategy.
     * \return The currently selected matching strategy.
     */
    auto strategy() const -> Strategy
    {
        return mStrategy.flags();
    }

};
ALLOW_BITWISE_FLAG_OPERATIONS(MatchingStrategy::Strategy)



/*!
 * \brief Abstract class for computing feature matches with additional filtering.
 *
 * This class defines a general interface for computing feature matches with
 * optional outlier rejection. Implementations may use techniques like ratio tests
 * or spatial constraints to refine the matches.
 */
class TL_EXPORT MatchingAlgorithm
{

public:

    MatchingAlgorithm() {}
    virtual ~MatchingAlgorithm() = default;

    /*!
     * \brief Compute matches between query and train descriptors.
     *
     * This method finds correspondences between keypoints from two images and
     * optionally filters incorrect matches.
     *
     * \param[in] queryDescriptor Set of descriptors from the query image.
     * \param[in] trainDescriptor Set of descriptors from the train image.
     * \param[in] keypoints1 Keypoints corresponding to the query descriptors.
     * \param[in] keypoints2 Keypoints corresponding to the train descriptors.
     * \param[out] wrongMatches Optional output vector storing incorrect matches.
     * \param[in] queryImageSize Optional size of the query image.
     * \param[in] trainImageSize Optional size of the train image.
     * \return A vector of DMatch objects representing the valid matches.
     */
    virtual auto compute(const cv::Mat &queryDescriptor,
                         const cv::Mat &trainDescriptor,
                         const std::vector<cv::KeyPoint> &keypoints1,
                         const std::vector<cv::KeyPoint> &keypoints2,
                         std::vector<cv::DMatch> *wrongMatches = nullptr,
                         const cv::Size &queryImageSize = cv::Size(),
                         const cv::Size &trainImageSize = cv::Size()) -> std::vector<cv::DMatch> = 0;
};



/*! \} */

} // namespace tl
