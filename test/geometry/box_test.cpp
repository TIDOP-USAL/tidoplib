#include <gtest/gtest.h>
#include "geometry/entities/bbox.h"

using namespace TL;
using namespace TL::geometry;

// Constructor por defecto

TEST(BoxI, DefaultConstructor)
{
  const BoxI box;
  EXPECT_EQ(TL_INT_MAX, box.pt1.x);
  EXPECT_EQ(TL_INT_MAX, box.pt1.y);
  EXPECT_EQ(TL_INT_MAX, box.pt1.z);
  EXPECT_EQ(TL_INT_MIN, box.pt2.x);
  EXPECT_EQ(TL_INT_MIN, box.pt2.y);
  EXPECT_EQ(TL_INT_MIN, box.pt2.z);
}

TEST(BoxD, DefaultConstructor)
{
  const BoxD box;
  EXPECT_EQ(TL_DOUBLE_MAX, box.pt1.x);
  EXPECT_EQ(TL_DOUBLE_MAX, box.pt1.y);
  EXPECT_EQ(TL_DOUBLE_MAX, box.pt1.z);
  EXPECT_EQ(TL_DOUBLE_MIN, box.pt2.x);
  EXPECT_EQ(TL_DOUBLE_MIN, box.pt2.y);
  EXPECT_EQ(TL_DOUBLE_MIN, box.pt2.z);
}

TEST(BoxF, DefaultConstructor)
{
  const BoxF box;
  EXPECT_EQ(TL_FLOAT_MAX, box.pt1.x);
  EXPECT_EQ(TL_FLOAT_MAX, box.pt1.y);
  EXPECT_EQ(TL_FLOAT_MAX, box.pt1.z);
  EXPECT_EQ(TL_FLOAT_MIN, box.pt2.x);
  EXPECT_EQ(TL_FLOAT_MIN, box.pt2.y);
  EXPECT_EQ(TL_FLOAT_MIN, box.pt2.z);
}

// Constructor de copia

TEST(BoxI, CopyConstructor) {
  const BoxI box(Point3I(0, 0, 0), Point3I(100, 100, 100));
  BoxI box2(box);
  EXPECT_EQ( box.pt1, box2.pt1);
  EXPECT_EQ( box.pt2, box2.pt2);
}

TEST(BoxF, CopyConstructor)
{
  const BoxF box(Point3F(0.f, 0.f, 0.f), Point3F(100.f, 100.f, 100.f));
  BoxF box2(box);
  EXPECT_EQ(box.pt1, box2.pt1);
  EXPECT_EQ(box.pt2, box2.pt2);
}

TEST(BoxD, CopyConstructor)
{
  const BoxD box(Point3D(0., 0., 0.), Point3D(100., 100., 100.));
  BoxD box2(box);
  EXPECT_EQ(box.pt1, box2.pt1);
  EXPECT_EQ(box.pt2, box2.pt2);
}

//constructor de copia tipos diferentes

TEST(BoxD, CopyConstructorDiff)
{
  const BoxD box(Point3D(0.5, 0.4, 1.9), Point3D(100.6, 100.4, 105.49));
  BoxI box2(box);
  EXPECT_EQ(1, box2.pt1.x);
  EXPECT_EQ(0, box2.pt1.y);
  EXPECT_EQ(2, box2.pt1.z);
  EXPECT_EQ(101, box2.pt2.x);
  EXPECT_EQ(100, box2.pt2.y);
  EXPECT_EQ(105, box2.pt2.z);
}

// Comprobación de que redondea bien con ventanas de enteros

TEST(Box, Round)
{
  // Tamaño par
  Point3I ptc(50, 50, 50);
  BoxI box(ptc, 50, 50, 50);
  EXPECT_EQ(ptc, box.getCenter());
  EXPECT_EQ(50, box.getWidth());
  EXPECT_EQ(50, box.getHeight());
  EXPECT_EQ(50, box.getDepth());

  //Tamaño impar
  Point3I ptc2(50, 50, 50);
  BoxI box2(ptc2, 51, 51, 51);
  EXPECT_EQ(ptc2, box2.getCenter());
  EXPECT_EQ(51, box2.getWidth());
  EXPECT_EQ(51, box2.getHeight());
  EXPECT_EQ(51, box2.getDepth());

  // Ventana double
  Point3D ptc3(50.67, 50.76, 50.76);
  BoxD box3(ptc3, 100.32, 254.25, 124.5);
  EXPECT_NEAR(ptc3.x, box3.getCenter().x, 0.01);
  EXPECT_NEAR(ptc3.y, box3.getCenter().y, 0.01);
  EXPECT_NEAR(ptc3.z, box3.getCenter().z, 0.01);
  EXPECT_NEAR(100.32, box3.getWidth(), 0.01);
  EXPECT_NEAR(254.25, box3.getHeight(), 0.01);
  EXPECT_NEAR(124.5, box3.getDepth(), 0.01);

  // Ventana float
  Point3F ptc4(50.67f, 34.45f, 10.23f);
  BoxF box4(ptc4, 100.34f, 254.23f, 123.34f);
  EXPECT_NEAR(ptc4.x, box4.getCenter().x, 0.01);
  EXPECT_NEAR(ptc4.y, box4.getCenter().y, 0.01);
  EXPECT_NEAR(ptc4.z, box4.getCenter().z, 0.01);
  EXPECT_NEAR(100.34f, box4.getWidth(), 0.01);
  EXPECT_NEAR(254.23f, box4.getHeight(), 0.01);
  EXPECT_NEAR(123.34f, box4.getDepth(), 0.01);
}
