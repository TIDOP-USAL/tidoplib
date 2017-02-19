#include <gtest/gtest.h>
#include "transform.h"
#include "core/utils.h"
#include "opencv2/core/core.hpp"

using namespace I3D;

// Pendiente
// - TrfMultiple
// - TrfPerspective

std::vector<cv::Point2d> ptsIn{
    cv::Point2d(4157222.543, 664789.307),
    cv::Point2d(4149043.336, 688836.443),
    cv::Point2d(4172803.511, 690340.078),
    cv::Point2d(4177148.376, 642997.635),
    cv::Point2d(4137012.190, 671808.029),
    cv::Point2d(4146292.729, 666952.887),
    cv::Point2d(4138759.902, 702670.738) };

  //cv::Point2f srcTri[3];
  //cv::Point2f dstTri[3];

  //srcTri[0] = cv::Point2f( 0,0 );
  //srcTri[1] = cv::Point2f( 1024 - 1, 0 );
  //srcTri[2] = cv::Point2f( 0, 760 - 1 );

  //dstTri[0] = cv::Point2f( 1024*0.0, 760*0.33 );
  //dstTri[1] = cv::Point2f( 1024*0.85, 760*0.25 );
  //dstTri[2] = cv::Point2f( 1024*0.15, 760*0.7 );

  //cv::Mat t_Affine( 2, 3, CV_32FC1 );

  //t_Affine = cv::getAffineTransform(srcTri, dstTri);

  //std::vector< cv::Point2d> src { 
  //  cv::Point2d( 0,0 ), 
  //  cv::Point2d( 1024 - 1, 0 ), 
  //  cv::Point2d( 0, 760 - 1 )
  //};

  //std::vector< cv::Point2d> dst {
  //  cv::Point2d(1024 * 0.0, 760 * 0.33),
  //  cv::Point2d( 1024*0.85, 760*0.25 ),
  //  cv::Point2d( 1024*0.15, 760*0.7 )
  //};

  //Affine<cv::Point2d> t_Affine2;
  //t_Affine2.compute(src, dst);

// Translate


TEST(Translate, DefaultConstructor)
{
  Translate<cv::Point2d> trf;
  EXPECT_EQ(transform_type::TRANSLATE, trf.getTransformType());
  EXPECT_EQ(0, trf.tx);
  EXPECT_EQ(0, trf.ty);
  EXPECT_EQ(1, trf.minNumberOfPoints());
  EXPECT_EQ(transform_type::TRANSLATE, trf.getTransformType());
}

TEST(Translate, Constructor)
{
  Translate<cv::Point2d> trf(150.0, 75.0);
  EXPECT_EQ(transform_type::TRANSLATE, trf.getTransformType());
  EXPECT_EQ(150.0, trf.tx);
  EXPECT_EQ(75.0, trf.ty);
  EXPECT_EQ(1, trf.minNumberOfPoints());
  EXPECT_EQ(transform_type::TRANSLATE, trf.getTransformType());
}

TEST(Translate, Transform)
{
  Translate<cv::Point2d> trf(150.0, 75.0);
  std::vector<cv::Point2d> ptsOut;
  trf.transform(ptsIn, &ptsOut);
  
  std::vector<cv::Point2d> pts_out {
    cv::Point2d(4157372.543,	664864.307),
    cv::Point2d(4149193.336,	688911.443),
    cv::Point2d(4172953.511,	690415.078),
    cv::Point2d(4177298.376,	643072.635),
    cv::Point2d(4137162.19, 671883.029),
    cv::Point2d(4146442.729,	667027.887),
    cv::Point2d(4138909.902,	702745.738) };

  for (int i = 0; i < ptsOut.size(); i++) {
    EXPECT_DOUBLE_EQ(pts_out[i].x, ptsOut[i].x);
    EXPECT_DOUBLE_EQ(pts_out[i].y, ptsOut[i].y);
  }
}


TEST(Translate, Compute)
{
  Translate<cv::Point2d> trf(150.0, 75.0);
  std::vector<cv::Point2d> ptsOut;
  trf.transform(ptsIn, &ptsOut);
  trf.compute(ptsIn,ptsOut);
  EXPECT_NEAR(150.0,trf.tx,0.1);
  EXPECT_NEAR(75.0,trf.ty,0.1);
}

// Rotation

TEST(Rotation, DefaultConstructor)
{
  Rotation<cv::Point2d> rot;
  EXPECT_EQ(transform_type::ROTATION, rot.getTransformType());
  EXPECT_EQ(0, rot.getAngle());
  EXPECT_EQ(1, rot.minNumberOfPoints());
  EXPECT_EQ(transform_type::ROTATION, rot.getTransformType());
}

