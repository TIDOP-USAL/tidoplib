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
= default;

Process::~Process()
= default;



/* ProcessBase */


ProcessBase::ProcessBase()
  : Process(),
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

ProcessBase::ProcessBase(const ProcessBase &process)
  : Process(),
    mProcessErrorEvent(new ProcessErrorEvent(*process.mProcessErrorEvent)),
    mProcessFinalizedEvent(new ProcessFinalizedEvent(*process.mProcessFinalizedEvent)),
    mProcessPauseEvent(new ProcessPauseEvent(*process.mProcessPauseEvent)),
    mProcessPausingEvent(new ProcessPausingEvent(*process.mProcessPausingEvent)),
    mProcessResumedEvent(new ProcessResumedEvent(*process.mProcessResumedEvent)),
    mProcessRunningEvent(new ProcessRunningEvent(*process.mProcessRunningEvent)),
    mProcessStoppedEvent(new ProcessStoppedEvent(*process.mProcessStoppedEvent)),
    mProcessStoppingEvent(new ProcessStoppingEvent(*process.mProcessStoppingEvent)),
    mProcessErrorEventHandler(process.mProcessErrorEventHandler),
    mProcessFinalizedEventHandler(process.mProcessFinalizedEventHandler),
    mProcessPauseEventHandler(process.mProcessPauseEventHandler),
    mProcessPausingEventHandler(process.mProcessPausingEventHandler),
    mProcessResumedEventHandler(process.mProcessResumedEventHandler),
    mProcessRunningEventHandler(process.mProcessRunningEventHandler),
    mProcessStoppedEventHandler(process.mProcessStoppedEventHandler),
    mProcessStoppingEventHandler(process.mProcessStoppingEventHandler)
{

}

ProcessBase::ProcessBase(ProcessBase &&process) TL_NOEXCEPT
  : Process(),
    mProcessErrorEvent(std::move(process.mProcessErrorEvent)),
    mProcessFinalizedEvent(std::move(process.mProcessFinalizedEvent)),
    mProcessPauseEvent(std::move(process.mProcessPauseEvent)),
    mProcessPausingEvent(std::move(process.mProcessPausingEvent)),
    mProcessResumedEvent(std::move(process.mProcessResumedEvent)),
    mProcessRunningEvent(std::move(process.mProcessRunningEvent)),
    mProcessStoppedEvent(std::move(process.mProcessStoppedEvent)),
    mProcessStoppingEvent(std::move(process.mProcessStoppingEvent)),
    mProcessErrorEventHandler(std::move(process.mProcessErrorEventHandler)),
    mProcessFinalizedEventHandler(std::move(process.mProcessFinalizedEventHandler)),
    mProcessPauseEventHandler(std::move(process.mProcessPauseEventHandler)),
    mProcessPausingEventHandler(std::move(process.mProcessPausingEventHandler)),
    mProcessResumedEventHandler(std::move(process.mProcessResumedEventHandler)),
    mProcessRunningEventHandler(std::move(process.mProcessRunningEventHandler)),
    mProcessStoppedEventHandler(std::move(process.mProcessStoppedEventHandler)),
    mProcessStoppingEventHandler(std::move(process.mProcessStoppingEventHandler))
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

}

ProcessBase &ProcessBase::operator=(const ProcessBase &process)
{
  if (this != &process){
    mProcessErrorEvent = std::make_unique<ProcessErrorEvent>(*process.mProcessErrorEvent);
    mProcessFinalizedEvent = std::make_unique<ProcessFinalizedEvent>(*process.mProcessFinalizedEvent);
    mProcessPauseEvent = std::make_unique<ProcessPauseEvent>(*process.mProcessPauseEvent);
    mProcessPausingEvent = std::make_unique<ProcessPausingEvent>(*process.mProcessPausingEvent);
    mProcessResumedEvent = std::make_unique<ProcessResumedEvent>(*process.mProcessResumedEvent);
    mProcessRunningEvent = std::make_unique<ProcessRunningEvent>(*process.mProcessRunningEvent);
    mProcessStoppedEvent = std::make_unique<ProcessStoppedEvent>(*process.mProcessStoppedEvent);
    mProcessStoppingEvent = std::make_unique<ProcessStoppingEvent>(*process.mProcessStoppingEvent);
    mProcessErrorEventHandler = process.mProcessErrorEventHandler;
    mProcessFinalizedEventHandler = process.mProcessFinalizedEventHandler;
    mProcessPauseEventHandler = process.mProcessPauseEventHandler;
    mProcessPausingEventHandler = process.mProcessPausingEventHandler;
    mProcessResumedEventHandler = process.mProcessResumedEventHandler;
    mProcessRunningEventHandler = process.mProcessRunningEventHandler;
    mProcessStoppedEventHandler = process.mProcessStoppedEventHandler;
    mProcessStoppingEventHandler = process.mProcessStoppingEventHandler;
  }
  return *this;
}

