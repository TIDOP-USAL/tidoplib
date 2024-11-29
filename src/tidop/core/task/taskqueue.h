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
 * \addtogroup core
 * \{
 */

class TL_EXPORT TaskQueue
  : public TaskBase
{

public:

    TaskQueue();
    ~TaskQueue() override;
    
    void push(std::shared_ptr<Task> task);
    void pop() TL_NOEXCEPT;
    auto size() const TL_NOEXCEPT -> size_t;
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


/*! \} */ // end of core

} // End namespace tl

