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
 
#define BOOST_TEST_MODULE Tidop Units test
#include <boost/test/unit_test.hpp>
#include <boost/test/tools/output_test_stream.hpp>
#include <tidop/core/units/length.h>
#include <tidop/core/units/mass.h>

using namespace tl;


/* LengthConverterTestSuite */

BOOST_AUTO_TEST_SUITE(LengthConverterTestSuite)

struct LengthConverterTest
{

    LengthConverterTest()
    {

    }

    ~LengthConverterTest()
    {

    }

    void setup()
    {

    }

    void teardown()
    {
    }

    double metres = 25.8;
    double decimetres = 258;
    double centimetres = 2580;
    double millimetres = 25800;
    double micrometres = 2.58e+7;
    double nanometres = 2.58e+10;
    double kilometres = 0.0258;
    double hectometres = 0.258;
    double decametres = 2.58;
    double inches = 1015.748;
    double feet = 84.64567;
    double yards = 28.21522;
    double chains = 1.28251;
    double furlongs = 0.128251;
    double miles = 0.01603138;
};

BOOST_FIXTURE_TEST_CASE(from_metres, LengthConverterTest)
{
    double _decimetres = LengthConverter::convert(metres, LengthConverter::Units::metre, LengthConverter::Units::decimetre);
    BOOST_CHECK_EQUAL(decimetres, _decimetres);

    double _centimetres = LengthConverter::convert(metres, LengthConverter::Units::metre, LengthConverter::Units::centimetre);
    BOOST_CHECK_EQUAL(centimetres, _centimetres);

    double _millimetres = LengthConverter::convert(metres, LengthConverter::Units::metre, LengthConverter::Units::millimetre);
    BOOST_CHECK_EQUAL(millimetres, _millimetres);

    double _micrometres = LengthConverter::convert(metres, LengthConverter::Units::metre, LengthConverter::Units::micrometre);
    BOOST_CHECK_EQUAL(micrometres, _micrometres);

    double _nanometres = LengthConverter::convert(metres, LengthConverter::Units::metre, LengthConverter::Units::nanometre);
    BOOST_CHECK_EQUAL(nanometres, _nanometres);

    double _kilometres = LengthConverter::convert(metres, LengthConverter::Units::metre, LengthConverter::Units::kilometre);
    BOOST_CHECK_EQUAL(kilometres, _kilometres);

    double _hectometres = LengthConverter::convert(metres, LengthConverter::Units::metre, LengthConverter::Units::hectometre);
    BOOST_CHECK_EQUAL(hectometres, _hectometres);

    double _decametres = LengthConverter::convert(metres, LengthConverter::Units::metre, LengthConverter::Units::decametre);
    BOOST_CHECK_EQUAL(decametres, _decametres);

    double _inches = LengthConverter::convert(metres, LengthConverter::Units::metre, LengthConverter::Units::inch);
    BOOST_CHECK_CLOSE(inches, _inches, 0.001);

    double _feet = LengthConverter::convert(metres, LengthConverter::Units::metre, LengthConverter::Units::foot);
    BOOST_CHECK_CLOSE(feet, _feet, 0.001);

    double _yards = LengthConverter::convert(metres, LengthConverter::Units::metre, LengthConverter::Units::yard);
    BOOST_CHECK_CLOSE(yards, _yards, 0.001);

    double _chains = LengthConverter::convert(metres, LengthConverter::Units::metre, LengthConverter::Units::chain);
    BOOST_CHECK_CLOSE(chains, _chains, 0.001);

    double _furlongs = LengthConverter::convert(metres, LengthConverter::Units::metre, LengthConverter::Units::furlong);
    BOOST_CHECK_CLOSE(furlongs, _furlongs, 0.001);

    double _miles = LengthConverter::convert(metres, LengthConverter::Units::metre, LengthConverter::Units::mile);
    BOOST_CHECK_CLOSE(miles, _miles, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_decimetres, LengthConverterTest)
{
    double _metres = LengthConverter::convert(decimetres, LengthConverter::Units::decimetre, LengthConverter::Units::metre);
    BOOST_CHECK_EQUAL(metres, _metres);

    double _centimetres = LengthConverter::convert(decimetres, LengthConverter::Units::decimetre, LengthConverter::Units::centimetre);
    BOOST_CHECK_EQUAL(centimetres, _centimetres);

    double _millimetres = LengthConverter::convert(decimetres, LengthConverter::Units::decimetre, LengthConverter::Units::millimetre);
    BOOST_CHECK_EQUAL(millimetres, _millimetres);

    double _micrometres = LengthConverter::convert(decimetres, LengthConverter::Units::decimetre, LengthConverter::Units::micrometre);
    BOOST_CHECK_EQUAL(micrometres, _micrometres);

    double _nanometres = LengthConverter::convert(decimetres, LengthConverter::Units::decimetre, LengthConverter::Units::nanometre);
    BOOST_CHECK_EQUAL(nanometres, _nanometres);

    double _kilometres = LengthConverter::convert(decimetres, LengthConverter::Units::decimetre, LengthConverter::Units::kilometre);
    BOOST_CHECK_EQUAL(kilometres, _kilometres);

    double _hectometres = LengthConverter::convert(decimetres, LengthConverter::Units::decimetre, LengthConverter::Units::hectometre);
    BOOST_CHECK_EQUAL(hectometres, _hectometres);

    double _decametres = LengthConverter::convert(decimetres, LengthConverter::Units::decimetre, LengthConverter::Units::decametre);
    BOOST_CHECK_CLOSE(decametres, _decametres, 0.001);

    double _inches = LengthConverter::convert(decimetres, LengthConverter::Units::decimetre, LengthConverter::Units::inch);
    BOOST_CHECK_CLOSE(inches, _inches, 0.001);

    double _feet = LengthConverter::convert(decimetres, LengthConverter::Units::decimetre, LengthConverter::Units::foot);
    BOOST_CHECK_CLOSE(feet, _feet, 0.001);

    double _yards = LengthConverter::convert(decimetres, LengthConverter::Units::decimetre, LengthConverter::Units::yard);
    BOOST_CHECK_CLOSE(yards, _yards, 0.001);

    double _chains = LengthConverter::convert(decimetres, LengthConverter::Units::decimetre, LengthConverter::Units::chain);
    BOOST_CHECK_CLOSE(chains, _chains, 0.001);

    double _furlongs = LengthConverter::convert(decimetres, LengthConverter::Units::decimetre, LengthConverter::Units::furlong);
    BOOST_CHECK_CLOSE(furlongs, _furlongs, 0.001);

    double _miles = LengthConverter::convert(decimetres, LengthConverter::Units::decimetre, LengthConverter::Units::mile);
    BOOST_CHECK_CLOSE(miles, _miles, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_centimetres, LengthConverterTest)
{
    double _metres = LengthConverter::convert(centimetres, LengthConverter::Units::centimetre, LengthConverter::Units::metre);
    BOOST_CHECK_EQUAL(metres, _metres);

    double _decimetres = LengthConverter::convert(centimetres, LengthConverter::Units::centimetre, LengthConverter::Units::decimetre);
    BOOST_CHECK_EQUAL(decimetres, _decimetres);

    double _millimetres = LengthConverter::convert(centimetres, LengthConverter::Units::centimetre, LengthConverter::Units::millimetre);
    BOOST_CHECK_EQUAL(millimetres, _millimetres);

    double _micrometres = LengthConverter::convert(centimetres, LengthConverter::Units::centimetre, LengthConverter::Units::micrometre);
    BOOST_CHECK_EQUAL(micrometres, _micrometres);

    double _nanometres = LengthConverter::convert(centimetres, LengthConverter::Units::centimetre, LengthConverter::Units::nanometre);
    BOOST_CHECK_EQUAL(nanometres, _nanometres);

    double _kilometres = LengthConverter::convert(centimetres, LengthConverter::Units::centimetre, LengthConverter::Units::kilometre);
    BOOST_CHECK_CLOSE(kilometres, _kilometres, 0.001);

    double _hectometres = LengthConverter::convert(centimetres, LengthConverter::Units::centimetre, LengthConverter::Units::hectometre);
    BOOST_CHECK_EQUAL(hectometres, _hectometres);

    double _decametres = LengthConverter::convert(centimetres, LengthConverter::Units::centimetre, LengthConverter::Units::decametre);
    BOOST_CHECK_CLOSE(decametres, _decametres, 0.001);

    double _inches = LengthConverter::convert(centimetres, LengthConverter::Units::centimetre, LengthConverter::Units::inch);
    BOOST_CHECK_CLOSE(inches, _inches, 0.001);

    double _feet = LengthConverter::convert(centimetres, LengthConverter::Units::centimetre, LengthConverter::Units::foot);
    BOOST_CHECK_CLOSE(feet, _feet, 0.001);

    double _yards = LengthConverter::convert(centimetres, LengthConverter::Units::centimetre, LengthConverter::Units::yard);
    BOOST_CHECK_CLOSE(yards, _yards, 0.001);

    double _chains = LengthConverter::convert(centimetres, LengthConverter::Units::centimetre, LengthConverter::Units::chain);
    BOOST_CHECK_CLOSE(chains, _chains, 0.001);

    double _furlongs = LengthConverter::convert(centimetres, LengthConverter::Units::centimetre, LengthConverter::Units::furlong);
    BOOST_CHECK_CLOSE(furlongs, _furlongs, 0.001);

    double _miles = LengthConverter::convert(centimetres, LengthConverter::Units::centimetre, LengthConverter::Units::mile);
    BOOST_CHECK_CLOSE(miles, _miles, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_millimetres, LengthConverterTest)
{
    double _metres = LengthConverter::convert(millimetres, LengthConverter::Units::millimetre, LengthConverter::Units::metre);
    BOOST_CHECK_EQUAL(metres, _metres);

    double _centimetres = LengthConverter::convert(millimetres, LengthConverter::Units::millimetre, LengthConverter::Units::centimetre);
    BOOST_CHECK_EQUAL(centimetres, _centimetres);

    double _decimetres = LengthConverter::convert(millimetres, LengthConverter::Units::millimetre, LengthConverter::Units::decimetre);
    BOOST_CHECK_EQUAL(decimetres, _decimetres);

    double _micrometres = LengthConverter::convert(millimetres, LengthConverter::Units::millimetre, LengthConverter::Units::micrometre);
    BOOST_CHECK_EQUAL(micrometres, _micrometres);

    double _nanometres = LengthConverter::convert(millimetres, LengthConverter::Units::millimetre, LengthConverter::Units::nanometre);
    BOOST_CHECK_EQUAL(nanometres, _nanometres);

    double _kilometres = LengthConverter::convert(millimetres, LengthConverter::Units::millimetre, LengthConverter::Units::kilometre);
    BOOST_CHECK_CLOSE(kilometres, _kilometres, 0.001);

    double _hectometres = LengthConverter::convert(millimetres, LengthConverter::Units::millimetre, LengthConverter::Units::hectometre);
    BOOST_CHECK_EQUAL(hectometres, _hectometres);

    double _decametres = LengthConverter::convert(millimetres, LengthConverter::Units::millimetre, LengthConverter::Units::decametre);
    BOOST_CHECK_CLOSE(decametres, _decametres, 0.001);

    double _inches = LengthConverter::convert(millimetres, LengthConverter::Units::millimetre, LengthConverter::Units::inch);
    BOOST_CHECK_CLOSE(inches, _inches, 0.001);

    double _feet = LengthConverter::convert(millimetres, LengthConverter::Units::millimetre, LengthConverter::Units::foot);
    BOOST_CHECK_CLOSE(feet, _feet, 0.001);

    double _yards = LengthConverter::convert(millimetres, LengthConverter::Units::millimetre, LengthConverter::Units::yard);
    BOOST_CHECK_CLOSE(yards, _yards, 0.001);

    double _chains = LengthConverter::convert(millimetres, LengthConverter::Units::millimetre, LengthConverter::Units::chain);
    BOOST_CHECK_CLOSE(chains, _chains, 0.001);

    double _furlongs = LengthConverter::convert(millimetres, LengthConverter::Units::millimetre, LengthConverter::Units::furlong);
    BOOST_CHECK_CLOSE(furlongs, _furlongs, 0.001);

    double _miles = LengthConverter::convert(millimetres, LengthConverter::Units::millimetre, LengthConverter::Units::mile);
    BOOST_CHECK_CLOSE(miles, _miles, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_micrometres, LengthConverterTest)
{
    double _metres = LengthConverter::convert(micrometres, LengthConverter::Units::micrometre, LengthConverter::Units::metre);
    BOOST_CHECK_CLOSE(metres, _metres, 0.001);

    double _centimetres = LengthConverter::convert(micrometres, LengthConverter::Units::micrometre, LengthConverter::Units::centimetre);
    BOOST_CHECK_EQUAL(centimetres, _centimetres);

    double _decimetres = LengthConverter::convert(micrometres, LengthConverter::Units::micrometre, LengthConverter::Units::decimetre);
    BOOST_CHECK_EQUAL(decimetres, _decimetres);

    double _millimetres = LengthConverter::convert(micrometres, LengthConverter::Units::micrometre, LengthConverter::Units::millimetre);
    BOOST_CHECK_EQUAL(millimetres, _millimetres);

    double _nanometres = LengthConverter::convert(micrometres, LengthConverter::Units::micrometre, LengthConverter::Units::nanometre);
    BOOST_CHECK_EQUAL(nanometres, _nanometres);

    double _kilometres = LengthConverter::convert(micrometres, LengthConverter::Units::micrometre, LengthConverter::Units::kilometre);
    BOOST_CHECK_CLOSE(kilometres, _kilometres, 0.001);

    double _hectometres = LengthConverter::convert(micrometres, LengthConverter::Units::micrometre, LengthConverter::Units::hectometre);
    BOOST_CHECK_EQUAL(hectometres, _hectometres);

    double _decametres = LengthConverter::convert(micrometres, LengthConverter::Units::micrometre, LengthConverter::Units::decametre);
    BOOST_CHECK_CLOSE(decametres, _decametres, 0.001);

    double _inches = LengthConverter::convert(micrometres, LengthConverter::Units::micrometre, LengthConverter::Units::inch);
    BOOST_CHECK_CLOSE(inches, _inches, 0.001);

    double _feet = LengthConverter::convert(micrometres, LengthConverter::Units::micrometre, LengthConverter::Units::foot);
    BOOST_CHECK_CLOSE(feet, _feet, 0.001);

    double _yards = LengthConverter::convert(micrometres, LengthConverter::Units::micrometre, LengthConverter::Units::yard);
    BOOST_CHECK_CLOSE(yards, _yards, 0.001);

    double _chains = LengthConverter::convert(micrometres, LengthConverter::Units::micrometre, LengthConverter::Units::chain);
    BOOST_CHECK_CLOSE(chains, _chains, 0.001);

    double _furlongs = LengthConverter::convert(micrometres, LengthConverter::Units::micrometre, LengthConverter::Units::furlong);
    BOOST_CHECK_CLOSE(furlongs, _furlongs, 0.001);

    double _miles = LengthConverter::convert(micrometres, LengthConverter::Units::micrometre, LengthConverter::Units::mile);
    BOOST_CHECK_CLOSE(miles, _miles, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_nanometres, LengthConverterTest)
{
    double _metres = LengthConverter::convert(nanometres, LengthConverter::Units::nanometre, LengthConverter::Units::metre);
    BOOST_CHECK_CLOSE(metres, _metres, 0.001);

    double _centimetres = LengthConverter::convert(nanometres, LengthConverter::Units::nanometre, LengthConverter::Units::centimetre);
    BOOST_CHECK_CLOSE(centimetres, _centimetres, 0.001);

    double _decimetres = LengthConverter::convert(nanometres, LengthConverter::Units::nanometre, LengthConverter::Units::decimetre);
    BOOST_CHECK_CLOSE(decimetres, _decimetres, 0.001);

    double _millimetres = LengthConverter::convert(nanometres, LengthConverter::Units::nanometre, LengthConverter::Units::millimetre);
    BOOST_CHECK_CLOSE(millimetres, _millimetres, 0.001);

    double _micrometres = LengthConverter::convert(nanometres, LengthConverter::Units::nanometre, LengthConverter::Units::micrometre);
    BOOST_CHECK_EQUAL(micrometres, _micrometres);

    double _kilometres = LengthConverter::convert(nanometres, LengthConverter::Units::nanometre, LengthConverter::Units::kilometre);
    BOOST_CHECK_CLOSE(kilometres, _kilometres, 0.001);

    double _hectometres = LengthConverter::convert(nanometres, LengthConverter::Units::nanometre, LengthConverter::Units::hectometre);
    BOOST_CHECK_EQUAL(hectometres, _hectometres);

    double _decametres = LengthConverter::convert(nanometres, LengthConverter::Units::nanometre, LengthConverter::Units::decametre);
    BOOST_CHECK_CLOSE(decametres, _decametres, 0.001);

    double _inches = LengthConverter::convert(nanometres, LengthConverter::Units::nanometre, LengthConverter::Units::inch);
    BOOST_CHECK_CLOSE(inches, _inches, 0.001);

    double _feet = LengthConverter::convert(nanometres, LengthConverter::Units::nanometre, LengthConverter::Units::foot);
    BOOST_CHECK_CLOSE(feet, _feet, 0.001);

    double _yards = LengthConverter::convert(nanometres, LengthConverter::Units::nanometre, LengthConverter::Units::yard);
    BOOST_CHECK_CLOSE(yards, _yards, 0.001);

    double _chains = LengthConverter::convert(nanometres, LengthConverter::Units::nanometre, LengthConverter::Units::chain);
    BOOST_CHECK_CLOSE(chains, _chains, 0.001);

    double _furlongs = LengthConverter::convert(nanometres, LengthConverter::Units::nanometre, LengthConverter::Units::furlong);
    BOOST_CHECK_CLOSE(furlongs, _furlongs, 0.001);

    double _miles = LengthConverter::convert(nanometres, LengthConverter::Units::nanometre, LengthConverter::Units::mile);
    BOOST_CHECK_CLOSE(miles, _miles, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_kilometres, LengthConverterTest)
{
    double _metres = LengthConverter::convert(kilometres, LengthConverter::Units::kilometre, LengthConverter::Units::metre);
    BOOST_CHECK_CLOSE(metres, _metres, 0.001);

    double _centimetres = LengthConverter::convert(kilometres, LengthConverter::Units::kilometre, LengthConverter::Units::centimetre);
    BOOST_CHECK_CLOSE(centimetres, _centimetres, 0.001);

    double _decimetres = LengthConverter::convert(kilometres, LengthConverter::Units::kilometre, LengthConverter::Units::decimetre);
    BOOST_CHECK_CLOSE(decimetres, _decimetres, 0.001);

    double _millimetres = LengthConverter::convert(kilometres, LengthConverter::Units::kilometre, LengthConverter::Units::millimetre);
    BOOST_CHECK_CLOSE(millimetres, _millimetres, 0.001);

    double _micrometres = LengthConverter::convert(kilometres, LengthConverter::Units::kilometre, LengthConverter::Units::micrometre);
    BOOST_CHECK_EQUAL(micrometres, _micrometres);

    double _nanometres = LengthConverter::convert(kilometres, LengthConverter::Units::kilometre, LengthConverter::Units::nanometre);
    BOOST_CHECK_CLOSE(nanometres, _nanometres, 0.001);

    double _hectometres = LengthConverter::convert(kilometres, LengthConverter::Units::kilometre, LengthConverter::Units::hectometre);
    BOOST_CHECK_EQUAL(hectometres, _hectometres);

    double _decametres = LengthConverter::convert(kilometres, LengthConverter::Units::kilometre, LengthConverter::Units::decametre);
    BOOST_CHECK_CLOSE(decametres, _decametres, 0.001);

    double _inches = LengthConverter::convert(kilometres, LengthConverter::Units::kilometre, LengthConverter::Units::inch);
    BOOST_CHECK_CLOSE(inches, _inches, 0.001);

    double _feet = LengthConverter::convert(kilometres, LengthConverter::Units::kilometre, LengthConverter::Units::foot);
    BOOST_CHECK_CLOSE(feet, _feet, 0.001);

    double _yards = LengthConverter::convert(kilometres, LengthConverter::Units::kilometre, LengthConverter::Units::yard);
    BOOST_CHECK_CLOSE(yards, _yards, 0.001);

    double _chains = LengthConverter::convert(kilometres, LengthConverter::Units::kilometre, LengthConverter::Units::chain);
    BOOST_CHECK_CLOSE(chains, _chains, 0.001);

    double _furlongs = LengthConverter::convert(kilometres, LengthConverter::Units::kilometre, LengthConverter::Units::furlong);
    BOOST_CHECK_CLOSE(furlongs, _furlongs, 0.001);

    double _miles = LengthConverter::convert(kilometres, LengthConverter::Units::kilometre, LengthConverter::Units::mile);
    BOOST_CHECK_CLOSE(miles, _miles, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_hectometres, LengthConverterTest)
{
    double _metres = LengthConverter::convert(hectometres, LengthConverter::Units::hectometre, LengthConverter::Units::metre);
    BOOST_CHECK_CLOSE(metres, _metres, 0.001);

    double _centimetres = LengthConverter::convert(hectometres, LengthConverter::Units::hectometre, LengthConverter::Units::centimetre);
    BOOST_CHECK_CLOSE(centimetres, _centimetres, 0.001);

    double _decimetres = LengthConverter::convert(hectometres, LengthConverter::Units::hectometre, LengthConverter::Units::decimetre);
    BOOST_CHECK_CLOSE(decimetres, _decimetres, 0.001);

    double _millimetres = LengthConverter::convert(hectometres, LengthConverter::Units::hectometre, LengthConverter::Units::millimetre);
    BOOST_CHECK_CLOSE(millimetres, _millimetres, 0.001);

    double _micrometres = LengthConverter::convert(hectometres, LengthConverter::Units::hectometre, LengthConverter::Units::micrometre);
    BOOST_CHECK_CLOSE(micrometres, _micrometres, 0.001);

    double _nanometres = LengthConverter::convert(hectometres, LengthConverter::Units::hectometre, LengthConverter::Units::nanometre);
    BOOST_CHECK_CLOSE(nanometres, _nanometres, 0.001);

    double _kilometres = LengthConverter::convert(hectometres, LengthConverter::Units::hectometre, LengthConverter::Units::kilometre);
    BOOST_CHECK_CLOSE(kilometres, _kilometres, 0.001);

    double _decametres = LengthConverter::convert(hectometres, LengthConverter::Units::hectometre, LengthConverter::Units::decametre);
    BOOST_CHECK_CLOSE(decametres, _decametres, 0.001);

    double _inches = LengthConverter::convert(hectometres, LengthConverter::Units::hectometre, LengthConverter::Units::inch);
    BOOST_CHECK_CLOSE(inches, _inches, 0.001);

    double _feet = LengthConverter::convert(hectometres, LengthConverter::Units::hectometre, LengthConverter::Units::foot);
    BOOST_CHECK_CLOSE(feet, _feet, 0.001);

    double _yards = LengthConverter::convert(hectometres, LengthConverter::Units::hectometre, LengthConverter::Units::yard);
    BOOST_CHECK_CLOSE(yards, _yards, 0.001);

    double _chains = LengthConverter::convert(hectometres, LengthConverter::Units::hectometre, LengthConverter::Units::chain);
    BOOST_CHECK_CLOSE(chains, _chains, 0.001);

    double _furlongs = LengthConverter::convert(hectometres, LengthConverter::Units::hectometre, LengthConverter::Units::furlong);
    BOOST_CHECK_CLOSE(furlongs, _furlongs, 0.001);

    double _miles = LengthConverter::convert(hectometres, LengthConverter::Units::hectometre, LengthConverter::Units::mile);
    BOOST_CHECK_CLOSE(miles, _miles, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_decametres, LengthConverterTest)
{
    double _metres = LengthConverter::convert(decametres, LengthConverter::Units::decametre, LengthConverter::Units::metre);
    BOOST_CHECK_CLOSE(metres, _metres, 0.001);

    double _centimetres = LengthConverter::convert(decametres, LengthConverter::Units::decametre, LengthConverter::Units::centimetre);
    BOOST_CHECK_CLOSE(centimetres, _centimetres, 0.001);

    double _decimetres = LengthConverter::convert(decametres, LengthConverter::Units::decametre, LengthConverter::Units::decimetre);
    BOOST_CHECK_CLOSE(decimetres, _decimetres, 0.001);

    double _millimetres = LengthConverter::convert(decametres, LengthConverter::Units::decametre, LengthConverter::Units::millimetre);
    BOOST_CHECK_CLOSE(millimetres, _millimetres, 0.001);

    double _micrometres = LengthConverter::convert(decametres, LengthConverter::Units::decametre, LengthConverter::Units::micrometre);
    BOOST_CHECK_CLOSE(micrometres, _micrometres, 0.001);

    double _nanometres = LengthConverter::convert(decametres, LengthConverter::Units::decametre, LengthConverter::Units::nanometre);
    BOOST_CHECK_CLOSE(nanometres, _nanometres, 0.001);

    double _kilometres = LengthConverter::convert(decametres, LengthConverter::Units::decametre, LengthConverter::Units::kilometre);
    BOOST_CHECK_CLOSE(kilometres, _kilometres, 0.001);

    double _hectometres = LengthConverter::convert(decametres, LengthConverter::Units::decametre, LengthConverter::Units::hectometre);
    BOOST_CHECK_CLOSE(hectometres, _hectometres, 0.001);

    double _inches = LengthConverter::convert(decametres, LengthConverter::Units::decametre, LengthConverter::Units::inch);
    BOOST_CHECK_CLOSE(inches, _inches, 0.001);

    double _feet = LengthConverter::convert(decametres, LengthConverter::Units::decametre, LengthConverter::Units::foot);
    BOOST_CHECK_CLOSE(feet, _feet, 0.001);

    double _yards = LengthConverter::convert(decametres, LengthConverter::Units::decametre, LengthConverter::Units::yard);
    BOOST_CHECK_CLOSE(yards, _yards, 0.001);

    double _chains = LengthConverter::convert(decametres, LengthConverter::Units::decametre, LengthConverter::Units::chain);
    BOOST_CHECK_CLOSE(chains, _chains, 0.001);

    double _furlongs = LengthConverter::convert(decametres, LengthConverter::Units::decametre, LengthConverter::Units::furlong);
    BOOST_CHECK_CLOSE(furlongs, _furlongs, 0.001);

    double _miles = LengthConverter::convert(decametres, LengthConverter::Units::decametre, LengthConverter::Units::mile);
    BOOST_CHECK_CLOSE(miles, _miles, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_inches, LengthConverterTest)
{
    double _metres = LengthConverter::convert(inches, LengthConverter::Units::inch, LengthConverter::Units::metre);
    BOOST_CHECK_CLOSE(metres, _metres, 0.001);

    double _centimetres = LengthConverter::convert(inches, LengthConverter::Units::inch, LengthConverter::Units::centimetre);
    BOOST_CHECK_CLOSE(centimetres, _centimetres, 0.001);

    double _decimetres = LengthConverter::convert(inches, LengthConverter::Units::inch, LengthConverter::Units::decimetre);
    BOOST_CHECK_CLOSE(decimetres, _decimetres, 0.001);

    double _millimetres = LengthConverter::convert(inches, LengthConverter::Units::inch, LengthConverter::Units::millimetre);
    BOOST_CHECK_CLOSE(millimetres, _millimetres, 0.001);

    double _micrometres = LengthConverter::convert(inches, LengthConverter::Units::inch, LengthConverter::Units::micrometre);
    BOOST_CHECK_CLOSE(micrometres, _micrometres, 0.001);

    double _nanometres = LengthConverter::convert(inches, LengthConverter::Units::inch, LengthConverter::Units::nanometre);
    BOOST_CHECK_CLOSE(nanometres, _nanometres, 0.001);

    double _kilometres = LengthConverter::convert(inches, LengthConverter::Units::inch, LengthConverter::Units::kilometre);
    BOOST_CHECK_CLOSE(kilometres, _kilometres, 0.001);

    double _hectometres = LengthConverter::convert(inches, LengthConverter::Units::inch, LengthConverter::Units::hectometre);
    BOOST_CHECK_CLOSE(hectometres, _hectometres, 0.001);

    double _decametres = LengthConverter::convert(inches, LengthConverter::Units::inch, LengthConverter::Units::decametre);
    BOOST_CHECK_CLOSE(decametres, _decametres, 0.001);

    double _feet = LengthConverter::convert(inches, LengthConverter::Units::inch, LengthConverter::Units::foot);
    BOOST_CHECK_CLOSE(feet, _feet, 0.001);

    double _yards = LengthConverter::convert(inches, LengthConverter::Units::inch, LengthConverter::Units::yard);
    BOOST_CHECK_CLOSE(yards, _yards, 0.001);

    double _chains = LengthConverter::convert(inches, LengthConverter::Units::inch, LengthConverter::Units::chain);
    BOOST_CHECK_CLOSE(chains, _chains, 0.001);

    double _furlongs = LengthConverter::convert(inches, LengthConverter::Units::inch, LengthConverter::Units::furlong);
    BOOST_CHECK_CLOSE(furlongs, _furlongs, 0.001);

    double _miles = LengthConverter::convert(inches, LengthConverter::Units::inch, LengthConverter::Units::mile);
    BOOST_CHECK_CLOSE(miles, _miles, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_feet, LengthConverterTest)
{
    double _metres = LengthConverter::convert(feet, LengthConverter::Units::foot, LengthConverter::Units::metre);
    BOOST_CHECK_CLOSE(metres, _metres, 0.001);

    double _centimetres = LengthConverter::convert(feet, LengthConverter::Units::foot, LengthConverter::Units::centimetre);
    BOOST_CHECK_CLOSE(centimetres, _centimetres, 0.001);

    double _decimetres = LengthConverter::convert(feet, LengthConverter::Units::foot, LengthConverter::Units::decimetre);
    BOOST_CHECK_CLOSE(decimetres, _decimetres, 0.001);

    double _millimetres = LengthConverter::convert(feet, LengthConverter::Units::foot, LengthConverter::Units::millimetre);
    BOOST_CHECK_CLOSE(millimetres, _millimetres, 0.001);

    double _micrometres = LengthConverter::convert(feet, LengthConverter::Units::foot, LengthConverter::Units::micrometre);
    BOOST_CHECK_CLOSE(micrometres, _micrometres, 0.001);

    double _nanometres = LengthConverter::convert(feet, LengthConverter::Units::foot, LengthConverter::Units::nanometre);
    BOOST_CHECK_CLOSE(nanometres, _nanometres, 0.001);

    double _kilometres = LengthConverter::convert(feet, LengthConverter::Units::foot, LengthConverter::Units::kilometre);
    BOOST_CHECK_CLOSE(kilometres, _kilometres, 0.001);

    double _hectometres = LengthConverter::convert(feet, LengthConverter::Units::foot, LengthConverter::Units::hectometre);
    BOOST_CHECK_CLOSE(hectometres, _hectometres, 0.001);

    double _decametres = LengthConverter::convert(feet, LengthConverter::Units::foot, LengthConverter::Units::decametre);
    BOOST_CHECK_CLOSE(decametres, _decametres, 0.001);

    double _inches = LengthConverter::convert(feet, LengthConverter::Units::foot, LengthConverter::Units::inch);
    BOOST_CHECK_CLOSE(inches, _inches, 0.001);

    double _yards = LengthConverter::convert(feet, LengthConverter::Units::foot, LengthConverter::Units::yard);
    BOOST_CHECK_CLOSE(yards, _yards, 0.001);

    double _chains = LengthConverter::convert(feet, LengthConverter::Units::foot, LengthConverter::Units::chain);
    BOOST_CHECK_CLOSE(chains, _chains, 0.001);

    double _furlongs = LengthConverter::convert(feet, LengthConverter::Units::foot, LengthConverter::Units::furlong);
    BOOST_CHECK_CLOSE(furlongs, _furlongs, 0.001);

    double _miles = LengthConverter::convert(feet, LengthConverter::Units::foot, LengthConverter::Units::mile);
    BOOST_CHECK_CLOSE(miles, _miles, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_yards, LengthConverterTest)
{
    double _metres = LengthConverter::convert(yards, LengthConverter::Units::yard, LengthConverter::Units::metre);
    BOOST_CHECK_CLOSE(metres, _metres, 0.001);

    double _centimetres = LengthConverter::convert(yards, LengthConverter::Units::yard, LengthConverter::Units::centimetre);
    BOOST_CHECK_CLOSE(centimetres, _centimetres, 0.001);

    double _decimetres = LengthConverter::convert(yards, LengthConverter::Units::yard, LengthConverter::Units::decimetre);
    BOOST_CHECK_CLOSE(decimetres, _decimetres, 0.001);

    double _millimetres = LengthConverter::convert(yards, LengthConverter::Units::yard, LengthConverter::Units::millimetre);
    BOOST_CHECK_CLOSE(millimetres, _millimetres, 0.001);

    double _micrometres = LengthConverter::convert(yards, LengthConverter::Units::yard, LengthConverter::Units::micrometre);
    BOOST_CHECK_CLOSE(micrometres, _micrometres, 0.001);

    double _nanometres = LengthConverter::convert(yards, LengthConverter::Units::yard, LengthConverter::Units::nanometre);
    BOOST_CHECK_CLOSE(nanometres, _nanometres, 0.001);

    double _kilometres = LengthConverter::convert(yards, LengthConverter::Units::yard, LengthConverter::Units::kilometre);
    BOOST_CHECK_CLOSE(kilometres, _kilometres, 0.001);

    double _hectometres = LengthConverter::convert(yards, LengthConverter::Units::yard, LengthConverter::Units::hectometre);
    BOOST_CHECK_CLOSE(hectometres, _hectometres, 0.001);

    double _decametres = LengthConverter::convert(yards, LengthConverter::Units::yard, LengthConverter::Units::decametre);
    BOOST_CHECK_CLOSE(decametres, _decametres, 0.001);

    double _inches = LengthConverter::convert(yards, LengthConverter::Units::yard, LengthConverter::Units::inch);
    BOOST_CHECK_CLOSE(inches, _inches, 0.001);

    double _feet = LengthConverter::convert(yards, LengthConverter::Units::yard, LengthConverter::Units::foot);
    BOOST_CHECK_CLOSE(feet, _feet, 0.001);

    double _chains = LengthConverter::convert(yards, LengthConverter::Units::yard, LengthConverter::Units::chain);
    BOOST_CHECK_CLOSE(chains, _chains, 0.001);

    double _furlongs = LengthConverter::convert(yards, LengthConverter::Units::yard, LengthConverter::Units::furlong);
    BOOST_CHECK_CLOSE(furlongs, _furlongs, 0.001);

    double _miles = LengthConverter::convert(yards, LengthConverter::Units::yard, LengthConverter::Units::mile);
    BOOST_CHECK_CLOSE(miles, _miles, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_chains, LengthConverterTest)
{
    double _metres = LengthConverter::convert(chains, LengthConverter::Units::chain, LengthConverter::Units::metre);
    BOOST_CHECK_CLOSE(metres, _metres, 0.001);

    double _centimetres = LengthConverter::convert(chains, LengthConverter::Units::chain, LengthConverter::Units::centimetre);
    BOOST_CHECK_CLOSE(centimetres, _centimetres, 0.001);

    double _decimetres = LengthConverter::convert(chains, LengthConverter::Units::chain, LengthConverter::Units::decimetre);
    BOOST_CHECK_CLOSE(decimetres, _decimetres, 0.001);

    double _millimetres = LengthConverter::convert(chains, LengthConverter::Units::chain, LengthConverter::Units::millimetre);
    BOOST_CHECK_CLOSE(millimetres, _millimetres, 0.001);

    double _micrometres = LengthConverter::convert(chains, LengthConverter::Units::chain, LengthConverter::Units::micrometre);
    BOOST_CHECK_CLOSE(micrometres, _micrometres, 0.001);

    double _nanometres = LengthConverter::convert(chains, LengthConverter::Units::chain, LengthConverter::Units::nanometre);
    BOOST_CHECK_CLOSE(nanometres, _nanometres, 0.001);

    double _kilometres = LengthConverter::convert(chains, LengthConverter::Units::chain, LengthConverter::Units::kilometre);
    BOOST_CHECK_CLOSE(kilometres, _kilometres, 0.001);

    double _hectometres = LengthConverter::convert(chains, LengthConverter::Units::chain, LengthConverter::Units::hectometre);
    BOOST_CHECK_CLOSE(hectometres, _hectometres, 0.001);

    double _decametres = LengthConverter::convert(chains, LengthConverter::Units::chain, LengthConverter::Units::decametre);
    BOOST_CHECK_CLOSE(decametres, _decametres, 0.001);

    double _inches = LengthConverter::convert(chains, LengthConverter::Units::chain, LengthConverter::Units::inch);
    BOOST_CHECK_CLOSE(inches, _inches, 0.001);

    double _feet = LengthConverter::convert(chains, LengthConverter::Units::chain, LengthConverter::Units::foot);
    BOOST_CHECK_CLOSE(feet, _feet, 0.001);

    double _yards = LengthConverter::convert(chains, LengthConverter::Units::chain, LengthConverter::Units::yard);
    BOOST_CHECK_CLOSE(yards, _yards, 0.001);

    double _furlongs = LengthConverter::convert(chains, LengthConverter::Units::chain, LengthConverter::Units::furlong);
    BOOST_CHECK_CLOSE(furlongs, _furlongs, 0.001);

    double _miles = LengthConverter::convert(chains, LengthConverter::Units::chain, LengthConverter::Units::mile);
    BOOST_CHECK_CLOSE(miles, _miles, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_furlongs, LengthConverterTest)
{
    double _metres = LengthConverter::convert(furlongs, LengthConverter::Units::furlong, LengthConverter::Units::metre);
    BOOST_CHECK_CLOSE(metres, _metres, 0.001);

    double _centimetres = LengthConverter::convert(furlongs, LengthConverter::Units::furlong, LengthConverter::Units::centimetre);
    BOOST_CHECK_CLOSE(centimetres, _centimetres, 0.001);

    double _decimetres = LengthConverter::convert(furlongs, LengthConverter::Units::furlong, LengthConverter::Units::decimetre);
    BOOST_CHECK_CLOSE(decimetres, _decimetres, 0.001);

    double _millimetres = LengthConverter::convert(furlongs, LengthConverter::Units::furlong, LengthConverter::Units::millimetre);
    BOOST_CHECK_CLOSE(millimetres, _millimetres, 0.001);

    double _micrometres = LengthConverter::convert(furlongs, LengthConverter::Units::furlong, LengthConverter::Units::micrometre);
    BOOST_CHECK_CLOSE(micrometres, _micrometres, 0.001);

    double _nanometres = LengthConverter::convert(furlongs, LengthConverter::Units::furlong, LengthConverter::Units::nanometre);
    BOOST_CHECK_CLOSE(nanometres, _nanometres, 0.001);

    double _kilometres = LengthConverter::convert(furlongs, LengthConverter::Units::furlong, LengthConverter::Units::kilometre);
    BOOST_CHECK_CLOSE(kilometres, _kilometres, 0.001);

    double _hectometres = LengthConverter::convert(furlongs, LengthConverter::Units::furlong, LengthConverter::Units::hectometre);
    BOOST_CHECK_CLOSE(hectometres, _hectometres, 0.001);

    double _decametres = LengthConverter::convert(furlongs, LengthConverter::Units::furlong, LengthConverter::Units::decametre);
    BOOST_CHECK_CLOSE(decametres, _decametres, 0.001);

    double _inches = LengthConverter::convert(furlongs, LengthConverter::Units::furlong, LengthConverter::Units::inch);
    BOOST_CHECK_CLOSE(inches, _inches, 0.001);

    double _feet = LengthConverter::convert(furlongs, LengthConverter::Units::furlong, LengthConverter::Units::foot);
    BOOST_CHECK_CLOSE(feet, _feet, 0.001);

    double _yards = LengthConverter::convert(furlongs, LengthConverter::Units::furlong, LengthConverter::Units::yard);
    BOOST_CHECK_CLOSE(yards, _yards, 0.001);

    double _chains = LengthConverter::convert(furlongs, LengthConverter::Units::furlong, LengthConverter::Units::chain);
    BOOST_CHECK_CLOSE(chains, _chains, 0.001);

    double _miles = LengthConverter::convert(furlongs, LengthConverter::Units::furlong, LengthConverter::Units::mile);
    BOOST_CHECK_CLOSE(miles, _miles, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_miles, LengthConverterTest)
{
    double _metres = LengthConverter::convert(miles, LengthConverter::Units::mile, LengthConverter::Units::metre);
    BOOST_CHECK_CLOSE(metres, _metres, 0.001);

    double _centimetres = LengthConverter::convert(miles, LengthConverter::Units::mile, LengthConverter::Units::centimetre);
    BOOST_CHECK_CLOSE(centimetres, _centimetres, 0.001);

    double _decimetres = LengthConverter::convert(miles, LengthConverter::Units::mile, LengthConverter::Units::decimetre);
    BOOST_CHECK_CLOSE(decimetres, _decimetres, 0.001);

    double _millimetres = LengthConverter::convert(miles, LengthConverter::Units::mile, LengthConverter::Units::millimetre);
    BOOST_CHECK_CLOSE(millimetres, _millimetres, 0.001);

    double _micrometres = LengthConverter::convert(miles, LengthConverter::Units::mile, LengthConverter::Units::micrometre);
    BOOST_CHECK_CLOSE(micrometres, _micrometres, 0.001);

    double _nanometres = LengthConverter::convert(miles, LengthConverter::Units::mile, LengthConverter::Units::nanometre);
    BOOST_CHECK_CLOSE(nanometres, _nanometres, 0.001);

    double _kilometres = LengthConverter::convert(miles, LengthConverter::Units::mile, LengthConverter::Units::kilometre);
    BOOST_CHECK_CLOSE(kilometres, _kilometres, 0.001);

    double _hectometres = LengthConverter::convert(miles, LengthConverter::Units::mile, LengthConverter::Units::hectometre);
    BOOST_CHECK_CLOSE(hectometres, _hectometres, 0.001);

    double _decametres = LengthConverter::convert(miles, LengthConverter::Units::mile, LengthConverter::Units::decametre);
    BOOST_CHECK_CLOSE(decametres, _decametres, 0.001);

    double _inches = LengthConverter::convert(miles, LengthConverter::Units::mile, LengthConverter::Units::inch);
    BOOST_CHECK_CLOSE(inches, _inches, 0.001);

    double _feet = LengthConverter::convert(miles, LengthConverter::Units::mile, LengthConverter::Units::foot);
    BOOST_CHECK_CLOSE(feet, _feet, 0.001);

    double _yards = LengthConverter::convert(miles, LengthConverter::Units::mile, LengthConverter::Units::yard);
    BOOST_CHECK_CLOSE(yards, _yards, 0.001);

    double _chains = LengthConverter::convert(miles, LengthConverter::Units::mile, LengthConverter::Units::chain);
    BOOST_CHECK_CLOSE(chains, _chains, 0.001);

    double _furlongs = LengthConverter::convert(miles, LengthConverter::Units::mile, LengthConverter::Units::furlong);
    BOOST_CHECK_CLOSE(furlongs, _furlongs, 0.001);
}


BOOST_AUTO_TEST_SUITE_END()



/* MassConverterTestSuite */

BOOST_AUTO_TEST_SUITE(MassConverterTestSuite)

struct MassConverterTest
{

    MassConverterTest()
    {

    }

    ~MassConverterTest()
    {

    }

    void setup()
    {

    }

    void teardown()
    {
    }

    double grams = 634.5;
    double decigrams = 6345.;
    double centigrams = 63450.;
    double milligrams = 634500;
    double micrograms = 6.345e+8;
    double nanograms = 6.345e+11;
    double kilograms = 0.6345;
    double hectograms = 6.345;
    double decagrams = 63.45;
    double grains = 9791.8313749;
    double ounces = 22.381328928000090883;
    double pounds = 1.39883305800000568;
    double stones = 0.099916647;
    double quarters = 0.0499583233;
    double hundredweights = 0.0124895808;
    double tons = 0.000624479;
};

BOOST_FIXTURE_TEST_CASE(from_grams, MassConverterTest)
{
    double _decigrams = MassConverter::convert(grams, MassConverter::Units::gram, MassConverter::Units::decigram);
    BOOST_CHECK_EQUAL(decigrams, _decigrams);

    double _centigrams = MassConverter::convert(grams, MassConverter::Units::gram, MassConverter::Units::centigram);
    BOOST_CHECK_EQUAL(centigrams, _centigrams);

    double _milligrams = MassConverter::convert(grams, MassConverter::Units::gram, MassConverter::Units::milligram);
    BOOST_CHECK_EQUAL(milligrams, _milligrams);

    double _micrograms = MassConverter::convert(grams, MassConverter::Units::gram, MassConverter::Units::microgram);
    BOOST_CHECK_EQUAL(micrograms, _micrograms);

    double _nanograms = MassConverter::convert(grams, MassConverter::Units::gram, MassConverter::Units::nanogram);
    BOOST_CHECK_EQUAL(nanograms, _nanograms);

    double _kilograms = MassConverter::convert(grams, MassConverter::Units::gram, MassConverter::Units::kilogram);
    BOOST_CHECK_CLOSE(kilograms, _kilograms, 0.001);

    double _hectograms = MassConverter::convert(grams, MassConverter::Units::gram, MassConverter::Units::hectogram);
    BOOST_CHECK_EQUAL(hectograms, _hectograms);

    double _decagrams = MassConverter::convert(grams, MassConverter::Units::gram, MassConverter::Units::decagram);
    BOOST_CHECK_EQUAL(decagrams, _decagrams);

    double _grains = MassConverter::convert(grams, MassConverter::Units::gram, MassConverter::Units::grain);
    BOOST_CHECK_CLOSE(grains, _grains, 0.001);

    double _ounces = MassConverter::convert(grams, MassConverter::Units::gram, MassConverter::Units::ounce);
    BOOST_CHECK_CLOSE(ounces, _ounces, 0.001);

    double _pounds = MassConverter::convert(grams, MassConverter::Units::gram, MassConverter::Units::pound);
    BOOST_CHECK_CLOSE(pounds, _pounds, 0.001);

    double _stones = MassConverter::convert(grams, MassConverter::Units::gram, MassConverter::Units::stone);
    BOOST_CHECK_CLOSE(stones, _stones, 0.001);

    double _quarters = MassConverter::convert(grams, MassConverter::Units::gram, MassConverter::Units::quarter);
    BOOST_CHECK_CLOSE(quarters, _quarters, 0.001);

    double _hundredweights = MassConverter::convert(grams, MassConverter::Units::gram, MassConverter::Units::hundredweight);
    BOOST_CHECK_CLOSE(hundredweights, _hundredweights, 0.001);

    double _tons = MassConverter::convert(grams, MassConverter::Units::gram, MassConverter::Units::ton);
    BOOST_CHECK_CLOSE(tons, _tons, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_decigrams, MassConverterTest)
{
    double _grams = MassConverter::convert(decigrams, MassConverter::Units::decigram, MassConverter::Units::gram);
    BOOST_CHECK_EQUAL(grams, _grams);

    double _centigrams = MassConverter::convert(decigrams, MassConverter::Units::decigram, MassConverter::Units::centigram);
    BOOST_CHECK_EQUAL(centigrams, _centigrams);

    double _milligrams = MassConverter::convert(decigrams, MassConverter::Units::decigram, MassConverter::Units::milligram);
    BOOST_CHECK_EQUAL(milligrams, _milligrams);

    double _micrograms = MassConverter::convert(decigrams, MassConverter::Units::decigram, MassConverter::Units::microgram);
    BOOST_CHECK_EQUAL(micrograms, _micrograms);

    double _nanograms = MassConverter::convert(decigrams, MassConverter::Units::decigram, MassConverter::Units::nanogram);
    BOOST_CHECK_EQUAL(nanograms, _nanograms);

    double _kilograms = MassConverter::convert(decigrams, MassConverter::Units::decigram, MassConverter::Units::kilogram);
    BOOST_CHECK_CLOSE(kilograms, _kilograms, 0.001);

    double _hectograms = MassConverter::convert(decigrams, MassConverter::Units::decigram, MassConverter::Units::hectogram);
    BOOST_CHECK_EQUAL(hectograms, _hectograms);

    double _decagrams = MassConverter::convert(decigrams, MassConverter::Units::decigram, MassConverter::Units::decagram);
    BOOST_CHECK_CLOSE(decagrams, _decagrams, 0.001);

    double _grains = MassConverter::convert(decigrams, MassConverter::Units::decigram, MassConverter::Units::grain);
    BOOST_CHECK_CLOSE(grains, _grains, 0.001);

    double _ounces = MassConverter::convert(decigrams, MassConverter::Units::decigram, MassConverter::Units::ounce);
    BOOST_CHECK_CLOSE(ounces, _ounces, 0.001);

    double _pounds = MassConverter::convert(decigrams, MassConverter::Units::decigram, MassConverter::Units::pound);
    BOOST_CHECK_CLOSE(pounds, _pounds, 0.001);

    double _stones = MassConverter::convert(decigrams, MassConverter::Units::decigram, MassConverter::Units::stone);
    BOOST_CHECK_CLOSE(stones, _stones, 0.001);

    double _quarters = MassConverter::convert(decigrams, MassConverter::Units::decigram, MassConverter::Units::quarter);
    BOOST_CHECK_CLOSE(quarters, _quarters, 0.001);

    double _hundredweights = MassConverter::convert(decigrams, MassConverter::Units::decigram, MassConverter::Units::hundredweight);
    BOOST_CHECK_CLOSE(hundredweights, _hundredweights, 0.001);

    double _tons = MassConverter::convert(decigrams, MassConverter::Units::decigram, MassConverter::Units::ton);
    BOOST_CHECK_CLOSE(tons, _tons, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_centigrams, MassConverterTest)
{
    double _grams = MassConverter::convert(centigrams, MassConverter::Units::centigram, MassConverter::Units::gram);
    BOOST_CHECK_EQUAL(grams, _grams);

    double _decigrams = MassConverter::convert(centigrams, MassConverter::Units::centigram, MassConverter::Units::decigram);
    BOOST_CHECK_EQUAL(decigrams, _decigrams);

    double _milligrams = MassConverter::convert(centigrams, MassConverter::Units::centigram, MassConverter::Units::milligram);
    BOOST_CHECK_EQUAL(milligrams, _milligrams);

    double _micrograms = MassConverter::convert(centigrams, MassConverter::Units::centigram, MassConverter::Units::microgram);
    BOOST_CHECK_EQUAL(micrograms, _micrograms);

    double _nanograms = MassConverter::convert(centigrams, MassConverter::Units::centigram, MassConverter::Units::nanogram);
    BOOST_CHECK_EQUAL(nanograms, _nanograms);

    double _kilograms = MassConverter::convert(centigrams, MassConverter::Units::centigram, MassConverter::Units::kilogram);
    BOOST_CHECK_CLOSE(kilograms, _kilograms, 0.001);

    double _hectograms = MassConverter::convert(centigrams, MassConverter::Units::centigram, MassConverter::Units::hectogram);
    BOOST_CHECK_CLOSE(hectograms, _hectograms, 0.001);

    double _decagrams = MassConverter::convert(centigrams, MassConverter::Units::centigram, MassConverter::Units::decagram);
    BOOST_CHECK_CLOSE(decagrams, _decagrams, 0.001);

    double _grains = MassConverter::convert(centigrams, MassConverter::Units::centigram, MassConverter::Units::grain);
    BOOST_CHECK_CLOSE(grains, _grains, 0.001);

    double _ounces = MassConverter::convert(centigrams, MassConverter::Units::centigram, MassConverter::Units::ounce);
    BOOST_CHECK_CLOSE(ounces, _ounces, 0.001);

    double _pounds = MassConverter::convert(centigrams, MassConverter::Units::centigram, MassConverter::Units::pound);
    BOOST_CHECK_CLOSE(pounds, _pounds, 0.001);

    double _stones = MassConverter::convert(centigrams, MassConverter::Units::centigram, MassConverter::Units::stone);
    BOOST_CHECK_CLOSE(stones, _stones, 0.001);

    double _quarters = MassConverter::convert(centigrams, MassConverter::Units::centigram, MassConverter::Units::quarter);
    BOOST_CHECK_CLOSE(quarters, _quarters, 0.001);

    double _hundredweights = MassConverter::convert(centigrams, MassConverter::Units::centigram, MassConverter::Units::hundredweight);
    BOOST_CHECK_CLOSE(hundredweights, _hundredweights, 0.001);

    double _tons = MassConverter::convert(centigrams, MassConverter::Units::centigram, MassConverter::Units::ton);
    BOOST_CHECK_CLOSE(tons, _tons, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_milligrams, MassConverterTest)
{
    double _grams = MassConverter::convert(milligrams, MassConverter::Units::milligram, MassConverter::Units::gram);
    BOOST_CHECK_EQUAL(grams, _grams);

    double _centigrams = MassConverter::convert(milligrams, MassConverter::Units::milligram, MassConverter::Units::centigram);
    BOOST_CHECK_EQUAL(centigrams, _centigrams);

    double _decigrams = MassConverter::convert(milligrams, MassConverter::Units::milligram, MassConverter::Units::decigram);
    BOOST_CHECK_EQUAL(decigrams, _decigrams);

    double _micrograms = MassConverter::convert(milligrams, MassConverter::Units::milligram, MassConverter::Units::microgram);
    BOOST_CHECK_EQUAL(micrograms, _micrograms);

    double _nanograms = MassConverter::convert(milligrams, MassConverter::Units::milligram, MassConverter::Units::nanogram);
    BOOST_CHECK_EQUAL(nanograms, _nanograms);

    double _kilograms = MassConverter::convert(milligrams, MassConverter::Units::milligram, MassConverter::Units::kilogram);
    BOOST_CHECK_CLOSE(kilograms, _kilograms, 0.001);

    double _hectograms = MassConverter::convert(milligrams, MassConverter::Units::milligram, MassConverter::Units::hectogram);
    BOOST_CHECK_CLOSE(hectograms, _hectograms, 0.001);

    double _decagrams = MassConverter::convert(milligrams, MassConverter::Units::milligram, MassConverter::Units::decagram);
    BOOST_CHECK_CLOSE(decagrams, _decagrams, 0.001);

    double _grains = MassConverter::convert(milligrams, MassConverter::Units::milligram, MassConverter::Units::grain);
    BOOST_CHECK_CLOSE(grains, _grains, 0.001);

    double _ounces = MassConverter::convert(milligrams, MassConverter::Units::milligram, MassConverter::Units::ounce);
    BOOST_CHECK_CLOSE(ounces, _ounces, 0.001);

    double _pounds = MassConverter::convert(milligrams, MassConverter::Units::milligram, MassConverter::Units::pound);
    BOOST_CHECK_CLOSE(pounds, _pounds, 0.001);

    double _stones = MassConverter::convert(milligrams, MassConverter::Units::milligram, MassConverter::Units::stone);
    BOOST_CHECK_CLOSE(stones, _stones, 0.001);

    double _quarters = MassConverter::convert(milligrams, MassConverter::Units::milligram, MassConverter::Units::quarter);
    BOOST_CHECK_CLOSE(quarters, _quarters, 0.001);

    double _hundredweights = MassConverter::convert(milligrams, MassConverter::Units::milligram, MassConverter::Units::hundredweight);
    BOOST_CHECK_CLOSE(hundredweights, _hundredweights, 0.001);

    double _tons = MassConverter::convert(milligrams, MassConverter::Units::milligram, MassConverter::Units::ton);
    BOOST_CHECK_CLOSE(tons, _tons, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_micrograms, MassConverterTest)
{
    double _grams = MassConverter::convert(micrograms, MassConverter::Units::microgram, MassConverter::Units::gram);
    BOOST_CHECK_CLOSE(grams, _grams, 0.001);

    double _centigrams = MassConverter::convert(micrograms, MassConverter::Units::microgram, MassConverter::Units::centigram);
    BOOST_CHECK_CLOSE(centigrams, _centigrams, 0.001);

    double _decigrams = MassConverter::convert(micrograms, MassConverter::Units::microgram, MassConverter::Units::decigram);
    BOOST_CHECK_CLOSE(decigrams, _decigrams, 0.001);

    double _milligrams = MassConverter::convert(micrograms, MassConverter::Units::microgram, MassConverter::Units::milligram);
    BOOST_CHECK_EQUAL(milligrams, _milligrams);

    double _nanograms = MassConverter::convert(micrograms, MassConverter::Units::microgram, MassConverter::Units::nanogram);
    BOOST_CHECK_EQUAL(nanograms, _nanograms);

    double _kilograms = MassConverter::convert(micrograms, MassConverter::Units::microgram, MassConverter::Units::kilogram);
    BOOST_CHECK_CLOSE(kilograms, _kilograms, 0.001);

    double _hectograms = MassConverter::convert(micrograms, MassConverter::Units::microgram, MassConverter::Units::hectogram);
    BOOST_CHECK_EQUAL(hectograms, _hectograms);

    double _decagrams = MassConverter::convert(micrograms, MassConverter::Units::microgram, MassConverter::Units::decagram);
    BOOST_CHECK_CLOSE(decagrams, _decagrams, 0.001);

    double _grains = MassConverter::convert(micrograms, MassConverter::Units::microgram, MassConverter::Units::grain);
    BOOST_CHECK_CLOSE(grains, _grains, 0.001);

    double _ounces = MassConverter::convert(micrograms, MassConverter::Units::microgram, MassConverter::Units::ounce);
    BOOST_CHECK_CLOSE(ounces, _ounces, 0.001);

    double _pounds = MassConverter::convert(micrograms, MassConverter::Units::microgram, MassConverter::Units::pound);
    BOOST_CHECK_CLOSE(pounds, _pounds, 0.001);

    double _stones = MassConverter::convert(micrograms, MassConverter::Units::microgram, MassConverter::Units::stone);
    BOOST_CHECK_CLOSE(stones, _stones, 0.001);

    double _quarters = MassConverter::convert(micrograms, MassConverter::Units::microgram, MassConverter::Units::quarter);
    BOOST_CHECK_CLOSE(quarters, _quarters, 0.001);

    double _hundredweights = MassConverter::convert(micrograms, MassConverter::Units::microgram, MassConverter::Units::hundredweight);
    BOOST_CHECK_CLOSE(hundredweights, _hundredweights, 0.001);

    double _tons = MassConverter::convert(micrograms, MassConverter::Units::microgram, MassConverter::Units::ton);
    BOOST_CHECK_CLOSE(tons, _tons, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_nanograms, MassConverterTest)
{
    double _grams = MassConverter::convert(nanograms, MassConverter::Units::nanogram, MassConverter::Units::gram);
    BOOST_CHECK_CLOSE(grams, _grams, 0.001);

    double _centigrams = MassConverter::convert(nanograms, MassConverter::Units::nanogram, MassConverter::Units::centigram);
    BOOST_CHECK_CLOSE(centigrams, _centigrams, 0.001);

    double _decigrams = MassConverter::convert(nanograms, MassConverter::Units::nanogram, MassConverter::Units::decigram);
    BOOST_CHECK_CLOSE(decigrams, _decigrams, 0.001);

    double _milligrams = MassConverter::convert(nanograms, MassConverter::Units::nanogram, MassConverter::Units::milligram);
    BOOST_CHECK_CLOSE(milligrams, _milligrams, 0.001);

    double _micrograms = MassConverter::convert(nanograms, MassConverter::Units::nanogram, MassConverter::Units::microgram);
    BOOST_CHECK_EQUAL(micrograms, _micrograms);

    double _kilograms = MassConverter::convert(nanograms, MassConverter::Units::nanogram, MassConverter::Units::kilogram);
    BOOST_CHECK_CLOSE(kilograms, _kilograms, 0.001);

    double _hectograms = MassConverter::convert(nanograms, MassConverter::Units::nanogram, MassConverter::Units::hectogram);
    BOOST_CHECK_CLOSE(hectograms, _hectograms, 0.001);

    double _decagrams = MassConverter::convert(nanograms, MassConverter::Units::nanogram, MassConverter::Units::decagram);
    BOOST_CHECK_CLOSE(decagrams, _decagrams, 0.001);

    double _grains = MassConverter::convert(nanograms, MassConverter::Units::nanogram, MassConverter::Units::grain);
    BOOST_CHECK_CLOSE(grains, _grains, 0.001);

    double _ounces = MassConverter::convert(nanograms, MassConverter::Units::nanogram, MassConverter::Units::ounce);
    BOOST_CHECK_CLOSE(ounces, _ounces, 0.001);

    double _pounds = MassConverter::convert(nanograms, MassConverter::Units::nanogram, MassConverter::Units::pound);
    BOOST_CHECK_CLOSE(pounds, _pounds, 0.001);

    double _stones = MassConverter::convert(nanograms, MassConverter::Units::nanogram, MassConverter::Units::stone);
    BOOST_CHECK_CLOSE(stones, _stones, 0.001);

    double _quarters = MassConverter::convert(nanograms, MassConverter::Units::nanogram, MassConverter::Units::quarter);
    BOOST_CHECK_CLOSE(quarters, _quarters, 0.001);

    double _hundredweights = MassConverter::convert(nanograms, MassConverter::Units::nanogram, MassConverter::Units::hundredweight);
    BOOST_CHECK_CLOSE(hundredweights, _hundredweights, 0.001);

    double _tons = MassConverter::convert(nanograms, MassConverter::Units::nanogram, MassConverter::Units::ton);
    BOOST_CHECK_CLOSE(tons, _tons, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_kilograms, MassConverterTest)
{
    double _grams = MassConverter::convert(kilograms, MassConverter::Units::kilogram, MassConverter::Units::gram);
    BOOST_CHECK_CLOSE(grams, _grams, 0.001);

    double _centigrams = MassConverter::convert(kilograms, MassConverter::Units::kilogram, MassConverter::Units::centigram);
    BOOST_CHECK_CLOSE(centigrams, _centigrams, 0.001);

    double _decigrams = MassConverter::convert(kilograms, MassConverter::Units::kilogram, MassConverter::Units::decigram);
    BOOST_CHECK_CLOSE(decigrams, _decigrams, 0.001);

    double _milligrams = MassConverter::convert(kilograms, MassConverter::Units::kilogram, MassConverter::Units::milligram);
    BOOST_CHECK_CLOSE(milligrams, _milligrams, 0.001);

    double _micrograms = MassConverter::convert(kilograms, MassConverter::Units::kilogram, MassConverter::Units::microgram);
    BOOST_CHECK_EQUAL(micrograms, _micrograms);

    double _nanograms = MassConverter::convert(kilograms, MassConverter::Units::kilogram, MassConverter::Units::nanogram);
    BOOST_CHECK_CLOSE(nanograms, _nanograms, 0.001);

    double _hectograms = MassConverter::convert(kilograms, MassConverter::Units::kilogram, MassConverter::Units::hectogram);
    BOOST_CHECK_EQUAL(hectograms, _hectograms);

    double _decagrams = MassConverter::convert(kilograms, MassConverter::Units::kilogram, MassConverter::Units::decagram);
    BOOST_CHECK_CLOSE(decagrams, _decagrams, 0.001);

    double _grains = MassConverter::convert(kilograms, MassConverter::Units::kilogram, MassConverter::Units::grain);
    BOOST_CHECK_CLOSE(grains, _grains, 0.001);

    double _ounces = MassConverter::convert(kilograms, MassConverter::Units::kilogram, MassConverter::Units::ounce);
    BOOST_CHECK_CLOSE(ounces, _ounces, 0.001);

    double _pounds = MassConverter::convert(kilograms, MassConverter::Units::kilogram, MassConverter::Units::pound);
    BOOST_CHECK_CLOSE(pounds, _pounds, 0.001);

    double _stones = MassConverter::convert(kilograms, MassConverter::Units::kilogram, MassConverter::Units::stone);
    BOOST_CHECK_CLOSE(stones, _stones, 0.001);

    double _quarters = MassConverter::convert(kilograms, MassConverter::Units::kilogram, MassConverter::Units::quarter);
    BOOST_CHECK_CLOSE(quarters, _quarters, 0.001);

    double _hundredweights = MassConverter::convert(kilograms, MassConverter::Units::kilogram, MassConverter::Units::hundredweight);
    BOOST_CHECK_CLOSE(hundredweights, _hundredweights, 0.001);

    double _tons = MassConverter::convert(kilograms, MassConverter::Units::kilogram, MassConverter::Units::ton);
    BOOST_CHECK_CLOSE(tons, _tons, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_hectograms, MassConverterTest)
{
    double _grams = MassConverter::convert(hectograms, MassConverter::Units::hectogram, MassConverter::Units::gram);
    BOOST_CHECK_CLOSE(grams, _grams, 0.001);

    double _centigrams = MassConverter::convert(hectograms, MassConverter::Units::hectogram, MassConverter::Units::centigram);
    BOOST_CHECK_CLOSE(centigrams, _centigrams, 0.001);

    double _decigrams = MassConverter::convert(hectograms, MassConverter::Units::hectogram, MassConverter::Units::decigram);
    BOOST_CHECK_CLOSE(decigrams, _decigrams, 0.001);

    double _milligrams = MassConverter::convert(hectograms, MassConverter::Units::hectogram, MassConverter::Units::milligram);
    BOOST_CHECK_CLOSE(milligrams, _milligrams, 0.001);

    double _micrograms = MassConverter::convert(hectograms, MassConverter::Units::hectogram, MassConverter::Units::microgram);
    BOOST_CHECK_CLOSE(micrograms, _micrograms, 0.001);

    double _nanograms = MassConverter::convert(hectograms, MassConverter::Units::hectogram, MassConverter::Units::nanogram);
    BOOST_CHECK_CLOSE(nanograms, _nanograms, 0.001);

    double _kilograms = MassConverter::convert(hectograms, MassConverter::Units::hectogram, MassConverter::Units::kilogram);
    BOOST_CHECK_CLOSE(kilograms, _kilograms, 0.001);

    double _decagrams = MassConverter::convert(hectograms, MassConverter::Units::hectogram, MassConverter::Units::decagram);
    BOOST_CHECK_CLOSE(decagrams, _decagrams, 0.001);

    double _grains = MassConverter::convert(hectograms, MassConverter::Units::hectogram, MassConverter::Units::grain);
    BOOST_CHECK_CLOSE(grains, _grains, 0.001);

    double _ounces = MassConverter::convert(hectograms, MassConverter::Units::hectogram, MassConverter::Units::ounce);
    BOOST_CHECK_CLOSE(ounces, _ounces, 0.001);

    double _pounds = MassConverter::convert(hectograms, MassConverter::Units::hectogram, MassConverter::Units::pound);
    BOOST_CHECK_CLOSE(pounds, _pounds, 0.001);

    double _stones = MassConverter::convert(hectograms, MassConverter::Units::hectogram, MassConverter::Units::stone);
    BOOST_CHECK_CLOSE(stones, _stones, 0.001);

    double _quarters = MassConverter::convert(hectograms, MassConverter::Units::hectogram, MassConverter::Units::quarter);
    BOOST_CHECK_CLOSE(quarters, _quarters, 0.001);

    double _hundredweights = MassConverter::convert(hectograms, MassConverter::Units::hectogram, MassConverter::Units::hundredweight);
    BOOST_CHECK_CLOSE(hundredweights, _hundredweights, 0.001);

    double _tons = MassConverter::convert(hectograms, MassConverter::Units::hectogram, MassConverter::Units::ton);
    BOOST_CHECK_CLOSE(tons, _tons, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_decagrams, MassConverterTest)
{
    double _grams = MassConverter::convert(decagrams, MassConverter::Units::decagram, MassConverter::Units::gram);
    BOOST_CHECK_CLOSE(grams, _grams, 0.001);

    double _centigrams = MassConverter::convert(decagrams, MassConverter::Units::decagram, MassConverter::Units::centigram);
    BOOST_CHECK_CLOSE(centigrams, _centigrams, 0.001);

    double _decigrams = MassConverter::convert(decagrams, MassConverter::Units::decagram, MassConverter::Units::decigram);
    BOOST_CHECK_CLOSE(decigrams, _decigrams, 0.001);

    double _milligrams = MassConverter::convert(decagrams, MassConverter::Units::decagram, MassConverter::Units::milligram);
    BOOST_CHECK_CLOSE(milligrams, _milligrams, 0.001);

    double _micrograms = MassConverter::convert(decagrams, MassConverter::Units::decagram, MassConverter::Units::microgram);
    BOOST_CHECK_CLOSE(micrograms, _micrograms, 0.001);

    double _nanograms = MassConverter::convert(decagrams, MassConverter::Units::decagram, MassConverter::Units::nanogram);
    BOOST_CHECK_CLOSE(nanograms, _nanograms, 0.001);

    double _kilograms = MassConverter::convert(decagrams, MassConverter::Units::decagram, MassConverter::Units::kilogram);
    BOOST_CHECK_CLOSE(kilograms, _kilograms, 0.001);

    double _hectograms = MassConverter::convert(decagrams, MassConverter::Units::decagram, MassConverter::Units::hectogram);
    BOOST_CHECK_CLOSE(hectograms, _hectograms, 0.001);

    double _grains = MassConverter::convert(decagrams, MassConverter::Units::decagram, MassConverter::Units::grain);
    BOOST_CHECK_CLOSE(grains, _grains, 0.001);

    double _ounces = MassConverter::convert(decagrams, MassConverter::Units::decagram, MassConverter::Units::ounce);
    BOOST_CHECK_CLOSE(ounces, _ounces, 0.001);

    double _pounds = MassConverter::convert(decagrams, MassConverter::Units::decagram, MassConverter::Units::pound);
    BOOST_CHECK_CLOSE(pounds, _pounds, 0.001);

    double _stones = MassConverter::convert(decagrams, MassConverter::Units::decagram, MassConverter::Units::stone);
    BOOST_CHECK_CLOSE(stones, _stones, 0.001);

    double _quarters = MassConverter::convert(decagrams, MassConverter::Units::decagram, MassConverter::Units::quarter);
    BOOST_CHECK_CLOSE(quarters, _quarters, 0.001);

    double _hundredweights = MassConverter::convert(decagrams, MassConverter::Units::decagram, MassConverter::Units::hundredweight);
    BOOST_CHECK_CLOSE(hundredweights, _hundredweights, 0.001);

    double _tons = MassConverter::convert(decagrams, MassConverter::Units::decagram, MassConverter::Units::ton);
    BOOST_CHECK_CLOSE(tons, _tons, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_grains, MassConverterTest)
{
    double _grams = MassConverter::convert(grains, MassConverter::Units::grain, MassConverter::Units::gram);
    BOOST_CHECK_CLOSE(grams, _grams, 0.001);

    double _centigrams = MassConverter::convert(grains, MassConverter::Units::grain, MassConverter::Units::centigram);
    BOOST_CHECK_CLOSE(centigrams, _centigrams, 0.001);

    double _decigrams = MassConverter::convert(grains, MassConverter::Units::grain, MassConverter::Units::decigram);
    BOOST_CHECK_CLOSE(decigrams, _decigrams, 0.001);

    double _milligrams = MassConverter::convert(grains, MassConverter::Units::grain, MassConverter::Units::milligram);
    BOOST_CHECK_CLOSE(milligrams, _milligrams, 0.001);

    double _micrograms = MassConverter::convert(grains, MassConverter::Units::grain, MassConverter::Units::microgram);
    BOOST_CHECK_CLOSE(micrograms, _micrograms, 0.001);

    double _nanograms = MassConverter::convert(grains, MassConverter::Units::grain, MassConverter::Units::nanogram);
    BOOST_CHECK_CLOSE(nanograms, _nanograms, 0.001);

    double _kilograms = MassConverter::convert(grains, MassConverter::Units::grain, MassConverter::Units::kilogram);
    BOOST_CHECK_CLOSE(kilograms, _kilograms, 0.001);

    double _hectograms = MassConverter::convert(grains, MassConverter::Units::grain, MassConverter::Units::hectogram);
    BOOST_CHECK_CLOSE(hectograms, _hectograms, 0.001);

    double _decagrams = MassConverter::convert(grains, MassConverter::Units::grain, MassConverter::Units::decagram);
    BOOST_CHECK_CLOSE(decagrams, _decagrams, 0.001);

    double _ounces = MassConverter::convert(grains, MassConverter::Units::grain, MassConverter::Units::ounce);
    BOOST_CHECK_CLOSE(ounces, _ounces, 0.001);

    double _pounds = MassConverter::convert(grains, MassConverter::Units::grain, MassConverter::Units::pound);
    BOOST_CHECK_CLOSE(pounds, _pounds, 0.001);

    double _stones = MassConverter::convert(grains, MassConverter::Units::grain, MassConverter::Units::stone);
    BOOST_CHECK_CLOSE(stones, _stones, 0.001);

    double _quarters = MassConverter::convert(grains, MassConverter::Units::grain, MassConverter::Units::quarter);
    BOOST_CHECK_CLOSE(quarters, _quarters, 0.001);

    double _hundredweights = MassConverter::convert(grains, MassConverter::Units::grain, MassConverter::Units::hundredweight);
    BOOST_CHECK_CLOSE(hundredweights, _hundredweights, 0.001);

    double _tons = MassConverter::convert(grains, MassConverter::Units::grain, MassConverter::Units::ton);
    BOOST_CHECK_CLOSE(tons, _tons, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_ounces, MassConverterTest)
{
    double _grams = MassConverter::convert(ounces, MassConverter::Units::ounce, MassConverter::Units::gram);
    BOOST_CHECK_CLOSE(grams, _grams, 0.001);

    double _centigrams = MassConverter::convert(ounces, MassConverter::Units::ounce, MassConverter::Units::centigram);
    BOOST_CHECK_CLOSE(centigrams, _centigrams, 0.001);

    double _decigrams = MassConverter::convert(ounces, MassConverter::Units::ounce, MassConverter::Units::decigram);
    BOOST_CHECK_CLOSE(decigrams, _decigrams, 0.001);

    double _milligrams = MassConverter::convert(ounces, MassConverter::Units::ounce, MassConverter::Units::milligram);
    BOOST_CHECK_CLOSE(milligrams, _milligrams, 0.001);

    double _micrograms = MassConverter::convert(ounces, MassConverter::Units::ounce, MassConverter::Units::microgram);
    BOOST_CHECK_CLOSE(micrograms, _micrograms, 0.001);

    double _nanograms = MassConverter::convert(ounces, MassConverter::Units::ounce, MassConverter::Units::nanogram);
    BOOST_CHECK_CLOSE(nanograms, _nanograms, 0.001);

    double _kilograms = MassConverter::convert(ounces, MassConverter::Units::ounce, MassConverter::Units::kilogram);
    BOOST_CHECK_CLOSE(kilograms, _kilograms, 0.001);

    double _hectograms = MassConverter::convert(ounces, MassConverter::Units::ounce, MassConverter::Units::hectogram);
    BOOST_CHECK_CLOSE(hectograms, _hectograms, 0.001);

    double _decagrams = MassConverter::convert(ounces, MassConverter::Units::ounce, MassConverter::Units::decagram);
    BOOST_CHECK_CLOSE(decagrams, _decagrams, 0.001);

    double _grains = MassConverter::convert(ounces, MassConverter::Units::ounce, MassConverter::Units::grain);
    BOOST_CHECK_CLOSE(grains, _grains, 0.001);

    double _pounds = MassConverter::convert(ounces, MassConverter::Units::ounce, MassConverter::Units::pound);
    BOOST_CHECK_CLOSE(pounds, _pounds, 0.001);

    double _stones = MassConverter::convert(ounces, MassConverter::Units::ounce, MassConverter::Units::stone);
    BOOST_CHECK_CLOSE(stones, _stones, 0.001);

    double _quarters = MassConverter::convert(ounces, MassConverter::Units::ounce, MassConverter::Units::quarter);
    BOOST_CHECK_CLOSE(quarters, _quarters, 0.001);

    double _hundredweights = MassConverter::convert(ounces, MassConverter::Units::ounce, MassConverter::Units::hundredweight);
    BOOST_CHECK_CLOSE(hundredweights, _hundredweights, 0.001);

    double _tons = MassConverter::convert(ounces, MassConverter::Units::ounce, MassConverter::Units::ton);
    BOOST_CHECK_CLOSE(tons, _tons, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_pounds, MassConverterTest)
{
    double _grams = MassConverter::convert(pounds, MassConverter::Units::pound, MassConverter::Units::gram);
    BOOST_CHECK_CLOSE(grams, _grams, 0.001);

    double _centigrams = MassConverter::convert(pounds, MassConverter::Units::pound, MassConverter::Units::centigram);
    BOOST_CHECK_CLOSE(centigrams, _centigrams, 0.001);

    double _decigrams = MassConverter::convert(pounds, MassConverter::Units::pound, MassConverter::Units::decigram);
    BOOST_CHECK_CLOSE(decigrams, _decigrams, 0.001);

    double _milligrams = MassConverter::convert(pounds, MassConverter::Units::pound, MassConverter::Units::milligram);
    BOOST_CHECK_CLOSE(milligrams, _milligrams, 0.001);

    double _micrograms = MassConverter::convert(pounds, MassConverter::Units::pound, MassConverter::Units::microgram);
    BOOST_CHECK_CLOSE(micrograms, _micrograms, 0.001);

    double _nanograms = MassConverter::convert(pounds, MassConverter::Units::pound, MassConverter::Units::nanogram);
    BOOST_CHECK_CLOSE(nanograms, _nanograms, 0.001);

    double _kilograms = MassConverter::convert(pounds, MassConverter::Units::pound, MassConverter::Units::kilogram);
    BOOST_CHECK_CLOSE(kilograms, _kilograms, 0.001);

    double _hectograms = MassConverter::convert(pounds, MassConverter::Units::pound, MassConverter::Units::hectogram);
    BOOST_CHECK_CLOSE(hectograms, _hectograms, 0.001);

    double _decagrams = MassConverter::convert(pounds, MassConverter::Units::pound, MassConverter::Units::decagram);
    BOOST_CHECK_CLOSE(decagrams, _decagrams, 0.001);

    double _grains = MassConverter::convert(pounds, MassConverter::Units::pound, MassConverter::Units::grain);
    BOOST_CHECK_CLOSE(grains, _grains, 0.001);

    double _ounces = MassConverter::convert(pounds, MassConverter::Units::pound, MassConverter::Units::ounce);
    BOOST_CHECK_CLOSE(ounces, _ounces, 0.001);

    double _stones = MassConverter::convert(pounds, MassConverter::Units::pound, MassConverter::Units::stone);
    BOOST_CHECK_CLOSE(stones, _stones, 0.001);

    double _quarters = MassConverter::convert(pounds, MassConverter::Units::pound, MassConverter::Units::quarter);
    BOOST_CHECK_CLOSE(quarters, _quarters, 0.001);

    double _hundredweights = MassConverter::convert(pounds, MassConverter::Units::pound, MassConverter::Units::hundredweight);
    BOOST_CHECK_CLOSE(hundredweights, _hundredweights, 0.001);

    double _tons = MassConverter::convert(pounds, MassConverter::Units::pound, MassConverter::Units::ton);
    BOOST_CHECK_CLOSE(tons, _tons, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_stones, MassConverterTest)
{
    double _grams = MassConverter::convert(stones, MassConverter::Units::stone, MassConverter::Units::gram);
    BOOST_CHECK_CLOSE(grams, _grams, 0.001);

    double _centigrams = MassConverter::convert(stones, MassConverter::Units::stone, MassConverter::Units::centigram);
    BOOST_CHECK_CLOSE(centigrams, _centigrams, 0.001);

    double _decigrams = MassConverter::convert(stones, MassConverter::Units::stone, MassConverter::Units::decigram);
    BOOST_CHECK_CLOSE(decigrams, _decigrams, 0.001);

    double _milligrams = MassConverter::convert(stones, MassConverter::Units::stone, MassConverter::Units::milligram);
    BOOST_CHECK_CLOSE(milligrams, _milligrams, 0.001);

    double _micrograms = MassConverter::convert(stones, MassConverter::Units::stone, MassConverter::Units::microgram);
    BOOST_CHECK_CLOSE(micrograms, _micrograms, 0.001);

    double _nanograms = MassConverter::convert(stones, MassConverter::Units::stone, MassConverter::Units::nanogram);
    BOOST_CHECK_CLOSE(nanograms, _nanograms, 0.001);

    double _kilograms = MassConverter::convert(stones, MassConverter::Units::stone, MassConverter::Units::kilogram);
    BOOST_CHECK_CLOSE(kilograms, _kilograms, 0.001);

    double _hectograms = MassConverter::convert(stones, MassConverter::Units::stone, MassConverter::Units::hectogram);
    BOOST_CHECK_CLOSE(hectograms, _hectograms, 0.001);

    double _decagrams = MassConverter::convert(stones, MassConverter::Units::stone, MassConverter::Units::decagram);
    BOOST_CHECK_CLOSE(decagrams, _decagrams, 0.001);

    double _grains = MassConverter::convert(stones, MassConverter::Units::stone, MassConverter::Units::grain);
    BOOST_CHECK_CLOSE(grains, _grains, 0.001);

    double _ounces = MassConverter::convert(stones, MassConverter::Units::stone, MassConverter::Units::ounce);
    BOOST_CHECK_CLOSE(ounces, _ounces, 0.001);

    double _pounds = MassConverter::convert(stones, MassConverter::Units::stone, MassConverter::Units::pound);
    BOOST_CHECK_CLOSE(pounds, _pounds, 0.001);

    double _quarters = MassConverter::convert(stones, MassConverter::Units::stone, MassConverter::Units::quarter);
    BOOST_CHECK_CLOSE(quarters, _quarters, 0.001);

    double _hundredweights = MassConverter::convert(stones, MassConverter::Units::stone, MassConverter::Units::hundredweight);
    BOOST_CHECK_CLOSE(hundredweights, _hundredweights, 0.001);

    double _tons = MassConverter::convert(stones, MassConverter::Units::stone, MassConverter::Units::ton);
    BOOST_CHECK_CLOSE(tons, _tons, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_quarters, MassConverterTest)
{
    double _grams = MassConverter::convert(quarters, MassConverter::Units::quarter, MassConverter::Units::gram);
    BOOST_CHECK_CLOSE(grams, _grams, 0.001);

    double _centigrams = MassConverter::convert(quarters, MassConverter::Units::quarter, MassConverter::Units::centigram);
    BOOST_CHECK_CLOSE(centigrams, _centigrams, 0.001);

    double _decigrams = MassConverter::convert(quarters, MassConverter::Units::quarter, MassConverter::Units::decigram);
    BOOST_CHECK_CLOSE(decigrams, _decigrams, 0.001);

    double _milligrams = MassConverter::convert(quarters, MassConverter::Units::quarter, MassConverter::Units::milligram);
    BOOST_CHECK_CLOSE(milligrams, _milligrams, 0.001);

    double _micrograms = MassConverter::convert(quarters, MassConverter::Units::quarter, MassConverter::Units::microgram);
    BOOST_CHECK_CLOSE(micrograms, _micrograms, 0.001);

    double _nanograms = MassConverter::convert(quarters, MassConverter::Units::quarter, MassConverter::Units::nanogram);
    BOOST_CHECK_CLOSE(nanograms, _nanograms, 0.001);

    double _kilograms = MassConverter::convert(quarters, MassConverter::Units::quarter, MassConverter::Units::kilogram);
    BOOST_CHECK_CLOSE(kilograms, _kilograms, 0.001);

    double _hectograms = MassConverter::convert(quarters, MassConverter::Units::quarter, MassConverter::Units::hectogram);
    BOOST_CHECK_CLOSE(hectograms, _hectograms, 0.001);

    double _decagrams = MassConverter::convert(quarters, MassConverter::Units::quarter, MassConverter::Units::decagram);
    BOOST_CHECK_CLOSE(decagrams, _decagrams, 0.001);

    double _grains = MassConverter::convert(quarters, MassConverter::Units::quarter, MassConverter::Units::grain);
    BOOST_CHECK_CLOSE(grains, _grains, 0.001);

    double _ounces = MassConverter::convert(quarters, MassConverter::Units::quarter, MassConverter::Units::ounce);
    BOOST_CHECK_CLOSE(ounces, _ounces, 0.001);

    double _pounds = MassConverter::convert(quarters, MassConverter::Units::quarter, MassConverter::Units::pound);
    BOOST_CHECK_CLOSE(pounds, _pounds, 0.001);

    double _stones = MassConverter::convert(quarters, MassConverter::Units::quarter, MassConverter::Units::stone);
    BOOST_CHECK_CLOSE(stones, _stones, 0.001);

    double _hundredweights = MassConverter::convert(quarters, MassConverter::Units::quarter, MassConverter::Units::hundredweight);
    BOOST_CHECK_CLOSE(hundredweights, _hundredweights, 0.001);

    double _tons = MassConverter::convert(quarters, MassConverter::Units::quarter, MassConverter::Units::ton);
    BOOST_CHECK_CLOSE(tons, _tons, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_hundredweights, MassConverterTest)
{
    double _grams = MassConverter::convert(hundredweights, MassConverter::Units::hundredweight, MassConverter::Units::gram);
    BOOST_CHECK_CLOSE(grams, _grams, 0.001);

    double _centigrams = MassConverter::convert(hundredweights, MassConverter::Units::hundredweight, MassConverter::Units::centigram);
    BOOST_CHECK_CLOSE(centigrams, _centigrams, 0.001);

    double _decigrams = MassConverter::convert(hundredweights, MassConverter::Units::hundredweight, MassConverter::Units::decigram);
    BOOST_CHECK_CLOSE(decigrams, _decigrams, 0.001);

    double _milligrams = MassConverter::convert(hundredweights, MassConverter::Units::hundredweight, MassConverter::Units::milligram);
    BOOST_CHECK_CLOSE(milligrams, _milligrams, 0.001);

    double _micrograms = MassConverter::convert(hundredweights, MassConverter::Units::hundredweight, MassConverter::Units::microgram);
    BOOST_CHECK_CLOSE(micrograms, _micrograms, 0.001);

    double _nanograms = MassConverter::convert(hundredweights, MassConverter::Units::hundredweight, MassConverter::Units::nanogram);
    BOOST_CHECK_CLOSE(nanograms, _nanograms, 0.001);

    double _kilograms = MassConverter::convert(hundredweights, MassConverter::Units::hundredweight, MassConverter::Units::kilogram);
    BOOST_CHECK_CLOSE(kilograms, _kilograms, 0.001);

    double _hectograms = MassConverter::convert(hundredweights, MassConverter::Units::hundredweight, MassConverter::Units::hectogram);
    BOOST_CHECK_CLOSE(hectograms, _hectograms, 0.001);

    double _decagrams = MassConverter::convert(hundredweights, MassConverter::Units::hundredweight, MassConverter::Units::decagram);
    BOOST_CHECK_CLOSE(decagrams, _decagrams, 0.001);

    double _grains = MassConverter::convert(hundredweights, MassConverter::Units::hundredweight, MassConverter::Units::grain);
    BOOST_CHECK_CLOSE(grains, _grains, 0.001);

    double _ounces = MassConverter::convert(hundredweights, MassConverter::Units::hundredweight, MassConverter::Units::ounce);
    BOOST_CHECK_CLOSE(ounces, _ounces, 0.001);

    double _pounds = MassConverter::convert(hundredweights, MassConverter::Units::hundredweight, MassConverter::Units::pound);
    BOOST_CHECK_CLOSE(pounds, _pounds, 0.001);

    double _stones = MassConverter::convert(hundredweights, MassConverter::Units::hundredweight, MassConverter::Units::stone);
    BOOST_CHECK_CLOSE(stones, _stones, 0.001);

    double _quarters = MassConverter::convert(hundredweights, MassConverter::Units::hundredweight, MassConverter::Units::quarter);
    BOOST_CHECK_CLOSE(quarters, _quarters, 0.001);

    double _tons = MassConverter::convert(hundredweights, MassConverter::Units::hundredweight, MassConverter::Units::ton);
    BOOST_CHECK_CLOSE(tons, _tons, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_tons, MassConverterTest)
{
    double _grams = MassConverter::convert(tons, MassConverter::Units::ton, MassConverter::Units::gram);
    BOOST_CHECK_CLOSE(grams, _grams, 0.001);

    double _centigrams = MassConverter::convert(tons, MassConverter::Units::ton, MassConverter::Units::centigram);
    BOOST_CHECK_CLOSE(centigrams, _centigrams, 0.001);

    double _decigrams = MassConverter::convert(tons, MassConverter::Units::ton, MassConverter::Units::decigram);
    BOOST_CHECK_CLOSE(decigrams, _decigrams, 0.001);

    double _milligrams = MassConverter::convert(tons, MassConverter::Units::ton, MassConverter::Units::milligram);
    BOOST_CHECK_CLOSE(milligrams, _milligrams, 0.001);

    double _micrograms = MassConverter::convert(tons, MassConverter::Units::ton, MassConverter::Units::microgram);
    BOOST_CHECK_CLOSE(micrograms, _micrograms, 0.001);

    double _nanograms = MassConverter::convert(tons, MassConverter::Units::ton, MassConverter::Units::nanogram);
    BOOST_CHECK_CLOSE(nanograms, _nanograms, 0.001);

    double _kilograms = MassConverter::convert(tons, MassConverter::Units::ton, MassConverter::Units::kilogram);
    BOOST_CHECK_CLOSE(kilograms, _kilograms, 0.001);

    double _hectograms = MassConverter::convert(tons, MassConverter::Units::ton, MassConverter::Units::hectogram);
    BOOST_CHECK_CLOSE(hectograms, _hectograms, 0.001);

    double _decagrams = MassConverter::convert(tons, MassConverter::Units::ton, MassConverter::Units::decagram);
    BOOST_CHECK_CLOSE(decagrams, _decagrams, 0.001);

    double _grains = MassConverter::convert(tons, MassConverter::Units::ton, MassConverter::Units::grain);
    BOOST_CHECK_CLOSE(grains, _grains, 0.001);

    double _ounces = MassConverter::convert(tons, MassConverter::Units::ton, MassConverter::Units::ounce);
    BOOST_CHECK_CLOSE(ounces, _ounces, 0.001);

    double _pounds = MassConverter::convert(tons, MassConverter::Units::ton, MassConverter::Units::pound);
    BOOST_CHECK_CLOSE(pounds, _pounds, 0.001);

    double _stones = MassConverter::convert(tons, MassConverter::Units::ton, MassConverter::Units::stone);
    BOOST_CHECK_CLOSE(stones, _stones, 0.001);

    double _quarters = MassConverter::convert(tons, MassConverter::Units::ton, MassConverter::Units::quarter);
    BOOST_CHECK_CLOSE(quarters, _quarters, 0.001);

    double _hundredweights = MassConverter::convert(tons, MassConverter::Units::ton, MassConverter::Units::hundredweight);
    BOOST_CHECK_CLOSE(hundredweights, _hundredweights, 0.001);
}

BOOST_AUTO_TEST_SUITE_END()
