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


#include "tidop/core/task/taskqueue.h"
#include "tidop/core/progress.h"

namespace tl
{


std::mutex TaskQueue::mtx;

TaskQueue::TaskQueue()
  : TaskBase()
{
}

TaskQueue::~TaskQueue() 
{
	TaskQueue::stop();
}

void TaskQueue::push(std::shared_ptr<Task> task)
{
    queue.push(task);

    if (status() == Status::finalized ||
        status() == Status::stopped) {
        setStatus(Status::start);
        //  setStatus(Status::running);
        //  // Al ser una cola se vuelve a ejecutar cada vez que se añade un elemento
        //  run();
    }

}

void TaskQueue::pop() TL_NOEXCEPT
{
    queue.pop();
}

auto TaskQueue::size() const TL_NOEXCEPT -> size_t
{
    return queue.size();
}

auto TaskQueue::empty() const TL_NOEXCEPT -> bool
{
    return queue.empty();
}

void TaskQueue::stop()
{
    TaskBase::stop();
}

void TaskQueue::execute(Progress */*progressBar*/)
{
    while (!queue.empty()) {
        
        if (status() == Status::stopping || status() == Status::stopped) {

        } else {
            std::lock_guard<std::mutex> lck(TaskQueue::mtx);
            queue.front()->run();
        }

        queue.pop();
    }
}


} // End namespace tl


