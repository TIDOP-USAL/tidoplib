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
 * License along with Foobar. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#include "tidop/core/event.h"


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


/* Process Running Event */

ProcessRunningEvent::ProcessRunningEvent()
  : EventBase(Event::Type::process_running)
{
}

void ProcessRunningEvent::clear()
{
}


/* Process Pausing Event */

ProcessPausingEvent::ProcessPausingEvent()
  : EventBase(Event::Type::process_pausing)
{
}

void ProcessPausingEvent::clear()
{
}


/* Process Pause Event */

ProcessPauseEvent::ProcessPauseEvent()
  : EventBase(Event::Type::process_paused)
{
}

void ProcessPauseEvent::clear()
{
}


/* Process Resumed Event */

ProcessResumedEvent::ProcessResumedEvent()
  : EventBase(Event::Type::process_resumed)
{
}

void ProcessResumedEvent::clear()
{
}


/* Process Stopping Event */

ProcessStoppingEvent::ProcessStoppingEvent()
  : EventBase(Event::Type::process_stopping)
{
}

void ProcessStoppingEvent::clear()
{
}


/* Process Stopped Event */

ProcessStoppedEvent::ProcessStoppedEvent()
  : EventBase(Event::Type::process_stopped)
{
}

void ProcessStoppedEvent::clear()
{
}


/* Process Finalized Event */

ProcessFinalizedEvent::ProcessFinalizedEvent()
  : EventBase(Event::Type::process_finalized)
{
}

void ProcessFinalizedEvent::clear()
{
}


/* Process Error Event */

ProcessErrorEvent::ProcessErrorEvent()
  : EventBase(Event::Type::process_error),
    mErrorMessage("")
{
}

std::string ProcessErrorEvent::errorMessage() const
{
  return mErrorMessage;
}

void ProcessErrorEvent::setErrorMessage(const std::string &error)
{
  mErrorMessage = error;
}

void ProcessErrorEvent::clear()
{
  mErrorMessage.clear();
}



/* Image Change Event */

ImageChangeEvent::ImageChangeEvent()
  : EventBase(Event::Type::process_error),
    mImage("")
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
