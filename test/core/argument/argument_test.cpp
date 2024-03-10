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
 
#define BOOST_TEST_MODULE Tidop Argument test
#include <boost/test/unit_test.hpp>
#include <tidop/core/console.h>

#include <array>

using namespace tl;


/* ArgumentTest */

/* Test de constructores */

BOOST_AUTO_TEST_CASE(ArgumentIntegerRequired_constructor)
{
    Argument_<int> arg_int("int", 'i', "integer argument");

    BOOST_CHECK_EQUAL("int", arg_int.name());
    BOOST_CHECK_EQUAL("integer argument", arg_int.description());
    BOOST_CHECK_EQUAL('i', arg_int.shortName());
    BOOST_CHECK_EQUAL("int", arg_int.typeName());
    BOOST_CHECK_EQUAL(true, arg_int.isRequired());

    auto arg_int2 = Argument::make<int>("int", "integer argument");
    BOOST_CHECK_EQUAL("int", arg_int2->name());
    BOOST_CHECK_EQUAL("integer argument", arg_int2->description());
    BOOST_CHECK_EQUAL(char(), arg_int2->shortName());
    BOOST_CHECK_EQUAL("int", arg_int2->typeName());
    BOOST_CHECK_EQUAL(true, arg_int2->isRequired());

    Argument_<int> arg_int3('i', "integer argument");

    BOOST_CHECK_EQUAL("", arg_int3.name());
    BOOST_CHECK_EQUAL("integer argument", arg_int3.description());
    BOOST_CHECK_EQUAL('i', arg_int3.shortName());
    BOOST_CHECK_EQUAL("int", arg_int3.typeName());
    BOOST_CHECK_EQUAL(true, arg_int3.isRequired());
}

BOOST_AUTO_TEST_CASE(ArgumentIntegerOptional_constructor)
{
    int val_i = 10;
    Argument_<int> arg_int("int", 'i', "integer argument", val_i);

    BOOST_CHECK_EQUAL("int", arg_int.name());
    BOOST_CHECK_EQUAL("integer argument", arg_int.description());
    BOOST_CHECK_EQUAL('i', arg_int.shortName());
    BOOST_CHECK_EQUAL("int", arg_int.typeName());
    BOOST_CHECK_EQUAL(false, arg_int.isRequired());
}

BOOST_AUTO_TEST_CASE(ArgumentDoubleRequired_constructor)
{
    Argument_<double> arg_double("double", 'd', "double argument");

    BOOST_CHECK_EQUAL("double", arg_double.name());
    BOOST_CHECK_EQUAL("double argument", arg_double.description());
    BOOST_CHECK_EQUAL('d', arg_double.shortName());
    BOOST_CHECK_EQUAL("double", arg_double.typeName());
    BOOST_CHECK_EQUAL(true, arg_double.isRequired());

    auto arg_double2 = Argument::make<double>("double", "double argument");

    BOOST_CHECK_EQUAL("double", arg_double2->name());
    BOOST_CHECK_EQUAL("double argument", arg_double2->description());
    BOOST_CHECK_EQUAL(char(), arg_double2->shortName());
    BOOST_CHECK_EQUAL("double", arg_double2->typeName());
    BOOST_CHECK_EQUAL(true, arg_double2->isRequired());

    Argument_<double> arg_double3('d', "double argument");

    BOOST_CHECK_EQUAL("", arg_double3.name());
    BOOST_CHECK_EQUAL("double argument", arg_double3.description());
    BOOST_CHECK_EQUAL('d', arg_double3.shortName());
    BOOST_CHECK_EQUAL("double", arg_double3.typeName());
    BOOST_CHECK_EQUAL(true, arg_double3.isRequired());
}

BOOST_AUTO_TEST_CASE(ArgumentDoubleOptional_constructor)
{
    Argument_<double> arg_double("double", 'd', "double argument", 10.);

    BOOST_CHECK_EQUAL("double", arg_double.name());
    BOOST_CHECK_EQUAL("double argument", arg_double.description());
    BOOST_CHECK_EQUAL('d', arg_double.shortName());
    BOOST_CHECK_EQUAL("double", arg_double.typeName());
    BOOST_CHECK_EQUAL(false, arg_double.isRequired());
}

