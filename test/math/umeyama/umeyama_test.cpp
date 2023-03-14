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

}


BOOST_AUTO_TEST_SUITE_END()
