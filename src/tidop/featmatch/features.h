#ifndef TL_FEATURES_H
#define TL_FEATURES_H

#include "config_tl.h"


#include <opencv2/features2d.hpp>

#include "tidop/core/flags.h"

namespace tl
{


class TL_EXPORT Feature
{

public:

  enum class Type
  {
    agast,
    akaze,
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
    surf
  };
  
///TODO: GLOH -> https://www.robots.ox.ac.uk/~vgg/research/affine/det_eval_files/mikolajczyk_pami2004.pdf

public:

  Feature(Type type) : mFeatType(type) {}
  virtual ~Feature() = default;

  /*!
   * \brief Recover the default values
   */
  virtual void reset() = 0;

  Type type() const { return mFeatType.flags(); }
  virtual std::string name() const = 0;

protected:

  tl::EnumFlags<Type> mFeatType;

};
ALLOW_BITWISE_FLAG_OPERATIONS(Feature::Type)

/*----------------------------------------------------------------*/

class TL_EXPORT KeypointDetector
{

public:

  KeypointDetector() {}
  virtual ~KeypointDetector() = default;

  /*!
   * \brief Detects keypoints in an image
   * \param[in] img Image
   * \param[out] keyPoints The detected keypoints
   * \param[in] mask Optional mask
   * \return true if error
   */
  virtual bool detect(const cv::Mat &img,
                      std::vector<cv::KeyPoint> &keyPoints,
                      cv::InputArray &mask = cv::noArray()) = 0;

};

/*----------------------------------------------------------------*/

class TL_EXPORT DescriptorExtractor
{

public:

  DescriptorExtractor() {}
  virtual ~DescriptorExtractor() = default;

