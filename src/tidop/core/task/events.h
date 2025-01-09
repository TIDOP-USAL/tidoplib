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

#include <string>

#include "tidop/core/base/event.h"


namespace tl
{

class Task;


/*! \addtogroup Task
 *  \{
 */


/*!
 * \class TaskRunningEvent
 * \brief Event triggered when a task starts running.
 * 
 * This event is dispatched when a task enters the "running" state.
 */
class TL_EXPORT TaskRunningEvent
    : public EventBase
{

public:

    TaskRunningEvent();

    void clear() override;
};


/*!
 * \class TaskPausingEvent
 * \brief Event triggered when a task is pausing.
 *
 * This event is dispatched when a task transitions to the "pausing" state.
 */
class TL_EXPORT TaskPausingEvent final
    : public EventBase
{

public:

    TaskPausingEvent();

    void clear() override;
};


/*!
 * \class TaskPauseEvent
 * \brief Event triggered when a task has paused.
 *
 * This event is dispatched when a task enters the "paused" state.
 */
class TL_EXPORT TaskPauseEvent final
	: public EventBase
{

public:

    TaskPauseEvent();

    void clear() override;
};


/*!
 * \class TaskResumedEvent
 * \brief Event triggered when a task resumes.
 *
 * This event is dispatched when a task transitions from "paused" to "running."
 */
class TL_EXPORT TaskResumedEvent final
	: public EventBase
{

public:

    TaskResumedEvent();

    void clear() override;
};


/*!
 * \class TaskStoppingEvent
 * \brief Event triggered when a task is stopping.
 *
 * This event is dispatched when a task transitions to the "stopping" state.
 */
class TL_EXPORT TaskStoppingEvent final
    : public EventBase
{

public:

    TaskStoppingEvent();

    void clear() override;
};


/*!
 * \class TaskStoppedEvent
 * \brief Event triggered when a task has stopped.
 *
 * This event is dispatched when a task enters the "stopped" state.
 */
class TL_EXPORT TaskStoppedEvent final
    : public EventBase
{

public:

    TaskStoppedEvent();

    void clear() override;
};


/*!
 * \class TaskFinalizedEvent
 * \brief Event triggered when a task is finalized.
 *
 * This event is dispatched when a task completes successfully.
 */
class TL_EXPORT TaskFinalizedEvent final
    : public EventBase
{

public:

	explicit TaskFinalizedEvent(Task const *task = nullptr);

    Task const *task() const;

    void clear() override;

private:

    Task const *mTask;
};


/*!
 * \class TaskErrorEvent
 * \brief Event triggered when a task encounters an error.
 *
 * This event is dispatched when a task transitions to the "error" state.
 */
class TL_EXPORT TaskErrorEvent final
    : public EventBase
{

public:

    TaskErrorEvent();

    /*!
     * \brief Gets the error message associated with the event.
     * \return The error message as a string.
     */
    std::string errorMessage() const;

    /*!
     * \brief Sets the error message for the event.
     * \param[in] error The error message to set.
     */
    void setErrorMessage(const std::string &error);

    void clear() override;

private:

    std::string mErrorMessage;
};


/*! \} */

} // namespace tl