BOOST_AUTO_TEST_CASE(ArgumentFloatRequired_constructor)
{
    Argument_<float> arg_float("float", 'f', "coma flotante");

    BOOST_CHECK_EQUAL("float", arg_float.name());
    BOOST_CHECK_EQUAL("coma flotante", arg_float.description());
    BOOST_CHECK_EQUAL('f', arg_float.shortName());
    BOOST_CHECK_EQUAL("float", arg_float.typeName());
    BOOST_CHECK_EQUAL(true, arg_float.isRequired());

    Argument_<float> arg_float2("float", "coma flotante");

    BOOST_CHECK_EQUAL("float", arg_float2.name());
    BOOST_CHECK_EQUAL("coma flotante", arg_float2.description());
    BOOST_CHECK_EQUAL(char(), arg_float2.shortName());
    BOOST_CHECK_EQUAL("float", arg_float2.typeName());
    BOOST_CHECK_EQUAL(true, arg_float2.isRequired());

    Argument_<float> arg_float3('f', "coma flotante");

    BOOST_CHECK_EQUAL("", arg_float3.name());
    BOOST_CHECK_EQUAL("coma flotante", arg_float3.description());
    BOOST_CHECK_EQUAL('f', arg_float3.shortName());
    BOOST_CHECK_EQUAL("float", arg_float3.typeName());
    BOOST_CHECK_EQUAL(true, arg_float3.isRequired());
}

BOOST_AUTO_TEST_CASE(ArgumentFloatOptional_constructor)
{
    Argument_<float> arg_float("float", 'f', "coma flotante", 0.5f);

    BOOST_CHECK_EQUAL("float", arg_float.name());
    BOOST_CHECK_EQUAL("coma flotante", arg_float.description());
    BOOST_CHECK_EQUAL('f', arg_float.shortName());
    BOOST_CHECK_EQUAL("float", arg_float.typeName());
    BOOST_CHECK_EQUAL(false, arg_float.isRequired());
}

BOOST_AUTO_TEST_CASE(ArgumentBooleanRequired_constructor)
{
    Argument_<bool> arg_bool("bool", 'b', "option");

    BOOST_CHECK_EQUAL("bool", arg_bool.name());
    BOOST_CHECK_EQUAL("option", arg_bool.description());
    BOOST_CHECK_EQUAL('b', arg_bool.shortName());
    BOOST_CHECK_EQUAL("bool", arg_bool.typeName());
    BOOST_CHECK_EQUAL(true, arg_bool.isRequired());

    Argument_<bool> arg_float2("bool", "option");

    BOOST_CHECK_EQUAL("bool", arg_float2.name());
    BOOST_CHECK_EQUAL("option", arg_float2.description());
    BOOST_CHECK_EQUAL(char(), arg_float2.shortName());
    BOOST_CHECK_EQUAL("bool", arg_float2.typeName());
    BOOST_CHECK_EQUAL(true, arg_float2.isRequired());

    Argument_<bool> arg_float3('b', "option");

    BOOST_CHECK_EQUAL("", arg_float3.name());
    BOOST_CHECK_EQUAL("option", arg_float3.description());
    BOOST_CHECK_EQUAL('b', arg_float3.shortName());
    BOOST_CHECK_EQUAL("bool", arg_float3.typeName());
    BOOST_CHECK_EQUAL(true, arg_float3.isRequired());
}

BOOST_AUTO_TEST_CASE(ArgumentBooleanOptional_constructor)
{
    Argument_<bool> arg_bool("bool", 'b', "option", true);

    BOOST_CHECK_EQUAL("bool", arg_bool.name());
    BOOST_CHECK_EQUAL("option", arg_bool.description());
    BOOST_CHECK_EQUAL('b', arg_bool.shortName());
    BOOST_CHECK_EQUAL("bool", arg_bool.typeName());
    BOOST_CHECK_EQUAL(false, arg_bool.isRequired());
}

