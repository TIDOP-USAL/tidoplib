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
 
#define BOOST_TEST_MODULE Tidop Argument test
#include <boost/test/unit_test.hpp>
#include <tidop/geometry/primitives/Segment.h>
//#include <tidop/geometry/bbox.h>

using namespace tl;
using namespace tl::geometry;

/* Segment */

/* Constructor por defecto */

BOOST_AUTO_TEST_CASE(Segment_DefaultConstructor)
{
    Segment<Point2d> segment;
    BOOST_CHECK(segment.type() == GeometryType::segment);
    BOOST_CHECK(segment.dimension() == Dimension::dim2);
    BOOST_CHECK_EQUAL(0., segment.pt1().x());
    BOOST_CHECK_EQUAL(0., segment.pt1().y());
    BOOST_CHECK_EQUAL(0., segment.pt2().x());
    BOOST_CHECK_EQUAL(0., segment.pt2().y());
}

/* Constructor punto inicial y final del segmento */

BOOST_AUTO_TEST_CASE(Segment_ConstructorPointIniEnd)
{
    Segment<Point2d> segment(Point2d(5.5, 93.2), Point2d(25.3, 654.4));
    BOOST_CHECK_EQUAL(5.5, segment.pt1().x());
    BOOST_CHECK_EQUAL(93.2, segment.pt1().y());
    BOOST_CHECK_EQUAL(25.3, segment.pt2().x());
    BOOST_CHECK_EQUAL(654.4, segment.pt2().y());
}

/* Constructor punto, angulo, longitud y centro */

BOOST_AUTO_TEST_CASE(Segment_ConstructorCenterAngleLenghtCenter)
{
    Segment<Point2d> segment(Point2d(5., 10.), 45. * consts::deg_to_rad<double>, 10.);
    BOOST_CHECK_CLOSE(8.5355339059327378, segment.pt1().x(), 0.1);
    BOOST_CHECK_CLOSE(6.4644660940672622, segment.pt1().y(), 0.1);
    BOOST_CHECK_CLOSE(1.4644660940672622, segment.pt2().x(), 0.1);
    BOOST_CHECK_CLOSE(13.535533905932738, segment.pt2().y(), 0.1);

    Segment<Point2i> segment2(Point2i(5, 10), 45. * consts::deg_to_rad<double>, 10.);
    BOOST_CHECK_EQUAL(9, segment2.pt1().x());
    BOOST_CHECK_EQUAL(6, segment2.pt1().y());
    BOOST_CHECK_EQUAL(1, segment2.pt2().x());
    BOOST_CHECK_EQUAL(14, segment2.pt2().y());
}

/* Constructor de copia */

BOOST_AUTO_TEST_CASE(Segment_CopyConstructor)
{
    Segment<Point2d> segment(Point2d(5.5, 93.2), Point2d(25.3, 654.4));
    Segment<Point2d> segment_c(segment);
    BOOST_CHECK_EQUAL(5.5, segment_c.pt1().x());
    BOOST_CHECK_EQUAL(93.2, segment_c.pt1().y());
    BOOST_CHECK_EQUAL(25.3, segment_c.pt2().x());
    BOOST_CHECK_EQUAL(654.4, segment_c.pt2().y());
}

/* Operador de asignación */

BOOST_AUTO_TEST_CASE(Segment_assignment)
{
    Segment<Point2d> segment(Point2d(5.5, 93.2), Point2d(25.3, 654.4));

    Segment<Point2d> segment_a = segment;

    BOOST_CHECK_EQUAL(5.5, segment_a.pt1().x());
    BOOST_CHECK_EQUAL(93.2, segment_a.pt1().y());
    BOOST_CHECK_EQUAL(25.3, segment_a.pt2().x());
    BOOST_CHECK_EQUAL(654.4, segment_a.pt2().y());
}

/* Conversión a un tipo diferente */

BOOST_AUTO_TEST_CASE(Segment_conversion)
{
    Segment<Point2d> segment(Point2d(5.5, 93.2), Point2d(25.3, 654.4));
    Segment<Point2i> segment_int = static_cast<Segment<Point2i>>(segment);
    BOOST_CHECK_EQUAL(6, segment_int.pt1().x());
    BOOST_CHECK_EQUAL(93, segment_int.pt1().y());
    BOOST_CHECK_EQUAL(25, segment_int.pt2().x());
    BOOST_CHECK_EQUAL(654, segment_int.pt2().y());

    Segment<Point2f> segment_float(Point2f(5.5f, 93.2f), Point2f(25.3f, 654.4f));
    Segment<Point2d> segment_double = static_cast<Segment<Point2d>>(segment_float);
    BOOST_CHECK_CLOSE(5.5, segment_double.pt1().x(), 0.01);
    BOOST_CHECK_CLOSE(93.2, segment_double.pt1().y(), 0.01);
    BOOST_CHECK_CLOSE(25.3, segment_double.pt2().x(), 0.01);
    BOOST_CHECK_CLOSE(654.4, segment_double.pt2().y(), 0.01);
}

