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
 * License along with Foobar. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/
 
#define BOOST_TEST_MODULE Tidop angle convertion test
#include <boost/test/unit_test.hpp>
#include <tidop/math/angles.h>

using namespace tl::math;


/* ---------------------------------------------------------------------------------- */
/*                          Test Radians Class                                        */
/* ---------------------------------------------------------------------------------- */

BOOST_AUTO_TEST_SUITE(RadiansTestSuite)

struct RadiansTest
{

  RadiansTest()
  {

  }

  ~RadiansTest()
  {

  }

  void setup()
  {
    angle_0 = Radians<double>(0);
    angle_pi_div2 = Radians<double>(consts::half_pi<double>);
    angle_minus_30 = Radians<double>(-30.*consts::deg_to_rad<double>);
    angle_pi = Radians<double>(consts::pi<double>);
    angle_2pi = Radians<double>(consts::two_pi<double>);
    angle_3pi = Radians<double>(3 * consts::pi<double>);
    angle_4pi = Radians<double>(4 * consts::pi<double>);
    angle_minus_pi_div2 = Radians<double>(-consts::half_pi<double>);
    angle_minus_pi = Radians<double>(-consts::pi<double>);
    angle_minus_2pi = Radians<double>(-consts::two_pi<double>);
    angle_minus_3pi = Radians<double>(-3 * consts::pi<double>);
    angle_minus_4pi = Radians<double>(-4 * consts::pi<double>);
    angle_1 = Radians<double>(-3 * consts::half_pi<double>);
  }
 
  void teardown()
  {
  }

  Radians<double> angle;
  Radians<double> angle_0;
  Radians<double> angle_pi_div2;
  Radians<double> angle_minus_30;
  Radians<double> angle_pi;
  Radians<double> angle_2pi;
  Radians<double> angle_3pi;
  Radians<double> angle_4pi;
  Radians<double> angle_minus_pi_div2;
  Radians<double> angle_minus_pi;
  Radians<double> angle_minus_2pi;
  Radians<double> angle_minus_3pi;
  Radians<double> angle_minus_4pi;
  Radians<double> angle_1;
};

BOOST_FIXTURE_TEST_CASE(default_constructor, RadiansTest)
{
  BOOST_CHECK(Angle::Unit::radians == angle.unit());
  BOOST_CHECK_EQUAL(0.0, angle.value());
}

BOOST_FIXTURE_TEST_CASE(normalize, RadiansTest) 
{
  /// Angulos entre 0 y 2*PI

  angle_0.normalize();
  BOOST_CHECK_EQUAL(0., angle_0.value());

  angle_pi_div2.normalize();
  BOOST_CHECK_EQUAL(consts::half_pi<double>, angle_pi_div2.value());

  angle_minus_30.normalize();
  BOOST_CHECK_CLOSE(-30.*consts::deg_to_rad<double>, angle_minus_30.value(), 0.01);

  angle_pi.normalize();
  BOOST_CHECK_EQUAL(consts::pi<double>, angle_pi.value());

  angle_2pi.normalize();
  BOOST_CHECK_EQUAL(0., angle_2pi.value());

  angle_3pi.normalize();
  BOOST_CHECK_EQUAL(consts::pi<double>, angle_3pi.value());

  angle_4pi.normalize();
  BOOST_CHECK_EQUAL(0., angle_4pi.value());

  angle_minus_pi_div2.normalize();
  BOOST_CHECK_EQUAL(-consts::half_pi<double>, angle_minus_pi_div2.value());

  angle_minus_pi.normalize();
  BOOST_CHECK_EQUAL(consts::pi<double>, angle_minus_pi.value());

  angle_minus_2pi.normalize();
  BOOST_CHECK_EQUAL(0, angle_minus_2pi.value());

  angle_minus_3pi.normalize();
  BOOST_CHECK_EQUAL(consts::pi<double>, angle_minus_3pi.value());

  angle_minus_4pi.normalize();
  BOOST_CHECK_EQUAL(0., angle_minus_4pi.value());

  angle_1.normalize();
  BOOST_CHECK_EQUAL(consts::half_pi<double>, angle_1.value());
}

