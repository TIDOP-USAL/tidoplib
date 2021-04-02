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

#include "gsm.h"

#include "tidop/core/messages.h"

namespace tl
{

GmsProperties::GmsProperties()
  : mRotation(true),
    mScale(true),
    mThreshold(6.)
{

}

void GmsProperties::reset()
{
  mRotation = true;
  mScale = true;
  mThreshold = 6.;
}

std::string GmsProperties::name() const
{
  return std::string("GMS");
}

bool GmsProperties::rotation() const
{
  return mRotation;
}

void GmsProperties::setRotation(bool rotation)
{
  mRotation = rotation;
}

bool GmsProperties::scale() const
{
  return mScale;
}

void GmsProperties::setScale(bool scale)
{
  mScale = scale;
}

double GmsProperties::threshold() const
{
  return mThreshold;
}

void GmsProperties::setThreshold(double threshold)
{
  mThreshold = threshold;
}

/*----------------------------------------------------------------*/


GsmImp::GsmImp(const std::shared_ptr<DescriptorMatcher> &descriptorMatcher)
  : GmsProperties(),
    mDescriptorMatcher(descriptorMatcher)
{
}

GsmImp::GsmImp(const std::shared_ptr<DescriptorMatcher> &descriptorMatcher,
                         bool rotation,
                         bool scale,
                         double threshold)
  : GmsProperties(),
    mDescriptorMatcher(descriptorMatcher)
{
  this->setRotation(rotation);
  this->setScale(scale);
  this->setThreshold(threshold);
}

bool GsmImp::compute(const cv::Mat &queryDescriptor,
                     const cv::Mat &trainDescriptor,
                     const std::vector<cv::KeyPoint> &keypoints1,
                     const std::vector<cv::KeyPoint> &keypoints2,
                     std::vector<cv::DMatch> *goodMatches,
                     std::vector<cv::DMatch> *wrongMatches,
                     const cv::Size &queryImageSize,
                     const cv::Size &trainImageSize)
{
#ifdef HAVE_OPENCV_XFEATURES2D 

#if CV_VERSION_MAJOR >= 4 || (CV_VERSION_MAJOR >= 3 && CV_VERSION_MINOR >= 4 && CV_VERSION_REVISION >= 1 )
  try {

    if (goodMatches == nullptr) return true;

    std::vector<cv::DMatch> matches;
    bool err = mDescriptorMatcher->match(queryDescriptor, trainDescriptor, matches);
    if (err) return true;

    cv::xfeatures2d::matchGMS(queryImageSize, trainImageSize, keypoints1, keypoints2, matches, *goodMatches);

    for (size_t i = 0; i < matches.size(); i++) {
      bool bWrong = true;
      for (size_t j = 0; j < goodMatches->size(); j++) {
        if (matches[i].queryIdx == (*goodMatches)[j].queryIdx &&
            matches[i].trainIdx == (*goodMatches)[j].trainIdx) {
          bWrong = false;
          break;
        }
      }
      if (bWrong) {
        wrongMatches->push_back(matches[i]);
      }
    }

    TL_TODO("devolver wrongMatches")

  } catch(std::exception &e){
    msgError(e.what());
    return true;
  }

#  else
  TL_COMPILER_WARNING("'matchGMS' not supported in OpenCV versions < 3.3.1")
  throw std::exception("'matchGMS' not supported in OpenCV versions < 3.3.1");
#endif
#else
  TL_COMPILER_WARNING("OpenCV not built with extra modules. 'matchGMS' not supported")
  throw std::exception("OpenCV not built with extra modules. 'matchGMS' not supported");
#endif // HAVE_OPENCV_XFEATURES2D

  return false;
}

} // namespace tl
