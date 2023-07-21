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

#include <string>
#if CPP_VERSION >= 20
#include <format>
#else
#include <fmt/format.h>
#endif

namespace tl
{


/*! \addtogroup core
 *  \{
 */

/*! \defgroup Messages Gestión de mensajes
 *
 *  \{
 */


class MessageHandler
{

public:

	  MessageHandler() = default;
	  virtual ~MessageHandler() = default;

#if CPP_VERSION >= 17
    virtual void info(std::string_view message) = 0;
    virtual void success(std::string_view message) = 0;
    virtual void warning(std::string_view message) = 0;
    virtual void error(std::string_view message) = 0;
#else
    virtual void info(const std::string &message) = 0;
    virtual void success(const std::string &message) = 0;
    virtual void warning(const std::string &message) = 0;
    virtual void error(const std::string &message) = 0;
#endif

};


/*! \} */ // end of Messages


/*! \} */ // end of core


} // End namespace tl