TEST(Rotation, Constructor)
{
  Rotation<cv::Point2d> rot(35 * I3D_DEG_TO_RAD);
  EXPECT_EQ(transform_type::ROTATION, rot.getTransformType());
  EXPECT_NEAR(35 * I3D_DEG_TO_RAD, rot.getAngle(), 0.0001);
  EXPECT_EQ(1, rot.minNumberOfPoints());
  EXPECT_EQ(transform_type::ROTATION, rot.getTransformType());
  rot.setAngle( 61 * I3D_DEG_TO_RAD);
  EXPECT_NEAR(61 * I3D_DEG_TO_RAD, rot.getAngle(), 0.0001);
}

TEST(Rotation, Transform)
{
  //Rotation<cv::Point2d> rot(35 * I3D_PI / 180.);
  //std::vector<cv::Point2d> ptsOut;
  //rot.transform(ptsIn, &ptsOut);
  //
  //std::vector<cv::Point2d> pts_out { };

  //for (int i = 0; i < ptsOut.size(); i++) {
  //  EXPECT_DOUBLE_EQ(pts_out[i].x, ptsOut[i].x);
  //  EXPECT_DOUBLE_EQ(pts_out[i].y, ptsOut[i].y);
  //}
}


TEST(Rotation, Compute)
{
  Rotation<cv::Point2d> trf(45. * I3D_DEG_TO_RAD );
  std::vector<cv::Point2d> ptsOut;
  trf.transform(ptsIn, &ptsOut);
  trf.compute(ptsIn,ptsOut);
  EXPECT_NEAR(45. * I3D_DEG_TO_RAD, trf.getAngle(), 0.1);
}

// Helmert 2D

TEST(Helmert2D, DefaultConstructor)
{
  Helmert2D<cv::Point2d> trf;
  EXPECT_EQ(transform_type::HELMERT_2D, trf.getTransformType());
  EXPECT_EQ(0.0,trf.getRotation());
  EXPECT_EQ(1.,trf.getScale());
  EXPECT_EQ(0.,trf.tx);
  EXPECT_EQ(0.,trf.ty);
  EXPECT_EQ(2,trf.minNumberOfPoints());
  EXPECT_EQ(transform_type::HELMERT_2D, trf.getTransformType());
}

TEST(Helmert2D, Constructor)
{
  Helmert2D<cv::Point2d> trf(150.0, 75.0, 0.25, 35 * I3D_DEG_TO_RAD);
  EXPECT_EQ(transform_type::HELMERT_2D, trf.getTransformType());
  EXPECT_NEAR( 35 * I3D_DEG_TO_RAD, trf.getRotation(), 0.0001);
  EXPECT_EQ(0.25, trf.getScale());
  EXPECT_EQ(150.0, trf.tx);
  EXPECT_EQ(75.0, trf.ty);
  EXPECT_EQ(2, trf.minNumberOfPoints());
  EXPECT_EQ(transform_type::HELMERT_2D, trf.getTransformType());

  trf.setRotation(47 * I3D_DEG_TO_RAD);
  EXPECT_DOUBLE_EQ( 47 * I3D_DEG_TO_RAD, trf.getRotation());

  trf.setScale(0.64);
  EXPECT_DOUBLE_EQ( 0.64, trf.getScale());

  trf.setParameters(0.56, 163.25, 2., .131);
  EXPECT_DOUBLE_EQ( .131, trf.getRotation());
  EXPECT_DOUBLE_EQ(2., trf.getScale());
  EXPECT_DOUBLE_EQ(0.56, trf.tx);
  EXPECT_DOUBLE_EQ(163.25, trf.ty);
}

TEST(Helmert2D, Transform)
{
  //Helmert2D<cv::Point2d> trf;
  //std::vector<cv::Point2d> ptsOut;
  //trf.transform(ptsIn, &ptsOut);
  //
  //std::vector<cv::Point2d> pts_out { };

  //for (int i = 0; i < ptsOut.size(); i++) {
  //  EXPECT_DOUBLE_EQ(pts_out[i].x, ptsOut[i].x);
  //  EXPECT_DOUBLE_EQ(pts_out[i].y, ptsOut[i].y);
  //}
}

TEST(Affine, DefaultConstructor)
{
  Affine<cv::Point2d> trf;
  EXPECT_EQ(transform_type::AFFINE, trf.getTransformType());
  EXPECT_EQ(0.0, trf.getRotation());
  EXPECT_EQ(1., trf.getScaleX());
  EXPECT_EQ(1., trf.getScaleY());
  EXPECT_EQ(0., trf.tx);
  EXPECT_EQ(0., trf.ty);
  EXPECT_EQ(3, trf.minNumberOfPoints());
}

