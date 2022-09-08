/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by Esteban Ruiz de Oña Crespo                       *
 *                                                                        *
 * This file is part of TidopLib                                          *
 *                                                                        *
 * TidopLib is free software: you can redistribute it and/or modify       *
 * it under the terms of the GNU Lesser General Public License as         *
 * published by the Free Software Foundation, either version 3 of the     *
 * License, or (at your option) any later version.                        *
 *                                                                        *
 * TidopLib is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU Lesser General Public License for more details.                    *
 *                                                                        *
 * You should have received a copy of the GNU Lesser General Public       *
 * License along with TidopLib. If not, see <http://www.gnu.org/licenses>.*
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/
 
#define BOOST_TEST_MODULE Tidop Plygon test
#include <boost/test/unit_test.hpp>
#include <tidop/geometry/entities/polygon.h>
#include <tidop/geometry/entities/window.h>
#include <tidop/geometry/entities/bbox.h>


using namespace tl;

BOOST_AUTO_TEST_SUITE(PolygonTestSuite)


struct PolygonTest
{
  PolygonTest()
    : polygon_initializer_list_constructor(nullptr),
      polygon_reserve_constructor(nullptr),
      polygon_copy_constructor_integer(nullptr),
      polygon_copy_constructor_double(nullptr),
      polygon_holes(nullptr)
  {
  }

  ~PolygonTest()
  {
    if (polygon_initializer_list_constructor) { 
      delete polygon_initializer_list_constructor;
      polygon_initializer_list_constructor = nullptr;
    }
    if (polygon_reserve_constructor) {
      delete polygon_reserve_constructor;
      polygon_reserve_constructor = nullptr;
    }
    if (polygon_copy_constructor_integer) {
      delete polygon_copy_constructor_integer;
      polygon_copy_constructor_integer = nullptr;
    }
    if (polygon_copy_constructor_double) {
      delete polygon_copy_constructor_double;
      polygon_copy_constructor_double = nullptr;
    }
    if(polygon_holes) {
      delete polygon_holes;
      polygon_holes = nullptr;
    }
  }

  void setup()
  {
    polygon_initializer_list_constructor = new PolygonI{
      PointI(4157222, 664789),
      PointI(4149043, 688836),
      PointI(4172803, 690340),
      PointI(4177148, 642997),
      PointI(4137012, 671808),
      PointI(4146292, 666953),
      PointI(4138759, 702670) };
             
    polygon_reserve_constructor = new PolygonD(10);

    polygon_copy_constructor_integer = new PolygonI(*polygon_initializer_list_constructor);
    
    polygon_copy_constructor_double = new PolygonD(*polygon_reserve_constructor);

    polygon_holes = new PolygonD({Point<double>(0., 0.),
                                  Point<double>(100., 0.),
                                  Point<double>(100., 100.),
                                  Point<double>(0., 100.)
                                 });
    PolygonHole<Point<double>> hole{Point<double>(20., 20.),
                                    Point<double>(80., 20.),
                                    Point<double>(50., 80.)};
    polygon_holes->addHole(hole);
  }

  void teardown()
  {

  }

  PolygonI polygon_default_constructor_integer;
  PolygonD polygon_default_constructor_double;
  PolygonF polygon_default_constructor_float;

  PolygonI *polygon_initializer_list_constructor;
  PolygonD *polygon_reserve_constructor;

  PolygonI *polygon_copy_constructor_integer;
  PolygonD *polygon_copy_constructor_double;

  PolygonD *polygon_holes;
};


BOOST_FIXTURE_TEST_CASE(size, PolygonTest) 
{
  BOOST_CHECK_EQUAL(0, polygon_default_constructor_integer.size());
  BOOST_CHECK_EQUAL(0, polygon_default_constructor_double.size());
  BOOST_CHECK_EQUAL(0, polygon_default_constructor_float.size());
  BOOST_CHECK_EQUAL(7, polygon_initializer_list_constructor->size());
  BOOST_CHECK_EQUAL(10, polygon_reserve_constructor->size());
  BOOST_CHECK_EQUAL(7, polygon_copy_constructor_integer->size());
  BOOST_CHECK_EQUAL(10, polygon_copy_constructor_double->size());
}