BOOST_FIXTURE_TEST_CASE(normalizePositive, RadiansTest) 
{
  /// Angulos entre -PI y PI

  angle_0.normalizePositive();
  BOOST_CHECK_EQUAL(0., angle_0.value());

  angle_minus_30.normalizePositive();
  BOOST_CHECK_CLOSE(5.75958653, angle_minus_30.value(), 0.01);

  angle_pi_div2.normalizePositive();
  BOOST_CHECK_EQUAL(consts::half_pi<double>, angle_pi_div2.value());

  angle_pi.normalizePositive();
  BOOST_CHECK_EQUAL(consts::pi<double>, angle_pi.value());

  angle_2pi.normalizePositive();
  BOOST_CHECK_EQUAL(0., angle_2pi.value());

  angle_3pi.normalizePositive();
  BOOST_CHECK_EQUAL(consts::pi<double>, angle_3pi.value());

  angle_4pi.normalizePositive();
  BOOST_CHECK_EQUAL(0., angle_4pi.value());

  angle_minus_pi_div2.normalizePositive();
  BOOST_CHECK_EQUAL(3*consts::half_pi<double>, angle_minus_pi_div2.value());

  angle_minus_pi.normalizePositive();
  BOOST_CHECK_EQUAL(consts::pi<double>, angle_minus_pi.value());

  angle_minus_2pi.normalizePositive();
  BOOST_CHECK_EQUAL(0, angle_minus_2pi.value());

  angle_minus_3pi.normalizePositive();
  BOOST_CHECK_EQUAL(consts::pi<double>, angle_minus_3pi.value());

  angle_minus_4pi.normalizePositive();
  BOOST_CHECK_EQUAL(0., angle_minus_4pi.value());
}

BOOST_AUTO_TEST_SUITE_END()



/* ---------------------------------------------------------------------------------- */
/*                          Test Gradians Class                                       */
/* ---------------------------------------------------------------------------------- */

BOOST_AUTO_TEST_SUITE(GradiansTestSuite)

struct GradiansTest
{

  GradiansTest()
  {

  }

  ~GradiansTest()
  {

  }

  void setup()
  {
    angle_0 = Gradians<double>(0.);
    angle_100 = Gradians<double>(100.);
    angle_minus_30 = Gradians<double>(-30.);
    angle_200 = Gradians<double>(200);
    angle_400 = Gradians<double>(400);
    angle_600 = Gradians<double>(600);
    angle_800 = Gradians<double>(800);
    angle_minus_100 = Gradians<double>(-100);
    angle_minus_200 = Gradians<double>(-200);
    angle_minus_400 = Gradians<double>(-400);
    angle_minus_600 = Gradians<double>(-600);
    angle_minus_800 = Gradians<double>(-800);
    angle_minus_300 = Gradians<double>(-300);
    angle2 = 23.23654564654;
  }
 
  void teardown()
  {
  }

  Gradians<double> angle;
  Gradians<double> angle_0;
  Gradians<double> angle_100;
  Gradians<double> angle_minus_30;
  Gradians<double> angle_200;
  Gradians<double> angle_400;
  Gradians<double> angle_600;
  Gradians<double> angle_800;
  Gradians<double> angle_minus_100;
  Gradians<double> angle_minus_200;
  Gradians<double> angle_minus_400;
  Gradians<double> angle_minus_600;
  Gradians<double> angle_minus_800;
  Gradians<double> angle_minus_300;
  Gradians<double> angle2;
};

BOOST_FIXTURE_TEST_CASE(default_constructor, GradiansTest)
{
  BOOST_CHECK(Angle::Unit::gradians == angle.unit());
  BOOST_CHECK_EQUAL(0.0, angle.value());
  BOOST_CHECK_EQUAL(0, angle.degrees());
  BOOST_CHECK_EQUAL(0, angle.minutes());
  BOOST_CHECK_EQUAL(0.0, angle.seconds());
}

