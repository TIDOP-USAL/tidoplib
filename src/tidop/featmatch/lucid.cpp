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

#include "lucid.h"

#include "tidop/core/messages.h"

namespace tl
{


LucidProperties::LucidProperties()
  : mLucidKernel(1),
    mBlurKernel(2)
{}

int LucidProperties::lucidKernel() const
{
  return mLucidKernel;
}

int LucidProperties::blurKernel() const
{
  return mBlurKernel;
}

void LucidProperties::setLucidKernel(int lucidKernel)
{
  mLucidKernel = lucidKernel;
}

void LucidProperties::setBlurKernel(int blurKernel)
{
  mBlurKernel = blurKernel;
}

void LucidProperties::reset()
{
  mLucidKernel = 1;
  mBlurKernel = 2;
}

std::string LucidProperties::name() const
{
  return std::string("LUCID");
}


/*----------------------------------------------------------------*/


LucidDescriptor::LucidDescriptor()
{
  update();
}

LucidDescriptor::LucidDescriptor(int lucidKernel, int blurKernel)
  : LucidProperties(),
    DescriptorExtractor()
{
  LucidProperties::setLucidKernel(lucidKernel);
  LucidProperties::setBlurKernel(blurKernel);
  update();
}

void LucidDescriptor::update()
{
  mLUCID = cv::xfeatures2d::LUCID::create(LucidProperties::lucidKernel(),
                                          LucidProperties::blurKernel());
}

cv::Mat LucidDescriptor::extract(const cv::Mat &img, 
                                 std::vector<cv::KeyPoint> &keyPoints)
{
  cv::Mat descriptors;
  mLUCID->compute(img, keyPoints, descriptors);
  return descriptors;
}

void LucidDescriptor::setLucidKernel(int lucidKernel)
{
  LucidProperties::setLucidKernel(lucidKernel);
  update();
}

void LucidDescriptor::setBlurKernel(int blurKernel)
{
  LucidProperties::setBlurKernel(blurKernel);
  update();
}

void LucidDescriptor::reset()
{
  LucidProperties::reset();
  update();
}



} // namespace tl
