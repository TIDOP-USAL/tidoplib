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

#include "tidop/core/console.h"

#include "tidop/core/defs.h"
#include "tidop/core/utils.h"
#include "tidop/core/messages.h"

#include <iostream>
#include <ctime>
#include <cstdio>
#include <utility>
#include <vector>
#include <iomanip>

namespace tl
{

struct msgProperties {
  const char *normal;
  const char *extend;
  Console::Color foreColor;
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
  //sObjConsole.reset();
}

Console &Console::instance()
{
  if (sObjConsole == nullptr) {
    std::lock_guard<std::mutex> lck(Console::mtx);
    if (sObjConsole == nullptr) {
      sObjConsole.reset(new Console());
    }
  }
  return *sObjConsole;
}

EnumFlags<MessageLevel> Console::messageLevel()
{
  return sLevel;
}

void Console::setMessageLevel(MessageLevel level)
{
  sLevel = level;
}

#ifdef TL_ENABLE_DEPRECATED_METHODS
void Console::setLogLevel(MessageLevel level)
{
  sLevel = level;
}
#endif

void Console::printMessage(const std::string &message)
{
  // Por si esta corriendo la barra de progreso
  std::cout << "\r";

  std::string aux(message);
  replaceString(&aux, "%", "%%");
  std::cout << aux << std::endl;
}

void Console::printErrorMessage(const std::string &message)
{
  setConsoleForegroundColor(messageProperties(MessageLevel::msg_error).foreColor,
                            messageProperties(MessageLevel::msg_error).intensity);
  //printMessage(message);

  // Por si esta corriendo la barra de progreso
  std::cout << "\r";

  std::string aux(message);
  replaceString(&aux, "%", "%%");
  std::cerr << aux << std::endl;

  reset();
}

void Console::reset()
{
#ifdef WIN32
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
#ifdef WIN32
  switch (backgroundColor) {
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
#ifdef WIN32
  switch (foregroundColor) {
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

void Console::setFontHeight(int16_t fontHeight)
{
#ifdef WIN32
  mCurrentFont.dwFontSize.Y = static_cast<SHORT>(fontHeight);
#endif
  update();
}

void Console::setTitle(const std::string &title)
{
#ifdef WIN32
  SetConsoleTitleA(title.c_str());
#else
  //printf("%c]0;%s%c", '\033', title, '\007');
#endif
}

#ifdef TL_MESSAGE_HANDLER

TL_DISABLE_WARNING(TL_UNREFERENCED_FORMAL_PARAMETER)

void Console::onMsgDebug(const std::string &message,
                         const std::string &)
{
  if (sLevel.isActive(MessageLevel::msg_debug)) {
    setConsoleForegroundColor(messageProperties(MessageLevel::msg_debug).foreColor,
                              messageProperties(MessageLevel::msg_debug).intensity);
    printMessage(message);
    reset();
  }
}

void Console::onMsgInfo(const std::string &message,
                        const std::string &)
{
  if (sLevel.isActive(MessageLevel::msg_info)) {
    setConsoleForegroundColor(messageProperties(MessageLevel::msg_info).foreColor,
                              messageProperties(MessageLevel::msg_info).intensity);
    printMessage(message);
    reset();
  }
}

void Console::onMsgWarning(const std::string &message,
                           const std::string &)
{
  if (sLevel.isActive(MessageLevel::msg_warning)) {
    setConsoleForegroundColor(messageProperties(MessageLevel::msg_warning).foreColor,
                              messageProperties(MessageLevel::msg_warning).intensity);
    printMessage(message);
    reset();
  }
}

void Console::onMsgError(const std::string &message,
                         const std::string &)
{
  if (sLevel.isActive(MessageLevel::msg_error)) {
    printErrorMessage(message);
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
  mForegroundColor = (mOldColorAttrs & 0x0007);
  mForegroundIntensity = (mOldColorAttrs & 0x0008);
  mBackgroundColor = (mOldColorAttrs & 0x0070);
  mBackgroundIntensity = (mOldColorAttrs & 0x0080);

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
  mForegroundColor = 0;
  mBackgroundColor = 0;
  mBold = 21;
}
#endif

void Console::update()
{
#ifdef WIN32
  SetConsoleTextAttribute(mHandle, mForegroundColor | mBackgroundColor | mForegroundIntensity | mBackgroundIntensity);
  SetCurrentConsoleFontEx(mHandle, FALSE, &mCurrentFont);
#else
  std::stringstream ss;
  ss << "\x1B[" << mBold;
  if (mForegroundColor != 0)
    ss << ";" << mForegroundColor;
  if (mBackgroundColor != 0)
    ss << ";" << mBackgroundColor;
  ss << "m";
  fprintf(mStream, "%s", ss.str().c_str());
#endif
}

#ifdef TL_ENABLE_DEPRECATED_METHODS
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
#endif // TL_ENABLE_DEPRECATED_METHODS


/* ---------------------------------------------------------------------------------- */

Argument::Argument(std::string name,
                   std::string description)
  : mName(std::move(name)),
    mDescription(std::move(description)),
    mShortName()
{

}

Argument::Argument(const char &shortName,
                   std::string description)
  : mName(""),
    mDescription(std::move(description)),
    mShortName(shortName)
{
}

Argument::Argument(std::string name,
                   const char &shortName,
                   std::string description)
  : mName(std::move(name)),
    mDescription(std::move(description)),
    mShortName(shortName)
{
}

Argument::Argument(const Argument &argument) = default;

Argument::Argument(Argument &&argument) TL_NOEXCEPT
  : mName(std::move(argument.mName)),
    mDescription(std::move(argument.mDescription)),
    mShortName(argument.mShortName)
{
}

Argument &Argument::operator=(const Argument &argument)
{
  if (this != &argument){
    this->mName = argument.mName;
    this->mDescription = argument.mName;
    this->mShortName = argument.mShortName;
  }
  return *this;
}

Argument &Argument::operator = (Argument &&argument) TL_NOEXCEPT
{
  if (this != &argument){
    this->mName = std::move(argument.mName);
    this->mDescription = std::move(argument.mName);
    this->mShortName = argument.mShortName;
  }
  return *this;
}

std::string Argument::name() const
{
  return mName;
}

void Argument::setName(const std::string &name)
{
  mName = name;
}

std::string Argument::description() const
{
  return mDescription;
}

void Argument::setDescription(const std::string &description)
{
  mDescription = description;
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
    mArguments(0),
    mVersion("0.0.0")
{
    init();
}

Command::Command(const Command &command)
  : mName(command.mName),
    mDescription(command.mDescription),
    mArguments(command.mArguments),
    mVersion(command.mVersion),
    mExamples(command.mExamples)
{

}

Command::Command(std::string name, std::string description)
  : mName(std::move(name)),
    mDescription(std::move(description)),
    mArguments(0),
    mVersion("0.0.0")
{
  init();
}

Command::Command(std::string name, 
                 std::string description,
                 std::initializer_list<std::shared_ptr<Argument>> arguments)
  : mName(std::move(name)),
    mDescription(std::move(description)),
    mArguments(arguments),
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
    std::size_t found_short_name = arg_cmd_name.find('-');

    if (found_name != std::string::npos && found_name == 0) {
      arg_cmd_name = (argv[i])+2;
      /// argumento-valor separado por =
      std::vector<std::string> v;
      split(arg_cmd_name, v, "=");
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
          for (auto &argument : mArguments) {
            if (argument->shortName()){
              if (argument->shortName() == opt){
                bFind = true;
                break;
              }
            }
          }
          if (!bFind) {
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
      std::size_t found_next_short_name = arg_value.find('-');
      if ((found_next_name != std::string::npos  && found_next_name == 0) ||
          (found_next_short_name != std::string::npos && found_next_short_name == 0)){
        //value = "true";
      } else {
        value = arg_value;
        i++;
      }
    } else {
      //value = "true";
    }

    cmd_in[arg_cmd_name] = value;

  }

  //std::map<std::string, std::string>::iterator it;
  if (cmd_in.find("h") != cmd_in.end() || cmd_in.find("help") != cmd_in.end()){
    showHelp();
    return Command::Status::show_help;
  }

  if (cmd_in.find("version") != cmd_in.end()){
    showVersion();
    return Command::Status::show_version;
  }

  if (cmd_in.find("licence") != cmd_in.end()){
    showLicence();
    return Command::Status::show_licence;
  }


  for (auto &argument : mArguments) {
    bool bOptional = !argument->isRequired();
    bool bFind = false;
    bool bFindValue = false;

    std::stringstream ss;
    std::string short_name;
    ss << argument->shortName();
    ss >> short_name;
    if (cmd_in.find(short_name) != cmd_in.end()){
      bFind = true;
      std::string value = cmd_in.find(short_name)->second;
      if (value.empty()){
        if (argument->typeName() == "bool"){
          value = "true";
          bFindValue = true;
        }
      } else {
        bFindValue = true;
      }

      if (bFindValue)
        argument->fromString(value);

    } else if (cmd_in.find(argument->name()) != cmd_in.end()){
      bFind = true;
      std::string value = cmd_in.find(argument->name())->second;
      if (value.empty()){
        if (argument->typeName() == "bool"){
          value = "true";
          bFindValue = true;
        }
      } else {
        bFindValue = true;
      }

      if (bFindValue)
        argument->fromString(value);

    } else {
      bFind = false;
    }

    if (!bFind && !bOptional) {
      msgError("Missing mandatory argument: %s", argument->name().c_str());
      return Command::Status::parse_error;
    }
    if (bFind && !bFindValue) {
      msgError("Missing value for argument: %s", argument->name().c_str());
      return Command::Status::parse_error;
    }
    if (!argument->isValid()){
      msgError("Invalid argument (%s) value", argument->name().c_str());
      return Command::Status::parse_error;
    }
  }

  return Command::Status::parse_success;
}

Command::iterator Command::begin() TL_NOEXCEPT
{
  return mArguments.begin();
}

Command::const_iterator Command::begin() const TL_NOEXCEPT
{
  return mArguments.cbegin();
}

Command::iterator Command::end() TL_NOEXCEPT
{
  return mArguments.end();
}

Command::const_iterator Command::end() const TL_NOEXCEPT
{
  return mArguments.cend();
}

void Command::push_back(const std::shared_ptr<Argument> &argument)
{
  mArguments.push_back(argument);
}

void Command::addArgument(const std::shared_ptr<Argument> &argument)
{
  mArguments.push_back(argument);
}

void Command::push_back(std::shared_ptr<Argument> &&argument) TL_NOEXCEPT
{
  mArguments.push_back(std::forward<std::shared_ptr<Argument>>(argument));
}

void Command::addArgument(std::shared_ptr<Argument> &&argument) TL_NOEXCEPT
{
  mArguments.push_back(std::forward<std::shared_ptr<Argument>>(argument));
}

void Command::clear() TL_NOEXCEPT
{
  mArguments.clear();
  mExamples.clear();
}

bool Command::empty() const TL_NOEXCEPT
{
  return mArguments.empty();
}

size_t Command::size() const TL_NOEXCEPT
{
  return mArguments.size();
}

Command &Command::operator=(const Command &command)
{
  if (this != &command) {
    this->mName = command.mName;
    this->mDescription = command.mDescription;
    this->mArguments = command.mArguments;
    this->mVersion = command.mVersion;
  }
  return (*this);
}

Command &Command::operator=(Command &&command) TL_NOEXCEPT
{
  if (this != &command) {
    this->mName = std::move(command.mName);
    this->mDescription = std::move(command.mDescription);
    this->mArguments = std::move(command.mArguments);
    this->mVersion = std::move(command.mVersion);
  }
  return (*this);
}

Command::iterator Command::erase(Command::const_iterator first, 
                                 Command::const_iterator last)
{
  return mArguments.erase(first, last);
}

void Command::showHelp() const
{

  Console &console = Console::instance();
  console.setConsoleForegroundColor(Console::Color::green, Console::Intensity::bright);
  console.setFontBold(true);
  std::cout << "\nUsage: " << mName << " [OPTION...] \n\n";
  console.reset();

  /// Descripción del comando
  std::cout << mDescription << "\n\n";

  int max_name_size = 7;
  for (const auto &arg : mArguments) {
    max_name_size = std::max(max_name_size, static_cast<int>(arg->name().size()));
  }
  max_name_size += 1;

  std::cout << "  -h, --" << std::left << std::setw(max_name_size) << "help" << "    Display this help and exit\n";
  std::cout << "    , --" << std::left << std::setw(max_name_size) << "version" << "    Show version information and exit\n";

  for (const auto &arg : mArguments) {

    if (arg->shortName()){
      std::cout << "  -" << arg->shortName() << ", ";
    } else {
      std::cout << "    , ";
    }

    if (!arg->name().empty()){
      std::cout << "--" << std::left << std::setw(max_name_size) << arg->name() << (arg->isRequired() ? "[R] " : "[O] ") << arg->description();
    } else {
      std::cout << "--" << std::left << std::setw(max_name_size) << "" << (arg->isRequired() ? "[R] " : "[O] ") << arg->description();
    }

    std::cout << "\n";
  }
  std::cout << "\n\n";

  std::cout << "R: Required argument\n";
  std::cout << "O: Optional argument\n\n";


  console.setConsoleForegroundColor(Console::Color::green, Console::Intensity::bright);
  console.setFontBold(true);
  std::cout << "Argument Syntax Conventions\n\n";
  console.reset();

  std::cout << "  - Arguments are options if they begin with a hyphen delimiter (-).\n";
  std::cout << "  - Multiple options may follow a hyphen delimiter in a single token if the options do not take arguments. ‘-abc’ is equivalent to ‘-a -b -c’.\n";
  std::cout << "  - Option names are single alphanumeric characters.\n";
  std::cout << "  - An option and its argument may or may not appear as separate tokens. ‘-o foo’ and ‘-ofoo’ are equivalent.\n";
  std::cout << "  - Long options (--) can have arguments specified after space or equal sign (=).  ‘--name=value’ is equivalent to ‘--name value’.\n\n";

  if (!mExamples.empty()){
    console.setConsoleForegroundColor(Console::Color::green, Console::Intensity::bright);
    console.setFontBold(true);
    std::cout << "Examples\n\n";
    console.reset();

    for (auto &example : mExamples){
      std::cout << "  " << example << "\n";
    }
  }
}

void Command::showVersion() const
{
  Console &console = Console::instance();
  console.setConsoleForegroundColor(Console::Color::green, Console::Intensity::bright);
  console.setFontBold(true);

  std::cout << "Version: " << mVersion << "\n";

  console.reset();
}

void Command::showLicence() const
{
  Console &console = Console::instance();
  console.setConsoleForegroundColor(Console::Color::green, Console::Intensity::bright);
  console.setFontBold(true);
  std::cout << "Licence\n\n";
  console.reset();

  std::cout << mLicence.productName() << ": " << mLicence.version() << "\n";

  //mLicence.productName();
  //mLicence.version();
  //mLicence.autor();
  //mLicence.autorEmail();
  //mLicence.type();
}

void Command::addExample(const std::string &example)
{
  mExamples.push_back(example);
}

void Command::setLicence(const Licence &licence)
{
  mLicence = licence;
}

void Command::init()
{

}


/* ---------------------------------------------------------------------------------- */

CommandList::CommandList()
  : mVersion("0.0.0")
{
}

CommandList::CommandList(std::string name,
                         std::string description)
  : mName(std::move(name)),
    mDescription(std::move(description)),
    mVersion("0.0.0")
{
}

CommandList::CommandList(const CommandList &commandList)
  : mName(commandList.mName),
    mDescription(commandList.mDescription),
    mCommands(commandList.mCommands),
    mVersion(commandList.mVersion)
{
}

CommandList::CommandList(CommandList &&commandList) TL_NOEXCEPT
  : mName(std::move(commandList.mName)),
    mDescription(std::move(commandList.mDescription)),
    mCommands(std::move(commandList.mCommands)),
    mVersion(std::move(commandList.mVersion))
{
}

CommandList::CommandList(std::string name,
                         std::string description,
                         std::initializer_list<std::shared_ptr<Command>> commands)
  : mName(std::move(name)),
    mDescription(std::move(description)),
    mCommands(commands),
    mVersion("0.0.0")
{
}

std::string CommandList::name() const
{
  return mName;
}

void CommandList::setName(const std::string &name)
{
  mName = name;
}

std::string CommandList::description() const
{
  return mDescription;
}

void CommandList::setDescription(const std::string &description)
{
  mDescription = description;
}

std::string CommandList::version() const
{
  return mVersion;
}

void CommandList::setVersion(const std::string &version)
{
  mVersion = version;
}

CommandList::Status CommandList::parse(int argc, const char * const argv[])
{
  if (argc <= 1) return Status::parse_error;

  std::string arg_cmd_name = std::string(argv[1]);
  std::size_t found_name = arg_cmd_name.find("--");
  std::size_t found_short_name = arg_cmd_name.find('-');
  if (found_name != std::string::npos && found_name == 0) {
    arg_cmd_name = (argv[1])+2;
  } else if (found_short_name != std::string::npos && found_short_name == 0) {
    arg_cmd_name = (argv[1])+1;
  }

  if (arg_cmd_name == "h" || arg_cmd_name == "help"){
    showHelp();
    return Status::show_help;
  }

  if (arg_cmd_name == "version"){
    showVersion();
    return Status::show_version;
  }

  if (arg_cmd_name == "licence"){
    showLicence();
    return Status::show_licence;
  }

  for (auto &command : mCommands){
    if(command->name().compare(arg_cmd_name) == 0){
      mCommand = command;
      std::vector<char const*> cmd_argv;
      for (size_t i = 0; i < static_cast<size_t>(argc); ++i) {
        if (i != 1)
          cmd_argv.push_back(argv[i]);
      }
      Command::Status status = command->parse(argc-1, cmd_argv.data());
      if (status == Command::Status::parse_error) {
        return Status::parse_error;
      } else if (status == Command::Status::show_help) {
        return Status::show_help;
      } else if (status == Command::Status::show_licence) {
        return Status::show_licence;
      } else if (status == Command::Status::show_version) {
        return Status::show_version;
      } else {
        return Status::parse_success;
      }
    }
  }

  return Status::parse_error;
}

CommandList::iterator CommandList::begin() TL_NOEXCEPT
{
  return mCommands.begin();
}

CommandList::const_iterator CommandList::begin() const TL_NOEXCEPT
{
  return mCommands.cbegin();
}

CommandList::iterator CommandList::end() TL_NOEXCEPT
{
  return mCommands.end();
}

CommandList::const_iterator CommandList::end() const TL_NOEXCEPT
{
  return mCommands.cend();
}

void CommandList::push_back(const std::shared_ptr<Command> &command)
{
  mCommands.push_back(command);
}

void CommandList::addCommand(const std::shared_ptr<Command> &command)
{
  mCommands.push_back(command);
}

void CommandList::push_back(std::shared_ptr<Command> &&command) TL_NOEXCEPT
{
  mCommands.push_back(std::forward<std::shared_ptr<Command>>(command));
}

void CommandList::addCommand(std::shared_ptr<Command> &&command) TL_NOEXCEPT
{
  mCommands.push_back(std::forward<std::shared_ptr<Command>>(command));
}

void CommandList::clear() TL_NOEXCEPT
{
  mCommands.clear();
}

bool CommandList::empty() const TL_NOEXCEPT
{
  return mCommands.empty();
}

CommandList::size_type CommandList::size() const TL_NOEXCEPT
{
  return mCommands.size();
}

CommandList::iterator CommandList::erase(CommandList::const_iterator first, CommandList::const_iterator last)
{
  return mCommands.erase(first, last);
}

CommandList &CommandList::operator=(const CommandList &cmdList)
{
  if (this != &cmdList) {
    this->mName = cmdList.mName;
    this->mDescription = cmdList.mDescription;
    this->mCommands = cmdList.mCommands;
    this->mVersion = cmdList.mVersion;
  }
  return (*this);
}

CommandList &CommandList::operator=(CommandList &&cmdList) TL_NOEXCEPT
{
  if (this != &cmdList) {
    this->mName = std::move(cmdList.mName);
    this->mDescription = std::move(cmdList.mDescription);
    this->mCommands = std::move(cmdList.mCommands);
    this->mVersion = std::move(cmdList.mVersion);
  }
  return (*this);
}

void CommandList::showHelp() const
{

  Console &console = Console::instance();
  console.setConsoleForegroundColor(Console::Color::green, Console::Intensity::bright);
  console.setFontBold(true);
  std::cout << "\nUsage: " << mName << " [--version] [-h | --help] [--licence] <command> [<args>] \n\n";
  console.reset();

  std::cout << mDescription << " \n\n";

  console.setConsoleForegroundColor(Console::Color::green, Console::Intensity::bright);
  console.setFontBold(true);
  std::cout << "Command list: \n\n";
  console.reset();

  size_t max_name_size = 10;
  for (auto &arg : mCommands) {
    max_name_size = std::max(max_name_size, arg->name().size());
  }
  max_name_size += 2;

  for (auto &arg : mCommands) {
    std::cout << std::left << std::setw(max_name_size) << arg->name() << arg->description() << "\n";
  }

}

void CommandList::showVersion() const
{
  Console &console = Console::instance();
  console.setConsoleForegroundColor(Console::Color::green, Console::Intensity::bright);
  console.setFontBold(true);

  std::cout << "Version: " << mVersion << "\n";

  console.reset();
}

void CommandList::showLicence() const
{
  Console &console = Console::instance();
  console.setConsoleForegroundColor(Console::Color::green, Console::Intensity::bright);
  console.setFontBold(true);
}

std::string CommandList::commandName() const
{
  return mCommand ? mCommand->name() : std::string();
}


} // End mamespace tl


