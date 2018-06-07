#include <gtest/gtest.h>
#include "geometry/entities/point.h"
#include "core/utils.h"

using namespace TL;
using namespace TL::geometry;

/* Constructor por defecto */

TEST(Point, DefaultConstructor) 
{
  
  PointI pt_int;

  EXPECT_EQ(0, pt_int.x);
  EXPECT_EQ(0, pt_int.y);

  PointD pt_double;
    
  EXPECT_EQ(0., pt_double.x);
  EXPECT_EQ(0., pt_double.y);

  PointF pt_float;
    
  EXPECT_EQ(0.f, pt_float.x);
  EXPECT_EQ(0.f, pt_float.y);

  Point<uint16_t> pt_uint16;
    
  EXPECT_EQ(0, pt_uint16.x);
  EXPECT_EQ(0, pt_uint16.y);
}

/* Constructor x y */

TEST(Point, ConstructorXY)
{
  PointI pt_int(23, 67);

  EXPECT_EQ(23, pt_int.x);
  EXPECT_EQ(67, pt_int.y);

  PointD pt_double(253.56, 562.94);
    
  EXPECT_NEAR(253.56, pt_double.x, 0.01);
  EXPECT_NEAR(562.94, pt_double.y, 0.01);

  PointF pt_float(564.26f, 646.65f);
    
  EXPECT_NEAR(564.26f, pt_float.x, 0.05);
  EXPECT_NEAR(646.65f, pt_float.y, 0.05);

  Point<int8_t> pt_int8(0, 4);
    
  EXPECT_EQ(0, pt_int8.x);
  EXPECT_EQ(4, pt_int8.y);
}

/* Constructor de copia */

TEST(Point, CopyConstructor) 
{
  PointI pt_int(23, 67);
  PointI pt_int_c(pt_int);

  EXPECT_EQ(23, pt_int_c.x);
  EXPECT_EQ(67, pt_int_c.y);

  PointD pt_double(253.56, 562.94);
  PointD pt_double_c(pt_double);

  EXPECT_NEAR(253.56, pt_double_c.x, 0.01);
  EXPECT_NEAR(562.94, pt_double_c.y, 0.01);

  PointF pt_float(564.26f, 646.65f);
  PointF pt_float_c(pt_float);

  EXPECT_NEAR(564.26f, pt_float_c.x, 0.05);
  EXPECT_NEAR(646.65f, pt_float_c.y, 0.05);
}

/* Constructor a partir de un array de coordenadas */

TEST(Point, VectConstructor) 
{
  std::array<int, 2> pt_int{ { 23, 67 } };
  PointI pt_int_c(pt_int);

  EXPECT_EQ(23, pt_int_c.x);
  EXPECT_EQ(67, pt_int_c.y);

  std::array<double, 2> pt_double{ { 253.56, 562.94 } };
  PointD pt_double_c(pt_double);

  EXPECT_NEAR(253.56, pt_double_c.x, 0.01);
  EXPECT_NEAR(562.94, pt_double_c.y, 0.01);

  std::array<float, 2> pt_float{ { 564.26f, 646.65f } };
  PointF pt_float_c(pt_float);

  EXPECT_NEAR(564.26f, pt_float_c.x, 0.05);
  EXPECT_NEAR(646.65f, pt_float_c.y, 0.05);
}

/* Operador de asignación */

TEST(Point, assing_operator)
{
  PointI pt_int(23, 67);
  PointI pt_int_c = pt_int;

  EXPECT_EQ(23, pt_int_c.x);
  EXPECT_EQ(67, pt_int_c.y);

  PointD pt_double(253.56, 562.94);
  PointD pt_double_c = pt_double;

  EXPECT_NEAR(253.56, pt_double_c.x, 0.01);
  EXPECT_NEAR(562.94, pt_double_c.y, 0.01);

  PointF pt_float(564.26f, 646.65f);
  PointF pt_float_c = pt_float;

  EXPECT_NEAR(564.26f, pt_float_c.x, 0.05);
  EXPECT_NEAR(646.65f, pt_float_c.y, 0.05);
}

/* Conversión de tipo */

