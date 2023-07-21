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
#if CPP_VERSION >= 20
#include <format>
#else
#include <fmt/format.h>
#endif

#include "tidop/core/console.h"
#include "tidop/core/log.h"
#include "tidop/core/msg/handler.h"

namespace tl
{

//class Console2;
//class Log2;

/*! \addtogroup core
 *  \{
 */

/*! \defgroup Messages Gestión de mensajes
 *
 *  \{
 */


class TL_EXPORT Message
{
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
        // Se comprueba que no este añadido
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
     * \brief Devuelve la fecha y hora del mensaje
     * \return Fecha y hora del mensaje
     */
    std::string date() const;

    /*!
     * \brief Devuelve el mensaje como cadena de texto
     * \return Mensaje
     */
    template<typename... Args>
    static std::string format(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        return FORMAT_NAMESPACE vformat(s.get(), FORMAT_NAMESPACE make_format_args(args...));
    }

    /*!
     * \brief setTimeLogFormat
     * \code
     * Message::setTimeLogFormat( "%d - %b - %Y (%H:%M:%S)" );
     * \endcode
     * \param[in] timeTemplate
     */
    //static void setTimeLogFormat(const std::string &timeTemplate);

#if CPP_VERSION >= 20 || defined(TL_HAVE_FMT)

    template<typename... Args>
    static void debug(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        if (stopHandler) return;

        Console2::debug(FORMAT_NAMESPACE vformat(s.get(), FORMAT_NAMESPACE make_format_args(args...)));
        Log2::debug(FORMAT_NAMESPACE vformat(s.get(), FORMAT_NAMESPACE make_format_args(args...)));
    }

    template<typename... Args>
    static void info(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        if (stopHandler) return;

        auto message = format(FORMAT_NAMESPACE vformat(s.get(), FORMAT_NAMESPACE make_format_args(args...)));
        Console2::info(message);
        Log2::info(message);

        std::list<MessageHandler *> handlers = messageHandlers;
        if (!stopHandler && !handlers.empty()) {
            for (auto &handler : handlers) {
              handler->info(message);
            }
        }
    }

    template<typename... Args>
    static void warning(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        if (stopHandler) return;

        Console2::warning(FORMAT_NAMESPACE vformat(s.get(), FORMAT_NAMESPACE make_format_args(args...)));
        Log2::warning(FORMAT_NAMESPACE vformat(s.get(),FORMAT_NAMESPACE make_format_args(args...)));
    }

    template<typename... Args>
    static void succes(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {   
        if (stopHandler) return;

        Console2::succes(FORMAT_NAMESPACE vformat(s.get(), FORMAT_NAMESPACE make_format_args(args...)));
        Log2::succes(FORMAT_NAMESPACE vformat(s.get(),FORMAT_NAMESPACE make_format_args(args...)));
    }

    template<typename... Args>
    static void error(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {   
        if (stopHandler) return;

        Console2::error(FORMAT_NAMESPACE vformat(s.get(), FORMAT_NAMESPACE make_format_args(args...)));
        Log2::error(FORMAT_NAMESPACE vformat(s.get(), FORMAT_NAMESPACE make_format_args(args...)));
    }

#endif

  private:

    /*!
     * \brief Mensaje
     */
    std::string mMessage;

    /*!
     * \brief Fecha y hora en la que se emitio el mensaje
     */
    std::string mDate;

    /*!
     * \brief Plantilla para el formateo de fecha y hora de los mensajes.
     *
     * Por defecto la plantilla es:
     * \code
     * std::string Message::timeLogTemplate = "%d/%b/%Y %H:%M:%S";
     * \endcode
     * \see setTimeLogFormat
     */
    static std::string sTimeLogFormat;

    static std::list<MessageHandler *> messageHandlers;
    static bool stopHandler;
};



/*! \} */ // end of Messages


/*! \} */ // end of core


} // End namespace tl
