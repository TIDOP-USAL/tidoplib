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

#include "tidop/core/app/Logger.h"
#include "tidop/core/app/App.h"
#include "tidop/core/base/Chrono.h"
#include "tidop/core/base/Path.h"


namespace tl
{

//std::mutex Logger::mtx;

Logger::Logger()
  : messageLevelFlags(MessageLevel::all)
{
}

auto Logger::instance() -> Logger &
{
    static Logger logger;
    return logger;
}

void Logger::open(const tl::Path &file)
{
    std::scoped_lock lck(mtx);
    if (_stream.is_open()) {
        _stream.close();
    }
    _stream.open(file.toString(), std::ofstream::app);

    //std::lock_guard<std::mutex> lck(Logger::mtx);

    //if (isOpen()) close();
    //_stream.open(file.toString(), std::ofstream::app);
}

void Logger::close()
{
    //std::lock_guard<std::mutex> lck(Logger::mtx);
    //_stream.close();
    std::scoped_lock lck(mtx);
    if (_stream.is_open()) {
        _stream.close();
    }
}

auto Logger::isOpen() const -> bool
{
    std::scoped_lock lck(mtx);
    return _stream.is_open();
}

auto Logger::messageLevel() const noexcept -> EnumFlags<MessageLevel>
{
    return messageLevelFlags;
}

void Logger::setMessageLevel(MessageLevel level)
{
    messageLevelFlags = level;
}

//static const char *getPadding(size_t level_len)
//{
//    static const char *padding[] = {"", " ", "  ", "   ", "    ", "    ", "    ", ""};
//    return padding[level_len < 8 ? level_len : 7];
//}



void Logger::debug(std::string_view message)
{
    logMessage(MessageLevel::debug, message);
}

void Logger::info(std::string_view message)
{
    logMessage(MessageLevel::info, message);
}

void Logger::success(std::string_view message)
{
    logMessage(MessageLevel::success, message);
}

void Logger::warning(std::string_view message)
{
    logMessage(MessageLevel::warning, message);
}                          

void Logger::error(std::string_view message)
{
    logMessage(MessageLevel::error, message);
}

void Logger::logMessage(MessageLevel level, std::string_view message)
{
    //std::lock_guard<std::mutex> lck(Logger::mtx);
    std::scoped_lock lck(mtx);

    if (!isOpen() || !messageLevelFlags.isEnabled(level))
        return;

    //auto level_name = levelToString(level);
    auto date = formatTimeToString("%d/%b/%Y %H:%M:%S");

    //constexpr std::string_view padding[] = {"", " ", "  ", "   "};
    //size_t level_len = level_name.length();
    //std::string_view pad = (level_len < 7) ? padding[7 - level_len] : "";
    //_stream << date << " - " << level_name << ":" << pad << " " << message << std::endl;
    auto now = std::chrono::system_clock::now();

    _stream << std::format("{:%d/%b/%Y %H:%M:%S} - {:<7}: {}\n", now, levelToString(level), message);
}

constexpr auto Logger::levelToString(MessageLevel level) -> std::string_view
{
    switch (level) {
        case MessageLevel::debug:   return "Debug";
        case MessageLevel::info:    return "Info";
        case MessageLevel::success: return "Success";
        case MessageLevel::warning: return "Warning";
        case MessageLevel::error:   return "Error";
        default:                    return "Unknown";
    }
}

} // End namespace tl
