#define BOOST_TEST_MODULE Tidop Transform test
#include <boost/test/unit_test.hpp>
#include <tidop/geometry/transform.h>
#include <tidop/core/utils.h>
#include <tidop/core/exception.h>
#include <tidop/math/algebra/vector.h>

using namespace tl;

BOOST_AUTO_TEST_SUITE(TranslationTestSuite)

struct TranslationTest
{
  TranslationTest()
    :trf_pointer(new Translation<PointD>(150.0, 75.0))
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
      PointD(4157222.543, 664789.307),
      PointD(4149043.336, 688836.443),
      PointD(4172803.511, 690340.078),
      PointD(4177148.376, 642997.635),
      PointD(4137012.190, 671808.029),
      PointD(4146292.729, 666952.887),
      PointD(4138759.902, 702670.738) };

    ptsOut = {
      PointD(4157372.543,	664864.307),
      PointD(4149193.336,	688911.443),
      PointD(4172953.511,	690415.078),
      PointD(4177298.376,	643072.635),
      PointD(4137162.19, 671883.029),
      PointD(4146442.729,	667027.887),
      PointD(4138909.902,	702745.738) };

    vectorIn = {
      math::Vector2d{{4157222.543, 664789.307}},
      math::Vector2d{{4149043.336, 688836.443}},
      math::Vector2d{{4172803.511, 690340.078}},
      math::Vector2d{{4177148.376, 642997.635}},
      math::Vector2d{{4137012.190, 671808.029}},
      math::Vector2d{{4146292.729, 666952.887}},
      math::Vector2d{{4138759.902, 702670.738}}
    };

    vectorOut = {
      math::Vector2d{{4157372.543,	664864.307}},
      math::Vector2d{{4149193.336,	688911.443}},
      math::Vector2d{{4172953.511,	690415.078}},
      math::Vector2d{{4177298.376,	643072.635}},
      math::Vector2d{{4137162.19,   671883.029}},
      math::Vector2d{{4146442.729,	667027.887}},
      math::Vector2d{{4138909.902,	702745.738}}
    };

  }

  void teardown()
  {

  }

  std::vector<PointD> ptsIn;
  std::vector<PointD> ptsOut;
  std::vector<math::Vector2d> vectorIn;
  std::vector<math::Vector2d> vectorOut;
  Translation<PointD> trf;
  Translation<PointD> *trf_pointer;
  //Translation<math::Vector2d> trf_vector;
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
  Translation<PointD> copy(*trf_pointer);
  BOOST_CHECK_EQUAL(150.0, copy.tx);
  BOOST_CHECK_EQUAL(75.0, copy.ty);
  BOOST_CHECK_EQUAL(1, copy.minNumberOfPoints());
}

BOOST_FIXTURE_TEST_CASE(assignement_operator, TranslationTest)
{
  Translation<PointD> assign = *trf_pointer;
  BOOST_CHECK_EQUAL(150.0, assign.tx);
  BOOST_CHECK_EQUAL(75.0, assign.ty);
  BOOST_CHECK_EQUAL(1, assign.minNumberOfPoints());
}

BOOST_FIXTURE_TEST_CASE(move_constructor, TranslationTest)
{
  Translation<PointD> move(Translation<PointD>(150.0, 75.0));
  BOOST_CHECK_EQUAL(150.0, move.tx);
  BOOST_CHECK_EQUAL(75.0, move.ty);
  BOOST_CHECK_EQUAL(1, move.minNumberOfPoints());
}

BOOST_FIXTURE_TEST_CASE(move_operator, TranslationTest)
{
  Translation<PointD> move_assign = Translation<PointD>(150.0, 75.0);
  BOOST_CHECK_EQUAL(150.0, move_assign.tx);
  BOOST_CHECK_EQUAL(75.0, move_assign.ty);
  BOOST_CHECK_EQUAL(1, move_assign.minNumberOfPoints());
}

BOOST_FIXTURE_TEST_CASE(transform_point, TranslationTest)
{
  PointD pt_out;
  trf_pointer->transform(ptsIn[0], &pt_out);
  BOOST_CHECK_EQUAL(ptsOut[0].x, pt_out.x);
  BOOST_CHECK_EQUAL(ptsOut[0].y, pt_out.y);
  pt_out = trf_pointer->transform(ptsIn[0], transform_order::direct);
  BOOST_CHECK_EQUAL(ptsOut[0].x, pt_out.x);
  BOOST_CHECK_EQUAL(ptsOut[0].y, pt_out.y);
}

