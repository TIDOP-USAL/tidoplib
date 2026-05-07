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
 
#define BOOST_TEST_MODULE Tidop algorithms envelope test
#include <boost/test/unit_test.hpp>
#include "tidop/geometry/primitives/Point.h"
#include "tidop/geometry/primitives/Segment.h"
#include "tidop/geometry/primitives/LineString.h"
#include "tidop/geometry/primitives/Polygon.h"
#include "tidop/geometry/primitives/MultiPoint.h"
#include "tidop/geometry/primitives/MultiLineString.h"
#include "tidop/geometry/primitives/MultiPolygon.h"
#include "tidop/geometry/base/GeometryCollection.h"
#include "tidop/geometry/algorithms/spatial/Envelope.h"

using namespace tl; 

BOOST_AUTO_TEST_SUITE(EnvelopeAlgorithmTest)


struct EnvelopeTestFixture
{

    void setup()
    {
        point2d1 = Point2d(1.0, 2.0);
        point2d2 = Point2d(-3.5, -4.2);
        point3d1 = Point3d(1.0, 2.0, 3.0);
        point3d2 = Point3d(4.0, 5.0, 6.0);
        point2dm1 = Point2dm(2.0, 3.0, 10.0);
        point2dm2 = Point2dm(3.0, 4.0, 20.0);
		point2i = Point2i(5, 10);
    }

    void teardown()
    {

    }

    Point2d point2d1;
    Point2d point2d2;
    Point3d point3d1;
    Point3d point3d2;
    Point2dm point2dm1;
    Point2dm point2dm2;
	Point2i point2i;
};

BOOST_FIXTURE_TEST_CASE(Envelope_Point2D, EnvelopeTestFixture)
{
    auto bbox = envelope(point2d1);
    
    // El envelope de un punto es una bounding box degenerada
    BOOST_TEST(bbox.min().x() == point2d1.x());
    BOOST_TEST(bbox.min().y() == point2d1.y());
    BOOST_TEST(bbox.max().x() == point2d1.x());
    BOOST_TEST(bbox.max().y() == point2d1.y());
    
    // Verificar dimensiones
    BOOST_TEST(bbox.width() == 0.0);
    BOOST_TEST(bbox.height() == 0.0);
}

BOOST_FIXTURE_TEST_CASE(Envelope_Point3D, EnvelopeTestFixture)
{
    auto bbox = envelope(point3d1);
    
    // Para 3D, también es degenerado
    BOOST_TEST(bbox.min().x() == point3d1.x());
    BOOST_TEST(bbox.min().y() == point3d1.y());
    BOOST_TEST(bbox.min().z() == point3d1.z());
    BOOST_TEST(bbox.max().x() == point3d1.x());
    BOOST_TEST(bbox.max().y() == point3d1.y());
    BOOST_TEST(bbox.max().z() == point3d1.z());
}

BOOST_FIXTURE_TEST_CASE(Envelope_Point2D_WithMeasure, EnvelopeTestFixture)
{
    auto bbox = envelope(point2dm1);
    
    // El envelope no debe incluir la medida
    BOOST_TEST(bbox.min().x() == point2dm1.x());
    BOOST_TEST(bbox.min().y() == point2dm1.y());
    BOOST_TEST(bbox.max().x() == point2dm1.x());
    BOOST_TEST(bbox.max().y() == point2dm1.y());
    // Esto tiene que dar error de compilación si se intenta acceder a la medida, lo que es correcto porque el envelope no debe incluirla
    //BOOST_TEST(bbox.max().m() == point2dm1.m());
}

BOOST_FIXTURE_TEST_CASE(Envelope_Point_Integer, EnvelopeTestFixture)
{
    auto bbox = envelope(point2i);
    
    BOOST_TEST(bbox.min().x() == point2i.x());
    BOOST_TEST(bbox.min().y() == point2i.y());
    BOOST_TEST(bbox.max().x() == point2i.x());
    BOOST_TEST(bbox.max().y() == point2i.y());
}

BOOST_FIXTURE_TEST_CASE(Envelope_Point_NegativeCoordinates, EnvelopeTestFixture)
{
    auto bbox = envelope(point2d2);
    
    BOOST_TEST(bbox.min().x() == point2d2.x());
    BOOST_TEST(bbox.min().y() == point2d2.y());
    BOOST_TEST(bbox.max().x() == point2d2.x());
    BOOST_TEST(bbox.max().y() == point2d2.y());
}

BOOST_FIXTURE_TEST_CASE(Envelope_Point_Zero, EnvelopeTestFixture)
{
    Point2d p{0.0, 0.0};
    
    auto bbox = envelope(p);
    
    BOOST_TEST(bbox.min().x() == 0.0);
    BOOST_TEST(bbox.min().y() == 0.0);
    BOOST_TEST(bbox.max().x() == 0.0);
    BOOST_TEST(bbox.max().y() == 0.0);
}

BOOST_AUTO_TEST_CASE(envelope_segment)
{
    Segment2d seg(Point2d{0, 10}, Point2d{5, -5});
    auto box = envelope(seg);

    // El min debe ser (0, -5) y el max (5, 10)
    BOOST_CHECK_EQUAL(box.min().x(), 0.0);
    BOOST_CHECK_EQUAL(box.min().y(), -5.0);
    BOOST_CHECK_EQUAL(box.max().x(), 5.0);
    BOOST_CHECK_EQUAL(box.max().y(), 10.0);
}

