#define BOOST_TEST_MODULE Tidop Plygon test
#include <boost/test/unit_test.hpp>
#include <tidop/geometry/entities/polygon.h>
#include <tidop/geometry/entities/window.h>
#include <tidop/geometry/entities/bbox.h>


using namespace tl;
using namespace tl::geometry;

BOOST_AUTO_TEST_SUITE(PolygonTestSuite)


struct PolygonTest
{
  PolygonTest()
  {
    pol_2 = new geometry::Polygon<Point<int>>{
      PointI(4157222, 664789),
      PointI(4149043, 688836),
      PointI(4172803, 690340),
      PointI(4177148, 642997),
      PointI(4137012, 671808),
      PointI(4146292, 666953),
      PointI(4138759, 702670) };
             
    pol_3 = new geometry::PolygonD(10);
  }
  ~PolygonTest()
  {
    if (pol_2) delete pol_2, pol_2 = nullptr;
    if (pol_3) delete pol_3, pol_3 = nullptr;
  }

  void setup()
  {
  }

  void teardown()
  {

  }

  geometry::Polygon<Point<int>> pol_1;
  geometry::Polygon<Point<int>> *pol_2;
  geometry::Polygon<Point<double>> *pol_3;
};


/* Constructor por defecto */

BOOST_FIXTURE_TEST_CASE(default_constructor, PolygonTest)
{

  WindowI w = pol_1.window();
  
  /*Comprobamos si se ha creado con el contructor por defecto*/
  BOOST_CHECK_EQUAL(0, pol_1.size());
  BOOST_CHECK(pol_1.type() == Entity::Type::polygon2d);
  BOOST_CHECK_EQUAL(TL_INT_MAX, w.pt1.x);
  BOOST_CHECK_EQUAL(TL_INT_MAX, w.pt1.y);
  BOOST_CHECK_EQUAL(TL_INT_MIN, w.pt2.x);
  BOOST_CHECK_EQUAL(TL_INT_MIN, w.pt2.y);
  BOOST_CHECK_EQUAL(0., pol_1.length());
  BOOST_CHECK(false == pol_1.is3D());
}

/* Constructor reserve */

BOOST_FIXTURE_TEST_CASE(constructor_reserve, PolygonTest)
{
  WindowD w = pol_3->window();

  BOOST_CHECK_EQUAL(10, pol_3->size());
  BOOST_CHECK(pol_3->type() == Entity::Type::polygon2d);
  BOOST_CHECK_EQUAL(0., w.pt1.x);
  BOOST_CHECK_EQUAL(0., w.pt1.y);
  BOOST_CHECK_EQUAL(0., w.pt2.x);
  BOOST_CHECK_EQUAL(0., w.pt2.y);
  BOOST_CHECK_EQUAL(0., pol_3->length());
}

/*Constructor de copia*/

BOOST_FIXTURE_TEST_CASE(copy_constructor, PolygonTest)
{
  PolygonD pol_c(*pol_3);
  BOOST_CHECK_EQUAL(10, pol_c.size());
  BOOST_CHECK(pol_c.type() == Entity::Type::polygon2d);
  auto w = pol_c.window();
  BOOST_CHECK_EQUAL(0., w.pt1.x);
  BOOST_CHECK_EQUAL(0., w.pt1.y);
  BOOST_CHECK_EQUAL(0., w.pt2.x);
  BOOST_CHECK_EQUAL(0., w.pt2.y);

  BOOST_CHECK_EQUAL(0., pol_c.length());

}


/* Constructor lista de inicializadores */

BOOST_FIXTURE_TEST_CASE(constructor_list, PolygonTest)
{
  BOOST_CHECK_EQUAL(7, pol_2->size());
  BOOST_CHECK(pol_2->type() == Entity::Type::polygon2d);
  WindowI w = pol_2->window();

  BOOST_CHECK_EQUAL(4137012, w.pt1.x);
  BOOST_CHECK_EQUAL(642997, w.pt1.y);
  BOOST_CHECK_EQUAL(4177148, w.pt2.x);
  BOOST_CHECK_EQUAL(702670, w.pt2.y);

  BOOST_CHECK_CLOSE(193131.62, pol_2->length(), 0.01);
}

