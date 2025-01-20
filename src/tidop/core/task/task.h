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
#include "tidop/core/base/chrono.h"

namespace tl
{

class Progress;

/*!
 * \addtogroup Task
 * \{
 */

 /*!
  * \class Task
  *
  * \brief Abstract base class for defining and managing tasks.
  *
  * This class is intended to be subclassed for specific task implementations. It enforces the definition of key methods
  * related to task control and provides a unified interface for task management.
  */
class TL_EXPORT Task
{

public:

    /*!
     * \enum Status
     * \brief Represents the lifecycle status of a task.
     *
     * Possible values are:
     * - `start`: The initial state of the task.
     * - `running`: The task is currently running.
     * - `pausing`: The task is in the process of pausing.
     * - `paused`: The task is paused.
     * - `stopping`: The task is in the process of stopping.
     * - `stopped`: The task has been stopped by the user.
     * - `finalized`: The task has completed successfully.
     * - `error`: The task encountered an error.
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

    /*!
     * \typedef Task::EventHandler
     * \brief Function type for handling general task events.
     * \param event Pointer to the `Event` object.
     */
    using EventHandler = typename std::function<void (Event *)>;

    /*!
     * \typedef Task::TaskErrorEventHandler
     * \brief Function type for handling task error events.
     * \param event Pointer to the `TaskErrorEvent` object.
     */
    using TaskErrorEventHandler = typename std::function<void (TaskErrorEvent *)>;

    /*!
     * \typedef Task::TaskFinalizedEventHandler
     * \brief Function type for handling task finalized events.
     * \param event Pointer to the `TaskFinalizedEvent` object.
     */
    using TaskFinalizedEventHandler = typename std::function<void (TaskFinalizedEvent *)>;

    /*!
     * \typedef Task::TaskPauseEventHandler
     * \brief Function type for handling task pause events.
     * \param event Pointer to the `TaskPauseEvent` object.
     */
    using TaskPauseEventHandler = typename std::function<void (TaskPauseEvent *)>;

    /*!
     * \typedef Task::TaskPausingEventHandler
     * \brief Function type for handling task pausing events.
     * \param event Pointer to the `TaskPausingEvent` object.
     */
    using TaskPausingEventHandler = typename std::function<void (TaskPausingEvent *)>;

    /*!
     * \typedef Task::TaskResumedEventHandler
     * \brief Function type for handling task resumed events.
     * \param event Pointer to the `TaskResumedEvent` object.
     */
    using TaskResumedEventHandler = typename std::function<void (TaskResumedEvent *)>;

    /*!
     * \typedef Task::TaskRunningEventHandler
     * \brief Function type for handling task running events.
     * \param event Pointer to the `TaskRunningEvent` object.
     */
    using TaskRunningEventHandler = typename std::function<void (TaskRunningEvent *)>;

    /*!
     * \typedef Task::TaskStoppedEventHandler
     * \brief Function type for handling task stopped events.
     * \param event Pointer to the `TaskStoppedEvent` object.
     */
    using TaskStoppedEventHandler = typename std::function<void (TaskStoppedEvent *)>;

    /*!
     * \typedef Task::TaskStoppingEventHandler
     * \brief Function type for handling task stopping events.
     * \param event Pointer to the `TaskStoppingEvent` object.
     */
    using TaskStoppingEventHandler = typename std::function<void (TaskStoppingEvent *)>;

public:

    /*!
     * \brief Default constructor.
     */
    Task();
    virtual ~Task();
    
    TL_DISABLE_COPY(Task)
    TL_DISABLE_MOVE(Task)
    
    /*!
     * \brief Starts the task.
     * 
     * \param progressBar Optional pointer to a `Progress` object for tracking task progress.
     * 
     * \note This method must be implemented by subclasses.
     */
    virtual void run(Progress *progressBar = nullptr) = 0;
    
    /*!
     * \brief Starts the task asynchronously.
     *
     * \param progressBar Optional pointer to a `Progress` object for tracking task progress.
     *
     * \note This method must be implemented by subclasses.
     */
    virtual void runAsync(Progress *progressBar = nullptr) = 0;
    
    /*!
     * \brief Pauses the task.
     *
     * \note This method must be implemented by subclasses.
     */
    virtual void pause() = 0;
    
    /*!
     * \brief Resets the task to its initial state.
     *
     * \note This method must be implemented by subclasses.
     */
    virtual void reset() = 0;
    
    /*!
     * \brief Resumes the task from a paused state.
     *
     * \note This method must be implemented by subclasses.
     */
    virtual void resume() = 0;
    
    /*!
     * \brief Stops the task.
     *
     * \note This method must be implemented by subclasses.
     */
    virtual void stop() = 0;

    /*!
     * \brief Gets the elapsed time of the task.
     *
     * \return The elapsed time as a double.
     *
     * \note This method must be implemented by subclasses.
     */
    virtual auto time() const -> double = 0;

    /*!
     * \brief Subscribes to a specific event type.
     *
     * \param[in] eventType The type of event to subscribe to.
     * \param[in] eventHandler The event handler to be called when the event occurs.
     */
    virtual void subscribe(Event::Type eventType,
                           const EventHandler &eventHandler) = 0;

    /*!
     * \brief Subscribes to general events.
     *
     * \param[in] eventHandler The event handler to be called for any event.
     */
    virtual void subscribe(const EventHandler &eventHandler) = 0;

