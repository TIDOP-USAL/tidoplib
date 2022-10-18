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
 
#define BOOST_TEST_MODULE Tidop GeometricTest test
#include <boost/test/unit_test.hpp>

#include <tidop/featmatch/geomtest.h>

using namespace tl;


BOOST_AUTO_TEST_SUITE(GeometricTestSuite)

struct GeometricTestTest
{
  GeometricTestTest()
  { }
    
  ~GeometricTestTest()
  { 
  }

  void setup()
  {
    homographyAllPointsTest = GeometricTestFactory::create(GeometricTest::Type::homography_all_points);
    homographyRANSACTest = GeometricTestFactory::create(GeometricTest::Type::homography_ransac);
    homographyLMedsTest = GeometricTestFactory::create(GeometricTest::Type::homography_lmeds);
    homographyRHOTest = GeometricTestFactory::create(GeometricTest::Type::homography_rho);
#if (CV_VERSION_MAJOR > 4 || (CV_VERSION_MAJOR == 4 && CV_VERSION_MINOR > 4))
    homographyUsacTest = GeometricTestFactory::create(GeometricTest::Type::homography_usac);
#endif

    fundamentalSevenPointsTest = GeometricTestFactory::create(GeometricTest::Type::fundamental_seven_points);
    fundamentalEightPointsTest = GeometricTestFactory::create(GeometricTest::Type::fundamental_eight_points);
    fundamentalRANSACTest = GeometricTestFactory::create(GeometricTest::Type::fundamental_ransac);
    fundamentalLMedsTest = GeometricTestFactory::create(GeometricTest::Type::fundamental_lmeds);
#if (CV_VERSION_MAJOR > 4 || (CV_VERSION_MAJOR == 4 && CV_VERSION_MINOR > 4))
    fundamentalUsacTest = GeometricTestFactory::create(GeometricTest::Type::fundamental_usac);
#endif
  }

  void teardown()
  {
  }

  std::shared_ptr<GeometricTest> homographyAllPointsTest;
  std::shared_ptr<GeometricTest> homographyRANSACTest;
  std::shared_ptr<GeometricTest> homographyLMedsTest;
  std::shared_ptr<GeometricTest> homographyRHOTest;
#if (CV_VERSION_MAJOR > 4 || (CV_VERSION_MAJOR == 4 && CV_VERSION_MINOR > 4))
  std::shared_ptr<GeometricTest> homographyUsacTest;
#endif

  std::shared_ptr<GeometricTest> fundamentalSevenPointsTest;
  std::shared_ptr<GeometricTest> fundamentalEightPointsTest;
  std::shared_ptr<GeometricTest> fundamentalRANSACTest;
  std::shared_ptr<GeometricTest> fundamentalLMedsTest;
#if (CV_VERSION_MAJOR > 4 || (CV_VERSION_MAJOR == 4 && CV_VERSION_MINOR > 4))
  std::shared_ptr<GeometricTest> fundamentalUsacTest;
#endif
};

