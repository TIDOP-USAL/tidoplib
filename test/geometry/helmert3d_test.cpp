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

#include <tidop/geometry/transform/helmert3d.h>
#include <tidop/geometry/entities/point.h>

using namespace tl;



BOOST_AUTO_TEST_SUITE(Helmert3DTestSuite)

struct Helmert3DTest
{
  Helmert3DTest()
    : trf2(new Helmert3D<Point3<double>>(1027871.7679,
                                         72364.1120, 
                                         941534.6321,
                                         -266.6595, 
                                         29421.7784 * math::consts::arcsecond_to_deg<double> * math::consts::deg_to_rad<double>,
                                         -25330.9216 * math::consts::arcsecond_to_deg<double> * math::consts::deg_to_rad<double>,
                                         1235.9212 * math::consts::arcsecond_to_deg<double> * math::consts::deg_to_rad<double>))
  {

  }

  ~Helmert3DTest()
  {
    if (trf2){
      delete trf2;
      trf2 = nullptr;
    }
  }

  void setup()
  {
    /// Datos obtenidos de:
    /// https://www.researchgate.net/publication/280881630_Least_Square_Approach_to_Estimate_3D_Coordinate_Transformation_Parameters_A_Case_of_Three_Reference_Systems_in_Sweden

    ptsIn = {
      Point3<double>(2441775.419, 799268.100, 5818729.162), 
      Point3<double>(3464655.838, 845749.989, 5270271.528),
      Point3<double>(3309991.828, 828932.118, 5370882.280),
      Point3<double>(3160763.338, 759160.187, 5469345.504),
      Point3<double>(2248123.493, 865686.595, 5886425.596),
      Point3<double>(3022573.157, 802945.690, 5540683.951),
      Point3<double>(3104219.427, 998384.028, 5463290.505),
      Point3<double>(2998189.685, 931451.634, 5533398.462),
      Point3<double>(3199093.294, 932231.327, 5420322.483),
      Point3<double>(3370658.823, 711876.990, 5349786.786),
      Point3<double>(3341340.173, 957912.343, 5330003.236),
      Point3<double>(2534031.166, 975174.455, 5752078.309),
      Point3<double>(2838909.903, 903822.098, 5620660.184),
      Point3<double>(2902495.079, 761455.843, 5609859.672),
      Point3<double>(2682407.890, 950395.934, 5688993.082),
      Point3<double>(2620258.868, 779138.041, 5743799.267),
      Point3<double>(3246470.535, 1077900.355, 5365277.896),
      Point3<double>(3249408.275, 692757.965, 5426396.948),
      Point3<double>(2763885.496, 733247.387, 5682653.347),
      Point3<double>(2368885.005, 994492.233, 5818478.154) };

    ptsOut = {
      Point3<double>(2441276.712, 799286.666, 5818162.025),
      Point3<double>(3464161.275, 845805.461, 5269712.429), 
      Point3<double>(3309496.800, 828981.942, 5370322.060),
      Point3<double>(3160269.913, 759204.574, 5468784.081),
      Point3<double>(2247621.426, 865698.413, 5885856.498),
      Point3<double>(3022077.340, 802985.055, 5540121.276),
      Point3<double>(3103716.966, 998426.412, 5462727.814),
      Point3<double>(2997689.029, 931490.201, 5532835.154),
      Point3<double>(3198593.776, 932277.179, 5419760.966),
      Point3<double>(3370168.626, 711928.884, 5349227.574),
      Point3<double>(3340840.578, 957963.383, 5329442.724),
      Point3<double>(2533526.497, 975196.347, 5751510.935),
      Point3<double>(2838409.359, 903854.897, 5620095.593),
      Point3<double>(2902000.172, 761490.908, 5609296.343),
      Point3<double>(2681904.794, 950423.098, 5688426.909),
      Point3<double>(2619761.810, 779162.964, 5743233.630),
      Point3<double>(3245966.134, 1077947.976, 5364716.214),
      Point3<double>(3248918.041, 692805.543, 5425836.841),
      Point3<double>(2763390.878, 733277.458, 5682089.111),
      Point3<double>(2368378.937, 994508.273, 5817909.286) };

    ptsIn2 = {
      Point3<double>(319163.2, 6398188, 63.408),
      Point3<double>(319165.1, 6398184, 67.314),
      Point3<double>(319149.6, 6398191, 73.83),
      Point3<double>(319162.6, 6398173, 63.742),
      Point3<double>(319179.8, 6398162, 62.934),
      Point3<double>(319189.6, 6398159, 70.388),
      Point3<double>(319203.4, 6398147, 61.847),
      Point3<double>(319243.7, 6398123, 62.437),
      Point3<double>(319237.4, 6398147, 73.744),
      Point3<double>(319245.1, 6398081, 63.62)
    };

    ptsOut2 = {
      Point3<double>(1271102, 6402555, 65.443),
      Point3<double>(1271103, 6402549, 64.45),
      Point3<double>(1271085, 6402553, 65.958),
      Point3<double>(1271100, 6402541, 63.297),
      Point3<double>(1271119, 6402529, 66.967),
      Point3<double>(1271127, 6402524, 64.711),
      Point3<double>(1271143, 6402512, 66.617),
      Point3<double>(1271181, 6402489, 64.109),
      Point3<double>(1271181, 6402510, 60.898),
      Point3<double>(1271182, 6402447, 63.616)
    };
  }

