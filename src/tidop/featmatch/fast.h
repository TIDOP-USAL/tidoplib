#ifndef TL_FAST_DETECTOR_H
#define TL_FAST_DETECTOR_H

#include "config_tl.h"

#include "tidop/featmatch/features.h"

#ifdef HAVE_OPENCV_CUDAFEATURES2D
#include <opencv2/cudafeatures2d.hpp>
#endif // HAVE_OPENCV_CUDAFEATURES2D

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


class TL_EXPORT FastProperties
  : public Fast
{
public:

  FastProperties();
  FastProperties(const FastProperties &fastProperties);
  ~FastProperties() override = default;

// Fast interface

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


class TL_EXPORT FastDetector
  : public FastProperties,
    public KeypointDetector
{

public:

  FastDetector();
  FastDetector(const FastDetector &fastDetector);
  FastDetector(int threshold, 
               bool nonmaxSuppression, 
               const std::string &detectorType);
  ~FastDetector() override = default;

private:

#if CV_VERSION_MAJOR >= 4
  cv::FastFeatureDetector::DetectorType convertDetectorType(const std::string &detectorType);
#else
  int convertDetectorType(const std::string &detectorType);
#endif

// KeypointDetector interface

public:

  bool detect(const cv::Mat &img,
              std::vector<cv::KeyPoint> &keyPoints,
              cv::InputArray &mask = cv::noArray()) override;

// Fast interface

public:

  void setThreshold(int threshold) override;
  void setNonmaxSuppression(bool nonmaxSuppression) override;
  void setDetectorType(const std::string &detectorType) override;

// Feature interface

public:

  void reset() override;

protected:

  cv::Ptr<cv::FastFeatureDetector> mFast;

};


/*----------------------------------------------------------------*/

#ifdef HAVE_OPENCV_CUDAFEATURES2D

class TL_EXPORT FastDetectorCuda
  : public FastProperties,
    public KeypointDetector
{

public:

  FastDetectorCuda();
  FastDetectorCuda(const FastDetector &fastDetector);
  FastDetectorCuda(int threshold, 
                   bool nonmaxSuppression, 
                   const std::string &detectorType);
  ~FastDetectorCuda() override = default;

private:

#if CV_VERSION_MAJOR >= 4
  cv::FastFeatureDetector::DetectorType convertDetectorType(const std::string &detectorType);
#else
  int convertDetectorType(const std::string &detectorType);
#endif
  
  void update();
  
// KeypointDetector interface

public:

  bool detect(const cv::Mat &img,
              std::vector<cv::KeyPoint> &keyPoints,
              cv::InputArray &mask = cv::noArray()) override;

// Fast interface

public:

  void setThreshold(int threshold) override;
  void setNonmaxSuppression(bool nonmaxSuppression) override;
  void setDetectorType(const std::string &detectorType) override;

// Feature interface

public:

  void reset() override;

protected:

  cv::Ptr<cv::cuda::FastFeatureDetector> mFast;

};

#endif // HAVE_OPENCV_CUDAFEATURES2D


/*! \} */ // end of FeatureDetectorAndDescriptor

/*! \} */ // end of Features

} // namespace tl

#endif // TL_AGAST_DETECTOR_H
