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
};

TEST_F(StatisticsTest, mean)
{
  double m = mean(vd.begin(), vd.end());
  EXPECT_NEAR(7.695, m, 0.01);
}

TEST_F(StatisticsTest, median)
{
  double m = median(vd.begin(), vd.end());
  EXPECT_NEAR(8, m, 0.01);

  m = median(l.begin(), l.end());
  EXPECT_NEAR(4.5, m, 0.01);
}

TEST_F(StatisticsTest, mode)
{
  int _mode = mode(vi.begin(), vi.end());
  EXPECT_EQ(1, _mode);

  /// TODO: si hay mas de un valor mas repetido???
}

TEST_F(StatisticsTest, range)
{
  double R = range(vd.begin(), vd.end());
  EXPECT_NEAR(3.5, R, 0.01);
}

TEST_F(StatisticsTest, variance)
{
  double v = variance(vi2.begin(), vi2.end());
  EXPECT_NEAR(33.2, v, 0.01);
}

TEST_F(StatisticsTest, standarDeviation)
{
  double s = standarDeviation(vi2.begin(), vi2.end());
  EXPECT_NEAR(5.76, s, 0.01);
}

TEST_F(StatisticsTest, coefficientOfVariation)
{
  double cov = coefficientOfVariation(vi2.begin(), vi2.end());
  EXPECT_NEAR(0.411, cov, 0.001);
}
