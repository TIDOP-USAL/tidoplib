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


#include "tidop/core/task/task.h"
#include "tidop/core/task/progress.h"

namespace tl
{

/* Task */

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


} // End namespace tl


