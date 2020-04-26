#define BOOST_TEST_MODULE Tidop Point test
#include <boost/test/unit_test.hpp>
#include <tidop/geometry/entities/point.h>
#include <tidop/geometry/entities/window.h>
#include <tidop/geometry/entities/bbox.h>
#include <tidop/core/utils.h>
#include <tidop/math/algebra/vector.h>

using namespace tl;
using namespace tl::geometry;

/* Constructor por defecto */

BOOST_AUTO_TEST_CASE(PointI_default_constructor) 
{
  
  PointI pt_int;

  BOOST_CHECK_EQUAL(0, pt_int.x);
  BOOST_CHECK_EQUAL(0, pt_int.y);
  BOOST_CHECK(pt_int.type() == Entity::Type::point2d);
  BOOST_CHECK(false == pt_int.is3D());
}

BOOST_AUTO_TEST_CASE(PointD_default_constructor) 
{
  PointD pt_double;
    
  BOOST_CHECK_EQUAL(0., pt_double.x);
  BOOST_CHECK_EQUAL(0., pt_double.y);
}

BOOST_AUTO_TEST_CASE(PointF_default_constructor) 
{
  PointF pt_float;
    
  BOOST_CHECK_EQUAL(0.f, pt_float.x);
  BOOST_CHECK_EQUAL(0.f, pt_float.y);
}

BOOST_AUTO_TEST_CASE(Point_uint16_default_constructor) 
{
  Point<uint16_t> pt_uint16;
    
  BOOST_CHECK_EQUAL(0, pt_uint16.x);
  BOOST_CHECK_EQUAL(0, pt_uint16.y);
}

/* Constructor x y */

BOOST_AUTO_TEST_CASE(Point_constructor_xy)
{
  PointI pt_int(23, 67);

  BOOST_CHECK_EQUAL(23, pt_int.x);
  BOOST_CHECK_EQUAL(67, pt_int.y);

  PointD pt_double(253.56, 562.94);
    
  BOOST_CHECK_CLOSE(253.56, pt_double.x, 0.01);
  BOOST_CHECK_CLOSE(562.94, pt_double.y, 0.01);

  PointF pt_float(564.26f, 646.65f);
    
  BOOST_CHECK_CLOSE(564.26f, pt_float.x, 0.05);
  BOOST_CHECK_CLOSE(646.65f, pt_float.y, 0.05);

  Point<int8_t> pt_int8(0, 4);
    
  BOOST_CHECK_EQUAL(0, pt_int8.x);
  BOOST_CHECK_EQUAL(4, pt_int8.y);
}

/* Constructor de copia */

BOOST_AUTO_TEST_CASE(Point_copy_constructor) 
{
  PointI pt_int(23, 67);
  PointI pt_int_c(pt_int);

  BOOST_CHECK_EQUAL(23, pt_int_c.x);
  BOOST_CHECK_EQUAL(67, pt_int_c.y);

  PointD pt_double(253.56, 562.94);
  PointD pt_double_c(pt_double);

  BOOST_CHECK_CLOSE(253.56, pt_double_c.x, 0.01);
  BOOST_CHECK_CLOSE(562.94, pt_double_c.y, 0.01);

  PointF pt_float(564.26f, 646.65f);
  PointF pt_float_c(pt_float);

  BOOST_CHECK_CLOSE(564.26f, pt_float_c.x, 0.05);
  BOOST_CHECK_CLOSE(646.65f, pt_float_c.y, 0.05);
}

/* Constructor a partir de un vector */

BOOST_AUTO_TEST_CASE(Point_constructor_vector)
{
  math::Vector2i pt_int{{23, 67}};
  PointI pt_int_c(pt_int);

  BOOST_CHECK_EQUAL(23, pt_int_c.x);
  BOOST_CHECK_EQUAL(67, pt_int_c.y);

  math::Vector2d pt_double{{253.56, 562.94}};
  PointD pt_double_c(pt_double);

  BOOST_CHECK_CLOSE(253.56, pt_double_c.x, 0.01);
  BOOST_CHECK_CLOSE(562.94, pt_double_c.y, 0.01);

  math::Vector2f pt_float{{564.26f, 646.65f}};
  PointF pt_float_c(pt_float);

  BOOST_CHECK_CLOSE(564.26f, pt_float_c.x, 0.05);
  BOOST_CHECK_CLOSE(646.65f, pt_float_c.y, 0.05);
}