BOOST_FIXTURE_TEST_CASE(transform_point_list, TranslationTest)
{
  std::vector<PointD> pts_out;
  trf_pointer->transform(ptsIn, &pts_out);

  for (size_t i = 0; i < ptsOut.size(); i++) {
    BOOST_CHECK_EQUAL(ptsOut[i].x, pts_out[i].x);
    BOOST_CHECK_EQUAL(ptsOut[i].y, pts_out[i].y);
  }
}

BOOST_FIXTURE_TEST_CASE(std_transform_point_list, TranslationTest)
{
  std::vector<PointD> pts_out2(ptsIn.size());
  std::transform(ptsIn.begin(), ptsIn.end(), pts_out2.begin(), *trf_pointer);

  for (size_t i = 0; i < ptsOut.size(); i++) {
    BOOST_CHECK_EQUAL(ptsOut[i].x, pts_out2[i].x);
    BOOST_CHECK_EQUAL(ptsOut[i].y, pts_out2[i].y);
  }

  // Alternativa sin reservar tamaño en pts_out2:
  std::vector<PointD> pts_out3;
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
//  std::vector<PointD> ptsIn2{
//    PointD(4157222.543, 664789.307),
//    PointD(4149043.336, 688836.443),
//    PointD(4172803.511, 690340.078),
//    PointD(4177148.376, 642997.635)};
//
//  transform_status status = trf.compute(ptsIn2, ptsOut);
//
//  BOOST_CHECK_EQUAL(transform_status::FAILURE, trf.compute(ptsIn2, ptsOut));
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
  BOOST_CHECK(transform_type::translation == trf.transformType());
}

BOOST_FIXTURE_TEST_CASE(isNumberOfPointsValid, TranslationTest)
{
  BOOST_CHECK(trf_pointer->isNumberOfPointsValid(1));
  BOOST_CHECK(trf_pointer->isNumberOfPointsValid(10));
  BOOST_CHECK(false == trf_pointer->isNumberOfPointsValid(0));
}

BOOST_FIXTURE_TEST_CASE(inverse, TranslationTest)
{
  Translation<PointD> trf_inv = trf_pointer->inverse();
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
  Helmert2D<PointD> trf_h2d = static_cast<Helmert2D<PointD>>(trf);
  BOOST_CHECK_EQUAL(0.0, trf_h2d.rotation());
  BOOST_CHECK_EQUAL(1., trf_h2d.scale());
  BOOST_CHECK_EQUAL(0., trf_h2d.tx);
  BOOST_CHECK_EQUAL(0., trf_h2d.ty);
  BOOST_CHECK_EQUAL(2, trf_h2d.minNumberOfPoints());

  // cast a afín
  Affine<PointD> trf_affine = static_cast<Affine<PointD>>(trf);
  BOOST_CHECK_EQUAL(0.0, trf_affine.rotation());
  BOOST_CHECK_EQUAL(1., trf_affine.scaleX());
  BOOST_CHECK_EQUAL(1., trf_affine.scaleY());
  BOOST_CHECK_EQUAL(0., trf_affine.tx);
  BOOST_CHECK_EQUAL(0., trf_affine.ty);
  BOOST_CHECK_EQUAL(3, trf_affine.minNumberOfPoints());

  //Affine<PointD> *trf_affine_ptr = static_cast<Affine<PointD>*>(trf_pointer);
}

BOOST_AUTO_TEST_SUITE_END()

/// Rotation

BOOST_AUTO_TEST_SUITE(RotationTestSuite)

struct RotationTest
{

  RotationTest()
    : rot2(new Rotation<PointD>(35. * TL_DEG_TO_RAD))
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
      PointD(4157222.543, 664789.307),
      PointD(4149043.336, 688836.443),
      PointD(4172803.511, 690340.078),
      PointD(4177148.376, 642997.635),
      PointD(4137012.190, 671808.029),
      PointD(4146292.729, 666952.887),
      PointD(4138759.902, 702670.738) };

    ptsOut = {
      PointD(3024089.863, 2929048.411),
      PointD(3003596.978, 2944055.271),
      PointD(3022197.724, 2958915.254),
      PointD(3052911.339, 2922626.707),
      PointD(3003508.738, 2923205.629),
      PointD(3013895.705, 2924551.628),
      PointD(2987238.257, 2949489.327) };
  }

  void teardown() 
  {
  }

  Rotation<PointD> rot;
  Rotation<PointD> *rot2;
  std::vector<PointD> ptsIn;
  std::vector<PointD> ptsOut;
};

BOOST_FIXTURE_TEST_CASE(default_constructor, RotationTest)
{
  BOOST_CHECK_EQUAL(0., rot.angle());
  BOOST_CHECK_EQUAL(1., rot.minNumberOfPoints());
}