BOOST_FIXTURE_TEST_CASE(value, GradiansTest)
{
  BOOST_CHECK_EQUAL(23.23654564654, angle2.value());
}

BOOST_FIXTURE_TEST_CASE(degrees, GradiansTest)
{
  BOOST_CHECK_EQUAL(23, angle2.degrees());
}

BOOST_FIXTURE_TEST_CASE(minutes, GradiansTest)
{
  BOOST_CHECK_EQUAL(23, angle2.minutes());
}

BOOST_FIXTURE_TEST_CASE(seconds, GradiansTest)
{
  BOOST_CHECK_CLOSE(65.4564654, angle2.seconds(), 0.01);
}

BOOST_FIXTURE_TEST_CASE(normalize, GradiansTest) 
{
  /// Angulos entre 0 y 2*PI

  angle_0.normalize();
  BOOST_CHECK_EQUAL(0., angle_0.value());

  angle_100.normalize();
  BOOST_CHECK_EQUAL(100., angle_100.value());

  angle_minus_30.normalize();
  BOOST_CHECK_EQUAL(-30., angle_minus_30.value());

  angle_200.normalize();
  BOOST_CHECK_EQUAL(200., angle_200.value());

  angle_400.normalize();
  BOOST_CHECK_EQUAL(0., angle_400.value());

  angle_600.normalize();
  BOOST_CHECK_EQUAL(200., angle_600.value());

  angle_800.normalize();
  BOOST_CHECK_EQUAL(0., angle_800.value());

  angle_minus_100.normalize();
  BOOST_CHECK_EQUAL(-100., angle_minus_100.value());

  angle_minus_200.normalize();
  BOOST_CHECK_EQUAL(200., angle_minus_200.value());

  angle_minus_400.normalize();
  BOOST_CHECK_EQUAL(0., angle_minus_400.value());

  angle_minus_600.normalize();
  BOOST_CHECK_EQUAL(200, angle_minus_600.value());

  angle_minus_800.normalize();
  BOOST_CHECK_EQUAL(0., angle_minus_800.value());

  angle_minus_300.normalize();
  BOOST_CHECK_EQUAL(100., angle_minus_300.value());
}

BOOST_FIXTURE_TEST_CASE(normalizePositive, GradiansTest) 
{
  /// Angulos entre -PI y PI

  angle_0.normalizePositive();
  BOOST_CHECK_EQUAL(0., angle_0.value());

  angle_minus_30.normalizePositive();
  BOOST_CHECK_CLOSE(370., angle_minus_30.value(), 0.01);

  angle_200.normalizePositive();
  BOOST_CHECK_EQUAL(200., angle_200.value());

  angle_400.normalizePositive();
  BOOST_CHECK_EQUAL(0, angle_400.value());

  angle_600.normalizePositive();
  BOOST_CHECK_EQUAL(200., angle_600.value());

  angle_800.normalizePositive();
  BOOST_CHECK_EQUAL(0., angle_800.value());

  angle_minus_100.normalizePositive();
  BOOST_CHECK_EQUAL(300., angle_minus_100.value());

  angle_minus_200.normalizePositive();
  BOOST_CHECK_EQUAL(200., angle_minus_200.value());

  angle_minus_400.normalizePositive();
  BOOST_CHECK_EQUAL(0., angle_minus_400.value());

  angle_minus_600.normalizePositive();
  BOOST_CHECK_EQUAL(200., angle_minus_600.value());

  angle_minus_800.normalizePositive();
  BOOST_CHECK_EQUAL(0., angle_minus_800.value());
}

BOOST_AUTO_TEST_SUITE_END()




/* ---------------------------------------------------------------------------------- */
/*                          Test Degrees Class                                        */
/* ---------------------------------------------------------------------------------- */

BOOST_AUTO_TEST_SUITE(DegreesTestSuite)

struct DegreesTest
{

  DegreesTest()
  {

  }

  ~DegreesTest()
  {

  }

