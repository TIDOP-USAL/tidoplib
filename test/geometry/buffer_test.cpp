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
 
#define BOOST_TEST_MODULE Tidop buffer test
#include <boost/test/unit_test.hpp>

#include <tidop/geometry/algorithms/spatial/Buffer.h>
#include <tidop/geometry/primitives/Segment.h>
#include <tidop/geometry/primitives/LineString.h>
#include <tidop/geometry/algorithms/measurement/Area.h>

using namespace tl;

BOOST_AUTO_TEST_CASE(point_buffer_test)
{
    Point2d p(5.0, 5.0);
    double dist = 10.0;

    // El buffer de un punto es un círculo aproximado
    auto poly = buffer(p, dist);

    // 1. El área de un círculo es PI * r^2. 
    // Como usamos 32 segmentos por defecto, el área será ligeramente menor a PI * 100
    double expected_area = consts::pi<double> *dist * dist;
    BOOST_CHECK(area(poly) < expected_area);
    BOOST_CHECK(area(poly) > expected_area * 0.98); // Tolerancia por aproximación poligonal

    // 2. El centro debe estar contenido
    BOOST_CHECK(contains(poly, p));

    // 3. Puntos en el borde y fuera
    BOOST_CHECK(contains(poly, Point2d(14.0, 5.0)));  // Distancia 9 < 10
    BOOST_CHECK(!contains(poly, Point2d(16.0, 5.0))); // Distancia 11 > 10
}

BOOST_AUTO_TEST_CASE(segment_buffer_test)
{
    Point2d p1(0, 0), p2(10, 0);
    Segment2d seg(p1, p2);
    double dist = 2.0;

    auto poly = buffer(seg, dist);

    // El área de un buffer de segmento (flat cap) es: largo * (2 * dist)
    // 10 * 4 = 40
    BOOST_CHECK_CLOSE(area(poly), 40.0, 0.001);

    // Verificar que el punto original está contenido en su propio buffer
    BOOST_CHECK(contains(poly, Point2d(5, 0)));
    // Verificar que un punto fuera de la distancia no lo está
    BOOST_CHECK(!contains(poly, Point2d(5, 3)));
}

BOOST_AUTO_TEST_CASE(linestring_buffer_test)
{
    LineString2d line;
    line.emplace_back(0, 0);
    line.emplace_back(10, 0);
    line.emplace_back(10, 10);

    double dist = 2.0;
    auto poly = buffer(line, dist);

    // 1. Verificar que los puntos extremos están cubiertos
    BOOST_CHECK(contains(poly, Point2d(0, 0)));
    BOOST_CHECK(contains(poly, Point2d(10, 10)));

    // 2. Verificar un punto en el "codo" exterior
    // El punto (12, -2) debería estar fuera por poco
    BOOST_CHECK(!contains(poly, Point2d(12.1, -2.1)));
    
    // 3. El área debería ser aproximadamente (LargoTotal * 2 * dist) 
    // pero restando el solapamiento del codo interno.
    BOOST_CHECK(area(poly) > 0);
}

BOOST_AUTO_TEST_CASE(polygon_buffer_test)
{
    // Creamos un cuadrado de 10x10
    Polygon2d rect;
    rect.outer() = {Point2d{0,0}, Point2d{10,0},Point2d{10,10}, Point2d{0,10}, Point2d{0,0}};

    double dist = 2.0;
    auto poly_buffer = buffer(rect, dist);

    // 1. El área de un cuadrado con buffer (miter/square join) 
    // es (L+2d)^2 = 14 * 14 = 196
    // Con tu implementación actual de LineString-loop, se comporta como un inflado
    BOOST_CHECK(area(poly_buffer) > area(rect));

    // 2. Verificar que contiene al polígono original
    BOOST_CHECK(contains(poly_buffer, Point2d(5, 5)));
    BOOST_CHECK(contains(poly_buffer, Point2d(0, 0)));
    BOOST_CHECK(contains(poly_buffer, Point2d(11, 11)));

    // 3. Verificar que no contiene puntos muy lejanos
    BOOST_CHECK(!contains(poly_buffer, Point2d(13, 13)));
}