BOOST_FIXTURE_TEST_CASE(constructor, RotationTest)
{

  BOOST_CHECK_CLOSE(35 * TL_DEG_TO_RAD, rot2->angle(), 0.0001);
  BOOST_CHECK_EQUAL(1., rot2->minNumberOfPoints());
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, RotationTest)
{
  Rotation<PointD> copy(*rot2);
  BOOST_CHECK_CLOSE(35 * TL_DEG_TO_RAD, copy.angle(), 0.0001);
  BOOST_CHECK_EQUAL(1., copy.minNumberOfPoints());
}

BOOST_FIXTURE_TEST_CASE(assignement_operator, RotationTest)
{
  Rotation<PointD> assign = *rot2;
  BOOST_CHECK_CLOSE(35 * TL_DEG_TO_RAD, assign.angle(), 0.0001);
  BOOST_CHECK_EQUAL(1., assign.minNumberOfPoints());
}

BOOST_FIXTURE_TEST_CASE(move_constructor, RotationTest)
{
  Rotation<PointD> move(Rotation<PointD>(35. * TL_DEG_TO_RAD));
  BOOST_CHECK_CLOSE(35 * TL_DEG_TO_RAD, move.angle(), 0.0001);
  BOOST_CHECK_EQUAL(1., move.minNumberOfPoints());
}

BOOST_FIXTURE_TEST_CASE(move_operator, RotationTest)
{
  Rotation<PointD> move_assign = Rotation<PointD>(35. * TL_DEG_TO_RAD);
  BOOST_CHECK_CLOSE(35 * TL_DEG_TO_RAD, move_assign.angle(), 0.0001);
  BOOST_CHECK_EQUAL(1., move_assign.minNumberOfPoints());
}

BOOST_FIXTURE_TEST_CASE(setAngle, RotationTest)
{
  rot.setAngle(61. * TL_DEG_TO_RAD);
  BOOST_CHECK_CLOSE(61. * TL_DEG_TO_RAD, rot.angle(), 0.0001);
}

BOOST_FIXTURE_TEST_CASE(transform_point, RotationTest)
{
  PointD pt_out;
  rot2->transform(ptsIn[0], &pt_out);
  BOOST_CHECK_CLOSE(ptsOut[0].x, pt_out.x, 0.1);
  BOOST_CHECK_CLOSE(ptsOut[0].y, pt_out.y, 0.1);
  pt_out = rot2->transform(ptsIn[0], transform_order::direct);
  BOOST_CHECK_CLOSE(ptsOut[0].x, pt_out.x, 0.1);
  BOOST_CHECK_CLOSE(ptsOut[0].y, pt_out.y, 0.1);
}

BOOST_FIXTURE_TEST_CASE(transform_point_list, RotationTest)
{
  std::vector<PointD> pts_out;
  rot2->transform(ptsIn, &pts_out);

  for (size_t i = 0; i < ptsOut.size(); i++) {
    BOOST_CHECK_CLOSE(pts_out[i].x, ptsOut[i].x, 0.1);
    BOOST_CHECK_CLOSE(pts_out[i].y, ptsOut[i].y, 0.1);
  }
}

BOOST_FIXTURE_TEST_CASE(std_transform_point_list, RotationTest)
{
  std::vector<PointD> pts_out2(ptsIn.size());
  std::transform(ptsIn.begin(), ptsIn.end(), pts_out2.begin(), *rot2);

  for (size_t i = 0; i < ptsOut.size(); i++) {
    BOOST_CHECK_CLOSE(ptsOut[i].x, pts_out2[i].x, 0.1);
    BOOST_CHECK_CLOSE(ptsOut[i].y, pts_out2[i].y, 0.1);
  }

  // Alternativa sin reservar tamaño en pts_out2:
  std::vector<PointD> pts_out3;
  std::transform(ptsIn.begin(), ptsIn.end(), std::back_inserter(pts_out3), *rot2);


  for (size_t i = 0; i < ptsOut.size(); i++) {
    BOOST_CHECK_CLOSE(ptsOut[i].x, pts_out3[i].x, 0.1);
    BOOST_CHECK_CLOSE(ptsOut[i].y, pts_out3[i].y, 0.1);
  }
}

