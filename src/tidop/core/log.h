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

#include <mutex>
#include <fstream>
#if CPP_VERSION >= 20
#include <format>
#else
#include <fmt/format.h>
#endif

#include "tidop/core/utils.h"
#include "tidop/core/flags.h"
#include "tidop/core/msg/handler.h"

namespace tl
{

/*! \addtogroup core
 *  \{
 */


/*! \defgroup Log Fichero log
 *
 *  \{
 */


/*!
 * \brief Clase para gestionar ficheros log
 *
 * Esta clase puede funcionar individualmente o si se subscribe a
 * al gestor de mensajes (MessageManager) recibe automaticamente
 * los mensajes
 */
class Log
    : public MessageHandler
{

private:

    std::ofstream _stream;
    static std::mutex mtx;
    static EnumFlags<MessageLevel> messageLevelFlags;

private:

    Log() = default;

public:

    ~Log() = default;

    TL_DISABLE_COPY(Log)
    TL_DISABLE_MOVE(Log)

    static Log &instance();

// MessageHandler

public:

#if CPP_VERSION >= 17
    void debug(std::string_view message) override;
#else
    void debug(const std::string &message) override;
#endif

#if CPP_VERSION >= 17
    void info(std::string_view message) override;
#else
    void info(const std::string &message) override;
#endif

#if CPP_VERSION >= 17
    void success(std::string_view message) override;
#else
    void success(const std::string &message) override;
#endif

#if CPP_VERSION >= 17
    void warning(std::string_view message) override;
#else
    void warning(const std::string &message) override;
#endif

#if CPP_VERSION >= 17
    void error(std::string_view message) override;
#else
    void error(const std::string &message) override;
#endif

public:

    void open(const std::string &file);
    void close();
    bool isOpen() const;

    Log &operator <<(MessageLevel level);
    Log &operator <<(decltype(std::endl<char, std::char_traits<char>>) _endl);

    template<typename T>
    Log &operator <<(T value)
    {
	    _stream << value;
	    return *this;
    }

    /*!
     * \brief Message levels
     * \return Flag with message levels activated
     * \see EnumFlags
     */
    static auto messageLevel() -> EnumFlags<MessageLevel>;

    /*!
     * \brief Sets the message level
     *
     * Several log levels can be combined:
     *
     * \code
     * Log log;
     * log.setMessageLevel(MessageLevel::msg_warning | MessageLevel::msg_error);
     * \endcode
     *
     * \param[in] level Message level.
     */
    static void setMessageLevel(MessageLevel level);

    static Log &debug();
    static Log &info();
    static Log &success();
    static Log &warning();
    static Log &error();

#if CPP_VERSION >= 20 || defined(TL_HAVE_FMT)

    template<typename... Args>
    static void debug(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        if (Log::instance().isOpen())
            Log::instance().debug(FORMAT_NAMESPACE vformat(s.get(), FORMAT_NAMESPACE make_format_args(args...)));
    }

    template<typename... Args>
    static void info(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        if (Log::instance().isOpen())
            Log::instance().info(FORMAT_NAMESPACE vformat(s.get(), FORMAT_NAMESPACE make_format_args(args...)));
    }

    template<typename... Args>
    static void success(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        if (Log::instance().isOpen())
            Log::instance().success(FORMAT_NAMESPACE vformat(s.get(), FORMAT_NAMESPACE make_format_args(args...)));
    }

    template<typename... Args>
    static void warning(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        if (Log::instance().isOpen())
            Log::instance().warning(FORMAT_NAMESPACE vformat(s.get(), FORMAT_NAMESPACE make_format_args(args...)));
    }

    template<typename... Args>
    static void error(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        if (Log::instance().isOpen())
            Log::instance().error(FORMAT_NAMESPACE vformat(s.get(), FORMAT_NAMESPACE make_format_args(args...)));
    }

#endif

};



/*! \} */ // end of Log

/*! \} */ // end of core

} // End namespace tl