BOOST_FIXTURE_TEST_CASE(type, PolygonTest)
{
  BOOST_CHECK(pol_1.type() == Entity::Type::polygon2d);
  BOOST_CHECK(pol_2->type() == Entity::Type::polygon2d);
  BOOST_CHECK(pol_3->type() == Entity::Type::polygon2d);
}

/* Operador de asignación */

BOOST_FIXTURE_TEST_CASE(assing_operator, PolygonTest)
{
  PolygonI polygon = *pol_2;

  BOOST_CHECK_EQUAL(7, polygon.size());

  WindowI w = polygon.window();

  BOOST_CHECK_EQUAL(4137012, w.pt1.x);
  BOOST_CHECK_EQUAL(642997, w.pt1.y);
  BOOST_CHECK_EQUAL(4177148, w.pt2.x);
  BOOST_CHECK_EQUAL(702670, w.pt2.y);

  BOOST_CHECK_CLOSE(193131.62, polygon.length(), 0.01);
}

BOOST_FIXTURE_TEST_CASE(area, PolygonTest)
{
  BOOST_CHECK_CLOSE(0, pol_1.area(), 0.01);
  BOOST_CHECK_CLOSE(2059476079.5, pol_2->area(), 0.01);
}

BOOST_FIXTURE_TEST_CASE(isInner, PolygonTest)
{
  BOOST_CHECK(false == pol_2->isInner(PointI(0,0)));
  BOOST_CHECK(pol_2->isInner(PointI(4160000, 675000)));
}

BOOST_AUTO_TEST_SUITE_END()


/* Polygon3D */

BOOST_AUTO_TEST_SUITE(Polygon3DTestSuite)


struct Polygon3DTest
{
  Polygon3DTest()
  {
    pol_2 = new Polygon3dI {
      Point3I(4157222, 664789, 10),
      Point3I(4149043, 688836, 20),
      Point3I(4172803, 690340, 30),
      Point3I(4177148, 642997, 25),
      Point3I(4137012, 671808, 20),
      Point3I(4146292, 666953, 15),
      Point3I(4138759, 702670, 10) };
             
    pol_3 = new Polygon3dD(10);
  }
  ~Polygon3DTest()
  {
    if (pol_2) delete pol_2, pol_2 = nullptr;
    if (pol_3) delete pol_3, pol_3 = nullptr;
  }

  void setup()
  {
  }

  void teardown()
  {

  }

  Polygon3dI  pol_1;
  Polygon3dI *pol_2;
  Polygon3dD *pol_3;
};


/* Constructor por defecto */

BOOST_FIXTURE_TEST_CASE(default_constructor, Polygon3DTest)
{
  BoundingBoxI bbox = pol_1.boundingBox();
  
  BOOST_CHECK_EQUAL(0, pol_1.size());
  BOOST_CHECK(pol_1.type() == Entity::Type::polygon3d);
  BOOST_CHECK_EQUAL(TL_INT_MAX, bbox.pt1.x);
  BOOST_CHECK_EQUAL(TL_INT_MAX, bbox.pt1.y);
  BOOST_CHECK_EQUAL(TL_INT_MAX, bbox.pt1.z);
  BOOST_CHECK_EQUAL(TL_INT_MIN, bbox.pt2.x);
  BOOST_CHECK_EQUAL(TL_INT_MIN, bbox.pt2.y);
  BOOST_CHECK_EQUAL(TL_INT_MIN, bbox.pt2.z);
  BOOST_CHECK_EQUAL(0., pol_1.length());
  BOOST_CHECK(pol_1.is3D());
}

/* Constructor reserve */