/* Constructor a partir de un array de coordenadas */

BOOST_AUTO_TEST_CASE(Point_constructor_array) 
{
  std::array<int, 2> pt_int{ { 23, 67 } };
  PointI pt_int_c(pt_int);

  BOOST_CHECK_EQUAL(23, pt_int_c.x);
  BOOST_CHECK_EQUAL(67, pt_int_c.y);

  std::array<double, 2> pt_double{ { 253.56, 562.94 } };
  PointD pt_double_c(pt_double);

  BOOST_CHECK_CLOSE(253.56, pt_double_c.x, 0.01);
  BOOST_CHECK_CLOSE(562.94, pt_double_c.y, 0.01);

  std::array<float, 2> pt_float{ { 564.26f, 646.65f } };
  PointF pt_float_c(pt_float);

  BOOST_CHECK_CLOSE(564.26f, pt_float_c.x, 0.05);
  BOOST_CHECK_CLOSE(646.65f, pt_float_c.y, 0.05);
}

/* Operador de asignación */

BOOST_AUTO_TEST_CASE(Point_assing_operator)
{
  PointI pt_int(23, 67);
  PointI pt_int_c = pt_int;

  BOOST_CHECK_EQUAL(23, pt_int_c.x);
  BOOST_CHECK_EQUAL(67, pt_int_c.y);

  PointD pt_double(253.56, 562.94);
  PointD pt_double_c = pt_double;

  BOOST_CHECK_CLOSE(253.56, pt_double_c.x, 0.01);
  BOOST_CHECK_CLOSE(562.94, pt_double_c.y, 0.01);

  PointF pt_float(564.26f, 646.65f);
  PointF pt_float_c = pt_float;

  BOOST_CHECK_CLOSE(564.26f, pt_float_c.x, 0.05);
  BOOST_CHECK_CLOSE(646.65f, pt_float_c.y, 0.05);
}

/* Conversión de tipo */

BOOST_AUTO_TEST_CASE(Point_conversion)
{
  PointI pt_int(23, 67);
  PointD pt_double = static_cast<PointD>(pt_int);

  BOOST_CHECK_EQUAL(23.0, pt_double.x);
  BOOST_CHECK_EQUAL(67.0, pt_double.y);

  PointF pt_float(564.26f, 646.65f);
  pt_double = static_cast<PointD>(pt_float);

  BOOST_CHECK_CLOSE(564.26, pt_double.x, 0.05);
  BOOST_CHECK_CLOSE(646.65, pt_double.y, 0.05);
   
  Point3F pt_float3D(564.26f, 646.65f, 23.32f);
  pt_float = static_cast<PointF>(pt_float3D);
  BOOST_CHECK_CLOSE(564.26f, pt_float.x, 0.05);
  BOOST_CHECK_CLOSE(646.65f, pt_float.y, 0.05);

  Point<uint16_t> pt_16 = static_cast<Point<uint16_t>>(pt_int);

  BOOST_CHECK_EQUAL(23, pt_16.x);
  BOOST_CHECK_EQUAL(67, pt_16.y);
}


/* Punto 3D */


/* Constructor por defecto */

BOOST_AUTO_TEST_CASE(Point3_default_constructor) 
{
  
  Point3I pt_int;

  BOOST_CHECK_EQUAL(0, pt_int.x);
  BOOST_CHECK_EQUAL(0, pt_int.y);
  BOOST_CHECK_EQUAL(0, pt_int.z);

  BOOST_CHECK(pt_int.type() == Entity::Type::point3d);
  BOOST_CHECK(pt_int.is3D());

  Point3D pt_double;
    
  BOOST_CHECK_EQUAL(0., pt_double.x);
  BOOST_CHECK_EQUAL(0., pt_double.y);
  BOOST_CHECK_EQUAL(0., pt_double.z);

  Point3F pt_float;
    
  BOOST_CHECK_EQUAL(0.f, pt_float.x);
  BOOST_CHECK_EQUAL(0.f, pt_float.y);
  BOOST_CHECK_EQUAL(0.f, pt_float.z);

  Point3<uint16_t> pt_uint16;
    
  BOOST_CHECK_EQUAL(0, pt_uint16.x);
  BOOST_CHECK_EQUAL(0, pt_uint16.y);
  BOOST_CHECK_EQUAL(0, pt_uint16.z);
}

/* Constructor x y */

