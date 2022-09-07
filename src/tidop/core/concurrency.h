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

#ifndef TL_CORE_CONCURRENCY_H
#define TL_CORE_CONCURRENCY_H

#include "config_tl.h"
#include "tidop/core/defs.h"

#include <functional>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <future>

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
  * \param[in] ini Initial index
  * \param[in] end End index
  * \param[in] f Function or lambda
  */
TL_EXPORT void parallel_for(size_t ini, 
                            size_t end, 
                            std::function<void(size_t)> f);



template<typename Iter, typename Function>
Function parallel_for_each(Iter first,
                           Iter last,
                           Function f)
{
  auto f_aux = [&](Iter ini, Iter end) {
    while(ini != end) {
      f(*ini++);
    }
  };

  size_t num_threads = optimalNumberOfThreads();
  std::vector<std::thread> threads(num_threads);
  auto size = std::distance(first, last);
  size_t block_size = size / num_threads;

  Iter block_ini = first;
  Iter block_end = block_ini;

  for(size_t i = 0; i < num_threads; i++) {

    if (i == num_threads - 1) {
      block_end = last;
    } else {
      block_end = block_ini;
      std::advance(block_end, block_size);
    }

    threads[i] = std::thread(f_aux, block_ini, block_end);

    block_ini = block_end;
  }

  for(auto &_thread : threads) {
    if(_thread.joinable())
      _thread.join();
  }

  return f;
}