BOOST_FIXTURE_TEST_CASE(default_constructor, GeometricTestTest)
{
  GeometricTest::Type type = homographyAllPointsTest->type();
  BOOST_CHECK(type == GeometricTest::Type::homography_all_points);
  const GeometricTest::Properties *properties = homographyAllPointsTest->properties();
  const AllPointsTestProperties *all_points_test_properties = static_cast<const AllPointsTestProperties *>(properties);
  BOOST_CHECK_EQUAL(0.999, all_points_test_properties->confidence);

  type = homographyRANSACTest->type();
  BOOST_CHECK(type == GeometricTest::Type::homography_ransac);
  properties = homographyRANSACTest->properties();
  const RANSACTestProperties *ransac_test_properties = static_cast<const RANSACTestProperties *>(properties);
  BOOST_CHECK_EQUAL(0.7, ransac_test_properties->distance);
  BOOST_CHECK_EQUAL(0.999, ransac_test_properties->confidence);
  BOOST_CHECK_EQUAL(2000, ransac_test_properties->iterations);

  type = homographyLMedsTest->type();
  BOOST_CHECK(type == GeometricTest::Type::homography_lmeds);
  properties = homographyLMedsTest->properties();
  const LMedsTestProperties *lmeds_test_properties = static_cast<const LMedsTestProperties *>(properties);
  BOOST_CHECK_EQUAL(0.999, lmeds_test_properties->confidence);
  BOOST_CHECK_EQUAL(2000, lmeds_test_properties->iterations);

  type = homographyRHOTest->type();
  BOOST_CHECK(type == GeometricTest::Type::homography_rho);
  properties = homographyRHOTest->properties();
  const RHOTestProperties *rho_test_properties = static_cast<const RHOTestProperties *>(properties);
  BOOST_CHECK_EQUAL(0.7, rho_test_properties->distance);
  BOOST_CHECK_EQUAL(0.999, rho_test_properties->confidence);

#if (CV_VERSION_MAJOR > 4 || (CV_VERSION_MAJOR == 4 && CV_VERSION_MINOR > 4))
  type = homographyUsacTest->type();
  BOOST_CHECK(type == GeometricTest::Type::homography_usac);
  properties = homographyUsacTest->properties();
  const UsacTestProperties *usac_test_properties = static_cast<const UsacTestProperties *>(properties);
  BOOST_CHECK_EQUAL(0.99, usac_test_properties->confidence);
  BOOST_CHECK_EQUAL(false, usac_test_properties->isParallel);
  BOOST_CHECK_EQUAL(5, usac_test_properties->loIterations);
  BOOST_CHECK_EQUAL(cv::LocalOptimMethod::LOCAL_OPTIM_INNER_LO, usac_test_properties->loMethod);
  BOOST_CHECK_EQUAL(14, usac_test_properties->loSampleSize);
  BOOST_CHECK_EQUAL(5000, usac_test_properties->maxIterations);
  BOOST_CHECK_EQUAL(cv::NeighborSearchMethod::NEIGH_GRID, usac_test_properties->neighborsSearch);
  BOOST_CHECK_EQUAL(0, usac_test_properties->randomGeneratorState);
  BOOST_CHECK_EQUAL(cv::SamplingMethod::SAMPLING_UNIFORM, usac_test_properties->sampler);
  BOOST_CHECK_EQUAL(cv::ScoreMethod::SCORE_METHOD_MSAC, usac_test_properties->score);
  BOOST_CHECK_EQUAL(1.5, usac_test_properties->threshold);
#endif

  type = fundamentalSevenPointsTest->type();
  BOOST_CHECK(type == GeometricTest::Type::fundamental_seven_points);

  type = fundamentalEightPointsTest->type();
  BOOST_CHECK(type == GeometricTest::Type::fundamental_eight_points);

  type = fundamentalRANSACTest->type();
  BOOST_CHECK(type == GeometricTest::Type::fundamental_ransac);
  properties = fundamentalRANSACTest->properties();
  ransac_test_properties = static_cast<const RANSACTestProperties *>(properties);
  BOOST_CHECK_EQUAL(0.7, ransac_test_properties->distance);
  BOOST_CHECK_EQUAL(0.999, ransac_test_properties->confidence);
  BOOST_CHECK_EQUAL(2000, ransac_test_properties->iterations);

  type = fundamentalLMedsTest->type();
  BOOST_CHECK(type == GeometricTest::Type::fundamental_lmeds);
  properties = fundamentalLMedsTest->properties();
  lmeds_test_properties = static_cast<const LMedsTestProperties *>(properties);
  BOOST_CHECK_EQUAL(0.999, lmeds_test_properties->confidence);
  BOOST_CHECK_EQUAL(2000, lmeds_test_properties->iterations);

#if (CV_VERSION_MAJOR > 4 || (CV_VERSION_MAJOR == 4 && CV_VERSION_MINOR > 4))
  type = fundamentalUsacTest->type();
  BOOST_CHECK(type == GeometricTest::Type::fundamental_usac);
  properties = fundamentalUsacTest->properties();
  usac_test_properties = static_cast<const UsacTestProperties *>(properties);
  BOOST_CHECK_EQUAL(0.99, usac_test_properties->confidence);
  BOOST_CHECK_EQUAL(false, usac_test_properties->isParallel);
  BOOST_CHECK_EQUAL(5, usac_test_properties->loIterations);
  BOOST_CHECK_EQUAL(cv::LocalOptimMethod::LOCAL_OPTIM_INNER_LO, usac_test_properties->loMethod);
  BOOST_CHECK_EQUAL(14, usac_test_properties->loSampleSize);
  BOOST_CHECK_EQUAL(5000, usac_test_properties->maxIterations);
  BOOST_CHECK_EQUAL(cv::NeighborSearchMethod::NEIGH_GRID, usac_test_properties->neighborsSearch);
  BOOST_CHECK_EQUAL(0, usac_test_properties->randomGeneratorState);
  BOOST_CHECK_EQUAL(cv::SamplingMethod::SAMPLING_UNIFORM, usac_test_properties->sampler);
  BOOST_CHECK_EQUAL(cv::ScoreMethod::SCORE_METHOD_MSAC, usac_test_properties->score);
  BOOST_CHECK_EQUAL(1.5, usac_test_properties->threshold);
#endif
}