BOOST_AUTO_TEST_CASE(Point3_constructor_xy)
{
  Point3I pt_int(23, 67, 54);

  BOOST_CHECK_EQUAL(23, pt_int.x);
  BOOST_CHECK_EQUAL(67, pt_int.y);
  BOOST_CHECK_EQUAL(54, pt_int.z);

  Point3D pt_double(253.56, 562.94, 345.89);
    
  BOOST_CHECK_CLOSE(253.56, pt_double.x, 0.01);
  BOOST_CHECK_CLOSE(562.94, pt_double.y, 0.01);
  BOOST_CHECK_CLOSE(345.89, pt_double.z, 0.01);

  Point3F pt_float(564.26f, 646.65f, 45.89f);
    
  BOOST_CHECK_CLOSE(564.26f, pt_float.x, 0.05);
  BOOST_CHECK_CLOSE(646.65f, pt_float.y, 0.05);
  BOOST_CHECK_CLOSE(45.89f, pt_float.z, 0.05);

  Point3<int8_t> pt_int8(0, 4, 1);
    
  BOOST_CHECK_EQUAL(0, pt_int8.x);
  BOOST_CHECK_EQUAL(4, pt_int8.y);
  BOOST_CHECK_EQUAL(1, pt_int8.z);
}

/* Constructor de copia */

BOOST_AUTO_TEST_CASE(Point3_copy_constructor) 
{
  Point3I pt_int(23, 67, 23);
  Point3I pt_int_c(pt_int);

  BOOST_CHECK_EQUAL(23, pt_int_c.x);
  BOOST_CHECK_EQUAL(67, pt_int_c.y);
  BOOST_CHECK_EQUAL(23, pt_int_c.z);

  Point3D pt_double(253.56, 562.94, 234.67);
  Point3D pt_double_c(pt_double);

  BOOST_CHECK_CLOSE(253.56, pt_double_c.x, 0.01);
  BOOST_CHECK_CLOSE(562.94, pt_double_c.y, 0.01);
  BOOST_CHECK_CLOSE(234.67, pt_double_c.z, 0.01);

  Point3F pt_float(564.26f, 646.65f, 56.21f);
  Point3F pt_float_c(pt_float);

  BOOST_CHECK_CLOSE(564.26f, pt_float_c.x, 0.05);
  BOOST_CHECK_CLOSE(646.65f, pt_float_c.y, 0.05);
  BOOST_CHECK_CLOSE(56.21f, pt_float_c.z, 0.05);
}

/* Constructor a partir de un vector */

BOOST_AUTO_TEST_CASE(Point3_vector_constructor)
{
  math::Vector3i pt_int{{23, 67, 23}};
  Point3I pt_int_c(pt_int);

  BOOST_CHECK_EQUAL(23, pt_int_c.x);
  BOOST_CHECK_EQUAL(67, pt_int_c.y);
  BOOST_CHECK_EQUAL(23, pt_int_c.z);

  math::Vector3d pt_double{{253.56, 562.94, 234.67}};
  Point3D pt_double_c(pt_double);

  BOOST_CHECK_CLOSE(253.56, pt_double_c.x, 0.01);
  BOOST_CHECK_CLOSE(562.94, pt_double_c.y, 0.01);
  BOOST_CHECK_CLOSE(234.67, pt_double_c.z, 0.01);

  math::Vector3f pt_float{{564.26f, 646.65f, 56.21f}};
  Point3F pt_float_c(pt_float);

  BOOST_CHECK_CLOSE(564.26f, pt_float_c.x, 0.05);
  BOOST_CHECK_CLOSE(646.65f, pt_float_c.y, 0.05);
  BOOST_CHECK_CLOSE(56.21f, pt_float_c.z, 0.05);
}

/* Constructor a partir de un array */

BOOST_AUTO_TEST_CASE(Point3_array_constructor)
{
  std::array<int, 3> pt_int{ { 23, 67, 23 } };
  Point3I pt_int_c(pt_int);

  BOOST_CHECK_EQUAL(23, pt_int_c.x);
  BOOST_CHECK_EQUAL(67, pt_int_c.y);
  BOOST_CHECK_EQUAL(23, pt_int_c.z);

  std::array<double, 3> pt_double{{ 253.56, 562.94, 234.67 }};
  Point3D pt_double_c(pt_double);

  BOOST_CHECK_CLOSE(253.56, pt_double_c.x, 0.01);
  BOOST_CHECK_CLOSE(562.94, pt_double_c.y, 0.01);
  BOOST_CHECK_CLOSE(234.67, pt_double_c.z, 0.01);

  std::array<float, 3> pt_float{ { 564.26f, 646.65f, 56.21f } };
  Point3F pt_float_c(pt_float);

  BOOST_CHECK_CLOSE(564.26f, pt_float_c.x, 0.05);
  BOOST_CHECK_CLOSE(646.65f, pt_float_c.y, 0.05);
  BOOST_CHECK_CLOSE(56.21f, pt_float_c.z, 0.05);
}

