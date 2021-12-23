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
 * License along with Foobar. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/
 
#define BOOST_TEST_MODULE Tidop utils test
#include <boost/test/unit_test.hpp>
#include <tidop/core/utils.h>


using namespace tl;

BOOST_AUTO_TEST_CASE(compare_insensitive_case)
{
  BOOST_CHECK_EQUAL(true, compareInsensitiveCase("AbcDEfgHI", "AbcDEfgHI"));
  BOOST_CHECK_EQUAL(true, compareInsensitiveCase("abcdefghi", "AbcDEfgHI"));
  BOOST_CHECK_EQUAL(true, compareInsensitiveCase("AbcDEfgHI", "ABCDEFGHI"));
  BOOST_CHECK_EQUAL(false, compareInsensitiveCase("AbcDEfgHI", "aaaaaa"));
}


/* ---------------------------------------------------------------------------------- */
/*                        Test Operaciones con cadenas                                */
/* ---------------------------------------------------------------------------------- */

BOOST_AUTO_TEST_CASE(split_to_numbers_empty_string)
{
  std::vector<int> vOut;
  BOOST_CHECK_EQUAL(0, splitToNumbers("", vOut));
  BOOST_CHECK_EQUAL(0, vOut.size());
  std::vector<double> vOutD;
  BOOST_CHECK_EQUAL(0, splitToNumbers("", vOutD));
  BOOST_CHECK_EQUAL(0, vOutD.size());
}


BOOST_AUTO_TEST_CASE(split_t_numbers_default_separator)
{
  std::vector<int> vOut;
  BOOST_CHECK_EQUAL(0, splitToNumbers("1,2,3,4", vOut));
  BOOST_CHECK_EQUAL(4, vOut.size());
  BOOST_CHECK_EQUAL(1, vOut[0]);
  BOOST_CHECK_EQUAL(2, vOut[1]);
  BOOST_CHECK_EQUAL(3, vOut[2]);
  BOOST_CHECK_EQUAL(4, vOut[3]);
  vOut.resize(0);
  BOOST_CHECK_EQUAL(0, splitToNumbers("0,5,6", vOut));
  BOOST_CHECK_EQUAL(3, vOut.size());
  BOOST_CHECK_EQUAL(0, vOut[0]);
  BOOST_CHECK_EQUAL(5, vOut[1]);
  BOOST_CHECK_EQUAL(6, vOut[2]);
  std::vector<double> vOutD;
  BOOST_CHECK_EQUAL(0, splitToNumbers("1.0,3.5,5.2", vOutD));
  BOOST_CHECK_EQUAL(3, vOutD.size());
  BOOST_CHECK_EQUAL(1.0, vOutD[0]);
  BOOST_CHECK_EQUAL(3.5, vOutD[1]);
  BOOST_CHECK_EQUAL(5.2, vOutD[2]);
  vOutD.resize(0);
  BOOST_CHECK_EQUAL(0, splitToNumbers("234.6,235253.6", vOutD));
  BOOST_CHECK_EQUAL(2, vOutD.size());
  BOOST_CHECK_EQUAL(234.6, vOutD[0]);
  BOOST_CHECK_EQUAL(235253.6, vOutD[1]);
}

BOOST_AUTO_TEST_CASE(split_t_numbers_separator)
{
  std::vector<int> vOut;
  BOOST_CHECK_EQUAL(0, splitToNumbers("1#2#3#4", vOut, "#"));
  BOOST_CHECK_EQUAL(4, vOut.size());
  BOOST_CHECK_EQUAL(1, vOut[0]);
  BOOST_CHECK_EQUAL(2, vOut[1]);
  BOOST_CHECK_EQUAL(3, vOut[2]);
  BOOST_CHECK_EQUAL(4, vOut[3]);
  vOut.resize(0);
  BOOST_CHECK_EQUAL(0, splitToNumbers("0-5-6", vOut, "-"));
  BOOST_CHECK_EQUAL(3, vOut.size());
  BOOST_CHECK_EQUAL(0, vOut[0]);
  BOOST_CHECK_EQUAL(5, vOut[1]);
  BOOST_CHECK_EQUAL(6, vOut[2]);
  std::vector<double> vOutD;
  BOOST_CHECK_EQUAL(0, splitToNumbers("1.0 3.5 5.2", vOutD, " "));
  BOOST_CHECK_EQUAL(3, vOutD.size());
  BOOST_CHECK_EQUAL(1.0, vOutD[0]);
  BOOST_CHECK_EQUAL(3.5, vOutD[1]);
  BOOST_CHECK_EQUAL(5.2, vOutD[2]);
  vOutD.resize(0);
  BOOST_CHECK_EQUAL(0, splitToNumbers("234.6@235253.6", vOutD, "@"));
  BOOST_CHECK_EQUAL(2, vOutD.size());
  BOOST_CHECK_EQUAL(234.6, vOutD[0]);
  BOOST_CHECK_EQUAL(235253.6, vOutD[1]);

  vOut.resize(0);
  BOOST_CHECK_EQUAL(1, splitToNumbers("345@45", vOut, ","));
  BOOST_CHECK_EQUAL(0, vOut.size());
  vOutD.resize(0);
  BOOST_CHECK_EQUAL(1, splitToNumbers("234.6@235253.6", vOutD, ","));
  BOOST_CHECK_EQUAL(0, vOutD.size());
}

BOOST_AUTO_TEST_CASE(replace_string)
{
  //std::string cadena = "Prueba cadena";
  //replaceString(&cadena, " ", "_");
  //BOOST_CHECK_EQUAL("Prueba_cadena", cadena);
  //replaceString(&cadena, " ", " ");
  //BOOST_CHECK_EQUAL("Prueba cadena", cadena);
}


BOOST_AUTO_TEST_CASE(split_string)
{
  std::vector<std::string> out;
  std::vector<std::string> v = split("");
  BOOST_CHECK_EQUAL(0, v.size());

}


BOOST_AUTO_TEST_CASE(split_default_separator)
{
  std::vector<std::string> out = split("cad1,cad2,cad3");
  BOOST_CHECK_EQUAL(3, out.size());
  BOOST_CHECK_EQUAL("cad1", out[0]);
  BOOST_CHECK_EQUAL("cad2", out[1]);
  BOOST_CHECK_EQUAL("cad3", out[2]);
}


BOOST_AUTO_TEST_CASE(split_separator)
{
  std::vector<std::string> out = split("cad1#cad2#cad3", "#");
  BOOST_CHECK_EQUAL(3, out.size());
  BOOST_CHECK_EQUAL("cad1", out[0]);
  BOOST_CHECK_EQUAL("cad2", out[1]);
  BOOST_CHECK_EQUAL("cad3", out[2]);
}