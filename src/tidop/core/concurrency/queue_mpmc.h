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
 * \brief Multi-producer multi-consumer queue
 * 
 * This class implements a thread-safe queue designed for multi-producer, multi-consumer (MPMC) scenarios.
 * It supports concurrent push and pop operations with synchronization, ensuring thread safety.
 * The queue can be stopped to notify all threads that no more elements should be added or processed.
 *
 */
template<typename T>
class QueueMPMC
  : public Queue<T>
{

private:

    std::condition_variable conditionVariable;
    bool queueStop{false};

public:

    /*!
     * \brief Default constructor
     *
     * Creates an MPMC queue with the default capacity (256).
     */
    QueueMPMC();
    
    /*!
     * \brief Constructor with specified queue capacity
     * \param[in] capacity Maximum capacity of the queue
     *
     * Creates an MPMC queue with the specified capacity.
     */
    explicit QueueMPMC(size_t capacity);
    
    ~QueueMPMC() override = default;
    
    TL_DISABLE_COPY(QueueMPMC)
    TL_DISABLE_MOVE(QueueMPMC)

    /*!
     * \brief Inserts an element into the queue
     * \param[in] value Element to insert into the queue
     *
     * If the queue is full, the thread will block until space becomes available or the queue is stopped.
     * If the queue has been stopped, the element will not be added.
     */
    void push(const T &value) override;
    /*!
     * \brief Extracts the first element from the queue
     * \param[out] value Extracted element from the queue
     * \return Returns `true` if an element was successfully extracted; `false` if the queue is empty and stopped.
     *
     * If the queue is empty, the thread will block until an element becomes available or the queue is stopped.
     * If stopped, the operation returns `false`.
     */
    auto pop(T &value) -> bool override;
    
    /*!
     * \brief Stops the queue and notifies all waiting threads
     *
     * This function signals all threads to stop blocking on push or pop operations. Once the queue is stopped,
     * no new elements will be added, and pop operations will return `false` if the queue is empty.
     */
    void stop();

};



/* Implementation */

template<typename T>
QueueMPMC<T>::QueueMPMC()
  : Queue<T>()
{
}

template<typename T>
QueueMPMC<T>::QueueMPMC(size_t capacity)
  : Queue<T>(capacity)
{
}

template<typename T>
void QueueMPMC<T>::push(const T &value)
{
    std::unique_lock<std::mutex> locker(this->mutex());

    conditionVariable.wait(locker, [this]() {
        return this->buffer().size() < this->capacity() || queueStop;
    });

    if (!queueStop) {
        this->buffer().push(value);
    }

    locker.unlock();
    conditionVariable.notify_one();
}

template<typename T>
bool QueueMPMC<T>::pop(T &value)
{
    std::unique_lock<std::mutex> locker(this->mutex());

    conditionVariable.wait(locker, [this]() {
        return !this->buffer().empty() || queueStop;
    });

    bool read_buffer = !this->buffer().empty();

    if (read_buffer) {
        value = this->buffer().front();
        this->buffer().pop();
    }

    locker.unlock();
    conditionVariable.notify_one();

    return read_buffer;
}

template<typename T>
void QueueMPMC<T>::stop()
{
    std::unique_lock<std::mutex> locker(this->mutex());
    queueStop = true;
    conditionVariable.notify_all();
}



/*! \} */


} // End namespace tl