/* Operador de asignación */

BOOST_AUTO_TEST_CASE(Point3_assing_operator)
{
  Point3I pt_int(23, 67, 23);
  Point3I pt_int_c = pt_int;

  BOOST_CHECK_EQUAL(23, pt_int_c.x);
  BOOST_CHECK_EQUAL(67, pt_int_c.y);
  BOOST_CHECK_EQUAL(23, pt_int_c.z);

  Point3D pt_double(253.56, 562.94, 234.67);
  Point3D pt_double_c = pt_double;

  BOOST_CHECK_CLOSE(253.56, pt_double_c.x, 0.01);
  BOOST_CHECK_CLOSE(562.94, pt_double_c.y, 0.01);
  BOOST_CHECK_CLOSE(234.67, pt_double_c.z, 0.01);

  Point3F pt_float(564.26f, 646.65f, 56.21f);
  Point3F pt_float_c = pt_float;

  BOOST_CHECK_CLOSE(564.26f, pt_float_c.x, 0.05);
  BOOST_CHECK_CLOSE(646.65f, pt_float_c.y, 0.05);
  BOOST_CHECK_CLOSE(56.21f, pt_float_c.z, 0.05);
}

/* Conversión de tipo */

BOOST_AUTO_TEST_CASE(Point3_conversion)
{
  Point3I pt_int(23, 67, 56);
  Point3D pt_double = static_cast<Point3D>(pt_int);

  BOOST_CHECK_EQUAL(23.0, pt_double.x);
  BOOST_CHECK_EQUAL(67.0, pt_double.y);
  BOOST_CHECK_EQUAL(56.0, pt_double.z);

  PointF pt_float(564.26f, 646.65f);
  pt_double = static_cast<Point3D>(pt_float);

  BOOST_CHECK_CLOSE(564.26, pt_double.x, 0.05);
  BOOST_CHECK_CLOSE(646.65, pt_double.y, 0.05);
  BOOST_CHECK_CLOSE(0.0, pt_double.z, 0.05);

}


/* Operaciones entre puntos */

BOOST_AUTO_TEST_CASE(Point_append)
{
  PointI pt_1(23, 67);
  PointI pt_2(67, 56);

  pt_2 += pt_1;

  BOOST_CHECK_EQUAL(90, pt_2.x);
  BOOST_CHECK_EQUAL(123, pt_2.y);

  PointD pt_1d(23.45, 67.52);
  PointD pt_2d(67.59, 56.39);

  pt_2d += pt_1d;

  BOOST_CHECK_CLOSE(91.04, pt_2d.x, 0.01);
  BOOST_CHECK_CLOSE(123.91, pt_2d.y, 0.01);

  // Tipos diferentes
  pt_1 += pt_2d;
  BOOST_CHECK_EQUAL(114, pt_1.x);
  BOOST_CHECK_EQUAL(191, pt_1.y);
}

BOOST_AUTO_TEST_CASE(Point_addition)
{
  PointI pt_1(23, 67);
  PointI pt_2(67, 56);

  PointI pt_3 = pt_1 + pt_2;

  BOOST_CHECK_EQUAL(90, pt_3.x);
  BOOST_CHECK_EQUAL(123, pt_3.y);

  PointD pt_1d(23.45, 67.52);
  PointD pt_2d(67.59, 56.39);

  PointD pt_3d = pt_1d + pt_2d;

  BOOST_CHECK_CLOSE(91.04, pt_3d.x, 0.01);
  BOOST_CHECK_CLOSE(123.91, pt_3d.y, 0.01);

}

BOOST_AUTO_TEST_CASE(Point_substraction)
{
  PointI pt_1(23, 67);
  PointI pt_2(67, 56);

  PointI pt_3 = pt_1 - pt_2;

  BOOST_CHECK_EQUAL(-44, pt_3.x);
  BOOST_CHECK_EQUAL(11, pt_3.y);

  PointD pt_1d(256.26, 2526.36);
  PointD pt_2d(894.66, 651.56);

  PointD pt_3d = pt_1d - pt_2d;

  BOOST_CHECK_CLOSE(-638.4, pt_3d.x, 0.01);
  BOOST_CHECK_CLOSE(1874.8, pt_3d.y, 0.01);

}

