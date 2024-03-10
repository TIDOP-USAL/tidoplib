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

#include "tidop/core/flags.h"
#include "tidop/geometry/size.h"

namespace tl
{

/*! \defgroup Features Features
 * 
 *  \{
 */


/*! \defgroup FeatureDetectorAndDescriptor Feature detection and extraction
 * 
 *  \{
 */


class TL_EXPORT Feature
{

public:

    enum class Type
    {
        agast,
        akaze,
        boost,
        brief,
        brisk,
        daisy,
        fast,
        freak,
        gftt,
        hog,
        kaze,
        latch,
        lucid,
        lss,
        msd,
        mser,
        orb,
        sift,
        star,
        surf,
        vgg,
        user_defined = 100
    };

    ///TODO: GLOH -> https://www.robots.ox.ac.uk/~vgg/research/affine/det_eval_files/mikolajczyk_pami2004.pdf

public:

    Feature() = default;
    virtual ~Feature() = default;

    /*!
     * \brief Recover the default values
     */
    virtual void reset() = 0;

    virtual auto type() const -> Type = 0;
    virtual auto name() const -> std::string = 0;

};
ALLOW_BITWISE_FLAG_OPERATIONS(Feature::Type)



/*----------------------------------------------------------------*/



class TL_EXPORT FeatureBase
    : public Feature
{

public:

    FeatureBase(Type type) : mFeatType(type) {}
    ~FeatureBase() override = default;

    auto type() const -> Type override
    {
        return mFeatType.flags();
    }

private:

    tl::EnumFlags<Type> mFeatType;

};



/*----------------------------------------------------------------*/


/*!
 * \brief Keypoint Detector class
 */
class TL_EXPORT KeypointDetector
{

public:

    KeypointDetector() = default;
    virtual ~KeypointDetector() = default;

    /*!
     * \brief Detects keypoints in an image
     * \param[in] img Image
     * \param[out] keyPoints The detected keypoints
     * \param[in] mask Optional mask
     * \return key points detected
     */
    virtual auto detect(const cv::Mat &img, cv::InputArray &mask = cv::noArray()) -> std::vector<cv::KeyPoint> = 0;

};



/*----------------------------------------------------------------*/


/*!
 * \brief Descriptor Extractor class
 */
class TL_EXPORT DescriptorExtractor
{

public:

    DescriptorExtractor() = default;
    virtual ~DescriptorExtractor() = default;

