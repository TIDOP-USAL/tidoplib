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
 
#define BOOST_TEST_MODULE Tidop Rotation test
#include <boost/test/unit_test.hpp>

#include <tidop/math/math.h>
#include <tidop/geometry/transform/rotation.h>
#include <tidop/geometry/entities/point.h>

using namespace tl;


BOOST_AUTO_TEST_SUITE(RotationTestSuite)

struct RotationTest
{

  RotationTest()
    : rot2(new Rotation<Point<double>>(35. * math::consts::deg_to_rad<double>))
  {

  }

  ~RotationTest()
  {
    if (rot2){
      delete rot2;
      rot2 = nullptr;
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
      Point<double>(3024089.863, 2929048.411),
      Point<double>(3003596.978, 2944055.271),
      Point<double>(3022197.724, 2958915.254),
      Point<double>(3052911.339, 2922626.707),
      Point<double>(3003508.738, 2923205.629),
      Point<double>(3013895.705, 2924551.628),
      Point<double>(2987238.257, 2949489.327) };
  }

  void teardown() 
  {
  }

  Rotation<Point<double>> rot;
  Rotation<Point<double>> *rot2;
  std::vector<Point<double>> ptsIn;
  std::vector<Point<double>> ptsOut;
};

BOOST_FIXTURE_TEST_CASE(default_constructor, RotationTest)
{
  BOOST_CHECK_EQUAL(0., rot.angle());
  BOOST_CHECK_EQUAL(1., rot.minNumberOfPoints());
}

BOOST_FIXTURE_TEST_CASE(constructor, RotationTest)
{

  BOOST_CHECK_CLOSE(35 * math::consts::deg_to_rad<double>, rot2->angle(), 0.0001);
  BOOST_CHECK_EQUAL(1., rot2->minNumberOfPoints());
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, RotationTest)
{
  Rotation<Point<double>> copy(*rot2);
  BOOST_CHECK_CLOSE(35 * math::consts::deg_to_rad<double>, copy.angle(), 0.0001);
  BOOST_CHECK_EQUAL(1., copy.minNumberOfPoints());
}

BOOST_FIXTURE_TEST_CASE(assignement_operator, RotationTest)
{
  Rotation<Point<double>> assign = *rot2;
  BOOST_CHECK_CLOSE(35 * math::consts::deg_to_rad<double>, assign.angle(), 0.0001);
  BOOST_CHECK_EQUAL(1., assign.minNumberOfPoints());
}

BOOST_FIXTURE_TEST_CASE(move_constructor, RotationTest)
{
  Rotation<Point<double>> move(Rotation<Point<double>>(35. * math::consts::deg_to_rad<double>));
  BOOST_CHECK_CLOSE(35 * math::consts::deg_to_rad<double>, move.angle(), 0.0001);
  BOOST_CHECK_EQUAL(1., move.minNumberOfPoints());
}

BOOST_FIXTURE_TEST_CASE(move_operator, RotationTest)
{
  Rotation<Point<double>> move_assign = Rotation<Point<double>>(35. * math::consts::deg_to_rad<double>);
  BOOST_CHECK_CLOSE(35 * math::consts::deg_to_rad<double>, move_assign.angle(), 0.0001);
  BOOST_CHECK_EQUAL(1., move_assign.minNumberOfPoints());
}

BOOST_FIXTURE_TEST_CASE(setAngle, RotationTest)
{
  rot.setAngle(61. * math::consts::deg_to_rad<double>);
  BOOST_CHECK_CLOSE(61. * math::consts::deg_to_rad<double>, rot.angle(), 0.0001);
}

BOOST_FIXTURE_TEST_CASE(transform_point, RotationTest)
{
  Point<double> pt_out;
  rot2->transform(ptsIn[0], pt_out);
  BOOST_CHECK_CLOSE(ptsOut[0].x, pt_out.x, 0.1);
  BOOST_CHECK_CLOSE(ptsOut[0].y, pt_out.y, 0.1);
  pt_out = rot2->transform(ptsIn[0], Transform::Order::direct);
  BOOST_CHECK_CLOSE(ptsOut[0].x, pt_out.x, 0.1);
  BOOST_CHECK_CLOSE(ptsOut[0].y, pt_out.y, 0.1);
}

