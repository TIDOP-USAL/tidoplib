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
 
#define BOOST_TEST_MODULE Tidop affine test

#include <boost/test/unit_test.hpp>
#include <tidop/math/algebra/affine.h>
#include <tidop/math/algebra/helmert.h>
#include <tidop/math/algebra/matrix.h>


using namespace tl;

BOOST_AUTO_TEST_SUITE(AffineTestSuite)

struct AffineTest
{

    AffineTest() {}

    ~AffineTest() {}

    void setup()
    {
        src_mat = {{4157222.543, 664789.307},
                   {4149043.336, 688836.443},
                   {4172803.511, 690340.078},
                   {4177148.376, 642997.635},
                   {4137012.190, 671808.029},
                   {4146292.729, 666952.887},
                   {4138759.902, 702670.738}};

        dst_mat = {{737107.092, 759565.279},
                   {731294.227, 764301.907},
                   {735901.291, 768078.488},
                   {744937.420, 757067.318},
                   {731760.522, 758392.053},
                   {734496.503, 758529.698},
                   {726807.795, 766227.040}};

        src_points = {Point<double>(4157222.543, 664789.307),
                       Point<double>(4149043.336, 688836.443),
                       Point<double>(4172803.511, 690340.078),
                       Point<double>(4177148.376, 642997.635),
                       Point<double>(4137012.190, 671808.029),
                       Point<double>(4146292.729, 666952.887),
                       Point<double>(4138759.902, 702670.738)};

        dst_points = {Point<double>(737107.092, 759565.279),
                       Point<double>(731294.227, 764301.907),
                       Point<double>(735901.291, 768078.488),
                       Point<double>(744937.420, 757067.318),
                       Point<double>(731760.522, 758392.053),
                       Point<double>(734496.503, 758529.698),
                       Point<double>(726807.795, 766227.040)};

        dst_points_helmert = {
          Point<double>(756172.466,	732337.103),
          Point<double>(751049.245,	736088.818),
          Point<double>(755699.431,	739803.813),
          Point<double>(763377.835,	730731.677),
          Point<double>(751027.184,  730876.407),
          Point<double>(753623.926,	731212.907),
          Point<double>(746959.564,	737447.332)};
        
        src_3d_mat = {{2441775.419, 799268.100, 5818729.162},
                      {3464655.838, 845749.989, 5270271.528},
                      {3309991.828, 828932.118, 5370882.280},
                      {3160763.338, 759160.187, 5469345.504},
                      {2248123.493, 865686.595, 5886425.596},
                      {3022573.157, 802945.690, 5540683.951},
                      {3104219.427, 998384.028, 5463290.505},
                      {2998189.685, 931451.634, 5533398.462},
                      {3199093.294, 932231.327, 5420322.483},
                      {3370658.823, 711876.990, 5349786.786},
                      {3341340.173, 957912.343, 5330003.236},
                      {2534031.166, 975174.455, 5752078.309},
                      {2838909.903, 903822.098, 5620660.184},
                      {2902495.079, 761455.843, 5609859.672},
                      {2682407.890, 950395.934, 5688993.082},
                      {2620258.868, 779138.041, 5743799.267},
                      {3246470.535, 1077900.355, 5365277.896},
                      {3249408.275, 692757.965, 5426396.948},
                      {2763885.496, 733247.387, 5682653.347},
                      {2368885.005, 994492.233, 5818478.154}};

        dst_3d_mat = {{2441276.712, 799286.666, 5818162.025},
                      {3464161.275, 845805.461, 5269712.429},
                      {3309496.800, 828981.942, 5370322.060},
                      {3160269.913, 759204.574, 5468784.081},
                      {2247621.426, 865698.413, 5885856.498},
                      {3022077.340, 802985.055, 5540121.276},
                      {3103716.966, 998426.412, 5462727.814},
                      {2997689.029, 931490.201, 5532835.154},
                      {3198593.776, 932277.179, 5419760.966},
                      {3370168.626, 711928.884, 5349227.574},
                      {3340840.578, 957963.383, 5329442.724},
                      {2533526.497, 975196.347, 5751510.935},
                      {2838409.359, 903854.897, 5620095.593},
                      {2902000.172, 761490.908, 5609296.343},
                      {2681904.794, 950423.098, 5688426.909},
                      {2619761.810, 779162.964, 5743233.630},
                      {3245966.134, 1077947.976, 5364716.214},
                      {3248918.041, 692805.543, 5425836.841},
                      {2763390.878, 733277.458, 5682089.111},
                      {2368378.937, 994508.273, 5817909.286}};
      
        src_3d_points = {
          Point3<double>(2441775.419, 799268.100, 5818729.162),
          Point3<double>(3464655.838, 845749.989, 5270271.528),
          Point3<double>(3309991.828, 828932.118, 5370882.280),
          Point3<double>(3160763.338, 759160.187, 5469345.504),
          Point3<double>(2248123.493, 865686.595, 5886425.596),
          Point3<double>(3022573.157, 802945.690, 5540683.951),
          Point3<double>(3104219.427, 998384.028, 5463290.505),
          Point3<double>(2998189.685, 931451.634, 5533398.462),
          Point3<double>(3199093.294, 932231.327, 5420322.483),
          Point3<double>(3370658.823, 711876.990, 5349786.786),
          Point3<double>(3341340.173, 957912.343, 5330003.236),
          Point3<double>(2534031.166, 975174.455, 5752078.309),
          Point3<double>(2838909.903, 903822.098, 5620660.184),
          Point3<double>(2902495.079, 761455.843, 5609859.672),
          Point3<double>(2682407.890, 950395.934, 5688993.082),
          Point3<double>(2620258.868, 779138.041, 5743799.267),
          Point3<double>(3246470.535, 1077900.355, 5365277.896),
          Point3<double>(3249408.275, 692757.965, 5426396.948),
          Point3<double>(2763885.496, 733247.387, 5682653.347),
          Point3<double>(2368885.005, 994492.233, 5818478.154)};

        dst_3d_points = {
          Point3<double>(2441276.712, 799286.666, 5818162.025),
          Point3<double>(3464161.275, 845805.461, 5269712.429),
          Point3<double>(3309496.800, 828981.942, 5370322.060),
          Point3<double>(3160269.913, 759204.574, 5468784.081),
          Point3<double>(2247621.426, 865698.413, 5885856.498),
          Point3<double>(3022077.340, 802985.055, 5540121.276),
          Point3<double>(3103716.966, 998426.412, 5462727.814),
          Point3<double>(2997689.029, 931490.201, 5532835.154),
          Point3<double>(3198593.776, 932277.179, 5419760.966),
          Point3<double>(3370168.626, 711928.884, 5349227.574),
          Point3<double>(3340840.578, 957963.383, 5329442.724),
          Point3<double>(2533526.497, 975196.347, 5751510.935),
          Point3<double>(2838409.359, 903854.897, 5620095.593),
          Point3<double>(2902000.172, 761490.908, 5609296.343),
          Point3<double>(2681904.794, 950423.098, 5688426.909),
          Point3<double>(2619761.810, 779162.964, 5743233.630),
          Point3<double>(3245966.134, 1077947.976, 5364716.214),
          Point3<double>(3248918.041, 692805.543, 5425836.841),
          Point3<double>(2763390.878, 733277.458, 5682089.111),
          Point3<double>(2368378.937, 994508.273, 5817909.286)};

    }

