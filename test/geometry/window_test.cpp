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
 
#define BOOST_TEST_MODULE Tidop Window test
#include <boost/test/unit_test.hpp>
#include <tidop/geometry/window.h>

using namespace tl;


BOOST_AUTO_TEST_SUITE(WindowTestSuite)

struct WindowTest
{

    WindowTest()
        : window_integer(nullptr),
        window_double(nullptr),
        window_float(nullptr),
        window_integer_copy(nullptr),
        window_double_copy(nullptr),
        window_float_copy(nullptr),
        window_constructor_center_side_integer_even_size(nullptr),
        window_constructor_center_side_integer_odd_size(nullptr),
        window_constructor_center_side_double(nullptr),
        window_constructor_center_side_float(nullptr),
        window_constructor_center_size_integer_even_size(nullptr),
        window_constructor_center_size_integer_odd_size(nullptr),
        window_constructor_center_size_double(nullptr),
        window_constructor_center_size_float(nullptr)
    {

    }

    ~WindowTest()
    {
        delete window_integer;
        delete window_double;
        delete window_float;
        delete window_integer_copy;
        delete window_double_copy;
        delete window_float_copy;
        delete window_constructor_center_side_integer_even_size;
        delete window_constructor_center_side_integer_odd_size;
        delete window_constructor_center_side_double;
        delete window_constructor_center_side_float;
        delete window_constructor_center_size_integer_even_size;
        delete window_constructor_center_size_integer_odd_size;
        delete window_constructor_center_size_double;
        delete window_constructor_center_size_float;
    }

    void setup()
    {
        window_integer = new WindowI(Point<int>(0, 0), Point<int>(100, 100));
        window_double = new WindowD(Point<double>(0., 0.), Point<double>(100., 100.));
        window_float = new WindowF(Point<float>(0.f, 0.f), Point<float>(100.f, 100.f));

        window_integer_copy = new WindowI(*window_integer);
        window_double_copy = new WindowD(*window_double);
        window_float_copy = new WindowF(*window_float);

        ptc_integer = Point<int>(50, 50);
        ptc_double = Point<double>(50.67, 50.76);
        ptc_float = Point<float>(50.67f, 34.45f);

        window_constructor_center_side_integer_even_size = new WindowI(ptc_integer, 50);
        window_constructor_center_side_integer_odd_size = new WindowI(ptc_integer, 51);
        window_constructor_center_side_double = new WindowD(ptc_double, 50.);
        window_constructor_center_side_float = new WindowF(ptc_float, 50.f);

        window_constructor_center_size_integer_even_size = new WindowI(ptc_integer, 50, 60);
        window_constructor_center_size_integer_odd_size = new WindowI(ptc_integer, 51, 61);
        window_constructor_center_size_double = new WindowD(ptc_double, 100.32, 254.25);
        window_constructor_center_size_float = new WindowF(ptc_float, 100.34f, 254.23f);
    }

    void teardown()
    {

    }

    WindowI window_default_constructor_integer;
    WindowD window_default_constructor_double;
    WindowF window_default_constructor_float;

    WindowI *window_integer;
    WindowD *window_double;
    WindowF *window_float;
    WindowI *window_integer_copy;
    WindowD *window_double_copy;
    WindowF *window_float_copy;

    Point<int> ptc_integer;
    Point<double> ptc_double;
    Point<float> ptc_float;

    WindowI *window_constructor_center_side_integer_even_size;
    WindowI *window_constructor_center_side_integer_odd_size;
    WindowD *window_constructor_center_side_double;
    WindowF *window_constructor_center_side_float;

    WindowI *window_constructor_center_size_integer_even_size;
    WindowI *window_constructor_center_size_integer_odd_size;
    WindowD *window_constructor_center_size_double;
    WindowF *window_constructor_center_size_float;
};


