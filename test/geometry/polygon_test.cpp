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
#include <tidop/geometry/primitives/Polygon.h>
#include <tidop/geometry/primitives/MultiPolygon.h>
#include <tidop/geometry/algorithms/measurement/Area.h>
#include <tidop/geometry/io/wkt/Proxy.h>

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
        if (polygon_holes) {
            delete polygon_holes;
            polygon_holes = nullptr;
        }
    }

    void setup()
    {
        polygon_initializer_list_constructor = new Polygon2i
        {
            Point2i(4157222, 664789),
            Point2i(4149043, 688836),
            Point2i(4172803, 690340),
            Point2i(4177148, 642997),
            Point2i(4137012, 671808),
            Point2i(4146292, 666953),
            Point2i(4138759, 702670)
        };

        polygon_reserve_constructor = new Polygon2d(10);

        polygon_copy_constructor_integer = new Polygon2i(*polygon_initializer_list_constructor);

        polygon_copy_constructor_double = new Polygon2d(*polygon_reserve_constructor);

        polygon_holes = new Polygon2d({Point2d(0., 0.),
                                       Point2d(100., 0.),
                                       Point2d(100., 100.),
                                       Point2d(0., 100.)});
        LinearRing<Point2d> hole{Point2d(20., 20.),
                                 Point2d(80., 20.),
                                 Point2d(50., 80.)};
        polygon_holes->addInner(hole);
    }

    void teardown()
    {

    }

    Polygon2i polygon_default_constructor_integer;
    Polygon2d polygon_default_constructor_double;
    Polygon2f polygon_default_constructor_float;

    Polygon2i *polygon_initializer_list_constructor;
    Polygon2d *polygon_reserve_constructor;

    Polygon2i *polygon_copy_constructor_integer;
    Polygon2d *polygon_copy_constructor_double;

    Polygon2d *polygon_holes;
};


BOOST_FIXTURE_TEST_CASE(size, PolygonTest)
{
    BOOST_CHECK_EQUAL(0, polygon_default_constructor_integer.outer().size());
    BOOST_CHECK_EQUAL(0, polygon_default_constructor_double.outer().size());
    BOOST_CHECK_EQUAL(0, polygon_default_constructor_float.outer().size());
    BOOST_CHECK_EQUAL(7, polygon_initializer_list_constructor->outer().size());
    BOOST_CHECK_EQUAL(0, polygon_reserve_constructor->outer().size());
    BOOST_CHECK_EQUAL(7, polygon_copy_constructor_integer->outer().size());
    BOOST_CHECK_EQUAL(0, polygon_copy_constructor_double->outer().size());
}

BOOST_FIXTURE_TEST_CASE(capacity, PolygonTest)
{
    BOOST_CHECK_EQUAL(0, polygon_default_constructor_integer.outer().capacity());
    BOOST_CHECK_EQUAL(0, polygon_default_constructor_double.outer().capacity());
    BOOST_CHECK_EQUAL(0, polygon_default_constructor_float.outer().capacity());
    BOOST_CHECK_EQUAL(7, polygon_initializer_list_constructor->outer().capacity());
    BOOST_CHECK_EQUAL(10, polygon_reserve_constructor->outer().capacity());
    BOOST_CHECK_EQUAL(7, polygon_copy_constructor_integer->outer().capacity());
    BOOST_CHECK_EQUAL(0, polygon_copy_constructor_double->outer().capacity());
}

