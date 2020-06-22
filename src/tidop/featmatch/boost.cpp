#include "boost.h"

#include <tidop/core/messages.h>

namespace tl
{

BoostProperties::BoostProperties()
  : mDescriptorType("BINBOOST_256"),
    bUseOrientation(true),
    mScaleFactor(6.25)
{

}

BoostProperties::BoostProperties(const BoostProperties &boostProperties)
  : Boost(boostProperties),
    mDescriptorType(boostProperties.mDescriptorType),
    bUseOrientation(boostProperties.bUseOrientation),
    mScaleFactor(boostProperties.mScaleFactor)
{

}

std::string BoostProperties::descriptorType() const
{
  return mDescriptorType;
}

bool BoostProperties::useOrientation() const
{
  return bUseOrientation;
}

double BoostProperties::scaleFactor() const
{
  return mScaleFactor;
}

void BoostProperties::setDescriptorType(const std::string &descriptorType)
{
  if (descriptorType.compare("BGM") == 0 ||
      descriptorType.compare("BGM_HARD") == 0 ||
      descriptorType.compare("BGM_BILINEAR") == 0 ||
      descriptorType.compare("LBGM") == 0||
      descriptorType.compare("BINBOOST_64") == 0||
      descriptorType.compare("BINBOOST_128") == 0||
      descriptorType.compare("BINBOOST_256") == 0) {
    mDescriptorType = descriptorType;
  }
}

void BoostProperties::setUseOrientation(bool useOrientation)
{
  bUseOrientation = useOrientation;
}

void BoostProperties::setScaleFactor(double scaleFactor)
{
  mScaleFactor = scaleFactor;
}

void BoostProperties::reset()
{
  mDescriptorType = "BINBOOST_256";
  bUseOrientation = true;
  mScaleFactor = 6.25;
}

std::string BoostProperties::name() const
{
  return std::string("BOOST");
}


/*----------------------------------------------------------------*/


BoostDescriptor::BoostDescriptor()
{
  update();
}

BoostDescriptor::BoostDescriptor(const BoostDescriptor &boostDescriptor)
  : BoostProperties(boostDescriptor),
    DescriptorExtractor(boostDescriptor)
{
  update();
}

BoostDescriptor::BoostDescriptor(const std::string &descriptorType,
                                 bool useOrientation,
                                 double scaleFactor)
{
  BoostProperties::setDescriptorType(descriptorType);
  BoostProperties::setUseOrientation(useOrientation);
  BoostProperties::setScaleFactor(scaleFactor);
  update();
}

void BoostDescriptor::update()
{
#if CV_VERSION_MAJOR >= 4 || (CV_VERSION_MAJOR >= 3 && CV_VERSION_MINOR > 2)
  int descriptor_type = cv::xfeatures2d::BoostDesc::BGM;
  std::string descriptorType = BoostProperties::descriptorType();
  if (descriptorType.compare("BGM") == 0 ) {
    descriptor_type = cv::xfeatures2d::BoostDesc::BGM;
  } else if (descriptorType.compare("BGM_HARD") == 0){
    descriptor_type = cv::xfeatures2d::BoostDesc::BGM_HARD;
  } else if (descriptorType.compare("BGM_BILINEAR") == 0){
    descriptor_type = cv::xfeatures2d::BoostDesc::BGM_BILINEAR;
  } else if (descriptorType.compare("LBGM") == 0){
    descriptor_type = cv::xfeatures2d::BoostDesc::LBGM;
  } else if (descriptorType.compare("BINBOOST_64") == 0){
    descriptor_type = cv::xfeatures2d::BoostDesc::BINBOOST_64;
  } else if (descriptorType.compare("BINBOOST_128") == 0){
    descriptor_type = cv::xfeatures2d::BoostDesc::BINBOOST_128;
  } else if (descriptorType.compare("BINBOOST_256") == 0){
    descriptor_type = cv::xfeatures2d::BoostDesc::BINBOOST_256;
  }

  mBoost = cv::xfeatures2d::BoostDesc::create(descriptor_type,
                                              BoostProperties::useOrientation(),
                                              static_cast<float>(BoostProperties::scaleFactor()));
#endif
}

void BoostDescriptor::reset()
{
  BoostProperties::reset();
  update();
}

void BoostDescriptor::setDescriptorType(const std::string &descriptorType)
{
  BoostProperties::setDescriptorType(descriptorType);
  update();
}

void BoostDescriptor::setUseOrientation(bool useOrientation)
{
  BoostProperties::setUseOrientation(useOrientation);
  update();
}

void BoostDescriptor::setScaleFactor(double scaleFactor)
{
  BoostProperties::setScaleFactor(scaleFactor);
  update();
}

bool BoostDescriptor::extract(const cv::Mat &img, std::vector<cv::KeyPoint> &keyPoints, cv::Mat &descriptors)
{

#if CV_VERSION_MAJOR >= 4 || (CV_VERSION_MAJOR >= 3 && CV_VERSION_MINOR > 2)
  try {
    mBoost->compute(img, keyPoints, descriptors);
  } catch (cv::Exception &e) {
    msgError("BOOST Descriptor error: %s", e.what());
    return true;
  }

#  else
  TL_COMPILER_WARNING("Boost Descriptor not supported in OpenCV versions < 3.3")
#endif
  return false;
}


} // namespace tl


