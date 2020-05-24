#ifndef TL_BOOST_DESCRIPTOR_H
#define TL_BOOST_DESCRIPTOR_H

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


class TL_EXPORT BoostProperties
  : public Boost
{

public:

  BoostProperties();
  BoostProperties(const BoostProperties &boostProperties);
  ~BoostProperties() override = default;

// Boost interface

public:

  std::string descriptorType() const override;
  bool useOrientation() const override;
  double scaleFactor() const override;
  void setDescriptorType(const std::string &descriptorType) override;
  void setUseOrientation(bool useOrientation) override;
  void setScaleFactor(double scaleFactor) override;

// Feature interface

public:

  void reset() override;
  std::string name() const override;

private:

  std::string mDescriptorType;
  bool bUseOrientation;
  double mScaleFactor;

};


/*----------------------------------------------------------------*/

class BoostDescriptor
  : public BoostProperties,
    public DescriptorExtractor
{

public:

  BoostDescriptor();
  BoostDescriptor(const BoostDescriptor &boostDescriptor);
  BoostDescriptor(const std::string &descriptorType,
                  bool useOrientation,
                  double scaleFactor);
  ~BoostDescriptor() override = default;

private:

  void update();

// Feature interface

public:

  void reset() override;

// Boost interface

public:

  void setDescriptorType(const std::string &descriptorType) override;
  void setUseOrientation(bool useOrientation) override;
  void setScaleFactor(double scaleFactor) override;

// DescriptorExtractor interface

public:

  bool extract(const cv::Mat &img,
               std::vector<cv::KeyPoint> &keyPoints,
               cv::Mat &descriptors) override;

protected:

#if CV_VERSION_MAJOR >= 4 || (CV_VERSION_MAJOR >= 3 && CV_VERSION_MINOR > 2)
  cv::Ptr<cv::xfeatures2d::BoostDesc> mBoost;
#endif

};

/*! \} */ // end of FeatureDetectorAndDescriptor

/*! \} */ // end of Features

} // namespace tl

#endif // TL_BOOST_DESCRIPTOR_H
