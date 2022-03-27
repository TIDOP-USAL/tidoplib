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
                            const std::function<void(size_t)> &f);



/*!
 * \brief Iterate over containers and execute a function in parallel
 * \param[in] it_begin Input iterator to the beginning of the first range of elements
 * \param[in] it_end Input iterator to the end of the first range of elements
 * \param[out] it_out_begin Output iterator to the beginning of the second range of elements
 * \param[in] f Function or lambda
 */
template<typename itIn, typename itOut> inline
void parallel_for(itIn it_begin, 
                  itIn it_end, 
                  itOut &it_out_begin, 
                  std::function<void(itIn, itIn, itOut &)> f)
{
  auto f_aux = [&](itIn ini, itIn end, itOut *out) {
    while (ini != end) {
      f(ini, end, out);
    }
  };

  int num_threads = optimalNumberOfThreads();
  std::vector<std::thread> threads(num_threads);
  auto size = std::distance(it_begin, it_end);
  int _size = size / num_threads;
  for (int i = 0; i < num_threads; i++) {
    itIn _ini = i * _size + it_begin;
    itIn _end = _ini + _size;
    itOut _out = i * _size + it_out_begin;
    if (i == num_threads -1) _end = it_end;
    threads[i] = std::thread(f_aux, _ini, it_out_begin, _end);
  }

  for (auto &_thread : threads) _thread.join();
}




/*--------------------------------------------------------------------------------*/

constexpr auto QueueMPMCDefaultCapacity = 256;

/*!
 * \brief Multi-producer multi-consumer queue
 */
template<typename T>
class QueueMPMC
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
   * \brief Inserts element at the end
   * \param[in] value
   */
  void push(const T &value);

  /*!
   * \brief Removes the first element
   * \param[in] value
   */
  void pop(T &value);

  /*!
   * \brief Returns the number of elements
   * \return Number of elements
   */
  size_t size() const;

private:

  size_t mCapacity{QueueMPMCDefaultCapacity};
  std::queue<T> mBuffer;
  mutable std::mutex mMutex;
  std::condition_variable mConditionVariable;
};


template<typename T>
QueueMPMC<T>::QueueMPMC(size_t capacity)
  : mCapacity(capacity)
{
}

template<typename T>
void QueueMPMC<T>::push(const T &value)
{
  std::unique_lock<std::mutex> locker(mMutex);

  while (true) {
    mConditionVariable.wait(locker, [this]() {
      return mBuffer.size() < mCapacity; 
      });
    mBuffer.push(value);
    locker.unlock();
    mConditionVariable.notify_one();
    return;
  }
}

template<typename T>
inline void QueueMPMC<T>::pop(T &value)
{
  std::unique_lock<std::mutex> locker(mMutex);
  while (true) {
    mConditionVariable.wait(locker, [this]() {
      return mBuffer.size() > 0; 
      });
    value = mBuffer.front();
    mBuffer.pop();
    locker.unlock();
    mConditionVariable.notify_one();
    return;
  }
}

template<typename T>
inline size_t QueueMPMC<T>::size() const
{
  std::unique_lock<std::mutex> locker(mMutex);
  return mBuffer.size();
}



/*!
 * \brief Producer Interface
 */
template<typename T>
class Producer
{
public:

  explicit Producer(QueueMPMC<T> *queue) : mQueue(queue){}
  Producer(const Producer &) = delete;
  Producer(Producer &&) = delete;
  ~Producer() = default;

  void operator=(const Producer &) = delete;
  void operator=(Producer &&) = delete;

  virtual void operator() (size_t size) = 0;
  virtual void operator() (size_t ini, size_t end) = 0;

private:

  QueueMPMC<T> *mQueue;

};


/*!
 * \brief Consumer Interface
 */
template<typename T>
class Consumer
{
public:

  explicit Consumer(QueueMPMC<T> *queue) : mQueue(queue){}
  Consumer(const Consumer &) = delete;
  Consumer(Consumer &&) = delete;
  ~Consumer() = default;

  void operator=(const Consumer &) = delete;
  void operator=(Consumer &&) = delete;

  virtual void operator() () = 0;

private:

  QueueMPMC<T> *mQueue;

};


/*! \} */ // end of concurrency

/*! \} */ // end of core


} // End namespace tl


#endif // TL_CORE_CONCURRENCY_H
