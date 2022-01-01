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
 
#define BOOST_TEST_MODULE Tidop Scaling test
#include <boost/test/unit_test.hpp>

#include <tidop/geometry/transform/scale.h>
#include <tidop/geometry/entities/point.h>

#include <tidop/core/utils.h>
#include <tidop/core/exception.h>
#include <tidop/math/algebra/vector.h>

using namespace tl;


BOOST_AUTO_TEST_SUITE(ScalingTestSuite)

struct ScalingTest
{

  ScalingTest()
    : trf_pointer(new Scaling<PointD>(0.25))
  {

  }
    
  ~ScalingTest()
  {
    if (trf_pointer){
      delete trf_pointer;
      trf_pointer = nullptr;
    }
  }

  void setup()
  {
    ptsIn = {
      PointD(4157222.543, 664789.307),
      PointD(4149043.336, 688836.443),
      PointD(4172803.511, 690340.078),
      PointD(4177148.376, 642997.635),
      PointD(4137012.190, 671808.029),
      PointD(4146292.729, 666952.887),
      PointD(4138759.902, 702670.738) };

    ptsOut = {
      PointD(1039305.636,  166197.327),
      PointD(1037260.834,  172209.111),
      PointD(1043200.878,  172585.020),
      PointD(1044287.094,  160749.409),
      PointD(1034253.048,  167952.007),
      PointD(1036573.182,  166738.222),
      PointD(1034689.976,  175667.685) };
  }

  void teardown()
  {
  }

  std::vector<PointD> ptsIn;
  std::vector<PointD> ptsOut;
  Scaling<PointD> trf;
  Scaling<PointD> *trf_pointer;
};

BOOST_FIXTURE_TEST_CASE(default_constructor, ScalingTest)
{
  BOOST_CHECK_EQUAL(1., trf.scale());
  BOOST_CHECK_EQUAL(1, trf.minNumberOfPoints());
}

BOOST_FIXTURE_TEST_CASE(constructor, ScalingTest)
{
  BOOST_CHECK_EQUAL(0.25, trf_pointer->scale());
  BOOST_CHECK_EQUAL(1, trf_pointer->minNumberOfPoints());
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, ScalingTest)
{
  Scaling<PointD> copy(*trf_pointer);
  BOOST_CHECK_EQUAL(0.25, copy.scale());
  BOOST_CHECK_EQUAL(1, copy.minNumberOfPoints());
}

BOOST_FIXTURE_TEST_CASE(assignement_operator, ScalingTest)
{
  Scaling<PointD> assign = *trf_pointer;
  BOOST_CHECK_EQUAL(0.25, assign.scale());
  BOOST_CHECK_EQUAL(1, assign.minNumberOfPoints());
}

BOOST_FIXTURE_TEST_CASE(move_constructor, ScalingTest)
{
  Scaling<PointD> move(Scaling<PointD>(0.25));
  BOOST_CHECK_EQUAL(0.25, move.scale());
  BOOST_CHECK_EQUAL(1, move.minNumberOfPoints());
}

BOOST_FIXTURE_TEST_CASE(move_operator, ScalingTest)
{
  Scaling<PointD> move(Scaling<PointD>(0.25));
  BOOST_CHECK_EQUAL(0.25, move.scale());
  BOOST_CHECK_EQUAL(1, move.minNumberOfPoints());

  Scaling<PointD> move_assig = Scaling<PointD>(0.25);
  BOOST_CHECK_EQUAL(0.25, move_assig.scale());
  BOOST_CHECK_EQUAL(1, move_assig.minNumberOfPoints());
}

BOOST_FIXTURE_TEST_CASE(transform_point, ScalingTest)
{
  PointD pt_out;
  trf_pointer->transform(ptsIn[0], pt_out);
  BOOST_CHECK_CLOSE(ptsOut[0].x, pt_out.x, 0.1);
  BOOST_CHECK_CLOSE(ptsOut[0].y, pt_out.y, 0.1);
  pt_out = trf_pointer->transform(ptsIn[0], Transform::Order::direct);
  BOOST_CHECK_CLOSE(ptsOut[0].x, pt_out.x, 0.1);
  BOOST_CHECK_CLOSE(ptsOut[0].y, pt_out.y, 0.1);
}

