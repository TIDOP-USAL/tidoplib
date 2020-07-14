#ifndef TL_BRIEF_DESCRIPTOR_H
#define TL_BRIEF_DESCRIPTOR_H

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


class TL_EXPORT BriefProperties
  : public Brief
{
public:

  BriefProperties();
  BriefProperties(const BriefProperties &briefProperties);
  ~BriefProperties() override = default;

// Brief interface

public:

  virtual std::string bytes() const override;
  virtual bool useOrientation() const override;
  virtual void setBytes(const std::string &bytes) override;
  virtual void setUseOrientation(bool useOrientation) override;

// Feature interface

public:

  virtual void reset() override;
  std::string name() const final;

private:

  std::string mBytes;
  bool mUseOrientation;
};


/*----------------------------------------------------------------*/


class TL_EXPORT BriefDescriptor
    : public BriefProperties,
      public DescriptorExtractor
{

public:

  BriefDescriptor();
  BriefDescriptor(const BriefDescriptor &briefDescriptor);
  BriefDescriptor(std::string bytes,
                  bool useOrientation);
  ~BriefDescriptor() override = default;

private:

  void update();

// DescriptorExtractor interface

public:

  bool extract(const cv::Mat &img,
               std::vector<cv::KeyPoint> &keyPoints,
               cv::Mat &descriptors) override;


// Brief interface

public:

  void setBytes(const std::string &bytes) override;
  void setUseOrientation(bool useOrientation) override;

// Feature interface

public:

  void reset() override;

protected:

  cv::Ptr<cv::xfeatures2d::BriefDescriptorExtractor> mBrief;
};

/*! \} */ // end of FeatureDetectorAndDescriptor

/*! \} */ // end of Features

} // namespace tl

#endif // TL_BRIEF_DESCRIPTOR_H
