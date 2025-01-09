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

#include "tidop/rastertools/process/imgprocess.h"

#include "tidop/core/base/exception.h"

#ifdef TL_HAVE_OPENCV
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <cstdarg>
#include <cstdio>


namespace tl
{



ImagingProcesses::ImagingProcesses()
{
}

ImagingProcesses::ImagingProcesses(const ImagingProcesses &imagingProcesses)
  : mProcessList(imagingProcesses.mProcessList)
{
}

ImagingProcesses::ImagingProcesses(std::initializer_list<std::shared_ptr<ImageProcess>> processes)
  : mProcessList(processes)
{
}

ImagingProcesses::~ImagingProcesses()
{
}

void ImagingProcesses::run(const cv::Mat &matIn, cv::Mat &matOut) const
{
    try {

        TL_ASSERT(!matIn.empty(), "Incorrect input data");

        matIn.copyTo(matOut);
        for (const auto &process : mProcessList) {
            process->run(matOut, matOut);
        }

    } catch (...) {
        std::throw_with_nested(std::runtime_error("ImagingProcesses::run() failed"));
    }
}

void ImagingProcesses::push_back(const std::shared_ptr<ImageProcess> &process)
{
    mProcessList.push_back(process);
}

void ImagingProcesses::pop_back()
{
    mProcessList.pop_back();
}

void ImagingProcesses::clear() TL_NOEXCEPT
{
    mProcessList.clear();
}

auto ImagingProcesses::empty() const TL_NOEXCEPT -> bool
{
    return mProcessList.empty();
}

auto ImagingProcesses::begin() TL_NOEXCEPT -> iterator
{
    return mProcessList.begin();
}

auto ImagingProcesses::begin() const TL_NOEXCEPT -> const_iterator
{
    return mProcessList.begin();
}

auto ImagingProcesses::end() TL_NOEXCEPT -> iterator
{
    return mProcessList.end();
}

auto ImagingProcesses::end() const TL_NOEXCEPT -> const_iterator
{
    return mProcessList.end();
}




Normalize::Normalize(double lowRange, double upRange)
 : ImageProcess(ProcessType::normalize),
    mLowRange(lowRange),
    mUpRange(upRange)
{
}

void Normalize::run(const cv::Mat &matIn, cv::Mat &matOut) const
{
    try {

        TL_ASSERT(!matIn.empty(), "Incorrect input data. Empty image");

        cv::normalize(matIn, matOut, mLowRange, mUpRange, cv::NORM_MINMAX);

    } catch (...) {
        std::throw_with_nested(std::runtime_error("Normalize::run() failed"));
    }
}






Binarize::Binarize(double thresh, double maxVal, bool bInverse)
  : ImageProcess(ProcessType::binarize),
    mThresh(thresh),
    mMaxVal(maxVal),
    bInverse(bInverse)
{
}

void Binarize::run(const cv::Mat &matIn, cv::Mat &matOut) const
{
    try {

        TL_ASSERT(!matIn.empty(), "Incorrect input data. Empty image");

        double th = mThresh, max = mMaxVal;
        if (th == 0.0 && max == 0.0) {
            cv::Scalar m, stdv;
            cv::meanStdDev(matIn, m, stdv);
            th = m[0] - stdv[0];
            max = m[0] + stdv[0];
        }

        cv::threshold(matIn, matOut, th, max, bInverse ? cv::THRESH_BINARY_INV : cv::THRESH_BINARY);

    } catch (...) {
        std::throw_with_nested(std::runtime_error("Binarize::run() failed"));
    }
}





EqualizeHistogram::EqualizeHistogram()
  : ImageProcess(ProcessType::equalize_hist)
{
}

void EqualizeHistogram::run(const cv::Mat &matIn, cv::Mat &matOut) const
{
    try {

        TL_ASSERT(!matIn.empty(), "Incorrect input data. Empty image");

        cv::equalizeHist(matIn, matOut);

    } catch (...) {
        std::throw_with_nested(std::runtime_error("EqualizeHistogram::run() failed"));
    }
}





FunctionProcess::FunctionProcess(std::function<void(const cv::Mat &, cv::Mat &)> f)
  : ImageProcess(ProcessType::function_process),
    mFunction(f)
{
}

void FunctionProcess::run(const cv::Mat &matIn, cv::Mat &matOut) const
{
    try {

        TL_ASSERT(!matIn.empty(), "Incorrect input data. Empty image");

        mFunction(matIn, matOut);

    } catch (...) {
        std::throw_with_nested(std::runtime_error("FunctionProcess::run() failed"));
    }
}


} // End namespace tl

#endif // TL_HAVE_OPENCV
