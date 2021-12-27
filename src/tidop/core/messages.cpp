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
 * License along with Foobar. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#include "tidop/core/messages.h"

#include "tidop/core/defs.h"
#include "tidop/core/utils.h"
#include "tidop/core/chrono.h"

#include <cstdarg>
#if defined WIN32
# include <windows.h>
#endif
#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstring>


namespace tl
{

#ifdef TL_MESSAGE_HANDLER


static struct _msgProperties _msgTemplate[] = {   
  { "Debug:   %s", "Debug:   %s (%s:%u, %s)"},
  { "Info:    %s", "Info:    %s (%s:%u, %s)"},
  { "Warning: %s", "Warning: %s (%s:%u, %s)"},
  { "Error:   %s", "Error:   %s (%s:%u, %s)"}
};


std::unique_ptr<MessageManager> MessageManager::sObjMessage;
bool MessageManager::sStopHandler = false;
std::mutex MessageManager::sMutex;

std::string MessageManager::Message::sTimeLogFormat = "%d/%b/%Y %H:%M:%S";

MessageManager::MessageManager() 
  : mListeners(0)
{
}

MessageManager::~MessageManager()
{
  sObjMessage.release();
}

MessageManager &MessageManager::instance()
{
  if (sObjMessage == nullptr) {
    std::lock_guard<std::mutex> lck(MessageManager::sMutex);
    if (sObjMessage == nullptr) {
      sObjMessage.reset(new MessageManager());
    }
  }
  return *sObjMessage;
}

void MessageManager::addListener(Listener *listener)
{ 
  // Se comprueba que no este añadido
  bool bAdded = false;
  for (auto l : mListeners) {
    if (l == listener){
      bAdded = true;
      break;
    }
  }
  if (!bAdded)
    mListeners.push_back(listener);
}

void MessageManager::pause()
{
  sStopHandler = true;
}

void MessageManager::release(const std::string &message, 
                             const MessageLevel &level, 
                             const std::string &file, 
                             int line, 
                             const std::string &function)
{
  MessageManager::instance();

  if (sStopHandler) return;

  std::lock_guard<std::mutex> lck(MessageManager::sMutex);

  std::string date = formatTimeToString("%d/%b/%Y %H:%M:%S");

  char buf[1000];
  #if defined _MSC_VER
    if (line == -1)
      sprintf_s(buf, 1000, messageProperties(level).normal, message.c_str(), file.c_str(), line, function.c_str());
    else
      sprintf_s(buf, 1000, messageProperties(level).extend, message.c_str(), file.c_str(), line, function.c_str());
  #else
    if (line == -1)
      snprintf(buf, 1000, messageProperties(level).normal, message.c_str(), file.c_str(), line, function.c_str());
    else
      snprintf(buf, 1000, messageProperties(level).extend, message.c_str(), file.c_str(), line, function.c_str());
  #endif

  switch (level) {
  case MessageLevel::msg_debug:
    sObjMessage->onDebug(buf, date);
    break;
  case MessageLevel::msg_info:
    sObjMessage->onInfo(buf, date);
    break;
  case MessageLevel::msg_warning:
    sObjMessage->onWarning(buf, date);
    break;
  case MessageLevel::msg_error:
    sObjMessage->onError(buf, date);
    break;
  default:
    break;
  }
}

void MessageManager::release(const Message &message)
{
  MessageManager::release(message.message(),
                          message.level(),
                          message.file(),
                          message.line(),
                          message.function());
}

void MessageManager::removeListener(Listener *listener)
{
  if (!mListeners.empty()) {
    mListeners.remove(listener);
  }
}

void MessageManager::resume()
{
  sStopHandler = false;
}

TL_DISABLE_WARNING(TL_UNREFERENCED_FORMAL_PARAMETER)
void MessageManager::onDebug(const std::string &message, const std::string &date)
{
#ifdef _DEBUG
  std::list<Listener *> listeners = mListeners;
  if (!sStopHandler && !listeners.empty()) {
    for (auto &lst : listeners) {
      lst->onMsgDebug(message, date);
    }
  }
#endif
}
TL_ENABLE_WARNING(TL_UNREFERENCED_FORMAL_PARAMETER)

void MessageManager::onInfo(const std::string &message, const std::string &date)
{
  std::list<Listener *> listeners = mListeners;
  if (!sStopHandler && !listeners.empty()) {
    for (auto &lst : listeners) {
      lst->onMsgInfo(message, date);
    }
  }
}

void MessageManager::onWarning(const std::string &message, const std::string &date)
{
  std::list<Listener *> listeners = mListeners;
  if (!sStopHandler && !listeners.empty()) {
    for (auto &lst : listeners) {
      lst->onMsgWarning(message, date);
    }
  }
}

void MessageManager::onError(const std::string &message, const std::string &date)
{
  std::list<Listener *> listeners = mListeners;
  if (!sStopHandler && !listeners.empty()) {
    for (auto &lst : listeners) {
      lst->onMsgError(message, date);
    }
  }
}

_msgProperties MessageManager::messageProperties(MessageLevel msgLevel)
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
  return _msgTemplate[iLevel];
}

