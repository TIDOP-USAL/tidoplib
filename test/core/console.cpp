#include <gtest/gtest.h>

#include "core/console.h"
#include "core/defs.h"

#include <memory>
#include <array>

using namespace TL;


TL_SUPPRESS_WARNINGS

/* Datos para los test */

class ConsoleTest : public testing::Test
{
public:

  virtual void SetUp()
  {

  }

  virtual void TearDown()
  {
  }

  Console console;
};

//TEST_F(ConsoleTest, Constructor)
//{

//}

/* ArgumentTest */

class ArgumentTest : public testing::Test
{
public:

  virtual void SetUp()
  {
    arg_int = std::make_shared<Argument_<int, true>>("int", 'i', "integer", &val_i);
    val_d = 2.5;
    arg_double = std::make_shared<Argument_<double, false>>("double", 'd', "double", &val_d);
    val_f = 0.567f;
    arg_float = std::make_shared<Argument_<float, false>>("float", 'f', "coma flotante", &val_f);

    arg_string = std::make_shared<ArgumentStringOptional>("string", 's', "Cadena de texto", &val_string);
////    arg2 = new Parameter({"par","p"}, "Parámetro");
////    arg3 = new ParameterList({"par_list", "l"}, "Lista de párametros", {"v1", "v2", "v3"}, 1);
  }

  virtual void TearDown()
  {

  }

  std::shared_ptr<Argument> arg1;

  int val_i;
  std::shared_ptr<Argument> arg_int;

  double val_d;
  std::shared_ptr<Argument> arg_double;

  float val_f;
  std::shared_ptr<Argument> arg_float;
  std::string val_string;
  std::shared_ptr<Argument> arg_string;

};


TEST_F(ArgumentTest, Constructor)
{
  EXPECT_EQ("int", arg_int->name());
  EXPECT_EQ("integer", arg_int->description());
  EXPECT_EQ('i', arg_int->shortName());
}

TEST_F(ArgumentTest, setGetName)
{
  arg_int->setName("new");
  EXPECT_EQ("new", arg_int->name());
}

TEST_F(ArgumentTest, setGetDescription)
{
  arg_int->setDescription("new description");
  EXPECT_EQ("new description", arg_int->description());
}

TEST_F(ArgumentTest, setGetShortName)
{
  arg_int->setShortName('n');
  EXPECT_EQ('n', arg_int->shortName());
}

TEST_F(ArgumentTest, isRequired)
{
  EXPECT_EQ(true, arg_int->isRequired());
  EXPECT_EQ(false, arg_double->isRequired());
  EXPECT_EQ(false, arg_string->isRequired());
}

TEST(ArgumentInt, setGetValue)
{
  int value = 10;
  Argument_<int, true> arg_int("opt", 'o', "opción", &value);
  EXPECT_EQ(10, arg_int.value());

  arg_int.setValue(15);
  EXPECT_EQ(15, arg_int.value());
}

TEST_F(ArgumentTest, typeName)
{
  EXPECT_EQ("int", arg_int->typeName());
  EXPECT_EQ("double", arg_double->typeName());
  EXPECT_EQ("float", arg_float->typeName());
  EXPECT_EQ("std::string", arg_string->typeName());
}

TEST_F(ArgumentTest, fromString)
{
  arg_int->fromString("2");
  EXPECT_EQ(2, val_i);

  arg_double->fromString("2.3");
  EXPECT_NEAR(2.3, val_d, 0.1);

  arg_float->fromString("0.3536");
  EXPECT_NEAR(0.3536, val_f, 0.001);
}

TEST_F(ArgumentTest, toString)
{
  arg_int->fromString("2");
  EXPECT_EQ("2", arg_int->toString());
}

class CommandTest : public testing::Test
{
public:

  Command *cmd_arg_posix;
  Command *cmd_arg_posix2;
  std::string cmd_help;
  int val2;
  double val3;
  float val_f;
  bool val_b;

