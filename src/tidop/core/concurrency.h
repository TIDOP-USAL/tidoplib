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
 * License along with Foobar. If not, see <http://www.gnu.org/licenses/>. *
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

namespace tl
{


/*! \addtogroup utilities
 *  \{
 */

TL_EXPORT uint32_t optimalNumberOfThreads();


/*!
 * \brief Ejecuta una función en paralelo
 * \param[in] ini
 * \param[in] end
 * \param[in] f Función o lambda
 */
TL_EXPORT void parallel_for(size_t ini, 
                            size_t end, 
                            const std::function<void(size_t)> &f);



/*!
 * \brief Ejecuta una función en paralelo
 * Método sobrecargado para trabajar con contenedores
 * \param[in] it_begin
 * \param[in] it_end
 * \param[out] it_out_begin 
 * \param[in] f Función o lambda
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



template<typename T>
class QueueSPSC
{

public:

  QueueSPSC();
  QueueSPSC(size_t capacity);
  ~QueueSPSC();

  void push(const T &value);
  void pop(T &value);
  size_t size() const;

private:

  size_t mCapacity;
  std::queue<T> mBuffer;
  std::mutex mMutex;
  std::condition_variable mConditionVariable;
};

template<typename T>
QueueSPSC<T>::QueueSPSC()
  : mCapacity(256)
{
}

template<typename T>
QueueSPSC<T>::QueueSPSC(size_t capacity)
  : mCapacity(capacity)
{
}

template<typename T>
QueueSPSC<T>::~QueueSPSC()
{
}

template<typename T>
void QueueSPSC<T>::push(const T &value)
{
  std::unique_lock<std::mutex> locker(mMutex);

  while (true) {
    mConditionVariable.wait(locker, [this]() {return mBuffer.size() < mCapacity; });
    mBuffer.push(value);
    locker.unlock();
    mConditionVariable.notify_one();
    return;
  }
}

template<typename T>
inline void QueueSPSC<T>::pop(T &value)
{
  std::unique_lock<std::mutex> locker(mMutex);
  while (true) {
    mConditionVariable.wait(locker, [this]() {return mBuffer.size() > 0; });
    value = mBuffer.front();
    mBuffer.pop();
    locker.unlock();
    mConditionVariable.notify_one();
    return;
  }
}

template<typename T>
inline size_t QueueSPSC<T>::size() const
{
  return mBuffer.size();
}




/*--------------------------------------------------------------------------------*/


template<typename T>
class Producer
{
public:

  Producer(QueueSPSC<T> *queue);
  ~Producer();

  virtual void operator() () = 0;

private:

  QueueSPSC<T> *mQueue;

};

template<typename T>
Producer<T>::Producer(QueueSPSC<T> *queue)
  : mQueue(queue) 
{
}

template<typename T>
Producer<T>::~Producer()
{
}


template<typename T>
class Consumer
{
public:

  Consumer(QueueSPSC<T> *queue);
  ~Consumer();

  virtual void operator() () = 0;

private:

  QueueSPSC<T> *mQueue;

};

template<typename T>
Consumer<T>::Consumer(QueueSPSC<T> *queue)
  : mQueue(queue) 
{
}

template<typename T>
Consumer<T>::~Consumer()
{
}



#ifdef TL_ENABLE_DEPRECATED_METHODS
/*!
 * \brief número optimo de hilos
 * \deprecated Usar en su lugar optimalNumberOfThreads()
 */
TL_DEPRECATED("optimalNumberOfThreads()", "2.0")
TL_EXPORT uint32_t getOptimalNumberOfThreads();
#endif // TL_ENABLE_DEPRECATED_METHODS


/*! \} */ // end of utilities


} // End namespace tl


#endif // TL_CORE_CONCURRENCY_H