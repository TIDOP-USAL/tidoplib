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
 
#define BOOST_TEST_MODULE Tidop Core base test
#include <boost/test/unit_test.hpp>
#include <tidop/core/base/Property.h>
#include <tidop/core/base/size.h>
#include <tidop/core/base/split.h>
#include <tidop/core/base/Meta.h>

#include <array>
#include <map>

using namespace tl;

/* property.h */

BOOST_AUTO_TEST_SUITE(PropertyTestSuite)


struct PropertyTest
{
    PropertyTest()
    {
    }

    ~PropertyTest()
    {
    }

    void setup()
    {
        property = new Property<int>(1);
        property_double = new Property<double>(2.0);
        property_float = new Property<float>(2.0f);
        property_string = new Property<std::string>("value");
        property_bool = new Property<bool>(true);
        property_int8 = new Property<int8_t>(-1);
        property_uint8 = new Property<uint8_t>(1);
        property_int16 = new Property<int16_t>(-1);
        property_uint16 = new Property<uint16_t>(1);
        property_int32 = new Property<int32_t>(-1);
        property_uint32 = new Property<uint32_t>(1);
        property_int64 = new Property<int64_t>(-1);
        property_uint64 = new Property<uint64_t>(1);
        property_size = new Property<Size<int>>({100,100});

        std::map<std::string, int> scores;
        scores["Alice"] = 100;
        scores["Bob"] = 85;
        property_map = new Property<std::map<std::string, int>>(scores);

    }

    void teardown() {}

    Property<int> *property;
    Property<double> *property_double;
    Property<float> *property_float;
    Property<std::string> *property_string;
    Property<bool> *property_bool;
    Property<int8_t> *property_int8;
    Property<uint8_t> *property_uint8;
    Property<int16_t> *property_int16;
    Property<uint16_t> *property_uint16;
    Property<int32_t> *property_int32;
    Property<uint32_t> *property_uint32;
    Property<int64_t> *property_int64;
    Property<uint64_t> *property_uint64;
    Property<Size<int>> *property_size;
    Property<std::map<std::string, int>> *property_map;

};

BOOST_FIXTURE_TEST_CASE(get_value, PropertyTest)
{
    BOOST_CHECK_EQUAL(1, property->value());
    BOOST_CHECK_EQUAL(2.0, property_double->value());
    BOOST_CHECK_EQUAL(2.0f, property_float->value());
    BOOST_CHECK_EQUAL("value", property_string->value());
    BOOST_CHECK_EQUAL(true, property_bool->value());
    BOOST_CHECK_EQUAL(-1, property_int8->value());
    BOOST_CHECK_EQUAL(1, property_uint8->value());
    BOOST_CHECK_EQUAL(-1, property_int16->value());
    BOOST_CHECK_EQUAL(1, property_uint16->value());
    BOOST_CHECK_EQUAL(-1, property_int32->value());
    BOOST_CHECK_EQUAL(1, property_uint32->value());
    BOOST_CHECK_EQUAL(-1, property_int64->value());
    BOOST_CHECK_EQUAL(1, property_uint64->value());

    auto size = property_size->value();
    BOOST_CHECK_EQUAL(100, size.width);
    BOOST_CHECK_EQUAL(100, size.height);

    auto map_property = property_map->value();
    auto it = map_property.find("Alice");
    BOOST_CHECK_EQUAL(100, it->second);
    it = map_property.find("Bob");
    BOOST_CHECK_EQUAL(85, it->second);
}

BOOST_FIXTURE_TEST_CASE(set_value, PropertyTest)
{
    property->setValue(2);
    BOOST_CHECK_EQUAL(2, property->value());
    property_double->setValue(3.0);
    BOOST_CHECK_EQUAL(3.0, property_double->value());
    property_float->setValue(3.0f);
    BOOST_CHECK_EQUAL(3.0f, property_float->value());
    property_string->setValue("value2");
    BOOST_CHECK_EQUAL("value2", property_string->value());
    property_bool->setValue(false);
    BOOST_CHECK_EQUAL(false, property_bool->value());
    property_int8->setValue(2);
    BOOST_CHECK_EQUAL(2, property_int8->value());
    property_uint8->setValue(2);
    BOOST_CHECK_EQUAL(2, property_uint8->value());
    property_int16->setValue(2);
    BOOST_CHECK_EQUAL(2, property_int16->value());
    property_uint16->setValue(2);
    BOOST_CHECK_EQUAL(2, property_uint16->value());
    property_int32->setValue(2);
    BOOST_CHECK_EQUAL(2, property_int32->value());
    property_uint32->setValue(2);
    BOOST_CHECK_EQUAL(2, property_uint32->value());
    property_int64->setValue(2);
    BOOST_CHECK_EQUAL(2, property_int64->value());
    property_uint64->setValue(2);
    BOOST_CHECK_EQUAL(2, property_uint64->value());
    property_size->setValue({250, 150});
    BOOST_CHECK_EQUAL(250, property_size->value().width);
    BOOST_CHECK_EQUAL(150, property_size->value().height);

    auto map_property = property_map->value();

    map_property["Rob"] = 65;
    property_map->setValue(map_property);

    auto map_property2 = property_map->value();
    auto it = map_property2.find("Rob");
    BOOST_CHECK_EQUAL(65, it->second);
}

