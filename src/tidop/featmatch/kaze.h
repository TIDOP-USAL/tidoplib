#ifndef TL_KAZE_DETECTOR_DESCRIPTOR_H
#define TL_KAZE_DETECTOR_DESCRIPTOR_H

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


class TL_EXPORT KazeProperties
  : public Kaze
{
public:

  KazeProperties();
  KazeProperties(const KazeProperties &kazeProperties);
  ~KazeProperties() override = default;

// Kaze interface

public:

  bool extendedDescriptor() const override;
  bool uprightDescriptor() const override;
  double threshold() const override;
  int octaves() const override;
  int octaveLayers() const override;
  std::string diffusivity() const override;
  void setExtendedDescriptor(bool extended) override;
  void setUprightDescriptor(bool uprightDescriptor) override;
  void setThreshold(double threshold) override;
  void setOctaves(int octaves) override;
  void setOctaveLayers(int octaveLayers) override;
  void setDiffusivity(const std::string &diffusivity) override;

// Feature interface

public:

  void reset() override;
  std::string name() const final;

private:

  bool mExtended;
  bool mUpright;
  double mThreshold;
  int mOctaves;
  int mOctaveLayers;
  std::string mDiffusivity;

};


/*----------------------------------------------------------------*/


class TL_EXPORT KazeDetectorDescriptor
  : public KazeProperties,
    public KeypointDetector,
    public DescriptorExtractor
{

public:

  KazeDetectorDescriptor();
  KazeDetectorDescriptor(const KazeDetectorDescriptor &kazeDetectorDescriptor);
  KazeDetectorDescriptor(bool extendedDescriptor,
                         bool uprightDescriptor,
                         double threshold,
                         int octaves,
                         int octaveLayers,
                         const std::string &diffusivity);
  ~KazeDetectorDescriptor() override = default;

private:

#if CV_VERSION_MAJOR >= 4
  cv::KAZE::DiffusivityType convertDiffusivity(const std::string &diffusivity);
#else
  int convertDiffusivity(const std::string &diffusivity);
#endif
  void updateCvKaze();

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

// Kaze interface

public:

  void setExtendedDescriptor(bool extended) override;
  void setUprightDescriptor(bool uprightDescriptor) override;
  void setThreshold(double threshold) override;
  void setOctaves(int octaves) override;
  void setOctaveLayers(int octaveLayers) override;
  void setDiffusivity(const std::string &diffusivity) override;

// Feature interface

public:

  void reset() override;

protected:

  cv::Ptr<cv::KAZE> mKaze;
};

/*! \} */ // end of FeatureDetectorAndDescriptor

/*! \} */ // end of Features

} // namespace tl

#endif // TL_KAZE_DETECTOR_DESCRIPTOR_H