BOOST_FIXTURE_TEST_CASE(default_constructor, WindowTest)
{
    BOOST_CHECK_EQUAL(TL_INT_MAX, window_default_constructor_integer.pt1.x);
    BOOST_CHECK_EQUAL(TL_INT_MAX, window_default_constructor_integer.pt1.y);
    BOOST_CHECK_EQUAL(TL_INT_MIN, window_default_constructor_integer.pt2.x);
    BOOST_CHECK_EQUAL(TL_INT_MIN, window_default_constructor_integer.pt2.y);

    BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, window_default_constructor_double.pt1.x);
    BOOST_CHECK_EQUAL(TL_DOUBLE_MAX, window_default_constructor_double.pt1.y);
    BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, window_default_constructor_double.pt2.x);
    BOOST_CHECK_EQUAL(TL_DOUBLE_MIN, window_default_constructor_double.pt2.y);

    BOOST_CHECK_EQUAL(TL_FLOAT_MAX, window_default_constructor_float.pt1.x);
    BOOST_CHECK_EQUAL(TL_FLOAT_MAX, window_default_constructor_float.pt1.y);
    BOOST_CHECK_EQUAL(TL_FLOAT_MIN, window_default_constructor_float.pt2.x);
    BOOST_CHECK_EQUAL(TL_FLOAT_MIN, window_default_constructor_float.pt2.y);
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, WindowTest)
{
    BOOST_CHECK_EQUAL(window_integer->pt1.x, window_integer_copy->pt1.x);
    BOOST_CHECK_EQUAL(window_integer->pt1.y, window_integer_copy->pt1.y);
    BOOST_CHECK_EQUAL(window_integer->pt2.x, window_integer_copy->pt2.x);
    BOOST_CHECK_EQUAL(window_integer->pt2.y, window_integer_copy->pt2.y);

    BOOST_CHECK_EQUAL(window_double->pt1.x, window_double_copy->pt1.x);
    BOOST_CHECK_EQUAL(window_double->pt1.y, window_double_copy->pt1.y);
    BOOST_CHECK_EQUAL(window_double->pt2.x, window_double_copy->pt2.x);
    BOOST_CHECK_EQUAL(window_double->pt2.y, window_double_copy->pt2.y);

    BOOST_CHECK_EQUAL(window_float->pt1.x, window_float_copy->pt1.x);
    BOOST_CHECK_EQUAL(window_float->pt1.y, window_float_copy->pt1.y);
    BOOST_CHECK_EQUAL(window_float->pt2.x, window_float_copy->pt2.x);
    BOOST_CHECK_EQUAL(window_float->pt2.y, window_float_copy->pt2.y);
}

BOOST_FIXTURE_TEST_CASE(copy_constructor_dif_types, WindowTest)
{
    const WindowD w(Point<double>(0.5, 0.4), Point<double>(100.6, 100.4));
    WindowI w2(w);
    BOOST_CHECK_EQUAL(1, w2.pt1.x);
    BOOST_CHECK_EQUAL(0, w2.pt1.y);
    BOOST_CHECK_EQUAL(101, w2.pt2.x);
    BOOST_CHECK_EQUAL(100, w2.pt2.y);

    const WindowI w_int(Point<int>(1, 4), Point<int>(100, 100));
    WindowD w_double(w_int);
    BOOST_CHECK_EQUAL(1, w_double.pt1.x);
    BOOST_CHECK_EQUAL(4, w_double.pt1.y);
    BOOST_CHECK_EQUAL(100, w_double.pt2.x);
    BOOST_CHECK_EQUAL(100, w_double.pt2.y);
}

