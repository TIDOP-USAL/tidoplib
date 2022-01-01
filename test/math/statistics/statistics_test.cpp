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
  BOOST_CHECK_CLOSE(2, tl::math::interquartileRange(vi.begin(), vi.end()), 0.1);
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
  BOOST_CHECK_CLOSE(5.7619441163552, tl::math::standarDeviation(vi2.begin(), vi2.end()), 0.1);
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
  BOOST_CHECK_CLOSE(4.3333333333333, tl::math::meanAbsoluteDeviation(vi2.begin(), vi2.end()), 0.1);
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
  BOOST_CHECK_CLOSE(0, tl::math::quantile(vi.begin(), vi.end(), 0.25), 0.1);
  BOOST_CHECK_CLOSE(8.5, tl::math::quantile(vi2.begin(), vi2.end(), 0.25), 0.1);
}

BOOST_FIXTURE_TEST_CASE(third_quartile, StatisticsTest)
{
  BOOST_CHECK_CLOSE(8.5, tl::math::quantile(vd.begin(), vd.end(), 0.75), 0.1);
  BOOST_CHECK_CLOSE(2, tl::math::quantile(vi.begin(), vi.end(), 0.75), 0.1);
  BOOST_CHECK_CLOSE(18.5, tl::math::quantile(vi2.begin(), vi2.end(), 0.75), 0.1);
}

BOOST_FIXTURE_TEST_CASE(quantile, StatisticsTest)
{
  BOOST_CHECK_CLOSE(8, tl::math::quantile(vd.begin(), vd.end(), 0.5), 0.1);
  BOOST_CHECK_CLOSE(4.5, tl::math::quantile(l.begin(), l.end(), 0.5), 0.1);
  BOOST_CHECK_CLOSE(14, tl::math::quantile(vi2.begin(), vi2.end(), 0.5), 0.1);
}

BOOST_FIXTURE_TEST_CASE(quartileCoefficientOfDispersion, StatisticsTest)
{
  BOOST_CHECK_CLOSE(0.10569, tl::math::quartileCoefficientOfDispersion(vd.begin(), vd.end()), 0.1);
  BOOST_CHECK_CLOSE(1, tl::math::quartileCoefficientOfDispersion(vi.begin(), vi.end()), 0.1);
  BOOST_CHECK_CLOSE(0.37037037037037, tl::math::quartileCoefficientOfDispersion(vi2.begin(), vi2.end()), 0.1);
}

BOOST_FIXTURE_TEST_CASE(quartileDeviation, StatisticsTest)
{
  BOOST_CHECK_CLOSE(0.8125, tl::math::quartileDeviation(vd.begin(), vd.end()), 0.1);
  BOOST_CHECK_CLOSE(1, tl::math::quartileDeviation(vi.begin(), vi.end()), 0.1);
  BOOST_CHECK_CLOSE(5, tl::math::quartileDeviation(vi2.begin(), vi2.end()), 0.1);
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(ConfusionMatrixTestSuite)

struct ConfusionMatrixTest
{
  ConfusionMatrixTest()
  { }

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

BOOST_FIXTURE_TEST_CASE(auc, ConfusionMatrixTest)
{
  //BOOST_CHECK_EQUAL(0.970221, mConfusionMatrix->auc());
}

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
  BOOST_CHECK_EQUAL(6, mConfusionMatrix->truePositives(threshold));
}

BOOST_FIXTURE_TEST_CASE(falsePositives, ConfusionMatrixTest)
{
  double threshold = this->data.back().first;
  BOOST_CHECK_EQUAL(25, mConfusionMatrix->falsePositives(threshold));
}

BOOST_FIXTURE_TEST_CASE(trueNegatives, ConfusionMatrixTest)
{
  double threshold = this->data.back().first;
  BOOST_CHECK_EQUAL(43, mConfusionMatrix->trueNegatives(threshold));
}

BOOST_FIXTURE_TEST_CASE(falseNegatives, ConfusionMatrixTest)
{
  double threshold = this->data.back().first;
  BOOST_CHECK_EQUAL(74, mConfusionMatrix->falseNegatives(threshold));
}

BOOST_FIXTURE_TEST_CASE(accuracy, ConfusionMatrixTest)
{
  double threshold = this->data.back().first;
  BOOST_CHECK_CLOSE(0.3311, mConfusionMatrix->accuracy(threshold), 0.1);
}

BOOST_FIXTURE_TEST_CASE(positivePredictiveValue, ConfusionMatrixTest)
{
  double threshold = this->data.back().first;
  BOOST_CHECK_CLOSE(0.1935, mConfusionMatrix->positivePredictiveValue(threshold), 0.1);
}

BOOST_FIXTURE_TEST_CASE(negativePredictiveValue, ConfusionMatrixTest)
{
  double threshold = this->data.back().first;
  BOOST_CHECK_CLOSE(0.3675, mConfusionMatrix->negativePredictiveValue(threshold), 0.1);
}

BOOST_FIXTURE_TEST_CASE(truePositiveRate, ConfusionMatrixTest)
{
  double threshold = this->data.back().first;
  BOOST_CHECK_EQUAL(0.0750, mConfusionMatrix->truePositiveRate(threshold));
}

BOOST_FIXTURE_TEST_CASE(falsePositiveRate, ConfusionMatrixTest)
{
  double threshold = this->data.back().first;
  BOOST_CHECK_CLOSE(0.3676, mConfusionMatrix->falsePositiveRate(threshold), 0.1);
}

BOOST_FIXTURE_TEST_CASE(trueNegativeRate, ConfusionMatrixTest)
{
  double threshold = this->data.back().first;
  BOOST_CHECK_CLOSE(0.6324, mConfusionMatrix->trueNegativeRate(threshold), 0.1);
}

BOOST_FIXTURE_TEST_CASE(falseNegativeRate, ConfusionMatrixTest)
{
  double threshold = this->data.back().first;
  BOOST_CHECK_EQUAL(0.9250, mConfusionMatrix->falseNegativeRate(threshold));
}

BOOST_AUTO_TEST_SUITE_END()