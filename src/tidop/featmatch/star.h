#ifndef TL_STAR_DETECTOR_H
#define TL_STAR_DETECTOR_H

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


class TL_EXPORT StarProperties
  : public Star
{
public:

  StarProperties();
  StarProperties(const StarProperties &starProperties);
  ~StarProperties() override = default;

// Star interface

public:

  int maxSize() const override;
  int responseThreshold() const override;
  int lineThresholdProjected() const override;
  int lineThresholdBinarized() const override;
  int suppressNonmaxSize() const override;
  void setMaxSize(int maxSize) override;
  void setResponseThreshold(int responseThreshold) override;
  void setLineThresholdProjected(int lineThresholdProjected) override;
  void setLineThresholdBinarized(int lineThresholdBinarized) override;
  void setSuppressNonmaxSize(int suppressNonmaxSize) override;

// Feature interface

public:

  void reset() override;
  std::string name() const final;

private:

  int mMaxSize;
  int mResponseThreshold;
  int mLineThresholdProjected;
  int mLineThresholdBinarized;
  int mSuppressNonmaxSize;
};



/*----------------------------------------------------------------*/


class TL_EXPORT StarDetector
  : public StarProperties,
    public KeypointDetector
{

public:

  StarDetector();
  StarDetector(const StarDetector &starDetector);
  StarDetector(int maxSize,
               int responseThreshold,
               int lineThresholdProjected,
               int lineThresholdBinarized,
               int suppressNonmaxSize);
  ~StarDetector() override = default;

private:

  void update();

// KeypointDetector interface

public:

  bool detect(const cv::Mat &img,
              std::vector<cv::KeyPoint> &keyPoints,
              cv::InputArray &mask = cv::noArray()) override;

// Star interface

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


/*! \} */ // end of FeatureDetectorAndDescriptor

/*! \} */ // end of Features

} // namespace tl

#endif // TL_STAR_DETECTOR_H