BOOST_FIXTURE_TEST_CASE(window, PolygonTest)
{
    auto bbox_i = polygon_default_constructor_integer.boundingBox();
    BOOST_CHECK_EQUAL(std::numeric_limits<int>::max(), bbox_i.pt1().x());
    BOOST_CHECK_EQUAL(std::numeric_limits<int>::max(), bbox_i.pt1().y());
    BOOST_CHECK_EQUAL(std::numeric_limits<int>::lowest(), bbox_i.pt2().x());
    BOOST_CHECK_EQUAL(std::numeric_limits<int>::lowest(), bbox_i.pt2().y());

    auto bbox_d = polygon_default_constructor_double.boundingBox();
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::max(), bbox_d.pt1().x());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::max(), bbox_d.pt1().y());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::lowest(), bbox_d.pt2().x());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::lowest(), bbox_d.pt2().y());

    auto bbox_f = polygon_default_constructor_float.boundingBox();
    BOOST_CHECK_EQUAL(std::numeric_limits<float>::max(), bbox_f.pt1().x());
    BOOST_CHECK_EQUAL(std::numeric_limits<float>::max(), bbox_f.pt1().y());
    BOOST_CHECK_EQUAL(std::numeric_limits<float>::lowest(), bbox_f.pt2().x());
    BOOST_CHECK_EQUAL(std::numeric_limits<float>::lowest(), bbox_f.pt2().y());

    bbox_i = polygon_initializer_list_constructor->boundingBox();
    BOOST_CHECK_EQUAL(4137012, bbox_i.pt1().x());
    BOOST_CHECK_EQUAL(642997, bbox_i.pt1().y());
    BOOST_CHECK_EQUAL(4177148, bbox_i.pt2().x());
    BOOST_CHECK_EQUAL(702670, bbox_i.pt2().y());

    bbox_d = polygon_reserve_constructor->boundingBox();
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::max(), bbox_d.pt1().x());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::max(), bbox_d.pt1().y());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::lowest(), bbox_d.pt2().x());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::lowest(), bbox_d.pt2().y());

    bbox_i = polygon_copy_constructor_integer->boundingBox();
    BOOST_CHECK_EQUAL(4137012, bbox_i.pt1().x());
    BOOST_CHECK_EQUAL(642997, bbox_i.pt1().y());
    BOOST_CHECK_EQUAL(4177148, bbox_i.pt2().x());
    BOOST_CHECK_EQUAL(702670, bbox_i.pt2().y());

    bbox_d = polygon_copy_constructor_double->boundingBox();
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::max(), bbox_d.pt1().x());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::max(), bbox_d.pt1().y());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::lowest(), bbox_d.pt2().x());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::lowest(), bbox_d.pt2().y());
}

BOOST_FIXTURE_TEST_CASE(perimeter, PolygonTest)
{
    BOOST_CHECK_EQUAL(0, polygon_default_constructor_integer.perimeter());
    BOOST_CHECK_EQUAL(0, polygon_default_constructor_integer.perimeter());
    BOOST_CHECK_EQUAL(0, polygon_default_constructor_integer.perimeter());
    BOOST_CHECK_CLOSE(235272.48352, polygon_initializer_list_constructor->perimeter(), 0.01);
    BOOST_CHECK_EQUAL(0., polygon_reserve_constructor->perimeter());
    BOOST_CHECK_CLOSE(235272.48352, polygon_copy_constructor_integer->perimeter(), 0.01);
    BOOST_CHECK_EQUAL(0., polygon_copy_constructor_double->perimeter());
}

BOOST_FIXTURE_TEST_CASE(type, PolygonTest)
{
    BOOST_CHECK(polygon_default_constructor_integer.type() == GeometryType::polygon);
    BOOST_CHECK(polygon_default_constructor_double.type() == GeometryType::polygon);
    BOOST_CHECK(polygon_default_constructor_float.type() == GeometryType::polygon);
    BOOST_CHECK(polygon_initializer_list_constructor->type() == GeometryType::polygon);
    BOOST_CHECK(polygon_reserve_constructor->type() == GeometryType::polygon);
    BOOST_CHECK(polygon_copy_constructor_integer->type() == GeometryType::polygon);
    BOOST_CHECK(polygon_copy_constructor_double->type() == GeometryType::polygon);
}

/* Operador de asignación */

BOOST_FIXTURE_TEST_CASE(assing_operator, PolygonTest)
{
    Polygon2i polygon;
    polygon = *polygon_initializer_list_constructor;

    BOOST_CHECK_EQUAL(7, polygon.outer().size());

    auto bbox = polygon.boundingBox();

    BOOST_CHECK_EQUAL(4137012, bbox.pt1().x());
    BOOST_CHECK_EQUAL(642997, bbox.pt1().y());
    BOOST_CHECK_EQUAL(4177148, bbox.pt2().x());
    BOOST_CHECK_EQUAL(702670, bbox.pt2().y());

    BOOST_CHECK_CLOSE(235272.4835, polygon.perimeter(), 0.01);
}