TEST(Point, conversion)
{
  PointI pt_int(23, 67);
  PointD pt_double = static_cast<PointD>(pt_int);

  EXPECT_EQ(23.0, pt_double.x);
  EXPECT_EQ(67.0, pt_double.y);

  PointF pt_float(564.26f, 646.65f);
  pt_double = static_cast<PointD>(pt_float);

  EXPECT_NEAR(564.26, pt_double.x, 0.05);
  EXPECT_NEAR(646.65, pt_double.y, 0.05);
   
  Point3F pt_float3D(564.26f, 646.65f, 23.32f);
  pt_float = static_cast<PointF>(pt_float3D);
  EXPECT_NEAR(564.26f, pt_float.x, 0.05);
  EXPECT_NEAR(646.65f, pt_float.y, 0.05);

  Point<uint16_t> pt_16 = static_cast<Point<uint16_t>>(pt_int);

  EXPECT_EQ(23, pt_16.x);
  EXPECT_EQ(67, pt_16.y);
}


/* Punto 3D */


/* Constructor por defecto */

TEST(Point3, DefaultConstructor) 
{
  
  Point3I pt_int;

  EXPECT_EQ(0, pt_int.x);
  EXPECT_EQ(0, pt_int.y);
  EXPECT_EQ(0, pt_int.z);

  Point3D pt_double;
    
  EXPECT_EQ(0., pt_double.x);
  EXPECT_EQ(0., pt_double.y);
  EXPECT_EQ(0., pt_double.z);

  Point3F pt_float;
    
  EXPECT_EQ(0.f, pt_float.x);
  EXPECT_EQ(0.f, pt_float.y);
  EXPECT_EQ(0.f, pt_float.z);

  Point3<uint16_t> pt_uint16;
    
  EXPECT_EQ(0, pt_uint16.x);
  EXPECT_EQ(0, pt_uint16.y);
  EXPECT_EQ(0, pt_uint16.z);
}

/* Constructor x y */

TEST(Point3, ConstructorXY)
{
  Point3I pt_int(23, 67, 54);

  EXPECT_EQ(23, pt_int.x);
  EXPECT_EQ(67, pt_int.y);
  EXPECT_EQ(54, pt_int.z);

  Point3D pt_double(253.56, 562.94, 345.89);
    
  EXPECT_NEAR(253.56, pt_double.x, 0.01);
  EXPECT_NEAR(562.94, pt_double.y, 0.01);
  EXPECT_NEAR(345.89, pt_double.z, 0.01);

  Point3F pt_float(564.26f, 646.65f, 45.89f);
    
  EXPECT_NEAR(564.26f, pt_float.x, 0.05);
  EXPECT_NEAR(646.65f, pt_float.y, 0.05);
  EXPECT_NEAR(45.89f, pt_float.z, 0.05);

  Point3<int8_t> pt_int8(0, 4, 1);
    
  EXPECT_EQ(0, pt_int8.x);
  EXPECT_EQ(4, pt_int8.y);
  EXPECT_EQ(1, pt_int8.z);
}

/* Constructor de copia */

TEST(Point3, CopyConstructor) 
{
  Point3I pt_int(23, 67, 23);
  Point3I pt_int_c(pt_int);

  EXPECT_EQ(23, pt_int_c.x);
  EXPECT_EQ(67, pt_int_c.y);
  EXPECT_EQ(23, pt_int_c.z);

  Point3D pt_double(253.56, 562.94, 234.67);
  Point3D pt_double_c(pt_double);

  EXPECT_NEAR(253.56, pt_double_c.x, 0.01);
  EXPECT_NEAR(562.94, pt_double_c.y, 0.01);
  EXPECT_NEAR(234.67, pt_double_c.z, 0.01);

  Point3F pt_float(564.26f, 646.65f, 56.21f);
  Point3F pt_float_c(pt_float);

  EXPECT_NEAR(564.26f, pt_float_c.x, 0.05);
  EXPECT_NEAR(646.65f, pt_float_c.y, 0.05);
  EXPECT_NEAR(56.21f, pt_float_c.z, 0.05);
}

/* Constructor a partir de un vector de coordenadas */

TEST(Point3, VectConstructor) 
{
  std::array<int, 3> pt_int{ { 23, 67, 23 } };
  Point3I pt_int_c(pt_int);

  EXPECT_EQ(23, pt_int_c.x);
  EXPECT_EQ(67, pt_int_c.y);
  EXPECT_EQ(23, pt_int_c.z);

  std::array<double, 3> pt_double{{ 253.56, 562.94, 234.67 }};
  Point3D pt_double_c(pt_double);

  EXPECT_NEAR(253.56, pt_double_c.x, 0.01);
  EXPECT_NEAR(562.94, pt_double_c.y, 0.01);
  EXPECT_NEAR(234.67, pt_double_c.z, 0.01);

  std::array<float, 3> pt_float{ { 564.26f, 646.65f, 56.21f } };
  Point3F pt_float_c(pt_float);

  EXPECT_NEAR(564.26f, pt_float_c.x, 0.05);
  EXPECT_NEAR(646.65f, pt_float_c.y, 0.05);
  EXPECT_NEAR(56.21f, pt_float_c.z, 0.05);
}

