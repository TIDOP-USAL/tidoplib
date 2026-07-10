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
 * \file Logger.h
 * \brief File-based message handler for logging to files
 *
 * \see tl::Message, tl::MessageHandler, tl::MessageLevel
 */

#pragma once

#include "tidop/config.h"

#include <mutex>
#include <fstream>

#include "tidop/core/base/Flags.h"
#include "tidop/core/base/Path.h"
#include "tidop/core/app/MessageHandler.h"
#include "tidop/core/base/Format.h"

namespace tl
{

/*! \addtogroup AppManagement
 *  \{
 */


/*!
 * \brief Singleton logger for file-based message handling
 *
 * Logger is a thread-safe singleton that writes timestamped messages to a log file.
 * It implements the MessageHandler interface and can be registered with the Message
 * class to automatically receive published messages.
 *
 * ### Features
 *
 * - **Thread-safe:** All operations protected by mutex
 * - **Singleton pattern:** Single instance accessed via instance()
 * - **Configurable levels:** Enable/disable individual message levels
 * - **Formatted logging:** Support for fmt::format and std::format
 * - **Append mode:** New logs are appended to existing files
 * 
 * ### Usage Example
 *
 * \code{.cpp}
 * // Initialize and open log file
 * Logger::instance().open("application.log");
 * Logger::instance().setMessageLevel(MessageLevel::all);
 *
 * // Log messages directly
 * Logger::instance().info("Application started");
 * Logger::instance().debug("Debug information");
 *
 * // Or use formatted logging with arguments
 * Logger::debug("Processing file: {}", filename);
 * Logger::info("Processed {} records in {} ms", count, elapsed);
 *
 * // Close log when done
 * Logger::instance().close();
 * \endcode
 * 
 * ### Log Levels
 *
 * Different message levels can be enabled or disabled independently using
 * \ref setMessageLevel. Multiple levels can be combined using bitwise OR:
 *
 * \code{.cpp}
 * Logger::instance().setMessageLevel(
 *     MessageLevel::warning | MessageLevel::error
 * );
 * \endcode
 *
 * ### Log Format
 *
 * Each log entry includes a timestamp in the format "DD/MMM/YYYY HH:MM:SS":
 * \code
 * 15/Dec/2024 14:32:15 - Debug:   Processing data
 * 15/Dec/2024 14:32:16 - Info:    Operation started
 * 15/Dec/2024 14:32:17 - Success: %Data processed
 * 15/Dec/2024 14:32:18 - Warning: Low memory
 * 15/Dec/2024 14:32:19 - Error:   Failed to save file
 * \endcode
 * \note Non-copyable and Non-movable. As a singleton, Logger cannot be copied or moved. 
 *        Attempting to do so will result in a compilation error.
 *
 * \see MessageHandler, Message
 */
class TL_EXPORT Logger
  : public MessageHandler
{

private:

    std::ofstream _stream;                      /*!< Output stream for the log file */
    mutable std::mutex mtx;                     /*!< Static mutex for thread-safe operations */
    EnumFlags<MessageLevel> messageLevelFlags;  /*!< Enabled message levels */

private:

    /*!
     * \brief Private constructor for singleton pattern
     */
    Logger();

public:

    ~Logger() override = default;

    TL_DISABLE_COPY(Logger)
    TL_DISABLE_MOVE(Logger)

    /*!
     * \brief Get the singleton instance of the logger
     *
     * Thread-safe access to the global Logger instance. The first call
     * creates the instance; subsequent calls return the same instance.
     *
     * ### Example
     * \code{.cpp}
     * Logger::instance().open("app.log");
     * Logger::instance().info("Starting application");
     * \endcode
     * 
     * \return Reference to the Logger singleton
     * \note This function is thread-safe due to static local variable initialization
     *       (C++11 Magic Statics).
     */
    [[nodiscard]]
    static auto instance() -> Logger&;

public:

    // ========================================================================
    // File Management
    // ========================================================================

    /*!
     * \brief Open a log file for writing
     *
     * Opens the specified file in append mode. If a file is already open, it is
     * closed first. If the file cannot be opened, the stream state will indicate
     * failure (check with isOpen()).
     *
     * ### Example
     * 
     * \code{.cpp}
     * Logger::instance().open("logs/application.log");
     * if (!Logger::instance().isOpen()) {
     *     std::cerr << "Failed to open log file\n";
     * }
     * \endcode
     * 
     * \param[in] file Path to the log file to open
     * \note Thread Safety
     * \see close(), isOpen()
     */
    void open(const Path &file);

    /*!
     * \brief Close the currently open log file
     *
     * Flushes any buffered data and closes the log file. After closing, isOpen()
     * will return false. Calling close() on an already-closed logger is safe.
     *
     * \see open(), isOpen()
     */
    void close();

    /*!
     * \brief Check if a log file is currently open
     *
     * \return True if the log file is open and ready for writing, false otherwise
     *
     * \see open(), close()
     */
    [[nodiscard]]
    auto isOpen() const -> bool;

    // ========================================================================
    // Message Level Configuration
    // ========================================================================

    /*!
     * \brief Get the currently enabled message levels
     *
     * Returns the flags representing which message levels are currently enabled
     * for logging.
     *
     * \return EnumFlags containing the active message levels
     *
     * ### Example
     * \code{.cpp}
     * auto levels = Logger::instance().messageLevel();
     * if (levels.isEnabled(MessageLevel::debug)) {
     *     std::cout << "Debug logging is enabled\n";
     * }
     * \endcode
     *
     * \see setMessageLevel(), MessageLevel
     */
    [[nodiscard]]
    auto messageLevel() const noexcept -> EnumFlags<MessageLevel>;

    /*!
     * \brief Set which message levels should be logged
     *
     * Configures which message levels trigger file writes. Multiple levels can be
     * combined using bitwise OR. Levels not specified will be ignored.
     *
     * ### Message Levels
     *
     * - `MessageLevel::debug` - Diagnostic information
     * - `MessageLevel::info` - General informational messages
     * - `MessageLevel::success` - Successful operation messages
     * - `MessageLevel::warning` - Warning messages
     * - `MessageLevel::error` - Error messages
     * - `MessageLevel::all` - All levels enabled
     *
     * ### Example
     * \code{.cpp}
     * // Log only warnings and errors
     * Logger::instance().setMessageLevel(
     *     MessageLevel::warning | MessageLevel::error
     * );
     *
     * // Log everything
     * Logger::instance().setMessageLevel(MessageLevel::all);
     * \endcode
     *
     * \param[in] level Message level(s) to enable
     * \see messageLevel(), MessageLevel
     */
    void setMessageLevel(MessageLevel level);
	
    [[nodiscard]]
    auto isEnabled(MessageLevel level) const -> bool override;

    // ========================================================================
    // Static Formatted Logging Methods
    // ========================================================================

    /*!
     * \brief Log a formatted debug message
     *
     * Static convenience method for logging debug messages with format arguments.
     * Only logs if the log file is open.
     *
     * ### Example
     * \code{.cpp}
     * Logger::debug("Processing file: {}", filename);
     * Logger::debug("Value: {}, Type: {}", value, type_name);
     * \endcode
     *
     * \tparam Args Variadic argument types
     * \param[in] s Format string (fmt::format or std::format compatible)
     * \param[in] args Arguments to substitute into the format string
     * \see info(), success(), warning(), error()
     */
    template<typename... Args>
    static void debug(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        logMessageFormat(MessageLevel::debug, s, std::forward<Args>(args)...);
    }

    /*!
     * \brief Log a formatted informational message
     *
     * \tparam Args Variadic argument types
     * \param[in] s Format string
     * \param[in] args Format arguments
     * \see debug(), success(), warning(), error()
     */
    template<typename... Args>
    static void info(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        logMessageFormat(MessageLevel::info, s, std::forward<Args>(args)...);
    }

    /*!
     * \brief Log a formatted success message
     *
     * \tparam Args Variadic argument types
     * \param[in] s Format string
     * \param[in] args Format arguments
     * \see debug(), info(), warning(), error()
     */
    template<typename... Args>
    static void success(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        logMessageFormat(MessageLevel::success, s, std::forward<Args>(args)...);
    }

    /*!
     * \brief Log a formatted warning message
     *
     * \tparam Args Variadic argument types
     * \param[in] s Format string
     * \param[in] args Format arguments
     * \see debug(), info(), success(), error()
     */
    template<typename... Args>
    static void warning(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        logMessageFormat(MessageLevel::warning, s, std::forward<Args>(args)...);
    }

    /*!
     * \brief Log a formatted error message
     *
     * \tparam Args Variadic argument types
     * \param[in] s Format string
     * \param[in] args Format arguments
     * \see debug(), info(), success(), warning()
     */
    template<typename... Args>
    static void error(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        logMessageFormat(MessageLevel::error, s, std::forward<Args>(args)...);
    }

    // ========================================================================
    // MessageHandler Implementation
    // ========================================================================

public:

    /*!
     * \brief Handle a debug message from the Message publisher
     *
     * Writes a debug-level message to the log file if debug logging is enabled.
     *
     * \param[in] message The message content to log
     * \see MessageHandler
     * \note Thread-safe.
     */
    void debug(std::string_view message) override;

    /*!
     * \brief Handle an informational message from the Message publisher
     *
     * Writes an info-level message to the log file if info logging is enabled.
     *
     * \param[in] message The message content to log
     *
     * \see MessageHandler
     * \note Thread-safe.
     */
    void info(std::string_view message) override;

    /*!
     * \brief Handle a success message from the Message publisher
     *
     * Writes a success-level message to the log file if success logging is enabled.
     *
     * \param[in] message The message content to log
     *
     * \see MessageHandler
     * \note Thread-safe.
     */
    void success(std::string_view message) override;

    /*!
     * \brief Handle a warning message from the Message publisher
     *
     * Writes a warning-level message to the log file if warning logging is enabled.
     *
     * \param[in] message The message content to log
     *
     * \see MessageHandler
     * \note Thread-safe.
     */
    void warning(std::string_view message) override;

    /*!
     * \brief Handle an error message from the Message publisher
     *
     * Writes an error-level message to the log file if error logging is enabled.
     *
     * \param[in] message The message content to log
     *
     * \see MessageHandler
     * \note Thread-safe.
     */
    void error(std::string_view message) override;
	
private:

    template<typename... Args>
    static void logMessageFormat(MessageLevel level, FORMAT_NAMESPACE format_string<Args...> fmt, Args&&... args)
    {
        auto &log = Logger::instance();
        if (log.isOpen() && log.messageLevel().isEnabled(level)) {
            log.dispatch(level, tl::format(fmt, std::forward<Args>(args)...));
        }
    }

    void dispatch(MessageLevel level, std::string_view message);

    static constexpr auto levelToString(MessageLevel level) -> std::string_view;
};

/*! \} */

} // End namespace tl

