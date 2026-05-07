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
 
#define BOOST_TEST_MODULE Tidop LineString test
#include <boost/test/unit_test.hpp>
#include <tidop/geometry/primitives/LineString.h>
#include <tidop/geometry/primitives/MultiLineString.h>
#include <tidop/geometry/spatial/BoundingBox.h>
#include <tidop/geometry/io/wkt/Proxy.h>
#include <tidop/geometry/algorithms/measurement/Length.h>
#include <tidop/geometry/algorithms/spatial/Envelope.h>

using namespace tl;


BOOST_AUTO_TEST_SUITE(LineStringTestSuite)

std::initializer_list<Point2i> initializer_list_line_string_integer
{
    Point2i(34, 34),
    Point2i(45, 54),
    Point2i(51, 45),
    Point2i(12, 47),
    Point2i(95, 25),
    Point2i(15, 36),
    Point2i(18, 82)
};

std::initializer_list<Point2d> initializer_list_line_string_double 
{
    Point2d(41572.22, 6647.89),
    Point2d(41490.43, 6888.36),
    Point2d(41728.03, 6903.40),
    Point2d(41771.48, 6429.97),
    Point2d(41370.12, 6718.08),
    Point2d(41462.92, 6669.53),
    Point2d(41387.59, 7026.70)
};



struct LineStringTest
{

    LineStringTest()
      : line_string_integer(nullptr),
        line_string_double(nullptr),
        line_string_float(nullptr),
        line_string_integer_copy(nullptr),
        line_string_double_copy(nullptr),
        line_string_float_copy(nullptr),
        line_string_constructor_reserve(nullptr),
        line_string_constructor_vector(nullptr)
    {

    }

    ~LineStringTest()
    {
        delete line_string_integer;
        delete line_string_double;
        delete line_string_float;
        delete line_string_integer_copy;
        delete line_string_double_copy;
        delete line_string_float_copy;
        delete line_string_constructor_reserve;
        delete line_string_constructor_vector;
    }

    void setup()
    {

        v_d.push_back(Point2d(41572.22, 6647.89));
        v_d.push_back(Point2d(41490.43, 6888.36));
        v_d.push_back(Point2d(41728.03, 6903.40));
        v_d.push_back(Point2d(41771.48, 6429.97));
        v_d.push_back(Point2d(41370.12, 6718.08));
        v_d.push_back(Point2d(41462.92, 6669.53));
        v_d.push_back(Point2d(41387.59, 7026.70));

        line_string_integer = new LineString<Point2i>(initializer_list_line_string_integer);
        line_string_double = new LineString<Point2d>(initializer_list_line_string_double);
        line_string_float = new LineString<Point<float>>();

        line_string_integer_copy = new LineString<Point2i>(*line_string_integer);
        line_string_double_copy = new LineString<Point2d>(*line_string_double);
        line_string_float_copy = new LineString<Point<float>>(*line_string_float);

        line_string_constructor_reserve = new LineString<Point2i>(10);

        line_string_constructor_vector = new LineString<Point2d>(v_d);
    }

    void teardown()
    {

    }

    std::vector<Point2d> v_d;

    LineString<Point2i> line_string_default_constructor_integer;
    LineString<Point2d> line_string_default_constructor_double;
    LineString<Point<float>> line_string_default_constructor_float;

    LineString<Point2i> *line_string_integer;
    LineString<Point2d> *line_string_double;
    LineString<Point<float>> *line_string_float;
    LineString<Point2i> *line_string_integer_copy;
    LineString<Point2d> *line_string_double_copy;
    LineString<Point<float>> *line_string_float_copy;

    LineString<Point2i> *line_string_constructor_reserve;

    LineString<Point2d> *line_string_constructor_vector;
};

BOOST_FIXTURE_TEST_CASE(copy_constructor, LineStringTest)
{
  for (size_t i = 0; i < line_string_double_copy->size(); i++) {
		BOOST_CHECK_EQUAL(v_d[i].x(), line_string_double_copy->at(i).x());
		BOOST_CHECK_EQUAL(v_d[i].y(), line_string_double_copy->at(i).y());
	}


  size_t j = 0;
	for (const auto &pt : initializer_list_line_string_integer) {
		BOOST_CHECK_EQUAL(pt.x(), line_string_integer_copy->at(j).x());
		BOOST_CHECK_EQUAL(pt.y(), line_string_integer_copy->at(j).y());
		j++;
	}
}

