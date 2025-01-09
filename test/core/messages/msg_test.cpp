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
#include "tidop/core/app/app.h"
#include "tidop/core/base/chrono.h"
#include "tidop/core/app/message.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <thread>

using namespace tl;


BOOST_AUTO_TEST_SUITE(MessageTestSuite)

class MessageTest
    : public MessageHandler 
{

public:

    MessageTest() {}
    ~MessageTest() {}

    void setup()
    {   
        Message::addMessageHandler(this);
    }

    void teardown()
    {

    }


// Heredado vía MessageHandler

    virtual void debug(String message) override
    {
        debugMessage = message;
    }

    virtual void info(String message) override
    {
        infoMessage = message;
    }

    virtual void success(String message) override
    {
        successMessage = message;
    }

    virtual void warning(String message) override
    {
        warningMessage = message;
    }

    virtual void error(String message) override
    {
        errorMessage = message;
    }

    std::string debugMessage;
    std::string infoMessage;
    std::string successMessage;
    std::string warningMessage;
    std::string errorMessage;
};

BOOST_FIXTURE_TEST_CASE(debug_message, MessageTest)
{
    Message::debug("This is a single debug message");
    BOOST_CHECK_EQUAL(debugMessage, "This is a single debug message");

    Message::debug("This is a debug message {} {}", "more", "complex");
    BOOST_CHECK_EQUAL(debugMessage, "This is a debug message more complex");

    Message::debug("DEBUG: {} > {}", 2, 1);
    BOOST_CHECK_EQUAL(debugMessage, "DEBUG: 2 > 1");
}

BOOST_FIXTURE_TEST_CASE(info_message, MessageTest)
{
    Message::info("This is a single info message");
    BOOST_CHECK_EQUAL(infoMessage, "This is a single info message");

    Message::info("This is a info message {} {}", "more", "complex");
    BOOST_CHECK_EQUAL(infoMessage, "This is a info message more complex");

    Message::info("INFO: {} > {}", 2, 1);
    BOOST_CHECK_EQUAL(infoMessage, "INFO: 2 > 1");
}

BOOST_FIXTURE_TEST_CASE(success_message, MessageTest)
{
    Message::success("The command has finished successfully");
    BOOST_CHECK_EQUAL(successMessage, "The command has finished successfully");

    Message::success("Processed {} of {} images", 450, 500);
    BOOST_CHECK_EQUAL(successMessage, "Processed 450 of 500 images");
}

BOOST_FIXTURE_TEST_CASE(warning_message, MessageTest)
{
    Message::warning("This is a single warning message");
    BOOST_CHECK_EQUAL(warningMessage, "This is a single warning message");

    Message::warning("WARNING: {} > {}", 2, 1);
    BOOST_CHECK_EQUAL(warningMessage, "WARNING: 2 > 1");
}

BOOST_FIXTURE_TEST_CASE(error_message, MessageTest)
{
    Message::error("This is a single error message");
    BOOST_CHECK_EQUAL(errorMessage, "This is a single error message");

    Message::error("ERROR: {} > {}", 2, 1);
    BOOST_CHECK_EQUAL(errorMessage, "ERROR: 2 > 1");
}

BOOST_AUTO_TEST_SUITE_END()
