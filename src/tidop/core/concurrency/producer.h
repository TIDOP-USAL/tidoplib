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
 * \brief Producer Interface
 *
 * This interface defines a producer that generates data and adds it to a queue.
 * It is designed to work with any queue implementing the `Queue<T>` interface.
 * Producers are typically used in the Producer-Consumer design pattern to generate
 * work items that consumers process.
 *
 */
template<typename T>
class Producer
{

public:

    /*!
     * \brief Constructor
     * \param[in] queue Pointer to the queue where data will be enqueued
     *
     * Initializes a producer with a reference to the queue it will use for enqueuing data.
     */
    explicit Producer(Queue<T> *queue) : mQueue(queue) {}

    virtual ~Producer() = default;

    /*!
     * \brief Default produce operator
     *
     * This method generates and enqueues data. The specific implementation should
     * define how the data is produced and added to the queue.
     */
    virtual void operator() () = 0;

    /*!
     * \brief Range-based produce operator
     * \param[in] ini Starting index of the range
     * \param[in] end Ending index of the range
     *
     * This method produces and enqueues data within a specified range. The specific
     * implementation should define how the data is generated and added to the queue
     * for the given range.
     */
    virtual void operator() (size_t ini, size_t end) = 0;

protected:

    /*!
     * \brief Access the queue
     * \return Pointer to the associated queue
     *
     * Provides derived classes access to the queue for enqueuing data.
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