BOOST_FIXTURE_TEST_CASE(constructor_center_side, WindowTest)
{
    BOOST_CHECK_EQUAL(25, window_constructor_center_side_integer_even_size->pt1.x);
    BOOST_CHECK_EQUAL(25, window_constructor_center_side_integer_even_size->pt1.y);
    BOOST_CHECK_EQUAL(75, window_constructor_center_side_integer_even_size->pt2.x);
    BOOST_CHECK_EQUAL(75, window_constructor_center_side_integer_even_size->pt2.y);

    BOOST_CHECK_EQUAL(25, window_constructor_center_side_integer_odd_size->pt1.x);
    BOOST_CHECK_EQUAL(25, window_constructor_center_side_integer_odd_size->pt1.y);
    BOOST_CHECK_EQUAL(76, window_constructor_center_side_integer_odd_size->pt2.x);
    BOOST_CHECK_EQUAL(76, window_constructor_center_side_integer_odd_size->pt2.y);

    BOOST_CHECK_CLOSE(25.67, window_constructor_center_side_double->pt1.x, 0.01);
    BOOST_CHECK_CLOSE(25.76, window_constructor_center_side_double->pt1.y, 0.01);
    BOOST_CHECK_CLOSE(75.67, window_constructor_center_side_double->pt2.x, 0.01);
    BOOST_CHECK_CLOSE(75.76, window_constructor_center_side_double->pt2.y, 0.01);

    BOOST_CHECK_CLOSE(25.67f, window_constructor_center_side_float->pt1.x, 0.01);
    BOOST_CHECK_CLOSE(9.45f, window_constructor_center_side_float->pt1.y, 0.01);
    BOOST_CHECK_CLOSE(75.67f, window_constructor_center_side_float->pt2.x, 0.01);
    BOOST_CHECK_CLOSE(59.45f, window_constructor_center_side_float->pt2.y, 0.01);
}

BOOST_FIXTURE_TEST_CASE(constructor_center_size, WindowTest)
{
    BOOST_CHECK_EQUAL(25, window_constructor_center_size_integer_even_size->pt1.x);
    BOOST_CHECK_EQUAL(20, window_constructor_center_size_integer_even_size->pt1.y);
    BOOST_CHECK_EQUAL(75, window_constructor_center_size_integer_even_size->pt2.x);
    BOOST_CHECK_EQUAL(80, window_constructor_center_size_integer_even_size->pt2.y);

    BOOST_CHECK_EQUAL(25, window_constructor_center_size_integer_odd_size->pt1.x);
    BOOST_CHECK_EQUAL(20, window_constructor_center_size_integer_odd_size->pt1.y);
    BOOST_CHECK_EQUAL(76, window_constructor_center_size_integer_odd_size->pt2.x);
    BOOST_CHECK_EQUAL(81, window_constructor_center_size_integer_odd_size->pt2.y);

    BOOST_CHECK_CLOSE(0.51, window_constructor_center_size_double->pt1.x, 0.01);
    BOOST_CHECK_CLOSE(-76.365, window_constructor_center_size_double->pt1.y, 0.01);
    BOOST_CHECK_CLOSE(100.83, window_constructor_center_size_double->pt2.x, 0.01);
    BOOST_CHECK_CLOSE(177.885, window_constructor_center_size_double->pt2.y, 0.01);

    BOOST_CHECK_CLOSE(0.5f, window_constructor_center_size_float->pt1.x, 0.01);
    BOOST_CHECK_CLOSE(-92.665f, window_constructor_center_size_float->pt1.y, 0.01);
    BOOST_CHECK_CLOSE(100.84f, window_constructor_center_size_float->pt2.x, 0.01);
    BOOST_CHECK_CLOSE(161.565f, window_constructor_center_size_float->pt2.y, 0.01);
}