BOOST_FIXTURE_TEST_CASE(setProperties, GeometricTestTest)
{
  {
    AllPointsTestProperties all_points_test_properties;
    all_points_test_properties.confidence = 0.98;
    homographyAllPointsTest->setProperties(&all_points_test_properties);
  }

  const GeometricTest::Properties *properties = homographyAllPointsTest->properties();
  const AllPointsTestProperties *all_points_test_properties = static_cast<const AllPointsTestProperties *>(properties);
  BOOST_CHECK_EQUAL(0.98, all_points_test_properties->confidence);


  {
    RANSACTestProperties ransac_test_properties;
    ransac_test_properties.confidence = 0.98;
    ransac_test_properties.distance = 0.8;
    ransac_test_properties.iterations = 1000;
    homographyRANSACTest->setProperties(&ransac_test_properties);
  }

  properties = homographyRANSACTest->properties();
  const RANSACTestProperties *ransac_test_properties = static_cast<const RANSACTestProperties *>(properties);
  BOOST_CHECK_EQUAL(0.8, ransac_test_properties->distance);
  BOOST_CHECK_EQUAL(0.98, ransac_test_properties->confidence);
  BOOST_CHECK_EQUAL(1000, ransac_test_properties->iterations);


  {
    LMedsTestProperties lmeds_test_properties;
    lmeds_test_properties.confidence = 0.98;
    lmeds_test_properties.iterations = 1000;
    homographyLMedsTest->setProperties(&lmeds_test_properties);
  }

  properties = homographyLMedsTest->properties();
  const LMedsTestProperties *lmeds_test_properties = static_cast<const LMedsTestProperties *>(properties);
  BOOST_CHECK_EQUAL(0.98, lmeds_test_properties->confidence);
  BOOST_CHECK_EQUAL(1000, lmeds_test_properties->iterations);


  {
    RHOTestProperties rho_test_properties;
    rho_test_properties.confidence = 0.98;
    rho_test_properties.distance = 0.8;
    homographyRHOTest->setProperties(&rho_test_properties);
  }

  properties = homographyRHOTest->properties();
  const RHOTestProperties *rho_test_properties = static_cast<const RHOTestProperties *>(properties);
  BOOST_CHECK_EQUAL(0.8, rho_test_properties->distance);
  BOOST_CHECK_EQUAL(0.98, rho_test_properties->confidence);


#if (CV_VERSION_MAJOR > 4 || (CV_VERSION_MAJOR == 4 && CV_VERSION_MINOR > 4))
  {
    UsacTestProperties usac_test_properties;
    usac_test_properties.confidence = 0.97;
    usac_test_properties.isParallel = true;
    usac_test_properties.loIterations = 7;
    usac_test_properties.loMethod = cv::LocalOptimMethod::LOCAL_OPTIM_INNER_AND_ITER_LO;
    usac_test_properties.loSampleSize = 11;
    usac_test_properties.maxIterations = 2000;
    usac_test_properties.neighborsSearch = cv::NeighborSearchMethod::NEIGH_FLANN_KNN;
    usac_test_properties.randomGeneratorState = 1;
    usac_test_properties.sampler = cv::SamplingMethod::SAMPLING_PROGRESSIVE_NAPSAC;
    usac_test_properties.score = cv::ScoreMethod::SCORE_METHOD_RANSAC;
    usac_test_properties.threshold = 2.0;
    homographyUsacTest->setProperties(&usac_test_properties);
  }

  properties = homographyUsacTest->properties();
  const UsacTestProperties *usac_test_properties = static_cast<const UsacTestProperties *>(properties);
  BOOST_CHECK_EQUAL(0.97, usac_test_properties->confidence);
  BOOST_CHECK_EQUAL(true, usac_test_properties->isParallel);
  BOOST_CHECK_EQUAL(7, usac_test_properties->loIterations);
  BOOST_CHECK_EQUAL(cv::LocalOptimMethod::LOCAL_OPTIM_INNER_AND_ITER_LO, usac_test_properties->loMethod);
  BOOST_CHECK_EQUAL(11, usac_test_properties->loSampleSize);
  BOOST_CHECK_EQUAL(2000, usac_test_properties->maxIterations);
  BOOST_CHECK_EQUAL(cv::NeighborSearchMethod::NEIGH_FLANN_KNN, usac_test_properties->neighborsSearch);
  BOOST_CHECK_EQUAL(1, usac_test_properties->randomGeneratorState);
  BOOST_CHECK_EQUAL(cv::SamplingMethod::SAMPLING_PROGRESSIVE_NAPSAC, usac_test_properties->sampler);
  BOOST_CHECK_EQUAL(cv::ScoreMethod::SCORE_METHOD_RANSAC, usac_test_properties->score);
  BOOST_CHECK_EQUAL(2.0, usac_test_properties->threshold);
#endif


  {
    RANSACTestProperties ransac_test_properties;
    ransac_test_properties.confidence = 0.98;
    ransac_test_properties.distance = 0.8;
    ransac_test_properties.iterations = 1000;
    fundamentalRANSACTest->setProperties(&ransac_test_properties);
  }

  properties = fundamentalRANSACTest->properties();
  ransac_test_properties = static_cast<const RANSACTestProperties *>(properties);
  BOOST_CHECK_EQUAL(0.8, ransac_test_properties->distance);
  BOOST_CHECK_EQUAL(0.98, ransac_test_properties->confidence);
  BOOST_CHECK_EQUAL(1000, ransac_test_properties->iterations);


  {
    LMedsTestProperties lmeds_test_properties;
    lmeds_test_properties.confidence = 0.98;
    lmeds_test_properties.iterations = 1000;
    fundamentalLMedsTest->setProperties(&lmeds_test_properties);
  }

  properties = fundamentalLMedsTest->properties();
  lmeds_test_properties = static_cast<const LMedsTestProperties *>(properties);
  BOOST_CHECK_EQUAL(0.98, lmeds_test_properties->confidence);
  BOOST_CHECK_EQUAL(1000, lmeds_test_properties->iterations);


#if (CV_VERSION_MAJOR > 4 || (CV_VERSION_MAJOR == 4 && CV_VERSION_MINOR > 4))
  {
    UsacTestProperties usac_test_properties;
    usac_test_properties.confidence = 0.97;
    usac_test_properties.isParallel = true;
    usac_test_properties.loIterations = 7;
    usac_test_properties.loMethod = cv::LocalOptimMethod::LOCAL_OPTIM_INNER_AND_ITER_LO;
    usac_test_properties.loSampleSize = 11;
    usac_test_properties.maxIterations = 2000;
    usac_test_properties.neighborsSearch = cv::NeighborSearchMethod::NEIGH_FLANN_KNN;
    usac_test_properties.randomGeneratorState = 1;
    usac_test_properties.sampler = cv::SamplingMethod::SAMPLING_PROGRESSIVE_NAPSAC;
    usac_test_properties.score = cv::ScoreMethod::SCORE_METHOD_RANSAC;
    usac_test_properties.threshold = 2.0;
    fundamentalUsacTest->setProperties(&usac_test_properties);
  }

  properties = fundamentalUsacTest->properties();
  usac_test_properties = static_cast<const UsacTestProperties *>(properties);
  BOOST_CHECK_EQUAL(0.97, usac_test_properties->confidence);
  BOOST_CHECK_EQUAL(true, usac_test_properties->isParallel);
  BOOST_CHECK_EQUAL(7, usac_test_properties->loIterations);
  BOOST_CHECK_EQUAL(cv::LocalOptimMethod::LOCAL_OPTIM_INNER_AND_ITER_LO, usac_test_properties->loMethod);
  BOOST_CHECK_EQUAL(11, usac_test_properties->loSampleSize);
  BOOST_CHECK_EQUAL(2000, usac_test_properties->maxIterations);
  BOOST_CHECK_EQUAL(cv::NeighborSearchMethod::NEIGH_FLANN_KNN, usac_test_properties->neighborsSearch);
  BOOST_CHECK_EQUAL(1, usac_test_properties->randomGeneratorState);
  BOOST_CHECK_EQUAL(cv::SamplingMethod::SAMPLING_PROGRESSIVE_NAPSAC, usac_test_properties->sampler);
  BOOST_CHECK_EQUAL(cv::ScoreMethod::SCORE_METHOD_RANSAC, usac_test_properties->score);
  BOOST_CHECK_EQUAL(2.0, usac_test_properties->threshold);
#endif
}

BOOST_AUTO_TEST_SUITE_END() 
