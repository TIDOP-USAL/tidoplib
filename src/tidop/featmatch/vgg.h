#ifndef TL_VGG_DESCRIPTOR_H
#define TL_VGG_DESCRIPTOR_H

#include "config_tl.h"

#include "tidop/featmatch/features.h"

#include <opencv2/xfeatures2d.hpp>


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


class TL_EXPORT VggProperties
  : public Vgg
{

public:

  VggProperties();
  VggProperties(const VggProperties &vggProperties);
  ~VggProperties() override = default;

// Vgg interface

public:

  std::string descriptorType() const override;
  void setDescriptorType(const std::string &descriptorType) override;
  double scaleFactor() const override;
  void setScaleFactor(double scaleFactor) override;
  double sigma() const override;
  void setSigma(double sigma) override;
  bool useNormalizeDescriptor() const override;
  void setUseNormalizeDescriptor(bool useNormalizeDescriptor) override;
  bool useNormalizeImage() const override;
  void setUseNormalizeImage(bool useNormalizeImage) override;
  bool useScaleOrientation() const override;
  void setUseScaleOrientation(bool useScaleOrientation) override;

// Feature interface

public:

  void reset() override;
  std::string name() const override;

private:

  std::string mDescriptorType;
  double mScaleFactor;
  double mSigma;
  bool bUseNormalizeDescriptor;
  bool bUseNormalizeImage;
  bool bUseScaleOrientation;

};



/*----------------------------------------------------------------*/


class TL_EXPORT VggDescriptor
  : public VggProperties,
    public DescriptorExtractor
{

public:

  VggDescriptor();
  VggDescriptor(const VggDescriptor &vggDescriptor);
  VggDescriptor(std::string descriptorType,
                double scaleFactor,
                double sigma,
                bool useNormalizeDescriptor,
                bool useNormalizeImage,
                bool useScaleOrientation);
  ~VggDescriptor() override = default;

private:

  void update();

// Feature interface

public:

  void reset() override;

// Vgg interface

public:

  void setDescriptorType(const std::string &descriptorType) override;
  void setScaleFactor(double scaleFactor) override;
  void setSigma(double sigma) override;
  void setUseNormalizeDescriptor(bool useNormalizeDescriptor) override;
  void setUseNormalizeImage(bool useNormalizeImage) override;
  void setUseScaleOrientation(bool useScaleOrientation) override;

// DescriptorExtractor interface

public:

  bool extract(const cv::Mat &img, std::vector<cv::KeyPoint> &keyPoints, cv::Mat &descriptors) override;

protected:

#if CV_VERSION_MAJOR >= 4 || (CV_VERSION_MAJOR >= 3 && CV_VERSION_MINOR > 2)
  cv::Ptr<cv::xfeatures2d::VGG> mVGG;
#endif
};

/*! \} */ // end of FeatureDetectorAndDescriptor

/*! \} */ // end of Features

} // namespace tl

#endif // TL_VGG_DESCRIPTOR_H
