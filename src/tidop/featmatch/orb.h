#ifndef TL_ORB_DETECTOR_DESCRIPTOR_H
#define TL_ORB_DETECTOR_DESCRIPTOR_H

#include "config_tl.h"

#include "tidop/featmatch/features.h"

#ifdef HAVE_CUDA
#include <opencv2/cudafeatures2d.hpp>
#endif // HAVE_CUDA

namespace tl
{



class TL_EXPORT OrbProperties
  : public IOrb
{
public:

  OrbProperties();
   ~OrbProperties() override;

// IOrb interface

public:

  virtual int featuresNumber() const override;
  virtual double scaleFactor() const override;
  virtual int levelsNumber() const override;
  virtual int edgeThreshold() const override;
  virtual int wta_k() const override;
  virtual std::string scoreType() const override;
  virtual int patchSize() const override;
  virtual int fastThreshold() const override;
  virtual void setFeaturesNumber(int featuresNumber) override;
  virtual void setScaleFactor(double scaleFactor) override;
  virtual void setLevelsNumber(int levelsNumber) override;
  virtual void setEdgeThreshold(int edgeThreshold) override;
  virtual void setWTA_K(int WTA_K) override;
  virtual void setScoreType(const std::string &scoreType) override;
  virtual void setPatchSize(int patchSize) override;
  virtual void setFastThreshold(int fastThreshold) override;

// Feature interface

public:

  void reset() override;
  std::string name() const final;

private:

  int mFeaturesNumber;
  double mScaleFactor;
  int mLevelsNumber;
  int mEdgeThreshold;
  int mWTA_K;
  std::string mScoreType;
  int mPatchSize;
  int mFastThreshold;
};


/*----------------------------------------------------------------*/


class TL_EXPORT OrbDetectorDescriptor
  : public OrbProperties,
    public KeypointDetector,
    public DescriptorExtractor
{

public:

  OrbDetectorDescriptor();
  OrbDetectorDescriptor(int featuresNumber,
                        double scaleFactor,
                        int levelsNumber,
                        int edgeThreshold,
                        int wta_k,
                        std::string scoreType,
                        int patchSize,
                        int fastThreshold);

  ~OrbDetectorDescriptor() override;

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

// IOrb interface

public:

  void setFeaturesNumber(int featuresNumber) override;
  void setScaleFactor(double scaleFactor) override;
  void setLevelsNumber(int levelsNumber) override;
  void setEdgeThreshold(int edgeThreshold) override;
  void setWTA_K(int WTA_K) override;
  void setScoreType(const std::string &scoreType) override;
  void setPatchSize(int patchSize) override;
  void setFastThreshold(int fastThreshold) override;

// Feature interface

public:

  void reset() override;

protected:

  cv::Ptr<cv::ORB> mOrb;
};



/*----------------------------------------------------------------*/

#ifdef HAVE_CUDA

class TL_EXPORT OrbCudaDetectorDescriptor
  : public OrbProperties,
    public KeypointDetector,
    public DescriptorExtractor
{

public:

  OrbCudaDetectorDescriptor();
  OrbCudaDetectorDescriptor(int featuresNumber,
                            double scaleFactor,
                            int levelsNumber,
                            int edgeThreshold,
                            int wta_k,
                            std::string scoreType,
                            int patchSize,
                            int fastThreshold);

  ~OrbCudaDetectorDescriptor() override;

private:

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

// IOrb interface

public:

  void setFeaturesNumber(int featuresNumber) override;
  void setScaleFactor(double scaleFactor) override;
  void setLevelsNumber(int levelsNumber) override;
  void setEdgeThreshold(int edgeThreshold) override;
  void setWTA_K(int WTA_K) override;
  void setScoreType(const std::string &scoreType) override;
  void setPatchSize(int patchSize) override;
  void setFastThreshold(int fastThreshold) override;

// Feature interface

public:

  void reset() override;

protected:

  cv::Ptr<cv::cuda::ORB> mOrb;
};

#endif // HAVE_CUDA

} // namespace tl



#endif // TL_ORB_DETECTOR_DESCRIPTOR_H