    void teardown()
    {

    }


    Matrix<double> src_mat;
    Matrix<double> dst_mat;
    std::vector<Point<double>> src_points;
    std::vector<Point<double>> dst_points;   
    std::vector<Point<double>> dst_points_helmert;   
    Matrix<double> src_3d_mat;
    Matrix<double> dst_3d_mat;
    std::vector<Point3<double>> src_3d_points;
    std::vector<Point3<double>> dst_3d_points;
};


BOOST_FIXTURE_TEST_CASE(default_constructor, AffineTest)
{
    Affine<double, 2> affine_2d;
    BOOST_CHECK_CLOSE(1., affine_2d(0, 0), 0.1);
    BOOST_CHECK_CLOSE(0., affine_2d(0, 1), 0.1);
    BOOST_CHECK_CLOSE(0., affine_2d(0, 2), 0.1);
    BOOST_CHECK_CLOSE(0., affine_2d(1, 0), 0.1);
    BOOST_CHECK_CLOSE(1., affine_2d(1, 1), 0.1);
    BOOST_CHECK_CLOSE(0., affine_2d(1, 2), 0.1);

    Affine<double, 3> affine_3d;
    BOOST_CHECK_CLOSE(1., affine_3d(0, 0), 0.1);
    BOOST_CHECK_CLOSE(0., affine_3d(0, 1), 0.1);
    BOOST_CHECK_CLOSE(0., affine_3d(0, 2), 0.1);
    BOOST_CHECK_CLOSE(0., affine_3d(0, 3), 0.1);
    BOOST_CHECK_CLOSE(0., affine_3d(1, 0), 0.1);
    BOOST_CHECK_CLOSE(1., affine_3d(1, 1), 0.1);
    BOOST_CHECK_CLOSE(0., affine_3d(1, 2), 0.1);
    BOOST_CHECK_CLOSE(0., affine_3d(1, 3), 0.1);
    BOOST_CHECK_CLOSE(0., affine_3d(2, 0), 0.1);
    BOOST_CHECK_CLOSE(0., affine_3d(2, 1), 0.1);
    BOOST_CHECK_CLOSE(1., affine_3d(2, 2), 0.1);
    BOOST_CHECK_CLOSE(0., affine_3d(2, 3), 0.1);
}