BOOST_FIXTURE_TEST_CASE(to_string, PropertyTest)
{
    BOOST_CHECK_EQUAL("1", property->toString());
    BOOST_CHECK_EQUAL("2.000000", property_double->toString());
    BOOST_CHECK_EQUAL("2.000000", property_float->toString());
    BOOST_CHECK_EQUAL("value", property_string->toString());
    BOOST_CHECK_EQUAL("true", property_bool->toString());
    BOOST_CHECK_EQUAL("-1", property_int8->toString());
    BOOST_CHECK_EQUAL("1", property_uint8->toString());
    BOOST_CHECK_EQUAL("-1", property_int16->toString());
    BOOST_CHECK_EQUAL("1", property_uint16->toString());
    BOOST_CHECK_EQUAL("-1", property_int32->toString());
    BOOST_CHECK_EQUAL("1", property_uint32->toString());
    BOOST_CHECK_EQUAL("-1", property_int64->toString());
    BOOST_CHECK_EQUAL("1", property_uint64->toString());
    BOOST_CHECK_EQUAL("100x100", property_size->toString());
    BOOST_CHECK_EQUAL("{\"Alice\":100,\"Bob\":85}", property_map->toString());
}

BOOST_FIXTURE_TEST_CASE(from_string, PropertyTest)
{
    property->fromString("2");
    BOOST_CHECK_EQUAL(2, property->value());
    property_double->fromString("3.0");
    BOOST_CHECK_EQUAL(3.0, property_double->value());
    property_float->fromString("3.0f");
    BOOST_CHECK_EQUAL(3.0f, property_float->value());
    property_string->fromString("value2");
    BOOST_CHECK_EQUAL("value2", property_string->value());
    property_bool->fromString("false");
    BOOST_CHECK_EQUAL(false, property_bool->value());
    property_int8->fromString("2");
    BOOST_CHECK_EQUAL(2, property_int8->value());
    property_uint8->fromString("2");
    BOOST_CHECK_EQUAL(2, property_uint8->value());
    property_int16->fromString("2");
    BOOST_CHECK_EQUAL(2, property_int16->value());
    property_uint16->fromString("2");
    BOOST_CHECK_EQUAL(2, property_uint16->value());
    property_int32->fromString("2");
    BOOST_CHECK_EQUAL(2, property_int32->value());
    property_uint32->fromString("2");
    BOOST_CHECK_EQUAL(2, property_uint32->value());
    property_int64->fromString("2");
    BOOST_CHECK_EQUAL(2, property_int64->value());
    property_uint64->fromString("2");
    BOOST_CHECK_EQUAL(2, property_uint64->value());
    property_size->fromString("150x150");
    BOOST_CHECK_EQUAL(150, property_size->value().width);
    BOOST_CHECK_EQUAL(150, property_size->value().height);
    property_map->fromString("{\"Alice\":100,\"Bob\":85,\"Rob\":65}");

    auto map_property = property_map->value();
    auto it = map_property.find("Rob");
    BOOST_CHECK_EQUAL(65, it->second);
}

BOOST_FIXTURE_TEST_CASE(value_out_of_range, PropertyTest)
{
    property_float->fromString("3.402823466e+38");
    BOOST_CHECK_EQUAL(3.402823466e+38f, property_float->value());
    BOOST_CHECK_THROW(property_float->fromString("3.402823466e+39"), Exception);
    BOOST_CHECK_THROW(property_float->fromString("-3.402823466e+39"), Exception);

    property_double->fromString("1.7976931348623157e+308");
    BOOST_CHECK_EQUAL(1.7976931348623157e+308, property_double->value());
    BOOST_CHECK_THROW(property_double->fromString("1.7976931348623157e+309"), Exception);
    BOOST_CHECK_THROW(property_double->fromString("-1.7976931348623157e+309"), Exception);

    property_int8->fromString("127");
    BOOST_CHECK_EQUAL(127, property_int8->value());
    constexpr auto min_int8_t = std::numeric_limits<int8_t>::min();
    constexpr auto max_int8_t = std::numeric_limits<int8_t>::max();
    BOOST_CHECK_THROW(property_int8->fromString(std::to_string(max_int8_t +1)), Exception);
    BOOST_CHECK_THROW(property_int8->fromString(std::to_string(min_int8_t -1)), Exception);

    property_uint8->fromString("255");
    BOOST_CHECK_EQUAL(255, property_uint8->value());
    constexpr auto min_uint8_t = std::numeric_limits<uint8_t>::min();
    constexpr auto max_uint8_t = std::numeric_limits<uint8_t>::max();
    BOOST_CHECK_THROW(property_uint8->fromString(std::to_string(max_uint8_t +1)), Exception);
    BOOST_CHECK_THROW(property_uint8->fromString(std::to_string(min_uint8_t -1)), Exception);

    property_int16->fromString("32767");
    BOOST_CHECK_EQUAL(32767, property_int16->value());
    constexpr auto min_int16 = std::numeric_limits<int16_t>::min();
    constexpr auto max_int16 = std::numeric_limits<int16_t>::max();
    BOOST_CHECK_THROW(property_int16->fromString(std::to_string(max_int16 + 1)), Exception);
    BOOST_CHECK_THROW(property_int16->fromString(std::to_string(min_int16 - 1)), Exception);

    property_uint16->fromString("65535");
    BOOST_CHECK_EQUAL(65535, property_uint16->value());
    constexpr auto min_uint16 = std::numeric_limits<uint16_t>::min();
    constexpr auto max_uint16 = std::numeric_limits<uint16_t>::max();
    BOOST_CHECK_THROW(property_uint16->fromString(std::to_string(max_uint16 + 1)), Exception);
    BOOST_CHECK_THROW(property_uint16->fromString(std::to_string(min_uint16 - 1)), Exception);

    property_int32->fromString("2147483647");
    BOOST_CHECK_EQUAL(2147483647, property_int32->value());
    constexpr int64_t min_int32 = std::numeric_limits<int32_t>::min();
    constexpr int64_t max_int32 = std::numeric_limits<int32_t>::max();
    BOOST_CHECK_THROW(property_int32->fromString(std::to_string(max_int32 + 1)), Exception);
    BOOST_CHECK_THROW(property_int32->fromString(std::to_string(min_int32 - 1)), Exception);

    property_uint32->fromString("4294967295");
    BOOST_CHECK_EQUAL(4294967295, property_uint32->value());
    constexpr uint64_t min_uint32 = std::numeric_limits<uint32_t>::min();
    constexpr uint64_t max_uint32 = std::numeric_limits<uint32_t>::max();
    BOOST_CHECK_THROW(property_uint32->fromString(std::to_string(max_uint32 + 1)), Exception);
    BOOST_CHECK_THROW(property_uint32->fromString(std::to_string(min_uint32 - 1)), Exception);

    property_int64->fromString("9223372036854775807");
    BOOST_CHECK_EQUAL(9223372036854775807, property_int64->value());
    BOOST_CHECK_THROW(property_int64->fromString("9223372036854775808"), Exception);
    BOOST_CHECK_THROW(property_int64->fromString("-9223372036854775809"), Exception);

    property_uint64->fromString("18446744073709551615");
    BOOST_CHECK_EQUAL(18446744073709551615, property_uint64->value());
    BOOST_CHECK_THROW(property_uint64->fromString("18446744073709551616"), Exception);
    BOOST_CHECK_THROW(property_uint64->fromString("-1"), Exception);

}