BOOST_FIXTURE_TEST_CASE(window, PolygonTest)
{
  WindowI w_i = polygon_default_constructor_integer.window();
  BOOST_CHECK_EQUAL(TL_INT_MAX, w_i.pt1.x);
  BOOST_CHECK_EQUAL(TL_INT_MAX, w_i.pt1.y);
  BOOST_CHECK_EQUAL(TL_INT_MIN, w_i.pt2.x);
  BOOST_CHECK_EQUAL(TL_INT_MIN, w_i.pt2.y);

  WindowD w_d = polygon_default_constructor_double.window();
  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, w_d.pt1.x);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, w_d.pt1.y);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, w_d.pt2.x);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, w_d.pt2.y);
    
  WindowF w_f = polygon_default_constructor_float.window();
  BOOST_CHECK_EQUAL(TL_FLOAT_MAX, w_f.pt1.x);
  BOOST_CHECK_EQUAL(TL_FLOAT_MAX, w_f.pt1.y);
  BOOST_CHECK_EQUAL(TL_FLOAT_MIN, w_f.pt2.x);
  BOOST_CHECK_EQUAL(TL_FLOAT_MIN, w_f.pt2.y);

  w_i = polygon_initializer_list_constructor->window();
  BOOST_CHECK_EQUAL(4137012, w_i.pt1.x);
  BOOST_CHECK_EQUAL(642997, w_i.pt1.y);
  BOOST_CHECK_EQUAL(4177148, w_i.pt2.x);
  BOOST_CHECK_EQUAL(702670, w_i.pt2.y);

  w_d = polygon_reserve_constructor->window();
  BOOST_CHECK_EQUAL(0., w_d.pt1.x);
  BOOST_CHECK_EQUAL(0., w_d.pt1.y);
  BOOST_CHECK_EQUAL(0., w_d.pt2.x);
  BOOST_CHECK_EQUAL(0., w_d.pt2.y);

  w_i = polygon_copy_constructor_integer->window();
  BOOST_CHECK_EQUAL(4137012, w_i.pt1.x);
  BOOST_CHECK_EQUAL(642997, w_i.pt1.y);
  BOOST_CHECK_EQUAL(4177148, w_i.pt2.x);
  BOOST_CHECK_EQUAL(702670, w_i.pt2.y);

  w_d = polygon_copy_constructor_double->window();
  BOOST_CHECK_EQUAL(0., w_d.pt1.x);
  BOOST_CHECK_EQUAL(0., w_d.pt1.y);
  BOOST_CHECK_EQUAL(0., w_d.pt2.x);
  BOOST_CHECK_EQUAL(0., w_d.pt2.y);
}

BOOST_FIXTURE_TEST_CASE(length, PolygonTest) 
{
  BOOST_CHECK_EQUAL(0, polygon_default_constructor_integer.length());
  BOOST_CHECK_EQUAL(0, polygon_default_constructor_integer.length());
  BOOST_CHECK_EQUAL(0, polygon_default_constructor_integer.length());
  BOOST_CHECK_CLOSE(193131.62, polygon_initializer_list_constructor->length(), 0.01);
  BOOST_CHECK_EQUAL(0., polygon_reserve_constructor->length());
  BOOST_CHECK_CLOSE(193131.62, polygon_copy_constructor_integer->length(), 0.01);
  BOOST_CHECK_EQUAL(0., polygon_copy_constructor_double->length());
}

BOOST_FIXTURE_TEST_CASE(type, PolygonTest)
{
  BOOST_CHECK(polygon_default_constructor_integer.type() == Entity::Type::polygon2d);
  BOOST_CHECK(polygon_default_constructor_double.type() == Entity::Type::polygon2d);
  BOOST_CHECK(polygon_default_constructor_float.type() == Entity::Type::polygon2d);
  BOOST_CHECK(polygon_initializer_list_constructor->type() == Entity::Type::polygon2d);
  BOOST_CHECK(polygon_reserve_constructor->type() == Entity::Type::polygon2d);
  BOOST_CHECK(polygon_copy_constructor_integer->type() == Entity::Type::polygon2d);
  BOOST_CHECK(polygon_copy_constructor_double->type() == Entity::Type::polygon2d);
}

/* Operador de asignación */

BOOST_FIXTURE_TEST_CASE(assing_operator, PolygonTest)
{
  PolygonI polygon;
  polygon = *polygon_initializer_list_constructor;

  BOOST_CHECK_EQUAL(7, polygon.size());

  WindowI w = polygon.window();

  BOOST_CHECK_EQUAL(4137012, w.pt1.x);
  BOOST_CHECK_EQUAL(642997, w.pt1.y);
  BOOST_CHECK_EQUAL(4177148, w.pt2.x);
  BOOST_CHECK_EQUAL(702670, w.pt2.y);

  BOOST_CHECK_CLOSE(193131.62, polygon.length(), 0.01);
}

BOOST_FIXTURE_TEST_CASE(move_operator, PolygonTest)
{
  PolygonI polygon_to_move(*polygon_initializer_list_constructor);
  PolygonI polygon;
  polygon = std::move(polygon_to_move);

  BOOST_CHECK_EQUAL(7, polygon.size());
  BOOST_CHECK_EQUAL(0, polygon_to_move.size());

  for (int i = 0; i < polygon.size(); i++) {
    BOOST_CHECK(polygon[i] == (*polygon_initializer_list_constructor)[i]);
  }

}

