#include <gtest/gtest.h>

#include "config_tl.h"

#include <tidop/math/statistics.h>

#include <array>
#include <list>

using namespace tl::math;

class StatisticsTest
  : public testing::Test
{
public:

  StatisticsTest() {}

protected:

  virtual void SetUp() override
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

  virtual void TearDown() override
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

TEST_F(StatisticsTest, mean)
{
  EXPECT_NEAR(7.695, mean(vd.begin(), vd.end()), 0.01);
  EXPECT_NEAR(7.695, mean(vd), 0.01);

  EXPECT_NEAR(1.142857143, mean(vi.begin(), vi.end()), 0.001);
  EXPECT_NEAR(1.142857143, mean(vi), 0.001);
}

TEST_F(StatisticsTest, median)
{
  EXPECT_NEAR(8, median(vd.begin(), vd.end()), 0.01);
  EXPECT_NEAR(8, median(vd), 0.01);
  EXPECT_NEAR(4.5, median(l.begin(), l.end()), 0.01);
  EXPECT_NEAR(4.5, median(l), 0.01);
}

TEST_F(StatisticsTest, mode)
{
  EXPECT_EQ(1, mode(vi.begin(), vi.end()));
  EXPECT_EQ(1, mode(vi));
  /// TODO: si hay mas de un valor mas repetido???
}

TEST_F(StatisticsTest, range)
{
  EXPECT_NEAR(3.5, range(vd.begin(), vd.end()), 0.01);
  EXPECT_NEAR(3.5, range(vd), 0.01);
}

TEST_F(StatisticsTest, variance)
{
  EXPECT_NEAR(33.2, variance(vi2.begin(), vi2.end()), 0.01);
  EXPECT_NEAR(33.2, variance(vi2), 0.01);
}

TEST_F(StatisticsTest, standarDeviation)
{
  EXPECT_NEAR(5.76, standarDeviation(vi2.begin(), vi2.end()), 0.01);
  EXPECT_NEAR(5.76, standarDeviation(vi2), 0.01);
}

TEST_F(StatisticsTest, coefficientOfVariation)
{
  EXPECT_NEAR(0.411, coefficientOfVariation(vi2.begin(), vi2.end()), 0.001);
  EXPECT_NEAR(0.411, coefficientOfVariation(vi2), 0.001);
}

TEST_F(StatisticsTest, covariance)
{
  EXPECT_NEAR(-33.06, covariance(x.begin(), x.end(), y.begin(), y.end()), 0.01);
  EXPECT_NEAR(-33.06, covariance(x, y), 0.01);
}

//Pearson correlation coefficient

TEST_F(StatisticsTest, pearsonCorrelationCoefficient)
{
  EXPECT_NEAR(-0.85, pearsonCorrelationCoefficient(x.begin(), x.end(), y.begin(), y.end()), 0.01);
  EXPECT_NEAR(-0.85, pearsonCorrelationCoefficient(x, y), 0.01);
}