BOOST_FIXTURE_TEST_CASE(invalid_argument, PropertyTest)
{
    BOOST_CHECK_THROW(property->fromString("hi"), Exception);
    BOOST_CHECK_THROW(property_double->fromString("hi"), Exception);
    BOOST_CHECK_THROW(property_float->fromString("hi"), Exception);
    BOOST_CHECK_THROW(property_bool->fromString("hi"), Exception);
    BOOST_CHECK_THROW(property_int8->fromString("hi"), Exception);
    BOOST_CHECK_THROW(property_uint8->fromString("hi"), Exception);
    BOOST_CHECK_THROW(property_int16->fromString("hi"), Exception);
    BOOST_CHECK_THROW(property_uint16->fromString("hi"), Exception);
    BOOST_CHECK_THROW(property_int32->fromString("hi"), Exception);
    BOOST_CHECK_THROW(property_uint32->fromString("hi"), Exception);
    BOOST_CHECK_THROW(property_int64->fromString("hi"), Exception);
    BOOST_CHECK_THROW(property_uint64->fromString("hi"), Exception);
    BOOST_CHECK_THROW(property_size->fromString("hi"), Exception);
    BOOST_CHECK_THROW(property_map->fromString("hi"), Exception);
}

BOOST_FIXTURE_TEST_CASE(type_name, PropertyTest)
{
    BOOST_CHECK_EQUAL("int", property->typeName());
    BOOST_CHECK_EQUAL("double", property_double->typeName());
    BOOST_CHECK_EQUAL("float", property_float->typeName());
    BOOST_CHECK_EQUAL("std::string", property_string->typeName());
    BOOST_CHECK_EQUAL("bool", property_bool->typeName());
    BOOST_CHECK_EQUAL("int8", property_int8->typeName());
    BOOST_CHECK_EQUAL("uint8", property_uint8->typeName());
    BOOST_CHECK_EQUAL("int16", property_int16->typeName());
    BOOST_CHECK_EQUAL("uint16", property_uint16->typeName());
    BOOST_CHECK_EQUAL("int", property_int32->typeName());
    BOOST_CHECK_EQUAL("uint", property_uint32->typeName());
    BOOST_CHECK_EQUAL("int64", property_int64->typeName());
    BOOST_CHECK_EQUAL("uint64", property_uint64->typeName());
    BOOST_CHECK_EQUAL("tl::Size", property_size->typeName());
}

BOOST_FIXTURE_TEST_CASE(type, PropertyTest)
{
    BOOST_CHECK(Type::type_int == property->type());
    BOOST_CHECK(Type::type_double == property_double->type());
    BOOST_CHECK(Type::type_float == property_float->type());
    BOOST_CHECK(Type::type_string == property_string->type());
    BOOST_CHECK(Type::type_bool == property_bool->type());
    BOOST_CHECK(Type::type_int8 == property_int8->type());
    BOOST_CHECK(Type::type_uchar == property_uint8->type());
    BOOST_CHECK(Type::type_int16 == property_int16->type());
    BOOST_CHECK(Type::type_uint16 == property_uint16->type());
    BOOST_CHECK(Type::type_int32 == property_int32->type());
    BOOST_CHECK(Type::type_uint32 == property_uint32->type());
    BOOST_CHECK(Type::type_int64 == property_int64->type());
    BOOST_CHECK(Type::type_uint64 == property_uint64->type());
    BOOST_CHECK(Type::type_size == property_size->type());
    BOOST_CHECK(Type::type_map == property_map->type());
}