BOOST_FIXTURE_TEST_CASE(move_operator, PolygonTest)
{
    Polygon2i polygon_to_move(*polygon_initializer_list_constructor);
    Polygon2i polygon;
    polygon = std::move(polygon_to_move);

    BOOST_CHECK_EQUAL(7, polygon.outer().size());
    BOOST_CHECK_EQUAL(0, polygon_to_move.outer().size());

    for (int i = 0; i < polygon.outer().size(); i++) {
        BOOST_CHECK(polygon.outer()[i] == polygon_initializer_list_constructor->outer()[i]);
    }
}

BOOST_FIXTURE_TEST_CASE(_area, PolygonTest)
{
    BOOST_CHECK_CLOSE(0, area(polygon_default_constructor_integer), 0.01);
    BOOST_CHECK_CLOSE(1037000000, area(*polygon_initializer_list_constructor), 0.01);
}

//BOOST_FIXTURE_TEST_CASE(isInner, PolygonTest)
//{
//  BOOST_CHECK(false == polygon_initializer_list_constructor->isInner(Point<int>(0,0)));
//  BOOST_CHECK(polygon_initializer_list_constructor->isInner(Point<int>(4160000, 675000)));
//}

BOOST_FIXTURE_TEST_CASE(holes, PolygonTest)
{
    BOOST_CHECK_EQUAL(1, polygon_holes->numInners());
    auto hole = polygon_holes->inner(0);
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
        polygon_initializer_list_constructor = new Polygon3i
        {
             Point3i(4157222, 664789, 10),
             Point3i(4149043, 688836, 20),
             Point3i(4172803, 690340, 30),
             Point3i(4177148, 642997, 25),
             Point3i(4137012, 671808, 20),
             Point3i(4146292, 666953, 15),
             Point3i(4138759, 702670, 10)
        };

        polygon_reserve_constructor = new Polygon3d(10);
    }

    ~Polygon3DTest()
    {
        if (polygon_initializer_list_constructor) {
            delete polygon_initializer_list_constructor;
            polygon_initializer_list_constructor = nullptr;
        }

        if (polygon_reserve_constructor) {
            delete polygon_reserve_constructor;
            polygon_reserve_constructor = nullptr;
        }

        if (polygon_holes) {
            delete polygon_holes;
            polygon_holes = nullptr;
        }
    }

    void setup()
    {
        LinearRing<Point3d> hole{Point3d(20., 20., 2.),
                                 Point3d(80., 20., 2.),
                                 Point3d(50., 80., 2.)};

        polygon_holes = new Polygon3d({Point3d(0., 0., 0.),
                                       Point3d(100., 0., 0.),
                                       Point3d(100., 100., 0.),
                                       Point3d(0., 100., 0.)});

        polygon_holes->addInner(hole);

    }

    void teardown()
    {

    }

    Polygon3i  polygon_default_constructor_integer;
    Polygon3i *polygon_initializer_list_constructor;
    Polygon3d *polygon_reserve_constructor;
    Polygon3d *polygon_holes;
};



BOOST_FIXTURE_TEST_CASE(default_constructor, Polygon3DTest)
{
    auto bbox = polygon_default_constructor_integer.boundingBox();

    BOOST_CHECK_EQUAL(0, polygon_default_constructor_integer.outer().size());
    BOOST_CHECK(polygon_default_constructor_integer.type() == GeometryType::polygon);
    BOOST_CHECK_EQUAL(std::numeric_limits<int>::max(), bbox.pt1().x());
    BOOST_CHECK_EQUAL(std::numeric_limits<int>::max(), bbox.pt1().y());
    BOOST_CHECK_EQUAL(std::numeric_limits<int>::max(), bbox.pt1().z());
    BOOST_CHECK_EQUAL(std::numeric_limits<int>::lowest(), bbox.pt2().x());
    BOOST_CHECK_EQUAL(std::numeric_limits<int>::lowest(), bbox.pt2().y());
    BOOST_CHECK_EQUAL(std::numeric_limits<int>::lowest(), bbox.pt2().z());
    BOOST_CHECK_EQUAL(0., polygon_default_constructor_integer.perimeter());
    BOOST_CHECK(Dimension::dim3 == polygon_default_constructor_integer.dimension());
}

