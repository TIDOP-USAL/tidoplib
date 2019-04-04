#include <gtest/gtest.h>
#include <tidop/geometry/transform.h>
#include <tidop/core/utils.h>
#include <tidop/core/exception.h>
#include <tidop/math/algebra/vector.h>

using namespace tl;
using namespace geometry;


class TranslationTest
  : public testing::Test
{

protected:

  virtual void SetUp() override
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

    trf_pointer = new Translation<PointD>(150.0, 75.0);
  }

  virtual void TearDown() override
  {
    if (trf_pointer){
      delete trf_pointer;
      trf_pointer = nullptr;
    }
  }

  std::vector<PointD> ptsIn;
  std::vector<PointD> ptsOut;
  std::vector<math::Vector2d> vectorIn;
  std::vector<math::Vector2d> vectorOut;
  Translation<PointD> trf;
  Translation<PointD> *trf_pointer;
  //Translation<math::Vector2d> trf_vector;
};

TEST_F(TranslationTest, DefaultConstructor)
{
  EXPECT_EQ(0., trf.tx);
  EXPECT_EQ(0., trf.ty);
  EXPECT_EQ(1., trf.minNumberOfPoints());
}

TEST_F(TranslationTest, Constructor)
{
  EXPECT_EQ(150.0, trf_pointer->tx);
  EXPECT_EQ(75.0, trf_pointer->ty);
  EXPECT_EQ(1, trf_pointer->minNumberOfPoints());
}

TEST_F(TranslationTest, Transform)
{

  // Convierte un punto
  PointD pt_out;
  trf_pointer->transform(ptsIn[0], &pt_out);
  EXPECT_DOUBLE_EQ(ptsOut[0].x, pt_out.x);
  EXPECT_DOUBLE_EQ(ptsOut[0].y, pt_out.y);
  pt_out = trf_pointer->transform(ptsIn[0], transform_order::direct);
  EXPECT_DOUBLE_EQ(ptsOut[0].x, pt_out.x);
  EXPECT_DOUBLE_EQ(ptsOut[0].y, pt_out.y);

  // convierte un listado de puntos
  std::vector<PointD> pts_out;
  trf_pointer->transform(ptsIn, &pts_out);

  for (size_t i = 0; i < ptsOut.size(); i++) {
    EXPECT_DOUBLE_EQ(ptsOut[i].x, pts_out[i].x);
    EXPECT_DOUBLE_EQ(ptsOut[i].y, pts_out[i].y);
  }

  // convierte un listado de puntos mediante std::transform
  std::vector<PointD> pts_out2(ptsIn.size());
  std::transform(ptsIn.begin(), ptsIn.end(), pts_out2.begin(), *trf_pointer);

  for (size_t i = 0; i < ptsOut.size(); i++) {
    EXPECT_DOUBLE_EQ(ptsOut[i].x, pts_out2[i].x);
    EXPECT_DOUBLE_EQ(ptsOut[i].y, pts_out2[i].y);
  }

  // Alternativa sin reservar tamaño en pts_out2:
  std::vector<PointD> pts_out3;
  std::transform(ptsIn.begin(), ptsIn.end(), std::back_inserter(pts_out3), *trf_pointer);

  for (size_t i = 0; i < ptsOut.size(); i++) {
    EXPECT_DOUBLE_EQ(ptsOut[i].x, pts_out3[i].x);
    EXPECT_DOUBLE_EQ(ptsOut[i].y, pts_out3[i].y);
  }
}

