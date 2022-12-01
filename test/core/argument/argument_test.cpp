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
  int val_i = 0;
  ArgumentIntegerRequired arg_int("int", 'i', "integer argument", &val_i);

  BOOST_CHECK_EQUAL("int", arg_int.name());
  BOOST_CHECK_EQUAL("integer argument", arg_int.description());
  BOOST_CHECK_EQUAL('i', arg_int.shortName());
  BOOST_CHECK_EQUAL("int", arg_int.typeName());
  BOOST_CHECK_EQUAL(true, arg_int.isRequired());

  ArgumentIntegerRequired arg_int2("int", "integer argument", &val_i);

  BOOST_CHECK_EQUAL("int", arg_int2.name());
  BOOST_CHECK_EQUAL("integer argument", arg_int2.description());
  BOOST_CHECK_EQUAL(char(), arg_int2.shortName());
  BOOST_CHECK_EQUAL("int", arg_int2.typeName());
  BOOST_CHECK_EQUAL(true, arg_int2.isRequired());

  ArgumentIntegerRequired arg_int3('i', "integer argument", &val_i);

  BOOST_CHECK_EQUAL("", arg_int3.name());
  BOOST_CHECK_EQUAL("integer argument", arg_int3.description());
  BOOST_CHECK_EQUAL('i', arg_int3.shortName());
  BOOST_CHECK_EQUAL("int", arg_int3.typeName());
  BOOST_CHECK_EQUAL(true, arg_int3.isRequired());
}

BOOST_AUTO_TEST_CASE(ArgumentIntegerOptional_constructor)
{
  int val_i = 10;
  ArgumentIntegerOptional arg_int("int", 'i', "integer argument", &val_i);

  BOOST_CHECK_EQUAL("int", arg_int.name());
  BOOST_CHECK_EQUAL("integer argument", arg_int.description());
  BOOST_CHECK_EQUAL('i', arg_int.shortName());
  BOOST_CHECK_EQUAL("int", arg_int.typeName());
  BOOST_CHECK_EQUAL(false, arg_int.isRequired());
}

BOOST_AUTO_TEST_CASE(ArgumentDoubleRequired_constructor)
{
  double val_d = 0.0;
  ArgumentDoubleRequired arg_double("double", 'd', "double argument", &val_d);

  BOOST_CHECK_EQUAL("double", arg_double.name());
  BOOST_CHECK_EQUAL("double argument", arg_double.description());
  BOOST_CHECK_EQUAL('d', arg_double.shortName());
  BOOST_CHECK_EQUAL("double", arg_double.typeName());
  BOOST_CHECK_EQUAL(true, arg_double.isRequired());

  ArgumentDoubleRequired arg_double2("double", "double argument", &val_d);

  BOOST_CHECK_EQUAL("double", arg_double2.name());
  BOOST_CHECK_EQUAL("double argument", arg_double2.description());
  BOOST_CHECK_EQUAL(char(), arg_double2.shortName());
  BOOST_CHECK_EQUAL("double", arg_double2.typeName());
  BOOST_CHECK_EQUAL(true, arg_double2.isRequired());

  ArgumentDoubleRequired arg_double3('d', "double argument", &val_d);

  BOOST_CHECK_EQUAL("", arg_double3.name());
  BOOST_CHECK_EQUAL("double argument", arg_double3.description());
  BOOST_CHECK_EQUAL('d', arg_double3.shortName());
  BOOST_CHECK_EQUAL("double", arg_double3.typeName());
  BOOST_CHECK_EQUAL(true, arg_double3.isRequired());
}

BOOST_AUTO_TEST_CASE(ArgumentDoubleOptional_constructor)
{
  double val_d = 10.;
  ArgumentDoubleOptional arg_double("double", 'd', "double argument", &val_d);

  BOOST_CHECK_EQUAL("double", arg_double.name());
  BOOST_CHECK_EQUAL("double argument", arg_double.description());
  BOOST_CHECK_EQUAL('d', arg_double.shortName());
  BOOST_CHECK_EQUAL("double", arg_double.typeName());
  BOOST_CHECK_EQUAL(false, arg_double.isRequired());
}

