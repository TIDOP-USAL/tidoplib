#define BOOST_TEST_MODULE Tidop bounding box test
#include <boost/test/unit_test.hpp>
#include <tidop/geometry/entities/bbox.h>

using namespace tl;

// Constructor por defecto

BOOST_AUTO_TEST_CASE(BoundingBoxI_defaultConstructor)
{
  const BoundingBoxI box;
  BOOST_CHECK_EQUAL(TL_INT_MAX, box.pt1.x);
  BOOST_CHECK_EQUAL(TL_INT_MAX, box.pt1.y);
  BOOST_CHECK_EQUAL(TL_INT_MAX, box.pt1.z);
  BOOST_CHECK_EQUAL(TL_INT_MIN, box.pt2.x);
  BOOST_CHECK_EQUAL(TL_INT_MIN, box.pt2.y);
  BOOST_CHECK_EQUAL(TL_INT_MIN, box.pt2.z);
  BOOST_CHECK(box.is3D());
}

BOOST_AUTO_TEST_CASE(BoundingBoxD_defaultConstructor)
{
  const BoundingBoxD box;
  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, box.pt1.x);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, box.pt1.y);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, box.pt1.z);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, box.pt2.x);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, box.pt2.y);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, box.pt2.z);
}

BOOST_AUTO_TEST_CASE(BoundingBoxF_defaultConstructor)
{
  const BoundingBoxF box;
  BOOST_CHECK_EQUAL(TL_FLOAT_MAX, box.pt1.x);
  BOOST_CHECK_EQUAL(TL_FLOAT_MAX, box.pt1.y);
  BOOST_CHECK_EQUAL(TL_FLOAT_MAX, box.pt1.z);
  BOOST_CHECK_EQUAL(TL_FLOAT_MIN, box.pt2.x);
  BOOST_CHECK_EQUAL(TL_FLOAT_MIN, box.pt2.y);
  BOOST_CHECK_EQUAL(TL_FLOAT_MIN, box.pt2.z);
}

// Constructor de copia

BOOST_AUTO_TEST_CASE(BoundingBoxI_copy_constructor) {
  const BoundingBoxI box(Point3I(0, 0, 0), Point3I(100, 100, 100));
  BoundingBoxI box2(box);
  BOOST_CHECK_EQUAL(box.pt1.x, box2.pt1.x);
  BOOST_CHECK_EQUAL(box.pt1.y, box2.pt1.y);
  BOOST_CHECK_EQUAL(box.pt1.z, box2.pt1.z);
  BOOST_CHECK(box.pt2 == box2.pt2);
}

BOOST_AUTO_TEST_CASE(BoundingBoxF_copy_constructor)
{
  const BoundingBoxF box(Point3F(0.f, 0.f, 0.f), Point3F(100.f, 100.f, 100.f));
  BoundingBoxF box2(box);
  BOOST_CHECK(box.pt1 == box2.pt1);
  BOOST_CHECK(box.pt2 == box2.pt2);
}

BOOST_AUTO_TEST_CASE(BoundingBoxD_copy_constructor)
{
  const BoundingBoxD box(Point3D(0., 0., 0.), Point3D(100., 100., 100.));
  BoundingBoxD box2(box);
  BOOST_CHECK(box.pt1 == box2.pt1);
  BOOST_CHECK(box.pt2 == box2.pt2);
}

//constructor de copia tipos diferentes

BOOST_AUTO_TEST_CASE(BoundingBoxD_copyConstructorDiff)
{
  const BoundingBoxD box(Point3D(0.5, 0.4, 1.9), Point3D(100.6, 100.4, 105.49));
  BoundingBoxI box2(box);
  BOOST_CHECK_EQUAL(1, box2.pt1.x);
  BOOST_CHECK_EQUAL(0, box2.pt1.y);
  BOOST_CHECK_EQUAL(2, box2.pt1.z);
  BOOST_CHECK_EQUAL(101, box2.pt2.x);
  BOOST_CHECK_EQUAL(100, box2.pt2.y);
  BOOST_CHECK_EQUAL(105, box2.pt2.z);
}

