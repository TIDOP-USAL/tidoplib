#ifndef TL_MSER_DETECTOR_H
#define TL_MSER_DETECTOR_H

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


class TL_EXPORT MserProperties
  : public Mser
{
public:

  MserProperties();
  MserProperties(const MserProperties &mserProperties);
  ~MserProperties() override = default;

// Mser interface

public:

  int delta() const override;
  int minArea() const override;
  int maxArea() const override;
  double maxVariation() const override;
  double minDiversity() const override;
  int maxEvolution() const override;
  double areaThreshold() const override;
  double minMargin() const override;
  int edgeBlurSize() const override;
  void setDelta(int delta) override;
  void setMinArea(int minArea) override;
  void setMaxArea(int maxArea) override;
  void setMaxVariation(double maxVariation) override;
  void setMinDiversity(double minDiversity) override;
  void setMaxEvolution(int maxEvolution) override;
  void setAreaThreshold(double areaThreshold) override;
  void setMinMargin(double minMargin) override;
  void setEdgeBlurSize(int edgeBlurSize) override;

// Feature interface

public:

  virtual void reset() override;
  std::string name() const final;

private:

  int mDelta;
  int mMinArea;
  int mMaxArea;
  double mMaxVariation;
  double mMinDiversity;
  int mMaxEvolution;
  double mAreaThreshold;
  double mMinMargin;
  int mEdgeBlurSize;
};


/*----------------------------------------------------------------*/


class TL_EXPORT MserDetector
  : public MserProperties,
    public KeypointDetector
{

public:

  MserDetector();
  MserDetector(const MserDetector &mserDetector);
  MserDetector(int delta,
               int minArea,
               int maxArea,
               double maxVariation,
               double minDiversity,
               int maxEvolution,
               double areaThreshold,
               double minMargin,
               int edgeBlurSize);
  ~MserDetector() override = default;

private:

  void update();

// KeypointDetector interface

public:

  bool detect(const cv::Mat &img,
              std::vector<cv::KeyPoint> &keyPoints,
              cv::InputArray &mask = cv::noArray()) override;

// Mser interface

public:

  void setDelta(int delta) override;
  void setMinArea(int minArea) override;
  void setMaxArea(int maxArea) override;
  void setMaxVariation(double maxVariation) override;
  void setMinDiversity(double minDiversity) override;
  void setMaxEvolution(int maxEvolution) override;
  void setAreaThreshold(double areaThreshold) override;
  void setMinMargin(double minMargin) override;
  void setEdgeBlurSize(int edgeBlurSize) override;

// Feature interface

public:

  void reset() override;

protected:

  cv::Ptr<cv::MSER> mMSER;

};

/*! \} */ // end of FeatureDetectorAndDescriptor

/*! \} */ // end of Features

} // namespace tl

#endif // TL_MSER_DETECTOR_H