BOOST_AUTO_TEST_CASE(ArgumentStringRequired_constructor)
{
    Argument_<std::string> arg_string("string", 's', "Cadena de texto");

    BOOST_CHECK_EQUAL("string", arg_string.name());
    BOOST_CHECK_EQUAL("Cadena de texto", arg_string.description());
    BOOST_CHECK_EQUAL('s', arg_string.shortName());
    BOOST_CHECK_EQUAL("std::string", arg_string.typeName());
    BOOST_CHECK_EQUAL(true, arg_string.isRequired());

    Argument_<std::string> arg_string2("string", "Cadena de texto");

    BOOST_CHECK_EQUAL("string", arg_string2.name());
    BOOST_CHECK_EQUAL("Cadena de texto", arg_string2.description());
    BOOST_CHECK_EQUAL(char(), arg_string2.shortName());
    BOOST_CHECK_EQUAL("std::string", arg_string2.typeName());
    BOOST_CHECK_EQUAL(true, arg_string2.isRequired());

    Argument_<std::string> arg_string3('s', "Cadena de texto");

    BOOST_CHECK_EQUAL("", arg_string3.name());
    BOOST_CHECK_EQUAL("Cadena de texto", arg_string3.description());
    BOOST_CHECK_EQUAL('s', arg_string3.shortName());
    BOOST_CHECK_EQUAL("std::string", arg_string3.typeName());
    BOOST_CHECK_EQUAL(true, arg_string3.isRequired());
}

BOOST_AUTO_TEST_CASE(ArgumentStringOptional_constructor)
{
    std::string val_string = "cadena";
    Argument_<std::string> arg_string("string", 's', "Cadena de texto", val_string);

    BOOST_CHECK_EQUAL("string", arg_string.name());
    BOOST_CHECK_EQUAL("Cadena de texto", arg_string.description());
    BOOST_CHECK_EQUAL('s', arg_string.shortName());
    BOOST_CHECK_EQUAL("std::string", arg_string.typeName());
    BOOST_CHECK_EQUAL(false, arg_string.isRequired());
}

BOOST_AUTO_TEST_CASE(ArgumentList_constructor)
{
    Argument_<int> arg_list("list", "lista de argumentos", 10);
    arg_list.setValidator(ValuesValidator<int>::create({0, 10, 20, 30, 40}));
    BOOST_CHECK_EQUAL("list", arg_list.name());
    BOOST_CHECK_EQUAL("lista de argumentos", arg_list.description());
    BOOST_CHECK_EQUAL(char(), arg_list.shortName());
    BOOST_CHECK_EQUAL("int", arg_list.typeName());
    BOOST_CHECK_EQUAL(false, arg_list.isRequired());
    BOOST_CHECK_EQUAL(10, arg_list.value());
}

///* Fin test de constructores */


BOOST_AUTO_TEST_SUITE(ArgumentTestSuite)

struct ArgumentTest
{
    ArgumentTest() { }
    ~ArgumentTest() { }

    void setup()
    {
        arg_int = Argument::make<int>("int", 'i', "integer");
        arg_double = Argument::make<double>("double", 'd', "double", 2.5);
        arg_float = Argument::make<float>("float", 'f', "coma flotante", 0.567f);
        arg_string = Argument::make<std::string>("string", 's', "Cadena de texto", "string");
        arg_bool = Argument::make<bool>("bool", 'b', "boolean", false);

        arg_list = Argument::make<int>("list", "lista de argumentos", 20);
        arg_list->setValidator(ValuesValidator<int>::create({0, 10, 20, 30, 40}));
    }

    void teardown() {}

    std::shared_ptr<Argument_<int>> arg_int;
    std::shared_ptr<Argument_<double>> arg_double;
    std::shared_ptr<Argument_<float>> arg_float;
    std::shared_ptr<Argument_<std::string>> arg_string;
    std::shared_ptr<Argument_<bool>> arg_bool;
    std::shared_ptr<Argument_<int>> arg_list;

};

BOOST_FIXTURE_TEST_CASE(Argument_setName, ArgumentTest)
{
    arg_int->setName("new_name");
    BOOST_CHECK_EQUAL("new_name", arg_int->name());
}

BOOST_FIXTURE_TEST_CASE(Argument_setDescription, ArgumentTest)
{
  arg_int->setDescription("new description");
  BOOST_CHECK_EQUAL("new description", arg_int->description());
}

