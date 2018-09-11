#include <gtest/gtest.h>

#include "config_tl.h"

#include "core/mathutils.h"
#include "geometry/entities/point.h"

using namespace TL;
using namespace TL::geometry;

// Para testear angulos de euler
//http://www.euclideanspace.com/maths/algebra/matrix/transforms/examples/index.htm
//http://euclideanspace.com/maths/geometry/rotations/conversions/index.htm

/* ---------------------------------------------------------------------------------- */
/*                                    Vectores                                        */
/* ---------------------------------------------------------------------------------- */

//TEST(vector2D, Test)
//{
//  PointI v1 = vector2D(PointI(150, 100), PointI(10, 10));
//
//  EXPECT_EQ(140, v1.x);
//  EXPECT_EQ(90, v1.y);
//}
//
//TEST(vector3D, Test)
//{
//  Point3I v1 = vector3D(Point3I(150, 100, 20), Point3I(10, 10, 30));
//
//  EXPECT_EQ(140, v1.x);
//  EXPECT_EQ(90, v1.y);
//  EXPECT_EQ(-10, v1.z);
//}

#ifdef HAVE_OPENCV
TEST(vectorAngle, TestCvPoint)
{
  EXPECT_NEAR(0, vectorAngle(cv::Point(0,0),cv::Point(0,0)),0.001);
  EXPECT_NEAR(0.785, vectorAngle(cv::Point(0,2),cv::Point(1,1)),0.001);
  EXPECT_NEAR(0.516, vectorAngle(cv::Point(23,567),cv::Point(345,555)),0.001);
  EXPECT_NEAR(0.442, vectorAngle(cv::Point(123,234),cv::Point(456,343)),0.001);
}

TEST(vectorAngle, TestCvPoint2d)
{

  EXPECT_NEAR(1.471, vectorAngle(cv::Point2d(23.423, 234234.5345), cv::Point2d(23423.345, 2345.345)), 0.001);
  EXPECT_NEAR(0.100, vectorAngle(cv::Point2d(2234.456, 245234534.345), cv::Point2d(2452345.565, 24357654.55)), 0.001);
}

#endif

TEST(vectorAngle, TestPointI)
{
  EXPECT_NEAR(0, vectorAngle(PointI(0, 0), PointI(0, 0)), 0.001);
  EXPECT_NEAR(0.785, vectorAngle(PointI(0, 2), PointI(1, 1)), 0.001);
  EXPECT_NEAR(0.516, vectorAngle(PointI(23, 567), PointI(345, 555)), 0.001);
  EXPECT_NEAR(0.442, vectorAngle(PointI(123, 234), PointI(456, 343)), 0.001);
}

TEST(vectorAngle, TestCvPointD)
{
  EXPECT_NEAR(1.471, vectorAngle(PointD(23.423, 234234.5345), PointD(23423.345, 2345.345)), 0.001);
  EXPECT_NEAR(0.100, vectorAngle(PointD(2234.456, 245234534.345), PointD(2452345.565, 24357654.55)), 0.001);
}

TEST(vectorAngle, TestCvPointF)
{
  EXPECT_NEAR(1.471f, vectorAngle(PointF(23.423f, 234234.5345f), PointF(23423.345f, 2345.345f)), 0.001);
  EXPECT_NEAR(0.100f, vectorAngle(PointF(2234.456f, 245234534.345f), PointF(2452345.565f, 24357654.55f)), 0.001);
}

TEST(azimut, Test1)
{
  //TODO: hacer mas pruebas
  EXPECT_NEAR(0.7853, azimut(PointI(0, 0), PointI(100, 100)), 0.0001);
}

TEST(regressionLinear, YX)
{
  std::vector<PointI> pts{
    PointI(160, 126),
    PointI(180, 103),
    PointI(200, 82),
    PointI(220, 75),
    PointI(240, 82),
    PointI(260, 40),
    PointI(280, 20)
  };

  double m = 0.;
  double b = 0.;
  double coef = regressionLinearYX(pts, &m, &b);

  EXPECT_NEAR(-0.7929, m, 0.0001);
  EXPECT_NEAR(249.9, b, 0.1);

}


//TEST(expRegression, test1){
//  std::vector<cv::Point> pts{
//    cv::Point(0, 3),
//    cv::Point(2, 4),
//    cv::Point(4, 11),
//    cv::Point(7, 25)
//  };
//  double A = 0.;
//  double r = 0.;
//  expRegression<cv::Point>(pts, &A, &r);
//
//  EXPECT_NEAR(1.3774, r, 0.0001);
//  EXPECT_NEAR(2.6770, A, 0.0001);
//
//}

//Para los calculos del test:
//http://keisan.casio.com/exec/system/1223596129
//http://onlinemschool.com/math/assistance/cartesian_coordinate/plane/
TEST(threePointsPlane, testPointInt)
{
  std::array<Point3I, 3> pts = {
    Point3I(1, 2, -2),
    Point3I(3, -2, 1),
    Point3I(5, 1, -4)
  };

  std::array<double, 4> plane;
  //Sin normalizar
  threePointsPlane(pts, plane, false);

  EXPECT_EQ(11, plane[0]);
  EXPECT_EQ(16, plane[1]);
  EXPECT_EQ(14, plane[2]);
  EXPECT_EQ(-15, plane[3]);

}


