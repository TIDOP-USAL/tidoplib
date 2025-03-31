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
#include "tidop/core/base/defs.h"
#include "tidop/core/base/flags.h"

#include <string>
//#if CPP_VERSION >= 20
//#include <format>
//#else
//#include <fmt/format.h>
//#endif

namespace tl
{


/*! \addtogroup AppManagement
 *  \{
 */

/*!
 * \brief Levels of messages emitted by the Message class.
 *
 * This enumeration defines the different levels of messages that can
 * be emitted and handled. The levels are designed to categorize messages
 * based on their importance or type.
 *
 * Each level can be combined using bitwise operations to enable multiple
 * levels simultaneously, as shown in the example below:
 *
 * ### Example Usage
 * \code{.cpp}
 * MessageLevel levels = MessageLevel::error | MessageLevel::warning;
 * \endcode
 */
enum class MessageLevel : int8_t
{
    debug = 1 << 0,                        /*!< Debug-level messages, typically for developers. */
    error = 1 << 1,                        /*!< Error-level messages, indicating critical issues. */
    warning = 1 << 2,                      /*!< Warning-level messages, highlighting potential issues. */
    success = 1 << 3,                      /*!< Success-level messages, indicating successful operations. */
    info = 1 << 4,                         /*!< Informational messages, providing general information. */
    all = error | warning | success | info /*!< Combination of all non-debug levels. */
};
ALLOW_BITWISE_FLAG_OPERATIONS(MessageLevel)

/*!
 * \brief Interface for message handlers.
 *
 * Classes implementing the `MessageHandler` interface can process 
 * messages emitted by the \ref Message class. For example:
 * 
 * ### Example Usage
 * \code{.cpp}
 * Console &console = App::console();
 * Message::addMessageHandler(&console);
 * \endcode
 */
class TL_EXPORT MessageHandler
{

public:

#if CPP_VERSION >= 17
    using String = std::string_view;
#else
    using String = const std::string &;
#endif

public:

    /*!
     * \brief Constructor.
     */
    MessageHandler() = default;

    /*!
     * \brief Destructor.
     */
    virtual ~MessageHandler() = default;

    /*!
     * \brief Handle a debug message.
     * \param[in] message Message string.
     */
    virtual void debug(String message) = 0;

    /*!
     * \brief Handle an informational message.
     * \param[in] message Message string.
     */
    virtual void info(String message) = 0;

    /*!
     * \brief Handle a success message.
     * \param[in] message Message string.
     */
    virtual void success(String message) = 0;

    /*!
     * \brief Handle a warning message.
     * \param[in] message Message string.
     */
    virtual void warning(String message) = 0;

    /*!
     * \brief Handle an error message.
     * \param[in] message Message string.
     */
    virtual void error(String message) = 0;

};


/*! \} */ 


} // End namespace tl

