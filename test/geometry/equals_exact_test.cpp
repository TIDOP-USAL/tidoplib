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
 
#define BOOST_TEST_MODULE Tidop algorithms equals exacts test
#include <boost/test/unit_test.hpp>

#include "geometry_test_fixture.h"
#include "tidop/geometry/primitives/Point.h"
#include "tidop/geometry/primitives/Segment.h"
#include "tidop/geometry/primitives/LineString.h"
#include "tidop/geometry/primitives/Polygon.h"
#include "tidop/geometry/primitives/MultiPoint.h"
#include "tidop/geometry/primitives/MultiLineString.h"
#include "tidop/geometry/primitives/MultiPolygon.h"
#include "tidop/geometry/base/GeometryCollection.h"
#include "tidop/geometry/algorithms/analysis/equalsExact.h"

using namespace tl; 
using namespace test; 


BOOST_AUTO_TEST_SUITE(equalsExact_tests)


// ============================================================================
// Point - Point
// ============================================================================

BOOST_FIXTURE_TEST_CASE(equals_exact_point_point, GeometryTestFixture)
{
    BOOST_CHECK(equalsExact(point2d1, point2d1));
    BOOST_CHECK(equalsExact(point2d2, point2d2));
    BOOST_CHECK(!equalsExact(point2d1, point2d2));
    BOOST_CHECK(!equalsExact(point2d4, point2d5));
    BOOST_CHECK(equalsExact(point2d4, point2d5, policy));
}

BOOST_FIXTURE_TEST_CASE(equals_exact_point_point_with_measure, GeometryTestFixture)
{
    BOOST_CHECK(equalsExact(point2dm1, point2dm1));
    BOOST_CHECK(equalsExact(point2dm2, point2dm2));
    BOOST_CHECK(!equalsExact(point2dm1, point2dm2));
}

// ============================================================================
// Segment - Segment
// ============================================================================

BOOST_AUTO_TEST_CASE(segment_same_order)
{
    Segment2d s1(Point2d(0,0), Point2d(1,1));
    Segment2d s2(Point2d(0,0), Point2d(1,1));
    BOOST_CHECK(equalsExact(s1, s2));
}

BOOST_AUTO_TEST_CASE(segment_different_order)
{
    Segment2d s1(Point2d(0,0), Point2d(1,1));
    Segment2d s2(Point2d(1,1), Point2d(0,0)); // orden inverso
    BOOST_CHECK(!equalsExact(s1, s2)); // debe ser false
}

BOOST_AUTO_TEST_CASE(segment_different_points)
{
    Segment2d s1(Point2d(0,0), Point2d(1,1));
    Segment2d s2(Point2d(0,0), Point2d(1,2));
    BOOST_CHECK(!equalsExact(s1, s2));
}

// -----------------------------------------------------------------------------
// LineString
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(linestring_same)
{
    LineString2d ls1({Point2d(0,0), Point2d(1,1), Point2d(2,0)});
    LineString2d ls2({Point2d(0,0), Point2d(1,1), Point2d(2,0)});
    BOOST_CHECK(equalsExact(ls1, ls2));
}

BOOST_AUTO_TEST_CASE(linestring_different_size)
{
    LineString2d ls1({Point2d(0,0), Point2d(1,1), Point2d(2,0)});
    LineString2d ls2({Point2d(0,0), Point2d(1,1)});
    BOOST_CHECK(!equalsExact(ls1, ls2));
}

BOOST_AUTO_TEST_CASE(linestring_different_point)
{
    LineString2d ls1({Point2d(0,0), Point2d(1,1), Point2d(2,0)});
    LineString2d ls2({Point2d(0,0), Point2d(1,2), Point2d(2,0)});
    BOOST_CHECK(!equalsExact(ls1, ls2));
}

BOOST_AUTO_TEST_CASE(linestring_empty)
{
    LineString2d ls1, ls2;
    BOOST_CHECK(equalsExact(ls1, ls2));
    ls1.emplace_back(0, 0);
    BOOST_CHECK(!equalsExact(ls1, ls2));
}

// -----------------------------------------------------------------------------
// Polygon
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(polygon_same_no_holes)
{
    LinearRing2d outer({Point2d(0,0), Point2d(1,0), Point2d(1,1), Point2d(0,1), Point2d(0,0)});
    Polygon2d p1(outer);
    Polygon2d p2(outer);
    BOOST_CHECK(equalsExact(p1, p2));
}

BOOST_AUTO_TEST_CASE(polygon_with_holes_same)
{
    LinearRing2d outer({Point2d(0,0), Point2d(2,0), Point2d(2,2), Point2d(0,2), Point2d(0,0)});
    LinearRing2d hole({Point2d(0.5,0.5), Point2d(1.5,0.5), Point2d(1.5,1.5), Point2d(0.5,1.5), Point2d(0.5,0.5)});
    Polygon2d p1(outer, {hole});
    Polygon2d p2(outer, {hole});
    BOOST_CHECK(equalsExact(p1, p2));
}