BOOST_FIXTURE_TEST_CASE(constructor_reserve, Polygon3DTest)
{
    auto bbox = polygon_reserve_constructor->boundingBox();

    BOOST_CHECK_EQUAL(0, polygon_reserve_constructor->outer().size());
    BOOST_CHECK_EQUAL(10, polygon_reserve_constructor->outer().capacity());
    BOOST_CHECK(polygon_reserve_constructor->type() == GeometryType::polygon);
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::max(), bbox.pt1().x());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::max(), bbox.pt1().y());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::max(), bbox.pt1().z());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::lowest(), bbox.pt2().x());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::lowest(), bbox.pt2().y());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::lowest(), bbox.pt2().z());
    BOOST_CHECK_EQUAL(0., polygon_reserve_constructor->perimeter());
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, Polygon3DTest)
{
    Polygon3d pol_c(*polygon_reserve_constructor);
    BOOST_CHECK_EQUAL(0, pol_c.outer().size());
    BOOST_CHECK_EQUAL(0, pol_c.outer().capacity());

    auto bbox = pol_c.boundingBox();
    BOOST_CHECK(pol_c.type() == GeometryType::polygon);
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::max(), bbox.pt1().x());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::max(), bbox.pt1().y());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::max(), bbox.pt1().z());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::lowest(), bbox.pt2().x());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::lowest(), bbox.pt2().y());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::lowest(), bbox.pt2().z());

    BOOST_CHECK_EQUAL(0., pol_c.perimeter());
}

BOOST_FIXTURE_TEST_CASE(move_constructor, Polygon3DTest)
{
    Polygon3d pol(std::move(Polygon3d()));
    BOOST_CHECK_EQUAL(0, pol.outer().size());

    auto bbox = pol.boundingBox();
    BOOST_CHECK(pol.type() == GeometryType::polygon);
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::max(), bbox.pt1().x());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::max(), bbox.pt1().y());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::max(), bbox.pt1().z());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::lowest(), bbox.pt2().x());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::lowest(), bbox.pt2().y());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::lowest(), bbox.pt2().z());

    BOOST_CHECK_EQUAL(0., pol.perimeter());
}

BOOST_FIXTURE_TEST_CASE(constructor_list, Polygon3DTest)
{
    BOOST_CHECK_EQUAL(7, polygon_initializer_list_constructor->outer().size());

    auto bbox = polygon_initializer_list_constructor->boundingBox();

    BOOST_CHECK_EQUAL(4137012, bbox.pt1().x());
    BOOST_CHECK_EQUAL(642997, bbox.pt1().y());
    BOOST_CHECK_EQUAL(10, bbox.pt1().z());
    BOOST_CHECK_EQUAL(4177148, bbox.pt2().x());
    BOOST_CHECK_EQUAL(702670, bbox.pt2().y());
    BOOST_CHECK_EQUAL(30, bbox.pt2().z());

    BOOST_CHECK_CLOSE(235272.45, polygon_initializer_list_constructor->perimeter(), 0.01);
}

BOOST_FIXTURE_TEST_CASE(type, Polygon3DTest)
{
    BOOST_CHECK(polygon_default_constructor_integer.type() == GeometryType::polygon);
    BOOST_CHECK(polygon_initializer_list_constructor->type() == GeometryType::polygon);
    BOOST_CHECK(polygon_reserve_constructor->type() == GeometryType::polygon);
}

/* Operador de asignación */

BOOST_FIXTURE_TEST_CASE(assing_operator, Polygon3DTest)
{
    Polygon3i polygon;
    polygon = *polygon_initializer_list_constructor;

    BOOST_CHECK_EQUAL(7, polygon.outer().size());

    auto bbox = polygon_initializer_list_constructor->boundingBox();

    BOOST_CHECK_EQUAL(4137012, bbox.pt1().x());
    BOOST_CHECK_EQUAL(642997, bbox.pt1().y());
    BOOST_CHECK_EQUAL(10, bbox.pt1().z());
    BOOST_CHECK_EQUAL(4177148, bbox.pt2().x());
    BOOST_CHECK_EQUAL(702670, bbox.pt2().y());
    BOOST_CHECK_EQUAL(30, bbox.pt2().z());
}