  virtual void SetUp()
  {
    cmd_help = "-h"; // Muestra la ayuda
    cmd_arg_posix = new Command;
    cmd_arg_posix2 = new Command("cmd_name", "cmd description");
    cmd_arg_posix2->push_back(std::make_shared<Argument_<int, true>>("int", 'i', "integer", &val2));
    val3 = 3.46;
    cmd_arg_posix2->push_back(std::make_shared<Argument_<double, false>>("par", 'p', "Parámetro", &val3));
    val_f = 2.23f;
    cmd_arg_posix2->push_back(std::make_shared<Argument_<float, false>>("float", 'f', "Parámetro float", &val_f));
    cmd_arg_posix2->push_back(std::make_shared<Argument_<bool, false>>("bool", 'b', "boolean", &val_b));
    //cmd_arg_posix2->push_back(ParameterList({"par_list", "l"}, "Lista de párametros", {"v1", "v2", "v3"}, 1));
  }

  virtual void TearDown()
  {
    if (cmd_arg_posix) delete cmd_arg_posix;
    if (cmd_arg_posix2) delete cmd_arg_posix2;
  }
};

TEST_F(CommandTest, DefaultConstructor)
{
  EXPECT_TRUE(cmd_arg_posix->name().empty());
}

TEST_F(CommandTest, Constructor)
{
  EXPECT_STREQ("cmd_name", cmd_arg_posix2->name().c_str());
  EXPECT_STREQ("cmd description", cmd_arg_posix2->description().c_str());
}

TEST_F(CommandTest, setGetName)
{
  cmd_arg_posix2->setName("new_name");
  EXPECT_STREQ("new_name", cmd_arg_posix2->name().c_str());
}

TEST_F(CommandTest, setGetDescription)
{
  cmd_arg_posix2->setDescription("New description");
  EXPECT_STREQ("New description", cmd_arg_posix2->description().c_str());
}

TEST_F(CommandTest, parseHelp)
{
  std::array<char const*, 2> argv{"" , "-h"};
  EXPECT_TRUE(cmd_arg_posix2->parse(argv.size(), argv.data()) == Command::Status::SHOW_HELP);

  std::array<char const*, 2> argv2{"" , "--help"};
  EXPECT_TRUE(cmd_arg_posix2->parse(argv2.size(), argv2.data()) == Command::Status::SHOW_HELP);
}

TEST_F(CommandTest, parseVersion)
{
  std::array<char const*, 2> argv2{"" , "--version"};
  EXPECT_TRUE(cmd_arg_posix2->parse(argv2.size(), argv2.data()) == Command::Status::SHOW_VERSION);
}

TEST_F(CommandTest, parseLicence)
{
  std::array<char const*, 2> argv2{"" , "--licence"};
  EXPECT_TRUE(cmd_arg_posix2->parse(argv2.size(), argv2.data()) == Command::Status::SHOW_LICENCE);
}


TEST_F(CommandTest, parse_option_ok)
{
  std::array<char const*, 4> argv{"" , "-i", "2", "-b"};
  EXPECT_TRUE(cmd_arg_posix2->parse(argv.size(), argv.data()) == Command::Status::PARSE_SUCCESS);

  std::array<char const*, 4> argv_large{"" , "--int", "2", "-b"};
  EXPECT_TRUE(cmd_arg_posix2->parse(argv_large.size(), argv_large.data()) == Command::Status::PARSE_SUCCESS);

}

//TEST_F(CommandTest, parse_option_ok_equal)
//{
//  std::array<char const*, 3> argv{"" , "-i=2", "-b"};
//  EXPECT_TRUE(cmd_arg_posix2->parse(argv.size(), argv.data()) == Command::Status::PARSE_SUCCESS);
//}

TEST_F(CommandTest, parseERROR)
{
  std::array<char const*, 2> argv{"" , "-b"};
  EXPECT_TRUE(cmd_arg_posix2->parse(argv.size(), argv.data()) == Command::Status::PARSE_ERROR);
}


TEST_F(CommandTest, size)
{
  EXPECT_EQ(0, cmd_arg_posix->size());
  EXPECT_EQ(4, cmd_arg_posix2->size());
}

TEST_F(CommandTest, clear)
{
  cmd_arg_posix2->clear();
  EXPECT_EQ(0, cmd_arg_posix2->size());
}

TEST_F(CommandTest, empty)
{
  EXPECT_TRUE(cmd_arg_posix->empty());
  EXPECT_FALSE(cmd_arg_posix2->empty());
}

TL_DEFAULT_WARNINGS





