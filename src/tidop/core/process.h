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

#ifndef TL_CORE_PROCESS_H
#define TL_CORE_PROCESS_H

#include "config_tl.h"

#include <thread>
#include <mutex>
#include <functional>
#include <map>
#include <list>

#include "tidop/core/defs.h"
#include "tidop/core/event.h"

namespace tl
{

class Progress;

/*!
 * \addtogroup core
 * \{
 */


class TL_EXPORT Process
{

public:

  enum class Status
  {
    start,             /*!< Inicio */
    running,           /*!< Corriendo */
    pausing,           /*!< Pausando */
    paused,            /*!< Pausado */
    stopping,
    stopped,           /*!< Detenido por el usuario */
    finalized,         /*!< Finalizado */
    error              /*!< Terminado con error */
  };

  using EventHandler = typename std::function<void (Event *)>;
  using ProcessErrorEventHandler = typename std::function<void (ProcessErrorEvent *)>;
  using ProcessFinalizedEventHandler = typename std::function<void (ProcessFinalizedEvent *)>;
  using ProcessPauseEventHandler = typename std::function<void (ProcessPauseEvent *)>;
  using ProcessPausingEventHandler = typename std::function<void (ProcessPausingEvent *)>;
  using ProcessResumedEventHandler = typename std::function<void (ProcessResumedEvent *)>;
  using ProcessRunningEventHandler = typename std::function<void (ProcessRunningEvent *)>;
  using ProcessStoppedEventHandler = typename std::function<void (ProcessStoppedEvent *)>;
  using ProcessStoppingEventHandler = typename std::function<void (ProcessStoppingEvent *)>;

public:

  Process();
  virtual ~Process();

  Process(const Process &) = delete;
  Process(Process &&) = delete;
  void operator=(const Process &) = delete;
  void operator=(Process &&) = delete;

  /*!
   * \brief Start the process
   */
  virtual void run(Progress *progressBar) = 0;

  /*!
   * \brief Starts the process asynchronously
   * Se tiene que realizar el control de excepciones desde execute y
   * establecer el mensaje de error:
   * mProcessErrorEvent->setErrorMessage(e.what());
   * eventTriggered(Event::Type::process_error);
   */
  virtual void runAsync(Progress *progressBar) = 0;

  /*!
   * \brief Pause the process
   */
  virtual void pause() = 0;

  /*!
   * \brief Restart the process
   */
  virtual void reset() = 0;

  /*!
   * \brief Continue running the process
   */
  virtual void resume() = 0;

  /*!
   * \brief Stops the process
   */
  virtual void stop() = 0;

