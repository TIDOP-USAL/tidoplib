#define BOOST_TEST_MODULE Tidop math utils test
#include <boost/test/unit_test.hpp>
#include <tidop/math/mathutils.h>
#include <tidop/geometry/entities/point.h>

using namespace tl;
using namespace tl::geometry;

// Para BOOST_AUTO_TEST_CASEear angulos de euler
//http://www.euclideanspace.com/maths/algebra/matrix/transforms/examples/index.htm
//http://euclideanspace.com/maths/geometry/rotations/conversions/index.htm


BOOST_AUTO_TEST_CASE(regressionLinear_YX)
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

  BOOST_CHECK_CLOSE(-0.7929, m, 0.1);
  BOOST_CHECK_CLOSE(249.9, b, 0.1);

}


//BOOST_AUTO_TEST_CASE(expRegression, BOOST_AUTO_TEST_CASE1){
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
//  BOOST_CHECK_CLOSE(1.3774, r, 0.0001);
//  BOOST_CHECK_CLOSE(2.6770, A, 0.0001);
//
//}

//Para los calculos del BOOST_AUTO_TEST_CASE:
//http://keisan.casio.com/exec/system/1223596129
//http://onlinemschool.com/math/assistance/cartesian_coordinate/plane/
BOOST_AUTO_TEST_CASE(threePointsPlane_PointInt)
{
  std::array<Point3I, 3> pts = {
    Point3I(1, 2, -2),
    Point3I(3, -2, 1),
    Point3I(5, 1, -4)
  };

  std::array<double, 4> plane;
  //Sin normalizar
  threePointsPlane(pts, plane, false);

  BOOST_CHECK_EQUAL(11, plane[0]);
  BOOST_CHECK_EQUAL(16, plane[1]);
  BOOST_CHECK_EQUAL(14, plane[2]);
  BOOST_CHECK_EQUAL(-15, plane[3]);

}


BOOST_AUTO_TEST_CASE(threePointsPlane_PointDouble)
{
  std::array<Point3D, 3> pts = {
    Point3D(34.34, 234.56, 158.65),
    Point3D(34.22, -245.91, 224.85),
    Point3D(25.65, 174.32, -148.69)
  };

  std::array<double, 4> plane;
  //Sin normalizar
  threePointsPlane(pts, plane, false);

  BOOST_CHECK_CLOSE(151655.5378, plane[0], 0.001);
  BOOST_CHECK_CLOSE(-612.1588, plane[1], 0.001);
  BOOST_CHECK_CLOSE(-4168.0555, plane[2], 0.001);
  BOOST_CHECK_CLOSE(-4403001.195, plane[3], 0.001);

}
