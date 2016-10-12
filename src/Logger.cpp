#include "Logger.h"

#include <cstdio>
#include <ctime>
#include <cstdarg>


#include "utils.h"

namespace I3D
{

#define lockfile(x)      ((void) (x))
#define unlockfile(x)    ((void) (x))

std::string LogMsg::logFile = "msg.log";
LogLevel LogMsg::logLevel = LogLevel::LOG_INFO;

LogMsg::LogMsg(const char *filename, LogLevel _ll)
{
  logFile = filename;
  logLevel = _ll;
  init();
}

LogMsg::~LogMsg()
{
}

void LogMsg::write(const char *logline, LogLevel type)
{
  if (logFile.empty() || logLevel > type) return;
  FILE *fp;
  errno_t err;
  err = fopen_s(&fp, logFile.c_str(), "a+");
  if( err != 0 ) {
    printf( "The file %s was not opened\n", logFile.c_str() );
    printf( logline );
    return;
  }
  char date[64];
  time_t now;
  now = time(NULL);
  struct tm _tm;
  err = localtime_s( &_tm, &now );
  if ( err == 0 ) {
    strftime(date, sizeof(date), "%d/%b/%Y %H:%M:%S", &_tm);
  } else {
    strcpy_s(date,"NULL");
  }

  lockfile(fp);

  fprintf(fp, "%s - %s", date, logline);
  fputc('\n', fp);
  fflush(fp);

  unlockfile(fp);
  fclose(fp);

  if (logLevel <= type) {

  }
}

//void LogMsg::Write(std::exception &e, char *msg, LogLevel type)
//{
//  //if (I3D_Log::logfile)
//  //char *logline;
//  //Write(logline, type);
//}


//#if defined( __QT_PRJ__ )

//#if ( QT_VERSION < 0x050000 )
//void MessageOutput(QtMsgType type, const char *msg)
//{
//  LogLevel ll;

//  char buf[500];
//  if (type == QtDebugMsg) {
//    ll = LOG_DEBUG;
//    sprintf_s(buf, 500, "Debug: %s", msg);
//  } /*else if (type == QtInfoMsg) {
//    ll = LOG_INFO;
//    sprintf(buf, "Info: %s", localMsg.constData());
//  }*/ else if (type == QtWarningMsg) {
//    ll = LOG_WARNING;
//    sprintf_s(buf, 500, "Warning: %s", msg);
//  } else {
//    ll = LOG_ERROR;
//    sprintf_s(buf, 500, "Error: %s", msg);
//  }

//  LogMsg::Write(buf, ll);

//}
//#else
//void MessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
//{
//  LogLevel ll;
//  QByteArray localMsg = msg.toLocal8Bit();

//  char buf[500];
//  if (type == QtDebugMsg) {
//    ll = LOG_DEBUG;
//    sprintf_s(buf, 500, "Debug: %s", localMsg.constData());
//    //sprintf_s(buf, 500, "Debug: %s (%s:%u, %s)", localMsg.constData(), context.file, context.line, context.function);
//  } else if (type == QtInfoMsg) {
//    ll = LOG_INFO;
//    sprintf_s(buf, 500, "Info: %s", localMsg.constData());
//  } else if (type == QtWarningMsg) {
//    ll = LOG_WARNING;
//    sprintf_s(buf, 500, "Warning: %s", localMsg.constData());
//    //sprintf_s(buf, 500, "Warning: %s (%s:%u, %s)", localMsg.constData(), context.file, context.line, context.function);
//  } else {
//    ll = LOG_ERROR;
//    sprintf_s(buf, 500, "Error: %s", localMsg.constData());
//    //sprintf_s(buf, 500, "Error: %s (%s:%u, %s)", localMsg.constData(), context.file, context.line, context.function);
//  }

//  LogMsg::Write(buf, ll);

//}
//#endif

//#endif

void MessageOutput(LogLevel ll, const char *msg)
{
  char buf[500];
  if (ll == LogLevel::LOG_DEBUG) {
    sprintf_s(buf, 500, "Debug: %s", msg);
  } else if (ll == LogLevel::LOG_INFO) {
    sprintf_s(buf, 500, "Info: %s", msg);
  } else if (ll == LogLevel::LOG_WARNING) {
    sprintf_s(buf, 500, "Warning: %s", msg);
  } else {
    sprintf_s(buf, 500, "Error: %s", msg);
  }
  LogMsg::write(buf, ll);
}

void MessageOutput(LogLevel ll, const char *msg, const char *file, int line, const char *function)
{
  char buf[500];
  if (ll == LogLevel::LOG_DEBUG) {
    sprintf_s(buf, 500, "Debug: %s (%s:%u, %s)", msg, file, line, function);
  } else if (ll == LogLevel::LOG_INFO) {
    sprintf_s(buf, 500, "Info: %s (%s:%u, %s)", msg, file, line, function);
  } else if (ll == LogLevel::LOG_WARNING) {
    sprintf_s(buf, 500, "Warning: %s (%s:%u, %s)", msg, file, line, function);
  } else {
    sprintf_s(buf, 500, "Error: %s (%s:%u, %s)", msg, file, line, function);
  }
  LogMsg::write(buf, ll);
}

void LogMsg::init()
{
//#ifdef __QT_PRJ__
//#if ( QT_VERSION < 0x050000 )
//  qInstallMsgHandler(MessageOutput);
//#else
//  qInstallMessageHandler(MessageOutput);
//#endif
//#endif
}

void LogMsg::debugMsg(const char *msg, ...)
{
#ifdef _DEBUG
  char buf[500];
  memset(buf, 0, sizeof(buf));
  va_list args;
  va_start(args, msg);
  vsnprintf_s( buf, _countof(buf), _TRUNCATE, msg, args);
  va_end(args);
  MessageOutput(LogLevel::LOG_DEBUG, buf);
#endif
}


void LogMsg::debugMsg(const char *msg, const char *file, int line, const char *function)
{
#ifdef _DEBUG
  MessageOutput(LogLevel::LOG_DEBUG, msg, file, line, function);
#endif
}


void LogMsg::debugMsg(const char *file, int line, const char *function, char *msg, ...)
{
#ifdef _DEBUG
  char buf[500];
  memset(buf, 0, sizeof(buf));
  std::string aux(msg);
  I3D::replaceString(&aux, "% ", "%% ");
  va_list args;
  va_start(args, msg);
  vsnprintf_s(buf, _countof(buf), _TRUNCATE, aux.c_str(), args);
  va_end(args);
  MessageOutput(LogLevel::LOG_DEBUG, buf, file, line, function);
#endif
}

void LogMsg::infoMsg(const char *msg, ...)
{
  char buf[500];
  memset(buf, 0, sizeof(buf));
  va_list args;
  va_start(args, msg);
  vsnprintf_s( buf, _countof(buf), _TRUNCATE, msg, args);
  va_end(args);
  MessageOutput(LogLevel::LOG_INFO, buf);
}

void LogMsg::infoMsg(const char *file, int line, const char *function, char *msg, ...)
{
  char buf[500];
  memset(buf, 0, sizeof(buf));
  std::string aux(msg);
  I3D::replaceString(&aux, "% ", "%% ");
  va_list args;
  va_start(args, msg);
  vsnprintf_s(buf, _countof(buf), _TRUNCATE, aux.c_str(), args);
  va_end(args);
  MessageOutput(LogLevel::LOG_INFO, buf, file, line, function);
}

void LogMsg::infoMsg(const char *msg, const char *file, int line, const char *function)
{
  MessageOutput(LogLevel::LOG_INFO, msg, file, line, function);
}

void LogMsg::warningMsg(const char *msg, ...)
{
  char buf[500];
  memset(buf, 0, sizeof(buf));
  va_list args;
  va_start(args, msg);
  vsnprintf_s( buf, _countof(buf), _TRUNCATE, msg, args);
  va_end(args);
  MessageOutput(LogLevel::LOG_WARNING, buf);
}

void LogMsg::warningMsg(const char *file, int line, const char *function, char *msg, ...)
{
  char buf[500];
  memset(buf, 0, sizeof(buf));
  va_list args;
  va_start(args, msg);
  vsnprintf_s( buf, _countof(buf), _TRUNCATE, msg, args);
  va_end(args);
  MessageOutput(LogLevel::LOG_WARNING, buf, file, line, function);
}

void LogMsg::warningMsg(const char *msg, const char *file, int line, const char *function)
{
  MessageOutput(LogLevel::LOG_WARNING, msg, file, line, function);
}

void LogMsg::errorMsg(const char *msg, ...)
{
  char buf[500];
  memset(buf, 0, sizeof(buf));
  std::string aux(msg);
  I3D::replaceString(&aux, "% ", "%% ");
  va_list args;
  va_start(args, msg);
  vsnprintf_s(buf, _countof(buf), _TRUNCATE, aux.c_str(), args);
  va_end(args);
  MessageOutput(LogLevel::LOG_ERROR, buf);
}

void LogMsg::errorMsg(const char *msg, const char *file, int line, const char *function )
{
  MessageOutput(LogLevel::LOG_ERROR, msg, file, line, function);
}

void LogMsg::errorMsg(const char *file, int line, const char *function, char *msg, ...)
{
  char buf[500];
  memset(buf, 0, sizeof(buf));
  std::string aux(msg);
  I3D::replaceString(&aux, "% ", "%% ");
  va_list args;
  va_start(args, msg);
  vsnprintf_s(buf, _countof(buf), _TRUNCATE, aux.c_str(), args);
  va_end(args);
  MessageOutput(LogLevel::LOG_ERROR, buf, file, line, function);
}

char *_msg(const char *msg, ...)
{
  static char buf[500];
  memset(buf, 0, sizeof(buf));
  std::string aux(msg);
  I3D::replaceString(&aux, "% ", "%% " );
  va_list args;
  va_start(args, msg);
  vsnprintf_s(buf, _countof(buf), _TRUNCATE, aux.c_str(), args);
  va_end(args);
  return buf;
}

} // End namespace I3D