BOOST_AUTO_TEST_CASE(polygon_different_hole_count)
{
    LinearRing2d outer({Point2d(0,0), Point2d(2,0), Point2d(2,2), Point2d(0,2), Point2d(0,0)});
    LinearRing2d hole({Point2d(0.5,0.5), Point2d(1.5,0.5), Point2d(1.5,1.5), Point2d(0.5,1.5), Point2d(0.5,0.5)});
    Polygon2d p1(outer, {hole});
    Polygon2d p2(outer, {}); // sin agujeros
    BOOST_CHECK(!equalsExact(p1, p2));
}

BOOST_AUTO_TEST_CASE(polygon_different_outer_point)
{
    LinearRing2d outer1({Point2d(0,0), Point2d(1,0), Point2d(1,1), Point2d(0,1), Point2d(0,0)});
    LinearRing2d outer2({Point2d(0,0), Point2d(1,0), Point2d(1,1), Point2d(0,1.1), Point2d(0,0)}); // último punto diferente
    Polygon2d p1(outer1);
    Polygon2d p2(outer2);
    BOOST_CHECK(!equalsExact(p1, p2));
}

// -----------------------------------------------------------------------------
// MultiPoint
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(multipoint_same)
{
    MultiPoint2d mp1({Point2d(0,0), Point2d(1,1), Point2d(2,2)});
    MultiPoint2d mp2({Point2d(0,0), Point2d(1,1), Point2d(2,2)});
    BOOST_CHECK(equalsExact(mp1, mp2));
}

BOOST_AUTO_TEST_CASE(multipoint_different_order)
{
    MultiPoint2d mp1({Point2d(0,0), Point2d(1,1), Point2d(2,2)});
    MultiPoint2d mp2({Point2d(2,2), Point2d(1,1), Point2d(0,0)}); // orden diferente -> false
    BOOST_CHECK(!equalsExact(mp1, mp2));
}

BOOST_AUTO_TEST_CASE(multipoint_different_size)
{
    MultiPoint2d mp1({Point2d(0,0), Point2d(1,1)});
    MultiPoint2d mp2({Point2d(0,0), Point2d(1,1), Point2d(2,2)});
    BOOST_CHECK(!equalsExact(mp1, mp2));
}

// -----------------------------------------------------------------------------
// MultiLineString
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(multilinestring_same)
{
    LineString2d ls1({Point2d(0,0), Point2d(1,1)});
    LineString2d ls2({Point2d(2,2), Point2d(3,3)});
    MultiLineString2d mls1({ls1, ls2});
    MultiLineString2d mls2({ls1, ls2});
    BOOST_CHECK(equalsExact(mls1, mls2));
}

BOOST_AUTO_TEST_CASE(multilinestring_different)
{
    LineString2d ls1({Point2d(0,0), Point2d(1,1)});
    LineString2d ls2({Point2d(2,2), Point2d(3,3)});
    MultiLineString2d mls1({ls1, ls2});
    MultiLineString2d mls2({ls1}); // solo una
    BOOST_CHECK(!equalsExact(mls1, mls2));
}

// -----------------------------------------------------------------------------
// MultiPolygon
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(multipolygon_same)
{
    LinearRing2d outer({Point2d(0,0), Point2d(1,0), Point2d(1,1), Point2d(0,1), Point2d(0,0)});
    Polygon2d poly1(outer);
    Polygon2d poly2(outer);
    MultiPolygon2d mp1({poly1, poly2});
    MultiPolygon2d mp2({poly1, poly2});
    BOOST_CHECK(equalsExact(mp1, mp2));
}

// -----------------------------------------------------------------------------
// GeometryCollection
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(geometrycollection_same)
{
    Point2d p(1,2);
    LineString2d ls({Point2d(0,0), Point2d(1,1)});
    GeometryCollection<Point2d> gc1;
    gc1.add(p);
    gc1.add(ls);
    BOOST_CHECK(equalsExact(gc1, gc1));
}

BOOST_AUTO_TEST_CASE(geometrycollection_different_order)
{
    Point2d p(1,2);
    LineString2d ls({Point2d(0,0), Point2d(1,1)});
    GeometryCollection<Point2d> gc1;
    gc1.add(p);
    gc1.add(ls);
    GeometryCollection<Point2d> gc2;
    gc2.add(ls);
    gc2.add(p);
    BOOST_CHECK(!equalsExact(gc1, gc2));
}

// -----------------------------------------------------------------------------
// Diferentes tipos de geometría
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(different_geometry_types)
{
    Point2d p(1,2);
    LineString2d ls({Point2d(1,2)});
    BOOST_CHECK(!equalsExact(p, ls));
}

// -----------------------------------------------------------------------------
// Geometrías vacías en colecciones
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(empty_collections)
{
    MultiPoint2d mp1, mp2;
    BOOST_CHECK(equalsExact(mp1, mp2));
    mp1.emplace_back(0, 0);
    BOOST_CHECK(!equalsExact(mp1, mp2));
}

BOOST_AUTO_TEST_SUITE_END()