BOOST_FIXTURE_TEST_CASE(compute, RotationTest)
{
  rot.compute(ptsIn, ptsOut);
  BOOST_CHECK_CLOSE(35. * TL_DEG_TO_RAD, rot.angle(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(transformType, RotationTest)
{
  BOOST_CHECK(transform_type::rotation == rot.transformType());
}

BOOST_FIXTURE_TEST_CASE(isNumberOfPointsValid, RotationTest)
{
  BOOST_CHECK(rot2->isNumberOfPointsValid(1));
  BOOST_CHECK(rot2->isNumberOfPointsValid(10));
  BOOST_CHECK(false == rot2->isNumberOfPointsValid(0));
}

BOOST_FIXTURE_TEST_CASE(inverse, RotationTest)
{
  Rotation<PointD> rot_inv = rot2->inverse();
  BOOST_CHECK_CLOSE(-35 * TL_DEG_TO_RAD, rot_inv.angle(), 0.0001);
}

BOOST_FIXTURE_TEST_CASE(casting, RotationTest)
{
  // cast a Rotation float
  Rotation<Point<float>> rot_float = static_cast<Rotation<Point<float>>>(*rot2);
  BOOST_CHECK_CLOSE(35 * TL_DEG_TO_RAD, rot_float.angle(), 0.0001);

  // cast a Helmert2D
  Helmert2D<PointD> trf_h2d = static_cast<Helmert2D<PointD>>(*rot2);
  BOOST_CHECK_CLOSE(35 * TL_DEG_TO_RAD, trf_h2d.rotation(), 0.0001);
  BOOST_CHECK_EQUAL(1., trf_h2d.scale());
  BOOST_CHECK_EQUAL(0., trf_h2d.tx);
  BOOST_CHECK_EQUAL(0., trf_h2d.ty);
  BOOST_CHECK_EQUAL(2, trf_h2d.minNumberOfPoints());

  // cast a afín
  Affine<PointD> trf_affine = static_cast<Affine<PointD>>(*rot2);
  BOOST_CHECK_CLOSE(35 * TL_DEG_TO_RAD, trf_affine.rotation(), 0.0001);
  BOOST_CHECK_EQUAL(1., trf_affine.scaleX());
  BOOST_CHECK_EQUAL(1., trf_affine.scaleY());
  BOOST_CHECK_EQUAL(0., trf_affine.tx);
  BOOST_CHECK_EQUAL(0., trf_affine.ty);
  BOOST_CHECK_EQUAL(3, trf_affine.minNumberOfPoints());

  //Affine<PointD> *trf_affine_ptr = static_cast<Affine<PointD>*>(trf_pointer);
}

BOOST_AUTO_TEST_SUITE_END()


/// Scaling

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
  trf_pointer->transform(ptsIn[0], &pt_out);
  BOOST_CHECK_CLOSE(ptsOut[0].x, pt_out.x, 0.1);
  BOOST_CHECK_CLOSE(ptsOut[0].y, pt_out.y, 0.1);
  pt_out = trf_pointer->transform(ptsIn[0], transform_order::direct);
  BOOST_CHECK_CLOSE(ptsOut[0].x, pt_out.x, 0.1);
  BOOST_CHECK_CLOSE(ptsOut[0].y, pt_out.y, 0.1);
}

BOOST_FIXTURE_TEST_CASE(transform_point_list, ScalingTest)
{
  std::vector<PointD> pts_out;
  trf_pointer->transform(ptsIn, &pts_out);

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
//  trf.compute(ptsIn, ptsOut);
//  BOOST_CHECK_CLOSE(0.25, trf.scale(), 0.01);
}

BOOST_FIXTURE_TEST_CASE(transformType, ScalingTest)
{
  BOOST_CHECK(transform_type::scaling == trf.transformType());
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

/// Helmert 2D

BOOST_AUTO_TEST_SUITE(Helmert2DTestSuite)

struct Helmert2DTest
{
  Helmert2DTest()
    : trf_pointer(new Helmert2D<PointD>(150.0, 75.0, 0.25, 35 * TL_DEG_TO_RAD))
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
      PointD(4157222.543, 664789.307),
      PointD(4149043.336, 688836.443),
      PointD(4172803.511, 690340.078),
      PointD(4177148.376, 642997.635),
      PointD(4137012.190, 671808.029),
      PointD(4146292.729, 666952.887),
      PointD(4138759.902, 702670.738) };

    ptsOut = {
      PointD(756172.466,	732337.103),
      PointD(751049.245,	736088.818),
      PointD(755699.431,	739803.813),
      PointD(763377.835,	730731.677),
      PointD(751027.184,  730876.407),
      PointD(753623.926,	731212.907),
      PointD(746959.564,	737447.332) };

  }

  void teardown() 
  {
  }

  std::vector<PointD> ptsIn;
  std::vector<PointD> ptsOut;
  Helmert2D<PointD> trf;
  Helmert2D<PointD> *trf_pointer;
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
  Helmert2D<PointD> copy(*trf_pointer);
  BOOST_CHECK_CLOSE( 35 * TL_DEG_TO_RAD, copy.rotation(), 0.0001);
  BOOST_CHECK_EQUAL(0.25, copy.scale());
  BOOST_CHECK_EQUAL(150.0, copy.tx);
  BOOST_CHECK_EQUAL(75.0, copy.ty);
  BOOST_CHECK_EQUAL(2, copy.minNumberOfPoints());
}

BOOST_FIXTURE_TEST_CASE(assignement_operator, Helmert2DTest)
{
  Helmert2D<PointD> assig = *trf_pointer;
  BOOST_CHECK_CLOSE( 35 * TL_DEG_TO_RAD, assig.rotation(), 0.0001);
  BOOST_CHECK_EQUAL(0.25, assig.scale());
  BOOST_CHECK_EQUAL(150.0, assig.tx);
  BOOST_CHECK_EQUAL(75.0, assig.ty);
  BOOST_CHECK_EQUAL(2, assig.minNumberOfPoints());
}

BOOST_FIXTURE_TEST_CASE(move_constructor, Helmert2DTest)
{
  Helmert2D<PointD> move(Helmert2D<PointD>(150.0, 75.0, 0.25, 35 * TL_DEG_TO_RAD));
  BOOST_CHECK_CLOSE( 35 * TL_DEG_TO_RAD, move.rotation(), 0.0001);
  BOOST_CHECK_EQUAL(0.25, move.scale());
  BOOST_CHECK_EQUAL(150.0, move.tx);
  BOOST_CHECK_EQUAL(75.0, move.ty);
  BOOST_CHECK_EQUAL(2, move.minNumberOfPoints());
}

BOOST_FIXTURE_TEST_CASE(move_operator, Helmert2DTest)
{
  Helmert2D<PointD> move_assig = Helmert2D<PointD>(150.0, 75.0, 0.25, 35 * TL_DEG_TO_RAD);
  BOOST_CHECK_CLOSE( 35 * TL_DEG_TO_RAD, move_assig.rotation(), 0.0001);
  BOOST_CHECK_EQUAL(0.25, move_assig.scale());
  BOOST_CHECK_EQUAL(150.0, move_assig.tx);
  BOOST_CHECK_EQUAL(75.0, move_assig.ty);
  BOOST_CHECK_EQUAL(2, move_assig.minNumberOfPoints());
}

BOOST_FIXTURE_TEST_CASE(transform_point, Helmert2DTest)
{
  PointD pt_out;
  trf_pointer->transform(ptsIn[0], &pt_out);
  BOOST_CHECK_CLOSE(ptsOut[0].x, pt_out.x, 0.1);
  BOOST_CHECK_CLOSE(ptsOut[0].y, pt_out.y, 0.1);
  pt_out = trf_pointer->transform(ptsIn[0], transform_order::direct);
  BOOST_CHECK_CLOSE(ptsOut[0].x, pt_out.x, 0.1);
  BOOST_CHECK_CLOSE(ptsOut[0].y, pt_out.y, 0.1);
}

BOOST_FIXTURE_TEST_CASE(transform_point_list, Helmert2DTest)
{
  std::vector<PointD> pts_out;
  trf_pointer->transform(ptsIn, &pts_out);

  for (size_t i = 0; i < ptsOut.size(); i++) {
    BOOST_CHECK_CLOSE(pts_out[i].x, ptsOut[i].x, 0.1);
    BOOST_CHECK_CLOSE(pts_out[i].y, ptsOut[i].y, 0.1);
  }
}

BOOST_FIXTURE_TEST_CASE(std_transform, Helmert2DTest)
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

BOOST_FIXTURE_TEST_CASE(compute, Helmert2DTest)
{
  trf.compute(ptsIn, ptsOut);
  BOOST_CHECK_CLOSE(35 * TL_DEG_TO_RAD, trf.rotation(), 0.01);
  BOOST_CHECK_CLOSE(0.25, trf.scale(), 0.01);
  BOOST_CHECK_CLOSE(150.0, trf.tx, 0.1);
  BOOST_CHECK_CLOSE(75.0, trf.ty, 0.1);
}

BOOST_FIXTURE_TEST_CASE(transformType, Helmert2DTest)
{
  BOOST_CHECK(transform_type::helmert_2d == trf.transformType());
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
  BOOST_CHECK_CLOSE( 35 * TL_DEG_TO_RAD, trf_pointer->rotation(), 0.0001);
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
  trf_pointer->setRotation(47 * TL_DEG_TO_RAD);
  BOOST_CHECK_EQUAL( 47 * TL_DEG_TO_RAD, trf_pointer->rotation());
}

BOOST_FIXTURE_TEST_CASE(setScale, Helmert2DTest)
{
  trf_pointer->setScale(0.64);
  BOOST_CHECK_EQUAL( 0.64, trf_pointer->scale());

}

BOOST_FIXTURE_TEST_CASE(inverse, Helmert2DTest)
{
  Helmert2D<PointD> trf_inv = trf_pointer->inverse();
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
  Affine<PointD> trf_affine = static_cast<Affine<PointD>>(*trf_pointer);
  BOOST_CHECK_EQUAL(35 * TL_DEG_TO_RAD, trf_affine.rotation());
  BOOST_CHECK_EQUAL(0.25, trf_affine.scaleX());
  BOOST_CHECK_EQUAL(0.25, trf_affine.scaleY());
  BOOST_CHECK_EQUAL(150.0, trf_affine.tx);
  BOOST_CHECK_EQUAL(75.0, trf_affine.ty);
  BOOST_CHECK_EQUAL(3, trf_affine.minNumberOfPoints());

  Translation<PointD> translation = static_cast<Translation<PointD>>(*trf_pointer);
  BOOST_CHECK_EQUAL(150.0, translation.tx);
  BOOST_CHECK_EQUAL(75.0, translation.ty);

  Rotation<PointD> rotation = static_cast<Rotation<PointD>>(*trf_pointer);
  BOOST_CHECK_EQUAL(35 * TL_DEG_TO_RAD, rotation.angle());
}

BOOST_AUTO_TEST_SUITE_END()


/// Affine

BOOST_AUTO_TEST_SUITE(AffineTestSuite)

struct AffineTest
{
  AffineTest()
    : trf_pointer(new Affine<PointD>(150.0, 75.0, 0.25, 0.30, TL_PI / 180 * 35))
  { }
    
  ~AffineTest()
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
      PointD(737107.092,	759565.279),
      PointD(731294.227,	764301.907),
      PointD(735901.291,	768078.488),
      PointD(744937.420,	757067.318),
      PointD(731760.522,  758392.053),
      PointD(734496.503,	758529.698),
      PointD(726807.795,	766227.040) };
  }

  void teardown()
  {
  }

  std::vector<PointD> ptsIn;
  std::vector<PointD> ptsOut;
  Affine<PointD> trf;
  Affine<PointD> *trf_pointer;
};