BOOST_FIXTURE_TEST_CASE(constructor_reserve, Polygon3DTest)
{
  BoundingBoxD bbox = pol_3->boundingBox();

  BOOST_CHECK_EQUAL(10, pol_3->size());
  BOOST_CHECK(pol_3->type() == Entity::Type::polygon3d);
  BOOST_CHECK_EQUAL(0., bbox.pt1.x);
  BOOST_CHECK_EQUAL(0., bbox.pt1.y);
  BOOST_CHECK_EQUAL(0., bbox.pt1.z);
  BOOST_CHECK_EQUAL(0., bbox.pt2.x);
  BOOST_CHECK_EQUAL(0., bbox.pt2.y);
  BOOST_CHECK_EQUAL(0., bbox.pt2.z);
  BOOST_CHECK_EQUAL(0., pol_3->length());
}

/*Constructor de copia*/

BOOST_FIXTURE_TEST_CASE(copy_constructor, Polygon3DTest)
{
  Polygon3dD pol_c(*pol_3);
  BOOST_CHECK_EQUAL(10, pol_c.size());

  BoundingBoxD bbox = pol_c.boundingBox();
  BOOST_CHECK(pol_c.type() == Entity::Type::polygon3d);
  BOOST_CHECK_EQUAL(0., bbox.pt1.x);
  BOOST_CHECK_EQUAL(0., bbox.pt1.y);
  BOOST_CHECK_EQUAL(0., bbox.pt1.z);
  BOOST_CHECK_EQUAL(0., bbox.pt2.x);
  BOOST_CHECK_EQUAL(0., bbox.pt2.y);
  BOOST_CHECK_EQUAL(0., bbox.pt2.z);

  BOOST_CHECK_EQUAL(0., pol_c.length());

}


/* Constructor lista de inicializadores */

BOOST_FIXTURE_TEST_CASE(constructor_list, Polygon3DTest)
{
  BOOST_CHECK_EQUAL(7, pol_2->size());

  BoundingBoxD bbox = pol_2->boundingBox();

  BOOST_CHECK_EQUAL(4137012, bbox.pt1.x);
  BOOST_CHECK_EQUAL(642997, bbox.pt1.y);
  BOOST_CHECK_EQUAL(10, bbox.pt1.z);
  BOOST_CHECK_EQUAL(4177148, bbox.pt2.x);
  BOOST_CHECK_EQUAL(702670, bbox.pt2.y);
  BOOST_CHECK_EQUAL(30, bbox.pt2.z);

  BOOST_CHECK_CLOSE(193131.62, pol_2->length(), 0.01);
}

BOOST_FIXTURE_TEST_CASE(type, Polygon3DTest)
{
  BOOST_CHECK(pol_1.type() == Entity::Type::polygon3d);
  BOOST_CHECK(pol_2->type() == Entity::Type::polygon3d);
  BOOST_CHECK(pol_3->type() == Entity::Type::polygon3d);
}

/* Operador de asignación */

BOOST_FIXTURE_TEST_CASE(assing_operator, Polygon3DTest)
{
  Polygon3dI polygon = *pol_2;

  BOOST_CHECK_EQUAL(7, polygon.size());

  BoundingBoxD bbox = pol_2->boundingBox();

  BOOST_CHECK_EQUAL(4137012, bbox.pt1.x);
  BOOST_CHECK_EQUAL(642997, bbox.pt1.y);
  BOOST_CHECK_EQUAL(10, bbox.pt1.z);
  BOOST_CHECK_EQUAL(4177148, bbox.pt2.x);
  BOOST_CHECK_EQUAL(702670, bbox.pt2.y);
  BOOST_CHECK_EQUAL(30, bbox.pt2.z);
}


BOOST_AUTO_TEST_SUITE_END()




/* MultiPolygon  */

BOOST_AUTO_TEST_SUITE(MultiPolygonTestSuite)


struct MultiPolygonTest
{
  MultiPolygonTest()
  {

  }

  ~MultiPolygonTest()
  {
  }

  void setup()
  {
    PolygonI polygon {
      PointI(4157222, 664789),
      PointI(4149043, 688836),
      PointI(4172803, 690340),
      PointI(4177148, 642997),
      PointI(4137012, 671808),
      PointI(4146292, 666953),
      PointI(4138759, 702670) 
    };

    multipolygon2.push_back(polygon);
  }