BOOST_FIXTURE_TEST_CASE(constructor_affine_2d, AffineTest)
{
    {
        Affine<double, 2> affine_2d(0.25, 0.30, 150.0, 75.0, consts::deg_to_rad<double> *35.);
        BOOST_CHECK_CLOSE(0.20478801107224795, affine_2d(0, 0), 0.1);
        BOOST_CHECK_CLOSE(-0.17207293090531381, affine_2d(0, 1), 0.1);
        BOOST_CHECK_CLOSE(150.0, affine_2d(0, 2), 0.1);
        BOOST_CHECK_CLOSE(0.14339410908776151, affine_2d(1, 0), 0.1);
        BOOST_CHECK_CLOSE(0.24574561328669753, affine_2d(1, 1), 0.1);
        BOOST_CHECK_CLOSE(75.0, affine_2d(1, 2), 0.1);
    }


    {
        Vector<double, 2> scale{0.25, 0.30};
        Vector<double, 2> translation{150.0, 75.0};

        Affine<double, 2> affine_2d(scale, translation, consts::deg_to_rad<double> *35.);
        BOOST_CHECK_CLOSE(0.20478801107224795, affine_2d(0, 0), 0.1);
        BOOST_CHECK_CLOSE(-0.17207293090531381, affine_2d(0, 1), 0.1);
        BOOST_CHECK_CLOSE(150.0, affine_2d(0, 2), 0.1);
        BOOST_CHECK_CLOSE(0.14339410908776151, affine_2d(1, 0), 0.1);
        BOOST_CHECK_CLOSE(0.24574561328669753, affine_2d(1, 1), 0.1);
        BOOST_CHECK_CLOSE(75.0, affine_2d(1, 2), 0.1);
    }
}

BOOST_FIXTURE_TEST_CASE(constructor_affine_3d, AffineTest)
{

}

BOOST_FIXTURE_TEST_CASE(affine_inverse, AffineTest)
{
    Affine<double, 2> affine(0.25, 0.3, 150., 75., consts::deg_to_rad<double> * 35.);
    auto inverse = affine.inverse();
}

BOOST_FIXTURE_TEST_CASE(transform_point2d, AffineTest)
{
    Affine<double, 2> affine(0.25, 0.3, 150., 75., consts::deg_to_rad<double> * 35.);

    auto transform_point = affine.transform(src_points[0]);

    BOOST_CHECK_CLOSE(737107.092, transform_point.x, 0.01);
    BOOST_CHECK_CLOSE(759565.279, transform_point.y, 0.01);

    transform_point = affine * src_points[1];

    BOOST_CHECK_CLOSE(731294.227, transform_point.x, 0.01);
    BOOST_CHECK_CLOSE(764301.907, transform_point.y, 0.01);

}

BOOST_FIXTURE_TEST_CASE(transform_vector_2d, AffineTest)
{
    Affine<double, 2> affine(0.25, 0.3, 150., 75., consts::deg_to_rad<double> *35.);

    Vector<double, 2> vector{4157222.543, 664789.307};
    auto transform_vector = affine.transform(vector);

    BOOST_CHECK_CLOSE(737107.092, transform_vector[0], 0.01);
    BOOST_CHECK_CLOSE(759565.279, transform_vector[1], 0.01);

    transform_vector = affine * Vector<double, 2>{4149043.336, 688836.443};
    
    BOOST_CHECK_CLOSE(731294.227, transform_vector[0], 0.01);
    BOOST_CHECK_CLOSE(764301.907, transform_vector[1], 0.01);
}

