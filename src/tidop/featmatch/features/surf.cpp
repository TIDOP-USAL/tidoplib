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

#include "surf.h"

#include "tidop/core/base/exception.h"


namespace tl
{


/* SURF properties */

SurfProperties::SurfProperties()
  : Feature("SURF", Feature::Type::surf)
{
    reset();
}

SurfProperties::SurfProperties(const SurfProperties &properties)
  : Feature(properties)
{
}

SurfProperties::SurfProperties(SurfProperties &&properties) TL_NOEXCEPT
  : Feature(std::forward<Feature>(properties))
{
}

auto SurfProperties::operator=(const SurfProperties &properties) -> SurfProperties &
{
    if (this != &properties) {
        Feature::operator=(properties);
    }
    return *this;
}

auto SurfProperties::operator=(SurfProperties &&properties) TL_NOEXCEPT -> SurfProperties &
{
    if (this != &properties) {
        Feature::operator=(std::forward<Feature>(properties));
    }
    return *this;
}

auto SurfProperties::hessianThreshold() const -> double
{
    return getProperty<double>("HessianThreshold");
}

void SurfProperties::setHessianThreshold(double hessianThreshold)
{
    setProperty("HessianThreshold", hessianThreshold);
}

auto SurfProperties::octaves() const -> int
{
    return getProperty<int>("Octaves");
}

void SurfProperties::setOctaves(int octaves)
{
    setProperty("Octaves", octaves);
}

auto SurfProperties::octaveLayers() const -> int
{
    return getProperty<int>("OctaveLayers");
}

void SurfProperties::setOctaveLayers(int octaveLayers)
{
    setProperty("OctaveLayers", octaveLayers);
}

auto SurfProperties::extendedDescriptor() const -> bool
{
    return getProperty<bool>("ExtendedDescriptor");
}

void SurfProperties::setExtendedDescriptor(bool extendedDescriptor)
{
    setProperty("ExtendedDescriptor", extendedDescriptor);
}

auto SurfProperties::upright() const -> bool
{
    return getProperty<bool>("Upright");
}

void SurfProperties::setUpright(bool upright)
{
    setProperty("Upright", upright);
}

void SurfProperties::reset()
{
    setHessianThreshold(surf_default_value_hessian_threshold);
    setOctaves(surf_default_value_octaves);
    setOctaveLayers(surf_default_value_octave_layers);
    setExtendedDescriptor(surf_default_value_extended_descriptor);
    setUpright(surf_default_value_upright);
}





SurfDetectorDescriptor::SurfDetectorDescriptor()
    : mProperties()
{
    init();
}

SurfDetectorDescriptor::SurfDetectorDescriptor(const SurfProperties &properties)
    : mProperties(properties)
{
    init();
}

SurfDetectorDescriptor::SurfDetectorDescriptor(const SurfDetectorDescriptor &surf)
    : mProperties(surf.mProperties)
{
    init();
}

SurfDetectorDescriptor::SurfDetectorDescriptor(SurfDetectorDescriptor &&surf) TL_NOEXCEPT
    : mProperties(std::move(surf.mProperties))
#if defined HAVE_OPENCV_XFEATURES2D  && defined OPENCV_ENABLE_NONFREE
    , mSurf(std::move(surf.mSurf))
#endif // HAVE_OPENCV_XFEATURES2D
{
}

auto SurfDetectorDescriptor::operator =(const SurfDetectorDescriptor &surf) -> SurfDetectorDescriptor &
{
    if (this != &surf) {
        mProperties = surf.mProperties;
        init();
    }
    return *this;
}

auto SurfDetectorDescriptor::operator =(SurfDetectorDescriptor &&surf) TL_NOEXCEPT -> SurfDetectorDescriptor &
{
    if (this != &surf) {
        mProperties = std::move(surf.mProperties);
#if defined HAVE_OPENCV_XFEATURES2D  && defined OPENCV_ENABLE_NONFREE
        mSurf = std::move(surf.mSurf);
#endif
    }
    return *this;
}

void SurfDetectorDescriptor::init()
{
#if defined HAVE_OPENCV_XFEATURES2D  && defined OPENCV_ENABLE_NONFREE
    mSurf = cv::xfeatures2d::SURF::create(mProperties.hessianThreshold(),
                                          mProperties.octaves(),
                                          mProperties.octaveLayers(),
                                          mProperties.extendedDescriptor(),
                                          mProperties.upright());
#endif
}

auto SurfDetectorDescriptor::detect(const cv::Mat &img,
                                    cv::InputArray &mask) -> std::vector<cv::KeyPoint>
{
    std::vector<cv::KeyPoint> keyPoints;

    try {

#if defined HAVE_OPENCV_XFEATURES2D  && defined OPENCV_ENABLE_NONFREE
        mSurf->detect(img, keyPoints, mask);
#else
        TL_COMPILER_WARNING("OpenCV not built with extra modules. Surf Detector/Descriptor not supported")
        throw TL_ERROR("OpenCV not built with extra modules. Surf Detector/Descriptor not supported");
#endif // HAVE_OPENCV_XFEATURES2D

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return keyPoints;
}

auto SurfDetectorDescriptor::extract(const cv::Mat &img, 
                                     std::vector<cv::KeyPoint> &keyPoints) -> cv::Mat
{
    cv::Mat descriptors;

    try {

#if defined HAVE_OPENCV_XFEATURES2D  && defined OPENCV_ENABLE_NONFREE
        mSurf->compute(img, keyPoints, descriptors);
#else
        TL_COMPILER_WARNING("OpenCV not built with extra modules. Surf Detector/Descriptor not supported")
        throw TL_ERROR("OpenCV not built with extra modules. Surf Detector/Descriptor not supported");
#endif // HAVE_OPENCV_XFEATURES2D

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return descriptors;
}






SurfCudaDetectorDescriptor::SurfCudaDetectorDescriptor()
  : mProperties()
{
    init();
}

SurfCudaDetectorDescriptor::SurfCudaDetectorDescriptor(const SurfProperties &properties)
  : mProperties(properties)
{
    init();
}

SurfCudaDetectorDescriptor::SurfCudaDetectorDescriptor(const SurfCudaDetectorDescriptor &surf)
  : mProperties(surf.mProperties)
{
    init();
}

SurfCudaDetectorDescriptor::SurfCudaDetectorDescriptor(SurfCudaDetectorDescriptor &&surf) TL_NOEXCEPT
  : mProperties(std::move(surf.mProperties))
#if defined HAVE_OPENCV_XFEATURES2D && defined HAVE_OPENCV_CUDAFEATURES2D && defined OPENCV_ENABLE_NONFREE
    , mSurf(std::move(surf.mSurf))
#endif
{
}

auto SurfCudaDetectorDescriptor::operator =(const SurfCudaDetectorDescriptor &surf) -> SurfCudaDetectorDescriptor &
{
    if (this != &surf) {
        mProperties = surf.mProperties;
        init();
    }
    return *this;
}

auto SurfCudaDetectorDescriptor::operator =(SurfCudaDetectorDescriptor &&surf) TL_NOEXCEPT -> SurfCudaDetectorDescriptor &
{
    if (this != &surf) {
        mProperties = std::move(surf.mProperties);
#if defined HAVE_OPENCV_XFEATURES2D && defined HAVE_OPENCV_CUDAFEATURES2D && defined OPENCV_ENABLE_NONFREE
        mSurf = std::move(surf.mSurf);
#endif
    }
    return *this;
}

void SurfCudaDetectorDescriptor::init()
{
#if defined HAVE_OPENCV_XFEATURES2D && defined HAVE_OPENCV_CUDAFEATURES2D && defined OPENCV_ENABLE_NONFREE
    mSurf = std::make_unique<cv::cuda::SURF_CUDA>(mProperties.hessianThreshold(),
                                                  mProperties.octaves(),
                                                  mProperties.octaveLayers(),
                                                  mProperties.extendedDescriptor(),
                                                  0.01f,
                                                  mProperties.upright());
#endif
}

auto SurfCudaDetectorDescriptor::detect(const cv::Mat &img,
                                        cv::InputArray &mask) -> std::vector<cv::KeyPoint>
{
    std::vector<cv::KeyPoint> keyPoints;

    try {

#if defined HAVE_OPENCV_XFEATURES2D && defined HAVE_OPENCV_CUDAFEATURES2D && defined OPENCV_ENABLE_NONFREE
        cv::cuda::GpuMat g_img(img);
        cv::cuda::GpuMat g_mask(mask);
        (*mSurf)(g_img, g_mask, keyPoints);
#else
        TL_COMPILER_WARNING("OpenCV not built with CUDAFEATURES2D. Cuda Surf Detector/Descriptor not supported")
        throw TL_ERROR("OpenCV not built with CUDAFEATURES2D. Cuda Surf Detector/Descriptor not supported");
#endif // HAVE_OPENCV_CUDAFEATURES2D

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return keyPoints;
}

auto SurfCudaDetectorDescriptor::extract(const cv::Mat &img,
                                         std::vector<cv::KeyPoint> &keyPoints) -> cv::Mat
{
    cv::Mat descriptors;

    try {

#if defined HAVE_OPENCV_XFEATURES2D && defined HAVE_OPENCV_CUDAFEATURES2D && defined OPENCV_ENABLE_NONFREE
        cv::cuda::GpuMat g_img(img);
        cv::cuda::GpuMat g_descriptors;
        (*mSurf)(g_img, cv::cuda::GpuMat(), keyPoints, g_descriptors, true);
        g_descriptors.download(descriptors);
#else
        TL_COMPILER_WARNING("OpenCV not built with CUDAFEATURES2D. Cuda Surf Detector/Descriptor not supported")
        throw TL_ERROR("OpenCV not built with CUDAFEATURES2D. Cuda Surf Detector/Descriptor not supported");
#endif // HAVE_OPENCV_CUDAFEATURES2D

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return descriptors;
}


} // namespace tl







