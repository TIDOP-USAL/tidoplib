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
  : IFlannMatcher(),
    mIndex(FlannMatcherProperties::Index::kdtree)
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

IFlannMatcher::Index FlannMatcherProperties::index() const
{
  return mIndex;
}

void FlannMatcherProperties::setIndex(IFlannMatcher::Index index)
{
  mIndex = index;
}

/*----------------------------------------------------------------*/


FlannMatcher::FlannMatcher()
  : FlannMatcherProperties(),
    DescriptorMatcher()
{
  update();
}

FlannMatcher::FlannMatcher(Index index)
  : FlannMatcherProperties(),
    DescriptorMatcher()
{
  FlannMatcherProperties::setIndex(index);
  update();
}

FlannMatcher::~FlannMatcher()
{

}

void FlannMatcher::update()
{
  cv::Ptr<cv::flann::IndexParams> indexParams;
  if (FlannMatcherProperties::index() == FlannMatcherProperties::Index::kdtree){
    indexParams = cv::makePtr<cv::flann::KDTreeIndexParams>();
  } else if (FlannMatcherProperties::index() == FlannMatcherProperties::Index::lsh){
    indexParams = cv::makePtr<cv::flann::LshIndexParams>(12, 20, 2);
  }
  mFlannBasedMatcher = cv::Ptr<cv::FlannBasedMatcher>(new cv::FlannBasedMatcher(indexParams));
}

bool FlannMatcher::match(cv::InputArray &queryDescriptors,
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

void FlannMatcher::reset()
{
  FlannMatcherProperties::reset();
  update();
}

void FlannMatcher::setIndex(IFlannMatcher::Index index)
{
}

/*----------------------------------------------------------------*/



BruteForceMatcherProperties::BruteForceMatcherProperties()
  : IBruteForceMatcher(),
    mNormType(BruteForceMatcherProperties::Norm::l2)
{
}

BruteForceMatcherProperties::~BruteForceMatcherProperties()
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

void BruteForceMatcherProperties::setNormType(IBruteForceMatcher::Norm normType)
{
  mNormType = normType;
}

/*----------------------------------------------------------------*/


BruteForceMatcher::BruteForceMatcher()
  : BruteForceMatcherProperties(),
    DescriptorMatcher()
{
  update();
}

BruteForceMatcher::BruteForceMatcher(IBruteForceMatcher::Norm normType)
  : BruteForceMatcherProperties(),
    DescriptorMatcher()
{
  BruteForceMatcherProperties::setNormType(normType);
  update();
}

void BruteForceMatcher::update()
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

  mBFMatcher = cv::BFMatcher::create(norm);
}

bool BruteForceMatcher::match(cv::InputArray &queryDescriptors,
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

void BruteForceMatcher::reset()
{
  BruteForceMatcherProperties::reset();
  update();
}

void BruteForceMatcher::setNormType(IBruteForceMatcher::Norm normType)
{
  BruteForceMatcherProperties::setNormType(normType);
  update();
}

/*----------------------------------------------------------------*/

#ifdef HAVE_CUDA

BruteForceMatcherCuda::BruteForceMatcherCuda()
  : BruteForceMatcherProperties(),
    DescriptorMatcher()
{
  update();
}

BruteForceMatcherCuda::BruteForceMatcherCuda(IBruteForceMatcher::Norm normType)
  : BruteForceMatcherProperties(),
    DescriptorMatcher()
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
  } else if (norm_type == BruteForceMatcherProperties::Norm::hamming2) {
    norm = cv::NORM_HAMMING2;
  }

  mBFMatcher = cv::cuda::DescriptorMatcher::createBFMatcher(norm);
}

