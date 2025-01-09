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
 * \brief Single-producer Single-consumer queue
 * 
 * This class implements a thread-safe queue designed for single-producer, single-consumer (SPSC) scenarios.
 * It efficiently synchronizes the producer and consumer threads, using blocking push and pop operations.
 * 
 */
template<typename T>
class QueueSPSC
  : public Queue<T>
{

private:

    std::condition_variable conditionVariable;

public:

    /*!
     * \brief Default constructor
     *
     * Creates an SPSC queue with the default capacity.
     */
    QueueSPSC() = default;
  
    /*!
     * \brief Constructor with specified queue capacity
     * \param[in] capacity Maximum capacity of the queue
     *
     * Creates an SPSC queue with the specified capacity.
     */
    explicit QueueSPSC(size_t capacity);
  
    ~QueueSPSC() override = default;
  
    TL_DISABLE_COPY(QueueSPSC)
    TL_DISABLE_MOVE(QueueSPSC)
    /*!
     * \brief Inserts an element into the queue
     * \param[in] value Element to insert into the queue
     * 
     * If the queue is full, the producer thread will block until space becomes available.
     */
    void push(const T &value) override;

    /*!
     * \brief Extracts the first element from the queue
     * \param[out] value Extracted element from the queue
     * \return Returns `true` if an element was successfully extracted; `false` if the queue is empty.
     *
     * If the queue is empty, the consumer thread will block until an element becomes available.
     */
    auto pop(T &value) -> bool override;

};



/* Implementation */

template<typename T>
QueueSPSC<T>::QueueSPSC(size_t capacity)
  : Queue<T>(capacity)
{
}

template<typename T>
void QueueSPSC<T>::push(const T &value)
{
    std::unique_lock<std::mutex> locker(this->mutex());

    conditionVariable.wait(locker, [this]() {
        return this->buffer().size() < this->capacity();
    });

    this->buffer().push(value);
    locker.unlock();
    conditionVariable.notify_one();
}

template<typename T>
auto QueueSPSC<T>::pop(T& value) -> bool
{
    std::unique_lock<std::mutex> locker(this->mutex());

    conditionVariable.wait(locker, [this]() {
        return !this->buffer().empty();
    });

    value = this->buffer().front();
    this->buffer().pop();
    locker.unlock();
    conditionVariable.notify_one();

    return true;

}


/*! \} */


} // End namespace tl

