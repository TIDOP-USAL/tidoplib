#ifndef TL_SURF_DETECTOR_DESCRIPTOR_H
#define TL_SURF_DETECTOR_DESCRIPTOR_H

#include "config_tl.h"

#include "tidop/featmatch/features.h"

#include <opencv2/xfeatures2d.hpp>
#ifdef HAVE_CUDA
#include <opencv2/cudafeatures2d.hpp>
#include "opencv2/xfeatures2d/cuda.hpp"
#endif // HAVE_CUDA


namespace tl
{


/*!
 * \brief SURF detector/descriptor properties class
 */
class TL_EXPORT SurfProperties
  : public ISurf
{

public:

  SurfProperties();
  ~SurfProperties() override;

  // ISurf interface

public:

  virtual double hessianThreshold() const override;
  virtual int octaves() const override;
  virtual int octaveLayers() const override;
  virtual bool extendedDescriptor() const override;
  virtual bool rotatedFeatures() const override;
  virtual void setHessianThreshold(double hessianThreshold) override;
  virtual void setOctaves(int octaves) override;
  virtual void setOctaveLayers(int octaveLayers) override;
  virtual void setExtendedDescriptor(bool extendedDescriptor) override;
  virtual void setRotatedFeatures(bool rotatedFeatures) override;

// Feature interface

public:

  virtual void reset() override;
  std::string name() const final;

private:

  double mHessianThreshold;
  int mOctaves;
  int mOctaveLayers;
  bool mExtendedDescriptor;
  bool mRotatedFeatures;
};


/*----------------------------------------------------------------*/

#ifdef OPENCV_ENABLE_NONFREE

class TL_EXPORT SurfDetectorDescriptor
  : public SurfProperties,
    public KeypointDetector,
    public DescriptorExtractor
{

public:

  SurfDetectorDescriptor();
  SurfDetectorDescriptor(double hessianThreshold,
                         int octaves,
                         int octaveLayers,
                         bool extendedDescriptor,
                         bool rotatedFeatures);

  ~SurfDetectorDescriptor() override;

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

// ISurf interface

public:

  void setHessianThreshold(double hessianThreshold) override;
  void setOctaves(int octaves) override;
  void setOctaveLayers(int octaveLayers) override;
  void setExtendedDescriptor(bool extendedDescriptor) override;
  void setRotatedFeatures(bool rotatedFeatures) override;

// Feature interface

public:

  void reset() override;

protected:

  cv::Ptr<cv::xfeatures2d::SURF> mSurf;
};


/*----------------------------------------------------------------*/

#ifdef HAVE_CUDA

class TL_EXPORT SurfCudaDetectorDescriptor
  : public SurfProperties,
    public KeypointDetector,
    public DescriptorExtractor
{

public:

  SurfCudaDetectorDescriptor();
  SurfCudaDetectorDescriptor(double hessianThreshold,
                             int octaves,
                             int octaveLayers,
                             bool extendedDescriptor,
                             bool rotatedFeatures);
                             
  ~SurfCudaDetectorDescriptor() override;

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

// ISurf interface

public:

  void setHessianThreshold(double hessianThreshold) override;
  void setOctaves(int octaves) override;
  void setOctaveLayers(int octaveLayers) override;
  void setExtendedDescriptor(bool extendedDescriptor) override;
  void setRotatedFeatures(bool rotatedFeatures) override;

// Feature interface

public:

  void reset() override;

protected:

  std::unique_ptr<cv::cuda::SURF_CUDA> mSurf;
};


#endif // HAVE_CUDA


#endif // OPENCV_ENABLE_NONFREE

} // namespace tl

#endif // TL_SURF_DETECTOR_DESCRIPTOR_H