BOOST_FIXTURE_TEST_CASE(center, WindowTest)
{
    BOOST_CHECK(Point<int>(0, 0) == window_default_constructor_integer.center());
    BOOST_CHECK(Point<double>(0., 0.) == window_default_constructor_double.center());
    BOOST_CHECK(Point<float>(0.f, 0.f) == window_default_constructor_float.center());

    BOOST_CHECK(Point<int>(50, 50) == window_integer->center());
    BOOST_CHECK(Point<double>(50., 50.) == window_double->center());
    BOOST_CHECK(Point<float>(50.f, 50.f) == window_float->center());

    BOOST_CHECK(Point<int>(50, 50) == window_integer_copy->center());
    BOOST_CHECK(Point<double>(50., 50.) == window_double_copy->center());
    BOOST_CHECK(Point<float>(50.f, 50.f) == window_float_copy->center());

    BOOST_CHECK(ptc_integer == window_constructor_center_side_integer_even_size->center());
    BOOST_CHECK(ptc_integer == window_constructor_center_side_integer_odd_size->center());
    BOOST_CHECK_CLOSE(ptc_double.x, window_constructor_center_side_double->center().x, 0.01);
    BOOST_CHECK_CLOSE(ptc_double.y, window_constructor_center_side_double->center().y, 0.01);
    BOOST_CHECK_CLOSE(ptc_float.x, window_constructor_center_side_float->center().x, 0.01);
    BOOST_CHECK_CLOSE(ptc_float.y, window_constructor_center_side_float->center().y, 0.01);
    BOOST_CHECK(ptc_integer == window_constructor_center_size_integer_even_size->center());
    BOOST_CHECK(ptc_integer == window_constructor_center_size_integer_odd_size->center());
    BOOST_CHECK_CLOSE(ptc_double.x, window_constructor_center_size_double->center().x, 0.01);
    BOOST_CHECK_CLOSE(ptc_double.y, window_constructor_center_size_double->center().y, 0.01);
    BOOST_CHECK_CLOSE(ptc_float.x, window_constructor_center_size_float->center().x, 0.01);
    BOOST_CHECK_CLOSE(ptc_float.y, window_constructor_center_size_float->center().y, 0.01);
}

BOOST_FIXTURE_TEST_CASE(width, WindowTest)
{
    BOOST_CHECK_EQUAL(0, window_default_constructor_integer.width());
    BOOST_CHECK_EQUAL(0., window_default_constructor_double.width());
    BOOST_CHECK_EQUAL(0.f, window_default_constructor_float.width());

    BOOST_CHECK_EQUAL(100, window_integer->width());
    BOOST_CHECK_EQUAL(100., window_double->width());
    BOOST_CHECK_EQUAL(100.f, window_float->width());

    BOOST_CHECK_EQUAL(100, window_integer_copy->width());
    BOOST_CHECK_EQUAL(100., window_double_copy->width());
    BOOST_CHECK_EQUAL(100.f, window_float_copy->width());

    BOOST_CHECK_EQUAL(50, window_constructor_center_side_integer_even_size->width());
    BOOST_CHECK_EQUAL(51, window_constructor_center_side_integer_odd_size->width());
    BOOST_CHECK_EQUAL(50., window_constructor_center_side_double->width());
    BOOST_CHECK_EQUAL(50.f, window_constructor_center_side_float->width());
    BOOST_CHECK_EQUAL(50, window_constructor_center_size_integer_even_size->width());
    BOOST_CHECK_EQUAL(51, window_constructor_center_size_integer_odd_size->width());
    BOOST_CHECK_EQUAL(100.32, window_constructor_center_size_double->width());
    BOOST_CHECK_EQUAL(100.34f, window_constructor_center_size_float->width());
}