  void teardown()
  {

  }

  MultiPolygon<PointI> multipolygon1;
  MultiPolygon<PointI> multipolygon2;

};

BOOST_FIXTURE_TEST_CASE(default_constructor, MultiPolygonTest) 
{
  BOOST_CHECK_EQUAL(0, multipolygon1.size());
  BOOST_CHECK(multipolygon1.type() == Entity::Type::multipolygon2d);
  BOOST_CHECK(false == multipolygon1.is3D());
  WindowI w = multipolygon1.window();
  BOOST_CHECK_EQUAL(TL_INT_MAX, w.pt1.x);
  BOOST_CHECK_EQUAL(TL_INT_MAX, w.pt1.y);
  BOOST_CHECK_EQUAL(TL_INT_MIN, w.pt2.x);
  BOOST_CHECK_EQUAL(TL_INT_MIN, w.pt2.y);
}

/* Constructor reserve */

BOOST_FIXTURE_TEST_CASE(constructor_reserve, MultiPolygonTest) 
{
  MultiPolygon<PointI> multipolygon(10);

  BOOST_CHECK_EQUAL(10, multipolygon.size());
  BOOST_CHECK(multipolygon.type() == Entity::Type::multipolygon2d);
  BOOST_CHECK(false == multipolygon.is3D());
  WindowI w = multipolygon.window();
  BOOST_CHECK_EQUAL(TL_INT_MAX, w.pt1.x);
  BOOST_CHECK_EQUAL(TL_INT_MAX, w.pt1.y);
  BOOST_CHECK_EQUAL(TL_INT_MIN, w.pt2.x);
  BOOST_CHECK_EQUAL(TL_INT_MIN, w.pt2.y);
}

/*Constructor de copia*/

BOOST_FIXTURE_TEST_CASE(copy_constructor, MultiPolygonTest)
{
	MultiPolygon<PointI> multipolygon_copy(multipolygon2);
  BOOST_CHECK_EQUAL(1, multipolygon_copy.size());
  BOOST_CHECK(multipolygon_copy.type() == Entity::Type::multipolygon2d);
  BOOST_CHECK(false == multipolygon_copy.is3D());
  WindowI w = multipolygon_copy.window();
  BOOST_CHECK_EQUAL(4137012, w.pt1.x);
  BOOST_CHECK_EQUAL(642997, w.pt1.y);
  BOOST_CHECK_EQUAL(4177148, w.pt2.x);
  BOOST_CHECK_EQUAL(702670, w.pt2.y);
}

/* Operador de asignación */

BOOST_FIXTURE_TEST_CASE(assing_operator, MultiPolygonTest)
{
	MultiPolygon<PointI> multipolygon_copy = multipolygon2;
  BOOST_CHECK_EQUAL(1, multipolygon_copy.size());
  BOOST_CHECK(multipolygon_copy.type() == Entity::Type::multipolygon2d);
  BOOST_CHECK(false == multipolygon_copy.is3D());
  WindowI w = multipolygon_copy.window();
  BOOST_CHECK_EQUAL(4137012, w.pt1.x);
  BOOST_CHECK_EQUAL(642997, w.pt1.y);
  BOOST_CHECK_EQUAL(4177148, w.pt2.x);
  BOOST_CHECK_EQUAL(702670, w.pt2.y);
}

BOOST_AUTO_TEST_SUITE_END()


/* MultiPolygon3D */

BOOST_AUTO_TEST_SUITE(MultiPolygon3DTestSuite)


struct MultiPolygon3DTest
{
  MultiPolygon3DTest()
  {

  }
  ~MultiPolygon3DTest()
  {
  }

  void setup()
  {
    Polygon3dI pol_2{
      Point3I(4157222, 664789, 10),
      Point3I(4149043, 688836, 20),
      Point3I(4172803, 690340, 30),
      Point3I(4177148, 642997, 25),
      Point3I(4137012, 671808, 20),
      Point3I(4146292, 666953, 15),
      Point3I(4138759, 702670, 10) 
    };
    multipolygon2.push_back(pol_2);
  }