BOOST_AUTO_TEST_CASE(envelope_linestring)
{
    LineString2d line;
    line.emplace_back(0, 0);
    line.emplace_back(10, 5);
    line.emplace_back(-2, 8);

    auto box = envelope(line);

    BOOST_CHECK_EQUAL(box.min().x(), -2.0);
    BOOST_CHECK_EQUAL(box.max().x(), 10.0);
    BOOST_CHECK_EQUAL(box.max().y(), 8.0);
}

BOOST_AUTO_TEST_CASE(envelope_polygon)
{
    Polygon2d poly;
    poly.outer() = {Point2d{0,0}, Point2d{10,0}, Point2d{10,10}, Point2d{0,10}};

    // Añadimos un hueco que se sale del outer para probar que se ignora 
    // (según la lógica estándar de OGC, el outer define el envelope)
    LinearRing2d hole = {Point2d{ -5, -5}, Point2d{15, -5}, Point2d{15, 15}, Point2d{-5, 15}};
    poly.addInner(hole);

    auto box = envelope(poly);

    BOOST_CHECK_EQUAL(box.min().x(), 0.0);
    BOOST_CHECK_EQUAL(box.max().x(), 10.0);
}

BOOST_AUTO_TEST_CASE(envelope_multipoint)
{
    MultiPoint2d mp;
    mp.emplace_back(0.0, 5.0);
    mp.emplace_back(10.0, -2.0);
    mp.emplace_back(5.0, 15.0);

    auto box = envelope(mp);

    BOOST_CHECK(box.isValid());
    BOOST_CHECK_EQUAL(box.min().x(), 0.0);
    BOOST_CHECK_EQUAL(box.min().y(), -2.0);
    BOOST_CHECK_EQUAL(box.max().x(), 10.0);
    BOOST_CHECK_EQUAL(box.max().y(), 15.0);
}

BOOST_AUTO_TEST_CASE(envelope_multipolygon)
{
    MultiPolygon2d mp;

    Polygon2d p1;
    p1.outer() = {Point2d{0,0}, Point2d{2,2}};

    Polygon2d p2;
    p2.outer() = {Point2d{10,10}, Point2d{12,12}};

    mp.push_back(p1);
    mp.push_back(p2);

    auto box = envelope(mp);

    BOOST_CHECK_EQUAL(box.min().x(), 0.0);
    BOOST_CHECK_EQUAL(box.min().y(), 0.0);
    BOOST_CHECK_EQUAL(box.max().x(), 12.0);
    BOOST_CHECK_EQUAL(box.max().y(), 12.0);
}

BOOST_AUTO_TEST_CASE(envelope_multilinestring)
{
    MultiLineString2d mls;

    LineString2d l1;
    l1.emplace_back(0, 0);
    l1.emplace_back(2, 2);

    LineString2d l2;
    l2.emplace_back(10, 10);
    l2.emplace_back(5, 5);

    mls.push_back(l1);
    mls.push_back(l2);

    auto box = envelope(mls);

    // El envelope total debe englobar ambas líneas: (0,0) a (10,10)
    BOOST_CHECK(box.isValid());
    BOOST_CHECK_EQUAL(box.min().x(), 0.0);
    BOOST_CHECK_EQUAL(box.min().y(), 0.0);
    BOOST_CHECK_EQUAL(box.max().x(), 10.0);
    BOOST_CHECK_EQUAL(box.max().y(), 10.0);
}

BOOST_AUTO_TEST_CASE(envelope_empty_multi)
{
    MultiPolygon2d empty_mp;
    auto box = envelope(empty_mp);

    BOOST_CHECK(!box.isValid());
}

BOOST_AUTO_TEST_CASE(envelope_mixed_geometries)
{
    Point2d p(1.0, 2.0);
    Segment2d seg(Point2d{0, 0}, Point2d{3, 4});
    LineString2d line = {Point2d{5, 5}, Point2d{7, 8}};
    Polygon2d poly;
    poly.outer() = {Point2d{10,10}, Point2d{12,10}, Point2d{12,12}, Point2d{10,12}};
    auto box = envelope(p, seg, line, poly);
    BOOST_CHECK(box.isValid());
    BOOST_CHECK_EQUAL(box.min().x(), 0.0);
    BOOST_CHECK_EQUAL(box.min().y(), 0.0);
    BOOST_CHECK_EQUAL(box.max().x(), 12.0);
    BOOST_CHECK_EQUAL(box.max().y(), 12.0);
}

BOOST_AUTO_TEST_CASE(envelope_geometry_collection)
{
    Point2d p(1.0, 2.0);
    LineString2d line = {Point2d{5, 5}, Point2d{7, 8}};
    Polygon2d poly;
    poly.outer() = {Point2d{10,10}, Point2d{12,10}, Point2d{12,12}, Point2d{10,12}};
    GeometryCollection<Point2d> gc;
    gc.add(p);
    gc.add(line);
    gc.add(poly);
    auto box = envelope(gc);
    BOOST_CHECK(box.isValid());
    BOOST_CHECK_EQUAL(box.min().x(), 1.0);
    BOOST_CHECK_EQUAL(box.min().y(), 2.0);
    BOOST_CHECK_EQUAL(box.max().x(), 12.0);
    BOOST_CHECK_EQUAL(box.max().y(), 12.0);
}

BOOST_AUTO_TEST_SUITE_END()