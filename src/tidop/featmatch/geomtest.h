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

#pragma once

#include "tidop/core/defs.h"

#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>

namespace tl
{


class TL_EXPORT GeometricTest
{

public:

    struct Properties
    {

    };

    enum class Type
    {
        homography_all_points,
        homography_ransac,
        homography_lmeds,
        homography_rho,
#if (CV_VERSION_MAJOR > 4 || (CV_VERSION_MAJOR == 4 && CV_VERSION_MINOR > 4))
        homography_usac,
#endif
        fundamental_seven_points,
        fundamental_eight_points,
        fundamental_ransac,
        fundamental_lmeds
#if (CV_VERSION_MAJOR > 4 || (CV_VERSION_MAJOR == 4 && CV_VERSION_MINOR > 4))
        ,
        fundamental_usac
#endif
    };

private:

    Type mType;

public:

    GeometricTest(Type type) : mType(type) {}
    virtual ~GeometricTest() = default;

public:

    virtual const Properties *properties() const = 0;
    virtual void setProperties(const Properties *properties) = 0;
    virtual std::vector<unsigned char> exec(const std::vector<cv::Point2f> &points1,
                                            const std::vector<cv::Point2f> &points2) = 0;

    Type type()
    {
        return mType;
    }

};



struct TL_EXPORT AllPointsTestProperties
  : GeometricTest::Properties
{
    double confidence{0.999};
};

struct TL_EXPORT RANSACTestProperties
  : GeometricTest::Properties
{
    double distance{0.7};
    double confidence{0.999};
    int iterations{2000};
};

struct TL_EXPORT LMedsTestProperties
  : GeometricTest::Properties
{
    double confidence{0.999};
    int iterations{2000};
};

struct TL_EXPORT RHOTestProperties
  : GeometricTest::Properties
{
    double distance{0.7};
    double confidence{0.999};
};

#if (CV_VERSION_MAJOR > 4 || (CV_VERSION_MAJOR == 4 && CV_VERSION_MINOR > 4))

struct TL_EXPORT UsacTestProperties
  : GeometricTest::Properties
{
    double confidence{0.99};
    bool isParallel{false};
    int	loIterations{5};
    cv::LocalOptimMethod loMethod{cv::LocalOptimMethod::LOCAL_OPTIM_INNER_LO};
    int loSampleSize{14};
    int maxIterations{5000};
    cv::NeighborSearchMethod neighborsSearch{cv::NeighborSearchMethod::NEIGH_GRID};
    int randomGeneratorState{0};
    cv::SamplingMethod sampler{cv::SamplingMethod::SAMPLING_UNIFORM};
    cv::ScoreMethod score{cv::ScoreMethod::SCORE_METHOD_MSAC};
    double threshold{1.5};
};

#endif


class TL_EXPORT GeometricTestFactory
{

private:

    GeometricTestFactory() = default;

public:

    static std::shared_ptr<GeometricTest> create(GeometricTest::Type type);
    static std::shared_ptr<GeometricTest> create(GeometricTest::Type type,
                                                 const GeometricTest::Properties *properties);

};

} // namespace tl
