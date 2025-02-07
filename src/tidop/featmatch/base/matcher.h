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


namespace tl
{

class GeometricTest;


/*! \addtogroup FeatureMatching
 * 
 *  \{
 */

class TL_EXPORT MatchingMethod
{

public:

    enum class Type
    {
        flann,
        brute_force
    };

public:

    MatchingMethod() {}
    virtual ~MatchingMethod() = default;

    /*!
     * \brief Recover the default values
     */
    virtual void reset() = 0;

    /*!
     * \brief Type of match method (flann or brute force)
     * \return
     */
    virtual auto type() const -> Type = 0;

    /*!
     * \brief Name of match method
     * \return
     */
    virtual auto name() const -> std::string = 0;

};
ALLOW_BITWISE_FLAG_OPERATIONS(MatchingMethod::Type)



/*----------------------------------------------------------------*/


class TL_EXPORT MatchingMethodBase
  : public MatchingMethod
{

private:

    tl::EnumFlags<Type> mMatchType;

public:

    explicit MatchingMethodBase(Type type) : mMatchType(type) {}
    ~MatchingMethodBase() override = default;

    // MatchingMethod interface

public:

    auto type() const -> Type override
    {
        return mMatchType.flags();
    }

};


/*----------------------------------------------------------------*/



class TL_EXPORT DescriptorMatcher
{

public:

    DescriptorMatcher() {}
    virtual ~DescriptorMatcher() = default;

public:

    /*!
     * \brief Compute matching
     * \param[in] queryDescriptors Query descriptors
     * \param[in] trainDescriptors Train descriptors
     * \param[out] matches Matches 1 to 2
     * \param[in] mask
     * \return
     */
    virtual void match(const cv::Mat &queryDescriptors,
                       const cv::Mat &trainDescriptors,
                       std::vector<cv::DMatch> &matches,
                       const cv::Mat mask = cv::Mat()) = 0;

    /*!
     * \brief Compute matching
     * \param[in] queryDescriptors Query descriptors
     * \param[in] trainDescriptors Train descriptors
     * \param[out] matches Matches 1 to 2 and 2 to 1
     * \param[in] mask
     * \return
     */
    virtual void match(const cv::Mat &queryDescriptors,
                       const cv::Mat &trainDescriptors,
                       std::vector<std::vector<cv::DMatch>> &matches,
                       const cv::Mat mask = cv::Mat()) = 0;

};


/*----------------------------------------------------------------*/


class TL_EXPORT FlannMatcher
    : public MatchingMethodBase
{

public:

    /// TODO: Por ahora valores por defecto
    enum class Index
    {
        kdtree,
        lsh
        ///TODO: https://docs.opencv.org/4.1.1/db/df4/structcv_1_1flann_1_1IndexParams.html
    };

public:

    FlannMatcher() : MatchingMethodBase(Type::flann) {}
    ~FlannMatcher() override = default;

    virtual auto index() const -> Index = 0;
    virtual void setIndex(Index index) = 0;

};



/*----------------------------------------------------------------*/



class TL_EXPORT BruteForceMatcher
  : public MatchingMethodBase
{

public:

    enum class Norm
    {
        l1,
        l2,
        hamming,
        hamming2
    };

public:

    BruteForceMatcher() : MatchingMethodBase(Type::brute_force) {}
    ~BruteForceMatcher() override = default;

    virtual auto normType() const -> Norm = 0;
    virtual void setNormType(Norm normType) = 0;

};



/*----------------------------------------------------------------*/


class TL_EXPORT MatchingStrategy
{

public:

    enum class Strategy
    {
        robust_matching,
        gms
    };

protected:

    tl::EnumFlags<Strategy> mStrategy;

public:

    MatchingStrategy(Strategy strategy) : mStrategy(strategy) {}
    virtual ~MatchingStrategy() = default;

    /*!
     * \brief Recover the default values
     */
    virtual void reset() = 0;

    auto strategy() const -> Strategy
    {
        return mStrategy.flags();
    }

    virtual auto name() const -> std::string = 0;

};
ALLOW_BITWISE_FLAG_OPERATIONS(MatchingStrategy::Strategy)


/*----------------------------------------------------------------*/


//class TL_EXPORT MatchingStrategyBase
//  : public MatchingStrategy
//{
//
//
//
//public:
//
//    MatchingStrategyBase(Strategy strategy) : mStrategy(strategy) {}
//    ~MatchingStrategyBase() override = default;
//
//// MatchingStrategy interface
//
//public:
//
//    auto strategy() const -> Strategy override
//    {
//        return mStrategy.flags();
//    }
//
//};


/*----------------------------------------------------------------*/


class TL_EXPORT MatchingAlgorithm
{

public:

    MatchingAlgorithm() {}
    virtual ~MatchingAlgorithm() = default;

    virtual bool compute(const cv::Mat &queryDescriptor,
                         const cv::Mat &trainDescriptor,
                         const std::vector<cv::KeyPoint> &keypoints1,
                         const std::vector<cv::KeyPoint> &keypoints2,
                         std::vector<cv::DMatch> *goodMatches,
                         std::vector<cv::DMatch> *wrongMatches = nullptr,
                         const cv::Size &queryImageSize = cv::Size(),
                         const cv::Size &trainImageSize = cv::Size()) = 0;
};


/*----------------------------------------------------------------*/



/*!
 * \brief The RobustMatcher class
 *
 * Robust Matching:
 *
 * http://docs.opencv.org/3.1.0/dc/d2c/tutorial_real_time_pose.html
 */
class TL_EXPORT RobustMatcher
  : public MatchingStrategy
{

public:

    RobustMatcher()
        : MatchingStrategy(Strategy::robust_matching)
    {
    }
    ~RobustMatcher() override = default;

    virtual auto ratio() const -> double = 0;
    virtual void setRatio(double ratio) = 0;

    virtual auto crossCheck() const -> bool = 0;
    virtual void setCrossCheck(bool crossCheck) = 0;

    virtual auto geometricTest() const -> std::shared_ptr<GeometricTest> = 0;
    virtual void setGeometricTest(std::shared_ptr<GeometricTest> geometricTest) = 0;

};



/*----------------------------------------------------------------*/


class TL_EXPORT Gms
  : public MatchingStrategy
{

public:

    Gms() : MatchingStrategy(Strategy::gms) {}
    ~Gms() override = default;

    virtual auto rotation() const -> bool = 0;
    virtual void setRotation(bool rotation) = 0;

    virtual auto scale() const -> bool = 0;
    virtual void setScale(bool scale) = 0;

    virtual auto threshold() const -> double = 0;
    virtual void setThreshold(double threshold) = 0;
};


/*! \} */

} // namespace tl
