#ifndef TL_GFTT_DETECTOR_H
#define TL_GFTT_DETECTOR_H

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


class TL_EXPORT GfttProperties
  : public Gftt
{
public:

  GfttProperties();
  GfttProperties(const GfttProperties &gfttProperties);
  ~GfttProperties() override = default;

// Gftt interface

public:

  int maxFeatures() const override;
  double qualityLevel() const override;
  double minDistance() const override;
  int blockSize() const override;
  bool harrisDetector() const override;
  double k() const override;
  void setMaxFeatures(int maxFeatures) override;
  void setQualityLevel(double qlevel) override;
  void setMinDistance(double minDistance) override;
  void setBlockSize(int blockSize) override;
  void setHarrisDetector(bool value) override;
  void setK(double k) override;

// Feature interface

public:

  void reset() override;
  std::string name() const final;

private:

  int mMaxFeatures;
  double mQualityLevel;
  double mMinDistance;
  int mBlockSize;
  bool mHarrisDetector;
  double mK;

};


/*----------------------------------------------------------------*/


class TL_EXPORT GfttDetector
  : public GfttProperties,
    public KeypointDetector
{

public:

  GfttDetector();
  GfttDetector(const GfttDetector &gfttDetector);
  GfttDetector(int maxFeatures,
               double qualityLevel,
               double minDistance,
               int blockSize,
               bool harrisDetector,
               double k);
  ~GfttDetector() override = default;

// KeypointDetector interface

public:

  bool detect(const cv::Mat &img,
              std::vector<cv::KeyPoint> &keyPoints,
              cv::InputArray &mask) override;

// Gftt interface

public:

  void setMaxFeatures(int maxFeatures) override;
  void setQualityLevel(double qlevel) override;
  void setMinDistance(double minDistance) override;
  void setBlockSize(int blockSize) override;
  void setHarrisDetector(bool value) override;
  void setK(double k) override;

// Feature interface

public:

  void reset() override;

protected:

  cv::Ptr<cv::GFTTDetector> mGFTT;

};

/*! \} */ // end of FeatureDetectorAndDescriptor

/*! \} */ // end of Features

} // namespace tl

#endif // TL_GFTT_DETECTOR_H
