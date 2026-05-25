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
 
#define BOOST_TEST_MODULE Tidop line fit test

#include <boost/test/unit_test.hpp>
#include <tidop/geometry/algorithms/fitting/LineFitter.h>
#include <tidop/geometry/algorithms/measurement/Distance.h>

using namespace tl;

// TEST para fitLine3D (PCA)

BOOST_AUTO_TEST_SUITE(fitLine3DPCA)

// Test 1: Puntos perfectamente colineales (recta y = x, z = x)
BOOST_AUTO_TEST_CASE(fitLine3D_perfect_line)
{
    std::vector<Point3<double>> points = {
        Point3<double>(0.0, 0.0, 0.0),
        Point3<double>(2.0, 2.0, 2.0),
        Point3<double>(4.0, 4.0, 4.0),
        Point3<double>(6.0, 6.0, 6.0),
        Point3<double>(8.0, 8.0, 8.0)
    };

    auto opt_line = fitLine3D(points, 1e-8);
    BOOST_REQUIRE(opt_line.has_value());

    auto line = opt_line.value();
    auto origin = line.getOrigin();
    auto direction = line.getDirection();

    // El origen debe ser el centroide (4,4,4)
    BOOST_CHECK_CLOSE(origin.x(), 4.0, 1e-9);
    BOOST_CHECK_CLOSE(origin.y(), 4.0, 1e-9);
    BOOST_CHECK_CLOSE(origin.z(), 4.0, 1e-9);

    // La dirección debe ser (1,1,1) normalizada => (0.57735, 0.57735, 0.57735)
    double expected_dir = 1.0 / std::sqrt(3.0);
    BOOST_CHECK_CLOSE(direction.x(), expected_dir, 1e-9);
    BOOST_CHECK_CLOSE(direction.y(), expected_dir, 1e-9);
    BOOST_CHECK_CLOSE(direction.z(), expected_dir, 1e-9);

    // Verificar que todos los puntos estén a distancia cero (dentro de epsilon)
    for (const auto &p : points) {
        double dist = line.distance(p);
        BOOST_CHECK_SMALL(dist, 1e-8);
    }
}

// Test 2: Exactamente dos puntos
BOOST_AUTO_TEST_CASE(fitLine3D_two_points)
{
    std::vector<Point3<double>> points = {
        Point3<double>(1.0, 2.0, 3.0),
        Point3<double>(4.0, 5.0, 6.0)
    };

    auto opt_line = fitLine3D(points);
    BOOST_REQUIRE(opt_line.has_value());

    auto line = opt_line.value();
    auto origin = line.getOrigin();
    auto direction = line.getDirection();

    // Centroide = (2.5, 3.5, 4.5)
    BOOST_CHECK_CLOSE(origin.x(), 2.5, 1e-9);
    BOOST_CHECK_CLOSE(origin.y(), 3.5, 1e-9);
    BOOST_CHECK_CLOSE(origin.z(), 4.5, 1e-9);

    // Dirección = (3,3,3) normalizada = (1,1,1)/√3
    double expected_dir = 1.0 / std::sqrt(3.0);
    BOOST_CHECK_CLOSE(direction.x(), expected_dir, 1e-9);
    BOOST_CHECK_CLOSE(direction.y(), expected_dir, 1e-9);
    BOOST_CHECK_CLOSE(direction.z(), expected_dir, 1e-9);

    // Distancias a la línea deben ser cero
    for (const auto &p : points) {
        BOOST_CHECK_SMALL(line.distance(p), 1e-8);
    }
}

// Test 3: Un solo punto -> debe fallar
BOOST_AUTO_TEST_CASE(fitLine3D_single_point)
{
    std::vector<Point3<double>> points = {Point3<double>(1.0, 2.0, 3.0)};
    auto opt_line = fitLine3D(points);
    BOOST_CHECK(!opt_line.has_value());
}

// Test 4: Todos los puntos coincidentes (degenerado)
BOOST_AUTO_TEST_CASE(fitLine3D_degenerate)
{
    std::vector<Point3<double>> points = {
        Point3<double>(5.0, 5.0, 5.0),
        Point3<double>(5.0, 5.0, 5.0),
        Point3<double>(5.0, 5.0, 5.0)
    };
    auto opt_line = fitLine3D(points, 1e-8);
    // La varianza es cero => mayor autovalor = 0 < tolerance -> nullopt
    BOOST_CHECK(!opt_line.has_value());
}

