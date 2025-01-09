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
 
#define BOOST_TEST_MODULE Tidop utils test
#include <boost/test/unit_test.hpp>
#include <tidop/core/base/string_utils.h>
#include <tidop/core/base/split.h>
#include <tidop/core/base/type_conversions.h>

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
  std::vector<std::string> v = split<std::string>("");
  BOOST_CHECK_EQUAL(0, v.size());

}


BOOST_AUTO_TEST_CASE(split_default_separator)
{
  std::vector<std::string> out = split<std::string>("cad1,cad2,cad3");
  BOOST_CHECK_EQUAL(3, out.size());
  BOOST_CHECK_EQUAL("cad1", out[0]);
  BOOST_CHECK_EQUAL("cad2", out[1]);
  BOOST_CHECK_EQUAL("cad3", out[2]);
}


BOOST_AUTO_TEST_CASE(split_separator)
{
  std::vector<std::string> out = split<std::string>("cad1#cad2#cad3", '#');
  BOOST_CHECK_EQUAL(3, out.size());
  BOOST_CHECK_EQUAL("cad1", out[0]);
  BOOST_CHECK_EQUAL("cad2", out[1]);
  BOOST_CHECK_EQUAL("cad3", out[2]);

  std::vector<int> out2 = split<int>("1,2,3", ',');
  BOOST_CHECK_EQUAL(3, out2.size());
  BOOST_CHECK_EQUAL(1, out2[0]);
  BOOST_CHECK_EQUAL(2, out2[1]);
  BOOST_CHECK_EQUAL(3, out2[2]);

  std::vector<double> out3 = split<double>("1.2,2.6,3.4", ',');
  BOOST_CHECK_EQUAL(3, out3.size());
  BOOST_CHECK_EQUAL(1.2, out3[0]);
  BOOST_CHECK_EQUAL(2.6, out3[1]);
  BOOST_CHECK_EQUAL(3.4, out3[2]);
}


BOOST_AUTO_TEST_CASE(_args_size)
{
  BOOST_CHECK_EQUAL(4, tl::args_size(1, 2, 3, 4));

  std::string arg1("1");
  int arg2 = 2;
  BOOST_CHECK_EQUAL(2, tl::args_size(arg1, arg2));
}

BOOST_AUTO_TEST_CASE(_args_empty)
{
  BOOST_CHECK(tl::args_empty());
  BOOST_CHECK(!tl::args_empty(1, 3));
}

BOOST_AUTO_TEST_CASE(convert_string_to)
{
  int out = convertStringTo<int>("123");
  BOOST_CHECK_EQUAL(123, out);

  double out2 = convertStringTo<double>("152.24");
  BOOST_CHECK_EQUAL(152.24, out2);

  bool out3 = convertStringTo<bool>("true");
  BOOST_CHECK_EQUAL(true, out3);

  bool out4 = convertStringTo<bool>("false");
  BOOST_CHECK_EQUAL(false, out4);

  bool out5 = convertStringTo<bool>("1");
  BOOST_CHECK_EQUAL(true, out5);

  bool out6 = convertStringTo<bool>("0");
  BOOST_CHECK_EQUAL(false, out6);
}

BOOST_AUTO_TEST_CASE(is_in_range)
{
	BOOST_CHECK(isInRange(1, 0, 2));
	BOOST_CHECK(isInRange(0.5, -1., 2.));
	BOOST_CHECK(!isInRange(-0.5, 0., 2.));

}