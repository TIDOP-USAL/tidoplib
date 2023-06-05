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
 
#define BOOST_TEST_MODULE Tidop messages test
#include <boost/test/unit_test.hpp>
#include <tidop/core/messages.h>
#include "tidop/core/app.h"
#include "tidop/core/msg/handler.h"
#include "tidop/core/msg/stream.h"
#include "tidop/core/console.h"
#include "tidop/core/chrono.h"
#include <fstream>
#include <iostream>
#include <vector>


using namespace tl;


BOOST_AUTO_TEST_SUITE(MessageTestSuite)


struct MessageTest
{
  MessageTest()
  {
  }

  ~MessageTest()
  {
  }

  void setup()
  {

  }

  void teardown()
  {

  }

};

// Info:    Impresión por consola [Time: 0.107769 seconds] (chrono.cpp:114, tl::Chrono::stop)
// Info:    Impresión por consola personalizada [Time: 2.289432 seconds] (chrono.cpp:114, tl::Chrono::stop)
// Info:    Impresión con macros [Time: 1.287309 seconds] (chrono.cpp:114, tl::Chrono::stop)
BOOST_FIXTURE_TEST_CASE(old_console, MessageTest)
{
    Console &console = App::console();
    console.setMessageLevel(MessageLevel::msg_error | MessageLevel::msg_warning | MessageLevel::msg_info | MessageLevel::msg_debug);
    App::messageManager().addListener(&console);

    Chrono chrono("Impresión por consola");
    chrono.run();

    for (size_t i = 0; i < 100; i++) {
      console.printMessage("Impresión por consola");
    }

    chrono.stop();

    //chrono.reset();
    //chrono.setMessage("Impresión por consola personalizada");
    //chrono.run();

    //for (size_t i = 0; i < 100; i++) {
    //  
    //    // Mensage de debug
    //    console.setConsoleForegroundColor(Console::Color::white, Console::Intensity::normal);
    //    console.printMessage("Mensaje de depuración por consola");
    //    console.reset();
    //    
    //    // Mensage de información
    //    console.setConsoleForegroundColor(Console::Color::white, Console::Intensity::bright);
    //    console.printMessage("Mensaje de información por consola");
    //    console.reset();

    //    // Mensage de warning
    //    console.setConsoleForegroundColor(Console::Color::magenta, Console::Intensity::bright);
    //    console.printMessage("Mensaje de warning por consola");
    //    console.reset();

    //    // Mensage de error
    //    console.printErrorMessage("Mensaje de error por consola");
    //}
    //
    //chrono.stop();

    chrono.reset();
    chrono.setMessage("Impresión con macros");
    chrono.run();

    for (size_t i = 0; i < 100; i++) {
      
        // Mensage de debug
        msgDebug("Mensaje de depuración por consola: %i, %lf, %f", 1, 2.3, 3.5f);
        
        // Mensage de información
        msgInfo("Mensaje de información por consola %i, %lf, %f", 1, 2.3, 3.5f);

        // Mensage de warning
        msgWarning("Mensaje de warning por consola %i, %lf, %f", 1, 2.3, 3.5f);

        // Mensage de error
        msgError("Mensaje de error por consola %i, %lf, %f", 1, 2.3, 3.5f);
    }

    chrono.stop();
}

