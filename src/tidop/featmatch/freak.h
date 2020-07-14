#ifndef TL_FREAK_DESCRIPTOR_H
#define TL_FREAK_DESCRIPTOR_H

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


class TL_EXPORT FreakProperties
  : public Freak
{

public:

  FreakProperties();
  FreakProperties(const FreakProperties &freakProperties);
  ~FreakProperties() override = default;

// Freak interface

public:

  bool orientationNormalized() const override;
  bool scaleNormalized() const override;
  double patternScale() const override;
  int octaves() const override;
  void setOrientationNormalized(bool orientationNormalized) override;
  void setScaleNormalized(bool scaleNormalized) override;
  void setPatternScale(double patternScale) override;
  void setOctaves(int octaves) override;

// Feature interface

public:

  void reset() override;
  std::string name() const final;

private:

  bool mOrientationNormalized;
  bool mScaleNormalized;
  double mPatternScale;
  int mOctaves;
};


/*----------------------------------------------------------------*/


class TL_EXPORT FreakDescriptor
    : public FreakProperties,
      public DescriptorExtractor
{

public:

  FreakDescriptor();
  FreakDescriptor(const FreakDescriptor &freakDescriptor);
  FreakDescriptor(bool orientationNormalized,
                  bool scaleNormalized,
                  double patternScale,
                  int octaves);
  ~FreakDescriptor() override = default;

private:

  void update();

// DescriptorExtractor interface

public:

  bool extract(const cv::Mat &img,
               std::vector<cv::KeyPoint> &keyPoints,
               cv::Mat &descriptors) override;

// Freak interface

public:

  void setOrientationNormalized(bool orientationNormalized) override;
  void setScaleNormalized(bool scaleNormalized) override;
  void setPatternScale(double patternScale) override;
  void setOctaves(int octaves) override;

// Feature interface

public:

  void reset() override;

protected:

  cv::Ptr<cv::xfeatures2d::FREAK> mFREAK;
};

/*! \} */ // end of FeatureDetectorAndDescriptor

/*! \} */ // end of Features

} // namespace tl

#endif // TL_FREAK_DESCRIPTOR_H
