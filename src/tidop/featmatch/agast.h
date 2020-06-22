#ifndef TL_AGAST_DETECTOR_H
#define TL_AGAST_DETECTOR_H

#include "config_tl.h"

#include "tidop/featmatch/features.h"

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


class TL_EXPORT AgastProperties
  : public Agast
{

public:

  AgastProperties();
  AgastProperties(const AgastProperties &agast);
  AgastProperties(AgastProperties &&agast) noexcept;
  ~AgastProperties() override = default;

  AgastProperties &operator =(const AgastProperties &agast);
  AgastProperties &operator =(AgastProperties &&agast) noexcept;

// Agast interface

public:

  int threshold() const override;
  bool nonmaxSuppression() const override;
  std::string detectorType() const override;
  void setThreshold(int threshold) override;
  void setNonmaxSuppression(bool nonmaxSuppression) override;
  void setDetectorType(const std::string &detectorType) override;

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


class TL_EXPORT AgastDetector
  : public AgastProperties,
    public KeypointDetector
{

public:

  AgastDetector();
  AgastDetector(const AgastDetector &agastDetector);
  AgastDetector(AgastDetector &&agastDetector) noexcept;
  AgastDetector(int threshold, 
                bool nonmaxSuppression, 
                const std::string &detectorType);
  ~AgastDetector() override = default;
  AgastDetector &operator =(const AgastDetector &agastDetector);
  AgastDetector &operator =(AgastDetector &&agastDetector) noexcept;

private:

#if CV_VERSION_MAJOR >= 4
  cv::AgastFeatureDetector::DetectorType convertDetectorType(const std::string &detectorType);
#else
  int convertDetectorType(const std::string &detectorType);
#endif

  void initAgastFromProperties();

// KeypointDetector interface

public:

  bool detect(const cv::Mat &img,
              std::vector<cv::KeyPoint> &keyPoints,
              cv::InputArray &mask = cv::noArray()) override;

// Agast interface

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

/*! \} */ // end of FeatureDetectorAndDescriptor

/*! \} */ // end of Features

} // namespace tl


#endif // TL_AGAST_DETECTOR_H