BOOST_FIXTURE_TEST_CASE(move_operator, Polygon3DTest)
{
    Polygon3i polygon_to_move(*polygon_initializer_list_constructor);
    Polygon3i polygon;
    polygon = std::move(polygon_to_move);

    BOOST_CHECK_EQUAL(7, polygon.outer().size());

    for (int i = 0; i < polygon.outer().size(); i++) {
        BOOST_CHECK(polygon.outer()[i] == polygon_initializer_list_constructor->outer()[i]);
    }
}

BOOST_FIXTURE_TEST_CASE(holes, Polygon3DTest)
{
    BOOST_CHECK_EQUAL(1, polygon_holes->numInners());
    auto &hole = polygon_holes->inner(0);
    BOOST_CHECK_EQUAL(3, hole.size());
}

BOOST_AUTO_TEST_SUITE_END()




/* MultiPolygon  */

//BOOST_AUTO_TEST_SUITE(MultiPolygonTestSuite)
//
//
//struct MultiPolygonTest
//{
//    MultiPolygonTest()
//    {
//
//    }
//
//    ~MultiPolygonTest()
//    {
//    }
//
//    void setup()
//    {
//        PolygonI polygon{
//          Point<int>(4157222, 664789),
//          Point<int>(4149043, 688836),
//          Point<int>(4172803, 690340),
//          Point<int>(4177148, 642997),
//          Point<int>(4137012, 671808),
//          Point<int>(4146292, 666953),
//          Point<int>(4138759, 702670)
//        };
//
//        multipolygon2.push_back(polygon);
//    }
//
//    void teardown()
//    {
//
//    }
//
//    MultiPolygon<Point<int>> multipolygon1;
//    MultiPolygon<Point<int>> multipolygon2;
//
//};

