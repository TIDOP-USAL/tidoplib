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

#include "tidop/featmatch/matching/matching.h"

#include <vector>

#ifdef HAVE_OPENCV_CUDAFEATURES2D
#include <opencv2/cudafeatures2d.hpp>
#endif // HAVE_OPENCV_CUDAFEATURES2D

namespace tl
{

/*! \addtogroup FeatureMatching
 *
 *  \{
 */

/*!
 * \brief Properties class for configuring a Brute-Force feature matcher.
 *
 * This class stores the configuration parameters for a Brute-Force descriptor matcher.
 * The primary parameter is the norm type, which determines how distances between descriptors are computed.
 *
 * #### Example Usage
 * \code{.cpp}
 * BruteForceMatcherProperties properties;
 * properties.setNormType("L2");
 * \endcode
 */
class TL_EXPORT BruteForceMatcherProperties
  : public MatchingMethod
{

public:

    /*!
     * \brief Default constructor. Initializes the properties with default values.
     */
    BruteForceMatcherProperties();

    /*!
     * \brief Copy constructor.
     * \param[in] properties Another instance to copy properties from.
     */
    BruteForceMatcherProperties(const BruteForceMatcherProperties &properties);
   
    /*!
     * \brief Move constructor.
     * \param[in] properties Another instance to move properties from.
     */
    BruteForceMatcherProperties(BruteForceMatcherProperties &&properties) TL_NOEXCEPT;
   
    /*!
     * \brief Destructor.
     */
    ~BruteForceMatcherProperties() override = default;

    /*!
     * \brief Copy assignment operator.
     * \param[in] properties Another instance to copy properties from.
     * \return Reference to this instance.
     */
    auto operator=(const BruteForceMatcherProperties &properties) -> BruteForceMatcherProperties &;
  
    /*!
     * \brief Move assignment operator.
     * \param[in] properties Another instance to move properties from.
     * \return Reference to this instance.
     */
    auto operator=(BruteForceMatcherProperties &&properties) TL_NOEXCEPT -> BruteForceMatcherProperties &;

    /*!
     * \brief Get the norm type used in Brute-Force matching.
     * \return The norm type as a string (e.g., "L2", "HAMMING").
     * 
     * \note Property name: `"NormType"`. 
     */
    auto normType() const -> std::string;

    /*!
     * \brief Set the norm type used in Brute-Force matching.
     * 
     * Valid values:
     * - "L1"
     * - "L2"
     * - "HAMMING"
     * - "HAMMING2"
     *
     * \param[in] normType Name of the norm type (e.g., "L2" for SIFT/SURF, "HAMMING" for ORB/BRIEF).
     */
    void setNormType(const std::string &normType);

// MatchingMethod interface

public:

    void reset() override;

};




/*!
 * \brief Feature descriptor matcher using Brute-Force method.
 *
 * This class provides an implementation of feature matching using OpenCV's Brute-Force matcher.
 * It supports both one-to-one and K-nearest neighbors (KNN) matching.
 *
 * \section brute_force_matcher_example Example Usage
 * \code{.cpp}
 * BruteForceMatcherProperties properties;
 * properties.setNormType("L2");
 *
 * BruteForceMatcher matcher(properties);
 *
 * tl::Path train_path("train.xml");
 * auto train_reader = FeaturesReaderFactory::create(train_path);
 * train_reader->read();
 * trainDescriptors = train_reader->descriptors();
 *
 * tl::Path query_path("query.xml");
 * auto query_reader = FeaturesReaderFactory::create(query_path);
 * query_reader->read();
 * queryDescriptors = query_reader->descriptors();
 * 
 * std::vector<cv::DMatch> matches;
 *
 * matcher.match(queryDescriptors, trainDescriptors, matches);
 * \endcode
 */
class TL_EXPORT BruteForceMatcher
  : public DescriptorMatcher
{

private:

    cv::Ptr<cv::BFMatcher> mBFMatcher;
    BruteForceMatcherProperties mProperties;

public:

    /*!
     * \brief Default constructor. Initializes the Brute-Force matcher with default properties.
     */
    BruteForceMatcher();

    /*!
     * \brief Constructor with custom properties.
     * \param properties Properties to configure the matcher.
     */
    explicit BruteForceMatcher(const BruteForceMatcherProperties &properties);
 
    /*!
     * \brief Copy constructor.
     * \param bf Another instance to copy from.
     */
    BruteForceMatcher(const BruteForceMatcher &bf);

    /*!
     * \brief Move constructor.
     * \param bf Another instance to move from.
     */
    BruteForceMatcher(BruteForceMatcher &&bf) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~BruteForceMatcher() override = default;

    /*!
     * \brief Copy assignment operator.
     * \param bf Another instance to copy from.
     * \return Reference to this instance.
     */
    auto operator=(const BruteForceMatcher &bf) -> BruteForceMatcher &;

    /*!
     * \brief Move assignment operator.
     * \param bf Another instance to move from.
     * \return Reference to this instance.
     */
    auto operator=(BruteForceMatcher &&bf) TL_NOEXCEPT -> BruteForceMatcher &;

    /*!
     * \brief Get the properties of the matcher.
     * \return Reference to the properties object.
     *
     * The following property names are valid when accessed via the `Properties` class:
     * - `"NormType"`
     */
    auto properties() -> BruteForceMatcherProperties &{ return mProperties; }
 
    /*!
     * \brief Get the properties of the matcher (const version).
     * \return Const reference to the properties object.
     *
     * The following property names are valid when accessed via the `Properties` class:
     * - `"NormType"`
     */
    auto properties() const -> const BruteForceMatcherProperties &{ return mProperties; }

private:

    void init();

// DescriptorMatcher interface

public:

    /*!
     * \brief Compute one-to-one descriptor matching using Brute-Force.
     *
     * Matches descriptors from the query set to the most similar descriptor in the train set.
     *
     * \param[in] queryDescriptors Set of descriptors from the query image.
     * \param[in] trainDescriptors Set of descriptors from the train image.
     * \param[out] matches Vector of best matches between query and train descriptors.
     * \param[in] mask Optional mask specifying valid descriptor pairs.
     */
    void match(const cv::Mat &queryDescriptors,
               const cv::Mat &trainDescriptors,
               std::vector<cv::DMatch> &matches,
               const cv::Mat mask = cv::Mat()) override;

    /*!
     * \brief Compute one-to-many descriptor matching (KNN matching) using Brute-Force.
     *
     * Each descriptor in the query set is matched to the `k` most similar descriptors in the train set.
     *
     * \param[in] queryDescriptors Set of descriptors from the query image.
     * \param[in] trainDescriptors Set of descriptors from the train image.
     * \param[out] matches Vector of best matches, each query descriptor may have multiple matches.
     * \param[in] k Number of nearest neighbors to find for each query descriptor.
     * \param[in] mask Optional mask specifying valid descriptor pairs.
     */
    void match(const cv::Mat &queryDescriptors,
               const cv::Mat &trainDescriptors,
               std::vector<std::vector<cv::DMatch>> &matches,
               int k = 2,
               const cv::Mat mask = cv::Mat()) override;

};




/// \cond
#ifdef HAVE_OPENCV_CUDAFEATURES2D
/// \endcond

/*!
 * \ingroup FeatureExtraction
 * \class BruteForceMatcherCuda
 * \brief CUDA-accelerated Brute-Force feature matcher.
 *
 * This class provides a GPU-accelerated implementation of feature matching using OpenCV's CUDA module.
 *
 * \section brute_force_cuda_example Example Usage
 * \code{.cpp}
 * BruteForceMatcherProperties properties;
 * properties.setNormType("Hamming");
 *
 * tl::Path train_path("train.xml");
 * auto train_reader = FeaturesReaderFactory::create(train_path);
 * train_reader->read();
 * trainDescriptors = train_reader->descriptors();
 *
 * tl::Path query_path("query.xml");
 * auto query_reader = FeaturesReaderFactory::create(query_path);
 * query_reader->read();
 * queryDescriptors = query_reader->descriptors();
 *
 * cv::Mat queryDescriptors, trainDescriptors;
 * std::vector<cv::DMatch> matches;
 *
 * matcher.match(queryDescriptors, trainDescriptors, matches);
 * \endcode
 */
class TL_EXPORT BruteForceMatcherCuda
  : public DescriptorMatcher
{

private:

    cv::Ptr<cv::cuda::DescriptorMatcher> mBFMatcher;
    BruteForceMatcherProperties mProperties;

public:

    /*!
     * \brief Default constructor. Initializes the CUDA Brute-Force matcher with default properties.
     */
    BruteForceMatcherCuda();

    /*!
     * \brief Constructor with custom properties.
     * \param properties Properties to configure the matcher.
     */
    explicit BruteForceMatcherCuda(const BruteForceMatcherProperties &properties);

    /*!
     * \brief Copy constructor.
     * \param bf Another instance to copy from.
     */
    BruteForceMatcherCuda(const BruteForceMatcherCuda &bf);

    /*!
     * \brief Move constructor.
     * \param bf Another instance to move from.
     */
    BruteForceMatcherCuda(BruteForceMatcherCuda &&bf) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~BruteForceMatcherCuda() override = default;

    /*!
     * \brief Copy assignment operator.
     * \param bf Another instance to copy from.
     * \return Reference to this instance.
     */
    auto operator=(const BruteForceMatcherCuda &bf) -> BruteForceMatcherCuda &;
 
    /*!
     * \brief Move assignment operator.
     * \param bf Another instance to move from.
     * \return Reference to this instance.
     */
    auto operator=(BruteForceMatcherCuda &&bf) TL_NOEXCEPT -> BruteForceMatcherCuda &;

    /*!
     * \brief Get the properties of the matcher.
     * \return Reference to the properties object.
     *
     * The following property names are valid when accessed via the `Properties` class:
     * - `"NormType"`
     */
    auto properties() -> BruteForceMatcherProperties &{ return mProperties; }
    
    /*!
     * \brief Get the properties of the matcher (const version).
     * \return Const reference to the properties object.
     *
     * The following property names are valid when accessed via the `Properties` class:
     * - `"NormType"`
     */
    auto properties() const -> const BruteForceMatcherProperties &{ return mProperties; }

private:

    void init();

// DescriptorMatcher interface

public:

    /*!
     * \brief Compute one-to-one descriptor matching using Brute-Force.
     *
     * Matches descriptors from the query set to the most similar descriptor in the train set.
     *
     * \param[in] queryDescriptors Set of descriptors from the query image.
     * \param[in] trainDescriptors Set of descriptors from the train image.
     * \param[out] matches Vector of best matches between query and train descriptors.
     * \param[in] mask Optional mask specifying valid descriptor pairs.
     */
    void match(const cv::Mat &queryDescriptors,
               const cv::Mat &trainDescriptors,
               std::vector<cv::DMatch> &matches,
               const cv::Mat mask = cv::Mat()) override;

    /*!
     * \brief Compute one-to-many descriptor matching (KNN matching) using Brute-Force.
     *
     * Each descriptor in the query set is matched to the `k` most similar descriptors in the train set.
     *
     * \param[in] queryDescriptors Set of descriptors from the query image.
     * \param[in] trainDescriptors Set of descriptors from the train image.
     * \param[out] matches Vector of best matches, each query descriptor may have multiple matches.
     * \param[in] k Number of nearest neighbors to find for each query descriptor.
     * \param[in] mask Optional mask specifying valid descriptor pairs.
     */
    void match(const cv::Mat &queryDescriptors,
               const cv::Mat &trainDescriptors,
               std::vector<std::vector<cv::DMatch>> &matches,
               int k = 2,
               const cv::Mat mask = cv::Mat()) override;

};

/// \cond
#endif // HAVE_OPENCV_CUDAFEATURES2D
/// \endcond

/*! \} */

} // namespace tl
