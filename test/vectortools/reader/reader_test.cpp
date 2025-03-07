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
 
#define BOOST_TEST_MODULE Tidop vector reader test
#include <boost/test/unit_test.hpp>
#include <tidop/vectortools/io/vectreader.h>
#include <tidop/graphic/layer.h>
#include <tidop/graphic/entities/point.h>
#include <tidop/graphic/entities/linestring.h>
#include <tidop/graphic/entities/polygon.h>


#include <array>

using namespace tl;

BOOST_AUTO_TEST_CASE(read_geojson)
{
    try {

        tl::Path file(tl::Path(TL_DATA_PATH).append("\\example.geojson"));
        std::unique_ptr<VectorReader> vector_reader = VectorReaderFactory::create(file);
        vector_reader->open();
        if (!vector_reader->isOpen()) throw std::runtime_error("Vector open error");

        int layers_count = vector_reader->layersCount();
        BOOST_CHECK_EQUAL(1, layers_count);

        std::shared_ptr<GLayer> layer = vector_reader->read(0);

        // read point
        auto entity = layer->begin();
        std::shared_ptr<GPoint> point = std::dynamic_pointer_cast<GPoint>(*entity);
        BOOST_CHECK_EQUAL(-105.01621, point->x);
        BOOST_CHECK_EQUAL(39.57422, point->y);

        auto point_data = entity->get()->data();
        BOOST_CHECK_EQUAL("Ubicación de ejemplo", point_data->value(0));

        // read linestring
        entity++;
        std::shared_ptr<GLineString> linestring = std::dynamic_pointer_cast<GLineString>(*entity);
        BOOST_CHECK_EQUAL(2, linestring->size());

        auto &first_point = linestring->at(0);
        BOOST_CHECK_EQUAL(-105.01621, first_point.x);
        BOOST_CHECK_EQUAL(39.57422, first_point.y);

        auto &second_point = linestring->at(1);
        BOOST_CHECK_EQUAL(-105.00376, second_point.x);
        BOOST_CHECK_EQUAL(39.59931, second_point.y);

        auto linestring_data = entity->get()->data();
        BOOST_CHECK_EQUAL("Línea de ejemplo", linestring_data->value(0));

        // read polygon
        entity++;
        std::shared_ptr<GPolygon> polygon = std::dynamic_pointer_cast<GPolygon>(*entity);
        BOOST_CHECK_EQUAL(4, polygon->size());

        auto &point1 = polygon->at(0);
        BOOST_CHECK_EQUAL(-105.02311, point1.x);
        BOOST_CHECK_EQUAL(39.60058, point1.y);

        auto &point2 = polygon->at(1);
        BOOST_CHECK_EQUAL(-105.01431, point2.x);
        BOOST_CHECK_EQUAL(39.59781, point2.y);

        auto &point3 = polygon->at(2);
        BOOST_CHECK_EQUAL(-105.01114, point3.x);
        BOOST_CHECK_EQUAL(39.60235, point3.y);

        auto &point4 = polygon->at(3);
        BOOST_CHECK_EQUAL(-105.02311, point4.x);
        BOOST_CHECK_EQUAL(39.60058, point4.y);

        auto polygon_data = entity->get()->data();
        BOOST_CHECK_EQUAL("Polígono de ejemplo", polygon_data->value(0));

        vector_reader->close();

    } catch (std::exception &e) {
        tl::printException(e);
    }
}

BOOST_AUTO_TEST_CASE(read_gml)
{
    try {

        tl::Path file(tl::Path(TL_DATA_PATH).append("\\example.gml"));
        std::unique_ptr<VectorReader> vector_reader = VectorReaderFactory::create(file);
        vector_reader->open();
        if (!vector_reader->isOpen()) return; // read support needs Xerces or libexpat 

        int layers_count = vector_reader->layersCount();
        BOOST_CHECK_EQUAL(3, layers_count);

        // read point
        std::shared_ptr<GLayer> layer = vector_reader->read(0);
        auto entity = layer->begin();
        std::shared_ptr<GPoint> point = std::dynamic_pointer_cast<GPoint>(*entity);
        BOOST_CHECK_EQUAL(-105.01621, point->x);
        BOOST_CHECK_EQUAL(39.57422, point->y);

        auto point_data = entity->get()->data();
        BOOST_CHECK_EQUAL("point.0", point_data->value(0));
        BOOST_CHECK_EQUAL("Ubicación de ejemplo", point_data->value(1));

        // read linestring
        layer = vector_reader->read(1);
        entity = layer->begin();
        std::shared_ptr<GLineString> linestring = std::dynamic_pointer_cast<GLineString>(*entity);
        BOOST_CHECK_EQUAL(2, linestring->size());

        auto &first_point = linestring->at(0);
        BOOST_CHECK_EQUAL(-105.01621, first_point.x);
        BOOST_CHECK_EQUAL(39.57422, first_point.y);

        auto &second_point = linestring->at(1);
        BOOST_CHECK_EQUAL(-105.00376, second_point.x);
        BOOST_CHECK_EQUAL(39.59931, second_point.y);

        auto linestring_data = entity->get()->data();
        BOOST_CHECK_EQUAL("linestring.0", linestring_data->value(0));
        BOOST_CHECK_EQUAL("Línea de ejemplo", linestring_data->value(1));

        // read polygon
        layer = vector_reader->read(2);
        entity = layer->begin();
        std::shared_ptr<GPolygon> polygon = std::dynamic_pointer_cast<GPolygon>(*entity);
        BOOST_CHECK_EQUAL(4, polygon->size());

        auto &point1 = polygon->at(0);
        BOOST_CHECK_EQUAL(-105.02311, point1.x);
        BOOST_CHECK_EQUAL(39.60058, point1.y);

        auto &point2 = polygon->at(1);
        BOOST_CHECK_EQUAL(-105.01431, point2.x);
        BOOST_CHECK_EQUAL(39.59781, point2.y);

        auto &point3 = polygon->at(2);
        BOOST_CHECK_EQUAL(-105.01114, point3.x);
        BOOST_CHECK_EQUAL(39.60235, point3.y);

        auto &point4 = polygon->at(3);
        BOOST_CHECK_EQUAL(-105.02311, point4.x);
        BOOST_CHECK_EQUAL(39.60058, point4.y);

        auto polygon_data = entity->get()->data();
        BOOST_CHECK_EQUAL("polygon.0", polygon_data->value(0));
        BOOST_CHECK_EQUAL("Polígono de ejemplo", polygon_data->value(1));

        vector_reader->close();

    } catch (std::exception &e) {
        tl::printException(e);
    }
}


BOOST_AUTO_TEST_CASE(read_kml)
{
    try {

        tl::Path file(tl::Path(TL_DATA_PATH).append("\\example.kml"));
        std::unique_ptr<VectorReader> vector_reader = VectorReaderFactory::create(file);
        vector_reader->open();
        if (!vector_reader->isOpen()) return; // read support needs Expat XML Parser

        int layers_count = vector_reader->layersCount();
        BOOST_CHECK_EQUAL(1, layers_count);

        std::shared_ptr<GLayer> layer = vector_reader->read(0);

        // read point
        auto entity = layer->begin();
        std::shared_ptr<GPoint> point = std::dynamic_pointer_cast<GPoint>(*entity);
        BOOST_CHECK_EQUAL(-105.01621, point->x);
        BOOST_CHECK_EQUAL(39.57422, point->y);

        auto point_data = entity->get()->data();
        BOOST_CHECK_EQUAL("Ubicación de ejemplo", point_data->value(0));

        // read linestring
        entity++;
        std::shared_ptr<GLineString> linestring = std::dynamic_pointer_cast<GLineString>(*entity);
        BOOST_CHECK_EQUAL(2, linestring->size());

        auto &first_point = linestring->at(0);
        BOOST_CHECK_EQUAL(-105.01621, first_point.x);
        BOOST_CHECK_EQUAL(39.57422, first_point.y);

        auto &second_point = linestring->at(1);
        BOOST_CHECK_EQUAL(-105.00376, second_point.x);
        BOOST_CHECK_EQUAL(39.59931, second_point.y);

        auto linestring_data = entity->get()->data();
        BOOST_CHECK_EQUAL("Línea de ejemplo", linestring_data->value(0));

        // read polygon
        entity++;
        std::shared_ptr<GPolygon> polygon = std::dynamic_pointer_cast<GPolygon>(*entity);
        BOOST_CHECK_EQUAL(4, polygon->size());

        auto &point1 = polygon->at(0);
        BOOST_CHECK_EQUAL(-105.02311, point1.x);
        BOOST_CHECK_EQUAL(39.60058, point1.y);

        auto &point2 = polygon->at(1);
        BOOST_CHECK_EQUAL(-105.01431, point2.x);
        BOOST_CHECK_EQUAL(39.59781, point2.y);

        auto &point3 = polygon->at(2);
        BOOST_CHECK_EQUAL(-105.01114, point3.x);
        BOOST_CHECK_EQUAL(39.60235, point3.y);

        auto &point4 = polygon->at(3);
        BOOST_CHECK_EQUAL(-105.02311, point4.x);
        BOOST_CHECK_EQUAL(39.60058, point4.y);

        auto polygon_data = entity->get()->data();
        BOOST_CHECK_EQUAL("Polígono de ejemplo", polygon_data->value(0));

        vector_reader->close();

    } catch (std::exception &e) {
        tl::printException(e);
    }
}


