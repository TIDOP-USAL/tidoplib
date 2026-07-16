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
#include <tidop/core/console/RangeValidator.h>
#include <tidop/core/console/ValuesValidator.h>
#include <tidop/core/console/CustomValidator.h>
#include <tidop/core/console/PathValidator.h>
#include <tidop/core/console/RegexValidator.h>

#include <array>
#include <fstream>
#include <regex>

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

BOOST_FIXTURE_TEST_CASE(to_string, ValidatorTest)
{
    BOOST_CHECK_EQUAL("Valid range [0 - 100]", range_validator->toString());
    BOOST_CHECK_EQUAL("Allowed values: [ XML, YML, TXT, BIN ]", value_validator->toString());
}

BOOST_AUTO_TEST_CASE(custom_validator_predicate)
{
    // Validador que comprueba que el número sea par
    auto is_even = [](int v) { return v % 2 == 0; };
    auto validator = CustomValidator<int>::create(is_even, "El valor debe ser par");

    BOOST_CHECK(validator->validate(2));
    BOOST_CHECK(validator->validate(0));
    BOOST_CHECK(!validator->validate(3));
    BOOST_CHECK(!validator->validate(-1));

    // Validador para strings: longitud > 5
    auto length_gt_5 = [](const std::string &s) { return s.length() > 5; };
    auto validator_str = CustomValidator<std::string>::create(length_gt_5, "Longitud mayor a 5");
    BOOST_CHECK(validator_str->validate("abcdef"));
    BOOST_CHECK(validator_str->validate("123456"));
    BOOST_CHECK(!validator_str->validate("abc"));
    BOOST_CHECK(!validator_str->validate(""));
}

BOOST_AUTO_TEST_CASE(custom_validator_toString)
{
    auto pred = [](double d) { return d > 0.0; };
    auto validator = CustomValidator<double>::create(pred, "Debe ser positivo");
    // Verificar que toString devuelve la descripción
    BOOST_CHECK_EQUAL(validator->toString(), "Custom rule: Debe ser positivo");
}

BOOST_AUTO_TEST_CASE(regex_validator_match)
{
    // Patrón para números enteros (opcionalmente con signo)
    auto validator_int = RegexValidator::create(R"(^[+-]?\d+$)", "número entero");

    BOOST_CHECK(validator_int->validate("123"));
    BOOST_CHECK(validator_int->validate("-456"));
    BOOST_CHECK(validator_int->validate("+789"));
    BOOST_CHECK(!validator_int->validate("12.3"));
    BOOST_CHECK(!validator_int->validate("abc"));
    BOOST_CHECK(!validator_int->validate(""));

    // Patrón para correo electrónico simplificado
    auto validator_email = RegexValidator::create(
        R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)",
        "dirección de correo"
    );
    BOOST_CHECK(validator_email->validate("test@example.com"));
    BOOST_CHECK(validator_email->validate("user.name@domain.co"));
    BOOST_CHECK(!validator_email->validate("test@example"));
    BOOST_CHECK(!validator_email->validate("test.example.com"));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(PathValidatorTestSuite)

struct PathValidatorTest
{

    PathValidatorTest()
    {
        // Crear directorio temporal y un archivo dentro
        temp_dir = Path::createTempDirectory(); // std::filesystem::temp_directory_path() / "test_validator_dir";
        temp_dir.createDirectories(); // std::filesystem::create_directories(temp_dir);
        temp_file = temp_dir;
        temp_file.append("test_file.txt");
        std::ofstream(temp_file.toString()) << "contenido de prueba";
        non_existent_path = Path(temp_dir).append("no_file.txt");
    }

    ~PathValidatorTest()
    {
        Path::removeDirectory(temp_dir);
    }

    Path temp_dir;
    Path temp_file;
    Path non_existent_path;

};

BOOST_FIXTURE_TEST_CASE(path_validator_any, PathValidatorTest)
{
    // 1. Cualquier ruta, debe existir (por defecto)
    auto validator = PathValidator::create(PathValidator::Type::any, true);

    // Ruta existente (directorio)
    BOOST_CHECK(validator->validate(temp_dir));
    // Ruta existente (archivo)
    BOOST_CHECK(validator->validate(temp_file));
    // Ruta no existente -> debe fallar
    BOOST_CHECK(!validator->validate(non_existent_path));

    // 2. Cualquier ruta, no es necesario que exista
    auto validator_no_exist = PathValidator::create(PathValidator::Type::any, false);
    BOOST_CHECK(validator_no_exist->validate(non_existent_path));
}

BOOST_FIXTURE_TEST_CASE(path_validator_file, PathValidatorTest)
{
    // Debe ser un archivo y debe existir
    auto validator = PathValidator::create(PathValidator::Type::file, true);

    // Archivo existente -> OK
    BOOST_CHECK(validator->validate(temp_file));
    // Directorio existente -> falla (no es archivo)
    BOOST_CHECK(!validator->validate(temp_dir));
    // Ruta no existente -> falla
    BOOST_CHECK(!validator->validate(non_existent_path));

    // Debe ser un archivo pero no es necesario que exista (solo valida tipo si existe)
    auto validator_no_exist = PathValidator::create(PathValidator::Type::file, false);
    // Ruta no existente -> OK porque no se comprueba existencia
    BOOST_CHECK(validator_no_exist->validate(non_existent_path));
    // Directorio existente -> falla porque no es archivo
    BOOST_CHECK(!validator_no_exist->validate(temp_dir));
}

BOOST_FIXTURE_TEST_CASE(path_validator_directory, PathValidatorTest)
{
    auto validator = PathValidator::create(PathValidator::Type::directory, true);

    // Directorio existente -> OK
    BOOST_CHECK(validator->validate(temp_dir));
    // Archivo existente -> falla
    BOOST_CHECK(!validator->validate(temp_file));
    // No existente -> falla
    BOOST_CHECK(!validator->validate(non_existent_path));

    // Directorio, no requiere existencia
    auto validator_no_exist = PathValidator::create(PathValidator::Type::directory, false);
    BOOST_CHECK(validator_no_exist->validate(non_existent_path));
    BOOST_CHECK(!validator_no_exist->validate(temp_file)); // archivo, no directorio
}

BOOST_AUTO_TEST_SUITE_END()