BOOST_AUTO_TEST_CASE(Point_substraction2)
{
  PointI pt_1(23, 67);
  PointI pt_2(67, 56);

  pt_1 -= pt_2;

  BOOST_CHECK_EQUAL(-44, pt_1.x);
  BOOST_CHECK_EQUAL(11, pt_1.y);

  PointD pt_1d(256.26, 2526.36);
  PointD pt_2d(894.66, 651.56);

  pt_1d -= pt_2d;

  BOOST_CHECK_CLOSE(-638.4, pt_1d.x, 0.01);
  BOOST_CHECK_CLOSE(1874.8, pt_1d.y, 0.01);

  // Tipos diferentes
  pt_1 -= pt_1d;
  BOOST_CHECK_EQUAL(594, pt_1.x);
  BOOST_CHECK_EQUAL(-1864, pt_1.y);
}

/* Multiplicación de un punto por un escalar */

BOOST_AUTO_TEST_CASE(Point_multiplication)
{
  PointI pt(23, 67);
  int s = 3;

  pt *= s;

  BOOST_CHECK_EQUAL(69, pt.x);
  BOOST_CHECK_EQUAL(201, pt.y);

  PointD pt_d(256.26, 2526.36);
  double s_d = 23.5;

  pt_d *= s_d;

  BOOST_CHECK_CLOSE(6022.11, pt_d.x, 0.01);
  BOOST_CHECK_CLOSE(59369.46, pt_d.y, 0.01);

  PointI pt2(23, 67);
  double s2 = 3.3;

  pt2 *= s2;
  BOOST_CHECK_EQUAL(76, pt2.x);
  BOOST_CHECK_EQUAL(221, pt2.y);

}

/* División de un punto por un escalar */

BOOST_AUTO_TEST_CASE(Point_division)
{
  PointI pt(23, 67);
  int s = 3;

  pt /= s;

  BOOST_CHECK_EQUAL(8, pt.x);
  BOOST_CHECK_EQUAL(22, pt.y);

  PointD pt_d(256.26, 2526.36);
  double s_d = 23.5;

  pt_d /= s_d;

  BOOST_CHECK_CLOSE(10.905, pt_d.x, 0.01);
  BOOST_CHECK_CLOSE(107.505, pt_d.y, 0.01);

  PointI pt2(23, 67);
  double s2 = 3.3;

  pt2 /= s2;
  BOOST_CHECK_EQUAL(7, pt2.x);
  BOOST_CHECK_EQUAL(20, pt2.y);
}


/* Igualdad == */
BOOST_AUTO_TEST_CASE(Point_equal)
{
  PointD pt1(256.26, 2526.36);
  PointD pt2(256.26, 2526.36);
  PointD pt3(234.323, 234.36);
  BOOST_CHECK(pt1 == pt2);
  BOOST_CHECK(pt1 != pt3);
}

/* Diferencia != */

BOOST_AUTO_TEST_CASE(Point_difference)
{
  PointD pt1(256.26, 2526.36);
  PointD pt2(256.26, 2526.36);
  PointD pt3(234.323, 234.36);
  BOOST_CHECK(pt1 == pt2);
  BOOST_CHECK(pt1 != pt3);
}

/* Invertir punto */

BOOST_AUTO_TEST_CASE(Point_invert)
{
  PointD pt1(256.26, 2526.36);
  PointD pt2(-256.26, -2526.36);
  BOOST_CHECK(pt2 == -pt1);
}

/* Punto por escalar */

BOOST_AUTO_TEST_CASE(Point_multiplication1)
{
  PointI pt(23, 67);
  int s = 3;

  PointI pt2 = pt * s;

  BOOST_CHECK_EQUAL(69, pt2.x);
  BOOST_CHECK_EQUAL(201, pt2.y);

  PointD pt_d(256.26, 2526.36);
  double s_d = 23.5;

  PointD pt_d2 = pt_d * s_d;

  BOOST_CHECK_CLOSE(6022.11, pt_d2.x, 0.01);
  BOOST_CHECK_CLOSE(59369.46, pt_d2.y, 0.01);

  pt2 = PointI(23, 67) * 3.3;
  BOOST_CHECK_EQUAL(76, pt2.x);
  BOOST_CHECK_EQUAL(221, pt2.y);

}

