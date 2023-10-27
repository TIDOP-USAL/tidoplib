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
#include "tidop/core/flags.h"

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

/*! \defgroup Messages Message handling
 *
 *  \{
 */

enum class MessageLevel : int8_t
{
	debug = 1 << 0,
	error = 1 << 1,
	warning = 1 << 2,
    success = 1 << 3,
	info = 1 << 4,
    all = error | warning | success | info
};
ALLOW_BITWISE_FLAG_OPERATIONS(MessageLevel);

class MessageHandler
{

public:

#if CPP_VERSION >= 17
    using String = std::string_view;
#else
    using String = const std::string &;
#endif

public:

    MessageHandler() = default;
	virtual ~MessageHandler() = default;

    virtual void debug(String message) = 0;
    virtual void info(String message) = 0;
    virtual void success(String message) = 0;
    virtual void warning(String message) = 0;
    virtual void error(String message) = 0;

};


/*! \} */ // end of Messages


/*! \} */ // end of core


} // End namespace tl