TEST_F(TranslationTest, Compute)
{

  trf_pointer->compute(ptsIn, ptsOut);
  EXPECT_NEAR(150.0, trf_pointer->tx, 0.1);
  EXPECT_NEAR(75.0,  trf_pointer->ty, 0.1);

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
//  EXPECT_EQ(transform_status::FAILURE, trf.compute(ptsIn2, ptsOut));
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

TEST_F(TranslationTest, transformType)
{
  EXPECT_EQ(transform_type::translation, trf.transformType());
}

TEST_F(TranslationTest, isNumberOfPointsValid)
{
  EXPECT_TRUE(trf_pointer->isNumberOfPointsValid(1));
  EXPECT_TRUE(trf_pointer->isNumberOfPointsValid(10));
  EXPECT_FALSE(trf_pointer->isNumberOfPointsValid(0));
}

TEST_F(TranslationTest, casting)
{
  //// cast a Translation float
  //Translation<Point<float>> trf_float = static_cast<Translation<Point<float>>>(trf);
  //EXPECT_EQ(0.f, trf_float.tx);
  //EXPECT_EQ(0.f, trf_float.ty);

  //// cast a Helmert2D
  //Helmert2D<PointD> trf_h2d = static_cast<Helmert2D<PointD>>(trf);
  //EXPECT_EQ(0.0, trf_h2d.rotation());
  //EXPECT_EQ(1., trf_h2d.scale());
  //EXPECT_EQ(0., trf_h2d.tx);
  //EXPECT_EQ(0., trf_h2d.ty);
  //EXPECT_EQ(2, trf_h2d.minNumberOfPoints());

  //// cast a afín
  //Affine<PointD> trf_affine = static_cast<Affine<PointD>>(trf);
  //EXPECT_EQ(0.0, trf_affine.rotation());
  //EXPECT_EQ(1., trf_affine.scaleX());
  //EXPECT_EQ(1., trf_affine.scaleY());
  //EXPECT_EQ(0., trf_affine.tx);
  //EXPECT_EQ(0., trf_affine.ty);
  //EXPECT_EQ(3, trf_affine.minNumberOfPoints());

  //Affine<PointD> *trf_affine_ptr = static_cast<Affine<PointD>*>(trf_pointer);
}


/// Rotation

class RotationTest
  : public testing::Test
{

protected:

  virtual void SetUp() override
  {
    rot2 = new Rotation<PointD>(35. * TL_DEG_TO_RAD);

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

  virtual void TearDown() override
  {
    if (rot2){
      delete rot2;
      rot2 = nullptr;
    }
  }

  Rotation<PointD> rot;
  Rotation<PointD> *rot2;
  std::vector<PointD> ptsIn;
  std::vector<PointD> ptsOut;
};

TEST_F(RotationTest, DefaultConstructor)
{
  EXPECT_EQ(0., rot.angle());
  EXPECT_EQ(1., rot.minNumberOfPoints());
}

TEST_F(RotationTest, Constructor)
{

  EXPECT_NEAR(35 * TL_DEG_TO_RAD, rot2->angle(), 0.0001);
  EXPECT_EQ(1., rot2->minNumberOfPoints());
}

TEST_F(RotationTest, setAngle)
{
  rot.setAngle(61. * TL_DEG_TO_RAD);
  EXPECT_NEAR(61. * TL_DEG_TO_RAD, rot.angle(), 0.0001);
}

TEST_F(RotationTest, Transform)
{
  // Convierte un punto
  PointD pt_out;
  rot2->transform(ptsIn[0], &pt_out);
  EXPECT_NEAR(ptsOut[0].x, pt_out.x, 0.001);
  EXPECT_NEAR(ptsOut[0].y, pt_out.y, 0.001);
  pt_out = rot2->transform(ptsIn[0], transform_order::direct);
  EXPECT_NEAR(ptsOut[0].x, pt_out.x, 0.001);
  EXPECT_NEAR(ptsOut[0].y, pt_out.y, 0.001);

  // convierte un listado de puntos
  std::vector<PointD> pts_out;
  rot2->transform(ptsIn, &pts_out);

  for (size_t i = 0; i < ptsOut.size(); i++) {
    EXPECT_NEAR(pts_out[i].x, ptsOut[i].x, 0.001);
    EXPECT_NEAR(pts_out[i].y, ptsOut[i].y, 0.001);
  }

  // convierte un listado de puntos mediante std::transform
  std::vector<PointD> pts_out2(ptsIn.size());
  std::transform(ptsIn.begin(), ptsIn.end(), pts_out2.begin(), *rot2);

  for (size_t i = 0; i < ptsOut.size(); i++) {
    EXPECT_NEAR(ptsOut[i].x, pts_out2[i].x, 0.001);
    EXPECT_NEAR(ptsOut[i].y, pts_out2[i].y, 0.001);
  }

  // Alternativa sin reservar tamaño en pts_out2:
  std::vector<PointD> pts_out3;
  std::transform(ptsIn.begin(), ptsIn.end(), std::back_inserter(pts_out3), *rot2);


  for (size_t i = 0; i < ptsOut.size(); i++) {
    EXPECT_NEAR(ptsOut[i].x, pts_out3[i].x, 0.001);
    EXPECT_NEAR(ptsOut[i].y, pts_out3[i].y, 0.001);
  }
}


TEST_F(RotationTest, Compute)
{
  rot.compute(ptsIn, ptsOut);
  EXPECT_NEAR(35. * TL_DEG_TO_RAD, rot.angle(), 0.1);
}


TEST_F(RotationTest, transformType)
{
  EXPECT_EQ(transform_type::rotation, rot.transformType());
}

TEST_F(RotationTest, isNumberOfPointsValid)
{
  EXPECT_TRUE(rot2->isNumberOfPointsValid(1));
  EXPECT_TRUE(rot2->isNumberOfPointsValid(10));
  EXPECT_FALSE(rot2->isNumberOfPointsValid(0));
}



/// Helmert 2D


class Helmert2DTest
  : public testing::Test
{

protected:

  virtual void SetUp() override
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

    trf_pointer = new Helmert2D<PointD>(150.0, 75.0, 0.25, 35 * TL_DEG_TO_RAD);
  }

  virtual void TearDown() override
  {
    if (trf_pointer){
      delete trf_pointer;
      trf_pointer = nullptr;
    }
  }

  std::vector<PointD> ptsIn;
  std::vector<PointD> ptsOut;
  Helmert2D<PointD> trf;
  Helmert2D<PointD> *trf_pointer;
};

TEST_F(Helmert2DTest, DefaultConstructor)
{
  EXPECT_EQ(0.0, trf.rotation());
  EXPECT_EQ(1., trf.scale());
  EXPECT_EQ(0., trf.tx);
  EXPECT_EQ(0., trf.ty);
  EXPECT_EQ(2, trf.minNumberOfPoints());
}

TEST_F(Helmert2DTest, Constructor)
{

  EXPECT_NEAR( 35 * TL_DEG_TO_RAD, trf_pointer->rotation(), 0.0001);
  EXPECT_EQ(0.25, trf_pointer->scale());
  EXPECT_EQ(150.0, trf_pointer->tx);
  EXPECT_EQ(75.0, trf_pointer->ty);
  EXPECT_EQ(2, trf_pointer->minNumberOfPoints());

}

TEST_F(Helmert2DTest, Transform)
{
  // Convierte un punto
  PointD pt_out;
  trf_pointer->transform(ptsIn[0], &pt_out);
  EXPECT_NEAR(ptsOut[0].x, pt_out.x, 0.001);
  EXPECT_NEAR(ptsOut[0].y, pt_out.y, 0.001);
  pt_out = trf_pointer->transform(ptsIn[0], transform_order::direct);
  EXPECT_NEAR(ptsOut[0].x, pt_out.x, 0.001);
  EXPECT_NEAR(ptsOut[0].y, pt_out.y, 0.001);

  // convierte un listado de puntos
  std::vector<PointD> pts_out;
  trf_pointer->transform(ptsIn, &pts_out);

  for (size_t i = 0; i < ptsOut.size(); i++) {
    EXPECT_NEAR(pts_out[i].x, ptsOut[i].x, 0.001);
    EXPECT_NEAR(pts_out[i].y, ptsOut[i].y, 0.001);
  }

  // convierte un listado de puntos mediante std::transform
  std::vector<PointD> pts_out2(ptsIn.size());
  std::transform(ptsIn.begin(), ptsIn.end(), pts_out2.begin(), *trf_pointer);

  for (size_t i = 0; i < ptsOut.size(); i++) {
    EXPECT_NEAR(ptsOut[i].x, pts_out2[i].x, 0.001);
    EXPECT_NEAR(ptsOut[i].y, pts_out2[i].y, 0.001);
  }

  // Alternativa sin reservar tamaño en pts_out2:
  std::vector<PointD> pts_out3;
  std::transform(ptsIn.begin(), ptsIn.end(), std::back_inserter(pts_out3), *trf_pointer);


  for (size_t i = 0; i < ptsOut.size(); i++) {
    EXPECT_NEAR(ptsOut[i].x, pts_out3[i].x, 0.001);
    EXPECT_NEAR(ptsOut[i].y, pts_out3[i].y, 0.001);
  }
}

TEST_F(Helmert2DTest, Compute)
{
  trf.compute(ptsIn, ptsOut);
  EXPECT_NEAR( 35 * TL_DEG_TO_RAD, trf.rotation(), 0.01);
  EXPECT_NEAR(0.25, trf.scale(), 0.01);
  EXPECT_NEAR(150.0, trf.tx, 0.01);
  EXPECT_NEAR(75.0, trf.ty, 0.01);
}

TEST_F(Helmert2DTest, transformType)
{
  EXPECT_EQ(transform_type::helmert_2d, trf.transformType());
}

TEST_F(Helmert2DTest, isNumberOfPointsValid)
{
  EXPECT_TRUE(trf_pointer->isNumberOfPointsValid(2));
  EXPECT_TRUE(trf_pointer->isNumberOfPointsValid(10));
  EXPECT_FALSE(trf_pointer->isNumberOfPointsValid(0));
  EXPECT_FALSE(trf_pointer->isNumberOfPointsValid(1));
}

TEST_F(Helmert2DTest, rotation)
{
  EXPECT_NEAR( 35 * TL_DEG_TO_RAD, trf_pointer->rotation(), 0.0001);
}

TEST_F(Helmert2DTest, scale)
{
  EXPECT_EQ(0.25, trf_pointer->scale());
}

TEST_F(Helmert2DTest, setParameters)
{

  trf_pointer->setParameters(0.56, 163.25, 2., .131);
  EXPECT_DOUBLE_EQ( .131, trf_pointer->rotation());
  EXPECT_DOUBLE_EQ(2., trf_pointer->scale());
  EXPECT_DOUBLE_EQ(0.56, trf_pointer->tx);
  EXPECT_DOUBLE_EQ(163.25, trf_pointer->ty);
}

TEST_F(Helmert2DTest, setRotation)
{
  trf_pointer->setRotation(47 * TL_DEG_TO_RAD);
  EXPECT_DOUBLE_EQ( 47 * TL_DEG_TO_RAD, trf_pointer->rotation());
}

TEST_F(Helmert2DTest, setScale)
{
  trf_pointer->setScale(0.64);
  EXPECT_DOUBLE_EQ( 0.64, trf_pointer->scale());

}


/// Affine

class AffineTest
  : public testing::Test
{

protected:

  virtual void SetUp() override
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

    trf_pointer = new Affine<PointD>(150.0, 75.0, 0.25, 0.30, TL_PI / 180 * 35);
  }

  virtual void TearDown() override
  {
    if (trf_pointer){
      delete trf_pointer;
      trf_pointer = nullptr;
    }
  }

  std::vector<PointD> ptsIn;
  std::vector<PointD> ptsOut;
  Affine<PointD> trf;
  Affine<PointD> *trf_pointer;
};