BOOST_FIXTURE_TEST_CASE(Argument_setShortName, ArgumentTest)
{
    arg_int->setShortName('n');
    BOOST_CHECK_EQUAL('n', arg_int->shortName());
}

BOOST_FIXTURE_TEST_CASE(Argument_setValue, ArgumentTest)
{
    arg_int->setValue(15);
    BOOST_CHECK_EQUAL(15, arg_int->value());
}

BOOST_FIXTURE_TEST_CASE(Argument_fromString, ArgumentTest)
{
    arg_int->fromString("2");
    BOOST_CHECK_EQUAL(2, arg_int->value());

    arg_double->fromString("2.3");
    BOOST_CHECK_CLOSE(2.3, arg_double->value(), 0.1);

    arg_float->fromString("0.3536");
    BOOST_CHECK_CLOSE(0.3536f, arg_float->value(), 0.001f);

    arg_bool->fromString("true");
    BOOST_CHECK_EQUAL(true, arg_bool->value());
    arg_bool->fromString("false");
    BOOST_CHECK_EQUAL(false, arg_bool->value());
    arg_bool->fromString("1");
    BOOST_CHECK_EQUAL(true, arg_bool->value());

    arg_list->fromString("10");
    BOOST_CHECK_EQUAL(10, arg_list->value());
}

BOOST_FIXTURE_TEST_CASE(ArgumentList_setValue, ArgumentTest)
{
    arg_list->setValue(10);
    BOOST_CHECK_EQUAL(10, arg_list->value());

    arg_list->setValue(20);
    BOOST_CHECK_EQUAL(20, arg_list->value());

    arg_list->setValue(40);
    BOOST_CHECK_EQUAL(40, arg_list->value());
}

BOOST_FIXTURE_TEST_CASE(ArgumentList_isValid, ArgumentTest)
{
    arg_list->setValue(15);
    BOOST_CHECK(false == arg_list->isValid());

    arg_list->setValue(20);
    BOOST_CHECK(arg_list->isValid());

    arg_list->setValue(50);
    BOOST_CHECK(false == arg_list->isValid());
}

BOOST_AUTO_TEST_SUITE_END()




/* Command Test */

BOOST_AUTO_TEST_SUITE(CommandTestSuite)

struct CommandTest
{
    CommandTest()
      : file(),
        cmd_arg_posix(new Command),
        cmd_arg_posix2(new Command("cmd_name", "cmd description")),
        cmd_arg_posix3(new Command("name", "Lista de inicializadores", {Argument::make<std::string>("input", 'i', "Fichero de entrada")})),
        cmd_arg_list(new Command)
    {
    }

    ~CommandTest()
    {
        if(cmd_arg_posix) delete cmd_arg_posix;
        if(cmd_arg_posix2) delete cmd_arg_posix2;
        if(cmd_arg_posix3) delete cmd_arg_posix3;
        if(cmd_arg_list) delete cmd_arg_list;
    }

    void setup()
    {
        /* addArgument with range validator */
        auto arg_int = Argument::make<int>("int", 'i', "integer");
        arg_int->setValidator(RangeValidator<int>::create(0, 10));
        cmd_arg_posix2->addArgument(arg_int);

        /* addArgument with Argument::make() */
        cmd_arg_posix2->addArgument(Argument::make<double>("par", 'p', "Parámetro", 3.46));

        /* addArgument */
        cmd_arg_posix2->addArgument<float>("float", 'f', "Parámetro float", 2.23f);
        cmd_arg_posix2->addArgument<bool>("bool", 'b', "boolean", false);

        /* Alternative to addArgument with push_back */
        cmd_arg_posix2->push_back(Argument::make<bool>("option", 'o', "Option", false));
        
        /* addArgument with list validator */
        auto arg_list = Argument::make<int>("list", "lista de argumentos", 20);
        arg_list->setValidator(ValuesValidator<int>::create({0, 10, 20, 30, 40}));
        cmd_arg_list->addArgument(arg_list);

        auto arg_options = Argument::make<std::string>("options", "lista de opciones", "OPT0");
        arg_options->setValidator(ValuesValidator<std::string>::create({"OPT0", "OPT1", "OPT2", "OPT3", "OPT4"}));
        cmd_arg_list->push_back(arg_options);
    }

    void teardown() {}

