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

#include "tidop/core/console.h"

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
#ifdef TL_OS_WINDOWS
    init(STD_OUTPUT_HANDLE);
#else
    init(stdout);
#endif
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
#ifdef TL_OS_WINDOWS
    switch (backgroundColor) {
        case Color::black:
            this->backgroundColor = 0;
            break;
        case Color::blue:
            this->backgroundColor = BACKGROUND_BLUE;
            break;
        case Color::green:
            this->backgroundColor = BACKGROUND_GREEN;
            break;
        case Color::cyan:
            this->backgroundColor = BACKGROUND_GREEN | BACKGROUND_BLUE;
            break;
        case Color::red:
            this->backgroundColor = BACKGROUND_RED;
            break;
        case Color::magenta:
            this->backgroundColor = BACKGROUND_RED | BACKGROUND_BLUE;
            break;
        case Color::yellow:
            this->backgroundColor = BACKGROUND_GREEN | BACKGROUND_RED;
            break;
        case Color::white:
            this->backgroundColor = BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED;
            break;
        default:
            this->backgroundColor = 0;
            break;
    }

    if (intensity == Intensity::normal)
        this->backgroundIntensity = 0;
    else
        this->backgroundIntensity = BACKGROUND_INTENSITY;
#else
        this->mBackgroundColor = static_cast<int>(backgroundColor) + 40 + static_cast<int>(intensity) * 60;
#endif

    update();
}


void Console::setForegroundColor(Color foregroundColor,
                                 Intensity intensity)
{
#ifdef TL_OS_WINDOWS

    switch(foregroundColor) {
        case Color::black:
            this->foregroundColor = 0;
            break;
        case Color::blue:
            this->foregroundColor = FOREGROUND_BLUE;
            break;
        case Color::green:
            this->foregroundColor = FOREGROUND_GREEN;
            break;
        case Color::cyan:
            this->foregroundColor = FOREGROUND_GREEN | FOREGROUND_BLUE;
            break;
        case Color::red:
            this->foregroundColor = FOREGROUND_RED;
            break;
        case Color::magenta:
            this->foregroundColor = FOREGROUND_RED | FOREGROUND_BLUE;
            break;
        case Color::yellow:
            this->foregroundColor = FOREGROUND_GREEN | FOREGROUND_RED;
            break;
        case Color::white:
            this->foregroundColor = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED;
            break;
        default:
            this->foregroundColor = 0;
            break;
    }

    if(intensity == Intensity::normal)
        this->foregroundIntensity = 0;
    else
        this->foregroundIntensity = FOREGROUND_INTENSITY;
#else
    mForegroundColor = static_cast<int>(foregroundColor) + 30 + static_cast<int>(intensity) * 60;
#endif

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
#ifdef TL_OS_WINDOWS
    mCurrentFont.FontWeight = bold ? FW_BOLD : FW_NORMAL;
#else
    mBold = bold ? 1 : 21;
#endif
    update();
}

void Console::setFontHeight(int16_t fontHeight)
{
#ifdef TL_OS_WINDOWS
    mCurrentFont.dwFontSize.Y = static_cast<SHORT>(fontHeight);
#else
    unusedParameter(fontHeight);
#endif
    update();
}

void Console::reset()
{
#ifdef TL_OS_WINDOWS
    foregroundColor = (oldColorAttrs & 0x0007);
    foregroundIntensity = (oldColorAttrs & 0x0008);
    backgroundColor = (oldColorAttrs & 0x0070);
    backgroundIntensity = (oldColorAttrs & 0x0080);
    update();
#else
    sprintf(mCommand, "%c[0;m", 0x1B);
    fprintf(mStream, "%s", mCommand);
#endif
}

Console &Console::operator <<(MessageLevel level)
{
    switch(level) {
    case MessageLevel::debug:
        _stream << "Debug:   ";
        break;
    case MessageLevel::info:
        _stream << "Info:    ";
        break;
    case MessageLevel::warning:
        setForegroundColor(Color::magenta, Intensity::normal);
        _stream << "Warning: ";
        break;
    case MessageLevel::success:
        setForegroundColor(Color::green, Intensity::normal);
        _stream << "Succes:  ";
        break;
    case MessageLevel::error:
        setForegroundColor(Color::red, Intensity::normal);
        _stream << "Error:   ";
        break;
    case MessageLevel::all:
        _stream << "Info:    ";
        break;
    }

    return *this;
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

Console &Console::debug()
{
    auto &console = Console::instance();
    console << MessageLevel::debug;
    return console;
}

Console &Console::info()
{
    auto &console = Console::instance();
    console << MessageLevel::info;
    return console;
}

Console &Console::success()
{
    auto &console = Console::instance();
    console << MessageLevel::success;
    return console;
}

Console &Console::warning()
{
    auto &console = Console::instance();
    console << MessageLevel::warning;
    return console;
}

Console &Console::error()
{
    auto &console = Console::instance();
    console << MessageLevel::error;
    return console;
}

void Console::debug(String message)
{
    std::lock_guard<std::mutex> lck(Console::mtx);

    if (messageLevelFlags.isEnabled(MessageLevel::debug))
        Console::instance() << MessageLevel::debug << message << std::endl;
}

void Console::info(String message)
{
    std::lock_guard<std::mutex> lck(Console::mtx);

    if (messageLevelFlags.isEnabled(MessageLevel::info))
        Console::instance() << MessageLevel::info << message << std::endl;
}

void Console::success(String message)
{
    std::lock_guard<std::mutex> lck(Console::mtx);

    if (messageLevelFlags.isEnabled(MessageLevel::success))
        Console::instance() << MessageLevel::success << message << std::endl;
}

void Console::warning(String message)
{
    std::lock_guard<std::mutex> lck(Console::mtx);

    if (messageLevelFlags.isEnabled(MessageLevel::warning))
        Console::instance() << MessageLevel::warning << message << std::endl;
}

void Console::error(String message)
{
    std::lock_guard<std::mutex> lck(Console::mtx);

    if (messageLevelFlags.isEnabled(MessageLevel::error))
        Console::instance() << MessageLevel::error << message << std::endl;
}

#ifdef TL_OS_WINDOWS
void Console::init(DWORD handle) 
{
    setConsoleUnicode();
    this->handle = GetStdHandle(handle);
    CONSOLE_SCREEN_BUFFER_INFO info;

    if(!GetConsoleScreenBufferInfo(this->handle, &info)) {
        oldColorAttrs = 0x0007;
    } else {
        oldColorAttrs = info.wAttributes;
    }

    foregroundColor = (oldColorAttrs & 0x0007);
    foregroundIntensity = (oldColorAttrs & 0x0008);
    backgroundColor = (oldColorAttrs & 0x0070);
    backgroundIntensity = (oldColorAttrs & 0x0080);
    mIniFont.cbSize = sizeof(mIniFont);
    GetCurrentConsoleFontEx(this->handle, FALSE, &mIniFont);
    mCurrentFont.cbSize = sizeof(mCurrentFont);
    mCurrentFont = mIniFont;
    //COORD fontSize = GetConsoleFontSize(mHandle, mIniFont.nFont);
    //CONSOLE_SCREEN_BUFFER_INFOEX cbi;
    //cbi.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
    //GetConsoleScreenBufferInfoEx(mHandle, &cbi);
    //cbi.wAttributes = BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY;
    //SetConsoleScreenBufferInfoEx(mHandle, &cbi);
}

#else

void Console::init(FILE *stream)
{
  mStream = stream;
  mForegroundColor = 0;
  mBackgroundColor = 0;
  mBold = 21;
}

#endif

} // End mamespace tl


