#define BOOST_TEST_MODULE Tidop WKT Reader test
#include <boost/test/unit_test.hpp>

#include <sstream>

#include <tidop/geometry/io/wkt/Proxy.h>
#include <tidop/geometry/primitives/MultiLineString.h>
#include <tidop/geometry/primitives/MultiPolygon.h>

using namespace tl;

BOOST_AUTO_TEST_SUITE(WKTReaderTestSuite)

BOOST_AUTO_TEST_CASE(read_point)
{
    Point2d p;
    std::stringstream ss("POINT (1.25 2.5)");
    ss >> read_wkt(p);

    BOOST_CHECK_CLOSE(p.x(), 1.25, 0.0001);
    BOOST_CHECK_CLOSE(p.y(), 2.5, 0.0001);
}

BOOST_AUTO_TEST_CASE(read_point_with_z)
{
    Point3d p;
    std::stringstream ss("POINT Z (1 2 3)");
    ss >> p;

    BOOST_CHECK_EQUAL(p.x(), 1.0);
    BOOST_CHECK_EQUAL(p.y(), 2.0);
    BOOST_CHECK_EQUAL(p.z(), 3.0);
}

BOOST_AUTO_TEST_CASE(read_linestring)
{
    LineString2d line;
    std::stringstream ss("LINESTRING (0 0, 1 1, 2 0)");
    ss >> read_wkt(line);

    BOOST_REQUIRE_EQUAL(line.size(), 3);
    BOOST_CHECK_EQUAL(line[0].x(), 0);
    BOOST_CHECK_EQUAL(line[0].y(), 0);
    BOOST_CHECK_EQUAL(line[2].x(), 2);
    BOOST_CHECK_EQUAL(line[2].y(), 0);
}

BOOST_AUTO_TEST_CASE(read_polygon_with_hole)
{
    Polygon2d polygon;
    std::stringstream ss("POLYGON ((0 0, 10 0, 10 10, 0 10, 0 0), (2 2, 8 2, 8 8, 2 8, 2 2))");
    ss >> read_wkt(polygon);

    BOOST_REQUIRE_EQUAL(polygon.outer().size(), 5);
    BOOST_REQUIRE_EQUAL(polygon.numInners(), 1);
    BOOST_CHECK_EQUAL(polygon.inner(0).size(), 5);
}

BOOST_AUTO_TEST_CASE(read_multi_point)
{
    MultiPoint2d points;
    std::stringstream ss("MULTIPOINT ((1 2), (3 4), (5 6))");
    ss >> read_wkt(points);

    BOOST_REQUIRE_EQUAL(points.size(), 3);
    BOOST_CHECK_EQUAL(points[1].x(), 3);
    BOOST_CHECK_EQUAL(points[1].y(), 4);
}

BOOST_AUTO_TEST_CASE(read_multi_line_string)
{
    MultiLineString2d lines;
    std::stringstream ss("MULTILINESTRING ((0 0, 1 1), (2 2, 3 3))");
    ss >> read_wkt(lines);

    BOOST_REQUIRE_EQUAL(lines.size(), 2);
    BOOST_REQUIRE_EQUAL(lines[0].size(), 2);
    BOOST_CHECK_EQUAL(lines[1][1].x(), 3);
    BOOST_CHECK_EQUAL(lines[1][1].y(), 3);
}

BOOST_AUTO_TEST_CASE(read_multi_polygon)
{
    MultiPolygon2d polygons;
    std::stringstream ss("MULTIPOLYGON (((0 0, 1 0, 0 1, 0 0)), ((2 2, 3 2, 2 3, 2 2)))");
    ss >> read_wkt(polygons);

    BOOST_REQUIRE_EQUAL(polygons.size(), 2);
    BOOST_REQUIRE_EQUAL(polygons[0].outer().size(), 4);
}

BOOST_AUTO_TEST_CASE(read_geometry_collection)
{
    GeometryCollection<Point2d> collection;
    std::stringstream ss("GEOMETRYCOLLECTION (POINT (1 2), LINESTRING (0 0, 3 3))");
    ss >> read_wkt(collection);

    BOOST_REQUIRE_EQUAL(collection.size(), 2);
    BOOST_CHECK(collection[0].index() == 0);
    BOOST_CHECK(collection[1].index() == 1);
}

BOOST_AUTO_TEST_CASE(read_empty_geometry)
{
    LineString2d line;
    line.push_back(Point2d(1, 2));
    std::stringstream ss("LINESTRING EMPTY");
    ss >> read_wkt(line);
    BOOST_CHECK(line.isEmpty());
}

BOOST_AUTO_TEST_CASE(dimension_mismatch_sets_fail)
{
    Point2d p;
    std::stringstream ss("POINT Z (1 2 3)");
    ss >> p;
    BOOST_CHECK(ss.fail());
}

BOOST_AUTO_TEST_SUITE_END()
