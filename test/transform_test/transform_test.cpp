#include <gtest/gtest.h>
#include "..\..\src\transform.h"

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
}




TEST(Projective, DefaultConstructor)
{
  Projective<cv::Point2d> trf;
  EXPECT_EQ(transform_type::PROJECTIVE, trf.getTransformType());
  EXPECT_EQ(4,trf.minNumberOfPoints());
}



GTEST_API_ int main(int argc, char **argv) {
  printf("Running main() from gtest_main.cc\n");
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
