#define BOOST_TEST_MODULE Tidop Console test
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <tidop/core/console.h>

using namespace tl;


/* ArgumentTest */

BOOST_AUTO_TEST_SUITE(ConsoleTestSuite)

/// https://stackoverflow.com/questions/5405016/can-i-check-my-programs-output-with-boost-test

struct cout_redirect
{
  cout_redirect(std::streambuf *new_buffer)
    : old(std::cout.rdbuf(new_buffer))
  {
  }

  ~cout_redirect()
  {
    std::cout.rdbuf(old);
  }

private:
  std::streambuf *old;
};


BOOST_AUTO_TEST_CASE(default_constructor)
{
  Console &console = Console::instance();
  EnumFlags<MessageLevel> message_level = console.messageLevel();
  BOOST_CHECK_EQUAL(true, message_level.isActive(MessageLevel::msg_error));
}

BOOST_AUTO_TEST_CASE(messageLevel)
{
  Console &console = Console::instance();
  console.setMessageLevel(MessageLevel::msg_debug);
  EnumFlags<MessageLevel> message_level = console.messageLevel();
  BOOST_CHECK_EQUAL(true, message_level.isActive(MessageLevel::msg_debug));
}

BOOST_AUTO_TEST_CASE(printMessage)
{
  Console &console = Console::instance();

  boost::test_tools::output_test_stream output;
  {
    cout_redirect guard(output.rdbuf());

    console.printMessage("Test");
  }

  BOOST_CHECK( output.is_equal( "\rTest\n" ) );
}


BOOST_AUTO_TEST_SUITE_END()


