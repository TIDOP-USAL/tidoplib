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

#define BOOST_TEST_MODULE Tidop moments test
#include <boost/test/unit_test.hpp>

#include <tidop/math/statistic/algorithms/descriptive/Moments.h>
#include <tidop/math/statistic/base/CentralMoments.h>

#include <array>
#include <list>
#include <span>

using namespace tl;


BOOST_AUTO_TEST_SUITE(CentralMomentsTestSuite)

struct CentralMomentsFixture
{

    std::vector<double> vd{8.0, 8.5, 7.5, 9.0, 6.25, 5.5, 8.5, 7.5, 8.5};
    std::vector<int> vi{1, 0, 1, 3, 2, 0, 1};
    std::vector<int> vi2{17, 15, 23, 7, 9, 13};

    struct Expected
    {
        double mean = 7.694444444444445;
        double variance_sample = 1.3402777777777777;
        double variance_pop = 1.191358024691358;
        double skewness = -0.08118654178361874;   // asimetría negativa leve
        double kurtosis = 2.271329798742822;      // platicúrtica
        double kurtosis_excess = -0.728670201257178;
    } expected_double;

    struct ExpectedInt
    {
        double mean = 1.1428571428571428;
        double variance_sample = 1.1428571428571428;
        double variance_pop = 0.9795918367346939;
        double skewness = 0.854399767375984;      // asimetría positiva
        double kurtosis = 2.3125;
        double kurtosis_excess = -0.6875;
    } expected_int;
};

BOOST_FIXTURE_TEST_CASE(construction_and_basic_ops, CentralMomentsFixture)
{
    // Default constructor
    CentralMoments<double> moments;
    BOOST_CHECK_EQUAL(moments.count(), 0);
    BOOST_CHECK(moments.empty());
    BOOST_CHECK_EQUAL(moments.mean(), 0.0);
    BOOST_CHECK_EQUAL(moments.secondMoment(), 0.0);
    BOOST_CHECK_EQUAL(moments.thirdMoment(), 0.0);
    BOOST_CHECK_EQUAL(moments.fourthMoment(), 0.0);

    // Add single value
    moments.add(5.0);
    BOOST_CHECK_EQUAL(moments.count(), 1);
    BOOST_CHECK(!moments.empty());
    BOOST_CHECK_EQUAL(moments.mean(), 5.0);
    BOOST_CHECK_EQUAL(moments.secondMoment(), 0.0);
}

BOOST_FIXTURE_TEST_CASE(double_precision_accumulation, CentralMomentsFixture)
{
    CentralMoments<double> moments;

    // Add all values one by one
    for (auto x : vd) {
        moments.add(x);
    }

    BOOST_CHECK_EQUAL(moments.count(), vd.size());
    BOOST_CHECK_CLOSE(moments.mean(), expected_double.mean, 1e-10);

    // Verify moments directly
    BOOST_CHECK_CLOSE(moments.secondMoment(), 10.722222222222221, 1e-10);
    BOOST_CHECK_CLOSE(moments.thirdMoment(), -9.7739197530864192, 1e-10);
    BOOST_CHECK_CLOSE(moments.fourthMoment(), 31.723122427983533, 1e-10);
}

BOOST_FIXTURE_TEST_CASE(integer_accumulation, CentralMomentsFixture)
{
    // Para enteros, CentralMoments debe usar double internamente
    CentralMoments<double> moments;

    for (auto x : vi) {
        moments.add(static_cast<double>(x));
    }

    BOOST_CHECK_EQUAL(moments.count(), vi.size());
    BOOST_CHECK_CLOSE(moments.mean(), expected_int.mean, 1e-10);
}

BOOST_FIXTURE_TEST_CASE(merge_operation, CentralMomentsFixture)
{
    // Dividir datos en dos mitades
    size_t mid = vd.size() / 2;

    CentralMoments<double> left;
    for (size_t i = 0; i < mid; ++i) {
        left.add(vd[i]);
    }

    CentralMoments<double> right;
    for (size_t i = mid; i < vd.size(); ++i) {
        right.add(vd[i]);
    }

    // Merge
    left.merge(right);

    // Comparar con acumulación directa
    CentralMoments<double> full;
    for (auto x : vd) {
        full.add(x);
    }

    BOOST_CHECK_EQUAL(left.count(), full.count());
    BOOST_CHECK_CLOSE(left.mean(), full.mean(), 1e-10);
}

BOOST_FIXTURE_TEST_CASE(constant_values, CentralMomentsFixture)
{
    CentralMoments<double> _moments;
    for (int i = 0; i < 10; ++i) {
        _moments.add(5.0);
    }

    BOOST_CHECK_CLOSE(_moments.mean(), 5.0, 1e-10);
}

BOOST_FIXTURE_TEST_CASE(numerical_stability, CentralMomentsFixture)
{
    // Probar con valores muy dispares (desbordamiento potencial)
    CentralMoments<double> _moments;
    _moments.add(1e200);
    _moments.add(1e-200);
    _moments.add(1e200);

    // No debe haber excepciones ni NaN
    BOOST_CHECK(std::isfinite(_moments.mean()));
}

