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
#include "tidop/core/concurrency.h"

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

///Pruebas basadas en C++ Concurrency in Action
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

/*! \} */ // end of concurrency

/*! \} */ // end of core


} // End namespace tl

