#if defined WIN32
# include <windows.h>
#endif
#include <iostream> //For I/O
#include <string> //For strings
#include <ctime> //For demo

#include "console.h"
#include "utils.h"



using namespace I3D;

namespace I3D
{

Console::Console() 
{ 
#ifdef WIN32
  init(STD_OUTPUT_HANDLE);
#endif
}

Console::Console(console_mode mode)
{ 
#ifdef WIN32
  DWORD handle;
  switch (mode) {
  case I3D::console_mode::INPUT:
    handle = STD_INPUT_HANDLE;
    break;
  case I3D::console_mode::OUTPUT:
    handle = STD_OUTPUT_HANDLE;
    break;
  case I3D::console_mode::OUTPUT_ERROR:
    break;
    handle = STD_ERROR_HANDLE;
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

void Console::setConsoleForegroundColor(console_color foreColor)
{
#ifdef WIN32
  switch (foreColor) {
  case I3D::console_color::BLACK:
    mForeColor = 0;
    break;
  case I3D::console_color::BLUE:
    mForeColor = FOREGROUND_BLUE;
    break;
  case I3D::console_color::GREEN:
    mForeColor = FOREGROUND_GREEN;
    break;
  case I3D::console_color::CYAN:
    mForeColor = FOREGROUND_GREEN | FOREGROUND_BLUE;
    break;
  case I3D::console_color::RED:
    mForeColor = FOREGROUND_RED;
    break;
  case I3D::console_color::MAGENTA:
    mForeColor = FOREGROUND_RED | FOREGROUND_BLUE;
    break;
  case I3D::console_color::YELLOW:
    mForeColor = FOREGROUND_GREEN | FOREGROUND_RED;
    break;
  case I3D::console_color::WHITE:
    mForeColor = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED;
    break;
  default:
    mForeColor = 0;
    break;
  }
#endif

  update();
}

void Console::setConsoleBackgroundColor(console_color backColor)
{
#ifdef WIN32
  switch (backColor) {
  case I3D::console_color::BLACK:
    mBackColor = 0;
    break;
  case I3D::console_color::BLUE:
    mBackColor = BACKGROUND_BLUE;
    break;
  case I3D::console_color::GREEN:
    mBackColor = BACKGROUND_GREEN;
    break;
  case I3D::console_color::CYAN:
    mBackColor = BACKGROUND_GREEN | BACKGROUND_BLUE;
    break;
  case I3D::console_color::RED:
    mBackColor = BACKGROUND_RED;
    break;
  case I3D::console_color::MAGENTA:
    mBackColor = BACKGROUND_RED | BACKGROUND_BLUE;
    break;
  case I3D::console_color::YELLOW:
    mBackColor = BACKGROUND_GREEN | BACKGROUND_RED;
    break;
  case I3D::console_color::WHITE:
    mBackColor = BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED;
    break;
  default:
    mBackColor = 0;
    break;
  }
#endif
  update();
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

CmdParser::CmdParser( int argc, const char* const argv[] )
{
  for ( int iArg = 1; iArg < argc; ++iArg ) {
    if ( stricmp(argv[iArg],"--") == 0 ) {
      // Es un parámetro

    } else {
      // Es una opción

  //    if ( filein == NULL ) {
  //      filein = argv[iArg];
  //    } else if ( fileout == NULL ) {
  //      fileout = argv[iArg];      
  //    }  
    }
  }
}

CmdParser::~CmdParser()
{}

/* ---------------------------------------------------------------------------------- */

bool Progress::operator()(double increment) 
{ 
  mProgress += increment;
  int percent = static_cast<int>(round(mProgress / mScale));
  if (percent > mPercent) {
    mPercent = percent;
    update();
  }
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

void Progress::terminate()
{
  // Aqui se debería lanzar un evento indicando que se ha terminado
}

/* ---------------------------------------------------------------------------------- */

void ProgressBar::update() 
{
  if (onProgress._Empty()) {
    
    cout << "\r";
    //cout << "[";

    Console console(console_mode::OUTPUT);
    if (bCustomConsole)
      //console.setConsoleForegroundColor(console_color::GREEN);
      console.setConsoleBackgroundColor(console_color::GREEN);
    int posInBar = static_cast<int>(round(mPercent * mSize / 100.));
		
		for (int a = 0; a < posInBar; a++) {
      if (bCustomConsole)
        cout << " ";
      else 
        cout << "#";
		}
    if (bCustomConsole)
      //console.setConsoleForegroundColor(console_color::YELLOW);
      console.setConsoleBackgroundColor(console_color::YELLOW);
		for (int b = 0; b < mSize - posInBar; b++) {
      if (bCustomConsole)
        cout << " ";
      else 
        cout << "-";
		}
    if (bCustomConsole) console.reset();
    cout /*<< "]"*/ << " " << mPercent << "%  completed" << flush;
  } else
    onProgress(mPercent);
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