BOOST_FIXTURE_TEST_CASE(default_constructor, AffineTest)
{
  BOOST_CHECK(transform_type::affine == trf.transformType());
  BOOST_CHECK_EQUAL(0.0, trf.rotation());
  BOOST_CHECK_EQUAL(1., trf.scaleX());
  BOOST_CHECK_EQUAL(1., trf.scaleY());
  BOOST_CHECK_EQUAL(0., trf.tx);
  BOOST_CHECK_EQUAL(0., trf.ty);
  BOOST_CHECK_EQUAL(3, trf.minNumberOfPoints());
}

BOOST_FIXTURE_TEST_CASE(constructor, AffineTest)
{
  BOOST_CHECK(transform_type::affine == trf_pointer->transformType());
  BOOST_CHECK_EQUAL(TL_PI / 180 * 35, trf_pointer->rotation());
  BOOST_CHECK_EQUAL(0.25, trf_pointer->scaleX());
  BOOST_CHECK_EQUAL(0.30, trf_pointer->scaleY());
  BOOST_CHECK_EQUAL(150.0, trf_pointer->tx);
  BOOST_CHECK_EQUAL(75.0, trf_pointer->ty);
  BOOST_CHECK_EQUAL(3, trf_pointer->minNumberOfPoints());
}

BOOST_FIXTURE_TEST_CASE(assignement_operator, AffineTest)
{
  Affine<PointD> assig = *trf_pointer;
  BOOST_CHECK_EQUAL(TL_PI / 180 * 35, assig.rotation());
  BOOST_CHECK_EQUAL(0.25, assig.scaleX());
  BOOST_CHECK_EQUAL(0.30, assig.scaleY());
  BOOST_CHECK_EQUAL(150.0, assig.tx);
  BOOST_CHECK_EQUAL(75.0, assig.ty);
  BOOST_CHECK_EQUAL(3, assig.minNumberOfPoints());
}