/* Escalar por punto */

BOOST_AUTO_TEST_CASE(Point_multiplication2)
{
  PointI pt(23, 67);
  int s = 3;

  PointI pt2 = s * pt;

  BOOST_CHECK_EQUAL(69, pt2.x);
  BOOST_CHECK_EQUAL(201, pt2.y);

  PointD pt_d(256.26, 2526.36);
  double s_d = 23.5;

  PointD pt_d2 = s_d * pt_d;

  BOOST_CHECK_CLOSE(6022.11, pt_d2.x, 0.01);
  BOOST_CHECK_CLOSE(59369.46, pt_d2.y, 0.01);


  pt2 =  3.3 * PointI(23, 67);
  BOOST_CHECK_EQUAL(76, pt2.x);
  BOOST_CHECK_EQUAL(221, pt2.y);
}

/* Punto entre escalar */

BOOST_AUTO_TEST_CASE(Point_division2)
{
  PointI pt(23, 67);
  int s = 3;

  PointI pt_1 = pt / s;

  BOOST_CHECK_EQUAL(8, pt_1.x);
  BOOST_CHECK_EQUAL(22, pt_1.y);

  PointD pt_d(256.26, 2526.36);
  double s_d = 23.5;

  PointD pt_d1 = pt_d / s_d;

  BOOST_CHECK_CLOSE(10.905, pt_d1.x, 0.01);
  BOOST_CHECK_CLOSE(107.505, pt_d1.y, 0.001);

  PointI pt2(23, 67);
  double s2 = 3.3;

  PointI pt2_1 = pt2 / s2;
  BOOST_CHECK_EQUAL(7, pt2_1.x);
  BOOST_CHECK_EQUAL(20, pt2_1.y);
}

/* MultiPoint */

/* Constructor por defecto */

BOOST_AUTO_TEST_CASE(MultiPoint_default_constructor) 
{
  
  MultiPointI multiPointI;

  BOOST_CHECK_EQUAL(0, multiPointI.size());
  BOOST_CHECK(multiPointI.type() == Entity::Type::multipoint2d);
  BOOST_CHECK(false == multiPointI.is3D());

  MultiPointD multiPointD;
  WindowD w = multiPointD.window();

  BOOST_CHECK_EQUAL(0, multiPointD.size());
  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, w.pt1.x);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, w.pt1.y);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, w.pt2.x);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, w.pt2.y);

  MultiPointF multiPointF;
    
  BOOST_CHECK_EQUAL(0, multiPointF.size());

  MultiPoint<uint16_t> multiPoint16;
    
  BOOST_CHECK_EQUAL(0, multiPoint16.size());
  
}

/* Constructor reserve */

BOOST_AUTO_TEST_CASE(MultiPoint_constructor_reserve)
{
  MultiPointD multiPointD(10);
  WindowD w = multiPointD.window();

  BOOST_CHECK_EQUAL(10, multiPointD.size());
  BOOST_CHECK(multiPointD.type() == Entity::Type::multipoint2d);
  BOOST_CHECK_EQUAL(0., w.pt1.x);
  BOOST_CHECK_EQUAL(0., w.pt1.y);
  BOOST_CHECK_EQUAL(0., w.pt2.x);
  BOOST_CHECK_EQUAL(0., w.pt2.y);

}

/* Constructor de copia */

BOOST_AUTO_TEST_CASE(MultiPoint_copy_constructor) 
{
  MultiPointD multiPointD;
  multiPointD.push_back(PointD(23.6, 94.4));
  multiPointD.push_back(PointD(75.36, 246.33));
  multiPointD.push_back(PointD(256.6, 619.3));
  multiPointD.push_back(PointD(62.36, 6.60));

  BOOST_CHECK(multiPointD.type() == Entity::Type::multipoint2d);
  BOOST_CHECK_EQUAL(4, multiPointD.size());

  MultiPointD multiPointD_c(multiPointD);

  BOOST_CHECK(multiPointD_c.type() == Entity::Type::multipoint2d);
  BOOST_CHECK_EQUAL(4, multiPointD_c.size());

  for (int i = 0; i < multiPointD_c.size(); i++) {
    BOOST_CHECK(multiPointD_c[i] == multiPointD[i]);
  }
}

/* Constructor a partir de un vector de puntos */

