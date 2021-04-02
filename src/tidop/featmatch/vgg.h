/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by Esteban Ruiz de Oña Crespo                       *
 *                                                                        *
 * This file is part of TidopLib                                          *
 *                                                                        *
 * TidopLib is free software: you can redistribute it and/or modify       *
 * it under the terms of the GNU Lesser General Public License as         *
 * published by the Free Software Foundation, either version 3 of the     *
 * License, or (at your option) any later version.                        *
 *                                                                        *
 * TidopLib is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU Lesser General Public License for more details.                    *
 *                                                                        *
 * You should have received a copy of the GNU Lesser General Public       *
 * License along with Foobar. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/
 
#ifndef TL_FEATMATCH_VGG_DESCRIPTOR_H
#define TL_FEATMATCH_VGG_DESCRIPTOR_H

#include "config_tl.h"

#include "tidop/featmatch/features.h"

#ifdef HAVE_OPENCV_XFEATURES2D 
#include <opencv2/xfeatures2d.hpp>
#endif // HAVE_OPENCV_XFEATURES2D


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

    cv::Mat extract(const cv::Mat &img,
                  std::vector<cv::KeyPoint> &keyPoints) override;

protected:

#ifdef HAVE_OPENCV_XFEATURES2D 
#if CV_VERSION_MAJOR >= 4 || (CV_VERSION_MAJOR >= 3 && CV_VERSION_MINOR > 2)
  cv::Ptr<cv::xfeatures2d::VGG> mVGG;
#endif
#endif // HAVE_OPENCV_XFEATURES2D
};

/*! \} */ // end of FeatureDetectorAndDescriptor

/*! \} */ // end of Features

} // namespace tl

#endif // TL_FEATMATCH_VGG_DESCRIPTOR_H
