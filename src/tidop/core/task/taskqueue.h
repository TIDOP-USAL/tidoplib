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

#include <queue>
#include <memory>

#include "tidop/config.h"
#include "tidop/core/task/task.h"

namespace tl
{

class Progress;

/*!
 * \addtogroup Task
 * \{
 */

 /*!
  * \class TaskQueue
  * 
  * \brief A queue of tasks that are executed in a FIFO (First In, First Out) order.
  *
  * The `TaskQueue` class manages a queue of tasks, where tasks are executed in the order they are added.
  * The class provides methods for pushing tasks into the queue, popping tasks, and controlling the
  * execution of the task queue. It extends the functionality of `TaskBase` to provide task management and
  * execution control.
  *
  */
class TL_EXPORT TaskQueue
  : public TaskBase
{

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes an empty task queue.
     */
    TaskQueue();
    ~TaskQueue() override;

    /*!
     * \brief Adds a task to the queue.
     * \param[in] task A shared pointer to the task to be added to the queue.
     *
     * This method adds a new task to the end of the queue.
     */
    void push(std::shared_ptr<Task> task);

    /*!
     * \brief Removes the task at the front of the queue.
     *
     * This method pops the task at the front of the queue. If the queue is empty, it does nothing.
     */
    void pop() TL_NOEXCEPT;

    /*!
     * \brief Gets the number of tasks in the queue.
     * \return The number of tasks currently in the queue.
     *
     * This method returns the size of the queue.
     */
    auto size() const TL_NOEXCEPT -> size_t;

    /*!
     * \brief Checks if the queue is empty.
     * \return `true` if the queue is empty, `false` otherwise.
     *
     * This method checks if there are any tasks left in the queue.
     */
    auto empty() const TL_NOEXCEPT -> bool;

// Task interface

public:

    void stop() override;

// TaskBase interface

private:

    void execute(Progress *progressBar = nullptr) override;

private:

    std::queue<std::shared_ptr<Task>> queue;
    static std::mutex mtx;
};


/*! \} */ 

} // End namespace tl

