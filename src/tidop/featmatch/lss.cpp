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
 * License along with Foobar. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/
 
#include "lss.h"

#include "tidop/core/exception.h"

#include "lss/lss.h"

namespace tl
{


LssProperties::LssProperties()
{
}

LssProperties::~LssProperties() = default;

void LssProperties::reset()
{
}

auto LssProperties::name() const -> std::string
{
    return std::string("LSS");
}

/*----------------------------------------------------------------*/


LssDescriptor::LssDescriptor()
  : DescriptorExtractor(),
    mLSS(new LSS)
{
    update();
}

LssDescriptor::~LssDescriptor() = default;

void LssDescriptor::update()
{
}

auto LssDescriptor::extract(const cv::Mat& img,
                            std::vector<cv::KeyPoint>& keyPoints) -> cv::Mat
{
    cv::Mat descriptors;

    try {

        cv::Mat cvImg;
        img.convertTo(cvImg, CV_32F);
        std::vector<cv::Mat_<float>> aux_des_left;

        std::vector<cv::KeyPoint> lss_key;

        for (size_t i = 0; i < keyPoints.size(); i++) {
            cv::Mat_<float> enter;
            int error = mLSS->compute(keyPoints[i].pt, cvImg, enter);
            if (error == 1) {
                aux_des_left.push_back(enter);
                lss_key.push_back(keyPoints[i]);
            }
        }

        cv::Mat tempDesc = cv::Mat(static_cast<int>(aux_des_left.size()), aux_des_left[0].rows * aux_des_left[0].cols, CV_32FC1);

        for (size_t i = 0; i < aux_des_left.size(); i++) {
            int i1 = 0;
            for (int j = 0; j < aux_des_left[i].cols; j++) {

                for (int k = 0; k < aux_des_left[i].rows; k++) {
                    float aux = aux_des_left[i].at<float>(k, j);
                    tempDesc.at<float>(static_cast<int>(i), i1) = aux;
                    i1++;
                }

            }
        }

        descriptors = tempDesc;
        keyPoints = lss_key;

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }

    return descriptors;
}

void LssDescriptor::reset()
{
    LssProperties::reset();
    update();
}


} // namespace tl
