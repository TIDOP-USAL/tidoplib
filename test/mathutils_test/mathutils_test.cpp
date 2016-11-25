#include <gtest/gtest.h>
#include "core/mathutils.h"

using namespace I3D;

/* ---------------------------------------------------------------------------------- */
/*                                    Vectores                                        */
/* ---------------------------------------------------------------------------------- */

TEST(vectorAngle, Test1){
  EXPECT_NEAR(0, vectorAngle(cv::Point(0,0),cv::Point(0,0)),0.001);
  EXPECT_NEAR(0.785, vectorAngle(cv::Point(0,2),cv::Point(1,1)),0.001);
  EXPECT_NEAR(0.516, vectorAngle(cv::Point(23,567),cv::Point(345,555)),0.001);
  EXPECT_NEAR(0.442, vectorAngle(cv::Point(123,234),cv::Point(456,343)),0.001);
  EXPECT_NEAR(1.471, vectorAngle(cv::Point2d(23.423,234234.5345),cv::Point2d(23423.345,2345.345)),0.001);
  EXPECT_NEAR(0.100, vectorAngle(cv::Point2d(2234.456,245234534.345),cv::Point2d(2452345.565,24357654.55)),0.001);
}


TEST(azimut,Test1){

  EXPECT_NEAR(0.7853, azimut(cv::Point(0,0),cv::Point(100,100)),0.0001);
}

/* ---------------------------------------------------------------------------------- */
/*                          Test conversión de ángulos                                */
/* ---------------------------------------------------------------------------------- */

//Datos para los test
std::vector<double> decimalDegrees{ 0., 0.5, 135.5742, 86.9997, -269.385 };
std::vector<std::vector<int>> dms{ { 0, 0, 0 }, { 0, 30, 0 }, { 135, 34, 27 }, { 86, 59, 59 }, { -269, 23, 6 } };
std::vector<double> radians{ 0.,0.00872664626,2.3662161708,1.518431213, -4.7016552055 };
std::vector<double> gradians{ 0., 0.555556, 150.63800, 96.666335, -299.3166718 };

TEST(degreesToDecimalDegrees, Test1) {
  for (int i = 0; i < dms.size(); i++) {
    EXPECT_NEAR(decimalDegrees[i], degreesToDecimalDegrees(dms[i][0], dms[i][1], dms[i][2]),0.001);
  }
}

TEST(degreesToRadians, Test1) {
  for (int i = 0; i < dms.size(); i++) {
    EXPECT_NEAR(radians[i], degreesToRadians(dms[i][0], dms[i][1], dms[i][2]), 0.000001);
  }
}

TEST(degreesToGradians, Test1) {
  for (int i = 0; i < dms.size(); i++) {
    EXPECT_NEAR(gradians[i], degreesToGradians(dms[i][0], dms[i][1], dms[i][2]), 0.0001);
  }
}

  
TEST(decimalDegreesToDegrees, Test1) {
  int degrees, minutes, seconds;
  for (int i = 0; i < decimalDegrees.size(); i++) {
    decimalDegreesToDegrees(decimalDegrees[i], &degrees, &minutes, &seconds);
    EXPECT_EQ(dms[i][0], degrees);
    EXPECT_EQ(dms[i][1], minutes);
    EXPECT_EQ(dms[i][2], seconds);
  }
}

TEST(decimalDegreesToRadians, Test1) {
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


GTEST_API_ int main(int argc, char **argv) {
  printf("Running main() from gtest_main.cc\n");
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
