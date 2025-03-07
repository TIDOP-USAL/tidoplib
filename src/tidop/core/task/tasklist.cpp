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


#include "tidop/core/task/tasklist.h"
#include "tidop/core/task/progress.h"

namespace tl
{


/* Task List */

TaskList::TaskList()
  : tasks(0),
    cancelOnError(false)
{
}

TaskList::TaskList(const TaskList &taskList)
  : TaskBase(),
    tasks(taskList.tasks),
    cancelOnError(false)
{
}

TaskList::TaskList(std::initializer_list<std::shared_ptr<Task>> tasks)
  : tasks(tasks),
    cancelOnError(false)
{
}

TaskList::~TaskList() = default;

void TaskList::push_back(const std::shared_ptr<Task> &task)
{
    this->tasks.push_back(task);
}

void TaskList::setCancelTaskOnError(bool cancel)
{
    this->cancelOnError = cancel;
}

auto TaskList::size() const TL_NOEXCEPT -> size_t
{
    return tasks.size();
}

auto TaskList::empty() const TL_NOEXCEPT -> bool
{
    return tasks.empty();
}

void TaskList::stop()
{
    TaskBase::stop();

    if (status() == Status::stopping) {
        for (const auto &task : tasks) {
            task->stop();
        }
    }
}

void TaskList::execute(Progress *progressBar)
{
    for (const auto &task : tasks) {

        if (status() == Status::stopping) return;

        task->run();

        if (task->status() == Status::error && this->cancelOnError){
            setStatus(Status::error);
            break;
        }

        if (progressBar) (*progressBar)();

    }
}


} // End namespace tl


