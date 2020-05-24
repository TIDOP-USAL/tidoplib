#ifndef TL_LUCID_DESCRIPTOR_H
#define TL_LUCID_DESCRIPTOR_H

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


class TL_EXPORT LucidProperties
  : public Lucid
{
public:

  LucidProperties();
  ~LucidProperties() override = default;

// Lucid interface

public:

  int lucidKernel() const override;
  int blurKernel() const override;
  void setLucidKernel(int lucidKernel) override;
  void setBlurKernel(int blurKernel) override;

// Feature interface

public:

  void reset() override;
  std::string name() const final;

private:

  int mLucidKernel;
  int mBlurKernel;
};


/*----------------------------------------------------------------*/


class TL_EXPORT LucidDescriptor
    : public LucidProperties,
      public DescriptorExtractor
{

public:

  LucidDescriptor();
  LucidDescriptor(int lucidKernel, int blurKernel);
  ~LucidDescriptor() override = default;

private:

  void update();

// DescriptorExtractor interface

public:

  bool extract(const cv::Mat &img,
               std::vector<cv::KeyPoint> &keyPoints,
               cv::Mat &descriptors) override;


// Lucid interface

public:

  void setLucidKernel(int lucidKernel) override;
  void setBlurKernel(int blurKernel) override;

// Feature interface

public:

  void reset() override;

protected:

  cv::Ptr<cv::xfeatures2d::LUCID> mLUCID;
};

/*! \} */ // end of FeatureDetectorAndDescriptor

/*! \} */ // end of Features

} // namespace tl

#endif // TL_LUCID_DESCRIPTOR_H
