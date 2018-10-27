#include "config_tl.h"

#include <gtest/gtest.h>
#ifdef HAVE_GMOCK
#include <gmock/gmock.h>
using ::testing::_;
#endif

#include <tidop/core/process.h>

using namespace tl;

class FakeProcess
  : public Process
{

public:

  FakeProcess()
    : Process(nullptr)
  {
    this->mProcessName = "Fake process";
  }

  // Process interface
public:
  Status run(Progress *progressBar) override
  {
    return Process::run(progressBar);
  }
};


TEST(Process, Constructor)
{
  FakeProcess process;
  EXPECT_EQ(FakeProcess::Status::START, process.getStatus());
  EXPECT_EQ(1, process.getProcessId());
  EXPECT_EQ("Fake process", process.getProcessName());
}

TEST(Process, control)
{
  FakeProcess process;
  process.run(nullptr);
  EXPECT_EQ(FakeProcess::Status::RUNNING, process.getStatus());

  process.pause();
  EXPECT_EQ(FakeProcess::Status::PAUSING, process.getStatus());

  process.resume();
  EXPECT_EQ(FakeProcess::Status::RUNNING, process.getStatus());

  process.stop();
  EXPECT_EQ(FakeProcess::Status::STOPPED, process.getStatus());

}

TEST(Process, reset)
{
  FakeProcess process;
  process.run(nullptr);
  process.reset();
  EXPECT_EQ(FakeProcess::Status::START, process.getStatus());
}



#ifdef HAVE_GMOCK

class MockProcess : public Process
{
public:

  MockProcess()
    : Process()
  {
    this->mProcessName = "Mock Process";
  }

  MOCK_METHOD0(pause, void());
  MOCK_METHOD0(reset, void());
  MOCK_METHOD0(resume, void());
  MOCK_METHOD1(run, Status(Progress *progressBar));
  MOCK_METHOD0(stop, void());

};

TEST(BatchProcess, Constructor)
{
  BatchProcess batch;
  EXPECT_FALSE(batch.isRunning());
}

TEST(BatchProcess, add)
{
  BatchProcess batch;
  std::shared_ptr<MockProcess> mock_process(new MockProcess);
  batch.add(mock_process);
  EXPECT_FALSE(batch.isRunning());
  batch.run(nullptr, nullptr);

//  EXPECT_CALL(mock_process, run(_))
//            .Times(1);
}

#endif
