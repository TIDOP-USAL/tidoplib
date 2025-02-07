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

#include "tidop/featmatch/io/impl/features/opencvreader.h"
#include "tidop/core/base/exception.h"


namespace tl
{




FeaturesReaderOpenCV::FeaturesReaderOpenCV(tl::Path file)
    : FeaturesReader(std::move(file)),
    mFileStorage(nullptr)
{

}

FeaturesReaderOpenCV::~FeaturesReaderOpenCV()
{
    if (mFileStorage) {
        delete mFileStorage;
        mFileStorage = nullptr;
    }
}

void FeaturesReaderOpenCV::read()
{
    try {

        open();
        if (isOpen()) {
            readkeypoints();
            readDescriptor();
            close();
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void FeaturesReaderOpenCV::open()
{
    try {

        mFileStorage = new cv::FileStorage(mFilePath.toString(), cv::FileStorage::READ);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

bool FeaturesReaderOpenCV::isOpen()
{
    if (mFileStorage == nullptr) return false;
    return mFileStorage->isOpened();
}

void FeaturesReaderOpenCV::readkeypoints()
{
    try {

        if (mFileStorage) {
            mKeyPoints.resize(0);
            (*mFileStorage)["keypoints"] >> mKeyPoints;
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void FeaturesReaderOpenCV::readDescriptor()
{
    try {

        if (mFileStorage) {
            mDescriptors.resize(0);
            (*mFileStorage)["descriptors"] >> mDescriptors;
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void FeaturesReaderOpenCV::close()
{
    if (mFileStorage)
        mFileStorage->release();
}


} // namespace tl

