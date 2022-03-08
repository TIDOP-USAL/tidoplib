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
#include <tidop/math/statistic/confmat.h>

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

    DescriptiveStatistics<double>::Config config{};
    config.skewness_method = SkewnessMethod::fisher_pearson;
    config.sample = false;
    stat_1_population = DescriptiveStatistics<double>(s_1, config);
  }

  void teardown()
  {
  }

  DescriptiveStatistics<double> stat_1;
  DescriptiveStatistics<int> stat_2;
  DescriptiveStatistics<int> stat_3;
  DescriptiveStatistics<float> stat_4;

  DescriptiveStatistics<double> stat_1_population;
};

BOOST_FIXTURE_TEST_CASE(min, DescriptiveStatisticsTest)
{
  BOOST_CHECK_EQUAL(5.5, stat_1.min());
  BOOST_CHECK_EQUAL(0, stat_2.min());
  BOOST_CHECK_EQUAL(7, stat_3.min());
  BOOST_CHECK_EQUAL(5.5, stat_1_population.min());
}

BOOST_FIXTURE_TEST_CASE(max, DescriptiveStatisticsTest)
{
  BOOST_CHECK_EQUAL(9.0, stat_1.max(), 0.1);
  BOOST_CHECK_EQUAL(3, stat_2.max());
  BOOST_CHECK_EQUAL(23, stat_3.max(), 0.1);
  BOOST_CHECK_EQUAL(9.0, stat_1_population.max(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(size, DescriptiveStatisticsTest)
{
  BOOST_CHECK_EQUAL(9, stat_1.size());
  BOOST_CHECK_EQUAL(7, stat_2.size());
  BOOST_CHECK_EQUAL(6, stat_3.size());
  BOOST_CHECK_EQUAL(9, stat_1_population.size());
}

BOOST_FIXTURE_TEST_CASE(sum, DescriptiveStatisticsTest)
{
  BOOST_CHECK_EQUAL(69.25, stat_1.sum());
  BOOST_CHECK_EQUAL(8, stat_2.sum());
  BOOST_CHECK_EQUAL(84, stat_3.sum());
  BOOST_CHECK_EQUAL(69.25, stat_1_population.sum());
}

BOOST_FIXTURE_TEST_CASE(mean, DescriptiveStatisticsTest)
{
  BOOST_CHECK_CLOSE(7.695, stat_1.mean(), 0.1);
  BOOST_CHECK_CLOSE(1.142857143, stat_2.mean(), 0.1);
  BOOST_CHECK_CLOSE(14, stat_3.mean(), 0.1);
  BOOST_CHECK_CLOSE(7.695f, stat_4.mean(), 0.1);
  BOOST_CHECK_CLOSE(7.695, stat_1_population.mean(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(median, DescriptiveStatisticsTest)
{
  BOOST_CHECK_CLOSE(8., stat_1.median(), 0.1);
  BOOST_CHECK_CLOSE(1., stat_2.median(), 0.1);
  BOOST_CHECK_CLOSE(14., stat_3.median(), 0.1);
  BOOST_CHECK_CLOSE(8.f, stat_4.median(), 0.1);
  BOOST_CHECK_CLOSE(8., stat_1_population.median(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(mode, DescriptiveStatisticsTest)
{
  BOOST_CHECK_CLOSE(8.5, stat_1.mode(), 0.1);
  BOOST_CHECK_EQUAL(1, stat_2.mode());
  BOOST_CHECK_EQUAL(7, stat_3.mode());
  BOOST_CHECK_CLOSE(8.5f, stat_4.mode(), 0.1);
  BOOST_CHECK_CLOSE(8.5, stat_1_population.mode(), 0.1);
  /// TODO: si hay mas de un valor mas repetido???
}

BOOST_FIXTURE_TEST_CASE(range, DescriptiveStatisticsTest)
{
  BOOST_CHECK_EQUAL(3.5, stat_1.range());
  BOOST_CHECK_EQUAL(3, stat_2.range());
  BOOST_CHECK_EQUAL(16, stat_3.range());
  BOOST_CHECK_EQUAL(3.5f, stat_4.range());
  BOOST_CHECK_EQUAL(3.5, stat_1_population.range());
}

BOOST_FIXTURE_TEST_CASE(variance, DescriptiveStatisticsTest)
{
  BOOST_CHECK_CLOSE(1.3402777777778, stat_1.variance(), 0.1);
  BOOST_CHECK_CLOSE(1.1428571428571, stat_2.variance(), 0.1);
  BOOST_CHECK_CLOSE(33.2, stat_3.variance(), 0.1);
  BOOST_CHECK_CLOSE(1.19135802, stat_1_population.variance(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(standarDeviation, DescriptiveStatisticsTest)
{
  BOOST_CHECK_CLOSE(1.1577036657875, stat_1.standarDeviation(), 0.1);
  BOOST_CHECK_CLOSE(1.0690449676497, stat_2.standarDeviation(), 0.1);
  BOOST_CHECK_CLOSE(5.7619441163552, stat_3.standarDeviation(), 0.1);
  BOOST_CHECK_CLOSE(1.09149348, stat_1_population.standarDeviation(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(coefficientOfVariation, DescriptiveStatisticsTest)
{
  BOOST_CHECK_CLOSE(0.150459682, stat_1.coefficientOfVariation(), 0.1);
  BOOST_CHECK_CLOSE(0.935414347, stat_2.coefficientOfVariation(), 0.1);
  BOOST_CHECK_CLOSE(0.411567437, stat_3.coefficientOfVariation(), 0.1);
  BOOST_CHECK_CLOSE(0.141854749, stat_1_population.coefficientOfVariation(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(meanAbsoluteDeviation, DescriptiveStatisticsTest)
{
  BOOST_CHECK_CLOSE(0.89506172839506, stat_1.meanAbsoluteDeviation(), 0.1);
  BOOST_CHECK_CLOSE(0.775510204, stat_2.meanAbsoluteDeviation(), 0.1);
  BOOST_CHECK_CLOSE(4.3333333333333, stat_3.meanAbsoluteDeviation(), 0.1);
  BOOST_CHECK_CLOSE(0.89506172839506, stat_1_population.meanAbsoluteDeviation(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(medianAbsoluteDeviation, DescriptiveStatisticsTest)
{
  BOOST_CHECK_CLOSE(0.5, stat_1.medianAbsoluteDeviation(), 0.1);
  BOOST_CHECK_CLOSE(1., stat_2.medianAbsoluteDeviation(), 0.1);
  BOOST_CHECK_CLOSE(4., stat_3.medianAbsoluteDeviation(), 0.1);
  BOOST_CHECK_CLOSE(0.5, stat_1_population.medianAbsoluteDeviation(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(sumOfSquares, DescriptiveStatisticsTest)
{
  BOOST_CHECK_CLOSE(10.7222222, stat_1.sumOfSquares(), 0.1);
  BOOST_CHECK_CLOSE(6.85714286, stat_2.sumOfSquares(), 0.1);
  BOOST_CHECK_CLOSE(166, stat_3.sumOfSquares(), 0.1);
  BOOST_CHECK_CLOSE(10.7222222, stat_1_population.sumOfSquares(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(rootMeanSquare, DescriptiveStatisticsTest)
{
  BOOST_CHECK_CLOSE(7.77147562, stat_1.rootMeanSquare(), 0.1);
  BOOST_CHECK_CLOSE(1.51185789, stat_2.rootMeanSquare(), 0.1);
  BOOST_CHECK_CLOSE(14.9554895, stat_3.rootMeanSquare(), 0.1);
  BOOST_CHECK_CLOSE(7.77147562, stat_1_population.rootMeanSquare(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(skewness, DescriptiveStatisticsTest)
{
  BOOST_CHECK_CLOSE(-1.01235079, stat_1.skewness(), 0.1);
  BOOST_CHECK_CLOSE(0.771716836, stat_2.skewness(), 0.1);
  BOOST_CHECK_CLOSE(0.451654786, stat_3.skewness(), 0.1);

  BOOST_CHECK_CLOSE(-0.835146793, stat_1_population.skewness(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(kurtosis, DescriptiveStatisticsTest)
{
  BOOST_CHECK_CLOSE(4.73030915, stat_1.kurtosis(), 0.1);
  BOOST_CHECK_CLOSE(5.6625, stat_2.kurtosis(), 0.1);
  BOOST_CHECK_CLOSE(6.14113079, stat_3.kurtosis(), 0.1);

  BOOST_CHECK_CLOSE(2.48341231, stat_1_population.kurtosis(), 0.1);
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




BOOST_AUTO_TEST_SUITE(ConfusionMatrixTestSuite)

struct ConfusionMatrixTest
{
  ConfusionMatrixTest()
  {
  }

  ~ConfusionMatrixTest()
  {
    delete mConfusionMatrix;
  }

  void setup()
  {
    data.push_back(std::make_pair(-0.037, 0));
    data.push_back(std::make_pair(0.288, 0));
    data.push_back(std::make_pair(-1.649, 0));
    data.push_back(std::make_pair(-0.074, 0));
    data.push_back(std::make_pair(0.833, 0));
    data.push_back(std::make_pair(-2.019, 0));
    data.push_back(std::make_pair(0.976, 0));
    data.push_back(std::make_pair(0.561, 0));
    data.push_back(std::make_pair(0.494, 0));
    data.push_back(std::make_pair(-1.699, 0));
    data.push_back(std::make_pair(0.981, 0));
    data.push_back(std::make_pair(0.808, 0));
    data.push_back(std::make_pair(0.044, 0));
    data.push_back(std::make_pair(-1.107, 0));
    data.push_back(std::make_pair(2.162, 0));
    data.push_back(std::make_pair(0.090, 0));
    data.push_back(std::make_pair(0.304, 0));
    data.push_back(std::make_pair(0.153, 0));
    data.push_back(std::make_pair(-0.234, 0));
    data.push_back(std::make_pair(-0.681, 0));
    data.push_back(std::make_pair(-0.332, 0));
    data.push_back(std::make_pair(-1.554, 0));
    data.push_back(std::make_pair(1.644, 0));
    data.push_back(std::make_pair(0.482, 0));
    data.push_back(std::make_pair(0.501, 0));
    data.push_back(std::make_pair(0.457, 0));
    data.push_back(std::make_pair(-0.155, 0));
    data.push_back(std::make_pair(-1.101, 0));
    data.push_back(std::make_pair(1.116, 0));
    data.push_back(std::make_pair(0.002, 0));
    data.push_back(std::make_pair(0.658, 0));
    data.push_back(std::make_pair(-1.167, 0));
    data.push_back(std::make_pair(1.277, 0));
    data.push_back(std::make_pair(1.059, 0));
    data.push_back(std::make_pair(-1.658, 0));
    data.push_back(std::make_pair(2.614, 0));
    data.push_back(std::make_pair(0.695, 0));
    data.push_back(std::make_pair(-0.272, 0));
    data.push_back(std::make_pair(-0.124, 0));
    data.push_back(std::make_pair(-0.886, 0));
    data.push_back(std::make_pair(1.769, 0));
    data.push_back(std::make_pair(0.131, 0));
    data.push_back(std::make_pair(-2.201, 0));
    data.push_back(std::make_pair(-0.740, 0));
    data.push_back(std::make_pair(1.513, 0));
    data.push_back(std::make_pair(-1.201, 0));
    data.push_back(std::make_pair(-1.687, 0));
    data.push_back(std::make_pair(-0.442, 0));
    data.push_back(std::make_pair(0.522, 0));
    data.push_back(std::make_pair(0.694, 0));
    data.push_back(std::make_pair(-0.696, 0));
    data.push_back(std::make_pair(0.796, 0));
    data.push_back(std::make_pair(-1.267, 0));
    data.push_back(std::make_pair(1.973, 0));
    data.push_back(std::make_pair(1.750, 0));
    data.push_back(std::make_pair(2.056, 0));
    data.push_back(std::make_pair(1.537, 0));
    data.push_back(std::make_pair(-1.399, 0));
    data.push_back(std::make_pair(1.302, 0));
    data.push_back(std::make_pair(0.055, 0));
    data.push_back(std::make_pair(0.656, 0));
    data.push_back(std::make_pair(-0.211, 0));
    data.push_back(std::make_pair(0.335, 0));
    data.push_back(std::make_pair(1.078, 0));
    data.push_back(std::make_pair(-0.234, 0));
    data.push_back(std::make_pair(-0.460, 0));
    data.push_back(std::make_pair(-0.680, 0));
    data.push_back(std::make_pair(-0.006, 0));
    data.push_back(std::make_pair(0.835, 1));
    data.push_back(std::make_pair(-0.334, 1));
    data.push_back(std::make_pair(1.172, 1));
    data.push_back(std::make_pair(2.962, 1));
    data.push_back(std::make_pair(1.001, 1));
    data.push_back(std::make_pair(1.402, 1));
    data.push_back(std::make_pair(0.829, 1));
    data.push_back(std::make_pair(1.593, 1));
    data.push_back(std::make_pair(2.861, 1));
    data.push_back(std::make_pair(-0.653, 1));
    data.push_back(std::make_pair(2.118, 1));
    data.push_back(std::make_pair(0.361, 1));
    data.push_back(std::make_pair(0.971, 1));
    data.push_back(std::make_pair(1.812, 1));
    data.push_back(std::make_pair(1.293, 1));
    data.push_back(std::make_pair(1.534, 1));
    data.push_back(std::make_pair(3.000, 1));
    data.push_back(std::make_pair(0.365, 1));
    data.push_back(std::make_pair(0.633, 1));
    data.push_back(std::make_pair(2.002, 1));
    data.push_back(std::make_pair(0.054, 1));
    data.push_back(std::make_pair(4.353, 1));
    data.push_back(std::make_pair(1.568, 1));
    data.push_back(std::make_pair(1.163, 1));
    data.push_back(std::make_pair(-0.777, 1));
    data.push_back(std::make_pair(0.547, 1));
    data.push_back(std::make_pair(-0.095, 1));
    data.push_back(std::make_pair(0.830, 1));
    data.push_back(std::make_pair(0.905, 1));
    data.push_back(std::make_pair(1.297, 1));
    data.push_back(std::make_pair(1.780, 1));
    data.push_back(std::make_pair(2.602, 1));
    data.push_back(std::make_pair(2.699, 1));
    data.push_back(std::make_pair(1.807, 1));
    data.push_back(std::make_pair(2.367, 1));
    data.push_back(std::make_pair(0.052, 1));
    data.push_back(std::make_pair(4.195, 1));
    data.push_back(std::make_pair(-0.659, 1));
    data.push_back(std::make_pair(3.030, 1));
    data.push_back(std::make_pair(0.850, 1));
    data.push_back(std::make_pair(2.062, 1));
    data.push_back(std::make_pair(1.366, 1));
    data.push_back(std::make_pair(4.850, 1));
    data.push_back(std::make_pair(2.779, 1));
    data.push_back(std::make_pair(2.282, 1));
    data.push_back(std::make_pair(0.586, 1));
    data.push_back(std::make_pair(0.976, 1));
    data.push_back(std::make_pair(0.501, 1));
    data.push_back(std::make_pair(0.620, 1));
    data.push_back(std::make_pair(1.287, 1));
    data.push_back(std::make_pair(1.457, 1));
    data.push_back(std::make_pair(1.374, 1));
    data.push_back(std::make_pair(1.015, 1));
    data.push_back(std::make_pair(1.116, 1));
    data.push_back(std::make_pair(0.321, 1));
    data.push_back(std::make_pair(1.784, 1));
    data.push_back(std::make_pair(4.680, 1));
    data.push_back(std::make_pair(0.747, 1));
    data.push_back(std::make_pair(0.948, 1));
    data.push_back(std::make_pair(1.508, 1));
    data.push_back(std::make_pair(0.378, 1));
    data.push_back(std::make_pair(1.250, 1));
    data.push_back(std::make_pair(0.225, 1));
    data.push_back(std::make_pair(1.373, 1));
    data.push_back(std::make_pair(-0.869, 1));
    data.push_back(std::make_pair(0.817, 1));
    data.push_back(std::make_pair(1.541, 1));
    data.push_back(std::make_pair(1.123, 1));
    data.push_back(std::make_pair(0.907, 1));
    data.push_back(std::make_pair(0.210, 1));
    data.push_back(std::make_pair(1.472, 1));
    data.push_back(std::make_pair(-0.099, 1));
    data.push_back(std::make_pair(2.951, 1));
    data.push_back(std::make_pair(1.254, 1));
    data.push_back(std::make_pair(0.789, 1));
    data.push_back(std::make_pair(0.882, 1));
    data.push_back(std::make_pair(0.554, 1));
    data.push_back(std::make_pair(0.560, 1));
    data.push_back(std::make_pair(1.273, 1));
    data.push_back(std::make_pair(-0.207, 1));

    mConfusionMatrix = new ConfusionMatrix<double>(data);

  }

  void teardown()
  {
  }

  std::vector<std::pair<double, int>> data;
  ConfusionMatrix<double> *mConfusionMatrix;
};


BOOST_FIXTURE_TEST_CASE(positives, ConfusionMatrixTest)
{
  BOOST_CHECK_EQUAL(80, mConfusionMatrix->positives());
}

BOOST_FIXTURE_TEST_CASE(negatives, ConfusionMatrixTest)
{
  BOOST_CHECK_EQUAL(68, mConfusionMatrix->negatives());
}

BOOST_FIXTURE_TEST_CASE(truePositives, ConfusionMatrixTest)
{
  double threshold = this->data.back().first;
  BOOST_CHECK_EQUAL(75, mConfusionMatrix->truePositives(threshold));
}

BOOST_FIXTURE_TEST_CASE(falsePositives, ConfusionMatrixTest)
{
  double threshold = this->data.back().first;
  BOOST_CHECK_EQUAL(43, mConfusionMatrix->falsePositives(threshold));
}

BOOST_FIXTURE_TEST_CASE(trueNegatives, ConfusionMatrixTest)
{
  double threshold = this->data.back().first;
  BOOST_CHECK_EQUAL(25, mConfusionMatrix->trueNegatives(threshold));
}

BOOST_FIXTURE_TEST_CASE(falseNegatives, ConfusionMatrixTest)
{
  double threshold = this->data.back().first;
  BOOST_CHECK_EQUAL(5, mConfusionMatrix->falseNegatives(threshold));
}

BOOST_FIXTURE_TEST_CASE(accuracy, ConfusionMatrixTest)
{
  double threshold = this->data.back().first;
  BOOST_CHECK_CLOSE(0.6756756, mConfusionMatrix->accuracy(threshold), 0.1);
  BOOST_CHECK_CLOSE(0.9064, ConfusionMatrix<double>::accuracy(20, 1820, 30, 2000), 0.1);
}

BOOST_FIXTURE_TEST_CASE(positivePredictiveValue, ConfusionMatrixTest)
{
  double threshold = this->data.back().first;
  BOOST_CHECK_CLOSE(0.63559, mConfusionMatrix->positivePredictiveValue(threshold), 0.1);
  BOOST_CHECK_CLOSE(0.1, ConfusionMatrix<double>::positivePredictiveValue(20, 180), 0.1);
}

BOOST_FIXTURE_TEST_CASE(negativePredictiveValue, ConfusionMatrixTest)
{
  double threshold = this->data.back().first;
  BOOST_CHECK_CLOSE(0.8333, mConfusionMatrix->negativePredictiveValue(threshold), 0.1);
  BOOST_CHECK_CLOSE(0.9945, ConfusionMatrix<double>::negativePredictiveValue(10, 1820), 0.1);
}

BOOST_FIXTURE_TEST_CASE(truePositiveRate, ConfusionMatrixTest)
{
  double threshold = this->data.back().first;
  BOOST_CHECK_EQUAL(0.9375, mConfusionMatrix->truePositiveRate(threshold));
  BOOST_CHECK_CLOSE(0.66666, ConfusionMatrix<double>::truePositiveRate(20, 10), 0.1);
}

BOOST_FIXTURE_TEST_CASE(falsePositiveRate, ConfusionMatrixTest)
{
  double threshold = this->data.back().first;
  BOOST_CHECK_CLOSE(0.6323529, mConfusionMatrix->falsePositiveRate(threshold), 0.1);
  BOOST_CHECK_CLOSE(0.09, ConfusionMatrix<double>::falsePositiveRate(180, 1820), 0.1);
}

BOOST_FIXTURE_TEST_CASE(trueNegativeRate, ConfusionMatrixTest)
{
  double threshold = this->data.back().first;
  BOOST_CHECK_CLOSE(0.367647, mConfusionMatrix->trueNegativeRate(threshold), 0.1);
  BOOST_CHECK_CLOSE(0.91, ConfusionMatrix<double>::trueNegativeRate(1820, 180), 0.1);
}

BOOST_FIXTURE_TEST_CASE(falseNegativeRate, ConfusionMatrixTest)
{
  double threshold = this->data.back().first;
  BOOST_CHECK_EQUAL(0.0625, mConfusionMatrix->falseNegativeRate(threshold));
  BOOST_CHECK_CLOSE(0.3333, ConfusionMatrix<double>::falseNegativeRate(10, 20), 0.1);
}

BOOST_AUTO_TEST_SUITE_END()