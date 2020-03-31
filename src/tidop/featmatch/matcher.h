#ifndef TL_MATCHER_H
#define TL_MATCHER_H

#include "config_tl.h"

#include <memory>

#include <opencv2/xfeatures2d.hpp>
#ifdef HAVE_CUDA
#include <opencv2/cudafeatures2d.hpp>
#endif // HAVE_CUDA

#include "tidop/core/flags.h"


namespace tl
{


class TL_EXPORT Match
{

public:

  enum class Type
  {
    flann,
    brute_force
  };

public:

  Match(Type type) : mMatchType(type) {}
  virtual ~Match() = default;

  /*!
   * \brief Recover the default values
   */
  virtual void reset() = 0;

  Type type() const { return mMatchType.flags(); }
  virtual std::string name() const = 0;

protected:

  tl::EnumFlags<Type> mMatchType;

};
ALLOW_BITWISE_FLAG_OPERATIONS(Match::Type)


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
   * \param[out] matches
   * \param[in] mask
   * \return true if error
   */
  virtual bool match(cv::InputArray &queryDescriptors,
                     cv::InputArray &trainDescriptors,
                     std::vector<std::vector<cv::DMatch>> &matches,
                     cv::InputArray mask = cv::noArray()) = 0;

};


/*----------------------------------------------------------------*/


class TL_EXPORT IFlannMatcher
  : public Match
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

  IFlannMatcher() : Match(Match::Type::flann) {}
  virtual ~IFlannMatcher() override = default;

  virtual Index index() const = 0;
  virtual void setIndex(Index index) = 0;

};

/*----------------------------------------------------------------*/

class TL_EXPORT FlannMatcherProperties
  : public IFlannMatcher
{

public:

  FlannMatcherProperties();
  virtual ~FlannMatcherProperties() override;

// Match interface

public:

  virtual void reset() override;
  std::string name() const final;

// IFlannMatcher interface

public:

  virtual Index index() const override;
  virtual void setIndex(Index index) override;

private:

  Index mIndex;
};

/*----------------------------------------------------------------*/

class TL_EXPORT FlannMatcher
  : public FlannMatcherProperties,
    public DescriptorMatcher
{

public:

  FlannMatcher();
  FlannMatcher(IFlannMatcher::Index index);
  ~FlannMatcher() override;

private:

  void update();

// DescriptorMatcher interface

public:

  bool match(cv::InputArray &queryDescriptors,
             cv::InputArray &trainDescriptors,
             std::vector<std::vector<cv::DMatch>> &matches,
             cv::InputArray mask = cv::noArray()) override;

// Match interface

public:

  void reset() override;

// IFlannMatcher interface

public:

  void setIndex(Index index) override;

protected:

  cv::Ptr<cv::FlannBasedMatcher> mFlannBasedMatcher;

};

/*----------------------------------------------------------------*/


class TL_EXPORT IBruteForceMatcher
  : public Match
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

  IBruteForceMatcher() : Match(Match::Type::brute_force) {}
  virtual ~IBruteForceMatcher() override = default;

  virtual Norm normType() const = 0;
  virtual void setNormType(Norm normType) = 0;

};

/*----------------------------------------------------------------*/

class TL_EXPORT BruteForceMatcherProperties
  : public IBruteForceMatcher
{

public:

  BruteForceMatcherProperties();
  ~BruteForceMatcherProperties() override;

// Match interface

public:

  virtual void reset() override;
  std::string name() const final;

// IBruteForceMatcher interface

public:

  virtual Norm normType() const override;
  virtual void setNormType(Norm normType) override;

private:

  Norm mNormType;

};

/*----------------------------------------------------------------*/

class TL_EXPORT BruteForceMatcher
  : public BruteForceMatcherProperties,
    public DescriptorMatcher
{

public:

  BruteForceMatcher();
  BruteForceMatcher(Norm normType);
  ~BruteForceMatcher() override {}

private:

  void update();

// DescriptorMatcher interface

public:

  bool match(cv::InputArray &queryDescriptors,
             cv::InputArray &trainDescriptors,
             std::vector<std::vector<cv::DMatch>> &matches,
             cv::InputArray mask = cv::noArray()) override;

// Match interface

public:

  void reset() override;

// IBruteForceMatcher interface

public:

  void setNormType(Norm normType) override;

protected:

  cv::Ptr<cv::BFMatcher> mBFMatcher;
};

/*----------------------------------------------------------------*/

#ifdef HAVE_CUDA