TEST(threePointsPlane, testPointDouble)
{
  std::array<Point3D, 3> pts = {
    Point3D(34.34, 234.56, 158.65),
    Point3D(34.22, -245.91, 224.85),
    Point3D(25.65, 174.32, -148.69)
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

//class VectorTest : public testing::Test
//{
//public:
//  
//  std::vector<double> decimalDegrees;
//  std::vector<std::vector<int>> dms;
//  std::vector<double> radians;
//  std::vector<double> gradians;
//
//  virtual void SetUp()
//  {
//    decimalDegrees.push_back(0.);
//    decimalDegrees.push_back(0.5);
//    decimalDegrees.push_back(135.5742);
//    decimalDegrees.push_back(86.9997);
//    decimalDegrees.push_back(-269.385);
//    
//    dms.push_back({ 0, 0, 0 });
//    dms.push_back({ 0, 30, 0 });
//    dms.push_back({ 135, 34, 27 });
//    dms.push_back({ 86, 59, 59 });
//    dms.push_back({ -269, 23, 6 });
//    
//    radians.push_back(0.);
//    radians.push_back(0.00872664626);
//    radians.push_back(2.3662161708);
//    radians.push_back(1.518431213);
//    radians.push_back(-4.7016552055);
//
//    gradians.push_back(0.);
//    gradians.push_back(0.555556);
//    gradians.push_back(150.63800);
//    gradians.push_back(96.666335);
//    gradians.push_back(-299.3166718);
//  }
// 
//  virtual void TearDown()
//  {
//  }
//};

//std::vector<double> decimalDegrees = { 0., 0.5, 135.5742, 86.9997, -269.385 };
//std::vector<std::vector<int>> dms{ { 0, 0, 0 }, { 0, 30, 0 }, { 135, 34, 27 }, { 86, 59, 59 }, { -269, 23, 6 } };
//std::vector<double> radians{ 0.,0.00872664626,2.3662161708,1.518431213, -4.7016552055 };
//std::vector<double> gradians{ 0., 0.555556, 150.63800, 96.666335, -299.3166718 };

//TEST_F(VectorTest, degreesToDecimalDegrees) {
//  //for (int i = 0; i < dms.size(); i++) {
//  //  EXPECT_NEAR(decimalDegrees[i], degreesToDecimalDegrees(dms[i][0], dms[i][1], dms[i][2]), 0.001);
//  //}
//}
//
//TEST_F(VectorTest, degreesToRadians) {
//  //for (int i = 0; i < dms.size(); i++) {
//  //  EXPECT_NEAR(radians[i], degreesToRadians(dms[i][0], dms[i][1], dms[i][2]), 0.000001);
//  //}
//}

//TEST(degreesToGradians, Test1) {
//  for (int i = 0; i < dms.size(); i++) {
//    EXPECT_NEAR(gradians[i], degreesToGradians(dms[i][0], dms[i][1], dms[i][2]), 0.0001);
//  }
//}
//
//  
//TEST(decimalDegreesToDegrees, Test1) {
//  int degrees, minutes, seconds;
//  for (int i = 0; i < decimalDegrees.size(); i++) {
//    decimalDegreesToDegrees(decimalDegrees[i], &degrees, &minutes, &seconds);
//    EXPECT_EQ(dms[i][0], degrees);
//    EXPECT_EQ(dms[i][1], minutes);
//    EXPECT_EQ(dms[i][2], seconds);
//  }
//}

//TEST_F(VectorTest, decimalDegreesToRadians) {
//  for (int i = 0; i < decimalDegrees.size(); i++) {
//    EXPECT_NEAR(radians[i], decimalDegreesToRadians(decimalDegrees[i]), 0.000001);
//  }
//}

//TEST(decimalDegreesToGradians, Test1) {
//  for (int i = 0; i < decimalDegrees.size(); i++) {
//    EXPECT_NEAR(gradians[i], decimalDegreesToGradians(decimalDegrees[i]), 0.0001);
//  }
//}
//
//TEST(radiansToDegrees, Test1) {
//  int degrees, minutes, seconds;
//  for (int i = 0; i < radians.size(); i++) {
//    radiansToDegrees(radians[i], &degrees, &minutes, &seconds);
//    EXPECT_EQ(dms[i][0], degrees);
//    EXPECT_EQ(dms[i][1], minutes);
//    EXPECT_EQ(dms[i][2], seconds);
//  }
//}
//
//TEST(radiansToDecimalDegrees, Test1) {
//  for (int i = 0; i < radians.size(); i++) {
//    EXPECT_NEAR(decimalDegrees[i], radiansToDecimalDegrees(radians[i]), 0.0001);
//  }
//}
//
//TEST(radiansToGradians, Test1) {
//  for (int i = 0; i < radians.size(); i++) {
//    EXPECT_NEAR(gradians[i], radiansToGradians(radians[i]), 0.0001);
//  }
//}
//
//
//TEST(gradiansToDegrees, Test1) {
//  int degrees, minutes, seconds;
//  for (int i = 0; i < gradians.size(); i++) {
//    gradiansToDegrees(gradians[i], &degrees, &minutes, &seconds);
//    EXPECT_EQ(dms[i][0], degrees);
//    EXPECT_EQ(dms[i][1], minutes);
//    EXPECT_EQ(dms[i][2], seconds);
//  }
//}
//
//TEST(gradiansToDecimalDegrees, Test1) {
//  for (int i = 0; i < gradians.size(); i++) {
//    EXPECT_NEAR(decimalDegrees[i], gradiansToDecimalDegrees(gradians[i]), 0.0001);
//  }
//}
//
//TEST(gradiansToRadians, Test1) {
//  for (int i = 0; i < gradians.size(); i++) {
//    EXPECT_NEAR(radians[i], gradiansToRadians(gradians[i]), 0.00001);
//  }
//}
//
