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
 
#define BOOST_TEST_MODULE Tidop umeyama test
#include <boost/test/unit_test.hpp>
#include <tidop/math/algebra/umeyama.h>
#include <tidop/math/algebra/matrix.h>
#include <tidop/math/algebra/rotation_matrix.h>
#include <tidop/math/algebra/euler_angles.h>
#include <tidop/math/algebra/rotation_convert.h>


using namespace tl::math;


BOOST_AUTO_TEST_SUITE(UmeyamaTestSuite)

struct UmeyamaTest
{

  UmeyamaTest(){}
  
  ~UmeyamaTest(){}

  void setup()
  {

    src_mat = {{2441775.419, 799268.100, 5818729.162},
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

    dst_mat = {{2441276.712, 799286.666, 5818162.025},
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
  }

  void teardown()
  {
    
  }


  Matrix<double> src_mat;
  Matrix<double> dst_mat;

};


BOOST_FIXTURE_TEST_CASE(compute, UmeyamaTest)
{

  Umeyama<Matrix<double>> umeyama(src_mat, dst_mat);
  auto transform = umeyama.transform();

  BOOST_CHECK_CLOSE(1, transform[0][0], 0.1);
  BOOST_CHECK_CLOSE(-3.80747e-05, transform[0][1], 0.1);
  BOOST_CHECK_CLOSE(-8.79539e-06, transform[0][2], 0.1);
  BOOST_CHECK_CLOSE(-419.568, transform[0][3], 0.1);
  BOOST_CHECK_CLOSE(3.80748e-05, transform[1][0], 0.1);
  BOOST_CHECK_CLOSE(1, transform[1][1], 0.1);
  BOOST_CHECK_CLOSE(4.1215e-06, transform[1][2], 0.1);
  BOOST_CHECK_CLOSE(-99.246, transform[1][3], 0.1);
  BOOST_CHECK_CLOSE(8.79523e-06, transform[2][0], 0.1);
  BOOST_CHECK_CLOSE(-4.12183e-06, transform[2][1], 0.1);
  BOOST_CHECK_CLOSE(1, transform[2][2], 0.1);
  BOOST_CHECK_CLOSE(-591.456, transform[2][3], 0.1);
  BOOST_CHECK_CLOSE(0, transform[3][0], 0.1);
  BOOST_CHECK_CLOSE(0, transform[3][1], 0.1);
  BOOST_CHECK_CLOSE(0, transform[3][2], 0.1);
  BOOST_CHECK_CLOSE(1, transform[3][3], 0.1);

  auto rotation = umeyama.rotation();
  BOOST_CHECK_CLOSE(1., rotation[0][0], 0.1);
  BOOST_CHECK_CLOSE(-3.80747e-05, rotation[0][1], 0.1);
  BOOST_CHECK_CLOSE(-8.79539e-06, rotation[0][2], 0.1);
  BOOST_CHECK_CLOSE(3.80748e-05, rotation[1][0], 0.1);
  BOOST_CHECK_CLOSE(1., rotation[1][1], 0.1);
  BOOST_CHECK_CLOSE(4.1215e-06, rotation[1][2], 0.1);
  BOOST_CHECK_CLOSE(8.79523e-06, rotation[2][0], 0.1);
  BOOST_CHECK_CLOSE(-4.12183e-06, rotation[2][1], 0.1);
  BOOST_CHECK_CLOSE(1., rotation[2][2], 0.1);

  auto scale = umeyama.scale();
  BOOST_CHECK_CLOSE(1., scale, 0.1);
  //BOOST_CHECK_CLOSE(1., scale[0], 0.1);
  //BOOST_CHECK_CLOSE(1., scale[1], 0.1);
  //BOOST_CHECK_CLOSE(1., scale[2], 0.1);

  auto translation = umeyama.translation();
  BOOST_CHECK_CLOSE(-419.568, translation[0], 0.1);
  BOOST_CHECK_CLOSE(-99.246,  translation[1], 0.1);
  BOOST_CHECK_CLOSE(-591.456, translation[2], 0.1);

  {
    Matrix<double> src_mat2 = {{74.19025779187848, 8.749411901636444, -18.612330072771144},
               {74.36043674951422,8.749411901636464, -2.045092574570816},
               {91.2480433111534, 8.749411901636464, -1.961327929447604},
               {91.60235952827264, 8.749411901636464, -19.02572480319629}};

    Matrix<double> dst_mat2 = {{65.76787270674683, -2.842170943040401e-14, -24.428301993342885},
                {70.61906389289197, 0, 6.908026593137352},
                {99.47031667189891, 0, 3.111022232462366},
                {95.375769367781, -2.842170943040401e-14, -28.312814463968905}};

    Umeyama<Matrix<double>> umeyama(src_mat2, dst_mat2);
    auto transform = umeyama.transform();

    tl::math::Matrix<double> src_mat(src_mat2.rows(), 4);
    for (size_t i = 0, j = 0; i < src_mat2.rows(); i++) {
      src_mat(i, 0) = src_mat2(i, 0);
      src_mat(i, 1) = src_mat2(i, 1);
      src_mat(i, 2) = src_mat2(i, 2);
      src_mat(i, 3) = 1;
    }

    auto transform_points = transform * src_mat.transpose();
    auto diff = dst_mat2 - transform_points.transpose().block(0,3, 0, 2);

    std::cout << "Coordenadas de entrada:\n" << src_mat2 << std::endl;
    std::cout << "Coordenadas de salida:\n" <<  dst_mat2 << std::endl;
    std::cout << "Coordenadas transformadas:\n" << transform_points.transpose().block(0,3, 0, 2) << std::endl;
    std::cout << "Diferencia:\n" << diff << std::endl;

    auto rotation = umeyama.rotation();
    tl::math::RotationMatrix<double> rotation_matrix;
    rotation_matrix[0][0] = rotation[0][0];
    rotation_matrix[0][1] = rotation[0][1];
    rotation_matrix[0][2] = rotation[0][2];
    rotation_matrix[1][0] = rotation[1][0];
    rotation_matrix[1][1] = rotation[1][1];
    rotation_matrix[1][2] = rotation[1][2];
    rotation_matrix[2][0] = rotation[2][0];
    rotation_matrix[2][1] = rotation[2][1];
    rotation_matrix[2][2] = rotation[2][2];
    tl::math::EulerAngles<double> eulerAngles;
    tl::math::RotationConverter<double>::convert(rotation_matrix, eulerAngles);
    tl::math::Vector<double> scale{transform[0][0], transform[1][1], transform[2][2]};
    auto translation = umeyama.translation(); 
    std::cout << "\nTranslation:   [" << translation[0] << ", " << translation[1] << ", " << translation[2] << "]" << std::endl;
    std::cout << "Rotation:      [" << eulerAngles.x << ", " << eulerAngles.y << ", " << eulerAngles.z << "]" << std::endl;
    std::cout << "Scale:         [" << scale[0] << ", " << scale[1] << ", " << scale[2] << "]" << std::endl;
  }

}


BOOST_AUTO_TEST_SUITE_END()
