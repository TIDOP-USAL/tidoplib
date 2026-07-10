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
 
#define BOOST_TEST_MODULE Tidop Unit test
#include <boost/test/unit_test.hpp>
#include <boost/test/tools/output_test_stream.hpp>
#include <tidop/core/units/LengthConverter.h>
#include <tidop/core/units/MassConverter.h>
#include <tidop/core/units/AreaConverter.h>

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
    double _decimetres = LengthConverter::convert(metres, LengthUnit::metre, LengthUnit::decimetre);
    BOOST_CHECK_EQUAL(decimetres, _decimetres);

    double _centimetres = LengthConverter::convert(metres, LengthUnit::metre, LengthUnit::centimetre);
    BOOST_CHECK_EQUAL(centimetres, _centimetres);

    double _millimetres = LengthConverter::convert(metres, LengthUnit::metre, LengthUnit::millimetre);
    BOOST_CHECK_EQUAL(millimetres, _millimetres);

    double _micrometres = LengthConverter::convert(metres, LengthUnit::metre, LengthUnit::micrometre);
    BOOST_CHECK_EQUAL(micrometres, _micrometres);

    double _nanometres = LengthConverter::convert(metres, LengthUnit::metre, LengthUnit::nanometre);
    BOOST_CHECK_EQUAL(nanometres, _nanometres);

    double _kilometres = LengthConverter::convert(metres, LengthUnit::metre, LengthUnit::kilometre);
    BOOST_CHECK_EQUAL(kilometres, _kilometres);

    double _hectometres = LengthConverter::convert(metres, LengthUnit::metre, LengthUnit::hectometre);
    BOOST_CHECK_EQUAL(hectometres, _hectometres);

    double _decametres = LengthConverter::convert(metres, LengthUnit::metre, LengthUnit::decametre);
    BOOST_CHECK_EQUAL(decametres, _decametres);

    double _inches = LengthConverter::convert(metres, LengthUnit::metre, LengthUnit::inch);
    BOOST_CHECK_CLOSE(inches, _inches, 0.001);

    double _feet = LengthConverter::convert(metres, LengthUnit::metre, LengthUnit::foot);
    BOOST_CHECK_CLOSE(feet, _feet, 0.001);

    double _yards = LengthConverter::convert(metres, LengthUnit::metre, LengthUnit::yard);
    BOOST_CHECK_CLOSE(yards, _yards, 0.001);

    double _chains = LengthConverter::convert(metres, LengthUnit::metre, LengthUnit::chain);
    BOOST_CHECK_CLOSE(chains, _chains, 0.001);

    double _furlongs = LengthConverter::convert(metres, LengthUnit::metre, LengthUnit::furlong);
    BOOST_CHECK_CLOSE(furlongs, _furlongs, 0.001);

    double _miles = LengthConverter::convert(metres, LengthUnit::metre, LengthUnit::mile);
    BOOST_CHECK_CLOSE(miles, _miles, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_decimetres, LengthConverterTest)
{
    double _metres = LengthConverter::convert(decimetres, LengthUnit::decimetre, LengthUnit::metre);
    BOOST_CHECK_EQUAL(metres, _metres);

    double _centimetres = LengthConverter::convert(decimetres, LengthUnit::decimetre, LengthUnit::centimetre);
    BOOST_CHECK_EQUAL(centimetres, _centimetres);

    double _millimetres = LengthConverter::convert(decimetres, LengthUnit::decimetre, LengthUnit::millimetre);
    BOOST_CHECK_EQUAL(millimetres, _millimetres);

    double _micrometres = LengthConverter::convert(decimetres, LengthUnit::decimetre, LengthUnit::micrometre);
    BOOST_CHECK_EQUAL(micrometres, _micrometres);

    double _nanometres = LengthConverter::convert(decimetres, LengthUnit::decimetre, LengthUnit::nanometre);
    BOOST_CHECK_EQUAL(nanometres, _nanometres);

    double _kilometres = LengthConverter::convert(decimetres, LengthUnit::decimetre, LengthUnit::kilometre);
    BOOST_CHECK_EQUAL(kilometres, _kilometres);

    double _hectometres = LengthConverter::convert(decimetres, LengthUnit::decimetre, LengthUnit::hectometre);
    BOOST_CHECK_EQUAL(hectometres, _hectometres);

    double _decametres = LengthConverter::convert(decimetres, LengthUnit::decimetre, LengthUnit::decametre);
    BOOST_CHECK_CLOSE(decametres, _decametres, 0.001);

    double _inches = LengthConverter::convert(decimetres, LengthUnit::decimetre, LengthUnit::inch);
    BOOST_CHECK_CLOSE(inches, _inches, 0.001);

    double _feet = LengthConverter::convert(decimetres, LengthUnit::decimetre, LengthUnit::foot);
    BOOST_CHECK_CLOSE(feet, _feet, 0.001);

    double _yards = LengthConverter::convert(decimetres, LengthUnit::decimetre, LengthUnit::yard);
    BOOST_CHECK_CLOSE(yards, _yards, 0.001);

    double _chains = LengthConverter::convert(decimetres, LengthUnit::decimetre, LengthUnit::chain);
    BOOST_CHECK_CLOSE(chains, _chains, 0.001);

    double _furlongs = LengthConverter::convert(decimetres, LengthUnit::decimetre, LengthUnit::furlong);
    BOOST_CHECK_CLOSE(furlongs, _furlongs, 0.001);

    double _miles = LengthConverter::convert(decimetres, LengthUnit::decimetre, LengthUnit::mile);
    BOOST_CHECK_CLOSE(miles, _miles, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_centimetres, LengthConverterTest)
{
    double _metres = LengthConverter::convert(centimetres, LengthUnit::centimetre, LengthUnit::metre);
    BOOST_CHECK_EQUAL(metres, _metres);

    double _decimetres = LengthConverter::convert(centimetres, LengthUnit::centimetre, LengthUnit::decimetre);
    BOOST_CHECK_EQUAL(decimetres, _decimetres);

    double _millimetres = LengthConverter::convert(centimetres, LengthUnit::centimetre, LengthUnit::millimetre);
    BOOST_CHECK_EQUAL(millimetres, _millimetres);

    double _micrometres = LengthConverter::convert(centimetres, LengthUnit::centimetre, LengthUnit::micrometre);
    BOOST_CHECK_EQUAL(micrometres, _micrometres);

    double _nanometres = LengthConverter::convert(centimetres, LengthUnit::centimetre, LengthUnit::nanometre);
    BOOST_CHECK_EQUAL(nanometres, _nanometres);

    double _kilometres = LengthConverter::convert(centimetres, LengthUnit::centimetre, LengthUnit::kilometre);
    BOOST_CHECK_CLOSE(kilometres, _kilometres, 0.001);

    double _hectometres = LengthConverter::convert(centimetres, LengthUnit::centimetre, LengthUnit::hectometre);
    BOOST_CHECK_EQUAL(hectometres, _hectometres);

    double _decametres = LengthConverter::convert(centimetres, LengthUnit::centimetre, LengthUnit::decametre);
    BOOST_CHECK_CLOSE(decametres, _decametres, 0.001);

    double _inches = LengthConverter::convert(centimetres, LengthUnit::centimetre, LengthUnit::inch);
    BOOST_CHECK_CLOSE(inches, _inches, 0.001);

    double _feet = LengthConverter::convert(centimetres, LengthUnit::centimetre, LengthUnit::foot);
    BOOST_CHECK_CLOSE(feet, _feet, 0.001);

    double _yards = LengthConverter::convert(centimetres, LengthUnit::centimetre, LengthUnit::yard);
    BOOST_CHECK_CLOSE(yards, _yards, 0.001);

    double _chains = LengthConverter::convert(centimetres, LengthUnit::centimetre, LengthUnit::chain);
    BOOST_CHECK_CLOSE(chains, _chains, 0.001);

    double _furlongs = LengthConverter::convert(centimetres, LengthUnit::centimetre, LengthUnit::furlong);
    BOOST_CHECK_CLOSE(furlongs, _furlongs, 0.001);

    double _miles = LengthConverter::convert(centimetres, LengthUnit::centimetre, LengthUnit::mile);
    BOOST_CHECK_CLOSE(miles, _miles, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_millimetres, LengthConverterTest)
{
    double _metres = LengthConverter::convert(millimetres, LengthUnit::millimetre, LengthUnit::metre);
    BOOST_CHECK_EQUAL(metres, _metres);

    double _centimetres = LengthConverter::convert(millimetres, LengthUnit::millimetre, LengthUnit::centimetre);
    BOOST_CHECK_EQUAL(centimetres, _centimetres);

    double _decimetres = LengthConverter::convert(millimetres, LengthUnit::millimetre, LengthUnit::decimetre);
    BOOST_CHECK_EQUAL(decimetres, _decimetres);

    double _micrometres = LengthConverter::convert(millimetres, LengthUnit::millimetre, LengthUnit::micrometre);
    BOOST_CHECK_EQUAL(micrometres, _micrometres);

    double _nanometres = LengthConverter::convert(millimetres, LengthUnit::millimetre, LengthUnit::nanometre);
    BOOST_CHECK_EQUAL(nanometres, _nanometres);

    double _kilometres = LengthConverter::convert(millimetres, LengthUnit::millimetre, LengthUnit::kilometre);
    BOOST_CHECK_CLOSE(kilometres, _kilometres, 0.001);

    double _hectometres = LengthConverter::convert(millimetres, LengthUnit::millimetre, LengthUnit::hectometre);
    BOOST_CHECK_EQUAL(hectometres, _hectometres);

    double _decametres = LengthConverter::convert(millimetres, LengthUnit::millimetre, LengthUnit::decametre);
    BOOST_CHECK_CLOSE(decametres, _decametres, 0.001);

    double _inches = LengthConverter::convert(millimetres, LengthUnit::millimetre, LengthUnit::inch);
    BOOST_CHECK_CLOSE(inches, _inches, 0.001);

    double _feet = LengthConverter::convert(millimetres, LengthUnit::millimetre, LengthUnit::foot);
    BOOST_CHECK_CLOSE(feet, _feet, 0.001);

    double _yards = LengthConverter::convert(millimetres, LengthUnit::millimetre, LengthUnit::yard);
    BOOST_CHECK_CLOSE(yards, _yards, 0.001);

    double _chains = LengthConverter::convert(millimetres, LengthUnit::millimetre, LengthUnit::chain);
    BOOST_CHECK_CLOSE(chains, _chains, 0.001);

    double _furlongs = LengthConverter::convert(millimetres, LengthUnit::millimetre, LengthUnit::furlong);
    BOOST_CHECK_CLOSE(furlongs, _furlongs, 0.001);

    double _miles = LengthConverter::convert(millimetres, LengthUnit::millimetre, LengthUnit::mile);
    BOOST_CHECK_CLOSE(miles, _miles, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_micrometres, LengthConverterTest)
{
    double _metres = LengthConverter::convert(micrometres, LengthUnit::micrometre, LengthUnit::metre);
    BOOST_CHECK_CLOSE(metres, _metres, 0.001);

    double _centimetres = LengthConverter::convert(micrometres, LengthUnit::micrometre, LengthUnit::centimetre);
    BOOST_CHECK_EQUAL(centimetres, _centimetres);

    double _decimetres = LengthConverter::convert(micrometres, LengthUnit::micrometre, LengthUnit::decimetre);
    BOOST_CHECK_EQUAL(decimetres, _decimetres);

    double _millimetres = LengthConverter::convert(micrometres, LengthUnit::micrometre, LengthUnit::millimetre);
    BOOST_CHECK_EQUAL(millimetres, _millimetres);

    double _nanometres = LengthConverter::convert(micrometres, LengthUnit::micrometre, LengthUnit::nanometre);
    BOOST_CHECK_EQUAL(nanometres, _nanometres);

    double _kilometres = LengthConverter::convert(micrometres, LengthUnit::micrometre, LengthUnit::kilometre);
    BOOST_CHECK_CLOSE(kilometres, _kilometres, 0.001);

    double _hectometres = LengthConverter::convert(micrometres, LengthUnit::micrometre, LengthUnit::hectometre);
    BOOST_CHECK_EQUAL(hectometres, _hectometres);

    double _decametres = LengthConverter::convert(micrometres, LengthUnit::micrometre, LengthUnit::decametre);
    BOOST_CHECK_CLOSE(decametres, _decametres, 0.001);

    double _inches = LengthConverter::convert(micrometres, LengthUnit::micrometre, LengthUnit::inch);
    BOOST_CHECK_CLOSE(inches, _inches, 0.001);

    double _feet = LengthConverter::convert(micrometres, LengthUnit::micrometre, LengthUnit::foot);
    BOOST_CHECK_CLOSE(feet, _feet, 0.001);

    double _yards = LengthConverter::convert(micrometres, LengthUnit::micrometre, LengthUnit::yard);
    BOOST_CHECK_CLOSE(yards, _yards, 0.001);

    double _chains = LengthConverter::convert(micrometres, LengthUnit::micrometre, LengthUnit::chain);
    BOOST_CHECK_CLOSE(chains, _chains, 0.001);

    double _furlongs = LengthConverter::convert(micrometres, LengthUnit::micrometre, LengthUnit::furlong);
    BOOST_CHECK_CLOSE(furlongs, _furlongs, 0.001);

    double _miles = LengthConverter::convert(micrometres, LengthUnit::micrometre, LengthUnit::mile);
    BOOST_CHECK_CLOSE(miles, _miles, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_nanometres, LengthConverterTest)
{
    double _metres = LengthConverter::convert(nanometres, LengthUnit::nanometre, LengthUnit::metre);
    BOOST_CHECK_CLOSE(metres, _metres, 0.001);

    double _centimetres = LengthConverter::convert(nanometres, LengthUnit::nanometre, LengthUnit::centimetre);
    BOOST_CHECK_CLOSE(centimetres, _centimetres, 0.001);

    double _decimetres = LengthConverter::convert(nanometres, LengthUnit::nanometre, LengthUnit::decimetre);
    BOOST_CHECK_CLOSE(decimetres, _decimetres, 0.001);

    double _millimetres = LengthConverter::convert(nanometres, LengthUnit::nanometre, LengthUnit::millimetre);
    BOOST_CHECK_CLOSE(millimetres, _millimetres, 0.001);

    double _micrometres = LengthConverter::convert(nanometres, LengthUnit::nanometre, LengthUnit::micrometre);
    BOOST_CHECK_EQUAL(micrometres, _micrometres);

    double _kilometres = LengthConverter::convert(nanometres, LengthUnit::nanometre, LengthUnit::kilometre);
    BOOST_CHECK_CLOSE(kilometres, _kilometres, 0.001);

    double _hectometres = LengthConverter::convert(nanometres, LengthUnit::nanometre, LengthUnit::hectometre);
    BOOST_CHECK_EQUAL(hectometres, _hectometres);

    double _decametres = LengthConverter::convert(nanometres, LengthUnit::nanometre, LengthUnit::decametre);
    BOOST_CHECK_CLOSE(decametres, _decametres, 0.001);

    double _inches = LengthConverter::convert(nanometres, LengthUnit::nanometre, LengthUnit::inch);
    BOOST_CHECK_CLOSE(inches, _inches, 0.001);

    double _feet = LengthConverter::convert(nanometres, LengthUnit::nanometre, LengthUnit::foot);
    BOOST_CHECK_CLOSE(feet, _feet, 0.001);

    double _yards = LengthConverter::convert(nanometres, LengthUnit::nanometre, LengthUnit::yard);
    BOOST_CHECK_CLOSE(yards, _yards, 0.001);

    double _chains = LengthConverter::convert(nanometres, LengthUnit::nanometre, LengthUnit::chain);
    BOOST_CHECK_CLOSE(chains, _chains, 0.001);

    double _furlongs = LengthConverter::convert(nanometres, LengthUnit::nanometre, LengthUnit::furlong);
    BOOST_CHECK_CLOSE(furlongs, _furlongs, 0.001);

    double _miles = LengthConverter::convert(nanometres, LengthUnit::nanometre, LengthUnit::mile);
    BOOST_CHECK_CLOSE(miles, _miles, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_kilometres, LengthConverterTest)
{
    double _metres = LengthConverter::convert(kilometres, LengthUnit::kilometre, LengthUnit::metre);
    BOOST_CHECK_CLOSE(metres, _metres, 0.001);

    double _centimetres = LengthConverter::convert(kilometres, LengthUnit::kilometre, LengthUnit::centimetre);
    BOOST_CHECK_CLOSE(centimetres, _centimetres, 0.001);

    double _decimetres = LengthConverter::convert(kilometres, LengthUnit::kilometre, LengthUnit::decimetre);
    BOOST_CHECK_CLOSE(decimetres, _decimetres, 0.001);

    double _millimetres = LengthConverter::convert(kilometres, LengthUnit::kilometre, LengthUnit::millimetre);
    BOOST_CHECK_CLOSE(millimetres, _millimetres, 0.001);

    double _micrometres = LengthConverter::convert(kilometres, LengthUnit::kilometre, LengthUnit::micrometre);
    BOOST_CHECK_EQUAL(micrometres, _micrometres);

    double _nanometres = LengthConverter::convert(kilometres, LengthUnit::kilometre, LengthUnit::nanometre);
    BOOST_CHECK_CLOSE(nanometres, _nanometres, 0.001);

    double _hectometres = LengthConverter::convert(kilometres, LengthUnit::kilometre, LengthUnit::hectometre);
    BOOST_CHECK_EQUAL(hectometres, _hectometres);

    double _decametres = LengthConverter::convert(kilometres, LengthUnit::kilometre, LengthUnit::decametre);
    BOOST_CHECK_CLOSE(decametres, _decametres, 0.001);

    double _inches = LengthConverter::convert(kilometres, LengthUnit::kilometre, LengthUnit::inch);
    BOOST_CHECK_CLOSE(inches, _inches, 0.001);

    double _feet = LengthConverter::convert(kilometres, LengthUnit::kilometre, LengthUnit::foot);
    BOOST_CHECK_CLOSE(feet, _feet, 0.001);

    double _yards = LengthConverter::convert(kilometres, LengthUnit::kilometre, LengthUnit::yard);
    BOOST_CHECK_CLOSE(yards, _yards, 0.001);

    double _chains = LengthConverter::convert(kilometres, LengthUnit::kilometre, LengthUnit::chain);
    BOOST_CHECK_CLOSE(chains, _chains, 0.001);

    double _furlongs = LengthConverter::convert(kilometres, LengthUnit::kilometre, LengthUnit::furlong);
    BOOST_CHECK_CLOSE(furlongs, _furlongs, 0.001);

    double _miles = LengthConverter::convert(kilometres, LengthUnit::kilometre, LengthUnit::mile);
    BOOST_CHECK_CLOSE(miles, _miles, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_hectometres, LengthConverterTest)
{
    double _metres = LengthConverter::convert(hectometres, LengthUnit::hectometre, LengthUnit::metre);
    BOOST_CHECK_CLOSE(metres, _metres, 0.001);

    double _centimetres = LengthConverter::convert(hectometres, LengthUnit::hectometre, LengthUnit::centimetre);
    BOOST_CHECK_CLOSE(centimetres, _centimetres, 0.001);

    double _decimetres = LengthConverter::convert(hectometres, LengthUnit::hectometre, LengthUnit::decimetre);
    BOOST_CHECK_CLOSE(decimetres, _decimetres, 0.001);

    double _millimetres = LengthConverter::convert(hectometres, LengthUnit::hectometre, LengthUnit::millimetre);
    BOOST_CHECK_CLOSE(millimetres, _millimetres, 0.001);

    double _micrometres = LengthConverter::convert(hectometres, LengthUnit::hectometre, LengthUnit::micrometre);
    BOOST_CHECK_CLOSE(micrometres, _micrometres, 0.001);

    double _nanometres = LengthConverter::convert(hectometres, LengthUnit::hectometre, LengthUnit::nanometre);
    BOOST_CHECK_CLOSE(nanometres, _nanometres, 0.001);

    double _kilometres = LengthConverter::convert(hectometres, LengthUnit::hectometre, LengthUnit::kilometre);
    BOOST_CHECK_CLOSE(kilometres, _kilometres, 0.001);

    double _decametres = LengthConverter::convert(hectometres, LengthUnit::hectometre, LengthUnit::decametre);
    BOOST_CHECK_CLOSE(decametres, _decametres, 0.001);

    double _inches = LengthConverter::convert(hectometres, LengthUnit::hectometre, LengthUnit::inch);
    BOOST_CHECK_CLOSE(inches, _inches, 0.001);

    double _feet = LengthConverter::convert(hectometres, LengthUnit::hectometre, LengthUnit::foot);
    BOOST_CHECK_CLOSE(feet, _feet, 0.001);

    double _yards = LengthConverter::convert(hectometres, LengthUnit::hectometre, LengthUnit::yard);
    BOOST_CHECK_CLOSE(yards, _yards, 0.001);

    double _chains = LengthConverter::convert(hectometres, LengthUnit::hectometre, LengthUnit::chain);
    BOOST_CHECK_CLOSE(chains, _chains, 0.001);

    double _furlongs = LengthConverter::convert(hectometres, LengthUnit::hectometre, LengthUnit::furlong);
    BOOST_CHECK_CLOSE(furlongs, _furlongs, 0.001);

    double _miles = LengthConverter::convert(hectometres, LengthUnit::hectometre, LengthUnit::mile);
    BOOST_CHECK_CLOSE(miles, _miles, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_decametres, LengthConverterTest)
{
    double _metres = LengthConverter::convert(decametres, LengthUnit::decametre, LengthUnit::metre);
    BOOST_CHECK_CLOSE(metres, _metres, 0.001);

    double _centimetres = LengthConverter::convert(decametres, LengthUnit::decametre, LengthUnit::centimetre);
    BOOST_CHECK_CLOSE(centimetres, _centimetres, 0.001);

    double _decimetres = LengthConverter::convert(decametres, LengthUnit::decametre, LengthUnit::decimetre);
    BOOST_CHECK_CLOSE(decimetres, _decimetres, 0.001);

    double _millimetres = LengthConverter::convert(decametres, LengthUnit::decametre, LengthUnit::millimetre);
    BOOST_CHECK_CLOSE(millimetres, _millimetres, 0.001);

    double _micrometres = LengthConverter::convert(decametres, LengthUnit::decametre, LengthUnit::micrometre);
    BOOST_CHECK_CLOSE(micrometres, _micrometres, 0.001);

    double _nanometres = LengthConverter::convert(decametres, LengthUnit::decametre, LengthUnit::nanometre);
    BOOST_CHECK_CLOSE(nanometres, _nanometres, 0.001);

    double _kilometres = LengthConverter::convert(decametres, LengthUnit::decametre, LengthUnit::kilometre);
    BOOST_CHECK_CLOSE(kilometres, _kilometres, 0.001);

    double _hectometres = LengthConverter::convert(decametres, LengthUnit::decametre, LengthUnit::hectometre);
    BOOST_CHECK_CLOSE(hectometres, _hectometres, 0.001);

    double _inches = LengthConverter::convert(decametres, LengthUnit::decametre, LengthUnit::inch);
    BOOST_CHECK_CLOSE(inches, _inches, 0.001);

    double _feet = LengthConverter::convert(decametres, LengthUnit::decametre, LengthUnit::foot);
    BOOST_CHECK_CLOSE(feet, _feet, 0.001);

    double _yards = LengthConverter::convert(decametres, LengthUnit::decametre, LengthUnit::yard);
    BOOST_CHECK_CLOSE(yards, _yards, 0.001);

    double _chains = LengthConverter::convert(decametres, LengthUnit::decametre, LengthUnit::chain);
    BOOST_CHECK_CLOSE(chains, _chains, 0.001);

    double _furlongs = LengthConverter::convert(decametres, LengthUnit::decametre, LengthUnit::furlong);
    BOOST_CHECK_CLOSE(furlongs, _furlongs, 0.001);

    double _miles = LengthConverter::convert(decametres, LengthUnit::decametre, LengthUnit::mile);
    BOOST_CHECK_CLOSE(miles, _miles, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_inches, LengthConverterTest)
{
    double _metres = LengthConverter::convert(inches, LengthUnit::inch, LengthUnit::metre);
    BOOST_CHECK_CLOSE(metres, _metres, 0.001);

    double _centimetres = LengthConverter::convert(inches, LengthUnit::inch, LengthUnit::centimetre);
    BOOST_CHECK_CLOSE(centimetres, _centimetres, 0.001);

    double _decimetres = LengthConverter::convert(inches, LengthUnit::inch, LengthUnit::decimetre);
    BOOST_CHECK_CLOSE(decimetres, _decimetres, 0.001);

    double _millimetres = LengthConverter::convert(inches, LengthUnit::inch, LengthUnit::millimetre);
    BOOST_CHECK_CLOSE(millimetres, _millimetres, 0.001);

    double _micrometres = LengthConverter::convert(inches, LengthUnit::inch, LengthUnit::micrometre);
    BOOST_CHECK_CLOSE(micrometres, _micrometres, 0.001);

    double _nanometres = LengthConverter::convert(inches, LengthUnit::inch, LengthUnit::nanometre);
    BOOST_CHECK_CLOSE(nanometres, _nanometres, 0.001);

    double _kilometres = LengthConverter::convert(inches, LengthUnit::inch, LengthUnit::kilometre);
    BOOST_CHECK_CLOSE(kilometres, _kilometres, 0.001);

    double _hectometres = LengthConverter::convert(inches, LengthUnit::inch, LengthUnit::hectometre);
    BOOST_CHECK_CLOSE(hectometres, _hectometres, 0.001);

    double _decametres = LengthConverter::convert(inches, LengthUnit::inch, LengthUnit::decametre);
    BOOST_CHECK_CLOSE(decametres, _decametres, 0.001);

    double _feet = LengthConverter::convert(inches, LengthUnit::inch, LengthUnit::foot);
    BOOST_CHECK_CLOSE(feet, _feet, 0.001);

    double _yards = LengthConverter::convert(inches, LengthUnit::inch, LengthUnit::yard);
    BOOST_CHECK_CLOSE(yards, _yards, 0.001);

    double _chains = LengthConverter::convert(inches, LengthUnit::inch, LengthUnit::chain);
    BOOST_CHECK_CLOSE(chains, _chains, 0.001);

    double _furlongs = LengthConverter::convert(inches, LengthUnit::inch, LengthUnit::furlong);
    BOOST_CHECK_CLOSE(furlongs, _furlongs, 0.001);

    double _miles = LengthConverter::convert(inches, LengthUnit::inch, LengthUnit::mile);
    BOOST_CHECK_CLOSE(miles, _miles, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_feet, LengthConverterTest)
{
    double _metres = LengthConverter::convert(feet, LengthUnit::foot, LengthUnit::metre);
    BOOST_CHECK_CLOSE(metres, _metres, 0.001);

    double _centimetres = LengthConverter::convert(feet, LengthUnit::foot, LengthUnit::centimetre);
    BOOST_CHECK_CLOSE(centimetres, _centimetres, 0.001);

    double _decimetres = LengthConverter::convert(feet, LengthUnit::foot, LengthUnit::decimetre);
    BOOST_CHECK_CLOSE(decimetres, _decimetres, 0.001);

    double _millimetres = LengthConverter::convert(feet, LengthUnit::foot, LengthUnit::millimetre);
    BOOST_CHECK_CLOSE(millimetres, _millimetres, 0.001);

    double _micrometres = LengthConverter::convert(feet, LengthUnit::foot, LengthUnit::micrometre);
    BOOST_CHECK_CLOSE(micrometres, _micrometres, 0.001);

    double _nanometres = LengthConverter::convert(feet, LengthUnit::foot, LengthUnit::nanometre);
    BOOST_CHECK_CLOSE(nanometres, _nanometres, 0.001);

    double _kilometres = LengthConverter::convert(feet, LengthUnit::foot, LengthUnit::kilometre);
    BOOST_CHECK_CLOSE(kilometres, _kilometres, 0.001);

    double _hectometres = LengthConverter::convert(feet, LengthUnit::foot, LengthUnit::hectometre);
    BOOST_CHECK_CLOSE(hectometres, _hectometres, 0.001);

    double _decametres = LengthConverter::convert(feet, LengthUnit::foot, LengthUnit::decametre);
    BOOST_CHECK_CLOSE(decametres, _decametres, 0.001);

    double _inches = LengthConverter::convert(feet, LengthUnit::foot, LengthUnit::inch);
    BOOST_CHECK_CLOSE(inches, _inches, 0.001);

    double _yards = LengthConverter::convert(feet, LengthUnit::foot, LengthUnit::yard);
    BOOST_CHECK_CLOSE(yards, _yards, 0.001);

    double _chains = LengthConverter::convert(feet, LengthUnit::foot, LengthUnit::chain);
    BOOST_CHECK_CLOSE(chains, _chains, 0.001);

    double _furlongs = LengthConverter::convert(feet, LengthUnit::foot, LengthUnit::furlong);
    BOOST_CHECK_CLOSE(furlongs, _furlongs, 0.001);

    double _miles = LengthConverter::convert(feet, LengthUnit::foot, LengthUnit::mile);
    BOOST_CHECK_CLOSE(miles, _miles, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_yards, LengthConverterTest)
{
    double _metres = LengthConverter::convert(yards, LengthUnit::yard, LengthUnit::metre);
    BOOST_CHECK_CLOSE(metres, _metres, 0.001);

    double _centimetres = LengthConverter::convert(yards, LengthUnit::yard, LengthUnit::centimetre);
    BOOST_CHECK_CLOSE(centimetres, _centimetres, 0.001);

    double _decimetres = LengthConverter::convert(yards, LengthUnit::yard, LengthUnit::decimetre);
    BOOST_CHECK_CLOSE(decimetres, _decimetres, 0.001);

    double _millimetres = LengthConverter::convert(yards, LengthUnit::yard, LengthUnit::millimetre);
    BOOST_CHECK_CLOSE(millimetres, _millimetres, 0.001);

    double _micrometres = LengthConverter::convert(yards, LengthUnit::yard, LengthUnit::micrometre);
    BOOST_CHECK_CLOSE(micrometres, _micrometres, 0.001);

    double _nanometres = LengthConverter::convert(yards, LengthUnit::yard, LengthUnit::nanometre);
    BOOST_CHECK_CLOSE(nanometres, _nanometres, 0.001);

    double _kilometres = LengthConverter::convert(yards, LengthUnit::yard, LengthUnit::kilometre);
    BOOST_CHECK_CLOSE(kilometres, _kilometres, 0.001);

    double _hectometres = LengthConverter::convert(yards, LengthUnit::yard, LengthUnit::hectometre);
    BOOST_CHECK_CLOSE(hectometres, _hectometres, 0.001);

    double _decametres = LengthConverter::convert(yards, LengthUnit::yard, LengthUnit::decametre);
    BOOST_CHECK_CLOSE(decametres, _decametres, 0.001);

    double _inches = LengthConverter::convert(yards, LengthUnit::yard, LengthUnit::inch);
    BOOST_CHECK_CLOSE(inches, _inches, 0.001);

    double _feet = LengthConverter::convert(yards, LengthUnit::yard, LengthUnit::foot);
    BOOST_CHECK_CLOSE(feet, _feet, 0.001);

    double _chains = LengthConverter::convert(yards, LengthUnit::yard, LengthUnit::chain);
    BOOST_CHECK_CLOSE(chains, _chains, 0.001);

    double _furlongs = LengthConverter::convert(yards, LengthUnit::yard, LengthUnit::furlong);
    BOOST_CHECK_CLOSE(furlongs, _furlongs, 0.001);

    double _miles = LengthConverter::convert(yards, LengthUnit::yard, LengthUnit::mile);
    BOOST_CHECK_CLOSE(miles, _miles, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_chains, LengthConverterTest)
{
    double _metres = LengthConverter::convert(chains, LengthUnit::chain, LengthUnit::metre);
    BOOST_CHECK_CLOSE(metres, _metres, 0.001);

    double _centimetres = LengthConverter::convert(chains, LengthUnit::chain, LengthUnit::centimetre);
    BOOST_CHECK_CLOSE(centimetres, _centimetres, 0.001);

    double _decimetres = LengthConverter::convert(chains, LengthUnit::chain, LengthUnit::decimetre);
    BOOST_CHECK_CLOSE(decimetres, _decimetres, 0.001);

    double _millimetres = LengthConverter::convert(chains, LengthUnit::chain, LengthUnit::millimetre);
    BOOST_CHECK_CLOSE(millimetres, _millimetres, 0.001);

    double _micrometres = LengthConverter::convert(chains, LengthUnit::chain, LengthUnit::micrometre);
    BOOST_CHECK_CLOSE(micrometres, _micrometres, 0.001);

    double _nanometres = LengthConverter::convert(chains, LengthUnit::chain, LengthUnit::nanometre);
    BOOST_CHECK_CLOSE(nanometres, _nanometres, 0.001);

    double _kilometres = LengthConverter::convert(chains, LengthUnit::chain, LengthUnit::kilometre);
    BOOST_CHECK_CLOSE(kilometres, _kilometres, 0.001);

    double _hectometres = LengthConverter::convert(chains, LengthUnit::chain, LengthUnit::hectometre);
    BOOST_CHECK_CLOSE(hectometres, _hectometres, 0.001);

    double _decametres = LengthConverter::convert(chains, LengthUnit::chain, LengthUnit::decametre);
    BOOST_CHECK_CLOSE(decametres, _decametres, 0.001);

    double _inches = LengthConverter::convert(chains, LengthUnit::chain, LengthUnit::inch);
    BOOST_CHECK_CLOSE(inches, _inches, 0.001);

    double _feet = LengthConverter::convert(chains, LengthUnit::chain, LengthUnit::foot);
    BOOST_CHECK_CLOSE(feet, _feet, 0.001);

    double _yards = LengthConverter::convert(chains, LengthUnit::chain, LengthUnit::yard);
    BOOST_CHECK_CLOSE(yards, _yards, 0.001);

    double _furlongs = LengthConverter::convert(chains, LengthUnit::chain, LengthUnit::furlong);
    BOOST_CHECK_CLOSE(furlongs, _furlongs, 0.001);

    double _miles = LengthConverter::convert(chains, LengthUnit::chain, LengthUnit::mile);
    BOOST_CHECK_CLOSE(miles, _miles, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_furlongs, LengthConverterTest)
{
    double _metres = LengthConverter::convert(furlongs, LengthUnit::furlong, LengthUnit::metre);
    BOOST_CHECK_CLOSE(metres, _metres, 0.001);

    double _centimetres = LengthConverter::convert(furlongs, LengthUnit::furlong, LengthUnit::centimetre);
    BOOST_CHECK_CLOSE(centimetres, _centimetres, 0.001);

    double _decimetres = LengthConverter::convert(furlongs, LengthUnit::furlong, LengthUnit::decimetre);
    BOOST_CHECK_CLOSE(decimetres, _decimetres, 0.001);

    double _millimetres = LengthConverter::convert(furlongs, LengthUnit::furlong, LengthUnit::millimetre);
    BOOST_CHECK_CLOSE(millimetres, _millimetres, 0.001);

    double _micrometres = LengthConverter::convert(furlongs, LengthUnit::furlong, LengthUnit::micrometre);
    BOOST_CHECK_CLOSE(micrometres, _micrometres, 0.001);

    double _nanometres = LengthConverter::convert(furlongs, LengthUnit::furlong, LengthUnit::nanometre);
    BOOST_CHECK_CLOSE(nanometres, _nanometres, 0.001);

    double _kilometres = LengthConverter::convert(furlongs, LengthUnit::furlong, LengthUnit::kilometre);
    BOOST_CHECK_CLOSE(kilometres, _kilometres, 0.001);

    double _hectometres = LengthConverter::convert(furlongs, LengthUnit::furlong, LengthUnit::hectometre);
    BOOST_CHECK_CLOSE(hectometres, _hectometres, 0.001);

    double _decametres = LengthConverter::convert(furlongs, LengthUnit::furlong, LengthUnit::decametre);
    BOOST_CHECK_CLOSE(decametres, _decametres, 0.001);

    double _inches = LengthConverter::convert(furlongs, LengthUnit::furlong, LengthUnit::inch);
    BOOST_CHECK_CLOSE(inches, _inches, 0.001);

    double _feet = LengthConverter::convert(furlongs, LengthUnit::furlong, LengthUnit::foot);
    BOOST_CHECK_CLOSE(feet, _feet, 0.001);

    double _yards = LengthConverter::convert(furlongs, LengthUnit::furlong, LengthUnit::yard);
    BOOST_CHECK_CLOSE(yards, _yards, 0.001);

    double _chains = LengthConverter::convert(furlongs, LengthUnit::furlong, LengthUnit::chain);
    BOOST_CHECK_CLOSE(chains, _chains, 0.001);

    double _miles = LengthConverter::convert(furlongs, LengthUnit::furlong, LengthUnit::mile);
    BOOST_CHECK_CLOSE(miles, _miles, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_miles, LengthConverterTest)
{
    double _metres = LengthConverter::convert(miles, LengthUnit::mile, LengthUnit::metre);
    BOOST_CHECK_CLOSE(metres, _metres, 0.001);

    double _centimetres = LengthConverter::convert(miles, LengthUnit::mile, LengthUnit::centimetre);
    BOOST_CHECK_CLOSE(centimetres, _centimetres, 0.001);

    double _decimetres = LengthConverter::convert(miles, LengthUnit::mile, LengthUnit::decimetre);
    BOOST_CHECK_CLOSE(decimetres, _decimetres, 0.001);

    double _millimetres = LengthConverter::convert(miles, LengthUnit::mile, LengthUnit::millimetre);
    BOOST_CHECK_CLOSE(millimetres, _millimetres, 0.001);

    double _micrometres = LengthConverter::convert(miles, LengthUnit::mile, LengthUnit::micrometre);
    BOOST_CHECK_CLOSE(micrometres, _micrometres, 0.001);

    double _nanometres = LengthConverter::convert(miles, LengthUnit::mile, LengthUnit::nanometre);
    BOOST_CHECK_CLOSE(nanometres, _nanometres, 0.001);

    double _kilometres = LengthConverter::convert(miles, LengthUnit::mile, LengthUnit::kilometre);
    BOOST_CHECK_CLOSE(kilometres, _kilometres, 0.001);

    double _hectometres = LengthConverter::convert(miles, LengthUnit::mile, LengthUnit::hectometre);
    BOOST_CHECK_CLOSE(hectometres, _hectometres, 0.001);

    double _decametres = LengthConverter::convert(miles, LengthUnit::mile, LengthUnit::decametre);
    BOOST_CHECK_CLOSE(decametres, _decametres, 0.001);

    double _inches = LengthConverter::convert(miles, LengthUnit::mile, LengthUnit::inch);
    BOOST_CHECK_CLOSE(inches, _inches, 0.001);

    double _feet = LengthConverter::convert(miles, LengthUnit::mile, LengthUnit::foot);
    BOOST_CHECK_CLOSE(feet, _feet, 0.001);

    double _yards = LengthConverter::convert(miles, LengthUnit::mile, LengthUnit::yard);
    BOOST_CHECK_CLOSE(yards, _yards, 0.001);

    double _chains = LengthConverter::convert(miles, LengthUnit::mile, LengthUnit::chain);
    BOOST_CHECK_CLOSE(chains, _chains, 0.001);

    double _furlongs = LengthConverter::convert(miles, LengthUnit::mile, LengthUnit::furlong);
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
    double _decigrams = MassConverter::convert(grams, MassUnit::gram, MassUnit::decigram);
    BOOST_CHECK_EQUAL(decigrams, _decigrams);

    double _centigrams = MassConverter::convert(grams, MassUnit::gram, MassUnit::centigram);
    BOOST_CHECK_EQUAL(centigrams, _centigrams);

    double _milligrams = MassConverter::convert(grams, MassUnit::gram, MassUnit::milligram);
    BOOST_CHECK_EQUAL(milligrams, _milligrams);

    double _micrograms = MassConverter::convert(grams, MassUnit::gram, MassUnit::microgram);
    BOOST_CHECK_EQUAL(micrograms, _micrograms);

    double _nanograms = MassConverter::convert(grams, MassUnit::gram, MassUnit::nanogram);
    BOOST_CHECK_EQUAL(nanograms, _nanograms);

    double _kilograms = MassConverter::convert(grams, MassUnit::gram, MassUnit::kilogram);
    BOOST_CHECK_CLOSE(kilograms, _kilograms, 0.001);

    double _hectograms = MassConverter::convert(grams, MassUnit::gram, MassUnit::hectogram);
    BOOST_CHECK_EQUAL(hectograms, _hectograms);

    double _decagrams = MassConverter::convert(grams, MassUnit::gram, MassUnit::decagram);
    BOOST_CHECK_EQUAL(decagrams, _decagrams);

    double _grains = MassConverter::convert(grams, MassUnit::gram, MassUnit::grain);
    BOOST_CHECK_CLOSE(grains, _grains, 0.001);

    double _ounces = MassConverter::convert(grams, MassUnit::gram, MassUnit::ounce);
    BOOST_CHECK_CLOSE(ounces, _ounces, 0.001);

    double _pounds = MassConverter::convert(grams, MassUnit::gram, MassUnit::pound);
    BOOST_CHECK_CLOSE(pounds, _pounds, 0.001);

    double _stones = MassConverter::convert(grams, MassUnit::gram, MassUnit::stone);
    BOOST_CHECK_CLOSE(stones, _stones, 0.001);

    double _quarters = MassConverter::convert(grams, MassUnit::gram, MassUnit::quarter);
    BOOST_CHECK_CLOSE(quarters, _quarters, 0.001);

    double _hundredweights = MassConverter::convert(grams, MassUnit::gram, MassUnit::hundredweight);
    BOOST_CHECK_CLOSE(hundredweights, _hundredweights, 0.001);

    double _tons = MassConverter::convert(grams, MassUnit::gram, MassUnit::ton);
    BOOST_CHECK_CLOSE(tons, _tons, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_decigrams, MassConverterTest)
{
    double _grams = MassConverter::convert(decigrams, MassUnit::decigram, MassUnit::gram);
    BOOST_CHECK_EQUAL(grams, _grams);

    double _centigrams = MassConverter::convert(decigrams, MassUnit::decigram, MassUnit::centigram);
    BOOST_CHECK_EQUAL(centigrams, _centigrams);

    double _milligrams = MassConverter::convert(decigrams, MassUnit::decigram, MassUnit::milligram);
    BOOST_CHECK_EQUAL(milligrams, _milligrams);

    double _micrograms = MassConverter::convert(decigrams, MassUnit::decigram, MassUnit::microgram);
    BOOST_CHECK_EQUAL(micrograms, _micrograms);

    double _nanograms = MassConverter::convert(decigrams, MassUnit::decigram, MassUnit::nanogram);
    BOOST_CHECK_EQUAL(nanograms, _nanograms);

    double _kilograms = MassConverter::convert(decigrams, MassUnit::decigram, MassUnit::kilogram);
    BOOST_CHECK_CLOSE(kilograms, _kilograms, 0.001);

    double _hectograms = MassConverter::convert(decigrams, MassUnit::decigram, MassUnit::hectogram);
    BOOST_CHECK_EQUAL(hectograms, _hectograms);

    double _decagrams = MassConverter::convert(decigrams, MassUnit::decigram, MassUnit::decagram);
    BOOST_CHECK_CLOSE(decagrams, _decagrams, 0.001);

    double _grains = MassConverter::convert(decigrams, MassUnit::decigram, MassUnit::grain);
    BOOST_CHECK_CLOSE(grains, _grains, 0.001);

    double _ounces = MassConverter::convert(decigrams, MassUnit::decigram, MassUnit::ounce);
    BOOST_CHECK_CLOSE(ounces, _ounces, 0.001);

    double _pounds = MassConverter::convert(decigrams, MassUnit::decigram, MassUnit::pound);
    BOOST_CHECK_CLOSE(pounds, _pounds, 0.001);

    double _stones = MassConverter::convert(decigrams, MassUnit::decigram, MassUnit::stone);
    BOOST_CHECK_CLOSE(stones, _stones, 0.001);

    double _quarters = MassConverter::convert(decigrams, MassUnit::decigram, MassUnit::quarter);
    BOOST_CHECK_CLOSE(quarters, _quarters, 0.001);

    double _hundredweights = MassConverter::convert(decigrams, MassUnit::decigram, MassUnit::hundredweight);
    BOOST_CHECK_CLOSE(hundredweights, _hundredweights, 0.001);

    double _tons = MassConverter::convert(decigrams, MassUnit::decigram, MassUnit::ton);
    BOOST_CHECK_CLOSE(tons, _tons, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_centigrams, MassConverterTest)
{
    double _grams = MassConverter::convert(centigrams, MassUnit::centigram, MassUnit::gram);
    BOOST_CHECK_EQUAL(grams, _grams);

    double _decigrams = MassConverter::convert(centigrams, MassUnit::centigram, MassUnit::decigram);
    BOOST_CHECK_EQUAL(decigrams, _decigrams);

    double _milligrams = MassConverter::convert(centigrams, MassUnit::centigram, MassUnit::milligram);
    BOOST_CHECK_EQUAL(milligrams, _milligrams);

    double _micrograms = MassConverter::convert(centigrams, MassUnit::centigram, MassUnit::microgram);
    BOOST_CHECK_EQUAL(micrograms, _micrograms);

    double _nanograms = MassConverter::convert(centigrams, MassUnit::centigram, MassUnit::nanogram);
    BOOST_CHECK_EQUAL(nanograms, _nanograms);

    double _kilograms = MassConverter::convert(centigrams, MassUnit::centigram, MassUnit::kilogram);
    BOOST_CHECK_CLOSE(kilograms, _kilograms, 0.001);

    double _hectograms = MassConverter::convert(centigrams, MassUnit::centigram, MassUnit::hectogram);
    BOOST_CHECK_CLOSE(hectograms, _hectograms, 0.001);

    double _decagrams = MassConverter::convert(centigrams, MassUnit::centigram, MassUnit::decagram);
    BOOST_CHECK_CLOSE(decagrams, _decagrams, 0.001);

    double _grains = MassConverter::convert(centigrams, MassUnit::centigram, MassUnit::grain);
    BOOST_CHECK_CLOSE(grains, _grains, 0.001);

    double _ounces = MassConverter::convert(centigrams, MassUnit::centigram, MassUnit::ounce);
    BOOST_CHECK_CLOSE(ounces, _ounces, 0.001);

    double _pounds = MassConverter::convert(centigrams, MassUnit::centigram, MassUnit::pound);
    BOOST_CHECK_CLOSE(pounds, _pounds, 0.001);

    double _stones = MassConverter::convert(centigrams, MassUnit::centigram, MassUnit::stone);
    BOOST_CHECK_CLOSE(stones, _stones, 0.001);

    double _quarters = MassConverter::convert(centigrams, MassUnit::centigram, MassUnit::quarter);
    BOOST_CHECK_CLOSE(quarters, _quarters, 0.001);

    double _hundredweights = MassConverter::convert(centigrams, MassUnit::centigram, MassUnit::hundredweight);
    BOOST_CHECK_CLOSE(hundredweights, _hundredweights, 0.001);

    double _tons = MassConverter::convert(centigrams, MassUnit::centigram, MassUnit::ton);
    BOOST_CHECK_CLOSE(tons, _tons, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_milligrams, MassConverterTest)
{
    double _grams = MassConverter::convert(milligrams, MassUnit::milligram, MassUnit::gram);
    BOOST_CHECK_EQUAL(grams, _grams);

    double _centigrams = MassConverter::convert(milligrams, MassUnit::milligram, MassUnit::centigram);
    BOOST_CHECK_EQUAL(centigrams, _centigrams);

    double _decigrams = MassConverter::convert(milligrams, MassUnit::milligram, MassUnit::decigram);
    BOOST_CHECK_EQUAL(decigrams, _decigrams);

    double _micrograms = MassConverter::convert(milligrams, MassUnit::milligram, MassUnit::microgram);
    BOOST_CHECK_EQUAL(micrograms, _micrograms);

    double _nanograms = MassConverter::convert(milligrams, MassUnit::milligram, MassUnit::nanogram);
    BOOST_CHECK_EQUAL(nanograms, _nanograms);

    double _kilograms = MassConverter::convert(milligrams, MassUnit::milligram, MassUnit::kilogram);
    BOOST_CHECK_CLOSE(kilograms, _kilograms, 0.001);

    double _hectograms = MassConverter::convert(milligrams, MassUnit::milligram, MassUnit::hectogram);
    BOOST_CHECK_CLOSE(hectograms, _hectograms, 0.001);

    double _decagrams = MassConverter::convert(milligrams, MassUnit::milligram, MassUnit::decagram);
    BOOST_CHECK_CLOSE(decagrams, _decagrams, 0.001);

    double _grains = MassConverter::convert(milligrams, MassUnit::milligram, MassUnit::grain);
    BOOST_CHECK_CLOSE(grains, _grains, 0.001);

    double _ounces = MassConverter::convert(milligrams, MassUnit::milligram, MassUnit::ounce);
    BOOST_CHECK_CLOSE(ounces, _ounces, 0.001);

    double _pounds = MassConverter::convert(milligrams, MassUnit::milligram, MassUnit::pound);
    BOOST_CHECK_CLOSE(pounds, _pounds, 0.001);

    double _stones = MassConverter::convert(milligrams, MassUnit::milligram, MassUnit::stone);
    BOOST_CHECK_CLOSE(stones, _stones, 0.001);

    double _quarters = MassConverter::convert(milligrams, MassUnit::milligram, MassUnit::quarter);
    BOOST_CHECK_CLOSE(quarters, _quarters, 0.001);

    double _hundredweights = MassConverter::convert(milligrams, MassUnit::milligram, MassUnit::hundredweight);
    BOOST_CHECK_CLOSE(hundredweights, _hundredweights, 0.001);

    double _tons = MassConverter::convert(milligrams, MassUnit::milligram, MassUnit::ton);
    BOOST_CHECK_CLOSE(tons, _tons, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_micrograms, MassConverterTest)
{
    double _grams = MassConverter::convert(micrograms, MassUnit::microgram, MassUnit::gram);
    BOOST_CHECK_CLOSE(grams, _grams, 0.001);

    double _centigrams = MassConverter::convert(micrograms, MassUnit::microgram, MassUnit::centigram);
    BOOST_CHECK_CLOSE(centigrams, _centigrams, 0.001);

    double _decigrams = MassConverter::convert(micrograms, MassUnit::microgram, MassUnit::decigram);
    BOOST_CHECK_CLOSE(decigrams, _decigrams, 0.001);

    double _milligrams = MassConverter::convert(micrograms, MassUnit::microgram, MassUnit::milligram);
    BOOST_CHECK_EQUAL(milligrams, _milligrams);

    double _nanograms = MassConverter::convert(micrograms, MassUnit::microgram, MassUnit::nanogram);
    BOOST_CHECK_EQUAL(nanograms, _nanograms);

    double _kilograms = MassConverter::convert(micrograms, MassUnit::microgram, MassUnit::kilogram);
    BOOST_CHECK_CLOSE(kilograms, _kilograms, 0.001);

    double _hectograms = MassConverter::convert(micrograms, MassUnit::microgram, MassUnit::hectogram);
    BOOST_CHECK_EQUAL(hectograms, _hectograms);

    double _decagrams = MassConverter::convert(micrograms, MassUnit::microgram, MassUnit::decagram);
    BOOST_CHECK_CLOSE(decagrams, _decagrams, 0.001);

    double _grains = MassConverter::convert(micrograms, MassUnit::microgram, MassUnit::grain);
    BOOST_CHECK_CLOSE(grains, _grains, 0.001);

    double _ounces = MassConverter::convert(micrograms, MassUnit::microgram, MassUnit::ounce);
    BOOST_CHECK_CLOSE(ounces, _ounces, 0.001);

    double _pounds = MassConverter::convert(micrograms, MassUnit::microgram, MassUnit::pound);
    BOOST_CHECK_CLOSE(pounds, _pounds, 0.001);

    double _stones = MassConverter::convert(micrograms, MassUnit::microgram, MassUnit::stone);
    BOOST_CHECK_CLOSE(stones, _stones, 0.001);

    double _quarters = MassConverter::convert(micrograms, MassUnit::microgram, MassUnit::quarter);
    BOOST_CHECK_CLOSE(quarters, _quarters, 0.001);

    double _hundredweights = MassConverter::convert(micrograms, MassUnit::microgram, MassUnit::hundredweight);
    BOOST_CHECK_CLOSE(hundredweights, _hundredweights, 0.001);

    double _tons = MassConverter::convert(micrograms, MassUnit::microgram, MassUnit::ton);
    BOOST_CHECK_CLOSE(tons, _tons, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_nanograms, MassConverterTest)
{
    double _grams = MassConverter::convert(nanograms, MassUnit::nanogram, MassUnit::gram);
    BOOST_CHECK_CLOSE(grams, _grams, 0.001);

    double _centigrams = MassConverter::convert(nanograms, MassUnit::nanogram, MassUnit::centigram);
    BOOST_CHECK_CLOSE(centigrams, _centigrams, 0.001);

    double _decigrams = MassConverter::convert(nanograms, MassUnit::nanogram, MassUnit::decigram);
    BOOST_CHECK_CLOSE(decigrams, _decigrams, 0.001);

    double _milligrams = MassConverter::convert(nanograms, MassUnit::nanogram, MassUnit::milligram);
    BOOST_CHECK_CLOSE(milligrams, _milligrams, 0.001);

    double _micrograms = MassConverter::convert(nanograms, MassUnit::nanogram, MassUnit::microgram);
    BOOST_CHECK_EQUAL(micrograms, _micrograms);

    double _kilograms = MassConverter::convert(nanograms, MassUnit::nanogram, MassUnit::kilogram);
    BOOST_CHECK_CLOSE(kilograms, _kilograms, 0.001);

    double _hectograms = MassConverter::convert(nanograms, MassUnit::nanogram, MassUnit::hectogram);
    BOOST_CHECK_CLOSE(hectograms, _hectograms, 0.001);

    double _decagrams = MassConverter::convert(nanograms, MassUnit::nanogram, MassUnit::decagram);
    BOOST_CHECK_CLOSE(decagrams, _decagrams, 0.001);

    double _grains = MassConverter::convert(nanograms, MassUnit::nanogram, MassUnit::grain);
    BOOST_CHECK_CLOSE(grains, _grains, 0.001);

    double _ounces = MassConverter::convert(nanograms, MassUnit::nanogram, MassUnit::ounce);
    BOOST_CHECK_CLOSE(ounces, _ounces, 0.001);

    double _pounds = MassConverter::convert(nanograms, MassUnit::nanogram, MassUnit::pound);
    BOOST_CHECK_CLOSE(pounds, _pounds, 0.001);

    double _stones = MassConverter::convert(nanograms, MassUnit::nanogram, MassUnit::stone);
    BOOST_CHECK_CLOSE(stones, _stones, 0.001);

    double _quarters = MassConverter::convert(nanograms, MassUnit::nanogram, MassUnit::quarter);
    BOOST_CHECK_CLOSE(quarters, _quarters, 0.001);

    double _hundredweights = MassConverter::convert(nanograms, MassUnit::nanogram, MassUnit::hundredweight);
    BOOST_CHECK_CLOSE(hundredweights, _hundredweights, 0.001);

    double _tons = MassConverter::convert(nanograms, MassUnit::nanogram, MassUnit::ton);
    BOOST_CHECK_CLOSE(tons, _tons, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_kilograms, MassConverterTest)
{
    double _grams = MassConverter::convert(kilograms, MassUnit::kilogram, MassUnit::gram);
    BOOST_CHECK_CLOSE(grams, _grams, 0.001);

    double _centigrams = MassConverter::convert(kilograms, MassUnit::kilogram, MassUnit::centigram);
    BOOST_CHECK_CLOSE(centigrams, _centigrams, 0.001);

    double _decigrams = MassConverter::convert(kilograms, MassUnit::kilogram, MassUnit::decigram);
    BOOST_CHECK_CLOSE(decigrams, _decigrams, 0.001);

    double _milligrams = MassConverter::convert(kilograms, MassUnit::kilogram, MassUnit::milligram);
    BOOST_CHECK_CLOSE(milligrams, _milligrams, 0.001);

    double _micrograms = MassConverter::convert(kilograms, MassUnit::kilogram, MassUnit::microgram);
    BOOST_CHECK_EQUAL(micrograms, _micrograms);

    double _nanograms = MassConverter::convert(kilograms, MassUnit::kilogram, MassUnit::nanogram);
    BOOST_CHECK_CLOSE(nanograms, _nanograms, 0.001);

    double _hectograms = MassConverter::convert(kilograms, MassUnit::kilogram, MassUnit::hectogram);
    BOOST_CHECK_EQUAL(hectograms, _hectograms);

    double _decagrams = MassConverter::convert(kilograms, MassUnit::kilogram, MassUnit::decagram);
    BOOST_CHECK_CLOSE(decagrams, _decagrams, 0.001);

    double _grains = MassConverter::convert(kilograms, MassUnit::kilogram, MassUnit::grain);
    BOOST_CHECK_CLOSE(grains, _grains, 0.001);

    double _ounces = MassConverter::convert(kilograms, MassUnit::kilogram, MassUnit::ounce);
    BOOST_CHECK_CLOSE(ounces, _ounces, 0.001);

    double _pounds = MassConverter::convert(kilograms, MassUnit::kilogram, MassUnit::pound);
    BOOST_CHECK_CLOSE(pounds, _pounds, 0.001);

    double _stones = MassConverter::convert(kilograms, MassUnit::kilogram, MassUnit::stone);
    BOOST_CHECK_CLOSE(stones, _stones, 0.001);

    double _quarters = MassConverter::convert(kilograms, MassUnit::kilogram, MassUnit::quarter);
    BOOST_CHECK_CLOSE(quarters, _quarters, 0.001);

    double _hundredweights = MassConverter::convert(kilograms, MassUnit::kilogram, MassUnit::hundredweight);
    BOOST_CHECK_CLOSE(hundredweights, _hundredweights, 0.001);

    double _tons = MassConverter::convert(kilograms, MassUnit::kilogram, MassUnit::ton);
    BOOST_CHECK_CLOSE(tons, _tons, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_hectograms, MassConverterTest)
{
    double _grams = MassConverter::convert(hectograms, MassUnit::hectogram, MassUnit::gram);
    BOOST_CHECK_CLOSE(grams, _grams, 0.001);

    double _centigrams = MassConverter::convert(hectograms, MassUnit::hectogram, MassUnit::centigram);
    BOOST_CHECK_CLOSE(centigrams, _centigrams, 0.001);

    double _decigrams = MassConverter::convert(hectograms, MassUnit::hectogram, MassUnit::decigram);
    BOOST_CHECK_CLOSE(decigrams, _decigrams, 0.001);

    double _milligrams = MassConverter::convert(hectograms, MassUnit::hectogram, MassUnit::milligram);
    BOOST_CHECK_CLOSE(milligrams, _milligrams, 0.001);

    double _micrograms = MassConverter::convert(hectograms, MassUnit::hectogram, MassUnit::microgram);
    BOOST_CHECK_CLOSE(micrograms, _micrograms, 0.001);

    double _nanograms = MassConverter::convert(hectograms, MassUnit::hectogram, MassUnit::nanogram);
    BOOST_CHECK_CLOSE(nanograms, _nanograms, 0.001);

    double _kilograms = MassConverter::convert(hectograms, MassUnit::hectogram, MassUnit::kilogram);
    BOOST_CHECK_CLOSE(kilograms, _kilograms, 0.001);

    double _decagrams = MassConverter::convert(hectograms, MassUnit::hectogram, MassUnit::decagram);
    BOOST_CHECK_CLOSE(decagrams, _decagrams, 0.001);

    double _grains = MassConverter::convert(hectograms, MassUnit::hectogram, MassUnit::grain);
    BOOST_CHECK_CLOSE(grains, _grains, 0.001);

    double _ounces = MassConverter::convert(hectograms, MassUnit::hectogram, MassUnit::ounce);
    BOOST_CHECK_CLOSE(ounces, _ounces, 0.001);

    double _pounds = MassConverter::convert(hectograms, MassUnit::hectogram, MassUnit::pound);
    BOOST_CHECK_CLOSE(pounds, _pounds, 0.001);

    double _stones = MassConverter::convert(hectograms, MassUnit::hectogram, MassUnit::stone);
    BOOST_CHECK_CLOSE(stones, _stones, 0.001);

    double _quarters = MassConverter::convert(hectograms, MassUnit::hectogram, MassUnit::quarter);
    BOOST_CHECK_CLOSE(quarters, _quarters, 0.001);

    double _hundredweights = MassConverter::convert(hectograms, MassUnit::hectogram, MassUnit::hundredweight);
    BOOST_CHECK_CLOSE(hundredweights, _hundredweights, 0.001);

    double _tons = MassConverter::convert(hectograms, MassUnit::hectogram, MassUnit::ton);
    BOOST_CHECK_CLOSE(tons, _tons, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_decagrams, MassConverterTest)
{
    double _grams = MassConverter::convert(decagrams, MassUnit::decagram, MassUnit::gram);
    BOOST_CHECK_CLOSE(grams, _grams, 0.001);

    double _centigrams = MassConverter::convert(decagrams, MassUnit::decagram, MassUnit::centigram);
    BOOST_CHECK_CLOSE(centigrams, _centigrams, 0.001);

    double _decigrams = MassConverter::convert(decagrams, MassUnit::decagram, MassUnit::decigram);
    BOOST_CHECK_CLOSE(decigrams, _decigrams, 0.001);

    double _milligrams = MassConverter::convert(decagrams, MassUnit::decagram, MassUnit::milligram);
    BOOST_CHECK_CLOSE(milligrams, _milligrams, 0.001);

    double _micrograms = MassConverter::convert(decagrams, MassUnit::decagram, MassUnit::microgram);
    BOOST_CHECK_CLOSE(micrograms, _micrograms, 0.001);

    double _nanograms = MassConverter::convert(decagrams, MassUnit::decagram, MassUnit::nanogram);
    BOOST_CHECK_CLOSE(nanograms, _nanograms, 0.001);

    double _kilograms = MassConverter::convert(decagrams, MassUnit::decagram, MassUnit::kilogram);
    BOOST_CHECK_CLOSE(kilograms, _kilograms, 0.001);

    double _hectograms = MassConverter::convert(decagrams, MassUnit::decagram, MassUnit::hectogram);
    BOOST_CHECK_CLOSE(hectograms, _hectograms, 0.001);

    double _grains = MassConverter::convert(decagrams, MassUnit::decagram, MassUnit::grain);
    BOOST_CHECK_CLOSE(grains, _grains, 0.001);

    double _ounces = MassConverter::convert(decagrams, MassUnit::decagram, MassUnit::ounce);
    BOOST_CHECK_CLOSE(ounces, _ounces, 0.001);

    double _pounds = MassConverter::convert(decagrams, MassUnit::decagram, MassUnit::pound);
    BOOST_CHECK_CLOSE(pounds, _pounds, 0.001);

    double _stones = MassConverter::convert(decagrams, MassUnit::decagram, MassUnit::stone);
    BOOST_CHECK_CLOSE(stones, _stones, 0.001);

    double _quarters = MassConverter::convert(decagrams, MassUnit::decagram, MassUnit::quarter);
    BOOST_CHECK_CLOSE(quarters, _quarters, 0.001);

    double _hundredweights = MassConverter::convert(decagrams, MassUnit::decagram, MassUnit::hundredweight);
    BOOST_CHECK_CLOSE(hundredweights, _hundredweights, 0.001);

    double _tons = MassConverter::convert(decagrams, MassUnit::decagram, MassUnit::ton);
    BOOST_CHECK_CLOSE(tons, _tons, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_grains, MassConverterTest)
{
    double _grams = MassConverter::convert(grains, MassUnit::grain, MassUnit::gram);
    BOOST_CHECK_CLOSE(grams, _grams, 0.001);

    double _centigrams = MassConverter::convert(grains, MassUnit::grain, MassUnit::centigram);
    BOOST_CHECK_CLOSE(centigrams, _centigrams, 0.001);

    double _decigrams = MassConverter::convert(grains, MassUnit::grain, MassUnit::decigram);
    BOOST_CHECK_CLOSE(decigrams, _decigrams, 0.001);

    double _milligrams = MassConverter::convert(grains, MassUnit::grain, MassUnit::milligram);
    BOOST_CHECK_CLOSE(milligrams, _milligrams, 0.001);

    double _micrograms = MassConverter::convert(grains, MassUnit::grain, MassUnit::microgram);
    BOOST_CHECK_CLOSE(micrograms, _micrograms, 0.001);

    double _nanograms = MassConverter::convert(grains, MassUnit::grain, MassUnit::nanogram);
    BOOST_CHECK_CLOSE(nanograms, _nanograms, 0.001);

    double _kilograms = MassConverter::convert(grains, MassUnit::grain, MassUnit::kilogram);
    BOOST_CHECK_CLOSE(kilograms, _kilograms, 0.001);

    double _hectograms = MassConverter::convert(grains, MassUnit::grain, MassUnit::hectogram);
    BOOST_CHECK_CLOSE(hectograms, _hectograms, 0.001);

    double _decagrams = MassConverter::convert(grains, MassUnit::grain, MassUnit::decagram);
    BOOST_CHECK_CLOSE(decagrams, _decagrams, 0.001);

    double _ounces = MassConverter::convert(grains, MassUnit::grain, MassUnit::ounce);
    BOOST_CHECK_CLOSE(ounces, _ounces, 0.001);

    double _pounds = MassConverter::convert(grains, MassUnit::grain, MassUnit::pound);
    BOOST_CHECK_CLOSE(pounds, _pounds, 0.001);

    double _stones = MassConverter::convert(grains, MassUnit::grain, MassUnit::stone);
    BOOST_CHECK_CLOSE(stones, _stones, 0.001);

    double _quarters = MassConverter::convert(grains, MassUnit::grain, MassUnit::quarter);
    BOOST_CHECK_CLOSE(quarters, _quarters, 0.001);

    double _hundredweights = MassConverter::convert(grains, MassUnit::grain, MassUnit::hundredweight);
    BOOST_CHECK_CLOSE(hundredweights, _hundredweights, 0.001);

    double _tons = MassConverter::convert(grains, MassUnit::grain, MassUnit::ton);
    BOOST_CHECK_CLOSE(tons, _tons, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_ounces, MassConverterTest)
{
    double _grams = MassConverter::convert(ounces, MassUnit::ounce, MassUnit::gram);
    BOOST_CHECK_CLOSE(grams, _grams, 0.001);

    double _centigrams = MassConverter::convert(ounces, MassUnit::ounce, MassUnit::centigram);
    BOOST_CHECK_CLOSE(centigrams, _centigrams, 0.001);

    double _decigrams = MassConverter::convert(ounces, MassUnit::ounce, MassUnit::decigram);
    BOOST_CHECK_CLOSE(decigrams, _decigrams, 0.001);

    double _milligrams = MassConverter::convert(ounces, MassUnit::ounce, MassUnit::milligram);
    BOOST_CHECK_CLOSE(milligrams, _milligrams, 0.001);

    double _micrograms = MassConverter::convert(ounces, MassUnit::ounce, MassUnit::microgram);
    BOOST_CHECK_CLOSE(micrograms, _micrograms, 0.001);

    double _nanograms = MassConverter::convert(ounces, MassUnit::ounce, MassUnit::nanogram);
    BOOST_CHECK_CLOSE(nanograms, _nanograms, 0.001);

    double _kilograms = MassConverter::convert(ounces, MassUnit::ounce, MassUnit::kilogram);
    BOOST_CHECK_CLOSE(kilograms, _kilograms, 0.001);

    double _hectograms = MassConverter::convert(ounces, MassUnit::ounce, MassUnit::hectogram);
    BOOST_CHECK_CLOSE(hectograms, _hectograms, 0.001);

    double _decagrams = MassConverter::convert(ounces, MassUnit::ounce, MassUnit::decagram);
    BOOST_CHECK_CLOSE(decagrams, _decagrams, 0.001);

    double _grains = MassConverter::convert(ounces, MassUnit::ounce, MassUnit::grain);
    BOOST_CHECK_CLOSE(grains, _grains, 0.001);

    double _pounds = MassConverter::convert(ounces, MassUnit::ounce, MassUnit::pound);
    BOOST_CHECK_CLOSE(pounds, _pounds, 0.001);

    double _stones = MassConverter::convert(ounces, MassUnit::ounce, MassUnit::stone);
    BOOST_CHECK_CLOSE(stones, _stones, 0.001);

    double _quarters = MassConverter::convert(ounces, MassUnit::ounce, MassUnit::quarter);
    BOOST_CHECK_CLOSE(quarters, _quarters, 0.001);

    double _hundredweights = MassConverter::convert(ounces, MassUnit::ounce, MassUnit::hundredweight);
    BOOST_CHECK_CLOSE(hundredweights, _hundredweights, 0.001);

    double _tons = MassConverter::convert(ounces, MassUnit::ounce, MassUnit::ton);
    BOOST_CHECK_CLOSE(tons, _tons, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_pounds, MassConverterTest)
{
    double _grams = MassConverter::convert(pounds, MassUnit::pound, MassUnit::gram);
    BOOST_CHECK_CLOSE(grams, _grams, 0.001);

    double _centigrams = MassConverter::convert(pounds, MassUnit::pound, MassUnit::centigram);
    BOOST_CHECK_CLOSE(centigrams, _centigrams, 0.001);

    double _decigrams = MassConverter::convert(pounds, MassUnit::pound, MassUnit::decigram);
    BOOST_CHECK_CLOSE(decigrams, _decigrams, 0.001);

    double _milligrams = MassConverter::convert(pounds, MassUnit::pound, MassUnit::milligram);
    BOOST_CHECK_CLOSE(milligrams, _milligrams, 0.001);

    double _micrograms = MassConverter::convert(pounds, MassUnit::pound, MassUnit::microgram);
    BOOST_CHECK_CLOSE(micrograms, _micrograms, 0.001);

    double _nanograms = MassConverter::convert(pounds, MassUnit::pound, MassUnit::nanogram);
    BOOST_CHECK_CLOSE(nanograms, _nanograms, 0.001);

    double _kilograms = MassConverter::convert(pounds, MassUnit::pound, MassUnit::kilogram);
    BOOST_CHECK_CLOSE(kilograms, _kilograms, 0.001);

    double _hectograms = MassConverter::convert(pounds, MassUnit::pound, MassUnit::hectogram);
    BOOST_CHECK_CLOSE(hectograms, _hectograms, 0.001);

    double _decagrams = MassConverter::convert(pounds, MassUnit::pound, MassUnit::decagram);
    BOOST_CHECK_CLOSE(decagrams, _decagrams, 0.001);

    double _grains = MassConverter::convert(pounds, MassUnit::pound, MassUnit::grain);
    BOOST_CHECK_CLOSE(grains, _grains, 0.001);

    double _ounces = MassConverter::convert(pounds, MassUnit::pound, MassUnit::ounce);
    BOOST_CHECK_CLOSE(ounces, _ounces, 0.001);

    double _stones = MassConverter::convert(pounds, MassUnit::pound, MassUnit::stone);
    BOOST_CHECK_CLOSE(stones, _stones, 0.001);

    double _quarters = MassConverter::convert(pounds, MassUnit::pound, MassUnit::quarter);
    BOOST_CHECK_CLOSE(quarters, _quarters, 0.001);

    double _hundredweights = MassConverter::convert(pounds, MassUnit::pound, MassUnit::hundredweight);
    BOOST_CHECK_CLOSE(hundredweights, _hundredweights, 0.001);

    double _tons = MassConverter::convert(pounds, MassUnit::pound, MassUnit::ton);
    BOOST_CHECK_CLOSE(tons, _tons, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_stones, MassConverterTest)
{
    double _grams = MassConverter::convert(stones, MassUnit::stone, MassUnit::gram);
    BOOST_CHECK_CLOSE(grams, _grams, 0.001);

    double _centigrams = MassConverter::convert(stones, MassUnit::stone, MassUnit::centigram);
    BOOST_CHECK_CLOSE(centigrams, _centigrams, 0.001);

    double _decigrams = MassConverter::convert(stones, MassUnit::stone, MassUnit::decigram);
    BOOST_CHECK_CLOSE(decigrams, _decigrams, 0.001);

    double _milligrams = MassConverter::convert(stones, MassUnit::stone, MassUnit::milligram);
    BOOST_CHECK_CLOSE(milligrams, _milligrams, 0.001);

    double _micrograms = MassConverter::convert(stones, MassUnit::stone, MassUnit::microgram);
    BOOST_CHECK_CLOSE(micrograms, _micrograms, 0.001);

    double _nanograms = MassConverter::convert(stones, MassUnit::stone, MassUnit::nanogram);
    BOOST_CHECK_CLOSE(nanograms, _nanograms, 0.001);

    double _kilograms = MassConverter::convert(stones, MassUnit::stone, MassUnit::kilogram);
    BOOST_CHECK_CLOSE(kilograms, _kilograms, 0.001);

    double _hectograms = MassConverter::convert(stones, MassUnit::stone, MassUnit::hectogram);
    BOOST_CHECK_CLOSE(hectograms, _hectograms, 0.001);

    double _decagrams = MassConverter::convert(stones, MassUnit::stone, MassUnit::decagram);
    BOOST_CHECK_CLOSE(decagrams, _decagrams, 0.001);

    double _grains = MassConverter::convert(stones, MassUnit::stone, MassUnit::grain);
    BOOST_CHECK_CLOSE(grains, _grains, 0.001);

    double _ounces = MassConverter::convert(stones, MassUnit::stone, MassUnit::ounce);
    BOOST_CHECK_CLOSE(ounces, _ounces, 0.001);

    double _pounds = MassConverter::convert(stones, MassUnit::stone, MassUnit::pound);
    BOOST_CHECK_CLOSE(pounds, _pounds, 0.001);

    double _quarters = MassConverter::convert(stones, MassUnit::stone, MassUnit::quarter);
    BOOST_CHECK_CLOSE(quarters, _quarters, 0.001);

    double _hundredweights = MassConverter::convert(stones, MassUnit::stone, MassUnit::hundredweight);
    BOOST_CHECK_CLOSE(hundredweights, _hundredweights, 0.001);

    double _tons = MassConverter::convert(stones, MassUnit::stone, MassUnit::ton);
    BOOST_CHECK_CLOSE(tons, _tons, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_quarters, MassConverterTest)
{
    double _grams = MassConverter::convert(quarters, MassUnit::quarter, MassUnit::gram);
    BOOST_CHECK_CLOSE(grams, _grams, 0.001);

    double _centigrams = MassConverter::convert(quarters, MassUnit::quarter, MassUnit::centigram);
    BOOST_CHECK_CLOSE(centigrams, _centigrams, 0.001);

    double _decigrams = MassConverter::convert(quarters, MassUnit::quarter, MassUnit::decigram);
    BOOST_CHECK_CLOSE(decigrams, _decigrams, 0.001);

    double _milligrams = MassConverter::convert(quarters, MassUnit::quarter, MassUnit::milligram);
    BOOST_CHECK_CLOSE(milligrams, _milligrams, 0.001);

    double _micrograms = MassConverter::convert(quarters, MassUnit::quarter, MassUnit::microgram);
    BOOST_CHECK_CLOSE(micrograms, _micrograms, 0.001);

    double _nanograms = MassConverter::convert(quarters, MassUnit::quarter, MassUnit::nanogram);
    BOOST_CHECK_CLOSE(nanograms, _nanograms, 0.001);

    double _kilograms = MassConverter::convert(quarters, MassUnit::quarter, MassUnit::kilogram);
    BOOST_CHECK_CLOSE(kilograms, _kilograms, 0.001);

    double _hectograms = MassConverter::convert(quarters, MassUnit::quarter, MassUnit::hectogram);
    BOOST_CHECK_CLOSE(hectograms, _hectograms, 0.001);

    double _decagrams = MassConverter::convert(quarters, MassUnit::quarter, MassUnit::decagram);
    BOOST_CHECK_CLOSE(decagrams, _decagrams, 0.001);

    double _grains = MassConverter::convert(quarters, MassUnit::quarter, MassUnit::grain);
    BOOST_CHECK_CLOSE(grains, _grains, 0.001);

    double _ounces = MassConverter::convert(quarters, MassUnit::quarter, MassUnit::ounce);
    BOOST_CHECK_CLOSE(ounces, _ounces, 0.001);

    double _pounds = MassConverter::convert(quarters, MassUnit::quarter, MassUnit::pound);
    BOOST_CHECK_CLOSE(pounds, _pounds, 0.001);

    double _stones = MassConverter::convert(quarters, MassUnit::quarter, MassUnit::stone);
    BOOST_CHECK_CLOSE(stones, _stones, 0.001);

    double _hundredweights = MassConverter::convert(quarters, MassUnit::quarter, MassUnit::hundredweight);
    BOOST_CHECK_CLOSE(hundredweights, _hundredweights, 0.001);

    double _tons = MassConverter::convert(quarters, MassUnit::quarter, MassUnit::ton);
    BOOST_CHECK_CLOSE(tons, _tons, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_hundredweights, MassConverterTest)
{
    double _grams = MassConverter::convert(hundredweights, MassUnit::hundredweight, MassUnit::gram);
    BOOST_CHECK_CLOSE(grams, _grams, 0.001);

    double _centigrams = MassConverter::convert(hundredweights, MassUnit::hundredweight, MassUnit::centigram);
    BOOST_CHECK_CLOSE(centigrams, _centigrams, 0.001);

    double _decigrams = MassConverter::convert(hundredweights, MassUnit::hundredweight, MassUnit::decigram);
    BOOST_CHECK_CLOSE(decigrams, _decigrams, 0.001);

    double _milligrams = MassConverter::convert(hundredweights, MassUnit::hundredweight, MassUnit::milligram);
    BOOST_CHECK_CLOSE(milligrams, _milligrams, 0.001);

    double _micrograms = MassConverter::convert(hundredweights, MassUnit::hundredweight, MassUnit::microgram);
    BOOST_CHECK_CLOSE(micrograms, _micrograms, 0.001);

    double _nanograms = MassConverter::convert(hundredweights, MassUnit::hundredweight, MassUnit::nanogram);
    BOOST_CHECK_CLOSE(nanograms, _nanograms, 0.001);

    double _kilograms = MassConverter::convert(hundredweights, MassUnit::hundredweight, MassUnit::kilogram);
    BOOST_CHECK_CLOSE(kilograms, _kilograms, 0.001);

    double _hectograms = MassConverter::convert(hundredweights, MassUnit::hundredweight, MassUnit::hectogram);
    BOOST_CHECK_CLOSE(hectograms, _hectograms, 0.001);

    double _decagrams = MassConverter::convert(hundredweights, MassUnit::hundredweight, MassUnit::decagram);
    BOOST_CHECK_CLOSE(decagrams, _decagrams, 0.001);

    double _grains = MassConverter::convert(hundredweights, MassUnit::hundredweight, MassUnit::grain);
    BOOST_CHECK_CLOSE(grains, _grains, 0.001);

    double _ounces = MassConverter::convert(hundredweights, MassUnit::hundredweight, MassUnit::ounce);
    BOOST_CHECK_CLOSE(ounces, _ounces, 0.001);

    double _pounds = MassConverter::convert(hundredweights, MassUnit::hundredweight, MassUnit::pound);
    BOOST_CHECK_CLOSE(pounds, _pounds, 0.001);

    double _stones = MassConverter::convert(hundredweights, MassUnit::hundredweight, MassUnit::stone);
    BOOST_CHECK_CLOSE(stones, _stones, 0.001);

    double _quarters = MassConverter::convert(hundredweights, MassUnit::hundredweight, MassUnit::quarter);
    BOOST_CHECK_CLOSE(quarters, _quarters, 0.001);

    double _tons = MassConverter::convert(hundredweights, MassUnit::hundredweight, MassUnit::ton);
    BOOST_CHECK_CLOSE(tons, _tons, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_tons, MassConverterTest)
{
    double _grams = MassConverter::convert(tons, MassUnit::ton, MassUnit::gram);
    BOOST_CHECK_CLOSE(grams, _grams, 0.001);

    double _centigrams = MassConverter::convert(tons, MassUnit::ton, MassUnit::centigram);
    BOOST_CHECK_CLOSE(centigrams, _centigrams, 0.001);

    double _decigrams = MassConverter::convert(tons, MassUnit::ton, MassUnit::decigram);
    BOOST_CHECK_CLOSE(decigrams, _decigrams, 0.001);

    double _milligrams = MassConverter::convert(tons, MassUnit::ton, MassUnit::milligram);
    BOOST_CHECK_CLOSE(milligrams, _milligrams, 0.001);

    double _micrograms = MassConverter::convert(tons, MassUnit::ton, MassUnit::microgram);
    BOOST_CHECK_CLOSE(micrograms, _micrograms, 0.001);

    double _nanograms = MassConverter::convert(tons, MassUnit::ton, MassUnit::nanogram);
    BOOST_CHECK_CLOSE(nanograms, _nanograms, 0.001);

    double _kilograms = MassConverter::convert(tons, MassUnit::ton, MassUnit::kilogram);
    BOOST_CHECK_CLOSE(kilograms, _kilograms, 0.001);

    double _hectograms = MassConverter::convert(tons, MassUnit::ton, MassUnit::hectogram);
    BOOST_CHECK_CLOSE(hectograms, _hectograms, 0.001);

    double _decagrams = MassConverter::convert(tons, MassUnit::ton, MassUnit::decagram);
    BOOST_CHECK_CLOSE(decagrams, _decagrams, 0.001);

    double _grains = MassConverter::convert(tons, MassUnit::ton, MassUnit::grain);
    BOOST_CHECK_CLOSE(grains, _grains, 0.001);

    double _ounces = MassConverter::convert(tons, MassUnit::ton, MassUnit::ounce);
    BOOST_CHECK_CLOSE(ounces, _ounces, 0.001);

    double _pounds = MassConverter::convert(tons, MassUnit::ton, MassUnit::pound);
    BOOST_CHECK_CLOSE(pounds, _pounds, 0.001);

    double _stones = MassConverter::convert(tons, MassUnit::ton, MassUnit::stone);
    BOOST_CHECK_CLOSE(stones, _stones, 0.001);

    double _quarters = MassConverter::convert(tons, MassUnit::ton, MassUnit::quarter);
    BOOST_CHECK_CLOSE(quarters, _quarters, 0.001);

    double _hundredweights = MassConverter::convert(tons, MassUnit::ton, MassUnit::hundredweight);
    BOOST_CHECK_CLOSE(hundredweights, _hundredweights, 0.001);
}

BOOST_AUTO_TEST_SUITE_END()


/* AreaConverterTestSuite */

BOOST_AUTO_TEST_SUITE(AreaConverterTestSuite)

struct AreaConverterTest
{
    AreaConverterTest() = default;
    ~AreaConverterTest() = default;

    void setup() {}
    void teardown() {}

    // Valores base en metros cuadrados (634.5 m²)
    double square_metres = 634.5;

    // Múltiplos SI (sistema métrico)
    double square_decametres = 6.345;          // 1 dam² = 100 m²
    double square_hectometres = 0.06345;       // 1 hm² = 10000 m²
    double square_kilometres = 0.0006345;      // 1 km² = 1000000 m²
    double hectares = 0.06345;                 // 1 ha = 10000 m² (alias de hectómetro cuadrado)

    // Unidades imperiales
    double acres = 0.1567883645;               // 1 acre = 4046.8564224 m²
    double square_yards = 758.8556843;         // 1 yd² = 0.83612736 m²
    double square_feet = 6829.701159;          // 1 ft² = 0.09290304 m²
};

BOOST_FIXTURE_TEST_CASE(from_square_metre, AreaConverterTest)
{

    // Conversiones a unidades SI
    double _square_decametres = AreaConverter::convert(square_metres,
                                                       AreaUnit::square_metre,
                                                       AreaUnit::square_decametre);
    BOOST_CHECK_CLOSE(square_decametres, _square_decametres, 0.001);

    double _square_hectometres = AreaConverter::convert(square_metres,
                                                        AreaUnit::square_metre,
                                                        AreaUnit::square_hectometre);
    BOOST_CHECK_CLOSE(square_hectometres, _square_hectometres, 0.001);

    double _square_kilometres = AreaConverter::convert(square_metres,
                                                       AreaUnit::square_metre,
                                                       AreaUnit::square_kilometre);
    BOOST_CHECK_CLOSE(square_kilometres, _square_kilometres, 0.001);

    double _hectares = AreaConverter::convert(square_metres,
                                              AreaUnit::square_metre,
                                              AreaUnit::hectare);
    BOOST_CHECK_CLOSE(hectares, _hectares, 0.001);

    // Conversiones a unidades imperiales
    double _acres = AreaConverter::convert(square_metres,
                                           AreaUnit::square_metre,
                                           AreaUnit::acre);
    BOOST_CHECK_CLOSE(acres, _acres, 0.001);

    double _square_yards = AreaConverter::convert(square_metres,
                                                  AreaUnit::square_metre,
                                                  AreaUnit::square_yard);
    BOOST_CHECK_CLOSE(square_yards, _square_yards, 0.001);

    double _square_feet = AreaConverter::convert(square_metres,
                                                 AreaUnit::square_metre,
                                                 AreaUnit::square_foot);
    BOOST_CHECK_CLOSE(square_feet, _square_feet, 0.001);

    // Prueba constexpr para verificar la conversión en tiempo de compilación
    constexpr double _square_decametres_constexpr = AreaConverter::convert(634.5,
                                                                           AreaUnit::square_metre,
                                                                           AreaUnit::square_decametre);
    BOOST_CHECK_CLOSE(square_decametres, _square_decametres, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_square_decametre, AreaConverterTest)
{
    double base_value = square_decametres; // 6.345 dam²

    // De decámetro cuadrado a metro cuadrado
    double _square_metres = AreaConverter::convert(base_value, 
                                                   AreaUnit::square_decametre, 
                                                   AreaUnit::square_metre);
    BOOST_CHECK_CLOSE(square_metres, _square_metres, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_square_hectometre, AreaConverterTest)
{
    double base_value = square_hectometres; // 0.06345 hm²

    // De hectómetro cuadrado a metro cuadrado
    double _square_metres = AreaConverter::convert(base_value,
                                                   AreaUnit::square_hectometre,
                                                   AreaUnit::square_metre);
    BOOST_CHECK_CLOSE(square_metres, _square_metres, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_square_kilometre, AreaConverterTest)
{
    double base_value = square_kilometres; // 0.0006345 km²

    // De kilómetro cuadrado a metro cuadrado
    double _square_metres = AreaConverter::convert(base_value,
                                                   AreaUnit::square_kilometre,
                                                   AreaUnit::square_metre);
    BOOST_CHECK_CLOSE(square_metres, _square_metres, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_hectare, AreaConverterTest)
{
    double base_value = hectares; // 0.06345 ha

    // De hectárea a metro cuadrado
    double _square_metres = AreaConverter::convert(base_value,
                                                   AreaUnit::hectare,
                                                   AreaUnit::square_metre);
    BOOST_CHECK_CLOSE(square_metres, _square_metres, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_acre, AreaConverterTest)
{
    double base_value = acres; // 0.1567904974 acres

    // De acre a metro cuadrado
    double _square_metres = AreaConverter::convert(base_value,
                                                   AreaUnit::acre,
                                                   AreaUnit::square_metre);
    BOOST_CHECK_CLOSE(square_metres, _square_metres, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_square_yard, AreaConverterTest)
{
    double base_value = square_yards; // 758.9254007 yd²

    // De yarda cuadrada a metro cuadrado
    double _square_metres = AreaConverter::convert(base_value,
                                                   AreaUnit::square_yard,
                                                   AreaUnit::square_metre);
    BOOST_CHECK_CLOSE(square_metres, _square_metres, 0.001);
}

BOOST_FIXTURE_TEST_CASE(from_square_foot, AreaConverterTest)
{
    double base_value = square_feet; // 6829.766591 ft²

    // De pie cuadrado a metro cuadrado
    double _square_metres = AreaConverter::convert(base_value,
                                                   AreaUnit::square_foot,
                                                   AreaUnit::square_metre);
    BOOST_CHECK_CLOSE(square_metres, _square_metres, 0.001);
}

BOOST_FIXTURE_TEST_CASE(cross_conversion, AreaConverterTest)
{
    // Conversión cruzada: unidades SI → Imperial
    double _acres_from_hectares = AreaConverter::convert(hectares,
                                                         AreaUnit::hectare,
                                                         AreaUnit::acre);
    BOOST_CHECK_CLOSE(acres, _acres_from_hectares, 0.001);

    // Conversión cruzada: unidades Imperial → SI
    double _hectares_from_acres = AreaConverter::convert(acres,
                                                         AreaUnit::acre,
                                                         AreaUnit::hectare);
    BOOST_CHECK_CLOSE(hectares, _hectares_from_acres, 0.001);

    // Conversión entre imperiales
    double _square_yards_from_feet = AreaConverter::convert(square_feet,
                                                            AreaUnit::square_foot,
                                                            AreaUnit::square_yard);
    BOOST_CHECK_CLOSE(square_yards, _square_yards_from_feet, 0.001);
}

BOOST_FIXTURE_TEST_CASE(identity_conversion, AreaConverterTest)
{
    // La conversión de una unidad a sí misma debe devolver el mismo valor
    double _same_metre = AreaConverter::convert(square_metres,
        AreaUnit::square_metre,
        AreaUnit::square_metre);
    BOOST_CHECK_EQUAL(square_metres, _same_metre);

    double _same_acre = AreaConverter::convert(acres,
                                               AreaUnit::acre,
                                               AreaUnit::acre);
    BOOST_CHECK_EQUAL(acres, _same_acre);
}

BOOST_AUTO_TEST_SUITE_END()