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
#include <tidop/core/concurrency.h>

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

//BOOST_FIXTURE_TEST_CASE(parallel_for_iterator_test, ConcurrencyTest)
//{
//  std::vector<int> aux(nums.size());
//  parallel_for(nums.begin(), nums.end(), aux.begin(), [&](size_t i) -> size_t {
//    return i + 1;
//  });
//
//  for (size_t i = 0; i < aux.size(); i++)
//    BOOST_CHECK_EQUAL(nums2[i], aux[i]);
//}

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
    Sum sum = parallel_for_each(nums.begin(), nums.end(), Sum());

    BOOST_CHECK_EQUAL(299, sum.sum);
}

//BOOST_FIXTURE_TEST_CASE(parallel_for_each_2_test, ConcurrencyTest)
//{
//  std::vector<int> aux;
//  parallel_for_each_2(nums.begin(), nums.end(), [&](size_t i) {
//    aux.push_back(i + 1);
//  });
//
//  for(size_t i = 0; i < aux.size(); i++)
//    BOOST_CHECK_EQUAL(nums2[i], aux[i]);
//}
//
//BOOST_FIXTURE_TEST_CASE(parallel_for_each_3_test, ConcurrencyTest)
//{
//  std::vector<int> aux;
//  parallel_for_each_3(nums.begin(), nums.end(), [&](size_t i) {
//    aux.push_back(i + 1);
//  });
//
//  for(size_t i = 0; i < aux.size(); i++)
//    BOOST_CHECK_EQUAL(nums2[i], aux[i]);
//}

BOOST_AUTO_TEST_SUITE_END()
