#ifndef TL_LATCH_DESCRIPTOR_H
#define TL_LATCH_DESCRIPTOR_H

#include "config_tl.h"

#include "tidop/featmatch/features.h"

#include <opencv2/xfeatures2d.hpp>

namespace tl
{



class TL_EXPORT LatchProperties
  : public ILatch
{
public:

  LatchProperties();
  ~LatchProperties() override;

// ILatch interface

public:

  virtual std::string bytes() const override;
  virtual bool rotationInvariance() const override;
  virtual int halfSsdSize() const override;
  virtual void setBytes(const std::string &bytes) override;
  virtual void setRotationInvariance(bool rotationInvariance) override;
  virtual void setHalfSsdSize(int halfSsdSize) override;

// Feature interface

public:

  virtual void reset() override;
  std::string name() const final;

private:

  std::string mBytes;
  bool mRotationInvariance;
  int mHalfSsdSize;
};

/*----------------------------------------------------------------*/


class TL_EXPORT LatchDescriptor
    : public LatchProperties,
      public DescriptorExtractor
{

public:

  LatchDescriptor();
  LatchDescriptor(std::string bytes,
                  bool rotationInvariance,
                  int halfSsdSize);

  ~LatchDescriptor() override;

private:

  void update();

// DescriptorExtractor interface

public:

  bool extract(const cv::Mat &img,
               std::vector<cv::KeyPoint> &keyPoints,
               cv::Mat &descriptors) override;


// IBrief interface

public:

  void setBytes(const std::string &bytes) override;
  void setRotationInvariance(bool rotationInvariance) override;
  void setHalfSsdSize(int halfSsdSize) override;

// Feature interface

public:

  void reset() override;

protected:

  cv::Ptr<cv::xfeatures2d::LATCH> mLATCH;
};



} // namespace tl

#endif // TL_LATCH_DESCRIPTOR_H