BOOST_AUTO_TEST_CASE(ArgumentFloatRequired_constructor)
{
  float val_f = 0.0;
  ArgumentFloatRequired arg_float("float", 'f', "coma flotante", &val_f);

  BOOST_CHECK_EQUAL("float", arg_float.name());
  BOOST_CHECK_EQUAL("coma flotante", arg_float.description());
  BOOST_CHECK_EQUAL('f', arg_float.shortName());
  BOOST_CHECK_EQUAL("float", arg_float.typeName());
  BOOST_CHECK_EQUAL(true, arg_float.isRequired());

  ArgumentFloatRequired arg_float2("float", "coma flotante", &val_f);

  BOOST_CHECK_EQUAL("float", arg_float2.name());
  BOOST_CHECK_EQUAL("coma flotante", arg_float2.description());
  BOOST_CHECK_EQUAL(char(), arg_float2.shortName());
  BOOST_CHECK_EQUAL("float", arg_float2.typeName());
  BOOST_CHECK_EQUAL(true, arg_float2.isRequired());

  ArgumentFloatRequired arg_float3('f', "coma flotante", &val_f);

  BOOST_CHECK_EQUAL("", arg_float3.name());
  BOOST_CHECK_EQUAL("coma flotante", arg_float3.description());
  BOOST_CHECK_EQUAL('f', arg_float3.shortName());
  BOOST_CHECK_EQUAL("float", arg_float3.typeName());
  BOOST_CHECK_EQUAL(true, arg_float3.isRequired());
}

BOOST_AUTO_TEST_CASE(ArgumentFloatOptional_constructor)
{
  float val_f = 0.5f;
  ArgumentFloatOptional arg_float("float", 'f', "coma flotante", &val_f);

  BOOST_CHECK_EQUAL("float", arg_float.name());
  BOOST_CHECK_EQUAL("coma flotante", arg_float.description());
  BOOST_CHECK_EQUAL('f', arg_float.shortName());
  BOOST_CHECK_EQUAL("float", arg_float.typeName());
  BOOST_CHECK_EQUAL(false, arg_float.isRequired());
}

BOOST_AUTO_TEST_CASE(ArgumentBooleanRequired_constructor)
{
  bool val_b = false;
  ArgumentBooleanRequired arg_bool("bool", 'b', "option", &val_b);

  BOOST_CHECK_EQUAL("bool", arg_bool.name());
  BOOST_CHECK_EQUAL("option", arg_bool.description());
  BOOST_CHECK_EQUAL('b', arg_bool.shortName());
  BOOST_CHECK_EQUAL("bool", arg_bool.typeName());
  BOOST_CHECK_EQUAL(true, arg_bool.isRequired());

  ArgumentBooleanRequired arg_float2("bool", "option", &val_b);

  BOOST_CHECK_EQUAL("bool", arg_float2.name());
  BOOST_CHECK_EQUAL("option", arg_float2.description());
  BOOST_CHECK_EQUAL(char(), arg_float2.shortName());
  BOOST_CHECK_EQUAL("bool", arg_float2.typeName());
  BOOST_CHECK_EQUAL(true, arg_float2.isRequired());

  ArgumentBooleanRequired arg_float3('b', "option", &val_b);

  BOOST_CHECK_EQUAL("", arg_float3.name());
  BOOST_CHECK_EQUAL("option", arg_float3.description());
  BOOST_CHECK_EQUAL('b', arg_float3.shortName());
  BOOST_CHECK_EQUAL("bool", arg_float3.typeName());
  BOOST_CHECK_EQUAL(true, arg_float3.isRequired());
}

BOOST_AUTO_TEST_CASE(ArgumentBooleanOptional_constructor)
{
  bool val_b = true;
  ArgumentBooleanOptional arg_bool("bool", 'b', "option", &val_b);

  BOOST_CHECK_EQUAL("bool", arg_bool.name());
  BOOST_CHECK_EQUAL("option", arg_bool.description());
  BOOST_CHECK_EQUAL('b', arg_bool.shortName());
  BOOST_CHECK_EQUAL("bool", arg_bool.typeName());
  BOOST_CHECK_EQUAL(false, arg_bool.isRequired());
}