    /*!
     * \brief Subscribes to task error events.
     *
     * \param[in] eventHandler The event handler to be called when a task error occurs.
     */
    virtual void subscribe(const TaskErrorEventHandler &eventHandler) = 0;

    /*!
     * \brief Subscribes to task finalized events.
     *
     * \param[in] eventHandler The event handler to be called when a task is finalized.
     */
    virtual void subscribe(const TaskFinalizedEventHandler &eventHandler) = 0;

    /*!
     * \brief Subscribes to task pause events.
     *
     * \param[in] eventHandler The event handler to be called when a task is paused.
     */
    virtual void subscribe(const TaskPauseEventHandler &eventHandler) = 0;

    /*!
     * \brief Subscribes to task pausing events.
     *
     * \param[in] eventHandler The event handler to be called when a task is pausing.
     */
    virtual void subscribe(const TaskPausingEventHandler &eventHandler) = 0;

    /*!
     * \brief Subscribes to task resumed events.
     *
     * \param[in] eventHandler The event handler to be called when a task is resumed.
     */
    virtual void subscribe(const TaskResumedEventHandler &eventHandler) = 0;

    /*!
     * \brief Subscribes to task running events.
     *
     * \param[in] eventHandler The event handler to be called when a task is running.
     */
    virtual void subscribe(const TaskRunningEventHandler &eventHandler) = 0;

    /*!
     * \brief Subscribes to task stopped events.
     *
     * \param[in] eventHandler The event handler to be called when a task is stopped.
     */
    virtual void subscribe(const TaskStoppedEventHandler &eventHandler) = 0;

    /*!
     * \brief Subscribes to task stopping events.
     *
     * \param[in] eventHandler The event handler to be called when a task is stopping.
     */
    virtual void subscribe(const TaskStoppingEventHandler &eventHandler) = 0;
    
    /*!
     * \brief Gets the current status of the task.
     *
     * \return The current status as a `Status` value.
     */
    virtual auto status() const -> Status = 0;
};



/*! 
 * \class TaskBase
 * \brief Base class for defining and managing tasks.
 *
 * This class is intended to be subclassed for specific task implementations. It provides a default implementation of the
 * `Task` interface and manages the task lifecycle and event handling.
 */
class TL_EXPORT TaskBase
  : public Task
{

private:

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

    /*!
     * \brief Default constructor.
     */
    TaskBase();

    /*!
     * \brief Copy constructor.
     * \param[in] task The TaskBase object to copy.
     */
    TaskBase(const TaskBase &task);

    /*!
     * \brief Move constructor.
     * \param[in] task The TaskBase object to move.
     */
    TaskBase(TaskBase &&task) TL_NOEXCEPT;

    ~TaskBase() override;
   
    /*!
     * \brief Copy assignment operator.
     * \param[in] task The TaskBase object to copy.
     * \return A reference to the assigned object.
     */
    auto operator=(const TaskBase &task)->TaskBase &;

    /*!
     * \brief Move assignment operator.
     * \param[in] task The TaskBase object to move.
     * \return A reference to the assigned object.
     */
    auto operator=(TaskBase &&task) TL_NOEXCEPT->TaskBase &;

protected:

    /*!
     * \brief Sets the current status of the task.
     * \param[in] status The new status.
     */
    void setStatus(Status status);

    /*!
     * \brief Triggers an event of a specific type.
     * \param[in] type The type of event to trigger.
     */
    void eventTriggered(Event::Type type) const;

    /*!
     * \brief Triggers a task error event.
     */
    void eventTaskErrorTriggered() const;

    /*!
     * \brief Triggers a task finalized event.
     */
    void eventTaskFinalizedTriggered() const;

    /*!
     * \brief Triggers a task pause event.
     */
    void eventTaskPauseTriggered() const;

    /*!
     * \brief Triggers a task pausing event.
     */
    void eventTaskPausingTriggered() const;

    /*!
     * \brief Triggers a task resumed event.
     */
    void eventTaskResumedTriggered() const;

    /*!
     * \brief Triggers a task running event.
     */
    void eventTaskRunningTriggered() const;

    /*!
     * \brief Triggers a task stopped event.
     */
    void eventTaskStoppedTriggered() const;

    /*!
     * \brief Triggers a task stopping event.
     */
    void eventTaskStoppingTriggered() const;

    /*!
     * \brief Gets the task error event object.
     * \return A pointer to the task error event.
     */
    auto errorEvent() const -> TaskErrorEvent*;

    /*!
     * \brief Gets the task finalized event object.
     * \return A pointer to the task finalized event.
     */
    auto finalizedEvent() const -> TaskFinalizedEvent*;

    /*!
     * \brief Gets the task pause event object.
     * \return A pointer to the task pause event.
     */
    auto pauseEvent() const -> TaskPauseEvent*;

    /*!
     * \brief Gets the task pausing event object.
     * \return A pointer to the task pausing event.
     */
    auto pausingEvent() const -> TaskPausingEvent*;

    /*!
     * \brief Gets the task resumed event object.
     * \return A pointer to the task resumed event.
     */
    auto resumedEvent() const -> TaskResumedEvent*;

    /*!
     * \brief Gets the task running event object.
     * \return A pointer to the task running event.
     */
    auto runningEvent() const -> TaskRunningEvent*;

    /*!
     * \brief Gets the task stopped event object.
     * \return A pointer to the task stopped event.
     */
    auto stoppedEvent() const -> TaskStoppedEvent*;

    /*!
     * \brief Gets the task stopping event object.
     * \return A pointer to the task stopping event.
     */
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

/*! \} */

} // End namespace tl

