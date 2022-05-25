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


#include "tidop/core/task.h"
#include "tidop/core/messages.h"
#include "tidop/core/progress.h"

#if defined WIN32
#include <windows.h>
#else
#include <spawn.h>
#include <sys/wait.h>
#endif
#include <locale>
#include <codecvt>
#include <utility>
#include <cstring>

#ifndef WIN32
extern char **environ;
#endif

namespace tl
{

/* Process */

Task::Task() = default;

Task::~Task() = default;



/* TaskBase */


TaskBase::TaskBase()
  : Task(),
    mTaskErrorEvent(new TaskErrorEvent),
    mTaskFinalizedEvent(new TaskFinalizedEvent(this)),
    mTaskPauseEvent(new TaskPauseEvent),
    mTaskPausingEvent(new TaskPausingEvent),
    mTaskResumedEvent(new TaskResumedEvent),
    mTaskRunningEvent(new TaskRunningEvent),
    mTaskStoppedEvent(new TaskStoppedEvent),
    mTaskStoppingEvent(new TaskStoppingEvent),
    mTaskErrorEventHandler(0),
    mTaskFinalizedEventHandler(0),
    mTaskPauseEventHandler(0),
    mTaskPausingEventHandler(0),
    mTaskResumedEventHandler(0),
    mTaskRunningEventHandler(0),
    mTaskStoppedEventHandler(0),
    mTaskStoppingEventHandler(0)
{

}

TaskBase::TaskBase(const TaskBase &task)
  : Task(),
    mTaskErrorEvent(new TaskErrorEvent(*task.mTaskErrorEvent)),
    mTaskFinalizedEvent(new TaskFinalizedEvent(*task.mTaskFinalizedEvent)),
    mTaskPauseEvent(new TaskPauseEvent(*task.mTaskPauseEvent)),
    mTaskPausingEvent(new TaskPausingEvent(*task.mTaskPausingEvent)),
    mTaskResumedEvent(new TaskResumedEvent(*task.mTaskResumedEvent)),
    mTaskRunningEvent(new TaskRunningEvent(*task.mTaskRunningEvent)),
    mTaskStoppedEvent(new TaskStoppedEvent(*task.mTaskStoppedEvent)),
    mTaskStoppingEvent(new TaskStoppingEvent(*task.mTaskStoppingEvent)),
    mTaskErrorEventHandler(task.mTaskErrorEventHandler),
    mTaskFinalizedEventHandler(task.mTaskFinalizedEventHandler),
    mTaskPauseEventHandler(task.mTaskPauseEventHandler),
    mTaskPausingEventHandler(task.mTaskPausingEventHandler),
    mTaskResumedEventHandler(task.mTaskResumedEventHandler),
    mTaskRunningEventHandler(task.mTaskRunningEventHandler),
    mTaskStoppedEventHandler(task.mTaskStoppedEventHandler),
    mTaskStoppingEventHandler(task.mTaskStoppingEventHandler)
{

}

TaskBase::TaskBase(TaskBase &&task) TL_NOEXCEPT
  : Task(),
    mTaskErrorEvent(std::move(task.mTaskErrorEvent)),
    mTaskFinalizedEvent(std::move(task.mTaskFinalizedEvent)),
    mTaskPauseEvent(std::move(task.mTaskPauseEvent)),
    mTaskPausingEvent(std::move(task.mTaskPausingEvent)),
    mTaskResumedEvent(std::move(task.mTaskResumedEvent)),
    mTaskRunningEvent(std::move(task.mTaskRunningEvent)),
    mTaskStoppedEvent(std::move(task.mTaskStoppedEvent)),
    mTaskStoppingEvent(std::move(task.mTaskStoppingEvent)),
    mTaskErrorEventHandler(std::move(task.mTaskErrorEventHandler)),
    mTaskFinalizedEventHandler(std::move(task.mTaskFinalizedEventHandler)),
    mTaskPauseEventHandler(std::move(task.mTaskPauseEventHandler)),
    mTaskPausingEventHandler(std::move(task.mTaskPausingEventHandler)),
    mTaskResumedEventHandler(std::move(task.mTaskResumedEventHandler)),
    mTaskRunningEventHandler(std::move(task.mTaskRunningEventHandler)),
    mTaskStoppedEventHandler(std::move(task.mTaskStoppedEventHandler)),
    mTaskStoppingEventHandler(std::move(task.mTaskStoppingEventHandler))
{

}

TaskBase::~TaskBase()
{
  if((mStatus != Status::start) || 
     mStatus != Status::finalized ||
     mStatus != Status::stopped) {
    if(mStatus == Status::running ||
        mStatus == Status::paused ||
        mStatus == Status::pausing) {
      stop();
    }

    //if(mStatus != Status::finalized) {
    //  mStatus = Status::finalized;
    //  eventTriggered(Event::Type::task_finalized);
    //}
  }
}

TaskBase &TaskBase::operator=(const TaskBase &task)
{
  if (this != &task){
    mTaskErrorEvent = std::make_unique<TaskErrorEvent>(*task.mTaskErrorEvent);
    mTaskFinalizedEvent = std::make_unique<TaskFinalizedEvent>(*task.mTaskFinalizedEvent);
    mTaskPauseEvent = std::make_unique<TaskPauseEvent>(*task.mTaskPauseEvent);
    mTaskPausingEvent = std::make_unique<TaskPausingEvent>(*task.mTaskPausingEvent);
    mTaskResumedEvent = std::make_unique<TaskResumedEvent>(*task.mTaskResumedEvent);
    mTaskRunningEvent = std::make_unique<TaskRunningEvent>(*task.mTaskRunningEvent);
    mTaskStoppedEvent = std::make_unique<TaskStoppedEvent>(*task.mTaskStoppedEvent);
    mTaskStoppingEvent = std::make_unique<TaskStoppingEvent>(*task.mTaskStoppingEvent);
    mTaskErrorEventHandler = task.mTaskErrorEventHandler;
    mTaskFinalizedEventHandler = task.mTaskFinalizedEventHandler;
    mTaskPauseEventHandler = task.mTaskPauseEventHandler;
    mTaskPausingEventHandler = task.mTaskPausingEventHandler;
    mTaskResumedEventHandler = task.mTaskResumedEventHandler;
    mTaskRunningEventHandler = task.mTaskRunningEventHandler;
    mTaskStoppedEventHandler = task.mTaskStoppedEventHandler;
    mTaskStoppingEventHandler = task.mTaskStoppingEventHandler;
  }
  return *this;
}

TaskBase &TaskBase::operator=(TaskBase &&task) TL_NOEXCEPT
{
  if (this != &task){
    mTaskErrorEvent = std::move(task.mTaskErrorEvent);
    mTaskFinalizedEvent = std::move(task.mTaskFinalizedEvent);
    mTaskPauseEvent = std::move(task.mTaskPauseEvent);
    mTaskPausingEvent = std::move(task.mTaskPausingEvent);
    mTaskResumedEvent = std::move(task.mTaskResumedEvent);
    mTaskRunningEvent = std::move(task.mTaskRunningEvent);
    mTaskStoppedEvent = std::move(task.mTaskStoppedEvent);
    mTaskStoppingEvent = std::move(task.mTaskStoppingEvent);
    mTaskErrorEventHandler = std::move(task.mTaskErrorEventHandler);
    mTaskFinalizedEventHandler = std::move(task.mTaskFinalizedEventHandler);
    mTaskPauseEventHandler = std::move(task.mTaskPauseEventHandler);
    mTaskPausingEventHandler = std::move(task.mTaskPausingEventHandler);
    mTaskResumedEventHandler = std::move(task.mTaskResumedEventHandler);
    mTaskRunningEventHandler = std::move(task.mTaskRunningEventHandler);
    mTaskStoppedEventHandler = std::move(task.mTaskStoppedEventHandler);
    mTaskStoppingEventHandler = std::move(task.mTaskStoppingEventHandler);
  }
  return *this;
}

void TaskBase::run(Progress *progressBar)
{
  mThread = std::move(std::thread(&TaskBase::executeTask, this, progressBar));
  mThread.join();
}

void TaskBase::runAsync(Progress *progressBar)
{
  mThread = std::thread(&TaskBase::executeTask, this, progressBar);
  mThread.detach();
}

void TaskBase::pause()
{
  if (mStatus == Status::running) {

    setStatus(Status::pausing);

    TL_TODO("Revisar")
    //while (mStatus != Status::paused || mStatus != Status::finalized || mStatus != Status::error);

    eventTriggered(Event::Type::task_paused);
  }
}

void TaskBase::reset()
{
  mStatus = Status::start;
  mTaskErrorEventHandler.clear();
  mTaskFinalizedEventHandler.clear();
  mTaskPauseEventHandler.clear();
  mTaskPausingEventHandler.clear();
  mTaskResumedEventHandler.clear();
  mTaskRunningEventHandler.clear();
  mTaskStoppedEventHandler.clear();
  mTaskStoppingEventHandler.clear();
}

void TaskBase::resume()
{
  eventTriggered(Event::Type::task_resumed);

  if (mStatus == Status::paused || mStatus == Status::pausing) {
    setStatus(Status::running);
  }
}

void TaskBase::stop()
{
  if (mStatus == Status::running ||
      mStatus == Status::paused ||
      mStatus == Status::pausing) {

    setStatus(Status::stopping);

  }
}

void TaskBase::subscribe(Event::Type eventType,
                         const EventHandler &eventHandler)
{
  switch (eventType) {
    case Event::Type::task_error:
      mTaskErrorEventHandler.emplace_back(eventHandler);
      break;
    case Event::Type::task_paused:
      mTaskPauseEventHandler.emplace_back(eventHandler);
      break;
    case Event::Type::task_pausing:
      mTaskPausingEventHandler.emplace_back(eventHandler);
      break;
    case Event::Type::task_resumed:
      mTaskResumedEventHandler.emplace_back(eventHandler);
      break;
    case Event::Type::task_running:
      mTaskRunningEventHandler.emplace_back(eventHandler);
      break;
    case Event::Type::task_stopped:
      mTaskStoppedEventHandler.emplace_back(eventHandler);
      break;
    case Event::Type::task_stopping:
      mTaskStoppingEventHandler.emplace_back(eventHandler);
      break;
    case Event::Type::task_finalized:
      mTaskFinalizedEventHandler.emplace_back(eventHandler);
      break;
  }


}

void TaskBase::subscribe(const EventHandler &eventHandler)
{
  mTaskErrorEventHandler.emplace_back(eventHandler);
  mTaskFinalizedEventHandler.emplace_back(eventHandler);
  mTaskPauseEventHandler.emplace_back(eventHandler);
  mTaskPausingEventHandler.emplace_back(eventHandler);
  mTaskResumedEventHandler.emplace_back(eventHandler);
  mTaskRunningEventHandler.emplace_back(eventHandler);
  mTaskStoppedEventHandler.emplace_back(eventHandler);
  mTaskStoppingEventHandler.emplace_back(eventHandler);
}

void TaskBase::subscribe(const TaskErrorEventHandler &eventHandler)
{
  mTaskErrorEventHandler.push_back(eventHandler);
}

void TaskBase::subscribe(const TaskFinalizedEventHandler &eventHandler)
{
  mTaskFinalizedEventHandler.push_back(eventHandler);
}

void TaskBase::subscribe(const TaskPauseEventHandler &eventHandler)
{
  mTaskPauseEventHandler.push_back(eventHandler);
}

void TaskBase::subscribe(const TaskPausingEventHandler &eventHandler)
{
  mTaskPausingEventHandler.push_back(eventHandler);
}

void TaskBase::subscribe(const TaskResumedEventHandler &eventHandler)
{
  mTaskResumedEventHandler.push_back(eventHandler);
}

void TaskBase::subscribe(const TaskRunningEventHandler &eventHandler)
{
  mTaskRunningEventHandler.push_back(eventHandler);
}

void TaskBase::subscribe(const TaskStoppedEventHandler &eventHandler)
{
  mTaskStoppedEventHandler.push_back(eventHandler);
}

void TaskBase::subscribe(const TaskStoppingEventHandler &eventHandler)
{
  mTaskStoppingEventHandler.push_back(eventHandler);
}

Task::Status TaskBase::status() const
{
  return mStatus;
}

void TaskBase::setStatus(Status status)
{
  mStatus = status;

  switch(mStatus) {
    case tl::Task::Status::running:
      eventTriggered(Event::Type::task_running);
      break;
    case tl::Task::Status::pausing:
      eventTriggered(Event::Type::task_pausing);
      break;
    case tl::Task::Status::paused:
      eventTriggered(Event::Type::task_paused);
      break;
    case tl::Task::Status::stopping:
      eventTriggered(Event::Type::task_stopping);
      break;
    case tl::Task::Status::stopped:
      eventTriggered(Event::Type::task_stopped);
      break;
    case tl::Task::Status::finalized:
      eventTriggered(Event::Type::task_finalized);
      break;
    case tl::Task::Status::error:
      eventTriggered(Event::Type::task_error);
      break;
    default:
      break;
  }
}

void TaskBase::eventTriggered(Event::Type type)
{
  switch (type) {
    case Event::Type::task_error:
      this->eventTaskErrorTriggered();
      break;
    case Event::Type::task_paused:
      this->eventTaskPauseTriggered();
      break;
    case Event::Type::task_pausing:
      this->eventTaskPausingTriggered();
      break;
    case Event::Type::task_resumed:
      this->eventTaskResumedTriggered();
      break;
    case Event::Type::task_running:
      this->eventTaskRunningTriggered();
      break;
    case Event::Type::task_stopped:
      this->eventTaskStoppedTriggered();
      break;
    case Event::Type::task_stopping:
      this->eventTaskStoppingTriggered();
      break;
    case Event::Type::task_finalized:
      this->eventTaskFinalizedTriggered();
      break;
    default:
      break;
  }

}

void TaskBase::eventTaskErrorTriggered()
{
  std::list<TaskErrorEventHandler> event_handler = mTaskErrorEventHandler;
  for (auto &handler : event_handler) {
    handler(mTaskErrorEvent.get());
  }
}

void TaskBase::eventTaskFinalizedTriggered()
{
  std::list<TaskFinalizedEventHandler> event_handler = mTaskFinalizedEventHandler;
  for (auto &handler : event_handler) {
    handler(mTaskFinalizedEvent.get());
  }
}

void TaskBase::eventTaskPauseTriggered()
{
  std::list<TaskPauseEventHandler> event_handler = mTaskPauseEventHandler;
  for (auto &handler : event_handler) {
    handler(mTaskPauseEvent.get());
  }
}

void TaskBase::eventTaskPausingTriggered()
{
  std::list<TaskPausingEventHandler> event_handler = mTaskPausingEventHandler;
  for (auto &handler : event_handler) {
    handler(mTaskPausingEvent.get());
  }
}

void TaskBase::eventTaskResumedTriggered()
{
  std::list<TaskResumedEventHandler> event_handler = mTaskResumedEventHandler;
  for (auto &handler : event_handler) {
    handler(mTaskResumedEvent.get());
  }
}

void TaskBase::eventTaskRunningTriggered()
{
  std::list<TaskRunningEventHandler> event_handler = mTaskRunningEventHandler;
  for (auto &handler : event_handler) {
    handler(mTaskRunningEvent.get());
  }
}

void TaskBase::eventTaskStoppedTriggered()
{
  std::list<TaskStoppedEventHandler> event_handler = mTaskStoppedEventHandler;
  for (auto &handler : event_handler) {
    handler(mTaskStoppedEvent.get());
  }
}

void TaskBase::eventTaskStoppingTriggered()
{
  std::list<TaskStoppingEventHandler> event_handler = mTaskStoppingEventHandler;
  for (auto &handler : event_handler) {
    handler(mTaskStoppingEvent.get());
  }
}

TaskErrorEvent *TaskBase::errorEvent()
{
  return mTaskErrorEvent.get();
}

TaskFinalizedEvent *TaskBase::finalizedEvent()
{
  return mTaskFinalizedEvent.get();
}

TaskPauseEvent *TaskBase::pauseEvent()
{
  return mTaskPauseEvent.get();
}

TaskPausingEvent *TaskBase::pausingEvent()
{
  return mTaskPausingEvent.get();
}

TaskResumedEvent *TaskBase::resumedEvent()
{
  return mTaskResumedEvent.get();
}

TaskRunningEvent *TaskBase::runningEvent()
{
  return mTaskRunningEvent.get();
}

TaskStoppedEvent *TaskBase::stoppedEvent()
{
  return mTaskStoppedEvent.get();
}

TaskStoppingEvent *TaskBase::stoppingEvent()
{
  return mTaskStoppingEvent.get();
}

void TaskBase::executeTask(Progress *progressBar) TL_NOEXCEPT
{
  if(mStatus != Status::start) return;

  try { 
    
    setStatus(Status::running);

    execute(progressBar);

    if (mStatus == Status::stopping)
      setStatus(Status::stopped);
    else
      setStatus(Status::finalized);

  } catch(const std::exception &e) {
    printException(e);
    mTaskErrorEvent->setErrorMessage(e.what());
    setStatus(Status::error);
  }

  //mStatus = Status::start;
}



/* Process */

#ifdef WIN32

WCHAR *toWCHAR(const char *str)
{
  size_t length = strlen(str);
  WCHAR *buffer = (WCHAR *)malloc(sizeof(WCHAR) * length);
  int nChars = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
  MultiByteToWideChar(CP_ACP, 0, str, -1, (LPWSTR)buffer, nChars);
  return buffer;
}


constexpr auto process_bufsize = 4096;
HANDLE pipeReadHandle = nullptr;
HANDLE pipeWriteHandle = nullptr;

unsigned long readFromPipe(void *)
{
  DWORD numberOfBytesRead;
  char buffer[process_bufsize];
  int err = 0;

  for (;;) {
    err = ReadFile(pipeReadHandle, buffer, process_bufsize, &numberOfBytesRead, nullptr);
    if (!err || numberOfBytesRead == 0) continue;
    buffer[numberOfBytesRead] = '\0';
    std::cout << buffer << std::endl;

    if (!err) break;
  }
  return 0;
}
#endif

Process::Process(std::string commandText/*,
                                 Priority priority*/)
  : mCommandText(std::move(commandText))/*,
    mPriority(priority)*/
#ifdef WIN32
    ,
    mPriority(Priority::normal),
    mThreadHandle(nullptr)
#endif
{
#ifdef WIN32
  ZeroMemory(&mStartUpInfo, sizeof(mStartUpInfo));
  mStartUpInfo.cb = sizeof(mStartUpInfo);
  mStartUpInfo.dwFlags |= STARTF_USESTDHANDLES;

  ZeroMemory(&mSecurityAttributes, sizeof(mSecurityAttributes));
  mSecurityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
  mSecurityAttributes.bInheritHandle = TRUE;
  mSecurityAttributes.lpSecurityDescriptor = NULL;

  ZeroMemory(&mProcessInformation, sizeof(mProcessInformation));
#endif
}

Process::~Process()
{
}

void Process::execute(Progress *)
{
  try {

#ifdef WIN32

    //if (!createPipe()) return;

    unsigned long priority = static_cast<unsigned long>(mPriority);

    if (!CreateProcess(nullptr,
                       toWCHAR(mCommandText.c_str()),
                       nullptr,
                       nullptr,
                       false, //true,
                       CREATE_NO_WINDOW | priority,
                       nullptr,
                       nullptr,
                       &mStartUpInfo,
                       &mProcessInformation)) {

      TL_THROW_EXCEPTION("CreateProcess failed (%d) %s",
                         GetLastError(),
                         formatErrorMsg(GetLastError()).c_str());
    }

    //mThreadHandle = CreateThread(0, 0, readFromPipe, nullptr, 0, nullptr);

    DWORD ret = WaitForSingleObject(mProcessInformation.hProcess, INFINITE);
    if (ret == WAIT_OBJECT_0) {

      unsigned long exitCode;
      if (GetExitCodeProcess(mProcessInformation.hProcess, &exitCode) == 0) {
        TL_THROW_EXCEPTION("Error (%d: %s) when executing the command: %s",
                           GetLastError(),
                           formatErrorMsg(GetLastError()).c_str(),
                           mCommandText.c_str());
      } else {
        if (exitCode == 0)
          eventTriggered(Event::Type::task_finalized);
        else
          eventTriggered(Event::Type::task_error);
      }

    } else if (ret == WAIT_FAILED) {

      TL_THROW_EXCEPTION("Error (%d: %s) when executing the command: %s",
                         GetLastError(),
                         formatErrorMsg(GetLastError()).c_str(),
                         mCommandText.c_str());

    } else if (ret == WAIT_ABANDONED) {

      TL_THROW_EXCEPTION("Error (%d: %s) when executing the command: %s",
                         GetLastError(),
                         formatErrorMsg(GetLastError()).c_str(),
                         mCommandText.c_str());

    } else if (ret == WAIT_TIMEOUT) {

      TL_THROW_EXCEPTION("Error (%d: %s) when executing the command: %s",
                         GetLastError(),
                         formatErrorMsg(GetLastError()).c_str(),
                         mCommandText.c_str());

    } /*else {

    }*/

    CloseHandle(mProcessInformation.hProcess);
    CloseHandle(mProcessInformation.hThread);
    //CloseHandle(mThreadHandle);
    //CloseHandle(pipeWriteHandle);
    //CloseHandle(pipeReadHandle);

#else
    pid_t pid;
    char *cmd = nullptr;
    strcpy(cmd, mCommandText.c_str());
    char *argv[] = {"sh", "-c", cmd, nullptr};
    int status;
    //printf("Run command: %s\n", cmd);
    status = posix_spawn(&pid, "/bin/sh", nullptr, nullptr, argv, environ);
    if (status == 0) {
      //printf("Child pid: %i\n", pid);
      if (waitpid(pid, &status, 0) != -1) {
        //printf("Child exited with status %i\n", status);
        return;
      } else {
        return; //Process::Status::error;
      }
    } else {
      printf("posix_spawn: %s\n", strerror(status));
      msgError("Error (%i: %s) when executing the command: %s", status, strerror(status), mCommandText.c_str());
      return; //Process::Status::error;
    }
    //  int posix_spawn(pid_t *pid, const char *path,
    //                  const posix_spawn_file_actions_t *file_actions,
    //                  const posix_spawnattr_t *attrp,
    //                  char *const argv[], char *const envp[]);
      /// Para escribir en un log la salida
      /// https://unix.stackexchange.com/questions/252901/get-output-of-posix-spawn

#endif
  } catch (...) {
    TL_THROW_EXCEPTION_WITH_NESTED("");
  }

}

#ifdef WIN32
Process::Priority Process::priority() const
{
  return static_cast<Priority>(GetPriorityClass(mProcessInformation.hProcess));
}

void Process::setPriority(Priority priority)
{
  SetPriorityClass(mProcessInformation.hProcess, 
                   static_cast<unsigned long>(priority));
  mPriority = priority;
}

std::string Process::formatErrorMsg(unsigned long errorCode)
{
  DWORD flags = FORMAT_MESSAGE_FROM_SYSTEM | 
                FORMAT_MESSAGE_IGNORE_INSERTS | 
                FORMAT_MESSAGE_MAX_WIDTH_MASK;

  TCHAR errorMessage[1024] = TEXT("");

  FormatMessage(flags,
                nullptr,
                errorCode,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                errorMessage,
                sizeof(errorMessage) / sizeof(TCHAR),
                nullptr);

  std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
  std::string strError = converter.to_bytes(errorMessage);

  return strError;
}


bool Process::createPipe()
{
  if (!CreatePipe(&pipeReadHandle, &pipeWriteHandle, &mSecurityAttributes, 0)) {
    msgError("CreateProcess failed (%d) %s", GetLastError(), formatErrorMsg(GetLastError()).c_str());
    eventTriggered(Event::Type::task_error);
    return false;
  }

  if (!SetHandleInformation(pipeReadHandle, HANDLE_FLAG_INHERIT, 0)) {
    msgError("CreateProcess failed (%d) %s", GetLastError(), formatErrorMsg(GetLastError()).c_str());
    eventTriggered(Event::Type::task_error);
    return false;
  }

  mStartUpInfo.hStdError = pipeWriteHandle;
  mStartUpInfo.hStdOutput = pipeWriteHandle;

  return true;
}

#endif



/* Task List */

TaskList::TaskList()
  : TaskBase(),
    mTasks(0)
{
}

TaskList::TaskList(const TaskList &taskList)
  : TaskBase(),
    mTasks(taskList.mTasks)
{
}

TaskList::TaskList(std::initializer_list<std::shared_ptr<Task>> tasks)
  : TaskBase(),
    mTasks(tasks)
{
}

TaskList::~TaskList()
{
}

void TaskList::push_back(const std::shared_ptr<Task> &task)
{
  mTasks.push_back(task);
}

void TaskList::stop()
{
  TaskBase::stop();

  if (status() == Status::stopping) {
    for (const auto &task : mTasks) {
      task->stop();
    }
  }
}

void TaskList::execute(Progress *progressBar)
{
  for(const auto &task : mTasks) {

    if (status() == Status::stopping) return;

    task->run();

    if(progressBar) (*progressBar)();

  }
}



/* Task Tree */

TaskTree::TaskTree()
{
}

TaskTree::~TaskTree()
{
}

void TaskTree::addTask(const std::shared_ptr<Task> &task,
                       const std::list<std::shared_ptr<Task>> &parentTasks)
{

}

void TaskTree::stop()
{
}

void TaskTree::execute(Progress *progressBar)
{
}

} // End namespace tl