  void setup()
  {
    angle_0 = Degrees<double>(0.);
    angle_90 = Degrees<double>(90.);
    angle_minus_30 = Degrees<double>(-30.);
    angle_180 = Degrees<double>(180);
    angle_360 = Degrees<double>(360);
    angle_540 = Degrees<double>(540);
    angle_720 = Degrees<double>(720);
    angle_minus_90 = Degrees<double>(-90);
    angle_minus_180 = Degrees<double>(-180);
    angle_minus_360 = Degrees<double>(-360);
    angle_minus_540 = Degrees<double>(-540);
    angle_minus_720 = Degrees<double>(-720);
    angle_minus_270 = Degrees<double>(-270);
    angle2 = 135.5742;
  }
 
  void teardown()
  {
  }

  Degrees<double> angle;
  Degrees<double> angle_0;
  Degrees<double> angle_90;
  Degrees<double> angle_minus_30;
  Degrees<double> angle_180;
  Degrees<double> angle_360;
  Degrees<double> angle_540;
  Degrees<double> angle_720;
  Degrees<double> angle_minus_90;
  Degrees<double> angle_minus_180;
  Degrees<double> angle_minus_360;
  Degrees<double> angle_minus_540;
  Degrees<double> angle_minus_720;
  Degrees<double> angle_minus_270;
  Degrees<double> angle2;
};

BOOST_FIXTURE_TEST_CASE(default_constructor, DegreesTest)
{
  BOOST_CHECK(Angle::Unit::degrees == angle.unit());
  BOOST_CHECK_EQUAL(0.0, angle.value());
  BOOST_CHECK_EQUAL(0, angle.degrees());
  BOOST_CHECK_EQUAL(0, angle.minutes());
  BOOST_CHECK_EQUAL(0.0, angle.seconds());
}

BOOST_FIXTURE_TEST_CASE(value, DegreesTest)
{
  BOOST_CHECK_EQUAL(135.5742, angle2.value());
}

BOOST_FIXTURE_TEST_CASE(degrees, DegreesTest)
{
  BOOST_CHECK_EQUAL(135, angle2.degrees());
}

BOOST_FIXTURE_TEST_CASE(minutes, DegreesTest)
{
  BOOST_CHECK_EQUAL(34, angle2.minutes());
}

BOOST_FIXTURE_TEST_CASE(seconds, DegreesTest)
{
  BOOST_CHECK_CLOSE(27.1199, angle2.seconds(), 0.001);
}

BOOST_FIXTURE_TEST_CASE(normalize, DegreesTest) 
{
  /// Angulos entre 0 y 2*PI

  angle_0.normalize();
  BOOST_CHECK_EQUAL(0., angle_0.value());

  angle_90.normalize();
  BOOST_CHECK_EQUAL(90., angle_90.value());

  angle_minus_30.normalize();
  BOOST_CHECK_EQUAL(-30., angle_minus_30.value());

  angle_180.normalize();
  BOOST_CHECK_EQUAL(180., angle_180.value());

  angle_360.normalize();
  BOOST_CHECK_EQUAL(0., angle_360.value());

  angle_540.normalize();
  BOOST_CHECK_EQUAL(180., angle_540.value());

  angle_720.normalize();
  BOOST_CHECK_EQUAL(0., angle_720.value());

  angle_minus_90.normalize();
  BOOST_CHECK_EQUAL(-90., angle_minus_90.value());

  angle_minus_180.normalize();
  BOOST_CHECK_EQUAL(180., angle_minus_180.value());

  angle_minus_360.normalize();
  BOOST_CHECK_EQUAL(0., angle_minus_360.value());

  angle_minus_540.normalize();
  BOOST_CHECK_EQUAL(180, angle_minus_540.value());

  angle_minus_720.normalize();
  BOOST_CHECK_EQUAL(0., angle_minus_720.value());

  angle_minus_270.normalize();
  BOOST_CHECK_EQUAL(90., angle_minus_270.value());
}

