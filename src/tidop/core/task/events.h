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

#include "tidop/core/event.h"


namespace tl
{

class Task;

/*! \addtogroup core
 *  \{
 */



/* Task Running Event */

class TL_EXPORT TaskRunningEvent
    : public EventBase
{

public:

    TaskRunningEvent();

    void clear() override;
};


/* Task Pausing Event */

class TL_EXPORT TaskPausingEvent final
    : public EventBase
{

public:

    TaskPausingEvent();

    void clear() override;
};


/* Task Pause Event */

class TL_EXPORT TaskPauseEvent final
	: public EventBase
{

public:

    TaskPauseEvent();

    void clear() override;
};


/* Task Resumed Event */

class TL_EXPORT TaskResumedEvent final
	: public EventBase
{

public:

    TaskResumedEvent();

    void clear() override;
};


/* Task Stopping Event */

class TL_EXPORT TaskStoppingEvent final
    : public EventBase
{

public:

    TaskStoppingEvent();

    void clear() override;
};


/* Task Stopped Event */

class TL_EXPORT TaskStoppedEvent final
    : public EventBase
{

public:

    TaskStoppedEvent();

    void clear() override;
};


/* Task Finalized Event */

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


/* Task Error Event */

class TL_EXPORT TaskErrorEvent final
    : public EventBase
{

public:

    TaskErrorEvent();

    std::string errorMessage() const;
    void setErrorMessage(const std::string &error);

    void clear() override;

private:

    std::string mErrorMessage;
};


/*! \} */ // end of core

} // namespace tl

