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

struct msgProperties {
    const char *normal;
    const char *extend;
    Console::Color foregroundColor;
    Console::Intensity intensity;
};

static struct msgProperties msgTemplate[] = {
  { "Debug:   %s", "Debug:   %s (%s:%u, %s)", Console::Color::white, Console::Intensity::normal},
  { "Info:    %s", "Info:    %s (%s:%u, %s)", Console::Color::white, Console::Intensity::bright},
  { "Warning: %s", "Warning: %s (%s:%u, %s)", Console::Color::magenta, Console::Intensity::bright},
  { "Error:   %s", "Error:   %s (%s:%u, %s)", Console::Color::red, Console::Intensity::bright}
};

msgProperties messageProperties(MessageLevel msgLevel)
{
  int iLevel = 0;
  switch (msgLevel) {
  case MessageLevel::msg_debug:
    iLevel = 0;
    break;
  case MessageLevel::msg_info:
    iLevel = 1;
    break;
  case MessageLevel::msg_warning:
    iLevel = 2;
    break;
  case MessageLevel::msg_error:
    iLevel = 3;
    break;
  default:
    iLevel = 3;
    break;
  }
  return msgTemplate[iLevel];
}



EnumFlags<MessageLevel> Console::sLevel = MessageLevel::msg_error;
std::mutex Console::mtx;

Console::Console()
#ifdef TL_MESSAGE_HANDLER
  : MessageManager::Listener(false)
#endif
{
#ifdef TL_OS_WINDOWS
  init(STD_OUTPUT_HANDLE);
#else
  init(stdout);
#endif
}

Console::~Console()
{
  reset();
}

#ifdef TL_ENABLE_DEPRECATED_METHODS

Console &Console::instance()
{
  static Console console;
  return console;
}
#endif // TL_ENABLE_DEPRECATED_METHODS

EnumFlags<MessageLevel> Console::messageLevel()
{
  return sLevel;
}

void Console::setMessageLevel(MessageLevel level)
{
  sLevel = level;
}

#if CPP_VERSION >= 17
void Console::printMessage(std::string_view message)
#else
void Console::printMessage(const std::string &message)
#endif
{
  std::lock_guard<std::mutex> lck(Console::mtx);

  // Por si esta corriendo la barra de progreso
  if (Progress::isRunning()) {
    std::cout << "\r" << std::string(50, ' ') << "\r";
  }

  //std::string aux(message);
  //replaceString(&aux, "%", "%%");
  std::cout << /*aux*/message << std::endl;
}

#if CPP_VERSION >= 17
void Console::printErrorMessage(std::string_view message)
#else
void Console::printErrorMessage(const std::string &message)
#endif
{
  std::lock_guard<std::mutex> lck(Console::mtx);

  setConsoleForegroundColor(messageProperties(MessageLevel::msg_error).foregroundColor,
                            messageProperties(MessageLevel::msg_error).intensity);

  // Por si esta corriendo la barra de progreso
  if (Progress::isRunning())
    std::cout << "\r" << std::string(50, ' ') << "\r";

  //std::string aux(message);
  //replaceString(&aux, "%", "%%");
  std::cerr << /*aux*/message << std::endl;

  reset();
}

void Console::reset()
{
#ifdef TL_OS_WINDOWS
  mForegroundColor = (mOldColorAttrs & 0x0007);
  mForegroundIntensity = (mOldColorAttrs & 0x0008);
  mBackgroundColor = (mOldColorAttrs & 0x0070);
  mBackgroundIntensity = (mOldColorAttrs & 0x0080);
  update();
#else
  sprintf(mCommand, "%c[0;m", 0x1B);
  fprintf(mStream, "%s", mCommand);
#endif
}

void Console::setConsoleBackgroundColor(Console::Color backgroundColor,
                                        Console::Intensity intensity)
{
#ifdef TL_OS_WINDOWS
    switch(backgroundColor) {
        case tl::Console::Color::black:
            mBackgroundColor = 0;
            break;
        case tl::Console::Color::blue:
            mBackgroundColor = BACKGROUND_BLUE;
            break;
        case tl::Console::Color::green:
            mBackgroundColor = BACKGROUND_GREEN;
            break;
        case tl::Console::Color::cyan:
            mBackgroundColor = BACKGROUND_GREEN | BACKGROUND_BLUE;
            break;
        case tl::Console::Color::red:
            mBackgroundColor = BACKGROUND_RED;
            break;
        case tl::Console::Color::magenta:
            mBackgroundColor = BACKGROUND_RED | BACKGROUND_BLUE;
            break;
        case tl::Console::Color::yellow:
            mBackgroundColor = BACKGROUND_GREEN | BACKGROUND_RED;
            break;
        case tl::Console::Color::white:
            mBackgroundColor = BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED;
            break;
        default:
            mBackgroundColor = 0;
            break;
    }
 
    if(intensity == Console::Intensity::normal)
        mBackgroundIntensity = 0;
    else
        mBackgroundIntensity = BACKGROUND_INTENSITY;
#else
    mBackgroundColor = static_cast<int>(backgroundColor) + 40 + static_cast<int>(intensity) * 60;
#endif
    update();
}

