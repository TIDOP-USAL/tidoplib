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

/*!
 * \file Message.h
 * \brief Observer pattern implementation for message broadcasting.
 *
 * The tl::Message class implements a message publisher that dispatches events
 * to registered handlers (tl::Logger, tl::Console, etc.) with different severity levels.
 */ 

#pragma once

#include "tidop/config.h"

#include <set> 
#include <mutex>

#include "tidop/core/base/Format.h"
#include "tidop/core/app/MessageHandler.h"

namespace tl
{


/*! \addtogroup AppManagement
 *  \{
 */

/*!
 * \brief Message publisher using Observer pattern.
 *
 * The Message class implements a publisher that broadcasts messages to
 * multiple registered handlers. It acts as a central event dispatcher
 * for application-level logging and console output.
 *
 * ### Observer Pattern
 *
 * This class follows the Observer pattern where:
 * - **Subject (Publisher):** Message class
 * - **Observer (Listener):** MessageHandler implementations (Logger, Console)
 * - **Event:** Message emission at different levels
 *
 * ### Message Levels
 *
 * Messages can be emitted at different severity levels:
 * - `debug()` - Diagnostic information
 * - `info()` - General information
 * - `success()` - Successful operation
 * - `warning()` - Warning condition
 * - `error()` - Error condition
 *
 * ### Example Usage
 *
 * \code{.cpp}
 * // Register handlers
 * Logger& logger = App::log();
 * logger.open("app.log");
 * logger.setMessageLevel(MessageLevel::all);
 * Message::addMessageHandler(&logger);
 *
 * Console& console = App::console();
 * console.setMessageLevel(MessageLevel::warning | MessageLevel::error);
 * Message::addMessageHandler(&console);
 *
 * // Emit messages (will be sent to both logger and console)
 * Message::info("Application started");
 * Message::warning("Low memory condition");
 * Message::error("Failed to open file: {}", filename);
 *
 * // Temporarily suppress messages
 * Message::pauseMessages();
 * // ... messages won't reach handlers
 * Message::resumeMessages();
 * \endcode
 *
 * ### Formatted Messages
 *
 * Messages support formatting with fmt::format or std::format (C++20):
 *
 * \code{.cpp}
 * int count = 42;
 * double elapsed = 1.5;
 * Message::info("Processed {} records in {:.2f} seconds", count, elapsed);
 * \endcode
 * 
 * \note Thread-safe. All operations are protected by internal mutexes.
 * \note Handlers can be safely added or removed while messages are dispatched.
 * \note Message emission can be temporarily paused and resumed.
 * \see MessageHandler, Logger, Console, MessageLevel
 */
class TL_EXPORT Message
{

private:

    static std::set<MessageHandler *> messageHandlers;   /*!< Registered message handlers */
    static std::mutex messageHandlersMutex;              /*!< Protects messageHandlers list */
    static bool stopHandler;                             /*!< Flag to pause message dispatch */
    static std::mutex stopHandlerMutex;                  /*!< Protects stopHandler flag */

public:

    /*!
     * \brief Register a message handler.
     *
     * Adds the given handler to the list of handlers that receive messages.
     * If the handler is already registered, it is not added again.
     *
     * ### Example
     * \code{.cpp}
     * Logger& logger = App::log();
     * Message::addMessageHandler(&logger);
     * \endcode
     *
     * \param[in] messageHandler Pointer to the handler to register
     *
     * \note
     * - Passing a nullptr has no effect (silently ignored)
     * - Handlers are called in registration order
     * - Adding a handler while messages are being dispatched is safe
     * \note Thread-safe.
     * \see removeMessageHandler()
     */
    static void addMessageHandler(MessageHandler *messageHandler);

    /*!
     * \brief Unregister a message handler.
     *
     * Removes the given handler from the list. If not registered, this
     * has no effect.
     *
     * \param[in] messageHandler Pointer to the handler to remove
     * \note Thread-safe.
     * \see addMessageHandler()
     */
    static void removeMessageHandler(MessageHandler *messageHandler);

    /*!
     * \brief Clear all registered handlers.
     *
     * Removes all handlers at once. Useful for cleanup or handler replacement.
     *
     * \note Thread-safe.
     */
    static void clearMessageHandlers();

    /*!
     * \brief Temporarily pause message handling.
     *
     * When paused, calls to debug(), info(), success(), warning(), and error()
     * have no effect. Messages are silently discarded. Useful for suppressing
     * output during critical sections or batch operations.
     *
     * ### Example
     * \code{.cpp}
     * Message::pauseMessages();
     * // ... perform silent operations
     * Message::resumeMessages();
     * \endcode
     * 
     * \note Thread-safe.
     * \see resumeMessages()
     */
    static void pauseMessages()
    {
        std::lock_guard<std::mutex> lck(stopHandlerMutex);
        stopHandler = true;
    }

    /*!
     * \brief Resume message handling.
     *
     * Re-enables message dispatch after a call to pauseMessages().
     * Messages will be sent to handlers again.
     *
     * \note Thread-safe.
     * \see pauseMessages()
     */
    static void resumeMessages()
    {
        std::lock_guard<std::mutex> lck(stopHandlerMutex);
        stopHandler = false;
    }

