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
 
#define BOOST_TEST_MODULE Tidop Validator test
#include <boost/test/unit_test.hpp>
#include <tidop/core/console/validator.h>

#include <array>

using namespace tl;


BOOST_AUTO_TEST_SUITE(ValidatorTestSuite)

struct ValidatorTest
{
    ValidatorTest() { }
    ~ValidatorTest() { }

    void setup()
    {
        range_validator = RangeValidator<int>::create(0, 100);
        value_validator = ValuesValidator<std::string>::create({"XML", "YML", "TXT", "BIN"});
    }

    void teardown() {}

    RangeValidator<double> def_range_validator;
    ValuesValidator<int> def_value_validator;

    std::shared_ptr<RangeValidator<int>> range_validator;
    std::shared_ptr<ValuesValidator<std::string>> value_validator;
};

BOOST_FIXTURE_TEST_CASE(default_constructors, ValidatorTest)
{
    BOOST_CHECK_EQUAL(std::numeric_limits<double>().max(), def_range_validator.max());
    BOOST_CHECK_EQUAL(std::numeric_limits<double>::lowest(), def_range_validator.min());

    BOOST_CHECK(!def_value_validator.validate(0));
    BOOST_CHECK(!def_value_validator.validate(10));
}

BOOST_FIXTURE_TEST_CASE(constructor, ValidatorTest)
{
    RangeValidator<int> validator1(5, 50);

    BOOST_CHECK_EQUAL(5, validator1.min());
    BOOST_CHECK_EQUAL(50, validator1.max());
    BOOST_CHECK(!validator1.validate(0));
    BOOST_CHECK(validator1.validate(5));
    BOOST_CHECK(validator1.validate(50));
    BOOST_CHECK(!validator1.validate(51));

    ValuesValidator<std::string> validator2({"XML", "YML", "TXT", "BIN"});
    BOOST_CHECK(validator2.validate("XML"));
    BOOST_CHECK(validator2.validate("YML"));
    BOOST_CHECK(validator2.validate("TXT"));
    BOOST_CHECK(validator2.validate("BIN"));
    BOOST_CHECK(!validator2.validate("CSV"));
}

BOOST_FIXTURE_TEST_CASE(create, ValidatorTest)
{
    BOOST_CHECK_EQUAL(100, range_validator->max());
    BOOST_CHECK_EQUAL(0, range_validator->min());
    BOOST_CHECK(range_validator->validate(0));
    BOOST_CHECK(range_validator->validate(50));
    BOOST_CHECK(range_validator->validate(100));
    BOOST_CHECK(!range_validator->validate(101));

    BOOST_CHECK(value_validator->validate("XML"));
    BOOST_CHECK(value_validator->validate("YML"));
    BOOST_CHECK(value_validator->validate("TXT"));
    BOOST_CHECK(value_validator->validate("BIN"));
    BOOST_CHECK(!value_validator->validate("CSV"));
}

BOOST_FIXTURE_TEST_CASE(set_values, ValidatorTest)
{
    value_validator->setValues({"JPG", "BMP", "PNG"});
    BOOST_CHECK(value_validator->validate("JPG"));
    BOOST_CHECK(value_validator->validate("BMP"));
    BOOST_CHECK(value_validator->validate("PNG"));
    BOOST_CHECK(!value_validator->validate("BIN"));
}

BOOST_FIXTURE_TEST_CASE(set_range, ValidatorTest)
{
    range_validator->setRange(50, 150);
    BOOST_CHECK_EQUAL(150, range_validator->max());
    BOOST_CHECK_EQUAL(50, range_validator->min());
    BOOST_CHECK(!range_validator->validate(0));
    BOOST_CHECK(range_validator->validate(50));
    BOOST_CHECK(range_validator->validate(100));
    BOOST_CHECK(!range_validator->validate(151));
}

BOOST_AUTO_TEST_SUITE_END()