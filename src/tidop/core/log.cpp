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

Log &Log::instance()
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

bool Log::isOpen() const
{
    return _stream.is_open();
}

Log &Log::operator <<(MessageLevel level)
{
    switch (level) {
    case MessageLevel::debug:
        _stream << "Debug:   ";
        break;
    case MessageLevel::info:
        _stream << "Info:    ";
        break;
    case MessageLevel::success:
        _stream << "Succes:  ";
        break;
    case MessageLevel::warning:
        _stream << "Warning: ";
        break;
    case MessageLevel::error:
        _stream << "Error:   ";
        break;
    }

    return *this;
}

Log &Log::operator <<(decltype(std::endl<char, std::char_traits<char>>) _endl)
{
    _stream << _endl;
    return *this;
}

auto Log::messageLevel() -> EnumFlags<MessageLevel>
{
    return messageLevelFlags;
}

void Log::setMessageLevel(MessageLevel level)
{
    messageLevelFlags = level;
}

Log &Log::debug()
{
    auto &log = Log::instance();
    log << MessageLevel::debug;
    return log;
}

Log &Log::info()
{
    auto &log = Log::instance();
    log << MessageLevel::info;
    return log;
}

Log &Log::success()
{
    auto &log = Log::instance();
    log << MessageLevel::success;
    return log;
}

Log &Log::warning()
{
    auto &log = Log::instance();
    log << MessageLevel::warning;
    return log;
}

Log &Log::error()
{
    auto &log = Log::instance();
    log << MessageLevel::error;
    return log;
}

void Log::debug(String message)
{
    std::lock_guard<std::mutex> lck(Log::mtx);

    if (Log::instance().isOpen() && messageLevelFlags.isEnabled(MessageLevel::debug))
        Log::instance() << MessageLevel::debug << message << std::endl;
}

void Log::info(String message)
{
    std::lock_guard<std::mutex> lck(Log::mtx);

    if (Log::instance().isOpen() && messageLevelFlags.isEnabled(MessageLevel::info))
        Log::instance() << MessageLevel::info << message << std::endl;
}

void Log::success(String message)
{
    std::lock_guard<std::mutex> lck(Log::mtx);

    if (Log::instance().isOpen() && messageLevelFlags.isEnabled(MessageLevel::success))
        Log::instance() << MessageLevel::success << message << std::endl;
}

void Log::warning(String message)
{
    std::lock_guard<std::mutex> lck(Log::mtx);

    if (Log::instance().isOpen() && messageLevelFlags.isEnabled(MessageLevel::warning)) 
        Log::instance() << MessageLevel::warning << message << std::endl;
}

void Log::error(String message)
{
    std::lock_guard<std::mutex> lck(Log::mtx);
    
    if (Log::instance().isOpen() && messageLevelFlags.isEnabled(MessageLevel::error))
        Log::instance() << MessageLevel::error << message << std::endl;
}

} // End mamespace tl
