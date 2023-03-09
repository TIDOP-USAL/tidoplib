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

#ifndef TL_CORE_PROCESS_H
#define TL_CORE_PROCESS_H

#include "config_tl.h"

#include <thread>
#include <mutex>
#include <functional>
#include <map>
#include <list>
#include <queue>
#include <memory>

#include "tidop/core/defs.h"
#include "tidop/core/event.h"

namespace tl
{

class Progress;

/*!
 * \addtogroup core
 * \{
 */


class TL_EXPORT Task
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
  using TaskErrorEventHandler = typename std::function<void (TaskErrorEvent *)>;
  using TaskFinalizedEventHandler = typename std::function<void (TaskFinalizedEvent *)>;
  using TaskPauseEventHandler = typename std::function<void (TaskPauseEvent *)>;
  using TaskPausingEventHandler = typename std::function<void (TaskPausingEvent *)>;
  using TaskResumedEventHandler = typename std::function<void (TaskResumedEvent *)>;
  using TaskRunningEventHandler = typename std::function<void (TaskRunningEvent *)>;
  using TaskStoppedEventHandler = typename std::function<void (TaskStoppedEvent *)>;
  using TaskStoppingEventHandler = typename std::function<void (TaskStoppingEvent *)>;

public:

  Task();
  virtual ~Task();

  Task(const Task &) = delete;
  Task(Task &&) = delete;
  void operator=(const Task &) = delete;
  void operator=(Task &&) = delete;

  /*!
   * \brief Start the process
   */
  virtual void run(Progress *progressBar = nullptr) = 0;

  /*!
   * \brief Starts the process asynchronously
   */
  virtual void runAsync(Progress *progressBar = nullptr) = 0;

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
  virtual void subscribe(const TaskErrorEventHandler &eventHandler) = 0;
  virtual void subscribe(const TaskFinalizedEventHandler &eventHandler) = 0;
  virtual void subscribe(const TaskPauseEventHandler &eventHandler) = 0;
  virtual void subscribe(const TaskPausingEventHandler &eventHandler) = 0;
  virtual void subscribe(const TaskResumedEventHandler &eventHandler) = 0;
  virtual void subscribe(const TaskRunningEventHandler &eventHandler) = 0;
  virtual void subscribe(const TaskStoppedEventHandler &eventHandler) = 0;
  virtual void subscribe(const TaskStoppingEventHandler &eventHandler) = 0;

  virtual Status status() const = 0;
};