  void teardown() 
  {
  }

  std::vector<Point3<double>> ptsIn;
  std::vector<Point3<double>> ptsOut;
  Helmert3D<Point3<double>> trf;
  std::vector<Point3<double>> ptsIn2;
  std::vector<Point3<double>> ptsOut2;
  Helmert3D<Point3<double>> *trf2;
};

//BOOST_FIXTURE_TEST_CASE(default_constructor, Helmert3DTest)
//{
//  BOOST_CHECK_EQUAL(0.0, trf.rotation());
//  BOOST_CHECK_EQUAL(1., trf.scale());
//  BOOST_CHECK_EQUAL(0., trf.tx);
//  BOOST_CHECK_EQUAL(0., trf.ty);
//  BOOST_CHECK_EQUAL(2, trf.minNumberOfPoints());
//}
//
//BOOST_FIXTURE_TEST_CASE(copy_constructor, Helmert3DTest)
//{
//  Helmert3D<PointD> copy(*trf_pointer);
//  BOOST_CHECK_CLOSE( 35 * math::consts::deg_to_rad<double>, copy.rotation(), 0.0001);
//  BOOST_CHECK_EQUAL(0.25, copy.scale());
//  BOOST_CHECK_EQUAL(150.0, copy.tx);
//  BOOST_CHECK_EQUAL(75.0, copy.ty);
//  BOOST_CHECK_EQUAL(2, copy.minNumberOfPoints());
//}
//
//BOOST_FIXTURE_TEST_CASE(assignement_operator, Helmert3DTest)
//{
//  Helmert3D<PointD> assig = *trf_pointer;
//  BOOST_CHECK_CLOSE( 35 * math::consts::deg_to_rad<double>, assig.rotation(), 0.0001);
//  BOOST_CHECK_EQUAL(0.25, assig.scale());
//  BOOST_CHECK_EQUAL(150.0, assig.tx);
//  BOOST_CHECK_EQUAL(75.0, assig.ty);
//  BOOST_CHECK_EQUAL(2, assig.minNumberOfPoints());
//}
//
//BOOST_FIXTURE_TEST_CASE(move_constructor, Helmert3DTest)
//{
//  Helmert3D<PointD> move(Helmert3D<PointD>(150.0, 75.0, 0.25, 35 * math::consts::deg_to_rad<double>));
//  BOOST_CHECK_CLOSE( 35 * math::consts::deg_to_rad<double>, move.rotation(), 0.0001);
//  BOOST_CHECK_EQUAL(0.25, move.scale());
//  BOOST_CHECK_EQUAL(150.0, move.tx);
//  BOOST_CHECK_EQUAL(75.0, move.ty);
//  BOOST_CHECK_EQUAL(2, move.minNumberOfPoints());
//}
//
//BOOST_FIXTURE_TEST_CASE(move_operator, Helmert3DTest)
//{
//  Helmert3D<PointD> move_assig = Helmert3D<PointD>(150.0, 75.0, 0.25, 35 * math::consts::deg_to_rad<double>);
//  BOOST_CHECK_CLOSE( 35 * math::consts::deg_to_rad<double>, move_assig.rotation(), 0.0001);
//  BOOST_CHECK_EQUAL(0.25, move_assig.scale());
//  BOOST_CHECK_EQUAL(150.0, move_assig.tx);
//  BOOST_CHECK_EQUAL(75.0, move_assig.ty);
//  BOOST_CHECK_EQUAL(2, move_assig.minNumberOfPoints());
//}