BOOST_FIXTURE_TEST_CASE(move_constructor, LineStringTest)
{
    LineString<Point2d> line_to_move(v_d);
    LineString<Point2d> lineString(std::move(line_to_move));

    BOOST_CHECK_EQUAL(7, lineString.size());
    BOOST_CHECK_EQUAL(0, line_to_move.size());

    for (int i = 0; i < lineString.size(); i++) {
        BOOST_CHECK(lineString[i] == v_d[i]);
    }

}

BOOST_FIXTURE_TEST_CASE(size, LineStringTest)
{
    BOOST_CHECK_EQUAL(0, line_string_default_constructor_integer.size());
    BOOST_CHECK_EQUAL(0, line_string_default_constructor_double.size());
    BOOST_CHECK_EQUAL(0, line_string_default_constructor_float.size());
    BOOST_CHECK_EQUAL(7, line_string_integer->size());
    BOOST_CHECK_EQUAL(7, line_string_double->size());
    BOOST_CHECK_EQUAL(0, line_string_float->size());
    BOOST_CHECK_EQUAL(7, line_string_integer_copy->size());
    BOOST_CHECK_EQUAL(7, line_string_double_copy->size());
    BOOST_CHECK_EQUAL(0, line_string_float_copy->size());
    BOOST_CHECK_EQUAL(10, line_string_constructor_reserve->size());
    BOOST_CHECK_EQUAL(7, line_string_constructor_vector->size());
}

BOOST_FIXTURE_TEST_CASE(length, LineStringTest)
{
    BOOST_CHECK_EQUAL(0, tl::length(line_string_default_constructor_integer));
    BOOST_CHECK_EQUAL(0, tl::length(line_string_default_constructor_double));
    BOOST_CHECK_EQUAL(0, tl::length(line_string_default_constructor_float));
    BOOST_CHECK_CLOSE(285.409, tl::length(*line_string_integer), 0.01);
    BOOST_CHECK_CLOSE(1931.316, tl::length(*line_string_double), 0.01);
    BOOST_CHECK_EQUAL(0, tl::length(*line_string_float));
    BOOST_CHECK_CLOSE(285.409, tl::length(*line_string_integer_copy), 0.01);
    BOOST_CHECK_CLOSE(1931.316, tl::length(*line_string_double_copy), 0.01);
    BOOST_CHECK_EQUAL(0, tl::length(*line_string_float_copy));
    BOOST_CHECK_CLOSE(0, tl::length(*line_string_constructor_reserve), 0.01);
    BOOST_CHECK_CLOSE(1931.316, tl::length(*line_string_constructor_vector), 0.01);
}

BOOST_FIXTURE_TEST_CASE(type, LineStringTest)
{
    BOOST_CHECK(GeometryType::linestring == line_string_default_constructor_integer.type());
    BOOST_CHECK(GeometryType::linestring == line_string_integer->type());
    BOOST_CHECK(GeometryType::linestring == line_string_integer_copy->type());
    BOOST_CHECK(GeometryType::linestring == line_string_constructor_reserve->type());
    BOOST_CHECK(GeometryType::linestring == line_string_constructor_vector->type());
}

BOOST_FIXTURE_TEST_CASE(is3D, LineStringTest)
{
    BOOST_CHECK(Dimension::dim2 == line_string_default_constructor_integer.dimension());
    BOOST_CHECK(Dimension::dim2 == line_string_integer->dimension());
    BOOST_CHECK(Dimension::dim2 == line_string_integer_copy->dimension());
    BOOST_CHECK(Dimension::dim2 == line_string_constructor_reserve->dimension());
    BOOST_CHECK(Dimension::dim2 == line_string_constructor_vector->dimension());
}

