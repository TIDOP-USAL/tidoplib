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
 * License along with Foobar. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/
 
#define BOOST_TEST_MODULE Tidop Console test
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <tidop/core/console.h>
#include <tidop/core/log.h>

using namespace tl;


/* ConsoleTestSuite */

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