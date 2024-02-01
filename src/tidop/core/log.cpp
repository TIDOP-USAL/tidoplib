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

#include "tidop/core/log.h"
#include "tidop/core/app.h"
#include "tidop/core/chrono.h"
#include "tidop/core/path.h"



namespace tl
{

std::mutex Log::mtx;
EnumFlags<MessageLevel> Log::messageLevelFlags = MessageLevel::all;

auto Log::instance() -> Log &
{
    static Log log;
    return log;
}

void Log::open(const std::string &file)
{
    _stream.open(file, std::ofstream::app);
}

void Log::close()
{
    _stream.close();
}

auto Log::isOpen() const -> bool
{
    return _stream.is_open();
}

auto Log::messageLevel() -> EnumFlags<MessageLevel>
{
    return messageLevelFlags;
}

void Log::setMessageLevel(MessageLevel level)
{
    messageLevelFlags = level;
}

void Log::debug(String message)
{
    std::lock_guard<std::mutex> lck(Log::mtx);

    auto date = formatTimeToString("%d/%b/%Y %H:%M:%S");

    if (Log::instance().isOpen() && messageLevelFlags.isEnabled(MessageLevel::debug))
        _stream << date << " - Debug:   " << message << std::endl;
}

void Log::info(String message)
{
    std::lock_guard<std::mutex> lck(Log::mtx);

    auto date = formatTimeToString("%d/%b/%Y %H:%M:%S");

    if (Log::instance().isOpen() && messageLevelFlags.isEnabled(MessageLevel::info))
        _stream << date << " - Info:    " << message << std::endl;
}

void Log::success(String message)
{
    std::lock_guard<std::mutex> lck(Log::mtx);

    auto date = formatTimeToString("%d/%b/%Y %H:%M:%S");

    if (Log::instance().isOpen() && messageLevelFlags.isEnabled(MessageLevel::success))
        _stream << date << " - Success: " << message << std::endl;
}

void Log::warning(String message)
{
    std::lock_guard<std::mutex> lck(Log::mtx);

    auto date = formatTimeToString("%d/%b/%Y %H:%M:%S");

    if (Log::instance().isOpen() && messageLevelFlags.isEnabled(MessageLevel::warning)) 
        _stream << date << " - Warning: " << message << std::endl;
}                          

void Log::error(String message)
{
    std::lock_guard<std::mutex> lck(Log::mtx);
    
    auto date = formatTimeToString("%d/%b/%Y %H:%M:%S");

    if (Log::instance().isOpen() && messageLevelFlags.isEnabled(MessageLevel::error))
        _stream << date << " - Error:   " << message << std::endl;
}

} // End mamespace tl
