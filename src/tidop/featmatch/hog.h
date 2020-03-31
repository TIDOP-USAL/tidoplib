#ifndef TL_HOG_DESCRIPTOR_H
#define TL_HOG_DESCRIPTOR_H

#include "config_tl.h"

#include "tidop/featmatch/features.h"

#include <memory>

#include <opencv2/objdetect.hpp>

namespace tl
{


class TL_EXPORT HogProperties
  : public IHog
{
public:

  HogProperties();
  ~HogProperties() override;

// IHog interface

public:

  virtual cv::Size winSize() const override;
  virtual cv::Size blockSize() const override;
  virtual cv::Size blockStride() const override;
  virtual cv::Size cellSize() const override;
  virtual int nbins() const override;
  virtual int derivAperture() const override;
  virtual void setWinSize(const cv::Size &winSize) override;
  virtual void setBlockSize(const cv::Size &blockSize) override;
  virtual void setBlockStride(const cv::Size &blockStride) override;
  virtual void setCellSize(const cv::Size &cellSize) override;
  virtual void setNbins(int nbins) override;
  virtual void setDerivAperture(int derivAperture) override;

// Feature interface

public:

  virtual void reset() override;
  std::string name() const final;

private:

  cv::Size mWinSize;
  cv::Size mBlockSize;
  cv::Size mBlockStride;
  cv::Size mCellSize;
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
  HogDescriptor(cv::Size winSize,
                cv::Size blockSize,
                cv::Size blockStride,
                cv::Size cellSize,
                int nbins,
                int derivAperture);

  ~HogDescriptor() override;

private:

  void update();
  void normalizepatch(const cv::Mat &gray, const cv::KeyPoint &keypoint, cv::Mat &output);

// DescriptorExtractor interface

public:

  bool extract(const cv::Mat &img,
               std::vector<cv::KeyPoint> &keyPoints,
               cv::Mat &descriptors) override;

// IHog interface

public:

  void setWinSize(const cv::Size &winSize) override;
  void setBlockSize(const cv::Size &blockSize) override;
  void setBlockStride(const cv::Size &blockStride) override;
  void setCellSize(const cv::Size &cellSize) override;
  void setNbins(int nbins) override;
  void setDerivAperture(int derivAperture) override;

// Feature interface

public:

  void reset() override;

protected:

  std::shared_ptr<cv::HOGDescriptor> mHOG;
};


} // namespace tl

#endif // TL_HOG_DESCRIPTOR_H