void Console::setConsoleForegroundColor(Console::Color foregroundColor,
                                        Console::Intensity intensity)
{
#ifdef TL_OS_WINDOWS
    switch(foregroundColor) {
        case tl::Console::Color::black:
            mForegroundColor = 0;
            break;
        case tl::Console::Color::blue:
            mForegroundColor = FOREGROUND_BLUE;
            break;
        case tl::Console::Color::green:
            mForegroundColor = FOREGROUND_GREEN;
            break;
        case tl::Console::Color::cyan:
            mForegroundColor = FOREGROUND_GREEN | FOREGROUND_BLUE;
            break;
        case tl::Console::Color::red:
            mForegroundColor = FOREGROUND_RED;
            break;
        case tl::Console::Color::magenta:
            mForegroundColor = FOREGROUND_RED | FOREGROUND_BLUE;
            break;
        case tl::Console::Color::yellow:
            mForegroundColor = FOREGROUND_GREEN | FOREGROUND_RED;
            break;
        case tl::Console::Color::white:
            mForegroundColor = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED;
            break;
        default:
            mForegroundColor = 0;
            break;
    }

    if(intensity == Console::Intensity::normal)
        mForegroundIntensity = 0;
    else
        mForegroundIntensity = FOREGROUND_INTENSITY;
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

void Console::setFontBold(bool bBold)
{
#ifdef TL_OS_WINDOWS
    mCurrentFont.FontWeight = bBold ? FW_BOLD : FW_NORMAL;
#else
    mBold = bBold ? 1 : 21
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

void Console::setTitle(const std::string &title)
{
#ifdef TL_OS_WINDOWS
    SetConsoleTitleA(title.c_str());
#else
    unusedParameter(title);
    //printf("%c]0;%s%c", '\033', title, '\007');
#endif
}

#ifdef TL_MESSAGE_HANDLER

#if CPP_VERSION >= 17
void Console::onMsgDebug(std::string_view message, 
                         std::string_view date)
#else
void Console::onMsgDebug(const std::string &message,
                         const std::string &date)
#endif
{
    unusedParameter(date);

    if(sLevel.isEnabled(MessageLevel::msg_debug)) {
        setConsoleForegroundColor(messageProperties(MessageLevel::msg_debug).foregroundColor,
                                  messageProperties(MessageLevel::msg_debug).intensity);
        printMessage(message);
        reset();
    }
}

#if CPP_VERSION >= 17
void Console::onMsgInfo(std::string_view message, 
                        std::string_view date)
#else
void Console::onMsgInfo(const std::string &message,
                        const std::string &date)
#endif
{
    unusedParameter(date);

    if(sLevel.isEnabled(MessageLevel::msg_info)) {
        setConsoleForegroundColor(messageProperties(MessageLevel::msg_info).foregroundColor,
                                  messageProperties(MessageLevel::msg_info).intensity);
        printMessage(message);
        reset();
    }
}

#if CPP_VERSION >= 17
void Console::onMsgWarning(std::string_view message, 
                           std::string_view date)
#else
void Console::onMsgWarning(const std::string &message,
                           const std::string &date)
#endif
{
    unusedParameter(date);

    if(sLevel.isEnabled(MessageLevel::msg_warning)) {
        setConsoleForegroundColor(messageProperties(MessageLevel::msg_warning).foregroundColor,
                                  messageProperties(MessageLevel::msg_warning).intensity);
        printMessage(message);
        reset();
    }
}

#if CPP_VERSION >= 17
void Console::onMsgError(std::string_view message, 
                         std::string_view date)
#else
void Console::onMsgError(const std::string &message,
                         const std::string &date)
#endif
{
    unusedParameter(date);

    if(sLevel.isEnabled(MessageLevel::msg_error)) {
        printErrorMessage(message);
    }
}

#endif // TL_MESSAGE_HANDLER

#ifdef TL_OS_WINDOWS
void Console::init(DWORD handle)
{
    setConsoleUnicode();
    mHandle = GetStdHandle(handle);
    CONSOLE_SCREEN_BUFFER_INFO info;
    if(!GetConsoleScreenBufferInfo(mHandle, &info)) {
        mOldColorAttrs = 0x0007;
    } else {
        mOldColorAttrs = info.wAttributes;
    }

    mForegroundColor = (mOldColorAttrs & 0x0007);
    mForegroundIntensity = (mOldColorAttrs & 0x0008);
    mBackgroundColor = (mOldColorAttrs & 0x0070);
    mBackgroundIntensity = (mOldColorAttrs & 0x0080);

    mIniFont.cbSize = sizeof(mIniFont);
    GetCurrentConsoleFontEx(mHandle, FALSE, &mIniFont);
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

void Console::update()
{
#ifdef TL_OS_WINDOWS
    SetConsoleTextAttribute(mHandle, mForegroundColor | mBackgroundColor | mForegroundIntensity | mBackgroundIntensity);
    SetCurrentConsoleFontEx(mHandle, FALSE, &mCurrentFont);
#else
    std::stringstream ss;
    ss << "\x1B[" << mBold;
    if(mForegroundColor != 0)
        ss << ";" << mForegroundColor;
    if(mBackgroundColor != 0)
        ss << ";" << mBackgroundColor;
    ss << "m";
    fprintf(mStream, "%s", ss.str().c_str());
#endif
}


















std::mutex Console2::mtx;

Console2::Console2(std::ostream &outstream)
  : _stream(outstream) 
{
#ifdef TL_OS_WINDOWS
    init(STD_OUTPUT_HANDLE);
#else
    init(stdout);
#endif
}

Console2 &Console2::instance()
{
    static Console2 console;
    return console;
}

void Console2::setTitle(const std::string &title)
{
#ifdef TL_OS_WINDOWS
    SetConsoleTitleA(title.c_str());
#else
    unusedParameter(title);
    //printf("%c]0;%s%c", '\033', title, '\007');
#endif
}

void Console2::setBackgroundColor(Color backgroundColor,
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


void Console2::setForegroundColor(Color foregroundColor,
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

void Console2::setConsoleUnicode()
{
#ifdef TL_OS_WINDOWS
    //SetConsoleOutputCP(1252);
    //SetConsoleCP(1252);
    SetConsoleOutputCP(CP_UTF8/*65001*/);
#endif
}

void Console2::setFontBold(bool bold)
{
#ifdef TL_OS_WINDOWS
    mCurrentFont.FontWeight = bold ? FW_BOLD : FW_NORMAL;
#else
    mBold = bold ? 1 : 21
#endif
    update();
}

void Console2::setFontHeight(int16_t fontHeight)
{
#ifdef TL_OS_WINDOWS
    mCurrentFont.dwFontSize.Y = static_cast<SHORT>(fontHeight);
#else
    unusedParameter(fontHeight);
#endif
    update();
}

void Console2::reset()
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

Console2 &Console2::operator <<(Level level)
{
    switch(level) {
    case Level::debug:
        _stream << "Debug:   ";
        break;
    case Level::info:
        _stream << "Info:    ";
        break;
    case Level::warning:
        setForegroundColor(Color::magenta, Intensity::normal);
        _stream << "Warning: ";
        break;
    case Level::error:
        setForegroundColor(Color::red, Intensity::normal);
        _stream << "Error:   ";
        break;
    }

    return *this;
}

Console2 &Console2::operator <<(decltype(std::endl<char, std::char_traits<char>>) _endl)
{
    _stream << _endl;
    reset();
    return *this;
}

Console2 &Console2::debug()
{
    auto &console = Console2::instance();
    console << Level::debug;
    return console;
}

Console2 &Console2::info()
{
    auto &console = Console2::instance();
    console << Level::info;
    return console;
}

Console2 &Console2::warning()
{
    auto &console = Console2::instance();
    console << Level::warning;
    return console;
}

Console2 &Console2::error()
{
    auto &console = Console2::instance();
    console << Level::error;
    return console;
}

//Console2 &Console2::clear()
//{
//    auto &console = Console2::instance();
//    console.reset();
//    return console;
//}

#if CPP_VERSION >= 17
void Console2::debug(std::string_view message)
#else
void Console2::debug(const std::string &message)
#endif
{
    std::lock_guard<std::mutex> lck(Console2::mtx);

    Console2::instance() << Level::debug << message << std::endl;
}

#if CPP_VERSION >= 17
void Console2::info(std::string_view message)
#else
void Console2::info(const std::string &message)
#endif
{
    std::lock_guard<std::mutex> lck(Console2::mtx);

    Console2::instance() << Level::info << message << std::endl;
}

#if CPP_VERSION >= 17
void Console2::warning(std::string_view message)
#else
void Console2::warning(const std::string &message)
#endif
{
    std::lock_guard<std::mutex> lck(Console2::mtx);

    Console2::instance() << Level::warning << message << std::endl;
}

#if CPP_VERSION >= 17
void Console2::error(std::string_view message)
#else
void Console2::error(const std::string &message)
#endif
{
    std::lock_guard<std::mutex> lck(Console2::mtx);

    Console2::instance() << Level::error << message << std::endl;
}

} // End mamespace tl


