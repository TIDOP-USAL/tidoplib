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
 
//#include "config_tl.h"
//
//#include <gtest/gtest.h>
//#ifdef HAVE_GMOCK
//#include <gmock/gmock.h>
//using ::testing::_;
//#endif
//
//#include <tidop/core/process.h>
//
//using namespace tl;
//
//#ifndef TL_OLD_PROCESS
//
//#else
//
//class FakeProcess
//  : public Process
//{
//
//public:
//
//  FakeProcess()
//    : Process(nullptr)
//  {
//    this->mProcessName = "Fake process";
//  }
//
//  // Process interface
//public:
//  Status run(Progress *progressBar) override
//  {
//    return Process::run(progressBar);
//  }
//};
//
//
//TEST(Process, Constructor)
//{
//  FakeProcess process;
//  EXPECT_EQ(FakeProcess::Status::start, process.status());
//  EXPECT_EQ(1, process.id());
//  EXPECT_EQ("Fake process", process.name());
//}
//
//TEST(Process, control)
//{
//  FakeProcess process;
//  process.run(nullptr);
//  EXPECT_EQ(FakeProcess::Status::running, process.status());
//
//  process.pause();
//  EXPECT_EQ(FakeProcess::Status::pausing, process.status());
//
//  process.resume();
//  EXPECT_EQ(FakeProcess::Status::running, process.status());
//
//  process.stop();
//  EXPECT_EQ(FakeProcess::Status::stopped, process.status());
//
//}
//
//TEST(Process, reset)
//{
//  FakeProcess process;
//  process.run(nullptr);
//  process.reset();
//  EXPECT_EQ(FakeProcess::Status::start, process.status());
//}
//
//
//
//#ifdef HAVE_GMOCK
//
//class MockProcess : public Process
//{
//public:
//
//  MockProcess()
//    : Process()
//  {
//    this->mProcessName = "Mock Process";
//  }
//
//  MOCK_METHOD0(pause, void());
//  MOCK_METHOD0(reset, void());
//  MOCK_METHOD0(resume, void());
//  MOCK_METHOD1(run, Status(Progress *progressBar));
//  MOCK_METHOD0(stop, void());
//
//};
//
//TEST(BatchProcess, Constructor)
//{
//  BatchProcess batch;
//  EXPECT_FALSE(batch.isRunning());
//}
//
//TEST(BatchProcess, add)
//{
//  BatchProcess batch;
//  std::shared_ptr<MockProcess> mock_process(new MockProcess);
//  batch.add(mock_process);
//  EXPECT_FALSE(batch.isRunning());
//  batch.run(nullptr, nullptr);
//
////  EXPECT_CALL(mock_process, run(_))
////            .Times(1);
//}
//
//#endif
//
//#endif

#define BOOST_TEST_MODULE Tidop process test
#include <boost/test/unit_test.hpp>
#include <tidop/core/process.h>
#include <tidop/core/console.h>

using namespace tl;

class Process1
  : public ProcessBase
{

public:

  Process1()
    : ProcessBase()
  {
  }

  ~Process1()
  {
  }

  size_t count() const
  {
    return _count;
  }

// Heredado vía ProcessBase

protected:

  void execute(Progress *progressBar = nullptr) override
  {
    _count = 0;

    try {

      for (size_t i = 0; i < 100; i++) {
        _count++;
      }
      eventTriggered(Event::Type::process_finalized);

    } catch (std::exception &e) {
      ProcessBase::errorEvent()->setErrorMessage(e.what());
      eventTriggered(Event::Type::process_error);
    }
  }



private:

  size_t _count{};
};


BOOST_AUTO_TEST_SUITE(ProcessTestSuite)

struct ProcessTest
{
  ProcessTest()
  {
  }

  ~ProcessTest()
  {
  }

  void setup()
  {
  }

  void teardown()
  {
  }

  Process1 process1;
}; 

BOOST_FIXTURE_TEST_CASE(DefaultConstructor, ProcessTest)
{
  BOOST_CHECK_EQUAL(0, process1.count());
}

BOOST_FIXTURE_TEST_CASE(run, ProcessTest)
{
  process1.run();
  BOOST_CHECK_EQUAL(100, process1.count());
}

BOOST_AUTO_TEST_SUITE_END()