#ifndef I3D_CONSOLE_H
#define I3D_CONSOLE_H

#include <functional>
#include <map>
#include <vector>

#include "defs.h"

//Namespace:
using namespace std;

namespace I3D
{

/* ---------------------------------------------------------------------------------- */

enum class console_color : int8_t {
  BLACK,
  BLUE,
  GREEN,
  CYAN,
  RED,
  MAGENTA,
  YELLOW,
  WHITE
};

enum class console_mode : int8_t {
  INPUT,
  OUTPUT,
  OUTPUT_ERROR
};

class I3D_EXPORT Console
{
private:
#ifdef WIN32
  /*!
   * \brief Manejador de la consola 
   */
  HANDLE h;

  WORD mForeColor;

  WORD mBackColor;

  WORD mOldColorAttrs;

#else
  //... Código para otros sistemas operativos
#endif

public:

  Console();

  Console(console_mode mode);

  ~Console();

  void reset();

  void setConsoleForegroundColor(console_color foreColor);

  void setConsoleBackgroundColor(console_color backColor);

private:

  void init( DWORD handle );

  void update();

};

/* ---------------------------------------------------------------------------------- */

// http://www.gnu.org/software/libc/manual/html_node/Argument-Syntax.html#Argument-Syntax

// POSIX
// Los argumentos son opciones si comienzan con un guión (‘-’).
// Múltiples opciones pueden seguir un delimitador de guión en un único token si las opciones no tienen argumentos. Asi '-abc' es equivalente a '-a -b -c'.
// Option names are single alphanumeric characters (as for isalnum; see Classification of Characters).
// Ciertas opciones requieren un argumento. Por ejemplo, el '-o' comando del comando ld requiere un argumento nombre-archivo de salida.
// Una opción y su argumento pueden o no pueden aparecer como fichas separadas. (En otras palabras, el espacio en blanco que los separa es opcional.) Por lo tanto, '-o foo' y '-ofoo' son equivalentes.
// 
//GNU añade opciones de larga duración a estas convenciones. Las opciones largas consisten en '-' seguido de un nombre hecho de caracteres alfanuméricos y guiones. Los nombres de opciones son por lo general de una a tres palabras de largo, con guiones para separar las palabras. Los usuarios pueden abreviar los nombres de las opciones, siempre y cuando las abreviaturas son únicos.
//
//Para especificar un argumento para una larga, debe escribirse '--name = valor'. Esta sintaxis permite una opción a largo para aceptar un argumento que es en sí opcional.

struct CmdOption
{
private:
  std::string mOption;
public:
  CmdOption(std::string opt) { mOption = opt; }
};

struct CmdParameter
{
private:
  std::string option;
public:
  CmdParameter(std::string option) : option(option) {}
};

struct CmdParameterInt : public CmdParameter
{
private:
  int *value;
public:
  CmdParameterInt(std::string option, int *value) : CmdParameter(option), value(value) {}
};

struct CmdParameterDouble : public CmdParameter
{
private:
  double *value;
public:
  CmdParameterDouble(std::string option, double *value) : CmdParameter(option), value(value) {}
};

struct CmdParameterFloat : public CmdParameter
{
private:
  float *value;
public:
  CmdParameterFloat(std::string option, float *value) : CmdParameter(option), value(value) {}
};

struct CmdParameterString : public CmdParameter
{
private:
  std::string *value;
public:
  CmdParameterString(std::string option, std::string *value) : CmdParameter(option), value(value) {}
};

class CmdParser
{
public:
  std::vector<CmdOption> options;
  std::vector<CmdParameter> parameters;

private:

  //int argc;

  //char** argv;

  //std::vector<string> options;

  //std::map<std::string, std::string> parameters;

public:
  CmdParser( int argc, const char* const argv[] );
  ~CmdParser();

private:

};


/* ---------------------------------------------------------------------------------- */

class I3D_EXPORT Progress
{
protected:

  /*!
   * \brief Valor actual
   */
  double mProgress;

  /*!
   * \brief Valor actual en tanto por ciento 
   */
  int mPercent; 

  /*!
   * \brief Valor mínimo
   */
  double mMinimun;
  
  /*!
   * \brief Valor máximo
   */
  double mMaximun;

  // Se podrian incluir mas eventos. Al iniciar, al finalizar...

  /*!
   * brief función progreso para la implementación personalizada
   */
  std::function<void(double)> onProgress;

  double mScale;

public:

  /*!
   * \brief Constructora por defecto
   */
  Progress() : mProgress(0.), mMinimun(0.), mMaximun(100.), mPercent(-1)
  {
    mScale = (mMaximun - mMinimun) / 100.;
  }

  /*!
   * \brief Constructora por defecto
   */
  Progress(double min, double max) : mProgress(0.), mMinimun(min), mMaximun(max), mPercent(-1)
  {
    mScale = (mMaximun - mMinimun) / 100.;
  }

  /*!
   * \brief Destructora
   */
  virtual ~Progress() {}

  /*!
   * \brief Operador de asignación
   */
  bool operator()(double increment = 1.);

  /*!
   * \brief 
   */
  void setListener(std::function<void(int)> progressFunction);

  void restart();

protected:

  /*!
   * \brief
   */
  virtual void update();

  void terminate();
};


class I3D_EXPORT ProgressBar : public Progress
{
private:
  bool bCustomConsole;

  /*!
   * \brief Longitud de la barra de progreso
   */
  const int mSize = 50;

public:

  ProgressBar(bool customConsole = false) 
    : Progress(), bCustomConsole(customConsole) {}

  ProgressBar(double min, double max, bool customConsole = false) 
    : Progress(min, max), bCustomConsole(customConsole) {}

  ~ProgressBar() {}

private:
  void update() override;
};

class I3D_EXPORT ProgressPercent : public Progress
{
private:

  bool bCustomConsole;

public:

  ProgressPercent(bool customConsole = false) 
    : Progress(), bCustomConsole(customConsole) {}

  ProgressPercent(double min, double max, bool customConsole = false) 
    : Progress(min, max), bCustomConsole(customConsole) {}

  ~ProgressPercent() {}

private:
  void update() override;
};

} // End namespace I3D

#endif // I3D_CONSOLE_H
