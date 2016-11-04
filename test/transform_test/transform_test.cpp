#include <gtest/gtest.h>
#include "..\..\src\transform.h"
#include "..\..\src\core\utils.h"
#include "opencv2/core/core.hpp"

using namespace I3D;

std::vector<cv::Point2d> ptsIn{
    cv::Point2d(4157222.543, 664789.307),
    cv::Point2d(4149043.336, 688836.443),
    cv::Point2d(4172803.511, 690340.078),
    cv::Point2d(4177148.376, 642997.635),
    cv::Point2d(4137012.190, 671808.029),
    cv::Point2d(4146292.729, 666952.887),
    cv::Point2d(4138759.902, 702670.738) };


TEST(Translate, DefaultConstructor)
{
  Translate<cv::Point2d> trf;
  EXPECT_EQ(transform_type::TRANSLATE, trf.getTransformType());
  EXPECT_EQ(0,trf.getTranslationX());
  EXPECT_EQ(0,trf.getTranslationY());
  EXPECT_EQ(1,trf.minNumberOfPoints());
}

TEST(Translate, Constructor)
{
  Translate<cv::Point2d> trf(150.0, 75.0);
  EXPECT_EQ(transform_type::TRANSLATE, trf.getTransformType());
  EXPECT_EQ(150.0,trf.getTranslationX());
  EXPECT_EQ(75.0,trf.getTranslationY());
  EXPECT_EQ(1,trf.minNumberOfPoints());
}

TEST(Translate, Compute)
{
  Translate<cv::Point2d> trf(150.0, 75.0);
  std::vector<cv::Point2d> ptsOut;
  trf.transform(ptsIn, &ptsOut);
  trf.compute(ptsIn,ptsOut);
  EXPECT_NEAR(150.0,trf.getTranslationX(),0.1);
  EXPECT_NEAR(75.0,trf.getTranslationY(),0.1);
}



TEST(Rotation, DefaultConstructor)
{
  Rotation<cv::Point2d> rot;
  EXPECT_EQ(transform_type::ROTATION, rot.getTransformType());
  EXPECT_EQ(0,rot.getAngle());
  EXPECT_EQ(1,rot.minNumberOfPoints());
}

TEST(Rotation, Constructor)
{
  Rotation<cv::Point2d> rot(35 * I3D_PI / 180.);
  EXPECT_EQ(transform_type::ROTATION, rot.getTransformType());
  EXPECT_NEAR(35 * I3D_PI / 180., rot.getAngle(), 0.0001);
  EXPECT_EQ(1,rot.minNumberOfPoints());
}




TEST(Helmert2D, DefaultConstructor)
{
  Helmert2D<cv::Point2d> trf;
  EXPECT_EQ(transform_type::HELMERT_2D, trf.getTransformType());
  EXPECT_EQ(0.0,trf.getRotation());
  EXPECT_EQ(1.,trf.getScale());
  EXPECT_EQ(0.,trf.x0);
  EXPECT_EQ(0.,trf.y0);
  EXPECT_EQ(2,trf.minNumberOfPoints());
}

TEST(Helmert2D, Constructor)
{
  Helmert2D<cv::Point2d> trf(150.0, 75.0, 0.25, I3D_PI / 180 * 35);
  EXPECT_EQ(transform_type::HELMERT_2D, trf.getTransformType());
  EXPECT_NEAR(I3D_PI/180 * 35, trf.getRotation(), 0.0001);
  EXPECT_EQ(0.25,trf.getScale());
  EXPECT_EQ(150.0,trf.x0);
  EXPECT_EQ(75.0,trf.y0);
  EXPECT_EQ(2,trf.minNumberOfPoints());
}


TEST(Afin, DefaultConstructor)
{
  Afin<cv::Point2d> trf;
  EXPECT_EQ(transform_type::AFIN, trf.getTransformType());
  EXPECT_EQ(0.0, trf.getRotation());
  EXPECT_EQ(1., trf.getScaleX());
  EXPECT_EQ(1., trf.getScaleY());
  EXPECT_EQ(0., trf.x0);
  EXPECT_EQ(0., trf.y0);
  EXPECT_EQ(3, trf.minNumberOfPoints());
}

