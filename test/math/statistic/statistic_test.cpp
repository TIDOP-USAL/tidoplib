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

#define BOOST_TEST_MODULE Tidop statistic test
#include <boost/test/unit_test.hpp>

#include <tidop/math/statistic/descriptive.h>
#include <tidop/math/statistic/series.h>

using namespace tl::math;

BOOST_AUTO_TEST_SUITE(SeriesTestSuite)

struct SeriesTest
{

  SeriesTest() {}
  ~SeriesTest() {}

  void setup()
  {
    s = Series<int>({1, 2, 2, 3, 3, 3, 4, 4, 4, 4});
    s2 = Series<double>({{"A", 2.}, {"B", 5.}});
    s3 = Series<double>({{3, 2.}, {7, 3.}});


  }

  void teardown()
  {

  }

  Series<int> s;
  Series<double> s2;
  Series<double> s3;
};

BOOST_FIXTURE_TEST_CASE(constructor, SeriesTest)
{

  BOOST_CHECK_EQUAL(1, s[0]);
  BOOST_CHECK_EQUAL(2, s[1]);
  BOOST_CHECK_EQUAL(2, s[2]);
  BOOST_CHECK_EQUAL(3, s[3]);
  BOOST_CHECK_EQUAL(3, s[4]);
  BOOST_CHECK_EQUAL(3, s[5]);
  BOOST_CHECK_EQUAL(4, s[6]);
  BOOST_CHECK_EQUAL(4, s[7]);
  BOOST_CHECK_EQUAL(4, s[8]);
  BOOST_CHECK_EQUAL(4, s[9]);

  BOOST_CHECK_EQUAL(2., s2[0]);
  BOOST_CHECK_EQUAL(5., s2[1]);
  BOOST_CHECK_EQUAL(2., s2["A"]);
  BOOST_CHECK_EQUAL(5., s2["B"]);

  BOOST_CHECK_EQUAL(2., s3[3]);
  BOOST_CHECK_EQUAL(3., s3[7]);
  BOOST_CHECK_EQUAL(2., s3["3"]);
  BOOST_CHECK_EQUAL(3., s3["7"]);
}

BOOST_FIXTURE_TEST_CASE(size, SeriesTest)
{
  BOOST_CHECK_EQUAL(10, s.size());
  BOOST_CHECK_EQUAL(2, s2.size());
  BOOST_CHECK_EQUAL(2, s3.size());
}


BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(DescriptiveStatisticsTestSuite)

struct DescriptiveStatisticsTest
{
  DescriptiveStatisticsTest()
  { }

  ~DescriptiveStatisticsTest()
  {
  }

  void setup()
  {
    Series<double> s_1({8.0, 8.5, 7.5, 9.0, 6.25, 5.5, 8.5, 7.5, 8.5});
    stat_1 = DescriptiveStatistics<double>(s_1);

    Series<int> s_2({1, 0, 1, 3, 2, 0, 1});
    stat_2 = DescriptiveStatistics<int>(s_2);

    Series<int> s_3({17, 15, 23, 7, 9, 13});
    stat_3 = DescriptiveStatistics<int>(s_3);

    Series<float> s_4({8.f, 8.5f, 7.5f, 9.f, 6.25f, 5.5f, 8.5f, 7.5f, 8.5f});
    stat_4 = DescriptiveStatistics<float>(s_4);
  }

  void teardown()
  {
  }

  DescriptiveStatistics<double> stat_1;
  DescriptiveStatistics<int> stat_2;
  DescriptiveStatistics<int> stat_3;
  DescriptiveStatistics<float> stat_4;
};

BOOST_FIXTURE_TEST_CASE(min, DescriptiveStatisticsTest)
{
  BOOST_CHECK_EQUAL(5.5, stat_1.min());
  BOOST_CHECK_EQUAL(0, stat_2.min());
  BOOST_CHECK_EQUAL(7, stat_3.min());
}