//constructor punto central y dimensiones

BOOST_AUTO_TEST_CASE(Box_constructor_center_dim)
{
  const BoundingBoxD box(Point3D(0.5, 0.4, 1.9), 50.);
  BOOST_CHECK_EQUAL(-24.5, box.pt1.x);
  BOOST_CHECK_EQUAL(-24.6, box.pt1.y);
  BOOST_CHECK_EQUAL(-23.1, box.pt1.z);
  BOOST_CHECK_EQUAL(25.5, box.pt2.x);
  BOOST_CHECK_EQUAL(25.4, box.pt2.y);
  BOOST_CHECK_EQUAL(26.9, box.pt2.z);
}

BOOST_AUTO_TEST_CASE(Box_constructor_centerDimWHD)
{
  const BoundingBoxD box(Point3D(0.5, 0.4, 1.9), 50., 50., 50.);
  BOOST_CHECK_EQUAL(-24.5, box.pt1.x);
  BOOST_CHECK_EQUAL(-24.6, box.pt1.y);
  BOOST_CHECK_EQUAL(-23.1, box.pt1.z);
  BOOST_CHECK_EQUAL(25.5, box.pt2.x);
  BOOST_CHECK_EQUAL(25.4, box.pt2.y);
  BOOST_CHECK_EQUAL(26.9, box.pt2.z);
}

BOOST_AUTO_TEST_CASE(Box_type)
{
  const BoundingBoxI box1;
  BOOST_CHECK(box1.type() == Entity::Type::bounding_box);

  const BoundingBoxI box2(Point3I(0, 0, 0), Point3I(100, 100, 100));
  BoundingBoxI box3(box2);
  BOOST_CHECK(box2.pt1 == box3.pt1);
  BOOST_CHECK(box2.pt2 == box3.pt2);
  BOOST_CHECK(box2.type() == Entity::Type::bounding_box);
  BOOST_CHECK(box3.type() == Entity::Type::bounding_box);
}


// Comprobación de que redondea bien con ventanas de enteros

BOOST_AUTO_TEST_CASE(Box_round)
{
  // Tamaño par
  Point3I ptc(50, 50, 50);
  BoundingBoxI box(ptc, 50, 50, 50);
  BOOST_CHECK(ptc == box.center());
  BOOST_CHECK_EQUAL(50, box.width());
  BOOST_CHECK_EQUAL(50, box.height());
  BOOST_CHECK_EQUAL(50, box.depth());

  //Tamaño impar
  Point3I ptc2(50, 50, 50);
  BoundingBoxI box2(ptc2, 51, 51, 51);
  BOOST_CHECK(ptc2 == box2.center());
  BOOST_CHECK_EQUAL(51, box2.width());
  BOOST_CHECK_EQUAL(51, box2.height());
  BOOST_CHECK_EQUAL(51, box2.depth());

  // Ventana double
  Point3D ptc3(50.67, 50.76, 50.76);
  BoundingBoxD box3(ptc3, 100.32, 254.25, 124.5);
  BOOST_CHECK_CLOSE(ptc3.x, box3.center().x, 0.01);
  BOOST_CHECK_CLOSE(ptc3.y, box3.center().y, 0.01);
  BOOST_CHECK_CLOSE(ptc3.z, box3.center().z, 0.01);
  BOOST_CHECK_CLOSE(100.32, box3.width(), 0.01);
  BOOST_CHECK_CLOSE(254.25, box3.height(), 0.01);
  BOOST_CHECK_CLOSE(124.5, box3.depth(), 0.01);

  // Ventana float
  Point3F ptc4(50.67f, 34.45f, 10.23f);
  BoundingBoxF box4(ptc4, 100.34f, 254.23f, 123.34f);
  BOOST_CHECK_CLOSE(ptc4.x, box4.center().x, 0.01f);
  BOOST_CHECK_CLOSE(ptc4.y, box4.center().y, 0.01f);
  BOOST_CHECK_CLOSE(ptc4.z, box4.center().z, 0.01f);
  BOOST_CHECK_CLOSE(100.34f, box4.width(), 0.01);
  BOOST_CHECK_CLOSE(254.23f, box4.height(), 0.01);
  BOOST_CHECK_CLOSE(123.34f, box4.depth(), 0.01);
}

