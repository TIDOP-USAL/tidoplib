#ifndef TL_BRISK_DETECTOR_DESCRIPTOR_H
#define TL_BRISK_DETECTOR_DESCRIPTOR_H

#include "config_tl.h"

#include "tidop/featmatch/features.h"

namespace tl
{


class TL_EXPORT BriskProperties
  : public IBrisk
{
public:

  BriskProperties();
  ~BriskProperties() override;

// IBrisk interface

public:

  virtual int threshold() const override;
  virtual int octaves() const override;
  virtual double patternScale() const override;
  virtual void setThreshold(int threshold) override;
  virtual void setOctaves(int octaves) override;
  virtual void setPatternScale(double patternScale) override;

// Feature interface

public:

  void reset() override;
  std::string name() const final;

private:

  int mThreshold;
  int mOctaves;
  double mPatternScale;
};


/*----------------------------------------------------------------*/


class TL_EXPORT BriskDetectorDescriptor
  : public BriskProperties,
    public KeypointDetector,
    public DescriptorExtractor
{

public:

  BriskDetectorDescriptor();
  BriskDetectorDescriptor(int threshold,
                          int octaves,
                          double patternScale);

  ~BriskDetectorDescriptor() override;

protected:

  void update();


// KeypointDetector interface

public:

  bool detect(const cv::Mat &img,
              std::vector<cv::KeyPoint> &keyPoints,
              cv::InputArray &mask = cv::noArray()) override;

// DescriptorExtractor interface

public:

  bool extract(const cv::Mat &img,
               std::vector<cv::KeyPoint> &keyPoints,
               cv::Mat &descriptors) override;

// IAkaze interface

public:

  void setThreshold(int threshold) override;
  void setOctaves(int octaves) override;
  void setPatternScale(double patternScale) override;

// Feature interface

public:

  void reset() override;

protected:

  cv::Ptr<cv::BRISK> mBrisk;

};


} // namespace tl

#endif // TL_BRISK_DETECTOR_DESCRIPTOR_H
