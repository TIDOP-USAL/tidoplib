#ifndef TL_STAR_DETECTOR_H
#define TL_STAR_DETECTOR_H

#include "config_tl.h"

#include "tidop/featmatch/features.h"

#include <opencv2/xfeatures2d.hpp>

namespace tl
{


class TL_EXPORT StarProperties
  : public IStar
{
public:

  StarProperties();
  ~StarProperties() override;

// IStar interface

public:

  virtual int maxSize() const override;
  virtual int responseThreshold() const override;
  virtual int lineThresholdProjected() const override;
  virtual int lineThresholdBinarized() const override;
  virtual int suppressNonmaxSize() const override;
  virtual void setMaxSize(int maxSize) override;
  virtual void setResponseThreshold(int responseThreshold) override;
  virtual void setLineThresholdProjected(int lineThresholdProjected) override;
  virtual void setLineThresholdBinarized(int lineThresholdBinarized) override;
  virtual void setSuppressNonmaxSize(int suppressNonmaxSize) override;

// Feature interface

public:

  virtual void reset() override;
  std::string name() const final;

private:

  int mMaxSize;
  int mResponseThreshold;
  int mLineThresholdProjected;
  int mLineThresholdBinarized;
  int mSuppressNonmaxSize;
};



/*----------------------------------------------------------------*/


class StarDetector
  : public StarProperties,
    public KeypointDetector
{

public:

  StarDetector();
  StarDetector(int maxSize,
               int responseThreshold,
               int lineThresholdProjected,
               int lineThresholdBinarized,
               int suppressNonmaxSize);
  ~StarDetector() override;

private:

  void update();

// KeypointDetector interface

public:

  bool detect(const cv::Mat &img,
              std::vector<cv::KeyPoint> &keyPoints,
              cv::InputArray &mask = cv::noArray()) override;

// IStar interface

public:

  void setMaxSize(int maxSize) override;
  void setResponseThreshold(int responseThreshold) override;
  void setLineThresholdProjected(int lineThresholdProjected) override;
  void setLineThresholdBinarized(int lineThresholdBinarized) override;
  void setSuppressNonmaxSize(int suppressNonmaxSize) override;

// Feature interface

public:

  void reset() override;

protected:

  cv::Ptr<cv::xfeatures2d::StarDetector> mSTAR;

};


} // namespace tl

#endif // TL_STAR_DETECTOR_H
