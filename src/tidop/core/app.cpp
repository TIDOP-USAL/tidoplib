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

#include "tidop/core/app.h"

#include "tidop/core/console/console.h"
#include "tidop/core/messages.h"
#include "tidop/core/msg/handler.h"
#include "tidop/core/msg/buffer.h"
#include "tidop/core/msg/stream.h"
#include "tidop/core/log.h"

#if TL_OS_LINUX
#include <unistd.h>
#endif

#include <array>

namespace tl
{

std::unique_ptr<Message> App::_message;


App::App()
{
    init();
}

App &App::instance()
{
    static App app;
    return app;
}

tl::Path App::path() const
{
    static std::array<char, TL_MAX_PATH> runfile;

#ifdef TL_OS_WINDOWS
    ::GetModuleFileNameA(NULL, runfile.data(), TL_MAX_PATH);
    return tl::Path(std::string(runfile.data()));
#elif defined TL_OS_LINUX
    std::array<char, 32> _path{};
    sprintf(_path.data(), "/proc/%d/exe", getpid());
    long len = readlink(_path.data(), runfile.data(), runfile.size());
    if (len >= 0)
        runfile.at(static_cast<size_t>(len)) = '\0';

    return tl::Path(std::string(runfile.data()));
#endif
}

std::string App::version() const
{
    return std::string();
}

Console &App::console()
{
    static Console console;
    return console;
}

Console2 &App::console2()
{
    static Console2 console;
    return console;
}

Log &App::log()
{
    static Log log;
    return log;
}

MessageManager &App::messageManager()
{
    //static MessageManager message_manager;
    //return message_manager;
    return MessageManager::instance();
}

MessageHandler &App::messageHandler()
{
    static MessageHandler message_handler;
    return message_handler;
}

Message &App::message()
{
    //MessageBuffer *buffer = App::messageHandler().buffer();
    //static Message message(buffer);
    //return message;
    static std::once_flag init_flag;

    std::call_once(init_flag, []() {
        MessageBuffer *buffer = App::messageHandler().buffer();
        _message = std::make_unique<Message>(buffer);

    });

    return *_message;
}

void App::init()
{
}

} // namespace tl
