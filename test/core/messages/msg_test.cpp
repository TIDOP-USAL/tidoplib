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
#include "tidop/core/console.h"
#include "tidop/core/log.h"
#include "tidop/core/chrono.h"
#include "tidop/core/msg/message.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <thread>

using namespace tl;


BOOST_AUTO_TEST_SUITE(MessageTestSuite)


struct MessageTest
{
  MessageTest()
  {
    //Console &console = App::console();
    //console.setMessageLevel(MessageLevel::msg_error | MessageLevel::msg_warning | MessageLevel::msg_info | MessageLevel::msg_debug);
    //App::messageManager().addListener(&console);
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
//BOOST_FIXTURE_TEST_CASE(old_console, MessageTest)
//{
//    Console &console = App::console();
//    console.setMessageLevel(MessageLevel::msg_error | MessageLevel::msg_warning | MessageLevel::msg_info | MessageLevel::msg_debug);
//    App::messageManager().addListener(&console);
//
//    Chrono chrono("Impresión por consola");
//    //chrono.run();
//
//    //for (size_t i = 0; i < 100; i++) {
//    //  console.printMessage("Impresión por consola");
//    //}
//
//    //chrono.stop();
//
//    //chrono.reset();
//    //chrono.setMessage("Impresión por consola personalizada");
//    //chrono.run();
//
//    //for (size_t i = 0; i < 100; i++) {
//    //  
//    //    // Mensage de debug
//    //    console.setConsoleForegroundColor(Console::Color::white, Console::Intensity::normal);
//    //    console.printMessage("Mensaje de depuración por consola");
//    //    console.reset();
//    //    
//    //    // Mensage de información
//    //    console.setConsoleForegroundColor(Console::Color::white, Console::Intensity::bright);
//    //    console.printMessage("Mensaje de información por consola");
//    //    console.reset();
//
//    //    // Mensage de warning
//    //    console.setConsoleForegroundColor(Console::Color::magenta, Console::Intensity::bright);
//    //    console.printMessage("Mensaje de warning por consola");
//    //    console.reset();
//
//    //    // Mensage de error
//    //    console.printErrorMessage("Mensaje de error por consola");
//    //}
//    //
//    //chrono.stop();
//
//    //chrono.reset();
//    chrono.setMessage("Impresión con macros");
//    chrono.run();
//
//    for (size_t i = 0; i < 100; i++) {
//      
//        // Mensage de debug
//        msgDebug("Mensaje de depuración por consola: %i, %lf, %f", 1, 2.3, 3.5f);
//        
//        // Mensage de información
//        msgInfo("Mensaje de información por consola %i, %lf, %f", 1, 2.3, 3.5f);
//
//        // Mensage de warning
//        msgWarning("Mensaje de warning por consola %i, %lf, %f", 1, 2.3, 3.5f);
//
//        // Mensage de error
//        msgError("Mensaje de error por consola %i, %lf, %f", 1, 2.3, 3.5f);
//    }
//
//    chrono.stop();
//}

//// Info:    Impresión por consola [Time: 0.380867 seconds] (chrono.cpp:114, tl::Chrono::stop)
//// Info:    Console 2 [Time: 2.454635 seconds] (chrono.cpp:114, tl::Chrono::stop)
//// Info:    Console 2 [Time: 2.386155 seconds] (chrono.cpp:114, tl::Chrono::stop)
//// Info:    App::message() [Time: 0.961311 seconds] (chrono.cpp:114, tl::Chrono::stop)
BOOST_FIXTURE_TEST_CASE(default_constructor, MessageTest)
{

    //Console &console = App::console();
    //App::messageHandler().subscribe(console.rdbuf());

    //std::ofstream log("D:\\dev\\sources\\tidoplib\\test\\core\\messages\\log.txt", std::ofstream::app);
    //App::messageHandler().subscribe(log.rdbuf());

    //Chrono chrono("Impresión por consola");
    //chrono.run();

    //for (size_t i = 0; i < 100; i++) {
    //  console << "Impresión por consola" << std::endl;
    //}

    //chrono.stop();

    //chrono.reset();
    //chrono.setMessage("Console 2");
    //chrono.run();

    //for (size_t i = 0; i < 100; i++) {
    //  console << MessageLevel::debug << "Mensaje de depuración por consola" << std::endl;
    //  console << MessageLevel::info << "Mensaje de información por consola" << std::endl;
    //  console << MessageLevel::warning << "Mensaje de warning por consola" << std::endl;
    //  console << MessageLevel::error << "Mensaje de error por consola" << std::endl;
    //}

    //chrono.stop();

    //chrono.reset();
    //chrono.setMessage("Console 2");
    //chrono.run();

    //for (size_t i = 0; i < 100; i++) {
    //  Console::debug() << "Mensaje de depuración por consola" << 1 << " " << 2.3 << " " << 3.5f  << std::endl;
    //  Console::info() << "Mensaje de información por consola" << 1 << " " << 2.3 << " " << 3.5f  << std::endl;
    //  Console::warning() << "Mensaje de warning por consola" << 1 << " " << 2.3 << " " << 3.5f  << std::endl;
    //  Console::error() << "Mensaje de error por consola" << 1 << " " << 2.3 << " " << 3.5f  << std::endl;
    //}

    //chrono.stop();

    //chrono.setMessage("Console 2 std::format");
    //chrono.run();

    auto f1 = []() {
        for (size_t i = 0; i < 100; i++) {
            Console::debug("Mensaje de depuración por consola {}, {}, {}", 1, 2.3, 3.5f);
            Console::info("Mensaje de información por consola {}, {}, {}", 1, 2.3, 3.5f);
            Console::success("Mensaje de exito por consola {}, {}, {}", 1, 2.3, 3.5f);
            Console::warning("Mensaje de warning por consola {}, {}, {}", 1, 2.3, 3.5f);
            Console::error("Mensaje de error por consola {}, {}, {}", 1, 2.3, 3.5f);
        }
    };

    //chrono.stop();

    //chrono.reset();
    //chrono.setMessage("App::message()");
    //chrono.run();

    App::log().open("D:\\dev\\sources\\tidoplib\\test\\core\\messages\\log.txt");

    auto &message = Message::instance();
    message.addMessageHandler(&Console::instance());
    message.addMessageHandler(&Log::instance());

    auto f2 = []() {
        for (size_t i = 0; i < 100; i++) {
            //App::message() << MessageLevel::debug << "Mensaje de depuración por consola: " << 1 << " " << 2.3 << " " << 3.5f << std::endl;
            //App::message() << MessageLevel::info << "Mensaje de información por consola: " << 1 << " " << 2.3 << " " << 3.5f  << std::endl;
            //App::message() << MessageLevel::warning << "Mensaje de warning por consola: " << 1 << " " << 2.3 << " " << 3.5f  << std::endl;
            //App::message() << MessageLevel::error << "Mensaje de error por consola: " << 1 << " " << 2.3 << " " << 3.5f  << std::endl;

            //App::debug("Mensaje de depuración por consola {}, {}, {}", 1, 2.3, 3.5f);
            ////App::pauseMessages();
            //App::info("Mensaje de información por consola {}, {}, {}", 1, 2.3, 3.5f);
            ////App::resumeMessages();
            //App::warning("Mensaje de warning por consola {}, {}, {}", 1, 2.3, 3.5f);
            //App::succes("Mensaje de éxito por consola {}, {}, {}", 1, 2.3, 3.5f);
            //App::error("Mensaje de error por consola {}, {}, {}", 1, 2.3, 3.5f);
            Message::info("Mensaje de información {}, {}, {}", 1, 2.3, 3.5f);
            Message::warning("Mensaje de warning {}, {}, {}", 1, 2.3, 3.5f);
            Message::success("Mensaje de éxito {}, {}, {}", 1, 2.3, 3.5f);
            Message::error("Mensaje de error {}, {}, {}", 1, 2.3, 3.5f);
        }
    };

    std::thread t1(f1);
    std::thread t2(f2);

    t1.join();
    t2.join();

    //chrono.stop();
}

//#if CPP_VERSION >= 20
//
//#include <format>
//
//BOOST_FIXTURE_TEST_CASE(format_messages, MessageTest)
//{
//    Console &console = App::console();
//    console.setMessageLevel(MessageLevel::msg_error | MessageLevel::msg_warning | MessageLevel::msg_info | MessageLevel::msg_debug);
//    App::messageManager().addListener(&console);
//
//    Chrono chrono("Impresión por consola std::format");
//    chrono.run();
//
//    for (size_t i = 0; i < 100; i++) {
//      std::cout << std::format("Impresión por consola") << std::endl;
//    }
//    chrono.stop();
//
//    chrono.reset();
//    chrono.setMessage("Impresión con mas parametros");
//    chrono.run();
//
//    for (size_t i = 0; i < 100; i++) {
//      
//        // Mensage de debug
//        std::cout << std::format("{}Mensaje de información por consola {}, {}, {}", "Debug:   ", 1, 2.3, 3.5f) << std::endl;
//        
//        // Mensage de información
//        std::cout << std::format("{}Mensaje de información por consola {}, {}, {}", "Info:    ", 1, 2.3, 3.5f) << std::endl;
//
//        // Mensage de warning
//        console.setConsoleForegroundColor(Console::Color::magenta, Console::Intensity::normal);
//        std::cout << std::format("{}Mensaje de warning por consola {}, {}, {}", "Warning: ", 1, 2.3, 3.5f) << std::endl;
//        console.reset();
//
//        // Mensage de error
//        console.setConsoleForegroundColor(Console::Color::red, Console::Intensity::normal);
//        std::cout << std::format("{}Mensaje de error por consola {}, {}, {}", "Error:   ", 1, 2.3, 3.5f) << std::endl;
//        console.reset();
//
//    }
//
//    chrono.stop();
//}
//
//#endif



//class Message3
//{
//public:
//
//    using iterator = std::vector<std::ostream *>::iterator;
//    using const_iterator = std::vector<std::ostream *>::const_iterator;
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
//        std::stringstream string_stream;
//        string_stream << "Debug:   " << std::vformat(s.get(), std::make_format_args(args...));
//
//        for (auto &message : Message3::instance()) {
//          message.r << string_stream.str() << std::endl;
//        }
//    }
//
//    template<typename... Args>
//    static void info(std::format_string<Args...> s, Args&&... args)
//    {
//        std::stringstream string_stream;
//        string_stream << "Info:    " << std::vformat(s.get(), std::make_format_args(args...));
//
//        for (auto &message : Message3::instance()) {
//          message.stream() << string_stream.str() << std::endl;
//        }
//    }
//
//    template<typename... Args>
//    static void warning(std::format_string<Args...> s, Args&&... args)
//    {
//        std::stringstream string_stream;
//        string_stream << "Warning: " << std::vformat(s.get(), std::make_format_args(args...));
//        
//        for (auto &message : Message3::instance()) {
//          message.stream() << string_stream.str() << std::endl;
//        }
//    }
//
//    template<typename... Args>
//    static void error(std::format_string<Args...> s, Args&&... args)
//    {
//        std::stringstream string_stream;
//        string_stream << "Error:   " << std::vformat(s.get(), std::make_format_args(args...));
//        
//        for (auto &message : Message3::instance()) {
//          message.stream() << string_stream.str() << std::endl;
//        }
//    }
//
//private:
//
//    auto begin() TL_NOEXCEPT -> iterator
//    {
//      buffer.begin();
//    }
//
//    auto end() TL_NOEXCEPT -> iterator
//    {
//      buffer.end();
//    }
//};
//
//
//BOOST_FIXTURE_TEST_CASE(message_3, MessageTest)
//{
//
//    Console &console = App::console();
//    Message3::instance().subscribe(&console.stream());
//
//    std::ofstream log("D:\\dev\\sources\\tidoplib\\test\\core\\messages\\log.txt", std::ofstream::app);
//    Message3::instance().subscribe(&log);
//
//    Chrono chrono("Message 3 std::format");
//
//    chrono.run();
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
////    //chrono.reset();
////    //chrono.setMessage("App::message()");
////    //chrono.run();
////
////    //for (size_t i = 0; i < 100; i++) {
////    //  App::message() << MessageLevel::debug << "Mensaje de depuración por consola: " << 1 << " " << 2.3 << " " << 3.5f << std::endl;
////    //  App::message() << MessageLevel::info << "Mensaje de información por consola: " << 1 << " " << 2.3 << " " << 3.5f  << std::endl;
////    //  App::message() << MessageLevel::warning << "Mensaje de warning por consola: " << 1 << " " << 2.3 << " " << 3.5f  << std::endl;
////    //  App::message() << MessageLevel::error << "Mensaje de error por consola: " << 1 << " " << 2.3 << " " << 3.5f  << std::endl;
////    //}
//
//    //chrono.stop();
//}



BOOST_AUTO_TEST_SUITE_END()
