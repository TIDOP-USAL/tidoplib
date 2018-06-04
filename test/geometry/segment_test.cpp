#include <gtest/gtest.h>
#include "geometry/entities/segment.h"
#include "core/utils.h"

using namespace TL;
using namespace TL::geometry;

/* Constructor por defecto */

TEST(Segment, DefaultConstructor)
{
  Segment<Point<double>> segment;
  EXPECT_EQ(0., segment.pt1.x);
  EXPECT_EQ(0., segment.pt1.y);
  EXPECT_EQ(0., segment.pt2.x);
  EXPECT_EQ(0., segment.pt2.y);
}

/* Constructor punto inicial y final del segmento */

TEST(Segment, ConstructorPointIniEnd)
{
  Segment<Point<double>> segment(PointD(5.5, 93.2), PointD(25.3, 654.4));
  EXPECT_EQ(5.5, segment.pt1.x);
  EXPECT_EQ(93.2, segment.pt1.y);
  EXPECT_EQ(25.3, segment.pt2.x);
  EXPECT_EQ(654.4, segment.pt2.y);
}

/* Constructor de copia */

TEST(Segment, CopyConstructor)
{
  Segment<Point<double>> segment(PointD(5.5, 93.2), PointD(25.3, 654.4));
  Segment<Point<double>> segment_c(segment);
  EXPECT_EQ(5.5, segment_c.pt1.x);
  EXPECT_EQ(93.2, segment_c.pt1.y);
  EXPECT_EQ(25.3, segment_c.pt2.x);
  EXPECT_EQ(654.4, segment_c.pt2.y);
}

/* Operador de asignación */

TEST(Segment, assignment)
{
  Segment<Point<double>> segment(PointD(5.5, 93.2), PointD(25.3, 654.4));

  SegmentD segment_a = segment;

  EXPECT_EQ(5.5, segment_a.pt1.x);
  EXPECT_EQ(93.2, segment_a.pt1.y);
  EXPECT_EQ(25.3, segment_a.pt2.x);
  EXPECT_EQ(654.4, segment_a.pt2.y);
}

/* Conversión a un tipo diferente */

TEST(Segment, conversion)
{
  Segment<Point<double>> segment(PointD(5.5, 93.2), PointD(25.3, 654.4));
  Segment<Point<int>> segment_int = static_cast<Segment<Point<int>>>(segment);
  EXPECT_EQ(6, segment_int.pt1.x);
  EXPECT_EQ(93, segment_int.pt1.y);
  EXPECT_EQ(25, segment_int.pt2.x);
  EXPECT_EQ(654, segment_int.pt2.y);
}

/* Angulo medido respecto al eje x */

TEST(Segment, angleOX)
{
  Segment<Point<double>> segment(PointD(5.5, 93.2), PointD(25.3, 654.4));
  EXPECT_NEAR(1.5355294, segment.angleOX(), 0.001);
}

/* Angulo medido respecto al eje y */

TEST(Segment, angleOY)
{
  Segment<Point<double>> segment(PointD(5.5, 93.2), PointD(25.3, 654.4));
  EXPECT_NEAR(0.03526, segment.angleOY(), 0.001);
}

/* Ventana envolvente */
TEST(Segment, getWindow)
{
  Segment<Point<double>> segment(PointD(56.23, 123.5), PointD(96.2, 34.4));
  WindowD w_s = segment.getWindow();
  WindowD w(PointD(56.23, 34.4), PointD(96.2, 123.5));
  EXPECT_EQ(w.pt1.x, w_s.pt1.x);
  EXPECT_EQ(w.pt1.y, w_s.pt1.y);
  EXPECT_EQ(w.pt2.x, w_s.pt2.x);
  EXPECT_EQ(w.pt2.y, w_s.pt2.y);
}

/* Comprueba si el segmento esta vacio */

TEST(Segment, isEmpty)
{
  Segment<Point<double>> segment(PointD(56.23, 123.5), PointD(96.2, 34.4));
  segment.isEmpty();

  EXPECT_FALSE(segment.isEmpty());

  Segment<Point<double>> segment2;
  EXPECT_TRUE(segment2.isEmpty());
}

///TODO:
//isNear
//isParallel
//length
//vector