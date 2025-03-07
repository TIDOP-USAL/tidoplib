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

#include "homography_all_points_test.h"
#include "tidop/core/base/exception.h"

#include <opencv2/calib3d.hpp>

namespace tl
{

HomographyAllPointsTest::HomographyAllPointsTest()
  : GeometricTest(GeometricTest::Type::homography_all_points)
{
}

void HomographyAllPointsTest::setProperties(const GeometricTest::Properties *properties)
{
    if (const AllPointsTestProperties *_properties = static_cast<const AllPointsTestProperties *>(properties))
        mProperties = *_properties;
}

auto HomographyAllPointsTest::exec(const std::vector<cv::Point2f> &points1, 
                                   const std::vector<cv::Point2f> &points2) -> std::vector<unsigned char>
{
    std::vector<unsigned char> inliers;

    try {

        TL_ASSERT(points1.size() == points2.size(), "Feature sizes are not equal");

        inliers.resize(points1.size(), 0);

        cv::findHomography(points1,
                           points2,
                           0,
                           3, // Not used
                           inliers,
                           2000, // Not used
                           mProperties.confidence);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }

    return inliers;
}



} // namespace tl
