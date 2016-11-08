#include "console.h"

#include "core\utils.h"

#include <iostream>
#include <string>
#include <ctime>

//#include "bprinter\table_printer.h"


using namespace I3D;
using namespace std;

namespace I3D
{

Console::Console() 
{ 
#ifdef WIN32
  init(STD_OUTPUT_HANDLE);
#endif
}

Console::Console(Console::Mode mode)
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
#endif
}

Console::~Console() 
{
  reset();
}

void Console::reset() 
{
#ifdef WIN32
  SetConsoleTextAttribute(h, mOldColorAttrs);
#endif
}

void Console::setConsoleForegroundColor(Console::Color foreColor)
{
#ifdef WIN32
  switch (foreColor) {
  case I3D::Console::Color::BLACK:
    mForeColor = 0;
    break;
  case I3D::Console::Color::BLUE:
    mForeColor = FOREGROUND_BLUE;
    break;
  case I3D::Console::Color::GREEN:
    mForeColor = FOREGROUND_GREEN;
    break;
  case I3D::Console::Color::CYAN:
    mForeColor = FOREGROUND_GREEN | FOREGROUND_BLUE;
    break;
  case I3D::Console::Color::RED:
    mForeColor = FOREGROUND_RED;
    break;
  case I3D::Console::Color::MAGENTA:
    mForeColor = FOREGROUND_RED | FOREGROUND_BLUE;
    break;
  case I3D::Console::Color::YELLOW:
    mForeColor = FOREGROUND_GREEN | FOREGROUND_RED;
    break;
  case I3D::Console::Color::WHITE:
    mForeColor = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED;
    break;
  default:
    mForeColor = 0;
    break;
  }
#endif

  update();
}

void Console::setConsoleBackgroundColor(Console::Color backColor)
{
#ifdef WIN32
  switch (backColor) {
  case I3D::Console::Color::BLACK:
    mBackColor = 0;
    break;
  case I3D::Console::Color::BLUE:
    mBackColor = BACKGROUND_BLUE;
    break;
  case I3D::Console::Color::GREEN:
    mBackColor = BACKGROUND_GREEN;
    break;
  case I3D::Console::Color::CYAN:
    mBackColor = BACKGROUND_GREEN | BACKGROUND_BLUE;
    break;
  case I3D::Console::Color::RED:
    mBackColor = BACKGROUND_RED;
    break;
  case I3D::Console::Color::MAGENTA:
    mBackColor = BACKGROUND_RED | BACKGROUND_BLUE;
    break;
  case I3D::Console::Color::YELLOW:
    mBackColor = BACKGROUND_GREEN | BACKGROUND_RED;
    break;
  case I3D::Console::Color::WHITE:
    mBackColor = BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED;
    break;
  default:
    mBackColor = 0;
    break;
  }
#endif
  update();
}

void Console::setConsoleUnicode() 
{
#ifdef WIN32
  SetConsoleOutputCP(1252);     
  SetConsoleCP(1252);
#endif
}

void Console::init(DWORD handle) 
{ 
#ifdef WIN32
  h = GetStdHandle(handle);
  CONSOLE_SCREEN_BUFFER_INFO info; 
  if (! GetConsoleScreenBufferInfo(h, &info)) {
    mOldColorAttrs = 0x0007;
  } else {
    mOldColorAttrs = info.wAttributes; 
  }
  mForeColor = (mOldColorAttrs & 0x0007);
  mBackColor = (mOldColorAttrs & 0x0070);
#endif
}

void Console::update()
{
  SetConsoleTextAttribute(h, mForeColor | mBackColor);
}

/* ---------------------------------------------------------------------------------- */

void CmdParser::addParameter(const char *name, const char *description, bool optional, const char *defValue)
{
  mCmdArgs.push_front(std::make_shared<CmdParameter>(name, description, optional, defValue));
}


void CmdParser::addParameterOption(const char *name, const char *options, const char *description, bool optional, const char *defValue)
{
  mCmdArgs.push_front(std::make_shared<CmdParameterOptions>(name, options, description, optional, defValue));
}


void CmdParser::addOption(const char *name, const char *description, bool optional)
{
  mCmdArgs.push_back(std::make_shared<CmdOption>(name, description, optional));
}

CmdParser::MSG CmdParser::parse(int argc, const char* const argv[])
{
  for (auto arg : mCmdArgs) {
    bool bOptional = arg->isOptional();
    std::string argName = (arg->getType() == ArgType::OPTION) ? "-" : "--";
    argName += arg->getName();
    bool bFind = false;
    for (int i = 1; i < argc; ++i) {
      std::string arg_name = std::string(argv[i]);
      std::size_t found = arg_name.find(argName);
      if (found != std::string::npos) {
        if (arg->getType() == ArgType::OPTION) {
          dynamic_cast<CmdOption *>(arg.get())->setOption(true);
          break;
        } else if (arg->getType() == ArgType::PARAMETER) {
          std::size_t val_pos = arg_name.find("=",found);
          std::string value = arg_name.substr(val_pos+1, arg_name.size() - val_pos);
          dynamic_cast<CmdParameter *>(arg.get())->setValue(value);
          bFind = true;
          break;
        } else if (arg->getType() == ArgType::PARAMETER_OPTIONS) {
          ;
        }

      }
    }
    // Ver si no es opcional y devolver un error si no existe
    if (bFind == false && bOptional == false) return CmdParser::MSG::PARSE_ERROR;
  }
  return CmdParser::MSG::PARSE_SUCCESS;
}

