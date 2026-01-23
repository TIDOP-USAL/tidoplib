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
 
#define BOOST_TEST_MODULE Tidop KD-Tree test

#include <boost/test/unit_test.hpp>
#include <tidop/geometry/primitives/Point.h>
#include <tidop/geometry/spatial/index/KdTree.h>
//#include <tidop/geometry/spatial/index/StaticKdTree.h>
//#include <tidop/geometry/spatial/index/DynamicKdTree.h>
#include <tidop/geometry/algorithms/measurement/Distance.h>

using namespace tl;

BOOST_AUTO_TEST_SUITE(KdTreeTestSuite)

// 1. Test de búsqueda por radio básica en 2D
BOOST_AUTO_TEST_CASE(RadiusSearch2D)
{
    // Creamos una nube de puntos en forma de cuadrícula
    std::vector<Point<double, Dimension::dim2>> points;
    for(int x = 0; x < 10; ++x) {
        for(int y = 0; y < 10; ++y) {
            points.emplace_back(static_cast<double>(x), static_cast<double>(y));
        }
    }

    KdTree<Point<double, Dimension::dim2>> tree(points);
    
    // Punto de búsqueda en el centro
    Point<double, Dimension::dim2> query(4.5, 4.5);
    double radius = 1.0; 
    
    std::vector<size_t> results;
    tree.radiusSearch(query, radius, results);

    // Con radio 1.0 desde (4.5, 4.5), debería encontrar:
    // (4,4), (4,5), (5,4), (5,5)
    BOOST_CHECK_EQUAL(results.size(), 4);
    
    // Verificamos que los índices recuperados correspondan a los puntos correctos
    for(size_t idx : results) {
        double d = distance(points[idx], query);
        BOOST_CHECK(d <= radius);
    }
}

// 2. Test con tipos Vector y dimensiones dinámicas (si aplica)
BOOST_AUTO_TEST_CASE(RadiusSearch3D_Vector)
{
    std::vector<Vector<double, 3>> points = {
        {0.0, 0.0, 0.0},
        {1.0, 1.0, 1.0},
        {2.0, 2.0, 2.0},
        {10.0, 10.0, 10.0}
    };

    KdTree<Vector<double, 3>> tree(points);
    
    std::vector<size_t> results;
    // Buscamos cerca del origen
    tree.radiusSearch({0.5, 0.5, 0.5}, 1.0, results);

    // Debería encontrar (0,0,0) y (1,1,1)
    BOOST_CHECK_EQUAL(results.size(), 2);
}

// 3. Test de conjunto vacío o radio cero
BOOST_AUTO_TEST_CASE(EdgeCases)
{
    std::vector<Point2d> points = {Point2d(1.0, 1.0)};
    KdTree<Point2d> tree(points);
    
    std::vector<size_t> results;
    
    // Radio 0 debe encontrar al menos el punto exacto si coincide
    tree.radiusSearch(Point2d(1.0, 1.0), 0.0, results);
    BOOST_CHECK_EQUAL(results.size(), 1);
    
    results.clear();
    // Radio pequeño que no alcanza nada
    tree.radiusSearch(Point2d(2.0, 2.0), 0.5, results);
    BOOST_CHECK(results.empty());
}

BOOST_AUTO_TEST_SUITE_END()




BOOST_AUTO_TEST_SUITE(KdTreeStaticTestSuite)

// 1. Test de búsqueda por radio básica en 2D
BOOST_AUTO_TEST_CASE(RadiusSearch2D)
{
    // Creamos una nube de puntos en forma de cuadrícula
    std::vector<Point<double, Dimension::dim2>> points;
    for (int x = 0; x < 10; ++x) {
        for (int y = 0; y < 10; ++y) {
            points.emplace_back(static_cast<double>(x), static_cast<double>(y));
        }
    }

    StaticKdTree<Point<double, Dimension::dim2>> tree(points);

    // Punto de búsqueda en el centro
    Point<double, Dimension::dim2> query(4.5, 4.5);
    double radius = 1.0;

    std::vector<size_t> results = tree.radiusSearch(query, radius);

    // Con radio 1.0 desde (4.5, 4.5), debería encontrar:
    // (4,4), (4,5), (5,4), (5,5)
    BOOST_CHECK_EQUAL(results.size(), 4);

    // Verificamos que los índices recuperados correspondan a los puntos correctos
    for (size_t idx : results) {
        double d = distance(points[idx], query);
        BOOST_CHECK(d <= radius);
    }
}