BOOST_AUTO_TEST_SUITE_END()




BOOST_AUTO_TEST_SUITE(PropertiesTestSuite)

struct PropertiesTest
{

    PropertiesTest()
    {
    }

    ~PropertiesTest()
    {
    }

    void setup()
    {
        properties.setProperty("key1", 1);
        properties.setProperty("key2", 2.0);
        properties.setProperty("key3", "value");
        properties.setProperty("key4", true);
    }

    void teardown() {}

    Properties properties;
};

BOOST_FIXTURE_TEST_CASE(set_get_property, PropertiesTest)
{
    BOOST_CHECK_EQUAL(1, properties.getProperty<int>("key1"));
    BOOST_CHECK_EQUAL(2.0, properties.getProperty<double>("key2"));
    BOOST_CHECK_EQUAL("value", properties.getProperty<std::string>("key3"));
    BOOST_CHECK_EQUAL(true, properties.getProperty<bool>("key4"));
}

BOOST_FIXTURE_TEST_CASE(has_property, PropertiesTest)
{
    BOOST_CHECK(properties.hasProperty("key1"));
}

BOOST_FIXTURE_TEST_CASE(has_property_error, PropertiesTest)
{
    BOOST_CHECK(!properties.hasProperty("key7"));
}

BOOST_FIXTURE_TEST_CASE(get_property_as_string, PropertiesTest)
{
    BOOST_CHECK_EQUAL("1", properties.getPropertyAsString("key1"));
    // std::to_string defaults to 6 decimal
    BOOST_CHECK_EQUAL("2.000000", properties.getPropertyAsString("key2"));
    BOOST_CHECK_EQUAL("value", properties.getPropertyAsString("key3"));
}

BOOST_FIXTURE_TEST_CASE(get_property_as_string_error, PropertiesTest)
{
    BOOST_CHECK_THROW(properties.getPropertyAsString("key7"), std::out_of_range);
}

BOOST_FIXTURE_TEST_CASE(get_property_type, PropertiesTest)
{
    BOOST_CHECK(Type::type_int == properties.getPropertyType("key1"));
    BOOST_CHECK(Type::type_double == properties.getPropertyType("key2"));
    BOOST_CHECK(Type::type_string == properties.getPropertyType("key3"));
    BOOST_CHECK(Type::type_bool == properties.getPropertyType("key4"));
}

BOOST_FIXTURE_TEST_CASE(get_property_type_error, PropertiesTest)
{
    BOOST_CHECK_THROW(properties.getPropertyType("key7"), std::out_of_range);
}

BOOST_FIXTURE_TEST_CASE(iteratorValueType, PropertiesTest)
{
    auto it = properties.begin();
    BOOST_CHECK_EQUAL("key1", it->first);
    BOOST_CHECK_EQUAL("1", it->second->toString());
    ++it;
    BOOST_CHECK_EQUAL("key2", it->first);
    BOOST_CHECK_EQUAL("2.000000", it->second->toString());
    ++it;
    BOOST_CHECK_EQUAL("key3", it->first);
    BOOST_CHECK_EQUAL("value", it->second->toString());
    ++it;
    BOOST_CHECK_EQUAL("key4", it->first);
    BOOST_CHECK_EQUAL("true", it->second->toString());

    ++it;
    BOOST_CHECK(it == properties.end());
}


BOOST_AUTO_TEST_SUITE_END()


/* meta.h */

BOOST_AUTO_TEST_CASE(_args_size)
{
    BOOST_CHECK_EQUAL(4, tl::argsSize(1, 2, 3, 4));

    std::string arg1("1");
    int arg2 = 2;
    BOOST_CHECK_EQUAL(2, tl::argsSize(arg1, arg2));
}

BOOST_AUTO_TEST_CASE(_args_empty)
{
    BOOST_CHECK(tl::argsEmpty());
    BOOST_CHECK(!tl::argsEmpty(1, 3));
}

BOOST_AUTO_TEST_CASE(is_in_range)
{
    BOOST_CHECK(isInRange(1, 0, 2));
    BOOST_CHECK(isInRange(0.5, -1., 2.));
    BOOST_CHECK(!isInRange(-0.5, 0., 2.));
}

/* format.h */

BOOST_AUTO_TEST_CASE(_format_)
{
    BOOST_CHECK_EQUAL("Hello world", format("Hello {}", "world"));
    BOOST_CHECK_EQUAL("Hello 1", format("Hello {}", 1));
    BOOST_CHECK_EQUAL("Hello 1.1", format("Hello {}", 1.1));
}

/* size.h */

BOOST_AUTO_TEST_SUITE(SizeTestSuite)

struct SizeTest
{

    SizeTest()
        : size_integer(nullptr),
        size_double(nullptr),
        size_float(nullptr),
        size_integer_copy(nullptr),
        size_double_copy(nullptr),
        size_float_copy(nullptr),
        size_constructor_integer_width_height(nullptr),
        size_constructor_double_width_height(nullptr),
        size_constructor_float_width_height(nullptr)
    {

    }

    ~SizeTest()
    {
        delete size_integer;
        delete size_double;
        delete size_float;
        delete size_integer_copy;
        delete size_double_copy;
        delete size_float_copy;
        delete size_constructor_integer_width_height;
        delete size_constructor_double_width_height;
        delete size_constructor_float_width_height;
    }

