#define BOOST_TEST_MODULE Tidop Affine test
#include <boost/test/unit_test.hpp>

#include <tidop/geometry/transform/affine.h>
#include <tidop/geometry/entities/point.h>

using namespace tl;


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
  BOOST_CHECK(Transform::Type::affine == trf.transformType());
  BOOST_CHECK_EQUAL(0.0, trf.rotation());
  BOOST_CHECK_EQUAL(1., trf.scaleX());
  BOOST_CHECK_EQUAL(1., trf.scaleY());
  BOOST_CHECK_EQUAL(0., trf.tx);
  BOOST_CHECK_EQUAL(0., trf.ty);
  BOOST_CHECK_EQUAL(3, trf.minNumberOfPoints());
}

BOOST_FIXTURE_TEST_CASE(constructor, AffineTest)
{
  BOOST_CHECK(Transform::Type::affine == trf_pointer->transformType());
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
  trf_pointer->transform(ptsIn, pts_out);

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
  BOOST_CHECK(Transform::Type::affine == trf.transformType());
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