BOOST_FIXTURE_TEST_CASE(max, DescriptiveStatisticsTest)
{
  BOOST_CHECK_EQUAL(9.0, stat_1.max(), 0.1);
  BOOST_CHECK_EQUAL(3, stat_2.max());
  BOOST_CHECK_EQUAL(23, stat_3.max(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(size, DescriptiveStatisticsTest)
{
  BOOST_CHECK_EQUAL(9, stat_1.size());
  BOOST_CHECK_EQUAL(7, stat_2.size());
  BOOST_CHECK_EQUAL(6, stat_3.size());
}

BOOST_FIXTURE_TEST_CASE(sum, DescriptiveStatisticsTest)
{
  BOOST_CHECK_EQUAL(69.25, stat_1.sum());
  BOOST_CHECK_EQUAL(8, stat_2.sum());
  BOOST_CHECK_EQUAL(84, stat_3.sum());
}

BOOST_FIXTURE_TEST_CASE(mean, DescriptiveStatisticsTest)
{
  BOOST_CHECK_CLOSE(7.695, stat_1.mean(), 0.1);
  BOOST_CHECK_CLOSE(1.142857143, stat_2.mean(), 0.1);
  BOOST_CHECK_CLOSE(14, stat_3.mean(), 0.1);
  BOOST_CHECK_CLOSE(7.695f, stat_4.mean(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(median, DescriptiveStatisticsTest)
{
  BOOST_CHECK_CLOSE(8., stat_1.median(), 0.1);
  BOOST_CHECK_CLOSE(1., stat_2.median(), 0.1);
  BOOST_CHECK_CLOSE(14., stat_3.median(), 0.1);
  BOOST_CHECK_CLOSE(8.f, stat_4.median(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(mode, DescriptiveStatisticsTest)
{
  BOOST_CHECK_CLOSE(8.5, stat_1.mode(), 0.1);
  BOOST_CHECK_EQUAL(1, stat_2.mode());
  BOOST_CHECK_EQUAL(7, stat_3.mode());
  BOOST_CHECK_CLOSE(8.5f, stat_4.mode(), 0.1);
  /// TODO: si hay mas de un valor mas repetido???
}

BOOST_FIXTURE_TEST_CASE(range, DescriptiveStatisticsTest)
{
  BOOST_CHECK_EQUAL(3.5, stat_1.range());
  BOOST_CHECK_EQUAL(3, stat_2.range());
  BOOST_CHECK_EQUAL(16, stat_3.range());
  BOOST_CHECK_EQUAL(3.5f, stat_4.range());
}

BOOST_FIXTURE_TEST_CASE(variance, DescriptiveStatisticsTest)
{
  BOOST_CHECK_CLOSE(1.3402777777778, stat_1.variance(), 0.1);
  BOOST_CHECK_CLOSE(1.1428571428571, stat_2.variance(), 0.1);
  BOOST_CHECK_CLOSE(33.2, stat_3.variance(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(populationVariance, DescriptiveStatisticsTest)
{
  //BOOST_CHECK_CLOSE(1.1913580246914, stat_1.populationVariance(), 0.1);
  //BOOST_CHECK_CLOSE(0.97959183673469, stat_2.populationVariance(), 0.1);
  //BOOST_CHECK_CLOSE(27.666666666667, stat_3.populationVariance(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(standarDeviation, DescriptiveStatisticsTest)
{
  BOOST_CHECK_CLOSE(1.1577036657875, stat_1.standarDeviation(), 0.1);
  BOOST_CHECK_CLOSE(1.0690449676497, stat_2.standarDeviation(), 0.1);
  BOOST_CHECK_CLOSE(5.7619441163552, stat_3.standarDeviation(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(coefficientOfVariation, DescriptiveStatisticsTest)
{
  BOOST_CHECK_CLOSE(0.150459682, stat_1.coefficientOfVariation(), 0.1);
  BOOST_CHECK_CLOSE(0.935414347, stat_2.coefficientOfVariation(), 0.1);
  BOOST_CHECK_CLOSE(0.411567437, stat_3.coefficientOfVariation(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(meanAbsoluteDeviation, DescriptiveStatisticsTest)
{
  BOOST_CHECK_CLOSE(0.89506172839506, stat_1.meanAbsoluteDeviation(), 0.1);
  BOOST_CHECK_CLOSE(0.775510204, stat_2.meanAbsoluteDeviation(), 0.1);
  BOOST_CHECK_CLOSE(4.3333333333333, stat_3.meanAbsoluteDeviation(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(medianAbsoluteDeviation, DescriptiveStatisticsTest)
{
  BOOST_CHECK_CLOSE(0.5, stat_1.medianAbsoluteDeviation(), 0.1);
  BOOST_CHECK_CLOSE(1., stat_2.medianAbsoluteDeviation(), 0.1);
  BOOST_CHECK_CLOSE(4., stat_3.medianAbsoluteDeviation(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(sumOfSquares, DescriptiveStatisticsTest)
{
  BOOST_CHECK_CLOSE(10.7222222, stat_1.sumOfSquares(), 0.1);
  BOOST_CHECK_CLOSE(6.85714286, stat_2.sumOfSquares(), 0.1);
  BOOST_CHECK_CLOSE(166, stat_3.sumOfSquares(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(rootMeanSquare, DescriptiveStatisticsTest)
{
  BOOST_CHECK_CLOSE(7.77147562, stat_1.rootMeanSquare(), 0.1);
  BOOST_CHECK_CLOSE(1.51185789, stat_2.rootMeanSquare(), 0.1);
  BOOST_CHECK_CLOSE(14.9554895, stat_3.rootMeanSquare(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(skewness, DescriptiveStatisticsTest)
{
  BOOST_CHECK_CLOSE(-1.01235079, stat_1.skewness(), 0.1);
  BOOST_CHECK_CLOSE(0.771716836, stat_2.skewness(), 0.1);
  BOOST_CHECK_CLOSE(0.451654786, stat_3.skewness(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(biweightMidvariance, DescriptiveStatisticsTest)
{
  //std::vector<int> data{2, 2};
  //BOOST_CHECK_EQUAL(0., tl::math::biweightMidvariance(data.begin(), data.end()));
  //Vector<double> data2{6.5, 3.8, 6.6, 5.7, 6.0, 6.4, 5.3};
  //BOOST_CHECK_CLOSE(0.677818, tl::math::biweightMidvariance(data2.begin(), data2.end()), 0.1);
  //Vector<int> data3{1, 2, 3, 4};
  //BOOST_CHECK_CLOSE(1.36477, tl::math::biweightMidvariance(data3.begin(), data3.end()), 0.1);
}

BOOST_FIXTURE_TEST_CASE(firstQuartile, DescriptiveStatisticsTest)
{
  BOOST_CHECK_CLOSE(6.875, stat_1.firstQuartile(), 0.1);
  BOOST_CHECK_CLOSE(0, stat_2.firstQuartile(), 0.1);
  BOOST_CHECK_CLOSE(8.5, stat_3.firstQuartile(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(secondQuartile, DescriptiveStatisticsTest)
{
  BOOST_CHECK_CLOSE(8.0, stat_1.secondQuartile(), 0.1);
  BOOST_CHECK_CLOSE(1, stat_2.secondQuartile(), 0.1);
  BOOST_CHECK_CLOSE(14, stat_3.secondQuartile(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(thirdQuartile, DescriptiveStatisticsTest)
{
  BOOST_CHECK_CLOSE(8.5, stat_1.thirdQuartile(), 0.1);
  BOOST_CHECK_CLOSE(2, stat_2.thirdQuartile(), 0.1);
  BOOST_CHECK_CLOSE(18.5, stat_3.thirdQuartile(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(interquartileRange, DescriptiveStatisticsTest)
{
  BOOST_CHECK_CLOSE(1.625, stat_1.interquartileRange(), 0.1);
  BOOST_CHECK_CLOSE(2, stat_2.interquartileRange(), 0.1);
  BOOST_CHECK_CLOSE(10, stat_3.interquartileRange(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(quartileCoefficientOfDispersion, DescriptiveStatisticsTest)
{
  //BOOST_CHECK_CLOSE(0.10569, tl::math::quartileCoefficientOfDispersion(vd.begin(), vd.end()), 0.1);
  //BOOST_CHECK_CLOSE(1, tl::math::quartileCoefficientOfDispersion(vi.begin(), vi.end()), 0.1);
  //BOOST_CHECK_CLOSE(0.37037037037037, tl::math::quartileCoefficientOfDispersion(vi2.begin(), vi2.end()), 0.1);
}

BOOST_FIXTURE_TEST_CASE(quartileDeviation, DescriptiveStatisticsTest)
{
  //BOOST_CHECK_CLOSE(0.8125, tl::math::quartileDeviation(vd.begin(), vd.end()), 0.1);
  //BOOST_CHECK_CLOSE(1, tl::math::quartileDeviation(vi.begin(), vi.end()), 0.1);
  //BOOST_CHECK_CLOSE(5, tl::math::quartileDeviation(vi2.begin(), vi2.end()), 0.1);
}

BOOST_AUTO_TEST_SUITE_END()