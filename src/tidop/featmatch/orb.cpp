#include "orb.h"

#include "tidop/core/messages.h"


namespace tl
{


OrbProperties::OrbProperties()
  : IOrb(),
    mFeaturesNumber(5000),
    mScaleFactor(1.2),
    mLevelsNumber(8),
    mEdgeThreshold(31),
    mWTA_K(2),
    mScoreType("Harris"),
    mPatchSize(31),
    mFastThreshold(20)
{}

OrbProperties::~OrbProperties()
{

}

int OrbProperties::featuresNumber() const
{
  return mFeaturesNumber;
}

double OrbProperties::scaleFactor() const
{
  return mScaleFactor;
}

int OrbProperties::levelsNumber() const
{
  return mLevelsNumber;
}

int OrbProperties::edgeThreshold() const
{
  return mEdgeThreshold;
}

int OrbProperties::wta_k() const
{
  return mWTA_K;
}

std::string OrbProperties::scoreType() const
{
  return mScoreType;
}

int OrbProperties::patchSize() const
{
  return mPatchSize;
}

int OrbProperties::fastThreshold() const
{
  return mFastThreshold;
}

void OrbProperties::setScaleFactor(double scaleFactor)
{
  mScaleFactor = scaleFactor;
}

void OrbProperties::setFeaturesNumber(int featuresNumber)
{
  mFeaturesNumber = featuresNumber;
}

void OrbProperties::setLevelsNumber(int levelsNumber)
{
  mLevelsNumber = levelsNumber;
}

void OrbProperties::setEdgeThreshold(int edgeThreshold)
{
  mEdgeThreshold = edgeThreshold;
}

void OrbProperties::setWTA_K(int WTA_K)
{
  mWTA_K = WTA_K;
}

void OrbProperties::setScoreType(const std::string &scoreType)
{
  mScoreType = scoreType;
}

void OrbProperties::setPatchSize(int patchSize)
{
  mPatchSize = patchSize;
}

void OrbProperties::setFastThreshold(int fastThreshold)
{
  mFastThreshold = fastThreshold;
}

void OrbProperties::reset()
{
  mFeaturesNumber = 5000;
  mScaleFactor = 1.2;
  mLevelsNumber = 8;
  mEdgeThreshold = 31;
  mWTA_K = 2;
  mScoreType = "Harris";
  mPatchSize = 31;
  mFastThreshold = 20;
}

std::string OrbProperties::name() const
{
  return std::string("ORB");
}


/*----------------------------------------------------------------*/


OrbDetectorDescriptor::OrbDetectorDescriptor()
  : OrbProperties(),
    KeypointDetector(),
    DescriptorExtractor(),
    mOrb(cv::ORB::create())
{
  mOrb->setMaxFeatures(OrbProperties::featuresNumber());
  mOrb->setScaleFactor(OrbProperties::scaleFactor());
  mOrb->setNLevels(OrbProperties::levelsNumber());
  mOrb->setEdgeThreshold(OrbProperties::edgeThreshold());
  mOrb->setWTA_K(OrbProperties::wta_k());

#if CV_VERSION_MAJOR >= 4
  cv::ORB::ScoreType score = cv::ORB::HARRIS_SCORE;
#else
  int score = cv::ORB::HARRIS_SCORE;
#endif
  if (OrbProperties::scoreType().compare("Harris") == 0){
    score = cv::ORB::HARRIS_SCORE;
  } else if (OrbProperties::scoreType().compare("FAST") == 0){
    score = cv::ORB::FAST_SCORE;
  }
  mOrb->setScoreType(score);
  mOrb->setPatchSize(OrbProperties::patchSize());
  mOrb->setFastThreshold(OrbProperties::fastThreshold());
}

OrbDetectorDescriptor::OrbDetectorDescriptor(int featuresNumber,
                                             double scaleFactor,
                                             int levelsNumber,
                                             int edgeThreshold,
                                             int wta_k,
                                             std::string scoreType,
                                             int patchSize,
                                             int fastThreshold)
  : OrbProperties(),
    KeypointDetector(),
    DescriptorExtractor(),
    mOrb(cv::ORB::create())
{
  setFeaturesNumber(featuresNumber);
  setScaleFactor(scaleFactor);
  setLevelsNumber(levelsNumber);
  setEdgeThreshold(edgeThreshold);
  setWTA_K(wta_k);
  setScoreType(scoreType);
  setPatchSize(patchSize);
  setFastThreshold(fastThreshold);
}

OrbDetectorDescriptor::~OrbDetectorDescriptor()
{

}

bool OrbDetectorDescriptor::detect(const cv::Mat &img,
                                   std::vector<cv::KeyPoint> &keyPoints,
                                   cv::InputArray &mask)
{

  try {
    mOrb->detect(img, keyPoints, mask);
  } catch (cv::Exception &e) {
    msgError("ORB Detector error: %s", e.what());
    return true;
  }

  return false;
}

bool OrbDetectorDescriptor::extract(const cv::Mat &img,
                                    std::vector<cv::KeyPoint> &keyPoints,
                                    cv::Mat &descriptors)
{

  try {
    mOrb->compute(img, keyPoints, descriptors);
  } catch (cv::Exception &e) {
    msgError("ORB Descriptor error: %s", e.what());
    return true;
  }

  return false;
}

void OrbDetectorDescriptor::setFeaturesNumber(int featuresNumber)
{
  OrbProperties::setFeaturesNumber(featuresNumber);
  mOrb->setMaxFeatures(featuresNumber);
}

void OrbDetectorDescriptor::setScaleFactor(double scaleFactor)
{
  OrbProperties::setScaleFactor(scaleFactor);
  mOrb->setScaleFactor(scaleFactor);
}

void OrbDetectorDescriptor::setLevelsNumber(int levelsNumber)
{
  OrbProperties::setLevelsNumber(levelsNumber);
  mOrb->setNLevels(levelsNumber);
}

void OrbDetectorDescriptor::setEdgeThreshold(int edgeThreshold)
{
  OrbProperties::setEdgeThreshold(edgeThreshold);
  mOrb->setEdgeThreshold(edgeThreshold);
}

void OrbDetectorDescriptor::setWTA_K(int WTA_K)
{
  OrbProperties::setWTA_K(WTA_K);
  mOrb->setWTA_K(WTA_K);
}

void OrbDetectorDescriptor::setScoreType(const std::string &scoreType)
{
  OrbProperties::setScoreType(scoreType);
#if CV_VERSION_MAJOR >= 4
  cv::ORB::ScoreType score = cv::ORB::ScoreType::HARRIS_SCORE;
#else
  int score = cv::ORB::HARRIS_SCORE;
#endif
  if (scoreType.compare("Harris") == 0){
    score = cv::ORB::HARRIS_SCORE;
  } else if (scoreType.compare("FAST") == 0){
    score = cv::ORB::FAST_SCORE;
  }
  mOrb->setScoreType(score);
}

void OrbDetectorDescriptor::setPatchSize(int patchSize)
{
  OrbProperties::setPatchSize(patchSize);
  mOrb->setPatchSize(patchSize);
}

void OrbDetectorDescriptor::setFastThreshold(int fastThreshold)
{
  OrbProperties::setFastThreshold(fastThreshold);
  mOrb->setFastThreshold(fastThreshold);
}

void OrbDetectorDescriptor::reset()
{
  OrbProperties::reset();
  mOrb->setMaxFeatures(OrbProperties::featuresNumber());
  mOrb->setScaleFactor(OrbProperties::scaleFactor());
  mOrb->setNLevels(OrbProperties::levelsNumber());
  mOrb->setEdgeThreshold(OrbProperties::edgeThreshold());
  mOrb->setWTA_K(OrbProperties::wta_k());

#if CV_VERSION_MAJOR >= 4
  cv::ORB::ScoreType score = cv::ORB::ScoreType::HARRIS_SCORE;
#else
  int score = cv::ORB::HARRIS_SCORE;
#endif

  if (OrbProperties::scoreType().compare("Harris") == 0){
    score = cv::ORB::HARRIS_SCORE;
  } else if (OrbProperties::scoreType().compare("FAST") == 0){
    score = cv::ORB::FAST_SCORE;
  }
  mOrb->setScoreType(score);
  mOrb->setPatchSize(OrbProperties::patchSize());
  mOrb->setFastThreshold(OrbProperties::fastThreshold());
}


/*----------------------------------------------------------------*/


#ifdef HAVE_CUDA

OrbCudaDetectorDescriptor::OrbCudaDetectorDescriptor()
  : OrbProperties(),
    KeypointDetector(),
    DescriptorExtractor()
{
  update();
}

OrbCudaDetectorDescriptor::OrbCudaDetectorDescriptor(int featuresNumber,
                                                     double scaleFactor,
                                                     int levelsNumber,
                                                     int edgeThreshold,
                                                     int wta_k,
                                                     std::string scoreType,
                                                     int patchSize,
                                                     int fastThreshold)
  : OrbProperties(),
    KeypointDetector(),
    DescriptorExtractor()
{
  setFeaturesNumber(featuresNumber);
  setScaleFactor(scaleFactor);
  setLevelsNumber(levelsNumber);
  setEdgeThreshold(edgeThreshold);
  setWTA_K(wta_k);
  setScoreType(scoreType);
  setPatchSize(patchSize);
  setFastThreshold(fastThreshold);
}

OrbCudaDetectorDescriptor::~OrbCudaDetectorDescriptor()
{

}

void OrbCudaDetectorDescriptor::update()
{

#if CV_VERSION_MAJOR >= 4
  cv::ORB::ScoreType score = cv::ORB::HARRIS_SCORE;
#else
  int score = cv::ORB::HARRIS_SCORE;
#endif
  if (OrbProperties::scoreType().compare("Harris") == 0){
    score = cv::ORB::HARRIS_SCORE;
  } else if (OrbProperties::scoreType().compare("FAST") == 0){
    score = cv::ORB::FAST_SCORE;
  }

  mOrb = cv::cuda::ORB::create(OrbProperties::featuresNumber(),
                               OrbProperties::scaleFactor(),
                               OrbProperties::levelsNumber(),
                               OrbProperties::edgeThreshold(),
                               0,
                               OrbProperties::wta_k(),
                               score,
                               OrbProperties::patchSize(),
                               OrbProperties::fastThreshold());
}

bool OrbCudaDetectorDescriptor::detect(const cv::Mat &img,
                                   std::vector<cv::KeyPoint> &keyPoints,
                                   cv::InputArray &mask)
{

  try {
    mOrb->detect(img, keyPoints, mask);
  } catch (cv::Exception &e) {
    msgError("ORB Detector error: %s", e.what());
    return true;
  }

  return false;
}

bool OrbCudaDetectorDescriptor::extract(const cv::Mat &img,
                                        std::vector<cv::KeyPoint> &keyPoints,
                                        cv::Mat &descriptors)
{

  try {
    mOrb->compute(img, keyPoints, descriptors);
  } catch (cv::Exception &e) {
    msgError("ORB Descriptor error: %s", e.what());
    return true;
  }

  return false;
}

void OrbCudaDetectorDescriptor::setFeaturesNumber(int featuresNumber)
{
  OrbProperties::setFeaturesNumber(featuresNumber);
  update();
}

void OrbCudaDetectorDescriptor::setScaleFactor(double scaleFactor)
{
  OrbProperties::setScaleFactor(scaleFactor);
  update();
}

void OrbCudaDetectorDescriptor::setLevelsNumber(int levelsNumber)
{
  OrbProperties::setLevelsNumber(levelsNumber);
  update();
}

void OrbCudaDetectorDescriptor::setEdgeThreshold(int edgeThreshold)
{
  OrbProperties::setEdgeThreshold(edgeThreshold);
  update();
}

void OrbCudaDetectorDescriptor::setWTA_K(int WTA_K)
{
  OrbProperties::setWTA_K(WTA_K);
  update();
}

void OrbCudaDetectorDescriptor::setScoreType(const std::string &scoreType)
{
  OrbProperties::setScoreType(scoreType);
  update();
}

void OrbCudaDetectorDescriptor::setPatchSize(int patchSize)
{
  OrbProperties::setPatchSize(patchSize);
  update();
}

void OrbCudaDetectorDescriptor::setFastThreshold(int fastThreshold)
{
  OrbProperties::setFastThreshold(fastThreshold);
  update();
}

void OrbCudaDetectorDescriptor::reset()
{
  OrbProperties::reset();
  update();
}

#endif // HAVE_CUDA

} // namespace tl