TEST(Affine, Constructor)
{
  Affine<cv::Point2d> trf(150.0, 75.0, 0.25, 0.25, I3D_PI / 180 * 35);
  EXPECT_EQ(transform_type::AFFINE, trf.getTransformType());
  EXPECT_NEAR(I3D_PI / 180 * 35,trf.getRotation(), 0.0001);
  EXPECT_EQ(0.25, trf.getScaleX());
  EXPECT_EQ(0.25, trf.getScaleY());
  EXPECT_EQ(150.0, trf.tx);
  EXPECT_EQ(75.0, trf.ty);
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
  Affine<cv::Point2d> trfAffine;
  Projective<cv::Point2d> trfProjective;

  EXPECT_TRUE(trfTranslate.isNumberOfPointsValid(1));
  EXPECT_TRUE(trfRotate.isNumberOfPointsValid(1));
  EXPECT_TRUE(trfHelmert2D.isNumberOfPointsValid(2));
  EXPECT_TRUE(trfAffine.isNumberOfPointsValid(3));
  EXPECT_TRUE(trfProjective.isNumberOfPointsValid(4));

  EXPECT_FALSE(trfTranslate.isNumberOfPointsValid(0));
  EXPECT_FALSE(trfRotate.isNumberOfPointsValid(0));
  EXPECT_FALSE(trfHelmert2D.isNumberOfPointsValid(1));
  EXPECT_FALSE(trfAffine.isNumberOfPointsValid(2));
  EXPECT_FALSE(trfProjective.isNumberOfPointsValid(2));
}


TEST(getTransformType, AllTrfTest)
{
  Translate<cv::Point2d> trfTranslate;
  Rotation<cv::Point2d> trfRotate;
  Helmert2D<cv::Point2d> trfHelmert2D;
  Affine<cv::Point2d> trfAffine;
  Projective<cv::Point2d> trfProjective;

  EXPECT_EQ(transform_type::TRANSLATE, trfTranslate.getTransformType());
  EXPECT_EQ(transform_type::ROTATION, trfRotate.getTransformType());
  EXPECT_EQ(transform_type::HELMERT_2D, trfHelmert2D.getTransformType());
  EXPECT_EQ(transform_type::AFFINE, trfAffine.getTransformType());
  EXPECT_EQ(transform_type::PROJECTIVE, trfProjective.getTransformType());

  Transform<cv::Point2d> *trf = &trfTranslate;
  EXPECT_EQ(transform_type::TRANSLATE, trf->getTransformType());
  trf = &trfRotate;
  EXPECT_EQ(transform_type::ROTATION, trfRotate.getTransformType());
  trf = &trfHelmert2D;
  EXPECT_EQ(transform_type::HELMERT_2D, trfHelmert2D.getTransformType());
  trf = &trfAffine;
  EXPECT_EQ(transform_type::AFFINE, trfAffine.getTransformType());
  trf = &trfProjective;
  EXPECT_EQ(transform_type::PROJECTIVE, trfProjective.getTransformType());
}

// Pendiente test de transformación de entidades
TEST(transform, Entities)
{
  WindowI wi(PointI(2, 3), PointI(100, 369));
  WindowD wd(PointD(23.3, 234.6), PointD(100., 369.));
  WindowF wf(PointF(31.f, 52.36f), PointF(100.34f, 369.36f));
  WindowI wi_out;
  Translate<PointI> trf(150.0, 75.0);
  transform(wi, &wi_out, &trf);
  EXPECT_EQ(152, wi_out.pt1.x);
  EXPECT_EQ(78, wi_out.pt1.y);
  EXPECT_EQ(250, wi_out.pt2.x);
  EXPECT_EQ(444, wi_out.pt2.y);
}

// Pendiente test de transformación de imagenes
// Pendiente test transformación multiple
TEST(TrfMultiple, DefaultConstructor)
{
  TrfMultiple<PointD> trfMul;
  trfMul.add(std::make_shared<Translate<PointD>>(150.0, 75.0));
  trfMul.add(std::make_shared<Rotation<PointD>>(45. * I3D_DEG_TO_RAD));

  EXPECT_EQ(transform_type::MULTIPLE, trfMul.getTransformType());

}

TEST(TrfMultiple, Constructor)
{
  TrfMultiple<PointD> trf{
    std::make_shared<Translate<PointD>>(150.0, 75.0),
    std::make_shared<Rotation<PointD>>(45. * I3D_DEG_TO_RAD)
  };


}

GTEST_API_ int main(int argc, char **argv) {
  printf("Running main() from gtest_main.cc\n");
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
