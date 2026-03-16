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

#pragma once

#include "tidop/geometry/primitives/Point.h"
#include "tidop/geometry/primitives/Segment.h"
#include "tidop/geometry/primitives/LineString.h"
#include "tidop/geometry/primitives/Polygon.h"
#include "tidop/geometry/primitives/MultiPoint.h"
#include "tidop/geometry/primitives/MultiLineString.h"
#include "tidop/geometry/primitives/MultiPolygon.h"
#include "tidop/geometry/base/GeometryCollection.h"
#include "tidop/geometry/base/PrecisionPolicy.h"

namespace tl
{
	
namespace test
{
	
struct GeometryTestFixture
{
	PrecisionPolicy<double, PrecisionModel::FixedPrecisionModel> policy;
	Point2d point2d1;
	Point2d point2d2;
    Point2d point2d3;
    Point2d point2d4;
    Point2d point2d5;
    Point2d point2d6;
    Point2d point2d7;
    Point2dm point2dm1;
    Point2dm point2dm2;

    Segment2d segment1;
    Segment2d segment2;
    Segment2d segment3;
    Segment2d segment4;
    Segment2d segment5;
    Segment2d segment6;
    Segment2d segment7;
    Segment2d segment8;
    Segment2d segment9;
    Segment2d segment10;
    Segment2d segment11;
    Segment2d segment12;
    Segment2d segment13;
    Segment2d segment14;
    Segment2d segment15;
    Segment2d segment16;
    Segment2d segment17;
    Segment2d segment_out;
    Segment2d segment_colineal_1;
    Segment2d segment_colineal_2;

    Polygon2d square;
    Polygon2d square_with_hole;

	GeometryTestFixture()
		: policy(0.001)
	{
	}

    void setup()
    {
        point2d1 = Point2d(0.0, 0.0);
        point2d2 = Point2d(2.5, 2.5);
		point2d3 = Point2d(10.0, 10.0);
        point2d4 = Point2d(15.0, 15.0);
        point2d5 = Point2d(15.0001, 15.0001);
        point2d6 = Point2d(5.0, -5.0);
        point2d7 = Point2d(5.0, 5.0);
        point2dm1 = Point2dm(0.0, 0.0, 10.0);
        point2dm2 = Point2dm(2.5, 2.5, 20.0);

        segment1 = Segment2d(point2d1, point2d3);                      // Segmento diagonal del cuadrado (0,0)-(10,10)
        segment2 = Segment2d(point2d1, Point2d(0.0, 10.0));            // Segmento vertical del cuadrado (0,0)-(0,10)
        segment3 = Segment2d(point2d1, Point2d(10.0, 0.0));            // Segmento horizontal del cuadrado (0,0)-(10,0)
        segment4 = Segment2d(Point2d(0.0, 5.0), Point2d(10.0, 5.0));   // Segmento horizontal dentro del cuadrado (0,5)-(10,5)
        segment5 = Segment2d(Point2d(-0.5, 5.0), Point2d(10.5, 5.0));  // Segmento que cruza el cuadrado pero se extiende fuera de él (-0.5,5)-(10.5,5)
        segment6 = Segment2d(point2d1, point2d7);                      // Segmento que va desde el vértice del cuadrado hasta su centro (0,0)-(5,5)
        segment7 = Segment2d(Point2d(-0.5, 0.0), Point2d(5.0, 0.0));   // Segmento que se extiende fuera del cuadrado pero toca su borde inferior (-0.5,0)-(5,0)
        segment8 = Segment2d(Point2d(10.5, -0.5), Point2d(7.5, 2.5));  // Segmento que toca el hueco del cuadrado con agujero pero intersecta el exterior (10.5,-0.5)-(7.5,2.5)
        segment9 = Segment2d(Point2d(2.5, 5.0), Point2d(7.5, 5.0));    // Segmento que toca el hueco del cuadrado con agujero
        segment10 = Segment2d(Point2d(3.5, 5.0), Point2d(6.5, 5.0));   // Segmento que se encuentra dentro del hueco del cuadrado con agujero
        segment11 = Segment2d(Point2d(0.0, 5.0), Point2d(2.5, 5.0));   // Segmento que toca el borde del hueco del cuadrado con agujero en un extremo y el borde exterior en el otro (0,5)-(2.5,5)
        segment12 = Segment2d(Point2d(-0.5, 5), Point2d(1.5, 5));      // un extremo exterior y otro interior
        segment13 = Segment2d(Point2d(0.5, 5), Point2d(3.5, 5));       // un extremo exterior (detro del agujero) y otro interior
        segment14 = Segment2d(Point2d(-0.5, 5), Point2d(2.5, 5));      // Toca en un borde interior y el otro extremo exterior (fuera del agujero)
        segment15 = Segment2d(Point2d(0, 5), Point2d(2.5, 5));         // Toca en un borde exterior y en un borde interior
        segment16 = Segment2d(Point2d(2.5, 5), point2d7);              // Toca en un borde interior y el otro extremo está en el agujero
        segment17 = Segment2d(point2d6, point2d7);                     // Segmento vertical 
        segment_colineal_1 = Segment2d(point2d1, Point2d(5, 0));
        segment_colineal_2 = Segment2d(Point2d(5, 0), Point2d(10, 0));

        segment_out = Segment2d(Point2d(15.0, 15.0), Point2d(20.0, 20.0));     // Segmento completamente fuera del cuadrado (15,15)-(20,20)
        
        LinearRing<Point2d> outer({ Point2d(0.0, 0.0),
                                    Point2d(10.0, 0.0),
                                    Point2d(10.0, 10.0),
                                    Point2d(0.0, 10.0),
                                    Point2d(0.0, 0.0) });
        square = Polygon2d(outer);
            
        LinearRing2d inner({ Point2d(2.5, 2.5),
                             Point2d(7.5, 2.5),
                             Point2d(7.5,7.5),
                             Point2d(2.5, 7.5),
                             Point2d(2.5, 2.5) });

        square_with_hole = Polygon2d(outer, {inner});
    }

    void teardown()
    {

    }

};

} // End test

} // End tl