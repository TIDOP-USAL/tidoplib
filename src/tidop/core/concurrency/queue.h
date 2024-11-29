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

#include <mutex>
#include <queue>

#include "tidop/config.h"
#include "tidop/core/defs.h"

namespace tl
{


/*! \addtogroup core
 *  \{
 */
 
/*!
 * \addtogroup concurrency
 *
 * \{
 */
 
constexpr auto QueueDefaultCapacity = 256;

template<typename T>
class Queue
{

private:

    size_t queueCapacity{QueueDefaultCapacity};
    std::queue<T> queueBuffer;
    mutable std::mutex _mutex;

public:

    /*!
     * \brief Default constructor
     */
    Queue() = default;
    
    /*!
     * \brief Constructor with queue capacity
     * \param[in] capacity Queue capacity
     */
    explicit Queue(size_t capacity);
    
    virtual ~Queue() = default;
    
    TL_DISABLE_COPY(Queue)
    TL_DISABLE_MOVE(Queue)
    
    /*!
     * \brief Inserts an element at the end of the queue
     * \param[in] value Element to insert
     */
    virtual void push(const T &value) = 0;
    
    /*!
     * \brief Extact the first item from the queue
     * \param[out] value Extact element
     */
    virtual auto pop(T& value) -> bool = 0;
    
    /*!
     * \brief Returns the number of elements
     * \return Size of the queue
     */
    auto size() const -> size_t;
    
    /*!
     * \brief Returns the capacity of the queue
     * \return Maximun number of elements in queue
     */
    auto capacity() const -> size_t;
    
    /*!
     * \brief checks whether the queue is empty
     */
    auto empty() const -> bool;

    /*!
     * \brief checks whether the queue is full
     */
    auto full() const -> bool;


protected:

    std::queue<T> &buffer();
    std::mutex &mutex() const;

};



/* Implementation */


template<typename T>
Queue<T>::Queue(size_t capacity)
  : queueCapacity(capacity)
{
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
    return queueBuffer.size() < queueCapacity;
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


/*! \} */ // end of concurrency

/*! \} */ // end of core

} // End namespace tl

