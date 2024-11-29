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



/* Task Tree */

class TL_EXPORT TaskTree
  : public TaskBase
{

public:

    TaskTree();
    ~TaskTree() override;

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


/*! \} */ // end of core

} // End namespace tl