BOOST_FIXTURE_TEST_CASE(height, WindowTest)
{
    BOOST_CHECK_EQUAL(0, window_default_constructor_integer.height());
    BOOST_CHECK_EQUAL(0., window_default_constructor_double.height());
    BOOST_CHECK_EQUAL(0.f, window_default_constructor_float.height());

    BOOST_CHECK_EQUAL(100, window_integer->height());
    BOOST_CHECK_EQUAL(100., window_double->height());
    BOOST_CHECK_EQUAL(100.f, window_float->height());

    BOOST_CHECK_EQUAL(100, window_integer_copy->height());
    BOOST_CHECK_EQUAL(100., window_double_copy->height());
    BOOST_CHECK_EQUAL(100.f, window_float_copy->height());

    BOOST_CHECK_EQUAL(50, window_constructor_center_side_integer_even_size->height());
    BOOST_CHECK_EQUAL(51, window_constructor_center_side_integer_odd_size->height());
    BOOST_CHECK_CLOSE(50., window_constructor_center_side_double->height(), 0.01);
    BOOST_CHECK_CLOSE(50.f, window_constructor_center_side_float->height(), 0.01);
    BOOST_CHECK_EQUAL(60, window_constructor_center_size_integer_even_size->height());
    BOOST_CHECK_EQUAL(61, window_constructor_center_size_integer_odd_size->height());
    BOOST_CHECK_CLOSE(254.25, window_constructor_center_size_double->height(), 0.01);
    BOOST_CHECK_CLOSE(254.23f, window_constructor_center_size_float->height(), 0.01);
}

BOOST_FIXTURE_TEST_CASE(isEmpty, WindowTest)
{
    BOOST_CHECK_EQUAL(true, window_default_constructor_integer.isEmpty());
    BOOST_CHECK_EQUAL(false, window_integer->isEmpty());
    BOOST_CHECK_EQUAL(false, window_integer_copy->isEmpty());
    BOOST_CHECK_EQUAL(false, window_constructor_center_side_integer_even_size->isEmpty());
    BOOST_CHECK_EQUAL(false, window_constructor_center_size_integer_even_size->isEmpty());
}

BOOST_FIXTURE_TEST_CASE(isValid, WindowTest)
{
    BOOST_CHECK_EQUAL(false, window_default_constructor_integer.isValid());
    BOOST_CHECK_EQUAL(true, window_integer->isValid());
    BOOST_CHECK_EQUAL(true, window_integer_copy->isValid());
    BOOST_CHECK_EQUAL(true, window_constructor_center_side_integer_even_size->isValid());
    BOOST_CHECK_EQUAL(true, window_constructor_center_size_integer_even_size->isValid());
}

BOOST_FIXTURE_TEST_CASE(normalized, WindowTest)
{
    WindowI w(Point<int>(100, 100), Point<int>(0, 0));

    BOOST_CHECK_EQUAL(false, w.isValid());
    w.normalized();
    BOOST_CHECK_EQUAL(true, w.isValid());
    BOOST_CHECK_EQUAL(0, w.pt1.x);
    BOOST_CHECK_EQUAL(0, w.pt1.y);
    BOOST_CHECK_EQUAL(100, w.pt2.x);
    BOOST_CHECK_EQUAL(100, w.pt2.y);
}

BOOST_FIXTURE_TEST_CASE(containsPoint, WindowTest)
{
    Point<int> point1(0, 0);
    Point<int> point2(50, 50);
    Point<int> point3(-50, -50);
    BOOST_CHECK_EQUAL(false, window_default_constructor_integer.containsPoint(point1));
    BOOST_CHECK_EQUAL(true, window_integer->containsPoint(point1));
    BOOST_CHECK_EQUAL(true, window_integer->containsPoint(point2));
    BOOST_CHECK_EQUAL(false, window_integer->containsPoint(point3));

}

BOOST_FIXTURE_TEST_CASE(containsWindow, WindowTest)
{
    WindowI w1(Point<int>(50, 50), 50);
    WindowI w2(Point<int>(200, 50), 50);
    BOOST_CHECK_EQUAL(false, window_default_constructor_integer.containsWindow(w1));
    BOOST_CHECK_EQUAL(true, window_integer->containsWindow(w1));
    BOOST_CHECK_EQUAL(false, window_integer->containsWindow(w2));
}

BOOST_AUTO_TEST_SUITE_END()