//// Info:    Impresión por consola [Time: 0.380867 seconds] (chrono.cpp:114, tl::Chrono::stop)
//// Info:    Console 2 [Time: 2.454635 seconds] (chrono.cpp:114, tl::Chrono::stop)
//// Info:    Console 2 [Time: 2.386155 seconds] (chrono.cpp:114, tl::Chrono::stop)
//// Info:    App::message() [Time: 0.961311 seconds] (chrono.cpp:114, tl::Chrono::stop)
BOOST_FIXTURE_TEST_CASE(default_constructor, MessageTest)
{

    Console2 &console = App::console2();
    App::messageHandler().subscribe(console.rdbuf());

    std::ofstream log("D:\\dev\\sources\\tidoplib\\test\\core\\messages\\log.txt", std::ofstream::app);
    App::messageHandler().subscribe(log.rdbuf());

    Chrono chrono("Impresión por consola");
    chrono.run();

    for (size_t i = 0; i < 100; i++) {
      console << "Impresión por consola" << std::endl;
    }

    chrono.stop();

    //chrono.reset();
    //chrono.setMessage("Console 2");
    //chrono.run();

    //for (size_t i = 0; i < 100; i++) {
    //  console << Level::debug << "Mensaje de depuración por consola" << std::endl;
    //  console << Level::info << "Mensaje de información por consola" << std::endl;
    //  console << Level::warning << "Mensaje de warning por consola" << std::endl;
    //  console << Level::error << "Mensaje de error por consola" << std::endl;
    //}

    //chrono.stop();

    chrono.reset();
    chrono.setMessage("Console 2");
    chrono.run();

    for (size_t i = 0; i < 100; i++) {
      Console2::debug() << "Mensaje de depuración por consola" << 1 << " " << 2.3 << " " << 3.5f  << std::endl;
      Console2::info() << "Mensaje de información por consola" << 1 << " " << 2.3 << " " << 3.5f  << std::endl;
      Console2::warning() << "Mensaje de warning por consola" << 1 << " " << 2.3 << " " << 3.5f  << std::endl;
      Console2::error() << "Mensaje de error por consola" << 1 << " " << 2.3 << " " << 3.5f  << std::endl;
    }

    chrono.stop();

    chrono.setMessage("Console 2 std::format");
    chrono.run();

    for (size_t i = 0; i < 100; i++) {
      Console2::debug("Mensaje de depuración por consola {}, {}, {}", 1, 2.3, 3.5f);
      Console2::info("Mensaje de información por consola {}, {}, {}", 1, 2.3, 3.5f);
      Console2::warning("Mensaje de warning por consola {}, {}, {}", 1, 2.3, 3.5f);
      Console2::error("Mensaje de error por consola {}, {}, {}", 1, 2.3, 3.5f);
    }

    chrono.stop();

    chrono.reset();
    chrono.setMessage("App::message()");
    chrono.run();

    for (size_t i = 0; i < 100; i++) {
      App::message() << Level::debug << "Mensaje de depuración por consola: " << 1 << " " << 2.3 << " " << 3.5f << std::endl;
      App::message() << Level::info << "Mensaje de información por consola: " << 1 << " " << 2.3 << " " << 3.5f  << std::endl;
      App::message() << Level::warning << "Mensaje de warning por consola: " << 1 << " " << 2.3 << " " << 3.5f  << std::endl;
      App::message() << Level::error << "Mensaje de error por consola: " << 1 << " " << 2.3 << " " << 3.5f  << std::endl;
    }

    chrono.stop();
}

#if CPP_VERSION >= 20

#include <format>

BOOST_FIXTURE_TEST_CASE(format_messages, MessageTest)
{
    Console &console = App::console();
    console.setMessageLevel(MessageLevel::msg_error | MessageLevel::msg_warning | MessageLevel::msg_info | MessageLevel::msg_debug);
    App::messageManager().addListener(&console);

    Chrono chrono("Impresión por consola std::format");
    chrono.run();

    for (size_t i = 0; i < 100; i++) {
      std::cout << std::format("Impresión por consola") << std::endl;
    }
    chrono.stop();

    chrono.reset();
    chrono.setMessage("Impresión con mas parametros");
    chrono.run();

    for (size_t i = 0; i < 100; i++) {
      
        // Mensage de debug
        std::cout << std::format("{}Mensaje de información por consola {}, {}, {}", "Debug:   ", 1, 2.3, 3.5f) << std::endl;
        
        // Mensage de información
        std::cout << std::format("{}Mensaje de información por consola {}, {}, {}", "Info:    ", 1, 2.3, 3.5f) << std::endl;

        // Mensage de warning
        console.setConsoleForegroundColor(Console::Color::magenta, Console::Intensity::normal);
        std::cout << std::format("{}Mensaje de warning por consola {}, {}, {}", "Warning: ", 1, 2.3, 3.5f) << std::endl;
        console.reset();

        // Mensage de error
        console.setConsoleForegroundColor(Console::Color::red, Console::Intensity::normal);
        std::cout << std::format("{}Mensaje de error por consola {}, {}, {}", "Error:   ", 1, 2.3, 3.5f) << std::endl;
        console.reset();

    }

    chrono.stop();
}

#endif



