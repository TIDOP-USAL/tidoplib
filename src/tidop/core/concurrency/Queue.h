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

/*!
 * \file Queue.h
 * \brief Thread-safe queue base class and concrete implementations
 *
 * This module provides thread-safe queue implementations for concurrent producer-consumer patterns.
 * It includes:
 * - Base abstract Queue class
 * - SPSC (Single Producer Single Consumer) queue
 * - MPMC (Multi Producer Multi Consumer) queue
 *
 * ### Features
 *
 * - Type-safe queue implementation with templates
 * - Thread-safe push/pop operations using mutexes
 * - Configurable capacity with overflow blocking
 * - Condition variables for efficient thread synchronization
 * - Support for queue stopping/shutdown
 *
 * \see QueueSPSC, QueueMPMC
 */
 
#pragma once

#include <mutex>
#include <queue>

#include "tidop/config.h"
#include "tidop/core/base/Exception.h"

namespace tl
{


/*!
 * \addtogroup concurrency
 *
 * \{
 */
 
/*!
 * \brief Default queue capacity if not specified
 */
constexpr auto QueueDefaultCapacity = 256;

/*!
 * \brief Abstract base class for a thread-safe queue with fixed capacity.
 * 
 * The `Queue` class defines a generic interface for a thread-safe queue implementation. 
 * It enforces the implementation of the `push` and `pop` methods in derived classes 
 * while providing common functionality such as checking size, capacity, and thread-safety 
 * via mutex locks.
 *
 * ### Thread Safety
 *
 * All operations are protected by an internal mutex. Multiple threads can safely call
 * push() and pop() concurrently on the same queue instance.
 *
 * \tparam T Element type
 */
template<typename T>
class Queue
{

private:

    size_t queueCapacity{QueueDefaultCapacity};
    std::queue<T> queueBuffer;
    mutable std::mutex _mutex;

public:

    /*!
     * \brief Default constructor.
     * Creates a queue with the default capacity (256 elements).
     */
    Queue() = default;
    
    /*!
     * \brief Constructor with queue capacity.
     * \param[in] capacity Queue maximum capacity. Must be greater than 0.
     *
     * \exception Exception If capacity is 0
     */
    explicit Queue(size_t capacity);

    /*!
     * \brief Virtual destructor
     */
    virtual ~Queue() = default;
    
    TL_DISABLE_COPY(Queue)
    TL_DISABLE_MOVE(Queue)
    
    /*!
     * \brief Inserts an element at the end of the queue.
     * \param[in] value Element to insert.
     * \note This function must be implemented in derived classes.
     */
    virtual void push(const T &value) = 0;
    
    /*!
     * \brief Extracts the first item from the queue.
     * \param[out] value Extracted element.
     * \return `true` if an element was successfully extracted, `false` if the queue was empty.
     * \note This function must be implemented in derived classes.
     */
    virtual auto pop(T& value) -> bool = 0;
    
    /*!
     * \brief Returns the number of elements currently in the queue.
     * \return Current size of the queue (thread-safe).
     *
     * Note: The returned size may change immediately after this call returns
     * if other threads are pushing/popping elements.
     */
    auto size() const -> size_t;
    
    /*!
     * \brief Returns the maximum capacity of the queue.
     * \return Maximum number of elements the queue can hold.
     */
    auto capacity() const -> size_t;
    
    /*!
     * \brief Checks whether the queue is empty.
     * \return `true` if the queue is empty (thread-safe), `false` otherwise.
     */
    auto empty() const -> bool;

    /*!
     * \brief Checks whether the queue is full.
     * \return `true` if the number of elements equals capacity, `false` otherwise (thread-safe).
     */
    auto full() const -> bool;


protected:

    /*!
     * \brief Provides access to the internal queue buffer.
     * \return Reference to the internal queue buffer.
     *
     * \warning Caller must hold the lock when accessing the buffer.
     * Use with mutex() accessor to get the lock.
     */
    auto buffer() -> std::queue<T>&;

    /*!
     * \brief Provides access to the internal mutex for synchronization.
     * \return Reference to the mutex used for thread synchronization.
     *
     * Derived classes use this to implement custom locking strategies.
     */
    auto mutex() const -> std::mutex&;

};



/* Implementation */


template<typename T>
Queue<T>::Queue(size_t capacity)
  : queueCapacity(capacity)
{
    TL_ASSERT(capacity > 0, "Queue capacity must be greater than 0");
}

template<typename T>
auto Queue<T>::size() const -> size_t
{
    std::lock_guard<std::mutex> locker(_mutex);
    return queueBuffer.size();
}

template<typename T>
auto Queue<T>::capacity() const -> size_t
{
    return queueCapacity;
}

template<typename T>
bool Queue<T>::empty() const
{
    std::lock_guard<std::mutex> locker(_mutex);
    return queueBuffer.empty();
}

template<typename T>
auto Queue<T>::full() const -> bool
{
    std::lock_guard<std::mutex> locker(_mutex);
    return queueBuffer.size() >= queueCapacity;
}

template<typename T>
auto Queue<T>::buffer() -> std::queue<T>&
{
    return queueBuffer;
}

template<typename T>
auto Queue<T>::mutex() const -> std::mutex&
{
    return _mutex;
}


/*! \} */

} // End namespace tl