BOOST_FIXTURE_TEST_CASE(transform_matrix, AffineTest)
{
    Affine<double, 2> affine(0.25, 0.3, 150., 75., consts::deg_to_rad<double> *35.);

    {
        
        auto out = affine.transform(src_mat);

        BOOST_CHECK_CLOSE(737107.092, out[0][0], 0.1);
        BOOST_CHECK_CLOSE(759565.279, out[0][1], 0.1);
        BOOST_CHECK_CLOSE(731294.227, out[1][0], 0.1);
        BOOST_CHECK_CLOSE(764301.907, out[1][1], 0.1);
        BOOST_CHECK_CLOSE(735901.291, out[2][0], 0.1);
        BOOST_CHECK_CLOSE(768078.488, out[2][1], 0.1);
        BOOST_CHECK_CLOSE(744937.420, out[3][0], 0.1);
        BOOST_CHECK_CLOSE(757067.318, out[3][1], 0.1);
        BOOST_CHECK_CLOSE(731760.522, out[4][0], 0.1);
        BOOST_CHECK_CLOSE(758392.053, out[4][1], 0.1);
        BOOST_CHECK_CLOSE(734496.503, out[5][0], 0.1);
        BOOST_CHECK_CLOSE(758529.698, out[5][1], 0.1);
        BOOST_CHECK_CLOSE(726807.795, out[6][0], 0.1);
        BOOST_CHECK_CLOSE(766227.040, out[6][1], 0.1);
    }

    {
        auto out = affine * src_mat;

        BOOST_CHECK_CLOSE(737107.092, out[0][0], 0.1);
        BOOST_CHECK_CLOSE(759565.279, out[0][1], 0.1);
        BOOST_CHECK_CLOSE(731294.227, out[1][0], 0.1);
        BOOST_CHECK_CLOSE(764301.907, out[1][1], 0.1);
        BOOST_CHECK_CLOSE(735901.291, out[2][0], 0.1);
        BOOST_CHECK_CLOSE(768078.488, out[2][1], 0.1);
        BOOST_CHECK_CLOSE(744937.420, out[3][0], 0.1);
        BOOST_CHECK_CLOSE(757067.318, out[3][1], 0.1);
        BOOST_CHECK_CLOSE(731760.522, out[4][0], 0.1);
        BOOST_CHECK_CLOSE(758392.053, out[4][1], 0.1);
        BOOST_CHECK_CLOSE(734496.503, out[5][0], 0.1);
        BOOST_CHECK_CLOSE(758529.698, out[5][1], 0.1);
        BOOST_CHECK_CLOSE(726807.795, out[6][0], 0.1);
        BOOST_CHECK_CLOSE(766227.040, out[6][1], 0.1);
    }

}

BOOST_FIXTURE_TEST_CASE(estimate_points, AffineTest)
{
    auto affine = Affine2DEstimator<double>::estimate(src_points, dst_points);

    BOOST_CHECK_CLOSE(0.20478801107224795, affine(0, 0), 0.1);
    BOOST_CHECK_CLOSE(-0.17207293090531381, affine(0, 1), 0.1);
    BOOST_CHECK_CLOSE(150.0, affine(0, 2), 0.1);
    BOOST_CHECK_CLOSE(0.14339410908776151, affine(1, 0), 0.1);
    BOOST_CHECK_CLOSE(0.24574561328669753, affine(1, 1), 0.1);
    BOOST_CHECK_CLOSE(75.0, affine(1, 2), 0.1);
        
    auto scale = affine.scale();
    BOOST_CHECK_CLOSE(0.25, scale.x(), 0.1);
    BOOST_CHECK_CLOSE(0.30, scale.y(), 0.1);

    auto translation = affine.translation();
    BOOST_CHECK_CLOSE(150.0, translation.x(), 0.1);
    BOOST_CHECK_CLOSE(75.0, translation.y(), 0.1);

    auto rotation = affine.rotation();
    double rotation1 = atan2(rotation[1][0], rotation[0][0]);
    double rotation2 = atan2(-rotation[0][1], rotation[1][1]);
    BOOST_CHECK_CLOSE(35 * consts::deg_to_rad<double>, rotation1, 0.1);
    BOOST_CHECK_CLOSE(35 * consts::deg_to_rad<double>, rotation2, 0.1);
}

