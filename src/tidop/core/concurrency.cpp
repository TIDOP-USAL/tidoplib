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

#include "tidop/core/concurrency.h"

#if defined TL_HAVE_OPENMP
#include <omp.h>  // OpenMP
#endif

#if defined (TL_HAVE_PPL) && ( defined (_MSC_VER) && _MSC_VER >= 1600)
#  define TL_MSVS_CONCURRENCY
#  include <ppl.h>  // Parallel Patterns Library (PPL)
#endif

#include <vector>


namespace tl
{



uint32_t optimalNumberOfThreads()
{
  uint32_t n_threads = 0;

#ifdef TL_HAVE_OPENMP
  n_threads =  omp_get_max_threads();
//#elif defined TL_MSVS_CONCURRENCY
//  return Concurrency::CurrentScheduler::Get()->GetNumberOfVirtualProcessors();
#else
  n_threads = std::thread::hardware_concurrency();
#endif
  return n_threads == 0 ? 1 : n_threads;
}

void parallel_for(size_t ini, 
                  size_t end, 
                  std::function<void(size_t)> f)
{
  size_t size = end - ini;
  if(size == 0) return;

#ifdef TL_HAVE_OPENMP
  #pragma omp parallel for
  for (long long i = static_cast<long long>(ini); i < static_cast<long long>(end); i++) {
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

  size_t block_size = size / num_threads;

  size_t block_ini = 0;
  size_t block_end = 0;

  for (size_t i = 0; i < num_threads; i++) {

    if (i == num_threads -1) block_end = end;
    else block_end = block_ini + block_size;

    threads[i] = std::thread(f_aux, block_ini, block_end);

    block_ini = block_end;
  }

  for(auto &_thread : threads) {
    if(_thread.joinable())
      _thread.join();
  }

#endif

}

} // End namespace tl