TEST(Afin, Constructor)
{
  Afin<cv::Point2d> trf(150.0, 75.0, 0.25, 0.25, I3D_PI / 180 * 35);
  EXPECT_EQ(transform_type::AFIN, trf.getTransformType());
  EXPECT_NEAR(I3D_PI / 180 * 35,trf.getRotation(), 0.0001);
  EXPECT_EQ(0.25, trf.getScaleX());
  EXPECT_EQ(0.25, trf.getScaleY());
  EXPECT_EQ(150.0, trf.x0);
  EXPECT_EQ(75.0, trf.y0);
  EXPECT_EQ(3, trf.minNumberOfPoints());

  std::vector<cv::Point2d> in;
  std::ifstream read_handler("C:\\Desarrollo\\datos\\CloudPoint_10.csv", std::ifstream::in);
  if (read_handler.is_open()) {
    //// get length of file:
    //read_handler.seekg (0, read_handler.end);
    //size_t length = read_handler.tellg();
    //read_handler.seekg (0, read_handler.beg);
    //if (length > 0) {
    //  in.resize(length);
    std::string aux;
      while (!read_handler.eof()){
        read_handler >> aux;
        std::vector<double> coord;
        splitToNumbers(aux, coord);
        in.push_back(cv::Point2d(coord[0],coord[1]));
      }
    //}
  }
  std::vector<cv::Point2d> out(in.size());
  trf.transform(in, &out);
  cv::Point2d kk = out[0];
}




TEST(Projective, DefaultConstructor)
{
  Projective<cv::Point2d> trf;
  EXPECT_EQ(transform_type::PROJECTIVE, trf.getTransformType());
  EXPECT_EQ(4,trf.minNumberOfPoints());
}

TEST(isNumberOfPointsValid, AllTrfTest)
{
  Translate<cv::Point2d> trfTranslate;
  Rotation<cv::Point2d> trfRotate;
  Helmert2D<cv::Point2d> trfHelmert2D;
  Afin<cv::Point2d> trfAfin;
  Projective<cv::Point2d> trfProjective;

  EXPECT_TRUE(trfTranslate.isNumberOfPointsValid(1));
  EXPECT_TRUE(trfRotate.isNumberOfPointsValid(1));
  EXPECT_TRUE(trfHelmert2D.isNumberOfPointsValid(2));
  EXPECT_TRUE(trfAfin.isNumberOfPointsValid(3));
  EXPECT_TRUE(trfProjective.isNumberOfPointsValid(4));

  EXPECT_FALSE(trfTranslate.isNumberOfPointsValid(0));
  EXPECT_FALSE(trfRotate.isNumberOfPointsValid(0));
  EXPECT_FALSE(trfHelmert2D.isNumberOfPointsValid(1));
  EXPECT_FALSE(trfAfin.isNumberOfPointsValid(2));
  EXPECT_FALSE(trfProjective.isNumberOfPointsValid(2));
}


TEST(getTransformType, AllTrfTest)
{
  Translate<cv::Point2d> trfTranslate;
  Rotation<cv::Point2d> trfRotate;
  Helmert2D<cv::Point2d> trfHelmert2D;
  Afin<cv::Point2d> trfAfin;
  Projective<cv::Point2d> trfProjective;

  EXPECT_EQ(transform_type::TRANSLATE,trfTranslate.getTransformType());
  EXPECT_EQ(transform_type::ROTATION,trfRotate.getTransformType());
  EXPECT_EQ(transform_type::HELMERT_2D,trfHelmert2D.getTransformType());
  EXPECT_EQ(transform_type::AFIN,trfAfin.getTransformType());
  EXPECT_EQ(transform_type::PROJECTIVE,trfProjective.getTransformType());

  Transform<cv::Point2d> *trf = &trfTranslate;
  EXPECT_EQ(transform_type::TRANSLATE,trf->getTransformType());
  trf = &trfRotate;
  EXPECT_EQ(transform_type::ROTATION,trfRotate.getTransformType());
  trf = &trfHelmert2D;
  EXPECT_EQ(transform_type::HELMERT_2D,trfHelmert2D.getTransformType());
  trf = &trfAfin;
  EXPECT_EQ(transform_type::AFIN,trfAfin.getTransformType());
  trf = &trfProjective;
  EXPECT_EQ(transform_type::PROJECTIVE,trfProjective.getTransformType());
}
GTEST_API_ int main(int argc, char **argv) {
  printf("Running main() from gtest_main.cc\n");
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
