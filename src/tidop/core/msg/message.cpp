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

//Message &Message::instance()
//{
//    static Message message;
//    return message;
//}

void Message::debug(String message)
{
    if (stopHandler) return;

    const std::list<MessageHandler *> handlers = messageHandlers;
    if (!stopHandler && !handlers.empty()) {
        for (MessageHandler *handler : handlers) {
            handler->debug(message);
        }
    }
}

void Message::info(String message)
{
    if (stopHandler) return;

    const std::list<MessageHandler *> handlers = messageHandlers;
    if (!stopHandler && !handlers.empty()) {
        for (MessageHandler *handler : handlers) {
            handler->info(message);
        }
    }
}

void Message::success(String message)
{
    if (stopHandler) return;

    const std::list<MessageHandler *> handlers = messageHandlers;
    if (!stopHandler && !handlers.empty()) {
        for (MessageHandler *handler : handlers) {
            handler->success(message);
        }
    }
}

void Message::warning(String message)
{
    if (stopHandler) return;

    const std::list<MessageHandler *> handlers = messageHandlers;
    if (!stopHandler && !handlers.empty()) {
        for (MessageHandler *handler : handlers) {
            handler->warning(message);
        }
    }
}

void Message::error(String message)
{
    if (stopHandler) return;

    const std::list<MessageHandler *> handlers = messageHandlers;
    if (!stopHandler && !handlers.empty()) {
        for (MessageHandler *handler : handlers) {
            handler->error(message);
        }
    }
}

} // End mamespace tl