BOOST_FIXTURE_TEST_CASE(assing_operator, LineStringTest)
{
    LineString2d lineString;
    lineString = *line_string_double;

    for (int i = 0; i < lineString.size(); i++) {
        BOOST_CHECK(lineString[i] == line_string_double->at(i));
    }
}

BOOST_FIXTURE_TEST_CASE(move_operator, LineStringTest)
{
    LineString<Point2d> line_to_move(v_d);
    LineString<Point2d> lineString;
    lineString = std::move(line_to_move);

    BOOST_CHECK_EQUAL(7, lineString.size());
    BOOST_CHECK_EQUAL(0, line_to_move.size());

    for (int i = 0; i < lineString.size(); i++) {
        BOOST_CHECK(lineString[i] == v_d[i]);
    }
}


BOOST_AUTO_TEST_SUITE_END()



/* LineString3D */

BOOST_AUTO_TEST_SUITE(LineString3DTestSuite)

std::initializer_list<Point3<int>> initializer_list_line_string_3d_integer 
{
    Point3<int>(34, 34, 3),
    Point3<int>(45, 54, 6),
    Point3<int>(51, 45, 43),
    Point3<int>(12, 47, 34),
    Point3<int>(95, 25, 24),
    Point3<int>(15, 36, 45),
    Point3<int>(18, 82, 36)
};


struct LineString3DTest
{

    LineString3DTest()
      : line_string_integer(nullptr),
        line_string_double(nullptr),
        line_string_float(nullptr),
        line_string_integer_copy(nullptr),
        line_string_double_copy(nullptr),
        line_string_float_copy(nullptr)
    {

    }

    ~LineString3DTest()
    {
        delete line_string_integer;
        delete line_string_double;
        delete line_string_float;
        delete line_string_integer_copy;
        delete line_string_double_copy;
        delete line_string_float_copy;
    }

    void setup()
    {
        v_d.push_back(Point3<double>(23.6, 94.4, 0.36));
        v_d.push_back(Point3<double>(75.36, 246.33, 454.3));
        v_d.push_back(Point3<double>(256.6, 619.3, 26.21));
        v_d.push_back(Point3<double>(62.36, 6.60, 62.61));

        line_string_integer = new LineString3i(initializer_list_line_string_3d_integer);
        line_string_double = new LineString3d(v_d);
        line_string_float = new LineString3f(10);

        line_string_integer_copy = new LineString3i(*line_string_integer);
        line_string_double_copy = new LineString3d(*line_string_double);
        line_string_float_copy = new LineString3f(*line_string_float);
    }

    void teardown()
    {

    }

    std::vector<Point3<double>> v_d;

    LineString3i line_string_default_constructor_integer;
    LineString3d line_string_default_constructor_double;
    LineString3f line_string_default_constructor_float;

    LineString3i *line_string_integer;
    LineString3d *line_string_double;
    LineString3f *line_string_float;
    LineString3i *line_string_integer_copy;
    LineString3d *line_string_double_copy;
    LineString3f *line_string_float_copy;
};


BOOST_FIXTURE_TEST_CASE(copy_constructor, LineString3DTest)
{
    for (size_t i = 0; i < line_string_double_copy->size(); i++) {
        BOOST_CHECK_EQUAL(v_d[i].x(), line_string_double_copy->at(i).x());
        BOOST_CHECK_EQUAL(v_d[i].y(), line_string_double_copy->at(i).y());
        BOOST_CHECK_EQUAL(v_d[i].z(), line_string_double_copy->at(i).z());
    }


    size_t j = 0;
    for (const auto &pt : initializer_list_line_string_3d_integer) {
        BOOST_CHECK_EQUAL(pt.x(), line_string_integer_copy->at(j).x());
        BOOST_CHECK_EQUAL(pt.y(), line_string_integer_copy->at(j).y());
        BOOST_CHECK_EQUAL(pt.z(), line_string_integer_copy->at(j).z());
        j++;
    }
}

BOOST_FIXTURE_TEST_CASE(move_constructor, LineString3DTest)
{
    LineString3d line_to_move(v_d);
    LineString3d lineString(std::move(line_to_move));

    BOOST_CHECK_EQUAL(4, lineString.size());
    BOOST_CHECK_EQUAL(0, line_to_move.size());

    for (int i = 0; i < lineString.size(); i++) {
        BOOST_CHECK(lineString[i] == v_d[i]);
    }

}