void CmdParser::printHelp()
{

  //... Solución rapida. modificar

  //bprinter::TablePrinter tp(&std::cout);
  //tp.AddColumn("Name", 25);
  //tp.AddColumn("Optional", 10);
  //tp.AddColumn("Type", 10);
  //tp.AddColumn("Description", 60);
  //tp.PrintHeader();
  //for (auto arg : mCmdArgs) {
  //  tp << arg->getName() 
  //     << (arg->isOptional() ? "O" : "R")
  //     << ((ArgType::OPTION == arg->getType()) ? "Option" : "Parameter")
  //     << arg->getDescription();
  //}
  //tp.PrintFooter();
  printf_s("%s: %s \n", mCmdName.c_str(), mCmdDescription.c_str());
  for (auto arg : mCmdArgs) {
     printf_s("%s [%s | %s]: %s \n", arg->getName().c_str(), ((ArgType::OPTION == arg->getType())? "Option" : "Parameter"), (arg->isOptional() ? "O" : "R"),arg->getDescription().c_str());
  }
  printf_s("Using:\n");
  printf_s("%s", mCmdName.c_str());
  for (auto arg : mCmdArgs) {
    printf_s( " %s%s%s", ((ArgType::OPTION == arg->getType())? "-" : "--"), arg->getName().c_str(), ((ArgType::OPTION == arg->getType())? "" : "=[value]"));
  }
  printf_s("\n");
}

bool CmdParser::hasOption(const std::string &option) const
{
  for (auto arg : mCmdArgs) {
    if (arg->getType() == ArgType::OPTION) {
      if (arg->getName() == option) {
        return true;
      }
    }
  }
  return false;
}

/* ---------------------------------------------------------------------------------- */

bool Progress::operator()(double increment) 
{ 
  if (mProgress == 0) initialize();
  mProgress += increment;
  int percent = I3D_ROUND_TO_INT(mProgress * mScale);
  if (percent > mPercent) {
    mPercent = percent;
    update();
  }
  if (mProgress == mMaximun) terminate();
  return true;
}

void Progress::update() 
{
  if (!onProgress._Empty()) onProgress(mPercent);
}

void Progress::restart()
{
  mPercent = 0;
  mProgress = 0.;
}

void Progress::init(double min, double max, std::string msg)
{
  mMinimun = min;
  mMaximun = max;
  mMsg = msg;
  restart();
  updateScale();
}

void Progress::initialize()
{
  printf(mMsg.c_str());
  printf("\n");
  if (!onInitialize._Empty()) onInitialize();
}

void Progress::terminate()
{
  printf("\n");
  if (!onTerminate._Empty()) onTerminate();
}

void Progress::updateScale()
{
  mScale = 100./(mMaximun - mMinimun);
}

void Progress::setOnProgressListener(std::function<void(double)> &progressFunction)
{
  onProgress = progressFunction;
}

void Progress::setOnInitializeListener(std::function<void(void)> &initializeFunction)
{
  onInitialize = initializeFunction;
}

void Progress::setOnTerminateListener(std::function<void(void)> &terminateFunction)
{
  onTerminate = terminateFunction;
}

/* ---------------------------------------------------------------------------------- */

void ProgressBar::update() 
{
  if (onProgress._Empty()) {
    
    cout << "\r";

    Console console(Console::Mode::OUTPUT);
  //  if (bCustomConsole)
  //    console.setConsoleBackgroundColor(Console::Color::GREEN);
    int posInBar = I3D_ROUND_TO_INT(mPercent * mSize / 100.);

		//for (int a = 0; a < posInBar; a++) {
  //    if (bCustomConsole) {
  //      cout << " ";
  //    } else {
  //      cout << "#";
  //    }
		//}

  //  if (bCustomConsole) console.setConsoleBackgroundColor(Console::Color::YELLOW);

		//for (int b = 0; b < mSize - posInBar; b++) {
  //    if (bCustomConsole) {
  //      cout << " ";
  //    } else {
  //      cout << "-";
  //    }
		//}
    int ini = mSize / 2 - 2;
    for (int i = 0; i < mSize; i++) {
      if (i < posInBar) {
        if (bCustomConsole) {
          console.setConsoleBackgroundColor(Console::Color::GREEN);
          //cout << " ";
        } else {
          cout << "#";
        }
      } else {
        if (bCustomConsole) {
          console.setConsoleBackgroundColor(Console::Color::YELLOW);
          //cout << " ";
        } else {
          cout << "-";
        }
      }
      if (bCustomConsole) {
        int nDigits = posInBar > 0 ? (int) log10 ((double) posInBar) + 1 : 1;
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
    onProgress(mPercent);
  }
}

/* ---------------------------------------------------------------------------------- */

void ProgressPercent::update() 
{
  if (onProgress._Empty()) {
    cout << "\r";
    cout << " " << mPercent << "%  completed" << flush;
  } else
    onProgress(mPercent);
}

} // End mamespace I3D