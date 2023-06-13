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

#define BOOST_TEST_MODULE Tidop Translation test
#include <boost/test/unit_test.hpp>

#include <tidop/geometry/transform/translation.h>
#include <tidop/geometry/entities/point.h>
#include <tidop/math/algebra/vector.h>
#include "tidop/math/algebra/matrix.h"
#include "tidop/math/algebra/vector.h"
#include "tidop/math/algebra/svd.h"

using namespace tl;

BOOST_AUTO_TEST_SUITE(TranslationTestSuite)

struct TranslationTest
{
  TranslationTest()
    : trf_pointer(new Translation<Point<double>>(150.0, 75.0))
  {}

  ~TranslationTest()
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
      Point<double>(4157372.543,	664864.307),
      Point<double>(4149193.336,	688911.443),
      Point<double>(4172953.511,	690415.078),
      Point<double>(4177298.376,	643072.635),
      Point<double>(4137162.19, 671883.029),
      Point<double>(4146442.729,	667027.887),
      Point<double>(4138909.902,	702745.738) };

    vectorIn = {
      Vector2d{{4157222.543, 664789.307}},
      Vector2d{{4149043.336, 688836.443}},
      Vector2d{{4172803.511, 690340.078}},
      Vector2d{{4177148.376, 642997.635}},
      Vector2d{{4137012.190, 671808.029}},
      Vector2d{{4146292.729, 666952.887}},
      Vector2d{{4138759.902, 702670.738}}
    };

    vectorOut = {
      Vector2d{{4157372.543,	664864.307}},
      Vector2d{{4149193.336,	688911.443}},
      Vector2d{{4172953.511,	690415.078}},
      Vector2d{{4177298.376,	643072.635}},
      Vector2d{{4137162.19,   671883.029}},
      Vector2d{{4146442.729,	667027.887}},
      Vector2d{{4138909.902,	702745.738}}
    };

  }

  void teardown()
  {

  }

  std::vector<Point<double>> ptsIn;
  std::vector<Point<double>> ptsOut;
  std::vector<Vector2d> vectorIn;
  std::vector<Vector2d> vectorOut;
  Translation<Point<double>> trf;
  Translation<Point<double>> *trf_pointer;
  //Translation<Vector2d> trf_vector;
};

BOOST_FIXTURE_TEST_CASE(default_constructor, TranslationTest)
{
  BOOST_CHECK_EQUAL(0., trf.tx);
  BOOST_CHECK_EQUAL(0., trf.ty);
  BOOST_CHECK_EQUAL(1., trf.minNumberOfPoints());
}

BOOST_FIXTURE_TEST_CASE(constructor, TranslationTest)
{
  BOOST_CHECK_EQUAL(150.0, trf_pointer->tx);
  BOOST_CHECK_EQUAL(75.0, trf_pointer->ty);
  BOOST_CHECK_EQUAL(1, trf_pointer->minNumberOfPoints());
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, TranslationTest)
{
  Translation<Point<double>> copy(*trf_pointer);
  BOOST_CHECK_EQUAL(150.0, copy.tx);
  BOOST_CHECK_EQUAL(75.0, copy.ty);
  BOOST_CHECK_EQUAL(1, copy.minNumberOfPoints());
}

BOOST_FIXTURE_TEST_CASE(assignement_operator, TranslationTest)
{
  Translation<Point<double>> assign = *trf_pointer;
  BOOST_CHECK_EQUAL(150.0, assign.tx);
  BOOST_CHECK_EQUAL(75.0, assign.ty);
  BOOST_CHECK_EQUAL(1, assign.minNumberOfPoints());
}

BOOST_FIXTURE_TEST_CASE(move_constructor, TranslationTest)
{
  Translation<Point<double>> move(Translation<Point<double>>(150.0, 75.0));
  BOOST_CHECK_EQUAL(150.0, move.tx);
  BOOST_CHECK_EQUAL(75.0, move.ty);
  BOOST_CHECK_EQUAL(1, move.minNumberOfPoints());
}

BOOST_FIXTURE_TEST_CASE(move_operator, TranslationTest)
{
  Translation<Point<double>> move_assign = Translation<Point<double>>(150.0, 75.0);
  BOOST_CHECK_EQUAL(150.0, move_assign.tx);
  BOOST_CHECK_EQUAL(75.0, move_assign.ty);
  BOOST_CHECK_EQUAL(1, move_assign.minNumberOfPoints());
}

BOOST_FIXTURE_TEST_CASE(transform_point, TranslationTest)
{
  Point<double> pt_out;
  trf_pointer->transform(ptsIn[0], pt_out);
  BOOST_CHECK_EQUAL(ptsOut[0].x, pt_out.x);
  BOOST_CHECK_EQUAL(ptsOut[0].y, pt_out.y);
  pt_out = trf_pointer->transform(ptsIn[0], Transform::Order::direct);
  BOOST_CHECK_EQUAL(ptsOut[0].x, pt_out.x);
  BOOST_CHECK_EQUAL(ptsOut[0].y, pt_out.y);
}