BOOST_FIXTURE_TEST_CASE(normalizePositive, DegreesTest) 
{
  /// Angulos entre -PI y PI

  angle_0.normalizePositive();
  BOOST_CHECK_EQUAL(0., angle_0.value());

  angle_minus_30.normalizePositive();
  BOOST_CHECK_CLOSE(330., angle_minus_30.value(), 0.01);

  angle_180.normalizePositive();
  BOOST_CHECK_EQUAL(180., angle_180.value());

  angle_360.normalizePositive();
  BOOST_CHECK_EQUAL(0, angle_360.value());

  angle_540.normalizePositive();
  BOOST_CHECK_EQUAL(180., angle_540.value());

  angle_720.normalizePositive();
  BOOST_CHECK_EQUAL(0., angle_720.value());

  angle_minus_90.normalizePositive();
  BOOST_CHECK_EQUAL(270., angle_minus_90.value());

  angle_minus_180.normalizePositive();
  BOOST_CHECK_EQUAL(180., angle_minus_180.value());

  angle_minus_360.normalizePositive();
  BOOST_CHECK_EQUAL(0., angle_minus_360.value());

  angle_minus_540.normalizePositive();
  BOOST_CHECK_EQUAL(180., angle_minus_540.value());

  angle_minus_720.normalizePositive();
  BOOST_CHECK_EQUAL(0., angle_minus_720.value());
}

BOOST_AUTO_TEST_SUITE_END()




/* ---------------------------------------------------------------------------------- */
/*                          Test conversión de ángulos                                */
/* ---------------------------------------------------------------------------------- */

BOOST_AUTO_TEST_SUITE(AngleConvertionTestSuite)

struct AngleConvertion
{

  AngleConvertion()
  {

  }

  ~AngleConvertion()
  {

  }

  void setup()
  {
    decimalDegrees.push_back(0.);
    decimalDegrees.push_back(0.5);
    decimalDegrees.push_back(135.5742);
    decimalDegrees.push_back(86.9997);
    decimalDegrees.push_back(-269.385);
    
    dms.push_back({ 0, 0, 0 });
    dms.push_back({ 0, 30, 0 });
    dms.push_back({ 135, 34, 27 });
    dms.push_back({ 86, 59, 59 });
    dms.push_back({ -269, 23, 6 });
    
    radians.push_back(0.);
    radians.push_back(0.00872664626);
    radians.push_back(2.3662161708);
    radians.push_back(1.518431213);
    radians.push_back(-4.7016552055);

    gradians.push_back(0.);
    gradians.push_back(0.555556);
    gradians.push_back(150.63800);
    gradians.push_back(96.666335);
    gradians.push_back(-299.3166718);
  }
 
  void teardown()
  {
  }

  std::vector<double> decimalDegrees;
  std::vector<std::vector<int>> dms;
  std::vector<double> radians;
  std::vector<double> gradians;
};

//std::vector<double> decimalDegrees = { 0., 0.5, 135.5742, 86.9997, -269.385 };
//std::vector<std::vector<int>> dms{ { 0, 0, 0 }, { 0, 30, 0 }, { 135, 34, 27 }, { 86, 59, 59 }, { -269, 23, 6 } };
//std::vector<double> radians{ 0.,0.00872664626,2.3662161708,1.518431213, -4.7016552055 };
//std::vector<double> gradians{ 0., 0.555556, 150.63800, 96.666335, -299.3166718 };

BOOST_FIXTURE_TEST_CASE(degrees_to_decimal_degrees, AngleConvertion) 
{
  for (int i = 0; i < dms.size(); i++) {
    BOOST_CHECK_CLOSE(decimalDegrees[i], degreesToDecimalDegrees(dms[i][0], dms[i][1], static_cast<double>(dms[i][2])), 0.1);
  }
}

BOOST_FIXTURE_TEST_CASE(degrees_to_radians, AngleConvertion) 
{
  for (int i = 0; i < dms.size(); i++) {
    BOOST_CHECK_CLOSE(radians[i], degreesToRadians(dms[i][0], dms[i][1], static_cast<double>(dms[i][2])), 0.1);
  }
}

