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

#ifndef TL_FEATMATCH_MSER_DETECTOR_H
#define TL_FEATMATCH_MSER_DETECTOR_H

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

  std::vector<cv::KeyPoint> detect(const cv::Mat &img,
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

#endif // TL_FEATMATCH_MSER_DETECTOR_H