///* Angulo medido respecto al eje x */
//
//BOOST_AUTO_TEST_CASE(Segment_angleOX)
//{
//  Segment<Point2d> segment(Point2d(5.5, 93.2), Point2d(25.3, 654.4));
//  BOOST_CHECK_CLOSE(1.5355294, segment.angleOX(), 0.001);
//}
//
///* Angulo medido respecto al eje y */
//
//BOOST_AUTO_TEST_CASE(Segment_angleOY)
//{
//  Segment<Point2d> segment(Point2d(5.5, 93.2), Point2d(25.3, 654.4));
//  BOOST_CHECK_CLOSE(0.0352669, segment.angleOY(), 0.001);
//}

/* Ventana envolvente */
BOOST_AUTO_TEST_CASE(Segment_window)
{
    Segment<Point2d> segment(Point2d(56.23, 123.5), Point2d(96.2, 34.4));
    BoundingBox<Point2d> w_s = segment.boundingBox();
    BoundingBox<Point2d> w(Point2d(56.23, 34.4), Point2d(96.2, 123.5));
    BOOST_CHECK_EQUAL(w.pt1().x(), w_s.pt1().x());
    BOOST_CHECK_EQUAL(w.pt1().y(), w_s.pt1().y());
    BOOST_CHECK_EQUAL(w.pt2().x(), w_s.pt2().x());
    BOOST_CHECK_EQUAL(w.pt2().y(), w_s.pt2().y());
}

/* Comprueba si el segmento esta vacio */

BOOST_AUTO_TEST_CASE(Segment_empty)
{
    Segment<Point2d> segment(Point2d(56.23, 123.5), Point2d(96.2, 34.4));

    BOOST_CHECK(false == segment.isEmpty());

    Segment<Point2d> segment2;
    BOOST_CHECK(segment2.isEmpty());
}

BOOST_AUTO_TEST_CASE(Segment_length)
{
    Segment<Point2d> segment(Point2d(56.23, 123.5), Point2d(96.2, 34.4));
    BOOST_CHECK_CLOSE(97.6545, segment.length(), 0.1);
}

BOOST_AUTO_TEST_CASE(Segment_vector)
{
    Segment<Point2d> segment(Point2d(56.23, 123.5), Point2d(96.2, 34.4));
    Vector<double, 2> v = segment.vector();
    BOOST_CHECK_CLOSE(39.97, v.x(), 0.1);
    BOOST_CHECK_CLOSE(-89.099, v.y(), 0.1);
}

