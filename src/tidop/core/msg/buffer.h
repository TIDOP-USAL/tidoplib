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

#pragma once


#include "tidop/config.h"
#include "tidop/core/defs.h"

#include <iostream>
#include <vector>

#include "tidop/core/flags.h"


namespace tl
{


/*! \addtogroup core
 *  \{
 */

/*! \defgroup Messages Gestión de mensajes
 *
 *  \{
 */



class MessageBuffer
  : public std::streambuf
{

private:

  std::vector<std::streambuf *> buffer;

public:

  MessageBuffer()
    : buffer(0)
  {
  }

  //static MessageBuffer &instance()
  //{
  //  static MessageBuffer message_buffer;
  //  return message_buffer;
  //}

  void subscribe(std::streambuf *sb)
  {
    buffer.push_back(sb);
  }

  int overflow(int c)
  {
    typedef std::streambuf::traits_type traits;
    //bool rc(true);
    std::vector<bool> rc(buffer.size(), true);
    if (!traits::eq_int_type(traits::eof(), c)) {
      
      for (size_t i = 0; i < buffer.size(); i++) {
          traits::eq_int_type(this->buffer[i]->sputc(c), traits::eof()) && (rc[i] = false);
      }

    }

    bool r = false;
    for (auto c : rc) {
      if (c) {
        r = true;
        break;
      }
    }

    return r ? traits::not_eof(c) : traits::eof();
  }

  int sync()
  {
    bool rc(true);
    for (size_t i = 0; i < buffer.size(); i++)
      this->buffer[i]->pubsync() != -1 || (rc = false);
    return rc ? 0 : -1;
  }
};


/*! \} */ // end of Messages


/*! \} */ // end of core


} // End namespace tl

