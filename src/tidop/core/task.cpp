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
#include "tidop/core/progress.h"

#ifdef TL_OS_LINUX
#include <spawn.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#endif
#include <array>
#include <locale>
#include <codecvt>
#include <utility>
#include <cstring>

#ifdef TL_OS_LINUX
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
    if(mStatus == Status::running ||
        mStatus == Status::paused ||
        mStatus == Status::pausing) {
	    TaskBase::stop();
    }
}

auto TaskBase::operator=(const TaskBase &task) -> TaskBase &
{
    if (this != &task) {
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

auto TaskBase::operator=(TaskBase &&task) TL_NOEXCEPT -> TaskBase &
{
    if (this != &task) {
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

void TaskBase::setStatus(Status status)
{
    mStatus = status;

    switch (mStatus) {
    case Status::running:
        eventTriggered(Event::Type::task_running);
        break;
    case Status::pausing:
        eventTriggered(Event::Type::task_pausing);
        break;
    case Status::paused:
        eventTriggered(Event::Type::task_paused);
        break;
    case Status::stopping:
        eventTriggered(Event::Type::task_stopping);
        break;
    case Status::stopped:
        eventTriggered(Event::Type::task_stopped);
        break;
    case Status::finalized:
        eventTriggered(Event::Type::task_finalized);
        break;
    case Status::error:
        eventTriggered(Event::Type::task_error);
        break;
    case Status::start:
        break;
    }
}

void TaskBase::eventTriggered(Event::Type type) const
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

void TaskBase::eventTaskErrorTriggered() const
{
    std::list<TaskErrorEventHandler> event_handler = mTaskErrorEventHandler;
    for (auto &handler : event_handler) {
        handler(mTaskErrorEvent.get());
    }
}

void TaskBase::eventTaskFinalizedTriggered() const
{
    std::list<TaskFinalizedEventHandler> event_handler = mTaskFinalizedEventHandler;
    for (auto &handler : event_handler) {
        handler(mTaskFinalizedEvent.get());
    }
}

void TaskBase::eventTaskPauseTriggered() const
{
    std::list<TaskPauseEventHandler> event_handler = mTaskPauseEventHandler;
    for (auto &handler : event_handler) {
        handler(mTaskPauseEvent.get());
    }
}

void TaskBase::eventTaskPausingTriggered() const
{
    std::list<TaskPausingEventHandler> event_handler = mTaskPausingEventHandler;
    for (auto &handler : event_handler) {
        handler(mTaskPausingEvent.get());
    }
}

void TaskBase::eventTaskResumedTriggered() const
{
    std::list<TaskResumedEventHandler> event_handler = mTaskResumedEventHandler;
    for (auto &handler : event_handler) {
        handler(mTaskResumedEvent.get());
    }
}

void TaskBase::eventTaskRunningTriggered() const
{
    std::list<TaskRunningEventHandler> event_handler = mTaskRunningEventHandler;
    for (auto &handler : event_handler) {
        handler(mTaskRunningEvent.get());
    }
}

void TaskBase::eventTaskStoppedTriggered() const
{
    std::list<TaskStoppedEventHandler> event_handler = mTaskStoppedEventHandler;
    for (auto &handler : event_handler) {
        handler(mTaskStoppedEvent.get());
    }
}

void TaskBase::eventTaskStoppingTriggered() const
{
    std::list<TaskStoppingEventHandler> event_handler = mTaskStoppingEventHandler;
    for (auto &handler : event_handler) {
        handler(mTaskStoppingEvent.get());
    }
}

auto TaskBase::errorEvent() const -> TaskErrorEvent*
{
    return mTaskErrorEvent.get();
}

auto TaskBase::finalizedEvent() const -> TaskFinalizedEvent*
{
    return mTaskFinalizedEvent.get();
}

auto TaskBase::pauseEvent() const -> TaskPauseEvent*
{
    return mTaskPauseEvent.get();
}

TaskPausingEvent *TaskBase::pausingEvent() const
{
    return mTaskPausingEvent.get();
}

auto TaskBase::resumedEvent() const -> TaskResumedEvent*
{
    return mTaskResumedEvent.get();
}

auto TaskBase::runningEvent() const -> TaskRunningEvent*
{
    return mTaskRunningEvent.get();
}

auto TaskBase::stoppedEvent() const -> TaskStoppedEvent*
{
    return mTaskStoppedEvent.get();
}

auto TaskBase::stoppingEvent() const -> TaskStoppingEvent*
{
    return mTaskStoppingEvent.get();
}

void TaskBase::executeTask(Progress *progressBar) TL_NOEXCEPT
{
    if (mStatus != Status::start) return;
       
    try {

        chrono.run();

        setStatus(Status::running);

        execute(progressBar);

        chrono.stop();

        if (mStatus == Status::stopping)
            setStatus(Status::stopped);
        else
            setStatus(Status::finalized);

    } catch (const std::exception &e) {
        printException(e);
        mTaskErrorEvent->setErrorMessage(e.what());
        setStatus(Status::error);
    } catch (...) {
        printException(tl::Exception("Unknown exception"));
        mTaskErrorEvent->setErrorMessage("Unknown exception");
        setStatus(Status::error);
    }

}

void TaskBase::run(Progress *progressBar)
{
    mThread = std::thread(&TaskBase::executeTask, this, progressBar);
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

auto TaskBase::time() const -> double
{
    return chrono.currentTime();
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
    default:
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

auto TaskBase::status() const -> Status
{
    return mStatus;
}





/* Process */

#ifdef TL_OS_WINDOWS


constexpr auto process_bufsize = 4096;
HANDLE pipeReadHandle = nullptr;
HANDLE pipeWriteHandle = nullptr;

unsigned long readFromPipe(void *)
{
    DWORD numberOfBytesRead;
    //char buffer[process_bufsize];
    std::array<char, process_bufsize + 1> buffer{};
    int err = 0;

    for (;;) {
        err = ReadFile(pipeReadHandle, buffer.data(), process_bufsize, &numberOfBytesRead, nullptr);
        if (!err || numberOfBytesRead == 0) continue;
        buffer[static_cast<size_t>(numberOfBytesRead)] = '\0';
        std::cout << buffer.data() << std::endl;

        if (!err) break;
    }

    return 0;
}
#endif

Process::Process(std::string commandText,
                 Priority priority)
  : mCommandText(std::move(commandText)),
    outputHandle(false)
#ifdef TL_OS_WINDOWS
    , mThreadHandle(nullptr)
#endif
{
    setPriority(priority);
#ifdef TL_OS_WINDOWS
    ZeroMemory(&mStartUpInfo, sizeof(mStartUpInfo));
    mStartUpInfo.cb = sizeof(mStartUpInfo);
    mStartUpInfo.dwFlags |= STARTF_USESTDHANDLES;

    ZeroMemory(&mSecurityAttributes, sizeof(mSecurityAttributes));
    mSecurityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
    mSecurityAttributes.bInheritHandle = TRUE;
    mSecurityAttributes.lpSecurityDescriptor = nullptr;

    ZeroMemory(&mProcessInformation, sizeof(mProcessInformation));
#endif
}

Process::~Process() = default;

void Process::execute(Progress *)
{
    try {

#ifdef TL_OS_WINDOWS

        if (outputHandle && !createPipe()) return;

        unsigned long priority = static_cast<unsigned long>(mPriority);

        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        std::wstring command = converter.from_bytes(mCommandText);

        bool success = CreateProcess(nullptr,
                                     const_cast<wchar_t *>(command.c_str()),
                                     nullptr,
                                     nullptr,
                                     /*false,*/ true,
                                     CREATE_NO_WINDOW | priority,
                                     nullptr,
                                     nullptr,
                                     &mStartUpInfo,
                                     &mProcessInformation);


        TL_ASSERT(success, "CreateProcess failed ({}) {}", GetLastError(), formatErrorMsg(GetLastError()));

        // Cerrar el extremo de escritura de la tubería del proceso hijo, ya que no lo necesitamos
        if (outputHandle) {
            CloseHandle(pipeWriteHandle);
            mThreadHandle = CreateThread(0, 0, readFromPipe, nullptr, 0, nullptr);
        }

        DWORD ret = WaitForSingleObject(mProcessInformation.hProcess, INFINITE);
        if (ret == WAIT_OBJECT_0) {

            unsigned long exitCode;

            if (GetExitCodeProcess(mProcessInformation.hProcess, &exitCode) == 0) {
                TL_THROW_EXCEPTION("Error ({}: {}) when executing the command: {}",
                                   GetLastError(),
                                   formatErrorMsg(GetLastError()),
                                   mCommandText);
            }

            if (exitCode == 0){
                eventTriggered(Event::Type::task_finalized);
            } else {
                eventTriggered(Event::Type::task_error);
            }

        } else {

            TL_THROW_EXCEPTION("Error ({}: {}) when executing the command: {}",
                               GetLastError(),
                               formatErrorMsg(GetLastError()),
                               mCommandText);

        }

        CloseHandle(mProcessInformation.hProcess);
        CloseHandle(mProcessInformation.hThread);
        if (outputHandle) {
            CloseHandle(mThreadHandle);
            CloseHandle(pipeReadHandle);
        }

#else
        pid_t pid;
        char *argv[] = {const_cast<char *>("sh"), const_cast<char *>("-c"), mCommandText.data(), nullptr};
        
        int status = posix_spawn(&pid, "/bin/sh", nullptr, nullptr, argv, environ);
        
        TL_ASSERT(status == 0, "Error({}: {}) when executing the command : {}", status, strerror(status), mCommandText);

            if (waitpid(pid, &status, 0) != -1) {
                eventTriggered(Event::Type::task_finalized);
                return;
            } else {
                eventTriggered(Event::Type::task_error);
                return;
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


auto Process::priority() const -> Priority
{
#ifdef TL_OS_WINDOWS
    return static_cast<Priority>(GetPriorityClass(mProcessInformation.hProcess));
#else
    return static_cast<Priority>(getpriority(PRIO_PROCESS, getpid()));
#endif
}

void Process::setPriority(Priority priority)
{
#ifdef TL_OS_WINDOWS
    SetPriorityClass(mProcessInformation.hProcess,
                     static_cast<unsigned long>(priority));
#else
    setpriority(PRIO_PROCESS, getpid(), static_cast<int>(priority));
#endif

    mPriority = priority;
}

#ifdef TL_OS_WINDOWS
//https ://stackoverflow.com/a/69410299

std::wstring string_to_wide_string(const std::string &string)
{
    if (string.empty()) {
        return L"";
    }

    auto size_needed = MultiByteToWideChar(CP_UTF8, 0, &string.at(0), (int)string.size(), nullptr, 0);
    TL_ASSERT(size_needed > 0, "MultiByteToWideChar() failed: {}", size_needed);

    std::wstring result(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &string.at(0), static_cast<int>(string.size()), &result.at(0), size_needed);
    
    return result;
}

std::string wide_string_to_string(const std::wstring &wide_string)
{
    if (wide_string.empty()) {
        return "";
    }

    const auto size_needed = WideCharToMultiByte(CP_UTF8, 0, &wide_string.at(0), static_cast<int>(wide_string.size()), nullptr, 0, nullptr, nullptr);
    TL_ASSERT(size_needed > 0, "WideCharToMultiByte() failed: {}", size_needed);

    std::string result(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wide_string.at(0), static_cast<int>(wide_string.size()), &result.at(0), size_needed, nullptr, nullptr);
    
    return result;
}

auto Process::formatErrorMsg(unsigned long errorCode) -> std::string
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

    std::string strError = wide_string_to_string(errorMessage);

    return strError;
}


auto Process::createPipe() -> bool
{
    if (!CreatePipe(&pipeReadHandle, &pipeWriteHandle, &mSecurityAttributes, 0)) {
        Message::error("CreateProcess failed ({}) {}", GetLastError(), formatErrorMsg(GetLastError()));
        eventTriggered(Event::Type::task_error);
        return false;
    }

    if (!SetHandleInformation(pipeReadHandle, HANDLE_FLAG_INHERIT, 0)) {
        Message::error("CreateProcess failed ({}) {}", GetLastError(), formatErrorMsg(GetLastError()));
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
  : tasks(0)
{
}

TaskList::TaskList(const TaskList &taskList)
  : TaskBase(),
    tasks(taskList.tasks)
{
}

TaskList::TaskList(std::initializer_list<std::shared_ptr<Task>> tasks)
  : tasks(tasks)
{
}

TaskList::~TaskList() = default;

void TaskList::push_back(const std::shared_ptr<Task> &task)
{
    this->tasks.push_back(task);
}

auto TaskList::size() const TL_NOEXCEPT -> size_t
{
    return tasks.size();
}

auto TaskList::empty() const TL_NOEXCEPT -> bool
{
    return tasks.empty();
}

void TaskList::stop()
{
    TaskBase::stop();

    if (status() == Status::stopping) {
        for (const auto &task : tasks) {
            task->stop();
        }
    }
}

void TaskList::execute(Progress *progressBar)
{
    for (const auto &task : tasks) {

        if (status() == Status::stopping) return;

        task->run();

        if (progressBar) (*progressBar)();

    }
}




/* Task Queue */

std::mutex TaskQueue::mtx;

TaskQueue::TaskQueue()
  : TaskBase()
{
}

TaskQueue::~TaskQueue() 
{
	TaskQueue::stop();
}

void TaskQueue::push(std::shared_ptr<Task> task)
{
    queue.push(task);

    if (status() == Status::finalized ||
        status() == Status::stopped) {
        setStatus(Status::start);
        //  setStatus(Status::running);
        //  // Al ser una cola se vuelve a ejecutar cada vez que se añade un elemento
        //  run();
    }

}

void TaskQueue::pop() TL_NOEXCEPT
{
    queue.pop();
}

auto TaskQueue::size() const TL_NOEXCEPT -> size_t
{
    return queue.size();
}

auto TaskQueue::empty() const TL_NOEXCEPT -> bool
{
    return queue.empty();
}

void TaskQueue::stop()
{
    TaskBase::stop();
}

void TaskQueue::execute(Progress */*progressBar*/)
{
    while (!queue.empty()) {
        
        if (status() == Status::stopping || status() == Status::stopped) {

        } else {
            std::lock_guard<std::mutex> lck(TaskQueue::mtx);
            queue.front()->run();
        }

        queue.pop();
    }
}




/* Task Tree */

TaskTree::TaskTree() = default;
TaskTree::~TaskTree() = default;

void TaskTree::addTask(const std::shared_ptr<Task> &task,
                       const std::list<std::shared_ptr<Task>> &parentTasks)
{
    unusedParameter(task, parentTasks);
}

void TaskTree::stop()
{
    TaskBase::stop();
}

void TaskTree::execute(Progress *progressBar)
{
    unusedParameter(progressBar);
}

} // End namespace tl


