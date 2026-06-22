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
 
#define BOOST_TEST_MODULE Tidop Concurrency test
#include <boost/test/unit_test.hpp>
#include <tidop/core/concurrency/Parallel.h>
#include <tidop/core/concurrency/QueueMPMC.h>
#include <tidop/core/concurrency/QueueSPSC.h>
#include <tidop/core/concurrency/Producer.h>
#include <tidop/core/concurrency/Consumer.h>

#include <thread>

using namespace tl;


BOOST_AUTO_TEST_SUITE(ConcurrencyTestSuite)


struct ConcurrencyTest
{
    ConcurrencyTest()
    {
    }

    ~ConcurrencyTest()
    {
    }

    void setup()
    {
        nums = {3, 4, 2, 8, 15, 267};
        nums2 = {4, 5, 3, 9, 16, 268};
    }

    void teardown()
    {

    }

    std::vector<int> nums;
    std::vector<int> nums2;

};


BOOST_FIXTURE_TEST_CASE(parallel_for_test, ConcurrencyTest)
{
    std::vector<int> aux(nums.size());

    parallel_for(0, nums.size(), [&](size_t i) {
        aux[i] = nums[i] + 1;
        });

    for (size_t i = 0; i < aux.size(); i++)
        BOOST_CHECK_EQUAL(nums2[i], aux[i]);
}

BOOST_FIXTURE_TEST_CASE(parallel_for_cancellation_test, ConcurrencyTest)
{
    // 1. Creamos un rango grande y una función que simule trabajo pesado
    // para dar tiempo a que ocurra la cancelación.
    size_t total_iterations = 10000;
    std::atomic<size_t> completed_iterations{0};

    std::stop_source stopSource;
    std::stop_token stopToken = stopSource.get_token();

    // 2. Lanzamos parallel_for en un hilo separado porque es una llamada bloqueante
    std::jthread worker_thread([&]() {
        parallel_for(0, total_iterations, [&](size_t i) {
            // Simulamos una carga de trabajo ligera pero notable
            std::this_thread::sleep_for(std::chrono::microseconds(100));

            completed_iterations.fetch_add(1, std::memory_order_relaxed);
            }, stopToken);
        });

    // 3. Dejamos que los hilos arranquen y procesen algunas iteraciones
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    // 4. Solicitamos la cancelación
    stopSource.request_stop();

    // 5. Esperamos a que el hilo del parallel_for termine (jthread hace join automáticamente aquí)
    worker_thread.join();

    // 6. Verificaciones (Asserts)
    // - Al menos algunas iteraciones debieron ejecutarse antes de cancelar.
    BOOST_CHECK_GT(completed_iterations.load(), 0);

    // - El bucle NO debió completarse por completo debido a la cancelación.
    BOOST_CHECK_LT(completed_iterations.load(), total_iterations);
}

struct Sum
{
    void operator()(int n)
    {
        sum += n;
    }
    int sum{0};
};

BOOST_FIXTURE_TEST_CASE(parallel_for_each_test, ConcurrencyTest)
{
    Sum sum = parallel_reduce(nums.begin(), nums.end(), Sum());

    BOOST_CHECK_EQUAL(299, sum.sum);
}



std::atomic<bool> done(false);
std::atomic_int producer_count(0);
std::atomic_int consumer_count(0);


template<typename T>
class ProducerTest
  : public Producer<int>
{

public:

    ProducerTest(Queue<T> *queue) : Producer<int>(queue){}
    ~ProducerTest() {}

    void operator() ()
    {
        queue()->push(++producer_count);
    }

    void operator() (size_t ini, size_t end)
    {
        for (size_t i = ini; i < end; ++i) {
            queue()->push(i);
            ++producer_count;
        }
    }
};

template<typename T>
class ConsumerTest
    : public Consumer<int>
{

public:

    ConsumerTest(Queue<T> *queue) : Consumer<int>(queue) {}
    ~ConsumerTest() {}

    void operator() ()
    {
        while (!done) {
            while (!queue()->empty()) {
                T value;
                if(queue()->pop(value))
                    ++consumer_count;
            }
        }

        while (!queue()->empty()) {
            T value;
            if (queue()->pop(value))
                ++consumer_count;
        }
    }
};

BOOST_FIXTURE_TEST_CASE(producer_consumer_mpmc_test, ConcurrencyTest)
{
    QueueMPMC<int> queue(10);
    ProducerTest<int> producer(&queue);
    ConsumerTest<int> consumer(&queue);

    size_t num_threads = tl::optimalNumberOfThreads();
    std::vector<std::thread> producer_threads(num_threads);
    std::vector<std::thread> consumer_threads(num_threads);

    size_t size = 10000 / num_threads;
    for (size_t i = 0; i < num_threads; ++i) {
        size_t _ini = i * size;
        size_t _end = _ini + size;
        if (i == num_threads - 1) 
            _end = 10000;

        producer_threads[i] = std::thread(producer, _ini, _end);
    }

    for (auto &consumer_thread : consumer_threads) {
        consumer_thread = std::thread(consumer);
    }

    for (auto &producer_thread : producer_threads) {
        if (producer_thread.joinable()) {
            producer_thread.join();
        }
    }

    done = true;
    queue.stop();


    for (auto &consumer_thread : consumer_threads) {
        if (consumer_thread.joinable()) {
            consumer_thread.join();
        }
    }

    BOOST_CHECK_EQUAL(producer_count, consumer_count);
}

BOOST_FIXTURE_TEST_CASE(producer_consumer_spsc_test, ConcurrencyTest)
{
    QueueSPSC<int> queue(10);
    ProducerTest<int> producer(&queue);
    ConsumerTest<int> consumer(&queue);

    done = false;

    std::thread consumer_thread(consumer);

    std::thread producer_thread([&]() {
        for (int i = 0; i < 10000; ++i) {
            producer();
        }
    });

    producer_thread.join();

    done = true;

    consumer_thread.join();

    BOOST_CHECK_EQUAL(producer_count, consumer_count);
}


BOOST_AUTO_TEST_SUITE_END()