  virtual void subscribe(Event::Type eventType,
                         const EventHandler &eventHandler) = 0;
  virtual void subscribe(const EventHandler &eventHandler) = 0;
  virtual void subscribe(const ProcessErrorEventHandler &eventHandler) = 0;
  virtual void subscribe(const ProcessFinalizedEventHandler &eventHandler) = 0;
  virtual void subscribe(const ProcessPauseEventHandler &eventHandler) = 0;
  virtual void subscribe(const ProcessPausingEventHandler &eventHandler) = 0;
  virtual void subscribe(const ProcessResumedEventHandler &eventHandler) = 0;
  virtual void subscribe(const ProcessRunningEventHandler &eventHandler) = 0;
  virtual void subscribe(const ProcessStoppedEventHandler &eventHandler) = 0;
  virtual void subscribe(const ProcessStoppingEventHandler &eventHandler) = 0;

};



class TL_EXPORT ProcessBase
  : public Process
{

public:

  ProcessBase();
  ProcessBase(const ProcessBase &process);
  ProcessBase(ProcessBase &&process) TL_NOEXCEPT;
  ~ProcessBase() override;

  ProcessBase &operator=(const ProcessBase &process);
  ProcessBase &operator=(ProcessBase &&process) TL_NOEXCEPT;

protected:

  /*!
   * \brief Ejecuta el proceso
   */
  virtual void execute(Progress *progressBar = nullptr) = 0;

  // Process interface

public:

  void run(Progress *progressBar = nullptr) final;
  void runAsync(Progress *progressBar = nullptr) final;
  void pause() override;
  void reset() override;
  void resume() override;
  void stop() override;

  void subscribe(Event::Type eventType,
                 const EventHandler &eventHandler) override;
  void subscribe(const EventHandler &eventHandler) override;
  void subscribe(const ProcessErrorEventHandler &eventHandler) override;
  void subscribe(const ProcessFinalizedEventHandler &eventHandler) override;
  void subscribe(const ProcessPauseEventHandler &eventHandler) override;
  void subscribe(const ProcessPausingEventHandler &eventHandler) override;
  void subscribe(const ProcessResumedEventHandler &eventHandler) override;
  void subscribe(const ProcessRunningEventHandler &eventHandler) override;
  void subscribe(const ProcessStoppedEventHandler &eventHandler) override;
  void subscribe(const ProcessStoppingEventHandler &eventHandler) override;

protected:

  void eventTriggered(Event::Type type);
  void eventProcessErrorTriggered();
  void eventProcessFinalizedTriggered();
  void eventProcessPauseTriggered();
  void eventProcessPausingTriggered();
  void eventProcessResumedTriggered();
  void eventProcessRunningTriggered();
  void eventProcessStoppedTriggered();
  void eventProcessStoppingTriggered();

  ProcessErrorEvent *errorEvent();
  ProcessFinalizedEvent *finalizedEvent();
  ProcessPauseEvent *pauseEvent();
  ProcessPausingEvent *pausingEvent();
  ProcessResumedEvent *resumedEvent();
  ProcessRunningEvent *runningEvent();
  ProcessStoppedEvent *stoppedEvent();
  ProcessStoppingEvent *stoppingEvent();

private:

  /// TODO: añadir clase chrono para medir tiempos
  Status mStatus{Status::start};
  std::thread mThread;
  std::mutex mMutex;
  std::unique_ptr<ProcessErrorEvent> mProcessErrorEvent;
  std::unique_ptr<ProcessFinalizedEvent> mProcessFinalizedEvent;
  std::unique_ptr<ProcessPauseEvent> mProcessPauseEvent;
  std::unique_ptr<ProcessPausingEvent> mProcessPausingEvent;
  std::unique_ptr<ProcessResumedEvent> mProcessResumedEvent;
  std::unique_ptr<ProcessRunningEvent> mProcessRunningEvent;
  std::unique_ptr<ProcessStoppedEvent> mProcessStoppedEvent;
  std::unique_ptr<ProcessStoppingEvent> mProcessStoppingEvent;
  std::list<ProcessErrorEventHandler> mProcessErrorEventHandler;
  std::list<ProcessFinalizedEventHandler> mProcessFinalizedEventHandler;
  std::list<ProcessPauseEventHandler> mProcessPauseEventHandler;
  std::list<ProcessPausingEventHandler> mProcessPausingEventHandler;
  std::list<ProcessResumedEventHandler> mProcessResumedEventHandler;
  std::list<ProcessRunningEventHandler> mProcessRunningEventHandler;
  std::list<ProcessStoppedEventHandler> mProcessStoppedEventHandler;
  std::list<ProcessStoppingEventHandler> mProcessStoppingEventHandler;
};




class TL_EXPORT ExternalProcess
  : public ProcessBase
{

public:

#ifdef WIN32

  enum class Priority
  {
    realtime = REALTIME_PRIORITY_CLASS,
    high = HIGH_PRIORITY_CLASS,
    above_normal = ABOVE_NORMAL_PRIORITY_CLASS,
    normal = NORMAL_PRIORITY_CLASS,
    below_normal = BELOW_NORMAL_PRIORITY_CLASS,
    idle = IDLE_PRIORITY_CLASS
  };

#endif

public:

  ExternalProcess(std::string commandText, 
                  Priority priority = Priority::normal);
  ~ExternalProcess() override;

#ifdef WIN32
  Priority priority() const;

  /*!
   * \brief Establece la prioridad del proceso
   * \param[in] priority
   */
  void setPriority(Priority priority);

private:

  std::string formatErrorMsg(unsigned long errorCode);
  bool createPipe();
#endif

private:

// ProcessBase
  
  void execute(Progress *progressBar = nullptr) override;

private:

  std::string mCommandText;
#ifdef WIN32
  STARTUPINFO mStartUpInfo;
  PROCESS_INFORMATION mProcessInformation;
  SECURITY_ATTRIBUTES mSecurityAttributes;
  Priority mPriority;
  HANDLE mThreadHandle;
#endif
  
};


/*! \} */ // end of core



/* ---------------------------------------------------------------------------------- */

} // End namespace tl

#endif // TL_CORE_PROCESS_H
