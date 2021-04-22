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


#include "tidop/core/process.h"


namespace tl
{

/* Process */

Process::Process()
{
}

Process::~Process()
{
}



/* ProcessBase */


ProcessBase::ProcessBase()
  : Process(),
  mStatus(Status::start),
  mProcessErrorEvent(new ProcessErrorEvent),
  mProcessFinalizedEvent(new ProcessFinalizedEvent),
  mProcessPauseEvent(new ProcessPauseEvent),
  mProcessPausingEvent(new ProcessPausingEvent),
  mProcessResumedEvent(new ProcessResumedEvent),
  mProcessRunningEvent(new ProcessRunningEvent),
  mProcessStoppedEvent(new ProcessStoppedEvent),
  mProcessStoppingEvent(new ProcessStoppingEvent),
  mProcessErrorEventHandler(0),
  mProcessFinalizedEventHandler(0),
  mProcessPauseEventHandler(0),
  mProcessPausingEventHandler(0),
  mProcessResumedEventHandler(0),
  mProcessRunningEventHandler(0),
  mProcessStoppedEventHandler(0),
  mProcessStoppingEventHandler(0)
{

}

ProcessBase::~ProcessBase()
{
  if (mStatus == Status::running ||
      mStatus == Status::paused ||
      mStatus == Status::pausing) {
    stop();
  }
  if (mStatus != Status::finalized) {
    mStatus = Status::finalized;
    eventTriggered(Event::Type::process_finalized);
  }

  if (mProcessErrorEvent) {
    delete mProcessErrorEvent;
    mProcessErrorEvent = nullptr;
  }

  if (mProcessFinalizedEvent) {
    delete mProcessFinalizedEvent;
    mProcessFinalizedEvent = nullptr;
  }

  if (mProcessPauseEvent) {
    delete mProcessPauseEvent;
    mProcessPauseEvent = nullptr;
  }

  if (mProcessPausingEvent) {
    delete mProcessPausingEvent;
    mProcessPausingEvent = nullptr;
  }

  if (mProcessResumedEvent) {
    delete mProcessResumedEvent;
    mProcessResumedEvent = nullptr;
  }

  if (mProcessRunningEvent) {
    delete mProcessRunningEvent;
    mProcessRunningEvent = nullptr;
  }

  if (mProcessStoppedEvent) {
    delete mProcessStoppedEvent;
    mProcessStoppedEvent = nullptr;
  }

  if (mProcessStoppingEvent) {
    delete mProcessStoppingEvent;
    mProcessStoppingEvent = nullptr;
  }
}

void ProcessBase::run(Progress *progressBar)
{

  if (mStatus == Status::finalized) return;

  try {

    mStatus = Status::running;
    eventTriggered(Event::Type::process_running);

    mThread = std::move(std::thread(&ProcessBase::execute, this, progressBar));
    mThread.join();

    mStatus = Status::finalized;
    eventTriggered(Event::Type::process_finalized);

  } catch (const std::exception &e) {
    mStatus = Status::error;
    mProcessErrorEvent->setErrorMessage(e.what());
    eventTriggered(Event::Type::process_error);
  }
}

void ProcessBase::runAsync(Progress *progressBar)
{
  if (mStatus == Status::finalized) return;

  mStatus = Status::running;
  eventTriggered(Event::Type::process_running);

  mThread = std::thread(&ProcessBase::execute, this, progressBar);
  mThread.detach();

  TL_TODO("Hay que activar Status::finalized")
}

void ProcessBase::pause()
{
  if (mStatus == Status::running) {
    mStatus = Status::pausing;
    eventTriggered(Event::Type::process_pausing);

    TL_TODO("Revisar")
    //while (mStatus != Status::paused || mStatus != Status::finalized || mStatus != Status::error);

    eventTriggered(Event::Type::process_paused);
  }
}

void ProcessBase::reset()
{
  mStatus = Status::start;
  mProcessErrorEventHandler.clear();
  mProcessFinalizedEventHandler.clear();
  mProcessPauseEventHandler.clear();
  mProcessPausingEventHandler.clear();
  mProcessResumedEventHandler.clear();
  mProcessRunningEventHandler.clear();
  mProcessStoppedEventHandler.clear();
  mProcessStoppingEventHandler.clear();
}

void ProcessBase::resume()
{
  eventTriggered(Event::Type::process_resumed);

  if (mStatus == Status::paused || mStatus == Status::pausing) {
    mStatus = Status::running;
    eventTriggered(Event::Type::process_running);
  }
}

void ProcessBase::stop()
{
  if (mStatus == Status::running ||
      mStatus == Status::paused ||
      mStatus == Status::pausing) {

    mStatus = Status::stopping;
    eventTriggered(Event::Type::process_stopping);

    TL_TODO("Revisar")
    //while (mStatus != Status::stopped || mStatus != Status::finalized || mStatus != Status::error);

    if (mStatus == Status::stopped)
      eventTriggered(Event::Type::process_stopped);

  }
}

void ProcessBase::subscribe(Event::Type eventType,
                            const EventHandler &eventHandler)
{
  switch (eventType) {
    case Event::Type::process_error:
      mProcessErrorEventHandler.push_back(eventHandler);
      break;
    case Event::Type::process_paused:
      mProcessPauseEventHandler.push_back(eventHandler);
      break;
    case Event::Type::process_pausing:
      mProcessPausingEventHandler.push_back(eventHandler);
      break;
    case Event::Type::process_resumed:
      mProcessResumedEventHandler.push_back(eventHandler);
      break;
    case Event::Type::process_running:
      mProcessRunningEventHandler.push_back(eventHandler);
      break;
    case Event::Type::process_stopped:
      mProcessStoppedEventHandler.push_back(eventHandler);
      break;
    case Event::Type::process_stopping:
      mProcessStoppingEventHandler.push_back(eventHandler);
      break;
    case Event::Type::process_finalized:
      mProcessFinalizedEventHandler.push_back(eventHandler);
      break;
    default:
      break;
  }

}

void ProcessBase::subscribe(const EventHandler &eventHandler)
{
  mProcessErrorEventHandler.push_back(eventHandler);
  mProcessFinalizedEventHandler.push_back(eventHandler);
  mProcessPauseEventHandler.push_back(eventHandler);
  mProcessPausingEventHandler.push_back(eventHandler);
  mProcessResumedEventHandler.push_back(eventHandler);
  mProcessRunningEventHandler.push_back(eventHandler);
  mProcessStoppedEventHandler.push_back(eventHandler);
  mProcessStoppingEventHandler.push_back(eventHandler);
}

void ProcessBase::subscribe(const ProcessErrorEventHandler &eventHandler)
{
  mProcessErrorEventHandler.push_back(eventHandler);
}

void ProcessBase::subscribe(const ProcessFinalizedEventHandler &eventHandler)
{
  mProcessFinalizedEventHandler.push_back(eventHandler);
}

void ProcessBase::subscribe(const ProcessPauseEventHandler &eventHandler)
{
  mProcessPauseEventHandler.push_back(eventHandler);
}

void ProcessBase::subscribe(const ProcessPausingEventHandler &eventHandler)
{
  mProcessPausingEventHandler.push_back(eventHandler);
}

void ProcessBase::subscribe(const ProcessResumedEventHandler &eventHandler)
{
  mProcessResumedEventHandler.push_back(eventHandler);
}

void ProcessBase::subscribe(const ProcessRunningEventHandler &eventHandler)
{
  mProcessRunningEventHandler.push_back(eventHandler);
}

void ProcessBase::subscribe(const ProcessStoppedEventHandler &eventHandler)
{
  mProcessStoppedEventHandler.push_back(eventHandler);
}

void ProcessBase::subscribe(const ProcessStoppingEventHandler &eventHandler)
{
  mProcessStoppingEventHandler.push_back(eventHandler);
}

void ProcessBase::eventTriggered(Event::Type type)
{
  switch (type) {
    case Event::Type::process_error:
      this->eventProcessErrorTriggered();
      break;
    case Event::Type::process_paused:
      this->eventProcessPauseTriggered();
      break;
    case Event::Type::process_pausing:
      this->eventProcessPausingTriggered();
      break;
    case Event::Type::process_resumed:
      this->eventProcessResumedTriggered();
      break;
    case Event::Type::process_running:
      this->eventProcessRunningTriggered();
      break;
    case Event::Type::process_stopped:
      this->eventProcessStoppedTriggered();
      break;
    case Event::Type::process_stopping:
      this->eventProcessStoppingTriggered();
      break;
    case Event::Type::process_finalized:
      this->eventProcessFinalizedTriggered();
      break;
    default:
      break;
  }

}

void ProcessBase::eventProcessErrorTriggered()
{
  std::list<ProcessErrorEventHandler> event_handler = mProcessErrorEventHandler;
  for (auto &handler : event_handler) {
    handler(mProcessErrorEvent);
  }
}

void ProcessBase::eventProcessFinalizedTriggered()
{
  std::list<ProcessFinalizedEventHandler> event_handler = mProcessFinalizedEventHandler;
  for (auto &handler : event_handler) {
    handler(mProcessFinalizedEvent);
  }
}

void ProcessBase::eventProcessPauseTriggered()
{
  std::list<ProcessPauseEventHandler> event_handler = mProcessPauseEventHandler;
  for (auto &handler : event_handler) {
    handler(mProcessPauseEvent);
  }
}

void ProcessBase::eventProcessPausingTriggered()
{
  std::list<ProcessPausingEventHandler> event_handler = mProcessPausingEventHandler;
  for (auto &handler : event_handler) {
    handler(mProcessPausingEvent);
  }
}

void ProcessBase::eventProcessResumedTriggered()
{
  std::list<ProcessResumedEventHandler> event_handler = mProcessResumedEventHandler;
  for (auto &handler : event_handler) {
    handler(mProcessResumedEvent);
  }
}

void ProcessBase::eventProcessRunningTriggered()
{
  std::list<ProcessRunningEventHandler> event_handler = mProcessRunningEventHandler;
  for (auto &handler : event_handler) {
    handler(mProcessRunningEvent);
  }
}

void ProcessBase::eventProcessStoppedTriggered()
{
  std::list<ProcessStoppedEventHandler> event_handler = mProcessStoppedEventHandler;
  for (auto &handler : event_handler) {
    handler(mProcessStoppedEvent);
  }
}

void ProcessBase::eventProcessStoppingTriggered()
{
  std::list<ProcessStoppingEventHandler> event_handler = mProcessStoppingEventHandler;
  for (auto &handler : event_handler) {
    handler(mProcessStoppingEvent);
  }
}

ProcessErrorEvent *ProcessBase::errorEvent()
{
  return mProcessErrorEvent;
}

ProcessFinalizedEvent *ProcessBase::finalizedEvent()
{
  return mProcessFinalizedEvent;
}

ProcessPauseEvent *ProcessBase::pauseEvent()
{
  return mProcessPauseEvent;
}

ProcessPausingEvent *ProcessBase::pausingEvent()
{
  return mProcessPausingEvent;
}

ProcessResumedEvent *ProcessBase::resumedEvent()
{
  return mProcessResumedEvent;
}

ProcessRunningEvent *ProcessBase::runningEvent()
{
  return mProcessRunningEvent;
}

ProcessStoppedEvent *ProcessBase::stoppedEvent()
{
  return mProcessStoppedEvent;
}

ProcessStoppingEvent *ProcessBase::stoppingEvent()
{
  return mProcessStoppingEvent;
}





} // End namespace tl


