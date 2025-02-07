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

#include <opencv2/features2d.hpp>

#include "tidop/core/base/flags.h"
#include "tidop/core/base/size.h"
#include "tidop/core/base/property.h"


namespace tl
{

/*! \addtogroup FeatureExtraction
 * 
 *  \{
 */

/*!
 * \brief Base class for feature detection and description algorithms.
 */
class TL_EXPORT Feature
  : public Properties
{

public:

    /*!
     * \brief Enum representing the type of feature algorithm.
     */
    enum class Type
    {
        agast,             /*!< AGAST corner detector. */
        akaze,             /*!< AKAZE feature detector and descriptor. */
        boost,             /*!< BOOST feature detection. */
        brief,             /*!< BRIEF descriptor. */
        brisk,             /*!< BRISK keypoint detection and description. */
        daisy,             /*!< DAISY descriptor. */
        fast,              /*!< FAST corner detector. */
        freak,             /*!< FREAK descriptor. */
        gftt,              /*!< Good Features to Track detector. */
        hog,               /*!< Histogram of Oriented Gradients. */
        kaze,              /*!< KAZE feature detector and descriptor. */
        latch,             /*!< LATCH descriptor. */
        lucid,             /*!< LUCID descriptor. */
        lss,               /*!< Line Segment Structure descriptor. */
        msd,               /*!< MSD detector. */
        mser,              /*!< MSER detector. */
        orb,               /*!< ORB detector and descriptor. */
        sift,              /*!< SIFT detector and descriptor. */
        star,              /*!< STAR detector. */
        surf,              /*!< SURF detector and descriptor. */
        vgg,               /*!< VGG descriptor. */
        user_defined = 100 /*!< User-defined feature type. */
    };

    ///TODO: GLOH -> https://www.robots.ox.ac.uk/~vgg/research/affine/det_eval_files/mikolajczyk_pami2004.pdf

public:

    /*!
     * \brief Constructor initializing the feature with a specific type.
     * \param[in] type The type of the feature.
     */
    Feature(std::string name, Type type) : Properties(std::move(name)), mFeatType(type) {}

    /*!
     * \brief Virtual destructor.
     */
    virtual ~Feature() = default;

    /*!
     * \brief Get the type of the feature.
     * \return The type of the feature.
     */
    auto type() const -> Type
    {
        return mFeatType.flags();
    }

    /*!
     * \brief Reset the feature to its default values.
     */
    virtual void reset() = 0;

private:

    tl::EnumFlags<Type> mFeatType;

};
ALLOW_BITWISE_FLAG_OPERATIONS(Feature::Type)






/*!
 * \brief Abstract base class for feature detectors.
 */
class TL_EXPORT FeatureDetector
{

public:

    /*!
     * \brief Default constructor.
     */
    FeatureDetector() = default;

    /*!
     * \brief Destructor.
     */
    virtual ~FeatureDetector() = default;

    /*!
     * \brief Detects keypoints in an image.
     * \param[in] img Input image.
     * \param[in] mask Optional mask to specify regions of interest.
     * \return Vector of detected keypoints.
     */
    virtual auto detect(const cv::Mat &img, cv::InputArray &mask = cv::noArray()) -> std::vector<cv::KeyPoint> = 0;

};


/*!
 * \brief Abstract base class for feature descriptors.
 */
class TL_EXPORT FeatureDescriptor
{

public:

    /*!
     * \brief Default constructor.
     */
    FeatureDescriptor() = default;

    /*!
     * \brief Destructor.
     */
    virtual ~FeatureDescriptor() = default;

    /*!
     * \brief Extracts descriptors from keypoints in an image.
     * \param[in] img Input image.
     * \param[in,out] keyPoints Input keypoints, may be refined.
     * \return Matrix of computed descriptors.
     */
    virtual auto extract(const cv::Mat &img, std::vector<cv::KeyPoint> &keyPoints) -> cv::Mat = 0;

};



/*! \} */

} // namespace tl
