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

/*!
 * \file Parallel.h
 * \brief Parallel computation utilities and thread management
 *
 * This module provides utilities for parallel iteration and reduction operations
 * with automatic thread pool management. It supports multiple parallel backends:
 * - OpenMP (if available)
 * - MSVC Parallel Patterns Library (PPL)
 * - Standard C++ threads (fallback)
 *
 * ### Functions
 *
 * - \ref optimalNumberOfThreads - Detects optimal thread count for the system
 * - \ref parallel_for - Parallel loop over index ranges
 * - \ref parallel_reduce - Parallel reduction operation over iterators
 * - \ref parallel_for_each - Alternative parallel iteration implementation
 *
 * ### Features
 *
 * - Automatic backend selection based on available libraries
 * - Efficient thread pool creation and management
 * - Load balancing across available processors
 * - Support for lambda functions and function objects
 *
 * \see tl::parallel_for, tl::parallel_reduce, tl::optimalNumberOfThreads
 */
 
#pragma once

#include "tidop/config.h"

#include <functional>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <stop_token>
#include <future>
#include <algorithm>

#if defined TL_HAVE_OPENMP
#include <omp.h>  // OpenMP
#endif

namespace tl
{


/*!
 * \addtogroup concurrency
 *
 * \{
 */

/*!
 * \brief Determines the optimal number of threads for parallel operations.
 *
 * This function detects the number of hardware threads available on the system
 * and returns the optimal number to use for parallel operations.
 *
 * \return Number of optimal threads (minimum: 1)
 *
 * \see parallel_for
 */
TL_EXPORT uint32_t optimalNumberOfThreads();

/*!
 * \brief Executes a function in parallel over a range of indices.
 *
 * This function provides a convenient way to parallelize loop operations over
 * a range of indices. It automatically divides the work among available threads
 * and coordinates their execution.
 *
 * ### Thread Safety
 *
 * The function object `f` must be thread-safe when accessing shared data structures.
 * Each thread receives a unique index, so operations on independent data are safe.
 *
 * ### Exception Handling
 *
 * Exceptions thrown in function `f` are propagated to the caller. The behavior
 * depends on the parallel backend being used.
 *
 * ### Example Usage
 * \code{.cpp}
 * std::vector<int> nums{3, 4, 2, 8, 15, 267, 54, 60, 29, 20, 39};
 * std::vector<int> aux(nums.size());
 *
 * parallel_for(0, nums.size(), [&](size_t i) {
 *   aux[i] = nums[i] + 1;
 * });
 * \endcode
 *
 * \tparam Function Type of the function/lambda. Must be callable with size_t parameter.
 * \param[in] ini First index to process (inclusive)
 * \param[in] end Last index to process (exclusive)
 * \param[in] f Function or lambda to execute for each index
 */
template<typename Function>
void parallel_for(size_t ini,
    size_t end,
    Function &&f,
    std::stop_token stopToken = {})
{
    size_t size = end - ini;
    if (size == 0) return;

    bool check_cancellation = stopToken.stop_possible();

#ifdef TL_HAVE_OPENMP

    if (check_cancellation) {

        if (stopToken.stop_requested()) return;

        bool thread_stopped = false;

#pragma omp parallel for firstprivate(thread_stopped) shared(stopToken, f)

        for (long long i = static_cast<long long>(ini); i < static_cast<long long>(end); i++) {

            if (!thread_stopped && (i & 127) == 0 && stopToken.stop_requested()) {
                thread_stopped = true;
            }

            if (!thread_stopped)
                f(static_cast<size_t>(i));
        }

    } else {

#pragma omp parallel for shared(f)
        for (long long i = static_cast<long long>(ini); i < static_cast<long long>(end); i++) {
            f(static_cast<size_t>(i));
        }
    }

#else

    auto func = std::forward<Function>(f);

    auto f_aux = [&](size_t ini, size_t end) {

        if (check_cancellation) {

            for (size_t r = ini; r < end; ++r) {

                if ((r & 127) == 0 && stopToken.stop_requested()) break;

                func(r);
            }

        } else {

            for (size_t r = ini; r < end; ++r) {
                func(r);
            }
        }
    };

    size_t min_per_thread = 25;
    size_t max_threads = (size + min_per_thread - 1) / min_per_thread;
    size_t hardware_threads = static_cast<size_t>(optimalNumberOfThreads());
    size_t num_threads = std::min(hardware_threads, max_threads);

    std::vector<std::jthread> threads;
    threads.reserve(num_threads);

    size_t block_size = size / num_threads;

    size_t block_ini = ini;
    size_t block_end = 0;

    for (size_t i = 0; i < num_threads; i++) {

        if (i == num_threads - 1) block_end = end;
        else block_end = block_ini + block_size;

        threads.emplace_back(f_aux, block_ini, block_end);

        block_ini = block_end;
    }

#endif

}


/*!
 * \brief Executes a reduction operation in parallel over a range of elements.
 *
 * This function provides a way to perform parallel reduction operations where each
 * thread processes a portion of the data and then combines results. This is useful
 * for operations like sum, product, or other aggregations.
 *
 * ### How It Works
 *
 * 1. The input range is divided into blocks, one per thread
 * 2. Each thread gets a copy of the function object and processes its block
 * 3. Results from all threads are combined at the end
 *
 * ### Requirements for Function Object
 *
 * The function object must:
 * - Have a call operator: `void operator()(const T&)`
 * - Have a `sum` member variable for combining results
 *
 * ### Example Usage
 *
 * \code{.cpp}
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
 * std::vector<int> nums{3, 4, 2, 8, 15, 267, 54, 60, 29, 20, 39};
 * Sum sum = parallel_reduce(nums.begin(), nums.end(), Sum());
 * \endcode
 *
 * \tparam Iter Iterator type for the range
 * \tparam Function Type of the function/reducer object
 * \param[in] first Iterator to the first element
 * \param[in] last Iterator to the past-the-end element
 * \param[in] f Function object implementing the reduction operation
 * \return The combined result from all threads
 *
 * \note Empty ranges return a copy of the input function object
 * \see parallel_for
 */
template<typename Iter, typename Function>
auto parallel_reduce(Iter first,
                     Iter last,
                     Function &&f,
                     std::stop_token stopToken = {}) -> std::decay_t<Function>
{
    size_t size = std::distance(first, last);
    if (size == 0) return f;

    size_t min_per_thread = 25;
    size_t max_threads = (size + min_per_thread - 1) / min_per_thread;
    size_t hardware_threads = static_cast<size_t>(optimalNumberOfThreads());
    size_t num_threads = std::min(hardware_threads, max_threads);

    std::vector<std::jthread> threads;
    threads.reserve(num_threads);

    std::vector<Function> thread_functors(num_threads, f);

    size_t block_size = size / num_threads;
    Iter block_ini = first;
    Iter block_end = block_ini;

    auto f_aux = [&](Iter ini, Iter end, Function &local_f) {
        while (ini != end && !stopToken.stop_requested()) {
            local_f(*ini++);
        }
    };

    for (size_t i = 0; i < num_threads; i++) {
        if (i == num_threads - 1) {
            block_end = last;
        } else {
            block_end = block_ini;
            std::advance(block_end, block_size);
        }

        threads.emplace_back(f_aux, block_ini, block_end, std::ref(thread_functors[i]));

        block_ini = block_end;
    }

    threads.clear(); // Ensure threads are joined before aggregating

    for (size_t i = 1; i < num_threads; i++) {
        thread_functors[0].sum += thread_functors[i].sum;
    }

    return thread_functors[0];
}

/// \cond

///Pruebas basadas en C++ Concurrency in Action
template<typename Iterator, typename Func>
void parallel_for_each(Iterator first,
                       Iterator last,
                       Func &&f,
                       std::stop_token stopToken = {})
{
    unsigned long const length = std::distance(first, last);
    if (!length) return;

    unsigned long const min_per_thread = 25;
    unsigned long const max_threads = (length + min_per_thread - 1) / min_per_thread;
    unsigned long const hardware_threads = std::thread::hardware_concurrency();
    unsigned long const num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
    unsigned long const block_size = length / num_threads;
    std::vector<std::future<void> > futures(num_threads - 1);
    std::vector<std::jthread> threads;
    threads.reserve(num_threads - 1);

    Iterator block_start = first;
    for (unsigned long i = 0; i < (num_threads - 1); ++i) {
        Iterator block_end = block_start;
        std::advance(block_end, block_size);
        auto task = std::packaged_task<void()>([=, &stopToken]() {
                for (Iterator it = block_start; it != block_end && !stopToken.stop_requested(); ++it) {
                    f(*it);
                }
            });
        futures[i] = task.get_future();
        threads.emplace_back(std::move(task));
        block_start = block_end;
    }

    std::for_each(block_start, last, [&](auto &val){ if (!stopToken.stop_requested()) f(val); });
    for (unsigned long i = 0; i < (num_threads - 1); ++i) {
        futures[i].get();
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
        std::future<void> first_half = std::async(&parallel_for_each<Iterator, Func>,
                                                  first, mid_point, f);
        parallel_for_each(mid_point, last, f);
        first_half.get();
    }
}


/// \endcond

/*! \} */


} // End namespace tl