BOOST_AUTO_TEST_CASE(MultiPoint_vector) 
{
  std::vector<PointD> multiPointD{ PointD(23.6, 94.4),
                                   PointD(75.36, 246.33),
                                   PointD(256.6, 619.3),
                                   PointD(62.36, 6.60) };

  MultiPointD multiPointD_c(multiPointD);

  BOOST_CHECK(multiPointD_c.type() == Entity::Type::multipoint2d);
  BOOST_CHECK_EQUAL(4, multiPointD_c.size());

  for (int i = 0; i < multiPointD_c.size(); i++) {
    BOOST_CHECK(multiPointD_c[i] == multiPointD[i]);
  }
}

/* Constructor lista de inicializadores */

BOOST_AUTO_TEST_CASE(MultiPoint_initializer_list) 
{
  MultiPointD multiPointD{ PointD(23.6, 94.4),
                           PointD(75.36, 246.33),
                           PointD(256.6, 619.3),
                           PointD(62.36, 6.60) };
  BOOST_CHECK(multiPointD.type() == Entity::Type::multipoint2d);
  BOOST_CHECK_EQUAL(4, multiPointD.size());

}


/* Operador de asignación */

BOOST_AUTO_TEST_CASE(MultiPoint_assing_operator)
{
  MultiPointD multiPointD{ PointD(23.6, 94.4),
                           PointD(75.36, 246.33),
                           PointD(256.6, 619.3),
                           PointD(62.36, 6.60) };

  MultiPointD multiPointD_c = multiPointD;

  BOOST_CHECK(multiPointD_c.type() == Entity::Type::multipoint2d);
  BOOST_CHECK_EQUAL(4, multiPointD_c.size());

  for (size_t i = 0; i < multiPointD_c.size(); i++) {
    BOOST_CHECK(multiPointD_c[i] == multiPointD[i]);
  }
}

BOOST_AUTO_TEST_CASE(MultiPoint_window)
{
  MultiPointD multiPointD {PointD(23.6, 94.4),
                           PointD(75.36, 246.33),
                           PointD(256.6, 619.3),
                           PointD(62.36, 6.60)};

  WindowD window = multiPointD.window();
  BOOST_CHECK_EQUAL(23.6, window.pt1.x);
  BOOST_CHECK_EQUAL(6.60, window.pt1.y);
  BOOST_CHECK_EQUAL(256.6, window.pt2.x);
  BOOST_CHECK_EQUAL(619.3, window.pt2.y);
}

/* MultiPoint3D */

/* Constructor por defecto */

BOOST_AUTO_TEST_CASE(MultiPoint3D_default_constructor) 
{
  
  MultiPoint3dI multiPointI;

  BOOST_CHECK_EQUAL(0, multiPointI.size());
  BOOST_CHECK(multiPointI.type() == Entity::Type::multipoint3d);
  BOOST_CHECK(multiPointI.is3D());

  MultiPoint3dD multiPointD;
  BoundingBoxD box = multiPointD.boundingBox();

  BOOST_CHECK_EQUAL(0, multiPointD.size());
  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, box.pt1.x);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, box.pt1.y);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, box.pt1.z);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, box.pt2.x);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, box.pt2.y);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, box.pt2.z);

  MultiPoint3dF multiPointF;
    
  BOOST_CHECK_EQUAL(0, multiPointF.size());

  MultiPoint3D<uint16_t> multiPoint16;
    
  BOOST_CHECK_EQUAL(0, multiPoint16.size());
  
}

/* Constructor reserve */

BOOST_AUTO_TEST_CASE(MultiPoint3D_constructor_reserve)
{
  MultiPoint3dD multiPointD(10);
  BoundingBoxD box = multiPointD.boundingBox();

  BOOST_CHECK_EQUAL(10, multiPointD.size());
  BOOST_CHECK(multiPointD.type() == Entity::Type::multipoint3d);
  BOOST_CHECK_EQUAL(0., box.pt1.x);
  BOOST_CHECK_EQUAL(0., box.pt1.y);
  BOOST_CHECK_EQUAL(0., box.pt1.z);
  BOOST_CHECK_EQUAL(0., box.pt2.x);
  BOOST_CHECK_EQUAL(0., box.pt2.y);
  BOOST_CHECK_EQUAL(0., box.pt2.z);

}

/* Constructor de copia */

