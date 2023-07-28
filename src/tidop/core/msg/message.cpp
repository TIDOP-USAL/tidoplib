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

#include "tidop/core/msg/message.h"


namespace tl
{

bool Message::stopHandler = false;
std::list<MessageHandler *> Message::messageHandlers;

//Message::Message(const char *message, ...)
//{
//  TL_TODO("revisar")
//  try {
//
//    mDate = formatTimeToString("%d/%b/%Y %H:%M:%S");
//
//    char buf[500];
//    memset(buf, 0, sizeof(buf));
//    std::string aux(message);
//    replaceString(&aux, "% ", "%% ");
//    replaceString(&aux, "%(\\s)", "%%");
//    va_list args;
//    va_start(args, message);
//#ifdef _MSC_VER
//  vsnprintf_s(buf, _countof(buf), _TRUNCATE, aux.c_str(), args);
//#else
//  vsnprintf(buf, sizeof(buf), aux.c_str(), args);
//#endif
//    va_end(args);
//    mMessage = buf;
//  } catch (...) {
//
//    // Por evitar un error en la constructora... 
//  }
//}

Message &Message::instance()
{
    static Message message;
    return message;
}

std::string Message::date() const
{
  return mDate;
}

//std::string Message::message() const
//{
//  return mMessage;
//}
//
//void Message::setTimeLogFormat(const std::string &timeTemplate)
//{
//  sTimeLogFormat = timeTemplate;
//}

void Message::debug(String message)
{
    if (stopHandler) return;

    std::list<MessageHandler *> handlers = messageHandlers;
    if (!stopHandler && !handlers.empty()) {
        for (auto &handler : handlers) {
            handler->debug(message);
        }
    }
}

void Message::info(String message)
{
    if (stopHandler) return;

    std::list<MessageHandler *> handlers = messageHandlers;
    if (!stopHandler && !handlers.empty()) {
        for (auto &handler : handlers) {
            handler->info(message);
        }
    }
}

void Message::success(String message)
{
    if (stopHandler) return;

    std::list<MessageHandler *> handlers = messageHandlers;
    if (!stopHandler && !handlers.empty()) {
        for (auto &handler : handlers) {
            handler->success(message);
        }
    }
}

void Message::warning(String message)
{
    if (stopHandler) return;

    std::list<MessageHandler *> handlers = messageHandlers;
    if (!stopHandler && !handlers.empty()) {
        for (auto &handler : handlers) {
            handler->warning(message);
        }
    }
}

void Message::error(String message)
{
    if (stopHandler) return;

    std::list<MessageHandler *> handlers = messageHandlers;
    if (!stopHandler && !handlers.empty()) {
        for (auto &handler : handlers) {
            handler->error(message);
        }
    }
}

} // End mamespace tl