BOOST_FIXTURE_TEST_CASE(size, LineString3DTest)
{
    BOOST_CHECK_EQUAL(0, line_string_default_constructor_integer.size());
    BOOST_CHECK_EQUAL(0, line_string_default_constructor_double.size());
    BOOST_CHECK_EQUAL(0, line_string_default_constructor_float.size());
    BOOST_CHECK_EQUAL(7, line_string_integer->size());
    BOOST_CHECK_EQUAL(4, line_string_double->size());
    BOOST_CHECK_EQUAL(10, line_string_float->size());
    BOOST_CHECK_EQUAL(7, line_string_integer_copy->size());
    BOOST_CHECK_EQUAL(4, line_string_double_copy->size());
    BOOST_CHECK_EQUAL(10, line_string_float_copy->size());
}

BOOST_FIXTURE_TEST_CASE(length, LineString3DTest)
{
    BOOST_CHECK_EQUAL(0, tl::length(line_string_default_constructor_integer));
    BOOST_CHECK_EQUAL(0, tl::length(line_string_default_constructor_double));
    BOOST_CHECK_EQUAL(0, tl::length(line_string_default_constructor_float));
    BOOST_CHECK_CLOSE(318.498, tl::length(*line_string_integer), 0.01);
    BOOST_CHECK_CLOSE(1721.262, tl::length(*line_string_double), 0.01);
    BOOST_CHECK_EQUAL(0, tl::length(*line_string_float));
    BOOST_CHECK_CLOSE(318.499, tl::length(*line_string_integer_copy), 0.01);
    BOOST_CHECK_CLOSE(1721.262, tl::length(*line_string_double_copy), 0.01);
    BOOST_CHECK_EQUAL(0, tl::length(*line_string_float_copy));
}

BOOST_FIXTURE_TEST_CASE(type, LineString3DTest)
{
    BOOST_CHECK(GeometryType::linestring == line_string_default_constructor_integer.type());
    BOOST_CHECK(GeometryType::linestring == line_string_integer->type());
    BOOST_CHECK(GeometryType::linestring == line_string_integer_copy->type());
    BOOST_CHECK(GeometryType::linestring == line_string_float->type());
    BOOST_CHECK(GeometryType::linestring == line_string_double_copy->type());
}

BOOST_FIXTURE_TEST_CASE(is3D, LineString3DTest)
{
    BOOST_CHECK(Dimension::dim3 == line_string_default_constructor_integer.dimension());
    BOOST_CHECK(Dimension::dim3 == line_string_integer->dimension());
    BOOST_CHECK(Dimension::dim3 == line_string_integer_copy->dimension());
    BOOST_CHECK(Dimension::dim3 == line_string_float->dimension());
    BOOST_CHECK(Dimension::dim3 == line_string_double_copy->dimension());
}

BOOST_FIXTURE_TEST_CASE(copy_assing_operator, LineString3DTest)
{
    LineString3d lineString;
    lineString = *line_string_double;

    for (int i = 0; i < lineString.size(); i++) {
        BOOST_CHECK(lineString[i] == line_string_double->at(i));
    }
}

BOOST_FIXTURE_TEST_CASE(move_operator, LineString3DTest)
{
    LineString3d line_to_move(v_d);
    LineString3d lineString;
    lineString = std::move(line_to_move);

    BOOST_CHECK_EQUAL(4, lineString.size());
    BOOST_CHECK_EQUAL(0, line_to_move.size());

    for (int i = 0; i < lineString.size(); i++) {
        BOOST_CHECK(lineString[i] == v_d[i]);
    }
}

BOOST_AUTO_TEST_SUITE_END()




/* MultiLineString  */

BOOST_AUTO_TEST_SUITE(MultiLineStringTestSuite)

std::initializer_list<Point2i> points1
{
    Point2i(34, 34),
    Point2i(45, 54),
    Point2i(51, 45),
    Point2i(12, 47),
    Point2i(95, 25),
    Point2i(15, 36),
    Point2i(18, 82)
};

