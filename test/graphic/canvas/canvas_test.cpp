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
 * License along with Foobar. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/
 
#define BOOST_TEST_MODULE Tidop canvas test
#include <boost/test/unit_test.hpp>
#include <tidop/graphic/canvas.h>

using namespace tl;
using namespace graph;

//class CanvasTest
//  : public testing::Test
//{
//public:
//
//protected:
//
//  virtual void SetUp() override
//  {
//    canvas2 = new CanvasCV();
//  }
//
//  virtual void TearDown() override
//  {
//    if (canvas2) {
//      delete canvas2;
//      canvas2 = nullptr;
//    }
//  }
//
//  CanvasCV canvas1;
//  Canvas *canvas2;
//};


//TEST_F(CanvasTest, DefaultConstructor)
//{
//  graph::Color bg_color(graph::Color::NAME::White);
//
//  EXPECT_EQ(100, canvas1.height());
//  EXPECT_EQ(100, canvas1.width());
//  EXPECT_TRUE(bg_color == canvas1.backgroundColor());
//
//  EXPECT_EQ(100, canvas2->height());
//  EXPECT_EQ(100, canvas2->width());
//  EXPECT_TRUE(bg_color == canvas2->backgroundColor());
//}
