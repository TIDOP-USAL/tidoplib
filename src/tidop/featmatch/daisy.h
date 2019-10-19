#ifndef TL_DAISY_DESCRIPTOR_H
#define TL_DAISY_DESCRIPTOR_H

#include "config_tl.h"

#include "tidop/featmatch/features.h"

#include <opencv2/xfeatures2d.hpp>

namespace tl
{

class TL_EXPORT DaisyProperties
  : public IDaisy
{
public:

  DaisyProperties();
  ~DaisyProperties() override;

// IDaisy interface

public:

  virtual double radius() const override;
  virtual int qRadius() const override;
  virtual int qTheta() const override;
  virtual int qHist() const override;
  virtual std::string norm() const override;
  virtual bool interpolation() const override;
  virtual bool useOrientation() const override;
  virtual void setRadius(double radius) override;
  virtual void setQRadius(int qRadius) override;
  virtual void setQTheta(int qTheta) override;
  virtual void setQHist(int qHist) override;
  virtual void setNorm(const std::string &norm) override;
  virtual void setInterpolation(bool interpolation) override;
  virtual void setUseOrientation(bool useOrientation) override;

// Feature interface

public:

  virtual void reset() override;
  std::string name() const final;

private:

  double mRadius;
  int mQRadius;
  int mQTheta;
  int mQHist;
  std::string mNorm;
  bool mInterpolation;
  bool mUseOrientation;
};


/*----------------------------------------------------------------*/


class TL_EXPORT DaisyDescriptor
    : public DaisyProperties,
      public DescriptorExtractor
{

public:

  DaisyDescriptor();
  DaisyDescriptor(double radius,
                  int qRadius,
                  int qTheta,
                  int qHist,
                  std::string norm,
                  bool interpolation,
                  bool useOrientation);

  ~DaisyDescriptor() override;

private:

  void update();

// DescriptorExtractor interface

public:

  bool extract(const cv::Mat &img,
               std::vector<cv::KeyPoint> &keyPoints,
               cv::Mat &descriptors) override;


// IDaisy interface

public:

  void setRadius(double radius) override;
  void setQRadius(int qRadius) override;
  void setQTheta(int qTheta) override;
  void setQHist(int qHist) override;
  void setNorm(const std::string &norm) override;
  void setInterpolation(bool interpolation) override;
  void setUseOrientation(bool useOrientation) override;

// Feature interface

public:

  void reset() override;

protected:

  cv::Ptr<cv::xfeatures2d::DAISY> mDAISY;
};


} // namespace tl

#endif // TL_DAISY_DESCRIPTOR_H
