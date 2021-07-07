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

#define BOOST_TEST_MODULE Tidop statistic test
#include <boost/test/unit_test.hpp>

#include <tidop/math/statistics.h>
#include <tidop/math/algebra/vector.h>

#include <array>
#include <list>

using namespace tl::math;


BOOST_AUTO_TEST_SUITE(StatisticsTestSuite)

struct StatisticsTest
{

  StatisticsTest() {}
  ~StatisticsTest() {}

  void setup()
  {
    vd.push_back(8.0);
    vd.push_back(8.5);
    vd.push_back(7.5);
    vd.push_back(9.0);
    vd.push_back(6.25);
    vd.push_back(5.5);
    vd.push_back(8.5);
    vd.push_back(7.5);
    vd.push_back(8.5);

    vi.push_back(1);
    vi.push_back(0);
    vi.push_back(1);
    vi.push_back(3);
    vi.push_back(2);
    vi.push_back(0);
    vi.push_back(1);

    vi2.push_back(17);
    vi2.push_back(15);
    vi2.push_back(23);
    vi2.push_back(7);
    vi2.push_back(9);
    vi2.push_back(13);

    l = { 1, 3, 4, 2, 6, 5, 8, 7};

    x = { 0., 1., 2., 3., 4., 5., 6., 7., 8., 9.};
    y = { 92.8, 92.3, 80., 89.1, 83.5, 68.9, 69.2, 67.1, 58.3, 61.2};

    tl_vector[0] = 8.0;
    tl_vector[1] = 8.5;
    tl_vector[2] = 7.5;
    tl_vector[3] = 9.0;
    tl_vector[4] = 6.25;
    tl_vector[5] = 5.5;
    tl_vector[6] = 8.5;
    tl_vector[7] = 7.5;
    tl_vector[8] = 8.5;
  }

  void teardown()
  {

  }

  std::vector<double> vd;
  std::vector<int> vi;
  std::vector<int> vi2;
  std::vector<float> vf;

  std::array<double, 3> a;
  std::list<int> l;

  std::list<double> x;
  std::list<double> y;

  tl::math::Vector<double, 9> tl_vector;
};

BOOST_FIXTURE_TEST_CASE(mean, StatisticsTest)
{
  BOOST_CHECK_CLOSE(7.695, tl::math::mean(vd.begin(), vd.end()), 0.1);
  BOOST_CHECK_CLOSE(1.142857143, tl::math::mean(vi.begin(), vi.end()), 0.1);
  BOOST_CHECK_CLOSE(14, tl::math::mean(vi2.begin(), vi2.end()), 0.1);
  BOOST_CHECK_CLOSE(7.695, tl::math::mean(tl_vector.begin(), tl_vector.end()), 0.1);
}

BOOST_FIXTURE_TEST_CASE(median, StatisticsTest)
{
  BOOST_CHECK_CLOSE(8, tl::math::median(vd.begin(), vd.end()), 0.1);
  BOOST_CHECK_CLOSE(1, tl::math::median(vi.begin(), vi.end()), 0.1);
  BOOST_CHECK_CLOSE(14, tl::math::median(vi2.begin(), vi2.end()), 0.1);
  BOOST_CHECK_CLOSE(4.5, tl::math::median(l.begin(), l.end()), 0.1);
  BOOST_CHECK_CLOSE(8, tl::math::median(tl_vector.begin(), tl_vector.end()), 0.1);
}

BOOST_FIXTURE_TEST_CASE(mode, StatisticsTest)
{
  BOOST_CHECK_CLOSE(8.5, tl::math::mode(vd.begin(), vd.end()), 0.1);
  BOOST_CHECK_EQUAL(1, tl::math::mode(vi.begin(), vi.end()));
  BOOST_CHECK_EQUAL(7, tl::math::mode(vi2.begin(), vi2.end()));
  BOOST_CHECK_EQUAL(1, tl::math::mode(vi));
  /// TODO: si hay mas de un valor mas repetido???
}

BOOST_FIXTURE_TEST_CASE(range, StatisticsTest)
{
  BOOST_CHECK_EQUAL(3.5, tl::math::range(vd.begin(), vd.end()));
  BOOST_CHECK_EQUAL(3, tl::math::range(vi.begin(), vi.end()));
  BOOST_CHECK_EQUAL(16, tl::math::range(vi2.begin(), vi2.end()));
}

BOOST_FIXTURE_TEST_CASE(interquartileRange, StatisticsTest)
{
  BOOST_CHECK_CLOSE(1.625, tl::math::interquartileRange(vd.begin(), vd.end()), 0.1);
  BOOST_CHECK_CLOSE(1, tl::math::interquartileRange(vi.begin(), vi.end()), 0.1);
  BOOST_CHECK_CLOSE(10, tl::math::interquartileRange(vi2.begin(), vi2.end()), 0.1);
}

BOOST_FIXTURE_TEST_CASE(variance, StatisticsTest)
{
  BOOST_CHECK_CLOSE(1.3402777777778, tl::math::variance(vd.begin(), vd.end()), 0.1);
  BOOST_CHECK_CLOSE(1.1428571428571, tl::math::variance(vi.begin(), vi.end()), 0.1);
  BOOST_CHECK_CLOSE(33.2, tl::math::variance(vi2.begin(), vi2.end()), 0.1);
}

BOOST_FIXTURE_TEST_CASE(populationVariance, StatisticsTest)
{
  BOOST_CHECK_CLOSE(1.1913580246914, tl::math::populationVariance(vd.begin(), vd.end()), 0.1);
  BOOST_CHECK_CLOSE(0.97959183673469, tl::math::populationVariance(vi.begin(), vi.end()), 0.1);
  BOOST_CHECK_CLOSE(27.666666666667, tl::math::populationVariance(vi2.begin(), vi2.end()), 0.1);
}