    std::string file;
    Command *cmd_arg_posix;
    Command *cmd_arg_posix2;
    Command *cmd_arg_posix3;
    Command *cmd_arg_list;
    std::string cmd_help;

};

BOOST_FIXTURE_TEST_CASE(DefaultConstructor, CommandTest)
{
    BOOST_CHECK(cmd_arg_posix->name().empty());
    BOOST_CHECK(cmd_arg_posix->description().empty());
    BOOST_CHECK_EQUAL("0.0.0", cmd_arg_posix->version());
    BOOST_CHECK(cmd_arg_posix->empty());
}

BOOST_FIXTURE_TEST_CASE(Constructor, CommandTest)
{
    BOOST_CHECK_EQUAL("cmd_name", cmd_arg_posix2->name());
    BOOST_CHECK_EQUAL("cmd description", cmd_arg_posix2->description());
}

BOOST_FIXTURE_TEST_CASE(set_and_get_name, CommandTest)
{
    cmd_arg_posix2->setName("new_name");
    BOOST_CHECK_EQUAL("new_name", cmd_arg_posix2->name());
}

BOOST_FIXTURE_TEST_CASE(set_and_get_description, CommandTest)
{
    cmd_arg_posix2->setDescription("New description");
    BOOST_CHECK_EQUAL("New description", cmd_arg_posix2->description());
}

BOOST_FIXTURE_TEST_CASE(set_and_get_version, CommandTest)
{
    cmd_arg_posix2->setVersion("1.0.0");
    BOOST_CHECK_EQUAL("1.0.0", cmd_arg_posix2->version());
}

// En C++20:
// error C2440: 'inicializando': no se puede realizar la conversión de 'const char [1]' a '_Ty'

BOOST_FIXTURE_TEST_CASE(parseHelp, CommandTest)
{
    std::array<char *, 2> argv{const_cast<char *>(""), const_cast<char *>("-h")};
    BOOST_CHECK(cmd_arg_posix2->parse(static_cast<int>(argv.size()), argv.data()) == Command::Status::show_help);

    std::array<char *, 2> argv2{const_cast<char *>("") , const_cast<char *>("--help")};
    BOOST_CHECK(cmd_arg_posix2->parse(static_cast<int>(argv2.size()), argv2.data()) == Command::Status::show_help);
}

BOOST_FIXTURE_TEST_CASE(parseVersion, CommandTest)
{
    std::array<char *, 2> argv2{const_cast<char *>(""), const_cast<char *>("--version")};
    BOOST_CHECK(cmd_arg_posix2->parse(static_cast<int>(argv2.size()), argv2.data()) == Command::Status::show_version);
}

BOOST_FIXTURE_TEST_CASE(parseLicence, CommandTest)
{
    std::array<char *, 2> argv2{const_cast<char *>(""), const_cast<char *>("--licence")};
    BOOST_CHECK(cmd_arg_posix2->parse(static_cast<int>(argv2.size()), argv2.data()) == Command::Status::show_licence);
}

