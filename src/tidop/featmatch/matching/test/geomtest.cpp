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

#include "tidop/featmatch/matching/test/geomtest.h"

#include "tidop/core/base/exception.h"
#include "tidop/core/base/common.h"
#include "tidop/featmatch/matching/test/impl/fundamental_7_points_test.h"
#include "tidop/featmatch/matching/test/impl/fundamental_8_points_test.h"
#include "tidop/featmatch/matching/test/impl/fundamental_lmeds_test.h"
#include "tidop/featmatch/matching/test/impl/fundamental_ransac_test.h"
#include "tidop/featmatch/matching/test/impl/fundamental_usac_test.h"
#include "tidop/featmatch/matching/test/impl/homography_all_points_test.h"
#include "tidop/featmatch/matching/test/impl/homography_lmeds_test.h"
#include "tidop/featmatch/matching/test/impl/homography_ransac_test.h"
#include "tidop/featmatch/matching/test/impl/homography_rho_test.h"
#include "tidop/featmatch/matching/test/impl/homography_usac_test.h"


namespace tl
{

GeometricTest::GeometricTest(Type type) 
  : mType(type)
{
}



auto GeometricTestFactory::create(GeometricTest::Type type) -> std::shared_ptr<GeometricTest>
{
    std::shared_ptr<GeometricTest> test;

    try {

        switch (type) {
        case tl::GeometricTest::Type::homography_all_points:
            test = std::make_shared<HomographyAllPointsTest>();
            break;
        case tl::GeometricTest::Type::homography_ransac:
            test = std::make_shared<HomographyRANSACTest>();
            break;
        case tl::GeometricTest::Type::homography_lmeds:
            test = std::make_shared<HomographyLMedsTest>();
            break;
        case tl::GeometricTest::Type::homography_rho:
            test = std::make_shared<HomographyRHOTest>();
            break;
#if (CV_VERSION_MAJOR > 4 || (CV_VERSION_MAJOR == 4 && CV_VERSION_MINOR > 4))
        case tl::GeometricTest::Type::homography_usac:
            test = std::make_shared<HomographyUsacTest>();
            break;
#endif
        case tl::GeometricTest::Type::fundamental_seven_points:
            test = std::make_shared<Fundamental7PointsTest>();
            break;
        case tl::GeometricTest::Type::fundamental_eight_points:
            test = std::make_shared<Fundamental8PointsTest>();
            break;
        case tl::GeometricTest::Type::fundamental_ransac:
            test = std::make_shared<FundamentalRANSACTest>();
            break;
        case tl::GeometricTest::Type::fundamental_lmeds:
            test = std::make_shared<FundamentalLMedsTest>();
            break;
#if (CV_VERSION_MAJOR > 4 || (CV_VERSION_MAJOR == 4 && CV_VERSION_MINOR > 4))
        case tl::GeometricTest::Type::fundamental_usac:
            test = std::make_shared<FundamentalUsacTest>();
            break;
#endif
        default:
            break;
        }

        TL_ASSERT(test, "No Geometric Test found");

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }

    return test;
}

auto GeometricTestFactory::create(GeometricTest::Type type,
                                  const GeometricTest::Properties *properties) -> std::shared_ptr<GeometricTest>
{
    std::shared_ptr<GeometricTest> test;

    try {

        test = GeometricTestFactory::create(type);
        test->setProperties(properties);

    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }

    return test;
}


} // namespace tl