//TL_TODO("Falta test Segment::isNear()")
//TL_TODO("Falta test Segment::isParallel()")
//
//BOOST_AUTO_TEST_CASE(Segment_split)
//{
//  Segment<Point2d> segment_horizontal(Point2d(0, 0), Point2d(100, 0));
//  std::vector<Segment<Point2d>> horizontal_segments = segment_horizontal.split(2);
//  BOOST_CHECK_CLOSE(0., horizontal_segments[0].pt1().x(), 0.1);
//  BOOST_CHECK_CLOSE(0., horizontal_segments[0].pt1().y(), 0.1);
//  BOOST_CHECK_CLOSE(50., horizontal_segments[0].pt2().x(), 0.1);
//  BOOST_CHECK_CLOSE(0, horizontal_segments[0].pt2().y(), 0.1);
//  BOOST_CHECK_CLOSE(50., horizontal_segments[1].pt1().x(), 0.1);
//  BOOST_CHECK_CLOSE(0, horizontal_segments[1].pt1().y(), 0.1);
//  BOOST_CHECK_CLOSE(100., horizontal_segments[1].pt2().x(), 0.1);
//  BOOST_CHECK_CLOSE(0., horizontal_segments[1].pt2().y(), 0.1);
//
//  Segment<Point2d> segment_vertical(Point2d(0, 0), Point2d(0, 100));
//  std::vector<Segment<Point2d>> vertical_segments = segment_vertical.split(2);
//  BOOST_CHECK_CLOSE(0., vertical_segments[0].pt1().x(), 0.1);
//  BOOST_CHECK_CLOSE(0., vertical_segments[0].pt1().y(), 0.1);
//  BOOST_CHECK_CLOSE(0., vertical_segments[0].pt2().x(), 0.1);
//  BOOST_CHECK_CLOSE(50., vertical_segments[0].pt2().y(), 0.1);
//  BOOST_CHECK_CLOSE(0., vertical_segments[1].pt1().x(), 0.1);
//  BOOST_CHECK_CLOSE(50., vertical_segments[1].pt1().y(), 0.1);
//  BOOST_CHECK_CLOSE(0., vertical_segments[1].pt2().x(), 0.1);
//  BOOST_CHECK_CLOSE(100., vertical_segments[1].pt2().y(), 0.1);
//
//  Segment<Point2d> segment(Point2d(0, 0), Point2d(100, 100));
//  std::vector<Segment<Point2d>> segments = segment.split(2);
//  BOOST_CHECK_CLOSE(0., segments[0].pt1().x(), 0.1);
//  BOOST_CHECK_CLOSE(0., segments[0].pt1().y(), 0.1);
//  BOOST_CHECK_CLOSE(50., segments[0].pt2().x(), 0.1);
//  BOOST_CHECK_CLOSE(50., segments[0].pt2().y(), 0.1);
//  BOOST_CHECK_CLOSE(50., segments[1].pt1().x(), 0.1);
//  BOOST_CHECK_CLOSE(50., segments[1].pt1().y(), 0.1);
//  BOOST_CHECK_CLOSE(100., segments[1].pt2().x(), 0.1);
//  BOOST_CHECK_CLOSE(100., segments[1].pt2().y(), 0.1);
//
//  Segment<Point2d> segment3(Point2d(0, 0), Point2d(100, 100));
//  std::vector<Segment<Point2d>> segments3 = segment3.split(3);
//  BOOST_CHECK_CLOSE(0., segments3[0].pt1().x(), 0.1);
//  BOOST_CHECK_CLOSE(0., segments3[0].pt1().y(), 0.1);
//  BOOST_CHECK_CLOSE(33.33333, segments3[0].pt2().x(), 0.1);
//  BOOST_CHECK_CLOSE(33.33333, segments3[0].pt2().y(), 0.1);
//  BOOST_CHECK_CLOSE(33.33333, segments3[1].pt1().x(), 0.1);
//  BOOST_CHECK_CLOSE(33.33333, segments3[1].pt1().y(), 0.1);
//  BOOST_CHECK_CLOSE(66.666666, segments3[1].pt2().x(), 0.1);
//  BOOST_CHECK_CLOSE(66.666666, segments3[1].pt2().y(), 0.1);
//  BOOST_CHECK_CLOSE(66.666666, segments3[2].pt1().x(), 0.1);
//  BOOST_CHECK_CLOSE(66.666666, segments3[2].pt1().y(), 0.1);
//  BOOST_CHECK_CLOSE(100., segments3[2].pt2().x(), 0.1);
//  BOOST_CHECK_CLOSE(100., segments3[2].pt2().y(), 0.1);
//
//}

/* Segment3D */

/* Constructor por defecto */

BOOST_AUTO_TEST_CASE(Segment3D_DefaultConstructor)
{
    Segment<Point3d> segment;
    BOOST_CHECK(segment.type() == GeometryType::segment);
    BOOST_CHECK(segment.dimension() == Dimension::dim3);
    BOOST_CHECK_EQUAL(0., segment.pt1().x());
    BOOST_CHECK_EQUAL(0., segment.pt1().y());
    BOOST_CHECK_EQUAL(0., segment.pt1().z());
    BOOST_CHECK_EQUAL(0., segment.pt2().x());
    BOOST_CHECK_EQUAL(0., segment.pt2().y());
    BOOST_CHECK_EQUAL(0., segment.pt2().z());
}

/* Constructor punto inicial y final del segmento */

BOOST_AUTO_TEST_CASE(Segment3D_ConstructorPointIniEnd)
{
    Segment<Point3d> segment(Point3d(5.5, 93.2, 10.1), Point3d(25.3, 654.4, 15.6));
    BOOST_CHECK(segment.type() == GeometryType::segment);
    BOOST_CHECK(segment.dimension() == Dimension::dim3);
    BOOST_CHECK_EQUAL(5.5, segment.pt1().x());
    BOOST_CHECK_EQUAL(93.2, segment.pt1().y());
    BOOST_CHECK_EQUAL(10.1, segment.pt1().z());
    BOOST_CHECK_EQUAL(25.3, segment.pt2().x());
    BOOST_CHECK_EQUAL(654.4, segment.pt2().y());
    BOOST_CHECK_EQUAL(15.6, segment.pt2().z());
}

/* Constructor de copia */

BOOST_AUTO_TEST_CASE(Segment3D_CopyConstructor)
{
    Segment<Point3d> segment(Point3d(5.5, 93.2, 10.1), Point3d(25.3, 654.4, 15.6));
    Segment<Point3d> segment_c(segment);
    BOOST_CHECK(segment_c.type() == GeometryType::segment);
    BOOST_CHECK(segment_c.dimension() == Dimension::dim3);
    BOOST_CHECK_EQUAL(5.5, segment_c.pt1().x());
    BOOST_CHECK_EQUAL(93.2, segment_c.pt1().y());
    BOOST_CHECK_EQUAL(10.1, segment_c.pt1().z());
    BOOST_CHECK_EQUAL(25.3, segment_c.pt2().x());
    BOOST_CHECK_EQUAL(654.4, segment_c.pt2().y());
    BOOST_CHECK_EQUAL(15.6, segment_c.pt2().z());
}

