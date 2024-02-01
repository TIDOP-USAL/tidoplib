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
 
#define BOOST_TEST_MODULE Tidop rotation test
#include <boost/test/unit_test.hpp>
#include <tidop/math/algebra/rotation.h>
#include <tidop/math/algebra/matrix.h>


using namespace tl;

BOOST_AUTO_TEST_SUITE(RotationTestSuite)

struct RotationTest
{

    RotationTest() {}

    ~RotationTest() {}

    void setup()
    {

        src_points = {Point<double>(4157222.543, 664789.307),
                      Point<double>(4149043.336, 688836.443),
                      Point<double>(4172803.511, 690340.078),
                      Point<double>(4177148.376, 642997.635),
                      Point<double>(4137012.190, 671808.029),
                      Point<double>(4146292.729, 666952.887),
                      Point<double>(4138759.902, 702670.738)};

        dst_points = {Point<double>(3024089.863, 2929048.411),
                      Point<double>(3003596.978, 2944055.271),
                      Point<double>(3022197.724, 2958915.254),
                      Point<double>(3052911.339, 2922626.707),
                      Point<double>(3003508.738, 2923205.629),
                      Point<double>(3013895.705, 2924551.628),
                      Point<double>(2987238.257, 2949489.327) };

        src_vector = {Vector2d{{4157222.543, 664789.307}},
                      Vector2d{{4149043.336, 688836.443}},
                      Vector2d{{4172803.511, 690340.078}},
                      Vector2d{{4177148.376, 642997.635}},
                      Vector2d{{4137012.190, 671808.029}},
                      Vector2d{{4146292.729, 666952.887}},
                      Vector2d{{4138759.902, 702670.738}}};
        
        dst_vector = {Vector2d{{3024089.863, 2929048.411}},
                      Vector2d{{3003596.978, 2944055.271}},
                      Vector2d{{3022197.724, 2958915.254}},
                      Vector2d{{3052911.339, 2922626.707}},
                      Vector2d{{3003508.738, 2923205.629}},
                      Vector2d{{3013895.705, 2924551.628}},
                      Vector2d{{2987238.257, 2949489.327}}};

        src_matrix = {{4157222.543, 664789.307},
                      {4149043.336, 688836.443},
                      {4172803.511, 690340.078},
                      {4177148.376, 642997.635},
                      {4137012.190, 671808.029},
                      {4146292.729, 666952.887},
                      {4138759.902, 702670.738}};

        dst_matrix = {{3024089.863, 2929048.411},
                      {3003596.978, 2944055.271},
                      {3022197.724, 2958915.254},
                      {3052911.339, 2922626.707},
                      {3003508.738, 2923205.629},
                      {3013895.705, 2924551.628},
                      {2987238.257, 2949489.327}};

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
      
        src_3d_points = {Point3<double>(2441775.419, 799268.100, 5818729.162),
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

        dst_3d_points = {Point3<double>(2441276.712, 799286.666, 5818162.025),
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

    std::vector<Point<double>> src_points;
    std::vector<Point<double>> dst_points;
    std::vector<Vector2d> src_vector;
    std::vector<Vector2d> dst_vector;
    Matrix<double> src_matrix;
    Matrix<double> dst_matrix;
    Matrix<double> src_3d_mat;
    Matrix<double> dst_3d_mat;
    std::vector<Point3<double>> src_3d_points;
    std::vector<Point3<double>> dst_3d_points;
};


BOOST_FIXTURE_TEST_CASE(default_constructor, RotationTest)
{
    /* 2D */
    Rotation<double, 2> rotation;

    BOOST_CHECK_EQUAL(0., rotation.angle());

    auto rotation_matrix = rotation.toMatrix();
    BOOST_CHECK_EQUAL(1., rotation_matrix(0, 0));
    BOOST_CHECK_EQUAL(0., rotation_matrix(0, 1));
    BOOST_CHECK_EQUAL(0., rotation_matrix(1, 0));
    BOOST_CHECK_EQUAL(1., rotation_matrix(1, 1));

    /* 3D */

    Rotation<double, 3> rotation3d;
    auto rotation_matrix_3d = rotation3d.toMatrix();
    BOOST_CHECK_EQUAL(1., rotation_matrix_3d(0, 0));
    BOOST_CHECK_EQUAL(0., rotation_matrix_3d(0, 1));
    BOOST_CHECK_EQUAL(0., rotation_matrix_3d(0, 2));
    BOOST_CHECK_EQUAL(0., rotation_matrix_3d(1, 0));
    BOOST_CHECK_EQUAL(1., rotation_matrix_3d(1, 1));
    BOOST_CHECK_EQUAL(0., rotation_matrix_3d(1, 2));
    BOOST_CHECK_EQUAL(0., rotation_matrix_3d(2, 0));
    BOOST_CHECK_EQUAL(0., rotation_matrix_3d(2, 1));
    BOOST_CHECK_EQUAL(1., rotation_matrix_3d(2, 2));

    //BOOST_CHECK_EQUAL(0, translate3d.x());
    //BOOST_CHECK_EQUAL(0, translate3d.y());
    //BOOST_CHECK_EQUAL(0, translate3d.z());

    //BOOST_CHECK_EQUAL(0, translate3d.toVector()[0]);
    //BOOST_CHECK_EQUAL(0, translate3d.toVector()[1]);
    //BOOST_CHECK_EQUAL(0, translate3d.toVector()[2]);

    //BOOST_CHECK_EQUAL(0, translate3d.at(0));
    //BOOST_CHECK_EQUAL(0, translate3d.at(1));
    //BOOST_CHECK_EQUAL(0, translate3d.at(2));
    //
    //BOOST_CHECK_EQUAL(0, translate3d[0]);
    //BOOST_CHECK_EQUAL(0, translate3d[1]);
    //BOOST_CHECK_EQUAL(0, translate3d[2]);
}

BOOST_FIXTURE_TEST_CASE(constructor_2d, RotationTest)
{
    Rotation<double, 2> rotation(0.);

    BOOST_CHECK_EQUAL(0., rotation.angle());

    auto rotation_matrix = rotation.toMatrix();
    BOOST_CHECK_EQUAL(1., rotation_matrix(0, 0));
    BOOST_CHECK_EQUAL(0., rotation_matrix(0, 1));
    BOOST_CHECK_EQUAL(0., rotation_matrix(1, 0));
    BOOST_CHECK_EQUAL(1., rotation_matrix(1, 1));

    Rotation<double, 2> rotation2(35. * consts::deg_to_rad<double>);

    BOOST_CHECK_EQUAL(35. * consts::deg_to_rad<double>, rotation2.angle());

    rotation_matrix = rotation2.toMatrix();
    BOOST_CHECK_CLOSE(0.8191520442, rotation_matrix(0, 0), 0.1);
    BOOST_CHECK_CLOSE(-0.5735764363, rotation_matrix(0, 1), 0.1);
    BOOST_CHECK_CLOSE(0.5735764363, rotation_matrix(1, 0), 0.1);
    BOOST_CHECK_CLOSE(0.8191520442, rotation_matrix(1, 1), 0.1); 
}

BOOST_FIXTURE_TEST_CASE(constructor_3d, RotationTest)
{
    Rotation<double, 3> rotation3d(0., 0., 0.);
    auto rotation_matrix_3d = rotation3d.toMatrix();
    BOOST_CHECK_EQUAL(1., rotation_matrix_3d(0, 0));
    BOOST_CHECK_EQUAL(0., rotation_matrix_3d(0, 1));
    BOOST_CHECK_EQUAL(0., rotation_matrix_3d(0, 2));
    BOOST_CHECK_EQUAL(0., rotation_matrix_3d(1, 0));
    BOOST_CHECK_EQUAL(1., rotation_matrix_3d(1, 1));
    BOOST_CHECK_EQUAL(0., rotation_matrix_3d(1, 2));
    BOOST_CHECK_EQUAL(0., rotation_matrix_3d(2, 0));
    BOOST_CHECK_EQUAL(0., rotation_matrix_3d(2, 1));
    BOOST_CHECK_EQUAL(1., rotation_matrix_3d(2, 2));

}


BOOST_AUTO_TEST_SUITE_END()