BOOST_FIXTURE_TEST_CASE(parse_option_ok, CommandTest)
{
    std::array<char *, 4> argv{const_cast<char *>(""), const_cast<char *>("-i"), const_cast<char *>("2"), const_cast<char *>("-b")};
    BOOST_CHECK(cmd_arg_posix2->parse(static_cast<int>(argv.size()), argv.data()) == Command::Status::parse_success);
    BOOST_CHECK_EQUAL(2, cmd_arg_posix2->value<int>("int"));
    BOOST_CHECK_EQUAL(2, cmd_arg_posix2->value<int>('i'));
    BOOST_CHECK_EQUAL(3.46, cmd_arg_posix2->value<double>("par"));
    BOOST_CHECK_EQUAL(3.46, cmd_arg_posix2->value<double>('p'));
    BOOST_CHECK_EQUAL(2.23f, cmd_arg_posix2->value<float>("float"));
    BOOST_CHECK_EQUAL(2.23f, cmd_arg_posix2->value<float>('f'));
    BOOST_CHECK_EQUAL(true, cmd_arg_posix2->value<bool>("bool"));
    BOOST_CHECK_EQUAL(true, cmd_arg_posix2->value<bool>('b'));
    BOOST_CHECK_EQUAL(false, cmd_arg_posix2->value<bool>("option"));
    BOOST_CHECK_EQUAL(false, cmd_arg_posix2->value<bool>('o'));

    std::array<char *, 4> argv_large{const_cast<char *>(""), const_cast<char *>("--int"), const_cast<char *>("2"), const_cast<char *>("-b")};
    BOOST_CHECK(cmd_arg_posix2->parse(static_cast<int>(argv_large.size()), argv_large.data()) == Command::Status::parse_success);
    BOOST_CHECK_EQUAL(2, cmd_arg_posix2->value<int>("int"));
    BOOST_CHECK_EQUAL(2, cmd_arg_posix2->value<int>('i'));
    BOOST_CHECK_EQUAL(3.46, cmd_arg_posix2->value<double>("par"));
    BOOST_CHECK_EQUAL(3.46, cmd_arg_posix2->value<double>('p'));
    BOOST_CHECK_EQUAL(2.23f, cmd_arg_posix2->value<float>("float"));
    BOOST_CHECK_EQUAL(2.23f, cmd_arg_posix2->value<float>('f'));
    BOOST_CHECK_EQUAL(true, cmd_arg_posix2->value<bool>("bool"));
    BOOST_CHECK_EQUAL(true, cmd_arg_posix2->value<bool>('b'));
    BOOST_CHECK_EQUAL(false, cmd_arg_posix2->value<bool>("option"));
    BOOST_CHECK_EQUAL(false, cmd_arg_posix2->value<bool>('o'));
}

BOOST_FIXTURE_TEST_CASE(parse_option_short, CommandTest)
{
    std::array<char *, 3> argv{const_cast<char *>(""), const_cast<char *>("-i2"), const_cast<char *>("-o")};
    BOOST_CHECK(cmd_arg_posix2->parse(static_cast<int>(argv.size()), argv.data()) == Command::Status::parse_success);
    BOOST_CHECK_EQUAL(2, cmd_arg_posix2->value<int>("int"));
    BOOST_CHECK_EQUAL(2, cmd_arg_posix2->value<int>('i'));
    BOOST_CHECK_EQUAL(3.46, cmd_arg_posix2->value<double>("par"));
    BOOST_CHECK_EQUAL(3.46, cmd_arg_posix2->value<double>('p'));
    BOOST_CHECK_EQUAL(2.23f, cmd_arg_posix2->value<float>("float"));
    BOOST_CHECK_EQUAL(2.23f, cmd_arg_posix2->value<float>('f'));
    BOOST_CHECK_EQUAL(false, cmd_arg_posix2->value<bool>("bool"));
    BOOST_CHECK_EQUAL(false, cmd_arg_posix2->value<bool>('b'));
    BOOST_CHECK_EQUAL(true, cmd_arg_posix2->value<bool>("option"));
    BOOST_CHECK_EQUAL(true, cmd_arg_posix2->value<bool>('o'));
}

BOOST_FIXTURE_TEST_CASE(parse_option_ok_equal, CommandTest)
{
    std::array<char *, 3> argv_large{const_cast<char *>("") , const_cast<char *>("--int=2"), const_cast<char *>("-b")};
    BOOST_CHECK(cmd_arg_posix2->parse(static_cast<int>(argv_large.size()), argv_large.data()) == Command::Status::parse_success);
    BOOST_CHECK_EQUAL(2, cmd_arg_posix2->value<int>("int"));
    BOOST_CHECK_EQUAL(2, cmd_arg_posix2->value<int>('i'));
    BOOST_CHECK_EQUAL(3.46, cmd_arg_posix2->value<double>("par"));
    BOOST_CHECK_EQUAL(3.46, cmd_arg_posix2->value<double>('p'));
    BOOST_CHECK_EQUAL(2.23f, cmd_arg_posix2->value<float>("float"));
    BOOST_CHECK_EQUAL(2.23f, cmd_arg_posix2->value<float>('f'));
    BOOST_CHECK_EQUAL(true, cmd_arg_posix2->value<bool>("bool"));
    BOOST_CHECK_EQUAL(true, cmd_arg_posix2->value<bool>('b'));
    BOOST_CHECK_EQUAL(false, cmd_arg_posix2->value<bool>("option"));
    BOOST_CHECK_EQUAL(false, cmd_arg_posix2->value<bool>('o'));
}