  void teardown()
  {

  }

  MultiPolygon3D<Point3I> multipolygon1;
  MultiPolygon3D<Point3I> multipolygon2;
};

/* Constructor por defecto */

BOOST_FIXTURE_TEST_CASE(default_constructor, MultiPolygon3DTest) 
{

  BoundingBoxI box = multipolygon1.boundingBox();
  BOOST_CHECK_EQUAL(0, multipolygon1.size());
  BOOST_CHECK(multipolygon1.type() == Entity::Type::multipoygon3d);
  BOOST_CHECK(multipolygon1.is3D());
  BOOST_CHECK_EQUAL(TL_INT_MAX, box.pt1.x);
  BOOST_CHECK_EQUAL(TL_INT_MAX, box.pt1.y);
  BOOST_CHECK_EQUAL(TL_INT_MAX, box.pt1.z);
  BOOST_CHECK_EQUAL(TL_INT_MIN, box.pt2.x);
  BOOST_CHECK_EQUAL(TL_INT_MIN, box.pt2.y);
  BOOST_CHECK_EQUAL(TL_INT_MIN, box.pt2.z);
}

/* Constructor reserve */

BOOST_FIXTURE_TEST_CASE(constructor_reserve, MultiPolygon3DTest) 
{
  MultiPolygon3D<Point3D> multipolygon(10);
  BoundingBoxD box = multipolygon.boundingBox();
  
  BOOST_CHECK_EQUAL(10, multipolygon.size());
  BOOST_CHECK(multipolygon.type() == Entity::Type::multipoygon3d);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, box.pt1.x);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, box.pt1.y);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, box.pt1.z);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, box.pt2.x);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, box.pt2.y);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, box.pt2.z);
}

/*Constructor de copia*/

BOOST_FIXTURE_TEST_CASE(copy_constructor, MultiPolygon3DTest) 
{
  MultiPolygon3D<Point3I> multipolygon_copy(multipolygon2);
  
  BOOST_CHECK(multipolygon_copy.type() == Entity::Type::multipoygon3d);
  BOOST_CHECK(multipolygon_copy.is3D());
	BOOST_CHECK_EQUAL(1, multipolygon_copy.size());
  BoundingBoxD bbox = multipolygon_copy.boundingBox();
  BOOST_CHECK_EQUAL(4137012, bbox.pt1.x);
  BOOST_CHECK_EQUAL(642997, bbox.pt1.y);
  BOOST_CHECK_EQUAL(10, bbox.pt1.z);
  BOOST_CHECK_EQUAL(4177148, bbox.pt2.x);
  BOOST_CHECK_EQUAL(702670, bbox.pt2.y);
  BOOST_CHECK_EQUAL(30, bbox.pt2.z);
}

/* Operador de asignación */

BOOST_FIXTURE_TEST_CASE(assing_operator, MultiPolygon3DTest)
{
  MultiPolygon3D<Point3I> multipolygon_copy = multipolygon2;
  
  BOOST_CHECK(multipolygon_copy.type() == Entity::Type::multipoygon3d);
  BOOST_CHECK(multipolygon_copy.is3D());

	BOOST_CHECK_EQUAL(1, multipolygon_copy.size());
  BoundingBoxD bbox = multipolygon_copy.boundingBox();
  BOOST_CHECK_EQUAL(4137012, bbox.pt1.x);
  BOOST_CHECK_EQUAL(642997, bbox.pt1.y);
  BOOST_CHECK_EQUAL(10, bbox.pt1.z);
  BOOST_CHECK_EQUAL(4177148, bbox.pt2.x);
  BOOST_CHECK_EQUAL(702670, bbox.pt2.y);
  BOOST_CHECK_EQUAL(30, bbox.pt2.z);
}

BOOST_AUTO_TEST_SUITE_END()