#include <gtest/gtest.h>

#include "math/angles.h"

using namespace TL::math;

/* ---------------------------------------------------------------------------------- */
/*                          Test conversión de ángulos                                */
/* ---------------------------------------------------------------------------------- */

//Datos para los test

class VectorTest : public testing::Test
{
public:
  
  std::vector<double> decimalDegrees;
  std::vector<std::vector<int>> dms;
  std::vector<double> radians;
  std::vector<double> gradians;

  virtual void SetUp()
  {
    decimalDegrees.push_back(0.);
    decimalDegrees.push_back(0.5);
    decimalDegrees.push_back(135.5742);
    decimalDegrees.push_back(86.9997);
    decimalDegrees.push_back(-269.385);
    
    dms.push_back({ 0, 0, 0 });
    dms.push_back({ 0, 30, 0 });
    dms.push_back({ 135, 34, 27 });
    dms.push_back({ 86, 59, 59 });
    dms.push_back({ -269, 23, 6 });
    
    radians.push_back(0.);
    radians.push_back(0.00872664626);
    radians.push_back(2.3662161708);
    radians.push_back(1.518431213);
    radians.push_back(-4.7016552055);

    gradians.push_back(0.);
    gradians.push_back(0.555556);
    gradians.push_back(150.63800);
    gradians.push_back(96.666335);
    gradians.push_back(-299.3166718);
  }
 
  virtual void TearDown()
  {
  }
};

std::vector<double> decimalDegrees = { 0., 0.5, 135.5742, 86.9997, -269.385 };
std::vector<std::vector<int>> dms{ { 0, 0, 0 }, { 0, 30, 0 }, { 135, 34, 27 }, { 86, 59, 59 }, { -269, 23, 6 } };
std::vector<double> radians{ 0.,0.00872664626,2.3662161708,1.518431213, -4.7016552055 };
std::vector<double> gradians{ 0., 0.555556, 150.63800, 96.666335, -299.3166718 };

TEST_F(VectorTest, degreesToDecimalDegrees) 
{
  for (int i = 0; i < dms.size(); i++) {
    EXPECT_NEAR(decimalDegrees[i], degreesToDecimalDegrees(dms[i][0], dms[i][1], dms[i][2]), 0.001);
  }
}

TEST_F(VectorTest, degreesToRadians) 
{
  for (int i = 0; i < dms.size(); i++) {
    EXPECT_NEAR(radians[i], degreesToRadians(dms[i][0], dms[i][1], dms[i][2]), 0.000001);
  }
}

TEST(degreesToGradians, Test1)
{
  for (int i = 0; i < dms.size(); i++) {
    EXPECT_NEAR(gradians[i], degreesToGradians(dms[i][0], dms[i][1], dms[i][2]), 0.0001);
  }
}

  
TEST(decimalDegreesToDegrees, Test1) 
{
  int degrees, minutes, seconds;
  for (int i = 0; i < decimalDegrees.size(); i++) {
    decimalDegreesToDegrees(decimalDegrees[i], &degrees, &minutes, &seconds);
    EXPECT_EQ(dms[i][0], degrees);
    EXPECT_EQ(dms[i][1], minutes);
    EXPECT_EQ(dms[i][2], seconds);
  }
}

TEST_F(VectorTest, decimalDegreesToRadians)
{
  for (int i = 0; i < decimalDegrees.size(); i++) {
    EXPECT_NEAR(radians[i], decimalDegreesToRadians(decimalDegrees[i]), 0.000001);
  }
}

TEST(decimalDegreesToGradians, Test1) {
  for (int i = 0; i < decimalDegrees.size(); i++) {
    EXPECT_NEAR(gradians[i], decimalDegreesToGradians(decimalDegrees[i]), 0.0001);
  }
}

TEST(radiansToDegrees, Test1) {
  int degrees, minutes, seconds;
  for (int i = 0; i < radians.size(); i++) {
    radiansToDegrees(radians[i], &degrees, &minutes, &seconds);
    EXPECT_EQ(dms[i][0], degrees);
    EXPECT_EQ(dms[i][1], minutes);
    EXPECT_EQ(dms[i][2], seconds);
  }
}

TEST(radiansToDecimalDegrees, Test1) {
  for (int i = 0; i < radians.size(); i++) {
    EXPECT_NEAR(decimalDegrees[i], radiansToDecimalDegrees(radians[i]), 0.0001);
  }
}

TEST(radiansToGradians, Test1) {
  for (int i = 0; i < radians.size(); i++) {
    EXPECT_NEAR(gradians[i], radiansToGradians(radians[i]), 0.0001);
  }
}


TEST(gradiansToDegrees, Test1) {
  int degrees, minutes, seconds;
  for (int i = 0; i < gradians.size(); i++) {
    gradiansToDegrees(gradians[i], &degrees, &minutes, &seconds);
    EXPECT_EQ(dms[i][0], degrees);
    EXPECT_EQ(dms[i][1], minutes);
    EXPECT_EQ(dms[i][2], seconds);
  }
}

TEST(gradiansToDecimalDegrees, Test1) {
  for (int i = 0; i < gradians.size(); i++) {
    EXPECT_NEAR(decimalDegrees[i], gradiansToDecimalDegrees(gradians[i]), 0.0001);
  }
}

TEST(gradiansToRadians, Test1) {
  for (int i = 0; i < gradians.size(); i++) {
    EXPECT_NEAR(radians[i], gradiansToRadians(gradians[i]), 0.00001);
  }
}