bool BruteForceMatcherCuda::match(cv::InputArray &queryDescriptors,
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

void BruteForceMatcherCuda::reset()
{
  BruteForceMatcherProperties::reset();
  update();
}

void BruteForceMatcherCuda::setNormType(IBruteForceMatcher::Norm normType)
{
  BruteForceMatcherProperties::setNormType(normType);
  update();
}

#endif // HAVE_CUDA

/*----------------------------------------------------------------*/

RobustMatcherProperties::RobustMatcherProperties()
  : IRobustMatcherRefinement(),
    mRatio(0.8),
    mCrossCheck(true),
    mGeometricTest(GeometricTest::homography),
    mHomographyComputeMethod(HomographyComputeMethod::ransac),
    mFundamentalComputeMethod(FundamentalComputeMethod::ransac),
    mEssentialComputeMethod(EssentialComputeMethod::ransac),
    mDistance(0.7),
    mConfidence(0.999),
    mMaxIters(2000)
{
}

double RobustMatcherProperties::ratio() const
{
  return mRatio;
}

void RobustMatcherProperties::setRatio(double ratio)
{
  mRatio = ratio;
}

bool RobustMatcherProperties::crossCheck() const
{
  return mCrossCheck;
}

void RobustMatcherProperties::setCrossCheck(bool crossCheck)
{
  mCrossCheck = crossCheck;
}

IRobustMatcherRefinement::GeometricTest RobustMatcherProperties::geometricTest() const
{
  return mGeometricTest;
}

void RobustMatcherProperties::setGeometricTest(IRobustMatcherRefinement::GeometricTest geometricTest)
{
  mGeometricTest = geometricTest;
}

IRobustMatcherRefinement::HomographyComputeMethod RobustMatcherProperties::homographyComputeMethod() const
{
  return mHomographyComputeMethod;
}

void RobustMatcherProperties::setHomographyComputeMethod(IRobustMatcherRefinement::HomographyComputeMethod computeMethod)
{
  mHomographyComputeMethod = computeMethod;
}

IRobustMatcherRefinement::FundamentalComputeMethod RobustMatcherProperties::fundamentalComputeMethod() const
{
  return mFundamentalComputeMethod;
}

void RobustMatcherProperties::setFundamentalComputeMethod(IRobustMatcherRefinement::FundamentalComputeMethod computeMethod)
{
  mFundamentalComputeMethod = computeMethod;
}

IRobustMatcherRefinement::EssentialComputeMethod RobustMatcherProperties::essentialComputeMethod() const
{
  return mEssentialComputeMethod;
}

void RobustMatcherProperties::setEssentialComputeMethod(IRobustMatcherRefinement::EssentialComputeMethod computeMethod)
{
  mEssentialComputeMethod = computeMethod;
}

double RobustMatcherProperties::distance() const
{
  return mDistance;
}

void RobustMatcherProperties::setDistance(double distance)
{
  mDistance = distance;
}

double RobustMatcherProperties::confidence() const
{
  return mConfidence;
}

void RobustMatcherProperties::setConfidence(double confidence)
{
  mConfidence = confidence;
}

int RobustMatcherProperties::maxIter() const
{
  return mMaxIters;
}

void RobustMatcherProperties::setMaxIters(int maxIter)
{
  mMaxIters = maxIter;
}

void RobustMatcherProperties::reset()
{
  mRatio = 0.8;
  mCrossCheck = true;
  mGeometricTest = GeometricTest::homography;
  mHomographyComputeMethod = HomographyComputeMethod::ransac;
  mFundamentalComputeMethod = FundamentalComputeMethod::ransac;
  mEssentialComputeMethod = EssentialComputeMethod::ransac;
  mDistance = 0.7;
  mConfidence = 0.999;
  mMaxIters = 2000;
}


/*----------------------------------------------------------------*/


RobustMatching::RobustMatching(const std::shared_ptr<DescriptorMatcher> &matcher)
  : IRobustMatching(),
    RobustMatcherProperties(),
    mMatcher(matcher)
{
}

RobustMatching::RobustMatching(const std::shared_ptr<DescriptorMatcher> &matcher,
                               double ratio,
                               bool crossCheck,
                               GeometricTest geometricTest,
                               HomographyComputeMethod homographyComputeMethod,
                               FundamentalComputeMethod fundamentalComputeMethod,
                               EssentialComputeMethod essentialComputeMethod,
                               double distance,
                               double confidence,
                               int maxIter)
  : IRobustMatching(),
    RobustMatcherProperties(),
    mMatcher(matcher)
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

RobustMatching::~RobustMatching()
{
}

void RobustMatching::setDescriptorMatcher(const std::shared_ptr<DescriptorMatcher> &matcher)
{
  mMatcher = matcher;
}

std::vector<cv::DMatch> RobustMatching::match(const cv::Mat &queryDescriptor,
                                              const cv::Mat &trainDescriptor,
                                              std::vector<cv::DMatch> *wrongMatches)
{
  if (this->crossCheck()){
    return this->robustMatch(queryDescriptor, trainDescriptor, wrongMatches);
  } else {
    return this->fastRobustMatch(queryDescriptor, trainDescriptor, wrongMatches);
  }
}

std::vector<cv::DMatch> RobustMatching::geometricFilter(const std::vector<cv::DMatch> &matches,
                                                        const std::vector<cv::KeyPoint> &keypoints1,
                                                        const std::vector<cv::KeyPoint> &keypoints2,
                                                        std::vector<cv::DMatch> *wrongMatches)
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

  IRobustMatcherRefinement::GeometricTest geometric_test = RobustMatcherProperties::geometricTest();
  if (geometric_test == IRobustMatcherRefinement::GeometricTest::essential) {


  } else if (geometric_test == IRobustMatcherRefinement::GeometricTest::homography){

    filter_matches = filterByHomographyMatrix(matches, pts1, pts2, wrongMatches);

  } else if (geometric_test == IRobustMatcherRefinement::GeometricTest::fundamental){

    filter_matches = filterByFundamentalMatrix(matches, pts1, pts2, wrongMatches);

  }

  return filter_matches;
}