BOOST_AUTO_TEST_CASE(WindowI_constructor_vector)
{
    std::vector<Point<double>> v{Point<double>(0.5, 1.5), Point<double>(3.6, 4.4)};
    WindowI w(v);
    BOOST_CHECK_EQUAL(1, w.pt1.x);
    BOOST_CHECK_EQUAL(2, w.pt1.y);
    BOOST_CHECK_EQUAL(4, w.pt2.x);
    BOOST_CHECK_EQUAL(4, w.pt2.y);

    std::vector<Point<double>> v2{Point<double>(-0.5, -1.5), Point<double>(-3.6, -4.4)};
    WindowI w2(v2);
    BOOST_CHECK_EQUAL(-4, w2.pt1.x);
    BOOST_CHECK_EQUAL(-4, w2.pt1.y);
    BOOST_CHECK_EQUAL(-1, w2.pt2.x);
    BOOST_CHECK_EQUAL(-2, w2.pt2.y);

    std::vector<Point<int>> v3{Point<int>(2, 3), Point<int>(10, 15)};
    WindowD w3(v3);
    BOOST_CHECK_EQUAL(2, w3.pt1.x);
    BOOST_CHECK_EQUAL(3, w3.pt1.y);
    BOOST_CHECK_EQUAL(10, w3.pt2.x);
    BOOST_CHECK_EQUAL(15, w3.pt2.y);
}

BOOST_AUTO_TEST_CASE(Window_assing_operator)
{
    const WindowD w(Point<double>(0., 0.), Point<double>(100., 100.));
    WindowD w2 = w;
    BOOST_CHECK(w.pt1 == w2.pt1);
    BOOST_CHECK(w.pt2 == w2.pt2);
}

/* ---------------------------------------------------------------------------------- */
/*                        Test Operaciones con ventanas                                */
/* ---------------------------------------------------------------------------------- */

// Comprueba si dos ventanas intersectan
BOOST_AUTO_TEST_CASE(intersectWindows_window_int)
{
    BOOST_CHECK(intersectWindows(WindowI(Point<int>(0, 0), Point<int>(100, 100)), WindowI(Point<int>(50, 50), Point<int>(150, 150))));
    BOOST_CHECK(intersectWindows(WindowI(Point<int>(0, 0), Point<int>(100, 100)), WindowI(Point<int>(-50, -50), Point<int>(50, 50))));
    BOOST_CHECK(intersectWindows(WindowI(Point<int>(0, 0), Point<int>(100, 100)), WindowI(Point<int>(-50, 0), Point<int>(50, 100))));
    BOOST_CHECK(intersectWindows(WindowI(Point<int>(0, 0), Point<int>(100, 100)), WindowI(Point<int>(0, 50), Point<int>(100, 150))));
    BOOST_CHECK(intersectWindows(WindowI(Point<int>(0, 0), Point<int>(100, 100)), WindowI(Point<int>(-50, -50), Point<int>(0, 0))));
    BOOST_CHECK(intersectWindows(WindowI(Point<int>(0, 0), Point<int>(100, 100)), WindowI(Point<int>(100, 100), Point<int>(150, 150))));
    BOOST_CHECK(false == intersectWindows(WindowI(Point<int>(0, 0), Point<int>(100, 100)), WindowI(Point<int>(150, 150), Point<int>(200, 200))));
    BOOST_CHECK(false == intersectWindows(WindowI(Point<int>(0, 0), Point<int>(100, 100)), WindowI(Point<int>(-100, -100), Point<int>(-1, -1))));
    BOOST_CHECK(false == intersectWindows(WindowI(Point<int>(0, 0), Point<int>(100, 100)), WindowI(Point<int>(101, 101), Point<int>(200, 200))));
    BOOST_CHECK(false == intersectWindows(WindowI(Point<int>(0, 0), Point<int>(100, 100)), WindowI(Point<int>(0, 101), Point<int>(100, 200))));
}