BOOST_AUTO_TEST_CASE(ArgumentStringRequired_constructor)
{
  std::string val_string;
  ArgumentStringRequired arg_string("string", 's', "Cadena de texto", &val_string);

  BOOST_CHECK_EQUAL("string", arg_string.name());
  BOOST_CHECK_EQUAL("Cadena de texto", arg_string.description());
  BOOST_CHECK_EQUAL('s', arg_string.shortName());
  BOOST_CHECK_EQUAL("std::string", arg_string.typeName());
  BOOST_CHECK_EQUAL(true, arg_string.isRequired());

  ArgumentStringRequired arg_string2("string", "Cadena de texto", &val_string);

  BOOST_CHECK_EQUAL("string", arg_string2.name());
  BOOST_CHECK_EQUAL("Cadena de texto", arg_string2.description());
  BOOST_CHECK_EQUAL(char(), arg_string2.shortName());
  BOOST_CHECK_EQUAL("std::string", arg_string2.typeName());
  BOOST_CHECK_EQUAL(true, arg_string2.isRequired());

  ArgumentStringRequired arg_string3('s', "Cadena de texto", &val_string);

  BOOST_CHECK_EQUAL("", arg_string3.name());
  BOOST_CHECK_EQUAL("Cadena de texto", arg_string3.description());
  BOOST_CHECK_EQUAL('s', arg_string3.shortName());
  BOOST_CHECK_EQUAL("std::string", arg_string3.typeName());
  BOOST_CHECK_EQUAL(true, arg_string3.isRequired());
}

BOOST_AUTO_TEST_CASE(ArgumentStringOptional_constructor)
{
  std::string val_string = "cadena";
  ArgumentStringOptional arg_string("string", 's', "Cadena de texto", &val_string);

  BOOST_CHECK_EQUAL("string", arg_string.name());
  BOOST_CHECK_EQUAL("Cadena de texto", arg_string.description());
  BOOST_CHECK_EQUAL('s', arg_string.shortName());
  BOOST_CHECK_EQUAL("std::string", arg_string.typeName());
  BOOST_CHECK_EQUAL(false, arg_string.isRequired());
}

BOOST_AUTO_TEST_CASE(ArgumentList_constructor)
{
  size_t idx = 1;
  std::vector<int> list{0, 10, 20, 30, 40};
  ArgumentList_<int, false> arg_list("list", "lista de argumentos", list, &idx);

  BOOST_CHECK_EQUAL("list", arg_list.name());
  BOOST_CHECK_EQUAL("lista de argumentos [Values: 0 10 20 30 40]", arg_list.description());
  BOOST_CHECK_EQUAL(char(), arg_list.shortName());
  BOOST_CHECK_EQUAL("int", arg_list.typeName());
  BOOST_CHECK_EQUAL(false, arg_list.isRequired());
}

/* Fin test de constructores */


BOOST_AUTO_TEST_SUITE(ArgumentTestSuite)

struct ArgumentTest
{
  ArgumentTest() { }
  ~ArgumentTest() { }
  
  void setup() 
  {
    val_i = 10;
    arg_int = std::make_shared<ArgumentIntegerRequired>("int", 'i', "integer", &val_i);
    val_d = 2.5;
    arg_double = std::make_shared<ArgumentDoubleOptional>("double", 'd', "double", &val_d);
    val_f = 0.567f;
    arg_float = std::make_shared<ArgumentFloatOptional>("float", 'f', "coma flotante", &val_f);

    arg_string = std::make_shared<ArgumentStringOptional>("string", 's', "Cadena de texto", &val_string);

    arg_bool = std::make_shared<ArgumentBooleanOptional>("bool", 'b', "boolean", &val_b);

    idx = 1;
    list.push_back(0);
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);
    list.push_back(40);
    arg_list = std::make_shared<ArgumentList_<int, false>>("list", "lista de argumentos", list, &idx);
  }

  void teardown() {}

  int val_i;
  std::shared_ptr<ArgumentIntegerRequired> arg_int;

  double val_d;
  std::shared_ptr<Argument> arg_double;

  float val_f;
  std::shared_ptr<Argument> arg_float;
  
  std::string val_string;
  std::shared_ptr<Argument> arg_string;

  bool val_b;
  std::shared_ptr<Argument> arg_bool;

  std::vector<int> list;
  size_t idx;
  std::shared_ptr<ArgumentList_<int, false>> arg_list;

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
  BOOST_CHECK_EQUAL(10, arg_int->value()); // Check default value
  arg_int->setValue(15);
  BOOST_CHECK_EQUAL(15, arg_int->value());
}

