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

#pragma once

#include "tidop/config.h"

#include <thread>
#include <mutex>
#include <functional>
#include <list>
#include <memory>

#include "tidop/core/task/events.h"
#include "tidop/core/chrono.h"

namespace tl
{

class Progress;

/*!
 * \addtogroup core
 * \{
 */


class TL_EXPORT Task
{

public:

    /*!
     * \brief Task status
     */
    enum class Status
    {
        start,             /*!< Init status */
        running,           /*!< Running task */
        pausing,           /*!< Task pausing */
        paused,            /*!< Paused task */
        stopping,          /*!< Task stopping */
        stopped,           /*!< Task stopped by user */
        finalized,         /*!< Task completed */
        error              /*!< Task finished with error */
    };
    
    using EventHandler = typename std::function<void (Event *)>;
    using TaskErrorEventHandler = typename std::function<void (TaskErrorEvent *)>;
    using TaskFinalizedEventHandler = typename std::function<void (TaskFinalizedEvent *)>;
    using TaskPauseEventHandler = typename std::function<void (TaskPauseEvent *)>;
    using TaskPausingEventHandler = typename std::function<void (TaskPausingEvent *)>;
    using TaskResumedEventHandler = typename std::function<void (TaskResumedEvent *)>;
    using TaskRunningEventHandler = typename std::function<void (TaskRunningEvent *)>;
    using TaskStoppedEventHandler = typename std::function<void (TaskStoppedEvent *)>;
    using TaskStoppingEventHandler = typename std::function<void (TaskStoppingEvent *)>;

public:

    Task();
    virtual ~Task();
    
    TL_DISABLE_COPY(Task)
    TL_DISABLE_MOVE(Task)
    
    /*!
     * \brief Start the process
     */
    virtual void run(Progress *progressBar = nullptr) = 0;
    
    /*!
     * \brief Starts the process asynchronously
     */
    virtual void runAsync(Progress *progressBar = nullptr) = 0;
    
    /*!
     * \brief Pause the process
     */
    virtual void pause() = 0;
    
    /*!
     * \brief Restart the process
     */
    virtual void reset() = 0;
    
    /*!
     * \brief Continue running the process
     */
    virtual void resume() = 0;
    
    /*!
     * \brief Stops the process
     */
    virtual void stop() = 0;
    
    virtual auto time() const -> double = 0;

    virtual void subscribe(Event::Type eventType,
                           const EventHandler &eventHandler) = 0;
    virtual void subscribe(const EventHandler &eventHandler) = 0;
    virtual void subscribe(const TaskErrorEventHandler &eventHandler) = 0;
    virtual void subscribe(const TaskFinalizedEventHandler &eventHandler) = 0;
    virtual void subscribe(const TaskPauseEventHandler &eventHandler) = 0;
    virtual void subscribe(const TaskPausingEventHandler &eventHandler) = 0;
    virtual void subscribe(const TaskResumedEventHandler &eventHandler) = 0;
    virtual void subscribe(const TaskRunningEventHandler &eventHandler) = 0;
    virtual void subscribe(const TaskStoppedEventHandler &eventHandler) = 0;
    virtual void subscribe(const TaskStoppingEventHandler &eventHandler) = 0;
    
    virtual auto status() const -> Status = 0;
};



class TL_EXPORT TaskBase
  : public Task
{

private:

    /// TODO: añadir clase chrono para medir tiempos
    Status mStatus{Status::start};
    std::thread mThread;
    std::mutex mMutex;
    std::unique_ptr<TaskErrorEvent> mTaskErrorEvent;
    std::unique_ptr<TaskFinalizedEvent> mTaskFinalizedEvent;
    std::unique_ptr<TaskPauseEvent> mTaskPauseEvent;
    std::unique_ptr<TaskPausingEvent> mTaskPausingEvent;
    std::unique_ptr<TaskResumedEvent> mTaskResumedEvent;
    std::unique_ptr<TaskRunningEvent> mTaskRunningEvent;
    std::unique_ptr<TaskStoppedEvent> mTaskStoppedEvent;
    std::unique_ptr<TaskStoppingEvent> mTaskStoppingEvent;
    std::list<TaskErrorEventHandler> mTaskErrorEventHandler;
    std::list<TaskFinalizedEventHandler> mTaskFinalizedEventHandler;
    std::list<TaskPauseEventHandler> mTaskPauseEventHandler;
    std::list<TaskPausingEventHandler> mTaskPausingEventHandler;
    std::list<TaskResumedEventHandler> mTaskResumedEventHandler;
    std::list<TaskRunningEventHandler> mTaskRunningEventHandler;
    std::list<TaskStoppedEventHandler> mTaskStoppedEventHandler;
    std::list<TaskStoppingEventHandler> mTaskStoppingEventHandler;
    mutable Chrono chrono;

public:

    TaskBase();
    TaskBase(const TaskBase &task);
    TaskBase(TaskBase &&task) TL_NOEXCEPT;
    ~TaskBase() override;
   
    auto operator=(const TaskBase &task) -> TaskBase &;
    auto operator=(TaskBase &&task) TL_NOEXCEPT -> TaskBase &;

protected:

    void setStatus(Status status);
    
    void eventTriggered(Event::Type type) const;
    void eventTaskErrorTriggered() const;
    void eventTaskFinalizedTriggered() const;
    void eventTaskPauseTriggered() const;
    void eventTaskPausingTriggered() const;
    void eventTaskResumedTriggered() const;
    void eventTaskRunningTriggered() const;
    void eventTaskStoppedTriggered() const;
    void eventTaskStoppingTriggered() const;

    auto errorEvent() const -> TaskErrorEvent*;
    auto finalizedEvent() const -> TaskFinalizedEvent*;
    auto pauseEvent() const -> TaskPauseEvent*;
    auto pausingEvent() const -> TaskPausingEvent*;
    auto resumedEvent() const -> TaskResumedEvent*;
    auto runningEvent() const -> TaskRunningEvent*;
    auto stoppedEvent() const -> TaskStoppedEvent*;
    auto stoppingEvent() const -> TaskStoppingEvent*;

private:

    void executeTask(Progress *progressBar) TL_NOEXCEPT;

protected:

    /*!
     * \brief Execute the task
     */
    virtual void execute(Progress *progressBar = nullptr) = 0;


// Task interface

public:

    void run(Progress *progressBar = nullptr) final;
    void runAsync(Progress *progressBar = nullptr) final;
    void pause() override;
    void reset() override;
    void resume() override;
    void stop() override;
    auto time() const -> double override;

    void subscribe(Event::Type eventType,
                   const EventHandler &eventHandler) override;
    void subscribe(const EventHandler &eventHandler) override;
    void subscribe(const TaskErrorEventHandler &eventHandler) override;
    void subscribe(const TaskFinalizedEventHandler &eventHandler) override;
    void subscribe(const TaskPauseEventHandler &eventHandler) override;
    void subscribe(const TaskPausingEventHandler &eventHandler) override;
    void subscribe(const TaskResumedEventHandler &eventHandler) override;
    void subscribe(const TaskRunningEventHandler &eventHandler) override;
    void subscribe(const TaskStoppedEventHandler &eventHandler) override;
    void subscribe(const TaskStoppingEventHandler &eventHandler) override;
   
    auto status() const -> Status override;

};

/*! \} */ // end of core

} // End namespace tl