  /*!
   * \brief Extract descriptors
   * \param[in] img Image
   * \param[in] keyPoints KeyPoints
   * \param[out] descriptors Computed descriptors
   * \return true if error
   */
  virtual bool extract(const cv::Mat &img,
                       std::vector<cv::KeyPoint> &keyPoints,
                       cv::Mat &descriptors) = 0;

};


/*----------------------------------------------------------------*/


class TL_EXPORT IAgast
  : public Feature
{

public:

  IAgast() : Feature(Feature::Type::agast) {}
  virtual ~IAgast() = default;

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
  virtual int threshold() const = 0;

  /*!
   * \brief Non Maximal Suppression for removing adjacent corners
   * \return
   */
  virtual bool nonmaxSuppression() const = 0;

  /*!
   * \brief Detector Types
   * - AGAST_5_8: AGAST-5 decision tree whith the 8 pixels mask
   * - AGAST_7_12d: AGAST-7 decision tree whith the Diamond shaped 12 pixels mask
   * - AGAST_7_12s: AGAST-7 decision tree whith the Squared shaped 12 pixels mask
   * - OAST_9_16: OAST-9 (Optimal AST) decision tree whith the 16 pixels mask
   * \return Detector Type
   */
  virtual std::string detectorType() const = 0;

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


class TL_EXPORT IAkaze
  : public Feature
{

public:

  IAkaze() : Feature(Feature::Type::akaze) {}
  virtual ~IAkaze() = default;

  /*!
   * \brief Type of the extracted descriptor
   * \return KAZE, KAZE_UPRIGHT, MLDB or MLDB_UPRIGHT
   */
  virtual std::string descriptorType() const = 0;

  /*!
   * \brief Size of the descriptor in bits. 0 -\> Full size
   * \return Descriptor size
   */
  virtual int descriptorSize() const = 0;

  /*!
   * \brief Number of channels in the descriptor [1, 2, 3 (default)]
   * \return Number of channels
   */
  virtual int descriptorChannels() const = 0;

  /*!
   * \brief Detector response threshold to accept point
   * \return threshold
   */
  virtual double threshold() const = 0;

  /*!
   * \brief Maximum octave evolution of the image
   * \return
   */
  virtual int octaves() const = 0;

  /*!
   * \brief Default number of sublevels per scale level
   * \return
   */
  virtual int octaveLayers() const = 0;

  /*!
   * \brief Diffusivity type
   * \return DIFF_PM_G1, DIFF_PM_G2 (default), DIFF_WEICKERT or DIFF_CHARBONNIER
   */
  virtual std::string diffusivity() const = 0;

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
 * \brief Interface for BRIEF
 *
 * Michael Calonder, Vincent Lepetit, Christoph Strecha, and Pascal Fua.
 * Brief: Binary robust independent elementary features. In Computer
 * Vision–ECCV 2010, pages 778–792. Springer, 2010
 * https://www.cs.ubc.ca/~lowe/525/papers/calonder_eccv10.pdf
 */
class TL_EXPORT IBrief
  : public Feature
{

public:

  IBrief() : Feature(Feature::Type::brief) {}
  virtual ~IBrief() = default;

  /*!
   * \brief Legth of the descriptor in bytes
   * Valid values are: 16, 32 (default) or 64
   * \return Legth of the descriptor
   */
  virtual std::string bytes() const = 0;

  /*!
   * \brief useOrientation
   * \return
   */
  virtual bool useOrientation() const = 0;

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
class TL_EXPORT IBrisk
  : public Feature
{

public:

  IBrisk() : Feature(Feature::Type::brisk) {}
  virtual ~IBrisk() = default;

  /*!
   * \brief AGAST detection threshold score (Default=30)
   * \return Threshold
   */
  virtual int threshold() const = 0;

  /*!
   * \brief Detection octaves (Default=3)
   * \return
   */
  virtual int octaves() const = 0;

  /*!
   * \brief Pattern Scale (Default=1.0)
   * \return
   */
  virtual double patternScale() const = 0;

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
class TL_EXPORT IDaisy
  : public Feature
{

public:

  IDaisy() : Feature(Feature::Type::daisy) {}
  virtual ~IDaisy() = default;

  /*!
   * \brief Radius of the descriptor at the initial scale (Default=15.)
   * \return
   */
  virtual double radius() const = 0;

  /*!
   * \brief Amount of radial range division quantity (Default=3)
   * \return
   */
  virtual int qRadius() const = 0;

  /*!
   * \brief Amount of angular range division quantity (Default=8)
   * \return
   */
  virtual int qTheta() const = 0;

  /*!
   * \brief Amount of gradient orientations range division quantity (Default=8)
   * \return
   */
  virtual int qHist() const = 0;

  /*!
   * \brief Descriptor normalization type
   * Normalization types:
   * - NRM_NONE: not make any normalization (Default)
   * - NRM_PARTIAL: mean that histograms are normalized independently for L2 norm equal to 1.0
   * - NRM_FULL: mean that descriptors are normalized for L2 norm equal to 1.0
   * - NRM_SIFT: mean that descriptors are normalized for L2 norm equal to 1.0 but no individual one is bigger than 0.154 as in SIFT
   * \return
   */
  virtual std::string norm() const = 0;

  //virtual homography() const = 0;

  /*!
   * \brief Interpolation disabled or enabled (Default)
   * \return
   */
  virtual bool interpolation() const = 0;

  /*!
   * \brief Using orientation (Default=false)
   * \return
   */
  virtual bool useOrientation() const = 0;

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
 *
 */
class TL_EXPORT IFast
  : public Feature
{

public:

  IFast() : Feature(Feature::Type::fast) {}
  virtual ~IFast() = default;

  /*!
   * \brief threshold (Default=10)
   * \return
   */
  virtual int threshold() const = 0;

  /*!
   * \brief Non Maximal Suppression for removing adjacent corners (default=true)
   * \return
   */
  virtual bool nonmaxSuppression() const = 0;

  /*!
   * \brief Detector Types
   * - TYPE_5_8: FAST-5 decision tree whith the 8 pixels mask
   * - TYPE_7_12: FAST-7 decision tree whith the 12 pixels mask
   * - TYPE_9_16: FAST-9 decision tree whith the 16 pixels mask (default)
   * \return Detector Type
   */
  virtual std::string detectorType() const = 0;

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
  virtual void setDetectorType(std::string detectorType) = 0;

};


/*----------------------------------------------------------------*/


/*!
 * \brief Interface for FREAK class
 * Alexandre Alahi, Raphael Ortiz, and Pierre Vandergheynst.
 * Freak: Fast retina keypoint. In Computer Vision and Pattern
 * Recognition (CVPR), 2012 IEEE Conference on, pages 510–517. Ieee, 2012.
 */
class TL_EXPORT IFreak
  : public Feature
{

public:

  IFreak() : Feature(Feature::Type::freak) {}
  virtual ~IFreak() = default;

  /*!
   * \brief Orientation normalization (Default=true)
   * \return
   */
  virtual bool orientationNormalized() const = 0;

  /*!
   * \brief Scale normalization (Default=true)
   * \return
   */
  virtual bool scaleNormalized() const = 0;

  /*!
   * \brief Scaling of the description pattern (Default=22.)
   * \return
   */
  virtual double patternScale() const = 0;

  /*!
   * \brief Number of octaves covered by the detected keypoints (Default=4)
   * \return Number of octaves
   */
  virtual int octaves() const = 0;

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


class TL_EXPORT IGftt
  : public Feature
{

public:

  IGftt() : Feature(Feature::Type::gftt) {}
  virtual ~IGftt() = default;

  virtual int maxFeatures() const = 0;
  virtual double qualityLevel() const = 0;
  virtual double minDistance() const = 0;
  virtual int blockSize() const = 0;
  virtual bool harrisDetector() const = 0;
  virtual double k() const = 0;

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
 * Navneet Dalal and Bill Triggs @cite Dalal2005
 */
class TL_EXPORT IHog
  : public Feature
{

public:

  IHog() : Feature(Feature::Type::hog) {}
  virtual ~IHog() = default;

  virtual cv::Size winSize() const = 0;
  virtual cv::Size blockSize() const = 0;
  virtual cv::Size blockStride() const = 0;
  virtual cv::Size cellSize() const = 0;
  virtual int nbins() const = 0;
  virtual int derivAperture() const = 0;
//  virtual double winSigma() const = 0;
//  virtual std::string histogramNormType() const = 0;
//  virtual double l2HysThreshold() const = 0;
//  virtual bool gammaCorrection() const = 0;
//  virtual double freeCoef() const = 0;
//  virtual int nlevels() const = 0;
//  virtual bool signedGradient() const = 0;

  virtual void setWinSize(const cv::Size &winSize) = 0;
  virtual void setBlockSize(const cv::Size &blockSize) = 0;
  virtual void setBlockStride(const cv::Size &blockStride) = 0;
  virtual void setCellSize(const cv::Size &cellSize) = 0;
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
 * \brief The IKaze class
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
class TL_EXPORT IKaze
  : public Feature
{

public:

  IKaze() : Feature(Feature::Type::kaze) {}
  virtual ~IKaze() = default;

  /*!
   * \brief Extended descriptor
   * \return true if use extended 128-element descriptors or false if use 64-element descriptors (Default)
   */
  virtual bool extendedDescriptor() const = 0;

  /*!
   * \brief Use of upright descriptors (non rotation-invariant)
   * \return true if use upright descriptors (Default=false)
   */
  virtual bool upright() const = 0;

  /*!
   * \brief Detector response threshold to accept point
   * \return threshold
   */
  virtual double threshold() const = 0;

  /*!
   * \brief Maximum octave evolution of the image
   * \return octaves
   */
  virtual int octaves() const = 0;

  /*!
   * \brief Default number of sublevels per scale level
   * \return
   */
  virtual int octaveLayers() const = 0;

  /*!
   * \brief Diffusivity type
   * \return DIFF_PM_G1, DIFF_PM_G2 (default), DIFF_WEICKERT or DIFF_CHARBONNIER
   */
  virtual std::string diffusivity() const = 0;

  /*!
   * \brief Set extended descriptor
   * \param[in] extended true for use extended 128-element descriptors or false for use 64-element descriptors (default)
   */
  virtual void setExtendedDescriptor(bool extended) = 0;

  /*!
   * \brief Set to enable use of upright descriptors (non rotation-invariant)
   * \param[in] upright
   */
  virtual void setUpright(bool upright) = 0;

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


class TL_EXPORT ILatch
  : public Feature
{

public:

  ILatch() : Feature(Feature::Type::latch) {}
  virtual ~ILatch() = default;

  virtual std::string bytes() const = 0;
  virtual bool rotationInvariance() const = 0;
  virtual int halfSsdSize() const = 0;

  virtual void setBytes(const std::string &bytes) = 0;
  virtual void setRotationInvariance(bool rotationInvariance) = 0;
  virtual void setHalfSsdSize(int halfSsdSize) = 0;

};


/*----------------------------------------------------------------*/


/*!
 * \brief Interface for LUCID class
 * Eric Christiansen David Kriegman Ziegler, Andrew and Serge J. Belongie.
 * Locally uniform comparison image descriptor
 */
class TL_EXPORT ILucid
  : public Feature
{
public:

  ILucid() : Feature(Feature::Type::lucid) {}
  virtual ~ILucid() = default;

  /*!
   * \brief kernel for descriptor construction
   * 1=3x3, 2=5x5, 3=7x7 and so forth. Default=1
   * \return
   */
  virtual int lucidKernel() const = 0;

  /*!
   * \brief kernel for blurring image prior to descriptor construction
   * 1=3x3, 2=5x5, 3=7x7 and so forth. Default=2
   * \return
   */
  virtual int blurKernel() const = 0;

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


class TL_EXPORT ILss
  : public Feature
{

public:

  ILss() : Feature(Feature::Type::lss) {}
  virtual ~ILss() = default;

};


/*----------------------------------------------------------------*/


class TL_EXPORT IMsd
  : public Feature
{

public:

  IMsd() : Feature(Feature::Type::msd) {}
  virtual ~IMsd() = default;

  virtual double thresholdSaliency() const = 0;
  virtual int patchRadius() const = 0;
  virtual int knn() const = 0;
  virtual int searchAreaRadius() const = 0;
  virtual double scaleFactor() const = 0;
  virtual int NMSRadius() const = 0;
  virtual int nScales() const = 0;
  virtual int NMSScaleRadius() const = 0;
  virtual bool computeOrientation() const = 0;
  virtual bool affineMSD() const = 0;
  //virtual int tilts() const = 0;
  virtual int affineTilts() const = 0;

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


class TL_EXPORT IMser
  : public Feature
{

public:

  IMser() : Feature(Feature::Type::mser) {}
  virtual ~IMser() = default;

  virtual int delta() const = 0;
  virtual int minArea() const = 0;
  virtual int maxArea() const = 0;
  virtual double maxVariation() const = 0;
  virtual double minDiversity() const = 0;
  virtual int maxEvolution() const = 0;
  virtual double areaThreshold() const = 0;
  virtual double minMargin() const = 0;
  virtual int edgeBlurSize() const = 0;

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
class TL_EXPORT IOrb
  : public Feature
{

public:

  IOrb() : Feature(Feature::Type::orb)  {}
  virtual ~IOrb() = default;

  virtual int featuresNumber() const = 0;
  virtual double scaleFactor() const = 0;
  virtual int levelsNumber() const = 0;
  virtual int edgeThreshold() const = 0;
  virtual int wta_k() const = 0;
  virtual std::string scoreType() const = 0;
  virtual int patchSize() const = 0;
  virtual int fastThreshold() const = 0;

  virtual void setScaleFactor(double scaleFactor) = 0;
  virtual void setFeaturesNumber(int featuresNumber) = 0;
  virtual void setLevelsNumber(int levelsNumber) = 0;
  virtual void setEdgeThreshold(int edgeThreshold) = 0;
  virtual void setWTA_K(int WTA_K) = 0;
  virtual void setScoreType(const std::string &scoreType) = 0;
  virtual void setPatchSize(int patchSize) = 0;
  virtual void setFastThreshold(int fastThreshold) = 0;

};


/*----------------------------------------------------------------*/


class TL_EXPORT ISift
  : public Feature
{
public:
  ISift() : Feature(Feature::Type::sift)  {}
  virtual ~ISift() = default;

  /*!
   * \brief featuresNumber
   * \return
   */
  virtual int featuresNumber() const = 0;

  /*!
   * \brief octaveLayers
   * \return
   */
  virtual int octaveLayers() const = 0;

  /*!
   * \brief contrastThreshold
   * \return
   */
  virtual double contrastThreshold() const = 0;

  /*!
   * \brief edgeThreshold
   * \return
   */
  virtual double edgeThreshold() const = 0;

  /*!
   * \brief sigma
   * \return
   */
  virtual double sigma() const = 0;

  /*!
   * \brief setFeaturesNumber
   * \param featuresNumber
   */
  virtual void setFeaturesNumber(int featuresNumber) = 0;

  /*!
   * \brief setOctaveLayers
   * \param octaveLayers
   */
  virtual void setOctaveLayers(int octaveLayers) = 0;

  /*!
   * \brief setContrastThreshold
   * \param contrastThreshold
   */
  virtual void setContrastThreshold(double contrastThreshold) = 0;

  /*!
   * \brief setEdgeThreshold
   * \param edgeThreshold
   */
  virtual void setEdgeThreshold(double edgeThreshold) = 0;

  /*!
   * \brief setSigma
   * \param sigma
   */
  virtual void setSigma(double sigma) = 0;

};


/*----------------------------------------------------------------*/


class TL_EXPORT IStar
  : public Feature
{

public:

  IStar() : Feature(Feature::Type::star) {}
  virtual ~IStar() = default;

  virtual int maxSize() const  = 0;
  virtual int responseThreshold() const  = 0;
  virtual int lineThresholdProjected() const  = 0;
  virtual int lineThresholdBinarized() const  = 0;
  virtual int suppressNonmaxSize() const  = 0;

  virtual void setMaxSize(int maxSize) = 0;
  virtual void setResponseThreshold(int responseThreshold) = 0;
  virtual void setLineThresholdProjected(int lineThresholdProjected) = 0;
  virtual void setLineThresholdBinarized(int lineThresholdBinarized) = 0;
  virtual void setSuppressNonmaxSize(int suppressNonmaxSize) = 0;

};


/*----------------------------------------------------------------*/


class TL_EXPORT ISurf
  : public Feature
{
public:

  ISurf() : Feature(Feature::Type::surf) {}
  virtual ~ISurf() = default;

  /*!
   * \brief Threshold for hessian keypoint detector used in SURF
   * \return
   */
  virtual double hessianThreshold() const = 0;

  /*!
   * \brief Threshold for hessian keypoint detector used in SURF
   * \param[in] hessianThreshold
   */
  virtual void setHessianThreshold(double hessianThreshold) = 0;

  /*!
   * \brief Number of pyramid octaves the keypoint detector will use.
   * \return
   */
  virtual int octaves() const = 0;

  /*!
   * \brief Set the number of pyramid octaves
   * \param[in] octaves Number of pyramid octaves the keypoint detector will use.
   */
  virtual void setOctaves(int octaves) = 0;

  /*!
   * \brief Number of octave layers within each octave
   * \return
   */
  virtual int octaveLayers() const = 0;

  /*!
   * \brief Set the number of octave layers
   * \param[in] octaveLayers Number of octave layers within each octave
   */
  virtual void setOctaveLayers(int octaveLayers) = 0;

  /*!
   * \brief Extended descriptor
   * \return true (use extended 128-element descriptors) or false (use 64-element descriptors)
   */
  virtual bool extendedDescriptor() const = 0;

  /*!
   * \brief setExtendedDescriptor
   * \param[in] extendedDescriptor true for use extended 128-element descriptors or false for use 64-element descriptors)
   */
  virtual void setExtendedDescriptor(bool extendedDescriptor) = 0;

  /*!
   * \brief Up-right or rotated features
   * \return true (do not compute orientation of features) or false (compute orientation)
   */
  virtual bool rotatedFeatures() const = 0;

  /*!
   * \brief compute orientation of features
   * \param[in] rotatedFeatures false for compute orientation
   */
  virtual void setRotatedFeatures(bool rotatedFeatures) = 0;

};


/*----------------------------------------------------------------*/

TL_EXPORT void featuresWrite(const std::string &fname, const std::vector<cv::KeyPoint> &keyPoints, const cv::Mat &descriptors);
TL_EXPORT void featuresRead(const std::string &fname, std::vector<cv::KeyPoint> &keyPoints, cv::Mat &descriptors);

/*----------------------------------------------------------------*/



} // namespace tl


#endif // TL_FEATURES_H