TEST_F(AffineTest, DefaultConstructor)
{
  EXPECT_EQ(transform_type::affine, trf.transformType());
  EXPECT_EQ(0.0, trf.rotation());
  EXPECT_EQ(1., trf.scaleX());
  EXPECT_EQ(1., trf.scaleY());
  EXPECT_EQ(0., trf.tx);
  EXPECT_EQ(0., trf.ty);
  EXPECT_EQ(3, trf.minNumberOfPoints());
}

TEST_F(AffineTest, Constructor)
{
  EXPECT_EQ(transform_type::affine, trf_pointer->transformType());
  EXPECT_EQ(3, trf_pointer->minNumberOfPoints());
}

TEST_F(AffineTest, Transform)
{
  std::vector<PointD> pts_out;
  trf_pointer->transform(ptsIn, &pts_out);

  for (size_t i = 0; i < ptsOut.size(); i++) {
    EXPECT_NEAR(pts_out[i].x, ptsOut[i].x, 0.001);
    EXPECT_NEAR(pts_out[i].y, ptsOut[i].y, 0.001);
  }
}

TEST_F(AffineTest, Compute)
{
  trf.compute(ptsIn, ptsOut);
  EXPECT_NEAR( 35 * TL_DEG_TO_RAD, trf.rotation(), 0.1);
  EXPECT_NEAR(0.25, trf.scaleX(), 0.1);
  EXPECT_NEAR(0.25, trf.scaleY(), 0.1);
  EXPECT_NEAR(150.0, trf.tx, 0.1);
  EXPECT_NEAR(75.0, trf.ty, 0.1);
}