/* Operador de asignación */

TEST(Point3, assing_operator)
{
  Point3I pt_int(23, 67, 23);
  Point3I pt_int_c = pt_int;

  EXPECT_EQ(23, pt_int_c.x);
  EXPECT_EQ(67, pt_int_c.y);
  EXPECT_EQ(23, pt_int_c.z);

  Point3D pt_double(253.56, 562.94, 234.67);
  Point3D pt_double_c = pt_double;

  EXPECT_NEAR(253.56, pt_double_c.x, 0.01);
  EXPECT_NEAR(562.94, pt_double_c.y, 0.01);
  EXPECT_NEAR(234.67, pt_double_c.z, 0.01);

  Point3F pt_float(564.26f, 646.65f, 56.21f);
  Point3F pt_float_c = pt_float;

  EXPECT_NEAR(564.26f, pt_float_c.x, 0.05);
  EXPECT_NEAR(646.65f, pt_float_c.y, 0.05);
  EXPECT_NEAR(56.21f, pt_float_c.z, 0.05);
}

/* Conversión de tipo */

TEST(Point3, conversion)
{
  Point3I pt_int(23, 67, 56);
  Point3D pt_double = static_cast<Point3D>(pt_int);

  EXPECT_EQ(23.0, pt_double.x);
  EXPECT_EQ(67.0, pt_double.y);
  EXPECT_EQ(56.0, pt_double.z);

  PointF pt_float(564.26f, 646.65f);
  pt_double = static_cast<Point3D>(pt_float);

  EXPECT_NEAR(564.26, pt_double.x, 0.05);
  EXPECT_NEAR(646.65, pt_double.y, 0.05);
  EXPECT_NEAR(0.0, pt_double.z, 0.05);

}


/* Operaciones entre puntos */

TEST(Point, append)
{
  PointI pt_1(23, 67);
  PointI pt_2(67, 56);

  pt_2 += pt_1;

  EXPECT_EQ(90, pt_2.x);
  EXPECT_EQ(123, pt_2.y);

  PointD pt_1d(23.45, 67.52);
  PointD pt_2d(67.59, 56.39);

  pt_2d += pt_1d;

  EXPECT_NEAR(91.04, pt_2d.x, 0.01);
  EXPECT_NEAR(123.91, pt_2d.y, 0.01);

  // Tipos diferentes
  pt_1 += pt_2d;
  EXPECT_EQ(114, pt_1.x);
  EXPECT_EQ(191, pt_1.y);
}

TEST(Point, sum)
{
  PointI pt_1(23, 67);
  PointI pt_2(67, 56);

  PointI pt_3 = pt_1 + pt_2;

  EXPECT_EQ(90, pt_3.x);
  EXPECT_EQ(123, pt_3.y);

  PointD pt_1d(23.45, 67.52);
  PointD pt_2d(67.59, 56.39);

  PointD pt_3d = pt_1d + pt_2d;

  EXPECT_NEAR(91.04, pt_3d.x, 0.01);
  EXPECT_NEAR(123.91, pt_3d.y, 0.01);

}

TEST(Point, minus)
{
  PointI pt_1(23, 67);
  PointI pt_2(67, 56);

  PointI pt_3 = pt_1 - pt_2;

  EXPECT_EQ(-44, pt_3.x);
  EXPECT_EQ(11, pt_3.y);

  PointD pt_1d(256.26, 2526.36);
  PointD pt_2d(894.66, 651.56);

  PointD pt_3d = pt_1d - pt_2d;

  EXPECT_NEAR(-638.4, pt_3d.x, 0.01);
  EXPECT_NEAR(1874.8, pt_3d.y, 0.01);

}

TEST(Point, subs)
{
  PointI pt_1(23, 67);
  PointI pt_2(67, 56);

  pt_1 -= pt_2;

  EXPECT_EQ(-44, pt_1.x);
  EXPECT_EQ(11, pt_1.y);

  PointD pt_1d(256.26, 2526.36);
  PointD pt_2d(894.66, 651.56);

  pt_1d -= pt_2d;

  EXPECT_NEAR(-638.4, pt_1d.x, 0.01);
  EXPECT_NEAR(1874.8, pt_1d.y, 0.01);

  // Tipos diferentes
  pt_1 -= pt_1d;
  EXPECT_EQ(594, pt_1.x);
  EXPECT_EQ(-1864, pt_1.y);
}