BOOST_FIXTURE_TEST_CASE(parse_error, CommandTest)
{
    std::array<char *, 2> argv{const_cast<char *>(""), const_cast<char *>("-b")};
    BOOST_CHECK(cmd_arg_posix2->parse(static_cast<int>(argv.size()), argv.data()) == Command::Status::parse_error);
}

BOOST_FIXTURE_TEST_CASE(parseOptionsParameter, CommandTest)
{
    std::array<char *, 5> argv{const_cast<char *>(""), const_cast<char *>("--options"), const_cast<char *>("OPT3"), const_cast<char *>("--list"), const_cast<char *>("10")};
    BOOST_CHECK(cmd_arg_list->parse(static_cast<int>(argv.size()), argv.data()) == Command::Status::parse_success);
    BOOST_CHECK_EQUAL("OPT3", cmd_arg_list->value<std::string>("options"));
    BOOST_CHECK_EQUAL(10, cmd_arg_list->value<int>("list"));

    std::array<char *, 3> argv2{const_cast<char *>(""), const_cast<char *>("--options"), const_cast<char *>("OPT_ERROR")};
    BOOST_CHECK(cmd_arg_list->parse(static_cast<int>(argv2.size()), argv2.data()) == Command::Status::parse_error);
}

BOOST_FIXTURE_TEST_CASE(size, CommandTest)
{
    BOOST_CHECK_EQUAL(0, cmd_arg_posix->size());
    BOOST_CHECK_EQUAL(5, cmd_arg_posix2->size());
}

BOOST_FIXTURE_TEST_CASE(clear, CommandTest)
{
    cmd_arg_posix2->clear();
    BOOST_CHECK_EQUAL(0, cmd_arg_posix2->size());
}

BOOST_FIXTURE_TEST_CASE(empty, CommandTest)
{
    BOOST_CHECK(cmd_arg_posix->empty());
    BOOST_CHECK(false == cmd_arg_posix2->empty());
}

BOOST_FIXTURE_TEST_CASE(parseTextWithHyphen, CommandTest)
{
    std::array<char *, 3> argv{const_cast<char *>(""), const_cast<char *>("--input"), const_cast<char *>("sdfsd-sdfsdf")};
    BOOST_CHECK(cmd_arg_posix3->parse(static_cast<int>(argv.size()), argv.data()) == Command::Status::parse_success);
}


BOOST_AUTO_TEST_SUITE_END()


/* CommandList Test */

BOOST_AUTO_TEST_SUITE(CommandListTestSuite)

struct CommandListTest
{

    CommandListTest()
        : cmd_list_transform(new CommandList("transform", "Transform description"))
    {
    }

    ~CommandListTest()
    {
        if (cmd_list_transform) delete cmd_list_transform;
    }

    void setup()
    {
        auto arg_compute = Argument::make<bool>("compute", "Calcula la transformación a partir de dos listas de puntos", false);
        auto arg_transform = Argument::make<bool>("transform", "Aplica la transformación a un listado de puntos", true);
        auto arg_tx = Argument::make<double>("tx", "Traslación en X", 0.0);
        auto arg_ty = Argument::make<double>("ty", "Traslación en Y", 0.0);
        auto arg_rotation = Argument::make<double>("rotation", "Rotación", 0.);
        auto arg_scale = Argument::make<double>("scale", "Escala", 1.);
        arg_scale->setValidator(RangeValidator<double>::create(0., 100));
        auto arg_scale_x = Argument::make<double>("scale_x", "Escala X", 1.);
        auto arg_scale_y = Argument::make<double>("scale_y", "Escala Y", 1.);


        auto cmd_translation = Command::create("Translation", "Translation transform", {
                                                             arg_compute,
                                                             arg_transform,
                                                             arg_tx,
                                                             arg_ty});

        auto cmd_rotation = Command::create("Rotation", "Rotation transform");
        cmd_rotation->addArgument(arg_compute);
        cmd_rotation->addArgument(arg_transform);
        cmd_rotation->addArgument(arg_rotation);

        cmd_list_transform->addCommand(cmd_translation);
        cmd_list_transform->addCommand(cmd_rotation);
    }

