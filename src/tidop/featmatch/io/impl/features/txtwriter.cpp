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

#include "tidop/featmatch/io/impl/features/txtwriter.h"
#include "tidop/core/base/exception.h"


namespace tl
{


FeaturesWriterTxt::FeaturesWriterTxt(tl::Path file)
    : FeaturesWriter(std::move(file))
{

}

void FeaturesWriterTxt::write()
{
    try {

        open();
        if (isOpen()) {
            writeHeader();
            writeBody();
            close();
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void FeaturesWriterTxt::open()
{
    try {

        ofs = std::ofstream(filePath().toString(), std::ofstream::trunc);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

bool FeaturesWriterTxt::isOpen()
{
    return ofs.is_open();
}

void FeaturesWriterTxt::writeHeader()
{
    try {

        ofs << keyPoints().size() << " " << descriptors().cols << " " << descriptors().type() << std::endl;

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void FeaturesWriterTxt::writeBody()
{
    try {

        int size = static_cast<int>(keyPoints().size());
        int type = descriptors().type();

        for (int r = 0; r < size; r++) {
            cv::KeyPoint kp = keyPoints()[static_cast<size_t>(r)];
            ofs << kp.pt.x << " " << kp.pt.y << " " << kp.size << " " << kp.angle;
            for (int c = 0; c < descriptors().cols; c++) {

                switch (type) {
                case CV_8U:
                    ofs << " " << static_cast<int>(descriptors().at<uchar>(r, c));
                    break;
                case CV_8S:
                    ofs << " " << static_cast<int>(descriptors().at<schar>(r, c));
                    break;
                case CV_16U:
                    ofs << " " << static_cast<int>(descriptors().at<ushort>(r, c));
                    break;
                case CV_16S:
                    ofs << " " << static_cast<int>(descriptors().at<short>(r, c));
                    break;
                case CV_32S:
                    ofs << " " << descriptors().at<int>(r, c);
                    break;
                case CV_32F:
                    ofs << " " << descriptors().at<float>(r, c);
                    break;
                case CV_64F:
                    ofs << " " << descriptors().at<double>(r, c);
                    break;
                default:
                    ofs << " " << -1;
                    break;
                }
            }
            ofs << std::endl;
        }

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("Catched exception");
    }
}

void FeaturesWriterTxt::close()
{
    ofs.close();
}



} // namespace tl

