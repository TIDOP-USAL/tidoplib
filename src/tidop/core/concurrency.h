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

#include <functional>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <future>
#include <algorithm>

#include "tidop/core/defs.h"
#include "tidop/math/math.h"

namespace tl
{


/*! \addtogroup core
 *  \{
 */

/*!
 * \defgroup concurrency Concurrency
 *
 * \{
 */

/*!
 * \brief Optimal number of threads
 */
TL_EXPORT uint32_t optimalNumberOfThreads();


/*!
 * \brief Iterates over a range of indices and executes a function in parallel
 * 
 * <h4>Example</h4>
 * 
 * \code
 * std::vector<int> nums{3, 4, 2, 8, 15, 267, 54, 60, 29, 20, 39};
 * std::vector<int> aux(nums.size());
 *
 * parallel_for(0, nums.size(), [&](size_t i) {
 *   aux[i] = nums[i] + 1;
 * });
 * \endcode
 * 
 * \param[in] ini Initial index
 * \param[in] end End index
 * \param[in] f Function or lambda
 */
TL_EXPORT void parallel_for(size_t ini, 
                            size_t end, 
                            std::function<void(size_t)> f);


/*!
 * \brief Iterates over a range and executes a function in parallel
 * 
 * <h4>Example</h4>
 * 
 * \code
 * std::vector<int> nums{3, 4, 2, 8, 15, 267, 54, 60, 29, 20, 39};
 * struct Sum
 * {
 *     void operator()(int n)
 *     {
 *         sum += n;
 *     }
 * 
 *     int sum{0};
 * };
 *
 * Sum sum = parallel_for_each(nums.begin(), nums.end(), Sum());
 * \endcode
 * 
 * \param[in] first First element
 * \param[in] last Last element
 * \param[in] f Function or lambda
 */
template<typename Iter, typename Function>
Function parallel_for_each(Iter first,
                           Iter last,
                           Function f)
{
    auto f_aux = [&](Iter ini, Iter end) {
        while (ini != end) {
            f(*ini++);
        }
    };

    size_t num_threads = optimalNumberOfThreads();
    std::vector<std::thread> threads(num_threads);
    auto size = std::distance(first, last);
    size_t block_size = size / num_threads;

    Iter block_ini = first;
    Iter block_end = block_ini;

    for (size_t i = 0; i < num_threads; i++) {

        if (i == num_threads - 1) {
            block_end = last;
        } else {
            block_end = block_ini;
            std::advance(block_end, block_size);
        }

        threads[i] = std::thread(f_aux, block_ini, block_end);

        block_ini = block_end;
    }

    for (auto &_thread : threads) {
        if (_thread.joinable())
            _thread.join();
    }

    return f;
}

/// \cond

template<typename Iterator, typename Func>
void parallel_for_each_2(Iterator first,
                         Iterator last,
                         Func f)
{
    unsigned long const length = std::distance(first, last);
    if (!length) return;

    unsigned long const min_per_thread = 25;
    unsigned long const max_threads = (length + min_per_thread - 1) / min_per_thread;
    unsigned long const hardware_threads = std::thread::hardware_concurrency();
    unsigned long const num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
    unsigned long const block_size = length / num_threads;
    std::vector<std::future<void> > futures(num_threads - 1);
    std::vector<std::thread> threads(num_threads - 1);

    Iterator block_start = first;
    for (unsigned long i = 0; i < (num_threads - 1); ++i) {
        Iterator block_end = block_start;
        std::advance(block_end, block_size);
        std::packaged_task<void(void)> task(
            [=]() {
                std::for_each(block_start, block_end, f);
            });
        futures[i] = task.get_future();
        threads[i] = std::thread(std::move(task));
        block_start = block_end;
    }

    std::for_each(block_start, last, f);
    for (unsigned long i = 0; i < (num_threads - 1); ++i) {
        futures[i].get();
    }

    for (auto &_thread : threads) {
        if (_thread.joinable())
            _thread.join();
    }
}

template<typename Iterator, typename Func>
void parallel_for_each_3(Iterator first,
                         Iterator last,
                         Func f)
{
    unsigned long const length = std::distance(first, last);
    if (!length) return;

    constexpr unsigned long min_per_thread = 25;
    if (length < (2 * min_per_thread)) {
        std::for_each(first, last, f);
    } else {
        Iterator const mid_point = first + length / 2;
        std::future<void> first_half = std::async(&parallel_for_each_2<Iterator, Func>,
                                                  first, mid_point, f);
        parallel_for_each_2(mid_point, last, f);
        first_half.get();
    }
}


/// \endcond


/*--------------------------------------------------------------------------------*/


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
    virtual bool pop(T &value) = 0;
    
