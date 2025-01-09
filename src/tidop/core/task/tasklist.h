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

#include <list>
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
 * \class TaskList
 * \brief A list of tasks that can be executed sequentially.
 *
 * The `TaskList` class manages a collection of tasks and provides methods to control their execution as a group.
 * Tasks in the list are executed sequentially. The list also supports cancellation of tasks on error and stopping
 * execution midway if needed.
 *
 */
class TL_EXPORT TaskList
  : public TaskBase
{

private:

    std::list<std::shared_ptr<Task>> tasks;
    bool cancelOnError;

public:

    /*!
     * \brief Default constructor.
     *
     * Initializes an empty task list with `cancelOnError` set to `false`.
     */
    TaskList();
   
    /*!
     * \brief Copy constructor.
     * \param[in] taskList The task list to be copied.
     *
     * Initializes a new `TaskList` by copying the tasks from another `TaskList`.
     */
    TaskList(const TaskList &taskList);
   
    /*!
     * \brief Constructor with an initializer list of tasks.
     * \param[in] tasks A list of tasks to be added to the task list.
     *
     * Initializes the task list with the given tasks.
     */
    TaskList(std::initializer_list<std::shared_ptr<Task>> tasks);
    
    ~TaskList() override;

    /*!
     * \brief Adds a task to the list.
     * \param[in] task The task to be added to the list.
     *
     * This method adds a task to the end of the task list.
     */
    void push_back(const std::shared_ptr<Task> &task);

    /*!
     * \brief Gets the number of tasks in the list.
     * \return The number of tasks in the list.
     *
     * This method returns the size of the `tasks` list.
     */
    auto size() const TL_NOEXCEPT -> size_t;

    /*!
     * \brief Checks if the task list is empty.
     * \return `true` if the task list is empty, `false` otherwise.
     *
     * This method checks if there are any tasks in the list.
     */
    auto empty() const TL_NOEXCEPT -> bool;

    /*!
     * \brief Sets whether to cancel the execution of the remaining tasks if an error occurs.
     * \param[in] cancel If `true`, stops the task execution on error; otherwise, continue with other tasks.
     */
    void setCancelTaskOnError(bool cancel);

// Task interface

public:

    void stop() override;

// TaskBase interface

private:

    void execute(Progress *progressBar = nullptr) override;

};


/*! \} */

} // End namespace tl

