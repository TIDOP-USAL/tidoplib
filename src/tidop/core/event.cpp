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

#include "tidop/core/event.h"

#include "tidop/core/task.h"

namespace tl
{



EventBase::EventBase(Type type)
  : mType(type)
{
}

EventBase::Type EventBase::type() const
{
  return mType;
}


/* Task Running Event */

TaskRunningEvent::TaskRunningEvent()
  : EventBase(Type::task_running)
{
}

void TaskRunningEvent::clear()
{
}


/* Task Pausing Event */

TaskPausingEvent::TaskPausingEvent()
  : EventBase(Type::task_pausing)
{
}

void TaskPausingEvent::clear()
{
}


/* Task Pause Event */

TaskPauseEvent::TaskPauseEvent()
  : EventBase(Type::task_paused)
{
}

void TaskPauseEvent::clear()
{
}


/* Task Resumed Event */

TaskResumedEvent::TaskResumedEvent()
  : EventBase(Type::task_resumed)
{
}

void TaskResumedEvent::clear()
{
}


/* Task Stopping Event */

TaskStoppingEvent::TaskStoppingEvent()
  : EventBase(Type::task_stopping)
{
}

void TaskStoppingEvent::clear()
{
}


/* Task Stopped Event */

TaskStoppedEvent::TaskStoppedEvent()
  : EventBase(Type::task_stopped)
{
}

void TaskStoppedEvent::clear()
{
}


/* Task Finalized Event */

TaskFinalizedEvent::TaskFinalizedEvent(Task const *task)
  : EventBase(Type::task_finalized),
    mTask(task)
{
}

Task const *TaskFinalizedEvent::task() const
{
    return mTask;
}

void TaskFinalizedEvent::clear()
{
}


/* Task Error Event */

TaskErrorEvent::TaskErrorEvent()
  : EventBase(Type::task_error)
{
}

std::string TaskErrorEvent::errorMessage() const
{
    return mErrorMessage;
}

void TaskErrorEvent::setErrorMessage(const std::string &error)
{
    mErrorMessage = error;
}

void TaskErrorEvent::clear()
{
    mErrorMessage.clear();
}



/* Image Change Event */

ImageChangeEvent::ImageChangeEvent()
  : EventBase(Type::image_change)
{
}

std::string ImageChangeEvent::image() const
{
    return mImage;
}

void ImageChangeEvent::setImage(const std::string &image)
{
    mImage = image;
}

void ImageChangeEvent::clear()
{
    mImage.clear();
}

} // namespace tl
