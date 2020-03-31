#ifndef TL_AKAZE_DETECTOR_DESCRIPTOR_H
#define TL_AKAZE_DETECTOR_DESCRIPTOR_H

#include "config_tl.h"

#include "tidop/featmatch/features.h"

namespace tl
{


/*!
 * \brief AKAZE detector/descriptor properties class
 */
class TL_EXPORT AkazeProperties
  : public IAkaze
{

public:

  AkazeProperties();
  ~AkazeProperties() override;

// IAkaze interface

public:

  virtual std::string descriptorType() const override;
  virtual int descriptorSize() const override;
  virtual int descriptorChannels() const override;
  virtual double threshold() const override;
  virtual int octaves() const override;
  virtual int octaveLayers() const override;
  virtual std::string diffusivity() const override;
  virtual void setDescriptorType(const std::string &descriptorType) override;
  virtual void setDescriptorSize(int descriptorSize) override;
  virtual void setDescriptorChannels(int channels) override;
  virtual void setThreshold(double threshold) override;
  virtual void setOctaves(int octaves) override;
  virtual void setOctaveLayers(int octaveLayers) override;
  virtual void setDiffusivity(const std::string &diffusivity) override;

// Feature interface

public:

  virtual void reset() override;
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
  AkazeDetectorDescriptor(const std::string &descriptorType,
                          int descriptorSize,
                          int descriptorChannels,
                          double threshold,
                          int octaves,
                          int octaveLayers,
                          std::string diffusivity);

  ~AkazeDetectorDescriptor() override;

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


// IAkaze interface

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

} // namespace tl

#endif // TL_AKAZE_DETECTOR_DESCRIPTOR_H
