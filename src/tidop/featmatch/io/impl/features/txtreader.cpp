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

#include "tidop/featmatch/io/impl/features/txtreader.h"
#include "tidop/core/base/exception.h"
#include "tidop/core/base/split.h"

namespace tl
{

FeaturesReaderTxt::FeaturesReaderTxt(tl::Path file)
    : FeaturesReader(std::move(file))
{

}

void FeaturesReaderTxt::read()
{
    try {

        open();
        if (isOpen()) {
            readHeader();
            readBody();
            close();
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void FeaturesReaderTxt::open()
{
    try {

        ifs = std::ifstream(mFilePath.toString());

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

bool FeaturesReaderTxt::isOpen()
{
    return ifs.is_open();
}

void FeaturesReaderTxt::readHeader()
{
    try {

        std::string line;
        std::getline(ifs, line);
        std::istringstream stream(line);
        stream >> mType >> mSize >> mCols;

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void FeaturesReaderTxt::readBody()
{
    try {

        keyPoints().resize(static_cast<size_t>(mSize));
        mDescriptors = cv::Mat(mSize, mCols, mType);

        int r = 0;
        std::string line;
        while (std::getline(ifs, line)) {

            std::vector<std::string> list = split<std::string>(line, ' ');
            keyPoints()[static_cast<size_t>(r)].pt.x = convertStringTo<float>(list[0]);
            keyPoints()[static_cast<size_t>(r)].pt.y = convertStringTo<float>(list[1]);
            keyPoints()[static_cast<size_t>(r)].size = convertStringTo<float>(list[2]);
            keyPoints()[static_cast<size_t>(r)].angle = convertStringTo<float>(list[3]);

            for (int c = 0; c < mCols; c++) {
                switch (mType) {
                case CV_8U:
                    mDescriptors.at<uchar>(r, c) = static_cast<uchar>(convertStringTo<int>(list[c + 4]));
                    break;
                case CV_8S:
                    mDescriptors.at<schar>(r, c) = static_cast<schar>(convertStringTo<int>(list[c + 4]));
                    break;
                case CV_16U:
                    mDescriptors.at<ushort>(r, c) = static_cast<ushort>(convertStringTo<int>(list[c + 4]));
                    break;
                case CV_16S:
                    mDescriptors.at<short>(r, c) = static_cast<short>(convertStringTo<int>(list[c + 4]));
                    break;
                case CV_32S:
                    mDescriptors.at<int>(r, c) = convertStringTo<int>(list[c + 4]);
                    break;
                case CV_32F:
                    mDescriptors.at<float>(r, c) = convertStringTo<float>(list[c + 4]);
                    break;
                case CV_64F:
                    mDescriptors.at<double>(r, c) = convertStringTo<double>(list[c + 4]);
                    break;
                default:
                    break;
                }
            }
            r++;
        }
    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void FeaturesReaderTxt::close()
{
    ifs.close();
}


} // namespace tl

