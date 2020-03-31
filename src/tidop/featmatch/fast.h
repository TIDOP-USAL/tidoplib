#ifndef TL_FAST_DETECTOR_H
#define TL_FAST_DETECTOR_H

#include "config_tl.h"

#include "tidop/featmatch/features.h"

namespace tl
{


class TL_EXPORT FastProperties
  : public IFast
{
public:

  FastProperties();
  ~FastProperties() override;

// IFast interface

public:

  virtual int threshold() const override;
  virtual bool nonmaxSuppression() const override;
  virtual std::string detectorType() const override;
  virtual void setThreshold(int threshold) override;
  virtual void setNonmaxSuppression(bool nonmaxSuppression) override;
  virtual void setDetectorType(std::string detectorType) override;

// Feature interface

public:

  void reset() override;
  std::string name() const final;

private:

  int mThreshold;
  bool mNonmaxSuppression;
  std::string mDetectorType;
};


/*----------------------------------------------------------------*/


class FastDetector
  : public FastProperties,
    public KeypointDetector
{

public:

  FastDetector();
  FastDetector(int threshold, bool nonmaxSuppression, const std::string &detectorType);
  ~FastDetector() override;

// KeypointDetector interface

public:

  bool detect(const cv::Mat &img,
              std::vector<cv::KeyPoint> &keyPoints,
              cv::InputArray &mask = cv::noArray()) override;

// IFast interface

public:

  void setThreshold(int threshold) override;
  void setNonmaxSuppression(bool nonmaxSuppression) override;
  void setDetectorType(std::string detectorType) override;

// Feature interface

public:

  void reset() override;

protected:

  cv::Ptr<cv::FastFeatureDetector> mFast;

};


} // namespace tl

#endif // TL_AGAST_DETECTOR_H
