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
 
#define BOOST_TEST_MODULE Tidop angle convertion test
#include <boost/test/unit_test.hpp>
#include <tidop/math/angles.h>
#include <tidop/core/utils.h>

using namespace tl;
using namespace math;


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

BOOST_FIXTURE_TEST_CASE(copy_constructor, RadiansTest)
{
  Radians<double> copy(angle_pi_div2);
  BOOST_CHECK_EQUAL(consts::half_pi<double>, copy.value());
}

BOOST_FIXTURE_TEST_CASE(move_constructor, RadiansTest)
{
  Radians<double> temp(-3 * consts::pi<double>);
  Radians<double> move(std::move(temp));
  BOOST_CHECK_EQUAL(-3 * consts::pi<double>, move.value());
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
    angle3 = -23.23654564654;
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
  Gradians<double> angle3;
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
  BOOST_CHECK_EQUAL(-23.23654564654, angle3.value());
}

BOOST_FIXTURE_TEST_CASE(degrees, GradiansTest)
{
  BOOST_CHECK_EQUAL(23, angle2.degrees());
  BOOST_CHECK_EQUAL(-23, angle3.degrees());
}

BOOST_FIXTURE_TEST_CASE(minutes, GradiansTest)
{
  BOOST_CHECK_EQUAL(23, angle2.minutes());
  BOOST_CHECK_EQUAL(23, angle3.minutes());
}

