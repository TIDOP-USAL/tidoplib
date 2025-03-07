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
 
#define BOOST_TEST_MODULE Tidop task test
#include <boost/test/unit_test.hpp>
#include <tidop/core/task.h>
#include <tidop/core/console.h>

using namespace tl;

class Task1
    : public TaskBase
{

public:

    Task1()
        : TaskBase()
    {
    }

    ~Task1()
    {
    }

    size_t count() const
    {
        return _count;
    }

    // Heredado vía TaskBase

protected:

    void execute(Progress *progressBar = nullptr) override
    {
        _count = 0;

        try {

            for (size_t i = 0; i < 100; i++) {
                _count++;
            }

        } catch (...) {
            TL_THROW_EXCEPTION_WITH_NESTED("");
        }
    }



private:

    size_t _count{};
};


BOOST_AUTO_TEST_SUITE(TaskTestSuite)

struct TaskTest
{
    TaskTest()
    {
    }

    ~TaskTest()
    {
    }

    void setup()
    {
    }

    void teardown()
    {
    }

    Task1 task1;

};

BOOST_FIXTURE_TEST_CASE(DefaultConstructor, TaskTest)
{
    BOOST_CHECK_EQUAL(0, task1.count());
}

BOOST_FIXTURE_TEST_CASE(run, TaskTest)
{
    BOOST_CHECK_EQUAL(0, task1.count());

    task1.run();

    BOOST_CHECK_EQUAL(100, task1.count());
}

BOOST_FIXTURE_TEST_CASE(running_event, TaskTest)
{

    task1.subscribe([&](TaskRunningEvent *event) {
        BOOST_CHECK_EQUAL(0, task1.count());
    });

    task1.run();
}

BOOST_FIXTURE_TEST_CASE(finalized_event, TaskTest)
{

    task1.subscribe([&](TaskFinalizedEvent *event) {
        BOOST_CHECK_EQUAL(100, task1.count());
    });

    task1.run();
}

BOOST_AUTO_TEST_SUITE_END()