std::initializer_list<Point2i> points2
{
    Point2i(76, 70),
    Point2i(26, 35),
    Point2i(55, 25),
    Point2i(82, 21)
};



struct MultiLineStringTest
{

    MultiLineStringTest()
        : multiline_string_integer(nullptr)
    {

    }

    ~MultiLineStringTest()
    {
        delete multiline_string_integer;
    }

    void setup()
    {

        multiline_string_integer = new MultiLineString<Point2i>();
        multiline_string_integer->push_back(LineString2i(points1));
        multiline_string_integer->push_back(LineString2i(points2));
    }

    void teardown()
    {

    }

    MultiLineString<Point2d> multiline_default_constructor;

    MultiLineString<Point2i> *multiline_string_integer;
};

BOOST_FIXTURE_TEST_CASE(default_constructor, MultiLineStringTest)
{
    auto bbox = envelope(multiline_default_constructor);

    BOOST_CHECK_EQUAL(0, multiline_default_constructor.size());
    BOOST_CHECK(multiline_default_constructor.type() == GeometryType::multilinestring);
    BOOST_CHECK(multiline_default_constructor.dimension() == Dimension::dim2);
    BOOST_CHECK(Dimension::dim2 == multiline_default_constructor.dimension());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::max(), bbox.min().x());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::max(), bbox.min().y());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::lowest(), bbox.max().x());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::lowest(), bbox.max().y());
}

BOOST_FIXTURE_TEST_CASE(push_back, MultiLineStringTest)
{
    BOOST_CHECK_EQUAL(2, multiline_string_integer->size());
    BOOST_CHECK_EQUAL(7, multiline_string_integer->at(0).size());
    BOOST_CHECK_EQUAL(4, multiline_string_integer->at(1).size());
    BOOST_CHECK_EQUAL(34, multiline_string_integer->at(0).at(0).x());
    BOOST_CHECK_EQUAL(34, multiline_string_integer->at(0).at(0).y());
}

BOOST_AUTO_TEST_CASE(MultiLineString_constructor_reserve)
{
    MultiLineString<Point2d> multiline(10);
    //WindowD w = multiline.window();
    auto bbox = envelope(multiline);

    BOOST_CHECK_EQUAL(10, multiline.size());
    BOOST_CHECK(multiline.type() == GeometryType::multilinestring);
    BOOST_CHECK(multiline.dimension() == Dimension::dim2);
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::max(), bbox.min().x());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::max(), bbox.min().y());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::lowest(), bbox.max().x());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::lowest(), bbox.max().y());
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, MultiLineStringTest)
{
    MultiLineString<Point2i> multiline(*multiline_string_integer);
    BOOST_CHECK_EQUAL(2, multiline.size());
    BOOST_CHECK_EQUAL(7, multiline.at(0).size());
    BOOST_CHECK_EQUAL(4, multiline.at(1).size());
}


BOOST_AUTO_TEST_CASE(MultiLineString_copy_constructor)
{
    LineString2i  line1 {
        Point2i{4157222, 664789},
        Point2i{4149043, 688836},
        Point2i{4172803, 690340},
        Point2i{4177148, 642997},
        Point2i{4137012, 671808},
        Point2i{4146292, 666952},
        Point2i{4138759, 702670}
    };

    std::vector<Point2i> vect2{
        Point2i(4150653, 668925),
        Point2i(4148532, 688836),
        Point2i(4120568, 693696),
        Point2i(4153569, 643656),
        Point2i(4152639, 665658),
        Point2i(4156305, 636996),
        Point2i(4139568, 653366)};
    LineString2i  line2(vect2);

    MultiLineString<Point2i> multiline;
    multiline.push_back(line1);
    multiline.push_back(line2);

    MultiLineString<Point2i> multiline_copy(multiline);
    auto bbox = envelope(multiline_copy);

    BOOST_CHECK(multiline_copy.type() == GeometryType::multilinestring);
    BOOST_CHECK(Dimension::dim2 == multiline_copy.dimension());
    BOOST_CHECK_EQUAL(2, multiline_copy.size());
    BOOST_CHECK_EQUAL(4120568, bbox.min().x());
    BOOST_CHECK_EQUAL(636996, bbox.min().y());
    BOOST_CHECK_EQUAL(4177148, bbox.max().x());
    BOOST_CHECK_EQUAL(702670, bbox.max().y());

}