BOOST_AUTO_TEST_CASE(read_kmz)
{
    try {

        tl::Path file(tl::Path(TL_DATA_PATH).append("\\example.kmz"));
        std::unique_ptr<VectorReader> vector_reader = VectorReaderFactory::create(file);
        vector_reader->open();
        if (!vector_reader->isOpen()) return; // read support needs Libkml

        int layers_count = vector_reader->layersCount();
        BOOST_CHECK_EQUAL(1, layers_count);

        std::shared_ptr<GLayer> layer = vector_reader->read(0);

        // read point
        auto entity = layer->begin();
        std::shared_ptr<GPoint> point = std::dynamic_pointer_cast<GPoint>(*entity);
        BOOST_CHECK_EQUAL(-105.01621, point->x);
        BOOST_CHECK_EQUAL(39.57422, point->y);

        auto point_data = entity->get()->data();
        BOOST_CHECK_EQUAL("Ubicación de ejemplo", point_data->value(0));

        // read linestring
        entity++;
        std::shared_ptr<GLineString> linestring = std::dynamic_pointer_cast<GLineString>(*entity);
        BOOST_CHECK_EQUAL(2, linestring->size());

        auto &first_point = linestring->at(0);
        BOOST_CHECK_EQUAL(-105.01621, first_point.x);
        BOOST_CHECK_EQUAL(39.57422, first_point.y);

        auto &second_point = linestring->at(1);
        BOOST_CHECK_EQUAL(-105.00376, second_point.x);
        BOOST_CHECK_EQUAL(39.59931, second_point.y);

        auto linestring_data = entity->get()->data();
        BOOST_CHECK_EQUAL("Línea de ejemplo", linestring_data->value(0));

        // read polygon
        entity++;
        std::shared_ptr<GPolygon> polygon = std::dynamic_pointer_cast<GPolygon>(*entity);
        BOOST_CHECK_EQUAL(4, polygon->size());

        auto &point1 = polygon->at(0);
        BOOST_CHECK_EQUAL(-105.02311, point1.x);
        BOOST_CHECK_EQUAL(39.60058, point1.y);

        auto &point2 = polygon->at(1);
        BOOST_CHECK_EQUAL(-105.01431, point2.x);
        BOOST_CHECK_EQUAL(39.59781, point2.y);

        auto &point3 = polygon->at(2);
        BOOST_CHECK_EQUAL(-105.01114, point3.x);
        BOOST_CHECK_EQUAL(39.60235, point3.y);

        auto &point4 = polygon->at(3);
        BOOST_CHECK_EQUAL(-105.02311, point4.x);
        BOOST_CHECK_EQUAL(39.60058, point4.y);

        auto polygon_data = entity->get()->data();
        BOOST_CHECK_EQUAL("Polígono de ejemplo", polygon_data->value(0));

        vector_reader->close();
    } catch (std::exception &e) {
        tl::printException(e);
    }
}

BOOST_AUTO_TEST_SUITE(VectorReaderTestSuite)

struct VectorReaderTest
{

    VectorReaderTest()
    {
    }

    ~VectorReaderTest() 
    {
    }

    void setup()
    {
        autos_name = "Autos";
        autos_points.emplace_back(-0.0014, 0.0002);
        autos_points.emplace_back(-0.0022, 0.0);
        autos_points.emplace_back(-0.0029, -0.0002);
        autos_points.emplace_back(-0.0032, -0.0004);
        autos_points.emplace_back(-0.0032, -0.0008);
        autos_points.emplace_back(-0.0032, -0.0014);
        autos_points.emplace_back(-0.0032, -0.0022);
        autos_points.emplace_back(-0.0032, 0.0022);
        autos_points.emplace_back(-0.0032, 0.0012);
        autos_points.emplace_back(-0.0032, 0.0002);
        autos_points.emplace_back(-0.0032, -0.0008);
        autos_points.emplace_back(-0.0032, -0.0018);
        autos_points.emplace_back(-0.0026, -0.0020);
        autos_points.emplace_back(-0.0026, -0.0012);
        autos_points.emplace_back(-0.0026, -0.0004);
        autos_points.emplace_back(-0.0026, 0.0004);
        autos_points.emplace_back(-0.0026, 0.0012);
        autos_points.emplace_back(0.0029, 0.0019);
        autos_points.emplace_back(0.0028, 0.0014);

        bridges_name = "Bridges";
        bridges_point = tl::Point<double>(0.0002, 0.0007);
        bridges_data_id = "110";
        bridges_data_name = "Cam Bridge";

        road_segments_name = "RoadSegments";
        roads.push_back({Point<double>(-0.0042, -0.0006), 
                         Point<double>(-0.0032, -0.0003),
                         Point<double>(-0.0026, -0.0001),
                         Point<double>(-0.0014, 0.0002),
                         Point<double>(0.0002, 0.0007)});
        roads.push_back({Point<double>(0.0002, 0.0007), 
                         Point<double>(0.0014, 0.0010),
                         Point<double>(0.0028, 0.0014)});
        roads.push_back({Point<double>(0.0028, 0.0014), 
                         Point<double>(0.0030, 0.0024)});
        roads.push_back({Point<double>(0.0002, 0.0007), 
                         Point<double>(0.0014, 0.0010),
                         Point<double>(0.0028, 0.0014),
                         Point<double>(0.0042, 0.0018)});
        roads.push_back({Point<double>(-0.0014, -0.0024), 
                         Point<double>(-0.0014, 0.0002)});

        road_segments_data_ids.emplace_back("102");
        road_segments_data_ids.emplace_back("103");
        road_segments_data_ids.emplace_back("104");
        road_segments_data_ids.emplace_back("105");
        road_segments_data_ids.emplace_back("106");
        road_segments_data_names.emplace_back("Route 5");
        road_segments_data_names.emplace_back("Route 5");
        road_segments_data_names.emplace_back("Route 5");
        road_segments_data_names.emplace_back("Main Street");
        road_segments_data_names.emplace_back("Dirt Road by Green Forest");

        streams_name = "Streams";
        streams.push_back({Point<double>(-0.0004, 0.0024), 
                           Point<double>(0.0002, 0.0017),
                           Point<double>(-0.0001, 0.0012),
                           Point<double>(0.0002, 0.0007),
                           Point<double>(0.0010, -0.0006)});
        streams.push_back({Point<double>(0.0034, -0.0024), 
                           Point<double>(0.0036, -0.0020),
                           Point<double>(0.0031, -0.0015)});

        streams_data_ids.emplace_back("111");
        streams_data_ids.emplace_back("112");
        streams_data_names.emplace_back("Cam Stream");
        streams_data_names.emplace_back("");

        forest_name = "Forests";
        forest = { Point<double>(-0.0014, -0.0024),
                   Point<double>(-0.0014, 0.0002),
                   Point<double>(0.0002, 0.0007),
                   Point<double>(0.0014, 0.0010),
                   Point<double>(0.0028, 0.0014),
                   Point<double>(0.0042, 0.0018),
                   Point<double>(0.0042, -0.0024),
                   Point<double>(-0.0014, -0.0024)};
        forest_data_id = "109";
        forest_data_name = "Green Forest";

        divided_routes_name = "DividedRoutes";
        LineString<Point<double>> line1 = {Point<double>(-0.0032, -0.0024),
                                           Point<double>(-0.0032, 0.0024)};
        LineString<Point<double>> line2 = {Point<double>(-0.0026, -0.0024),
                                           Point<double>(-0.0026, 0.0024)};
        divided_routes.push_back(line1);
        divided_routes.push_back(line2);
        divided_routes_data_id = "119";
        divided_routes_data_name = "Route 75";

        basic_polygons_name = "BasicPolygons";

        basic_polygons.push_back({Point<double>(-1, 0), 
                                  Point<double>(0, 1),
                                  Point<double>(1, 0),
                                  Point<double>(0, -1),
                                  Point<double>(-1, 0)});
        basic_polygons.push_back({Point<double>(-2, 6),
                                  Point<double>(1, 6),
                                  Point<double>(1, 3),
                                  Point<double>(-2, 3),
                                  Point<double>(-2, 6)});
        basic_polygons.push_back({Point<double>(-1, 5),
                                  Point<double>(2, 5),
                                  Point<double>(2, 2),
                                  Point<double>(-1, 2),
                                  Point<double>(-1, 5)});

        ponds_name = "Ponds";
        ponds.push_back({Point<double>(-0.0020, 0.0018),
                         Point<double>(-0.0018, 0.0020),
                         Point<double>(-0.0018, 0.0016),
                         Point<double>(-0.0020, 0.0018)});
        ponds.push_back({Point<double>(-0.0016, 0.0016),
                         Point<double>(-0.0016, 0.0020),
                         Point<double>(-0.0014, 0.0018),
                         Point<double>(-0.0016, 0.0016)});
        ponds_mapinfo.push_back({Point<double>(-0.0020, 0.0018),
                         Point<double>(-0.0018, 0.0020),
                         Point<double>(-0.0018, 0.0016),
                         Point<double>(-0.0020, 0.0018)});
        ponds_mapinfo.push_back({Point<double>(-0.0014, 0.0018),
                         Point<double>(-0.0016, 0.0020),
                         Point<double>(-0.0016, 0.0016),
                         Point<double>(-0.0014, 0.0018)});
        ponds_data_id = "120";
        ponds_data_name = "Stock Pond";


        buildings_name = "Buildings";
        buildings_point.emplace_back(0.0010, 0.0006);
        buildings_point.emplace_back(0.0022, 0.0009);
        buildings.push_back({Point<double>(0.0008, 0.0005),
                             Point<double>(0.0008, 0.0007),
                             Point<double>(0.0012, 0.0007),
                             Point<double>(0.0012, 0.0005),
                             Point<double>(0.0008, 0.0005)});
        buildings.push_back({Point<double>(0.0020, 0.0008),
                             Point<double>(0.0020, 0.0010),
                             Point<double>(0.0024, 0.0010),
                             Point<double>(0.0024, 0.0008),
                             Point<double>(0.0020, 0.0008)});

        buildings_data_ids.emplace_back("113");
        buildings_data_ids.emplace_back("114");
        buildings_data_names.emplace_back("123 Main Street");
        buildings_data_names.emplace_back("215 Main Street");


        lakes_name = "Lakes";
        lakes.push_back({Point<double>(0.0006, -0.0018),
                         Point<double>(0.0010, -0.0006),
                         Point<double>(0.0024, -0.0001),
                         Point<double>(0.0031, -0.0015),
                         Point<double>(0.0006, -0.0018)});
        lakes.push_back({Point<double>(0.0017, -0.0011),
                         Point<double>(0.0017, -0.0006),
                         Point<double>(0.0025, -0.0006),
                         Point<double>(0.0025, -0.0011),
                         Point<double>(0.0017, -0.0011)});
        lakes.push_back({Point<double>(0.0017, -0.0011),
                         Point<double>(0.0025, -0.0011),
                         Point<double>(0.0025, -0.0006),
                         Point<double>(0.0017, -0.0006),
                         Point<double>(0.0017, -0.0011)});
        lakes_data_id = "101";
        lakes_data_name = "Blue Lake";


        lakes_with_elevations_name = "LakesWithElevation";
        lakes_with_elevations.push_back({Point<double>(0.0006, -0.0018),
                         Point<double>(0.0010, -0.0006),
                         Point<double>(0.0024, -0.0001),
                         Point<double>(0.0031, -0.0015),
                         Point<double>(0.0006, -0.0018)});
        lakes_with_elevations.push_back({Point<double>(0.0010, -0.0016),
                                         Point<double>(0.0012, -0.0006),
                                         Point<double>(0.0016, -0.0005),
                                         Point<double>(0.0016, -0.0012),
                                         Point<double>(0.0024, -0.0012),
                                         Point<double>(0.0024, -0.0014),
                                         Point<double>(0.0010, -0.0016)});
        lakes_with_elevations.push_back({Point<double>(0.0011, -0.0015),
                                         Point<double>(0.0013, -0.0007),
                                         Point<double>(0.0015, -0.0007),
                                         Point<double>(0.0015, -0.0013),
                                         Point<double>(0.0011, -0.0015)});
        lakes_with_elevations_inner.push_back({Point<double>(0.0017, -0.0011),
                                               Point<double>(0.0025, -0.0011),
                                               Point<double>(0.0025, -0.0006),
                                               Point<double>(0.0017, -0.0006),
                                               Point<double>(0.0017, -0.0011)});
        lakes_with_elevations_inner_mapinfo.push_back({Point<double>(0.0017, -0.0011),
                                                       Point<double>(0.0017, -0.0006),
                                               Point<double>(0.0025, -0.0006),
                                               Point<double>(0.0025, -0.0011),
                                               Point<double>(0.0017, -0.0011)});
        lakes_with_elevations_data_id_shape = "101";
        lakes_with_elevations_data_name = "Blue Lake";
        lakes_with_elevations_data_ids = {"101.1", "101.2", "101.3"};
        lakes_with_elevations_data_elevations = {"500", "490", "480"};
    }

