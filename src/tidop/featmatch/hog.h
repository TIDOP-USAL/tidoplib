#ifndef TL_HOG_DESCRIPTOR_H
#define TL_HOG_DESCRIPTOR_H

#include "config_tl.h"

#include "tidop/featmatch/features.h"

#include <memory>

#include <opencv2/objdetect.hpp>

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


class TL_EXPORT HogProperties
  : public Hog
{
public:

  HogProperties();
  HogProperties(const HogProperties &hogProperties);
  ~HogProperties() override = default;

// Hog interface

public:

  Size<int> winSize() const override;
  Size<int> blockSize() const override;
  Size<int> blockStride() const override;
  Size<int> cellSize() const override;
  int nbins() const override;
  int derivAperture() const override;
  void setWinSize(const Size<int> &winSize) override;
  void setBlockSize(const Size<int> &blockSize) override;
  void setBlockStride(const Size<int> &blockStride) override;
  void setCellSize(const Size<int> &cellSize) override;
  void setNbins(int nbins) override;
  void setDerivAperture(int derivAperture) override;

// Feature interface

public:

  virtual void reset() override;
  std::string name() const final;

private:

  Size<int> mWinSize;
  Size<int> mBlockSize;
  Size<int> mBlockStride;
  Size<int> mCellSize;
  int mNbins;
  int mDerivAperture;
};


/*----------------------------------------------------------------*/


class TL_EXPORT HogDescriptor
    : public HogProperties,
      public DescriptorExtractor
{

public:

  HogDescriptor();
  HogDescriptor(const HogDescriptor &hogDescriptor);
  HogDescriptor(Size<int> winSize,
                Size<int> blockSize,
                Size<int> blockStride,
                Size<int> cellSize,
                int nbins,
                int derivAperture);
  ~HogDescriptor() override = default;

private:

  void update();
  void normalizepatch(const cv::Mat &gray, 
                      const cv::KeyPoint &keypoint, 
                      cv::Mat &output);

// DescriptorExtractor interface

public:

  bool extract(const cv::Mat &img,
               std::vector<cv::KeyPoint> &keyPoints,
               cv::Mat &descriptors) override;

// Hog interface

public:

  void setWinSize(const Size<int> &winSize) override;
  void setBlockSize(const Size<int> &blockSize) override;
  void setBlockStride(const Size<int> &blockStride) override;
  void setCellSize(const Size<int> &cellSize) override;
  void setNbins(int nbins) override;
  void setDerivAperture(int derivAperture) override;

// Feature interface

public:

  void reset() override;

protected:

  std::shared_ptr<cv::HOGDescriptor> mHOG;
};

/*! \} */ // end of FeatureDetectorAndDescriptor

/*! \} */ // end of Features

} // namespace tl

#endif // TL_HOG_DESCRIPTOR_H
