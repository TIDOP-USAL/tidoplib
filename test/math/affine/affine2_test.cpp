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
#include <tidop/math/algebra/transform.h>
#include <tidop/math/algebra/matrix.h>


using namespace tl;
using namespace tl::math;

BOOST_AUTO_TEST_SUITE(AffineTestSuite)

struct AffineTest
{

    AffineTest() {}

    ~AffineTest() {}

    void setup()
    {
        src_mat = { {4157222.543, 664789.307},
                    {4149043.336, 688836.443},
                    {4172803.511, 690340.078},
                    {4177148.376, 642997.635},
                    {4137012.190, 671808.029},
                    {4146292.729, 666952.887},
                    {4138759.902, 702670.738}};

        dst_mat = { {737107.092, 759565.279},
                    {731294.227, 764301.907},
                    {735901.291, 768078.488},
                    {744937.420, 757067.318},
                    {731760.522, 758392.053},
                    {734496.503, 758529.698},
                    {726807.795, 766227.040}};

        src_points = { Point<double>(4157222.543, 664789.307),
                       Point<double>(4149043.336, 688836.443),
                       Point<double>(4172803.511, 690340.078),
                       Point<double>(4177148.376, 642997.635),
                       Point<double>(4137012.190, 671808.029),
                       Point<double>(4146292.729, 666952.887),
                       Point<double>(4138759.902, 702670.738)};

        dst_points = { Point<double>(737107.092, 759565.279),
                       Point<double>(731294.227, 764301.907),
                       Point<double>(735901.291, 768078.488),
                       Point<double>(744937.420, 757067.318),
                       Point<double>(731760.522, 758392.053),
                       Point<double>(734496.503, 758529.698),
                       Point<double>(726807.795, 766227.040)};

    }

    void teardown()
    {

    }


    Matrix<double> src_mat;
    Matrix<double> dst_mat;
    std::vector<Point<double>> src_points;
    std::vector<Point<double>> dst_points;
};


BOOST_FIXTURE_TEST_CASE(default_constructor, AffineTest)
{

    Transform<double, 2> affine_2d = Affine2D<double>();
    BOOST_CHECK_CLOSE(1., affine_2d[0][0], 0.1);
    BOOST_CHECK_CLOSE(0., affine_2d[0][1], 0.1);
    BOOST_CHECK_CLOSE(0., affine_2d[0][2], 0.1);
    BOOST_CHECK_CLOSE(0., affine_2d[1][0], 0.1);
    BOOST_CHECK_CLOSE(1., affine_2d[1][1], 0.1);
    BOOST_CHECK_CLOSE(0., affine_2d[1][2], 0.1);
    BOOST_CHECK_CLOSE(0., affine_2d[2][0], 0.1);
    BOOST_CHECK_CLOSE(0., affine_2d[2][1], 0.1);
    BOOST_CHECK_CLOSE(1., affine_2d[2][2], 0.1);

    Transform<double, 3> affine_3d = Affine3D<double>();
    BOOST_CHECK_CLOSE(1., affine_3d[0][0], 0.1);
    BOOST_CHECK_CLOSE(0., affine_3d[0][1], 0.1);
    BOOST_CHECK_CLOSE(0., affine_3d[0][2], 0.1);
    BOOST_CHECK_CLOSE(0., affine_3d[0][3], 0.1);
    BOOST_CHECK_CLOSE(0., affine_3d[1][0], 0.1);
    BOOST_CHECK_CLOSE(1., affine_3d[1][1], 0.1);
    BOOST_CHECK_CLOSE(0., affine_3d[1][2], 0.1);
    BOOST_CHECK_CLOSE(0., affine_3d[1][3], 0.1);
    BOOST_CHECK_CLOSE(0., affine_3d[2][0], 0.1);
    BOOST_CHECK_CLOSE(0., affine_3d[2][1], 0.1);
    BOOST_CHECK_CLOSE(1., affine_3d[2][2], 0.1);
    BOOST_CHECK_CLOSE(0., affine_3d[2][3], 0.1);
    BOOST_CHECK_CLOSE(0., affine_3d[3][0], 0.1);
    BOOST_CHECK_CLOSE(0., affine_3d[3][1], 0.1);
    BOOST_CHECK_CLOSE(0., affine_3d[3][2], 0.1);
    BOOST_CHECK_CLOSE(1., affine_3d[3][3], 0.1);
}

BOOST_FIXTURE_TEST_CASE(constructor_affine_2d, AffineTest)
{
    //Affine2D<double> affine(150.0, 75.0, 0.25, 0.30, consts::deg_to_rad<double> * 35.);
    //auto dest = affine.transform(src_mat);
    //BOOST_CHECK_CLOSE(src_mat[0][0], dest[0][0], 0.1);
    //BOOST_CHECK_CLOSE(src_mat[0][1], dest[0][1], 0.1);
    //BOOST_CHECK_CLOSE(src_mat[1][0], dest[1][0], 0.1);
    //BOOST_CHECK_CLOSE(src_mat[1][1], dest[1][1], 0.1);
    //BOOST_CHECK_CLOSE(src_mat[2][0], dest[2][0], 0.1);
    //BOOST_CHECK_CLOSE(src_mat[2][1], dest[2][1], 0.1);
    //BOOST_CHECK_CLOSE(src_mat[3][0], dest[3][0], 0.1);
    //BOOST_CHECK_CLOSE(src_mat[3][1], dest[3][1], 0.1);
    //BOOST_CHECK_CLOSE(src_mat[4][0], dest[4][0], 0.1);
    //BOOST_CHECK_CLOSE(src_mat[4][1], dest[4][1], 0.1);
    //BOOST_CHECK_CLOSE(src_mat[5][0], dest[5][0], 0.1);
    //BOOST_CHECK_CLOSE(src_mat[5][1], dest[5][1], 0.1);
    //BOOST_CHECK_CLOSE(src_mat[6][0], dest[6][0], 0.1);
    //BOOST_CHECK_CLOSE(src_mat[6][1], dest[6][1], 0.1);

    //affine.transform(src_points);

}

BOOST_FIXTURE_TEST_CASE(constructor_affine_3d, AffineTest)
{

}

BOOST_AUTO_TEST_SUITE_END()
