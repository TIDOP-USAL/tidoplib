#include <gtest/gtest.h>

#include "config_tl.h"

#include "core/mathutils.h"
#include "geometry/entities/point.h"

using namespace TL;
using namespace TL::geometry;

// Para testear angulos de euler
//http://www.euclideanspace.com/maths/algebra/matrix/transforms/examples/index.htm
//http://euclideanspace.com/maths/geometry/rotations/conversions/index.htm


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