//BOOST_FIXTURE_TEST_CASE(default_constructor, MultiPolygonTest) 
//{
//  BOOST_CHECK_EQUAL(0, multipolygon1.size());
//  BOOST_CHECK(multipolygon1.type() == Entity::Type::multipolygon2d);
//  BOOST_CHECK(false == multipolygon1.is3D());
//  WindowI w = multipolygon1.window();
//  BOOST_CHECK_EQUAL(std::numeric_limits<int>::max(), w.pt1().x());
//  BOOST_CHECK_EQUAL(std::numeric_limits<int>::max(), w.pt1().y());
//  BOOST_CHECK_EQUAL(std::numeric_limits<int>::lowest(), w.pt2().x());
//  BOOST_CHECK_EQUAL(std::numeric_limits<int>::lowest(), w.pt2().y());
//}
//
///* Constructor reserve */
//
//BOOST_FIXTURE_TEST_CASE(constructor_reserve, MultiPolygonTest) 
//{
//  MultiPolygon<Point<int>> multipolygon(10);
//
//  BOOST_CHECK_EQUAL(10, multipolygon.size());
//  BOOST_CHECK(multipolygon.type() == Entity::Type::multipolygon2d);
//  BOOST_CHECK(false == multipolygon.is3D());
//  WindowI w = multipolygon.window();
//  BOOST_CHECK_EQUAL(std::numeric_limits<int>::max(), w.pt1().x());
//  BOOST_CHECK_EQUAL(std::numeric_limits<int>::max(), w.pt1().y());
//  BOOST_CHECK_EQUAL(std::numeric_limits<int>::lowest(), w.pt2().x());
//  BOOST_CHECK_EQUAL(std::numeric_limits<int>::lowest(), w.pt2().y());
//}
//
///*Constructor de copia*/
//
//BOOST_FIXTURE_TEST_CASE(copy_constructor, MultiPolygonTest)
//{
//	MultiPolygon<Point<int>> multipolygon_copy(multipolygon2);
//  BOOST_CHECK_EQUAL(1, multipolygon_copy.size());
//  BOOST_CHECK(multipolygon_copy.type() == Entity::Type::multipolygon2d);
//  BOOST_CHECK(false == multipolygon_copy.is3D());
//  WindowI w = multipolygon_copy.window();
//  BOOST_CHECK_EQUAL(4137012, w.pt1().x());
//  BOOST_CHECK_EQUAL(642997, w.pt1().y());
//  BOOST_CHECK_EQUAL(4177148, w.pt2().x());
//  BOOST_CHECK_EQUAL(702670, w.pt2().y());
//}
//
///* Operador de asignación */
//
//BOOST_FIXTURE_TEST_CASE(assing_operator, MultiPolygonTest)
//{
//  MultiPolygon<Point<int>> multipolygon_copy;
//  multipolygon_copy = multipolygon2;
//  BOOST_CHECK_EQUAL(1, multipolygon_copy.size());
//  BOOST_CHECK(multipolygon_copy.type() == Entity::Type::multipolygon2d);
//  BOOST_CHECK(false == multipolygon_copy.is3D());
//  WindowI w = multipolygon_copy.window();
//  BOOST_CHECK_EQUAL(4137012, w.pt1().x());
//  BOOST_CHECK_EQUAL(642997, w.pt1().y());
//  BOOST_CHECK_EQUAL(4177148, w.pt2().x());
//  BOOST_CHECK_EQUAL(702670, w.pt2().y());
//}
//
//BOOST_AUTO_TEST_SUITE_END()
//
//
///* MultiPolygon3D */
//
//BOOST_AUTO_TEST_SUITE(MultiPolygon3DTestSuite)
//
//
//struct MultiPolygon3DTest
//{
//  MultiPolygon3DTest()
//  {
//
//  }
//  ~MultiPolygon3DTest()
//  {
//  }
//
//  void setup()
//  {
//    Polygon3dI polygon_initializer_list_constructor{
//      Point3i(4157222, 664789, 10),
//      Point3i(4149043, 688836, 20),
//      Point3i(4172803, 690340, 30),
//      Point3i(4177148, 642997, 25),
//      Point3i(4137012, 671808, 20),
//      Point3i(4146292, 666953, 15),
//      Point3i(4138759, 702670, 10) 
//    };
//    multipolygon2.push_back(polygon_initializer_list_constructor);
//  }
//
//  void teardown()
//  {
//
//  }
//
//  MultiPolygon3D<Point3i> multipolygon1;
//  MultiPolygon3D<Point3i> multipolygon2;
//};
//
///* Constructor por defecto */
//
//BOOST_FIXTURE_TEST_CASE(default_constructor, MultiPolygon3DTest) 
//{
//
//  BoundingBoxI box = multipolygon1.boundingBox();
//  BOOST_CHECK_EQUAL(0, multipolygon1.size());
//  BOOST_CHECK(multipolygon1.type() == Entity::Type::multipolygon3d);
//  BOOST_CHECK(multipolygon1.is3D());
//  BOOST_CHECK_EQUAL(std::numeric_limits<int>::max(), box.pt1().x());
//  BOOST_CHECK_EQUAL(std::numeric_limits<int>::max(), box.pt1().y());
//  BOOST_CHECK_EQUAL(std::numeric_limits<int>::max(), box.pt1.z);
//  BOOST_CHECK_EQUAL(std::numeric_limits<int>::lowest(), box.pt2().x());
//  BOOST_CHECK_EQUAL(std::numeric_limits<int>::lowest(), box.pt2().y());
//  BOOST_CHECK_EQUAL(std::numeric_limits<int>::lowest(), box.pt2.z);
//}
//
///* Constructor reserve */
//
//BOOST_FIXTURE_TEST_CASE(constructor_reserve, MultiPolygon3DTest) 
//{
//  MultiPolygon3D<Point3<double>> multipolygon(10);
//  BoundingBoxD box = multipolygon.boundingBox();
//  
//  BOOST_CHECK_EQUAL(10, multipolygon.size());
//  BOOST_CHECK(multipolygon.type() == Entity::Type::multipolygon3d);
//  BOOST_CHECK_EQUAL(std::numeric_limits<double>::max(), box.pt1().x());
//  BOOST_CHECK_EQUAL(std::numeric_limits<double>::max(), box.pt1().y());
//  BOOST_CHECK_EQUAL(std::numeric_limits<double>::max(), box.pt1.z);
//  BOOST_CHECK_EQUAL(std::numeric_limits<double>::lowest(), box.pt2().x());
//  BOOST_CHECK_EQUAL(std::numeric_limits<double>::lowest(), box.pt2().y());
//  BOOST_CHECK_EQUAL(std::numeric_limits<double>::lowest(), box.pt2.z);
//}
//
///*Constructor de copia*/
//
//BOOST_FIXTURE_TEST_CASE(copy_constructor, MultiPolygon3DTest) 
//{
//  MultiPolygon3D<Point3i> multipolygon_copy(multipolygon2);
//  
//  BOOST_CHECK(multipolygon_copy.type() == Entity::Type::multipolygon3d);
//  BOOST_CHECK(multipolygon_copy.is3D());
//	BOOST_CHECK_EQUAL(1, multipolygon_copy.size());
//  BoundingBoxD bbox = multipolygon_copy.boundingBox();
//  BOOST_CHECK_EQUAL(4137012, bbox.pt1().x());
//  BOOST_CHECK_EQUAL(642997, bbox.pt1().y());
//  BOOST_CHECK_EQUAL(10, bbox.pt1.z);
//  BOOST_CHECK_EQUAL(4177148, bbox.pt2().x());
//  BOOST_CHECK_EQUAL(702670, bbox.pt2().y());
//  BOOST_CHECK_EQUAL(30, bbox.pt2.z);
//}
//
///* Operador de asignación */
//
//BOOST_FIXTURE_TEST_CASE(assing_operator, MultiPolygon3DTest)
//{
//  MultiPolygon3D<Point3i> multipolygon_copy;
//  multipolygon_copy = multipolygon2;
//  
//  BOOST_CHECK(multipolygon_copy.type() == Entity::Type::multipolygon3d);
//  BOOST_CHECK(multipolygon_copy.is3D());
//
//	BOOST_CHECK_EQUAL(1, multipolygon_copy.size());
//  BoundingBoxD bbox = multipolygon_copy.boundingBox();
//  BOOST_CHECK_EQUAL(4137012, bbox.pt1().x());
//  BOOST_CHECK_EQUAL(642997, bbox.pt1().y());
//  BOOST_CHECK_EQUAL(10, bbox.pt1.z);
//  BOOST_CHECK_EQUAL(4177148, bbox.pt2().x());
//  BOOST_CHECK_EQUAL(702670, bbox.pt2().y());
//  BOOST_CHECK_EQUAL(30, bbox.pt2.z);
//}
//
//BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(MultiGeometryTests)