BOOST_FIXTURE_TEST_CASE(degrees_to_gradians, AngleConvertion)
{
  for (int i = 0; i < dms.size(); i++) {
    BOOST_CHECK_CLOSE(gradians[i], degreesToGradians(dms[i][0], dms[i][1], static_cast<double>(dms[i][2])), 0.1);
  }
}

  
BOOST_FIXTURE_TEST_CASE(decimal_degrees_to_degrees, AngleConvertion) 
{
  int degrees, minutes;
  double seconds;
  for (int i = 0; i < decimalDegrees.size(); i++) {
    decimalDegreesToDegrees(decimalDegrees[i], &degrees, &minutes, &seconds);
    BOOST_CHECK_EQUAL(dms[i][0], degrees);
    BOOST_CHECK_EQUAL(dms[i][1], minutes);
    BOOST_CHECK_EQUAL(dms[i][2], TL_ROUND_TO_INT(seconds));
  }
}

BOOST_FIXTURE_TEST_CASE(decimal_degrees_to_radians, AngleConvertion)
{
  for (int i = 0; i < decimalDegrees.size(); i++) {
    BOOST_CHECK_CLOSE(radians[i], decimalDegreesToRadians(decimalDegrees[i]), 0.1);
  }
}

BOOST_FIXTURE_TEST_CASE(decimal_degrees_to_gradians, AngleConvertion) {
  for (int i = 0; i < decimalDegrees.size(); i++) {
    BOOST_CHECK_CLOSE(gradians[i], decimalDegreesToGradians(decimalDegrees[i]), 0.1);
  }
}

BOOST_FIXTURE_TEST_CASE(radians_to_degrees, AngleConvertion) {
  int degrees, minutes;
  double seconds;
  for (int i = 0; i < radians.size(); i++) {
    radiansToDegrees(radians[i], &degrees, &minutes, &seconds);
    BOOST_CHECK_EQUAL(dms[i][0], degrees);
    BOOST_CHECK_EQUAL(dms[i][1], minutes);
    BOOST_CHECK_EQUAL(dms[i][2], TL_ROUND_TO_INT(seconds));
  }
}

BOOST_FIXTURE_TEST_CASE(radians_to_decimal_degrees, AngleConvertion) {
  for (int i = 0; i < radians.size(); i++) {
    BOOST_CHECK_CLOSE(decimalDegrees[i], radiansToDecimalDegrees(radians[i]), 0.1);
  }
}

BOOST_FIXTURE_TEST_CASE(radians_to_gradians, AngleConvertion) {
  for (int i = 0; i < radians.size(); i++) {
    BOOST_CHECK_CLOSE(gradians[i], radiansToGradians(radians[i]), 0.1);
  }
}


BOOST_FIXTURE_TEST_CASE(gradians_to_degrees, AngleConvertion) {
  int degrees, minutes;
  double seconds;
  for (int i = 0; i < gradians.size(); i++) {
    gradiansToDegrees(gradians[i], &degrees, &minutes, &seconds);
    BOOST_CHECK_EQUAL(dms[i][0], degrees);
    BOOST_CHECK_EQUAL(dms[i][1], minutes);
    BOOST_CHECK_EQUAL(dms[i][2], TL_ROUND_TO_INT(seconds));
  }
}

BOOST_FIXTURE_TEST_CASE(gradians_to_decimalDegrees, AngleConvertion) {
  for (int i = 0; i < gradians.size(); i++) {
    BOOST_CHECK_CLOSE(decimalDegrees[i], gradiansToDecimalDegrees(gradians[i]), 0.1);
  }
}

BOOST_FIXTURE_TEST_CASE(gradians_to_radians, AngleConvertion) {
  for (int i = 0; i < gradians.size(); i++) {
    BOOST_CHECK_CLOSE(radians[i], gradiansToRadians(gradians[i]), 0.1);
  }
}

BOOST_AUTO_TEST_SUITE_END()