std::vector<cv::DMatch> RobustMatching::filterByHomographyMatrix(const std::vector<cv::DMatch> &matches,
                                                                 const std::vector<cv::Point2f> &points1,
                                                                 const std::vector<cv::Point2f> &points2,
                                                                 std::vector<cv::DMatch> *wrongMatches)
{
  std::vector<cv::DMatch> filter_matches;

  int hcm = cv::RANSAC;
  IRobustMatcherRefinement::HomographyComputeMethod homographyComputeMethod = this->homographyComputeMethod();
  if (homographyComputeMethod == IRobustMatcherRefinement::HomographyComputeMethod::all_points){
    hcm = 0;
  } else if (homographyComputeMethod == IRobustMatcherRefinement::HomographyComputeMethod::ransac){
    hcm = cv::RANSAC;
  } else if (homographyComputeMethod == IRobustMatcherRefinement::HomographyComputeMethod::lmeds){
    hcm = cv::LMEDS;
  } else if (homographyComputeMethod == IRobustMatcherRefinement::HomographyComputeMethod::rho){
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

std::vector<cv::DMatch> RobustMatching::filterByEssentialMatrix(const std::vector<cv::DMatch> &matches,
                                                                const std::vector<cv::Point2f> &points1,
                                                                const std::vector<cv::Point2f> &points2,
                                                                std::vector<cv::DMatch> *wrongMatches)
{
  std::vector<cv::DMatch> filter_matches;

  int fm = cv::RANSAC;
  IRobustMatcherRefinement::EssentialComputeMethod essentialComputeMethod = this->essentialComputeMethod();
  if (essentialComputeMethod == IRobustMatcherRefinement::EssentialComputeMethod::ransac){
    fm = cv::RANSAC;
  } else if (essentialComputeMethod == IRobustMatcherRefinement::EssentialComputeMethod::lmeds){
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

std::vector<cv::DMatch> RobustMatching::filterByFundamentalMatrix(const std::vector<cv::DMatch> &matches,
                                                                  const std::vector<cv::Point2f> &points1,
                                                                  const std::vector<cv::Point2f> &points2,
                                                                  std::vector<cv::DMatch> *wrongMatches)
{
  int fm_method = cv::FM_RANSAC;
  IRobustMatcherRefinement::FundamentalComputeMethod fundamentalComputeMethod = this->fundamentalComputeMethod();
  if (fundamentalComputeMethod == IRobustMatcherRefinement::FundamentalComputeMethod::algorithm_7_point){
    fm_method = cv::FM_7POINT;
  } else if (fundamentalComputeMethod == IRobustMatcherRefinement::FundamentalComputeMethod::algorithm_8_point){
    fm_method = cv::FM_8POINT;
  } else if (fundamentalComputeMethod == IRobustMatcherRefinement::FundamentalComputeMethod::ransac){
    fm_method = cv::FM_RANSAC;
  } else if (fundamentalComputeMethod == IRobustMatcherRefinement::FundamentalComputeMethod::lmeds){
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

std::vector<cv::DMatch> RobustMatching::robustMatch(const cv::Mat &queryDescriptor,
                                                    const cv::Mat &trainDescriptor,
                                                    std::vector<cv::DMatch> *wrongMatches)
{

  std::vector<cv::DMatch> goodMatches;

  std::vector<std::vector<cv::DMatch>> matches12;
  std::vector<std::vector<cv::DMatch>> matches21;

  bool err = mMatcher->match(queryDescriptor, trainDescriptor, matches12);
  if (err) return goodMatches;

  err = mMatcher->match(trainDescriptor, queryDescriptor, matches21);
  if (err) return goodMatches;

  std::vector<std::vector<cv::DMatch>> wrong_matches12;
  std::vector<std::vector<cv::DMatch>> wrong_matches21;
  std::vector<std::vector<cv::DMatch>> good_matches12 = this->ratioTest(matches12, this->ratio(), &wrong_matches12);
  std::vector<std::vector<cv::DMatch>> good_matches21 = this->ratioTest(matches21, this->ratio(), &wrong_matches21);

  matches12.clear();
  matches21.clear();

  if (wrongMatches){
    for (size_t i = 0; i < wrong_matches12.size(); i++){
      wrongMatches->push_back(wrong_matches12[i][0]);
    }
  }


  goodMatches = this->crossCheckTest(good_matches12, good_matches21, wrongMatches);

  return goodMatches;
}

std::vector<cv::DMatch> RobustMatching::fastRobustMatch(const cv::Mat &queryDescriptor,
                                                        const cv::Mat &trainDescriptor,
                                                        std::vector<cv::DMatch> *wrongMatches)
{
  std::vector<cv::DMatch> goodMatches;

  std::vector<std::vector<cv::DMatch>> matches;
  bool err = mMatcher->match(queryDescriptor, trainDescriptor, matches);
  if (err) return goodMatches;

  std::vector<std::vector<cv::DMatch>> ratio_test_wrong_matches;
  std::vector<std::vector<cv::DMatch>> ratio_test_matches = this->ratioTest(matches, this->ratio(), &ratio_test_wrong_matches);

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



/*----------------------------------------------------------------*/


void matchesWrite(const std::string &fname, const std::vector<cv::DMatch> &matches, const std::vector<cv::DMatch> &wrongMatches)
{

  std::string ext = fs::path(fname).extension().string();
  int flags = 0;
  if (ext.compare("xml") == 0) {
    flags = cv::FileStorage::WRITE | cv::FileStorage::FORMAT_XML;
  } else if (ext.compare("yml") == 0) {
    flags = cv::FileStorage::WRITE | cv::FileStorage::FORMAT_YAML;
  } else if (ext.compare("bin") == 0) {

  } else {
    msgError("file extension '%s' not valid", ext.c_str());
    return;
  }

  if (ext.compare("bin") == 0) {
    FILE* fp = std::fopen(fname.c_str(), "wb");
    if (fp) {
      // Cabecera
      int size = static_cast<int>(matches.size());
      std::fwrite("TIDOPLIB-Matching-#01", sizeof("TIDOPLIB-Matching-#01"), 1, fp);
      std::fwrite(&size, sizeof(int32_t), 1, fp);
      char extraHead[100]; // Reserva de espacio para futuros usos
      std::fwrite(&extraHead, sizeof(char), 100, fp);
      //Cuerpo
      for (size_t i = 0; i < matches.size(); i++) {
        std::fwrite(&matches[i].queryIdx, sizeof(int32_t), 1, fp);
        std::fwrite(&matches[i].trainIdx, sizeof(int32_t), 1, fp);
        std::fwrite(&matches[i].imgIdx, sizeof(int32_t), 1, fp);
        std::fwrite(&matches[i].distance, sizeof(float), 1, fp);
      }
      std::fclose(fp);
    } else {
      //msgError("No pudo escribir archivo %s", matches_file);
    }
  } else {
    cv::FileStorage fs(fname, flags);
    if (fs.isOpened()) {
      if (!matches.empty()) write(fs, "matches", matches);
      if (!wrongMatches.empty()) write(fs, "wrong_matches", wrongMatches);
      fs.release();
    } else {
      //msgError("No pudo escribir archivo %s", matches_file);
    }
  }
}

void matchesRead(const std::string &fname, std::vector<cv::DMatch> *matches, std::vector<cv::DMatch> *wrongMatches)
{

  std::string ext = fs::path(fname).extension().string();
  if (ext.empty() == false) {
    if (ext.compare("bin") == 0) {
      if (FILE* fp = std::fopen(fname.c_str(), "rb")) {
        //cabecera
        char h[22];
        int size;
        char extraHead[100];
        std::fread(h, sizeof(char), 22, fp);
        std::fread(&size, sizeof(int32_t), 1, fp);
        std::fread(&extraHead, sizeof(char), 100, fp);
        //Cuerpo
        if (matches){
          matches->resize(static_cast<size_t>(size));
          for (auto &match : *matches) {
            std::fread(&match.queryIdx, sizeof(int32_t), 1, fp);
            std::fread(&match.trainIdx, sizeof(int32_t), 1, fp);
            std::fread(&match.imgIdx, sizeof(int32_t), 1, fp);
            std::fread(&match.distance, sizeof(float), 1, fp);
          }
        }

        /// TODO: wrong matches
        /// ....
        std::fclose(fp);
      } else
        msgError("No pudo leer archivo %s", fname.c_str());
    } else if (ext.compare("xml") == 0 || ext.compare("yml") == 0) {

      cv::FileStorage fs(fname.c_str(), cv::FileStorage::READ);
      if (fs.isOpened()) {
        if (matches) {
          matches->resize(0);
          fs["matches"] >> *matches;
        }
        if (wrongMatches) {
          wrongMatches->resize(0);
          fs["wrong_matches"] >> *wrongMatches;
        }
        fs.release();
      } else {
        msgError("No pudo leer archivo %s", fname.c_str());
      }
    }
  } else msgError("Fichero no valido: %s", fname.c_str());
}


} // namespace tl