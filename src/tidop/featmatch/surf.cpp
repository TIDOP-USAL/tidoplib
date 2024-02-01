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

#include "tidop/core/exception.h"


namespace tl
{


SurfProperties::SurfProperties()
{
}

SurfProperties::SurfProperties(const SurfProperties &surfProperties)
  : Surf(surfProperties),
    mHessianThreshold(surfProperties.mHessianThreshold),
    mOctaves(surfProperties.octaves()),
    mOctaveLayers(surfProperties.mOctaveLayers),
    mExtendedDescriptor(surfProperties.mExtendedDescriptor),
    mUpright(surfProperties.mUpright)
{
}

auto SurfProperties::hessianThreshold() const -> double
{
    return mHessianThreshold;
}

void SurfProperties::setHessianThreshold(double hessianThreshold)
{
    mHessianThreshold = hessianThreshold;
}

auto SurfProperties::octaves() const -> int
{
    return mOctaves;
}

void SurfProperties::setOctaves(int octaves)
{
    mOctaves = octaves;
}

auto SurfProperties::octaveLayers() const -> int
{
    return mOctaveLayers;
}

void SurfProperties::setOctaveLayers(int octaveLayers)
{
    mOctaveLayers = octaveLayers;
}

auto SurfProperties::extendedDescriptor() const -> bool
{
    return mExtendedDescriptor;
}

void SurfProperties::setExtendedDescriptor(bool extendedDescriptor)
{
    mExtendedDescriptor = extendedDescriptor;
}

auto SurfProperties::upright() const -> bool
{
    return mUpright;
}

void SurfProperties::setUpright(bool upright)
{
    mUpright = upright;
}

void SurfProperties::reset()
{
    mHessianThreshold = surf_default_value_hessian_threshold;
    mOctaves = surf_default_value_octaves;
    mOctaveLayers = surf_default_value_octave_layers;
    mExtendedDescriptor = surf_default_value_extended_descriptor;
    mUpright = surf_default_value_upright;
}

auto SurfProperties::name() const -> std::string
{
    return std::string("SURF");
}


/*----------------------------------------------------------------*/

//#ifdef OPENCV_ENABLE_NONFREE

SurfDetectorDescriptor::SurfDetectorDescriptor()
{
#ifdef HAVE_OPENCV_XFEATURES2D 
    mSurf = cv::xfeatures2d::SURF::create(SurfProperties::hessianThreshold(),
                                          SurfProperties::octaves(),
                                          SurfProperties::octaveLayers(),
                                          SurfProperties::extendedDescriptor(),
                                          SurfProperties::upright());
#endif // HAVE_OPENCV_XFEATURES2D
}

SurfDetectorDescriptor::SurfDetectorDescriptor(const SurfDetectorDescriptor &surfDetectorDescriptor)
    : SurfProperties(surfDetectorDescriptor)
#ifdef HAVE_OPENCV_XFEATURES2D 
    ,
    mSurf(surfDetectorDescriptor.mSurf)
#endif // HAVE_OPENCV_XFEATURES2D
{
}

SurfDetectorDescriptor::SurfDetectorDescriptor(double hessianThreshold,
                                               int octaves,
                                               int octaveLayers,
                                               bool extendedDescriptor,
                                               bool upright)
#ifdef HAVE_OPENCV_XFEATURES2D 
    : mSurf(cv::xfeatures2d::SURF::create())
#endif // HAVE_OPENCV_XFEATURES2D
{
    setHessianThreshold(hessianThreshold);
    setOctaves(octaves);
    setOctaveLayers(octaveLayers);
    setExtendedDescriptor(extendedDescriptor);
    setUpright(upright);
}

auto SurfDetectorDescriptor::detect(const cv::Mat &img,
                                    cv::InputArray &mask) -> std::vector<cv::KeyPoint>
{
    std::vector<cv::KeyPoint> keyPoints;

    try {

#ifdef HAVE_OPENCV_XFEATURES2D 
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

#ifdef HAVE_OPENCV_XFEATURES2D 
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

void SurfDetectorDescriptor::setHessianThreshold(double hessianThreshold)
{
    SurfProperties::setHessianThreshold(hessianThreshold);
#ifdef HAVE_OPENCV_XFEATURES2D 
    mSurf->setHessianThreshold(hessianThreshold);
#endif // HAVE_OPENCV_XFEATURES2D
}

void SurfDetectorDescriptor::setOctaves(int octaves)
{
    SurfProperties::setOctaves(octaves);
#ifdef HAVE_OPENCV_XFEATURES2D 
    mSurf->setNOctaves(octaves);
#endif // HAVE_OPENCV_XFEATURES2D
}

void SurfDetectorDescriptor::setOctaveLayers(int octaveLayers)
{
    SurfProperties::setOctaveLayers(octaveLayers);
#ifdef HAVE_OPENCV_XFEATURES2D 
    mSurf->setNOctaveLayers(octaveLayers);
#endif // HAVE_OPENCV_XFEATURES2D
}

void SurfDetectorDescriptor::setExtendedDescriptor(bool extendedDescriptor)
{
    SurfProperties::setExtendedDescriptor(extendedDescriptor);
#ifdef HAVE_OPENCV_XFEATURES2D 
    mSurf->setExtended(extendedDescriptor);
#endif // HAVE_OPENCV_XFEATURES2D
}

void SurfDetectorDescriptor::setUpright(bool upright)
{
    SurfProperties::setUpright(upright);
#ifdef HAVE_OPENCV_XFEATURES2D 
    mSurf->setUpright(upright);
#endif // HAVE_OPENCV_XFEATURES2D
}

void SurfDetectorDescriptor::reset()
{
    SurfProperties::reset();
#ifdef HAVE_OPENCV_XFEATURES2D 
    mSurf->setHessianThreshold(SurfProperties::hessianThreshold());
    mSurf->setNOctaves(SurfProperties::octaves());
    mSurf->setNOctaveLayers(SurfProperties::octaveLayers());
    mSurf->setExtended(SurfProperties::extendedDescriptor());
    mSurf->setUpright(SurfProperties::upright());
#endif // HAVE_OPENCV_XFEATURES2D
}



/*----------------------------------------------------------------*/


//#ifdef HAVE_OPENCV_CUDAFEATURES2D

SurfCudaDetectorDescriptor::SurfCudaDetectorDescriptor()
#if defined HAVE_OPENCV_CUDAFEATURES2D
  : mSurf(new cv::cuda::SURF_CUDA())
#endif
{
#if defined HAVE_OPENCV_CUDAFEATURES2D
    mSurf->hessianThreshold = SurfProperties::hessianThreshold();
    mSurf->nOctaves = SurfProperties::octaves();
    mSurf->nOctaveLayers = SurfProperties::octaveLayers();
    mSurf->extended = SurfProperties::extendedDescriptor();
    mSurf->upright = SurfProperties::upright();
#endif
}

SurfCudaDetectorDescriptor::SurfCudaDetectorDescriptor(const SurfCudaDetectorDescriptor &surfDetectorDescriptor)
  : SurfProperties(surfDetectorDescriptor)
{
#if defined HAVE_OPENCV_CUDAFEATURES2D
    mSurf = std::make_unique<cv::cuda::SURF_CUDA>();
    mSurf->hessianThreshold = SurfProperties::hessianThreshold();
    mSurf->nOctaves = SurfProperties::octaves();
    mSurf->nOctaveLayers = SurfProperties::octaveLayers();
    mSurf->extended = SurfProperties::extendedDescriptor();
    mSurf->upright = SurfProperties::upright();
#endif
}

SurfCudaDetectorDescriptor::SurfCudaDetectorDescriptor(double hessianThreshold,
                                                       int octaves,
                                                       int octaveLayers,
                                                       bool extendedDescriptor,
                                                       bool upright)
{
#if defined HAVE_OPENCV_CUDAFEATURES2D
    mSurf = std::make_unique<cv::cuda::SURF_CUDA>();
    setHessianThreshold(hessianThreshold);
    setOctaves(octaves);
    setOctaveLayers(octaveLayers);
    setExtendedDescriptor(extendedDescriptor);
    setUpright(upright);
#endif
}

auto SurfCudaDetectorDescriptor::detect(const cv::Mat &img,
                                        cv::InputArray &mask) -> std::vector<cv::KeyPoint>
{
    std::vector<cv::KeyPoint> keyPoints;

    try {

#if defined HAVE_OPENCV_CUDAFEATURES2D
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

#if defined HAVE_OPENCV_CUDAFEATURES2D
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

void SurfCudaDetectorDescriptor::setHessianThreshold(double hessianThreshold)
{
    SurfProperties::setHessianThreshold(hessianThreshold);
#if defined HAVE_OPENCV_CUDAFEATURES2D
    mSurf->hessianThreshold = hessianThreshold;
#endif
}

void SurfCudaDetectorDescriptor::setOctaves(int octaves)
{
    SurfProperties::setOctaves(octaves);
#if defined HAVE_OPENCV_CUDAFEATURES2D
    mSurf->nOctaves = octaves;
#endif
}

void SurfCudaDetectorDescriptor::setOctaveLayers(int octaveLayers)
{
    SurfProperties::setOctaveLayers(octaveLayers);
#if defined HAVE_OPENCV_CUDAFEATURES2D
    mSurf->nOctaveLayers = octaveLayers;
#endif
}

void SurfCudaDetectorDescriptor::setExtendedDescriptor(bool extendedDescriptor)
{
    SurfProperties::setExtendedDescriptor(extendedDescriptor);
#if defined HAVE_OPENCV_CUDAFEATURES2D
    mSurf->extended = extendedDescriptor;
#endif
}

void SurfCudaDetectorDescriptor::setUpright(bool upright)
{
    SurfProperties::setUpright(upright);
#if defined HAVE_OPENCV_CUDAFEATURES2D
    mSurf->upright = upright;
#endif
}

void SurfCudaDetectorDescriptor::reset()
{
    SurfProperties::reset();
#if defined HAVE_OPENCV_CUDAFEATURES2D
    mSurf->hessianThreshold = SurfProperties::hessianThreshold();
    mSurf->nOctaves = SurfProperties::octaves();
    mSurf->nOctaveLayers = SurfProperties::octaveLayers();
    mSurf->extended = SurfProperties::extendedDescriptor();
    mSurf->upright = SurfProperties::upright();
#endif
}


} // namespace tl







