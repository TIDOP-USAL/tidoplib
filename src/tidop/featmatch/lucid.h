#ifndef TL_LUCID_DESCRIPTOR_H
#define TL_LUCID_DESCRIPTOR_H

#include "config_tl.h"

#include "tidop/featmatch/features.h"

#include <opencv2/xfeatures2d.hpp>

namespace tl
{


class TL_EXPORT LucidProperties
  : public ILucid
{
public:

  LucidProperties();
  ~LucidProperties() override;

// ILucid interface

public:

  virtual int lucidKernel() const override;
  virtual int blurKernel() const override;
  virtual void setLucidKernel(int lucidKernel) override;
  virtual void setBlurKernel(int blurKernel) override;

// Feature interface

public:

  virtual void reset() override;
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

  ~LucidDescriptor() override;

private:

  void update();

// DescriptorExtractor interface

public:

  bool extract(const cv::Mat &img,
               std::vector<cv::KeyPoint> &keyPoints,
               cv::Mat &descriptors) override;


// ILucid interface

public:

  void setLucidKernel(int lucidKernel) override;
  void setBlurKernel(int blurKernel) override;

// Feature interface

public:

  void reset() override;

protected:

  cv::Ptr<cv::xfeatures2d::LUCID> mLUCID;
};


} // namespace tl

#endif // TL_LUCID_DESCRIPTOR_H