BOOST_FIXTURE_TEST_CASE(estimate_matrix, AffineTest)
{
    auto affine = Affine2DEstimator<double>::estimate(src_mat, dst_mat);

    BOOST_CHECK_CLOSE(0.20478801107224795, affine(0, 0), 0.1);
    BOOST_CHECK_CLOSE(-0.17207293090531381, affine(0, 1), 0.1);
    BOOST_CHECK_CLOSE(150.0, affine(0, 2), 0.1);
    BOOST_CHECK_CLOSE(0.14339410908776151, affine(1, 0), 0.1);
    BOOST_CHECK_CLOSE(0.24574561328669753, affine(1, 1), 0.1);
    BOOST_CHECK_CLOSE(75.0, affine(1, 2), 0.1);

    auto scale = affine.scale();
    BOOST_CHECK_CLOSE(0.25, scale.x(), 0.1);
    BOOST_CHECK_CLOSE(0.30, scale.y(), 0.1);

    auto translation = affine.translation();
    BOOST_CHECK_CLOSE(150.0, translation.x(), 0.1);
    BOOST_CHECK_CLOSE(75.0, translation.y(), 0.1);
}


BOOST_FIXTURE_TEST_CASE(helmert2d_estimate_points, AffineTest)
{
    auto affine = HelmertEstimator<double, 2>::estimate(src_points, dst_points_helmert);

    BOOST_CHECK_CLOSE(0.20478801107224795, affine(0, 0), 0.1);
    BOOST_CHECK_CLOSE(0.14339411103233352, affine(0, 1), 0.1);
    BOOST_CHECK_CLOSE(150.0, affine(0, 2), 0.1);
    BOOST_CHECK_CLOSE(0.14339410908776151, affine(1, 0), 0.1);
    BOOST_CHECK_CLOSE(0.20478801190648055, affine(1, 1), 0.1);
    BOOST_CHECK_CLOSE(75.0, affine(1, 2), 0.1);

    auto scale = affine.scale();
    BOOST_CHECK_CLOSE(0.25, scale.x(), 0.1);
    BOOST_CHECK_CLOSE(0.25, scale.y(), 0.1);

    auto translation = affine.translation();
    BOOST_CHECK_CLOSE(150.0, translation.x(), 0.1);
    BOOST_CHECK_CLOSE(75.0, translation.y(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(helmert3d_estimate_matrix, AffineTest)
{
    auto affine = HelmertEstimator<double, 3>::estimate(src_3d_mat, dst_3d_mat);
    
    BOOST_CHECK_CLOSE(-419.5684, affine(0, 3), 0.1);
    BOOST_CHECK_CLOSE(-99.2460, affine(1, 3), 0.1);
    BOOST_CHECK_CLOSE(-591.4559, affine(2, 3), 0.1);

    auto scale = affine.scale();
    BOOST_CHECK_CLOSE(1.0237, scale.x(), 0.1);
    BOOST_CHECK_CLOSE(1.0237, scale.y(), 0.1);
    BOOST_CHECK_CLOSE(1.0237, scale.z(), 0.1);

    auto translation = affine.translation();
    BOOST_CHECK_CLOSE(-419.5684, translation.x(), 0.1);
    BOOST_CHECK_CLOSE(-99.2460, translation.y(), 0.1);
    BOOST_CHECK_CLOSE(-591.4559, translation.z(), 0.1);
}

BOOST_FIXTURE_TEST_CASE(helmert3d_estimate_points, AffineTest)
{
    auto affine = HelmertEstimator<double, 3>::estimate(src_3d_points, dst_3d_points);

    //Por ahora no se devuelve la escala
    //BOOST_CHECK_CLOSE(1.0237, affine.scale(), 0.1);
    //BOOST_CHECK_CLOSE(0.8502, omega, 0.1);
    //BOOST_CHECK_CLOSE(1.8141, phi, 0.01);
    //BOOST_CHECK_CLOSE(-7.8535, kappa, 0.1);
    BOOST_CHECK_CLOSE(-419.5684, affine(0, 3), 0.1);
    BOOST_CHECK_CLOSE(-99.2460, affine(1, 3), 0.1);
    BOOST_CHECK_CLOSE(-591.4559, affine(2, 3), 0.1);

    auto scale = affine.scale();
    BOOST_CHECK_CLOSE(1.0237, scale.x(), 0.1);
    BOOST_CHECK_CLOSE(1.0237, scale.y(), 0.1);
    BOOST_CHECK_CLOSE(1.0237, scale.z(), 0.1);

    auto translation = affine.translation();
    BOOST_CHECK_CLOSE(-419.5684, translation.x(), 0.1);
    BOOST_CHECK_CLOSE(-99.2460, translation.y(), 0.1);
    BOOST_CHECK_CLOSE(-591.4559, translation.z(), 0.1);

}

BOOST_AUTO_TEST_SUITE_END()
