#include "akaze.h"

#include "tidop/core/messages.h"


namespace tl
{


AkazeProperties::AkazeProperties()
  : mDescriptorType("MLDB"),
    mDescriptorSize(0),
    mDescriptorChannels(3),
    mThreshold(0.001),
    mOctaves(4),
    mOctaveLayers(4),
    mDiffusivity("DIFF_PM_G2")
{
}

AkazeProperties::AkazeProperties(const AkazeProperties &akazeProperties)
  : Akaze(akazeProperties),
    mDescriptorType(akazeProperties.mDescriptorType),
    mDescriptorSize(akazeProperties.mDescriptorSize),
    mDescriptorChannels(akazeProperties.mDescriptorChannels),
    mThreshold(akazeProperties.mThreshold),
    mOctaves(akazeProperties.mOctaves),
    mOctaveLayers(akazeProperties.mOctaveLayers),
    mDiffusivity(akazeProperties.mDiffusivity)
{
}

std::string AkazeProperties::descriptorType() const
{
  return mDescriptorType;
}

int AkazeProperties::descriptorSize() const
{
  return mDescriptorSize;
}

int AkazeProperties::descriptorChannels() const
{
  return mDescriptorChannels;
}

double AkazeProperties::threshold() const
{
  return mThreshold;
}

int AkazeProperties::octaves() const
{
  return mOctaves;
}

int AkazeProperties::octaveLayers() const
{
  return mOctaveLayers;
}

std::string AkazeProperties::diffusivity() const
{
  return mDiffusivity;
}

void AkazeProperties::setDescriptorType(const std::string &descriptorType)
{
  if (descriptorType.compare("KAZE") == 0 ||
      descriptorType.compare("KAZE_UPRIGHT") == 0 ||
      descriptorType.compare("MLDB") == 0 ||
      descriptorType.compare("MLDB_UPRIGHT") == 0){
    mDescriptorType = descriptorType;
  }
}

void AkazeProperties::setDescriptorSize(int descriptorSize)
{
  mDescriptorSize = descriptorSize;
}

void AkazeProperties::setDescriptorChannels(int channels)
{
  mDescriptorChannels = channels;
}

void AkazeProperties::setThreshold(double threshold)
{
  mThreshold = threshold;
}

void AkazeProperties::setOctaves(int octaves)
{
  mOctaves = octaves;
}

void AkazeProperties::setOctaveLayers(int octaveLayers)
{
  mOctaveLayers = octaveLayers;
}

void AkazeProperties::setDiffusivity(const std::string &diffusivity)
{
  if (diffusivity.compare("DIFF_PM_G1") == 0 ||
      diffusivity.compare("DIFF_PM_G2") == 0 ||
      diffusivity.compare("DIFF_WEICKERT") == 0 ||
      diffusivity.compare("DIFF_CHARBONNIER") == 0){
    mDiffusivity = diffusivity;
  }
}

void AkazeProperties::reset()
{
  mDescriptorType = "MLDB";
  mDescriptorSize = 0;
  mDescriptorChannels = 3;
  mThreshold = 0.001;
  mOctaves = 4;
  mOctaveLayers = 4;
  mDiffusivity = "DIFF_PM_G2";
}

std::string AkazeProperties::name() const
{
  return std::string("AKAZE");
}



/*----------------------------------------------------------------*/


AkazeDetectorDescriptor::AkazeDetectorDescriptor()
{
  mAkaze = cv::AKAZE::create(convertDescriptorType(AkazeProperties::descriptorType()),
                             AkazeProperties::descriptorSize(),
                             AkazeProperties::descriptorChannels(),
                             static_cast<float>(AkazeProperties::threshold()),
                             AkazeProperties::octaves(),
                             AkazeProperties::octaveLayers(),
                             convertDiffusivity(AkazeProperties::diffusivity()));
}

AkazeDetectorDescriptor::AkazeDetectorDescriptor(const AkazeDetectorDescriptor &akazeDetectorDescriptor)
  : AkazeProperties(akazeDetectorDescriptor),
    KeypointDetector(akazeDetectorDescriptor),
    DescriptorExtractor(akazeDetectorDescriptor)
{
  mAkaze = cv::AKAZE::create(convertDescriptorType(AkazeProperties::descriptorType()),
                             AkazeProperties::descriptorSize(),
                             AkazeProperties::descriptorChannels(),
                             static_cast<float>(AkazeProperties::threshold()),
                             AkazeProperties::octaves(),
                             AkazeProperties::octaveLayers(),
                             convertDiffusivity(AkazeProperties::diffusivity()));
}

AkazeDetectorDescriptor::AkazeDetectorDescriptor(const std::string &descriptorType,
                                                 int descriptorSize,
                                                 int descriptorChannels,
                                                 double threshold,
                                                 int octaves,
                                                 int octaveLayers,
                                                 const std::string &diffusivity)
  : mAkaze(cv::AKAZE::create())
{
  setDescriptorType(descriptorType);
  setDescriptorSize(descriptorSize);
  setDescriptorChannels(descriptorChannels);
  setThreshold(threshold);
  setOctaves(octaves);
  setOctaveLayers(octaveLayers);
  setDiffusivity(diffusivity);
}

#if CV_VERSION_MAJOR >= 4
cv::AKAZE::DescriptorType AkazeDetectorDescriptor::convertDescriptorType(const std::string &descriptorType)
{
  cv::AKAZE::DescriptorType descriptor_type = cv::AKAZE::DescriptorType::DESCRIPTOR_MLDB;

  if (descriptorType.compare("KAZE") == 0){
    descriptor_type = cv::AKAZE::DESCRIPTOR_KAZE;
  } else if (descriptorType.compare("KAZE_UPRIGHT") == 0){
    descriptor_type = cv::AKAZE::DESCRIPTOR_KAZE_UPRIGHT;
  } else if (descriptorType.compare("MLDB") == 0){
    descriptor_type = cv::AKAZE::DESCRIPTOR_MLDB;
  } else if (descriptorType.compare("MLDB_UPRIGHT") == 0){
    descriptor_type = cv::AKAZE::DESCRIPTOR_MLDB_UPRIGHT;
  }

  return descriptor_type;
}

cv::KAZE::DiffusivityType AkazeDetectorDescriptor::convertDiffusivity(const std::string &diffusivity)
{
  cv::KAZE::DiffusivityType diff = cv::KAZE::DiffusivityType::DIFF_PM_G1;

  if (diffusivity.compare("DIFF_PM_G1") == 0){
    diff = cv::KAZE::DIFF_PM_G1;
  } else if (diffusivity.compare("DIFF_PM_G2") == 0){
    diff = cv::KAZE::DIFF_PM_G2;
  } else if (diffusivity.compare("DIFF_WEICKERT") == 0){
    diff = cv::KAZE::DIFF_WEICKERT;
  } else if (diffusivity.compare("DIFF_CHARBONNIER") == 0){
    diff = cv::KAZE::DIFF_CHARBONNIER;
  }

  return diff;
}

#else

int AkazeDetectorDescriptor::convertDescriptorType(const std::string &descriptorType)
{
  int descriptor_type = cv::AKAZE::DESCRIPTOR_MLDB;

  if (descriptorType.compare("KAZE") == 0){
    descriptor_type = cv::AKAZE::DESCRIPTOR_KAZE;
  } else if (descriptorType.compare("KAZE_UPRIGHT") == 0){
    descriptor_type = cv::AKAZE::DESCRIPTOR_KAZE_UPRIGHT;
  } else if (descriptorType.compare("MLDB") == 0){
    descriptor_type = cv::AKAZE::DESCRIPTOR_MLDB;
  } else if (descriptorType.compare("MLDB_UPRIGHT") == 0){
    descriptor_type = cv::AKAZE::DESCRIPTOR_MLDB_UPRIGHT;
  }

  return descriptor_type;
}

int AkazeDetectorDescriptor::convertDiffusivity(const std::string &diffusivity)
{
  int diff = cv::KAZE::DIFF_PM_G1;

  if (diffusivity.compare("DIFF_PM_G1") == 0){
    diff = cv::KAZE::DIFF_PM_G1;
  } else if (diffusivity.compare("DIFF_PM_G2") == 0){
    diff = cv::KAZE::DIFF_PM_G2;
  } else if (diffusivity.compare("DIFF_WEICKERT") == 0){
    diff = cv::KAZE::DIFF_WEICKERT;
  } else if (diffusivity.compare("DIFF_CHARBONNIER") == 0){
    diff = cv::KAZE::DIFF_CHARBONNIER;
  }

  return diff;
}
#endif

bool AkazeDetectorDescriptor::detect(const cv::Mat &img,
                                     std::vector<cv::KeyPoint> &keyPoints,
                                     cv::InputArray &mask)
{

  try {
    mAkaze->detect(img, keyPoints, mask);
  } catch (cv::Exception &e) {
    msgError("AKAZE Detector error: %s", e.what());
    return true;
  }

  return false;
}


bool AkazeDetectorDescriptor::extract(const cv::Mat &img,
                                      std::vector<cv::KeyPoint> &keyPoints,
                                      cv::Mat &descriptors)
{

  try {
    mAkaze->compute(img, keyPoints, descriptors);
  } catch (cv::Exception &e) {
    msgError("AKAZE Descriptor error: %s", e.what());
    return true;
  }

  return false;
}

void AkazeDetectorDescriptor::setDescriptorType(const std::string &descriptorType)
{
  AkazeProperties::setDescriptorType(descriptorType);
  mAkaze->setDescriptorType(convertDescriptorType(descriptorType));
}

void AkazeDetectorDescriptor::setDescriptorSize(int descriptorSize)
{
  AkazeProperties::setDescriptorSize(descriptorSize);
  mAkaze->setDescriptorSize(descriptorSize);
}

void AkazeDetectorDescriptor::setDescriptorChannels(int channels)
{
  AkazeProperties::setDescriptorChannels(channels);
  mAkaze->setDescriptorChannels(channels);
}

void AkazeDetectorDescriptor::setThreshold(double threshold)
{
  AkazeProperties::setThreshold(threshold);
  mAkaze->setThreshold(threshold);
}

void AkazeDetectorDescriptor::setOctaves(int octaves)
{
  AkazeProperties::setOctaves(octaves);
  mAkaze->setNOctaves(octaves);
}

void AkazeDetectorDescriptor::setOctaveLayers(int octaveLayers)
{
  AkazeProperties::setOctaveLayers(octaveLayers);
  mAkaze->setNOctaveLayers(octaveLayers);
}

void AkazeDetectorDescriptor::setDiffusivity(const std::string &diffusivity)
{
  AkazeProperties::setDiffusivity(diffusivity);
  mAkaze->setDiffusivity(convertDiffusivity(diffusivity));
}

void AkazeDetectorDescriptor::reset()
{
  AkazeProperties::reset();
  mAkaze->setDescriptorType(convertDescriptorType(AkazeProperties::descriptorType()));
  mAkaze->setDescriptorSize(AkazeProperties::descriptorSize());
  mAkaze->setDescriptorChannels(AkazeProperties::descriptorChannels());
  mAkaze->setThreshold(AkazeProperties::threshold());
  mAkaze->setNOctaves(AkazeProperties::octaves());
  mAkaze->setNOctaveLayers(AkazeProperties::octaveLayers());
  mAkaze->setDiffusivity(convertDiffusivity(AkazeProperties::diffusivity()));

}


} // namespace tl
