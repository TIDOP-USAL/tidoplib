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
 
#define BOOST_TEST_MODULE Tidop convex hull test
#include <boost/test/unit_test.hpp>
#include "tidop/geometry/primitives/Point.h"
#include "tidop/geometry/algorithms/spatial/ConvexHull.h"
#include "tidop/geometry/algorithms/analysis/Equals.h"

#include <variant>

using namespace tl;


//BOOST_AUTO_TEST_CASE(ConvexHull_Point2D)
//{
//    Point2d p{3.0, 4.0};
//    
//    auto hull = convexHull(p);
//    
//    // Convex hull de un punto debe ser el mismo punto
//    BOOST_CHECK(equals(hull, p));
//}
//
//BOOST_AUTO_TEST_CASE(ConvexHull_Point3D)
//{
//    Point3d p{1.0, 2.0, 3.0};
//    
//    auto hull = convexHull(p);
//    
//    BOOST_CHECK(equals(hull, p));
//}