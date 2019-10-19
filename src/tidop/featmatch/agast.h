#ifndef TL_AGAST_DETECTOR_H
#define TL_AGAST_DETECTOR_H

#include "config_tl.h"

#include "tidop/featmatch/features.h"

namespace tl
{


/*!
 * \brief AGAST detector properties class
 */
class TL_EXPORT AgastProperties
  : public IAgast
{

public:

  AgastProperties();
  ~AgastProperties() override;

// IAgast interface

public:

  virtual int threshold() const override;
  virtual bool nonmaxSuppression() const override;
  virtual std::string detectorType() const override;
  virtual void setThreshold(int threshold) override;
  virtual void setNonmaxSuppression(bool nonmaxSuppression) override;
  virtual void setDetectorType(const std::string &detectorType) override;

// Feature interface

public:

  virtual void reset() override;
  std::string name() const final;

private:

  int mThreshold;
  bool mNonmaxSuppression;
  std::string mDetectorType;

};


/*----------------------------------------------------------------*/


class TL_EXPORT AgastDetector
  : public AgastProperties,
    public KeypointDetector
{

public:

  AgastDetector();
  AgastDetector(int threshold, bool nonmaxSuppression, std::string detectorType);
  ~AgastDetector() override;

// KeypointDetector interface

public:

  bool detect(const cv::Mat &img,
              std::vector<cv::KeyPoint> &keyPoints,
              cv::InputArray &mask = cv::noArray()) override;

// IAgast interface

public:

  void setThreshold(int threshold) override;
  void setNonmaxSuppression(bool nonmaxSuppression) override;
  void setDetectorType(const std::string &detectorType) override;

// Feature interface

public:

  void reset() override;

protected:

  cv::Ptr<cv::AgastFeatureDetector> mAgast;

};


} // namespace tl


#endif // TL_AGAST_DETECTOR_H
