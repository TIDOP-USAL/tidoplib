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


#include "tidop/core/task/Task.h"
#include "tidop/core/task/Progress.h"
#include "tidop/core/base/Exception.h"

namespace tl
{

Task::~Task()
{
    auto _status = status();
    if(_status == Status::running ||
       _status == Status::paused ||
       _status == Status::pausing) {
	    Task::stop();
    }
}

void Task::setStatus(Status status)
{
    mStatus.store(status);
}

void Task::interruptionPoint(std::stop_token token) const
{
    if (token.stop_requested())
        TL_THROW_EXCEPTION("Task cancelled");
}

void Task::pausePoint(std::stop_token stopToken)
{
    if (mStatus.load() != Status::pausing) return;

    std::unique_lock lock(mMutex);

    if (mStatus.load() == Status::pausing) {

        setStatus(Status::paused);

        lock.unlock();
        triggerEvent<TaskEventType::task_paused>();
        lock.lock();

        mCV.wait(lock, stopToken, [this]() {
            return mStatus != Status::paused;
            });
    }
}

void Task::executeTask(Progress *progressBar, std::stop_token stopToken) noexcept
{
    if (mStatus.load() != Status::start) return;
       
    ExecutionGuard guard(*this, stopToken);

    try {

        execute(progressBar, stopToken);

    } catch (const std::exception &e) {
        printException(e);
        guard.setFailed(e.what());
        //mTaskErrorEvent->setErrorMessage(e.what());
        //mLastErrorMessage = e.what();
        //setStatus(Status::error);
    } catch (...) {
        printException(tl::Exception("Unknown exception"));
        guard.setFailed("Unknown exception");
        //guard.setFailed(std::current_exception());
        //mTaskErrorEvent->setErrorMessage("Unknown exception");
        //mLastErrorMessage = "Unknown exception";
        //setStatus(Status::error);
    }

}

void Task::run(Progress *progressBar)
{
    this->executeTask(progressBar, std::stop_token{});
}

void Task::runAsync(Progress *progressBar)
{
    mThread = std::jthread([this, progressBar](std::stop_token token) {
        this->executeTask(progressBar, token);
        });
}

void Task::pause()
{
    if (mStatus == Status::running) {
        setStatus(Status::pausing);
        triggerEvent<TaskEventType::task_pausing>();
    }
}

void Task::reset()
{
    setStatus(Status::start);
}

void Task::resume()
{
    {
        std::lock_guard lock(mMutex);

        if (status() == Status::paused || status() == Status::pausing) {
            mStatus = Status::running;
        } else {
            return;
        }
    }

    triggerEvent<TaskEventType::task_resumed>();

    mCV.notify_all();
}

void Task::stop()
{
    if (mStatus == Status::running || mStatus == Status::paused) {
        setStatus(Status::stopping);
        triggerEvent<TaskEventType::task_stopping>();
    }
    mThread.request_stop();
    mCV.notify_all();
}

auto Task::time() const -> double
{
    return chrono.currentTime();
}

void Task::subscribe(TaskEventType eventType,
                     const EventHandler &handler)
{
    std::lock_guard lock(mMutex);
    mEventHandlers[eventType].push_back(handler);
}

void Task::subscribe(const EventHandler &handler)
{
    std::lock_guard lock(mMutex);

    // Esto podria ser un array
    const std::vector<TaskEventType> allTypes = {
        TaskEventType::task_error,    
        TaskEventType::task_paused,
        TaskEventType::task_pausing,  
        TaskEventType::task_resumed,
        TaskEventType::task_running,  
        TaskEventType::task_stopped,
        TaskEventType::task_stopping, 
        TaskEventType::task_finalized
    };

    for (auto type : allTypes) {
        mEventHandlers[type].push_back(handler);
    }
}

auto Task::status() const -> Status
{
    return mStatus.load();
}

Task::ExecutionGuard::ExecutionGuard(Task &task, std::stop_token stopToken)
  : mTask(task), 
    mStopToken(stopToken)
{
    mTask.chrono.run();
    mTask.setStatus(Status::running);
    mTask.triggerEvent<TaskEventType::task_running>();
}

Task::ExecutionGuard::~ExecutionGuard()
{
    mTask.chrono.stop();

    if (mFailed) {
        mTask.setStatus(Status::error);
        mTask.triggerEvent<TaskEventType::task_error>();
    } else if (mStopToken.stop_requested()) {
        mTask.setStatus(Status::stopped);
        mTask.triggerEvent<TaskEventType::task_stopped>();
    } else {
        mTask.setStatus(Status::finalized);
        mTask.triggerEvent<TaskEventType::task_finalized>();
    }
}

void Task::ExecutionGuard::setFailed(const std::string &errorMsg)
{
    mFailed = true;
    mTask.mLastErrorMessage = errorMsg;
}

} // namespace tl


