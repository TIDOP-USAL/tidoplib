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
#include <stop_token>
#include <condition_variable>

#include "tidop/core/base/Chrono.h"
#include "tidop/core/task/events/Events.h"
#include "tidop/core/task/events/Concepts.h"

namespace tl
{

class Progress;

/*!
 * \addtogroup Task
 * \{
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
    using EventHandler = typename std::function<void(Event &)>;

private:

    std::atomic<Status> mStatus{Status::start};
    std::jthread mThread;
    mutable std::mutex mMutex;
    std::unordered_map<TaskEventType, std::vector<EventHandler>> mEventHandlers;
    mutable Chrono chrono;
    std::condition_variable_any mCV;
    std::string mLastErrorMessage;
    // reemplazar mLastErrorMessage
    //std::exception_ptr mException;

private: 

    class ExecutionGuard
    {

    public:

        ExecutionGuard(Task &task, std::stop_token stopToken);
        ~ExecutionGuard();
        void setFailed(const std::string &errorMsg);

    private:

        Task &mTask;
        std::stop_token mStopToken;
        bool mFailed{false};
    };

public:

    /*!
     * \brief Default constructor.
     */
    Task() = default;

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
    void run(Progress *progressBar = nullptr);

    /*!
     * \brief Starts the task asynchronously.
     *
     * \param progressBar Optional pointer to a `Progress` object for tracking task progress.
     *
     * \note This method must be implemented by subclasses.
     */
    void runAsync(Progress *progressBar = nullptr);

    /*!
     * \brief Pauses the task.
     *
     * \note This method must be implemented by subclasses.
     */
    void pause();

    /*!
     * \brief Resets the task to its initial state.
     *
     * \note This method must be implemented by subclasses.
     */
    void reset();

    /*!
     * \brief Resumes the task from a paused state.
     *
     * \note This method must be implemented by subclasses.
     */
    void resume();

    /*!
     * \brief Stops the task.
     *
     * \note This method must be implemented by subclasses.
     */
    virtual void stop();

    /*!
     * \brief Gets the elapsed time of the task.
     *
     * \return The elapsed time as a double.
     *
     * \note This method must be implemented by subclasses.
     */
    auto time() const -> double;

    /*!
     * \brief Subscribes to a specific event type.
     *
     * \param[in] eventType The type of event to subscribe to.
     * \param[in] handler The event handler to be called when the event occurs.
     */
    void subscribe(TaskEventType eventType, const EventHandler &handler);

    /*!
     * \brief Subscribes to general events.
     *
     * \param[in] handler The event handler to be called for any event.
     */
    void subscribe(const EventHandler &handler);

    template<TaskEventHandlerConcept Handler>
    void subscribe(Handler &&handler)
    {
        using EventT = typename CallableTraits<std::decay_t<Handler>>::argument_type;

        subscribe(EventTraits<EventT>::type, [h = std::forward<Handler>(handler)](Event &e) {
                std::invoke(h, static_cast<EventT &>(e));
            });
    }

    /*!
     * \brief Gets the current status of the task.
     *
     * \return The current status as a `Status` value.
     */
    auto status() const -> Status;

private:

    /*!
     * \brief Sets the current status of the task.
     * \param[in] status The new status.
     */
    void setStatus(Status status);
//    void changeStatus(Status status);

protected:

    /*!
     * \brief Triggers an event of a specific type.
     * \param[in] type The type of event to trigger.
     */
    //void triggerEvent(TaskEventType type) const;

    template<TaskEventType T>
    void triggerEvent()
    {
        using Event = typename EventTypeTraits<T>::type;

        Event event = [&]() {
            if constexpr (std::is_constructible_v<Event, const Task *>) {
                return Event(this);
            } else {
                return Event{};
            }
        }();

        if constexpr (T == TaskEventType::task_error) {
            event.setErrorMessage(mLastErrorMessage);
        }

        TaskEventType type = T;

        std::vector<EventHandler> handlersToCall;
        {
            std::lock_guard lock(mMutex);
            auto it = mEventHandlers.find(T);
            if (it != mEventHandlers.end()) {
                handlersToCall = it->second;
            }
        }

        if (handlersToCall.empty()) return;

        for (const auto &handler : handlersToCall) {
            handler(event);
        }

    }

    void interruptionPoint(std::stop_token token) const;
    void pausePoint(std::stop_token stopToken);

private:

    void executeTask(Progress *progressBar, std::stop_token stopToken) noexcept;

protected:

    /*!
     * \brief Execute the task
     */
    virtual void execute(Progress *progressBar, std::stop_token stopToken) = 0;

};


/*! \} */

} // End namespace tl