BOOST_FIXTURE_TEST_CASE(move_constructor, AffineTest)
{
  Affine<PointD> move(Affine<PointD>(150.0, 75.0, 0.25, 0.30, TL_PI / 180 * 35));
  BOOST_CHECK_EQUAL(TL_PI / 180 * 35, move.rotation());
  BOOST_CHECK_EQUAL(0.25, move.scaleX());
  BOOST_CHECK_EQUAL(0.30, move.scaleY());
  BOOST_CHECK_EQUAL(150.0, move.tx);
  BOOST_CHECK_EQUAL(75.0, move.ty);
  BOOST_CHECK_EQUAL(3, move.minNumberOfPoints());
}

BOOST_FIXTURE_TEST_CASE(move_operator, AffineTest)
{
  Affine<PointD> move_assig = Affine<PointD>(150.0, 75.0, 0.25, 0.30, 35 * TL_DEG_TO_RAD);
  BOOST_CHECK_EQUAL(TL_PI / 180 * 35, move_assig.rotation());
  BOOST_CHECK_EQUAL(0.25, move_assig.scaleX());
  BOOST_CHECK_EQUAL(0.30, move_assig.scaleY());
  BOOST_CHECK_EQUAL(150.0, move_assig.tx);
  BOOST_CHECK_EQUAL(75.0, move_assig.ty);
  BOOST_CHECK_EQUAL(3, move_assig.minNumberOfPoints());
}

