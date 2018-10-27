/****************************************************************************
 *                                                                          *
 *  This file is part of TidopLib and can not be copied and/or distributed  *
 *  without the express permision of ITOS3D ENGINEERING S.L                 *
 *                                                                          *
 *  Contact: http://www.itos3d.com                                          *
 *           http://tidop.usal.es                                           *
 *                                                                          *
 *--------------------------------------------------------------------------*
 *                                                                          *
 *  Copyright (C) 2018, ITOS3D ENGINEERING S.L - All rights reserved        *
 *                                                                          *
 ****************************************************************************/


#include "tidop/core/log.h"
#include "tidop/core/messages.h"


#if (__cplusplus >= 201703L)
//C++17
//http://en.cppreference.com/w/cpp/filesystem
#include <filesystem>
namespace fs = std::filesystem;
#elif defined HAVE_BOOST
//Boost
//http://www.boost.org/doc/libs/1_66_0/libs/filesystem/doc/index.htm
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#endif


namespace tl
{

std::unique_ptr<Log> Log::sObjLog;
std::string Log::sLogFile = "";
EnumFlags<MessageLevel> Log::sLevel = MessageLevel::MSG_ERROR;
std::string Log::sTimeLogFormat = "%d/%b/%Y %H:%M:%S";
std::mutex Log::mtx;

Log::Log()
#ifdef TL_MESSAGE_HANDLER
  : MessageManager::Listener(false)
#endif
{
}

Log::~Log()
{
  sObjLog.release();
}

Log &Log::getInstance()
{
  if (sObjLog.get() == nullptr) {
    std::lock_guard<std::mutex> lck(Log::mtx);
    if (sObjLog.get() == nullptr) {
      sObjLog.reset(new Log());
    }
  }
  return *sObjLog;
}

EnumFlags<MessageLevel> Log::getLogLevel() const
{
  return sLevel;
}

void Log::setLogFile(const char* file)
{
  TL_TODO("Se tiene que comprobar si existe el directorio e intentar crearlo en caso contrario")
  TL_TODO("Comprobar si tiene permisos de escritura")
  sLogFile = file;
}

void Log::setLogLevel(MessageLevel level)
{
  sLevel = level;
}

void Log::write(const char *msg)
{

  std::string date = formatTimeToString("%d/%b/%Y %H:%M:%S");

  if (sLogFile.empty()) {
    // Log por defecto
    fs::path logPath(getRunfile());
    logPath.replace_extension(".log");
    sLogFile = logPath.string();
  }
  std::ofstream hLog(sLogFile,std::ofstream::app);
  if (hLog.is_open()) {
    std::lock_guard<std::mutex> lck(Log::mtx);
    hLog << date << " - " << msg << "\n";
    hLog.close();
  } else {
    msgError("Permission denied: %s", sLogFile.c_str());
  }
}

#ifdef TL_MESSAGE_HANDLER

void Log::onMsgDebug(const char *msg, const char *date)
{
  if (sLevel.isActive(MessageLevel::MSG_DEBUG)) {
    _write(msg, date);
  }
}

void Log::onMsgInfo(const char *msg, const char *date)
{
  if (sLevel.isActive(MessageLevel::MSG_INFO)) {
    _write(msg, date);
  }
}

void Log::onMsgWarning(const char *msg, const char *date)
{
  if (sLevel.isActive(MessageLevel::MSG_WARNING)) {
    _write(msg, date);
  }
}

void Log::onMsgError(const char *msg, const char *date)
{
  if (sLevel.isActive(MessageLevel::MSG_ERROR)) {
    _write(msg, date);
  }
}

void Log::_write(const char *msg, const char *date)
{
  TL_TODO("Permitir que se pueda pausar la escritura de los mensajes en el log")
  if (sLogFile.empty()) {
    // Log por defecto
    fs::path logPath(getRunfile());
    logPath.replace_extension(".log");
    sLogFile = logPath.string();
  }
  std::ofstream hLog(sLogFile,std::ofstream::app);
  if (hLog.is_open()) {
    std::lock_guard<std::mutex> lck(Log::mtx);
    hLog << date << " - " << msg << "\n";
    hLog.close();
  } else {
    //Error al abrir/crear archivo. Se saca el error por consola
    printf("The file %s was not opened\n", sLogFile.c_str());
  }
}


#endif // TL_MESSAGE_HANDLER

} // End mamespace tl
