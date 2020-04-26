#define BOOST_TEST_MODULE Tidop operations test
#include <boost/test/unit_test.hpp>
#include <tidop/geometry/operations.h>
#include <tidop/geometry/entities/point.h>
#include <tidop/geometry/entities/linestring.h>
#include <tidop/geometry/entities/segment.h>

using namespace tl;
using namespace tl::geometry;


BOOST_AUTO_TEST_SUITE(GeometricOperationsTestSuite)

struct GeometricOperations
{
  GeometricOperations()
  {

  }

  ~GeometricOperations()
  {

  }

  void setup()
  {
    pt_int = PointI(10, 50);

    pt1 = PointD(21.45, 34.67);
    pt2 = PointD(234.54, 45.76);

    pt1d = Point3D(3423.56, 345.78, 34.8);
    pt2d = Point3D(1234.45, 456.98, 45.9);

    segment = Segment<PointI>(PointI(0, 0), PointI(50, 50));
  }

  void teardown()
  {

  }

  PointI pt_int;
  PointD pt1;
  PointD pt2;
  Point3D pt1d;
  Point3D pt2d;
  Segment<PointI> segment;
  LineStringI line;
};


BOOST_FIXTURE_TEST_CASE(distance, GeometricOperations)
{
  BOOST_CHECK_CLOSE(213.378, tl::geometry::distance(pt1, pt2), 0.001);
  BOOST_CHECK_CLOSE(213.378, tl::geometry::distance(pt2, pt1), 0.001);
}

BOOST_FIXTURE_TEST_CASE(distance3D, GeometricOperations)
{
  BOOST_CHECK_CLOSE(2191.961, tl::geometry::distance3D(pt1d, pt2d), 0.001);
  BOOST_CHECK_CLOSE(2191.961, tl::geometry::distance3D(pt2d, pt1d), 0.001);
}

BOOST_FIXTURE_TEST_CASE(isLeft, GeometricOperations)
{
  BOOST_CHECK_EQUAL(1, tl::geometry::isLeft(segment.pt1, segment.pt2, pt_int));
}

BOOST_AUTO_TEST_SUITE_END()