BOOST_FIXTURE_TEST_CASE(transform_point_list, RotationTest)
{
  std::vector<Point<double>> pts_out;
  rot2->transform(ptsIn, pts_out);

  for (size_t i = 0; i < ptsOut.size(); i++) {
    BOOST_CHECK_CLOSE(pts_out[i].x, ptsOut[i].x, 0.1);
    BOOST_CHECK_CLOSE(pts_out[i].y, ptsOut[i].y, 0.1);
  }
}

BOOST_FIXTURE_TEST_CASE(std_transform_point_list, RotationTest)
{
  std::vector<Point<double>> pts_out2(ptsIn.size());
  std::transform(ptsIn.begin(), ptsIn.end(), pts_out2.begin(), *rot2);

  for (size_t i = 0; i < ptsOut.size(); i++) {
    BOOST_CHECK_CLOSE(ptsOut[i].x, pts_out2[i].x, 0.1);
    BOOST_CHECK_CLOSE(ptsOut[i].y, pts_out2[i].y, 0.1);
  }

  // Alternativa sin reservar tamaño en pts_out2:
  std::vector<Point<double>> pts_out3;
  std::transform(ptsIn.begin(), ptsIn.end(), std::back_inserter(pts_out3), *rot2);


  for (size_t i = 0; i < ptsOut.size(); i++) {
    BOOST_CHECK_CLOSE(ptsOut[i].x, pts_out3[i].x, 0.1);
    BOOST_CHECK_CLOSE(ptsOut[i].y, pts_out3[i].y, 0.1);
  }
}

BOOST_FIXTURE_TEST_CASE(compute, RotationTest)
{
  rot.compute(ptsIn, ptsOut);
  BOOST_CHECK_CLOSE(35. * math::consts::deg_to_rad<double>, rot.angle(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(transformType, RotationTest)
{
  BOOST_CHECK(Transform::Type::rotation == rot.transformType());
}

BOOST_FIXTURE_TEST_CASE(isNumberOfPointsValid, RotationTest)
{
  BOOST_CHECK(rot2->isNumberOfPointsValid(1));
  BOOST_CHECK(rot2->isNumberOfPointsValid(10));
  BOOST_CHECK(false == rot2->isNumberOfPointsValid(0));
}

BOOST_FIXTURE_TEST_CASE(inverse, RotationTest)
{
  Rotation<Point<double>> rot_inv = rot2->inverse();
  BOOST_CHECK_CLOSE(-35 * math::consts::deg_to_rad<double>, rot_inv.angle(), 0.0001);
}

BOOST_FIXTURE_TEST_CASE(casting, RotationTest)
{
  // cast a Rotation float
  Rotation<Point<float>> rot_float = static_cast<Rotation<Point<float>>>(*rot2);
  BOOST_CHECK_CLOSE(35 * math::consts::deg_to_rad<double>, rot_float.angle(), 0.0001);

  // cast a Helmert2D
  Helmert2D<Point<double>> trf_h2d = static_cast<Helmert2D<Point<double>>>(*rot2);
  BOOST_CHECK_CLOSE(35 * math::consts::deg_to_rad<double>, trf_h2d.rotation(), 0.0001);
  BOOST_CHECK_EQUAL(1., trf_h2d.scale());
  BOOST_CHECK_EQUAL(0., trf_h2d.tx);
  BOOST_CHECK_EQUAL(0., trf_h2d.ty);
  BOOST_CHECK_EQUAL(2, trf_h2d.minNumberOfPoints());

  // cast a afín
  Affine<Point<double>> trf_affine = static_cast<Affine<Point<double>>>(*rot2);
  BOOST_CHECK_CLOSE(35 * math::consts::deg_to_rad<double>, trf_affine.rotation(), 0.0001);
  BOOST_CHECK_EQUAL(1., trf_affine.scaleX());
  BOOST_CHECK_EQUAL(1., trf_affine.scaleY());
  BOOST_CHECK_EQUAL(0., trf_affine.tx);
  BOOST_CHECK_EQUAL(0., trf_affine.ty);
  BOOST_CHECK_EQUAL(3, trf_affine.minNumberOfPoints());

  //Affine<Point<double>> *trf_affine_ptr = static_cast<Affine<Point<double>>*>(trf_pointer);
}

BOOST_AUTO_TEST_SUITE_END()
