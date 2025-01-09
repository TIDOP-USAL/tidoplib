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
  * \class TaskTree
  * 
  * \brief Class for managing a task tree with parent-child dependencies.
  *
  * This class allows tasks to be organized in a hierarchical structure where tasks can have parent tasks.
  * The tasks are executed respecting these dependencies. Tasks in the tree can be stopped or executed based on their status.
  */
class TL_EXPORT TaskTree
  : public TaskBase
{

public:

    /*!
     * \brief Default constructor for TaskTree.
     */
    TaskTree();
    ~TaskTree() override;

    /*!
     * \brief Add a task to the task tree.
     *
     * This method adds a new task to the task tree. It also assigns the parent tasks that need to be completed
     * before the task can execute.
     *
     * \param[in] task The task to be added.
     * \param[in] parentTasks A list of parent tasks that must be completed before the task can be run.
     */
    void addTask(const std::shared_ptr<Task> &task, 
                 const std::list<std::shared_ptr<Task>> &parentTasks);
 
// Task interface

public:

    void stop() override;

// TaskBase interface

private:

    void execute(Progress *progressBar = nullptr) override;

private:

};


/*! \} */

} // End namespace tl