    void teardown() {}

    CommandList *cmd_list_transform;
};


BOOST_FIXTURE_TEST_CASE(DefaultConstructor, CommandListTest)
{
    CommandList cmd_list;
    BOOST_CHECK(cmd_list.name().empty());
    BOOST_CHECK(cmd_list.description().empty());
    BOOST_CHECK_EQUAL("0.0.0", cmd_list.version());
    BOOST_CHECK(cmd_list.empty());
}

BOOST_FIXTURE_TEST_CASE(Constructor, CommandListTest)
{
    BOOST_CHECK_EQUAL("transform", cmd_list_transform->name());
    BOOST_CHECK_EQUAL("Transform description", cmd_list_transform->description());
}

BOOST_FIXTURE_TEST_CASE(set_and_get_name, CommandListTest)
{
    cmd_list_transform->setName("new_name");
    BOOST_CHECK_EQUAL("new_name", cmd_list_transform->name());
}

BOOST_FIXTURE_TEST_CASE(set_and_get_description, CommandListTest)
{
    cmd_list_transform->setDescription("New description");
    BOOST_CHECK_EQUAL("New description", cmd_list_transform->description());
}

BOOST_FIXTURE_TEST_CASE(set_and_get_version, CommandListTest)
{
    cmd_list_transform->setVersion("1.0.0");
    BOOST_CHECK_EQUAL("1.0.0", cmd_list_transform->version());
}


BOOST_FIXTURE_TEST_CASE(parseHelp, CommandListTest)
{
    std::array<char *, 2> argv{const_cast<char *>(""), const_cast<char *>("-h")};
    BOOST_CHECK(cmd_list_transform->parse(static_cast<int>(argv.size()), argv.data()) == Command::Status::show_help);

    std::array<char *, 3> argv2{const_cast<char *>(""), const_cast<char *>("Translation"), const_cast<char *>("-h")};
    BOOST_CHECK(cmd_list_transform->parse(static_cast<int>(argv2.size()), argv2.data()) == Command::Status::show_help);
}

BOOST_FIXTURE_TEST_CASE(parseVersion, CommandListTest)
{
    std::array<char *, 2> argv{const_cast<char *>(""), const_cast<char *>("--version")};
    BOOST_CHECK(cmd_list_transform->parse(static_cast<int>(argv.size()), argv.data()) == Command::Status::show_version);
}

BOOST_FIXTURE_TEST_CASE(parseLicence, CommandListTest)
{
    std::array<char *, 2> argv2{const_cast<char *>(""), const_cast<char *>("--licence")};
    BOOST_CHECK(cmd_list_transform->parse(static_cast<int>(argv2.size()), argv2.data()) == Command::Status::show_licence);
}

BOOST_FIXTURE_TEST_CASE(parseCommand, CommandListTest)
{
    std::array<char *, 4> argv2{const_cast<char *>(""), const_cast<char *>("Rotation"), const_cast<char *>("--compute"), const_cast<char *>("1")};
    BOOST_CHECK(cmd_list_transform->parse(static_cast<int>(argv2.size()), argv2.data()) == Command::Status::parse_success);

    BOOST_CHECK_EQUAL("Rotation", cmd_list_transform->commandName());
}


BOOST_FIXTURE_TEST_CASE(parse_error, CommandListTest)
{
    std::array<char *, 2> argv{const_cast<char *>(""), const_cast<char *>("-b")};
    BOOST_CHECK(cmd_list_transform->parse(static_cast<int>(argv.size()), argv.data()) == Command::Status::parse_error);
}

BOOST_FIXTURE_TEST_CASE(size, CommandListTest)
{
    BOOST_CHECK_EQUAL(2, cmd_list_transform->size());
}

BOOST_FIXTURE_TEST_CASE(clear, CommandListTest)
{
    cmd_list_transform->clear();
    BOOST_CHECK_EQUAL(0, cmd_list_transform->size());
}

BOOST_FIXTURE_TEST_CASE(empty, CommandListTest)
{
    BOOST_CHECK(false == cmd_list_transform->empty());
}

BOOST_AUTO_TEST_SUITE_END()