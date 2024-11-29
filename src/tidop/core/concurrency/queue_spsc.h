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
#include "tidop/core/defs.h"
#include "tidop/core/concurrency/queue.h"

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
 
/*!
 * \brief Single-producer Single-consumer queue
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
     */
    QueueSPSC() = default;
  
    /*!
     * \brief Constructor with queue capacity
     * \param[in] capacity Queue capacity
     */
    explicit QueueSPSC(size_t capacity);
  
    ~QueueSPSC() override = default;
  
    TL_DISABLE_COPY(QueueSPSC)
    TL_DISABLE_MOVE(QueueSPSC)

    void push(const T &value) override;
    bool pop(T &value) override;

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


/*! \} */ // end of concurrency

/*! \} */ // end of core


} // End namespace tl

