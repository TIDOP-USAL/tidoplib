#define BOOST_TEST_MODULE Tidop statistic test
#include <boost/test/unit_test.hpp>

#include <tidop/math/statistics.h>

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
};

BOOST_FIXTURE_TEST_CASE(mean, StatisticsTest)
{
  BOOST_CHECK_CLOSE(7.695, tl::math::mean(vd.begin(), vd.end()), 0.1);
  BOOST_CHECK_CLOSE(7.695, tl::math::mean(vd), 0.1);

  BOOST_CHECK_CLOSE(1.142857143, tl::math::mean(vi.begin(), vi.end()), 0.1);
  BOOST_CHECK_CLOSE(1.142857143, tl::math::mean(vi), 0.1);
}

BOOST_FIXTURE_TEST_CASE(median, StatisticsTest)
{
  BOOST_CHECK_CLOSE(8, tl::math::median(vd.begin(), vd.end()), 0.1);
  BOOST_CHECK_CLOSE(8, tl::math::median(vd), 0.01);
  BOOST_CHECK_CLOSE(4.5, tl::math::median(l.begin(), l.end()), 0.1);
  BOOST_CHECK_CLOSE(4.5, tl::math::median(l), 0.01);
}

BOOST_FIXTURE_TEST_CASE(mode, StatisticsTest)
{
  BOOST_CHECK_EQUAL(1, tl::math::mode(vi.begin(), vi.end()));
  BOOST_CHECK_EQUAL(1, tl::math::mode(vi));
  /// TODO: si hay mas de un valor mas repetido???
}

BOOST_FIXTURE_TEST_CASE(range, StatisticsTest)
{
  BOOST_CHECK_CLOSE(3.5, tl::math::range(vd.begin(), vd.end()), 0.1);
  BOOST_CHECK_CLOSE(3.5, tl::math::range(vd), 0.01);
}

BOOST_FIXTURE_TEST_CASE(variance, StatisticsTest)
{
  BOOST_CHECK_CLOSE(33.2, tl::math::variance(vi2.begin(), vi2.end()), 0.1);
  BOOST_CHECK_CLOSE(33.2, tl::math::variance(vi2), 0.1);
}

BOOST_FIXTURE_TEST_CASE(standarDeviation, StatisticsTest)
{
  BOOST_CHECK_CLOSE(5.76, tl::math::standarDeviation(vi2.begin(), vi2.end()), 0.1);
  BOOST_CHECK_CLOSE(5.76, tl::math::standarDeviation(vi2), 0.1);
}

BOOST_FIXTURE_TEST_CASE(coefficientOfVariation, StatisticsTest)
{
  BOOST_CHECK_CLOSE(0.411567, tl::math::coefficientOfVariation(vi2.begin(), vi2.end()), 0.1);
  BOOST_CHECK_CLOSE(0.411567, tl::math::coefficientOfVariation(vi2), 0.1);
}

BOOST_FIXTURE_TEST_CASE(covariance, StatisticsTest)
{
  BOOST_CHECK_CLOSE(-33.06, tl::math::covariance(x.begin(), x.end(), y.begin(), y.end()), 0.1);
  BOOST_CHECK_CLOSE(-33.06, tl::math::covariance(x, y), 0.1);
}

//Pearson correlation coefficient

BOOST_FIXTURE_TEST_CASE(pearsonCorrelationCoefficient, StatisticsTest)
{
  BOOST_CHECK_CLOSE(-0.8459, tl::math::pearsonCorrelationCoefficient(x.begin(), x.end(), y.begin(), y.end()), 0.1);
  BOOST_CHECK_CLOSE(-0.8459, tl::math::pearsonCorrelationCoefficient(x, y), 0.1);
}

BOOST_AUTO_TEST_SUITE_END()