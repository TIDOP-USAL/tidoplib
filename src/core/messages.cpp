#include <cstdarg>
#if defined WIN32
# include <windows.h>
#endif
#include <fstream>

#include "messages.h"
#include "utils.h"

#ifdef I3D_MESSAGE_HANDLER

using namespace I3D;

std::mutex mtx;

struct msgProperties {
  const char *normal;
  const char *extend;
  unsigned short foreColor;
};

struct msgProperties msgTemplate[] = {   
  { "Debug: %s",   "Debug: %s (%s:%u, %s)", FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED},
  { "Verbose: %s", "Verbose: %s (%s:%u, %s)", FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED},
  { "Info: %s",    "Info: %s (%s:%u, %s)", FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED},
  { "Warning: %s", "Warning: %s (%s:%u, %s)", FOREGROUND_GREEN | FOREGROUND_RED },
  { "Error: %s",   "Error: %s (%s:%u, %s)", FOREGROUND_RED },
};

msgProperties GetMessageProperties( MessageLevel msgLevel ) 
{
  return msgTemplate[static_cast<int>(msgLevel)];
}

char *_msg(const char *msg, ...)
{
  static char buf[500];
  memset(buf, 0, sizeof(buf));
  std::string aux(msg);
  replaceString(&aux, "% ", "%% " );
  va_list args;
  va_start(args, msg);
  vsnprintf_s(buf, _countof(buf), _TRUNCATE, aux.c_str(), args);
  va_end(args);
  return buf;
}

namespace I3D
{


// inicialización de miembros estáticos

MessageLevel Message::sLevel = MessageLevel::MSG_ERROR;
EnumFlags<MessageOutput> Message::sOutput = MessageOutput::MSG_CONSOLE;
std::string Message::sLastMessage = "";
std::unique_ptr<Message> Message::sObjMessage;
std::string Message::sLogFile = "";
std::string Message::sTimeLogFormat = "%d/%b/%Y %H:%M:%S";

Message &Message::get()
{
  if (sObjMessage.get() == 0) {
    sObjMessage.reset(new Message());
  }
  return *sObjMessage;
}

Message &Message::message(const char *msg, ...)
{
  if (sObjMessage.get() == 0) {
    sObjMessage.reset(new Message());
  }
  char buf[500];
  memset(buf, 0, sizeof(buf));
  std::string aux(msg);
  I3D::replaceString(&aux, "% ", "%% ");
  va_list args;
  va_start(args, msg);
  vsnprintf_s(buf, _countof(buf), _TRUNCATE, aux.c_str(), args);
  va_end(args);
  sLastMessage = buf;
  return *sObjMessage;
}

void Message::print()
{
  print(sLevel, sOutput.getFlags());
}

void Message::print(const MessageLevel &level, const MessageOutput &output)
{
  if (sLevel <= level) 
    Message::_print( level, output, messageOutput(level) );
}

void Message::print(const MessageLevel &level, const MessageOutput &output, const char *file, int line, const char *function)
{
  if (sLevel <= level) 
    Message::_print( level, output, messageOutput(level, file, line, function) );
}


void Message::setMessageLevel(const MessageLevel &level)
{
  sLevel = level;
}

void Message::setMessageOutput(const MessageOutput &output)
{
  sOutput = output;
}

void Message::setTimeLogFormat( const std::string timeTemplate)
{
  sTimeLogFormat = timeTemplate;
}


MessageLevel Message::getMessageLevel() const
{
  return sLevel;
}



EnumFlags<MessageOutput> Message::getMessageOutput() const
{
  return sOutput;
}

std::string Message::messageOutput(const MessageLevel &msgLevel)
{
  char buf[500];
  sprintf_s(buf, 500, GetMessageProperties(msgLevel).normal, sLastMessage.c_str());
  return std::string(buf);
}

std::string Message::messageOutput(const MessageLevel &msgLevel, const char *file, int line, const char *function)
{
  char buf[500];
  sprintf_s(buf, 500, GetMessageProperties(msgLevel).extend, sLastMessage.c_str(), file, line, function);
  return std::string(buf);
}

void Message::_print(const MessageLevel &level, const MessageOutput &output, const std::string &msgOut)
{
  EnumFlags<MessageOutput> flag(output);
#ifdef  I3D_ENABLE_CONSOLE
  if (flag.isActive( MessageOutput::MSG_CONSOLE ) ) {
#ifdef WIN32
    HANDLE h = GetStdHandle((level == MessageLevel::MSG_ERROR) ? STD_ERROR_HANDLE : STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, GetMessageProperties(level).foreColor);
#else
    //char command[13];
    //// Command is the control command to the terminal
    //sprintf (command, "%c[%d;%dm", 0x1B, attribute, fg + 30);
    //fprintf (stream, "%s", command);
#endif
    printf_s(msgOut.c_str());
    printf_s("\n");
#ifdef WIN32
    SetConsoleTextAttribute(h, 0 | 0 | 0);
#else
    //char command[13];
    //// Command is the control command to the terminal
    //sprintf (command, "%c[%d;%dm", 0x1B, attribute, fg + 30);
    //fprintf (stream, "%s", command);
#endif
  }

#endif //I3D_ENABLE_CONSOLE


  if (flag.isActive( MessageOutput::MSG_LOG )) {
    if (sLogFile.empty()) {
      // Log por defecto
      char _logfile[_MAX_PATH];
      changeFileExtension(getRunfile(), "log", _logfile);
      sLogFile = _logfile;
    }
    std::ofstream hLog(sLogFile,std::ofstream::app);
    if (hLog.is_open()) {
      errno_t err;
      char date[64];
      time_t now;
      now = time(NULL);
      struct tm _tm;
      std::lock_guard<std::mutex> lck(mtx);
      err = localtime_s(&_tm, &now);
      if (err == 0) {
        strftime(date, sizeof(date), "%d/%b/%Y %H:%M:%S", &_tm);
      } else {
        strcpy_s(date, "NULL");
      }
      hLog << date << " - " << msgOut << "\n";
      hLog.close();
    } else {
      //Error al abrir/crear archivo. Se saca el error por consola
      Message::message("The file %s was not opened\n", sLogFile.c_str()).print(MessageLevel::MSG_ERROR, MessageOutput::MSG_CONSOLE);
    }
  }
  return;
}

} // End mamespace I3D


#endif  // I3D_MESSAGE_HANDLER