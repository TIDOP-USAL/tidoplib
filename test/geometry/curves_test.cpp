#include <gtest/gtest.h>
#include <tidop/geometry/entities/curves.h>

using namespace tl;
using namespace tl::geometry;

/* Circle */

/* Constructor por defecto */

TEST(CircleI, DefaultConstructor)
{
  const CircleI circle;
  EXPECT_EQ(0, circle.center.x);
  EXPECT_EQ(0, circle.center.y);
  EXPECT_EQ(1, circle.radius);
  EXPECT_TRUE(circle.type() == Entity::Type::CIRCLE);
}

TEST(CircleD, DefaultConstructor)
{
  const CircleD circle;
  EXPECT_DOUBLE_EQ(0., circle.center.x);
  EXPECT_DOUBLE_EQ(0., circle.center.y);
  EXPECT_DOUBLE_EQ(1., circle.radius);
  EXPECT_TRUE(circle.Entity::type() == Entity::Type::CIRCLE);
}

TEST(CircleF, DefaultConstructor)
{
  const CircleF circle;
  EXPECT_FLOAT_EQ(0.f, circle.center.x);
  EXPECT_FLOAT_EQ(0.f, circle.center.y);
  EXPECT_FLOAT_EQ(1.f, circle.radius);
  EXPECT_TRUE(circle.type() == Entity::Type::CIRCLE);
}

// Constructor Centro y radio, y constructor de copia

TEST(CircleD, CopyConstructor) {
  const CircleD circle(PointD(50.5, 32.65), 25.);
  EXPECT_DOUBLE_EQ(50.5, circle.center.x);
  EXPECT_DOUBLE_EQ(32.65, circle.center.y);
  EXPECT_DOUBLE_EQ(25., circle.radius);
  EXPECT_TRUE(circle.type() == Entity::Type::CIRCLE);

  CircleD circle2(circle);
  EXPECT_DOUBLE_EQ(50.5, circle2.center.x);
  EXPECT_DOUBLE_EQ(32.65, circle2.center.y);
  EXPECT_DOUBLE_EQ(25., circle2.radius);
  EXPECT_TRUE(circle2.type() == Entity::Type::CIRCLE);
}

/* Operador de asignación */

TEST(CircleD, assignment)
{
  const CircleD circle(PointD(50.5, 32.65), 25.);

  CircleD circle2 = circle;
  EXPECT_DOUBLE_EQ(50.5, circle2.center.x);
  EXPECT_DOUBLE_EQ(32.65, circle2.center.y);
  EXPECT_DOUBLE_EQ(25., circle2.radius);
  EXPECT_TRUE(circle2.type() == Entity::Type::CIRCLE);
}

/* Conversión a un tipo diferente */

TEST(CircleD, conversion)
{
  CircleI circle(PointI(50, 32), 25);
  CircleD circle2 = static_cast<Circle<double>>(circle);
  EXPECT_DOUBLE_EQ(50., circle2.center.x);
  EXPECT_DOUBLE_EQ(32., circle2.center.y);
  EXPECT_DOUBLE_EQ(25., circle2.radius);
  EXPECT_TRUE(circle2.type() == Entity::Type::CIRCLE);
}

TEST(CircleD, area)
{
  CircleD circle(PointD(50., 50.), 25.);
  EXPECT_NEAR(1963.495, circle.area(), 0.1);
}

TEST(CircleD, length)
{
  CircleD circle(PointD(50., 50.), 25.);
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
  EXPECT_TRUE(ellipse.type() == Entity::Type::ELLIPSE);
}

TEST(EllipseD, DefaultConstructor)
{
  const EllipseD ellipse;
  EXPECT_EQ(0., ellipse.center.x);
  EXPECT_EQ(0., ellipse.center.y);
  EXPECT_EQ(1., ellipse.a);
  EXPECT_EQ(1., ellipse.b);
  EXPECT_TRUE(ellipse.type() == Entity::Type::ELLIPSE);
}

TEST(EllipseF, DefaultConstructor)
{
  const EllipseF ellipse;
  EXPECT_FLOAT_EQ(0.f, ellipse.center.x);
  EXPECT_FLOAT_EQ(0.f, ellipse.center.y);
  EXPECT_FLOAT_EQ(1.f, ellipse.a);
  EXPECT_FLOAT_EQ(1.f, ellipse.b);
  EXPECT_TRUE(ellipse.type() == Entity::Type::ELLIPSE);
}

// Constructor Centro y semiejes

TEST(EllipseD, Constructor) {
  const EllipseD ellipse(PointD(50.5, 32.65), 25., 10.);
  EXPECT_DOUBLE_EQ(50.5, ellipse.center.x);
  EXPECT_DOUBLE_EQ(32.65, ellipse.center.y);
  EXPECT_DOUBLE_EQ(25., ellipse.a);
  EXPECT_DOUBLE_EQ(10., ellipse.b);
  EXPECT_TRUE(ellipse.type() == Entity::Type::ELLIPSE);
}

// Constructor de copia

TEST(EllipseD, CopyConstructor) {
  EllipseD ellipse(PointD(50.5, 32.65), 25., 10.);
  EllipseD ellipse2(ellipse);

  EXPECT_DOUBLE_EQ(50.5, ellipse2.center.x);
  EXPECT_DOUBLE_EQ(32.65, ellipse2.center.y);
  EXPECT_DOUBLE_EQ(25., ellipse2.a);
  EXPECT_DOUBLE_EQ(10., ellipse2.b);
  EXPECT_TRUE(ellipse2.type() == Entity::Type::ELLIPSE);
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
  EXPECT_TRUE(ellipse2.type() == Entity::Type::ELLIPSE);
}

TEST(EllipseD, conversion)
{
  EllipseI ellipse(PointI(50, 32), 25, 10);
  EllipseD ellipse2 = static_cast<EllipseD>(ellipse);
  EXPECT_DOUBLE_EQ(50., ellipse2.center.x);
  EXPECT_DOUBLE_EQ(32., ellipse2.center.y);
  EXPECT_DOUBLE_EQ(25., ellipse2.a);
  EXPECT_DOUBLE_EQ(10., ellipse2.b);
  EXPECT_TRUE(ellipse2.type() == Entity::Type::ELLIPSE);
}

TEST(EllipseD, area)
{
  EllipseD ellipse(PointD(50., 50.), 3., 2.);
  EXPECT_NEAR(18.85, ellipse.area(), 0.01);
}

TEST(EllipseD, length)
{
  EllipseD ellipse(PointD(50., 50.), 3., 2.);
  EXPECT_NEAR(15.87, ellipse.length(), 0.1);
}