    void teardown()
    {
    }

    std::string autos_name;
    std::vector<tl::Point<double>> autos_points;

    std::string bridges_name;
    tl::Point<double> bridges_point;
    std::string bridges_data_id;
    std::string bridges_data_name;

    std::string road_segments_name;
    std::vector<LineString<Point<double>>> roads;
    std::vector<std::string> road_segments_data_ids;
    std::vector<std::string> road_segments_data_names;

    std::string streams_name;
    std::vector<LineString<Point<double>>> streams;
    std::vector<std::string> streams_data_ids;
    std::vector<std::string> streams_data_names;

    std::string forest_name;
    tl::Polygon<Point<double>> forest;
    std::string forest_data_id;
    std::string forest_data_name;

    std::string divided_routes_name;
    tl::MultiLineString<Point<double>> divided_routes;
    std::string divided_routes_data_id;
    std::string divided_routes_data_name;

    std::string basic_polygons_name;
    std::vector<tl::Polygon<Point<double>>> basic_polygons;

    std::string ponds_name;
    std::vector<tl::Polygon<Point<double>>> ponds;
    std::vector<tl::Polygon<Point<double>>> ponds_mapinfo;
    std::string ponds_data_id;
    std::string ponds_data_name;

    std::string buildings_name;
    std::vector<Point<double>> buildings_point;
    std::vector<tl::Polygon<Point<double>>> buildings;
    std::vector<std::string> buildings_data_ids;
    std::vector<std::string> buildings_data_names;

    std::string lakes_name;
    std::vector<tl::Polygon<Point<double>>> lakes;
    std::string lakes_data_id;
    std::string lakes_data_name;  
    
    std::string lakes_with_elevations_name;
    std::vector<tl::Polygon<Point<double>>> lakes_with_elevations;
    std::vector<tl::Polygon<Point<double>>> lakes_with_elevations_inner;
    std::vector<tl::Polygon<Point<double>>> lakes_with_elevations_inner_mapinfo;
    std::string lakes_with_elevations_data_id_shape;
    std::vector<std::string> lakes_with_elevations_data_ids;
    std::string lakes_with_elevations_data_name;
    std::vector<std::string> lakes_with_elevations_data_elevations;
};


BOOST_FIXTURE_TEST_CASE(read_shape_point, VectorReaderTest)
{
    
    try {

        tl::Path file(tl::Path(TL_DATA_PATH).append("\\OGC_WMS_TEST\\shapefile\\Autos.shp"));
        std::unique_ptr<VectorReader> vector_reader = VectorReaderFactory::create(file);
        vector_reader->open();
        if (!vector_reader->isOpen()) return;

        int layers_count = vector_reader->layersCount();
        BOOST_CHECK_EQUAL(1, layers_count);

        std::shared_ptr<GLayer> layer = vector_reader->read(0);

        BOOST_CHECK_EQUAL(autos_name, layer->name());

        int i = 0;
        for (auto &entity : *layer) {

            std::shared_ptr<GPoint> point = std::dynamic_pointer_cast<GPoint>(entity);
            BOOST_CHECK_EQUAL(autos_points.at(i).x, point->x);
            BOOST_CHECK_EQUAL(autos_points.at(i).y, point->y);

            point->data();

            i++;
        }

        vector_reader->close();

    } catch (std::exception &e) {
        tl::printException(e);
    }

    try {

        tl::Path file(tl::Path(TL_DATA_PATH).append("\\OGC_WMS_TEST\\shapefile\\Bridges.shp"));
        std::unique_ptr<VectorReader> vector_reader = VectorReaderFactory::create(file);
        vector_reader->open();
        if (!vector_reader->isOpen()) return;

        int layers_count = vector_reader->layersCount();
        BOOST_CHECK_EQUAL(1, layers_count);

        std::shared_ptr<GLayer> layer = vector_reader->read(0);

        BOOST_CHECK_EQUAL(bridges_name, layer->name());

        int i = 0;
        for (auto &entity : *layer) {

            std::shared_ptr<GPoint> point = std::dynamic_pointer_cast<GPoint>(entity);
            BOOST_CHECK_EQUAL(bridges_point.x, point->x);
            BOOST_CHECK_EQUAL(bridges_point.y, point->y);

            auto data = point->data();
            BOOST_CHECK_EQUAL(bridges_data_id, data->value(0));
            BOOST_CHECK_EQUAL(bridges_data_name, data->value(1));

            i++;
        }

        vector_reader->close();

    } catch (std::exception &e) {
        tl::printException(e);
    }

}

