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
#include "tidop/core/log.h"

#if TL_OS_LINUX
#include <unistd.h>
#endif

#include <array>

namespace tl
{

bool App::stopHandler = false;

App::App()
  : messageHandlers(0)
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

void App::addMessageHandler(MessageHandler *messageHandler)
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

void App::pauseMessages()
{
    stopHandler = true;
}

void App::resumeMessages()
{
    stopHandler = false;
}

Console &App::console()
{
    static Console console;
    return console;
}

Console2 &App::console2()
{
    return Console2::instance();
}

Log &App::log()
{
    static Log log;
    return log;
}

Log2 &App::log2()
{
    return Log2::instance();
}

MessageManager &App::messageManager()
{
    return MessageManager::instance();
}

void App::init()
{
  // The console is initialised
  Console2::instance();
}

} // namespace tl
