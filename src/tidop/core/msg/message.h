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

#include <list>
#ifdef TL_HAVE_FMT
#include <fmt/format.h>
#else
#include <format>
#endif

#include "tidop/core/msg/handler.h"

namespace tl
{


/*! \addtogroup core
 *  \{
 */

/*! \defgroup Messages Gestión de mensajes
 *
 *  \{
 */



class TL_EXPORT Message
{

#if CPP_VERSION >= 17
    using String = std::string_view;
#else
    using String = const std::string &;
#endif

private:

    Message(){}

public:
   
    /*!
     * \brief Destructora
     */
    ~Message() = default;

    static Message &instance();

    void addMessageHandler(MessageHandler *messageHandler)
    {
        bool added = false;
        for (auto handler : messageHandlers) {
            if (handler == messageHandler) {
                added = true;
                break;
            }
        }

        if (!added)
            messageHandlers.push_back(messageHandler);
    }

    void pauseMessages()
    {
        stopHandler = true;
    }

    void resumeMessages()
    {
        stopHandler = false;
    }

    /*!
     * \brief Devuelve el mensaje como cadena de texto
     * \return Mensaje
     */
    template<typename... Args>
    static std::string format(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        return FORMAT_NAMESPACE vformat(s.get(), FORMAT_NAMESPACE make_format_args(args...));
    }

    static void debug(String message);
    static void info(String message);
    static void success(String message);
    static void warning(String message);
    static void error(String message);

#if CPP_VERSION >= 20 || defined(TL_HAVE_FMT)

    template<typename... Args>
    static void debug(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        auto message = format(s, std::forward<Args>(args)...);
        Message::debug(message);
    }

    template<typename... Args>
    static void info(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        auto message = format(s, std::forward<Args>(args)...);
        Message::info(message);
    }

    template<typename... Args>
    static void warning(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        auto message = format(s, std::forward<Args>(args)...);
        Message::warning(message);
    }

    template<typename... Args>
    static void success(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {   
        auto message = format(s, std::forward<Args>(args)...);
        Message::success(message);
    }

    template<typename... Args>
    static void error(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {   
        auto message = format(s, std::forward<Args>(args)...);
        Message::error(message);
    }

#endif

  private:

    static std::list<MessageHandler *> messageHandlers;
    static bool stopHandler;
};



/*! \} */ // end of Messages


/*! \} */ // end of core


} // End namespace tl