    /*!
     * \brief Returns the number of elements
     * \return Size of the queue
     */
    size_t size() const;
    
    /*!
     * \brief Returns the capacity of the queue
     * \return Maximun number of elements in queue
     */
    size_t capacity() const;
    
    /*!
     * \brief checks whether the queue is empty
     */
    bool empty() const;

    /*!
     * \brief checks whether the queue is full
     */
    bool full() const;


protected:

    std::queue<T> &buffer();
    std::mutex &mutex();

};



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
    
    ~QueueMPMC() = default;
    
    TL_DISABLE_COPY(QueueMPMC)
    TL_DISABLE_MOVE(QueueMPMC)
    
    void push(const T &value) override;
    bool pop(T &value) override;
    
    void stop();

};


/*!
 * \brief Producer Interface
 */
template<typename T>
class Producer
{

public:

    explicit Producer(Queue<T> *queue) : mQueue(queue) {}
    virtual ~Producer() = default;

    TL_DISABLE_COPY(Producer)
    TL_DISABLE_MOVE(Producer)

    virtual void operator() () = 0;
    virtual void operator() (size_t ini, size_t end) = 0;

protected:

    Queue<T> *queue()
    {
        return mQueue;
    }

private:

    Queue<T> *mQueue;

};



/*!
 * \brief Consumer Interface
 */
template<typename T>
class Consumer
{
public:

    explicit Consumer(Queue<T> *queue) : mQueue(queue) {}
    virtual ~Consumer() = default;

	TL_DISABLE_COPY(Consumer)
    TL_DISABLE_MOVE(Consumer)

    virtual void operator() () = 0;

protected:

    Queue<T> *queue()
    {
        return mQueue;
    }

private:

    Queue<T> *mQueue;

};




/* Implementation */


/* Queue */

template<typename T>
Queue<T>::Queue(size_t capacity)
  : queueCapacity(capacity)
{
}

template<typename T>
inline size_t Queue<T>::size() const
{
    std::lock_guard<std::mutex> locker(_mutex);
    return queueBuffer.size();
}

template<typename T>
inline size_t Queue<T>::capacity() const
{
    return queueCapacity;
}

template<typename T>
inline bool Queue<T>::empty() const
{
    std::lock_guard<std::mutex> locker(_mutex);
    return queueBuffer.empty();
}

template<typename T>
inline bool Queue<T>::full() const
{
    std::lock_guard<std::mutex> locker(_mutex);
    return queueBuffer.size() < queueCapacity;
}

template<typename T>
inline std::queue<T> &Queue<T>::buffer()
{
    return queueBuffer;
}

template<typename T>
inline std::mutex &Queue<T>::mutex()
{
    return _mutex;
}


/* QueueSPSC */

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
inline bool QueueSPSC<T>::pop(T &value)
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


/* QueueMPMC */

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
inline bool QueueMPMC<T>::pop(T &value)
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
inline void QueueMPMC<T>::stop()
{
    std::unique_lock<std::mutex> locker(this->mutex());
    queueStop = true;
    conditionVariable.notify_all();
}



/*! \} */ // end of concurrency

/*! \} */ // end of core


} // End namespace tl

