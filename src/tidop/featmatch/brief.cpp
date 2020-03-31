#include "brief.h"

#include "tidop/core/messages.h"


namespace tl
{


BriefProperties::BriefProperties()
  : IBrief(),
    mBytes("32"),
    mUseOrientation(false)
{
}

BriefProperties::~BriefProperties()
{

}

std::string BriefProperties::bytes() const
{
  return mBytes;
}

bool BriefProperties::useOrientation() const
{
  return mUseOrientation;
}

void BriefProperties::setBytes(const std::string &bytes)
{
  if (bytes.compare("16") == 0 ||
      bytes.compare("32") == 0 ||
      bytes.compare("64") == 0) {
    mBytes = bytes;
  }
}

void BriefProperties::setUseOrientation(bool useOrientation)
{
  mUseOrientation = useOrientation;
}

void BriefProperties::reset()
{
  mBytes = "32";
  mUseOrientation = false;
}

std::string BriefProperties::name() const
{
  return std::string("BRIEF");
}


/*----------------------------------------------------------------*/


BriefDescriptor::BriefDescriptor()
  : BriefProperties(),
    DescriptorExtractor()
{
  update();
}

BriefDescriptor::BriefDescriptor(std::string bytes, bool useOrientation)
  : BriefProperties(),
    DescriptorExtractor()
{
  BriefProperties::setBytes(bytes);
  BriefProperties::setUseOrientation(useOrientation);
  update();
}

BriefDescriptor::~BriefDescriptor()
{

}

void BriefDescriptor::update()
{
  mBrief = cv::xfeatures2d::BriefDescriptorExtractor::create(std::stoi(BriefProperties::bytes()),
                                                             BriefProperties::useOrientation());
}

bool BriefDescriptor::extract(const cv::Mat &img,
                              std::vector<cv::KeyPoint> &keyPoints,
                              cv::Mat &descriptors)
{

  try {
    mBrief->compute(img, keyPoints, descriptors);
  } catch (cv::Exception &e) {
    msgError("BRIEF Descriptor error: %s", e.what());
    return true;
  }

  return false;
}

void BriefDescriptor::setBytes(const std::string &bytes)
{
  BriefProperties::setBytes(bytes);
  update();
}

void BriefDescriptor::setUseOrientation(bool useOrientation)
{
  BriefProperties::setUseOrientation(useOrientation);
  update();
}

void BriefDescriptor::reset()
{
  BriefProperties::reset();
  update();
}


} // namespace tl
