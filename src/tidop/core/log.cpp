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
#include "tidop/core/messages.h"
#include "tidop/core/chrono.h"
#include "tidop/core/path.h"



namespace tl
{

std::string Log::sLogFile;
EnumFlags<MessageLevel> Log::sLevel = MessageLevel::msg_error;
std::string Log::sTimeLogFormat = "%d/%b/%Y %H:%M:%S";
std::mutex Log::mtx;

#ifdef TL_MESSAGE_HANDLER
bool Log::sPauseListener = false;
#endif

Log::Log()
#ifdef TL_MESSAGE_HANDLER
  : MessageManager::Listener(false)
#endif
{
}

Log::~Log() = default;

#ifdef TL_ENABLE_DEPRECATED_METHODS

Log &Log::instance()
{
  static Log log;
  return log;
}

#endif // TL_ENABLE_DEPRECATED_METHODS

EnumFlags<MessageLevel> Log::logLevel()
{
  return sLevel;
}

void Log::setMessageLevel(MessageLevel level)
{
  sLevel = level;
}
  
#ifdef TL_ENABLE_DEPRECATED_METHODS
void Log::setLogLevel(MessageLevel level)
{
  sLevel = level;
}
#endif

void Log::setLogFile(const std::string &file)
{
  sLogFile = file;
}

void Log::write(const std::string &message)
{

  std::string date = formatTimeToString("%d/%b/%Y %H:%M:%S");
  this->_write(message, date);
}

#ifdef TL_MESSAGE_HANDLER

void Log::pauseListener()
{
  sPauseListener = true;
}

void Log::resumeListener()
{
  sPauseListener = false;
}

#if CPP_VERSION >= 17
void Log::onMsgDebug(std::string_view message, 
                     std::string_view date)
#else
void Log::onMsgDebug(const std::string &message, 
                     const std::string &date)
#endif
{
  if (sLevel.isEnabled(MessageLevel::msg_debug)) {
    _write(message, date);
  }
}

#if CPP_VERSION >= 17
void Log::onMsgInfo(std::string_view message, 
                    std::string_view date)
#else
void Log::onMsgInfo(const std::string &message, 
                    const std::string &date) 
#endif
{
  if (sLevel.isEnabled(MessageLevel::msg_info)) {
    _write(message, date);
  }
}

#if CPP_VERSION >= 17
void Log::onMsgWarning(std::string_view message, 
                       std::string_view date)
#else
void Log::onMsgWarning(const std::string &message, 
                       const std::string &date)
#endif
{
  if (sLevel.isEnabled(MessageLevel::msg_warning)) {
    _write(message, date);
  }
}

#if CPP_VERSION >= 17
void Log::onMsgError(std::string_view message, 
                     std::string_view date)
#else
void Log::onMsgError(const std::string &message, 
                     const std::string &date)
#endif
{
  if (sLevel.isEnabled(MessageLevel::msg_error)) {
    _write(message, date);
  }
}

#if CPP_VERSION >= 17
void Log::_write(std::string_view message, 
                 std::string_view date)
#else

void Log::_write(const std::string &message, 
                 const std::string &date)
#endif
{
  if (sLogFile.empty()) {
    // Log por defecto
    Path log_path(App::instance().path());
    log_path.replaceExtension(".log");
    sLogFile = log_path.toString();
  }
  
  Path log_parent_path = Path(sLogFile).parentPath();
  if (!log_parent_path.exists()) {
    int err = log_parent_path.createDirectories();
    if (err == -1) {
      MessageManager::instance().removeListener(this);
      return;
    }
  }

  std::ofstream hLog(sLogFile,std::ofstream::app);
  if (hLog.is_open()) {
    std::lock_guard<std::mutex> lck(Log::mtx);
    hLog << date << " - " << message << "\n";
    hLog.close();
  } else {
    MessageManager::instance().removeListener(this);
  }
}


#endif // TL_MESSAGE_HANDLER



std::mutex Log2::mtx;
EnumFlags<MessageLevel> Log2::messageLevelFlags = MessageLevel::msg_all;

Log2 &Log2::instance()
{
    static Log2 log;
    return log;
}

void Log2::open(const std::string &file)
{
    _stream.open(file, std::ofstream::app);
}

void Log2::close()
{
    _stream.close();
}

bool Log2::isOpen() const
{
    return _stream.is_open();
}

Log2 &Log2::operator <<(Level level)
{
    switch (level) {
    case Level::debug:
        _stream << "Debug:   ";
        break;
    case Level::info:
        _stream << "Info:    ";
        break;
    case Level::succes:
        _stream << "Succes:  ";
        break;
    case Level::warning:
        _stream << "Warning: ";
        break;
    case Level::error:
        _stream << "Error:   ";
        break;
    }

    return *this;
}

Log2 &Log2::operator <<(decltype(std::endl<char, std::char_traits<char>>) _endl)
{
    _stream << _endl;
    return *this;
}

auto Log2::messageLevel() -> EnumFlags<MessageLevel>
{
    return messageLevelFlags;
}

void Log2::setMessageLevel(MessageLevel level)
{
    messageLevelFlags = level;
}

Log2 &Log2::debug()
{
    auto &log = Log2::instance();
    log << Level::debug;
    return log;
}

Log2 &Log2::info()
{
    auto &log = Log2::instance();
    log << Level::info;
    return log;
}

Log2 &Log2::succes()
{
    auto &log = Log2::instance();
    log << Level::succes;
    return log;
}

Log2 &Log2::warning()
{
    auto &log = Log2::instance();
    log << Level::warning;
    return log;
}

Log2 &Log2::error()
{
    auto &log = Log2::instance();
    log << Level::error;
    return log;
}

#if CPP_VERSION >= 17
void Log2::debug(std::string_view message)
#else
void Log2::debug(const std::string &message)
#endif
{
    std::lock_guard<std::mutex> lck(Log2::mtx);

    if (Log2::instance().isOpen() && messageLevelFlags.isEnabled(MessageLevel::msg_debug))
        Log2::instance() << Level::debug << message << std::endl;
}

#if CPP_VERSION >= 17
void Log2::info(std::string_view message)
#else
void Log2::info(const std::string &message)
#endif
{
    std::lock_guard<std::mutex> lck(Log2::mtx);

    if (Log2::instance().isOpen())
        Log2::instance() << Level::info << message << std::endl;
}

#if CPP_VERSION >= 17
void Log2::succes(std::string_view message)
#else
void Log2::succes(const std::string &message)
#endif
{
    std::lock_guard<std::mutex> lck(Log2::mtx);

    if (Log2::instance().isOpen())
        Log2::instance() << Level::succes << message << std::endl;
}

#if CPP_VERSION >= 17
void Log2::warning(std::string_view message)
#else
void Log2::warning(const std::string &message)
#endif
{
    std::lock_guard<std::mutex> lck(Log2::mtx);

    if (Log2::instance().isOpen()) 
        Log2::instance() << Level::warning << message << std::endl;
}

#if CPP_VERSION >= 17
void Log2::error(std::string_view message)
#else
void Log2::error(const std::string &message)
#endif
{
    std::lock_guard<std::mutex> lck(Log2::mtx);
    
    if (Log2::instance().isOpen())
        Log2::instance() << Level::error << message << std::endl;
}

} // End mamespace tl
