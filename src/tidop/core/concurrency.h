/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by Esteban Ruiz de O�a Crespo                       *
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

namespace tl
{


/*! \addtogroup utilities
 *  \{
 */

TL_EXPORT uint32_t optimalNumberOfThreads();


/*!
 * \brief Ejecuta una funci�n en paralelo
 * \param[in] ini
 * \param[in] end
 * \param[in] f Funci�n o lambda
 */
TL_EXPORT void parallel_for(size_t ini, 
                            size_t end, 
                            const std::function<void(size_t)> &f);



/*!
 * \brief Ejecuta una funci�n en paralelo
 * M�todo sobrecargado para trabajar con contenedores
 * \param[in] it_begin
 * \param[in] it_end
 * \param[out] it_out_begin 
 * \param[in] f Funci�n o lambda
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


class Queue
{
public:
  Queue();
  ~Queue();

private:

};

Queue::Queue()
{
}

Queue::~Queue()
{
}


class Producer
{
public:
  Producer(Queue &queue);
  ~Producer();

private:

};

Producer::Producer(Queue &queue)
{
}

Producer::~Producer()
{
}

class Consumer
{
public:
  Consumer(Queue &queue);
  ~Consumer();

private:

};

Consumer::Consumer(Queue &queue)
{
}

Consumer::~Consumer()
{
}



#ifdef TL_ENABLE_DEPRECATED_METHODS
/*!
 * \brief n�mero optimo de hilos
 * \deprecated Usar en su lugar optimalNumberOfThreads()
 */
TL_DEPRECATED("optimalNumberOfThreads()", "2.0")
TL_EXPORT uint32_t getOptimalNumberOfThreads();
#endif // TL_ENABLE_DEPRECATED_METHODS


/*! \} */ // end of utilities


} // End namespace tl


#endif // TL_CORE_CONCURRENCY_H