BOOST_FIXTURE_TEST_CASE(transform_point, Helmert3DTest)
{
  //Point3<double> pt_out;
  //trf2->transform(ptsIn2[0], pt_out);
  //BOOST_CHECK_CLOSE(ptsOut2[0].x, pt_out.x, 0.1);
  //BOOST_CHECK_CLOSE(ptsOut2[0].y, pt_out.y, 0.1);
  //pt_out = trf2->transform(ptsIn2[0], Transform::Order::direct);
  //BOOST_CHECK_CLOSE(ptsOut2[0].x, pt_out.x, 0.1);
  //BOOST_CHECK_CLOSE(ptsOut2[0].y, pt_out.y, 0.1);
}

BOOST_FIXTURE_TEST_CASE(transform_point_list, Helmert3DTest)
{
  std::vector<Point3<double>> pts_out;
  trf.compute(ptsIn, ptsOut);
  trf.transform(ptsIn, pts_out);

  // Complicado de testear con datos reales...
  //for (size_t i = 0; i < ptsOut.size(); i++) {
  //  BOOST_CHECK_CLOSE(pts_out[i].x, ptsOut[i].x, 0.01);
  //  BOOST_CHECK_CLOSE(pts_out[i].y, ptsOut[i].y, 0.01);
  //}
}

//BOOST_FIXTURE_TEST_CASE(std_transform, Helmert3DTest)
//{
//  std::vector<PointD> pts_out2(ptsIn.size());
//  std::transform(ptsIn.begin(), ptsIn.end(), pts_out2.begin(), *trf_pointer);
//
//  for (size_t i = 0; i < ptsOut.size(); i++) {
//    BOOST_CHECK_CLOSE(ptsOut[i].x, pts_out2[i].x, 0.1);
//    BOOST_CHECK_CLOSE(ptsOut[i].y, pts_out2[i].y, 0.1);
//  }
//
//  // Alternativa sin reservar tamaño en pts_out2:
//  std::vector<PointD> pts_out3;
//  std::transform(ptsIn.begin(), ptsIn.end(), std::back_inserter(pts_out3), *trf_pointer);
//
//
//  for (size_t i = 0; i < ptsOut.size(); i++) {
//    BOOST_CHECK_CLOSE(ptsOut[i].x, pts_out3[i].x, 0.1);
//    BOOST_CHECK_CLOSE(ptsOut[i].y, pts_out3[i].y, 0.1);
//  }
//}

BOOST_FIXTURE_TEST_CASE(compute, Helmert3DTest)
{
  trf.compute(ptsIn, ptsOut);
  double omega = trf.omega() * math::consts::deg_to_arcsecond<double> * math::consts::rad_to_deg<double>;
  double phi = trf.phi() * math::consts::deg_to_arcsecond<double> *math::consts::rad_to_deg<double>;
  double kappa = trf.kappa() * math::consts::deg_to_arcsecond<double> *math::consts::rad_to_deg<double>;
 
  BOOST_CHECK_CLOSE(1.0237, trf.scale(), 0.1);
  BOOST_CHECK_CLOSE(0.8502, omega, 0.1);
  BOOST_CHECK_CLOSE(1.8141, phi, 0.01);
  BOOST_CHECK_CLOSE(-7.8535, kappa, 0.1);
  BOOST_CHECK_CLOSE(-419.5684, trf.tx, 0.1);
  BOOST_CHECK_CLOSE(-99.2460, trf.ty, 0.1);
  BOOST_CHECK_CLOSE(-591.4559, trf.tz, 0.1);


  //trf2->compute(ptsIn2, ptsOut2);
  //omega = trf2->omega() * math::consts::deg_to_arcsecond<double> *math::consts::rad_to_deg<double>;
  //phi = trf2->phi() * math::consts::deg_to_arcsecond<double> *math::consts::rad_to_deg<double>;
  //kappa = trf2->kappa() * math::consts::deg_to_arcsecond<double> *math::consts::rad_to_deg<double>;

  //BOOST_CHECK_CLOSE(-266.6595, trf2->scale(), 0.1);
  //BOOST_CHECK_CLOSE(29421.7784, omega, 0.1);
  //BOOST_CHECK_CLOSE(-25330.9216, phi, 0.01);
  //BOOST_CHECK_CLOSE(1235.9212, kappa, 0.1);
  //BOOST_CHECK_CLOSE(1027871.7679, trf2->tx, 0.1);
  //BOOST_CHECK_CLOSE(72364.1120, trf2->ty, 0.1);
  //BOOST_CHECK_CLOSE(941534.6321, trf2->tz, 0.1);
}