/* ---------------------------------------------------------------------------------- */

MessageManager::Listener::Listener(bool add)
{
  if (add) {
    MessageManager::instance().addListener(this);
  }
}

MessageManager::Listener::~Listener()
{
  MessageManager::instance().removeListener(this);
}

/* ---------------------------------------------------------------------------------- */


MessageManager::Message::Message(const char *message, ...)
  : mLevel(MessageLevel::msg_error),
    mFile(""), 
    mLine(-1),
    mFunction("")
{
  TL_TODO("revisar")
  try {

    mDate = formatTimeToString("%d/%b/%Y %H:%M:%S");

    char buf[500];
    memset(buf, 0, sizeof(buf));
    std::string aux(message);
    replaceString(&aux, "% ", "%% ");
    replaceString(&aux, "%(\s)", "%%");
    va_list args;
    va_start(args, message);
#ifdef _MSC_VER
  vsnprintf_s(buf, _countof(buf), _TRUNCATE, aux.c_str(), args);
#else
  vsnprintf(buf, sizeof(buf), aux.c_str(), args);
#endif
    va_end(args);
    mMessage = buf;
  } catch (...) {

    // Por evitar un error en la constructora... 
  }
}

std::string MessageManager::Message::date() const
{
  return mDate;
}

std::string MessageManager::Message::file() const
{
  return mFile;
}

std::string MessageManager::Message::function() const
{
  return mFunction;
}

MessageLevel MessageManager::Message::level() const
{
  return mLevel;
}

int MessageManager::Message::line() const
{
  return mLine;
}

std::string MessageManager::Message::message() const
{
  return mMessage;
}

void MessageManager::Message::setTimeLogFormat(const std::string &timeTemplate)
{
  sTimeLogFormat = timeTemplate;
}

void MessageManager::Message::setMessageLevel(const MessageLevel &level)
{
  mLevel = level;
}
    
void MessageManager::Message::setMessageProperties(const MessageLevel &level, 
                                                   const std::string &file, 
                                                   int line, 
                                                   const std::string &function)
{
  mLevel = level;
  mLine = line;
  mFile = file;
  mFunction = function;
}

#endif  // TL_MESSAGE_HANDLER

#ifdef TL_ENABLE_DEPRECATED_METHODS

MessageManager &MessageManager::getInstance()
{
  if (sObjMessage.get() == nullptr) {
    std::lock_guard<std::mutex> lck(MessageManager::sMutex);
    if (sObjMessage.get() == nullptr) {
      sObjMessage.reset(new MessageManager());
    }
  }
  return *sObjMessage;
}

const char *MessageManager::Message::getDate() const
{
  return mDate.c_str();
}

const char *MessageManager::Message::getFile() const
{
  return mFile.c_str();
}

const char *MessageManager::Message::getFunction() const
{
  return mFunction.c_str();
}

MessageLevel MessageManager::Message::getLevel() const
{
  return mLevel;
}

int MessageManager::Message::getLine() const
{
  return mLine;
}

const char *MessageManager::Message::getMessage() const
{
  return mMessage.c_str();
}
#endif // TL_ENABLE_DEPRECATED_METHODS

} // End mamespace tl