TEST_F(AffineTest, transformType)
{
  EXPECT_EQ(transform_type::affine, trf.transformType());
}

TEST_F(AffineTest, isNumberOfPointsValid)
{
  EXPECT_TRUE(trf_pointer->isNumberOfPointsValid(3));
  EXPECT_TRUE(trf_pointer->isNumberOfPointsValid(10));
  EXPECT_FALSE(trf_pointer->isNumberOfPointsValid(0));
  EXPECT_FALSE(trf_pointer->isNumberOfPointsValid(2));
}

TEST_F(AffineTest, rotation)
{
  EXPECT_NEAR( 35 * TL_DEG_TO_RAD, trf_pointer->rotation(), 0.0001);
}

TEST_F(AffineTest, scaleX)
{
  EXPECT_EQ(0.25, trf_pointer->scaleX());
}

TEST_F(AffineTest, scaleY)
{
  EXPECT_EQ(0.3, trf_pointer->scaleY());
}

TEST_F(AffineTest, setParameters)
{

  trf_pointer->setParameters(200., 175., 1.25, 1.5, 45 * TL_DEG_TO_RAD);
  EXPECT_DOUBLE_EQ(45 * TL_DEG_TO_RAD, trf_pointer->rotation());
  EXPECT_DOUBLE_EQ(1.25, trf_pointer->scaleX());
  EXPECT_DOUBLE_EQ(1.5, trf_pointer->scaleY());
  EXPECT_DOUBLE_EQ(200., trf_pointer->tx);
  EXPECT_DOUBLE_EQ(175., trf_pointer->ty);
}