/* Multiplicación de un punto por un escalar */

TEST(Point, multiplication)
{
  PointI pt(23, 67);
  int s = 3;

  pt *= s;

  EXPECT_EQ(69, pt.x);
  EXPECT_EQ(201, pt.y);

  PointD pt_d(256.26, 2526.36);
  double s_d = 23.5;

  pt_d *= s_d;

  EXPECT_NEAR(6022.11, pt_d.x, 0.01);
  EXPECT_NEAR(59369.46, pt_d.y, 0.01);

  PointI pt2(23, 67);
  double s2 = 3.3;

  pt2 *= s2;
  EXPECT_EQ(76, pt2.x);
  EXPECT_EQ(221, pt2.y);

}

/* División de un punto por un escalar */

TEST(Point, division)
{
  PointI pt(23, 67);
  int s = 3;

  pt /= s;

  EXPECT_EQ(8, pt.x);
  EXPECT_EQ(22, pt.y);

  PointD pt_d(256.26, 2526.36);
  double s_d = 23.5;

  pt_d /= s_d;

  EXPECT_NEAR(10.904, pt_d.x, 0.001);
  EXPECT_NEAR(107.505, pt_d.y, 0.001);

  PointI pt2(23, 67);
  double s2 = 3.3;

  pt2 /= s2;
  EXPECT_EQ(7, pt2.x);
  EXPECT_EQ(20, pt2.y);
}


/* Igualdad == */
TEST(Point, equal)
{
  PointD pt1(256.26, 2526.36);
  PointD pt2(256.26, 2526.36);
  PointD pt3(234.323, 234.36);
  EXPECT_TRUE(pt1 == pt2);
  EXPECT_FALSE(pt1 == pt3);
}

/* Diferencia != */

TEST(Point, dif)
{
  PointD pt1(256.26, 2526.36);
  PointD pt2(256.26, 2526.36);
  PointD pt3(234.323, 234.36);
  EXPECT_FALSE(pt1 != pt2);
  EXPECT_TRUE(pt1 != pt3);
}

/* Invertir punto */

TEST(Point, inv)
{
  PointD pt1(256.26, 2526.36);
  PointD pt2(-256.26, -2526.36);
  EXPECT_TRUE(pt2 == -pt1);
}

/* Punto por escalar */

TEST(Point, multiplication1)
{
  PointI pt(23, 67);
  int s = 3;

  PointI pt2 = pt * s;

  EXPECT_EQ(69, pt2.x);
  EXPECT_EQ(201, pt2.y);

  PointD pt_d(256.26, 2526.36);
  double s_d = 23.5;

  PointD pt_d2 = pt_d * s_d;

  EXPECT_NEAR(6022.11, pt_d2.x, 0.01);
  EXPECT_NEAR(59369.46, pt_d2.y, 0.01);

  pt2 = PointI(23, 67) * 3.3;
  EXPECT_EQ(76, pt2.x);
  EXPECT_EQ(221, pt2.y);

}

/* Escalar por punto */

TEST(Point, multiplication2)
{
  PointI pt(23, 67);
  int s = 3;

  PointI pt2 = s * pt;

  EXPECT_EQ(69, pt2.x);
  EXPECT_EQ(201, pt2.y);

  PointD pt_d(256.26, 2526.36);
  double s_d = 23.5;

  PointD pt_d2 = s_d * pt_d;

  EXPECT_NEAR(6022.11, pt_d2.x, 0.01);
  EXPECT_NEAR(59369.46, pt_d2.y, 0.01);


  pt2 =  3.3 * PointI(23, 67);
  EXPECT_EQ(76, pt2.x);
  EXPECT_EQ(221, pt2.y);
}

/* Punto entre escalar */

TEST(Point, division2)
{
  PointI pt(23, 67);
  int s = 3;

  PointI pt_1 = pt / s;

  EXPECT_EQ(8, pt_1.x);
  EXPECT_EQ(22, pt_1.y);

  PointD pt_d(256.26, 2526.36);
  double s_d = 23.5;

  PointD pt_d1 = pt_d / s_d;

  EXPECT_NEAR(10.904, pt_d1.x, 0.001);
  EXPECT_NEAR(107.505, pt_d1.y, 0.001);

  PointI pt2(23, 67);
  double s2 = 3.3;

  PointI pt2_1 = pt2 / s2;
  EXPECT_EQ(7, pt2_1.x);
  EXPECT_EQ(20, pt2_1.y);
}