    /*!
     * \brief Emit a debug message.
     *
     * Sends a debug-level message to all registered handlers.
     * Only sent if message handling is not paused.
     *
     * \param[in] message The message content
     * \note Thread-safe.
     * \see info(), success(), warning(), error()
     */
    static void debug(std::string_view message);

    /*!
     * \brief Emit an informational message.
     * \param[in] message The message content
     * \see debug(), success(), warning(), error()
     */
    static void info(std::string_view message);

    /*!
     * \brief Emit a success message.
     * \param[in] message The message content
     * \see debug(), info(), warning(), error()
     */
    static void success(std::string_view message);

    /*!
     * \brief Emit a warning message.
     * \param[in] message The message content
     * \see debug(), info(), success(), error()
     */
    static void warning(std::string_view message);

    /*!
     * \brief Emit an error message.
     * \param[in] message The message content
     * \see debug(), info(), success(), warning()
     */
    static void error(std::string_view message);

    /*!
     * \brief Emit a formatted debug message.
     *
     * ### Example
     * \code{.cpp}
     * int value = 42;
     * Message::debug("Value is {}", value);
     * \endcode
     *     
     * \tparam Args Argument types for format string
     * \param[in] s Format string (fmt::format or std::format compatible)
     * \param[in] args Arguments to substitute into format string
     * \see info(), success(), warning(), error()
     */
    template<typename... Args>
    static void debug(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        dispatch_format(MessageLevel::debug, s, std::forward<Args>(args)...);
    }

    /*!
     * \brief Emit a formatted informational message.
     * \tparam Args Argument types
     * \param[in] s Format string
     * \param[in] args Format arguments
     */
    template<typename... Args>
    static void info(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        dispatch_format(MessageLevel::info, s, std::forward<Args>(args)...);
    }

    /*!
     * \brief Emit a formatted success message.
     * \tparam Args Argument types
     * \param[in] s Format string
     * \param[in] args Format arguments
     */
    template<typename... Args>
    static void success(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        dispatch_format(MessageLevel::success, s, std::forward<Args>(args)...);
    }

    /*!
     * \brief Emit a formatted warning message.
     * \tparam Args Argument types
     * \param[in] s Format string
     * \param[in] args Format arguments
     */
    template<typename... Args>
    static void warning(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        dispatch_format(MessageLevel::warning, s, std::forward<Args>(args)...);
    }

    /*!
     * \brief Emit a formatted error message.
     * \tparam Args Argument types
     * \param[in] s Format string
     * \param[in] args Format arguments
     */
    template<typename... Args>
    static void error(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        dispatch_format(MessageLevel::error, s, std::forward<Args>(args)...);
    }
	
private:

    static void dispatch(MessageLevel level, std::string_view message)
    {
        {
            std::scoped_lock lck(stopHandlerMutex);
            if (stopHandler) return;
        }

        std::set<MessageHandler *> handlers_snapshot;
        {
            std::scoped_lock lck(messageHandlersMutex);
            handlers_snapshot = messageHandlers;
        }

        if (handlers_snapshot.empty()) return;

        bool any_enabled = false;
        for (MessageHandler *handler : handlers_snapshot) {
            if (handler->isEnabled(level)) {
                any_enabled = true;
                break;
            }
        }

        if (!any_enabled) return;

        for (MessageHandler *handler : handlers_snapshot) {
            if (handler->isEnabled(level)) {
                switch (level) {
                    case MessageLevel::debug:   handler->debug(message);   break;
                    case MessageLevel::info:    handler->info(message);    break;
                    case MessageLevel::success: handler->success(message); break;
                    case MessageLevel::warning: handler->warning(message); break;
                    case MessageLevel::error:   handler->error(message);   break;
                    default: break;
                }
            }
        }
    }

    template<typename... Args>
    static void dispatch_format(MessageLevel level, FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        {
            std::scoped_lock lck(stopHandlerMutex);
            if (stopHandler) return;
        }

        std::set<MessageHandler *> handlers_snapshot;
        {
            std::scoped_lock lck(messageHandlersMutex);
            handlers_snapshot = messageHandlers;
        }

        if (handlers_snapshot.empty()) return;

        bool any_enabled = false;
        for (MessageHandler *handler : handlers_snapshot) {
            if (handler->isEnabled(level)) {
                any_enabled = true;
                break;
            }
        }

        if (!any_enabled) return;

        std::string formatted_message = tl::format(s, std::forward<Args>(args)...);

        for (MessageHandler *handler : handlers_snapshot) {
            if (handler->isEnabled(level)) {
                switch (level) {
                    case MessageLevel::debug:   handler->debug(formatted_message);   break;
                    case MessageLevel::info:    handler->info(formatted_message);    break;
                    case MessageLevel::success: handler->success(formatted_message); break;
                    case MessageLevel::warning: handler->warning(formatted_message); break;
                    case MessageLevel::error:   handler->error(formatted_message);   break;
                    default: break;
                }
            }
        }
    }
};


/*! \} */


} // End namespace tl
