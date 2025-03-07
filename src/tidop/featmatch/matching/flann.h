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

namespace tl
{

/*! \addtogroup FeatureMatching
 *
 *  \{
 */


/*!
 * \brief Properties class for configuring a FLANN-based matcher.
 *
 * This class holds the configuration parameters for a FLANN-based feature descriptor matcher.
 * The parameters include the type of FLANN index to use.
 *
 * #### Example Usage
 * \code{.cpp}
 * FlannMatcherProperties properties;
 * properties.setIndex("kdtree");
 * \endcode
 */
class TL_EXPORT FlannMatcherProperties
  : public MatchingMethod
{

public:

    /*!
     * \brief Default constructor. Initializes the properties with default values.
     */
    FlannMatcherProperties();

    /*!
     * \brief Copy constructor.
     * \param[in] properties Another instance to copy properties from.
     */
    FlannMatcherProperties(const FlannMatcherProperties &properties);

    /*!
     * \brief Move constructor.
     * \param[in] properties Another instance to move properties from.
     */
    FlannMatcherProperties(FlannMatcherProperties &&properties) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~FlannMatcherProperties() override;

    /*!
     * \brief Copy assignment operator.
     * \param[in] properties Another instance to copy properties from.
     * \return Reference to this instance.
     */
    auto operator=(const FlannMatcherProperties &properties) -> FlannMatcherProperties &;

    /*!
     * \brief Move assignment operator.
     * \param[in] properties Another instance to move properties from.
     * \return Reference to this instance.
     */
    auto operator=(FlannMatcherProperties &&properties) TL_NOEXCEPT -> FlannMatcherProperties &;
  
    /*!
     * \brief Get the index type used in FLANN-based matching.
     * \return The index type as a string.
     * 
     * \note Property name: `"Index"`.
     */
    auto index() const -> std::string;

    /*!
     * \brief Set the index type used in FLANN-based matching.
     * \param index Name of the FLANN index type ("kdtree", "lsh").
     */
    void setIndex(const std::string &index);

// MatchingMethod interface

public:

    void reset() override;

};




/*!
 * \brief Feature descriptor matcher using FLANN (Fast Library for Approximate Nearest Neighbors).
 *
 * This class provides an implementation of feature matching using the FLANN library.
 * It supports both one-to-one and K-nearest neighbors (KNN) matching.
 *
 * #### Example Usage
 * \code{.cpp}
 * FlannMatcherProperties properties;
 * properties.setIndex("kdtree");
 *
 * FlannMatcher matcher(properties);
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
class TL_EXPORT FlannMatcher
  : public DescriptorMatcher
{

private:

    cv::Ptr<cv::FlannBasedMatcher> mFlannBasedMatcher;
    FlannMatcherProperties mProperties;

public:

    /*!
     * \brief Default constructor. Initializes the FLANN matcher with default properties.
     */
    FlannMatcher();

    /*!
     * \brief Constructor with custom properties.
     * \param[in] properties Properties to configure the matcher.
     */
    explicit FlannMatcher(const FlannMatcherProperties &properties);

    /*!
     * \brief Copy constructor.
     * \param[in] flann Another instance to copy from.
     */
    FlannMatcher(const FlannMatcher &flann);

    /*!
     * \brief Move constructor.
     * \param[in] flann Another instance to move from.
     */
    FlannMatcher(FlannMatcher &&flann) TL_NOEXCEPT;

    /*!
     * \brief Destructor.
     */
    ~FlannMatcher() override = default;

    /*!
     * \brief Copy assignment operator.
     * \param[in] flann Another instance to copy from.
     * \return Reference to this instance.
     */
    auto operator=(const FlannMatcher &flann) -> FlannMatcher &;

    /*!
     * \brief Move assignment operator.
     * \param[in] flann Another instance to move from.
     * \return Reference to this instance.
     */
    auto operator=(FlannMatcher &&flann) TL_NOEXCEPT -> FlannMatcher &;

    /*!
     * \brief Get the properties of the matcher.
     * \return Reference to the properties object.
     *
     * The following property names are valid when accessed via the `Properties` class:
     * - `"Index"`
     */
    auto properties() -> FlannMatcherProperties &{ return mProperties; }

    /*!
     * \brief Get the properties of the matcher (const version).
     * \return Const reference to the properties object.
     * 
     * The following property names are valid when accessed via the `Properties` class:
     * - `"Index"`
     */
    auto properties() const -> const FlannMatcherProperties &{ return mProperties; }

private:

    void init();

// DescriptorMatcher interface

public:

    /*!
     * \brief Compute one-to-one descriptor matching using FLANN.
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
     * \brief Compute one-to-many descriptor matching (KNN matching) using FLANN.
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

/*! \} */

} // namespace tl