    void setup()
    {
        size_integer = new Size<int>(100, 100);
        size_double = new Size<double>(100.4, 100.2);
        size_float = new Size<float>(100.4f, 100.2f);

        size_integer_copy = new Size<int>(*size_integer);
        size_double_copy = new Size<double>(*size_double);
        size_float_copy = new Size<float>(*size_float);

        size_constructor_integer_width_height = new Size<int>(100, 100);
        size_constructor_double_width_height = new Size<double>(100.4, 100.2);
        size_constructor_float_width_height = new Size<float>(100.4f, 100.2f);

    }

    void teardown()
    {

    }

    Size<int> size_default_constructor_integer;
    Size<double> size_default_constructor_double;
    Size<float> size_default_constructor_float;

    Size<int> *size_integer;
    Size<double> *size_double;
    Size<float> *size_float;
    Size<int> *size_integer_copy;
    Size<double> *size_double_copy;
    Size<float> *size_float_copy;

    Size<int> *size_constructor_integer_width_height;
    Size<double> *size_constructor_double_width_height;
    Size<float> *size_constructor_float_width_height;
};


BOOST_FIXTURE_TEST_CASE(default_constructor, SizeTest)
{

    BOOST_CHECK_EQUAL(0, size_default_constructor_integer.width);
    BOOST_CHECK_EQUAL(0, size_default_constructor_integer.height);


    BOOST_CHECK_EQUAL(0., size_default_constructor_double.width);
    BOOST_CHECK_EQUAL(0., size_default_constructor_double.height);

    BOOST_CHECK_EQUAL(0.f, size_default_constructor_float.width);
    BOOST_CHECK_EQUAL(0.f, size_default_constructor_float.height);
}

BOOST_FIXTURE_TEST_CASE(copy_constructor, SizeTest)
{
    BOOST_CHECK_EQUAL(size_integer->width, size_integer_copy->width);
    BOOST_CHECK_EQUAL(size_integer->height, size_integer_copy->height);

    BOOST_CHECK_EQUAL(size_double->width, size_double_copy->width);
    BOOST_CHECK_EQUAL(size_double->height, size_double_copy->height);

    BOOST_CHECK_EQUAL(size_float->width, size_float_copy->width);
    BOOST_CHECK_EQUAL(size_float->height, size_float_copy->height);
}

BOOST_FIXTURE_TEST_CASE(move_constructor, SizeTest)
{
    Size<int> size(100, 100);
    Size<int> size2(std::move(size));
    BOOST_CHECK_EQUAL(100, size2.width);
    BOOST_CHECK_EQUAL(100, size2.height);
}

BOOST_FIXTURE_TEST_CASE(copy_assignment, SizeTest)
{
    Size<int> size(100, 100);
    Size<int> size2(50, 50);
    size2 = size;
    BOOST_CHECK_EQUAL(100, size2.width);
    BOOST_CHECK_EQUAL(100, size2.height);
}

BOOST_FIXTURE_TEST_CASE(move_assignment, SizeTest)
{
    Size<int> size2;
    size2 = Size<int>(100, 100);
    BOOST_CHECK_EQUAL(100, size2.width);
    BOOST_CHECK_EQUAL(100, size2.height);
}

BOOST_FIXTURE_TEST_CASE(constructor_width_height, SizeTest)
{
    BOOST_CHECK_EQUAL(100, size_constructor_integer_width_height->width);
    BOOST_CHECK_EQUAL(100, size_constructor_integer_width_height->height);

    BOOST_CHECK_EQUAL(100.4, size_constructor_double_width_height->width);
    BOOST_CHECK_EQUAL(100.2, size_constructor_double_width_height->height);

    BOOST_CHECK_EQUAL(100.4f, size_constructor_float_width_height->width);
    BOOST_CHECK_EQUAL(100.2f, size_constructor_float_width_height->height);
}

BOOST_FIXTURE_TEST_CASE(isEmpty, SizeTest)
{
    BOOST_CHECK_EQUAL(true, size_default_constructor_integer.isEmpty());
    BOOST_CHECK_EQUAL(true, size_default_constructor_double.isEmpty());
    BOOST_CHECK_EQUAL(true, size_default_constructor_float.isEmpty());
    BOOST_CHECK_EQUAL(false, size_integer->isEmpty());
    BOOST_CHECK_EQUAL(false, size_double->isEmpty());
    BOOST_CHECK_EQUAL(false, size_double->isEmpty());
}

BOOST_FIXTURE_TEST_CASE(cast, SizeTest)
{
    Size<int> size = static_cast<Size<int>>(*size_double);
    BOOST_CHECK_EQUAL(100, size.width);
    BOOST_CHECK_EQUAL(100, size.height);

    Size<double> size2 = static_cast<Size<double>>(*size_integer);
    BOOST_CHECK_EQUAL(100., size2.width);
    BOOST_CHECK_EQUAL(100., size2.height);
}

BOOST_FIXTURE_TEST_CASE(is_equal, SizeTest)
{
    Size<int> size = *size_integer;
    BOOST_CHECK(size == *size_integer);
}

BOOST_FIXTURE_TEST_CASE(is_not_equal, SizeTest)
{
    Size<int> size(50, 50);
    BOOST_CHECK(size != *size_integer);
}

BOOST_FIXTURE_TEST_CASE(add1, SizeTest)
{
    Size<int> size1(50, 50);
    Size<int> size2(20, 30);
    Size<int> size3 = size1 + size2;
    BOOST_CHECK_EQUAL(70, size3.width);
    BOOST_CHECK_EQUAL(80, size3.height);
}

