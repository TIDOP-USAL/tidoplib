#include "core/console.h"

#include "config_tl.h"

#include "core/defs.h"
#include "core/utils.h"
#include "core/messages.h"

#include <iostream>
#include <ctime>
#include <cstdio>

using namespace TL;
using namespace std;

namespace TL
{

struct msgProperties {
  const char *normal;
  const char *extend;
  Console::Color foreColor;
  Console::Intensity intensity;
};

struct msgProperties msgTemplate[] = {   
  { "Debug: %s",   "Debug: %s (%s:%u, %s)",   Console::Color::WHITE, Console::Intensity::NORMAL},
  { "Info: %s",    "Info: %s (%s:%u, %s)",    Console::Color::WHITE, Console::Intensity::BRIGHT},
  { "Warning: %s", "Warning: %s (%s:%u, %s)", Console::Color::MAGENTA, Console::Intensity::BRIGHT},
  { "Error: %s",   "Error: %s (%s:%u, %s)",   Console::Color::RED, Console::Intensity::BRIGHT}
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

Console::Console(bool add) 
#ifdef TL_MESSAGE_HANDLER 
  : MessageManager::Listener(add)
#endif
{ 
#ifdef WIN32
  init(STD_OUTPUT_HANDLE);
#else
  init(stdout);
#endif
}

Console::Console(Console::Mode mode, bool add)
#ifdef TL_MESSAGE_HANDLER 
  : MessageManager::Listener(add)
#endif
{ 
#ifdef WIN32
  DWORD handle;
  switch (mode) {
  case Console::Mode::INPUT:
    handle = STD_INPUT_HANDLE;
    break;
  case Console::Mode::OUTPUT:
    handle = STD_OUTPUT_HANDLE;
    break;
  case Console::Mode::OUTPUT_ERROR:
    handle = STD_ERROR_HANDLE;
    break;
  default:
    handle = STD_OUTPUT_HANDLE;
    break;
  }
  init(handle);
#else
  FILE *stream;
    switch (mode) {
    case Console::Mode::INPUT:
      stream = stdin;
      break;
    case Console::Mode::OUTPUT:
      stream = stdout;
      break;
    case Console::Mode::OUTPUT_ERROR:
      stream = stderr;
      break;
    default:
      stream = stdout;
      break;
    }
    init(stream);
#endif
}

Console::Console(const Console &console, bool add) :
#ifdef TL_MESSAGE_HANDLER 
  MessageManager::Listener(add),
#endif
#ifdef WIN32
  mHandle(console.mHandle),
  mOldColorAttrs(console.mOldColorAttrs),
  mForeIntensity(console.mForeIntensity),
  mForeColor(console.mForeColor),
  mBackIntensity(console.mBackIntensity),
  mBackColor(console.mBackColor)
#else
  mStream(console.mStream),
  /*mCommand(0),*/
  mBold(console.mBold),
  mForeIntensity(console.mForeIntensity),
  mForeColor(console.mForeColor),
  mBackIntensity(console.mBackIntensity),
  mBackColor(console.mBackColor)
#endif
{
}

Console::~Console() 
{
  reset();
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
  mBackColor = static_cast<int>(backColor) + 40;
  mBackIntensity = static_cast<int>(intensity);
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
  mForeColor = static_cast<int>(foreColor) + 30;
  mForeIntensity = static_cast<int>(intensity);
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

TL_DISABLE_WARNING(4100)

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

TL_ENABLE_WARNING(4100)

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
  mForeIntensity = 0;
  mBackColor = 0;
  mBackIntensity = 0;
  mBold = 21;
}
#endif

void Console::update()
{
#ifdef WIN32
  SetConsoleTextAttribute(mHandle, mForeColor | mBackColor | mForeIntensity | mBackIntensity);
  SetCurrentConsoleFontEx(mHandle, FALSE, &mCurrentFont);
#else
  //sprintf(mCommand, "%c[%d;%d;%d;%dm", 0x1B, mBold, mForeIntensity, mForeColor, mBackColor);
  sprintf(mCommand, "\x1B[%i;%i;%im", mBold, mForeIntensity, mForeColor);
  fprintf(mStream, "%s", mCommand);
#endif
}



/* ---------------------------------------------------------------------------------- */

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

  printf("Listado de parámetros: \n\n", mCmdName.c_str(), mCmdDescription.c_str());

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

/* ---------------------------------------------------------------------------------- */

//std::mutex mtx_prgs; //Mejor añadirlo a la clase como dato miembro estático
std::mutex Progress::sMutex;

Progress::Progress() 
  : mProgress(0.), 
    mMinimun(0.), 
    mMaximun(100.), 
    mPercent(-1), 
    mMsg(""),
    onProgress(0),
    onInitialize(0),
    onTerminate(0)
{
  updateScale();
}

Progress::Progress(double min, double max, const std::string &msg) 
  : mProgress(0.), 
    mMinimun(min), 
    mMaximun(max), 
    mPercent(-1), 
    mMsg(msg),
    onProgress(0),
    onInitialize(0),
    onTerminate(0)
{
  updateScale();
}

bool Progress::operator()(double increment) 
{ 
  std::lock_guard<std::mutex> lck(Progress::sMutex);
  if (mProgress == 0) initialize();
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
  if (onProgress == NULL) {
    
    cout << "\r";

    Console console(Console::Mode::OUTPUT);
    int posInBar = TL_ROUND_TO_INT(mPercent * mSize / 100.);

    int ini = mSize / 2 - 2;
    for (int i = 0; i < mSize; i++) {
      if (i < posInBar) {
        if (bCustomConsole) {
          console.setConsoleBackgroundColor(Console::Color::GREEN);
        } else {
          cout << "#";
        }
      } else {
        if (bCustomConsole) {
          console.setConsoleBackgroundColor(Console::Color::YELLOW);
        } else {
          cout << "-";
        }
      }
      if (bCustomConsole) {
        int n;
        if (i == ini) {
          n = mPercent / 100 % 10;
          if ( n > 0 ) cout << n;
          else cout << " ";
        } else if (i == ini + 1) {
          n = mPercent / 10 % 10;
          if ( n > 0 || mPercent >= 10) cout << n;
          else cout << " ";
        } else if (i == ini + 2) {
          n = mPercent % 10;
          cout << n;
        } else if (i == ini + 3) {
          cout << "%";
        } else {
          cout << " ";
        }
      }
    }

    if (bCustomConsole) {
      console.reset();
    } else {
      cout << " " << mPercent << "%  completed" << flush;
    }
  } else {
    (*onProgress)(mPercent);
  }
}

void ProgressBar::terminate()
{
  if (onTerminate == NULL)
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
  if (onProgress == NULL) {
    cout << "\r";
    cout << " " << mPercent << "%  completed" << flush;
  } else
    (*onProgress)(mPercent);
}

void ProgressPercent::terminate()
{
  if (onTerminate == NULL)
    printf("\n");
  else 
    (*onTerminate)();
}

/* ---------------------------------------------------------------------------------- */

} // End mamespace TL