BOOST_FIXTURE_TEST_CASE(read_gml_point, VectorReaderTest)
{
    
    try {

        tl::Path file(tl::Path(TL_DATA_PATH).append("\\OGC_WMS_TEST\\gml\\Autos.xml"));
        std::unique_ptr<VectorReader> vector_reader = VectorReaderFactory::create(file);
        vector_reader->open();
        if (!vector_reader->isOpen()) return;

        int layers_count = vector_reader->layersCount();
        BOOST_CHECK_EQUAL(1, layers_count);

        std::shared_ptr<GLayer> layer = vector_reader->read(0);

        BOOST_CHECK_EQUAL("AutoFeature", layer->name());

        int i = 0;
        for (auto &entity : *layer) {

            std::shared_ptr<GPoint> point = std::dynamic_pointer_cast<GPoint>(entity);
            BOOST_CHECK_EQUAL(autos_points.at(i).x, point->x);
            BOOST_CHECK_EQUAL(autos_points.at(i).y, point->y);

            point->data();

            i++;
        }

        vector_reader->close();

    } catch (std::exception &e) {
        tl::printException(e);
    }

    try {

        tl::Path file(tl::Path(TL_DATA_PATH).append("\\OGC_WMS_TEST\\gml\\Bridges.xml"));
        std::unique_ptr<VectorReader> vector_reader = VectorReaderFactory::create(file);
        vector_reader->open();
        if (!vector_reader->isOpen()) return;

        int layers_count = vector_reader->layersCount();
        BOOST_CHECK_EQUAL(1, layers_count);

        std::shared_ptr<GLayer> layer = vector_reader->read(0);

        BOOST_CHECK_EQUAL("BridgeFeature", layer->name());

        int i = 0;
        for (auto &entity : *layer) {

            std::shared_ptr<GPoint> point = std::dynamic_pointer_cast<GPoint>(entity);
            BOOST_CHECK_EQUAL(bridges_point.x, point->x);
            BOOST_CHECK_EQUAL(bridges_point.y, point->y);

            auto data = point->data();
            BOOST_CHECK_EQUAL("BlueLake.110", data->value(0));
            BOOST_CHECK_EQUAL(bridges_data_name, data->value(1));

            i++;
        }

        vector_reader->close();

    } catch (std::exception &e) {
        tl::printException(e);
    }
}

BOOST_FIXTURE_TEST_CASE(read_mapinfo_point, VectorReaderTest)
{
    
    try {

        tl::Path file(tl::Path(TL_DATA_PATH).append("\\OGC_WMS_TEST\\mapinfo\\Autos.mif"));
        std::unique_ptr<VectorReader> vector_reader = VectorReaderFactory::create(file);
        vector_reader->open();
        if (!vector_reader->isOpen()) return;

        int layers_count = vector_reader->layersCount();
        BOOST_CHECK_EQUAL(1, layers_count);

        std::shared_ptr<GLayer> layer = vector_reader->read(0);

        BOOST_CHECK_EQUAL(autos_name, layer->name());

        int i = 0;
        for (auto &entity : *layer) {

            std::shared_ptr<GPoint> point = std::dynamic_pointer_cast<GPoint>(entity);
            BOOST_CHECK_EQUAL(autos_points.at(i).x, point->x);
            BOOST_CHECK_EQUAL(autos_points.at(i).y, point->y);

            point->data();

            i++;
        }

        vector_reader->close();

    } catch (std::exception &e) {
        tl::printException(e);
    }

    
    try {

        tl::Path file(tl::Path(TL_DATA_PATH).append("\\OGC_WMS_TEST\\mapinfo\\Bridges.mif"));
        std::unique_ptr<VectorReader> vector_reader = VectorReaderFactory::create(file);
        vector_reader->open();
        if (!vector_reader->isOpen()) return;

        int layers_count = vector_reader->layersCount();
        BOOST_CHECK_EQUAL(1, layers_count);

        std::shared_ptr<GLayer> layer = vector_reader->read(0);

        BOOST_CHECK_EQUAL(bridges_name, layer->name());

        int i = 0;
        for (auto &entity : *layer) {

            std::shared_ptr<GPoint> point = std::dynamic_pointer_cast<GPoint>(entity);
            BOOST_CHECK_EQUAL(bridges_point.x, point->x);
            BOOST_CHECK_EQUAL(bridges_point.y, point->y);

            auto data = point->data();
            BOOST_CHECK_EQUAL(bridges_data_id, data->value(0));
            BOOST_CHECK_EQUAL(bridges_data_name, data->value(1));

            i++;
        }

        vector_reader->close();

    } catch (std::exception &e) {
        tl::printException(e);
    }

    try {

        tl::Path file(tl::Path(TL_DATA_PATH).append("\\OGC_WMS_TEST\\mapinfo\\Buildings.mif"));
        std::unique_ptr<VectorReader> vector_reader = VectorReaderFactory::create(file);
        vector_reader->open();
        if (!vector_reader->isOpen()) return;

        int layers_count = vector_reader->layersCount();
        BOOST_CHECK_EQUAL(1, layers_count);

        std::shared_ptr<GLayer> layer = vector_reader->read(0);

        BOOST_CHECK_EQUAL(buildings_name, layer->name());

        int i = 0;
        for (auto &entity : *layer) {

            if (std::shared_ptr<GPoint> point = std::dynamic_pointer_cast<GPoint>(entity)) {

                BOOST_CHECK_EQUAL(buildings_point.at(i).x, point->x);
                BOOST_CHECK_EQUAL(buildings_point.at(i).y, point->y);

                auto data = point->data();
                BOOST_CHECK_EQUAL(buildings_data_ids.at(i), data->value(0));
                BOOST_CHECK_EQUAL(buildings_data_names.at(i), data->value(1));

                i++;
            }
        }

        vector_reader->close();

    } catch (std::exception &e) {
        tl::printException(e);
    }
}



BOOST_FIXTURE_TEST_CASE(read_shape_linestring, VectorReaderTest)
{
    
    try {

        tl::Path file(tl::Path(TL_DATA_PATH).append("\\OGC_WMS_TEST\\shapefile\\RoadSegments.shp"));
        std::unique_ptr<VectorReader> vector_reader = VectorReaderFactory::create(file);
        vector_reader->open();
        if (!vector_reader->isOpen()) return;

        int layers_count = vector_reader->layersCount();
        BOOST_CHECK_EQUAL(1, layers_count);

        std::shared_ptr<GLayer> layer = vector_reader->read(0);

        BOOST_CHECK_EQUAL(road_segments_name, layer->name());

        int i = 0;
        for (auto &entity : *layer) {

            std::shared_ptr<GLineString> linestring = std::dynamic_pointer_cast<GLineString>(entity);

            auto &road = roads.at(i);
            for (size_t j = 0; j < linestring->size(); j++) {
                BOOST_CHECK_EQUAL(road.at(j).x, linestring->at(j).x);
                BOOST_CHECK_EQUAL(road.at(j).y, linestring->at(j).y);
            }

            auto data = linestring->data();
            BOOST_CHECK_EQUAL(road_segments_data_ids.at(i), data->value(0));
            BOOST_CHECK_EQUAL(road_segments_data_names.at(i), data->value(1));

            i++;
        }

        vector_reader->close();

    } catch (std::exception &e) {
        tl::printException(e);
    }


    try {

        tl::Path file(tl::Path(TL_DATA_PATH).append("\\OGC_WMS_TEST\\shapefile\\Streams.shp"));
        std::unique_ptr<VectorReader> vector_reader = VectorReaderFactory::create(file);
        vector_reader->open();
        if (!vector_reader->isOpen()) return;

        int layers_count = vector_reader->layersCount();
        BOOST_CHECK_EQUAL(1, layers_count);

        std::shared_ptr<GLayer> layer = vector_reader->read(0);

        BOOST_CHECK_EQUAL(streams_name, layer->name());

        int i = 0;
        for (auto &entity : *layer) {

            std::shared_ptr<GLineString> linestring = std::dynamic_pointer_cast<GLineString>(entity);

            auto &stream = streams.at(i);
            for (size_t j = 0; j < linestring->size(); j++) {
                BOOST_CHECK_EQUAL(stream.at(j).x, linestring->at(j).x);
                BOOST_CHECK_EQUAL(stream.at(j).y, linestring->at(j).y);
            }

            auto data = linestring->data();
            BOOST_CHECK_EQUAL(streams_data_ids.at(i), data->value(0));
            BOOST_CHECK_EQUAL(streams_data_names.at(i), data->value(1));

            i++;
        }

        vector_reader->close();

    } catch (std::exception &e) {
        tl::printException(e);
    }
}