BOOST_FIXTURE_TEST_CASE(add2, SizeTest)
{
    Size<int> size1(50, 50);
    Size<int> size2(20, 30);
    size2 += size1;
    BOOST_CHECK_EQUAL(70, size2.width);
    BOOST_CHECK_EQUAL(80, size2.height);
}

BOOST_FIXTURE_TEST_CASE(minus1, SizeTest)
{
    Size<int> size1(50, 50);
    Size<int> size2(20, 30);
    Size<int> size3 = size1 - size2;
    BOOST_CHECK_EQUAL(30, size3.width);
    BOOST_CHECK_EQUAL(20, size3.height);
}

BOOST_FIXTURE_TEST_CASE(minus2, SizeTest)
{
    Size<int> size1(50, 50);
    Size<int> size2(20, 30);
    size2 -= size1;
    BOOST_CHECK_EQUAL(-30, size2.width);
    BOOST_CHECK_EQUAL(-20, size2.height);
}

BOOST_FIXTURE_TEST_CASE(mul1, SizeTest)
{
    Size<int> size1(14, 23);
    Size<int> size2 = size1 * 10;
    BOOST_CHECK_EQUAL(140, size2.width);
    BOOST_CHECK_EQUAL(230, size2.height);
}

BOOST_FIXTURE_TEST_CASE(mul2, SizeTest)
{
    Size<int> size1(14, 23);
    size1 *= 10;
    BOOST_CHECK_EQUAL(140, size1.width);
    BOOST_CHECK_EQUAL(230, size1.height);
}

BOOST_FIXTURE_TEST_CASE(div1, SizeTest)
{
    Size<double> size1(14., 23.);
    Size<double> size2 = size1 / 10.;
    BOOST_CHECK_EQUAL(1.4, size2.width);
    BOOST_CHECK_EQUAL(2.3, size2.height);
}

BOOST_FIXTURE_TEST_CASE(div2, SizeTest)
{
    Size<double> size1(14., 23.);
    size1 /= 10.;
    BOOST_CHECK_EQUAL(1.4, size1.width);
    BOOST_CHECK_EQUAL(2.3, size1.height);
}

BOOST_AUTO_TEST_SUITE_END()


/* split.h */ 

BOOST_AUTO_TEST_CASE(split_string)
{
    std::vector<std::string> v = split<std::string>("");
    BOOST_CHECK_EQUAL(0, v.size());
}


BOOST_AUTO_TEST_CASE(split_default_separator)
{
    std::vector<std::string> out = split<std::string>("cad1,cad2,cad3");
    BOOST_CHECK_EQUAL(3, out.size());
    BOOST_CHECK_EQUAL("cad1", out[0]);
    BOOST_CHECK_EQUAL("cad2", out[1]);
    BOOST_CHECK_EQUAL("cad3", out[2]);
}

BOOST_AUTO_TEST_CASE(split_separator)
{
    std::vector<std::string> out = split<std::string>("cad1#cad2#cad3", '#');
    BOOST_CHECK_EQUAL(3, out.size());
    BOOST_CHECK_EQUAL("cad1", out[0]);
    BOOST_CHECK_EQUAL("cad2", out[1]);
    BOOST_CHECK_EQUAL("cad3", out[2]);

    std::vector<int> out2 = split<int>("1,2,3", ',');
    BOOST_CHECK_EQUAL(3, out2.size());
    BOOST_CHECK_EQUAL(1, out2[0]);
    BOOST_CHECK_EQUAL(2, out2[1]);
    BOOST_CHECK_EQUAL(3, out2[2]);

    std::vector<double> out3 = split<double>("1.2,2.6,3.4", ',');
    BOOST_CHECK_EQUAL(3, out3.size());
    BOOST_CHECK_EQUAL(1.2, out3[0]);
    BOOST_CHECK_EQUAL(2.6, out3[1]);
    BOOST_CHECK_EQUAL(3.4, out3[2]);
}


/* string_utils.h */

BOOST_AUTO_TEST_CASE(compare_insensitive_case)
{
    BOOST_CHECK_EQUAL(true, compareInsensitiveCase("AbcDEfgHI", "AbcDEfgHI"));
    BOOST_CHECK_EQUAL(true, compareInsensitiveCase("abcdefghi", "AbcDEfgHI"));
    BOOST_CHECK_EQUAL(true, compareInsensitiveCase("AbcDEfgHI", "ABCDEFGHI"));
    BOOST_CHECK_EQUAL(false, compareInsensitiveCase("AbcDEfgHI", "aaaaaa"));
}

BOOST_AUTO_TEST_CASE(test_ltrim)
{
    std::string s = "  Hello world";
    ltrim(s);
    BOOST_CHECK_EQUAL("Hello world", s);
}

BOOST_AUTO_TEST_CASE(test_rtrim)
{
    std::string s = "Hello world  ";
    rtrim(s);
    BOOST_CHECK_EQUAL("Hello world", s);
}

BOOST_AUTO_TEST_CASE(test_trim)
{
    std::string s = "  Hello world  ";
    trim(s);
    BOOST_CHECK_EQUAL("Hello world", s);
}

BOOST_AUTO_TEST_CASE(test_ltrim_copy)
{
    std::string s = "  Hello world";
    std::string out = ltrim_copy(s);
    BOOST_CHECK_EQUAL("Hello world", out);
}