ProcessBase &ProcessBase::operator=(ProcessBase &&process) TL_NOEXCEPT
{
  if (this != &process){
    mProcessErrorEvent = std::move(process.mProcessErrorEvent);
    mProcessFinalizedEvent = std::move(process.mProcessFinalizedEvent);
    mProcessPauseEvent = std::move(process.mProcessPauseEvent);
    mProcessPausingEvent = std::move(process.mProcessPausingEvent);
    mProcessResumedEvent = std::move(process.mProcessResumedEvent);
    mProcessRunningEvent = std::move(process.mProcessRunningEvent);
    mProcessStoppedEvent = std::move(process.mProcessStoppedEvent);
    mProcessStoppingEvent = std::move(process.mProcessStoppingEvent);
    mProcessErrorEventHandler = std::move(process.mProcessErrorEventHandler);
    mProcessFinalizedEventHandler = std::move(process.mProcessFinalizedEventHandler);
    mProcessPauseEventHandler = std::move(process.mProcessPauseEventHandler);
    mProcessPausingEventHandler = std::move(process.mProcessPausingEventHandler);
    mProcessResumedEventHandler = std::move(process.mProcessResumedEventHandler);
    mProcessRunningEventHandler = std::move(process.mProcessRunningEventHandler);
    mProcessStoppedEventHandler = std::move(process.mProcessStoppedEventHandler);
    mProcessStoppingEventHandler = std::move(process.mProcessStoppingEventHandler);
  }
  return *this;
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
      mProcessErrorEventHandler.emplace_back(eventHandler);
      break;
    case Event::Type::process_paused:
      mProcessPauseEventHandler.emplace_back(eventHandler);
      break;
    case Event::Type::process_pausing:
      mProcessPausingEventHandler.emplace_back(eventHandler);
      break;
    case Event::Type::process_resumed:
      mProcessResumedEventHandler.emplace_back(eventHandler);
      break;
    case Event::Type::process_running:
      mProcessRunningEventHandler.emplace_back(eventHandler);
      break;
    case Event::Type::process_stopped:
      mProcessStoppedEventHandler.emplace_back(eventHandler);
      break;
    case Event::Type::process_stopping:
      mProcessStoppingEventHandler.emplace_back(eventHandler);
      break;
    case Event::Type::process_finalized:
      mProcessFinalizedEventHandler.emplace_back(eventHandler);
      break;
    default:
      break;
  }

}

void ProcessBase::subscribe(const EventHandler &eventHandler)
{
  mProcessErrorEventHandler.emplace_back(eventHandler);
  mProcessFinalizedEventHandler.emplace_back(eventHandler);
  mProcessPauseEventHandler.emplace_back(eventHandler);
  mProcessPausingEventHandler.emplace_back(eventHandler);
  mProcessResumedEventHandler.emplace_back(eventHandler);
  mProcessRunningEventHandler.emplace_back(eventHandler);
  mProcessStoppedEventHandler.emplace_back(eventHandler);
  mProcessStoppingEventHandler.emplace_back(eventHandler);
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
    handler(mProcessErrorEvent.get());
  }
}

void ProcessBase::eventProcessFinalizedTriggered()
{
  std::list<ProcessFinalizedEventHandler> event_handler = mProcessFinalizedEventHandler;
  for (auto &handler : event_handler) {
    handler(mProcessFinalizedEvent.get());
  }
}

void ProcessBase::eventProcessPauseTriggered()
{
  std::list<ProcessPauseEventHandler> event_handler = mProcessPauseEventHandler;
  for (auto &handler : event_handler) {
    handler(mProcessPauseEvent.get());
  }
}

void ProcessBase::eventProcessPausingTriggered()
{
  std::list<ProcessPausingEventHandler> event_handler = mProcessPausingEventHandler;
  for (auto &handler : event_handler) {
    handler(mProcessPausingEvent.get());
  }
}

void ProcessBase::eventProcessResumedTriggered()
{
  std::list<ProcessResumedEventHandler> event_handler = mProcessResumedEventHandler;
  for (auto &handler : event_handler) {
    handler(mProcessResumedEvent.get());
  }
}

void ProcessBase::eventProcessRunningTriggered()
{
  std::list<ProcessRunningEventHandler> event_handler = mProcessRunningEventHandler;
  for (auto &handler : event_handler) {
    handler(mProcessRunningEvent.get());
  }
}

void ProcessBase::eventProcessStoppedTriggered()
{
  std::list<ProcessStoppedEventHandler> event_handler = mProcessStoppedEventHandler;
  for (auto &handler : event_handler) {
    handler(mProcessStoppedEvent.get());
  }
}

void ProcessBase::eventProcessStoppingTriggered()
{
  std::list<ProcessStoppingEventHandler> event_handler = mProcessStoppingEventHandler;
  for (auto &handler : event_handler) {
    handler(mProcessStoppingEvent.get());
  }
}

ProcessErrorEvent *ProcessBase::errorEvent()
{
  return mProcessErrorEvent.get();
}

ProcessFinalizedEvent *ProcessBase::finalizedEvent()
{
  return mProcessFinalizedEvent.get();
}

ProcessPauseEvent *ProcessBase::pauseEvent()
{
  return mProcessPauseEvent.get();
}

ProcessPausingEvent *ProcessBase::pausingEvent()
{
  return mProcessPausingEvent.get();
}

ProcessResumedEvent *ProcessBase::resumedEvent()
{
  return mProcessResumedEvent.get();
}

ProcessRunningEvent *ProcessBase::runningEvent()
{
  return mProcessRunningEvent.get();
}

ProcessStoppedEvent *ProcessBase::stoppedEvent()
{
  return mProcessStoppedEvent.get();
}

ProcessStoppingEvent *ProcessBase::stoppingEvent()
{
  return mProcessStoppingEvent.get();
}





} // End namespace tl


