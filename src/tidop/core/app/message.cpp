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

#include "tidop/core/app/message.h"

#include <algorithm>
#include <utility>

namespace tl
{

std::set<MessageHandler *> Message::messageHandlers;
std::mutex Message::messageHandlersMutex;
bool Message::stopHandler = false;
std::mutex Message::stopHandlerMutex;

void Message::addMessageHandler(MessageHandler *messageHandler)
{
    if (messageHandler == nullptr)
        return;

    std::lock_guard<std::mutex> lck(messageHandlersMutex);
    messageHandlers.insert(messageHandler);
}

void Message::removeMessageHandler(MessageHandler *messageHandler)
{
    if (messageHandler == nullptr)
        return;

    std::lock_guard<std::mutex> lck(messageHandlersMutex);
    messageHandlers.erase(messageHandler);
}

void Message::clearMessageHandlers()
{
    std::lock_guard<std::mutex> lck(messageHandlersMutex);
    messageHandlers.clear();
}

void Message::debug(String message)
{
    dispatch(message, [message](MessageHandler *h) { h->debug(message); });
}

void Message::info(String message)
{
    dispatch(message, [message](MessageHandler *h) { h->info(message); });
}

void Message::success(String message)
{
    dispatch(message, [message](MessageHandler *h) { h->success(message); });
}

void Message::warning(String message)
{
    dispatch(message, [message](MessageHandler *h) { h->warning(message); });
}

void Message::error(String message)
{
    dispatch(message, [message](MessageHandler *h) { h->error(message); });
}

} // End namespace tl
