﻿/**************************************************************************
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

#include "tidop/core/console/console.h"

#include "tidop/core/progress.h"

#include <map>
#include <iomanip>

namespace tl
{

std::mutex Console::mtx;
EnumFlags<MessageLevel> Console::messageLevelFlags = MessageLevel::all;

Console::Console()
  : _stream(std::cout) 
{
    init();
}

Console &Console::instance()
{
    static Console console;
    return console;
}

void Console::setTitle(const std::string &title)
{
#ifdef TL_OS_WINDOWS
    SetConsoleTitleA(title.c_str());
#else
    unusedParameter(title);
    //printf("%c]0;%s%c", '\033', title, '\007');
#endif
}

void Console::setBackgroundColor(Color backgroundColor,
                                 Intensity intensity)
{
    this->backgroundColor = static_cast<int>(backgroundColor) + 40 + static_cast<int>(intensity);

    update();
}


void Console::setForegroundColor(Color foregroundColor,
                                 Intensity intensity)
{
    this->foregroundColor = static_cast<int>(foregroundColor) + 30 + static_cast<int>(intensity);

    update();
}

void Console::setConsoleUnicode()
{
#ifdef TL_OS_WINDOWS
    //SetConsoleOutputCP(1252);
    //SetConsoleCP(1252);
    SetConsoleOutputCP(CP_UTF8/*65001*/);
#endif
}

void Console::setFontBold(bool bold)
{
    fontBold = bold ? 1 : 21;
    update();
}

void Console::setFontFaint(bool faint)
{
    fontFaint = faint ? 2 : 22;
    update();
}

void Console::setFontItalic(bool italic)
{
    fontItalic = italic ? 3 : 23;
    update();
}

void Console::setFontUnderline(bool underline)
{
    fontUnderline = underline ? 4 : 24;
    update();
}

void Console::setFontBlink(bool blink)
{
    fontBlink = blink ? 5 : 25;
    update();
}

void Console::setFontReverse(bool reverse)
{
    fontReverse = reverse ? 7 : 27;
    update();
}

void Console::setFontStrikethrough(bool strikethrough)
{
    fontStrikethrough = strikethrough ? 9 : 29;
    update();
}

void Console::reset()
{
    foregroundColor = 39;
    backgroundColor = 49;
    fontBold = 21;
    fontFaint = 22;
    fontItalic = 23;
    fontUnderline = 24;
    fontBlink = 25;
    fontReverse = 27;
    fontStrikethrough = 29;

    update();
}

Console &Console::operator <<(decltype(std::endl<char, std::char_traits<char>>) _endl)
{
    _stream << _endl;
    reset();
    return *this;
}

auto Console::messageLevel() -> EnumFlags<MessageLevel>
{
    return messageLevelFlags;
}

void Console::setMessageLevel(MessageLevel level)
{
    messageLevelFlags = level;
}

inline std::ostream &Console::red(std::ostream &os)
{
    Console::instance().setForegroundColor(Color::red);
    return os;
}

inline std::ostream &Console::green(std::ostream &os)
{
    Console::instance().setForegroundColor(Color::green);
    return os;
}

inline std::ostream &Console::blue(std::ostream &os)
{
    Console::instance().setForegroundColor(Color::blue);
    return os;
}

inline std::ostream &Console::cyan(std::ostream &os)
{
    Console::instance().setForegroundColor(Color::cyan);
    return os;
}

inline std::ostream &Console::magenta(std::ostream &os)
{
    Console::instance().setForegroundColor(Color::magenta);
    return os;
}

inline std::ostream &Console::yellow(std::ostream &os)
{
    Console::instance().setForegroundColor(Color::yellow);
    return os;
}

inline std::ostream &Console::black(std::ostream &os)
{
    Console::instance().setForegroundColor(Color::black);
    return os;
}

inline std::ostream &Console::white(std::ostream &os)
{
    Console::instance().setForegroundColor(Color::white);
    return os;
}

std::ostream &Console::bg_red(std::ostream &os)
{
    Console::instance().setBackgroundColor(Color::red);
    return os;
}

std::ostream &Console::bg_green(std::ostream &os)
{
    Console::instance().setBackgroundColor(Color::green);
    return os;
}

std::ostream &Console::bg_blue(std::ostream &os)
{
    Console::instance().setBackgroundColor(Color::blue);
    return os;
}

std::ostream &Console::bg_cyan(std::ostream &os)
{
    Console::instance().setBackgroundColor(Color::cyan);
    return os;
}

std::ostream &Console::bg_magenta(std::ostream &os)
{
    Console::instance().setBackgroundColor(Color::magenta);
    return os;
}

std::ostream &Console::bg_yellow(std::ostream &os)
{
    Console::instance().setBackgroundColor(Color::yellow);
    return os;
}

std::ostream &Console::bg_black(std::ostream &os)
{
    Console::instance().setBackgroundColor(Color::black);
    return os;
}

std::ostream &Console::bg_white(std::ostream &os)
{
    Console::instance().setBackgroundColor(Color::white);
    return os;
}

inline std::ostream &Console::bold(std::ostream &os)
{
    Console::instance().setFontBold(true);
    return os;
}

inline std::ostream &Console::faint(std::ostream &os)
{
    Console::instance().setFontFaint(true);
    return os;
}

inline std::ostream &Console::italic(std::ostream &os)
{
    Console::instance().setFontItalic(true);
    return os;
}

inline std::ostream &Console::underline(std::ostream &os)
{
    Console::instance().setFontUnderline(true);
    return os;
}

std::ostream &Console::reverse(std::ostream &os)
{
    Console::instance().setFontReverse(true);
    return os;
}

std::ostream &Console::strikethrough(std::ostream &os)
{
    Console::instance().setFontStrikethrough(true);
    return os;
}

std::ostream& Console::clear(std::ostream &os)
{
    Console::instance().reset();
    return os;
}

void Console::debug(String message)
{
    std::lock_guard<std::mutex> lck(Console::mtx);

    if (messageLevelFlags.isEnabled(MessageLevel::debug)) {
        Progress::cleanConsole();
        _stream << "Debug:   " << message << std::endl;
    }
}

void Console::info(String message)
{
    std::lock_guard<std::mutex> lck(Console::mtx);

    if (messageLevelFlags.isEnabled(MessageLevel::info)) {
        Progress::cleanConsole();
        _stream << "Info:    " << message << std::endl;
    }
}

void Console::success(String message)
{
    std::lock_guard<std::mutex> lck(Console::mtx);

    if (messageLevelFlags.isEnabled(MessageLevel::success)) {
        Progress::cleanConsole();
        setForegroundColor(Color::green, Intensity::normal);
        _stream << "Success: " << message << std::endl;
        reset();
    }
}

void Console::warning(String message)
{
    std::lock_guard<std::mutex> lck(Console::mtx);

    if (messageLevelFlags.isEnabled(MessageLevel::warning)) {
        Progress::cleanConsole();
        setForegroundColor(Color::magenta, Intensity::normal);
        _stream << "Warning: " << message << std::endl;
        reset();
    }
}

void Console::error(String message)
{
    std::lock_guard<std::mutex> lck(Console::mtx);

    if (messageLevelFlags.isEnabled(MessageLevel::error)) {
        Progress::cleanConsole();
        setForegroundColor(Color::red, Intensity::normal);
        _stream << "Error:   " << message << std::endl;
        reset();
    }
}

void Console::init()
{
    reset();
}

} // End mamespace tl


