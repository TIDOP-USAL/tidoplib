#ifndef TL_LATCH_DESCRIPTOR_H
#define TL_LATCH_DESCRIPTOR_H

#include "config_tl.h"

#include "tidop/featmatch/features.h"

#include <opencv2/xfeatures2d.hpp>

namespace tl
{

/*! \addtogroup Features
 * 
 *  \{
 */

/*! \addtogroup FeatureDetectorAndDescriptor
 * 
 *  \{
 */


class TL_EXPORT LatchProperties
  : public Latch
{
public:

  LatchProperties();
  LatchProperties(const LatchProperties &latchProperties);
  ~LatchProperties() override = default;

// Latch interface

public:

  std::string bytes() const override;
  bool rotationInvariance() const override;
  int halfSsdSize() const override;
  void setBytes(const std::string &bytes) override;
  void setRotationInvariance(bool rotationInvariance) override;
  void setHalfSsdSize(int halfSsdSize) override;

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
  LatchDescriptor(const LatchDescriptor &latchDescriptor);
  LatchDescriptor(const std::string &bytes,
                  bool rotationInvariance,
                  int halfSsdSize);
  ~LatchDescriptor() override = default;

private:

  void update();

// DescriptorExtractor interface

public:

  bool extract(const cv::Mat &img,
               std::vector<cv::KeyPoint> &keyPoints,
               cv::Mat &descriptors) override;


// Latch interface

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

/*! \} */ // end of FeatureDetectorAndDescriptor

/*! \} */ // end of Features

} // namespace tl

#endif // TL_LATCH_DESCRIPTOR_H
