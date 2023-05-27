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
    Chrono chrono("Impresión por consola");
    chrono.run();

    Console &console = App::console();
    console.setTitle("Transform Example");
    console.setMessageLevel(MessageLevel::msg_error | MessageLevel::msg_warning | MessageLevel::msg_info | MessageLevel::msg_debug);
    App::messageManager().addListener(&console);

    for (size_t i = 0; i < 100; i++) {
      console.printMessage("Impresión por consola");
    }

    chrono.stop();
    chrono.reset();
    chrono.setMessage("Impresión por consola personalizada");
    chrono.run();

    for (size_t i = 0; i < 100; i++) {
      
        // Mensage de debug
        console.setConsoleForegroundColor(Console::Color::white, Console::Intensity::normal);
        console.printMessage("Mensaje de depuración por consola");
        console.reset();
        
        // Mensage de información
        console.setConsoleForegroundColor(Console::Color::white, Console::Intensity::bright);
        console.printMessage("Mensaje de información por consola");
        console.reset();

        // Mensage de warning
        console.setConsoleForegroundColor(Console::Color::magenta, Console::Intensity::bright);
        console.printMessage("Mensaje de warning por consola");
        console.reset();

        // Mensage de error
        console.printErrorMessage("Mensaje de error por consola");
    }
    
    chrono.stop();

    chrono.reset();
    chrono.setMessage("Impresión con macros");
    chrono.run();

    for (size_t i = 0; i < 100; i++) {
      
        //// Mensage de debug
        //msgDebug("Mensaje de depuración por consola");
        //
        //// Mensage de información
        //msgInfo("Mensaje de información por consola");

        //// Mensage de warning
        //msgWarning("Mensaje de warning por consola");

        //// Mensage de error
        //msgError("Mensaje de error por consola");

      tl::MessageManager::release(tl::MessageManager::Message("Mensaje de depuración por consola").message(), tl::MessageLevel::msg_debug);
      tl::MessageManager::release(tl::MessageManager::Message("Mensaje de información por consola").message(), tl::MessageLevel::msg_info);
      tl::MessageManager::release(tl::MessageManager::Message("Mensaje de warning por consola").message(), tl::MessageLevel::msg_warning);
      tl::MessageManager::release(tl::MessageManager::Message("Mensaje de error por consola").message(), tl::MessageLevel::msg_error);
    }

    chrono.stop();
}

// Info:    Impresión por consola [Time: 0.380867 seconds] (chrono.cpp:114, tl::Chrono::stop)
// Info:    Console 2 [Time: 2.454635 seconds] (chrono.cpp:114, tl::Chrono::stop)
// Info:    Console 2 [Time: 2.386155 seconds] (chrono.cpp:114, tl::Chrono::stop)
// Info:    App::message() [Time: 0.961311 seconds] (chrono.cpp:114, tl::Chrono::stop)
BOOST_FIXTURE_TEST_CASE(default_constructor, MessageTest)
{

    Chrono chrono("Impresión por consola");
    chrono.run();

    Console2 &console = Console2::instance();

    for (size_t i = 0; i < 100; i++) {
      console << "Impresión por consola" << std::endl;
    }

    chrono.stop();

    chrono.reset();
    chrono.setMessage("Console 2");
    chrono.run();

    for (size_t i = 0; i < 100; i++) {
      console << Level::debug << "Mensaje de depuración por consola" << std::endl;
      console << Level::info << "Mensaje de información por consola" << std::endl;
      console << Level::warning << "Mensaje de warning por consola" << std::endl;
      console << Level::error << "Mensaje de error por consola" << std::endl;
    }

    chrono.stop();

    chrono.reset();
    chrono.setMessage("Console 2");
    chrono.run();

    for (size_t i = 0; i < 100; i++) {
      Console2::debug() << "Mensaje de depuración por consola" << std::endl;
      Console2::info() << "Mensaje de información por consola" << std::endl;
      Console2::warning() << "Mensaje de warning por consola" << std::endl;
      Console2::error() << "Mensaje de error por consola" << std::endl;
    }

    chrono.stop();
  //Message message;

  //std::ostream &kk = std::cout;
  //kk  << "sdfsdf";
  //message << Message::Level::debug << "Hello World" << std::endl;

  //std::ofstream fout("D:\\dev\\sources\\tidoplib\\test\\core\\messages\\log.txt", std::ofstream::app);
  //teebuf        sbuf(fout.rdbuf(), std::cout.rdbuf());
  //std::ostream  out(&sbuf);
  //out << "hello, world!\n";
  // 
  //MessageBuffer msgbuff /*= MessageBuffer::instance()*/;
  //msgbuff.subscribe(fout.rdbuf());
  ////msgbuff.subscribe(std::cout.rdbuf());
  //msgbuff.subscribe(console.rdbuf());
  ////std::ostream  out(&msgbuff);
  ////out << "hello, world!\n";
  
  //console.stream() << "Console" << std::endl;
  //fout << "Log" << std::endl;

  //App::messageHandler().subscribe(fout.rdbuf());
  App::messageHandler().subscribe(console.rdbuf());
  //MessageHandler message_manager(&msgbuff);
  //message_manager << Level::info << "MessageManager" << std::endl;
  //App::message() << "Hola" << std::endl;

    chrono.reset();
    chrono.setMessage("App::message()");
    chrono.run();

    for (size_t i = 0; i < 100; i++) {
      App::message() << Level::debug << "Mensaje de depuración por consola" << std::endl;
      App::message() << Level::info << "Mensaje de información por consola" << std::endl;
      App::message() << Level::warning << "Mensaje de warning por consola" << std::endl;
      App::message() << Level::error << "Mensaje de error por consola" << std::endl;
    }

    chrono.stop();
}


BOOST_AUTO_TEST_SUITE_END()