BOOST_AUTO_TEST_CASE(intersectWindows_different_window)
{
    BOOST_CHECK(intersectWindows(WindowI(Point<int>(0, 0), Point<int>(100, 100)), WindowD(Point<double>(50.5, 50.5), Point<double>(150.5, 150.5))));
    BOOST_CHECK(intersectWindows(WindowD(Point<double>(-50.5, -50.5), Point<double>(50.5, 50.5)), WindowI(Point<int>(0, 0), Point<int>(100, 100))));
    BOOST_CHECK(intersectWindows(WindowI(Point<int>(0, 0), Point<int>(100, 100)), WindowD(Point<double>(-50.5, 0.5), Point<double>(50.5, 100.5))));
    BOOST_CHECK(intersectWindows(WindowF(Point<float>(0, 50), Point<float>(100, 150)), WindowI(Point<int>(0, 0), Point<int>(100, 100))));
    BOOST_CHECK(intersectWindows(WindowI(Point<int>(0, 0), Point<int>(100, 100)), WindowF(Point<float>(-50, -50), Point<float>(0, 0))));
    BOOST_CHECK(intersectWindows(WindowI(Point<int>(0, 0), Point<int>(100, 100)), WindowF(Point<float>(100, 100), Point<float>(150, 150))));
    BOOST_CHECK(false == intersectWindows(WindowI(Point<int>(0, 0), Point<int>(100, 100)), WindowD(Point<double>(150.5, 150.5), Point<double>(200.5, 200.5))));
    BOOST_CHECK(false == intersectWindows(WindowI(Point<int>(0, 0), Point<int>(100, 100)), WindowD(Point<double>(-100, -100), Point<double>(-0.5, -0.5))));
    BOOST_CHECK(false == intersectWindows(WindowI(Point<int>(0, 0), Point<int>(100, 100)), WindowF(Point<float>(101, 101), Point<float>(200, 200))));
    BOOST_CHECK(false == intersectWindows(WindowI(Point<int>(0, 0), Point<int>(100, 100)), WindowF(Point<float>(0, 101), Point<float>(100, 200))));
}

BOOST_AUTO_TEST_CASE(windowIntersection_window_int)
{
    WindowI w = windowIntersection(WindowI(Point<int>(0, 0), Point<int>(100, 100)), WindowI(Point<int>(50, 50), Point<int>(150, 150)));
    BOOST_CHECK_EQUAL(50, w.pt1.x);
    BOOST_CHECK_EQUAL(50, w.pt1.y);
    BOOST_CHECK_EQUAL(100, w.pt2.x);
    BOOST_CHECK_EQUAL(100, w.pt2.y);

    w = windowIntersection(WindowI(Point<int>(0, 0), Point<int>(100, 100)), WindowI(Point<int>(-50, -50), Point<int>(50, 50)));
    BOOST_CHECK_EQUAL(0, w.pt1.x);
    BOOST_CHECK_EQUAL(0, w.pt1.y);
    BOOST_CHECK_EQUAL(50, w.pt2.x);
    BOOST_CHECK_EQUAL(50, w.pt2.y);

    w = windowIntersection(WindowI(Point<int>(0, 0), Point<int>(100, 100)), WindowI(Point<int>(150, 150), Point<int>(200, 200)));
    BOOST_CHECK_EQUAL(TL_INT_MAX, w.pt1.x);
    BOOST_CHECK_EQUAL(TL_INT_MAX, w.pt1.y);
    BOOST_CHECK_EQUAL(TL_INT_MIN, w.pt2.x);
    BOOST_CHECK_EQUAL(TL_INT_MIN, w.pt2.y);
}

BOOST_AUTO_TEST_CASE(join_window)
{
    WindowI w = joinWindow(WindowI(Point<int>(0, 0), Point<int>(100, 100)), WindowI(Point<int>(50, 50), Point<int>(150, 150)));
    BOOST_CHECK_EQUAL(0, w.pt1.x);
    BOOST_CHECK_EQUAL(0, w.pt1.y);
    BOOST_CHECK_EQUAL(150, w.pt2.x);
    BOOST_CHECK_EQUAL(150, w.pt2.y);
}