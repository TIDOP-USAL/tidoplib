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
 
#define BOOST_TEST_MODULE Tidop Helmert2D test
#include <boost/test/unit_test.hpp>

#include <tidop/geometry/transform/helmert2d.h>
#include <tidop/geometry/entities/point.h>

using namespace tl;



BOOST_AUTO_TEST_SUITE(Helmert2DTestSuite)

struct Helmert2DTest
{
  Helmert2DTest()
    : trf_pointer(new Helmert2D<Point<double>>(150.0, 75.0, 0.25, 35 * math::consts::deg_to_rad<double>))
  {

  }

  ~Helmert2DTest()
  {
    if (trf_pointer){
      delete trf_pointer;
      trf_pointer = nullptr;
    }
  }

  void setup()
  {
    ptsIn = {
      Point<double>(4157222.543, 664789.307),
      Point<double>(4149043.336, 688836.443),
      Point<double>(4172803.511, 690340.078),
      Point<double>(4177148.376, 642997.635),
      Point<double>(4137012.190, 671808.029),
      Point<double>(4146292.729, 666952.887),
      Point<double>(4138759.902, 702670.738) };

    ptsOut = {
      Point<double>(756172.466,	732337.103),
      Point<double>(751049.245,	736088.818),
      Point<double>(755699.431,	739803.813),
      Point<double>(763377.835,	730731.677),
      Point<double>(751027.184,  730876.407),
      Point<double>(753623.926,	731212.907),
      Point<double>(746959.564,	737447.332) };

  }

  void teardown() 
  {
  }

  std::vector<Point<double>> ptsIn;
  std::vector<Point<double>> ptsOut;
  Helmert2D<Point<double>> trf;
  Helmert2D<Point<double>> *trf_pointer;
};

BOOST_FIXTURE_TEST_CASE(default_constructor, Helmert2DTest)
{
  BOOST_CHECK_EQUAL(0.0, trf.rotation());
  BOOST_CHECK_EQUAL(1., trf.scale());
  BOOST_CHECK_EQUAL(0., trf.tx);
  BOOST_CHECK_EQUAL(0., trf.ty);
  BOOST_CHECK_EQUAL(2, trf.minNumberOfPoints());
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, Helmert2DTest)
{
  Helmert2D<Point<double>> copy(*trf_pointer);
  BOOST_CHECK_CLOSE( 35 * math::consts::deg_to_rad<double>, copy.rotation(), 0.0001);
  BOOST_CHECK_EQUAL(0.25, copy.scale());
  BOOST_CHECK_EQUAL(150.0, copy.tx);
  BOOST_CHECK_EQUAL(75.0, copy.ty);
  BOOST_CHECK_EQUAL(2, copy.minNumberOfPoints());
}

BOOST_FIXTURE_TEST_CASE(assignement_operator, Helmert2DTest)
{
  Helmert2D<Point<double>> assig = *trf_pointer;
  BOOST_CHECK_CLOSE( 35 * math::consts::deg_to_rad<double>, assig.rotation(), 0.0001);
  BOOST_CHECK_EQUAL(0.25, assig.scale());
  BOOST_CHECK_EQUAL(150.0, assig.tx);
  BOOST_CHECK_EQUAL(75.0, assig.ty);
  BOOST_CHECK_EQUAL(2, assig.minNumberOfPoints());
}

BOOST_FIXTURE_TEST_CASE(move_constructor, Helmert2DTest)
{
  Helmert2D<Point<double>> move(Helmert2D<Point<double>>(150.0, 75.0, 0.25, 35 * math::consts::deg_to_rad<double>));
  BOOST_CHECK_CLOSE( 35 * math::consts::deg_to_rad<double>, move.rotation(), 0.0001);
  BOOST_CHECK_EQUAL(0.25, move.scale());
  BOOST_CHECK_EQUAL(150.0, move.tx);
  BOOST_CHECK_EQUAL(75.0, move.ty);
  BOOST_CHECK_EQUAL(2, move.minNumberOfPoints());
}

