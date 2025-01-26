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

#include "geomtest.h"

#include "tidop/core/base/exception.h"
#include "tidop/core/base/common.h"

namespace tl
{

/// \cond

/* Homography geometric test */


class HomographyAllPointsTest
    : public GeometricTest
{

public:

    HomographyAllPointsTest()
        : GeometricTest(GeometricTest::Type::homography_all_points)
    {
    }
    ~HomographyAllPointsTest() override = default;

public:

    auto properties() const -> const Properties* override
    {
        return &mProperties;
    }

    void setProperties(const GeometricTest::Properties *properties) override
    {
        if (const AllPointsTestProperties *_properties = static_cast<const AllPointsTestProperties *>(properties))
            mProperties = *_properties;
    }

    auto exec(const std::vector<cv::Point2f> &points1,
              const std::vector<cv::Point2f> &points2) -> std::vector<unsigned char> override
    {
        std::vector<unsigned char> inliers;

        try {

            TL_ASSERT(points1.size() == points2.size(), "Feature sizes are not equal");

            inliers.resize(points1.size(), 0);

            cv::findHomography(cv::Mat(points1),
                               cv::Mat(points2),
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

private:

    AllPointsTestProperties mProperties;

};




class HomographyRANSACTest
    : public GeometricTest
{

public:

    HomographyRANSACTest()
        : GeometricTest(GeometricTest::Type::homography_ransac)
    {
    }
    ~HomographyRANSACTest() override = default;

public:

    auto properties() const -> const Properties* override
    {
        return &mProperties;
    }

    void setProperties(const GeometricTest::Properties *properties) override
    {
        if (const RANSACTestProperties *_properties = static_cast<const RANSACTestProperties *>(properties))
            mProperties = *_properties;
    }

    auto exec(const std::vector<cv::Point2f> &points1,
              const std::vector<cv::Point2f> &points2) -> std::vector<unsigned char> override
    {
        std::vector<unsigned char> inliers;

        try {

            TL_ASSERT(points1.size() == points2.size(), "Feature sizes are not equal");

            inliers.resize(points1.size(), 0);

            cv::findHomography(cv::Mat(points1),
                               cv::Mat(points2),
                               cv::RANSAC,
                               mProperties.distance,
                               inliers,
                               mProperties.iterations,
                               mProperties.confidence);
        } catch (...) {
            TL_THROW_EXCEPTION_WITH_NESTED("");
        }

        return inliers;
    }

private:

    RANSACTestProperties mProperties;

};




class HomographyLMedsTest
    : public GeometricTest
{

public:

    HomographyLMedsTest()
        : GeometricTest(GeometricTest::Type::homography_lmeds)
    {
    }
    ~HomographyLMedsTest() override = default;

public:

    auto properties() const -> const Properties* override
    {
        return &mProperties;
    }

    void setProperties(const GeometricTest::Properties *properties) override
    {
        if (const LMedsTestProperties *_properties = static_cast<const LMedsTestProperties *>(properties))
            mProperties = *_properties;
    }

    auto exec(const std::vector<cv::Point2f> &points1,
              const std::vector<cv::Point2f> &points2) -> std::vector<unsigned char> override
    {
        std::vector<unsigned char> inliers;

        try {

            TL_ASSERT(points1.size() == points2.size(), "Feature sizes are not equal");

            inliers.resize(points1.size(), 0);

            cv::findHomography(cv::Mat(points1),
                               cv::Mat(points2),
                               cv::LMEDS,
                               0, // Not used
                               inliers,
                               mProperties.iterations, // Not used
                               mProperties.confidence);
        } catch (...) {
            TL_THROW_EXCEPTION_WITH_NESTED("");
        }

        return inliers;
    }

private:

    LMedsTestProperties mProperties;

};




class HomographyRHOTest
    : public GeometricTest
{


public:

    HomographyRHOTest()
        : GeometricTest(GeometricTest::Type::homography_rho)
    {
    }
    ~HomographyRHOTest() override = default;

public:

    auto properties() const -> const Properties* override
    {
        return &mProperties;
    }

    void setProperties(const GeometricTest::Properties *properties) override
    {
        if (const RHOTestProperties *_properties = static_cast<const RHOTestProperties *>(properties))
            mProperties = *_properties;
    }

    auto exec(const std::vector<cv::Point2f> &points1,
              const std::vector<cv::Point2f> &points2) -> std::vector<unsigned char> override
    {
        std::vector<unsigned char> inliers;

        try {

            TL_ASSERT(points1.size() == points2.size(), "Feature sizes are not equal");

            inliers.resize(points1.size(), 0);

            cv::findHomography(cv::Mat(points1),
                               cv::Mat(points2),
                               cv::RHO,
                               mProperties.distance,
                               inliers,
                               0, // Not used
                               mProperties.confidence);
        } catch (...) {
            TL_THROW_EXCEPTION_WITH_NESTED("");
        }

        return inliers;
    }

private:

    RHOTestProperties mProperties;

};




#if (CV_VERSION_MAJOR > 4 || (CV_VERSION_MAJOR == 4 && CV_VERSION_MINOR > 4))

class HomographyUsacTest
    : public GeometricTest
{

public:

    HomographyUsacTest()
        : GeometricTest(GeometricTest::Type::homography_usac)
    {
    }
    ~HomographyUsacTest() override = default;

public:

    auto properties() const -> const Properties* override
    {
        return &mProperties;
    }

    void setProperties(const GeometricTest::Properties *properties) override
    {
        if (const UsacTestProperties *_properties = static_cast<const UsacTestProperties *>(properties))
            mProperties = *_properties;
    }

    auto exec(const std::vector<cv::Point2f> &points1,
              const std::vector<cv::Point2f> &points2) -> std::vector<unsigned char> override
    {
        std::vector<unsigned char> inliers;

        try {

            TL_ASSERT(points1.size() == points2.size(), "Feature sizes are not equal");

            inliers.resize(points1.size(), 0);

            cv::UsacParams params;
            params.confidence = mProperties.confidence;
            params.isParallel = mProperties.isParallel;
            params.loIterations = mProperties.loIterations;
            params.loMethod = mProperties.loMethod;
            params.loSampleSize = mProperties.loSampleSize;
            params.maxIterations = mProperties.maxIterations;
            params.neighborsSearch = mProperties.neighborsSearch;
            params.randomGeneratorState = mProperties.randomGeneratorState;
            params.sampler = mProperties.sampler;
            params.score = mProperties.score;
            params.threshold = mProperties.threshold;

            cv::findHomography(cv::Mat(points1),
                               cv::Mat(points2),
                               inliers,
                               params);
        } catch (...) {
            TL_THROW_EXCEPTION_WITH_NESTED("");
        }

        return inliers;
    }

private:

    UsacTestProperties mProperties;

};

#endif


/* Fundamental Matrix geometric test */


class Fundamental7PointsTest
    : public GeometricTest
{

public:

    Fundamental7PointsTest()
        : GeometricTest(GeometricTest::Type::fundamental_seven_points)
    {
    }

    ~Fundamental7PointsTest() override = default;

public:

    auto properties() const -> const Properties* override
    {
        return nullptr;
    }

    void setProperties(const GeometricTest::Properties *properties) override
    {
        unusedParameter(properties);
    }

    auto exec(const std::vector<cv::Point2f> &points1,
              const std::vector<cv::Point2f> &points2) -> std::vector<unsigned char> override
    {
        std::vector<unsigned char> inliers;

        try {

            TL_ASSERT(points1.size() == points2.size(), "Feature sizes are not equal");

            inliers.resize(points1.size(), 0);

            cv::findFundamentalMat(cv::Mat(points1),
                                   cv::Mat(points2),
                                   inliers,
                                   cv::FM_7POINT);

        } catch (...) {
            TL_THROW_EXCEPTION_WITH_NESTED("");
        }

        return inliers;
    }

};



class Fundamental8PointsTest
    : public GeometricTest
{

public:

    Fundamental8PointsTest()
        : GeometricTest(GeometricTest::Type::fundamental_eight_points)
    {
    }

    ~Fundamental8PointsTest() override = default;

public:

    auto properties() const -> const Properties* override
    {
        return nullptr;
    }

    void setProperties(const GeometricTest::Properties *properties = nullptr) override
    {
        unusedParameter(properties);
    }

    auto exec(const std::vector<cv::Point2f> &points1,
              const std::vector<cv::Point2f> &points2) -> std::vector<unsigned char> override
    {
        std::vector<unsigned char> inliers;

        try {

            TL_ASSERT(points1.size() == points2.size(), "Feature sizes are not equal");

            inliers.resize(points1.size(), 0);

            cv::findFundamentalMat(cv::Mat(points1),
                                   cv::Mat(points2),
                                   inliers,
                                   cv::FM_8POINT);

        } catch (...) {
            TL_THROW_EXCEPTION_WITH_NESTED("");
        }

        return inliers;
    }

};



class FundamentalRANSACTest
    : public GeometricTest
{

public:

    FundamentalRANSACTest()
        : GeometricTest(GeometricTest::Type::fundamental_ransac)
    {
    }

    ~FundamentalRANSACTest() override = default;

public:

    auto properties() const -> const Properties* override
    {
        return &mProperties;
    }

    void setProperties(const GeometricTest::Properties *properties = nullptr) override
    {
        if (const RANSACTestProperties *_properties = static_cast<const RANSACTestProperties *>(properties))
            mProperties = *_properties;
    }

    auto exec(const std::vector<cv::Point2f> &points1,
              const std::vector<cv::Point2f> &points2) -> std::vector<unsigned char> override
    {
        std::vector<unsigned char> inliers;

        try {

            TL_ASSERT(points1.size() == points2.size(), "Feature sizes are not equal");

            inliers.resize(points1.size(), 0);

            cv::findFundamentalMat(cv::Mat(points1),
                                   cv::Mat(points2),
                                   cv::FM_RANSAC,
                                   mProperties.distance,
                                   mProperties.confidence,
                                   mProperties.iterations,
                                   inliers);

        } catch (...) {
            TL_THROW_EXCEPTION_WITH_NESTED("");
        }

        return inliers;
    }

private:

    RANSACTestProperties mProperties;

};



class FundamentalLMedSTest
    : public GeometricTest
{

public:

    FundamentalLMedSTest()
        : GeometricTest(GeometricTest::Type::fundamental_lmeds)
    {
    }

    ~FundamentalLMedSTest() override = default;

public:

    auto properties() const -> const Properties* override
    {
        return &mProperties;
    }

    void setProperties(const GeometricTest::Properties *properties = nullptr) override
    {
        if (const LMedsTestProperties *_properties = static_cast<const LMedsTestProperties *>(properties))
            mProperties = *_properties;
    }

    auto exec(const std::vector<cv::Point2f> &points1,
              const std::vector<cv::Point2f> &points2) -> std::vector<unsigned char> override
    {
        std::vector<unsigned char> inliers;

        try {

            TL_ASSERT(points1.size() == points2.size(), "Feature sizes are not equal");

            inliers.resize(points1.size(), 0);

            cv::findFundamentalMat(cv::Mat(points1),
                                   cv::Mat(points2),
                                   cv::FM_LMEDS,
                                   0,
                                   mProperties.confidence,
                                   mProperties.iterations,
                                   inliers);
        } catch (...) {
            TL_THROW_EXCEPTION_WITH_NESTED("");
        }

        return inliers;
    }

private:

    LMedsTestProperties mProperties;

};



class FundamentalUsacTest
    : public GeometricTest
{

public:

    FundamentalUsacTest()
        : GeometricTest(GeometricTest::Type::fundamental_usac)
    {
    }

    ~FundamentalUsacTest() override = default;

public:

    auto properties() const -> const Properties* override
    {
        return &mProperties;
    }

    void setProperties(const GeometricTest::Properties *properties = nullptr) override
    {
        if (const UsacTestProperties *_properties = static_cast<const UsacTestProperties *>(properties))
            mProperties = *_properties;
    }

    auto exec(const std::vector<cv::Point2f> &points1,
              const std::vector<cv::Point2f> &points2) -> std::vector<unsigned char> override
    {
        std::vector<unsigned char> inliers;

        try {

            TL_ASSERT(points1.size() == points2.size(), "Feature sizes are not equal");

            inliers.resize(points1.size(), 0);

            cv::UsacParams params;
            params.confidence = mProperties.confidence;
            params.isParallel = mProperties.isParallel;
            params.loIterations = mProperties.loIterations;
            params.loMethod = mProperties.loMethod;
            params.loSampleSize = mProperties.loSampleSize;
            params.maxIterations = mProperties.maxIterations;
            params.neighborsSearch = mProperties.neighborsSearch;
            params.randomGeneratorState = mProperties.randomGeneratorState;
            params.sampler = mProperties.sampler;
            params.score = mProperties.score;
            params.threshold = mProperties.threshold;
            cv::findFundamentalMat(cv::Mat(points1),
                                   cv::Mat(points2),
                                   inliers,
                                   params);

        } catch (...) {
            TL_THROW_EXCEPTION_WITH_NESTED("");
        }

        return inliers;
    }

private:

    UsacTestProperties mProperties;

};

/// \endcond



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
            test = std::make_shared<FundamentalLMedSTest>();
            break;
#if (CV_VERSION_MAJOR > 4 || (CV_VERSION_MAJOR == 4 && CV_VERSION_MINOR > 4))
        case tl::GeometricTest::Type::fundamental_usac:
            test = std::make_shared<FundamentalUsacTest>();
            break;
#endif
        default:
            break;
        }

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
