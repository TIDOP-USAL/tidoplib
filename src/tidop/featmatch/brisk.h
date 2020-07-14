#ifndef TL_BRISK_DETECTOR_DESCRIPTOR_H
#define TL_BRISK_DETECTOR_DESCRIPTOR_H

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


class TL_EXPORT BriskProperties
  : public Brisk
{
public:

  BriskProperties();
  BriskProperties(const BriskProperties &briskProperties);
  ~BriskProperties() override = default;

// Brisk interface

public:

  int threshold() const override;
  int octaves() const override;
  double patternScale() const override;
  void setThreshold(int threshold) override;
  void setOctaves(int octaves) override;
  void setPatternScale(double patternScale) override;

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
  BriskDetectorDescriptor(const BriskDetectorDescriptor &briskDetectorDescriptor);
  BriskDetectorDescriptor(int threshold,
                          int octaves,
                          double patternScale);
  ~BriskDetectorDescriptor() override = default;

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

// Brisk interface

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

/*! \} */ // end of FeatureDetectorAndDescriptor

/*! \} */ // end of Features

} // namespace tl

#endif // TL_BRISK_DETECTOR_DESCRIPTOR_H