//class Message3
//{
//
//private:
//
//    std::vector<std::ostream *> buffer;
//
//
//
//    Message3()
//    {
//
//    }
//
//public:
//
//    static Message3 &instance()
//    {
//      static Message3 message3;
//      return message3;
//    }
//
//    ~Message3()
//    {
//
//    }
//
//    void subscribe(std::ostream *sb)
//    {
//      buffer.push_back(sb);
//    }
//
//    template<typename... Args>
//    static void debug(std::format_string<Args...> s, Args&&... args)
//    {
//        std::stringstream s;
//        s << "Debug:   " << std::vformat(s.get(), std::make_format_args(args...));
//        for (size_t i = 0; i < buffer.size(); i++) {
//          buffer. << s << std::endl;
//        }
//    }
//
//    template<typename... Args>
//    static void info(std::format_string<Args...> s, Args&&... args)
//    {
//        std::stringstream s;
//        s << "Info:    " << std::vformat(s.get(), std::make_format_args(args...));
//        for (size_t i = 0; i < buffer.size(); i++) {
//          buffer << s << std::endl;
//        }
//    }
//
//    template<typename... Args>
//    static void warning(std::format_string<Args...> s, Args&&... args)
//    {
//        std::stringstream s;
//        s << "Warning: " << std::vformat(s.get(), std::make_format_args(args...));
//        for (size_t i = 0; i < buffer.size(); i++) {
//          buffer << s << std::endl;
//        }
//    }
//
//    template<typename... Args>
//    static void error(std::format_string<Args...> s, Args&&... args)
//    {
//        std::stringstream s;
//        s << "Error:   " << std::vformat(s.get(), std::make_format_args(args...));
//        for (size_t i = 0; i < buffer.size(); i++) {
//          buffer << s << std::endl;
//        }
//    }
//
//private:
//
//};
//
//
//BOOST_FIXTURE_TEST_CASE(message_3, MessageTest)
//{
//
//    Console2 &console = App::console2();
//    Message3::instance().subscribe(&console.stream());
//
//    std::ofstream log("D:\\dev\\sources\\tidoplib\\test\\core\\messages\\log.txt", std::ofstream::app);
//    App::messageHandler().subscribe(&log);
//
//    Chrono chrono("Message 3");
//    chrono.run();
//
//    //for (size_t i = 0; i < 100; i++) {
//    //  Console2::debug() << "Mensaje de depuración por consola" << 1 << " " << 2.3 << " " << 3.5f  << std::endl;
//    //  Console2::info() << "Mensaje de información por consola" << 1 << " " << 2.3 << " " << 3.5f  << std::endl;
//    //  Console2::warning() << "Mensaje de warning por consola" << 1 << " " << 2.3 << " " << 3.5f  << std::endl;
//    //  Console2::error() << "Mensaje de error por consola" << 1 << " " << 2.3 << " " << 3.5f  << std::endl;
//    //}
//
//    //chrono.stop();
//
//    //chrono.setMessage("Console 2 std::format");
//    //chrono.run();
//
//    for (size_t i = 0; i < 100; i++) {
//      Message3::debug("Mensaje de depuración por consola {}, {}, {}", 1, 2.3, 3.5f);
//      Message3::info("Mensaje de información por consola {}, {}, {}", 1, 2.3, 3.5f);
//      Message3::warning("Mensaje de warning por consola {}, {}, {}", 1, 2.3, 3.5f);
//      Message3::error("Mensaje de error por consola {}, {}, {}", 1, 2.3, 3.5f);
//    }
//
//    chrono.stop();
//
//    //chrono.reset();
//    //chrono.setMessage("App::message()");
//    //chrono.run();
//
//    //for (size_t i = 0; i < 100; i++) {
//    //  App::message() << Level::debug << "Mensaje de depuración por consola: " << 1 << " " << 2.3 << " " << 3.5f << std::endl;
//    //  App::message() << Level::info << "Mensaje de información por consola: " << 1 << " " << 2.3 << " " << 3.5f  << std::endl;
//    //  App::message() << Level::warning << "Mensaje de warning por consola: " << 1 << " " << 2.3 << " " << 3.5f  << std::endl;
//    //  App::message() << Level::error << "Mensaje de error por consola: " << 1 << " " << 2.3 << " " << 3.5f  << std::endl;
//    //}
//
//    //chrono.stop();
//}


BOOST_AUTO_TEST_SUITE_END()