/* Operador de asignación */

BOOST_AUTO_TEST_CASE(Segment3D_assignment)
{
    Segment<Point3d> segment(Point3d(5.5, 93.2, 10.1), Point3d(25.3, 654.4, 15.6));
    Segment<Point3d> segment_a = segment;
    BOOST_CHECK(segment_a.type() == GeometryType::segment);
    BOOST_CHECK(segment_a.dimension() == Dimension::dim3);
    BOOST_CHECK_EQUAL(5.5, segment_a.pt1().x());
    BOOST_CHECK_EQUAL(93.2, segment_a.pt1().y());
    BOOST_CHECK_EQUAL(10.1, segment_a.pt1().z());
    BOOST_CHECK_EQUAL(25.3, segment_a.pt2().x());
    BOOST_CHECK_EQUAL(654.4, segment_a.pt2().y());
    BOOST_CHECK_EQUAL(15.6, segment_a.pt2().z());
}

BOOST_AUTO_TEST_CASE(Segment3D_bbox)
{
    Segment<Point3d> segment(Point3d(5.5, 93.2, 10.1), Point3d(25.3, 654.4, 15.6));
    BoundingBox<Point3d> bbox = segment.boundingBox();

    BOOST_CHECK_EQUAL(5.5, bbox.pt1().x());
    BOOST_CHECK_EQUAL(93.2, bbox.pt1().y());
    BOOST_CHECK_EQUAL(10.1, bbox.pt1().z());
    BOOST_CHECK_EQUAL(25.3, bbox.pt2().x());
    BOOST_CHECK_EQUAL(654.4, bbox.pt2().y());
    BOOST_CHECK_EQUAL(15.6, bbox.pt2().z());
}

/* Comprueba si el segmento esta vacio */

BOOST_AUTO_TEST_CASE(Segment3D_isEmpty)
{
    Segment<Point3d> segment(Point3d(5.5, 93.2, 10.1), Point3d(25.3, 654.4, 15.6));
    BOOST_CHECK(false == segment.isEmpty());

    Segment<Point3d> segment2;
    BOOST_CHECK(segment2.isEmpty());
}

BOOST_AUTO_TEST_CASE(Segment3D_length)
{
    Segment<Point3d> segment(Point3d(5.5, 93.2, 10.1), Point3d(25.3, 654.4, 15.6));
    BOOST_CHECK_CLOSE(561.55, segment.length(), 0.1);
}

BOOST_AUTO_TEST_CASE(Segment3D_vector)
{
    Segment<Point3d> segment(Point3d(5.5, 93.2, 10.1), Point3d(25.3, 654.4, 15.6));
    auto v = segment.vector();
    BOOST_CHECK_CLOSE(19.8, v.x(), 0.1);
    BOOST_CHECK_CLOSE(561.19, v.y(), 0.1);
    BOOST_CHECK_CLOSE(5.5, v.z(), 0.1);
}

/* Conversión a un tipo diferente */

BOOST_AUTO_TEST_CASE(Segment3D_conversion)
{
    Segment<Point3d> segment(Point3d(5.5, 93.2, 2.3), Point3d(25.3, 654.4, 52.6));
    Segment<Point3i> segment_int = static_cast<Segment<Point3i>>(segment);
    BOOST_CHECK_EQUAL(6, segment_int.pt1().x());
    BOOST_CHECK_EQUAL(93, segment_int.pt1().y());
    BOOST_CHECK_EQUAL(2, segment_int.pt1().z());
    BOOST_CHECK_EQUAL(25, segment_int.pt2().x());
    BOOST_CHECK_EQUAL(654, segment_int.pt2().y());
    BOOST_CHECK_EQUAL(53, segment_int.pt2().z());

    Segment<Point3f> segment_float(Point3<float>(5.5f, 93.2f, 2.3f), Point3f(25.3f, 654.4f, 52.6f));
    Segment<Point3d> segment_double = static_cast<Segment<Point3d>>(segment_float);
    BOOST_CHECK_CLOSE(5.5, segment_double.pt1().x(), 0.01);
    BOOST_CHECK_CLOSE(93.2, segment_double.pt1().y(), 0.01);
    BOOST_CHECK_CLOSE(2.3, segment_double.pt1().z(), 0.01);
    BOOST_CHECK_CLOSE(25.3, segment_double.pt2().x(), 0.01);
    BOOST_CHECK_CLOSE(654.4, segment_double.pt2().y(), 0.01);
    BOOST_CHECK_CLOSE(52.6, segment_double.pt2().z(), 0.01);
}