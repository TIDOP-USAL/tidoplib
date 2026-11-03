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

#include <type_traits>

#include "tidop/core/base/Event.h"

namespace tl
{

/*! \addtogroup Task
 *  \{
 */

template<typename Event>
struct EventTraits;

template<>
struct EventTraits<TaskRunningEvent>
{
    static constexpr TaskEventType type = TaskEventType::task_running;
};

template<>
struct EventTraits<TaskPausingEvent>
{
    static constexpr TaskEventType type = TaskEventType::task_pausing;
};

template<>
struct EventTraits<TaskPauseEvent>
{
    static constexpr TaskEventType type = TaskEventType::task_paused;
};

template<>
struct EventTraits<TaskResumedEvent>
{
    static constexpr TaskEventType type = TaskEventType::task_resumed;
};

template<>
struct EventTraits<TaskStoppingEvent>
{
    static constexpr TaskEventType type = TaskEventType::task_stopping;
};

template<>
struct EventTraits<TaskStoppedEvent>
{
    static constexpr TaskEventType type = TaskEventType::task_stopped;
};

template<>
struct EventTraits<TaskFinalizedEvent>
{
    static constexpr TaskEventType type = TaskEventType::task_finalized;
};

template<>
struct EventTraits<TaskErrorEvent>
{
    static constexpr TaskEventType type = TaskEventType::task_error;
};



template<TaskEventType>
struct EventTypeTraits;

template<>
struct EventTypeTraits<TaskEventType::task_running>
{
    using type = TaskRunningEvent;
};

template<>
struct EventTypeTraits<TaskEventType::task_pausing>
{
    using type = TaskPausingEvent;
};

template<>
struct EventTypeTraits<TaskEventType::task_paused>
{
    using type = TaskPauseEvent;
};

template<>
struct EventTypeTraits<TaskEventType::task_resumed>
{
    using type = TaskResumedEvent;
};

template<>
struct EventTypeTraits<TaskEventType::task_stopping>
{
    using type = TaskStoppingEvent;
};

template<>
struct EventTypeTraits<TaskEventType::task_stopped>
{
    using type = TaskStoppedEvent;
};

template<>
struct EventTypeTraits<TaskEventType::task_finalized>
{
    using type = TaskFinalizedEvent;
};

template<>
struct EventTypeTraits<TaskEventType::task_error>
{
    using type = TaskErrorEvent;
};


/*! \} */

} // namespace tl

