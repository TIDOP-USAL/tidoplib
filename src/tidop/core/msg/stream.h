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

#include "tidop/core/app.h"
#include "tidop/core/console.h"
#include "tidop/core/msg/buffer.h"


namespace tl
{


/*! \addtogroup core
 *  \{
 */

/*! \defgroup Messages Gestión de mensajes
 *
 *  \{
 */

class Message
  : public std::ostream
{

public:

    Message();
    ~Message();

    Message &operator <<(Level level)
    {
      switch(level) {
        case Level::debug:
            this->stream() << "Debug:   ";
            break;
        case Level::info:
            this->stream() << "Info:    ";
            break;
        case Level::warning:
            App::console2().setForegroundColor(Console2::Color::magenta, Console2::Intensity::normal);
            this->stream() << "Warning: ";
            break;
        case Level::error:
            App::console2().setForegroundColor(Console2::Color::red, Console2::Intensity::normal);
            this->stream() << "Error:   ";
            break;
        }

      return *this;
    }
    
    Message &operator <<(decltype(std::endl<char, std::char_traits<char>>) _endl)
    {
	    this->stream() << _endl;
      App::console2().reset();
	    return *this;
    }
    
    std::ostream &stream()
    {
        return *this;
    }
};


template<typename T>
inline Message &operator <<(Message &message, T value)
{
	message.stream() << value;
	return message;
}

/*! \} */ // end of Messages


/*! \} */ // end of core


} // End namespace tl
