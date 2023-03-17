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
 * License along with TidopLib. If not, see <http://www.gnu.org/licenses>.*
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#ifndef TL_FEATMATCH_KAZE_DETECTOR_DESCRIPTOR_H
#define TL_FEATMATCH_KAZE_DETECTOR_DESCRIPTOR_H

#include "tidop/config.h"

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


class TL_EXPORT KazeProperties
  : public Kaze
{
public:

  KazeProperties();
  KazeProperties(const KazeProperties &kazeProperties);
  ~KazeProperties() override = default;

// Kaze interface

public:

  bool extendedDescriptor() const override;
  bool uprightDescriptor() const override;
  double threshold() const override;
  int octaves() const override;
  int octaveLayers() const override;
  std::string diffusivity() const override;
  void setExtendedDescriptor(bool extended) override;
  void setUprightDescriptor(bool uprightDescriptor) override;
  void setThreshold(double threshold) override;
  void setOctaves(int octaves) override;
  void setOctaveLayers(int octaveLayers) override;
  void setDiffusivity(const std::string &diffusivity) override;

// Feature interface

public:

  void reset() override;
  std::string name() const final;

private:

  bool mExtended;
  bool mUpright;
  double mThreshold;
  int mOctaves;
  int mOctaveLayers;
  std::string mDiffusivity;

};


/*----------------------------------------------------------------*/


class TL_EXPORT KazeDetectorDescriptor
  : public KazeProperties,
    public KeypointDetector,
    public DescriptorExtractor
{

public:

  KazeDetectorDescriptor();
  KazeDetectorDescriptor(const KazeDetectorDescriptor &kazeDetectorDescriptor);
  KazeDetectorDescriptor(bool extendedDescriptor,
                         bool uprightDescriptor,
                         double threshold,
                         int octaves,
                         int octaveLayers,
                         const std::string &diffusivity);
  ~KazeDetectorDescriptor() override = default;

private:

#if CV_VERSION_MAJOR >= 4
  cv::KAZE::DiffusivityType convertDiffusivity(const std::string &diffusivity);
#else
  int convertDiffusivity(const std::string &diffusivity);
#endif
  void updateCvKaze();

// KeypointDetector interface

public:

  std::vector<cv::KeyPoint> detect(const cv::Mat &img,
                                   cv::InputArray &mask = cv::noArray()) override;

// DescriptorExtractor interface

public:

  cv::Mat extract(const cv::Mat &img,
                  std::vector<cv::KeyPoint> &keyPoints) override;

// Kaze interface

public:

  void setExtendedDescriptor(bool extended) override;
  void setUprightDescriptor(bool uprightDescriptor) override;
  void setThreshold(double threshold) override;
  void setOctaves(int octaves) override;
  void setOctaveLayers(int octaveLayers) override;
  void setDiffusivity(const std::string &diffusivity) override;

// Feature interface

public:

  void reset() override;

protected:

  cv::Ptr<cv::KAZE> mKaze;
};

/*! \} */ // end of FeatureDetectorAndDescriptor

/*! \} */ // end of Features

} // namespace tl

#endif // TL_FEATMATCH_KAZE_DETECTOR_DESCRIPTOR_H
