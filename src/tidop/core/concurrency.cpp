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

#include "concurrency.h"

#if defined HAVE_OMP
#include <omp.h>  // OpenMP
#endif

#if defined (HAVE_PPL) && ( defined (_MSC_VER) && _MSC_VER >= 1600)
#  define TL_MSVS_CONCURRENCY
#  include <ppl.h>  // Parallel Patterns Library (PPL)
#endif

#include <vector>



///TODO: A?adir m?todo para ejecutar c?digo de forma asincrona
// std::async
// Concurrency::task<T> (PPL)

namespace tl
{

uint32_t optimalNumberOfThreads()
{
#ifdef HAVE_OMP
  //TODO: Sin probar
  return omp_get_max_threads();
#elif defined TL_MSVS_CONCURRENCY
  return Concurrency::CurrentScheduler::Get()->GetNumberOfVirtualProcessors();
#else
  uint32_t n_threads = std::thread::hardware_concurrency();
  return n_threads == 0 ? 1 : n_threads;
#endif
}


void parallel_for(size_t ini, 
                  size_t end, 
                  const std::function<void(size_t)> &f)
{
  //uint64_t time_ini = getTickCount();
#ifdef HAVE_OMP
  //TODO: Sin probar
  #pragma omp parallel for
  for (size_t i = ini; i < end; i++) {
    f(i);
  }
#elif defined TL_MSVS_CONCURRENCY
  Concurrency::cancellation_token_source cts;
  //Concurrency::run_with_cancellation_token([ini, end, f]() {
  //  Concurrency::parallel_for(ini, end, f);
  //},cts.get_token());
  Concurrency::parallel_for(ini, end, f);
#else

  auto f_aux = [&](size_t ini, size_t end) {
    for (size_t r = ini; r < end; r++) {
      f(r);
    }
  };

  size_t num_threads = optimalNumberOfThreads();
  std::vector<std::thread> threads(num_threads);

  size_t size = (end - ini) / num_threads;
  for (size_t i = 0; i < num_threads; i++) {
    size_t _ini = i * size + ini;
    size_t _end = _ini + size;
    if (i == num_threads -1) _end = end;
    threads[i] = std::thread(f_aux, _ini, _end);
  }

  for (auto &_thread : threads) _thread.join();
#endif
  //double time = (getTickCount() - time_ini) / 1000.;
  //printf("Time %f", time);
}




#ifdef TL_ENABLE_DEPRECATED_METHODS

uint32_t getOptimalNumberOfThreads()
{
#ifdef HAVE_OMP
  //TODO: Sin probar
  return omp_get_max_threads();
#elif defined TL_MSVS_CONCURRENCY
  return Concurrency::CurrentScheduler::Get()->GetNumberOfVirtualProcessors();
#else
  uint32_t n_threads = std::thread::hardware_concurrency();
  return n_threads == 0 ? 1 : n_threads;
#endif
}

#endif // TL_ENABLE_DEPRECATED_METHODS

} // End namespace tl