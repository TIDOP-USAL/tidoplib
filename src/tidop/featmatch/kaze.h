#ifndef TL_KAZE_DETECTOR_DESCRIPTOR_H
#define TL_KAZE_DETECTOR_DESCRIPTOR_H

#include "config_tl.h"

#include "tidop/featmatch/features.h"

namespace tl
{


class TL_EXPORT KazeProperties
  : public IKaze
{
public:

  KazeProperties();
  ~KazeProperties() override;

// IKaze interface

public:

  virtual bool extendedDescriptor() const override;
  virtual bool upright() const override;
  virtual double threshold() const override;
  virtual int octaves() const override;
  virtual int octaveLayers() const override;
  virtual std::string diffusivity() const override;
  virtual void setExtendedDescriptor(bool extended) override;
  virtual void setUpright(bool upright) override;
  virtual void setThreshold(double threshold) override;
  virtual void setOctaves(int octaves) override;
  virtual void setOctaveLayers(int octaveLayers) override;
  virtual void setDiffusivity(const std::string &diffusivity) override;

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
  KazeDetectorDescriptor(bool extendedDescriptor,
                         bool upright,
                         double threshold,
                         int octaves,
                         int octaveLayers,
                         std::string diffusivity);

  ~KazeDetectorDescriptor() override;

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

// IKaze interface

public:

  void setExtendedDescriptor(bool extended) override;
  void setUpright(bool upright) override;
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

} // namespace tl

#endif // TL_KAZE_DETECTOR_DESCRIPTOR_H