BOOST_FIXTURE_TEST_CASE(read_gml_linestring, VectorReaderTest)
{
    
    try {

        tl::Path file(tl::Path(TL_DATA_PATH).append("\\OGC_WMS_TEST\\gml\\RoadSegments.xml"));
        std::unique_ptr<VectorReader> vector_reader = VectorReaderFactory::create(file);
        vector_reader->open();
        if (!vector_reader->isOpen()) return;

        int layers_count = vector_reader->layersCount();
        BOOST_CHECK_EQUAL(1, layers_count);

        std::shared_ptr<GLayer> layer = vector_reader->read(0);

        BOOST_CHECK_EQUAL("RoadSegmentFeature", layer->name());

        int i = 0;
        for (auto &entity : *layer) {

            std::shared_ptr<GLineString> linestring = std::dynamic_pointer_cast<GLineString>(entity);

            auto &road = roads.at(i);
            for (size_t j = 0; j < linestring->size(); j++) {
                BOOST_CHECK_EQUAL(road.at(j).x, linestring->at(j).x);
                BOOST_CHECK_EQUAL(road.at(j).y, linestring->at(j).y);
            }

            auto data = linestring->data();
            BOOST_CHECK_EQUAL(std::string("BlueLake.").append(road_segments_data_ids.at(i)), data->value(0));
            BOOST_CHECK_EQUAL(road_segments_data_names.at(i), data->value(1));

            i++;
        }

        vector_reader->close();

    } catch (std::exception &e) {
        tl::printException(e);
    }


    try {

        tl::Path file(tl::Path(TL_DATA_PATH).append("\\OGC_WMS_TEST\\gml\\Streams.xml"));
        std::unique_ptr<VectorReader> vector_reader = VectorReaderFactory::create(file);
        vector_reader->open();
        if (!vector_reader->isOpen()) return;

        int layers_count = vector_reader->layersCount();
        BOOST_CHECK_EQUAL(1, layers_count);

        std::shared_ptr<GLayer> layer = vector_reader->read(0);

        BOOST_CHECK_EQUAL("StreamFeature", layer->name());

        int i = 0;
        for (auto &entity : *layer) {

            std::shared_ptr<GLineString> linestring = std::dynamic_pointer_cast<GLineString>(entity);

            auto &stream = streams.at(i);
            for (size_t j = 0; j < linestring->size(); j++) {
                BOOST_CHECK_EQUAL(stream.at(j).x, linestring->at(j).x);
                BOOST_CHECK_EQUAL(stream.at(j).y, linestring->at(j).y);
            }

            auto data = linestring->data();
            BOOST_CHECK_EQUAL(std::string("BlueLake.").append(streams_data_ids.at(i)), data->value(0));
            BOOST_CHECK_EQUAL(streams_data_names.at(i), data->value(1));

            i++;
        }

        vector_reader->close();

    } catch (std::exception &e) {
        tl::printException(e);
    }

}

BOOST_FIXTURE_TEST_CASE(read_mapinfo_linestring, VectorReaderTest)
{
    
    try {

        tl::Path file(tl::Path(TL_DATA_PATH).append("\\OGC_WMS_TEST\\mapinfo\\RoadSegments.mif"));
        std::unique_ptr<VectorReader> vector_reader = VectorReaderFactory::create(file);
        vector_reader->open();
        if (!vector_reader->isOpen()) return;

        int layers_count = vector_reader->layersCount();
        BOOST_CHECK_EQUAL(1, layers_count);

        std::shared_ptr<GLayer> layer = vector_reader->read(0);

        BOOST_CHECK_EQUAL(road_segments_name, layer->name());

        int i = 0;
        for (auto &entity : *layer) {

            std::shared_ptr<GLineString> linestring = std::dynamic_pointer_cast<GLineString>(entity);

            auto &road = roads.at(i);
            for (size_t j = 0; j < linestring->size(); j++) {
                BOOST_CHECK_EQUAL(road.at(j).x, linestring->at(j).x);
                BOOST_CHECK_EQUAL(road.at(j).y, linestring->at(j).y);
            }

            auto data = linestring->data();
            BOOST_CHECK_EQUAL(road_segments_data_ids.at(i), data->value(0));
            BOOST_CHECK_EQUAL(road_segments_data_names.at(i), data->value(1));

            i++;
        }

        vector_reader->close();

    } catch (std::exception &e) {
        tl::printException(e);
    }

    try {

        tl::Path file(tl::Path(TL_DATA_PATH).append("\\OGC_WMS_TEST\\mapinfo\\Streams.mif"));
        std::unique_ptr<VectorReader> vector_reader = VectorReaderFactory::create(file);
        vector_reader->open();
        if (!vector_reader->isOpen()) return;

        int layers_count = vector_reader->layersCount();
        BOOST_CHECK_EQUAL(1, layers_count);

        std::shared_ptr<GLayer> layer = vector_reader->read(0);

        BOOST_CHECK_EQUAL(streams_name, layer->name());

        int i = 0;
        for (auto &entity : *layer) {

            std::shared_ptr<GLineString> linestring = std::dynamic_pointer_cast<GLineString>(entity);

            auto &stream = streams.at(i);
            for (size_t j = 0; j < linestring->size(); j++) {
                BOOST_CHECK_EQUAL(stream.at(j).x, linestring->at(j).x);
                BOOST_CHECK_EQUAL(stream.at(j).y, linestring->at(j).y);
            }

            auto data = linestring->data();
            BOOST_CHECK_EQUAL(streams_data_ids.at(i), data->value(0));
            BOOST_CHECK_EQUAL(streams_data_names.at(i), data->value(1));

            i++;
        }

        vector_reader->close();

    } catch (std::exception &e) {
        tl::printException(e);
    }
}


BOOST_FIXTURE_TEST_CASE(read_shape_polygon, VectorReaderTest)
{
    
    try {

        tl::Path file(tl::Path(TL_DATA_PATH).append("\\OGC_WMS_TEST\\shapefile\\Forests.shp"));
        std::unique_ptr<VectorReader> vector_reader = VectorReaderFactory::create(file);
        vector_reader->open();
        if (!vector_reader->isOpen()) return;

        int layers_count = vector_reader->layersCount();
        BOOST_CHECK_EQUAL(1, layers_count);

        std::shared_ptr<GLayer> layer = vector_reader->read(0);

        BOOST_CHECK_EQUAL(forest_name, layer->name());

        for (auto &entity : *layer) {

            std::shared_ptr<GPolygon> polygon = std::dynamic_pointer_cast<GPolygon>(entity);

            for (size_t j = 0; j < polygon->size(); j++) {
                BOOST_CHECK_EQUAL(forest.at(j).x, polygon->at(j).x);
                BOOST_CHECK_EQUAL(forest.at(j).y, polygon->at(j).y);
            }

            auto data = polygon->data();
            BOOST_CHECK_EQUAL(forest_data_id, data->value(0));
            BOOST_CHECK_EQUAL(forest_data_name, data->value(1));

        }

        vector_reader->close();

    } catch (std::exception &e) {
        tl::printException(e);
    }


    try {

        tl::Path file(tl::Path(TL_DATA_PATH).append("\\OGC_WMS_TEST\\shapefile\\BasicPolygons.shp"));
        std::unique_ptr<VectorReader> vector_reader = VectorReaderFactory::create(file);
        vector_reader->open();
        if (!vector_reader->isOpen()) return;

        int layers_count = vector_reader->layersCount();
        BOOST_CHECK_EQUAL(1, layers_count);

        std::shared_ptr<GLayer> layer = vector_reader->read(0);

        BOOST_CHECK_EQUAL(basic_polygons_name, layer->name());

        int i = 0;
        for (auto &entity : *layer) {

            std::shared_ptr<GPolygon> polygon = std::dynamic_pointer_cast<GPolygon>(entity);

            for (size_t j = 0; j < polygon->size(); j++) {
                BOOST_CHECK_EQUAL(basic_polygons.at(i).at(j).x, polygon->at(j).x);
                BOOST_CHECK_EQUAL(basic_polygons.at(i).at(j).y, polygon->at(j).y);
            }

            i++;
        }

        vector_reader->close();

    } catch (std::exception &e) {
        tl::printException(e);
    }


    try {

        tl::Path file(tl::Path(TL_DATA_PATH).append("\\OGC_WMS_TEST\\shapefile\\Ponds.shp"));
        std::unique_ptr<VectorReader> vector_reader = VectorReaderFactory::create(file);
        vector_reader->open();
        if (!vector_reader->isOpen()) return;

        int layers_count = vector_reader->layersCount();
        BOOST_CHECK_EQUAL(1, layers_count);

        std::shared_ptr<GLayer> layer = vector_reader->read(0);

        BOOST_CHECK_EQUAL(ponds_name, layer->name());

        int i = 0;
        for (auto &entity : *layer) {

            std::shared_ptr<GMultiPolygon> multi_polygon = std::dynamic_pointer_cast<GMultiPolygon>(entity);

            for (size_t j = 0; j < multi_polygon->size(); j++) {
                for (size_t k = 0; k < multi_polygon->at(j).size(); k++) {

                    BOOST_CHECK_EQUAL(ponds.at(j).at(k).x, multi_polygon->at(j).at(k).x);
                    BOOST_CHECK_EQUAL(ponds.at(j).at(k).y, multi_polygon->at(j).at(k).y);

                }
            }
                        
            auto data = multi_polygon->data();
            BOOST_CHECK_EQUAL(ponds_data_id, data->value(0));
            BOOST_CHECK_EQUAL(ponds_data_name, data->value(2));

            i++;
        }

        vector_reader->close();

    } catch (std::exception &e) {
        tl::printException(e);
    }


    try {

        tl::Path file(tl::Path(TL_DATA_PATH).append("\\OGC_WMS_TEST\\shapefile\\Buildings.shp"));
        std::unique_ptr<VectorReader> vector_reader = VectorReaderFactory::create(file);
        vector_reader->open();
        if (!vector_reader->isOpen()) return;

        int layers_count = vector_reader->layersCount();
        BOOST_CHECK_EQUAL(1, layers_count);

        std::shared_ptr<GLayer> layer = vector_reader->read(0);

        BOOST_CHECK_EQUAL(buildings_name, layer->name());

        int i = 0;
        for (auto &entity : *layer) {

            std::shared_ptr<GPolygon> polygon = std::dynamic_pointer_cast<GPolygon>(entity);

            for (size_t j = 0; j < polygon->size(); j++) {
                BOOST_CHECK_EQUAL(buildings.at(i).at(j).x, polygon->at(j).x);
                BOOST_CHECK_EQUAL(buildings.at(i).at(j).y, polygon->at(j).y);
            }

            auto data = polygon->data();
            BOOST_CHECK_EQUAL(buildings_data_ids.at(i), data->value(0));
            BOOST_CHECK_EQUAL(buildings_data_names.at(i), data->value(1));

            i++;
        }

        vector_reader->close();

    } catch (std::exception &e) {
        tl::printException(e);
    }


    try {

        tl::Path file(tl::Path(TL_DATA_PATH).append("\\OGC_WMS_TEST\\shapefile\\Lakes.shp"));
        std::unique_ptr<VectorReader> vector_reader = VectorReaderFactory::create(file);
        vector_reader->open();
        if (!vector_reader->isOpen()) return;

        int layers_count = vector_reader->layersCount();
        BOOST_CHECK_EQUAL(1, layers_count);

        std::shared_ptr<GLayer> layer = vector_reader->read(0);

        BOOST_CHECK_EQUAL(lakes_name, layer->name());

        for (auto &entity : *layer) {

            std::shared_ptr<GPolygon> polygon = std::dynamic_pointer_cast<GPolygon>(entity);

            for (size_t j = 0; j < polygon->size(); j++) {

                BOOST_CHECK_EQUAL(lakes.at(0).at(j).x, polygon->at(j).x);
                BOOST_CHECK_EQUAL(lakes.at(0).at(j).y, polygon->at(j).y);

            }
            
            for (size_t j = 0; j < polygon->holes(); j++) {

                for (size_t k = 0; k < polygon->hole(j).size(); k++) {
                    BOOST_CHECK_EQUAL(lakes.at(2).at(k).x, polygon->hole(j).at(k).x);
                    BOOST_CHECK_EQUAL(lakes.at(2).at(k).y, polygon->hole(j).at(k).y);
                }

            }

            auto data = polygon->data();
            BOOST_CHECK_EQUAL(lakes_data_id, data->value(0));
            BOOST_CHECK_EQUAL(lakes_data_name, data->value(1));

        }

        vector_reader->close();

    } catch (std::exception &e) {
        tl::printException(e);
    }


    try {

        tl::Path file(tl::Path(TL_DATA_PATH).append("\\OGC_WMS_TEST\\shapefile\\LakesWithElevation.shp"));
        std::unique_ptr<VectorReader> vector_reader = VectorReaderFactory::create(file);
        vector_reader->open();
        if (!vector_reader->isOpen()) return;

        int layers_count = vector_reader->layersCount();
        BOOST_CHECK_EQUAL(1, layers_count);

        std::shared_ptr<GLayer> layer = vector_reader->read(0);

        BOOST_CHECK_EQUAL(lakes_with_elevations_name, layer->name());

        int i = 0;

        for (auto &entity : *layer) {

            std::shared_ptr<GPolygon> polygon = std::dynamic_pointer_cast<GPolygon>(entity);

            for (size_t j = 0; j < polygon->size(); j++) {

                BOOST_CHECK_EQUAL(lakes_with_elevations.at(i).at(j).x, polygon->at(j).x);
                BOOST_CHECK_EQUAL(lakes_with_elevations.at(i).at(j).y, polygon->at(j).y);

            }
            
            for (size_t j = 0; j < polygon->holes(); j++) {

                for (size_t k = 0; k < polygon->hole(j).size(); k++) {
                    BOOST_CHECK_EQUAL(lakes_with_elevations_inner.at(i).at(k).x, polygon->hole(j).at(k).x);
                    BOOST_CHECK_EQUAL(lakes_with_elevations_inner.at(i).at(k).y, polygon->hole(j).at(k).y);
                }

            }

            auto data = polygon->data();
            BOOST_CHECK_EQUAL(lakes_with_elevations_data_id_shape, data->value(0));
            BOOST_CHECK_EQUAL(lakes_with_elevations_data_name, data->value(1));
            BOOST_CHECK_EQUAL(tl::convertStringTo<int>(lakes_with_elevations_data_elevations[i]), tl::convertStringTo<int>(data->value(2)));

            i++;
        }

        vector_reader->close();

    } catch (std::exception &e) {
        tl::printException(e);
    }
}

