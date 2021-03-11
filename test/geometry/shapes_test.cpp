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
 * License along with Foobar. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/
 
#define BOOST_TEST_MODULE Tidop Shapes test
#include <boost/test/unit_test.hpp>
#include <tidop/geometry/entities/shapes.h>

using namespace tl;

/* Circle */

/* Constructor por defecto */

BOOST_AUTO_TEST_CASE(CircleI_default_constructor)
{
  const CircleI circle;
  BOOST_CHECK_EQUAL(0, circle.center.x);
  BOOST_CHECK_EQUAL(0, circle.center.y);
  BOOST_CHECK_EQUAL(1, circle.radius);
  BOOST_CHECK(circle.type() == Entity::Type::circle);
  BOOST_CHECK(false == circle.is3D());
}

BOOST_AUTO_TEST_CASE(CircleD_default_constructor)
{
  const CircleD circle;
  BOOST_CHECK_EQUAL(0., circle.center.x);
  BOOST_CHECK_EQUAL(0., circle.center.y);
  BOOST_CHECK_EQUAL(1., circle.radius);
  BOOST_CHECK(circle.Entity::type() == Entity::Type::circle);
}

BOOST_AUTO_TEST_CASE(CircleF_default_constructor)
{
  const CircleF circle;
  BOOST_CHECK_EQUAL(0.f, circle.center.x);
  BOOST_CHECK_EQUAL(0.f, circle.center.y);
  BOOST_CHECK_EQUAL(1.f, circle.radius);
  BOOST_CHECK(circle.type() == Entity::Type::circle);
}

// Constructor Centro y radio

BOOST_AUTO_TEST_CASE(CircleD_copy_center_radius)
{
  const CircleD circle(PointD(50.5, 32.65), 25.);
  BOOST_CHECK_EQUAL(50.5, circle.center.x);
  BOOST_CHECK_EQUAL(32.65, circle.center.y);
  BOOST_CHECK_EQUAL(25., circle.radius);
  BOOST_CHECK(circle.type() == Entity::Type::circle);
}

// Constructor de copia

BOOST_AUTO_TEST_CASE(CircleD_copy_constructor)
{
  const CircleD circle(PointD(50.5, 32.65), 25.);
  CircleD circle2(circle);
  BOOST_CHECK_EQUAL(50.5, circle2.center.x);
  BOOST_CHECK_EQUAL(32.65, circle2.center.y);
  BOOST_CHECK_EQUAL(25., circle2.radius);
  BOOST_CHECK(circle2.type() == Entity::Type::circle);
}


/* Operador de asignación */

BOOST_AUTO_TEST_CASE(CircleD_assignment)
{
  const CircleD circle(PointD(50.5, 32.65), 25.);

  CircleD circle2 = circle;
  BOOST_CHECK_EQUAL(50.5, circle2.center.x);
  BOOST_CHECK_EQUAL(32.65, circle2.center.y);
  BOOST_CHECK_EQUAL(25., circle2.radius);
  BOOST_CHECK(circle2.type() == Entity::Type::circle);
}

/* Conversión a un tipo diferente */

BOOST_AUTO_TEST_CASE(CircleD_conversion)
{
  CircleI circle(PointI(50, 32), 25);
  CircleD circle2 = static_cast<Circle<double>>(circle);
  BOOST_CHECK_EQUAL(50., circle2.center.x);
  BOOST_CHECK_EQUAL(32., circle2.center.y);
  BOOST_CHECK_EQUAL(25., circle2.radius);
  BOOST_CHECK(circle2.type() == Entity::Type::circle);
}

BOOST_AUTO_TEST_CASE(CircleD_area)
{
  CircleD circle(PointD(50., 50.), 25.);
  BOOST_CHECK_CLOSE(1963.495, circle.area(), 0.1);
}

BOOST_AUTO_TEST_CASE(CircleD_length)
{
  CircleD circle(PointD(50., 50.), 25.);
  BOOST_CHECK_CLOSE(157.0796, circle.length(), 0.01);
}


/* Ellipse */

// Constructor por defecto