BOOST_AUTO_TEST_CASE(test_rtrim_copy)
{
    std::string s = "Hello world  ";
    std::string out = rtrim_copy(s);
    BOOST_CHECK_EQUAL("Hello world", out);
}

BOOST_AUTO_TEST_CASE(test_trim_copy)
{
    std::string s = "  Hello world  ";
    std::string out = trim_copy(s);
    BOOST_CHECK_EQUAL("Hello world", out);
}

BOOST_AUTO_TEST_CASE(test_replace_string)
{
    std::string str = "Hello world";
    replaceString(&str, " ", "_");
    BOOST_CHECK_EQUAL("Hello_world", str);
    replaceString(&str, "_", " ");
    BOOST_CHECK_EQUAL("Hello world", str);
}



/* TypeConversions.h */

BOOST_AUTO_TEST_CASE(test_number_cast)
{
    BOOST_CHECK_EQUAL(1, numberCast<int>(1.0));
    BOOST_CHECK_EQUAL(1.0, numberCast<double>(1));
    BOOST_CHECK_EQUAL(1.0f, numberCast<float>(1));
}

BOOST_AUTO_TEST_CASE(test_convert_string_to)
{
    int out = convertStringTo<int>("123");
    BOOST_CHECK_EQUAL(123, out);

    double out2 = convertStringTo<double>("152.24");
    BOOST_CHECK_EQUAL(152.24, out2);

    bool out3 = convertStringTo<bool>("true");
    BOOST_CHECK_EQUAL(true, out3);

    bool out4 = convertStringTo<bool>("false");
    BOOST_CHECK_EQUAL(false, out4);

    bool out5 = convertStringTo<bool>("1");
    BOOST_CHECK_EQUAL(true, out5);

    bool out6 = convertStringTo<bool>("0");
    BOOST_CHECK_EQUAL(false, out6);

    BOOST_CHECK_EQUAL(convertStringTo<int>("123"), 123);
    BOOST_CHECK_THROW(convertStringTo<int>("999999999999"), Exception);

    BOOST_CHECK_EQUAL(convertStringTo<float>("3.14"), 3.14f);
    BOOST_CHECK_THROW(convertStringTo<float>("1e50"), Exception);

    BOOST_CHECK_EQUAL(convertStringTo<int8_t>("127"), 127);
    BOOST_CHECK_THROW(convertStringTo<int8_t>("128"), Exception);

    BOOST_CHECK_EQUAL(convertStringTo<uint8_t>("255"), 255);
    BOOST_CHECK_THROW(convertStringTo<uint8_t>("256"), Exception);

    BOOST_CHECK_EQUAL(convertStringTo<bool>("true"), true);
    BOOST_CHECK_EQUAL(convertStringTo<bool>("false"), false);
    BOOST_CHECK_THROW(convertStringTo<bool>("maybe"), Exception);
}

BOOST_AUTO_TEST_CASE(test_to_string_with_precision)
{
    BOOST_CHECK_EQUAL("1.23", toStringWithPrecision(1.23456789, 2));
    BOOST_CHECK_EQUAL("1.234568", toStringWithPrecision(1.23456789, 6));
    BOOST_CHECK_EQUAL("1.234567890", toStringWithPrecision(1.23456789, 9));
}


/* Type.h */

BOOST_AUTO_TEST_CASE(get_type_id)
{
    BOOST_CHECK(Type::type_bool == getTypeId<bool>());
    BOOST_CHECK(Type::type_int8 == getTypeId<signed char>());
    BOOST_CHECK(Type::type_uint8 == getTypeId<unsigned char>());
    BOOST_CHECK(Type::type_int16 == getTypeId<short>());
    BOOST_CHECK(Type::type_uint16 == getTypeId<unsigned short>());
    BOOST_CHECK(Type::type_int32 == getTypeId<int>());
    BOOST_CHECK(Type::type_uint32 == getTypeId<unsigned int>());
    BOOST_CHECK(Type::type_int64 == getTypeId<long long>());
    BOOST_CHECK(Type::type_uint64 == getTypeId<unsigned long long>());
    BOOST_CHECK(Type::type_float32 == getTypeId<float>());
    BOOST_CHECK(Type::type_float64 == getTypeId<double>());
    BOOST_CHECK(Type::type_string == getTypeId<std::string>());
    BOOST_CHECK(Type::type_wstring == getTypeId<std::wstring>());
    BOOST_CHECK(Type::type_path == getTypeId<tl::Path>());
    BOOST_CHECK(Type::type_size == getTypeId<Size<int>>());
    auto type = getTypeId<std::map<int, std::string>>();
    BOOST_CHECK(Type::type_map == type);
    BOOST_CHECK(Type::type_schar == getTypeId<signed char>());
    BOOST_CHECK(Type::type_uchar == getTypeId<unsigned char>());
    BOOST_CHECK(Type::type_short == getTypeId<short>());
    BOOST_CHECK(Type::type_ushort == getTypeId<unsigned short>());
    BOOST_CHECK(Type::type_int == getTypeId<int>());
    BOOST_CHECK(Type::type_uint == getTypeId<unsigned int>());
    BOOST_CHECK(Type::type_float == getTypeId<float>());
    BOOST_CHECK(Type::type_double == getTypeId<double>());
}

