#ifndef TL_AKAZE_DETECTOR_DESCRIPTOR_H
#define TL_AKAZE_DETECTOR_DESCRIPTOR_H

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


class TL_EXPORT AkazeProperties
  : public Akaze
{

public:

  AkazeProperties();
  AkazeProperties(const AkazeProperties &akazeProperties);
  ~AkazeProperties() override = default;

// Akaze interface

public:

  std::string descriptorType() const override;
  int descriptorSize() const override;
  int descriptorChannels() const override;
  double threshold() const override;
  int octaves() const override;
  int octaveLayers() const override;
  std::string diffusivity() const override;
  void setDescriptorType(const std::string &descriptorType) override;
  void setDescriptorSize(int descriptorSize) override;
  void setDescriptorChannels(int channels) override;
  void setThreshold(double threshold) override;
  void setOctaves(int octaves) override;
  void setOctaveLayers(int octaveLayers) override;
  void setDiffusivity(const std::string &diffusivity) override;

// Feature interface

public:

  void reset() override;
  std::string name() const final;

private:

  std::string mDescriptorType;
  int mDescriptorSize;
  int mDescriptorChannels;
  double mThreshold;
  int mOctaves;
  int mOctaveLayers;
  std::string mDiffusivity;

};


/*----------------------------------------------------------------*/


class TL_EXPORT AkazeDetectorDescriptor
  : public AkazeProperties,
    public KeypointDetector,
    public DescriptorExtractor
{

public:

  AkazeDetectorDescriptor();
  AkazeDetectorDescriptor(const AkazeDetectorDescriptor &akazeDetectorDescriptor);
  AkazeDetectorDescriptor(const std::string &descriptorType,
                          int descriptorSize,
                          int descriptorChannels,
                          double threshold,
                          int octaves,
                          int octaveLayers,
                          const std::string &diffusivity);

  ~AkazeDetectorDescriptor() override = default;

private:

#if CV_VERSION_MAJOR >= 4
  cv::AKAZE::DescriptorType convertDescriptorType(const std::string &descriptorType);
  cv::KAZE::DiffusivityType convertDiffusivity(const std::string &descriptorType);
#else
  int convertDescriptorType(const std::string &descriptorType);
  int convertDiffusivity(const std::string &diffusivity);
#endif

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


// Akaze interface

public:

  void setDescriptorType(const std::string &descriptorType) override;
  void setDescriptorSize(int descriptorSize) override;
  void setDescriptorChannels(int channels) override;
  void setThreshold(double threshold) override;
  void setOctaves(int octaves) override;
  void setOctaveLayers(int octaveLayers) override;
  void setDiffusivity(const std::string &diffusivity) override;

// Feature interface

public:

  void reset() override;

protected:

  cv::Ptr<cv::AKAZE> mAkaze;

};

/*! \} */ // end of FeatureDetectorAndDescriptor

/*! \} */ // end of Features

} // namespace tl

#endif // TL_AKAZE_DETECTOR_DESCRIPTOR_H
