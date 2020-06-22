#ifndef TL_ORB_DETECTOR_DESCRIPTOR_H
#define TL_ORB_DETECTOR_DESCRIPTOR_H

#include "config_tl.h"

#include "tidop/featmatch/features.h"

#ifdef HAVE_OPENCV_CUDAFEATURES2D
#include <opencv2/cudafeatures2d.hpp>
#endif // HAVE_CUDA

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


class TL_EXPORT OrbProperties
  : public Orb
{
public:

  OrbProperties();
  OrbProperties(const OrbProperties &orbProperties);
  ~OrbProperties() override = default;

// Orb interface

public:

  int featuresNumber() const override;
  double scaleFactor() const override;
  int levelsNumber() const override;
  int edgeThreshold() const override;
  int firstLevel() const override;
  int wta_k() const override;
  std::string scoreType() const override;
  int patchSize() const override;
  int fastThreshold() const override;
  void setFeaturesNumber(int featuresNumber) override;
  void setScaleFactor(double scaleFactor) override;
  void setLevelsNumber(int levelsNumber) override;
  void setEdgeThreshold(int edgeThreshold) override;
  void setFirstLevel(int firstLevel) override;
  void setWTA_K(int WTA_K) override;
  void setScoreType(const std::string &scoreType) override;
  void setPatchSize(int patchSize) override;
  void setFastThreshold(int fastThreshold) override;

// Feature interface

public:

  void reset() override;
  std::string name() const final;

private:

  int mFeaturesNumber;
  double mScaleFactor;
  int mLevelsNumber;
  int mEdgeThreshold;
  int mFirstLevel;
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
  OrbDetectorDescriptor(const OrbDetectorDescriptor &orbDetectorDescriptor);
  OrbDetectorDescriptor(int featuresNumber,
                        double scaleFactor,
                        int levelsNumber,
                        int edgeThreshold,
                        int wta_k,
                        const std::string &scoreType,
                        int patchSize,
                        int fastThreshold);

  ~OrbDetectorDescriptor() override = default;

private:

#if CV_VERSION_MAJOR >= 4
  cv::ORB::ScoreType convertScoreType(const std::string &scoreType);
#else
  int convertScoreType(const std::string &scoreType);
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

// Orb interface

public:

  void setFeaturesNumber(int featuresNumber) override;
  void setScaleFactor(double scaleFactor) override;
  void setLevelsNumber(int levelsNumber) override;
  void setEdgeThreshold(int edgeThreshold) override;
  void setFirstLevel(int firstLevel) override;
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

#ifdef HAVE_OPENCV_CUDAFEATURES2D

class TL_EXPORT OrbCudaDetectorDescriptor
  : public OrbProperties,
    public KeypointDetector,
    public DescriptorExtractor
{

public:

  OrbCudaDetectorDescriptor();
  OrbCudaDetectorDescriptor(const OrbCudaDetectorDescriptor &orbCudaDetectorDescriptor);
  OrbCudaDetectorDescriptor(int featuresNumber,
                            double scaleFactor,
                            int levelsNumber,
                            int edgeThreshold,
                            int wta_k,
                            const std::string &scoreType,
                            int patchSize,
                            int fastThreshold);

  ~OrbCudaDetectorDescriptor() override = default;

private:

#if CV_VERSION_MAJOR >= 4
  cv::ORB::ScoreType convertScoreType(const std::string &scoreType);
#else
  int convertScoreType(const std::string &scoreType);
#endif

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

// Orb interface

public:

  void setFeaturesNumber(int featuresNumber) override;
  void setScaleFactor(double scaleFactor) override;
  void setLevelsNumber(int levelsNumber) override;
  void setEdgeThreshold(int edgeThreshold) override;
  void setFirstLevel(int firstLevel) override;
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

#endif // HAVE_OPENCV_CUDAFEATURES2D

/*! \} */ // end of FeatureDetectorAndDescriptor

/*! \} */ // end of Features

} // namespace tl



#endif // TL_ORB_DETECTOR_DESCRIPTOR_H