// Test 5: Puntos con ruido pequeño alrededor de una línea
BOOST_AUTO_TEST_CASE(fitLine3D_noisy)
{
    // Línea verdadera: eje X (y=0, z=0)
    std::vector<Point3<double>> points;
    for (int i = -5; i <= 5; ++i) {
        double x = static_cast<double>(i);
        double y = 0.0 + (i % 2) * 0.01;   // ruido pequeño alternante
        double z = 0.0 - (i % 2) * 0.01;
        points.push_back(Point3<double>(x, y, z));
    }

    auto opt_line = fitLine3D(points, 1e-8);
    BOOST_REQUIRE(opt_line.has_value());

    auto line = opt_line.value();
    auto direction = line.getDirection();

    // La dirección estimada debe ser aproximadamente (1,0,0)
    BOOST_CHECK_CLOSE(direction.x(), 1.0, 1e-2);   // tolerancia 1%
    BOOST_CHECK_SMALL(direction.y(), 1e-2);
    BOOST_CHECK_SMALL(direction.z(), 1e-2);

    // El origen debe ser cercano a (0,0,0) (centroide)
    auto origin = line.getOrigin();
    BOOST_CHECK_SMALL(origin.x(), 1e-1);
    BOOST_CHECK_SMALL(origin.y(), 1e-1);
    BOOST_CHECK_SMALL(origin.z(), 1e-1);
}

// Test 6: Verificar que la función rechaza puntos casi coincidentes
//        (extensión muy pequeña, por debajo de la tolerancia)
BOOST_AUTO_TEST_CASE(fitLine3D_small_extent)
{
    std::vector<Point3<double>> points = {
        Point3<double>(0.0, 0.0, 0.0),
        Point3<double>(1e-10, 1e-10, 1e-10),
        Point3<double>(2e-10, 2e-10, 2e-10)
    };
    // Tolerancia 1e-8 > mayor autovalor (≈ 6e-20) -> nullopt
    auto opt_line = fitLine3D(points, 1e-8);
    BOOST_CHECK(!opt_line.has_value());

    // Reduciendo la tolerancia debería funcionar
    auto opt_line2 = fitLine3D(points, 1e-20);
    BOOST_CHECK(opt_line2.has_value());
}

BOOST_AUTO_TEST_SUITE_END()


// TEST para fitLine2D (PCA)

BOOST_AUTO_TEST_SUITE(fitLine2D_PCA)

// Caso 1: Línea perfecta y = 2x + 1
BOOST_AUTO_TEST_CASE(perfect_line)
{
    std::vector<Point2<double>> points;
    for (int x = -5; x <= 5; ++x) {
        double y = 2.0 * x + 1.0;
        points.push_back(Point2<double>(static_cast<double>(x), y));
    }

    auto opt_line = fitLine2D(points, 1e-8);
    BOOST_REQUIRE(opt_line.has_value());

    auto &line = opt_line.value();
    auto dir = line.getDirection();
    auto origin = line.getOrigin();

    // La dirección debe ser paralela a (1, 2) normalizado: (1/√5, 2/√5)
    double expected_dir_x = 1.0 / std::sqrt(5.0);
    double expected_dir_y = 2.0 / std::sqrt(5.0);
    BOOST_CHECK_CLOSE(dir.x(), expected_dir_x, 1e-8);
    BOOST_CHECK_CLOSE(dir.y(), expected_dir_y, 1e-8);

    // El origen (centroide) debe ser (0,1) porque x medio es 0
    BOOST_CHECK_SMALL(origin.x(), 1e-8);
    BOOST_CHECK_CLOSE(origin.y(), 1.0, 1e-8);

    // Distancia de todos los puntos a la línea debe ser cero
    for (const auto &p : points) {
        BOOST_CHECK_SMALL(line.distance(p), 1e-8);
    }
}

// Caso 2: Dos puntos
BOOST_AUTO_TEST_CASE(two_points)
{
    std::vector<Point2<double>> points = {
        Point2<double>(0.0, 0.0),
        Point2<double>(3.0, 4.0)
    };

    auto opt_line = fitLine2D(points);
    BOOST_REQUIRE(opt_line.has_value());

    auto &line = opt_line.value();
    auto dir = line.getDirection();
    auto origin = line.getOrigin();

    // Centroide = (1.5, 2.0)
    BOOST_CHECK_CLOSE(origin.x(), 1.5, 1e-8);
    BOOST_CHECK_CLOSE(origin.y(), 2.0, 1e-8);

    // Dirección = vector entre puntos normalizado: (3,4)/5 = (0.6, 0.8)
    BOOST_CHECK_CLOSE(dir.x(), 0.6, 1e-8);
    BOOST_CHECK_CLOSE(dir.y(), 0.8, 1e-8);

    // Distancias a la línea cero
    BOOST_CHECK_SMALL(line.distance(points[0]), 1e-8);
    BOOST_CHECK_SMALL(line.distance(points[1]), 1e-8);
}