BOOST_AUTO_TEST_CASE(span_range)
{
    std::array<double, 5> data = {1,2,3,4,5};
    std::span<double> s(data);

    auto m = moments(s);

    BOOST_CHECK_CLOSE(3.0, m.mean(), 0.001);
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(MomentsFunctionTestSuite)

struct MomentsTestFixture
{
    std::vector<double> vd{8.0, 8.5, 7.5, 9.0, 6.25, 5.5, 8.5, 7.5, 8.5};
    std::vector<int> vi{1, 0, 1, 3, 2, 0, 1};
    std::array<double, 9> arr{8.0, 8.5, 7.5, 9.0, 6.25, 5.5, 8.5, 7.5, 8.5};
    std::list<double> lst{8.0, 8.5, 7.5, 9.0, 6.25, 5.5, 8.5, 7.5, 8.5};

    // No contiguo - no debería usar SIMD
    struct NonContiguous
    {
        double data[9] = {8.0, 8.5, 7.5, 9.0, 6.25, 5.5, 8.5, 7.5, 8.5};

        auto begin() const { return data; }
        auto end() const { return data + 9; }
    } non_contiguous;
};

BOOST_FIXTURE_TEST_CASE(contiguous_vector_double, MomentsTestFixture)
{
    auto _moments = tl::moments(vd);

    BOOST_CHECK_EQUAL(_moments.count(), 9);
    BOOST_CHECK_CLOSE(_moments.mean(), 7.694444444444445, 1e-10);
}

BOOST_FIXTURE_TEST_CASE(contiguous_vector_int, MomentsTestFixture)
{
    auto _moments = tl::moments(vi);

    BOOST_CHECK_EQUAL(_moments.count(), 7);
    // Debe usar double internamente
    BOOST_CHECK_CLOSE(_moments.mean(), 1.1428571428571428, 1e-10);
}

BOOST_FIXTURE_TEST_CASE(contiguous_array, MomentsTestFixture)
{
    auto _moments = tl::moments(arr);

    BOOST_CHECK_EQUAL(_moments.count(), 9);
    BOOST_CHECK_CLOSE(_moments.mean(), 7.694444444444445, 1e-10);
    BOOST_CHECK_CLOSE(_moments.secondMoment(), 10.722222222222221, 1e-10);
}

BOOST_FIXTURE_TEST_CASE(non_contiguous_list, MomentsTestFixture)
{
    // list no es contiguous_range -> debe usar versión scalar
    auto _moments = tl::moments(lst);

    BOOST_CHECK_EQUAL(_moments.count(), 9);
    BOOST_CHECK_CLOSE(_moments.mean(), 7.694444444444445, 1e-10);
}

BOOST_FIXTURE_TEST_CASE(comparison_scalar_vs_simd, MomentsTestFixture)
{
    // Para rangos contiguos, ambas versiones deben dar el mismo resultado
    auto moments_simd = tl::moments(vd);           // Usa SIMD si TL_HAVE_SIMD_INTRINSICS
    auto moments_scalar = tl::detail::momentsScalar(vd);  // Forzar scalar

    BOOST_CHECK_CLOSE(moments_simd.mean(), moments_scalar.mean(), 1e-10);
    BOOST_CHECK_CLOSE(moments_simd.secondMoment(), moments_scalar.secondMoment(), 1e-10);
    BOOST_CHECK_CLOSE(moments_simd.thirdMoment(), moments_scalar.thirdMoment(), 1e-10);
    BOOST_CHECK_CLOSE(moments_simd.fourthMoment(), moments_scalar.fourthMoment(), 1e-10);
}

BOOST_FIXTURE_TEST_CASE(empty_range, MomentsTestFixture)
{
    std::vector<double> empty;
    auto _moments = tl::moments(empty);

    BOOST_CHECK_EQUAL(_moments.count(), 0);
    BOOST_CHECK(_moments.empty());
    // Las estadísticas deben ser NaN o 0 según tu diseño
    BOOST_CHECK_EQUAL(_moments.mean(), 0.0);
}

BOOST_FIXTURE_TEST_CASE(single_element_range, MomentsTestFixture)
{
    std::vector<double> single{42.0};
    auto _moments = tl::moments(single);

    BOOST_CHECK_EQUAL(_moments.count(), 1);
    BOOST_CHECK_CLOSE(_moments.mean(), 42.0, 1e-10);
}

#ifdef TL_HAVE_SIMD_INTRINSICS
BOOST_FIXTURE_TEST_CASE(simd_alignment, MomentsTestFixture)
{
    for (size_t n = 1; n <= 20; ++n) {
        std::vector<double> data(n, 1.0);
        for (size_t i = 0; i < n; ++i) {
            data[i] = static_cast<double>(i);
        }

        auto moments_simd = tl::moments(data);
        auto moments_scalar = tl::detail::momentsScalar(data);

        BOOST_CHECK_CLOSE(moments_simd.mean(), moments_scalar.mean(), 1e-10);
    }
}
#endif

BOOST_AUTO_TEST_SUITE_END()