BOOST_FIXTURE_TEST_CASE(move_operator, Helmert2DTest)
{
  Helmert2D<Point<double>> move_assig = Helmert2D<Point<double>>(150.0, 75.0, 0.25, 35 * math::consts::deg_to_rad<double>);
  BOOST_CHECK_CLOSE( 35 * math::consts::deg_to_rad<double>, move_assig.rotation(), 0.0001);
  BOOST_CHECK_EQUAL(0.25, move_assig.scale());
  BOOST_CHECK_EQUAL(150.0, move_assig.tx);
  BOOST_CHECK_EQUAL(75.0, move_assig.ty);
  BOOST_CHECK_EQUAL(2, move_assig.minNumberOfPoints());
}

BOOST_FIXTURE_TEST_CASE(transform_point, Helmert2DTest)
{
  Point<double> pt_out;
  trf_pointer->transform(ptsIn[0], pt_out);
  BOOST_CHECK_CLOSE(ptsOut[0].x, pt_out.x, 0.1);
  BOOST_CHECK_CLOSE(ptsOut[0].y, pt_out.y, 0.1);
  pt_out = trf_pointer->transform(ptsIn[0], Transform::Order::direct);
  BOOST_CHECK_CLOSE(ptsOut[0].x, pt_out.x, 0.1);
  BOOST_CHECK_CLOSE(ptsOut[0].y, pt_out.y, 0.1);
}

BOOST_FIXTURE_TEST_CASE(transform_point_list, Helmert2DTest)
{
  std::vector<Point<double>> pts_out;
  trf_pointer->transform(ptsIn, pts_out);

  for (size_t i = 0; i < ptsOut.size(); i++) {
    BOOST_CHECK_CLOSE(pts_out[i].x, ptsOut[i].x, 0.1);
    BOOST_CHECK_CLOSE(pts_out[i].y, ptsOut[i].y, 0.1);
  }
}

BOOST_FIXTURE_TEST_CASE(std_transform, Helmert2DTest)
{
  std::vector<Point<double>> pts_out2(ptsIn.size());
  std::transform(ptsIn.begin(), ptsIn.end(), pts_out2.begin(), *trf_pointer);

  for (size_t i = 0; i < ptsOut.size(); i++) {
    BOOST_CHECK_CLOSE(ptsOut[i].x, pts_out2[i].x, 0.1);
    BOOST_CHECK_CLOSE(ptsOut[i].y, pts_out2[i].y, 0.1);
  }

  // Alternativa sin reservar tamaño en pts_out2:
  std::vector<Point<double>> pts_out3;
  std::transform(ptsIn.begin(), ptsIn.end(), std::back_inserter(pts_out3), *trf_pointer);


  for (size_t i = 0; i < ptsOut.size(); i++) {
    BOOST_CHECK_CLOSE(ptsOut[i].x, pts_out3[i].x, 0.1);
    BOOST_CHECK_CLOSE(ptsOut[i].y, pts_out3[i].y, 0.1);
  }
}

BOOST_FIXTURE_TEST_CASE(compute, Helmert2DTest)
{
  trf.compute(ptsIn, ptsOut);
  BOOST_CHECK_CLOSE(35 * math::consts::deg_to_rad<double>, trf.rotation(), 0.01);
  BOOST_CHECK_CLOSE(0.25, trf.scale(), 0.01);
  BOOST_CHECK_CLOSE(150.0, trf.tx, 0.1);
  BOOST_CHECK_CLOSE(75.0, trf.ty, 0.1);
}

BOOST_FIXTURE_TEST_CASE(transformType, Helmert2DTest)
{
  BOOST_CHECK(Transform::Type::helmert_2d == trf.transformType());
}