BOOST_FIXTURE_TEST_CASE(area, PolygonTest)
{
  BOOST_CHECK_CLOSE(0, polygon_default_constructor_integer.area(), 0.01);
  BOOST_CHECK_CLOSE(1037000000, polygon_initializer_list_constructor->area(), 0.01);
}

BOOST_FIXTURE_TEST_CASE(isInner, PolygonTest)
{
  BOOST_CHECK(false == polygon_initializer_list_constructor->isInner(PointI(0,0)));
  BOOST_CHECK(polygon_initializer_list_constructor->isInner(PointI(4160000, 675000)));
}

BOOST_FIXTURE_TEST_CASE(holes, PolygonTest)
{
  BOOST_CHECK_EQUAL(1, polygon_holes->holes());
  auto hole = polygon_holes->hole(0);
  BOOST_CHECK_EQUAL(3, hole.size());
}


BOOST_AUTO_TEST_SUITE_END()


/* Polygon3D */

BOOST_AUTO_TEST_SUITE(Polygon3DTestSuite)


struct Polygon3DTest
{
  Polygon3DTest()
    : polygon_holes(nullptr)
  {
    polygon_initializer_list_constructor = new Polygon3dI {
      Point3I(4157222, 664789, 10),
      Point3I(4149043, 688836, 20),
      Point3I(4172803, 690340, 30),
      Point3I(4177148, 642997, 25),
      Point3I(4137012, 671808, 20),
      Point3I(4146292, 666953, 15),
      Point3I(4138759, 702670, 10) };
             
    polygon_reserve_constructor = new Polygon3dD(10);
  }

  ~Polygon3DTest()
  {
    if(polygon_initializer_list_constructor) {
      delete polygon_initializer_list_constructor;
      polygon_initializer_list_constructor = nullptr;
    }

    if(polygon_reserve_constructor) {
      delete polygon_reserve_constructor;
      polygon_reserve_constructor = nullptr;
    }

    if(polygon_holes) {
      delete polygon_holes;
      polygon_holes = nullptr;
    }
  }

  void setup()
  {
    Polygon3DHole<Point3<double>> hole{Point3<double>(20., 20., 2.),
                                       Point3<double>(80., 20., 2.),
                                       Point3<double>(50., 80., 2.)};

    polygon_holes = new Polygon3dD({Point3<double>(0., 0., 0.),
                                    Point3<double>(100., 0., 0.),
                                    Point3<double>(100., 100., 0.),
                                    Point3<double>(0., 100., 0.)});

    polygon_holes->addHole(hole);

  }

  void teardown()
  {

  }

  Polygon3dI  polygon_default_constructor_integer;
  Polygon3dI *polygon_initializer_list_constructor;
  Polygon3dD *polygon_reserve_constructor;
  Polygon3dD *polygon_holes;
};



BOOST_FIXTURE_TEST_CASE(default_constructor, Polygon3DTest)
{
  BoundingBoxI bbox = polygon_default_constructor_integer.boundingBox();
  
  BOOST_CHECK_EQUAL(0, polygon_default_constructor_integer.size());
  BOOST_CHECK(polygon_default_constructor_integer.type() == Entity::Type::polygon3d);
  BOOST_CHECK_EQUAL(TL_INT_MAX, bbox.pt1.x);
  BOOST_CHECK_EQUAL(TL_INT_MAX, bbox.pt1.y);
  BOOST_CHECK_EQUAL(TL_INT_MAX, bbox.pt1.z);
  BOOST_CHECK_EQUAL(TL_INT_MIN, bbox.pt2.x);
  BOOST_CHECK_EQUAL(TL_INT_MIN, bbox.pt2.y);
  BOOST_CHECK_EQUAL(TL_INT_MIN, bbox.pt2.z);
  BOOST_CHECK_EQUAL(0., polygon_default_constructor_integer.length());
  BOOST_CHECK(polygon_default_constructor_integer.is3D());
}