template<typename Iterator, typename Func>
void parallel_for_each_2(Iterator first,
                         Iterator last, 
                         Func f)
{
  unsigned long const length = std::distance(first, last);
  if(!length)
    return;
  unsigned long const min_per_thread = 25;
  unsigned long const max_threads = (length + min_per_thread - 1) / min_per_thread;
  unsigned long const hardware_threads = std::thread::hardware_concurrency();
  unsigned long const num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
  unsigned long const block_size = length / num_threads;
  std::vector<std::future<void> > futures(num_threads - 1);
  std::vector<std::thread> threads(num_threads - 1);
  //join_threads joiner(threads);
  Iterator block_start = first;
  for(unsigned long i = 0; i < (num_threads - 1); ++i) {
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
  for(unsigned long i = 0; i < (num_threads - 1); ++i) {
    futures[i].get();
  }

  for(auto &_thread : threads) {
    if(_thread.joinable())
      _thread.join();
  }
}

template<typename Iterator, typename Func>
void parallel_for_each_3(Iterator first, 
                       Iterator last, 
                       Func f)
{
  unsigned long const length = std::distance(first, last);
  if(!length)
    return;
  unsigned long const min_per_thread = 25;
  if(length < (2 * min_per_thread)) {
    std::for_each(first, last, f);
  } else {
    Iterator const mid_point = first + length / 2;
    std::future<void> first_half = std::async(&parallel_for_each_2<Iterator, Func>,
      first, mid_point, f);
    parallel_for_each_2(mid_point, last, f);
    first_half.get();
  }
}

/*--------------------------------------------------------------------------------*/


constexpr auto QueueDefaultCapacity = 256;

template<typename T>
class Queue
{

public:

  Queue() = default;
  Queue(size_t capacity) : mCapacity(capacity) {}

  virtual ~Queue() = default;

  /*!
   * \brief Inserts an element at the end
   * \param[in] value
   */
  virtual void push(const T &value) = 0;

  /*!
   * \brief Removes the first element
   * \param[in] value
   */
  virtual bool pop(T &value) = 0;

  /*!
   * \brief Returns the number of elements
   * \return Number of elements
   */
  size_t size() const;

  /*!
   * \brief Returns the capacity
   * \return Maximun number of elements in queue
   */
  size_t capacity() const;

  bool empty() const;
  bool full() const;

protected:

  std::queue<T> &buffer();
  std::mutex &mutex();

private:

  size_t mCapacity{QueueDefaultCapacity};
  std::queue<T> mBuffer;
  mutable std::mutex mMutex;

};

template<typename T>
inline size_t Queue<T>::size() const
{
  std::lock_guard<std::mutex> locker(mMutex);
  return mBuffer.size();
}

template<typename T>
inline size_t Queue<T>::capacity() const
{
  return mCapacity;
}

template<typename T>
inline bool Queue<T>::empty() const
{
  std::lock_guard<std::mutex> locker(mMutex);
  return mBuffer.empty();
}

template<typename T>
inline bool Queue<T>::full() const
{
  std::lock_guard<std::mutex> locker(mMutex);
  return mBuffer.size() < mCapacity;
}

template<typename T>
inline std::queue<T> &Queue<T>::buffer()
{
  return mBuffer;
}

template<typename T>
inline std::mutex &Queue<T>::mutex()
{
  return mMutex;
}



/*--------------------------------------------------------------------------------*/



/*!
 * \brief Single-producer Single-consumer queue
 */
template<typename T>
class QueueSPSC
  : public Queue<T>
{

public:

  QueueSPSC() = default;
  QueueSPSC(size_t capacity);
  QueueSPSC(const QueueSPSC &) = delete;
  QueueSPSC(QueueSPSC &&) = delete;
  ~QueueSPSC() = default;

  void operator=(const QueueSPSC &) = delete;
  void operator=(QueueSPSC &&) = delete;

  /*!
   * \brief Inserts an element at the end
   * \param[in] value
   */
  void push(const T &value) override;

  /*!
   * \brief Removes the first element
   * \param[in] value
   */
  bool pop(T &value) override;

private:

  std::condition_variable mConditionVariable;
};


template<typename T>
QueueSPSC<T>::QueueSPSC(size_t capacity)
  : Queue(capacity)
{
}

template<typename T>
void QueueSPSC<T>::push(const T &value)
{
  std::unique_lock<std::mutex> locker(mutex());

  mConditionVariable.wait(locker, [this]() {
    return buffer().size() < capacity();
  });

  buffer().push(value);
  locker.unlock();
  mConditionVariable.notify_one();
}

template<typename T>
inline bool QueueSPSC<T>::pop(T &value)
{
  std::unique_lock<std::mutex> locker(mutex());

  mConditionVariable.wait(locker, [this]() {
    return !buffer().empty();
  });

  value = buffer().front();
  buffer().pop();
  locker.unlock();
  mConditionVariable.notify_one();

  return true;

}


/*--------------------------------------------------------------------------------*/


/*!
 * \brief Multi-producer multi-consumer queue
 */
template<typename T>
class QueueMPMC
  : public Queue<T>
{

public:

  QueueMPMC() = default;
  QueueMPMC(size_t capacity);
  QueueMPMC(const QueueMPMC &) = delete;
  QueueMPMC(QueueMPMC &&) = delete;
  ~QueueMPMC() = default;

  void operator=(const QueueMPMC &) = delete;
  void operator=(QueueMPMC &&) = delete;

  /*!
   * \brief Inserts an element at the end
   * \param[in] value
   */
  void push(const T &value) override;

  /*!
   * \brief Removes the first element
   * \param[in] value
   */
  bool pop(T &value) override;

  void stop()
  {
    std::unique_lock<std::mutex> locker(mutex());
    mStop = true;
    mConditionVariable.notify_all();
  }

private:

  std::condition_variable mConditionVariable;
  bool mStop;
};


template<typename T>
QueueMPMC<T>::QueueMPMC(size_t capacity)
  : Queue(capacity),
  mStop(false)
{
}

template<typename T>
void QueueMPMC<T>::push(const T &value)
{
  std::unique_lock<std::mutex> locker(mutex());

  mConditionVariable.wait(locker, [this]() {
    return buffer().size() < capacity() || mStop;
  });

  if (!mStop) {
    buffer().push(value);
  }

  locker.unlock();
  mConditionVariable.notify_one();
}

template<typename T>
inline bool QueueMPMC<T>::pop(T &value)
{
  std::unique_lock<std::mutex> locker(mutex());

  mConditionVariable.wait(locker, [this]() {
    return !buffer().empty() || mStop;
  });

  bool read_buffer = !buffer().empty();

  if (read_buffer) {
    value = buffer().front();
    buffer().pop();
  }

  locker.unlock();
  mConditionVariable.notify_one();

  return read_buffer;
}




/*--------------------------------------------------------------------------------*/




/*!
 * \brief Producer Interface
 */
template<typename T>
class Producer
{
public:

  explicit Producer(Queue<T> *queue) : mQueue(queue) {}
  ~Producer() = default;

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


/*--------------------------------------------------------------------------------*/


/*!
 * \brief Consumer Interface
 */
template<typename T>
class Consumer
{
public:

  explicit Consumer(Queue<T> *queue) : mQueue(queue) {}
  ~Consumer() = default;

  virtual void operator() () = 0;

protected:

  Queue<T> *queue()
  {
    return mQueue;
  }

private:

  Queue<T> *mQueue;

};


/*! \} */ // end of concurrency

/*! \} */ // end of core


} // End namespace tl


#endif // TL_CORE_CONCURRENCY_H