BOOST_FIXTURE_TEST_CASE(read_gml_polygon, VectorReaderTest)
{
    
    try {

        tl::Path file(tl::Path(TL_DATA_PATH).append("\\OGC_WMS_TEST\\gml\\Forests.xml"));
        std::unique_ptr<VectorReader> vector_reader = VectorReaderFactory::create(file);
        vector_reader->open();
        if (!vector_reader->isOpen()) return;

        int layers_count = vector_reader->layersCount();
        BOOST_CHECK_EQUAL(1, layers_count);

        std::shared_ptr<GLayer> layer = vector_reader->read(0);

        BOOST_CHECK_EQUAL("ForestFeature", layer->name());

        for (auto &entity : *layer) {

            std::shared_ptr<GMultiPolygon> multi_polygon = std::dynamic_pointer_cast<GMultiPolygon>(entity);

            for (size_t j = 0; j < multi_polygon->size(); j++) {
                for (size_t k = 0; k < multi_polygon->at(j).size(); k++) {

                    BOOST_CHECK_EQUAL(forest.at(k).x, multi_polygon->at(j).at(k).x);
                    BOOST_CHECK_EQUAL(forest.at(k).y, multi_polygon->at(j).at(k).y);

                }
            }

            auto data = multi_polygon->data();
            BOOST_CHECK_EQUAL(std::string("BlueLake.").append(forest_data_id), data->value(0));
            BOOST_CHECK_EQUAL(forest_data_name, data->value(1));

        }

        vector_reader->close();

    } catch (std::exception &e) {
        tl::printException(e);
    }

    try {

        tl::Path file(tl::Path(TL_DATA_PATH).append("\\OGC_WMS_TEST\\gml\\BasicPolygons.xml"));
        std::unique_ptr<VectorReader> vector_reader = VectorReaderFactory::create(file);
        vector_reader->open();
        if (!vector_reader->isOpen()) return;

        int layers_count = vector_reader->layersCount();
        BOOST_CHECK_EQUAL(1, layers_count);

        std::shared_ptr<GLayer> layer = vector_reader->read(0);

        BOOST_CHECK_EQUAL("PolygonFeature", layer->name());

        int i = 0;
        for (auto &entity : *layer) {

            std::shared_ptr<GPolygon> polygon = std::dynamic_pointer_cast<GPolygon>(entity);

            for (size_t j = 0; j < polygon->size(); j++) {
                BOOST_CHECK_EQUAL(basic_polygons.at(i).at(j).x, polygon->at(j).x);
                BOOST_CHECK_EQUAL(basic_polygons.at(i).at(j).y, polygon->at(j).y);
            }

            i++;
        }

        vector_reader->close();

    } catch (std::exception &e) {
        tl::printException(e);
    }


    try {

        tl::Path file(tl::Path(TL_DATA_PATH).append("\\OGC_WMS_TEST\\gml\\Ponds.xml"));
        std::unique_ptr<VectorReader> vector_reader = VectorReaderFactory::create(file);
        vector_reader->open();
        if (!vector_reader->isOpen()) return;

        int layers_count = vector_reader->layersCount();
        BOOST_CHECK_EQUAL(1, layers_count);

        std::shared_ptr<GLayer> layer = vector_reader->read(0);

        BOOST_CHECK_EQUAL("PondFeature", layer->name());

        int i = 0;
        for (auto &entity : *layer) {

            std::shared_ptr<GMultiPolygon> multi_polygon = std::dynamic_pointer_cast<GMultiPolygon>(entity);

            for (size_t j = 0; j < multi_polygon->size(); j++) {
                for (size_t k = 0; k < multi_polygon->at(j).size(); k++) {

                    BOOST_CHECK_EQUAL(ponds.at(j).at(k).x, multi_polygon->at(j).at(k).x);
                    BOOST_CHECK_EQUAL(ponds.at(j).at(k).y, multi_polygon->at(j).at(k).y);

                }
            }

            auto data = multi_polygon->data();
            BOOST_CHECK_EQUAL(std::string("BlueLake.").append(ponds_data_id), data->value(0));
            BOOST_CHECK_EQUAL(ponds_data_name, data->value(2));

            i++;
        }

        vector_reader->close();

    } catch (std::exception &e) {
        tl::printException(e);
    }

    try {

        tl::Path file(tl::Path(TL_DATA_PATH).append("\\OGC_WMS_TEST\\gml\\Buildings.xml"));
        std::unique_ptr<VectorReader> vector_reader = VectorReaderFactory::create(file);
        vector_reader->open();
        if (!vector_reader->isOpen()) return;

        int layers_count = vector_reader->layersCount();
        BOOST_CHECK_EQUAL(1, layers_count);

        std::shared_ptr<GLayer> layer = vector_reader->read(0);

        BOOST_CHECK_EQUAL("BuildingFeature", layer->name());

        int i = 0;
        for (auto &entity : *layer) {

            std::shared_ptr<GPolygon> polygon = std::dynamic_pointer_cast<GPolygon>(entity);

            for (size_t j = 0; j < polygon->size(); j++) {
                BOOST_CHECK_EQUAL(buildings.at(i).at(j).x, polygon->at(j).x);
                BOOST_CHECK_EQUAL(buildings.at(i).at(j).y, polygon->at(j).y);
            }

            auto data = polygon->data();
            BOOST_CHECK_EQUAL(std::string("BlueLake.").append(buildings_data_ids.at(i)), data->value(0));
            BOOST_CHECK_EQUAL(buildings_data_names.at(i), data->value(1));

            i++;
        }

        vector_reader->close();

    } catch (std::exception &e) {
        tl::printException(e);
    }

    try {

        tl::Path file(tl::Path(TL_DATA_PATH).append("\\OGC_WMS_TEST\\gml\\Lakes.xml"));
        std::unique_ptr<VectorReader> vector_reader = VectorReaderFactory::create(file);
        vector_reader->open();
        if (!vector_reader->isOpen()) return;

        int layers_count = vector_reader->layersCount();
        BOOST_CHECK_EQUAL(1, layers_count);

        std::shared_ptr<GLayer> layer = vector_reader->read(0);

        BOOST_CHECK_EQUAL("LakeFeature", layer->name());

        for (auto &entity : *layer) {

            std::shared_ptr<GPolygon> polygon = std::dynamic_pointer_cast<GPolygon>(entity);

            for (size_t j = 0; j < polygon->size(); j++) {

                BOOST_CHECK_EQUAL(lakes.at(0).at(j).x, polygon->at(j).x);
                BOOST_CHECK_EQUAL(lakes.at(0).at(j).y, polygon->at(j).y);

            }

            for (size_t j = 0; j < polygon->holes(); j++) {

                for (size_t k = 0; k < polygon->hole(j).size(); k++) {
                    BOOST_CHECK_EQUAL(lakes.at(2).at(k).x, polygon->hole(j).at(k).x);
                    BOOST_CHECK_EQUAL(lakes.at(2).at(k).y, polygon->hole(j).at(k).y);
                }

            }

            auto data = polygon->data();
            BOOST_CHECK_EQUAL(std::string("BlueLake.").append(lakes_data_id), data->value(0));
            BOOST_CHECK_EQUAL(lakes_data_name, data->value(1));

        }

        vector_reader->close();

    } catch (std::exception &e) {
        tl::printException(e);
    }

    try {

        tl::Path file(tl::Path(TL_DATA_PATH).append("\\OGC_WMS_TEST\\gml\\LakesWithElevation.xml"));
        std::unique_ptr<VectorReader> vector_reader = VectorReaderFactory::create(file);
        vector_reader->open();
        if (!vector_reader->isOpen()) return;

        int layers_count = vector_reader->layersCount();
        BOOST_CHECK_EQUAL(1, layers_count);

        std::shared_ptr<GLayer> layer = vector_reader->read(0);

        BOOST_CHECK_EQUAL("LakeFeature", layer->name());

        int i = 0;

        for (auto &entity : *layer) {

            std::shared_ptr<GPolygon> polygon = std::dynamic_pointer_cast<GPolygon>(entity);

            for (size_t j = 0; j < polygon->size(); j++) {

                BOOST_CHECK_EQUAL(lakes_with_elevations.at(i).at(j).x, polygon->at(j).x);
                BOOST_CHECK_EQUAL(lakes_with_elevations.at(i).at(j).y, polygon->at(j).y);

            }
            
            for (size_t j = 0; j < polygon->holes(); j++) {

                for (size_t k = 0; k < polygon->hole(j).size(); k++) {
                    BOOST_CHECK_EQUAL(lakes_with_elevations_inner.at(i).at(k).x, polygon->hole(j).at(k).x);
                    BOOST_CHECK_EQUAL(lakes_with_elevations_inner.at(i).at(k).y, polygon->hole(j).at(k).y);
                }

            }

            auto data = polygon->data();
            BOOST_CHECK_EQUAL(std::string("BlueLake.").append(lakes_with_elevations_data_ids.at(i)), data->value(0));
            BOOST_CHECK_EQUAL(lakes_with_elevations_data_name, data->value(1));
            BOOST_CHECK_EQUAL(lakes_with_elevations_data_elevations.at(i), data->value(2));

            i++;
        }

        vector_reader->close();

    } catch (std::exception &e) {
        tl::printException(e);
    }
}

