#include "matcher.h"

#include "tidop/core/messages.h"

#include <opencv2/xfeatures2d.hpp>
#include <opencv2/calib3d.hpp>

#if (__cplusplus >= 201703L)
//C++17
//http://en.cppreference.com/w/cpp/filesystem
#include <filesystem>
namespace fs = std::filesystem;
#elif defined HAVE_BOOST
//Boost
//http://www.boost.org/doc/libs/1_66_0/libs/filesystem/doc/index.htm
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#endif

namespace tl
{


FlannMatcherProperties::FlannMatcherProperties()
  : mIndex(FlannMatcherProperties::Index::kdtree)
{
}

FlannMatcherProperties::~FlannMatcherProperties()
{
}

void FlannMatcherProperties::reset()
{
  mIndex = FlannMatcherProperties::Index::kdtree;
}

std::string FlannMatcherProperties::name() const
{
  return std::string("Flann Based Matching");
}

FlannMatcher::Index FlannMatcherProperties::index() const
{
  return mIndex;
}

void FlannMatcherProperties::setIndex(FlannMatcher::Index index)
{
  mIndex = index;
}

/*----------------------------------------------------------------*/


FlannMatcherImp::FlannMatcherImp()
{
  update();
}

FlannMatcherImp::FlannMatcherImp(Index index)
{
  FlannMatcherProperties::setIndex(index);
  update();
}

void FlannMatcherImp::update()
{
  cv::Ptr<cv::flann::IndexParams> indexParams;
  if (FlannMatcherProperties::index() == FlannMatcherProperties::Index::kdtree){
    indexParams = cv::makePtr<cv::flann::KDTreeIndexParams>();
  } else if (FlannMatcherProperties::index() == FlannMatcherProperties::Index::lsh){
    indexParams = cv::makePtr<cv::flann::LshIndexParams>(12, 20, 2);
  }
  mFlannBasedMatcher = cv::Ptr<cv::FlannBasedMatcher>(new cv::FlannBasedMatcher(indexParams));
}

bool FlannMatcherImp::match(cv::InputArray &queryDescriptors,
                         cv::InputArray &trainDescriptors,
                         std::vector<cv::DMatch> &matches,
                         cv::InputArray mask)
{
  try {
    mFlannBasedMatcher->match(queryDescriptors, trainDescriptors, matches, mask);
  } catch (cv::Exception &e) {
    msgError("Flann Based Matcher error: %s", e.what());
    return true;
  }
  return false;
}

bool FlannMatcherImp::match(cv::InputArray &queryDescriptors,
                         cv::InputArray &trainDescriptors,
                         std::vector<std::vector<cv::DMatch>> &matches,
                         cv::InputArray mask)
{
  try {
    mFlannBasedMatcher->knnMatch(queryDescriptors, trainDescriptors, matches, 2, mask);
  } catch (cv::Exception &e) {
    msgError("Flann Based Matcher error: %s", e.what());
    return true;
  }
  return false;
}

void FlannMatcherImp::reset()
{
  FlannMatcherProperties::reset();
  update();
}

void FlannMatcherImp::setIndex(FlannMatcher::Index index)
{
  FlannMatcherProperties::setIndex(index);
}

/*----------------------------------------------------------------*/



BruteForceMatcherProperties::BruteForceMatcherProperties()
  : mNormType(BruteForceMatcherProperties::Norm::l2)
{
}

void BruteForceMatcherProperties::reset()
{
  mNormType = BruteForceMatcherProperties::Norm::l2;
}

std::string BruteForceMatcherProperties::name() const
{
  return std::string("Brute Force Matching");
}

BruteForceMatcherProperties::Norm BruteForceMatcherProperties::normType() const
{
  return mNormType;
}

void BruteForceMatcherProperties::setNormType(BruteForceMatcher::Norm normType)
{
  mNormType = normType;
}

/*----------------------------------------------------------------*/


BruteForceMatcherImp::BruteForceMatcherImp()
{
  update();
}

BruteForceMatcherImp::BruteForceMatcherImp(BruteForceMatcher::Norm normType)
{
  BruteForceMatcherProperties::setNormType(normType);
  update();
}

void BruteForceMatcherImp::update()
{
  int norm = cv::NORM_L2;
  BruteForceMatcherProperties::Norm norm_type = BruteForceMatcherProperties::normType();
  if (norm_type == BruteForceMatcherProperties::Norm::l1) {
    norm = cv::NORM_L1;
  } else if (norm_type == BruteForceMatcherProperties::Norm::l2) {
    norm = cv::NORM_L2;
  } else if (norm_type == BruteForceMatcherProperties::Norm::hamming) {
    norm = cv::NORM_HAMMING;
  } else if (norm_type == BruteForceMatcherProperties::Norm::hamming2) {
    norm = cv::NORM_HAMMING2;
  }

  try {
    mBFMatcher = cv::BFMatcher::create(norm);
  } catch (cv::Exception &e) {
    msgError("Brute-force Matcher error: %s", e.what());
  }
}

bool BruteForceMatcherImp::match(cv::InputArray &queryDescriptors,
                                 cv::InputArray &trainDescriptors,
                                 std::vector<cv::DMatch> &matches,
                                 cv::InputArray mask)
{
  try {
    mBFMatcher->match(queryDescriptors, trainDescriptors, matches, mask);
  } catch (cv::Exception &e) {
    msgError("Brute-force Matcher error: %s", e.what());
    return true;
  }
  return false;
}

bool BruteForceMatcherImp::match(cv::InputArray &queryDescriptors,
                                 cv::InputArray &trainDescriptors,
                                 std::vector<std::vector<cv::DMatch>> &matches,
                                 cv::InputArray mask)
{
  try {
    mBFMatcher->knnMatch(queryDescriptors, trainDescriptors, matches, 2, mask);
  } catch (cv::Exception &e) {
    msgError("Brute-force Matcher error: %s", e.what());
    return true;
  }
  return false;
}

void BruteForceMatcherImp::reset()
{
  BruteForceMatcherProperties::reset();
  update();
}

void BruteForceMatcherImp::setNormType(BruteForceMatcher::Norm normType)
{
  BruteForceMatcherProperties::setNormType(normType);
  update();
}

/*----------------------------------------------------------------*/

#ifdef HAVE_OPENCV_CUDAFEATURES2D

BruteForceMatcherCuda::BruteForceMatcherCuda()
{
  update();
}

BruteForceMatcherCuda::BruteForceMatcherCuda(BruteForceMatcher::Norm normType)
{
  BruteForceMatcherProperties::setNormType(normType);
  update();
}

void BruteForceMatcherCuda::update()
{
  int norm = cv::NORM_L2;
  BruteForceMatcherProperties::Norm norm_type = BruteForceMatcherProperties::normType();
  if (norm_type == BruteForceMatcherProperties::Norm::l1) {
    norm = cv::NORM_L1;
  } else if (norm_type == BruteForceMatcherProperties::Norm::l2) {
    norm = cv::NORM_L2;
  } else if (norm_type == BruteForceMatcherProperties::Norm::hamming) {
    norm = cv::NORM_HAMMING;
  }
  ///La implementación de BFMatcher con Cuda no incluye NORM_HAMMING2
  /*else if (norm_type == BruteForceMatcherProperties::Norm::hamming2) {
    norm = cv::NORM_HAMMING2;
  }*/

  try {
    mBFMatcher = cv::cuda::DescriptorMatcher::createBFMatcher(norm);
  } catch (cv::Exception &e) {
    msgError("Brute-force Matcher error: %s", e.what());
  }
}

bool BruteForceMatcherCuda::match(cv::InputArray &queryDescriptors,
                                  cv::InputArray &trainDescriptors,
                                  std::vector<cv::DMatch> &matches,
                                  cv::InputArray mask)
{
  try {
    cv::cuda::GpuMat gQueryDescriptors(queryDescriptors);
    cv::cuda::GpuMat gTrainDescriptors(trainDescriptors);
    cv::cuda::GpuMat gMask(mask);
    mBFMatcher->match(gQueryDescriptors, gTrainDescriptors, matches, gMask);
  } catch (cv::Exception &e) {
    msgError("Brute-force Matcher error: %s", e.what());
    return true;
  }
  return false;
}

bool BruteForceMatcherCuda::match(cv::InputArray &queryDescriptors,
                                  cv::InputArray &trainDescriptors,
                                  std::vector<std::vector<cv::DMatch>> &matches,
                                  cv::InputArray mask)
{
  try {
    cv::cuda::GpuMat gQueryDescriptors(queryDescriptors);
    cv::cuda::GpuMat gTrainDescriptors(trainDescriptors);
    cv::cuda::GpuMat gMask(mask);
    mBFMatcher->knnMatch(gQueryDescriptors, gTrainDescriptors, matches, 2, gMask);
  } catch (cv::Exception &e) {
    msgError("Brute-force Matcher error: %s", e.what());
    return true;
  }
  return false;
}

void BruteForceMatcherCuda::reset()
{
  BruteForceMatcherProperties::reset();
  update();
}

void BruteForceMatcherCuda::setNormType(BruteForceMatcher::Norm normType)
{
  BruteForceMatcherProperties::setNormType(normType);
  update();
}

#endif // HAVE_OPENCV_CUDAFEATURES2D

/*----------------------------------------------------------------*/

RobustMatchingProperties::RobustMatchingProperties()
  : mRatio(0.8),
    mCrossCheck(true),
    mGeometricTest(GeometricTest::fundamental),
    mHomographyComputeMethod(HomographyComputeMethod::ransac),
    mFundamentalComputeMethod(FundamentalComputeMethod::ransac),
    mEssentialComputeMethod(EssentialComputeMethod::ransac),
    mDistance(0.7),
    mConfidence(0.999),
    mMaxIters(2000)
{
}

double RobustMatchingProperties::ratio() const
{
  return mRatio;
}

void RobustMatchingProperties::setRatio(double ratio)
{
  mRatio = ratio;
}

bool RobustMatchingProperties::crossCheck() const
{
  return mCrossCheck;
}

void RobustMatchingProperties::setCrossCheck(bool crossCheck)
{
  mCrossCheck = crossCheck;
}

RobustMatcher::GeometricTest RobustMatchingProperties::geometricTest() const
{
  return mGeometricTest;
}

void RobustMatchingProperties::setGeometricTest(RobustMatcher::GeometricTest geometricTest)
{
  mGeometricTest = geometricTest;
}

RobustMatcher::HomographyComputeMethod RobustMatchingProperties::homographyComputeMethod() const
{
  return mHomographyComputeMethod;
}

void RobustMatchingProperties::setHomographyComputeMethod(RobustMatcher::HomographyComputeMethod computeMethod)
{
  mHomographyComputeMethod = computeMethod;
}

RobustMatcher::FundamentalComputeMethod RobustMatchingProperties::fundamentalComputeMethod() const
{
  return mFundamentalComputeMethod;
}

void RobustMatchingProperties::setFundamentalComputeMethod(RobustMatcher::FundamentalComputeMethod computeMethod)
{
  mFundamentalComputeMethod = computeMethod;
}

RobustMatcher::EssentialComputeMethod RobustMatchingProperties::essentialComputeMethod() const
{
  return mEssentialComputeMethod;
}

void RobustMatchingProperties::setEssentialComputeMethod(RobustMatcher::EssentialComputeMethod computeMethod)
{
  mEssentialComputeMethod = computeMethod;
}

double RobustMatchingProperties::distance() const
{
  return mDistance;
}

void RobustMatchingProperties::setDistance(double distance)
{
  mDistance = distance;
}

double RobustMatchingProperties::confidence() const
{
  return mConfidence;
}

void RobustMatchingProperties::setConfidence(double confidence)
{
  mConfidence = confidence;
}

int RobustMatchingProperties::maxIter() const
{
  return mMaxIters;
}

void RobustMatchingProperties::setMaxIters(int maxIter)
{
  mMaxIters = maxIter;
}

void RobustMatchingProperties::reset()
{
  mRatio = 0.8;
  mCrossCheck = true;
  mGeometricTest = GeometricTest::fundamental;
  mHomographyComputeMethod = HomographyComputeMethod::ransac;
  mFundamentalComputeMethod = FundamentalComputeMethod::ransac;
  mEssentialComputeMethod = EssentialComputeMethod::ransac;
  mDistance = 0.7;
  mConfidence = 0.999;
  mMaxIters = 2000;
}

std::string RobustMatchingProperties::name() const
{
  return std::string("Robust Matcher");
}


/*----------------------------------------------------------------*/


RobustMatchingImp::RobustMatchingImp(const std::shared_ptr<DescriptorMatcher> &descriptorMatcher)
  : mDescriptorMatcher(descriptorMatcher)
{

}

RobustMatchingImp::RobustMatchingImp(const std::shared_ptr<DescriptorMatcher> &descriptorMatcher,
                                               double ratio,
                                               bool crossCheck,
                                               GeometricTest geometricTest,
                                               HomographyComputeMethod homographyComputeMethod,
                                               FundamentalComputeMethod fundamentalComputeMethod,
                                               EssentialComputeMethod essentialComputeMethod,
                                               double distance,
                                               double confidence,
                                               int maxIter)
  : mDescriptorMatcher(descriptorMatcher)
{
  this->setRatio(ratio);
  this->setCrossCheck(crossCheck);
  this->setGeometricTest(geometricTest);
  this->setHomographyComputeMethod(homographyComputeMethod);
  this->setFundamentalComputeMethod(fundamentalComputeMethod);
  this->setEssentialComputeMethod(essentialComputeMethod);
  this->setDistance(distance);
  this->setConfidence(confidence);
  this->setMaxIters(maxIter);
}

void RobustMatchingImp::setDescriptorMatcher(const std::shared_ptr<DescriptorMatcher> &descriptorMatcher)
{
  mDescriptorMatcher = descriptorMatcher;
}

std::vector<cv::DMatch> RobustMatchingImp::geometricFilter(const std::vector<cv::DMatch> &matches, const std::vector<cv::KeyPoint> &keypoints1, const std::vector<cv::KeyPoint> &keypoints2, std::vector<cv::DMatch> *wrongMatches)
{
  std::vector<cv::DMatch> filter_matches;

  // Convert keypoints into Point2f
  size_t nPoints = matches.size();
  std::vector<cv::Point2f> pts1(nPoints);
  std::vector<cv::Point2f> pts2(nPoints);
  for (size_t igm = 0; igm < nPoints; igm++) {
    pts1[igm] = keypoints1[static_cast<size_t>(matches[igm].queryIdx)].pt;
    pts2[igm] = keypoints2[static_cast<size_t>(matches[igm].trainIdx)].pt;
  }

  ///TODO: Sería mejor 3 clases GeometricTest (GeometricTestHomography, GeometricTestFundamental y GeometricTestEssential)
  ///      Se crearia una clase GeometricTestFactory que cree la clase adecuada
  RobustMatcher::GeometricTest geometric_test = RobustMatchingProperties::geometricTest();
  if (geometric_test == RobustMatcher::GeometricTest::essential) {


  } else if (geometric_test == RobustMatcher::GeometricTest::homography){

    filter_matches = filterByHomographyMatrix(matches, pts1, pts2, wrongMatches);

  } else if (geometric_test == RobustMatcher::GeometricTest::fundamental){

    filter_matches = filterByFundamentalMatrix(matches, pts1, pts2, wrongMatches);

  }

  return filter_matches;
}

std::vector<cv::DMatch> RobustMatchingImp::filterByHomographyMatrix(const std::vector<cv::DMatch> &matches, const std::vector<cv::Point2f> &points1, const std::vector<cv::Point2f> &points2, std::vector<cv::DMatch> *wrongMatches)
{
  std::vector<cv::DMatch> filter_matches;

  int hcm = cv::RANSAC;
  RobustMatcher::HomographyComputeMethod homographyComputeMethod = this->homographyComputeMethod();
  if (homographyComputeMethod == RobustMatcher::HomographyComputeMethod::all_points){
    hcm = 0;
  } else if (homographyComputeMethod == RobustMatcher::HomographyComputeMethod::ransac){
    hcm = cv::RANSAC;
  } else if (homographyComputeMethod == RobustMatcher::HomographyComputeMethod::lmeds){
    hcm = cv::LMEDS;
  } else if (homographyComputeMethod == RobustMatcher::HomographyComputeMethod::rho){
    hcm = cv::RHO;
  }

  size_t nPoints = matches.size();
  std::vector<uchar> inliers(nPoints, 0);
  cv::Mat H = cv::findHomography(cv::Mat(points1), cv::Mat(points2), hcm, this->distance(), inliers, this->maxIter(), this->confidence());

  // extract the surviving (inliers) matches
  std::vector<uchar>::const_iterator itIn = inliers.begin();
  std::vector<cv::DMatch>::const_iterator itM = matches.begin();
  // for all matches
  for (; itIn != inliers.end(); ++itIn, ++itM) {

    if (*itIn) {
      filter_matches.push_back(*itM);
    } else {
      if (wrongMatches) wrongMatches->push_back(*itM);
    }
  }


  return filter_matches;
}

std::vector<cv::DMatch> RobustMatchingImp::filterByEssentialMatrix(const std::vector<cv::DMatch> &matches, const std::vector<cv::Point2f> &points1, const std::vector<cv::Point2f> &points2, std::vector<cv::DMatch> *wrongMatches)
{
  std::vector<cv::DMatch> filter_matches;

  int fm = cv::RANSAC;
  RobustMatcher::EssentialComputeMethod essentialComputeMethod = this->essentialComputeMethod();
  if (essentialComputeMethod == RobustMatcher::EssentialComputeMethod::ransac){
    fm = cv::RANSAC;
  } else if (essentialComputeMethod == RobustMatcher::EssentialComputeMethod::lmeds){
    fm = cv::LMEDS;
  }

  // Convert keypoints into Point2f
  size_t nPoints = matches.size();

  TL_TODO("Se necesita la calibración de la cámara (focal y pp) con lo cual no se si es interesante")

//  std::vector<uchar> inliers(nPoints, 0);
//  cv::findEssentialMat(	InputArray 	points1,
//  InputArray 	points2,
//  InputArray 	cameraMatrix,
//  int 	method = RANSAC,
//  double 	prob = 0.999,
//  double 	threshold = 1.0,
//  OutputArray 	mask = noArray()
//  )

      return filter_matches;
}

std::vector<cv::DMatch> RobustMatchingImp::filterByFundamentalMatrix(const std::vector<cv::DMatch> &matches, const std::vector<cv::Point2f> &points1, const std::vector<cv::Point2f> &points2, std::vector<cv::DMatch> *wrongMatches)
{
  int fm_method = cv::FM_RANSAC;
  RobustMatcher::FundamentalComputeMethod fundamentalComputeMethod = this->fundamentalComputeMethod();
  if (fundamentalComputeMethod == RobustMatcher::FundamentalComputeMethod::algorithm_7_point){
    fm_method = cv::FM_7POINT;
  } else if (fundamentalComputeMethod == RobustMatcher::FundamentalComputeMethod::algorithm_8_point){
    fm_method = cv::FM_8POINT;
  } else if (fundamentalComputeMethod == RobustMatcher::FundamentalComputeMethod::ransac){
    fm_method = cv::FM_RANSAC;
  } else if (fundamentalComputeMethod == RobustMatcher::FundamentalComputeMethod::lmeds){
    fm_method = cv::FM_LMEDS;
  }

  // Convert keypoints into Point2f
  size_t nPoints = matches.size();
  std::vector<uchar> inliers(nPoints, 0);
  cv::Mat fundamental = cv::findFundamentalMat(cv::Mat(points1), cv::Mat(points2), inliers,
                                               fm_method, this->distance(), this->confidence());

  std::vector<cv::DMatch> filter_matches;
  // extract the surviving (inliers) matches
  std::vector<uchar>::const_iterator itIn = inliers.begin();
  std::vector<cv::DMatch>::const_iterator itM = matches.begin();
  // for all matches
  for (; itIn != inliers.end(); ++itIn, ++itM) {

    if (*itIn) { // it is a valid match
      filter_matches.push_back(*itM);
    } else {
      if (wrongMatches) wrongMatches->push_back(*itM);
    }
  }

  return filter_matches;
}

std::vector<cv::DMatch> RobustMatchingImp::match(const cv::Mat &queryDescriptor, const cv::Mat &trainDescriptor, std::vector<cv::DMatch> *wrongMatches)
{
  if (this->crossCheck()){
    return this->robustMatch(queryDescriptor, trainDescriptor, wrongMatches);
  } else {
    return this->fastRobustMatch(queryDescriptor, trainDescriptor, wrongMatches);
  }
}

std::vector<cv::DMatch> RobustMatchingImp::robustMatch(const cv::Mat &queryDescriptor, const cv::Mat &trainDescriptor, std::vector<cv::DMatch> *wrongMatches)
{
  std::vector<cv::DMatch> goodMatches;

  std::vector<std::vector<cv::DMatch>> matches12;
  std::vector<std::vector<cv::DMatch>> matches21;

  bool err = mDescriptorMatcher->match(queryDescriptor, trainDescriptor, matches12);
  if (err) return goodMatches;

  err = mDescriptorMatcher->match(trainDescriptor, queryDescriptor, matches21);
  if (err) return goodMatches;

  std::vector<std::vector<cv::DMatch>> wrong_matches12;
  std::vector<std::vector<cv::DMatch>> wrong_matches21;
  std::vector<std::vector<cv::DMatch>> good_matches12 = RobustMatchingImp::ratioTest(matches12, this->ratio(), &wrong_matches12);
  std::vector<std::vector<cv::DMatch>> good_matches21 = RobustMatchingImp::ratioTest(matches21, this->ratio(), &wrong_matches21);

  matches12.clear();
  matches21.clear();

  if (wrongMatches){
    for (auto &wrong_match : wrong_matches12){
      wrongMatches->push_back(wrong_match[0]);
    }
  }

  goodMatches = RobustMatchingImp::crossCheckTest(good_matches12, good_matches21, wrongMatches);

  return goodMatches;
}

std::vector<cv::DMatch> RobustMatchingImp::fastRobustMatch(const cv::Mat &queryDescriptor, const cv::Mat &trainDescriptor, std::vector<cv::DMatch> *wrongMatches)
{
  std::vector<cv::DMatch> goodMatches;

  std::vector<std::vector<cv::DMatch>> matches;
  bool err = mDescriptorMatcher->match(queryDescriptor, trainDescriptor, matches);
  if (err) return goodMatches;

  std::vector<std::vector<cv::DMatch>> ratio_test_wrong_matches;
  std::vector<std::vector<cv::DMatch>> ratio_test_matches = RobustMatchingImp::ratioTest(matches, this->ratio(), &ratio_test_wrong_matches);

  for (auto &match : ratio_test_matches){
    goodMatches.push_back(match[0]);
  }

  if (wrongMatches) {
    for (auto &wrong_match : ratio_test_wrong_matches){
      wrongMatches->push_back(wrong_match[0]);
    }
  }

  return goodMatches;
}

bool RobustMatchingImp::compute(const cv::Mat &queryDescriptor,
                                     const cv::Mat &trainDescriptor,
                                     const std::vector<cv::KeyPoint> &keypoints1,
                                     const std::vector<cv::KeyPoint> &keypoints2,
                                     std::vector<cv::DMatch> *goodMatches,
                                     std::vector<cv::DMatch> *wrongMatches,
                                     const cv::Size &queryImageSize,
                                     const cv::Size &trainImageSize)
{
  try {
    *goodMatches = this->match(queryDescriptor, trainDescriptor, wrongMatches);
    *goodMatches = this->geometricFilter(*goodMatches, keypoints1, keypoints2, wrongMatches);
    return false;
  } catch(std::exception &e){
    msgError(e.what());
    return true;
  }
}


/*----------------------------------------------------------------*/


GmsProperties::GmsProperties()
  : mRotation(true),
    mScale(true),
    mThreshold(6.)
{

}

void GmsProperties::reset()
{
  mRotation = true;
  mScale = true;
  mThreshold = 6.;
}

std::string GmsProperties::name() const
{
  return std::string("GMS");
}

bool GmsProperties::rotation() const
{
  return mRotation;
}

void GmsProperties::setRotation(bool rotation)
{
  mRotation = rotation;
}

bool GmsProperties::scale() const
{
  return mScale;
}

void GmsProperties::setScale(bool scale)
{
  mScale = scale;
}

double GmsProperties::threshold() const
{
  return mThreshold;
}

void GmsProperties::setThreshold(double threshold)
{
  mThreshold = threshold;
}

/*----------------------------------------------------------------*/


GsmImp::GsmImp(const std::shared_ptr<DescriptorMatcher> &descriptorMatcher)
  : GmsProperties(),
    mDescriptorMatcher(descriptorMatcher)
{
}

GsmImp::GsmImp(const std::shared_ptr<DescriptorMatcher> &descriptorMatcher,
                         bool rotation,
                         bool scale,
                         double threshold)
  : GmsProperties(),
    mDescriptorMatcher(descriptorMatcher)
{
  this->setRotation(rotation);
  this->setScale(scale);
  this->setThreshold(threshold);
}

bool GsmImp::compute(const cv::Mat &queryDescriptor,
                     const cv::Mat &trainDescriptor,
                     const std::vector<cv::KeyPoint> &keypoints1,
                     const std::vector<cv::KeyPoint> &keypoints2,
                     std::vector<cv::DMatch> *goodMatches,
                     std::vector<cv::DMatch> *wrongMatches,
                     const cv::Size &queryImageSize,
                     const cv::Size &trainImageSize)
{
#if CV_VERSION_MAJOR >= 4 || (CV_VERSION_MAJOR >= 3 && CV_VERSION_MINOR >= 4 && CV_VERSION_REVISION >= 1 )
  try {

    if (goodMatches == nullptr) return true;

    std::vector<cv::DMatch> matches;
    bool err = mDescriptorMatcher->match(queryDescriptor, trainDescriptor, matches);
    if (err) return true;

    cv::xfeatures2d::matchGMS(queryImageSize, trainImageSize, keypoints1, keypoints2, matches, *goodMatches);

    for (size_t i = 0; i < matches.size(); i++) {
      bool bWrong = true;
      for (size_t j = 0; j < goodMatches->size(); j++) {
        if (matches[i].queryIdx == (*goodMatches)[j].queryIdx &&
            matches[i].trainIdx == (*goodMatches)[j].trainIdx) {
          bWrong = false;
          break;
        }
      }
      if (bWrong) {
        wrongMatches->push_back(matches[i]);
      }
    }

    /// TODO: devolver wrongMatches

  } catch(std::exception &e){
    msgError(e.what());
    return true;
  }
#  else
  Q_UNUSED(queryDescriptor)
  Q_UNUSED(trainDescriptor)
  Q_UNUSED(keypoints1)
  Q_UNUSED(keypoints2)
  Q_UNUSED(goodMatches)
  Q_UNUSED(wrongMatches)
  Q_UNUSED(queryImageSize)
  Q_UNUSED(trainImageSize)
  TL_COMPILER_WARNING("'matchGMS' not supported in OpenCV versions < 3.3.1")
#endif
  return false;
}



/*----------------------------------------------------------------*/



void passPointsWrite(const std::string &fname,
                     const std::vector<std::vector<std::pair<std::string, int>>> &pass_points)
{
  std::ofstream ofs(fname, std::ofstream::trunc);
  if (ofs.is_open()){

    for (size_t i = 0; i < pass_points.size(); i++) {

      ofs << i;

      for (size_t j = 0; j < pass_points[i].size(); j++){
        ofs << ";" << pass_points[i][j].first
          << ";" << pass_points[i][j].second;
      }

      ofs << std::endl;
    }

    ofs.close();
  }
}

void passPointsRead(const std::string &fname, std::vector<std::vector<std::pair<std::string, int>>> &pass_points)
{
  pass_points.resize(0);
  std::ifstream ifs(fname);
  std::string line;
  if (ifs.is_open()) {

    int r = 0;
    while (std::getline(ifs, line)) {

      std::vector<std::string> list;
      boost::split(list, line, boost::is_any_of(";"));
      int size = list.size();
      if (size >= 1){
        if (size == 1 || size % 2 == 0){
          /// deleted point
          pass_points.push_back(std::vector<std::pair<std::string, int>>());
        } else {
          std::vector<std::pair<std::string, int>> pass_point;
          for(int i = 1; i < size; i++){
            std::string idImage = list[i];
            int idx = stringToNumber<int>(list[++i]);
            pass_point.emplace_back(idImage, idx);
          }
          pass_points.push_back(pass_point);
        }
      }

      r++;
    }

    ifs.close();
  }
}



} // namespace tl