BOOST_FIXTURE_TEST_CASE(Argument_fromString, ArgumentTest)
{
  arg_int->fromString("2");
  BOOST_CHECK_EQUAL(2, val_i);

  arg_double->fromString("2.3");
  BOOST_CHECK_CLOSE(2.3, val_d, 0.1);

  arg_float->fromString("0.3536");
  BOOST_CHECK_CLOSE(0.3536f, val_f, 0.001f);

  arg_bool->fromString("true");
  BOOST_CHECK_EQUAL(true, val_b);
  arg_bool->fromString("false");
  BOOST_CHECK_EQUAL(false, val_b);
  arg_bool->fromString("1");
  BOOST_CHECK_EQUAL(true, val_b);

  arg_list->fromString("20");
  BOOST_CHECK_EQUAL(2, idx);
  arg_list->fromString("15"); // Bad value
  BOOST_CHECK_EQUAL(20, arg_list->value());
  BOOST_CHECK_EQUAL(2, idx);
}

BOOST_FIXTURE_TEST_CASE(ArgumentList_setValue, ArgumentTest)
{
  BOOST_CHECK_EQUAL(10, arg_list->value());
  BOOST_CHECK_EQUAL(1, idx);

  arg_list->setValue(15); // bad value
  BOOST_CHECK_EQUAL(10, arg_list->value());
  BOOST_CHECK_EQUAL(1, idx);

  arg_list->setValue(20);
  BOOST_CHECK_EQUAL(20, arg_list->value());
  BOOST_CHECK_EQUAL(2, idx);

  arg_list->setValue(40);
  BOOST_CHECK_EQUAL(40, arg_list->value());
  BOOST_CHECK_EQUAL(4, idx);
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


//
///* ArgumentValidator */
//
//
//class ArgumentValidatorTest : public testing::Test
//{
//  void SetUp() override
//  {
//    validator = new ArgumentValidator<int>();
//  }
//
//  void TearDown() override
//  {
//    if (validator) delete validator;
//  }
//
//public:
//
//  ArgumentValidator<int> *validator;
//  ArgumentValidator<double> validator_double;
//
//};
//
//TEST_F(ArgumentValidatorTest, Constructor)
//{
//  validator->setRange(0, 150);
//  EXPECT_TRUE(validator->validate(56));
//  EXPECT_FALSE(validator->validate(200));
//
//  validator_double.setRange(3.4, 34.6);
//  EXPECT_TRUE(validator_double.validate(34));
//  EXPECT_FALSE(validator_double.validate(0.));
//}


/* Command Test */

BOOST_AUTO_TEST_SUITE(CommandTestSuite)

struct CommandTest
{
  CommandTest()
    : file(),
      cmd_arg_posix(new Command),
      cmd_arg_posix2(new Command("cmd_name", "cmd description")),
      cmd_arg_posix3(new Command("name", "Lista de inicializadores",{
                                   std::make_shared<ArgumentStringRequired>("input", 'i', "Fichero de entrada", &file)
                                 })),
      cmd_arg_list(new Command)
  {
  }

  ~CommandTest()
  {
    if (cmd_arg_posix) delete cmd_arg_posix;
    if (cmd_arg_posix2) delete cmd_arg_posix2;
    if (cmd_arg_posix3) delete cmd_arg_posix3;
    if (cmd_arg_list) delete cmd_arg_list;
  }
 
  void setup()
  {
    cmd_arg_posix2->push_back(std::make_shared<Argument_<int, true>>("int", 'i', "integer", &val2));
    val3 = 3.46;
    cmd_arg_posix2->push_back(std::make_shared<Argument_<double, false>>("par", 'p', "Parámetro", &val3));
    val_f = 2.23f;
    cmd_arg_posix2->push_back(std::make_shared<Argument_<float, false>>("float", 'f', "Parámetro float", &val_f));
    cmd_arg_posix2->push_back(std::make_shared<Argument_<bool, false>>("bool", 'b', "boolean", &val_b));
    cmd_arg_posix2->push_back(std::make_shared<Argument_<bool, false>>("option", 'o', "Option", &opt));
    //cmd_arg_posix2->push_back(ParameterList({"par_list", "l"}, "Lista de párametros", {"v1", "v2", "v3"}, 1));

    idx = 3;
    options.push_back("OPT0");
    options.push_back("OPT1");
    options.push_back("OPT2");
    options.push_back("OPT3");
    options.push_back("OPT4");

    cmd_arg_list->push_back(std::make_shared<ArgumentList_<std::string, false>>("options", "lista de opciones", options, &idx));
  }

  void teardown()
  {

  }

  std::string file;
  Command *cmd_arg_posix;
  Command *cmd_arg_posix2;
  Command *cmd_arg_posix3;
  Command *cmd_arg_list;
  std::string cmd_help;
  int val2;
  double val3;
  float val_f;
  bool val_b = false;
  bool opt = false;
  

  size_t idx;
  std::vector<std::string> options;

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
  BOOST_CHECK_EQUAL(2, val2);
  BOOST_CHECK(val_b);
  BOOST_CHECK(false == opt);

  std::array<char *, 4> argv_large{const_cast<char *>(""), const_cast<char *>("--int"), const_cast<char *>("2"), const_cast<char *>("-b")};
  BOOST_CHECK(cmd_arg_posix2->parse(static_cast<int>(argv_large.size()), argv_large.data()) == Command::Status::parse_success);
  BOOST_CHECK_EQUAL(2, val2);
  BOOST_CHECK(val_b);
  BOOST_CHECK(false == opt);
}

BOOST_FIXTURE_TEST_CASE(parse_option_short, CommandTest)
{
  std::array<char *, 3> argv{const_cast<char *>(""), const_cast<char *>("-i2"), const_cast<char *>("-b")};
  BOOST_CHECK(cmd_arg_posix2->parse(static_cast<int>(argv.size()), argv.data()) == Command::Status::parse_success);
  BOOST_CHECK_EQUAL(2, val2);
  BOOST_CHECK(val_b);
  BOOST_CHECK(false == opt);
}

BOOST_FIXTURE_TEST_CASE(parse_option_ok_equal, CommandTest)
{
  std::array<char *, 3> argv_large{const_cast<char *>("") , const_cast<char *>("--int=2"), const_cast<char *>("-b")};
  BOOST_CHECK(cmd_arg_posix2->parse(static_cast<int>(argv_large.size()), argv_large.data()) == Command::Status::parse_success);
  BOOST_CHECK_EQUAL(2, val2);
  BOOST_CHECK(val_b);
  BOOST_CHECK(false == opt);
}

BOOST_FIXTURE_TEST_CASE(parseMultipleOptions, CommandTest)
{
  std::array<char *, 2> argv{const_cast<char *>(""), const_cast<char *>("-b")};
  BOOST_CHECK(cmd_arg_posix2->parse(static_cast<int>(argv.size()), argv.data()) == Command::Status::parse_error);
}

BOOST_FIXTURE_TEST_CASE(parse_error, CommandTest)
{
  std::array<char *, 2> argv{const_cast<char *>(""), const_cast<char *>("-b")};
  BOOST_CHECK(cmd_arg_posix2->parse(static_cast<int>(argv.size()), argv.data()) == Command::Status::parse_error);
}

BOOST_FIXTURE_TEST_CASE(parseOptionsParameter, CommandTest)
{
  std::array<char *, 3> argv{const_cast<char *>(""), const_cast<char *>("--options"), const_cast<char *>("OPT3")};
  BOOST_CHECK(cmd_arg_list->parse(static_cast<int>(argv.size()), argv.data()) == Command::Status::parse_success);
  BOOST_CHECK_EQUAL(3, idx);

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

//class CommandListTest : public testing::Test
//{
//public:
//
//  CommandList *cmd_list;
//  CommandList *cmd_list2;
//
//  virtual void SetUp() override
//  {
//    cmd_list = new CommandList;
//
//    std::string file;
//    std::string output;
//    //std::shared_ptr<ArgumentStringRequired> arg = std::make_shared<ArgumentStringRequired>("input", 'i', "Fichero de entrada", &file);
//    cmd_list2 = new CommandList("cmd_list", "Lista de comandos");
//
//    std::shared_ptr<Command> cmd(new Command("cmd1","comando 1", {
//                                               std::make_shared<ArgumentStringRequired>("input", 'i', "Fichero de entrada", &file)
//                                             }));
//    cmd_list2->push_back(cmd);
//
//    std::shared_ptr<Command> cmd2(new Command("cmd2","comando 2", {
//                                                std::make_shared<ArgumentStringRequired>("input", 'i', "Fichero de entrada", &file),
//                                                std::make_shared<ArgumentStringRequired>("output", 'o', "Fichero de salida", &output)
//                                              }));
//    cmd_list2->push_back(cmd2);
//  }
//
//  virtual void TearDown() override
//  {
//    if (cmd_list) delete cmd_list;
//    if (cmd_list2) delete cmd_list2;
//  }
//};
//
//
//TEST_F(CommandListTest, DefaultConstructor)
//{
//  EXPECT_TRUE(cmd_list->name().empty());
//  EXPECT_TRUE(cmd_list->description().empty());
//  EXPECT_STREQ("0.0.0", cmd_list->version().c_str());
//  EXPECT_TRUE(cmd_list->empty());
//  int size = 0;
//  EXPECT_EQ(size, cmd_list->size());
//}
//
//TEST_F(CommandListTest, Constructor)
//{
//  EXPECT_STREQ("cmd_list", cmd_list2->name().c_str());
//  EXPECT_STREQ("Lista de comandos", cmd_list2->description().c_str());
//  EXPECT_STREQ("0.0.0", cmd_list2->version().c_str());
//  EXPECT_FALSE(cmd_list2->empty());
//  EXPECT_EQ(2, cmd_list2->size());
//}
//
//TEST_F(CommandListTest, setGetName)
//{
//  cmd_list->setName("new_name");
//  EXPECT_STREQ("new_name", cmd_list->name().c_str());
//}
//
//TEST_F(CommandListTest, setGetDescription)
//{
//  cmd_list->setDescription("New description");
//  EXPECT_STREQ("New description", cmd_list->description().c_str());
//}
//
//TEST_F(CommandListTest, setGetVersion)
//{
//  cmd_list->setVersion("1.0.0");
//  EXPECT_STREQ("1.0.0", cmd_list->version().c_str());
//}
//
////TEST_F(CommandListTest, parseHelp)
////{
////  std::array<char const*, 2> argv{"" , "-h"};
////  EXPECT_TRUE(cmd_list2->parse(argv.size(), argv.data()) == CommandList::Status::SHOW_HELP);
////
////  std::array<char const*, 2> argv2{"" , "--help"};
////  EXPECT_TRUE(cmd_list2->parse(argv2.size(), argv2.data()) == CommandList::Status::SHOW_HELP);
////}
////
////TEST_F(CommandListTest, parseVersion)
////{
////  std::array<char const*, 2> argv2{"" , "--version"};
////  EXPECT_TRUE(cmd_list2->parse(argv2.size(), argv2.data()) == CommandList::Status::SHOW_VERSION);
////}
////
////TEST_F(CommandListTest, parseLicence)
////{
////  std::array<char const*, 2> argv2{"" , "--licence"};
////  EXPECT_TRUE(cmd_list2->parse(argv2.size(), argv2.data()) == CommandList::Status::SHOW_LICENCE);
////}
//
//
//TL_DEFAULT_WARNINGS
//
//#endif
//
//
//