BOOST_AUTO_TEST_CASE(MultiPoint3D_copy_constructor) 
{
  MultiPoint3dD multiPointD;
  multiPointD.push_back(Point3D(23.6, 94.4, 2.3));
  multiPointD.push_back(Point3D(75.36, 246.33, 36.36));
  multiPointD.push_back(Point3D(256.6, 619.3, 56.12));
  multiPointD.push_back(Point3D(62.36, 6.60, 24.63));

  BOOST_CHECK(multiPointD.type() == Entity::Type::multipoint3d);
  BOOST_CHECK_EQUAL(4, multiPointD.size());

  MultiPoint3dD multiPointD_c(multiPointD);
  BoundingBoxD box = multiPointD_c.boundingBox();
  BOOST_CHECK_EQUAL(23.6, box.pt1.x);
  BOOST_CHECK_EQUAL(6.60, box.pt1.y);
  BOOST_CHECK_EQUAL(2.3, box.pt1.z);
  BOOST_CHECK_EQUAL(256.6, box.pt2.x);
  BOOST_CHECK_EQUAL(619.3, box.pt2.y);
  BOOST_CHECK_EQUAL(56.12, box.pt2.z);

  BOOST_CHECK(multiPointD_c.type() == Entity::Type::multipoint3d);
  BOOST_CHECK_EQUAL(4, multiPointD_c.size());

  for (size_t i = 0; i < multiPointD_c.size(); i++) {
    BOOST_CHECK(multiPointD_c[i] == multiPointD[i]);
  }
}

/* Constructor a partir de un vector de puntos */

BOOST_AUTO_TEST_CASE(MultiPoint3D_vector) 
{
  std::vector<Point3D> multiPointD{ Point3D(23.6, 94.4, 0.36),
                                    Point3D(75.36, 246.33, 454.3),
                                    Point3D(256.6, 619.3, 26.21),
                                    Point3D(62.36, 6.60, 62.61) };

  MultiPoint3dD multiPointD_c(multiPointD);

  BOOST_CHECK(multiPointD_c.type() == Entity::Type::multipoint3d);
  BOOST_CHECK_EQUAL(4, multiPointD_c.size());

  for (size_t i = 0; i < multiPointD_c.size(); i++) {
    BOOST_CHECK(multiPointD_c[i] == multiPointD[i]);
  }
}

/* Constructor lista de inicializadores */

BOOST_AUTO_TEST_CASE(MultiPoint3D_initializer_list) 
{
  MultiPoint3dD multiPointD{ Point3D(23.6, 94.4, 0.36),
                             Point3D(75.36, 246.33, 454.3),
                             Point3D(256.6, 619.3, 26.21),
                             Point3D(62.36, 6.60, 62.61) };
  BOOST_CHECK(multiPointD.type() == Entity::Type::multipoint3d);
  BOOST_CHECK_EQUAL(4, multiPointD.size());

}


/* Operador de asignación */

BOOST_AUTO_TEST_CASE(MultiPoint3D_assing_operator)
{
  MultiPoint3dD multiPointD{ Point3D(23.6, 94.4, 0.36),
                             Point3D(75.36, 246.33, 454.3),
                             Point3D(256.6, 619.3, 26.21),
                             Point3D(62.36, 6.60, 62.61) };

  MultiPoint3dD multiPointD_c = multiPointD;

  BOOST_CHECK(multiPointD_c.type() == Entity::Type::multipoint3d);
  BOOST_CHECK_EQUAL(4, multiPointD_c.size());

  for (size_t i = 0; i < multiPointD_c.size(); i++) {
    BOOST_CHECK(multiPointD_c[i] == multiPointD[i]);
  }
}

BOOST_AUTO_TEST_CASE(MultiPoint3D_box) 
{
  std::vector<Point3D> multiPointD{ Point3D(23.6, 94.4, 0.36),
                                    Point3D(75.36, 246.33, 454.3),
                                    Point3D(256.6, 619.3, 26.21),
                                    Point3D(62.36, 6.60, 62.61) };

  MultiPoint3dD multiPointD_c(multiPointD);
  BoundingBoxD box = multiPointD_c.boundingBox();
  BOOST_CHECK_EQUAL(23.6, box.pt1.x);
  BOOST_CHECK_EQUAL(6.60, box.pt1.y);
  BOOST_CHECK_EQUAL(0.36, box.pt1.z);
  BOOST_CHECK_EQUAL(256.6, box.pt2.x);
  BOOST_CHECK_EQUAL(619.3, box.pt2.y);
  BOOST_CHECK_EQUAL(454.3, box.pt2.z);
}