class TL_EXPORT TaskBase
  : public Task
{

public:

  TaskBase();
  TaskBase(const TaskBase &process);
  TaskBase(TaskBase &&process) TL_NOEXCEPT;
  ~TaskBase() override;

  TaskBase &operator=(const TaskBase &process);
  TaskBase &operator=(TaskBase &&process) TL_NOEXCEPT;

protected:

  /*!
   * \brief Ejecuta el proceso
   */
  virtual void execute(Progress *progressBar = nullptr) = 0;

  // Task interface

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
  void subscribe(const TaskErrorEventHandler &eventHandler) override;
  void subscribe(const TaskFinalizedEventHandler &eventHandler) override;
  void subscribe(const TaskPauseEventHandler &eventHandler) override;
  void subscribe(const TaskPausingEventHandler &eventHandler) override;
  void subscribe(const TaskResumedEventHandler &eventHandler) override;
  void subscribe(const TaskRunningEventHandler &eventHandler) override;
  void subscribe(const TaskStoppedEventHandler &eventHandler) override;
  void subscribe(const TaskStoppingEventHandler &eventHandler) override;
 
  Status status() const override;

protected:

  void setStatus(Status status);

  void eventTriggered(Event::Type type);
  void eventTaskErrorTriggered();
  void eventTaskFinalizedTriggered();
  void eventTaskPauseTriggered();
  void eventTaskPausingTriggered();
  void eventTaskResumedTriggered();
  void eventTaskRunningTriggered();
  void eventTaskStoppedTriggered();
  void eventTaskStoppingTriggered();

  TaskErrorEvent *errorEvent();
  TaskFinalizedEvent *finalizedEvent();
  TaskPauseEvent *pauseEvent();
  TaskPausingEvent *pausingEvent();
  TaskResumedEvent *resumedEvent();
  TaskRunningEvent *runningEvent();
  TaskStoppedEvent *stoppedEvent();
  TaskStoppingEvent *stoppingEvent();

private:

  void executeTask(Progress *progressBar) TL_NOEXCEPT;

private:

  /// TODO: añadir clase chrono para medir tiempos
  Status mStatus{Status::start};
  std::thread mThread;
  std::mutex mMutex;
  std::unique_ptr<TaskErrorEvent> mTaskErrorEvent;
  std::unique_ptr<TaskFinalizedEvent> mTaskFinalizedEvent;
  std::unique_ptr<TaskPauseEvent> mTaskPauseEvent;
  std::unique_ptr<TaskPausingEvent> mTaskPausingEvent;
  std::unique_ptr<TaskResumedEvent> mTaskResumedEvent;
  std::unique_ptr<TaskRunningEvent> mTaskRunningEvent;
  std::unique_ptr<TaskStoppedEvent> mTaskStoppedEvent;
  std::unique_ptr<TaskStoppingEvent> mTaskStoppingEvent;
  std::list<TaskErrorEventHandler> mTaskErrorEventHandler;
  std::list<TaskFinalizedEventHandler> mTaskFinalizedEventHandler;
  std::list<TaskPauseEventHandler> mTaskPauseEventHandler;
  std::list<TaskPausingEventHandler> mTaskPausingEventHandler;
  std::list<TaskResumedEventHandler> mTaskResumedEventHandler;
  std::list<TaskRunningEventHandler> mTaskRunningEventHandler;
  std::list<TaskStoppedEventHandler> mTaskStoppedEventHandler;
  std::list<TaskStoppingEventHandler> mTaskStoppingEventHandler;
};




class TL_EXPORT Process
  : public TaskBase
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

  Process(std::string commandText/*,
          Priority priority = Priority::normal*/);
  ~Process() override;

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

// TaskBase
  
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


/*!
 * \brief Lista de tareas
 */
class TL_EXPORT TaskList
  : public TaskBase
{

public:

  /*!
   * \brief Constructora por defecto
   */
  TaskList();

  /*!
   * \brief Constructor de copia
   * \param[in] taskList Lista de tareas que se copia
   */
  TaskList(const TaskList &taskList);

  /*!
   * \brief Constructor de lista
   * \param[in] tasks Listado de tareas
   */
  TaskList(std::initializer_list<std::shared_ptr<Task>> tasks);

  ~TaskList() override;

  void push_back(const std::shared_ptr<Task> &task);
  size_t size() const TL_NOEXCEPT;
  bool empty() const TL_NOEXCEPT;

// Task interface

public:

  void stop() override;

// TaskBase interface

private:

  virtual void execute(Progress *progressBar = nullptr) override;

private:

  std::list<std::shared_ptr<Task>> mTasks;

};


class TaskQueue
  : public TaskBase
{

public:

  TaskQueue();
  ~TaskQueue();

  void push(std::shared_ptr<Task> task);
  void pop() TL_NOEXCEPT;
  size_t size() const TL_NOEXCEPT;
  bool empty() const TL_NOEXCEPT;

// Task interface

public:

  void stop() override;

// TaskBase interface

private:

  virtual void execute(Progress *progressBar = nullptr) override;

private:

  std::queue<std::shared_ptr<Task>> mQueue;

};


/* Task Tree */

class TL_EXPORT TaskTree
  : public TaskBase
{

public:

  TaskTree();
  ~TaskTree();

  void addTask(const std::shared_ptr<Task> &task, 
               const std::list<std::shared_ptr<Task>> &parentTasks);
 
  // Task interface

public:

  void stop() override;

// TaskBase interface

private:

  virtual void execute(Progress *progressBar = nullptr) override;

private:

};





/*! \} */ // end of core

} // End namespace tl

#endif // TL_CORE_PROCESS_H
