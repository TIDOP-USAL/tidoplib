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

#include "tidop/core/app/Message.h"

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

    std::scoped_lock lck(messageHandlersMutex);
    messageHandlers.insert(messageHandler);
}

void Message::removeMessageHandler(MessageHandler *messageHandler)
{
    if (messageHandler == nullptr)
        return;

    std::scoped_lock lck(messageHandlersMutex);
    messageHandlers.erase(messageHandler);
}

void Message::clearMessageHandlers()
{
    std::scoped_lock lck(messageHandlersMutex);
    messageHandlers.clear();
}

void Message::debug(std::string_view message)
{
    dispatch(MessageLevel::debug, message);
}

void Message::info(std::string_view message)
{
    dispatch(MessageLevel::info, message);
}

void Message::success(std::string_view message)
{
    dispatch(MessageLevel::success, message);
}

void Message::warning(std::string_view message)
{
    dispatch(MessageLevel::warning, message);
}

void Message::error(std::string_view message)
{
    dispatch(MessageLevel::error, message);
}

} // End namespace tl
