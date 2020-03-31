#include "fast.h"

#include "tidop/core/messages.h"


namespace tl
{


FastProperties::FastProperties()
  : IFast(),
    mThreshold(10),
    mNonmaxSuppression(true),
    mDetectorType("TYPE_9_16")
{

}

FastProperties::~FastProperties()
{
}

int FastProperties::threshold() const
{
  return mThreshold;
}

bool FastProperties::nonmaxSuppression() const
{
  return mNonmaxSuppression;
}

std::string FastProperties::detectorType() const
{
  return mDetectorType;
}

void FastProperties::setThreshold(int threshold)
{
  mThreshold = threshold;
}

void FastProperties::setNonmaxSuppression(bool nonmaxSuppression)
{
  mNonmaxSuppression = nonmaxSuppression;
}

void FastProperties::setDetectorType(std::string detectorType)
{
  mDetectorType = detectorType;
}

void FastProperties::reset()
{
  mThreshold = 10;
  mNonmaxSuppression = true;
  mDetectorType = "TYPE_9_16";
}

std::string FastProperties::name() const
{
  return std::string("FAST");
}


/*----------------------------------------------------------------*/


FastDetector::FastDetector()
  : FastProperties(),
    KeypointDetector(),
    mFast(cv::FastFeatureDetector::create())
{
  mFast->setThreshold(FastProperties::threshold());
  mFast->setNonmaxSuppression(FastProperties::nonmaxSuppression());
  
  
#if CV_VERSION_MAJOR >= 4
  cv::FastFeatureDetector::DetectorType type = cv::FastFeatureDetector::DetectorType::TYPE_9_16;
#else
  int type = cv::FastFeatureDetector::TYPE_9_16;
#endif

  std::string detectorType = FastProperties::detectorType();
  if (detectorType.compare("TYPE_5_8") == 0){
    type = cv::FastFeatureDetector::TYPE_5_8;
  } else if (detectorType.compare("TYPE_7_12") == 0) {
    type = cv::FastFeatureDetector::TYPE_7_12;
  } else if (detectorType.compare("TYPE_9_16") == 0) {
    type = cv::FastFeatureDetector::TYPE_9_16;
  }
  mFast->setType(type);
}


FastDetector::FastDetector(int threshold, bool nonmaxSuppression, const std::string &detectorType)
  : FastProperties(),
    KeypointDetector(),
    mFast(cv::FastFeatureDetector::create())
{
  setThreshold(threshold);
  setNonmaxSuppression(nonmaxSuppression);
  setDetectorType(detectorType);
}

FastDetector::~FastDetector()
{

}

bool FastDetector::detect(const cv::Mat &img,
                          std::vector<cv::KeyPoint> &keyPoints,
                          cv::InputArray &mask)
{

  try {
    mFast->detect(img, keyPoints, mask);
  } catch (cv::Exception &e) {
    msgError("FAST Detector error: %s", e.what());
    return true;
  }

  return false;
}

void FastDetector::setThreshold(int threshold)
{
  FastProperties::setThreshold(threshold);
  mFast->setThreshold(threshold);
}

void FastDetector::setNonmaxSuppression(bool nonmaxSuppression)
{
  FastProperties::setNonmaxSuppression(nonmaxSuppression);
  mFast->setNonmaxSuppression(nonmaxSuppression);
}

void FastDetector::setDetectorType(std::string detectorType)
{
  FastProperties::setDetectorType(detectorType);

#if CV_VERSION_MAJOR >= 4
  cv::FastFeatureDetector::DetectorType type = cv::FastFeatureDetector::DetectorType::TYPE_9_16;
#else
  int type = cv::FastFeatureDetector::TYPE_9_16;
#endif

  if (detectorType.compare("TYPE_5_8") == 0){
    type = cv::FastFeatureDetector::TYPE_5_8;
  } else if (detectorType.compare("TYPE_7_12") == 0) {
    type = cv::FastFeatureDetector::TYPE_7_12;
  } else if (detectorType.compare("TYPE_9_16") == 0) {
    type = cv::FastFeatureDetector::TYPE_9_16;
  }
  mFast->setType(type);
}

void FastDetector::reset()
{
  FastProperties::reset();

  mFast->setThreshold(FastProperties::threshold());
  mFast->setNonmaxSuppression(FastProperties::nonmaxSuppression());
  setDetectorType(FastProperties::detectorType());
}


} // namespace tl