BOOST_AUTO_TEST_CASE(multi_polygon_area_complex)
{
    Polygon2d island1;
    island1.outer().push_back(Point2d(0, 0));
    island1.outer().push_back(Point2d(10, 0));
    island1.outer().push_back(Point2d(10, 10));
    island1.outer().push_back(Point2d(0, 10));

    Polygon2d island2;
    island2.outer().push_back(Point2d(20, 0));
    island2.outer().push_back(Point2d(30, 0));
    island2.outer().push_back(Point2d(30, 10));
    island2.outer().push_back(Point2d(20, 10));

    LinearRing2d lake;
    lake.push_back(Point2d(24, 4));
    lake.push_back(Point2d(26, 4));
    lake.push_back(Point2d(26, 6));
    lake.push_back(Point2d(24, 6));
    island2.addInner(lake);

    MultiPolygon2d archipelago;
    archipelago.push_back(island1);
    archipelago.push_back(island2);

    BOOST_CHECK_CLOSE(area(archipelago), 196.0, 0.001);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_CASE(test_polygon_2d_wkt_output)
{
    // Crear anillo exterior (cuadrado)
    tl::LinearRing<Point2d> outer = {
        Point2d(0,0),
        Point2d(10,0),
        Point2d(10,10),
        Point2d(0,10),
        Point2d(0,0)
    };

    // Crear hueco interior
    tl::LinearRing<Point2d> inner = {
        Point2d(2,2),
        Point2d(8,2),
        Point2d(8,8),
        Point2d(2,8),
        Point2d(2,2)
    };

    tl::Polygon<Point2d> poly(outer, {inner});

    // Test con streams y precisión
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << wkt(poly);

    std::string expected = "POLYGON ((0.0 0.0, 10.0 0.0, 10.0 10.0, 0.0 10.0, 0.0 0.0), "
        "(2.0 2.0, 8.0 2.0, 8.0 8.0, 2.0 8.0, 2.0 2.0))";

    BOOST_CHECK_EQUAL(ss.str(), expected);

    // Test con format moderno
    std::string fmt_out = tl::format("{:.0f}", wkt(poly));
    BOOST_CHECK(fmt_out.find("POLYGON ((0 0") != std::string::npos);

    fmt_out = tl::format("{:.2f}", wkt(poly));
    BOOST_CHECK(fmt_out.find("2.00 8.00") != std::string::npos);
}
BOOST_AUTO_TEST_CASE(test_polygon_3d_wkt_output)
{
    // Crear anillo exterior (cuadrado)
    LinearRing<Point3d> outer = {
        Point3d(0,0,0), 
        Point3d(10,0,0),
        Point3d(10,10,0), 
        Point3d(0,10,0), 
        Point3d(0,0,0)
    };

    // Crear hueco interior
    LinearRing<Point3d> inner = {
        Point3d(2,2,0),
        Point3d(8,2,0), 
        Point3d(8,8,0), 
        Point3d(2,8,0), 
        Point3d(2,2,0)
    };

    tl::Polygon<Point3d> poly(outer, {inner});

    // Test con streams y precisión
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << wkt(poly);

    std::string expected = "POLYGON Z ((0.0 0.0 0.0, 10.0 0.0 0.0, 10.0 10.0 0.0, 0.0 10.0 0.0, 0.0 0.0 0.0), "
        "(2.0 2.0 0.0, 8.0 2.0 0.0, 8.0 8.0 0.0, 2.0 8.0 0.0, 2.0 2.0 0.0))";

    BOOST_CHECK_EQUAL(ss.str(), expected);

    // Test con format moderno
    std::string fmt_out = tl::format("{:.0f}", wkt(poly));
    BOOST_CHECK(fmt_out.find("POLYGON Z ((0 0 0") != std::string::npos);

    fmt_out = tl::format("{:.2f}", wkt(poly));
    BOOST_CHECK(fmt_out.find("2.00 8.00 0.00") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(test_polygon_zm_wkt)
{
    LinearRing<Point3dm> outer = {
        Point3dm(0, 0, 0, 1),
        Point3dm(10, 0, 0, 1),
        Point3dm(0, 10, 0, 1),
        Point3dm(0, 0, 0, 1)
    };

    tl::Polygon<Point3dm> poly(outer);

    std::stringstream ss;
    ss << wkt(poly);

    BOOST_CHECK(ss.str().find("POLYGON ZM ((") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(test_multipolygon_zm_wkt)
{
    LinearRing<Point3dm> outer1 = {
        Point3dm(0, 0, 0, 1),
        Point3dm(1, 1, 0, 1),
        Point3dm(0, 0, 0, 1)
    };

    tl::LinearRing<Point3dm> outer2 = {
        Point3dm(5, 5, 0, 1),
        Point3dm(6, 6, 0, 1),
        Point3dm(5, 5, 0, 1)
    };

    // Añadir dos polígonos simples
    tl::Polygon<Point3dm> p1(outer1);
    tl::Polygon<Point3dm> p2(outer2);

    MultiPolygon<Point3dm> mpoly;
    mpoly.push_back(p1);
    mpoly.push_back(p2);

    std::stringstream ss;
    ss << wkt(mpoly);

    // Verificamos que el prefijo es correcto y hay triple paréntesis al inicio
    std::string result = ss.str();
    BOOST_CHECK(result.find("MULTIPOLYGON ZM") != std::string::npos);
    BOOST_CHECK(result.find("(((") != std::string::npos);
}