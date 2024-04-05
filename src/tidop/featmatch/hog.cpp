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

#include "tidop/core/exception.h"

#include <opencv2/imgproc.hpp>


namespace tl
{


HogProperties::HogProperties()
  : mWinSize(16, 16),
    mBlockSize(4, 4),
    mBlockStride(2, 2),
    mCellSize(2, 2)
{
}

HogProperties::HogProperties(const HogProperties &hogProperties)
  : Hog(hogProperties),
    mWinSize(hogProperties.mWinSize),
    mBlockSize(hogProperties.mBlockSize),
    mBlockStride(hogProperties.mBlockStride),
    mCellSize(hogProperties.mCellSize),
    mNbins(hogProperties.mNbins),
    mDerivAperture(hogProperties.mDerivAperture)
{
}

auto HogProperties::winSize() const -> Size<int>
{
    return mWinSize;
}

auto HogProperties::blockSize() const -> Size<int>
{
    return mBlockSize;
}

auto HogProperties::blockStride() const -> Size<int>
{
    return mBlockStride;
}

auto HogProperties::cellSize() const -> Size<int>
{
    return mCellSize;
}

auto HogProperties::nbins() const -> int
{
    return mNbins;
}

auto HogProperties::derivAperture() const -> int
{
    return mDerivAperture;
}

void HogProperties::setWinSize(const Size<int> &winSize)
{
    mWinSize = winSize;
}

void HogProperties::setBlockSize(const Size<int> &blockSize)
{
    mBlockSize = blockSize;
}

void HogProperties::setBlockStride(const Size<int> &blockStride)
{
    mBlockStride = blockStride;
}

void HogProperties::setCellSize(const Size<int> &cellSize)
{
    mCellSize = cellSize;
}

void HogProperties::setNbins(int nbins)
{
    mNbins = nbins;
}

void HogProperties::setDerivAperture(int derivAperture)
{
    mDerivAperture = derivAperture;
}

void HogProperties::reset()
{
    mWinSize = Size<int>(16, 16);
    mBlockSize = Size<int>(4, 4);
    mBlockStride = Size<int>(2, 2);
    mCellSize = Size<int>(2, 2);
    mNbins = 9;
    mDerivAperture = 1;
}

auto HogProperties::name() const -> std::string
{
    return std::string("HOG");
}


/*----------------------------------------------------------------*/


HogDescriptor::HogDescriptor()
{
    update();
}

HogDescriptor::HogDescriptor(const HogDescriptor &hogDescriptor)
  : HogProperties(hogDescriptor),
    DescriptorExtractor(hogDescriptor)
{
    update();
}

HogDescriptor::HogDescriptor(const Size<int> &winSize,
                             const Size<int> &blockSize,
                             const Size<int> &blockStride,
                             const Size<int> &cellSize,
                             int nbins,
                             int derivAperture)
{
    HogProperties::setWinSize(winSize);
    HogProperties::setBlockSize(blockSize);
    HogProperties::setBlockStride(blockStride);
    HogProperties::setCellSize(cellSize);
    HogProperties::setNbins(nbins);
    HogProperties::setDerivAperture(derivAperture);
    update();
}

void HogDescriptor::update()
{
    cv::Size win_size(HogProperties::winSize().width,
                      HogProperties::winSize().height);
    cv::Size block_size(HogProperties::blockSize().width,
                        HogProperties::blockSize().height);
    cv::Size block_stride(HogProperties::blockStride().width,
                          HogProperties::blockStride().height);
    cv::Size cell_size(HogProperties::cellSize().width,
                       HogProperties::cellSize().height);

    mHOG = std::make_shared<cv::HOGDescriptor>(win_size,
                                               block_size,
                                               block_stride,
                                               cell_size,
                                               HogProperties::nbins(),
                                               HogProperties::derivAperture());
}

void HogDescriptor::normalizepatch(const cv::Mat &gray,
                                   const cv::KeyPoint &keypoint,
                                   cv::Mat &output)
{
    try {

        cv::Point center = keypoint.pt;

        cv::Size outsize(HogProperties::winSize().width, HogProperties::winSize().height);
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

void HogDescriptor::setWinSize(const Size<int> &winSize)
{
    HogProperties::setWinSize(winSize);
    update();
}

void HogDescriptor::setBlockSize(const Size<int> &blockSize)
{
    HogProperties::setBlockSize(blockSize);
    update();
}

void HogDescriptor::setBlockStride(const Size<int> &blockStride)
{
    HogProperties::setBlockStride(blockStride);
    update();
}

void HogDescriptor::setCellSize(const Size<int> &cellSize)
{
    HogProperties::setCellSize(cellSize);
    update();
}

void HogDescriptor::setNbins(int nbins)
{
    HogProperties::setNbins(nbins);
    update();
}

void HogDescriptor::setDerivAperture(int derivAperture)
{
    HogProperties::setDerivAperture(derivAperture);
    update();
}

void HogDescriptor::reset()
{
    HogProperties::reset();
    update();
}


} // namespace tl