BOOST_AUTO_TEST_CASE(EllipseI_DefaultConstructor)
{
  const EllipseI ellipse;
  BOOST_CHECK_EQUAL(0, ellipse.center.x);
  BOOST_CHECK_EQUAL(0, ellipse.center.y);
  BOOST_CHECK_EQUAL(1, ellipse.a);
  BOOST_CHECK_EQUAL(1, ellipse.b);
  BOOST_CHECK(ellipse.type() == Entity::Type::ellipse);
  BOOST_CHECK(false == ellipse.is3D());
}

BOOST_AUTO_TEST_CASE(EllipseD_DefaultConstructor)
{
  const EllipseD ellipse;
  BOOST_CHECK_EQUAL(0., ellipse.center.x);
  BOOST_CHECK_EQUAL(0., ellipse.center.y);
  BOOST_CHECK_EQUAL(1., ellipse.a);
  BOOST_CHECK_EQUAL(1., ellipse.b);
  BOOST_CHECK(ellipse.type() == Entity::Type::ellipse);
}

BOOST_AUTO_TEST_CASE(EllipseF_DefaultConstructor)
{
  const EllipseF ellipse;
  BOOST_CHECK_EQUAL(0.f, ellipse.center.x);
  BOOST_CHECK_EQUAL(0.f, ellipse.center.y);
  BOOST_CHECK_EQUAL(1.f, ellipse.a);
  BOOST_CHECK_EQUAL(1.f, ellipse.b);
  BOOST_CHECK(ellipse.type() == Entity::Type::ellipse);
}

// Constructor Centro y semiejes

BOOST_AUTO_TEST_CASE(EllipseD_Constructor) 
{
  const EllipseD ellipse(PointD(50.5, 32.65), 25., 10.);
  BOOST_CHECK_EQUAL(50.5, ellipse.center.x);
  BOOST_CHECK_EQUAL(32.65, ellipse.center.y);
  BOOST_CHECK_EQUAL(25., ellipse.a);
  BOOST_CHECK_EQUAL(10., ellipse.b);
  BOOST_CHECK(ellipse.type() == Entity::Type::ellipse);
}

// Constructor de copia

BOOST_AUTO_TEST_CASE(EllipseD_CopyConstructor) 
{
  EllipseD ellipse(PointD(50.5, 32.65), 25., 10.);
  EllipseD ellipse2(ellipse);

  BOOST_CHECK_EQUAL(50.5, ellipse2.center.x);
  BOOST_CHECK_EQUAL(32.65, ellipse2.center.y);
  BOOST_CHECK_EQUAL(25., ellipse2.a);
  BOOST_CHECK_EQUAL(10., ellipse2.b);
  BOOST_CHECK(ellipse2.type() == Entity::Type::ellipse);
}

// Operador de asignación
BOOST_AUTO_TEST_CASE(EllipseD_assignment)
{
  const EllipseD ellipse(PointD(50.5, 32.65), 25., 10.);

  EllipseD ellipse2 = ellipse;
  BOOST_CHECK_EQUAL(50.5, ellipse2.center.x);
  BOOST_CHECK_EQUAL(32.65, ellipse2.center.y);
  BOOST_CHECK_EQUAL(25., ellipse2.a);
  BOOST_CHECK_EQUAL(10., ellipse2.b);
  BOOST_CHECK(ellipse2.type() == Entity::Type::ellipse);
}

BOOST_AUTO_TEST_CASE(EllipseD_conversion)
{
  EllipseI ellipse(PointI(50, 32), 25, 10);
  EllipseD ellipse2 = static_cast<EllipseD>(ellipse);
  BOOST_CHECK_EQUAL(50., ellipse2.center.x);
  BOOST_CHECK_EQUAL(32., ellipse2.center.y);
  BOOST_CHECK_EQUAL(25., ellipse2.a);
  BOOST_CHECK_EQUAL(10., ellipse2.b);
  BOOST_CHECK(ellipse2.type() == Entity::Type::ellipse);
}

BOOST_AUTO_TEST_CASE(EllipseD_area)
{
  EllipseD ellipse(PointD(50., 50.), 3., 2.);
  BOOST_CHECK_CLOSE(18.85, ellipse.area(), 0.01);
}

BOOST_AUTO_TEST_CASE(EllipseD_length)
{
  EllipseD ellipse(PointD(50., 50.), 3., 2.);
  BOOST_CHECK_CLOSE(15.87, ellipse.length(), 0.1);
}


/* Triangle */

// Constructor por defecto

