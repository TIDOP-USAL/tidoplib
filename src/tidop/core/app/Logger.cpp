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

std::mutex Logger::mtx;

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
    std::lock_guard<std::mutex> lck(Logger::mtx);

    if (isOpen()) close();
    _stream.open(file.toString(), std::ofstream::app);
}

void Logger::close()
{
    std::lock_guard<std::mutex> lck(Logger::mtx);
    _stream.close();
}

auto Logger::isOpen() const -> bool
{
    return _stream.is_open();
}

auto Logger::messageLevel() const -> EnumFlags<MessageLevel>
{
    return messageLevelFlags;
}

void Logger::setMessageLevel(MessageLevel level)
{
    messageLevelFlags = level;
}

static const char *getPadding(size_t level_len)
{
    static const char *padding[] = {"", " ", "  ", "   ", "    ", "    ", "    ", ""};
    return padding[level_len < 8 ? level_len : 7];
}

void Logger::logMessage(MessageLevel level, String level_name, String message)
{
    std::lock_guard<std::mutex> lck(Logger::mtx);

    if (!isOpen() || !messageLevelFlags.isEnabled(level))
        return;

    auto date = formatTimeToString("%d/%b/%Y %H:%M:%S");

#if TL_CPP_VERSION>= 17
    constexpr std::string_view padding[] = {"", " ", "  ", "   "};
    size_t level_len = level_name.length();
    std::string_view pad = (level_len < 7) ? padding[7 - level_len] : "";
    _stream << date << " - " << level_name << ":" << pad << " " << message << std::endl;
#else
    size_t level_len = level_name.length();
    auto pad = getPadding(7 - (level_len < 7 ? level_len : 7));
    _stream << date << " - " << level_name << ":" << pad << " " << message << std::endl;
#endif
}

void Logger::debug(String message)
{
    logMessage(MessageLevel::debug, "Debug", message);
}

void Logger::info(String message)
{
    logMessage(MessageLevel::info, "Info", message);
}

void Logger::success(String message)
{
    logMessage(MessageLevel::success, "Success", message);
}

void Logger::warning(String message)
{
    logMessage(MessageLevel::warning, "Warning", message);
}                          

void Logger::error(String message)
{
    logMessage(MessageLevel::error, "Error", message);
}

} // End namespace tl