    /*!
     * \brief Extract descriptors
     * \param[in] img Image
     * \param[in] keyPoints KeyPoints
     * \param[out] descriptors Computed descriptors
     * \return Computed descriptors
     */
    virtual auto extract(const cv::Mat &img, std::vector<cv::KeyPoint> &keyPoints) -> cv::Mat = 0;

};


/*----------------------------------------------------------------*/


/*!
 * \brief Agast Interface
 * AGAST: Adaptive and Generic Corner Detection Based on the Accelerated Segment Test
 *
 * Mair E., Hager G.D., Burschka D., Suppa M., Hirzinger G. (2010) Adaptive and Generic
 * Corner Detection Based on the Accelerated Segment Test. In: Daniilidis K., Maragos P.,
 * Paragios N. (eds) Computer Vision – ECCV 2010. ECCV 2010.
 * Lecture Notes in Computer Science, vol 6312. Springer, Berlin, Heidelberg
 * https://mediatum.ub.tum.de/doc/1287456/1287456.pdf
 */
class TL_EXPORT Agast
    : public FeatureBase
{

public:

    Agast() : FeatureBase(Type::agast) {}
    ~Agast() override = default;

    /*!
     * \brief threshold
     * The AST applies a minimum difference threshold when comparing
     * the value of a pixel on the pattern with the brightness of
     * the nucleus. This parameter controls the sensitivity of the
     * corner response.
     * A large threshold value results in few but therefore only
     * strong corners, while a small threshold value yields also
     * corners with smoother gradients.
     * \return
     */
    virtual auto threshold() const -> int = 0;

    /*!
     * \brief Non Maximal Suppression for removing adjacent corners
     * \return
     */
    virtual auto nonmaxSuppression() const -> bool = 0;

    /*!
     * \brief Detector Types
     * - AGAST_5_8: AGAST-5 decision tree whith the 8 pixels mask
     * - AGAST_7_12d: AGAST-7 decision tree whith the Diamond shaped 12 pixels mask
     * - AGAST_7_12s: AGAST-7 decision tree whith the Squared shaped 12 pixels mask
     * - OAST_9_16: OAST-9 (Optimal AST) decision tree whith the 16 pixels mask
     * \return Detector Type
     */
    virtual auto detectorType() const -> std::string = 0;

    /*!
     * \brief Set Threshold
     * \param[in] threshold Default=10
     */
    virtual void setThreshold(int threshold) = 0;

    /*!
     * \brief Set Non Maximal Suppression
     * \param[in] nonmaxSuppression Non Maximal Suppression for removing adjacent corners (Default=true)
     */
    virtual void setNonmaxSuppression(bool nonmaxSuppression) = 0;

    /*!
     * \brief Set the Detector Type
     * Suported types:
     * - AGAST_5_8: AGAST-5 decision tree whith the 8 pixels mask
     * - AGAST_7_12d: AGAST-7 decision tree whith the Diamond shaped 12 pixels mask
     * - AGAST_7_12s: AGAST-7 decision tree whith the Squared shaped 12 pixels mask
     * - OAST_9_16: OAST-9 (Optimal AST) decision tree whith the 16 pixels mask
     *
     * \param[in] detectorType Detector Type (Default = OAST_9_16)
     */
    virtual void setDetectorType(const std::string &detectorType) = 0;

};


/*----------------------------------------------------------------*/


class TL_EXPORT Akaze
    : public FeatureBase
{

public:

    Akaze() : FeatureBase(Feature::Type::akaze) {}
    ~Akaze() override = default;

    /*!
     * \brief Type of the extracted descriptor
     * \return KAZE, KAZE_UPRIGHT, MLDB or MLDB_UPRIGHT
     */
    virtual auto descriptorType() const -> std::string = 0;

    /*!
     * \brief Size of the descriptor in bits. 0 -\> Full size
     * \return Descriptor size
     */
    virtual auto descriptorSize() const -> int = 0;

    /*!
     * \brief Number of channels in the descriptor [1, 2, 3 (default)]
     * \return Number of channels
     */
    virtual auto descriptorChannels() const -> int = 0;

    /*!
     * \brief Detector response threshold to accept point
     * \return threshold
     */
    virtual auto threshold() const -> double = 0;

    /*!
     * \brief Maximum octave evolution of the image
     * \return
     */
    virtual auto octaves() const -> int = 0;

    /*!
     * \brief Default number of sublevels per scale level
     * \return
     */
    virtual auto octaveLayers() const -> int = 0;

    /*!
     * \brief Diffusivity type
     * \return DIFF_PM_G1, DIFF_PM_G2 (default), DIFF_WEICKERT or DIFF_CHARBONNIER
     */
    virtual auto diffusivity() const -> std::string = 0;

    /*!
     * \brief Set the type of the extracted descriptor
     * \param[in] descriptorType Descriptor type: KAZE, KAZE_UPRIGHT, MLDB or MLDB_UPRIGHT
     */
    virtual void setDescriptorType(const std::string &descriptorType) = 0;

    /*!
     * \brief Set the size of the descriptor in bits.
     * \param[in] descriptorSize
     */
    virtual void setDescriptorSize(int descriptorSize) = 0;

    /*!
     * \brief Set the number of channels in the descriptor
     * \param[in] channels 1, 2 or 3 (default)
     */
    virtual void setDescriptorChannels(int channels) = 0;

    /*!
     * \brief Set the detector response threshold to accept point
     * \param[in] threshold
     */
    virtual void setThreshold(double threshold) = 0;

    /*!
     * \brief Set the maximum octave evolution of the image
     * \param[in] octaves
     */
    virtual void setOctaves(int octaves) = 0;

    /*!
     * \brief Sets the default number of sublevels per scale level
     * \param[in] octaveLayers
     */
    virtual void setOctaveLayers(int octaveLayers) = 0;

    /*!
     * \brief Set diffusivity type
     * \param[in] diffusivity DIFF_PM_G1, DIFF_PM_G2 (default), DIFF_WEICKERT or DIFF_CHARBONNIER
     */
    virtual void setDiffusivity(const std::string &diffusivity) = 0;

};


/*----------------------------------------------------------------*/


/*!
 * \brief The Boost class
 */
class TL_EXPORT Boost
    : public FeatureBase
{

public:

    Boost() : FeatureBase(Feature::Type::boost) {}
    ~Boost() override = default;

    /*!
     * \brief Type of descriptor used
     * Available types are BINBOOST_256 (default), BGM, BGM_HARD,
     * BGM_BILINEAR, LBGM, BINBOOST_64, BINBOOST_128, BINBOOST_256
     * \return Descriptor type
     */
    virtual auto descriptorType() const -> std::string = 0;

    /*!
     * \brief Sample patterns using keypoints orientation
     * \return true if use keypoints orientation
     */
    virtual auto useOrientation() const -> bool = 0;

    /*!
     * \brief Scale factor for adjust the sampling window of detected keypoints
     * \return Scale factor
     */
    virtual auto scaleFactor() const -> double = 0;

    /*!
     * \brief Set the type of descriptor to use
     * Available types are BINBOOST_256 (default), BGM, BGM_HARD,
     * BGM_BILINEAR, LBGM, BINBOOST_64, BINBOOST_128, BINBOOST_256
     * \param[in] descriptorType Type of descriptor to use.
     */
    virtual void setDescriptorType(const std::string &descriptorType) = 0;

    /*!
     * \brief Sample patterns using keypoints orientation
     * \param[in] useOrientation true for use keypoints orientation
     */
    virtual void setUseOrientation(bool useOrientation) = 0;

    /*!
     * \brief Adjust the sampling window of detected keypoints
     * \param[in] scaleFactor Scale factor
     */
    virtual void setScaleFactor(double scaleFactor) = 0;
};



/*----------------------------------------------------------------*/



/*!
 * \brief Interface for BRIEF
 *
 * Michael Calonder, Vincent Lepetit, Christoph Strecha, and Pascal Fua.
 * Brief: Binary robust independent elementary features. In Computer
 * Vision–ECCV 2010, pages 778–792. Springer, 2010
 * https://www.cs.ubc.ca/~lowe/525/papers/calonder_eccv10.pdf
 */
class TL_EXPORT Brief
    : public FeatureBase
{

public:

    Brief() : FeatureBase(Feature::Type::brief) {}
    ~Brief() override = default;

    /*!
     * \brief Legth of the descriptor in bytes
     * Valid values are: 16, 32 (default) or 64
     * \return Legth of the descriptor
     */
    virtual auto bytes() const -> std::string = 0;

    /*!
     * \brief useOrientation
     * \return
     */
    virtual auto useOrientation() const -> bool = 0;

    /*!
     * \brief Set the legth of the descriptor in bytes
     * \param[in] bytes
     */
    virtual void setBytes(const std::string &bytes) = 0;

    /*!
     * \brief setUseOrientation
     * \param[in] useOrientation
     */
    virtual void setUseOrientation(bool useOrientation) = 0;

};


/*----------------------------------------------------------------*/


/*!
 * \brief Interface for BRISK class
 * Stefan Leutenegger, Margarita Chli, and Roland Yves Siegwart.
 * Brisk: Binary robust invariant scalable keypoints. In Computer
 * Vision (ICCV), 2011 IEEE International Conference on, pages
 * 2548–2555. IEEE, 2011
 * http://margaritachli.com/papers/ICCV2011paper.pdf
 */
class TL_EXPORT Brisk
    : public FeatureBase
{

public:

    Brisk() : FeatureBase(Feature::Type::brisk) {}
    ~Brisk() override = default;

    /*!
     * \brief AGAST detection threshold score (Default=30)
     * \return Threshold
     */
    virtual auto threshold() const -> int = 0;

    /*!
     * \brief Detection octaves (Default=3)
     * \return
     */
    virtual auto octaves() const -> int = 0;

    /*!
     * \brief Pattern Scale (Default=1.0)
     * \return
     */
    virtual auto patternScale() const -> double = 0;

    /*!
     * \brief Set the AGAST detection threshold score
     * \param[in] threshold AGAST detection threshold
     */
    virtual void setThreshold(int threshold) = 0;

    /*!
     * \brief Set detection octaves
     * \param[in] octaves Octaves
     */
    virtual void setOctaves(int octaves) = 0;

    /*!
     * \brief Set Pattern Scale
     * \param[in] patternScale Pattern Scale
     */
    virtual void setPatternScale(double patternScale) = 0;

};


/*----------------------------------------------------------------*/


/*!
 * \brief Interface for DAISY class
 * E. Tola, V. Lepetit, and P. Fua.
 * DAISY: An Efficient Dense Descriptor Applied to Wide Baseline
 * Stereo. IEEE Transactions on Pattern Analysis and Machine
 * Intelligence, 32(5):815–830, May 2010.
 */
class TL_EXPORT Daisy
  : public FeatureBase
{

public:

    Daisy() : FeatureBase(Feature::Type::daisy) {}
    ~Daisy() override = default;
    
    /*!
     * \brief Radius of the descriptor at the initial scale (Default=15.)
     * \return
     */
    virtual auto radius() const -> double = 0;
    
    /*!
     * \brief Amount of radial range division quantity (Default=3)
     * \return
     */
    virtual auto qRadius() const -> int = 0;
    
    /*!
     * \brief Amount of angular range division quantity (Default=8)
     * \return
     */
    virtual auto qTheta() const -> int = 0;
    
    /*!
     * \brief Amount of gradient orientations range division quantity (Default=8)
     * \return
     */
    virtual auto qHist() const -> int = 0;
    
    /*!
     * \brief Descriptor normalization type
     * Normalization types:
     * - NRM_NONE: not make any normalization (Default)
     * - NRM_PARTIAL: mean that histograms are normalized independently for L2 norm equal to 1.0
     * - NRM_FULL: mean that descriptors are normalized for L2 norm equal to 1.0
     * - NRM_SIFT: mean that descriptors are normalized for L2 norm equal to 1.0 but no individual one is bigger than 0.154 as in SIFT
     * \return
     */
    virtual auto norm() const -> std::string = 0;
    
    //virtual homography() const = 0;
    
    /*!
     * \brief Interpolation disabled or enabled (Default)
     * \return
     */
    virtual auto interpolation() const -> bool = 0;
    
    /*!
     * \brief Using orientation (Default=false)
     * \return
     */
    virtual auto useOrientation() const -> bool = 0;
    
    /*!
     * \brief Set the radius of the descriptor at the initial scale
     * \param[in] radius Radius of the descriptor
     */
    virtual void setRadius(double radius) = 0;
    
    /*!
     * \brief Set radial range division
     * \param[in] qRadius Radial range division
     */
    virtual void setQRadius(int qRadius) = 0;
    
    /*!
     * \brief Set angular range division
     * \param[in] qTheta Angular range division
     */
    virtual void setQTheta(int qTheta) = 0;
    
    /*!
     * \brief Set gradient orientations range division
     * \param[in] qHist gradient orientations range division
     */
    virtual void setQHist(int qHist) = 0;
    
    /*!
     * \brief Set normalization type
     * Normalization types:
     * - NRM_NONE: not make any normalization (Default)
     * - NRM_PARTIAL: mean that histograms are normalized independently for L2 norm equal to 1.0
     * - NRM_FULL: mean that descriptors are normalized for L2 norm equal to 1.0
     * - NRM_SIFT: mean that descriptors are normalized for L2 norm equal to 1.0 but no individual one is bigger than 0.154 as in SIFT
     * \param[in] norm Normalization type
     */
    virtual void setNorm(const std::string &norm) = 0;
    
    /*!
     * \brief Enable or disable interpolation
     * \param[in] interpolation true for enable interpolation or false for disable
     */
    virtual void setInterpolation(bool interpolation) = 0;
    
    /*!
     * \brief Enable or disable orientation use
     * \param[in] useOrientation true for enable orientation or false for disable
     */
    virtual void setUseOrientation(bool useOrientation) = 0;

};


/*----------------------------------------------------------------*/


/*!
 * \brief Interface for FAST class
 * Rosten E., Drummond T. (2006) Machine Learning for High-Speed Corner Detection.
 * In: Leonardis A., Bischof H., Pinz A. (eds) Computer Vision – ECCV 2006. ECCV 2006.
 * Lecture Notes in Computer Science, vol 3951. Springer, Berlin, Heidelberg
 */
class TL_EXPORT Fast
    : public FeatureBase
{

public:

    Fast() : FeatureBase(Feature::Type::fast) {}
    ~Fast() override = default;

    /*!
     * \brief threshold (Default=10)
     * \return
     */
    virtual auto threshold() const -> int = 0;

    /*!
     * \brief Non Maximal Suppression for removing adjacent corners (default=true)
     * \return
     */
    virtual auto nonmaxSuppression() const -> bool = 0;

    /*!
     * \brief Detector Types
     * - TYPE_5_8: FAST-5 decision tree whith the 8 pixels mask
     * - TYPE_7_12: FAST-7 decision tree whith the 12 pixels mask
     * - TYPE_9_16: FAST-9 decision tree whith the 16 pixels mask (default)
     * \return Detector Type
     */
    virtual auto detectorType() const -> std::string = 0;

    /*!
     * \brief Set the threshold
     * \param threshold Threshold
     */
    virtual void setThreshold(int threshold) = 0;

    /*!
     * \brief Set Non Maximal Suppression
     * \param[in] nonmaxSuppression Non Maximal Suppression for removing adjacent corners (Default=true)
     */
    virtual void setNonmaxSuppression(bool nonmaxSuppression) = 0;

    /*!
     * \brief Set the Detector Type
     * Suported types:
     * - TYPE_5_8: FAST-5 decision tree whith the 8 pixels mask
     * - TYPE_7_12: FAST-7 decision tree whith the 12 pixels mask
     * - TYPE_9_16: FAST-9 decision tree whith the 16 pixels mask (default)
     *
     * \param[in] detectorType Detector Type
     */
    virtual void setDetectorType(const std::string &detectorType) = 0;

};


/*----------------------------------------------------------------*/


/*!
 * \brief Interface for FREAK class
 * Alexandre Alahi, Raphael Ortiz, and Pierre Vandergheynst.
 * Freak: Fast retina keypoint. In Computer Vision and Pattern
 * Recognition (CVPR), 2012 IEEE Conference on, pages 510–517. Ieee, 2012.
 */
class TL_EXPORT Freak
    : public FeatureBase
{

public:

    Freak() : FeatureBase(Feature::Type::freak) {}
    ~Freak() override = default;

    /*!
     * \brief Orientation normalization (Default=true)
     * \return
     */
    virtual auto orientationNormalized() const -> bool = 0;

    /*!
     * \brief Scale normalization (Default=true)
     * \return
     */
    virtual auto scaleNormalized() const -> bool = 0;

    /*!
     * \brief Scaling of the description pattern (Default=22.)
     * \return
     */
    virtual auto patternScale() const -> double = 0;

    /*!
     * \brief Number of octaves covered by the detected keypoints (Default=4)
     * \return Number of octaves
     */
    virtual auto octaves() const -> int = 0;

    /*!
     * \brief Enable/disable orientation normalization
     * \param[in] orientationNormalized true for Enable
     */
    virtual void setOrientationNormalized(bool orientationNormalized) = 0;

    /*!
     * \brief Enable/disable scale normalization
     * \param[in] scaleNormalized true for Enable
     */
    virtual void setScaleNormalized(bool scaleNormalized) = 0;

    /*!
     * \brief Set scaling of the description pattern
     * \param[in] patternScale Pattern Scale
     */
    virtual void setPatternScale(double patternScale) = 0;

    /*!
     * \brief Number of octaves covered by the detected keypoints
     * \param[in] octaves Number of octaves covered by the detected keypoints
     */
    virtual void setOctaves(int octaves) = 0;

};


/*----------------------------------------------------------------*/


class TL_EXPORT Gftt
    : public FeatureBase
{

public:

    Gftt() : FeatureBase(Feature::Type::gftt) {}
    ~Gftt() override = default;

    virtual auto maxFeatures() const -> int = 0;
    virtual auto qualityLevel() const -> double = 0;
    virtual auto minDistance() const -> double = 0;
    virtual auto blockSize() const -> int = 0;
    virtual auto harrisDetector() const -> bool = 0;
    virtual auto k() const -> double = 0;

    virtual void setMaxFeatures(int maxFeatures) = 0;
    virtual void setQualityLevel(double qlevel) = 0;
    virtual void setMinDistance(double minDistance) = 0;
    virtual void setBlockSize(int blockSize) = 0;
    virtual void setHarrisDetector(bool value) = 0;
    virtual void setK(double k) = 0;

};


/*----------------------------------------------------------------*/


/*!
 * \brief HOG (Histogram of Oriented Gradients)
 * Navneet Dalal and Bill Triggs \cite Dalal2005
 */
class TL_EXPORT Hog
    : public FeatureBase
{

public:

    Hog() : FeatureBase(Feature::Type::hog) {}
    ~Hog() override = default;

    virtual auto winSize() const -> Size<int> = 0;
    virtual auto blockSize() const -> Size<int> = 0;
    virtual auto blockStride() const -> Size<int> = 0;
    virtual auto cellSize() const -> Size<int> = 0;
    virtual auto nbins() const -> int = 0;
    virtual auto derivAperture() const -> int = 0;
    //  virtual double winSigma() const = 0;
    //  virtual std::string histogramNormType() const = 0;
    //  virtual double l2HysThreshold() const = 0;
    //  virtual bool gammaCorrection() const = 0;
    //  virtual double freeCoef() const = 0;
    //  virtual int nlevels() const = 0;
    //  virtual bool signedGradient() const = 0;

    virtual void setWinSize(const Size<int> &winSize) = 0;
    virtual void setBlockSize(const Size<int> &blockSize) = 0;
    virtual void setBlockStride(const Size<int> &blockStride) = 0;
    virtual void setCellSize(const Size<int> &cellSize) = 0;
    virtual void setNbins(int nbins) = 0;
    virtual void setDerivAperture(int derivAperture) = 0;
    //  virtual void setWinSigma(double winSigma) = 0;
    //  virtual void setHistogramNormType(const std::string &histogramNormType) = 0;
    //  virtual void setL2HysThreshold(double l2HysThreshold) = 0;
    //  virtual void setGammaCorrection(bool gammaCorrection) = 0;
    //  virtual void setFreeCoef(double freeCoef) = 0;
    //  virtual void setNlevels(int nlevels) = 0;
    //  virtual void setSignedGradient(bool signedGradient) = 0;

};


/*----------------------------------------------------------------*/


/*!
 * \brief Kaze Interface
 *
 * Multiscale 2D feature detection and description algorithm in nonlinear scale spaces.
 *
 * Alcantarilla P.F., Bartoli A., Davison A.J. (2012) KAZE Features.
 * In: Fitzgibbon A., Lazebnik S., Perona P., Sato Y., Schmid C. (eds)
 * Computer Vision – ECCV 2012. ECCV 2012. Lecture Notes in Computer
 * Science, vol 7577. Springer, Berlin, Heidelberg
 *
 * https://www.doc.ic.ac.uk/~ajd/Publications/alcantarilla_etal_eccv2012.pdf
 */
class TL_EXPORT Kaze
    : public FeatureBase
{

public:

    Kaze() : FeatureBase(Feature::Type::kaze) {}
    ~Kaze() override = default;

    /*!
     * \brief Extended descriptor
     * \return true if use extended 128-element descriptors or false if use 64-element descriptors (Default)
     */
    virtual auto extendedDescriptor() const -> bool = 0;

    /*!
     * \brief Use of upright descriptors (non rotation-invariant)
     * \return true if use upright descriptors (Default=false)
     */
    virtual auto uprightDescriptor() const -> bool = 0;

    /*!
     * \brief Detector response threshold to accept point
     * \return threshold
     */
    virtual auto threshold() const -> double = 0;

    /*!
     * \brief Maximum octave evolution of the image
     * \return octaves
     */
    virtual auto octaves() const -> int = 0;

    /*!
     * \brief Default number of sublevels per scale level
     * \return
     */
    virtual auto octaveLayers() const -> int = 0;

    /*!
     * \brief Diffusivity type
     * \return DIFF_PM_G1, DIFF_PM_G2 (default), DIFF_WEICKERT or DIFF_CHARBONNIER
     */
    virtual auto diffusivity() const -> std::string = 0;

    /*!
     * \brief Set extended descriptor
     * \param[in] extended true for use extended 128-element descriptors or false for use 64-element descriptors (default)
     */
    virtual void setExtendedDescriptor(bool extended) = 0;

    /*!
     * \brief Set to enable use of upright descriptors (non rotation-invariant)
     * \param[in] upright
     */
    virtual void setUprightDescriptor(bool uprightDescriptor) = 0;

    /*!
     * \brief Set the detector response threshold to accept point
     * \param[in] threshold Threshold
     */
    virtual void setThreshold(double threshold) = 0;

    /*!
     * \brief Set the maximum octave evolution of the image
     * \param[in] octaves
     */
    virtual void setOctaves(int octaves) = 0;

    /*!
     * \brief Sets the default number of sublevels per scale level
     * \param[in] octaveLayers
     */
    virtual void setOctaveLayers(int octaveLayers) = 0;

    /*!
     * \brief Set diffusivity type
     * \param[in] diffusivity DIFF_PM_G1, DIFF_PM_G2 (default), DIFF_WEICKERT or DIFF_CHARBONNIER
     */
    virtual void setDiffusivity(const std::string &diffusivity) = 0;

};


/*----------------------------------------------------------------*/


class TL_EXPORT Latch
    : public FeatureBase
{

public:

    Latch() : FeatureBase(Feature::Type::latch) {}
    ~Latch() override = default;

    virtual auto bytes() const -> std::string = 0;
    virtual auto rotationInvariance() const -> bool = 0;
    virtual auto halfSsdSize() const -> int = 0;

    virtual void setBytes(const std::string &bytes) = 0;
    virtual void setRotationInvariance(bool rotationInvariance) = 0;
    virtual void setHalfSsdSize(int halfSsdSize) = 0;

};


/*----------------------------------------------------------------*/


/*!
 * \brief LUCID Interface
 * Eric Christiansen David Kriegman Ziegler, Andrew and Serge J. Belongie.
 * Locally uniform comparison image descriptor
 */
class TL_EXPORT Lucid
    : public FeatureBase
{
public:

    Lucid() : FeatureBase(Feature::Type::lucid) {}
    ~Lucid() override = default;

    /*!
     * \brief kernel for descriptor construction
     * 1=3x3, 2=5x5, 3=7x7 and so forth. Default=1
     * \return
     */
    virtual auto lucidKernel() const -> int = 0;

    /*!
     * \brief kernel for blurring image prior to descriptor construction
     * 1=3x3, 2=5x5, 3=7x7 and so forth. Default=2
     * \return
     */
    virtual auto blurKernel() const -> int = 0;

    /*!
     * \brief Set the kernel for descriptor construction
     * \param[in] lucidKernel kernel for descriptor construction
     */
    virtual void setLucidKernel(int lucidKernel) = 0;

    /*!
     * \brief Set the kernel for descriptor construction
     * \param[in] blurKernel kernel for descriptor construction
     */
    virtual void setBlurKernel(int blurKernel) = 0;

};


/*----------------------------------------------------------------*/


class TL_EXPORT Lss
    : public FeatureBase
{

public:

    Lss() : FeatureBase(Feature::Type::lss) {}
    ~Lss() override = default;

};


/*----------------------------------------------------------------*/

class TL_EXPORT Msd
    : public FeatureBase
{

public:

    Msd() : FeatureBase(Feature::Type::msd) {}
    ~Msd() override = default;

    virtual auto thresholdSaliency() const -> double = 0;
    virtual auto patchRadius() const -> int = 0;
    virtual auto knn() const -> int = 0;
    virtual auto searchAreaRadius() const -> int= 0;
    virtual auto scaleFactor() const -> double = 0;
    virtual auto NMSRadius() const -> int = 0;
    virtual auto nScales() const -> int = 0;
    virtual auto NMSScaleRadius() const -> int = 0;
    virtual auto computeOrientation() const -> bool = 0;
    virtual auto affineMSD() const -> bool = 0;
    //virtual int tilts() const = 0;
    virtual auto affineTilts() const -> int = 0;

    virtual void setThresholdSaliency(double thresholdSaliency) = 0;
    virtual void setPatchRadius(int patchRadius) = 0;
    virtual void setKNN(int knn) = 0;
    virtual void setSearchAreaRadius(int searchAreaRadius) = 0;
    virtual void setScaleFactor(double scaleFactor) = 0;
    virtual void setNMSRadius(int NMSRadius) = 0;
    virtual void setNScales(int nScales) = 0;
    virtual void setNMSScaleRadius(int NMSScaleR) = 0;
    virtual void setComputeOrientation(bool computeOrientation) = 0;
    virtual void setAffineMSD(bool affineMSD) = 0;
    virtual void setAffineTilts(int affineTilts) = 0;
};


/*----------------------------------------------------------------*/


class TL_EXPORT Mser
    : public FeatureBase
{

public:

    Mser() : FeatureBase(Feature::Type::mser) {}
    ~Mser() override = default;

    virtual auto delta() const -> int = 0;
    virtual auto minArea() const -> int = 0;
    virtual auto maxArea() const -> int = 0;
    virtual auto maxVariation() const -> double = 0;
    virtual auto minDiversity() const -> double = 0;
    virtual auto maxEvolution() const -> int = 0;
    virtual auto areaThreshold() const -> double = 0;
    virtual auto minMargin() const -> double = 0;
    virtual auto edgeBlurSize() const -> int = 0;

    virtual void setDelta(int delta) = 0;
    virtual void setMinArea(int minArea) = 0;
    virtual void setMaxArea(int maxArea) = 0;
    virtual void setMaxVariation(double maxVariation) = 0;
    virtual void setMinDiversity(double minDiversity) = 0;
    virtual void setMaxEvolution(int maxEvolution) = 0;
    virtual void setAreaThreshold(double areaThreshold) = 0;
    virtual void setMinMargin(double minMargin) = 0;
    virtual void setEdgeBlurSize(int edgeBlurSize) = 0;

};


/*----------------------------------------------------------------*/


/*!
 * \brief Interface ORB
 */
class TL_EXPORT Orb
  : public FeatureBase
{

public:

    Orb() : FeatureBase(Feature::Type::orb)  {}
    ~Orb() override = default;
    
    /*!
     * \brief The maximum number of features to retain
     * \return Number of features to retain
     */
    virtual auto featuresNumber() const -> int = 0;
    
    /*!
     * \brief Pyramid decimation ratio
     * ScaleFactor==2 means the classical pyramid, where each next
     * level has 4x less pixels than the previous, but such a big
     * scale factor will degrade feature matching scores dramatically.
     * On the other hand, too close to 1 scale factor will mean that to
     * cover certain scale range you will need more pyramid levels and
     * so the speed will suffer.
     * \return
     */
    virtual auto scaleFactor() const -> double = 0;
    
    /*!
     * \brief Returns the number of pyramid levels
     * The smallest level will have linear size equal to
     * input_image_linear_size/pow(scaleFactor, nlevels - firstLevel)
     * \return
     */
    virtual auto levelsNumber() const -> int = 0;
    
    /*!
     * \brief Size of the border where the features are not detected
     * \return
     */
    virtual auto edgeThreshold() const -> int = 0;
    
    /*!
     * \brief The level of pyramid to put source image to
     * \return
     */
    virtual auto firstLevel() const -> int = 0;
    
    /*!
     * \brief The number of points that produce each element of the oriented BRIEF descriptor
     * \return
     */
    virtual auto wta_k() const -> int = 0;
    
    /*!
     * \brief scoreType
     * \return
     */
    virtual auto scoreType() const -> std::string = 0;
    
    /*!
     * \brief Size of the patch used by the oriented BRIEF descriptor
     * \return
     */
    virtual auto patchSize() const -> int = 0;
    
    /*!
     * \brief Returns the fast threshold
     * \return
     */
    virtual auto fastThreshold() const -> int = 0;
    
    /*!
     * \brief setScaleFactor
     * \param[in] scaleFactor
     */
    virtual void setScaleFactor(double scaleFactor) = 0;
    
    /*!
     * \brief setFeaturesNumber
     * \param[in] featuresNumber
     */
    virtual void setFeaturesNumber(int featuresNumber) = 0;
    
    /*!
     * \brief Set the number of pyramid levels
     * \param[in] levelsNumber Number of pyramid levels
     */
    virtual void setLevelsNumber(int levelsNumber) = 0;
    
    /*!
     * \brief setEdgeThreshold
     * \param[in] edgeThreshold
     */
    virtual void setEdgeThreshold(int edgeThreshold) = 0;
    
    /*!
     * \brief Set the first level of pyramid
     * \param[in] firstLevel First level
     */
    virtual void setFirstLevel (int firstLevel) = 0;
    
    /*!
     * \brief Set the number of points that produce each element of the oriented BRIEF descriptor
     * The default value 2 means the BRIEF where we take a random point pair and compare their
     * brightnesses, so we get 0/1 response. Other possible values are 3 and 4.
     * \param[in] WTA_K
     */
    virtual void setWTA_K(int WTA_K) = 0;
    
    /*!
     * \brief Set HARRIS_SCORE (Harris algorithm) or FAST_SCORE
     * \param[in] scoreType
     */
    virtual void setScoreType(const std::string &scoreType) = 0;
    
    /*!
     * \brief Set the size of the patch used by the oriented BRIEF descriptor.
     * \param[in] patchSize
     */
    virtual void setPatchSize(int patchSize) = 0;
    
    /*!
     * \brief Set the fast threshold
     * \param[in] fastThreshold
     */
    virtual void setFastThreshold(int fastThreshold) = 0;

};


/*----------------------------------------------------------------*/


class TL_EXPORT Sift
    : public FeatureBase
{

public:

    Sift() : FeatureBase(Feature::Type::sift) {}
    ~Sift() override = default;

    /*!
     * \brief The number of best features to retain
     * The features are ranked by their scores (measured in
     * SIFT algorithm as the local contrast)
     * \return
     */
    virtual auto featuresNumber() const -> int = 0;

    /*!
     * \brief The number of layers in each octave.
     * 3 is the value used in D. Lowe paper. The number of octaves
     * is computed automatically from the image resolution.
     * \return
     */
    virtual auto octaveLayers() const -> int = 0;

    /*!
     * \brief The contrast threshold used to filter out weak features in semi-uniform (low-contrast) regions.
     * The larger the threshold, the less features are produced by the detector.
     * \return
     */
    virtual auto contrastThreshold() const -> double = 0;

    /*!
     * \brief The threshold used to filter out edge-like features
     * Note that the its meaning is different from the contrastThreshold, i.e. the larger
     * the edgeThreshold, the less features are filtered out (more features are retained).
     * \return
     */
    virtual auto edgeThreshold() const -> double = 0;

    /*!
     * \brief The sigma of the Gaussian applied to the input image at the octave 0.
     * If your image is captured with a weak camera with soft lenses, you might want to reduce the number.
     * \return
     */
    virtual auto sigma() const -> double = 0;

    /*!
     * \brief Set the number of best features to retain
     * \param[in] featuresNumber Number of features
     */
    virtual void setFeaturesNumber(int featuresNumber) = 0;

    /*!
     * \brief Set the number of layers in each octave
     * \param[in] octaveLayers The number of layers in each octave (3 by default)
     */
    virtual void setOctaveLayers(int octaveLayers) = 0;

    /*!
     * \brief Set the contrast threshold
     * \param[in] contrastThreshold Contrast threshold
     */
    virtual void setContrastThreshold(double contrastThreshold) = 0;

    /*!
     * \brief setEdgeThreshold
     * \param edgeThreshold
     */
    virtual void setEdgeThreshold(double edgeThreshold) = 0;

    /*!
     * \brief Set sigma of the Gaussian
     * \param[in] sigma Sigma of the Gaussian
     */
    virtual void setSigma(double sigma) = 0;

};


/*----------------------------------------------------------------*/


class TL_EXPORT Star
    : public FeatureBase
{

public:

    Star() : FeatureBase(Feature::Type::star) {}
    ~Star() override = default;

    virtual auto maxSize() const -> int = 0;
    virtual auto responseThreshold() const -> int = 0;
    virtual auto lineThresholdProjected() const -> int = 0;
    virtual auto lineThresholdBinarized() const -> int = 0;
    virtual auto suppressNonmaxSize() const -> int = 0;

    virtual void setMaxSize(int maxSize) = 0;
    virtual void setResponseThreshold(int responseThreshold) = 0;
    virtual void setLineThresholdProjected(int lineThresholdProjected) = 0;
    virtual void setLineThresholdBinarized(int lineThresholdBinarized) = 0;
    virtual void setSuppressNonmaxSize(int suppressNonmaxSize) = 0;

};


/*----------------------------------------------------------------*/


class TL_EXPORT Surf
    : public FeatureBase
{

public:

    Surf() : FeatureBase(Feature::Type::surf) {}
    ~Surf() override = default;

    /*!
     * \brief Threshold for hessian keypoint detector used in SURF
     * \return Threshold
     */
    virtual auto hessianThreshold() const -> double = 0;

    /*!
     * \brief Threshold for hessian keypoint detector used in SURF
     * \param[in] hessianThreshold Threshold for hessian keypoint detector
     */
    virtual void setHessianThreshold(double hessianThreshold) = 0;

    /*!
     * \brief Number of a gaussian pyramid octaves that the detector uses.
     * \return Number of octaves
     */
    virtual auto octaves() const -> int = 0;

    /*!
     * \brief Set the number of a gaussian pyramid octaves that the detector uses.
     * It is set to 4 by default. If you want to get very large features, use the
     * larger value. If you want just small features, decrease it.
     * \param[in] octaves The number of a gaussian pyramid octaves that the detector uses.
     */
    virtual void setOctaves(int octaves) = 0;

    /*!
     * \brief Number of octave layers within each octave
     * \return
     */
    virtual auto octaveLayers() const -> int = 0;

    /*!
     * \brief Set the number of octave layers
     * \param[in] octaveLayers Number of octave layers within each octave
     */
    virtual void setOctaveLayers(int octaveLayers) = 0;

    /*!
     * \brief Extended descriptor
     * \return true (use extended 128-element descriptors) or false (use 64-element descriptors)
     */
    virtual auto extendedDescriptor() const -> bool = 0;

    /*!
     * \brief setExtendedDescriptor
     * \param[in] extendedDescriptor true for use extended 128-element descriptors or false for use 64-element descriptors)
     */
    virtual void setExtendedDescriptor(bool extendedDescriptor) = 0;

    /*!
     * \brief Up-right or rotated features
     * \return true (do not compute orientation of features) or false (compute orientation)
     */
    virtual auto upright() const -> bool = 0;

    /*!
     * \brief compute orientation of features
     * \param[in] upright false for compute orientation
     */
    virtual void setUpright(bool upright) = 0;

};


/*----------------------------------------------------------------*/



/*!
 * \brief Vgg interface
 * K. Simonyan, A. Vedaldi, and A. Zisserman. Learning local feature
 * descriptors using convex optimisation. IEEE Transactions on Pattern
 * Analysis and Machine Intelligence, 2014.
 */
class TL_EXPORT Vgg
    : public FeatureBase
{

public:

    Vgg() : FeatureBase(Feature::Type::vgg) {}
    ~Vgg() override = default;

    /*!
     * \brief Type of descriptor to use
     * Available types are VGG_120 (default), VGG_80, VGG_64, VGG_48
     * \return Type of descriptor
     */
    virtual auto descriptorType() const -> std::string = 0;

    /*!
     * \brief Set the type of descriptor to use
     * Available types are VGG_120 (default), VGG_80, VGG_64, VGG_48
     * \param[in] descriptorType Type of descriptor to use.
     */
    virtual void setDescriptorType(const std::string &descriptorType) = 0;

    virtual auto scaleFactor() const -> double = 0;
    virtual void setScaleFactor(double scaleFactor) = 0;

    /*!
     * \brief Gaussian kernel value for image blur
     * \return Gaussian kernel value
     */
    virtual auto sigma() const -> double = 0;

    /*!
     * \brief Set gaussian kernel value for image blur (default is 1.4f)
     * \param[in] sigma
     */
    virtual void setSigma(double sigma) = 0;

    virtual auto useNormalizeDescriptor() const -> bool = 0;
    virtual void setUseNormalizeDescriptor(bool useNormalizeDescriptor) = 0;

    virtual auto useNormalizeImage() const -> bool = 0;
    virtual void setUseNormalizeImage(bool useNormalizeImage) = 0;

    virtual auto useScaleOrientation() const -> bool = 0;
    virtual void setUseScaleOrientation(bool useScaleOrientation) = 0;
};


/*! \} */ // end of FeatureDetectorAndDescriptor

/*! \} */ // end of Features

} // namespace tl