BOOST_FIXTURE_TEST_CASE(read_mapinfo_polygon, VectorReaderTest)
{
    
    try {

        tl::Path file(tl::Path(TL_DATA_PATH).append("\\OGC_WMS_TEST\\mapinfo\\Forests.mif"));
        std::unique_ptr<VectorReader> vector_reader = VectorReaderFactory::create(file);
        vector_reader->open();
        if (!vector_reader->isOpen()) return;

        int layers_count = vector_reader->layersCount();
        BOOST_CHECK_EQUAL(1, layers_count);

        std::shared_ptr<GLayer> layer = vector_reader->read(0);

        BOOST_CHECK_EQUAL(forest_name, layer->name());

        for (auto &entity : *layer) {

            std::shared_ptr<GPolygon> polygon = std::dynamic_pointer_cast<GPolygon>(entity);

            for (size_t j = 0; j < polygon->size(); j++) {
                BOOST_CHECK_EQUAL(forest.at(j).x, polygon->at(j).x);
                BOOST_CHECK_EQUAL(forest.at(j).y, polygon->at(j).y);
            }

            auto data = polygon->data();
            BOOST_CHECK_EQUAL(forest_data_id, data->value(0));
            BOOST_CHECK_EQUAL(forest_data_name, data->value(1));

        }

        vector_reader->close();

    } catch (std::exception &e) {
        tl::printException(e);
    }


    try {

        tl::Path file(tl::Path(TL_DATA_PATH).append("\\OGC_WMS_TEST\\mapinfo\\BasicPolygons.mif"));
        std::unique_ptr<VectorReader> vector_reader = VectorReaderFactory::create(file);
        vector_reader->open();
        if (!vector_reader->isOpen()) return;

        int layers_count = vector_reader->layersCount();
        BOOST_CHECK_EQUAL(1, layers_count);

        std::shared_ptr<GLayer> layer = vector_reader->read(0);

        BOOST_CHECK_EQUAL(basic_polygons_name, layer->name());

        int i = 0;
        for (auto &entity : *layer) {

            std::shared_ptr<GPolygon> polygon = std::dynamic_pointer_cast<GPolygon>(entity);

            for (size_t j = 0; j < polygon->size(); j++) {
                BOOST_CHECK_EQUAL(basic_polygons.at(i).at(j).x, polygon->at(j).x);
                BOOST_CHECK_EQUAL(basic_polygons.at(i).at(j).y, polygon->at(j).y);
            }

            i++;
        }

        vector_reader->close();

    } catch (std::exception &e) {
        tl::printException(e);
    }

    try {

        tl::Path file(tl::Path(TL_DATA_PATH).append("\\OGC_WMS_TEST\\mapinfo\\Ponds.mif"));
        std::unique_ptr<VectorReader> vector_reader = VectorReaderFactory::create(file);
        vector_reader->open();
        if (!vector_reader->isOpen()) return;

        int layers_count = vector_reader->layersCount();
        BOOST_CHECK_EQUAL(1, layers_count);

        std::shared_ptr<GLayer> layer = vector_reader->read(0);

        BOOST_CHECK_EQUAL(ponds_name, layer->name());

        int i = 0;
        for (auto &entity : *layer) {

            std::shared_ptr<GMultiPolygon> multi_polygon = std::dynamic_pointer_cast<GMultiPolygon>(entity);

            for (size_t j = 0; j < multi_polygon->size(); j++) {
                for (size_t k = 0; k < multi_polygon->at(j).size(); k++) {

                    BOOST_CHECK_EQUAL(ponds_mapinfo.at(j).at(k).x, multi_polygon->at(j).at(k).x);
                    BOOST_CHECK_EQUAL(ponds_mapinfo.at(j).at(k).y, multi_polygon->at(j).at(k).y);

                }
            }
                        
            auto data = multi_polygon->data();
            BOOST_CHECK_EQUAL(ponds_data_id, data->value(0));
            BOOST_CHECK_EQUAL(ponds_data_name, data->value(2));

            i++;
        }

        vector_reader->close();

    } catch (std::exception &e) {
        tl::printException(e);
    }


    try {

        tl::Path file(tl::Path(TL_DATA_PATH).append("\\OGC_WMS_TEST\\mapinfo\\Buildings.mif"));
        std::unique_ptr<VectorReader> vector_reader = VectorReaderFactory::create(file);
        vector_reader->open();
        if (!vector_reader->isOpen()) return;

        int layers_count = vector_reader->layersCount();
        BOOST_CHECK_EQUAL(1, layers_count);

        std::shared_ptr<GLayer> layer = vector_reader->read(0);

        BOOST_CHECK_EQUAL(buildings_name, layer->name());

        int i = 0;
        for (auto &entity : *layer) {

            if (std::shared_ptr<GPolygon> polygon = std::dynamic_pointer_cast<GPolygon>(entity)) {

                for (size_t j = 0; j < polygon->size(); j++) {
                    BOOST_CHECK_EQUAL(buildings.at(i).at(j).x, polygon->at(j).x);
                    BOOST_CHECK_EQUAL(buildings.at(i).at(j).y, polygon->at(j).y);
                }

                auto data = polygon->data();
                BOOST_CHECK_EQUAL(buildings_data_ids.at(i), data->value(0));
                BOOST_CHECK_EQUAL(buildings_data_names.at(i), data->value(1));

                i++;
            }
        }

        vector_reader->close();

    } catch (std::exception &e) {
        tl::printException(e);
    }


    try {

        tl::Path file(tl::Path(TL_DATA_PATH).append("\\OGC_WMS_TEST\\mapinfo\\Lakes.mif"));
        std::unique_ptr<VectorReader> vector_reader = VectorReaderFactory::create(file);
        vector_reader->open();
        if (!vector_reader->isOpen()) return;

        int layers_count = vector_reader->layersCount();
        BOOST_CHECK_EQUAL(1, layers_count);

        std::shared_ptr<GLayer> layer = vector_reader->read(0);

        BOOST_CHECK_EQUAL(lakes_name, layer->name());

        for (auto &entity : *layer) {

            std::shared_ptr<GPolygon> polygon = std::dynamic_pointer_cast<GPolygon>(entity);

            for (size_t j = 0; j < polygon->size(); j++) {

                BOOST_CHECK_EQUAL(lakes.at(0).at(j).x, polygon->at(j).x);
                BOOST_CHECK_EQUAL(lakes.at(0).at(j).y, polygon->at(j).y);

            }
            
            for (size_t j = 0; j < polygon->holes(); j++) {

                for (size_t k = 0; k < polygon->hole(j).size(); k++) {
                    BOOST_CHECK_EQUAL(lakes.at(1).at(k).x, polygon->hole(j).at(k).x);
                    BOOST_CHECK_EQUAL(lakes.at(1).at(k).y, polygon->hole(j).at(k).y);
                }

            }   

            auto data = polygon->data();
            BOOST_CHECK_EQUAL(lakes_data_id, data->value(0));
            BOOST_CHECK_EQUAL(lakes_data_name, data->value(1));

        }

        vector_reader->close();

    } catch (std::exception &e) {
        tl::printException(e);
    }

    try {

        tl::Path file(tl::Path(TL_DATA_PATH).append("\\OGC_WMS_TEST\\mapinfo\\LakesWithElevation.mif"));
        std::unique_ptr<VectorReader> vector_reader = VectorReaderFactory::create(file);
        vector_reader->open();
        if (!vector_reader->isOpen()) return;

        int layers_count = vector_reader->layersCount();
        BOOST_CHECK_EQUAL(1, layers_count);

        std::shared_ptr<GLayer> layer = vector_reader->read(0);

        BOOST_CHECK_EQUAL(lakes_with_elevations_name, layer->name());

        int i = 0;

        for (auto &entity : *layer) {

            std::shared_ptr<GPolygon> polygon = std::dynamic_pointer_cast<GPolygon>(entity);

            for (size_t j = 0; j < polygon->size(); j++) {

                BOOST_CHECK_EQUAL(lakes_with_elevations.at(i).at(j).x, polygon->at(j).x);
                BOOST_CHECK_EQUAL(lakes_with_elevations.at(i).at(j).y, polygon->at(j).y);

            }
            
            for (size_t j = 0; j < polygon->holes(); j++) {

                for (size_t k = 0; k < polygon->hole(j).size(); k++) {
                    BOOST_CHECK_EQUAL(lakes_with_elevations_inner_mapinfo.at(i).at(k).x, polygon->hole(j).at(k).x);
                    BOOST_CHECK_EQUAL(lakes_with_elevations_inner_mapinfo.at(i).at(k).y, polygon->hole(j).at(k).y);
                }

            }

            auto data = polygon->data();
            BOOST_CHECK_EQUAL(lakes_with_elevations_data_id_shape, data->value(0));
            BOOST_CHECK_EQUAL(lakes_with_elevations_data_name, data->value(1));
            BOOST_CHECK_EQUAL(tl::convertStringTo<int>(lakes_with_elevations_data_elevations[i]), tl::convertStringTo<int>(data->value(2)));

            i++;
        }

        vector_reader->close();

    } catch (std::exception &e) {
        tl::printException(e);
    }
}