BOOST_AUTO_TEST_CASE(MultiLineString_assing_operator)
{
    LineString2i  line1{
        Point2i{4157222, 664789},
        Point2i{4149043, 688836},
        Point2i{4172803, 690340},
        Point2i{4177148, 642997},
        Point2i{4137012, 671808},
        Point2i{4146292, 666952},
        Point2i{4138759, 702670}
    };

    std::vector<Point2i> vect{
        Point2i(4150653, 668925),
        Point2i(4148532, 688836),
        Point2i(4120568, 693696),
        Point2i(4153569, 643656),
        Point2i(4152639, 665658),
        Point2i(4156305, 636996),
        Point2i(4139568, 653366)};
    LineString2i  line2(vect);

    MultiLineString<Point2i> multiline;
    multiline.push_back(line1);
    multiline.push_back(line2);

    MultiLineString<Point2i> multiline_copy = multiline;
    auto bbox = envelope(multiline_copy);

    BOOST_CHECK(multiline_copy.type() == GeometryType::multilinestring);
    BOOST_CHECK(Dimension::dim2 == multiline_copy.dimension());
    BOOST_CHECK_EQUAL(2, multiline_copy.size());
    BOOST_CHECK_EQUAL(4120568, bbox.min().x());
    BOOST_CHECK_EQUAL(636996, bbox.min().y());
    BOOST_CHECK_EQUAL(4177148, bbox.max().x());
    BOOST_CHECK_EQUAL(702670, bbox.max().y());

    LineString<Point2i> linestring = multiline_copy[1];
    for (size_t i = 0; i < linestring.size(); i++) {
        BOOST_CHECK_EQUAL(vect[i].x(), linestring[i].x());
        BOOST_CHECK_EQUAL(vect[i].y(), linestring[i].y());
    }
}

BOOST_AUTO_TEST_SUITE_END()

/* MultiLineString3D */

BOOST_AUTO_TEST_CASE(MultiLineString3D_default_constructor)
{
    MultiLineString3d multiline_3d;
    BoundingBox3d box = envelope(multiline_3d);

    BOOST_CHECK_EQUAL(0, multiline_3d.size());
    BOOST_CHECK(multiline_3d.type() == GeometryType::multilinestring);
    BOOST_CHECK(Dimension::dim3 == multiline_3d.dimension());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::max(), box.min().x());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::max(), box.min().y());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::max(), box.min().z());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::lowest(), box.max().x());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::lowest(), box.max().y());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::lowest(), box.max().z());
}

BOOST_AUTO_TEST_CASE(MultiLineString3D_constructor_reserve)
{
    MultiLineString3d multiline_3d(10);
    BoundingBox3d box = envelope(multiline_3d);

    BOOST_CHECK_EQUAL(10, multiline_3d.size());
    BOOST_CHECK(multiline_3d.type() == GeometryType::multilinestring);
    BOOST_CHECK(Dimension::dim3 == multiline_3d.dimension());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::max(), box.min().x());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::max(), box.min().y());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::max(), box.min().z());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::lowest(), box.max().x());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::lowest(), box.max().y());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::lowest(), box.max().z());
}

BOOST_AUTO_TEST_CASE(MultiLineString3D_copy_constructor)
{
    std::vector<Point3d> vector{
        Point3d(23.6, 94.4, 0.36),
        Point3d(75.36, 246.33, 454.3),
        Point3d(256.6, 619.3, 26.21),
        Point3d(62.36, 6.60, 62.61)
    };

    LineString3d linestring(vector);

    MultiLineString3d multiline;
    multiline.push_back(linestring);

    MultiLineString3d multiline_copy(multiline);

    BOOST_CHECK(multiline_copy.type() == GeometryType::multilinestring);
    BOOST_CHECK(multiline_copy.dimension() == Dimension::dim3);

    BOOST_CHECK_EQUAL(1, multiline_copy.size());
    auto box = envelope(multiline_copy);
    BOOST_CHECK_EQUAL(23.6, box.min().x());
    BOOST_CHECK_EQUAL(6.60, box.min().y());
    BOOST_CHECK_EQUAL(0.36, box.min().z());
    BOOST_CHECK_EQUAL(256.6, box.max().x());
    BOOST_CHECK_EQUAL(619.3, box.max().y());
    BOOST_CHECK_EQUAL(454.3, box.max().z());
}

