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

#ifndef TL_FEATMATCH_BRIEF_DESCRIPTOR_H
#define TL_FEATMATCH_BRIEF_DESCRIPTOR_H

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


class TL_EXPORT BriefProperties
  : public Brief
{
public:

  BriefProperties();
  BriefProperties(const BriefProperties &briefProperties);
  ~BriefProperties() override = default;

// Brief interface

public:

  virtual std::string bytes() const override;
  virtual bool useOrientation() const override;
  virtual void setBytes(const std::string &bytes) override;
  virtual void setUseOrientation(bool useOrientation) override;

// Feature interface

public:

  virtual void reset() override;
  std::string name() const final;

private:

  std::string mBytes;
  bool mUseOrientation;
};


/*----------------------------------------------------------------*/


class TL_EXPORT BriefDescriptor
    : public BriefProperties,
      public DescriptorExtractor
{

public:

  BriefDescriptor();
  BriefDescriptor(const BriefDescriptor &briefDescriptor);
  BriefDescriptor(std::string bytes,
                  bool useOrientation);
  ~BriefDescriptor() override = default;

private:

  void update();

// DescriptorExtractor interface

public:

  cv::Mat extract(const cv::Mat &img,
                  std::vector<cv::KeyPoint> &keyPoints) override;


// Brief interface

public:

  void setBytes(const std::string &bytes) override;
  void setUseOrientation(bool useOrientation) override;

// Feature interface

public:

  void reset() override;

protected:

  cv::Ptr<cv::xfeatures2d::BriefDescriptorExtractor> mBrief;
};

/*! \} */ // end of FeatureDetectorAndDescriptor

/*! \} */ // end of Features

} // namespace tl

#endif // TL_FEATMATCH_BRIEF_DESCRIPTOR_H
