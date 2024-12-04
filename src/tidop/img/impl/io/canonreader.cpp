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

#include "tidop/img/impl/io/canonreader.h"

#include "tidop/core/exception.h"
#include "tidop/img/metadata.h"

#ifdef TL_HAVE_EDSDK

#include <utility>

namespace tl
{

ImageReaderCanon::ImageReaderCanon(tl::Path file)
  : ImageReader(std::move(file)),
    mInputStream(nullptr)
    mRows(0),
    mCols(0),
    mBands(0),
    mDataType(DataType::TL_16U),
    mColorDepth(16)
{
    RegisterEDSDK::init();
}

ImageReaderCanon::~ImageReaderCanon()
{
    this->close();
}

void ImageReaderCanon::open()
{
    this->close();

    EdsError err = EdsCreateFileStream(fileName().c_str(),
                                       kEdsFileCreateDisposition_OpenExisting,
                                       kEdsAccess_Read, &mInputStream);
    if (err != EDS_ERR_OK) throw std::runtime_error("Open Error");

    err = EdsCreateImageRef(mInputStream, &mEdsImage);
    if (err != EDS_ERR_OK) throw std::runtime_error("Open Error");

    this->update();
}

bool ImageReaderCanon::isOpen() const
{
    mInputStream != nullptr;
}

void ImageReaderCanon::close()
{
    EdsRelease(mInputStream);
    EdsRelease(mEdsImage);
    mCols = 0;
    mRows = 0;
    mBands = 0;
    mDataType = DataType::TL_16;
    mColorDepth = 16;
}

cv::Mat ImageReaderCanon::read(const Rect<int> &rect,
                               const Size<int> size,
                               Affine<Point<int>> *trf)
{
}


cv::Mat ImageReaderCanon::read(double scaleX,
                               double scaleY,
                               const Rect<int> &rect,
                               Affine<Point<int>> *trf)
{

    RectI rect_to_read;
    Point<int> offset;
    RectI rect_full_image(0, 0, this->cols(), this->rows());
    if (rect.isEmpty()) {
        rect_to_read = rect_full_image;
    } else {
        rect_to_read = intersect(rect_full_image, rect);
        offset = rect_to_read.topLeft() - rect.topLeft();
    }

    Size<int> size_to_read;
    if (size.isEmpty()) {
        size_to_read = rect_to_read.size();
    } else {
        size_to_read = size;
    }

    double scaleX = size_to_read.width / static_cast<double>(rect_to_read.width);
    double scaleY = size_to_read.height / static_cast<double>(rect_to_read.height);
    offset.x *= scaleX; // Corregido por la escala
    offset.y *= scaleY;
    if (trf) trf->setParameters(offset.x, offset.y, scaleX, scaleY, 0.);

    EdsRect rect;
    rect.point.x = rect_to_read.x;
    rect.point.y = rect_to_read.y;
    rect.size.width = rect_to_read.width;
    rect.size.height = rect_to_read.height;

    EdsSize eds_size;
    eds_size.width = size_to_read.width;
    eds_size.height = size_to_read.height;

    EdsError err = EDS_ERR_OK;
    EdsStreamRef dstStreamRef;
    err = EdsCreateMemoryStream(0, &dstStreamRef);
    if (err == EDS_ERR_OK) {
        //EdsUInt32 sharpness = 2;
        //EdsError err = EdsSetPropertyData(mEdsImage, kEdsPropID_Sharpness, 0, sizeof(sharpness), &sharpness);

        EdsPictureStyleDesc psDesc;
        err = EdsGetPropertyData(mEdsImage, kEdsPropID_PictureStyleDesc | kEdsPropID_AtCapture_Flag, 0, sizeof(psDesc), &psDesc);

        //EdsPictureStyleDesc pictureStyleDesc;
        //pictureStyleDesc.sharpness = 2;
        psDesc.sharpThreshold = 0;
        psDesc.sharpFineness = 0;
        psDesc.sharpness = 0;

        err = EdsSetPropertyData(mEdsImage, kEdsPropID_PictureStyleDesc, 0, sizeof(psDesc), &psDesc);

        if (err != EDS_ERR_OK) {
            Message::error("EdsSetPropertyData: kEdsPropID_SaveTo");
            return Status::FAILURE;
        }
        err = EdsGetImage(mEdsImage, kEdsImageSrc_RAWFullView, kEdsTargetImageType_RGB, rect, eds_size, dstStreamRef);
        if (err == EDS_ERR_OK) {
            EdsVoid *pBuff;
            EdsGetPointer(dstStreamRef, &pBuff);
            aux = cv::Mat(size.height, size.width, CV_MAKETYPE(depth, mBands), pBuff);
            if (aux.empty() == false) {
                cvtColor(aux, *image, CV_RGB2BGR);
                status = Status::SUCCESS;
            }
        }
    }

    EdsRelease(dstStreamRef);
}

cv::Mat ImageReaderCanon::read(const WindowI &window,
                               double scaleX,
                               double scaleY,
                               Affine<Point<int>> *trf)
{
}

int ImageReaderCanon::rows() const
{
    return mRows;
}

int ImageReaderCanon::cols() const
{
    return mCols;
}

int ImageReaderCanon::channels() const
{
    return mBands;
}

DataType ImageReaderCanon::dataType() const
{
    return mDataType;
}

int ImageReaderCanon::depth() const
{
    return mColorDepth;
}

void ImageReaderCanon::update()
{
    EdsImageInfo imageInfo;
    EdsError err = EdsGetImageInfo(mEdsImage, kEdsImageSrc_FullView, &imageInfo);
    if (err == EDS_ERR_OK) {
        mCols = imageInfo.width;
        mRows = imageInfo.height;
        mBands = imageInfo.numOfComponents;
        mColorDepth = imageInfo.componentDepth;
        mDataType = imageInfo.componentDepth == 16 ? DataType::TL_16U : DataType::TL_8U;
    }
}

} // End namespace tl

#endif // TL_HAVE_EDSDK