BOOST_AUTO_TEST_CASE(MultiLineString3D_assing_operator)
{
    std::vector<Point3<double>> vector{
      Point3<double>(23.6, 94.4, 0.36),
      Point3<double>(75.36, 246.33, 454.3),
      Point3<double>(256.6, 619.3, 26.21),
      Point3<double>(62.36, 6.60, 62.61)
    };

    LineString3d linestring(vector);

    MultiLineString3d multiline;
    multiline.push_back(linestring);

    MultiLineString3d multiline_copy = multiline;

    BOOST_CHECK(multiline_copy.type() == GeometryType::multilinestring);
    BOOST_CHECK(multiline_copy.dimension() == Dimension::dim3);

    BOOST_CHECK_EQUAL(1, multiline_copy.size());
    auto box = envelope(multiline_copy);
    BOOST_CHECK_EQUAL(23.6, box.min().x());
    BOOST_CHECK_EQUAL(6.60, box.min().y());
    BOOST_CHECK_EQUAL(0.36, box.min().z());
    BOOST_CHECK_EQUAL(256.6, box.max().x());
    BOOST_CHECK_EQUAL(619.3, box.max().y());
    BOOST_CHECK_EQUAL(454.3, box.max().z());
}


BOOST_AUTO_TEST_CASE(test_linestring_wkt)
{
    LineString<Point2d> line;
    line.push_back(Point2d(0.0, 0.0));
    line.push_back(Point2d(1.123, 2.456));
    line.push_back(Point2d(5.0, 10.0));

    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << wkt(line);
    BOOST_CHECK_EQUAL(ss.str(), "LINESTRING (0.00 0.00, 1.12 2.46, 5.00 10.00)");

    std::string fmt_out = tl::format("{:.1f}", wkt(line));
    BOOST_CHECK_EQUAL(fmt_out, "LINESTRING (0.0 0.0, 1.1 2.5, 5.0 10.0)");

    LineString<Point3d> line_3d;
    line_3d.push_back(Point3d(0.0, 0.0, 0.0));
    line_3d.push_back(Point3d(1.123, 2.456, 2.14));
    line_3d.push_back(Point3d(5.0, 10.0, 4.65));

    ss.str("");
    ss << std::fixed << std::setprecision(2) << wkt(line_3d);
    BOOST_CHECK_EQUAL(ss.str(), "LINESTRING Z (0.00 0.00 0.00, 1.12 2.46 2.14, 5.00 10.00 4.65)");

    fmt_out = tl::format("{:.1f}", wkt(line_3d));
    BOOST_CHECK_EQUAL(fmt_out, "LINESTRING Z (0.0 0.0 0.0, 1.1 2.5 2.1, 5.0 10.0 4.7)");

    LineString<Point2dm> line_m;
    line_m.push_back(Point2dm(0.0, 0.0, 1.));
    line_m.push_back(Point2dm(1.123, 2.456, 3.));
    line_m.push_back(Point2dm(5.0, 10.0, 7.));

    ss.str("");
    ss << std::fixed << std::setprecision(2) << wkt(line_m);
    BOOST_CHECK_EQUAL(ss.str(), "LINESTRING M (0.00 0.00 1.00, 1.12 2.46 3.00, 5.00 10.00 7.00)");

    fmt_out = tl::format("{:.1f}", wkt(line_m));
    BOOST_CHECK_EQUAL(fmt_out, "LINESTRING M (0.0 0.0 1.0, 1.1 2.5 3.0, 5.0 10.0 7.0)");
}