BOOST_AUTO_TEST_CASE(get_type_name)
{
    BOOST_CHECK_EQUAL("bool", getTypeName<bool>());
    BOOST_CHECK_EQUAL("int8", getTypeName<signed char>());
    BOOST_CHECK_EQUAL("uint8", getTypeName<unsigned char>());
    BOOST_CHECK_EQUAL("int16", getTypeName<short>());
    BOOST_CHECK_EQUAL("uint16", getTypeName<unsigned short>());
    BOOST_CHECK_EQUAL("int", getTypeName<int>());
    BOOST_CHECK_EQUAL("uint", getTypeName<unsigned int>());
    BOOST_CHECK_EQUAL("int64", getTypeName<long long>());
    BOOST_CHECK_EQUAL("uint64", getTypeName<unsigned long long>());
    BOOST_CHECK_EQUAL("float", getTypeName<float>());
    BOOST_CHECK_EQUAL("double", getTypeName<double>());
    BOOST_CHECK_EQUAL("std::string", getTypeName<std::string>());
    BOOST_CHECK_EQUAL("std::wstring", getTypeName<std::wstring>());
    BOOST_CHECK_EQUAL("tl::Path", getTypeName<tl::Path>());
    BOOST_CHECK_EQUAL("tl::Size", getTypeName<Size<int>>());
    auto name = getTypeName<std::map<int, std::string>>();
    BOOST_CHECK_EQUAL("std::map", name);
    BOOST_CHECK_EQUAL("int8", getTypeName<signed char>());
    BOOST_CHECK_EQUAL("uint8", getTypeName<unsigned char>());
    BOOST_CHECK_EQUAL("int16", getTypeName<short>());
    BOOST_CHECK_EQUAL("uint16", getTypeName<unsigned short>());
    BOOST_CHECK_EQUAL("int", getTypeName<int>());
    BOOST_CHECK_EQUAL("uint", getTypeName<unsigned int>());
    BOOST_CHECK_EQUAL("float", getTypeName<float>());
    BOOST_CHECK_EQUAL("double", getTypeName<double>());
}

BOOST_AUTO_TEST_CASE(type_to_string)
{
    BOOST_CHECK_EQUAL("bool", typeToString(Type::type_bool));
    BOOST_CHECK_EQUAL("int8", typeToString(Type::type_int8));
    BOOST_CHECK_EQUAL("uint8", typeToString(Type::type_uint8));
    BOOST_CHECK_EQUAL("int16", typeToString(Type::type_int16));
    BOOST_CHECK_EQUAL("uint16", typeToString(Type::type_uint16));
    BOOST_CHECK_EQUAL("int", typeToString(Type::type_int));
    BOOST_CHECK_EQUAL("uint", typeToString(Type::type_uint));
    BOOST_CHECK_EQUAL("int64", typeToString(Type::type_int64));
    BOOST_CHECK_EQUAL("uint64", typeToString(Type::type_uint64));
    BOOST_CHECK_EQUAL("float", typeToString(Type::type_float32));
    BOOST_CHECK_EQUAL("double", typeToString(Type::type_float64));
    BOOST_CHECK_EQUAL("std::string", typeToString(Type::type_string));
    BOOST_CHECK_EQUAL("std::wstring", typeToString(Type::type_wstring));
    BOOST_CHECK_EQUAL("tl::Path", typeToString(Type::type_path));
    BOOST_CHECK_EQUAL("tl::Size", typeToString(Type::type_size));
    BOOST_CHECK_EQUAL("std::map", typeToString(Type::type_map));
    BOOST_CHECK_EQUAL("int8", typeToString(Type::type_schar));
    BOOST_CHECK_EQUAL("uint8", typeToString(Type::type_uchar));
    BOOST_CHECK_EQUAL("int16", typeToString(Type::type_short));
    BOOST_CHECK_EQUAL("uint16", typeToString(Type::type_ushort));
    BOOST_CHECK_EQUAL("int", typeToString(Type::type_int));
    BOOST_CHECK_EQUAL("uint", typeToString(Type::type_uint));
    BOOST_CHECK_EQUAL("float", typeToString(Type::type_float));
    BOOST_CHECK_EQUAL("double", typeToString(Type::type_double));
}

BOOST_AUTO_TEST_CASE(string_to_type)
{
    BOOST_CHECK(Type::type_bool == stringToType("bool"));
    BOOST_CHECK(Type::type_int8 == stringToType("int8"));
    BOOST_CHECK(Type::type_uint8 == stringToType("uint8"));
    BOOST_CHECK(Type::type_int16 == stringToType("int16"));
    BOOST_CHECK(Type::type_uint16 == stringToType("uint16"));
    BOOST_CHECK(Type::type_int == stringToType("int"));
    BOOST_CHECK(Type::type_uint == stringToType("uint"));
    BOOST_CHECK(Type::type_int64 == stringToType("int64"));
    BOOST_CHECK(Type::type_uint64 == stringToType("uint64"));
    BOOST_CHECK(Type::type_float == stringToType("float"));
    BOOST_CHECK(Type::type_double == stringToType("double"));
    BOOST_CHECK(Type::type_string == stringToType("std::string"));
    BOOST_CHECK(Type::type_wstring == stringToType("std::wstring"));
    BOOST_CHECK(Type::type_path == stringToType("tl::Path"));
    BOOST_CHECK(Type::type_size == stringToType("tl::Size"));
    BOOST_CHECK(Type::type_map == stringToType("std::map"));
}