BOOST_FIXTURE_TEST_CASE(isNumberOfPointsValid, Helmert2DTest)
{
  BOOST_CHECK(trf_pointer->isNumberOfPointsValid(2));
  BOOST_CHECK(trf_pointer->isNumberOfPointsValid(10));
  BOOST_CHECK(false == trf_pointer->isNumberOfPointsValid(0));
  BOOST_CHECK(false == trf_pointer->isNumberOfPointsValid(1));
}

BOOST_FIXTURE_TEST_CASE(rotation, Helmert2DTest)
{
  BOOST_CHECK_CLOSE( 35 * math::consts::deg_to_rad<double>, trf_pointer->rotation(), 0.0001);
}

BOOST_FIXTURE_TEST_CASE(scale, Helmert2DTest)
{
  BOOST_CHECK_EQUAL(0.25, trf_pointer->scale());
}

BOOST_FIXTURE_TEST_CASE(setParameters, Helmert2DTest)
{

  trf_pointer->setParameters(0.56, 163.25, 2., .131);
  BOOST_CHECK_EQUAL( .131, trf_pointer->rotation());
  BOOST_CHECK_EQUAL(2., trf_pointer->scale());
  BOOST_CHECK_EQUAL(0.56, trf_pointer->tx);
  BOOST_CHECK_EQUAL(163.25, trf_pointer->ty);
}

BOOST_FIXTURE_TEST_CASE(setRotation, Helmert2DTest)
{
  trf_pointer->setRotation(47 * math::consts::deg_to_rad<double>);
  BOOST_CHECK_EQUAL( 47 * math::consts::deg_to_rad<double>, trf_pointer->rotation());
}

BOOST_FIXTURE_TEST_CASE(setScale, Helmert2DTest)
{
  trf_pointer->setScale(0.64);
  BOOST_CHECK_EQUAL( 0.64, trf_pointer->scale());

}

BOOST_FIXTURE_TEST_CASE(inverse, Helmert2DTest)
{
  Helmert2D<Point<double>> trf_inv = trf_pointer->inverse();
  BOOST_CHECK_CLOSE(-663.564, trf_inv.tx, 0.1);
  BOOST_CHECK_CLOSE(98.400, trf_inv.ty, 0.1);
  BOOST_CHECK_CLOSE(-0.6108, trf_inv.rotation(), 0.1);
  BOOST_CHECK_CLOSE(4., trf_inv.scale(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(casting, Helmert2DTest)
{
  // cast a Translation float
  Helmert2D<Point<float>> trf_h2d = static_cast<Helmert2D<Point<float>>>(trf);
  BOOST_CHECK_EQUAL(0.0, trf_h2d.rotation());
  BOOST_CHECK_EQUAL(1., trf_h2d.scale());
  BOOST_CHECK_EQUAL(0., trf_h2d.tx);
  BOOST_CHECK_EQUAL(0., trf_h2d.ty);
  BOOST_CHECK_EQUAL(2, trf_h2d.minNumberOfPoints());

  // cast a afín
  Affine<Point<double>> trf_affine = static_cast<Affine<Point<double>>>(*trf_pointer);
  BOOST_CHECK_EQUAL(35 * math::consts::deg_to_rad<double>, trf_affine.rotation());
  BOOST_CHECK_EQUAL(0.25, trf_affine.scaleX());
  BOOST_CHECK_EQUAL(0.25, trf_affine.scaleY());
  BOOST_CHECK_EQUAL(150.0, trf_affine.tx);
  BOOST_CHECK_EQUAL(75.0, trf_affine.ty);
  BOOST_CHECK_EQUAL(3, trf_affine.minNumberOfPoints());

  Translation<Point<double>> translation = static_cast<Translation<Point<double>>>(*trf_pointer);
  BOOST_CHECK_EQUAL(150.0, translation.tx);
  BOOST_CHECK_EQUAL(75.0, translation.ty);

  Rotation<Point<double>> rotation = static_cast<Rotation<Point<double>>>(*trf_pointer);
  BOOST_CHECK_EQUAL(35 * math::consts::deg_to_rad<double>, rotation.angle());
}

BOOST_AUTO_TEST_SUITE_END()