TEST_F(AffineTest, setRotation)
{
  trf_pointer->setRotation(47 * TL_DEG_TO_RAD);
  EXPECT_DOUBLE_EQ( 47 * TL_DEG_TO_RAD, trf_pointer->rotation());
}

TEST_F(AffineTest, setScaleX)
{
  trf_pointer->setScaleX(0.64);
  EXPECT_DOUBLE_EQ( 0.64, trf_pointer->scaleX());
}

TEST_F(AffineTest, setScaleY)
{
  trf_pointer->setScaleY(0.64);
  EXPECT_DOUBLE_EQ( 0.64, trf_pointer->scaleY());
}

//TEST(Projective, DefaultConstructor)
//{
//  Projective<PointD> trf;
//  EXPECT_EQ(transform_type::PROJECTIVE, trf.getTransformType());
//  EXPECT_EQ(4,trf.minNumberOfPoints());
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
//  EXPECT_TRUE(trfTranslate.isNumberOfPointsValid(1));
//  EXPECT_TRUE(trfRotate.isNumberOfPointsValid(1));
//  EXPECT_TRUE(trfHelmert2D.isNumberOfPointsValid(2));
//  EXPECT_TRUE(trfAffine.isNumberOfPointsValid(3));
//  EXPECT_TRUE(trfProjective.isNumberOfPointsValid(4));
//
//  EXPECT_FALSE(trfTranslate.isNumberOfPointsValid(0));
//  EXPECT_FALSE(trfRotate.isNumberOfPointsValid(0));
//  EXPECT_FALSE(trfHelmert2D.isNumberOfPointsValid(1));
//  EXPECT_FALSE(trfAffine.isNumberOfPointsValid(2));
//  EXPECT_FALSE(trfProjective.isNumberOfPointsValid(2));
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
//  EXPECT_EQ(transform_type::TRANSLATE, trfTranslate.getTransformType());
//  EXPECT_EQ(transform_type::ROTATION, trfRotate.getTransformType());
//  EXPECT_EQ(transform_type::HELMERT_2D, trfHelmert2D.getTransformType());
//  EXPECT_EQ(transform_type::AFFINE, trfAffine.getTransformType());
//  EXPECT_EQ(transform_type::PROJECTIVE, trfProjective.getTransformType());
//
//  Transform<PointD> *trf = &trfTranslate;
//  EXPECT_EQ(transform_type::TRANSLATE, trf->getTransformType());
//  trf = &trfRotate;
//  EXPECT_EQ(transform_type::ROTATION, trfRotate.getTransformType());
//  trf = &trfHelmert2D;
//  EXPECT_EQ(transform_type::HELMERT_2D, trfHelmert2D.getTransformType());
//  trf = &trfAffine;
//  EXPECT_EQ(transform_type::AFFINE, trfAffine.getTransformType());
//  trf = &trfProjective;
//  EXPECT_EQ(transform_type::PROJECTIVE, trfProjective.getTransformType());
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
//  EXPECT_EQ(152, wi_out.pt1.x);
//  EXPECT_EQ(78, wi_out.pt1.y);
//  EXPECT_EQ(250, wi_out.pt2.x);
//  EXPECT_EQ(444, wi_out.pt2.y);
//}
//
//// Pendiente test de transformación de imagenes
//// Pendiente test transformación multiple
//TEST(TrfMultiple, DefaultConstructor)
//{
//  TrfMultiple<PointD> trfMul;
//  trfMul.add(std::make_shared<Translate<PointD>>(150.0, 75.0));
//  trfMul.add(std::make_shared<Rotation<PointD>>(45. * TL_DEG_TO_RAD));
//
//  EXPECT_EQ(transform_type::MULTIPLE, trfMul.getTransformType());
//
//}
//
//TEST(TrfMultiple, Constructor)
//{
//  TrfMultiple<PointD> trf{
//    std::make_shared<Translate<PointD>>(150.0, 75.0),
//    std::make_shared<Rotation<PointD>>(45. * TL_DEG_TO_RAD)
//  };
//
//
//}