BOOST_FIXTURE_TEST_CASE(transformType, Helmert3DTest)
{
  BOOST_CHECK(Transform::Type::helmert_3d == trf.transformType());
}

//BOOST_FIXTURE_TEST_CASE(isNumberOfPointsValid, Helmert3DTest)
//{
//  BOOST_CHECK(trf_pointer->isNumberOfPointsValid(2));
//  BOOST_CHECK(trf_pointer->isNumberOfPointsValid(10));
//  BOOST_CHECK(false == trf_pointer->isNumberOfPointsValid(0));
//  BOOST_CHECK(false == trf_pointer->isNumberOfPointsValid(1));
//}
//
//BOOST_FIXTURE_TEST_CASE(rotation, Helmert3DTest)
//{
//  BOOST_CHECK_CLOSE( 35 * math::consts::deg_to_rad<double>, trf_pointer->rotation(), 0.0001);
//}
//
//BOOST_FIXTURE_TEST_CASE(scale, Helmert3DTest)
//{
//  BOOST_CHECK_EQUAL(0.25, trf_pointer->scale());
//}
//
//BOOST_FIXTURE_TEST_CASE(setParameters, Helmert3DTest)
//{
//
//  trf_pointer->setParameters(0.56, 163.25, 2., .131);
//  BOOST_CHECK_EQUAL( .131, trf_pointer->rotation());
//  BOOST_CHECK_EQUAL(2., trf_pointer->scale());
//  BOOST_CHECK_EQUAL(0.56, trf_pointer->tx);
//  BOOST_CHECK_EQUAL(163.25, trf_pointer->ty);
//}
//
//BOOST_FIXTURE_TEST_CASE(setRotation, Helmert3DTest)
//{
//  trf_pointer->setRotation(47 * math::consts::deg_to_rad<double>);
//  BOOST_CHECK_EQUAL( 47 * math::consts::deg_to_rad<double>, trf_pointer->rotation());
//}
//
//BOOST_FIXTURE_TEST_CASE(setScale, Helmert3DTest)
//{
//  trf_pointer->setScale(0.64);
//  BOOST_CHECK_EQUAL( 0.64, trf_pointer->scale());
//
//}
//
//BOOST_FIXTURE_TEST_CASE(inverse, Helmert3DTest)
//{
//  Helmert3D<PointD> trf_inv = trf_pointer->inverse();
//  BOOST_CHECK_CLOSE(-663.564, trf_inv.tx, 0.1);
//  BOOST_CHECK_CLOSE(98.400, trf_inv.ty, 0.1);
//  BOOST_CHECK_CLOSE(-0.6108, trf_inv.rotation(), 0.1);
//  BOOST_CHECK_CLOSE(4., trf_inv.scale(), 0.1);
//}
//
//BOOST_FIXTURE_TEST_CASE(casting, Helmert3DTest)
//{
//  // cast a Translation float
//  Helmert3D<Point<float>> trf_h3D = static_cast<Helmert3D<Point<float>>>(trf);
//  BOOST_CHECK_EQUAL(0.0, trf_h3D.rotation());
//  BOOST_CHECK_EQUAL(1., trf_h3D.scale());
//  BOOST_CHECK_EQUAL(0., trf_h3D.tx);
//  BOOST_CHECK_EQUAL(0., trf_h3D.ty);
//  BOOST_CHECK_EQUAL(2, trf_h3D.minNumberOfPoints());
//
//  // cast a afín
//  Affine<PointD> trf_affine = static_cast<Affine<PointD>>(*trf_pointer);
//  BOOST_CHECK_EQUAL(35 * math::consts::deg_to_rad<double>, trf_affine.rotation());
//  BOOST_CHECK_EQUAL(0.25, trf_affine.scaleX());
//  BOOST_CHECK_EQUAL(0.25, trf_affine.scaleY());
//  BOOST_CHECK_EQUAL(150.0, trf_affine.tx);
//  BOOST_CHECK_EQUAL(75.0, trf_affine.ty);
//  BOOST_CHECK_EQUAL(3, trf_affine.minNumberOfPoints());
//
//  Translation<PointD> translation = static_cast<Translation<PointD>>(*trf_pointer);
//  BOOST_CHECK_EQUAL(150.0, translation.tx);
//  BOOST_CHECK_EQUAL(75.0, translation.ty);
//
//  Rotation<PointD> rotation = static_cast<Rotation<PointD>>(*trf_pointer);
//  BOOST_CHECK_EQUAL(35 * math::consts::deg_to_rad<double>, rotation.angle());
//}

BOOST_AUTO_TEST_SUITE_END()