class TL_EXPORT BruteForceMatcherCuda
  : public BruteForceMatcherProperties,
    public DescriptorMatcher
{

public:

  BruteForceMatcherCuda();
  BruteForceMatcherCuda(Norm normType);
  ~BruteForceMatcherCuda() override {}

private:

  void update();

// DescriptorMatcher interface

public:

  bool match(cv::InputArray &queryDescriptors,
             cv::InputArray &trainDescriptors,
             std::vector<std::vector<cv::DMatch>> &matches,
             cv::InputArray mask = cv::noArray()) override;

// Match interface

public:

  void reset() override;

// IBruteForceMatcher interface

public:

  void setNormType(Norm normType) override;

protected:

  cv::Ptr<cv::cuda::DescriptorMatcher> mBFMatcher;
};

#endif // HAVE_CUDA

/*----------------------------------------------------------------*/


class TL_EXPORT IRobustMatcherRefinement
{

public:

  enum class GeometricTest
  {
    homography,
    fundamental,
    essential
  };

  enum class HomographyComputeMethod
  {
    all_points,
    ransac,
    lmeds,
    rho
  };

  enum class FundamentalComputeMethod
  {
    algorithm_7_point,
    algorithm_8_point,
    ransac,
    lmeds
  };

  enum class EssentialComputeMethod
  {
    ransac,
    lmeds
  };

public:

  IRobustMatcherRefinement() {}
  virtual ~IRobustMatcherRefinement() = default;

  virtual double ratio() const = 0;
  virtual void setRatio(double ratio) = 0;

  virtual bool crossCheck() const = 0;
  virtual void setCrossCheck(bool crossCheck) = 0;

  virtual GeometricTest geometricTest() const = 0;
  virtual void setGeometricTest(GeometricTest geometricTest) = 0;

  virtual HomographyComputeMethod homographyComputeMethod() const = 0;
  virtual void setHomographyComputeMethod(HomographyComputeMethod computeMethod) = 0;

  virtual FundamentalComputeMethod fundamentalComputeMethod() const = 0;
  virtual void setFundamentalComputeMethod(FundamentalComputeMethod computeMethod) = 0;

  virtual EssentialComputeMethod essentialComputeMethod() const = 0;
  virtual void setEssentialComputeMethod(EssentialComputeMethod computeMethod) = 0;

  virtual double distance() const = 0;
  virtual void setDistance(double distance) = 0;

  virtual double confidence() const = 0;
  virtual void setConfidence(double confidence) = 0;

  virtual int maxIter() const = 0;
  virtual void setMaxIters(int maxIter) = 0;

  /*!
   * \brief Recover the default values
   */
  virtual void reset() = 0;

};


/*----------------------------------------------------------------*/

class TL_EXPORT RobustMatcherProperties
  : public IRobustMatcherRefinement
{

public:

  RobustMatcherProperties();
  ~RobustMatcherProperties() override = default;

// IRobustMatcherRefinement interface

public:

  double ratio() const override;
  void setRatio(double ratio) override;
  bool crossCheck() const override;
  void setCrossCheck(bool crossCheck) override;
  GeometricTest geometricTest() const override;
  void setGeometricTest(GeometricTest geometricTest) override;
  HomographyComputeMethod homographyComputeMethod() const override;
  void setHomographyComputeMethod(HomographyComputeMethod computeMethod) override;
  FundamentalComputeMethod fundamentalComputeMethod() const override;
  void setFundamentalComputeMethod(FundamentalComputeMethod computeMethod) override;
  EssentialComputeMethod essentialComputeMethod() const override;
  void setEssentialComputeMethod(EssentialComputeMethod computeMethod) override;
  double distance() const override;
  void setDistance(double distance) override;
  double confidence() const override;
  void setConfidence(double confidence) override;
  int maxIter() const override;
  void setMaxIters(int maxIter) override;
  void reset() override;

private:

  double mRatio;
  bool mCrossCheck;
  GeometricTest mGeometricTest;
  HomographyComputeMethod mHomographyComputeMethod;
  FundamentalComputeMethod mFundamentalComputeMethod;
  EssentialComputeMethod mEssentialComputeMethod;
  double mDistance;
  double mConfidence;
  int mMaxIters;

};

/*----------------------------------------------------------------*/

/*!
 * \brief Matching robusto
 *
 * Basado en:
 *
 * http://docs.opencv.org/3.1.0/dc/d2c/tutorial_real_time_pose.html
 */
class TL_EXPORT IRobustMatching
{

public:

  IRobustMatching() {}
  virtual ~IRobustMatching() = default;

public:

  /*!
   * \brief Establece el metodo de matching
   * \param[in] matcher
   */
  virtual void setDescriptorMatcher(const std::shared_ptr<DescriptorMatcher> &matcher) = 0;

  /*!
   * \brief Test de ratio
   * \param[in] matches
   */
  /*!
   * \brief Ratio test
   * \param[in] matches
   * \param[in] ratio
   * \param[out] goodMatches
   * \param[out] wrongMatches
   */
  static std::vector<std::vector<cv::DMatch>> ratioTest(const std::vector<std::vector<cv::DMatch>> &matches,
                        double ratio,
                        std::vector<std::vector<cv::DMatch>> *wrongMatches = nullptr)
  {
    std::vector<std::vector<cv::DMatch>> goodMatches;

    for (size_t i = 0; i < matches.size(); i++){

      if (matches[i].size() > 1){
        // check distance ratio
        if (matches[i][0].distance / matches[i][1].distance <= static_cast<float>(ratio)) {
            goodMatches.push_back(matches[i]);
        } else {
          if (wrongMatches){
            wrongMatches->push_back(matches[i]);
          }
        }
      }

    }

    return goodMatches;
  }

  /*!
   * \brief test cruzado
   * Busqueda de matches simétricos
   * \param[in] matches12
   * \param[in] matches21
   * \param[out] goodMatches
   * \param[out] wrongMatches
   */
  static std::vector<cv::DMatch> crossCheckTest(const std::vector<std::vector<cv::DMatch>> &matches12,
                                                const std::vector<std::vector<cv::DMatch>> &matches21,
                                                std::vector<cv::DMatch> *wrongMatches = nullptr)
  {
    std::vector<cv::DMatch> goodMatches;

    for (size_t i = 0; i < matches12.size(); i++){

      if (matches12[i].empty() || matches12[i].size() < 2)
        continue;

      bool findGoodMatch = false;
      for (size_t j = 0; j < matches21.size(); j++){

        if (matches21[j].empty() || matches21[j].size() < 2)
          continue;

        if (matches12[i][0].queryIdx == matches21[j][0].trainIdx &&
            matches21[j][0].queryIdx == matches12[i][0].trainIdx) {
            goodMatches.push_back(matches12[i][0]);
          findGoodMatch = true;
          break;
        }

      }

      if (findGoodMatch == false && wrongMatches)
        wrongMatches->push_back(matches12[i][0]);

    }

    return goodMatches;
  }

  virtual std::vector<cv::DMatch> geometricFilter(const std::vector<cv::DMatch> &matches,
                                                  const std::vector<cv::KeyPoint>& keypoints1,
                                                  const std::vector<cv::KeyPoint>& keypoints2,
                                                  std::vector<cv::DMatch> *wrongMatches = nullptr) = 0;

  virtual std::vector<cv::DMatch> filterByHomographyMatrix(const std::vector<cv::DMatch> &matches,
                                                           const std::vector<cv::Point2f>& points1,
                                                           const std::vector<cv::Point2f>& points2,
                                                           std::vector<cv::DMatch> *wrongMatches = nullptr) = 0;
  virtual std::vector<cv::DMatch> filterByEssentialMatrix(const std::vector<cv::DMatch> &matches,
                                                          const std::vector<cv::Point2f>& points1,
                                                          const std::vector<cv::Point2f>& points2,
                                                          std::vector<cv::DMatch> *wrongMatches = nullptr) = 0;
  virtual std::vector<cv::DMatch> filterByFundamentalMatrix(const std::vector<cv::DMatch> &matches,
                                                            const std::vector<cv::Point2f>& points1,
                                                            const std::vector<cv::Point2f>& points2,
                                                            std::vector<cv::DMatch> *wrongMatches = nullptr) = 0;

  /*!
   * \brief Matching
   * \param[in] queryDescriptor Query descriptor
   * \param[in] trainDescriptor Train descriptor
   * \param[out] wrongMatches Wrong matches
   * \return Good matches
   */
  virtual std::vector<cv::DMatch> match(const cv::Mat &queryDescriptor,
                                        const cv::Mat &trainDescriptor,
                                        std::vector<cv::DMatch> *wrongMatches = nullptr) = 0;

private:

  /*!
   * \brief Robust matching
   * Feature matching using ratio and symmetry tests
   * \param[in] queryDescriptor Query descriptor
   * \param[in] trainDescriptor Train descriptor
   * \param[out] wrongMatches Wrong matches
   * \return Good matches
   */
  virtual std::vector<cv::DMatch> robustMatch(const cv::Mat &queryDescriptor,
                                              const cv::Mat &trainDescriptor,
                                              std::vector<cv::DMatch> *wrongMatches) = 0;

  /*!
   * \brief Robust matching
   * Feature matching using ratio test
   * \param[in] queryDescriptor Query descriptor
   * \param[in] trainDescriptor Train descriptor
   * \param[out] wrongMatches Wrong matches
   * \return Good matches
   */
  virtual std::vector<cv::DMatch> fastRobustMatch(const cv::Mat &queryDescriptor,
                                                  const cv::Mat &trainDescriptor,
                                                  std::vector<cv::DMatch> *wrongMatches) = 0;
};


/*----------------------------------------------------------------*/


class TL_EXPORT RobustMatching
  : public IRobustMatching,
    public RobustMatcherProperties
{

public:

  RobustMatching(const std::shared_ptr<DescriptorMatcher> &matcher);
  RobustMatching(const std::shared_ptr<DescriptorMatcher> &matcher,
                 double ratio,
                 bool crossCheck,
                 GeometricTest geometricTest,
                 HomographyComputeMethod homographyComputeMethod,
                 FundamentalComputeMethod fundamentalComputeMethod,
                 EssentialComputeMethod essentialComputeMethod,
                 double distance,
                 double confidence,
                 int maxIter);
  ~RobustMatching() override;

// IRobustMatching interface

public:

  void setDescriptorMatcher(const std::shared_ptr<DescriptorMatcher> &matcher) override;

  std::vector<cv::DMatch> match(const cv::Mat &queryDescriptor,
                                const cv::Mat &trainDescriptor,
                                std::vector<cv::DMatch> *wrongMatches = nullptr) override;

  std::vector<cv::DMatch> geometricFilter(const std::vector<cv::DMatch> &matches,
                                          const std::vector<cv::KeyPoint>& keypoints1,
                                          const std::vector<cv::KeyPoint>& keypoints2,
                                          std::vector<cv::DMatch> *wrongMatches = nullptr) override;

  std::vector<cv::DMatch> filterByHomographyMatrix(const std::vector<cv::DMatch> &matches,
                                                   const std::vector<cv::Point2f>& points1,
                                                   const std::vector<cv::Point2f>& points2,
                                                   std::vector<cv::DMatch> *wrongMatches = nullptr) override;
  std::vector<cv::DMatch> filterByEssentialMatrix(const std::vector<cv::DMatch> &matches,
                                                  const std::vector<cv::Point2f>& points1,
                                                  const std::vector<cv::Point2f>& points2,
                                                  std::vector<cv::DMatch> *wrongMatches = nullptr) override;
  std::vector<cv::DMatch> filterByFundamentalMatrix(const std::vector<cv::DMatch> &matches,
                                                    const std::vector<cv::Point2f>& points1,
                                                    const std::vector<cv::Point2f>& points2,
                                                    std::vector<cv::DMatch> *wrongMatches = nullptr) override;

private:

  std::vector<cv::DMatch> robustMatch(const cv::Mat &queryDescriptor,
                                      const cv::Mat &trainDescriptor,
                                      std::vector<cv::DMatch> *wrongMatches) override;
  std::vector<cv::DMatch> fastRobustMatch(const cv::Mat &queryDescriptor,
                                          const cv::Mat &trainDescriptor,
                                          std::vector<cv::DMatch> *wrongMatches) override;

protected:

  std::shared_ptr<DescriptorMatcher> mMatcher;
};

/*----------------------------------------------------------------*/

/*!
 * \brief Matches write
 * \param[in] fname File name
 * \param[in] matches Correct matches
 * \param[in] wrongMatches Wrong matches
 */
TL_EXPORT void matchesWrite(const std::string &fname, const std::vector<cv::DMatch> &matches, const std::vector<cv::DMatch> &wrongMatches = std::vector<cv::DMatch>());

/*!
 * \brief Matches read
 * \param[in] fname File name
 * \param[out] matches Correct matches
 * \param[out] wrongMatches Wrong matches
 */
TL_EXPORT void matchesRead(const std::string &fname, std::vector<cv::DMatch> *matches, std::vector<cv::DMatch> *wrongMatches = nullptr);


/*----------------------------------------------------------------*/


} // namespace tl


#endif // TL_MATCHER_H