// Caso 3: Un solo punto -> fallo
BOOST_AUTO_TEST_CASE(single_point)
{
    std::vector<Point2<double>> points = {Point2<double>(1.0, 1.0)};
    auto opt_line = fitLine2D(points);
    BOOST_CHECK(!opt_line.has_value());
}

// Caso 4: Puntos degenerados (todos iguales)
BOOST_AUTO_TEST_CASE(degenerate)
{
    std::vector<Point2<double>> points = {
        Point2<double>(2.0, 2.0),
        Point2<double>(2.0, 2.0),
        Point2<double>(2.0, 2.0)
    };
    auto opt_line = fitLine2D(points, 1e-8);
    BOOST_CHECK(!opt_line.has_value()); // autovalor mayor = 0 < tolerance
}

// Caso 5: Línea vertical x = 3
BOOST_AUTO_TEST_CASE(vertical_line)
{
    std::vector<Point2<double>> points;
    for (int y = -5; y <= 5; ++y) {
        points.push_back(Point2<double>(3.0, static_cast<double>(y)));
    }

    auto opt_line = fitLine2D(points, 1e-8);
    BOOST_REQUIRE(opt_line.has_value());

    auto line = opt_line.value();
    auto dir = line.getDirection();

    // La dirección debe ser (0,1) o (0,-1) (vertical)
    BOOST_CHECK_SMALL(dir.x(), 1e-8);
    BOOST_CHECK_CLOSE(std::abs(dir.y()), 1.0, 1e-8);

    // El origen debe tener x = 3
    BOOST_CHECK_CLOSE(line.getOrigin().x(), 3.0, 1e-8);
}

// Caso 6: Puntos con ruido pequeño alrededor de y = x
BOOST_AUTO_TEST_CASE(noisy_line)
{
    std::vector<Point2<double>> points;
    std::mt19937 rng(42);
    std::normal_distribution<double> noise(0.0, 0.02); // ruido σ=0.02

    for (int i = -10; i <= 10; ++i) {
        double x = static_cast<double>(i);
        double y = x + noise(rng);
        points.push_back(Point2<double>(x, y));
    }

    auto opt_line = fitLine2D(points, 1e-8);
    BOOST_REQUIRE(opt_line.has_value());

    auto dir = opt_line.value().getDirection();
    // Dirección esperada (1,1) normalizada => (0.7071, 0.7071)
    double expected = 1.0 / std::sqrt(2.0);
    BOOST_CHECK_CLOSE(dir.x(), expected, 5e-2);  // tolerancia 5%
    BOOST_CHECK_CLOSE(dir.y(), expected, 5e-2);
}

BOOST_AUTO_TEST_SUITE_END()



// TEST para fitLine2DSVD

BOOST_AUTO_TEST_SUITE(fitLine2D_SVD)

// Caso 1: Línea perfecta y = -x
BOOST_AUTO_TEST_CASE(perfect_line)
{
    std::vector<Point2<double>> points;
    for (int x = -5; x <= 5; ++x) {
        points.push_back(Point2<double>(static_cast<double>(x), -static_cast<double>(x)));
    }

    auto opt_line = fitLine2DSVD(points, 1e-8);
    BOOST_REQUIRE(opt_line.has_value());

    auto &line = opt_line.value();
    auto dir = line.getDirection();
    // Dirección (1, -1) normalizada = (0.7071, -0.7071)
    double expected = 1.0 / std::sqrt(2.0);
    BOOST_CHECK_CLOSE(std::abs(dir.x()), expected, 1e-8);
    BOOST_CHECK_CLOSE(std::abs(dir.y()), expected, 1e-8);
    // El producto punto con (1,-1) debe ser ≈ ±1
    double dot = dir.x() * expected - dir.y() * expected; // cuidado con signo
    BOOST_CHECK_CLOSE(std::abs(dot), 1.0, 1e-8);
}