BOOST_FIXTURE_TEST_CASE(transform_point_list, TranslationTest)
{
  std::vector<Point<double>> pts_out;
  trf_pointer->transform(ptsIn, pts_out);

  for (size_t i = 0; i < ptsOut.size(); i++) {
    BOOST_CHECK_EQUAL(ptsOut[i].x, pts_out[i].x);
    BOOST_CHECK_EQUAL(ptsOut[i].y, pts_out[i].y);
  }
}

BOOST_FIXTURE_TEST_CASE(std_transform_point_list, TranslationTest)
{
  std::vector<Point<double>> pts_out2(ptsIn.size());
  std::transform(ptsIn.begin(), ptsIn.end(), pts_out2.begin(), *trf_pointer);

  for (size_t i = 0; i < ptsOut.size(); i++) {
    BOOST_CHECK_EQUAL(ptsOut[i].x, pts_out2[i].x);
    BOOST_CHECK_EQUAL(ptsOut[i].y, pts_out2[i].y);
  }

  // Alternativa sin reservar tamaño en pts_out2:
  std::vector<Point<double>> pts_out3;
  std::transform(ptsIn.begin(), ptsIn.end(), std::back_inserter(pts_out3), *trf_pointer);

  for (size_t i = 0; i < ptsOut.size(); i++) {
    BOOST_CHECK_EQUAL(ptsOut[i].x, pts_out3[i].x);
    BOOST_CHECK_EQUAL(ptsOut[i].y, pts_out3[i].y);
  }
}

BOOST_FIXTURE_TEST_CASE(compute, TranslationTest)
{

  trf_pointer->compute(ptsIn, ptsOut);
  BOOST_CHECK_CLOSE(150.0, trf_pointer->tx, 0.1);
  BOOST_CHECK_CLOSE(75.0,  trf_pointer->ty, 0.1);

//  // Error
//
//  std::vector<Point<double>> ptsIn2{
//    Point<double>(4157222.543, 664789.307),
//    Point<double>(4149043.336, 688836.443),
//    Point<double>(4172803.511, 690340.078),
//    Point<double>(4177148.376, 642997.635)};
//
//  Transform::Status status = trf.compute(ptsIn2, ptsOut);
//
//  BOOST_CHECK_EQUAL(Transform::Status::FAILURE, trf.compute(ptsIn2, ptsOut));
//
//  //EXPECT_THROW({
//  //      try
//  //      {
//  //        // Distinto número de puntos
//  //        trf.compute(ptsIn2, ptsOut);
//  //      } catch( TL::Exception &e ) {
//  //          EXPECT_STREQ( "Sets of points with different size", e.what() );
//  //          throw;
//  //      }
//  //  }, TL::Exception );
}

BOOST_FIXTURE_TEST_CASE(transformType, TranslationTest)
{
  BOOST_CHECK(Transform::Type::translation == trf.transformType());
}

BOOST_FIXTURE_TEST_CASE(isNumberOfPointsValid, TranslationTest)
{
  BOOST_CHECK(trf_pointer->isNumberOfPointsValid(1));
  BOOST_CHECK(trf_pointer->isNumberOfPointsValid(10));
  BOOST_CHECK(false == trf_pointer->isNumberOfPointsValid(0));
}

BOOST_FIXTURE_TEST_CASE(inverse, TranslationTest)
{
  Translation<Point<double>> trf_inv = trf_pointer->inverse();
  BOOST_CHECK_EQUAL(-150.0, trf_inv.tx);
  BOOST_CHECK_EQUAL(-75.0, trf_inv.ty);
}

BOOST_FIXTURE_TEST_CASE(casting, TranslationTest)
{
  // cast a Translation float
  Translation<Point<float>> trf_float = static_cast<Translation<Point<float>>>(trf);
  BOOST_CHECK_EQUAL(0.f, trf_float.tx);
  BOOST_CHECK_EQUAL(0.f, trf_float.ty);

  // cast a Helmert2D
  Helmert2D<Point<double>> trf_h2d = static_cast<Helmert2D<Point<double>>>(trf);
  BOOST_CHECK_EQUAL(0.0, trf_h2d.rotation());
  BOOST_CHECK_EQUAL(1., trf_h2d.scale());
  BOOST_CHECK_EQUAL(0., trf_h2d.tx);
  BOOST_CHECK_EQUAL(0., trf_h2d.ty);
  BOOST_CHECK_EQUAL(2, trf_h2d.minNumberOfPoints());

  //// cast a afín
  //Affine<Point<double>> trf_affine = static_cast<Affine<Point<double>>>(trf);
  //BOOST_CHECK_EQUAL(0.0, trf_affine.rotation());
  //BOOST_CHECK_EQUAL(1., trf_affine.scaleX());
  //BOOST_CHECK_EQUAL(1., trf_affine.scaleY());
  //BOOST_CHECK_EQUAL(0., trf_affine.tx);
  //BOOST_CHECK_EQUAL(0., trf_affine.ty);
  //BOOST_CHECK_EQUAL(3, trf_affine.minNumberOfPoints());

  //Affine<Point<double>> *trf_affine_ptr = static_cast<Affine<Point<double>>*>(trf_pointer);
}

BOOST_AUTO_TEST_SUITE_END()

