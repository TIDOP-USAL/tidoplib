#include <gtest/gtest.h>

#include <tidop/core/console.h>
#include <tidop/core/defs.h>

#include <memory>
#include <array>

using namespace TL;

/// Google test con visual studio da algunas excepciones que a priori no tienen mucho sentido.
/// Si se ejecuta el mismo código por separado no da ningén error. En Linux el test corre sin problemas.
#ifndef _MSC_VER

TL_SUPPRESS_WARNINGS

/* Datos para los test */



TEST(ConsoleTest, Constructor)
{
  Console &console = Console::getInstance();
  EnumFlags<MessageLevel> flag = console.messageLevel();
  EXPECT_TRUE(flag.isActive(MessageLevel::MSG_ERROR));
  EXPECT_FALSE(flag.isActive(MessageLevel::MSG_INFO));
  EXPECT_FALSE(flag.isActive(MessageLevel::MSG_WARNING));
  EXPECT_FALSE(flag.isActive(MessageLevel::MSG_DEBUG));

  console.setLogLevel(MessageLevel::MSG_VERBOSE);
  flag = console.messageLevel();
  EXPECT_TRUE(flag.isActive(MessageLevel::MSG_ERROR));
  EXPECT_TRUE(flag.isActive(MessageLevel::MSG_INFO));
  EXPECT_TRUE(flag.isActive(MessageLevel::MSG_WARNING));
  EXPECT_TRUE(flag.isActive(MessageLevel::MSG_VERBOSE));
  EXPECT_FALSE(flag.isActive(MessageLevel::MSG_DEBUG));
}



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

    opt = std::make_shared<ArgumentBooleanOptional>("bool", 'b', "boolean", &val_b);

    idx = 1;
    list.push_back(0);
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.push_back(4);
    arg_list = std::make_shared<ArgumentList_<int, false>>("list", "lista de argumentos", list, &idx);

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

  bool val_b;
  std::shared_ptr<Argument> opt;

  std::vector<int> list;
  size_t idx;
  std::shared_ptr<Argument> arg_list;
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
  EXPECT_EQ(false, opt->isRequired());
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

TEST(ArgumentList, setGetValue)
{
  size_t idx = 1;
  std::vector<int> list;
  list.push_back(0);
  list.push_back(10);
  list.push_back(20);
  list.push_back(30);
  list.push_back(40);
  ArgumentList_<int, true> arg_list("list", "lista de argumentos", list, &idx);
  EXPECT_EQ(10, arg_list.value());

  arg_list.setValue(15);
  EXPECT_EQ(10, arg_list.value());

  arg_list.setValue(20);
  EXPECT_EQ(20, arg_list.value());

  arg_list.setValue(50);

}

TEST(ArgumentList, isValid)
{
  size_t idx = 1;
  std::vector<int> list;
  list.push_back(0);
  list.push_back(10);
  list.push_back(20);
  list.push_back(30);
  list.push_back(40);
  ArgumentList_<int, true> arg_list("list", "lista de argumentos", list, &idx);
  EXPECT_EQ(10, arg_list.value());

  arg_list.setValue(15);
  EXPECT_EQ(10, arg_list.value());
  EXPECT_FALSE(arg_list.isValid());

  arg_list.setValue(20);
  EXPECT_EQ(20, arg_list.value());
  EXPECT_TRUE(arg_list.isValid());

  arg_list.setValue(50);
  EXPECT_FALSE(arg_list.isValid());

}

TEST_F(ArgumentTest, typeName)
{
  EXPECT_EQ("int", arg_int->typeName());
  EXPECT_EQ("double", arg_double->typeName());
  EXPECT_EQ("float", arg_float->typeName());
  EXPECT_EQ("std::string", arg_string->typeName());
  EXPECT_EQ("bool", opt->typeName());
  EXPECT_EQ("int", arg_list->typeName());
}

TEST_F(ArgumentTest, fromString)
{
  arg_int->fromString("2");
  EXPECT_EQ(2, val_i);

  arg_double->fromString("2.3");
  EXPECT_NEAR(2.3, val_d, 0.1);

  arg_float->fromString("0.3536");
  EXPECT_FLOAT_EQ(0.3536f, val_f);

  arg_list->fromString("4");
  EXPECT_EQ(4, list[idx]);
}

TEST_F(ArgumentTest, toString)
{
  arg_int->fromString("2");
  EXPECT_EQ("2", arg_int->toString());
}


/* ArgumentValidator */


class ArgumentValidatorTest : public testing::Test
{
  void SetUp() override
  {
    validator = new ArgumentValidator<int>();
  }

  void TearDown() override
  {
    if (validator) delete validator;
  }

public:

  ArgumentValidator<int> *validator;
  ArgumentValidator<double> validator_double;

};

TEST_F(ArgumentValidatorTest, Constructor)
{
  validator->setRange(0, 150);
  EXPECT_TRUE(validator->validate(56));
  EXPECT_FALSE(validator->validate(200));

  validator_double.setRange(3.4, 34.6);
  EXPECT_TRUE(validator_double.validate(34));
  EXPECT_FALSE(validator_double.validate(0.));
}

/* Command Test */

class CommandTest : public testing::Test
{
public:

  Command *cmd_arg_posix;
  Command *cmd_arg_posix2;
  Command *cmd_arg_posix3;
  Command *cmd_arg_list;
  std::string cmd_help;
  int val2;
  double val3;
  float val_f;
  bool val_b;
  bool opt;
  std::string file;

  size_t idx;
  std::vector<std::string> options;

  virtual void SetUp() override
  {

    cmd_arg_posix = new Command;
    cmd_arg_posix2 = new Command("cmd_name", "cmd description");
    cmd_arg_posix2->push_back(std::make_shared<Argument_<int, true>>("int", 'i', "integer", &val2));
    val3 = 3.46;
    cmd_arg_posix2->push_back(std::make_shared<Argument_<double, false>>("par", 'p', "Parámetro", &val3));
    val_f = 2.23f;
    cmd_arg_posix2->push_back(std::make_shared<Argument_<float, false>>("float", 'f', "Parámetro float", &val_f));
    cmd_arg_posix2->push_back(std::make_shared<Argument_<bool, false>>("bool", 'b', "boolean", &val_b));
    cmd_arg_posix2->push_back(std::make_shared<Argument_<bool, false>>("option", 'o', "Option", &opt));
    //cmd_arg_posix2->push_back(ParameterList({"par_list", "l"}, "Lista de párametros", {"v1", "v2", "v3"}, 1));


    cmd_arg_posix3 = new Command("name", "Lista de inicializadores",{
                                   std::make_shared<ArgumentStringRequired>("input", 'i', "Fichero de entrada", &file)
                                 });
    cmd_arg_list = new Command;
    idx = 3;
    options.push_back("OPT0");
    options.push_back("OPT1");
    options.push_back("OPT2");
    options.push_back("OPT3");
    options.push_back("OPT4");

    cmd_arg_list->push_back(std::make_shared<ArgumentList_<std::string, false>>("options", "lista de opciones", options, &idx));
  }

  virtual void TearDown() override
  {
    if (cmd_arg_posix) delete cmd_arg_posix;
    if (cmd_arg_posix2) delete cmd_arg_posix2;
    if (cmd_arg_posix3) delete cmd_arg_posix3;
    if (cmd_arg_list) delete cmd_arg_list;
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
  EXPECT_EQ(2, val2);
  EXPECT_TRUE(val_b);
  EXPECT_FALSE(opt);

  std::array<char const*, 4> argv_large{"" , "--int", "2", "-b"};
  EXPECT_TRUE(cmd_arg_posix2->parse(argv_large.size(), argv_large.data()) == Command::Status::PARSE_SUCCESS);
  EXPECT_EQ(2, val2);
  EXPECT_TRUE(val_b);
  EXPECT_FALSE(opt);
}

TEST_F(CommandTest, parse_option_short)
{
  std::array<char const*, 3> argv{"" , "-i2", "-b"};
  EXPECT_TRUE(cmd_arg_posix2->parse(argv.size(), argv.data()) == Command::Status::PARSE_SUCCESS);
  EXPECT_EQ(2, val2);
  EXPECT_TRUE(val_b);
  EXPECT_FALSE(opt);
}

TEST_F(CommandTest, parse_option_ok_equal)
{
  std::array<char const*, 3> argv_large{"" , "--int=2", "-b"};
  EXPECT_TRUE(cmd_arg_posix2->parse(argv_large.size(), argv_large.data()) == Command::Status::PARSE_SUCCESS);
  EXPECT_EQ(2, val2);
  EXPECT_TRUE(val_b);
  EXPECT_FALSE(opt);
}

TEST_F(CommandTest, parseMultipleOptions)
{
  std::array<char const*, 2> argv{"" , "-b"};
  EXPECT_TRUE(cmd_arg_posix2->parse(argv.size(), argv.data()) == Command::Status::PARSE_ERROR);
}

TEST_F(CommandTest, parseERROR)
{
  std::array<char const*, 2> argv{"" , "-b"};
  EXPECT_TRUE(cmd_arg_posix2->parse(argv.size(), argv.data()) == Command::Status::PARSE_ERROR);
}


TEST_F(CommandTest, parseOptionsParameter)
{
  std::array<char const*, 3> argv{"" , "--options", "OPT3"};
  EXPECT_TRUE(cmd_arg_list->parse(argv.size(), argv.data()) == Command::Status::PARSE_SUCCESS);
  EXPECT_EQ(3, idx);

  std::array<char const*, 3> argv2{"" , "--options", "OPT_ERROR"};
  EXPECT_TRUE(cmd_arg_list->parse(argv2.size(), argv2.data()) == Command::Status::PARSE_ERROR);
}

TEST_F(CommandTest, size)
{
  EXPECT_EQ(0, cmd_arg_posix->size());
  EXPECT_EQ(5, cmd_arg_posix2->size());
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

TEST_F(CommandTest, parseTextWithHyphen)
{
  std::array<char const*, 3> argv{"" , "--input", "sdfsd-sdfsdf"};
  EXPECT_TRUE(cmd_arg_posix3->parse(argv.size(), argv.data()) == Command::Status::PARSE_SUCCESS);
}


TL_DEFAULT_WARNINGS

#endif



