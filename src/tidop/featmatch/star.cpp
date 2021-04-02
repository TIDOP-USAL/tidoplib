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

#include "star.h"

#include "tidop/core/messages.h"


namespace tl
{


StarProperties::StarProperties()
  : mMaxSize(45),
    mResponseThreshold(30),
    mLineThresholdProjected(10),
    mLineThresholdBinarized(8),
    mSuppressNonmaxSize(5)
{
}

StarProperties::StarProperties(const StarProperties &starProperties)
  : Star(starProperties),
    mMaxSize(starProperties.mMaxSize),
    mResponseThreshold(starProperties.mResponseThreshold),
    mLineThresholdProjected(starProperties.mLineThresholdProjected),
    mLineThresholdBinarized(starProperties.mLineThresholdBinarized),
    mSuppressNonmaxSize(starProperties.mSuppressNonmaxSize)
{
}

int StarProperties::maxSize() const
{
  return mMaxSize;
}

int StarProperties::responseThreshold() const
{
  return mResponseThreshold;
}

int StarProperties::lineThresholdProjected() const
{
  return mLineThresholdProjected;
}

int StarProperties::lineThresholdBinarized() const
{
  return mLineThresholdBinarized;
}

int StarProperties::suppressNonmaxSize() const
{
  return mSuppressNonmaxSize;
}

void StarProperties::setMaxSize(int maxSize)
{
  mMaxSize = maxSize;
}

void StarProperties::setResponseThreshold(int responseThreshold)
{
  mResponseThreshold = responseThreshold;
}

void StarProperties::setLineThresholdProjected(int lineThresholdProjected)
{
  mLineThresholdProjected = lineThresholdProjected;
}

void StarProperties::setLineThresholdBinarized(int lineThresholdBinarized)
{
  mLineThresholdBinarized = lineThresholdBinarized;
}

void StarProperties::setSuppressNonmaxSize(int suppressNonmaxSize)
{
  mSuppressNonmaxSize = suppressNonmaxSize;
}

void StarProperties::reset()
{
  mMaxSize = 45;
  mResponseThreshold = 30;
  mLineThresholdProjected = 10;
  mLineThresholdBinarized = 8;
  mSuppressNonmaxSize = 5;
}

std::string StarProperties::name() const
{
  return std::string("STAR");
}


/*----------------------------------------------------------------*/


StarDetector::StarDetector()
{
  update();
}

StarDetector::StarDetector(const StarDetector &starDetector)
  : StarProperties(starDetector)
{
  update();
}

StarDetector::StarDetector(int maxSize,
                           int responseThreshold,
                           int lineThresholdProjected,
                           int lineThresholdBinarized,
                           int suppressNonmaxSize)
{
  StarProperties::setMaxSize(maxSize);
  StarProperties::setResponseThreshold(responseThreshold);
  StarProperties::setLineThresholdProjected(lineThresholdProjected);
  StarProperties::setLineThresholdBinarized(lineThresholdBinarized);
  StarProperties::setSuppressNonmaxSize(suppressNonmaxSize);
  update();
}

void StarDetector::update()
{
#ifdef HAVE_OPENCV_XFEATURES2D
  mSTAR = cv::xfeatures2d::StarDetector::create(StarProperties::maxSize(),
                                                StarProperties::responseThreshold(),
                                                StarProperties::lineThresholdProjected(),
                                                StarProperties::lineThresholdBinarized(),
                                                StarProperties::suppressNonmaxSize());
#endif // HAVE_OPENCV_XFEATURES2D
}

std::vector<cv::KeyPoint> StarDetector::detect(const cv::Mat &img, cv::InputArray &mask)
{
  std::vector<cv::KeyPoint> keyPoints;

#ifdef HAVE_OPENCV_XFEATURES2D 
  mSTAR->detect(img, keyPoints, mask);
#else
  TL_COMPILER_WARNING("OpenCV not built with extra modules. Star Detector not supported")
  throw std::exception("OpenCV not built with extra modules. Star Detector not supported");
#endif // HAVE_OPENCV_XFEATURES2D

  return keyPoints;
}

void StarDetector::setMaxSize(int maxSize)
{
  StarProperties::setMaxSize(maxSize);
  update();
}

void StarDetector::setResponseThreshold(int responseThreshold)
{
  StarProperties::setResponseThreshold(responseThreshold);
  update();
}

void StarDetector::setLineThresholdProjected(int lineThresholdProjected)
{
  StarProperties::setLineThresholdProjected(lineThresholdProjected);
  update();
}

void StarDetector::setLineThresholdBinarized(int lineThresholdBinarized)
{
  StarProperties::setLineThresholdBinarized(lineThresholdBinarized);
  update();
}

void StarDetector::setSuppressNonmaxSize(int suppressNonmaxSize)
{
  StarProperties::setSuppressNonmaxSize(suppressNonmaxSize);
  update();
}

void StarDetector::reset()
{
  StarProperties::reset();
  update();
}


} // namespace tl
