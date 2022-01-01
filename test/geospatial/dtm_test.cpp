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
 
#define BOOST_TEST_MODULE Tidop geospatial dtm test
#include <boost/test/unit_test.hpp>
#include <tidop/geospatial/dtm.h>

using namespace tl;
using namespace geospatial;

//class DtmTest
//  : public testing::Test
//{
//public:

//  virtual void SetUp()
//  {
//    double radius = 0.;
//    dtm_linear = new DtmLinear(radius);
//    double radius1 = 0.0;
//    double radius2 = 0.0;
//    double angle = 0.0;
//    dtm_linear = new DtmNearest(radius1, radius2, angle);
//  }

//  virtual void TearDown()
//  {
//    if (dtm_linear){
//      delete dtm_linear;
//      dtm_linear = nullptr;
//    }
//  }

//  Dtm *dtm_linear;
//  Dtm *dtm_nearest;
//};

//TEST_F(DtmTest, Constructor)
//{
//  //EXPECT_TRUE(dtm_linear->interpolation() == Dtm::Interpolation::LINEAR);
//  //EXPECT_TRUE(dtm_nearest->interpolation() == Dtm::Interpolation::NEAREST);
//}
