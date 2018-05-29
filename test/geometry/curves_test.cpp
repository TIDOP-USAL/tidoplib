#include <gtest/gtest.h>
#include "geometry/entities/curves.h"

using namespace TL;
using namespace TL::geometry;

/* Circle */

/* Constructor por defecto */

TEST(CircleI, DefaultConstructor)
{
  const CircleI circle;
  EXPECT_EQ(0, circle.center.x);
  EXPECT_EQ(0, circle.center.y);
  EXPECT_EQ(1, circle.radius);
}

TEST(CircleD, DefaultConstructor)
{
  const CircleD circle;
  EXPECT_DOUBLE_EQ(0., circle.center.x);
  EXPECT_DOUBLE_EQ(0., circle.center.y);
  EXPECT_DOUBLE_EQ(1., circle.radius);
}

TEST(CircleF, DefaultConstructor)
{
  const CircleF circle;
  EXPECT_FLOAT_EQ(0.f, circle.center.x);
  EXPECT_FLOAT_EQ(0.f, circle.center.y);
  EXPECT_FLOAT_EQ(1.f, circle.radius);
}

// Constructor Centro y radio, y constructor de copia

TEST(CircleD, CopyConstructor) {
  const CircleD circle(PointD(50.5, 32.65), 25.);
  EXPECT_DOUBLE_EQ(50.5, circle.center.x);
  EXPECT_DOUBLE_EQ(32.65, circle.center.y);
  EXPECT_DOUBLE_EQ(25., circle.radius);

  CircleD circle2(circle);
  EXPECT_DOUBLE_EQ(50.5, circle2.center.x);
  EXPECT_DOUBLE_EQ(32.65, circle2.center.y);
  EXPECT_DOUBLE_EQ(25., circle2.radius);
}

/* Operador de asignación */

TEST(CircleD, assignment)
{
  const CircleD circle(PointD(50.5, 32.65), 25.);

  CircleD circle2 = circle;
  EXPECT_DOUBLE_EQ(50.5, circle2.center.x);
  EXPECT_DOUBLE_EQ(32.65, circle2.center.y);
  EXPECT_DOUBLE_EQ(25., circle2.radius);
}

/* Conversión a un tipo diferente */

TEST(CircleD, conversion)
{
  CircleI circle(PointI(50, 32), 25);
  CircleD circle2 = static_cast<Circle<double>>(circle);
  EXPECT_DOUBLE_EQ(50., circle2.center.x);
  EXPECT_DOUBLE_EQ(32., circle2.center.y);
  EXPECT_DOUBLE_EQ(25., circle2.radius);
}

TEST(CircleD, area)
{
  CircleD circle(PointI(50., 50.), 25.);
  EXPECT_NEAR(1963.495, circle.area(), 0.1);
}

TEST(CircleD, length)
{
  CircleD circle(PointI(50., 50.), 25.);
  EXPECT_NEAR(157.0796, circle.length(), 0.01);
}


/* Ellipse */

// Constructor por defecto

TEST(EllipseI, DefaultConstructor)
{
  const EllipseI ellipse;
  EXPECT_EQ(0, ellipse.center.x);
  EXPECT_EQ(0, ellipse.center.y);
  EXPECT_EQ(1, ellipse.a);
  EXPECT_EQ(1, ellipse.b);
}

TEST(EllipseD, DefaultConstructor)
{
  const EllipseD ellipse;
  EXPECT_EQ(0., ellipse.center.x);
  EXPECT_EQ(0., ellipse.center.y);
  EXPECT_EQ(1., ellipse.a);
  EXPECT_EQ(1., ellipse.b);
}

TEST(EllipseF, DefaultConstructor)
{
  const EllipseF ellipse;
  EXPECT_FLOAT_EQ(0.f, ellipse.center.x);
  EXPECT_FLOAT_EQ(0.f, ellipse.center.y);
  EXPECT_FLOAT_EQ(1.f, ellipse.a);
  EXPECT_FLOAT_EQ(1.f, ellipse.b);
}

// Constructor Centro y semiejes

TEST(EllipseD, Constructor) {
  const EllipseD ellipse(PointD(50.5, 32.65), 25., 10.);
  EXPECT_DOUBLE_EQ(50.5, ellipse.center.x);
  EXPECT_DOUBLE_EQ(32.65, ellipse.center.y);
  EXPECT_DOUBLE_EQ(25., ellipse.a);
  EXPECT_DOUBLE_EQ(10., ellipse.b);
}

// Constructor de copia

TEST(EllipseD, CopyConstructor) {
  EllipseD ellipse(PointD(50.5, 32.65), 25., 10.);
  EllipseD ellipse2(ellipse);

  EXPECT_DOUBLE_EQ(50.5, ellipse2.center.x);
  EXPECT_DOUBLE_EQ(32.65, ellipse2.center.y);
  EXPECT_DOUBLE_EQ(25., ellipse2.a);
  EXPECT_DOUBLE_EQ(10., ellipse2.b);
}

// Operador de asignación
TEST(EllipseD, assignment)
{
  const EllipseD ellipse(PointD(50.5, 32.65), 25., 10.);

  EllipseD ellipse2 = ellipse;
  EXPECT_DOUBLE_EQ(50.5, ellipse2.center.x);
  EXPECT_DOUBLE_EQ(32.65, ellipse2.center.y);
  EXPECT_DOUBLE_EQ(25., ellipse2.a);
  EXPECT_DOUBLE_EQ(10., ellipse2.b);
}

TEST(EllipseD, conversion)
{
  EllipseI ellipse(PointI(50, 32), 25, 10);
  EllipseD ellipse2 = static_cast<EllipseD>(ellipse);
  EXPECT_DOUBLE_EQ(50., ellipse2.center.x);
  EXPECT_DOUBLE_EQ(32., ellipse2.center.y);
  EXPECT_DOUBLE_EQ(25., ellipse2.a);
  EXPECT_DOUBLE_EQ(10., ellipse2.b);
}

TEST(EllipseD, area)
{
  EllipseD ellipse(PointI(50., 50.), 3., 2.);
  EXPECT_NEAR(18.85, ellipse.area(), 0.01);
}

TEST(EllipseD, length)
{
  EllipseD ellipse(PointI(50., 50.), 3., 2.);
  EXPECT_NEAR(15.87, ellipse.length(), 0.1);
}

