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

#ifndef TL_CORE_APP_H
#define TL_CORE_APP_H


#include "tidop/config.h"

#include <string>
#include <memory>
#include <mutex>
#if CPP_VERSION >= 20
#include <format>
#endif

#include "tidop/core/defs.h"
#include "tidop/core/path.h"
#include "tidop/core/log.h"

namespace tl
{

class Console;
class Log;
class MessageManager;

class MessageHandler;
class Console2;
class Log2;
class Message;

/*! \addtogroup core
 *  \{
 */

/*!
 * \brief Información de la aplicación 
 */
class TL_EXPORT App
{

private:

    App();

public:

    ~App() = default;

    TL_DISABLE_COPY(App)
    TL_DISABLE_MOVE(App)

    /*!
     * \brief Singleton
     */
    static App &instance();

    tl::Path path() const;
    std::string version() const;

    static Console &console();
    static Console2 &console2();
    static Log &log();
    static Log2 &log2();
    static MessageManager &messageManager();
    static MessageHandler &messageHandler();
    static Message &message();

#if CPP_VERSION >= 20

    template<typename... Args>
    static void debug(std::format_string<Args...> s, Args&&... args)
    {
        Console2::debug(std::vformat(s.get(), std::make_format_args(args...)));
        Log2::debug(std::vformat(s.get(), std::make_format_args(args...)));
    }

    template<typename... Args>
    static void info(std::format_string<Args...> s, Args&&... args)
    {
        Console2::info(std::vformat(s.get(), std::make_format_args(args...)));
        Log2::info(std::vformat(s.get(), std::make_format_args(args...)));
    }

    template<typename... Args>
    static void warning(std::format_string<Args...> s, Args&&... args)
    {
        Console2::warning(std::vformat(s.get(), std::make_format_args(args...)));
        Log2::warning(std::vformat(s.get(), std::make_format_args(args...)));
    }

    template<typename... Args>
    static void error(std::format_string<Args...> s, Args&&... args)
    {
        Console2::error(std::vformat(s.get(), std::make_format_args(args...)));
        Log2::error(std::vformat(s.get(), std::make_format_args(args...)));
    }

#endif

private:

    void init();

private:

  //static std::unique_ptr<Message> _message;

};

/*! \} */ // end of core

} // namespace tl

#endif TL_CORE_APP_H