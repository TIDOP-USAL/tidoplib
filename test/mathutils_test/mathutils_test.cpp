#include <gtest/gtest.h>
#include "core/mathutils.h"

using namespace I3D;

// Para testear angulos de euler
//http://www.euclideanspace.com/maths/algebra/matrix/transforms/examples/index.htm
//http://euclideanspace.com/maths/geometry/rotations/conversions/index.htm

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
  //... hacer mas pruebas
  EXPECT_NEAR(0.7853, azimut(cv::Point(0,0),cv::Point(100,100)),0.0001);
}

TEST(regressionLinear,YX){
  std::vector<cv::Point> pts{
    cv::Point(160, 126),
    cv::Point(180, 103),
    cv::Point(200, 82),
    cv::Point(220, 75),
    cv::Point(240, 82),
    cv::Point(260, 40),
    cv::Point(280, 20)
  };
  double m = 0.;
  double b = 0.;
  double coef = regressionLinearYX(pts, &m, &b);

  EXPECT_NEAR(-0.7929, m, 0.0001);
  EXPECT_NEAR(249.9, b, 0.1);

}


TEST(expRegression, test1){
  std::vector<cv::Point> pts{
    cv::Point(0, 3),
    cv::Point(2, 4),
    cv::Point(4, 11),
    cv::Point(7, 25)
  };
  double A = 0.;
  double r = 0.;
  expRegression<cv::Point>(pts, &A, &r);

  EXPECT_NEAR(1.3774, r, 0.0001);
  EXPECT_NEAR(2.6770, A, 0.0001);

}

//Para los calculos del test:
//http://keisan.casio.com/exec/system/1223596129
//http://onlinemschool.com/math/assistance/cartesian_coordinate/plane/
TEST(threePointsPlane, testPointInt){
  std::array<cv::Point3i,3> pts = {
    cv::Point3i(1, 2, -2),
    cv::Point3i(3, -2, 1),
    cv::Point3i(5, 1, -4)
  };

  std::array<double, 4> plane;
  //Sin normalizar
  threePointsPlane(pts, plane, false);

  EXPECT_EQ(11, plane[0]);
  EXPECT_EQ(16, plane[1]);
  EXPECT_EQ(14, plane[2]);
  EXPECT_EQ(-15, plane[3]);

}


TEST(threePointsPlane, testPointDouble){
  std::array<cv::Point3d,3> pts = {
    cv::Point3d(34.34, 234.56, 158.65),
    cv::Point3d(34.22, -245.91, 224.85),
    cv::Point3d(25.65, 174.32, -148.69)
  };

  std::array<double, 4> plane;
  //Sin normalizar
  threePointsPlane(pts, plane, false);

  EXPECT_NEAR(151655.5378, plane[0], 0.001);
  EXPECT_NEAR(-612.1588, plane[1], 0.001);
  EXPECT_NEAR(-4168.0555, plane[2], 0.001);
  EXPECT_NEAR(-4403001.195, plane[3], 0.001);

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