BOOST_FIXTURE_TEST_CASE(transform, AffineTest)
{
  std::vector<PointD> pts_out;
  trf_pointer->transform(ptsIn, &pts_out);

  for (size_t i = 0; i < ptsOut.size(); i++) {
    BOOST_CHECK_CLOSE(pts_out[i].x, ptsOut[i].x, 0.1);
    BOOST_CHECK_CLOSE(pts_out[i].y, ptsOut[i].y, 0.1);
  }
}

BOOST_FIXTURE_TEST_CASE(compute, AffineTest)
{
  trf.compute(ptsIn, ptsOut);
  BOOST_CHECK_CLOSE( 35 * TL_DEG_TO_RAD, trf.rotation(), 0.1);
  BOOST_CHECK_CLOSE(0.25, trf.scaleX(), 0.1);
  BOOST_CHECK_CLOSE(0.30, trf.scaleY(), 0.1);
  BOOST_CHECK_CLOSE(150.0, trf.tx, 0.1);
  BOOST_CHECK_CLOSE(75.0, trf.ty, 0.1);
}

BOOST_FIXTURE_TEST_CASE(transformType, AffineTest)
{
  BOOST_CHECK(transform_type::affine == trf.transformType());
}

BOOST_FIXTURE_TEST_CASE(isNumberOfPointsValid, AffineTest)
{
  BOOST_CHECK(trf_pointer->isNumberOfPointsValid(3));
  BOOST_CHECK(trf_pointer->isNumberOfPointsValid(10));
  BOOST_CHECK(false == trf_pointer->isNumberOfPointsValid(0));
  BOOST_CHECK(false == trf_pointer->isNumberOfPointsValid(2));
}

BOOST_FIXTURE_TEST_CASE(rotation, AffineTest)
{
  BOOST_CHECK_CLOSE( 35 * TL_DEG_TO_RAD, trf_pointer->rotation(), 0.0001);
}

BOOST_FIXTURE_TEST_CASE(scaleX, AffineTest)
{
  BOOST_CHECK_EQUAL(0.25, trf_pointer->scaleX());
}

BOOST_FIXTURE_TEST_CASE(scaleY, AffineTest)
{
  BOOST_CHECK_EQUAL(0.3, trf_pointer->scaleY());
}

BOOST_FIXTURE_TEST_CASE(setParameters, AffineTest)
{

  trf_pointer->setParameters(200., 175., 1.25, 1.5, 45 * TL_DEG_TO_RAD);
  BOOST_CHECK_EQUAL(45 * TL_DEG_TO_RAD, trf_pointer->rotation());
  BOOST_CHECK_EQUAL(1.25, trf_pointer->scaleX());
  BOOST_CHECK_EQUAL(1.5, trf_pointer->scaleY());
  BOOST_CHECK_EQUAL(200., trf_pointer->tx);
  BOOST_CHECK_EQUAL(175., trf_pointer->ty);
}

BOOST_FIXTURE_TEST_CASE(setRotation, AffineTest)
{
  trf_pointer->setRotation(47 * TL_DEG_TO_RAD);
  BOOST_CHECK_EQUAL( 47 * TL_DEG_TO_RAD, trf_pointer->rotation());
}

BOOST_FIXTURE_TEST_CASE(setScaleX, AffineTest)
{
  trf_pointer->setScaleX(0.64);
  BOOST_CHECK_EQUAL( 0.64, trf_pointer->scaleX());
}

BOOST_FIXTURE_TEST_CASE(setScaleY, AffineTest)
{
  trf_pointer->setScaleY(0.64);
  BOOST_CHECK_EQUAL( 0.64, trf_pointer->scaleY());
}

BOOST_AUTO_TEST_SUITE_END()


