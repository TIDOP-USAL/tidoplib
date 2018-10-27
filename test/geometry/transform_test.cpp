#include <gtest/gtest.h>
#include <tidop/geometry/transform.h>
#include <tidop/core/utils.h>
#include <tidop/core/exception.h>


using namespace tl;
using namespace geometry;

// Pendiente
// - TrfMultiple
// - TrfPerspective

//std::vector<PointD> ptsIn{
//    PointD(4157222.543, 664789.307),
//    PointD(4149043.336, 688836.443),
//    PointD(4172803.511, 690340.078),
//    PointD(4177148.376, 642997.635),
//    PointD(4137012.190, 671808.029),
//    PointD(4146292.729, 666952.887),
//    PointD(4138759.902, 702670.738) };
//
//TEST(Translate, DefaultConstructor)
//{
//  Translate<PointD> trf;
//  EXPECT_EQ(transform_type::TRANSLATE, trf.getTransformType());
//  EXPECT_EQ(0, trf.tx);
//  EXPECT_EQ(0, trf.ty);
//  EXPECT_EQ(1, trf.minNumberOfPoints());
//  EXPECT_EQ(transform_type::TRANSLATE, trf.getTransformType());
//}
//
//TEST(Translate, Constructor)
//{
//  Translate<PointD> trf(150.0, 75.0);
//  EXPECT_EQ(transform_type::TRANSLATE, trf.getTransformType());
//  EXPECT_EQ(150.0, trf.tx);
//  EXPECT_EQ(75.0, trf.ty);
//  EXPECT_EQ(1, trf.minNumberOfPoints());
//  EXPECT_EQ(transform_type::TRANSLATE, trf.getTransformType());
//}
//
//TEST(Translate, Transform)
//{
//  Translate<PointD> trf(150.0, 75.0);
//  std::vector<PointD> ptsOut;
//  trf.transform(ptsIn, &ptsOut);
//  
//  std::vector<PointD> pts_out {
//    PointD(4157372.543,	664864.307),
//    PointD(4149193.336,	688911.443),
//    PointD(4172953.511,	690415.078),
//    PointD(4177298.376,	643072.635),
//    PointD(4137162.19, 671883.029),
//    PointD(4146442.729,	667027.887),
//    PointD(4138909.902,	702745.738) };
//
//  for (int i = 0; i < ptsOut.size(); i++) {
//    EXPECT_DOUBLE_EQ(pts_out[i].x, ptsOut[i].x);
//    EXPECT_DOUBLE_EQ(pts_out[i].y, ptsOut[i].y);
//  }
//}
//
//
//TEST(Translate, Compute)
//{
//  Translate<PointD> trf(150.0, 75.0);
//  std::vector<PointD> ptsOut;
//  trf.transform(ptsIn, &ptsOut);
//  trf.compute(ptsIn,ptsOut);
//  EXPECT_NEAR(150.0,trf.tx,0.1);
//  EXPECT_NEAR(75.0,trf.ty,0.1);
//
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
//}
//
//// Rotation
//
//TEST(Rotation, DefaultConstructor)
//{
//  Rotation<PointD> rot;
//  EXPECT_EQ(transform_type::ROTATION, rot.getTransformType());
//  EXPECT_EQ(0, rot.getAngle());
//  EXPECT_EQ(1, rot.minNumberOfPoints());
//  EXPECT_EQ(transform_type::ROTATION, rot.getTransformType());
//}
//
//TEST(Rotation, Constructor)
//{
//  Rotation<PointD> rot(35 * TL_DEG_TO_RAD);
//  EXPECT_EQ(transform_type::ROTATION, rot.getTransformType());
//  EXPECT_NEAR(35 * TL_DEG_TO_RAD, rot.getAngle(), 0.0001);
//  EXPECT_EQ(1, rot.minNumberOfPoints());
//  EXPECT_EQ(transform_type::ROTATION, rot.getTransformType());
//  rot.setAngle( 61 * TL_DEG_TO_RAD);
//  EXPECT_NEAR(61 * TL_DEG_TO_RAD, rot.getAngle(), 0.0001);
//}
//
//TEST(Rotation, Transform)
//{
//  //Rotation<PointD> rot(35 * TL_PI / 180.);
//  //std::vector<PointD> ptsOut;
//  //rot.transform(ptsIn, &ptsOut);
//  //
//  //std::vector<PointD> pts_out { };
//
//  //for (int i = 0; i < ptsOut.size(); i++) {
//  //  EXPECT_DOUBLE_EQ(pts_out[i].x, ptsOut[i].x);
//  //  EXPECT_DOUBLE_EQ(pts_out[i].y, ptsOut[i].y);
//  //}
//}
//
//
//TEST(Rotation, Compute)
//{
//  Rotation<PointD> trf(45. * TL_DEG_TO_RAD );
//  std::vector<PointD> ptsOut;
//  trf.transform(ptsIn, &ptsOut);
//  trf.compute(ptsIn,ptsOut);
//  EXPECT_NEAR(45. * TL_DEG_TO_RAD, trf.getAngle(), 0.1);
//}
//
//// Helmert 2D
//
//TEST(Helmert2D, DefaultConstructor)
//{
//  Helmert2D<PointD> trf;
//  EXPECT_EQ(transform_type::HELMERT_2D, trf.getTransformType());
//  EXPECT_EQ(0.0,trf.getRotation());
//  EXPECT_EQ(1.,trf.getScale());
//  EXPECT_EQ(0.,trf.tx);
//  EXPECT_EQ(0.,trf.ty);
//  EXPECT_EQ(2,trf.minNumberOfPoints());
//  EXPECT_EQ(transform_type::HELMERT_2D, trf.getTransformType());
//}
//
//TEST(Helmert2D, Constructor)
//{
//  Helmert2D<PointD> trf(150.0, 75.0, 0.25, 35 * TL_DEG_TO_RAD);
//  EXPECT_EQ(transform_type::HELMERT_2D, trf.getTransformType());
//  EXPECT_NEAR( 35 * TL_DEG_TO_RAD, trf.getRotation(), 0.0001);
//  EXPECT_EQ(0.25, trf.getScale());
//  EXPECT_EQ(150.0, trf.tx);
//  EXPECT_EQ(75.0, trf.ty);
//  EXPECT_EQ(2, trf.minNumberOfPoints());
//  EXPECT_EQ(transform_type::HELMERT_2D, trf.getTransformType());
//
//  trf.setRotation(47 * TL_DEG_TO_RAD);
//  EXPECT_DOUBLE_EQ( 47 * TL_DEG_TO_RAD, trf.getRotation());
//
//  trf.setScale(0.64);
//  EXPECT_DOUBLE_EQ( 0.64, trf.getScale());
//
//  trf.setParameters(0.56, 163.25, 2., .131);
//  EXPECT_DOUBLE_EQ( .131, trf.getRotation());
//  EXPECT_DOUBLE_EQ(2., trf.getScale());
//  EXPECT_DOUBLE_EQ(0.56, trf.tx);
//  EXPECT_DOUBLE_EQ(163.25, trf.ty);
//}
//
//TEST(Helmert2D, Transform)
//{
//  //Helmert2D<PointD> trf;
//  //std::vector<PointD> ptsOut;
//  //trf.transform(ptsIn, &ptsOut);
//  //
//  //std::vector<PointD> pts_out { };
//
//  //for (int i = 0; i < ptsOut.size(); i++) {
//  //  EXPECT_DOUBLE_EQ(pts_out[i].x, ptsOut[i].x);
//  //  EXPECT_DOUBLE_EQ(pts_out[i].y, ptsOut[i].y);
//  //}
//}
//
//TEST(Affine, DefaultConstructor)
//{
//  Affine<PointD> trf;
//  EXPECT_EQ(transform_type::AFFINE, trf.getTransformType());
//  EXPECT_EQ(0.0, trf.getRotation());
//  EXPECT_EQ(1., trf.getScaleX());
//  EXPECT_EQ(1., trf.getScaleY());
//  EXPECT_EQ(0., trf.tx);
//  EXPECT_EQ(0., trf.ty);
//  EXPECT_EQ(3, trf.minNumberOfPoints());
//}
//
//TEST(Affine, Constructor)
//{
//  Affine<PointD> trf(150.0, 75.0, 0.25, 0.25, TL_PI / 180 * 35);
//  EXPECT_EQ(transform_type::AFFINE, trf.getTransformType());
//  EXPECT_NEAR(TL_PI / 180 * 35,trf.getRotation(), 0.0001);
//  EXPECT_EQ(0.25, trf.getScaleX());
//  EXPECT_EQ(0.25, trf.getScaleY());
//  EXPECT_EQ(150.0, trf.tx);
//  EXPECT_EQ(75.0, trf.ty);
//  EXPECT_EQ(3, trf.minNumberOfPoints());
//
//  std::vector<PointD> in;
//  std::ifstream read_handler("C:\\Desarrollo\\datos\\CloudPoint_10.csv", std::ifstream::in);
//  if (read_handler.is_open()) {
//    //// get length of file:
//    //read_handler.seekg (0, read_handler.end);
//    //size_t length = read_handler.tellg();
//    //read_handler.seekg (0, read_handler.beg);
//    //if (length > 0) {
//    //  in.resize(length);
//    std::string aux;
//      while (!read_handler.eof()){
//        read_handler >> aux;
//        std::vector<double> coord;
//        splitToNumbers(aux, coord);
//        in.push_back(PointD(coord[0],coord[1]));
//      }
//    //}
//  }
//  std::vector<PointD> out(in.size());
//  trf.transform(in, &out);
//  PointD kk = out[0];
//}
//
//
//
//
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
