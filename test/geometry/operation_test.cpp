#include <gtest/gtest.h>
#include <tidop/geometry/operations.h>
#include <tidop/geometry/entities/point.h>
#include <tidop/geometry/entities/linestring.h>
#include <tidop/geometry/entities/segment.h>

using namespace TL;
using namespace TL::geometry;


class OperationsGeometriesTest : public testing::Test
{
public:

  virtual void SetUp()
  {
    pt_int = PointI(10, 50);

    pt1 = PointD(21.45, 34.67);
    pt2 = PointD(234.54, 45.76);

    pt1d = Point3D(3423.56, 345.78, 34.8);
    pt2d = Point3D(1234.45, 456.98, 45.9);

    segment = Segment<PointI>(PointI(0, 0), PointI(50, 50));
  }

  virtual void TearDown()
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


TEST_F(OperationsGeometriesTest, distance)
{
  EXPECT_NEAR(213.378, distance(pt1, pt2), 0.001);
  EXPECT_NEAR(213.378, distance(pt2, pt1), 0.001);
}

TEST_F(OperationsGeometriesTest, distance3D)
{
  EXPECT_NEAR(2191.961, distance3D(pt1d, pt2d), 0.001);
  EXPECT_NEAR(2191.961, distance3D(pt2d, pt1d), 0.001);
}

TEST_F(OperationsGeometriesTest, isLeft)
{
  EXPECT_EQ(1, isLeft(segment.pt1, segment.pt2, pt_int));
}
