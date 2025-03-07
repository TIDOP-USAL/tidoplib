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

#include "tidop/config.h"
#include "tidop/core/base/defs.h"
#include "tidop/core/concurrency/queue.h"

namespace tl
{

/*!
 * \addtogroup concurrency
 *
 * \{
 */
 
/*!
 * \brief Consumer Interface
 * 
 * This interface defines a consumer that retrieves data from a queue and processes it.
 * It is designed to work with any queue implementing the `Queue<T>` interface.
 * Consumers are typically used in the Producer-Consumer design pattern to consume and 
 * process work items produced by producers.
 * 
 */
template<typename T>
class Consumer
{

public:

    /*!
     * \brief Constructor
     * \param[in] queue Pointer to the queue from which data will be dequeued
     *
     * Initializes a consumer with a reference to the queue it will use for dequeuing data.
     */
    explicit Consumer(Queue<T> *queue) : mQueue(queue) {}

    /*!
     * \brief Virtual destructor
     */
    virtual ~Consumer() = default;

    /*!
     * \brief Consume operator
     *
     * This method defines the behavior for consuming data from the queue. The specific
     * implementation should handle dequeuing and processing of the data.
     */
    virtual void operator() () = 0;

protected:

    /*!
     * \brief Access the queue
     * \return Pointer to the associated queue
     *
     * Provides derived classes access to the queue for dequeuing data.
     */
    auto queue() -> Queue<T>*
    {
        return mQueue;
    }

private:

    Queue<T> *mQueue;

};


/*! \} */


} // End namespace tl

