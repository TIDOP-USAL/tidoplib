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

#include "hog.h"

#include "tidop/core/base/exception.h"

#include <opencv2/imgproc.hpp>


namespace tl
{


HogProperties::HogProperties()
  : Feature("HOG", Feature::Type::hog)
{
    reset();
}

HogProperties::HogProperties(const HogProperties &properties)
  : Feature(properties)
{
}

HogProperties::HogProperties(HogProperties &&properties) TL_NOEXCEPT
  : Feature(std::forward<Feature>(properties))
{
}

auto HogProperties::operator =(const HogProperties &properties) -> HogProperties &
{
    if (this != &properties) {
        Feature::operator=(properties);
    }

    return *this;
}

auto HogProperties::operator =(HogProperties &&properties) TL_NOEXCEPT -> HogProperties &
{
    if (this != &properties) {
        Feature::operator=(std::forward<Feature>(properties));
    }

    return *this;
}

auto HogProperties::winSize() const -> Size<int>
{
    return getProperty<Size<int>>("WinSize");
}

auto HogProperties::blockSize() const -> Size<int>
{
    return getProperty<Size<int>>("BlockSize");
}

auto HogProperties::blockStride() const -> Size<int>
{
    return getProperty<Size<int>>("BlockStride");
}

auto HogProperties::cellSize() const -> Size<int>
{
    return getProperty<Size<int>>("CellSize");
}

auto HogProperties::nbins() const -> int
{
    return getProperty<int>("Nbins");
}

auto HogProperties::derivAperture() const -> int
{
    return getProperty<int>("DerivAperture");
}

void HogProperties::setWinSize(const Size<int> &winSize)
{
    setProperty("WinSize", winSize);
}

void HogProperties::setBlockSize(const Size<int> &blockSize)
{
    setProperty("BlockSize", blockSize);
}

void HogProperties::setBlockStride(const Size<int> &blockStride)
{
    setProperty("BlockStride", blockStride);
}

void HogProperties::setCellSize(const Size<int> &cellSize)
{
    setProperty("CellSize", cellSize);
}

void HogProperties::setNbins(int nbins)
{
    setProperty("Nbins", nbins);
}

void HogProperties::setDerivAperture(int derivAperture)
{
    setProperty("DerivAperture", derivAperture);
}

void HogProperties::reset()
{
    setWinSize({16, 16});
    setBlockSize({4, 4});
    setBlockStride({2, 2});
    setCellSize({2, 2});
    setNbins(9);
    setDerivAperture(1);
}



/* HOG descriptor */

HogDescriptor::HogDescriptor()
  : mProperties()
{
    init();
}

HogDescriptor::HogDescriptor(const HogProperties &properties)
  : mProperties(properties)
{
    init();
}

HogDescriptor::HogDescriptor(const HogDescriptor &hog)
  : mProperties(hog.mProperties)
{
    init();
}

HogDescriptor::HogDescriptor(HogDescriptor &&hog) TL_NOEXCEPT
  : mProperties(std::move(hog.mProperties)),
    mHOG(std::move(hog.mHOG))
{
    init();
}

auto HogDescriptor::operator =(const HogDescriptor &hog) -> HogDescriptor &
{
    if (this != &hog) {
        mProperties = hog.mProperties;
        init();
    }

    return *this;
}

auto HogDescriptor::operator =(HogDescriptor &&hog) TL_NOEXCEPT -> HogDescriptor &
{
    if (this != &hog) {
        mProperties = std::move(hog.mProperties);
        mHOG = std::move(hog.mHOG);
    }

    return *this;
}

void HogDescriptor::init()
{
    cv::Size win_size(mProperties.winSize().width,
                      mProperties.winSize().height);
    cv::Size block_size(mProperties.blockSize().width,
                        mProperties.blockSize().height);
    cv::Size block_stride(mProperties.blockStride().width,
                          mProperties.blockStride().height);
    cv::Size cell_size(mProperties.cellSize().width,
                       mProperties.cellSize().height);

    mHOG = std::make_shared<cv::HOGDescriptor>(win_size,
                                               block_size,
                                               block_stride,
                                               cell_size,
                                               mProperties.nbins(),
                                               mProperties.derivAperture());
}

void HogDescriptor::normalizepatch(const cv::Mat &gray,
                                   const cv::KeyPoint &keypoint,
                                   cv::Mat &output)
{
    try {

        cv::Point center = keypoint.pt;

        cv::Size outsize(mProperties.winSize().width, mProperties.winSize().height);
        output = cv::Mat::zeros(outsize, CV_8UC1);
        cv::Size maskenter;
        maskenter.height = cvRound(keypoint.size);
        maskenter.width = cvRound(keypoint.size);

        cv::Mat input;
        // Rotation and scale comes from the left corner, that is the center.
        cv::getRectSubPix(gray, maskenter, center, input);


        cv::Point2f pt;

        // here there are a problem with the center, it depends on the value it´s not a trivial thing.
            // solved
        if ((input.cols % 4) == 1) {
            pt.x = static_cast<float>(cvRound(input.cols / 2.0));
            pt.y = static_cast<float>(cvRound(input.rows / 2.0));
        } else if ((input.cols % 4) == 0) {
            pt.x = (static_cast<float>(input.cols) / 2.0f) - 0.5f;
            pt.y = (static_cast<float>(input.rows) / 2.0f) - 0.5f;
        } else if ((input.cols % 4) == 2) {
            pt.x = (static_cast<float>(input.cols) / 2.0f) - 0.5f;
            pt.y = (static_cast<float>(input.rows) / 2.0f) - 0.5f;
        } else if ((input.cols % 4) == 3) {
            pt.x = (static_cast<float>(input.cols) / 2.0f) - 0.5f;
            pt.y = (static_cast<float>(input.rows) / 2.0f) - 0.5f;
        }
        // to calculate the scale, is the size of the keypoint between
        // the scale is related to the diagonal of both pathces
        // float scale = 1.0f*std::sqrt((maskenter.height*maskenter.height)+(maskenter.height*maskenter.height))/std::sqrt((outsize.height*outsize.height)+(outsize.height*outsize.height));

        cv::Mat transform = cv::getRotationMatrix2D(pt, static_cast<double>(keypoint.angle), 1.0);
        cv::Mat source1;

        //ROTATE
        cv::warpAffine(input, source1, transform, input.size(), cv::INTER_LINEAR + cv::WARP_INVERSE_MAP, cv::BORDER_REPLICATE);//+cv::WARP_INVERSE_MAP


        // works best in 2 steps instead of one.
        if (outsize.height != input.cols) cv::resize(source1, output, outsize);
        else source1.copyTo(output);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

auto HogDescriptor::extract(const cv::Mat &img, std::vector<cv::KeyPoint> &keyPoints) -> cv::Mat
{
    cv::Mat descriptors;

    try {

        int size = static_cast<int>(keyPoints.size());
        descriptors = cv::Mat(size, static_cast<int>(mHOG->getDescriptorSize()), CV_32FC1);

        for (int i = 0; i < size; i++) {
            std::vector<float> hogdescriptor_aux;
            cv::Mat patch;
            normalizepatch(img, keyPoints[static_cast<size_t>(i)], patch);
            mHOG->compute(patch, hogdescriptor_aux);
            for (size_t j = 0; j < hogdescriptor_aux.size(); j++)
                descriptors.at<float>(i, static_cast<int>(j)) = hogdescriptor_aux[j];
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }

    return descriptors;
}


} // namespace tl
