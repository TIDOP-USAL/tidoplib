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

#include "lucid.h"

#include "tidop/core/base/exception.h"

namespace tl
{


/* LUCID properties */

LucidProperties::LucidProperties()
  : Feature("LUCID", Feature::Type::lucid)
{
    reset();
}

LucidProperties::LucidProperties(const LucidProperties &properties)
  : Feature(properties)
{
}

LucidProperties::LucidProperties(LucidProperties &&properties) TL_NOEXCEPT
  : Feature(std::forward<Feature>(properties))
{
}

LucidProperties::~LucidProperties() = default;

auto LucidProperties::operator =(const LucidProperties &properties) -> LucidProperties &
{
    if (this != &properties) {
        Feature::operator=(properties);
    }

    return *this;
}

auto LucidProperties::operator =(LucidProperties &&properties) TL_NOEXCEPT -> LucidProperties &
{
    if (this != &properties) {
        Feature::operator=(std::forward<Feature>(properties));
    }

    return *this;
}

auto LucidProperties::lucidKernel() const -> int
{
    return getProperty<int>("LucidKernel");
}

auto LucidProperties::blurKernel() const -> int
{
    return getProperty<int>("BlurKernel");
}

void LucidProperties::setLucidKernel(int lucidKernel)
{
    setProperty("LucidKernel", lucidKernel);
}

void LucidProperties::setBlurKernel(int blurKernel)
{
    setProperty("BlurKernel", blurKernel);
}

void LucidProperties::reset()
{
    setLucidKernel(1);
    setBlurKernel(2);
}



/* LUCID Descriptor */

LucidDescriptor::LucidDescriptor()
  : mProperties()
{
    init();
}

LucidDescriptor::LucidDescriptor(const LucidProperties &properties)
  : mProperties(properties)
{
    init();
}

LucidDescriptor::LucidDescriptor(const LucidDescriptor &lucid)
  : mProperties(lucid.mProperties)
{
    init();
}

LucidDescriptor::LucidDescriptor(LucidDescriptor &&lucid) TL_NOEXCEPT
  : mProperties(std::move(lucid.mProperties))
#ifdef HAVE_OPENCV_XFEATURES2D 
    ,
    mLUCID(std::move(lucid.mLUCID))
#endif // HAVE_OPENCV_XFEATURES2D
{
    init();
}

auto LucidDescriptor::operator =(const LucidDescriptor &lucid) -> LucidDescriptor &
{
    if (this != &lucid) {
        mProperties = lucid.mProperties;
        init();
    }

    return *this;
}

auto LucidDescriptor::operator =(LucidDescriptor &&lucid) TL_NOEXCEPT -> LucidDescriptor &
{
    if (this != &lucid) {
        mProperties = std::move(lucid.mProperties);
#ifdef HAVE_OPENCV_XFEATURES2D 
        mLUCID = std::move(lucid.mLUCID);
#endif // HAVE_OPENCV_XFEATURES2D
    }

    return *this;
}

LucidDescriptor::~LucidDescriptor() = default;

void LucidDescriptor::init()
{
#ifdef HAVE_OPENCV_XFEATURES2D
    mLUCID = cv::xfeatures2d::LUCID::create(mProperties.lucidKernel(),
                                            mProperties.blurKernel());
#endif
}

auto LucidDescriptor::extract(const cv::Mat &img,
                              std::vector<cv::KeyPoint> &keyPoints) -> cv::Mat
{
    cv::Mat descriptors;

    try {

#ifdef HAVE_OPENCV_XFEATURES2D 
        mLUCID->compute(img, keyPoints, descriptors);
#else
        tl::unusedParameter(img, keyPoints);
        TL_COMPILER_WARNING("OpenCV not built with extra modules. Lucid Descriptor not supported")
        throw TL_ERROR("OpenCV not built with extra modules. Lucid Descriptor not supported");
#endif // HAVE_OPENCV_XFEATURES2D

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return descriptors;
}

} // namespace tl
