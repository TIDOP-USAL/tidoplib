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

#include "tidop/core/utils.h"

#include "tidop/core/messages.h"
#include "tidop/core/exception.h"

#include <boost/algorithm/string.hpp>
#include <sstream>

namespace tl
{

/* ---------------------------------------------------------------------------------- */
/*                             Operaciones con cadenas                                */
/* ---------------------------------------------------------------------------------- */

int splitToNumbers(const std::string &cad, std::vector<int> &vOut, const char *chs)
{
  int r_err = 0;
  char *dup = strdup(cad.c_str()); // -> warning C4996: 'strdup': The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: _strdup
  //char *dup = _strdup(cad.c_str());
  vOut.resize(0);

  try {
    char *token = strtok(dup, chs);
    //char *context = NULL;
    //char *token = strtok_s(dup, chs, &context);
    while (token != nullptr) {
      char *pEnd;
      int number = strtol(token, &pEnd, 10);
      if (*pEnd == 0) {
        vOut.push_back(number);
        token = strtok(nullptr, chs);
        //token = strtok_s(dup, chs, &context);
      } else
        throw std::runtime_error("Split string to numbers fail");
    }
  } catch (std::exception &e) {
    vOut.resize(0);
    msgError(e.what());
    r_err = 1;
  }

  free(dup);
  return r_err;
}

int splitToNumbers(const std::string &cad, std::vector<double> &vOut, const char *chs)
{
  int r_err = 0;
  char *dup = strdup(cad.c_str());
  //char *dup = _strdup(cad.c_str());
  vOut.resize(0);

  try {
    char *token = strtok(dup, chs);
    //char *context = NULL;
    //char *token = strtok_s(dup, chs, &context);
    while (token != NULL) {
      //vOut.push_back(atof(token));
      char *pEnd;
      double number = strtod(token, &pEnd);
      if (*pEnd == 0) {
        vOut.push_back(number);
        token = strtok(nullptr, chs);
        //token = strtok_s(dup, chs, &context);
      } else
        throw std::runtime_error("Split string to numbers fail");
    }
  } catch (std::exception &e) {
    vOut.resize(0);
    msgError(e.what());
    r_err = 1;
  }

  free(dup);
  return r_err;
}

void replaceString(std::string *str, const std::string &str_old, const std::string &str_new)
{
  std::size_t ini = str->find(str_old);
  //std::size_t end;
  while (ini != std::string::npos) {
    //end = ini + str_old.size();
    str->replace(ini, str_old.size(), str_new);
    ini = str->find(str_old, str_new.size() + ini);
  }
}

std::vector<std::string> split(const std::string &in, const std::string &chs)
{
  std::vector<std::string> out;
  boost::split(out, in, boost::is_any_of(chs));
  return out;
}

int stringToInteger(const std::string &text, Base base)
{
  std::istringstream ss(text);
  switch (base) {
  case Base::octal:
    ss.setf(std::ios_base::oct, std::ios::basefield);
    break;
  case Base::decimal:
    ss.setf(std::ios_base::dec, std::ios::basefield);
    break;
  case Base::hexadecimal:
    ss.setf(std::ios_base::hex, std::ios::basefield);
    break;
  default:
    break;
  }
  int number;
  return ss >> number ? number : 0;
}

bool compareInsensitiveCase(const std::string &source, const std::string &compare)
{
  return boost::iequals(source, compare);
}


} // End namespace tl

