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

#ifndef TL_FEATMATCH_SURF_DETECTOR_DESCRIPTOR_H
#define TL_FEATMATCH_SURF_DETECTOR_DESCRIPTOR_H

#include "config_tl.h"

#include "tidop/featmatch/features.h"

#include <opencv2/xfeatures2d.hpp>
#ifdef HAVE_OPENCV_CUDAFEATURES2D
#include <opencv2/cudafeatures2d.hpp>
#include <opencv2/xfeatures2d/cuda.hpp>
#endif // HAVE_OPENCV_CUDAFEATURES2D


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


class TL_EXPORT SurfProperties
  : public Surf
{

public:

  SurfProperties();
  SurfProperties(const SurfProperties &surfProperties);
  ~SurfProperties() override = default;

  // Surf interface

public:

  double hessianThreshold() const override;
  int octaves() const override;
  int octaveLayers() const override;
  bool extendedDescriptor() const override;
  bool upright() const override;
  void setHessianThreshold(double hessianThreshold) override;
  void setOctaves(int octaves) override;
  void setOctaveLayers(int octaveLayers) override;
  void setExtendedDescriptor(bool extendedDescriptor) override;
  void setUpright(bool upright) override;

// Feature interface

public:

  void reset() override;
  std::string name() const final;

private:

  double mHessianThreshold;
  int mOctaves;
  int mOctaveLayers;
  bool mExtendedDescriptor;
  bool mUpright;
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
  SurfDetectorDescriptor(const SurfDetectorDescriptor &surfDetectorDescriptor);
  SurfDetectorDescriptor(double hessianThreshold,
                         int octaves,
                         int octaveLayers,
                         bool extendedDescriptor,
                         bool upright);

  ~SurfDetectorDescriptor() override = default;

// KeypointDetector interface

public:

  std::vector<cv::KeyPoint> detect(const cv::Mat &img,
                                   cv::InputArray &mask = cv::noArray()) override;

// DescriptorExtractor interface

public:

  cv::Mat extract(const cv::Mat &img,
                  std::vector<cv::KeyPoint> &keyPoints) override;

// Surf interface

public:

  void setHessianThreshold(double hessianThreshold) override;
  void setOctaves(int octaves) override;
  void setOctaveLayers(int octaveLayers) override;
  void setExtendedDescriptor(bool extendedDescriptor) override;
  void setUpright(bool upright) override;

// Feature interface

public:

  void reset() override;

protected:

  cv::Ptr<cv::xfeatures2d::SURF> mSurf;
};


/*----------------------------------------------------------------*/

#ifdef HAVE_OPENCV_CUDAFEATURES2D

class TL_EXPORT SurfCudaDetectorDescriptor
  : public SurfProperties,
    public KeypointDetector,
    public DescriptorExtractor
{

public:

  SurfCudaDetectorDescriptor();
  SurfCudaDetectorDescriptor(const SurfCudaDetectorDescriptor &surfDetectorDescriptor);
  SurfCudaDetectorDescriptor(double hessianThreshold,
                             int octaves,
                             int octaveLayers,
                             bool extendedDescriptor,
                             bool upright);
  ~SurfCudaDetectorDescriptor() override = default;

// KeypointDetector interface

public:

  std::vector<cv::KeyPoint> detect(const cv::Mat &img,
                                   cv::InputArray &mask = cv::noArray()) override;

// DescriptorExtractor interface

public:

  cv::Mat extract(const cv::Mat &img,
                  std::vector<cv::KeyPoint> &keyPoints) override;

// Surf interface

public:

  void setHessianThreshold(double hessianThreshold) override;
  void setOctaves(int octaves) override;
  void setOctaveLayers(int octaveLayers) override;
  void setExtendedDescriptor(bool extendedDescriptor) override;
  void setUpright(bool upright) override;

// Feature interface

public:

  void reset() override;

protected:

  std::unique_ptr<cv::cuda::SURF_CUDA> mSurf;
};


#endif // HAVE_OPENCV_CUDAFEATURES2D

#endif // OPENCV_ENABLE_NONFREE


/*! \} */ // end of FeatureDetectorAndDescriptor

/*! \} */ // end of Features

} // namespace tl

#endif // TL_FEATMATCH_SURF_DETECTOR_DESCRIPTOR_H
