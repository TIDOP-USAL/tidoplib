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

#include <mutex>
#include <fstream>
#ifdef TL_HAVE_FMT
#include <fmt/format.h>
#else
#include <format>
#endif

#include "tidop/core/base/flags.h"
#include "tidop/core/base/path.h"
#include "tidop/core/app/messagehandler.h"

namespace tl
{

/*! \addtogroup AppManagement
 *  \{
 */


 /*!
  * \brief Log file class.
  *
  * This class handles logging messages to a file. It can operate independently,
  * writing messages directly to a log file, or integrate with the \ref Message class
  * for message handling.
  *
  * This class supports multiple message levels such as debug, info, success,
  * warning, and error, which can be configured using \ref setMessageLevel.
  */
class TL_EXPORT Log
    : public MessageHandler
{

private:

    std::ofstream _stream;                     /*!< Stream used to write to the log file. */
    static std::mutex mtx;                     /*!< Mutex for thread-safe operations. */
    EnumFlags<MessageLevel> messageLevelFlags; /*!< Flags for activated message levels. */

private:

    Log();

public:

    ~Log() override = default;

    TL_DISABLE_COPY(Log)
    TL_DISABLE_MOVE(Log)

    /*!
     * \brief Get the singleton instance of the log.
     * \return A reference to the Log instance.
     */
    static auto instance() -> Log&;

public:

    /*!
     * \brief Open the log file.
     * \param[in] file Path to the log file.
     */
    void open(const Path &file);

    /*!
     * \brief Close the log file.
     */
    void close();

    /*!
     * \brief Check if the log file is open.
     * \return True if the log file is open, false otherwise.
     */
    auto isOpen() const -> bool;

    /*!
     * \brief Get the current message levels.
     * \return Flags indicating the activated message levels.
     * \see EnumFlags
     */
    auto messageLevel() -> EnumFlags<MessageLevel>;

    /*!
     * \brief Set the message level for logging.
     *
     * Multiple log levels can be combined:
     *
     * \code
     * Log log;
     * log.setMessageLevel(MessageLevel::msg_warning | MessageLevel::msg_error);
     * \endcode
     *
     * \param[in] level Message level(s) to activate.
     */
    void setMessageLevel(MessageLevel level);

/// \cond
#if CPP_VERSION >= 20 || defined(TL_HAVE_FMT)
/// \endcond

    /*!
     * \brief Log a debug message using formatted arguments.
     * \tparam Args Argument types.
     * \param[in] s Format string.
     * \param[in] args Arguments for formatting.
     */
    template<typename... Args>
    static void debug(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        if (Log::instance().isOpen())
            Log::instance().debug(FORMAT_NAMESPACE vformat(s.get(), FORMAT_NAMESPACE make_format_args(args...)));
    }

    /*!
     * \brief Log an informational message using formatted arguments.
     * \tparam Args Argument types.
     * \param[in] s Format string.
     * \param[in] args Arguments for formatting.
     */
    template<typename... Args>
    static void info(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        if (Log::instance().isOpen())
            Log::instance().info(FORMAT_NAMESPACE vformat(s.get(), FORMAT_NAMESPACE make_format_args(args...)));
    }

    /*!
     * \brief Log a success message using formatted arguments.
     * \tparam Args Argument types.
     * \param[in] s Format string.
     * \param[in] args Arguments for formatting.
     */
    template<typename... Args>
    static void success(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        if (Log::instance().isOpen())
            Log::instance().success(FORMAT_NAMESPACE vformat(s.get(), FORMAT_NAMESPACE make_format_args(args...)));
    }

    /*!
     * \brief Log a warning message using formatted arguments.
     * \tparam Args Argument types.
     * \param[in] s Format string.
     * \param[in] args Arguments for formatting.
     */
    template<typename... Args>
    static void warning(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        if (Log::instance().isOpen())
            Log::instance().warning(FORMAT_NAMESPACE vformat(s.get(), FORMAT_NAMESPACE make_format_args(args...)));
    }

    /*!
     * \brief Log an error message using formatted arguments.
     * \tparam Args Argument types.
     * \param[in] s Format string.
     * \param[in] args Arguments for formatting.
     */
    template<typename... Args>
    static void error(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        if (Log::instance().isOpen())
            Log::instance().error(FORMAT_NAMESPACE vformat(s.get(), FORMAT_NAMESPACE make_format_args(args...)));
    }

#endif

// MessageHandler

public:

    /*!
     * \brief Handle a debug message.
     * \param[in] message The debug message to log.
     */
    void debug(String message) override;

    /*!
     * \brief Handle an informational message.
     * \param[in] message The informational message to log.
     */
    void info(String message) override;

    /*!
     * \brief Handle a success message.
     * \param[in] message The success message to log.
     */
    void success(String message) override;

    /*!
     * \brief Handle a warning message.
     * \param[in] message The warning message to log.
     */
    void warning(String message) override;

    /*!
     * \brief Handle an error message.
     * \param[in] message The error message to log.
     */
    void error(String message) override;

};



/*! \} */ // end of Log

/*! \} */

} // End namespace tl

