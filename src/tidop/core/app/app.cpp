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

#include "tidop/core/app/app.h"

#include "tidop/core/console/console.h"
#include "tidop/core/app/logger.h"

#ifdef TL_OS_LINUX
#include <unistd.h>
#include <cstdio>
#endif

#include <array>

namespace tl
{

App::App()
{
    init();
}

auto App::instance() -> App&
{
    static App app;
    return app;
}

auto App::path() const -> Path
{
    static Path cached_path;
    static std::once_flag flag;

    std::call_once(flag, [&]() {

        std::array<char, TL_MAX_PATH> runfile{};

#ifdef TL_OS_WINDOWS
        ::GetModuleFileNameA(nullptr, runfile.data(), static_cast<DWORD>(runfile.size()));
        cached_path = Path(std::string(runfile.data()));

#elif defined TL_OS_LINUX
        std::array<char, 32> proc_path{};
        snprintf(proc_path.data(), proc_path.size(), "/proc/%d/exe", getpid());

        long len = readlink(proc_path.data(), runfile.data(), runfile.size() - 1);
        if (len > 0) {
            runfile[static_cast<size_t>(len)] = '\0';
            cached_path = Path(std::string(runfile.data()));
        }

#else
        cached_path = Path();
#endif
    });

    return cached_path;
}

auto App::console() -> Console&
{
    return Console::instance();
}

auto App::log() -> Logger&
{
    return Logger::instance();
}

void App::init()
{
    Console::instance();
}

} // namespace tl
