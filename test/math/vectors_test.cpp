#define BOOST_TEST_MODULE Tidop vectors test
#include <boost/test/unit_test.hpp>
#include "config_tl.h"

#include <tidop/math/algebra/vectors.h>
#include <tidop/geometry/entities/point.h>

using namespace tl;
using namespace geometry;
using namespace math;

/* ---------------------------------------------------------------------------------- */
/*                                    Vectores                                        */
/* ---------------------------------------------------------------------------------- */


#ifdef HAVE_OPENCV
BOOST_AUTO_TEST_CASE(vectorAngle_TestCvPoint)
{
  BOOST_CHECK_CLOSE(0, vectorAngle(cv::Point(0, 0), cv::Point(0, 0)), 0.1);
  BOOST_CHECK_CLOSE(0.785, vectorAngle(cv::Point(0, 2), cv::Point(1, 1)), 0.1);
  BOOST_CHECK_CLOSE(0.516, vectorAngle(cv::Point(23, 567), cv::Point(345, 555)), 0.1);
  BOOST_CHECK_CLOSE(0.442, vectorAngle(cv::Point(123, 234), cv::Point(456, 343)), 0.1);
}

BOOST_AUTO_TEST_CASE(vectorAngle_TestCvPoint2d)
{

  BOOST_CHECK_CLOSE(1.471, vectorAngle(cv::Point2d(23.423, 234234.5345), cv::Point2d(23423.345, 2345.345)), 0.1);
  BOOST_CHECK_CLOSE(0.10033, vectorAngle(cv::Point2d(2234.456, 245234534.345), cv::Point2d(2452345.565, 24357654.55)), 0.1);
}

#endif

BOOST_AUTO_TEST_CASE(vectorAngle_TestPointI)
{
  BOOST_CHECK_CLOSE(0, vectorAngle(PointI(0, 0), PointI(0, 0)), 0.1);
  BOOST_CHECK_CLOSE(0.785, vectorAngle(PointI(0, 2), PointI(1, 1)), 0.1);
  BOOST_CHECK_CLOSE(0.516, vectorAngle(PointI(23, 567), PointI(345, 555)), 0.1);
  BOOST_CHECK_CLOSE(0.442, vectorAngle(PointI(123, 234), PointI(456, 343)), 0.1);
}

BOOST_AUTO_TEST_CASE(vectorAngle_TestCvPointD)
{
  BOOST_CHECK_CLOSE(1.471, vectorAngle(PointD(23.423, 234234.5345), PointD(23423.345, 2345.345)), 0.1);
  BOOST_CHECK_CLOSE(0.10033, vectorAngle(PointD(2234.456, 245234534.345), PointD(2452345.565, 24357654.55)), 0.1);
}

BOOST_AUTO_TEST_CASE(vectorAngle_TestCvPointF)
{
  BOOST_CHECK_CLOSE(1.471f, vectorAngle(PointF(23.423f, 234234.5345f), PointF(23423.345f, 2345.345f)), 0.1);
  BOOST_CHECK_CLOSE(0.10033f, vectorAngle(PointF(2234.456f, 245234534.345f), PointF(2452345.565f, 24357654.55f)), 0.1);
}

BOOST_AUTO_TEST_CASE(azimut_Test1)
{
  //TODO: hacer mas pruebas
  BOOST_CHECK_CLOSE(0.7853, azimut(PointI(0, 0), PointI(100, 100)), 0.1);
}

