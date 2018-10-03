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

#include "tidop/core/console.h"

#include "config_tl.h"

#include "tidop/core/defs.h"
#include "tidop/core/utils.h"
#include "tidop/core/messages.h"

#include <iostream>
#include <ctime>
#include <cstdio>
#include <vector>

using namespace TL;

namespace TL
{

struct msgProperties {
  const char *normal;
  const char *extend;
  Console::Color foreColor;
  Console::Intensity intensity;
};

static struct msgProperties msgTemplate[] = {   
  { "Debug:   %s", "Debug:   %s (%s:%u, %s)", Console::Color::WHITE, Console::Intensity::NORMAL},
  { "Info:    %s", "Info:    %s (%s:%u, %s)", Console::Color::WHITE, Console::Intensity::BRIGHT},
  { "Warning: %s", "Warning: %s (%s:%u, %s)", Console::Color::MAGENTA, Console::Intensity::BRIGHT},
  { "Error:   %s", "Error:   %s (%s:%u, %s)", Console::Color::RED, Console::Intensity::BRIGHT}
};

msgProperties getMessageProperties( MessageLevel msgLevel ) 
{
  int iLevel = 0;
  switch (msgLevel) {
  case TL::MessageLevel::MSG_DEBUG:
    iLevel = 0;
    break;
  case TL::MessageLevel::MSG_INFO:
    iLevel = 1;
    break;
  case TL::MessageLevel::MSG_WARNING:
    iLevel = 2;
    break;
  case TL::MessageLevel::MSG_ERROR:
    iLevel = 3;
    break;
  default:
    iLevel = 3;
    break;
  }
  return msgTemplate[iLevel];
}



EnumFlags<MessageLevel> Console::sLevel = MessageLevel::MSG_ERROR;
std::unique_ptr<Console> Console::sObjConsole;
std::mutex Console::mtx;

Console::Console()
#ifdef TL_MESSAGE_HANDLER 
  : MessageManager::Listener(false)
#endif
{ 
#ifdef WIN32
  init(STD_OUTPUT_HANDLE);
#else
  init(stdout);
#endif
}

Console::~Console() 
{
  reset();
  sObjConsole.release();
}

Console &Console::getInstance()
{
  if (sObjConsole.get() == nullptr) {
    std::lock_guard<std::mutex> lck(Console::mtx);
    if (sObjConsole.get() == nullptr) {
      sObjConsole.reset(new Console());
    }
  }
  return *sObjConsole;
}

EnumFlags<MessageLevel> Console::getMessageLevel() const
{
  return sLevel;
}

void Console::printMessage(const char *msg)
{
  // Por si esta corriendo la barra de progreso
  std::cout << "\r";

  std::string aux(msg);
  TL::replaceString(&aux, "%", "%%");
  printf_s("%s\n", aux.c_str());
}

void Console::printErrorMessage(const char *msg)
{
  setConsoleForegroundColor(getMessageProperties(MessageLevel::MSG_ERROR).foreColor, 
                            getMessageProperties(MessageLevel::MSG_ERROR).intensity);
  printMessage(msg);
  reset();
}

void Console::reset() 
{
#ifdef WIN32
  SetConsoleTextAttribute(mHandle, mOldColorAttrs);
#else
  sprintf(mCommand, "%c[0;m", 0x1B);
  fprintf(mStream, "%s", mCommand);
#endif
}

void Console::setConsoleBackgroundColor(Console::Color backColor, Console::Intensity intensity)
{
#ifdef WIN32
  switch (backColor) {
  case TL::Console::Color::BLACK:
    mBackColor = 0;
    break;
  case TL::Console::Color::BLUE:
    mBackColor = BACKGROUND_BLUE;
    break;
  case TL::Console::Color::GREEN:
    mBackColor = BACKGROUND_GREEN;
    break;
  case TL::Console::Color::CYAN:
    mBackColor = BACKGROUND_GREEN | BACKGROUND_BLUE;
    break;
  case TL::Console::Color::RED:
    mBackColor = BACKGROUND_RED;
    break;
  case TL::Console::Color::MAGENTA:
    mBackColor = BACKGROUND_RED | BACKGROUND_BLUE;
    break;
  case TL::Console::Color::YELLOW:
    mBackColor = BACKGROUND_GREEN | BACKGROUND_RED;
    break;
  case TL::Console::Color::WHITE:
    mBackColor = BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED;
    break;
  default:
    mBackColor = 0;
    break;
  }
  if(intensity == Console::Intensity::NORMAL)
      mBackIntensity = 0;
  else
      mBackIntensity = BACKGROUND_INTENSITY;
#else
  mBackColor = static_cast<int>(backColor) + 40 + static_cast<int>(intensity) * 60;
#endif
  update();
}

void Console::setConsoleForegroundColor(Console::Color foreColor, Console::Intensity intensity)
      {
#ifdef WIN32
  switch (foreColor) {
  case TL::Console::Color::BLACK:
    mForeColor = 0;
    break;
  case TL::Console::Color::BLUE:
    mForeColor = FOREGROUND_BLUE;
    break;
  case TL::Console::Color::GREEN:
    mForeColor = FOREGROUND_GREEN;
    break;
  case TL::Console::Color::CYAN:
    mForeColor = FOREGROUND_GREEN | FOREGROUND_BLUE;
    break;
  case TL::Console::Color::RED:
    mForeColor = FOREGROUND_RED;
    break;
  case TL::Console::Color::MAGENTA:
    mForeColor = FOREGROUND_RED | FOREGROUND_BLUE;
    break;
  case TL::Console::Color::YELLOW:
    mForeColor = FOREGROUND_GREEN | FOREGROUND_RED;
    break;
  case TL::Console::Color::WHITE:
    mForeColor = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED;
    break;
  default:
    mForeColor = 0;
    break;
  }

  if(intensity == Console::Intensity::NORMAL)
      mForeIntensity = 0;
  else
      mForeIntensity = FOREGROUND_INTENSITY;
#else
  mForeColor = static_cast<int>(foreColor) + 30 + static_cast<int>(intensity) * 60;
#endif

  update();
}

void Console::setConsoleUnicode() 
{
#ifdef WIN32
  //SetConsoleOutputCP(1252);     
  //SetConsoleCP(1252);
  SetConsoleOutputCP(CP_UTF8/*65001*/);
#endif
}

void Console::setFontBold(bool bBold)
{
#ifdef WIN32
  if (bBold) {
    mCurrentFont.FontWeight = FW_BOLD;
  } else {
    mCurrentFont.FontWeight = FW_NORMAL;
  }
#else
  if (bBold) {
    mBold = 1;
  } else {
    mBold = 21;
  }
#endif
  update();
}

void Console::setFontHeight(int16_t size)
{
#ifdef WIN32
  mCurrentFont.dwFontSize.Y = static_cast<SHORT>(size);
#endif
  update();
}

void Console::setLogLevel(MessageLevel level)
{
  sLevel = level;
}

void Console::setTitle(const char *title)
{
#ifdef WIN32
  SetConsoleTitleA(title);
#else
  //printf("%c]0;%s%c", '\033', title, '\007');
#endif
}

#ifdef TL_MESSAGE_HANDLER 

TL_DISABLE_WARNING(TL_UNREFERENCED_FORMAL_PARAMETER)

void Console::onMsgDebug(const char *msg, const char *date)
{
  if (sLevel.isActive(MessageLevel::MSG_DEBUG)) {
    setConsoleForegroundColor(getMessageProperties(MessageLevel::MSG_DEBUG).foreColor, 
                              getMessageProperties(MessageLevel::MSG_DEBUG).intensity);
    printMessage(msg);
    reset();
  }
}

void Console::onMsgInfo(const char *msg, const char *date)
{
  if (sLevel.isActive(MessageLevel::MSG_INFO)) {
    setConsoleForegroundColor(getMessageProperties(MessageLevel::MSG_INFO).foreColor, 
                              getMessageProperties(MessageLevel::MSG_INFO).intensity);
    printMessage(msg);
    reset();
  }
}

void Console::onMsgWarning(const char *msg, const char *date)
{
  if (sLevel.isActive(MessageLevel::MSG_WARNING)) {
    setConsoleForegroundColor(getMessageProperties(MessageLevel::MSG_WARNING).foreColor, 
                              getMessageProperties(MessageLevel::MSG_WARNING).intensity);
    printMessage(msg);
    reset();
  }
}

void Console::onMsgError(const char *msg, const char *date)
{
  if (sLevel.isActive(MessageLevel::MSG_ERROR)) {
    printErrorMessage(msg);
  }
}

TL_ENABLE_WARNING(TL_UNREFERENCED_FORMAL_PARAMETER)

#endif // TL_MESSAGE_HANDLER 

#ifdef WIN32
void Console::init(DWORD handle) 
{
  setConsoleUnicode();
  mHandle = GetStdHandle(handle);
  CONSOLE_SCREEN_BUFFER_INFO info; 
  if (! GetConsoleScreenBufferInfo(mHandle, &info)) {
    mOldColorAttrs = 0x0007;
  } else {
    mOldColorAttrs = info.wAttributes; 
  }
  mForeColor = (mOldColorAttrs & 0x0007);
  mForeIntensity = (mOldColorAttrs & 0x0008);
  mBackColor = (mOldColorAttrs & 0x0070);
  mBackIntensity = (mOldColorAttrs & 0x0080);

  mIniFont.cbSize = sizeof(mIniFont);
  GetCurrentConsoleFontEx(mHandle, FALSE, &mIniFont);
  mCurrentFont.cbSize = sizeof(mCurrentFont);
  mCurrentFont = mIniFont;
  //COORD fontSize = GetConsoleFontSize(mHandle, mIniFont.nFont);
}
#else
void Console::init(FILE *stream)
{
  mStream = stream;
  mForeColor = 0;
  mBackColor = 0;
  mBold = 21;
}
#endif

void Console::update()
{
#ifdef WIN32
  SetConsoleTextAttribute(mHandle, mForeColor | mBackColor | mForeIntensity | mBackIntensity);
  SetCurrentConsoleFontEx(mHandle, FALSE, &mCurrentFont);
#else
  stringstream ss;
  ss << "\x1B[" << mBold;
  if (mForeColor != 0)
    ss << ";" << mForeColor;
  if (mBackColor != 0)
    ss << ";" << mBackColor;
  ss << "m";
  fprintf(mStream, "%s", ss.str().c_str());
#endif
}




/* ---------------------------------------------------------------------------------- */

Argument::Argument(const std::string &name, const std::string &description)
  : mName(name),
    mDescription(description),
    mShortName()
{

}

Argument::Argument(const char &shortName, const std::string &description)
  : mName(""),
    mDescription(description),
    mShortName(shortName)
{
}

Argument::Argument(const std::string &name, const char &shortName, const std::string &description)
  : mName(name),
    mDescription(description),
    mShortName(shortName)
{
}

Argument::Argument(const Argument &argument)
  : mName(argument.mName),
    mDescription(argument.mDescription),
    mShortName(argument.mShortName)
{
}

Argument::~Argument()
{
}

std::string Argument::description() const
{
  return mDescription;
}

void Argument::setDescription(const std::string &description)
{
  mDescription = description;
}

std::string Argument::name() const
{
  return mName;
}

void Argument::setName(const std::string &name)
{
  mName = name;
}

char Argument::shortName() const
{
  return mShortName;
}

void Argument::setShortName(const char &shortName)
{
  mShortName = shortName;
}

/* ---------------------------------------------------------------------------------- */

Command::Command()
  : mName(""),
    mDescription(""),
    mCmdArgs(0),
    mVersion("0.0.0")
{
    init();
}

Command::Command(const Command &command)
  : mName(command.mName),
    mDescription(command.mDescription),
    mCmdArgs(command.mCmdArgs),
    mVersion(command.mVersion)
{

}

Command::Command(const std::string &name, const std::string &description)
  : mName(name),
    mDescription(description),
    mCmdArgs(0),
    mVersion("0.0.0")
{
  init();
}
  
Command::Command(const std::string &name, const std::string &description,
                 std::initializer_list<std::shared_ptr<TL::Argument>> arguments)
  : mName(name),
    mDescription(description),
    mCmdArgs(arguments),
    mVersion("0.0.0")
{
}

std::string Command::name() const
{
  return mName;
}

void Command::setName(const std::string &name)
{
  mName = name;
}

std::string Command::description() const
{
  return mDescription;
}

void Command::setDescription(const std::string &description)
{
  mDescription = description;
}

std::string Command::version() const
{
    return mVersion;
}

void Command::setVersion(const std::string &version)
{
    mVersion = version;
}

Command::Status Command::parse(int argc, const char * const argv[])
{

  std::map<std::string, std::string> cmd_in;

  for (int i = 1; i < argc; ++i) {
    std::string arg_cmd_name = std::string(argv[i]);
    std::size_t found_name = arg_cmd_name.find("--");
    std::size_t found_short_name = arg_cmd_name.find("-");

    if (found_name != std::string::npos && found_name == 0) {
      arg_cmd_name = (argv[i])+2;
      /// argumento-valor separado por =
      std::vector<std::string> v;
      TL::split(arg_cmd_name, v, "=");
      if(v.size() == 2){
        cmd_in[v[0]] = v[1];
        continue;
      }
    } else if (found_short_name != std::string::npos && found_short_name == 0) {
      arg_cmd_name = (argv[i])+1;
      if (arg_cmd_name.size() > 1) {
        /// Se da el caso de combinación de multiples opciones o
        /// parametro corto seguido de argumento
        /// Habría que ver si lo que sigue son todo nombres cortos
        bool check_combined = true;
        for (auto &opt : arg_cmd_name){
          bool bFind = false;
          for (auto &arg : mCmdArgs) {
            if (arg->shortName()){
              if (arg->shortName() == opt){
                bFind = true;
                break;
              }
            }
          }
          if (bFind == false) {
            /// Si no encuentra no es opción
            check_combined = false;
            break;
          }
        }

        if (check_combined){
          for (auto &opt : arg_cmd_name){
            std::stringstream ss;
            std::string short_name;
            ss << opt;
            ss >> short_name;
            cmd_in[short_name] = "true";
          }
        } else {
          std::string short_name = arg_cmd_name.substr(0, 1);
          std::string arg_value = arg_cmd_name.substr(1, arg_cmd_name.size()-1);
          cmd_in[short_name] = arg_value;
        }
        continue;
      }
    } else {
      continue;
    }

    std::string value;

    if (i+1 < argc) {
      /// Se comprueba si el elemento siguiente es un valor
      std::string arg_value = std::string(argv[i+1]);
      std::size_t found_next_name = arg_value.find("--");
      std::size_t found_next_short_name = arg_value.find("-");
      if ((found_next_name != std::string::npos  && found_next_name == 0) ||
          (found_next_short_name != std::string::npos && found_next_short_name == 0)){
        value = "true";
      } else {
        value = arg_value;
        i++;
      }
    } else {
      value = "true";
    }

    cmd_in[arg_cmd_name] = value;

  }

  std::map<std::string, std::string>::iterator it;
  if (cmd_in.find("h") != cmd_in.end() || cmd_in.find("help") != cmd_in.end()){
    showHelp();
    return Command::Status::SHOW_HELP;
  }

  if (cmd_in.find("version") != cmd_in.end()){
    showVersion();
    return Command::Status::SHOW_VERSION;
  }

  if (cmd_in.find("licence") != cmd_in.end()){
    showLicence();
    return Command::Status::SHOW_LICENCE;
  }


  for (auto &arg : mCmdArgs) {
    bool bOptional = !arg->isRequired();
    bool bFind = false;

    std::stringstream ss;
    std::string short_name;
    ss << arg->shortName();
    ss >> short_name;
    if (cmd_in.find(short_name) != cmd_in.end()){
      arg->fromString(cmd_in.find(short_name)->second);
      bFind = true;
    } else if (cmd_in.find(arg->name()) != cmd_in.end()){
      arg->fromString(cmd_in.find(arg->name())->second);
      bFind = true;
    } else {
      bFind = false;
    }

    if (bFind == false && bOptional == false) {
      msgError("Falta %s. Parámetro obligatorio ", arg->name().c_str());
      //printHelp();
      return Command::Status::PARSE_ERROR;
    }
  }

  return Command::Status::PARSE_SUCCESS;
}

Command::iterator Command::begin()
{
  return mCmdArgs.begin();
}

Command::const_iterator Command::begin() const
{
  return mCmdArgs.cbegin();
}

Command::iterator Command::end()
{
  return mCmdArgs.end();
}

Command::const_iterator Command::end() const
{
  return mCmdArgs.cend();
}

void Command::push_back(const std::shared_ptr<Argument> &arg)
{
  mCmdArgs.push_back(arg);
}

void Command::push_back(std::shared_ptr<Argument> &&arg) TL_NOEXCEPT
{
  mCmdArgs.push_back(std::forward<std::shared_ptr<Argument>>(arg));
}

void Command::clear()
{
  mCmdArgs.clear();
}

bool Command::empty() const
{
  return mCmdArgs.empty();
}

Command &Command::operator=(const Command &command)
{
  if (this != &command) {
    this->mName = command.mName;
    this->mDescription = command.mDescription;
    this->mCmdArgs = command.mCmdArgs;
    this->mVersion = command.mVersion;
  }
  return (*this);
}

Command &Command::operator=(Command &&command) TL_NOEXCEPT
{
  if (this != &command) {
    this->mName = std::move(command.mName);
    this->mDescription = std::move(command.mDescription);
    this->mCmdArgs = std::move(command.mCmdArgs);
    this->mVersion = std::move(command.mVersion);
  }
  return (*this);
}

Command::iterator Command::erase(Command::const_iterator first, Command::const_iterator last)
{
  return mCmdArgs.erase(first, last);
}

void Command::showHelp() const
{


//  Console &console = Console::getInstance();
//  console.setConsoleForegroundColor(Console::Color::GREEN, Console::Intensity::BRIGHT);
//  console.setFontBold(true);

//  /// Nombre del comando
//  printf("%s \n\n", mName.c_str());

//  console.setConsoleForegroundColor(Console::Color::WHITE, Console::Intensity::BRIGHT);
//  console.setFontBold(false);

//  /// Descripción del comando
//  printf("%s \n\n", mDescription.c_str());

//  /// Sintaxis
//  console.setFontBold(true);
//  printf_s("\nSyntax:\n\n");
//  console.setFontBold(false);

//  printf_s("%s", mName.c_str());
//  for (auto arg : mCmdArgs) {
//     printf_s( " [--%s|-%c] [value]", arg->name().c_str(), arg->shortName());
//  }
//  printf_s("\n\n");

//  /// Parámetros
//  console.setFontBold(true);
//  printf_s("Parameters:\n\n");
//  console.setFontBold(false);

//  for (auto arg : mCmdArgs) {
//     printf_s("- [%s|%c] %s (%s)\n", arg->name().c_str(), arg->shortName(), arg->description().c_str(), (arg->isRequired() ? "Required" : "Optional"));
//  }

  /// Linux syntax
  TL_TODO("Solucion rapida. revisar")

  /// Uso
  Console &console = Console::getInstance();
  console.setConsoleForegroundColor(Console::Color::GREEN, Console::Intensity::BRIGHT);
  console.setFontBold(true);
  printf("Usage: %s [OPTION...] \n\n", mName.c_str());
  console.reset();

  /// Descripción del comando
  printf("%s \n\n", mDescription.c_str());

  size_t max_name_size = 7;
  for (auto arg : mCmdArgs) {
    max_name_size = std::max(max_name_size, arg->name().size());
  }
  std::string name_tmpl = std::string("%s%-").append(std::to_string(max_name_size)).append("s %s");
  printf_s( "  -h, ");
  printf_s(name_tmpl.c_str(), "--", "help", "Display this help and exit\n");
  printf_s( "    , ");
  printf_s(name_tmpl.c_str(), "--", "version", "Output version information and exit\n");

  for (auto arg : mCmdArgs) {
    if (arg->shortName()){
      printf_s( "  -%c, ", arg->shortName());
    } else {
      printf_s( "    , ");
    }
    if (!arg->name().empty()){
      printf_s(name_tmpl.c_str(), "--", arg->name().c_str(), arg->description().c_str());
    } else {
      printf_s(name_tmpl.c_str(), "  ", "", arg->description().c_str());
    }
    printf_s("\n");
  }
  printf_s("\n\n");

  console.setConsoleForegroundColor(Console::Color::GREEN, Console::Intensity::BRIGHT);
  console.setFontBold(true);
  printf("Argument Syntax Conventions\n\n");
  console.reset();

  printf_s("  - Arguments are options if they begin with a hyphen delimiter (-).\n");
  printf_s("  - Multiple options may follow a hyphen delimiter in a single token if the options do not take arguments. ‘-abc’ is equivalent to ‘-a -b -c’.\n");
  printf_s("  - Option names are single alphanumeric characters.\n");
  printf_s("  - An option and its argument may or may not appear as separate tokens. ‘-o foo’ and ‘-ofoo’ are equivalent.\n");
  printf_s("  - Long options (--) can have arguments specified after space or equal sign (=).  ‘--name=value’ is equivalent to ‘--name value’.\n\n");


//  for (auto arg : mCmdArgs) {
//     printf_s("- [%s|%c] %s (%s)\n", arg->name().c_str(), arg->shortName(), arg->description().c_str(), (arg->isRequired() ? "Required" : "Optional"));
//  }
}

void Command::showVersion() const
{
  Console &console = Console::getInstance();
  console.setConsoleForegroundColor(Console::Color::GREEN, Console::Intensity::BRIGHT);
  console.setFontBold(true);

  printf_s("Version: %s\n", mVersion.c_str());

  console.reset();
}

void Command::showLicence() const
{
  Console &console = Console::getInstance();
  console.setConsoleForegroundColor(Console::Color::GREEN, Console::Intensity::BRIGHT);
  console.setFontBold(true);
}

size_t Command::size() const
{
  return mCmdArgs.size();
}


void Command::init()
{

}


/* ---------------------------------------------------------------------------------- */


std::mutex Progress::sMutex;

Progress::Progress() 
  : mProgress(0.), 
    mMinimun(0.), 
    mMaximun(100.), 
    mPercent(-1), 
    mMsg(""),
    onProgress(nullptr),
    onInitialize(nullptr),
    onTerminate(nullptr)
{
  updateScale();
}

Progress::Progress(double min, double max, const std::string &msg) 
  : mProgress(0.), 
    mMinimun(min), 
    mMaximun(max), 
    mPercent(-1), 
    mMsg(msg),
    onProgress(nullptr),
    onInitialize(nullptr),
    onTerminate(nullptr)
{
  updateScale();
}

bool Progress::operator()(double increment) 
{ 
  std::lock_guard<std::mutex> lck(Progress::sMutex);
  if (mProgress == 0.) initialize();
  mProgress += increment;
  int percent = TL_ROUND_TO_INT(mProgress * mScale);
  if (percent > mPercent) {
    mPercent = percent;
    updateProgress();
  }
  if (mProgress == mMaximun) terminate();
  return true;
}

void Progress::init(double min, double max, const std::string &msg)
{
  mMinimun = min;
  mMaximun = max;
  mMsg = msg;
  restart();
  updateScale();
}

void Progress::setMinimun(double min)
{
  mMinimun = min;
}

void Progress::setMaximun(double max)
{
  mMaximun = max;
}

void Progress::restart()
{
  mPercent = 0;
  mProgress = 0.;
}

void Progress::setOnProgressListener(std::function<void(double)> &progressFunction)
{
  *onProgress = progressFunction;
}

void Progress::setOnInitializeListener(std::function<void(void)> &initializeFunction)
{
  *onInitialize = initializeFunction;
}

void Progress::setOnTerminateListener(std::function<void(void)> &terminateFunction)
{
  *onTerminate = terminateFunction;
}

/* metodos protected*/

void Progress::initialize()
{
  printf_s("%s\n", mMsg.c_str());

  if (onInitialize) (*onInitialize)();
}

//void Progress::updateProgress() 
//{
//  if (onProgress) (*onProgress)(mPercent);
//}

void Progress::updateScale()
{
  mScale = 100./(mMaximun - mMinimun);
}

//void Progress::terminate()
//{
//  printf("\n");
//  if (onTerminate) (*onTerminate)();
//}


/* ---------------------------------------------------------------------------------- */

ProgressBar::ProgressBar(bool customConsole)
  : Progress(), 
    bCustomConsole(customConsole) 
{
}

ProgressBar::ProgressBar(double min, double max, bool customConsole)
  : Progress(min, max),
    bCustomConsole(customConsole) 
{
}

void ProgressBar::updateProgress() 
{
  if (onProgress == nullptr) {
    
    std::cout << "\r";

    Console &console = Console::getInstance();
    int posInBar = TL_ROUND_TO_INT(mPercent * mSize / 100.);

    int ini = mSize / 2 - 2;
    for (int i = 0; i < mSize; i++) {
      if (i < posInBar) {
        if (bCustomConsole) {
          console.setConsoleBackgroundColor(Console::Color::GREEN);
        } else {
          std::cout << "#";
        }
      } else {
        if (bCustomConsole) {
          console.setConsoleBackgroundColor(Console::Color::YELLOW);
        } else {
          std::cout << "-";
        }
      }
      if (bCustomConsole) {
        int n;
        if (i == ini) {
          n = mPercent / 100 % 10;
          if ( n > 0 ) std::cout << n;
          else std::cout << " ";
        } else if (i == ini + 1) {
          n = mPercent / 10 % 10;
          if ( n > 0 || mPercent >= 10) std::cout << n;
          else std::cout << " ";
        } else if (i == ini + 2) {
          n = mPercent % 10;
          std::cout << n;
        } else if (i == ini + 3) {
          std::cout << "%";
        } else {
          std::cout << " ";
        }
      }
    }

    if (bCustomConsole) {
      console.reset();
    } else {
      std::cout << " " << mPercent << "%  completed" << std::flush;
    }
  } else {
    (*onProgress)(mPercent);
  }
}

void ProgressBar::terminate()
{
  if (onTerminate == nullptr)
    printf("\n");
  else 
    (*onTerminate)();
}

/* ---------------------------------------------------------------------------------- */

ProgressPercent::ProgressPercent(bool customConsole)
  : Progress(),
    bCustomConsole(customConsole) 
{
}

ProgressPercent::ProgressPercent(double min, double max, bool customConsole)
  : Progress(min, max), 
    bCustomConsole(customConsole) 
{
}

void ProgressPercent::updateProgress() 
{
  if (onProgress == nullptr) {
    std::cout << "\r";
    std::cout << " " << mPercent << "%  completed" << std::flush;
  } else
    (*onProgress)(mPercent);
}

void ProgressPercent::terminate()
{
  if (onTerminate == nullptr)
    printf("\n");
  else 
    (*onTerminate)();
}




/* ---------------------------------------------------------------------------------- */

#ifdef TL_SHOW_DEPRECATED

/* Deprecated class */

CmdArgument::CmdArgument(const char *name, const char *description, bool optional)
  : mName(name),
    mDescription(description),
    bOptional(optional)
{
}

const char *CmdArgument::getDescription() const
{
  return mDescription.c_str();
}

const char *CmdArgument::getName() const
{
  return mName.c_str();
}

bool CmdArgument::isOptional() const
{
  return bOptional;
}


/* ---------------------------------------------------------------------------------- */

CmdOption::CmdOption(const char *name, const char *description)
  : CmdArgument(name, description, true), mValue(false)
{
}

CmdArgument::Type CmdOption::getType() const
{
  return CmdArgument::Type::OPTION;
}

bool CmdOption::isActive() const
{
  return mValue;
}

void CmdOption::setActive(bool active)
{
  mValue = active;
}


/* ---------------------------------------------------------------------------------- */

CmdParameter::CmdParameter(const char *name, const char *description, bool optional, const char *defValue)
  : CmdArgument(name, description, optional), mValue(defValue)
{
}

CmdArgument::Type CmdParameter::getType() const
{
  return CmdArgument::Type::PARAMETER;
}

const char *CmdParameter::getValue() const
{
  return mValue.c_str();
}

void CmdParameter::setValue(const std::string &value)
{
  mValue = value;
}


/* ---------------------------------------------------------------------------------- */

CmdParameterOptions::CmdParameterOptions(const char *name, const char *options, const char *description, bool optional, const char *defValue)
  : CmdArgument(name, description, optional), mValue(defValue)
{
  split(options, mOptions, ",");
}

int CmdParameterOptions::getIndex() const
{
  for ( size_t i = 0; i < mOptions.size(); i++ ) {
    if (mOptions[i] == mValue) {
      return static_cast<int>(i);
      break;
    }
  }
  return 0;
}

int CmdParameterOptions::getIndex(const std::string &value) const
{
  for ( size_t i = 0; i < mOptions.size(); i++ ) {
    if (mOptions[i] == value) {
      return static_cast<int>(i);
      break;
    }
  }
  return 0;
}

std::vector<std::string> CmdParameterOptions::getOptions() const
{
  return mOptions;
}

CmdArgument::Type CmdParameterOptions::getType() const
{
  return CmdArgument::Type::PARAMETER_OPTIONS;
}

const char *CmdParameterOptions::getValue() const
{
  return mValue.c_str();
}

void CmdParameterOptions::setValue(const std::string &value)
{
  for (auto opt : mOptions) {
    if (value == opt) {
      mValue = value;
      break;
    }
  }
}

/* ---------------------------------------------------------------------------------- */

CmdParser::CmdParser()
  : mCmdName(""),
    mCmdDescription(""),
    mCmdArgs(0)
{
}

CmdParser::CmdParser(const char *name, const char *description)
  : mCmdName(name),
    mCmdDescription(description),
    mCmdArgs(0)
{
}

CmdParser::CmdParser(const char *name, const char *description, std::initializer_list<std::shared_ptr<CmdArgument>> cmd_args)
  : mCmdName(name),
    mCmdDescription(description),
    mCmdArgs(cmd_args)
{
}

void CmdParser::addParameter(const char *name, const char *description, bool optional, const char *defValue)
{
  mCmdArgs.push_back(std::make_shared<CmdParameter>(name, description, optional, defValue));
}


void CmdParser::addParameterOption(const char *name, const char *options, const char *description, bool optional, const char *defValue)
{
  mCmdArgs.push_back(std::make_shared<CmdParameterOptions>(name, options, description, optional, defValue));
}


void CmdParser::addOption(const char *name, const char *description)
{
  mCmdArgs.push_back(std::make_shared<CmdOption>(name, description));
}

CmdParser::Status CmdParser::parse(int argc, const char* const argv[])
{
  for (auto arg : mCmdArgs) {
    // Comando de ayuda
    if (argc > 1 && strcmp(argv[1], "-help") == 0) {
      printHelp();
      return CmdParser::Status::PARSE_HELP;
    }
    bool bOptional = arg->isOptional();
    std::string argName = (arg->getType() == CmdArgument::Type::OPTION) ? "-" : "--";
    argName += arg->getName();
    bool bFind = false;
    for (int i = 1; i < argc; ++i) {
      std::string arg_name = std::string(argv[i]);
      std::size_t found = arg_name.find(argName);
      if (found != std::string::npos) {
        if (arg_name == argName && arg->getType() == CmdArgument::Type::OPTION) {
          dynamic_cast<CmdOption *>(arg.get())->setActive(true);
          break;
        } else {
          std::size_t val_pos = arg_name.find("=",found);
          std::string name = arg_name.substr(0, val_pos);
          if (val_pos != std::string::npos && name == argName) {
            if (arg->getType() == CmdArgument::Type::PARAMETER) {
              std::string value = arg_name.substr(val_pos+1, arg_name.size() - val_pos);
              TL::replaceString(&value, "\"", "\\");
              dynamic_cast<CmdParameter *>(arg.get())->setValue(value);
              bFind = true;
              break;
            } else if (arg->getType() == CmdArgument::Type::PARAMETER_OPTIONS) {
              std::string value = arg_name.substr(val_pos+1, arg_name.size() - val_pos);
              dynamic_cast<CmdParameterOptions *>(arg.get())->setValue(value);
              bFind = true;
              break;
            }
          }
        }
      }
    }
    // Ver si no es opcional y devolver un error si no existe
    if (bFind == false && bOptional == false) {
      msgError("Falta %s. Parámetro obligatorio ", arg->getName());
      printHelp();
      return CmdParser::Status::PARSE_ERROR;
    }
  }
  return CmdParser::Status::PARSE_SUCCESS;
}

void CmdParser::printHelp()
{

  Console console(Console::Mode::OUTPUT, false);

  console.setConsoleForegroundColor(Console::Color::GREEN, Console::Intensity::BRIGHT);
  console.setFontBold(true);
  //TODO: Solución rapida. modificar
  printf("%s: %s \n\n", mCmdName.c_str(), mCmdDescription.c_str());
  //printf_s("%s: %s \n", mCmdName.c_str(), mCmdDescription.c_str());

  console.setConsoleForegroundColor(Console::Color::WHITE, Console::Intensity::BRIGHT);
  console.setFontBold(false);

  printf("Listado de parámetros: \n\n");

  //TODO: Añadir automaticamente el valor por defecto
  for (auto arg : mCmdArgs) {
    std::string s_type, s_description;
    if (arg->getType() == CmdArgument::Type::OPTION) {
      s_type = "Opción";
      s_description = arg->getDescription();
    } else if (arg->getType() == CmdArgument::Type::PARAMETER) {
      s_type = "Parámetro";
      s_description = arg->getDescription();
    } else if (arg->getType() == CmdArgument::Type::PARAMETER_OPTIONS) {
      s_type = "Lista de opciones";
      s_description = arg->getDescription();
      s_description = ". Los valores posibles son: ";
      for (auto opt : dynamic_cast<CmdParameterOptions *>(arg.get())->getOptions()) {
        s_description += " ";
        s_description += opt;
      }
    } else continue;
     printf_s("- %s [%s | %s]: %s \n", arg->getName(), s_type.c_str(), (arg->isOptional() ? "O" : "R"), s_description.c_str());
    //printf_s("%s [%s | %s]: %s \n", arg->getName(), ((ArgType::OPTION == arg->getType())? "Option" : "Parameter"), (arg->isOptional() ? "O" : "R"), arg->getDescription().c_str());
  }
  printf_s("\nUso:\n\n");
  printf_s("%s", mCmdName.c_str());
  for (auto arg : mCmdArgs) {
    printf_s( " %s%s%s", ((CmdArgument::Type::OPTION == arg->getType())? "-" : "--"),
             arg->getName(), ((CmdArgument::Type::OPTION == arg->getType())? "" : "=[value]"));
  }
  printf_s("\n\n");
  //printf_s("\n\nPulse intro para continuar");
  //getchar();
}

bool CmdParser::hasOption(const std::string &option) const
{
  for (auto arg : mCmdArgs) {
    if (arg->getType() == CmdArgument::Type::OPTION) {
      if (arg->getName() == option) {
        return dynamic_cast<CmdOption *>(arg.get())->isActive();
      }
    }
  }
  return false;
}

/* End deprecated class */

#endif // TL_SHOW_DEPRECATED

/* ---------------------------------------------------------------------------------- */

} // End mamespace TL