/* Operador de asignación */

BOOST_AUTO_TEST_CASE(Box_assing_operator)
{
  const BoundingBoxD box(Point3D(0., 0., 0.), Point3D(100., 100., 100.));
  BoundingBoxD box2 = box;
  BOOST_CHECK(box.pt1 == box2.pt1);
  BOOST_CHECK(box.pt2 == box2.pt2);
}

BOOST_AUTO_TEST_CASE(Box_isEmpty)
{
  BoundingBoxI box;
  BOOST_CHECK(box.isEmpty());
}


BOOST_AUTO_TEST_CASE(Box_containsPoint)
{
  BoundingBoxI box(Point3I(0, 0, 0), Point3I(100, 100, 100));
  BOOST_CHECK(box.containsPoint(Point3I(50, 50, 50)));
  BOOST_CHECK_EQUAL(false, box.containsPoint(Point3I(150, 50, 50)));
  BOOST_CHECK(box.containsPoint(Point3I(0, 0, 0)));
  BOOST_CHECK(box.containsPoint(Point3I(100, 100, 100)));
}

BOOST_AUTO_TEST_CASE(Box_containsBbox)
{
  BoundingBoxI box(Point3I(0, 0, 0), Point3I(100, 100, 100));
  BOOST_CHECK(box.containsBox(BoundingBoxI(Point3I(50, 50, 50), 50)));
  BOOST_CHECK_EQUAL(false, box.containsBox(BoundingBoxI(Point3I(200, 50, 50), 50)));
}

BOOST_AUTO_TEST_CASE(Box_join)
{
  BoundingBoxI box1(Point3I(0, 0, 0), Point3I(50, 50, 50));
  BoundingBoxI box2(Point3I(20, 30, 10), Point3I(100, 100, 100));
  BoundingBoxI box3 = joinBoundingBoxes(box1, box2);
  BOOST_CHECK_EQUAL(0, box3.pt1.x);
  BOOST_CHECK_EQUAL(0, box3.pt1.y);
  BOOST_CHECK_EQUAL(0, box3.pt1.z);
  BOOST_CHECK_EQUAL(100, box3.pt2.x);
  BOOST_CHECK_EQUAL(100, box3.pt2.y);
  BOOST_CHECK_EQUAL(100, box3.pt2.z);
}

BOOST_AUTO_TEST_CASE(Box_intersect)
{
  BoundingBoxI box1(Point3I(0, 0, 0), Point3I(50, 50, 50));
  BoundingBoxI box2(Point3I(20, 30, 10), Point3I(100, 100, 100));
  BoundingBoxI box3 = intersectBoundingBoxes(box1, box2);
  BOOST_CHECK_EQUAL(20, box3.pt1.x);
  BOOST_CHECK_EQUAL(30, box3.pt1.y);
  BOOST_CHECK_EQUAL(10, box3.pt1.z);
  BOOST_CHECK_EQUAL(50, box3.pt2.x);
  BOOST_CHECK_EQUAL(50, box3.pt2.y);
  BOOST_CHECK_EQUAL(50, box3.pt2.z);
}

BOOST_AUTO_TEST_CASE(Box_cast)
{
  const BoundingBoxD box(Point3D(0.5, 0.4, 1.9), Point3D(100.6, 100.4, 105.49));
  BoundingBoxI box2 = static_cast<BoundingBoxI>(box);
  BOOST_CHECK_EQUAL(1, box2.pt1.x);
  BOOST_CHECK_EQUAL(0, box2.pt1.y);
  BOOST_CHECK_EQUAL(2, box2.pt1.z);
  BOOST_CHECK_EQUAL(101, box2.pt2.x);
  BOOST_CHECK_EQUAL(100, box2.pt2.y);
  BOOST_CHECK_EQUAL(105, box2.pt2.z);
}