// Caso 2: Dos puntos
BOOST_AUTO_TEST_CASE(two_points)
{
    std::vector<Point2<double>> points = {
        Point2<double>(1.0, 2.0),
        Point2<double>(4.0, 6.0)
    };
    auto opt_line = fitLine2DSVD(points);
    BOOST_REQUIRE(opt_line.has_value());

    auto &line = opt_line.value();
    auto dir = line.getDirection();
    // Vector (3,4) normalizado
    BOOST_CHECK_CLOSE(dir.x(), 0.6, 1e-8);
    BOOST_CHECK_CLOSE(dir.y(), 0.8, 1e-8);
}

// Caso 3: Un punto -> fallo
BOOST_AUTO_TEST_CASE(single_point)
{
    std::vector<Point2<double>> points = {Point2<double>(5.0, 5.0)};
    auto opt_line = fitLine2DSVD(points);
    BOOST_CHECK(!opt_line.has_value());
}

// Caso 4: Puntos coincidentes
BOOST_AUTO_TEST_CASE(degenerate)
{
    std::vector<Point2<double>> points = {
        Point2<double>(0.0, 0.0),
        Point2<double>(0.0, 0.0)
    };
    auto opt_line = fitLine2DSVD(points, 1e-8);
    BOOST_CHECK(!opt_line.has_value());
}

// Caso 5: Línea vertical (x = -2)
BOOST_AUTO_TEST_CASE(vertical_line)
{
    std::vector<Point2<double>> points;
    for (int y = -5; y <= 5; ++y) {
        points.push_back(Point2<double>(-2.0, static_cast<double>(y)));
    }
    auto opt_line = fitLine2DSVD(points);
    BOOST_REQUIRE(opt_line.has_value());

    auto dir = opt_line.value().getDirection();
    BOOST_CHECK_SMALL(dir.x(), 1e-8);
    BOOST_CHECK_CLOSE(std::abs(dir.y()), 1.0, 1e-8);
}

// Caso 6: Ruido moderado
BOOST_AUTO_TEST_CASE(noisy_line)
{
    std::vector<Point2<double>> points;
    std::mt19937 rng(123);
    std::normal_distribution<double> noise(0.0, 0.02);

    for (int i = -10; i <= 10; ++i) {
        double x = static_cast<double>(i);
        double y = 2.0 * x + 0.5 + noise(rng);
        points.push_back(Point2<double>(x, y));
    }

    auto opt_line = fitLine2DSVD(points, 1e-8);
    BOOST_REQUIRE(opt_line.has_value());

    auto dir = opt_line.value().getDirection();
    // Dirección esperada (1,2) normalizada = (0.4472, 0.8944)
    double expected_x = 1.0 / std::sqrt(5.0);
    double expected_y = 2.0 / std::sqrt(5.0);
    BOOST_CHECK_CLOSE(dir.x(), expected_x, 5e-2);
    BOOST_CHECK_CLOSE(dir.y(), expected_y, 5e-2);
}

BOOST_AUTO_TEST_SUITE_END()

// -------------------------------------------------------------------
// TEST comparativo: PCA vs SVD deben dar resultados casi idénticos
// -------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(compare_pca_and_svd)
{
    std::vector<Point2<double>> points;
    // Generar puntos en una línea con ruido
    std::mt19937 rng(999);
    std::normal_distribution<double> noise(0.0, 0.01);
    for (int i = 0; i < 100; ++i) {
        double x = static_cast<double>(i) / 10.0;
        double y = 1.5 * x + 2.0 + noise(rng);
        points.push_back(Point2<double>(x, y));
    }

    auto line_pca = fitLine2D(points);
    auto line_svd = fitLine2DSVD(points);
    BOOST_REQUIRE(line_pca.has_value());
    BOOST_REQUIRE(line_svd.has_value());

    // Las direcciones deben ser iguales (salvo signo)
    auto dir_pca = line_pca->getDirection();
    auto dir_svd = line_svd->getDirection();
    // Asegurar mismo sentido (producto punto positivo)
    if (dir_pca.dotProduct(dir_svd) < 0) dir_svd = -dir_svd;
    double dot = dir_pca.dotProduct(dir_svd);
    BOOST_CHECK_CLOSE(dot, 1.0, 1e-6);

    // El origen (centroide) debe ser el mismo
    auto ori_pca = line_pca->getOrigin();
    auto ori_svd = line_svd->getOrigin();
    BOOST_CHECK_SMALL(distance(ori_pca, ori_svd), 1e-6);
}