BOOST_AUTO_TEST_CASE(TriangleI_DefaultConstructor)
{
  Triangle<int> triangle;
  BOOST_CHECK(triangle.type() == Entity::Type::triangle);
  BOOST_CHECK(false == triangle.is3D());

}

//BOOST_AUTO_TEST_CASE(TriangleD_DefaultConstructor)
//{
//  const TriangleD triangle;
//  BOOST_CHECK_EQUAL(0., triangle.center.x);
//  BOOST_CHECK_EQUAL(0., triangle.center.y);
//  BOOST_CHECK_EQUAL(1., triangle.a);
//  BOOST_CHECK_EQUAL(1., triangle.b);
//  BOOST_CHECK(triangle.type() == Entity::Type::triangle);
//}
//
//BOOST_AUTO_TEST_CASE(TriangleF_DefaultConstructor)
//{
//  const TriangleF triangle;
//  BOOST_CHECK_EQUAL(0.f, triangle.center.x);
//  BOOST_CHECK_EQUAL(0.f, triangle.center.y);
//  BOOST_CHECK_EQUAL(1.f, triangle.a);
//  BOOST_CHECK_EQUAL(1.f, triangle.b);
//  BOOST_CHECK(triangle.type() == Entity::Type::triangle);
//}
//
//// Constructor Centro y semiejes
//
//BOOST_AUTO_TEST_CASE(TriangleD_Constructor) 
//{
//  const TriangleD triangle(PointD(50.5, 32.65), 25., 10.);
//  BOOST_CHECK_EQUAL(50.5, triangle.center.x);
//  BOOST_CHECK_EQUAL(32.65, triangle.center.y);
//  BOOST_CHECK_EQUAL(25., triangle.a);
//  BOOST_CHECK_EQUAL(10., triangle.b);
//  BOOST_CHECK(triangle.type() == Entity::Type::triangle);
//}
//
//// Constructor de copia
//
//BOOST_AUTO_TEST_CASE(TriangleD_CopyConstructor) 
//{
//  TriangleD triangle(PointD(50.5, 32.65), 25., 10.);
//  TriangleD triangle2(triangle);
//
//  BOOST_CHECK_EQUAL(50.5, triangle2.center.x);
//  BOOST_CHECK_EQUAL(32.65, triangle2.center.y);
//  BOOST_CHECK_EQUAL(25., triangle2.a);
//  BOOST_CHECK_EQUAL(10., triangle2.b);
//  BOOST_CHECK(triangle2.type() == Entity::Type::triangle);
//}
//
//// Operador de asignación
//BOOST_AUTO_TEST_CASE(TriangleD_assignment)
//{
//  const TriangleD triangle(PointD(50.5, 32.65), 25., 10.);
//
//  TriangleD triangle2 = triangle;
//  BOOST_CHECK_EQUAL(50.5, triangle2.center.x);
//  BOOST_CHECK_EQUAL(32.65, triangle2.center.y);
//  BOOST_CHECK_EQUAL(25., triangle2.a);
//  BOOST_CHECK_EQUAL(10., triangle2.b);
//  BOOST_CHECK(triangle2.type() == Entity::Type::triangle);
//}
//
//BOOST_AUTO_TEST_CASE(TriangleD_conversion)
//{
//  TriangleI triangle(PointI(50, 32), 25, 10);
//  TriangleD triangle2 = static_cast<TriangleD>(triangle);
//  BOOST_CHECK_EQUAL(50., triangle2.center.x);
//  BOOST_CHECK_EQUAL(32., triangle2.center.y);
//  BOOST_CHECK_EQUAL(25., triangle2.a);
//  BOOST_CHECK_EQUAL(10., triangle2.b);
//  BOOST_CHECK(triangle2.type() == Entity::Type::triangle);
//}
//
//BOOST_AUTO_TEST_CASE(TriangleD_area)
//{
//  TriangleD triangle(PointD(50., 50.), 3., 2.);
//  BOOST_CHECK_CLOSE(18.85, triangle.area(), 0.01);
//}
//
//BOOST_AUTO_TEST_CASE(TriangleD_length)
//{
//  TriangleD triangle(PointD(50., 50.), 3., 2.);
//  BOOST_CHECK_CLOSE(15.87, triangle.length(), 0.1);
//}
//