BOOST_FIXTURE_TEST_CASE(seconds, GradiansTest)
{
  BOOST_CHECK_CLOSE(65.4564654, angle2.seconds(), 0.01);
  BOOST_CHECK_CLOSE(65.4564654, angle3.seconds(), 0.01);
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

BOOST_FIXTURE_TEST_CASE(setDegrees, GradiansTest)
{
  {
    Gradians<double> angle_1;
    angle_1.setDegrees(23);
    BOOST_CHECK_EQUAL(23, angle_1.degrees());
    BOOST_CHECK_CLOSE(23., angle_1.value(), 0.1);

    angle_1.setMinutes(23);
    BOOST_CHECK_EQUAL(23, angle_1.degrees());
    BOOST_CHECK_EQUAL(23, angle_1.minutes());
    BOOST_CHECK_CLOSE(23.23, angle_1.value(), 0.1);

    angle_1.setSeconds(65.4564654);
    BOOST_CHECK_EQUAL(23, angle_1.degrees());
    BOOST_CHECK_EQUAL(23, angle_1.minutes());
    BOOST_CHECK_CLOSE(65.4564654, angle_1.seconds(), 0.1);
    BOOST_CHECK_CLOSE(23.23654564654, angle_1.value(), 0.1);

    Gradians<double> angle_2;
    angle_2.setSeconds(65.4564654);
    BOOST_CHECK_CLOSE(65.4564654, angle_2.seconds(), 0.1);
    BOOST_CHECK_CLOSE(0.00654564654, angle_2.value(), 0.1);

    angle_2.setMinutes(23);
    BOOST_CHECK_EQUAL(23, angle_2.minutes());
    BOOST_CHECK_CLOSE(65.4564654, angle_2.seconds(), 0.1);
    BOOST_CHECK_CLOSE(0.23654564654, angle_2.value(), 0.1);

    angle_2.setDegrees(23);
    BOOST_CHECK_EQUAL(23, angle_2.degrees());
    BOOST_CHECK_EQUAL(23, angle_2.minutes());
    BOOST_CHECK_CLOSE(65.4564654, angle_2.seconds(), 0.1);
    BOOST_CHECK_CLOSE(23.23654564654, angle_2.value(), 0.1);
  }

  {
    Gradians<double> angle_1;
    angle_1.setDegrees(-23);
    BOOST_CHECK_EQUAL(-23, angle_1.degrees());
    BOOST_CHECK_CLOSE(-23., angle_1.value(), 0.1);

    angle_1.setMinutes(23);
    BOOST_CHECK_EQUAL(-23, angle_1.degrees());
    BOOST_CHECK_EQUAL(23, angle_1.minutes());
    BOOST_CHECK_CLOSE(-23.23, angle_1.value(), 0.1);

    angle_1.setSeconds(65.4564654);
    BOOST_CHECK_EQUAL(-23, angle_1.degrees());
    BOOST_CHECK_EQUAL(23, angle_1.minutes());
    BOOST_CHECK_CLOSE(65.4564654, angle_1.seconds(), 0.1);
    BOOST_CHECK_CLOSE(-23.23654564654, angle_1.value(), 0.1);

    Gradians<double> angle_2;
    angle_2.setSeconds(65.4564654);
    BOOST_CHECK_CLOSE(65.4564654, angle_2.seconds(), 0.1);
    BOOST_CHECK_CLOSE(0.00654564654, angle_2.value(), 0.1);

    angle_2.setMinutes(23);
    BOOST_CHECK_EQUAL(23, angle_2.minutes());
    BOOST_CHECK_CLOSE(65.4564654, angle_2.seconds(), 0.1);
    BOOST_CHECK_CLOSE(0.23654564654, angle_2.value(), 0.1);

    angle_2.setDegrees(-23);
    BOOST_CHECK_EQUAL(-23, angle_2.degrees());
    BOOST_CHECK_EQUAL(23, angle_2.minutes());
    BOOST_CHECK_CLOSE(65.4564654, angle_2.seconds(), 0.1);
    BOOST_CHECK_CLOSE(-23.23654564654, angle_2.value(), 0.1);
  }
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
    angle3 = -135.5742;
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
  Degrees<double> angle3;
};

BOOST_FIXTURE_TEST_CASE(default_constructor, DegreesTest)
{
  BOOST_CHECK_EQUAL(true, Angle::Unit::degrees == angle.unit());
  BOOST_CHECK_EQUAL(0.0, angle.value());
  BOOST_CHECK_EQUAL(0, angle.degrees());
  BOOST_CHECK_EQUAL(0, angle.minutes());
  BOOST_CHECK_EQUAL(0.0, angle.seconds());
}

BOOST_FIXTURE_TEST_CASE(constructor_degrees_minutes_seconds, DegreesTest)
{
  Degrees<double> degrees_minutes_seconds(43, 2, 1.9604);

  BOOST_CHECK(Angle::Unit::degrees == degrees_minutes_seconds.unit());
  BOOST_CHECK_CLOSE(43.0338778888889, degrees_minutes_seconds.value(), 0.01);
  BOOST_CHECK_EQUAL(43, degrees_minutes_seconds.degrees());
  BOOST_CHECK_EQUAL(2, degrees_minutes_seconds.minutes());
  BOOST_CHECK_CLOSE(1.9604, degrees_minutes_seconds.seconds(), 0.01);

  Degrees<double> degrees_minutes_seconds_neg(-43, 2, 1.9604);

  BOOST_CHECK(Angle::Unit::degrees == degrees_minutes_seconds_neg.unit());
  BOOST_CHECK_CLOSE(-43.0338778888889, degrees_minutes_seconds_neg.value(), 0.01);
  BOOST_CHECK_EQUAL(-43, degrees_minutes_seconds_neg.degrees());
  BOOST_CHECK_EQUAL(2, degrees_minutes_seconds_neg.minutes());
  BOOST_CHECK_CLOSE(1.9604, degrees_minutes_seconds_neg.seconds(), 0.01);
}

BOOST_FIXTURE_TEST_CASE(value, DegreesTest)
{
  BOOST_CHECK_EQUAL(135.5742, angle2.value());
  BOOST_CHECK_EQUAL(-135.5742, angle3.value());
}

BOOST_FIXTURE_TEST_CASE(degrees, DegreesTest)
{
  BOOST_CHECK_EQUAL(135, angle2.degrees());
  BOOST_CHECK_EQUAL(-135, angle3.degrees());
}

BOOST_FIXTURE_TEST_CASE(minutes, DegreesTest)
{
  BOOST_CHECK_EQUAL(34, angle2.minutes());
  BOOST_CHECK_EQUAL(34, angle3.minutes());
}

BOOST_FIXTURE_TEST_CASE(seconds, DegreesTest)
{
  BOOST_CHECK_CLOSE(27.1199, angle2.seconds(), 0.001);
  BOOST_CHECK_CLOSE(27.1199, angle3.seconds(), 0.001);
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

BOOST_FIXTURE_TEST_CASE(setDegrees, DegreesTest)
{
  {
    Degrees<double> angle_1;
    angle_1.setDegrees(43);
    BOOST_CHECK_EQUAL(43, angle_1.degrees());
    BOOST_CHECK_CLOSE(43., angle_1.value(), 0.01);

    angle_1.setMinutes(2);
    BOOST_CHECK_EQUAL(43, angle_1.degrees());
    BOOST_CHECK_EQUAL(2, angle_1.minutes());
    BOOST_CHECK_CLOSE(43.033333333, angle_1.value(), 0.01);

    angle_1.setSeconds(1.9604);
    BOOST_CHECK_EQUAL(43, angle_1.degrees());
    BOOST_CHECK_EQUAL(2, angle_1.minutes());
    BOOST_CHECK_CLOSE(1.9604, angle_1.seconds(), 0.1);
    BOOST_CHECK_CLOSE(43.0338778888889, angle_1.value(), 0.01);

    Degrees<double> angle_2;
    angle_2.setSeconds(1.9604);
    BOOST_CHECK_CLOSE(1.9604, angle_2.seconds(), 0.1);
    BOOST_CHECK_CLOSE(0.00054455, angle_2.value(), 0.1);

    angle_2.setMinutes(2);
    BOOST_CHECK_EQUAL(2, angle_2.minutes());
    BOOST_CHECK_CLOSE(1.9604, angle_2.seconds(), 0.1);
    BOOST_CHECK_CLOSE(0.03387788, angle_2.value(), 0.1);

    angle_2.setDegrees(43);
    BOOST_CHECK_EQUAL(43, angle_2.degrees());
    BOOST_CHECK_EQUAL(2, angle_2.minutes());
    BOOST_CHECK_CLOSE(1.9604, angle_2.seconds(), 0.1);
    BOOST_CHECK_CLOSE(43.0338778888889, angle_2.value(), 0.1);
  }
  
  {
    Degrees<double> angle_1;
    angle_1.setDegrees(-43);
    BOOST_CHECK_EQUAL(-43, angle_1.degrees());
    BOOST_CHECK_CLOSE(-43., angle_1.value(), 0.01);

    angle_1.setMinutes(2);
    BOOST_CHECK_EQUAL(-43, angle_1.degrees());
    BOOST_CHECK_EQUAL(2, angle_1.minutes());
    BOOST_CHECK_CLOSE(-43.033333333, angle_1.value(), 0.01);

    angle_1.setSeconds(1.9604);
    BOOST_CHECK_EQUAL(-43, angle_1.degrees());
    BOOST_CHECK_EQUAL(2, angle_1.minutes());
    BOOST_CHECK_CLOSE(1.9604, angle_1.seconds(), 0.1);
    BOOST_CHECK_CLOSE(-43.0338778888889, angle_1.value(), 0.01);

    Degrees<double> angle_2;
    angle_2.setSeconds(1.9604);
    BOOST_CHECK_CLOSE(1.9604, angle_2.seconds(), 0.1);
    BOOST_CHECK_CLOSE(0.00054455, angle_2.value(), 0.1);

    angle_2.setMinutes(2);
    BOOST_CHECK_EQUAL(2, angle_2.minutes());
    BOOST_CHECK_CLOSE(1.9604, angle_2.seconds(), 0.1);
    BOOST_CHECK_CLOSE(0.03387788, angle_2.value(), 0.1);

    angle_2.setDegrees(-43);
    BOOST_CHECK_EQUAL(-43, angle_2.degrees());
    BOOST_CHECK_EQUAL(2, angle_2.minutes());
    BOOST_CHECK_CLOSE(1.9604, angle_2.seconds(), 0.1);
    BOOST_CHECK_CLOSE(-43.0338778888889, angle_2.value(), 0.1);
  }

  {
    Degrees<double> angle_1;
    angle_1.setDegrees(74);
    BOOST_CHECK_EQUAL(74, angle_1.degrees());
    BOOST_CHECK_CLOSE(74., angle_1.value(), 0.01);

    angle_1.setMinutes(59);
    BOOST_CHECK_EQUAL(74, angle_1.degrees());
    BOOST_CHECK_EQUAL(59, angle_1.minutes());
    BOOST_CHECK_CLOSE(74.983333, angle_1.value(), 0.01);

    angle_1.setSeconds(17.8786);
    BOOST_CHECK_EQUAL(74, angle_1.degrees());
    BOOST_CHECK_EQUAL(59, angle_1.minutes());
    BOOST_CHECK_CLOSE(17.8786, angle_1.seconds(), 0.1);
    BOOST_CHECK_CLOSE(74.98829961, angle_1.value(), 0.01);

    Degrees<double> angle_2;
    angle_2.setSeconds(17.8786);
    BOOST_CHECK_CLOSE(17.8786, angle_2.seconds(), 0.1);
    BOOST_CHECK_CLOSE(0.00496627, angle_2.value(), 0.1);

    angle_2.setMinutes(59);
    BOOST_CHECK_EQUAL(59, angle_2.minutes());
    BOOST_CHECK_CLOSE(17.8786, angle_2.seconds(), 0.1);
    BOOST_CHECK_CLOSE(0.988299611, angle_2.value(), 0.1);

    angle_2.setDegrees(74);
    BOOST_CHECK_EQUAL(74, angle_2.degrees());
    BOOST_CHECK_EQUAL(59, angle_2.minutes());
    BOOST_CHECK_CLOSE(17.8786, angle_2.seconds(), 0.1);
    BOOST_CHECK_CLOSE(74.98829961, angle_2.value(), 0.1);

  }
  
}

BOOST_FIXTURE_TEST_CASE(user_defined_literals, DegreesTest)
{
    auto angle_rad = 1.0_rad;
    BOOST_CHECK_CLOSE(1.0, angle_rad.value(), 0.1);

    auto angle_grad = 23.23654564654_grad;
    BOOST_CHECK_CLOSE(23.23654564654, angle_grad.value(), 0.1);
    BOOST_CHECK_EQUAL(23, angle_grad.degrees());
    BOOST_CHECK_EQUAL(23, angle_grad.minutes());
    BOOST_CHECK_CLOSE(65.4564654, angle_grad.seconds(), 0.01);

    auto angle_deg = 135.5742_deg;
    BOOST_CHECK_CLOSE(135.5742, angle_deg.value(), 0.1);
    BOOST_CHECK_EQUAL(135, angle_deg.degrees());
    BOOST_CHECK_EQUAL(34, angle_deg.minutes());
    BOOST_CHECK_CLOSE(27.1199, angle_deg.seconds(), 0.01);

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
    
    degrees1 = Degrees<double>(0, 0, 0);
    degrees2 = Degrees<double>(0, 30, 0);
    degrees3 = Degrees<double>(135, 34, 27);
    degrees4 = Degrees<double>(86, 59, 59);
    degrees5 = Degrees<double>(-269, 23, 6);

    radians1 = Radians<double>(0.);
    radians2 = Radians<double>(0.00872664626);
    radians3 = Radians<double>(2.3662161708);
    radians4 = Radians<double>(1.518431213);
    radians5 = Radians<double>(-4.7016552055);

    gradians1 = Gradians<double>(0.);
    gradians2 = Gradians<double>(0.555556);
    gradians3 = Gradians<double>(150.63800);
    gradians4 = Gradians<double>(96.666335);
    gradians5 = Gradians<double>(-299.3166718);

  }
 
  void teardown()
  {
  }

  Radians<double> radians1;
  Radians<double> radians2;
  Radians<double> radians3;
  Radians<double> radians4;
  Radians<double> radians5;
  Gradians<double> gradians1;
  Gradians<double> gradians2;
  Gradians<double> gradians3;
  Gradians<double> gradians4;
  Gradians<double> gradians5;
  Degrees<double> degrees1;
  Degrees<double> degrees2;
  Degrees<double> degrees3;
  Degrees<double> degrees4;
  Degrees<double> degrees5;
};


BOOST_FIXTURE_TEST_CASE(degrees_to_radians, AngleConvertion) 
{
  Radians<double> rad = degrees1;
  BOOST_CHECK_CLOSE(0., rad.value(), 0.1);
  rad = degrees2;
  BOOST_CHECK_CLOSE(0.00872664626, rad.value(), 0.1);
  rad = degrees3;
  BOOST_CHECK_CLOSE(2.3662161708, rad.value(), 0.1);
  rad = degrees4;
  BOOST_CHECK_CLOSE(1.518431213, rad.value(), 0.1);
  rad = degrees5;
  BOOST_CHECK_CLOSE(-4.7016552055, rad.value(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(degrees_to_gradians, AngleConvertion)
{
  Gradians<double> rad = degrees1;
  BOOST_CHECK_CLOSE(0., rad.value(), 0.1);
  rad = degrees2;
  BOOST_CHECK_CLOSE(0.555556, rad.value(), 0.1);
  rad = degrees3;
  BOOST_CHECK_CLOSE(150.63800, rad.value(), 0.1);
  rad = degrees4;
  BOOST_CHECK_CLOSE(96.666335, rad.value(), 0.1);
  rad = degrees5;
  BOOST_CHECK_CLOSE(-299.3166718, rad.value(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(radians_to_degrees, AngleConvertion)
{
  Degrees<double> deg = radians1;
  BOOST_CHECK_EQUAL(0, deg.degrees());
  BOOST_CHECK_EQUAL(0, deg.minutes());
  BOOST_CHECK_EQUAL(0., deg.seconds());

  deg = radians2;
  BOOST_CHECK_EQUAL(0, deg.degrees());
  BOOST_CHECK_EQUAL(30, deg.minutes());
  BOOST_CHECK_EQUAL(0, roundToInteger(deg.seconds()));

  deg = radians3;
  BOOST_CHECK_EQUAL(135, deg.degrees());
  BOOST_CHECK_EQUAL(34, deg.minutes());
  BOOST_CHECK_EQUAL(27, roundToInteger(deg.seconds()));

  deg = radians4;
  BOOST_CHECK_EQUAL(86, deg.degrees());
  BOOST_CHECK_EQUAL(59, deg.minutes());
  BOOST_CHECK_EQUAL(59, roundToInteger(deg.seconds()));

  deg = radians5;
  BOOST_CHECK_EQUAL(-269, deg.degrees());
  BOOST_CHECK_EQUAL(23, deg.minutes());
  BOOST_CHECK_EQUAL(6, roundToInteger(deg.seconds()));
}

BOOST_FIXTURE_TEST_CASE(radians_to_gradians, AngleConvertion) 
{
  Gradians<double> rad = radians1;
  BOOST_CHECK_CLOSE(0., rad.value(), 0.1);
  rad = radians2;
  BOOST_CHECK_CLOSE(0.555556, rad.value(), 0.1);
  rad = radians3;
  BOOST_CHECK_CLOSE(150.63800, rad.value(), 0.1);
  rad = radians4;
  BOOST_CHECK_CLOSE(96.666335, rad.value(), 0.1);
  rad = radians5;
  BOOST_CHECK_CLOSE(-299.3166718, rad.value(), 0.1);
}


BOOST_FIXTURE_TEST_CASE(gradians_to_degrees, AngleConvertion)
{
  Degrees<double> deg = gradians1;
  BOOST_CHECK_EQUAL(0, deg.degrees());
  BOOST_CHECK_EQUAL(0, deg.minutes());
  BOOST_CHECK_EQUAL(0., deg.seconds());

  deg = gradians2;
  BOOST_CHECK_EQUAL(0, deg.degrees());
  BOOST_CHECK_EQUAL(30, deg.minutes());
  BOOST_CHECK_EQUAL(0, roundToInteger(deg.seconds()));

  deg = gradians3;
  BOOST_CHECK_EQUAL(135, deg.degrees());
  BOOST_CHECK_EQUAL(34, deg.minutes());
  BOOST_CHECK_EQUAL(27, roundToInteger(deg.seconds()));

  deg = gradians4;
  BOOST_CHECK_EQUAL(86, deg.degrees());
  BOOST_CHECK_EQUAL(59, deg.minutes());
  BOOST_CHECK_EQUAL(59, roundToInteger(deg.seconds()));

  deg = gradians5;
  BOOST_CHECK_EQUAL(-269, deg.degrees());
  BOOST_CHECK_EQUAL(23, deg.minutes());
  BOOST_CHECK_EQUAL(6, roundToInteger(deg.seconds()));
}

BOOST_FIXTURE_TEST_CASE(gradians_to_radians, AngleConvertion) 
{
  Radians<double> rad = gradians1;

  BOOST_CHECK_CLOSE(0., rad.value(), 0.1);

  rad = gradians2;
  BOOST_CHECK_CLOSE(0.00872664626, rad.value(), 0.1);

  rad = gradians3;
  BOOST_CHECK_CLOSE(2.3662161708, rad.value(), 0.1);

  rad = gradians4;
  BOOST_CHECK_CLOSE(1.518431213, rad.value(), 0.1);

  rad = gradians5;
  BOOST_CHECK_CLOSE(-4.7016552055, rad.value(), 0.1);
}

BOOST_AUTO_TEST_SUITE_END()
