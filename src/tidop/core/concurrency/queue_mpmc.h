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
 * \brief Multi-producer multi-consumer queue
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
     */
    QueueMPMC();
    
    /*!
     * \brief Constructor with queue capacity
     * \param[in] capacity Queue capacity
     */
    explicit QueueMPMC(size_t capacity);
    
    ~QueueMPMC() override = default;
    
    TL_DISABLE_COPY(QueueMPMC)
    TL_DISABLE_MOVE(QueueMPMC)
    
    void push(const T &value) override;
    bool pop(T &value) override;
    
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



/*! \} */ // end of concurrency

/*! \} */ // end of core


} // End namespace tl

