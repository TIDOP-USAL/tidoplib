#include "agast.h"

#include "tidop/core/messages.h"


namespace tl
{


AgastProperties::AgastProperties()
  : mThreshold(10),
    mNonmaxSuppression(true),
    mDetectorType("OAST_9_16")
{
}

AgastProperties::AgastProperties(const AgastProperties &agast)
  : Agast(agast),
    mThreshold(agast.mThreshold),
    mNonmaxSuppression(agast.nonmaxSuppression()),
    mDetectorType(agast.mDetectorType)
{
}

AgastProperties::AgastProperties(AgastProperties &&agast) noexcept
  : mThreshold(agast.mThreshold),
    mNonmaxSuppression(agast.mNonmaxSuppression),
    mDetectorType(std::move(agast.mDetectorType))
{
}

AgastProperties &AgastProperties::operator =(const AgastProperties &agast)
{
  if (this != &agast) {
    this->mThreshold = agast.mThreshold;
    this->mNonmaxSuppression = agast.mNonmaxSuppression;
    this->mDetectorType = agast.mDetectorType;
  }
  return *this;
}

AgastProperties &AgastProperties::operator =(AgastProperties &&agast) noexcept
{
  if (this != &agast) {
    this->mThreshold = agast.mThreshold;
    this->mNonmaxSuppression = agast.mNonmaxSuppression;
    this->mDetectorType = std::move(agast.mDetectorType);
  }
  return *this;
}

int AgastProperties::threshold() const
{
  return mThreshold;
}

bool AgastProperties::nonmaxSuppression() const
{
  return mNonmaxSuppression;
}

std::string AgastProperties::detectorType() const
{
  return mDetectorType;
}

void AgastProperties::setThreshold(int threshold)
{
  mThreshold = threshold;
}

void AgastProperties::setNonmaxSuppression(bool nonmaxSuppression)
{
  mNonmaxSuppression = nonmaxSuppression;
}

void AgastProperties::setDetectorType(const std::string &detectorType)
{
  if (detectorType.compare("AGAST_5_8") == 0 ||
      detectorType.compare("AGAST_7_12d") == 0 ||
      detectorType.compare("AGAST_7_12s") == 0 ||
      detectorType.compare("OAST_9_16") == 0) {
    mDetectorType = detectorType;
  }
}

void AgastProperties::reset()
{
  mThreshold = 10;
  mNonmaxSuppression = true;
  mDetectorType = "OAST_9_16";
}

std::string AgastProperties::name() const
{
  return std::string("AGAST");
}


/*----------------------------------------------------------------*/


AgastDetector::AgastDetector()
{
  this->initAgastFromProperties();
}

AgastDetector::AgastDetector(const AgastDetector &agastDetector)
  : AgastProperties(agastDetector)
{
  this->initAgastFromProperties();
}

AgastDetector::AgastDetector(AgastDetector &&agastDetector) noexcept
  : AgastProperties(std::forward<AgastProperties>(agastDetector))
{
  this->initAgastFromProperties();
}

AgastDetector::AgastDetector(int threshold, 
                             bool nonmaxSuppression, 
                             const std::string &detectorType)
  : mAgast(cv::AgastFeatureDetector::create())
{
  setThreshold(threshold);
  setNonmaxSuppression(nonmaxSuppression);
  setDetectorType(detectorType);
}

AgastDetector &AgastDetector::operator =(const AgastDetector &agastDetector)
{
  if (this != &agastDetector){
    AgastProperties::operator=(agastDetector);
    this->initAgastFromProperties();
  }
  return *this;
}

AgastDetector &AgastDetector::operator =(AgastDetector &&agastDetector) noexcept
{
  if (this != &agastDetector){
    AgastProperties::operator=(std::forward<AgastProperties>(agastDetector));
    this->initAgastFromProperties();
  }
  return *this;
}

#if CV_VERSION_MAJOR >= 4
cv::AgastFeatureDetector::DetectorType AgastDetector::convertDetectorType(const std::string &detectorType)
{
  cv::AgastFeatureDetector::DetectorType detector_type = cv::AgastFeatureDetector::DetectorType::OAST_9_16;
  if (detectorType.compare("AGAST_5_8") == 0 ) {
    detector_type = cv::AgastFeatureDetector::AGAST_5_8;
  } else if (detectorType.compare("AGAST_7_12d") == 0){
    detector_type = cv::AgastFeatureDetector::AGAST_7_12d;
  } else if (detectorType.compare("AGAST_7_12s") == 0){
    detector_type = cv::AgastFeatureDetector::AGAST_7_12s;
  }
  return detector_type;
}
#else
int AgastDetector::convertDetectorType(const std::string &detectorType)
{
  int detector_type = cv::AgastFeatureDetector::OAST_9_16;
  if (detectorType.compare("AGAST_5_8") == 0 ) {
    detector_type = cv::AgastFeatureDetector::AGAST_5_8;
  } else if (detectorType.compare("AGAST_7_12d") == 0){
    detector_type = cv::AgastFeatureDetector::AGAST_7_12d;
  } else if (detectorType.compare("AGAST_7_12s") == 0){
    detector_type = cv::AgastFeatureDetector::AGAST_7_12s;
  }
  return detector_type;
}
#endif

void AgastDetector::initAgastFromProperties()
{
  mAgast = cv::AgastFeatureDetector::create(AgastProperties::threshold(),
                                            AgastProperties::nonmaxSuppression(),
                                            convertDetectorType(AgastProperties::detectorType()));
}

bool AgastDetector::detect(const cv::Mat &img,
                           std::vector<cv::KeyPoint> &keyPoints,
                           cv::InputArray &mask)
{

  try {
    mAgast->detect(img, keyPoints, mask);
  } catch (cv::Exception &e) {
    msgError("AGAST Detector error: %s", e.what());
    return true;
  }

  return false;
}

void AgastDetector::setThreshold(int threshold)
{
  AgastProperties::setThreshold(threshold);
  mAgast->setThreshold(threshold);
}

void AgastDetector::setNonmaxSuppression(bool nonmaxSuppression)
{
  AgastProperties::setNonmaxSuppression(nonmaxSuppression);
  mAgast->setNonmaxSuppression(nonmaxSuppression);
}

void AgastDetector::setDetectorType(const std::string &detectorType)
{
  AgastProperties::setDetectorType(detectorType);
  mAgast->setType(convertDetectorType(AgastProperties::detectorType()));
}

void AgastDetector::reset()
{
  AgastProperties::reset();

  mAgast->setThreshold(AgastProperties::threshold());
  mAgast->setNonmaxSuppression(AgastProperties::nonmaxSuppression());
  mAgast->setType(convertDetectorType(AgastProperties::detectorType()));
}


} // namespace tl