BOOST_FIXTURE_TEST_CASE(transform_point_list, ScalingTest)
{
  std::vector<PointD> pts_out;
  trf_pointer->transform(ptsIn, pts_out);

  for (size_t i = 0; i < ptsOut.size(); i++) {
    BOOST_CHECK_CLOSE(pts_out[i].x, ptsOut[i].x, 0.1);
    BOOST_CHECK_CLOSE(pts_out[i].y, ptsOut[i].y, 0.1);
  }
}

BOOST_FIXTURE_TEST_CASE(std_transform_point_list, ScalingTest)
{
  std::vector<PointD> pts_out2(ptsIn.size());
  std::transform(ptsIn.begin(), ptsIn.end(), pts_out2.begin(), *trf_pointer);

  for (size_t i = 0; i < ptsOut.size(); i++) {
    BOOST_CHECK_CLOSE(ptsOut[i].x, pts_out2[i].x, 0.1);
    BOOST_CHECK_CLOSE(ptsOut[i].y, pts_out2[i].y, 0.1);
  }

  // Alternativa sin reservar tamaño en pts_out2:
  std::vector<PointD> pts_out3;
  std::transform(ptsIn.begin(), ptsIn.end(), std::back_inserter(pts_out3), *trf_pointer);


  for (size_t i = 0; i < ptsOut.size(); i++) {
    BOOST_CHECK_CLOSE(ptsOut[i].x, pts_out3[i].x, 0.1);
    BOOST_CHECK_CLOSE(ptsOut[i].y, pts_out3[i].y, 0.1);
  }
}

BOOST_FIXTURE_TEST_CASE(compute, ScalingTest)
{
  /// TODO: no esta bien calculado
  trf.compute(ptsIn, ptsOut);
  BOOST_CHECK_CLOSE(0.25, trf.scale(), 0.01);
}

BOOST_FIXTURE_TEST_CASE(transformType, ScalingTest)
{
  BOOST_CHECK(Transform::Type::scaling == trf.transformType());
}

BOOST_FIXTURE_TEST_CASE(isNumberOfPointsValid, ScalingTest)
{
  BOOST_CHECK(trf_pointer->isNumberOfPointsValid(1));
  BOOST_CHECK(trf_pointer->isNumberOfPointsValid(10));
  BOOST_CHECK(false == trf_pointer->isNumberOfPointsValid(0));
}

BOOST_FIXTURE_TEST_CASE(scale, ScalingTest)
{
  BOOST_CHECK_EQUAL(0.25, trf_pointer->scale());
}

BOOST_FIXTURE_TEST_CASE(setScale, ScalingTest)
{
  trf_pointer->setScale(0.64);
  BOOST_CHECK_EQUAL( 0.64, trf_pointer->scale());
}

BOOST_FIXTURE_TEST_CASE(inverse, ScalingTest)
{
  Scaling<PointD> trf_inv = trf_pointer->inverse();
  BOOST_CHECK_CLOSE(4., trf_inv.scale(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(casting, ScalingTest)
{
//  // cast a Translation float
//  Scaling<Point<float>> trf_ = static_cast<Scaling<Point<float>>>(*trf_pointer);
//  BOOST_CHECK_EQUAL(0.25, trf_.scale());
//  BOOST_CHECK_EQUAL(1, trf_.minNumberOfPoints());

//  // cast a Helmert2D float
//  Helmert2D<PointD> trf_h2d = static_cast<Helmert2D<PointD>>(*trf_pointer);
//  BOOST_CHECK_EQUAL(0.0, trf_h2d);
//  BOOST_CHECK_EQUAL(0.25, trf_h2d.scale());
//  BOOST_CHECK_EQUAL(0., trf_h2d.tx);
//  BOOST_CHECK_EQUAL(0., trf_h2d.ty);
//  BOOST_CHECK_EQUAL(2, trf_h2d.minNumberOfPoints());

//  // cast a afín
//  Affine<PointD> trf_affine = static_cast<Affine<PointD>>(*trf_pointer);
//  BOOST_CHECK_EQUAL(0., trf_affine.rotation());
//  BOOST_CHECK_EQUAL(0.25, trf_affine.scaleX());
//  BOOST_CHECK_EQUAL(0.25, trf_affine.scaleY());
//  BOOST_CHECK_EQUAL(.0, trf_affine.tx);
//  BOOST_CHECK_EQUAL(.0, trf_affine.ty);
//  BOOST_CHECK_EQUAL(3, trf_affine.minNumberOfPoints());

}

BOOST_AUTO_TEST_SUITE_END()