BOOST_FIXTURE_TEST_CASE(constructor_reserve, Polygon3DTest)
{
  BoundingBoxD bbox = polygon_reserve_constructor->boundingBox();

  BOOST_CHECK_EQUAL(10, polygon_reserve_constructor->size());
  BOOST_CHECK(polygon_reserve_constructor->type() == Entity::Type::polygon3d);
  BOOST_CHECK_EQUAL(0., bbox.pt1.x);
  BOOST_CHECK_EQUAL(0., bbox.pt1.y);
  BOOST_CHECK_EQUAL(0., bbox.pt1.z);
  BOOST_CHECK_EQUAL(0., bbox.pt2.x);
  BOOST_CHECK_EQUAL(0., bbox.pt2.y);
  BOOST_CHECK_EQUAL(0., bbox.pt2.z);
  BOOST_CHECK_EQUAL(0., polygon_reserve_constructor->length());
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, Polygon3DTest)
{
  Polygon3dD pol_c(*polygon_reserve_constructor);
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

BOOST_FIXTURE_TEST_CASE(move_constructor, Polygon3DTest)
{
  Polygon3dD pol(std::move(Polygon3dD()));
  BOOST_CHECK_EQUAL(0, pol.size());

  BoundingBoxD bbox = pol.boundingBox();
  BOOST_CHECK(pol.type() == Entity::Type::polygon3d);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, bbox.pt1.x);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, bbox.pt1.y);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, bbox.pt1.z);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, bbox.pt2.x);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, bbox.pt2.y);
  BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, bbox.pt2.z);

  BOOST_CHECK_EQUAL(0., pol.length());
}

BOOST_FIXTURE_TEST_CASE(constructor_list, Polygon3DTest)
{
  BOOST_CHECK_EQUAL(7, polygon_initializer_list_constructor->size());

  BoundingBoxD bbox = polygon_initializer_list_constructor->boundingBox();

  BOOST_CHECK_EQUAL(4137012, bbox.pt1.x);
  BOOST_CHECK_EQUAL(642997, bbox.pt1.y);
  BOOST_CHECK_EQUAL(10, bbox.pt1.z);
  BOOST_CHECK_EQUAL(4177148, bbox.pt2.x);
  BOOST_CHECK_EQUAL(702670, bbox.pt2.y);
  BOOST_CHECK_EQUAL(30, bbox.pt2.z);

  BOOST_CHECK_CLOSE(193131.62, polygon_initializer_list_constructor->length(), 0.01);
}

BOOST_FIXTURE_TEST_CASE(type, Polygon3DTest)
{
  BOOST_CHECK(polygon_default_constructor_integer.type() == Entity::Type::polygon3d);
  BOOST_CHECK(polygon_initializer_list_constructor->type() == Entity::Type::polygon3d);
  BOOST_CHECK(polygon_reserve_constructor->type() == Entity::Type::polygon3d);
}

/* Operador de asignación */

BOOST_FIXTURE_TEST_CASE(assing_operator, Polygon3DTest)
{
  Polygon3dI polygon;
  polygon = *polygon_initializer_list_constructor;

  BOOST_CHECK_EQUAL(7, polygon.size());

  BoundingBoxD bbox = polygon_initializer_list_constructor->boundingBox();

  BOOST_CHECK_EQUAL(4137012, bbox.pt1.x);
  BOOST_CHECK_EQUAL(642997, bbox.pt1.y);
  BOOST_CHECK_EQUAL(10, bbox.pt1.z);
  BOOST_CHECK_EQUAL(4177148, bbox.pt2.x);
  BOOST_CHECK_EQUAL(702670, bbox.pt2.y);
  BOOST_CHECK_EQUAL(30, bbox.pt2.z);
}


BOOST_FIXTURE_TEST_CASE(move_operator, Polygon3DTest)
{
  Polygon3dI polygon_to_move(*polygon_initializer_list_constructor);
  Polygon3dI polygon;
  polygon = std::move(polygon_to_move);

  BOOST_CHECK_EQUAL(7, polygon.size());
  BOOST_CHECK_EQUAL(0, polygon_to_move.size());

  for(int i = 0; i < polygon.size(); i++) {
    BOOST_CHECK(polygon[i] == (*polygon_initializer_list_constructor)[i]);
  }

}

BOOST_FIXTURE_TEST_CASE(holes, Polygon3DTest)
{
  BOOST_CHECK_EQUAL(1, polygon_holes->holes());
  auto hole = polygon_holes->hole(0);
  BOOST_CHECK_EQUAL(3, hole.size());
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
  MultiPolygon<PointI> multipolygon_copy;
  multipolygon_copy = multipolygon2;
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
    Polygon3dI polygon_initializer_list_constructor{
      Point3I(4157222, 664789, 10),
      Point3I(4149043, 688836, 20),
      Point3I(4172803, 690340, 30),
      Point3I(4177148, 642997, 25),
      Point3I(4137012, 671808, 20),
      Point3I(4146292, 666953, 15),
      Point3I(4138759, 702670, 10) 
    };
    multipolygon2.push_back(polygon_initializer_list_constructor);
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
  MultiPolygon3D<Point3I> multipolygon_copy;
  multipolygon_copy = multipolygon2;
  
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