// 2. Test con tipos Vector y dimensiones dinámicas (si aplica)
BOOST_AUTO_TEST_CASE(RadiusSearch3D_Vector)
{
    std::vector<Vector<double, 3>> points = {
        {0.0, 0.0, 0.0},
        {1.0, 1.0, 1.0},
        {2.0, 2.0, 2.0},
        {10.0, 10.0, 10.0}
    };

    StaticKdTree<Vector<double, 3>> tree(points);

    
    // Buscamos cerca del origen
    std::vector<size_t> results = tree.radiusSearch({0.5, 0.5, 0.5}, 1.0);

    // Debería encontrar (0,0,0) y (1,1,1)
    BOOST_CHECK_EQUAL(results.size(), 2);
}

// 3. Test de conjunto vacío o radio cero
BOOST_AUTO_TEST_CASE(EdgeCases)
{
    std::vector<Point2d> points = {Point2d(1.0, 1.0)};
    StaticKdTree<Point2d> tree(points);

    // Radio 0 debe encontrar al menos el punto exacto si coincide
    std::vector<size_t> results = tree.radiusSearch(Point2d(1.0, 1.0), 0.0);
    BOOST_CHECK_EQUAL(results.size(), 1);

    results.clear();
    // Radio pequeño que no alcanza nada
    results = tree.radiusSearch(Point2d(2.0, 2.0), 0.5);
    BOOST_CHECK(results.empty());
}

BOOST_AUTO_TEST_SUITE_END()



BOOST_AUTO_TEST_SUITE(KdTreeDynamicTestSuite)

// 1. Test de búsqueda por radio básica en 2D
BOOST_AUTO_TEST_CASE(RadiusSearch2D)
{
    // Creamos una nube de puntos en forma de cuadrícula
    std::vector<Point<double, Dimension::dim2>> points;
    for (int x = 0; x < 10; ++x) {
        for (int y = 0; y < 10; ++y) {
            points.emplace_back(static_cast<double>(x), static_cast<double>(y));
        }
    }

    DynamicKdTree<Point<double, Dimension::dim2>> tree(points);

    // Punto de búsqueda en el centro
    Point<double, Dimension::dim2> query(4.5, 4.5);
    double radius = 1.0;

    std::vector<size_t> results = tree.radiusSearch(query, radius);

    // Con radio 1.0 desde (4.5, 4.5), debería encontrar:
    // (4,4), (4,5), (5,4), (5,5)
    BOOST_CHECK_EQUAL(results.size(), 4);

    // Verificamos que los índices recuperados correspondan a los puntos correctos
    for (size_t idx : results) {
        double d = distance(points[idx], query);
        BOOST_CHECK(d <= radius);
    }
}

// 2. Test con tipos Vector y dimensiones dinámicas (si aplica)
BOOST_AUTO_TEST_CASE(RadiusSearch3D_Vector)
{
    std::vector<Vector<double, 3>> points = {
        {0.0, 0.0, 0.0},
        {1.0, 1.0, 1.0},
        {2.0, 2.0, 2.0},
        {10.0, 10.0, 10.0}
    };

    DynamicKdTree<Vector<double, 3>> tree(points);


    // Buscamos cerca del origen
    std::vector<size_t> results = tree.radiusSearch({0.5, 0.5, 0.5}, 1.0);

    // Debería encontrar (0,0,0) y (1,1,1)
    BOOST_CHECK_EQUAL(results.size(), 2);
}

// 3. Test de conjunto vacío o radio cero
BOOST_AUTO_TEST_CASE(EdgeCases)
{
    std::vector<Point2d> points = {Point2d(1.0, 1.0)};
    DynamicKdTree<Point2d> tree(points);

    // Radio 0 debe encontrar al menos el punto exacto si coincide
    std::vector<size_t> results = tree.radiusSearch(Point2d(1.0, 1.0), 0.0);
    BOOST_CHECK_EQUAL(results.size(), 1);

    results.clear();
    // Radio pequeño que no alcanza nada
    results = tree.radiusSearch(Point2d(2.0, 2.0), 0.5);
    BOOST_CHECK(results.empty());
}

BOOST_AUTO_TEST_SUITE_END()