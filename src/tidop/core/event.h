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

#ifndef TL_CORE_EVENT_H
#define TL_CORE_EVENT_H

#include "config_tl.h"

#include <string>

#include "tidop/core/defs.h"


namespace tl
{

/*! \addtogroup core
 *  \{
 */

class TL_EXPORT Event
{

public:

  enum class Type
  {
    /* Task events*/

    task_running,
    task_pausing,
    task_paused,
    task_resumed,
    task_stopping,
    task_stopped,
    task_finalized,
    task_error,

    /* Image events */

    image_change
  };

public:

  Event() = default;

  virtual Type type() const = 0;
  virtual void clear() = 0;

};


class TL_EXPORT EventBase
  : public Event
{

public:

  EventBase(Type type);

  Type type() const override;

private:

  Type mType;

};



/* Task Running Event */

class TL_EXPORT TaskRunningEvent
  : public EventBase
{

public:

  TaskRunningEvent();

  void clear() override;
};


/* Task Pausing Event */

class TL_EXPORT TaskPausingEvent
  : public EventBase
{

public:

  TaskPausingEvent();

  void clear() override;
};


/* Task Pause Event */

class TL_EXPORT TaskPauseEvent
  : public EventBase
{

public:

  TaskPauseEvent();

  void clear() override;
};


/* Task Resumed Event */

class TL_EXPORT TaskResumedEvent
  : public EventBase
{

public:

  TaskResumedEvent();

  void clear() override;
};


/* Task Stopping Event */

class TL_EXPORT TaskStoppingEvent
  : public EventBase
{

public:

  TaskStoppingEvent();

  void clear() override;
};


/* Task Stopped Event */

class TL_EXPORT TaskStoppedEvent
  : public EventBase
{

public:

  TaskStoppedEvent();

  void clear() override;
};


/* Task Finalized Event */

class TL_EXPORT TaskFinalizedEvent
  : public EventBase
{

public:

  TaskFinalizedEvent();

  void clear() override;
};


/* Task Error Event */

class TL_EXPORT TaskErrorEvent
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


/* Image Change Event */

class TL_EXPORT ImageChangeEvent
  : public EventBase
{

public:

  ImageChangeEvent();

  std::string image() const;
  void setImage(const std::string &image);

  void clear() override;

private:

  std::string mImage;

};

/*! \} */ // end of core

} // namespace tl


#endif // TL_CORE_EVENT_H