//TEST(Projective, default_constructor)
//{
//  Projective<PointD> trf;
//  BOOST_CHECK_EQUAL(transform_type::PROJECTIVE, trf.getTransformType());
//  BOOST_CHECK_EQUAL(4,trf.minNumberOfPoints());
//}
//
//TEST(isNumberOfPointsValid, AllTrfTest)
//{
//  Translate<PointD> trfTranslate;
//  Rotation<PointD> trfRotate;
//  Helmert2D<PointD> trfHelmert2D;
//  Affine<PointD> trfAffine;
//  Projective<PointD> trfProjective;
//
//  BOOST_CHECK(trfTranslate.isNumberOfPointsValid(1));
//  BOOST_CHECK(trfRotate.isNumberOfPointsValid(1));
//  BOOST_CHECK(trfHelmert2D.isNumberOfPointsValid(2));
//  BOOST_CHECK(trfAffine.isNumberOfPointsValid(3));
//  BOOST_CHECK(trfProjective.isNumberOfPointsValid(4));
//
//  BOOST_CHECK(false == trfTranslate.isNumberOfPointsValid(0));
//  BOOST_CHECK(false == trfRotate.isNumberOfPointsValid(0));
//  BOOST_CHECK(false == trfHelmert2D.isNumberOfPointsValid(1));
//  BOOST_CHECK(false == trfAffine.isNumberOfPointsValid(2));
//  BOOST_CHECK(false == trfProjective.isNumberOfPointsValid(2));
//}
//
//
//TEST(getTransformType, AllTrfTest)
//{
//  Translate<PointD> trfTranslate;
//  Rotation<PointD> trfRotate;
//  Helmert2D<PointD> trfHelmert2D;
//  Affine<PointD> trfAffine;
//  Projective<PointD> trfProjective;
//
//  BOOST_CHECK_EQUAL(transform_type::TRANSLATE, trfTranslate.getTransformType());
//  BOOST_CHECK_EQUAL(transform_type::ROTATION, trfRotate.getTransformType());
//  BOOST_CHECK_EQUAL(transform_type::HELMERT_2D, trfHelmert2D.getTransformType());
//  BOOST_CHECK_EQUAL(transform_type::AFFINE, trfAffine.getTransformType());
//  BOOST_CHECK_EQUAL(transform_type::PROJECTIVE, trfProjective.getTransformType());
//
//  Transform<PointD> *trf = &trfTranslate;
//  BOOST_CHECK_EQUAL(transform_type::TRANSLATE, trf->getTransformType());
//  trf = &trfRotate;
//  BOOST_CHECK_EQUAL(transform_type::ROTATION, trfRotate.getTransformType());
//  trf = &trfHelmert2D;
//  BOOST_CHECK_EQUAL(transform_type::HELMERT_2D, trfHelmert2D.getTransformType());
//  trf = &trfAffine;
//  BOOST_CHECK_EQUAL(transform_type::AFFINE, trfAffine.getTransformType());
//  trf = &trfProjective;
//  BOOST_CHECK_EQUAL(transform_type::PROJECTIVE, trfProjective.getTransformType());
//}
//
//// Pendiente test de transformación de entidades
//TEST(transform, Entities)
//{
//  WindowI wi(PointI(2, 3), PointI(100, 369));
//  WindowD wd(PointD(23.3, 234.6), PointD(100., 369.));
//  WindowF wf(PointF(31.f, 52.36f), PointF(100.34f, 369.36f));
//  WindowI wi_out;
//  Translate<PointI> trf(150.0, 75.0);
//  transform(wi, &wi_out, &trf);
//  BOOST_CHECK_EQUAL(152, wi_out.pt1.x);
//  BOOST_CHECK_EQUAL(78, wi_out.pt1.y);
//  BOOST_CHECK_EQUAL(250, wi_out.pt2.x);
//  BOOST_CHECK_EQUAL(444, wi_out.pt2.y);
//}
//
//// Pendiente test de transformación de imagenes
//// Pendiente test transformación multiple
//TEST(TrfMultiple, default_constructor)
//{
//  TrfMultiple<PointD> trfMul;
//  trfMul.add(std::make_shared<Translate<PointD>>(150.0, 75.0));
//  trfMul.add(std::make_shared<Rotation<PointD>>(45. * TL_DEG_TO_RAD));
//
//  BOOST_CHECK_EQUAL(transform_type::MULTIPLE, trfMul.getTransformType());
//
//}
//
//TEST(TrfMultiple, constructor)
//{
//  TrfMultiple<PointD> trf{
//    std::make_shared<Translate<PointD>>(150.0, 75.0),
//    std::make_shared<Rotation<PointD>>(45. * TL_DEG_TO_RAD)
//  };
//
//
//}
