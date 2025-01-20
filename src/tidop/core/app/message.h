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

#include <list>
#ifdef TL_HAVE_FMT
#include <fmt/format.h>
#else
#include <format>
#endif

#include "tidop/core/app/messagehandler.h"

namespace tl
{


/*! \addtogroup AppManagement
 *  \{
 */

/*!
 * \brief Message class
 *
 * The `Message` class is responsible for managing and emitting messages 
 * at different levels (debug, info, success, warning, error). It allows 
 * for the registration of \ref MessageHandler instances to process these messages.
 *
 * ### Example Usage
 * \code{.cpp}
 * // Add the Console as a message handler
 * Console &console = App::console();
 * console.setTitle("Transform Example");
 * console.setMessageLevel(MessageLevel::all);
 * Message::addMessageHandler(&console);
 * 
 * Message::warning("This is a {} message", "warning");
 * Message::info("{} + {} = {}", 1, 1, 2);
 * \endcode
 */
class TL_EXPORT Message
{

#if CPP_VERSION >= 17
    using String = std::string_view;
#else
    using String = const std::string &;
#endif

private:

    static std::list<MessageHandler *> messageHandlers; /*!< List of registered message handlers. */
    static bool stopHandler;                            /*!< Indicates whether message handling is paused. */

public:

    /*!
     * \brief Add a message handler.
     * \param[in] messageHandler Pointer to the message handler to be added.
     */
    static void addMessageHandler(MessageHandler *messageHandler)
    {
        bool added = false;
        for (const auto handler : messageHandlers) {
            if (handler == messageHandler) {
                added = true;
                break;
            }
        }

        if (!added)
            messageHandlers.push_back(messageHandler);
    }

    /*!
     * \brief Pause message handling.
     *
     * When paused, emitted messages are not processed by handlers until
     * \ref resumeMessages is called.
     */
    static void pauseMessages()
    {
        stopHandler = true;
    }

    /*!
     * \brief Resume message handling.
     */
    static void resumeMessages()
    {
        stopHandler = false;
    }

    /*!
     * \brief Format a message string with arguments.
     * \tparam Args Argument types.
     * \param[in] s Format string.
     * \param[in] args Arguments for formatting.
     * \return Formatted string.
     */
    template<typename... Args>
    static std::string format(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        return FORMAT_NAMESPACE vformat(s.get(), FORMAT_NAMESPACE make_format_args(args...));
    }

    /*!
     * \brief Emit a debug message.
     * \param[in] message Message string.
     */
    static void debug(String message);

    /*!
     * \brief Emit an informational message.
     * \param[in] message Message string.
     */
    static void info(String message);

    /*!
     * \brief Emit a success message.
     * \param[in] message Message string.
     */
    static void success(String message);
    
    /*!
     * \brief Emit a warning message.
     * \param[in] message Message string.
     */
    static void warning(String message);

    /*!
     * \brief Emit an error message.
     * \param[in] message Message string.
     */
    static void error(String message);

/// \cond
#if CPP_VERSION >= 20 || defined(TL_HAVE_FMT)
/// \endcond

    /*!
     * \brief Emit a debug message with formatted arguments.
     * \tparam Args Argument types.
     * \param[in] s Format string.
     * \param[in] args Arguments for formatting.
     */
    template<typename... Args>
    static void debug(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        auto message = format(s, std::forward<Args>(args)...);
        Message::debug(message);
    }

    /*!
     * \brief Emit an informational message with formatted arguments.
     * \tparam Args Argument types.
     * \param[in] s Format string.
     * \param[in] args Arguments for formatting.
     */
    template<typename... Args>
    static void info(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        auto message = format(s, std::forward<Args>(args)...);
        Message::info(message);
    }

    /*!
     * \brief Emit a success message with formatted arguments.
     * \tparam Args Argument types.
     * \param[in] s Format string.
     * \param[in] args Arguments for formatting.
     */
    template<typename... Args>
    static void success(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        auto message = format(s, std::forward<Args>(args)...);
        Message::success(message);
    }

    /*!
     * \brief Emit a warning message with formatted arguments.
     * \tparam Args Argument types.
     * \param[in] s Format string.
     * \param[in] args Arguments for formatting.
     */
    template<typename... Args>
    static void warning(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        auto message = format(s, std::forward<Args>(args)...);
        Message::warning(message);
    }

    /*!
     * \brief Emit an error message with formatted arguments.
     * \tparam Args Argument types.
     * \param[in] s Format string.
     * \param[in] args Arguments for formatting.
     */
    template<typename... Args>
    static void error(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        auto message = format(s, std::forward<Args>(args)...);
        Message::error(message);
    }

/// \cond
#endif
/// \endcond

};


/*! \} */


} // End namespace tl