BOOST_FIXTURE_TEST_CASE(read_shape_multilinestring, VectorReaderTest)
{
    
    try {

        tl::Path file(tl::Path(TL_DATA_PATH).append("\\OGC_WMS_TEST\\shapefile\\DividedRoutes.shp"));
        std::unique_ptr<VectorReader> vector_reader = VectorReaderFactory::create(file);
        vector_reader->open();
        if (!vector_reader->isOpen()) return;

        int layers_count = vector_reader->layersCount();
        BOOST_CHECK_EQUAL(1, layers_count);

        std::shared_ptr<GLayer> layer = vector_reader->read(0);

        BOOST_CHECK_EQUAL(divided_routes_name, layer->name());

        for (auto &entity : *layer) {

            auto multilinestring = std::dynamic_pointer_cast<GMultiLineString>(entity);

            for (size_t j = 0; j < multilinestring->size(); j++) {
                for (size_t k = 0; k < multilinestring->at(j).size(); k++) {

                    BOOST_CHECK_EQUAL(divided_routes.at(j).at(k).x, multilinestring->at(j).at(k).x);
                    BOOST_CHECK_EQUAL(divided_routes.at(j).at(k).y, multilinestring->at(j).at(k).y);

                }
            }

            auto data = multilinestring->data();
            BOOST_CHECK_EQUAL(divided_routes_data_id, data->value(0));
            BOOST_CHECK_EQUAL(divided_routes_data_name, data->value(1));

        }

        vector_reader->close();

    } catch (std::exception &e) {
        tl::printException(e);
    }
}

BOOST_FIXTURE_TEST_CASE(read_gml_multilinestring, VectorReaderTest)
{
    
    try {

        tl::Path file(tl::Path(TL_DATA_PATH).append("\\OGC_WMS_TEST\\gml\\DividedRoutes.xml"));
        std::unique_ptr<VectorReader> vector_reader = VectorReaderFactory::create(file);
        vector_reader->open();
        if (!vector_reader->isOpen()) return;

        int layers_count = vector_reader->layersCount();
        BOOST_CHECK_EQUAL(1, layers_count);

        std::shared_ptr<GLayer> layer = vector_reader->read(0);

        BOOST_CHECK_EQUAL("DividedRouteFeature", layer->name());

        for (auto &entity : *layer) {

            auto multilinestring = std::dynamic_pointer_cast<GMultiLineString>(entity);

            for (size_t j = 0; j < multilinestring->size(); j++) {
                for (size_t k = 0; k < multilinestring->at(j).size(); k++) {

                    BOOST_CHECK_EQUAL(divided_routes.at(j).at(k).x, multilinestring->at(j).at(k).x);
                    BOOST_CHECK_EQUAL(divided_routes.at(j).at(k).y, multilinestring->at(j).at(k).y);

                }
            }

            auto data = multilinestring->data();
            BOOST_CHECK_EQUAL(std::string("BlueLake.").append(divided_routes_data_id), data->value(0));
            BOOST_CHECK_EQUAL(divided_routes_data_name, data->value(1));

        }

        vector_reader->close();

    } catch (std::exception &e) {
        tl::printException(e);
    }
}

BOOST_FIXTURE_TEST_CASE(read_mapinfo_multilinestring, VectorReaderTest)
{
    
    try {

        tl::Path file(tl::Path(TL_DATA_PATH).append("\\OGC_WMS_TEST\\mapinfo\\DividedRoutes.mif"));
        std::unique_ptr<VectorReader> vector_reader = VectorReaderFactory::create(file);
        vector_reader->open();
        if (!vector_reader->isOpen()) return;

        int layers_count = vector_reader->layersCount();
        BOOST_CHECK_EQUAL(1, layers_count);

        std::shared_ptr<GLayer> layer = vector_reader->read(0);

        BOOST_CHECK_EQUAL(divided_routes_name, layer->name());

        for (auto &entity : *layer) {

            auto multilinestring = std::dynamic_pointer_cast<GMultiLineString>(entity);

            for (size_t j = 0; j < multilinestring->size(); j++) {
                for (size_t k = 0; k < multilinestring->at(j).size(); k++) {
                    
                    BOOST_CHECK_EQUAL(divided_routes.at(j).at(k).x, multilinestring->at(j).at(k).x);
                    BOOST_CHECK_EQUAL(divided_routes.at(j).at(k).y, multilinestring->at(j).at(k).y);

                }
            }

            auto data = multilinestring->data();
            BOOST_CHECK_EQUAL(divided_routes_data_id, data->value(0));
            BOOST_CHECK_EQUAL(divided_routes_data_name, data->value(1));

        }

        vector_reader->close();

    } catch (std::exception &e) {
        tl::printException(e);
    }

}

BOOST_AUTO_TEST_SUITE_END()