BOOST_FIXTURE_TEST_CASE(standarDeviation, StatisticsTest)
{
  BOOST_CHECK_CLOSE(1.1577036657875, tl::math::standarDeviation(vd.begin(), vd.end()), 0.1);
  BOOST_CHECK_CLOSE(1.0690449676497, tl::math::standarDeviation(vi.begin(), vi.end()), 0.1);
  BOOST_CHECK_CLOSE(5.76, tl::math::standarDeviation(vi2.begin(), vi2.end()), 0.1);
}

BOOST_FIXTURE_TEST_CASE(coefficientOfVariation, StatisticsTest)
{
  BOOST_CHECK_CLOSE(0.411567, tl::math::coefficientOfVariation(vi2.begin(), vi2.end()), 0.1);
}

BOOST_FIXTURE_TEST_CASE(covariance, StatisticsTest)
{
  BOOST_CHECK_CLOSE(-33.06, tl::math::covariance(x.begin(), x.end(), y.begin(), y.end()), 0.1);
}

//Pearson correlation coefficient

BOOST_FIXTURE_TEST_CASE(pearsonCorrelationCoefficient, StatisticsTest)
{
  BOOST_CHECK_CLOSE(-0.8459, tl::math::pearsonCorrelationCoefficient(x.begin(), x.end(), y.begin(), y.end()), 0.1);
}

BOOST_FIXTURE_TEST_CASE(meanAbsoluteDeviation, StatisticsTest)
{
  BOOST_CHECK_CLOSE(0.89506172839506, tl::math::meanAbsoluteDeviation(vd.begin(), vd.end()), 0.1);
  BOOST_CHECK_CLOSE(0.77551020408163, tl::math::meanAbsoluteDeviation(vi.begin(), vi.end()), 0.1);
  std::vector<int> data{ 2, 2, 3, 4, 14 };
  BOOST_CHECK_EQUAL(3.6, tl::math::meanAbsoluteDeviation(data.begin(), data.end()));
}

BOOST_FIXTURE_TEST_CASE(medianAbsoluteDeviation, StatisticsTest)
{
  std::vector<int> data{ 2, 2, 3, 4, 14 };
  BOOST_CHECK_EQUAL(1, tl::math::medianAbsoluteDeviation(data.begin(), data.end()));
  Vector<int> data2{ 1, 1, 2, 2, 4, 6, 9 };
  BOOST_CHECK_EQUAL(1, tl::math::medianAbsoluteDeviation(data2.begin(), data2.end()));
  Vector<double> data3{ 6.5, 3.8, 6.6, 5.7, 6.0, 6.4, 5.3 };
  BOOST_CHECK_EQUAL(0.5, tl::math::medianAbsoluteDeviation(data3.begin(), data3.end()));
}

BOOST_FIXTURE_TEST_CASE(biweightMidvariance, StatisticsTest)
{
  std::vector<int> data{ 2, 2 };
  BOOST_CHECK_EQUAL(0., tl::math::biweightMidvariance(data.begin(), data.end()));
  Vector<double> data2{ 6.5, 3.8, 6.6, 5.7, 6.0, 6.4, 5.3 };
  BOOST_CHECK_CLOSE(0.677818, tl::math::biweightMidvariance(data2.begin(), data2.end()), 0.1);
  Vector<int> data3{ 1, 2, 3, 4 };
  BOOST_CHECK_CLOSE(1.36477, tl::math::biweightMidvariance(data3.begin(), data3.end()), 0.1);
}

BOOST_FIXTURE_TEST_CASE(first_quartile, StatisticsTest)
{
  BOOST_CHECK_CLOSE(6.875, tl::math::quantile(vd.begin(), vd.end(), 0.25), 0.1);
  BOOST_CHECK_CLOSE(0.5, tl::math::quantile(vi.begin(), vi.end(), 0.25), 0.1);
}

BOOST_FIXTURE_TEST_CASE(third_quartile, StatisticsTest)
{
  BOOST_CHECK_CLOSE(8.5, tl::math::quantile(vd.begin(), vd.end(), 0.75), 0.1);
  BOOST_CHECK_CLOSE(1.5, tl::math::quantile(vi.begin(), vi.end(), 0.75), 0.1);
}

BOOST_FIXTURE_TEST_CASE(quantile, StatisticsTest)
{
  BOOST_CHECK_CLOSE(8, tl::math::quantile(vd.begin(), vd.end(), 0.5), 0.1);
  BOOST_CHECK_CLOSE(4.5, tl::math::quantile(l.begin(), l.end(), 0.5), 0.1);
  BOOST_CHECK_CLOSE(8, tl::math::quantile(tl_vector.begin(), tl_vector.end(), 0.5), 0.1);
}

BOOST_FIXTURE_TEST_CASE(quartileCoefficientOfDispersion, StatisticsTest)
{
  BOOST_CHECK_CLOSE(0.10569, tl::math::quartileCoefficientOfDispersion(vd.begin(), vd.end()), 0.1);
  BOOST_CHECK_CLOSE(0.5, tl::math::quartileCoefficientOfDispersion(vi.begin(), vi.end()), 0.1);
}

BOOST_FIXTURE_TEST_CASE(quartileDeviation, StatisticsTest)
{
  BOOST_CHECK_CLOSE(0.8125, tl::math::quartileDeviation(vd.begin(), vd.end()), 0.1);
  //BOOST_CHECK_CLOSE(1, tl::math::quartileDeviation(vi.begin(), vi.end()), 0.1);
}

BOOST_AUTO_TEST_SUITE_END()