#ifndef I3D_CONSOLE_H
#define I3D_CONSOLE_H

//#if defined WIN32
//// Para que no den problemas std::numeric_limits<T>().max() 
//#define NOMINMAX 
//# include <windows.h>
//#endif
#include <functional>
#include <map>
#include <vector>
#include <list>
#include <memory>
#include <iostream>
#include <sstream>

#include "core/defs.h"
#include "core/utils.h"

namespace I3D
{

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Clase para gestionar la configuración de la ventana de comandos
 */
class I3D_EXPORT Console
{
public:

  enum class Intensity : int8_t{
    NORMAL,
    BRIGHT
  };
  
  /*!
   * \brief Tipos de color de fondo y caracter.
   */
  enum class Color : int8_t {
    BLACK,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE
  };

  /*!
   * \brief Modo de consola
   */
  enum class Mode : int8_t {
    INPUT,
    OUTPUT,
    OUTPUT_ERROR
  };

private:

#ifdef WIN32

  /*!
   * \brief Manejador de la consola 
   */
  HANDLE h;

  /*!
   * \brief Intensidad de caracter
   */
  WORD mForeIntensity;

  /*!
   * \brief Color de caracteres
   */
  WORD mForeColor;

  /*!
   * \brief Intensidad de fondo
   */
  WORD mBackIntensity;

  /*!
   * \brief Color de fondo
   */
  WORD mBackColor;

  /*!
   * \brief Configuración de la consola al iniciar. Se recupera al salir o con el método reset
   */
  WORD mOldColorAttrs;

#else

  FILE *mStream;

  char mCommand[13];

  /*!
   * \brief Intensidad de caracter
   */
  int mForeIntensity;

  /*!
   * \brief Color de caracteres
   */
  int mForeColor;

  /*!
   * \brief Intensidad de fondo
   */
  int mBackIntensity;

  /*!
   * \brief Color de fondo
   */
  int mBackColor;
  
#endif

public:

  /*!
   * \brief Constructora por defecto
   */
  Console();

  /*!
   * \brief Constructora
   * \param mode Modo de consola
   */
  Console(Console::Mode mode);

  /*!
   * Destructora
   */
  ~Console();

  /*!
   * \brief Recupera los valores iniciales
   */
  void reset();

  /*!
   * \brief Establece el color de caracter
   * \param foreColor Color
   */
  void setConsoleForegroundColor(Console::Color foreColor, Console::Intensity intensity = Console::Intensity::NORMAL);

  /*!
   * \brief Establece el color de fondo
   * \param backColor Color
   */
  void setConsoleBackgroundColor(Console::Color backColor, Console::Intensity intensity = Console::Intensity::NORMAL);

  /*!
   * \brief setConsoleUnicode
   */
  void setConsoleUnicode();

private:

#ifdef WIN32
  /*!
   * \brief Inicializa la consola guardando la configuración  actual.
   * \param handle
   */
  void init( DWORD handle );
#else
  /*!
   * \brief Inicializa la consola guardando la configuración  actual.
   */
  void init(FILE *stream);
#endif

  /*!
   * \brief Actualiza la consola
   */
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

enum class ArgType : int8_t {
  OPTION,
  PARAMETER,
  PARAMETER_OPTIONS
};

/*!
 * \brief The CmdArgument struct
 */
struct I3D_EXPORT CmdArgument
{
protected:

  /*!
   * \brief mName
   */
  std::string mName;

  /*!
   * \brief mDescription
   */
  std::string mDescription;

  /*!
   * \brief bOptional
   */
  bool bOptional;

public:

  /*!
   * \brief CmdArgument
   * \param name
   * \param description
   * \param optional
   */
  CmdArgument(const char *name, const char *description, bool optional = false) 
    : mName(name), mDescription(description), bOptional(optional) {}
  
  /*!
   * \brief CmdArgument
   */
  CmdArgument(CmdArgument const&) = delete;

  /*!
   * \brief operator =
   */
  void operator=(CmdArgument const&) = delete;

  /*!
   * \brief getName
   * \return
   */
  const std::string &getName() const { return mName; }

  /*!
   * \brief getDescription
   * \return
   */
  const std::string &getDescription() const { return mDescription; }

  /*!
   * \brief isOptional
   * \return
   */
  bool isOptional() const { return bOptional; }

  /*!
   * \brief getType
   * \return
   */
  virtual ArgType getType() const = 0;
};

struct I3D_EXPORT CmdOption : public CmdArgument
{
private:

  /*!
   * \brief mValue
   */
  bool mValue;

public:

  /*!
   * \brief CmdOption
   * \param name
   * \param description
   * \param optional
   */
  CmdOption(const char *name, const char *description, bool optional = false)
    : CmdArgument(name, description, optional), mValue(false) {}

  /*!
   * \brief getType
   * \return
   */
  ArgType getType() const override { return ArgType::OPTION; }

  /*!
   * \brief isActive
   * \return
   */
  bool isActive() const { return mValue; }

  /*!
   * \brief setOption
   * \param option
   */
  void setOption(bool option) { mValue = option; }
};

/*!
 * \brief The CmdParameter struct
 */
struct I3D_EXPORT CmdParameter : public CmdArgument
{
private:

  /*!
   * \brief mDefValue
   */
  std::string mDefValue;

public:

  /*!
   * \brief CmdParameterString
   * \param name
   * \param description
   * \param optional
   */
  //CmdParameter(std::string name, std::string description, bool optional)
  //  : CmdArgument(name, description, optional)
  //{
  //  mDefValue = "";
  //}

  /*!
   * \brief CmdParameterString
   * \param name
   * \param description
   * \param optional
   * \param defValue
   */
  CmdParameter(const char *name, const char *description, bool optional = false, const char *defValue = "") 
    : CmdArgument(name, description, optional) 
  {
    mDefValue = defValue;
  }

  /*!
   * \brief getType
   * \return
   */
  ArgType getType() const override { return ArgType::PARAMETER; }

  /*!
   * \brief getValue
   * \return
   */
  std::string getValue() const { return mDefValue; }

  /*!
   * \brief setValue
   * \param value
   */
  void setValue(std::string value) { mDefValue = value; }
};


/*!
 * \brief Un parametro que toma un valor de una lista predefinida de opciones
 */
struct I3D_EXPORT CmdParameterOptions : public CmdArgument
{

private:

  /*!
   * \brief mDefValue
   */
  std::string mDefValue;

  /*!
   * \brief listado de opciones
   */
  std::vector<std::string> mOptions;

public:

  /*!
   * \brief CmdParameterString
   * \param name
   * \param description
   * \param optional
   */
  //CmdParameterOptions(std::string name, std::string options, std::string description, bool optional)
  //  : CmdArgument(name, description, optional)
  //{
  //  mDefValue = "";
  //  split(options, mOptions, ",");
  //}

  /*!
   * \brief CmdParameterString
   * \param name
   * \param description
   * \param optional
   * \param defValue
   */
  CmdParameterOptions(const char *name, const char *options, const char *description, bool optional = false, const char *defValue = "") 
    : CmdArgument(name, description, optional), mDefValue(defValue)
  {
    split(options, mOptions, ",");
  }

  /*!
   * \brief getType
   * \return
   */
  ArgType getType() const override;

  /*!
   * \brief getValue
   * \return
   */
  std::string getValue() const;

  /*!
   * \brief Indice de opción
   * \return Indice
   */
  int getIndex(std::string value) const;

  /*!
   * \brief Indice de opción seleccionada
   * \return Indice
   */
  int getIndex() const;

  /*!
   * \brief setValue
   * \param value
   */
  void setValue(std::string value);

  /*!
   * \brief listado de opciones
   */
  std::vector<std::string> getOptions() { return mOptions; }
};

// Parametros de entrada:
// in<<c:/in.txt
// Parametros de salida
// out<<c:/out.txt
// Opciones:
// -b
// Opción con valores predefinidos
// -a=VALUE2
// Parametro
// --p=value

/*!
 * \brief Parser de los argumentos de entrada  de la consola
 */
class I3D_EXPORT CmdParser
{
public:

  enum class MSG
  {
    PARSE_SUCCESS,
    PARSE_ERROR
  };

private:

  std::string mCmdName;

  std::string mCmdDescription;

  /*!
   * \brief Listado de los argumentos
   */
  std::list<std::shared_ptr<CmdArgument>> mCmdArgs;

  //... Seria conveniente tener un mensaje de help y otro de copyright

public:

  /*!
   * \brief Constructora por defecto
   */
  CmdParser() {}

  CmdParser(const char *name, const char *description)
    : mCmdName(name), mCmdDescription(description) {}

  /*!
   * \brief Constructor de lista
   * \param[in] Listado argumentos
   */
  CmdParser(const char *name, const char *description, std::initializer_list<std::shared_ptr<CmdArgument>> cmd_args)
    : mCmdName(name), mCmdDescription(description), mCmdArgs(cmd_args) {}
  
  /*!
   * \brief Destructora
   */
  ~CmdParser(){}

  void addParameter(const char *name, const char *description, bool optional = false, const char *defValue = "");

  void addParameterOption(const char *name, const char *options, const char *description, bool optional = false, const char *defValue = "");

  void addOption(const char *name, const char *description, bool optional = false);

  /*!
   * \brief parsea los argumentos de entrada
   */
  CmdParser::MSG parse(int argc, const char* const argv[]);

  /*!
   * \brief Muestra por consola la ayuda del programa
   */
  void printHelp();

  /*!
   * \brief Limpia el parser de comando
   */
  void clear() { mCmdArgs.clear(); }
  
  /*!
   * \brief Devuelve el valor de un parametro
   * param[in] name Nombre del parámetro
   * return Valor del parámetro en el tipo indicado
   */
  template<typename T>
  T getValue( const char *name) const
  { 
    T t = T();
    void *_value = (void *)&t;

    std::string _name(name);
    for (auto arg : mCmdArgs) {
      if (arg->getType() == ArgType::PARAMETER || arg->getType() == ArgType::PARAMETER_OPTIONS) {
        if (arg->getName() == _name) {
          std::string value = (arg->getType() == ArgType::PARAMETER) ? 
            dynamic_cast<CmdParameter *>(arg.get())->getValue() : dynamic_cast<CmdParameterOptions *>(arg.get())->getValue();
          std::stringstream strm_value(value);
          if (typeid(T) == typeid(std::string)) {
            *(std::string *)_value = value;
          } else if (typeid(T) == typeid(int)) {
            strm_value >> *(int *)_value;
          } else if (typeid(T) == typeid(double)) {
            strm_value >> *(double *)_value;
          } else if (typeid(T) == typeid(float)) {
            strm_value >> *(float *)_value;
          } else {
            throw std::runtime_error("Tipo de dato  no permitido");
          }
        }
      }
    }
    return t;
  }

  /*!
   * \brief Devuelve el indice de un parámetro 
   * param[in] name Nombre del parámetro
   * return Valor del parámetro en el tipo indicado
   */
  template<typename T>
  T getParameterOptionIndex(const char *name) const
  {
    std::string _name(name);
    for (auto arg : mCmdArgs) {
      if ( arg->getType() == ArgType::PARAMETER_OPTIONS ) {
        if ( arg->getName() == _name ) {
          CmdParameterOptions *cpo = dynamic_cast<CmdParameterOptions *>(arg.get());
          return static_cast<T>(cpo->getIndex());
        }
      }
    }
    return static_cast<T>(0);
  }
  
  bool hasOption( const std::string &option) const;
};


/* ---------------------------------------------------------------------------------- */

//// Eventos de la clase de progreso
//
///*!
// * brief Evento que se produce cada vez que se
// * avanza una posición en la función de progreso
// */
//class OnProgress : public Event
//{
//public:
//  OnProgress() : Event("Progress") {}
//
//  ~OnProgress() {}
//};
//
///*!
// * \brief Evento que se ejecuta al inicializar la función de 
// * progreso
// */
//class OnInitialize : public Event
//{
//public:
//  OnInitialize() : Event("Initialize") {}
//
//  ~OnInitialize() {}
//
//private:
//
//};
//
///*!
// * \brief Evento que se ejecuta al terminar la función de progreso
// */
//class OnTerminate : public Event
//{
//public:
//  OnTerminate() : Event("Terminate") {}
//
//  ~OnTerminate() {}
//};
//
//
//class ProgressEvents
//{
//private:
//
//
//public:
//  ProgressEvents()  {}
//
//  ~ProgressEvents()  {}
//
//  virtual void onProgress(double progress) = 0;
//
//  virtual void onInitialize() = 0;
//
//  virtual void onTerminate() = 0;
//
//  void fire(Event _event);
//};

/*!
 * \brief Función objeto de progreso
 */
class I3D_EXPORT Progress
{
protected:

  /*!
   * \brief Valor actual
   */
  double mProgress;

  /*!
   * \brief Valor mínimo
   */
  double mMinimun;
  
  /*!
   * \brief Valor máximo
   */
  double mMaximun;

  /*!
   * \brief Valor actual en tanto por ciento 
   */
  int mPercent; 

  /*!
   * brief Mensaje que se puede añadir con información del proceso.
   */
  std::string mMsg;

  /*!
   * brief Manejador del evento que se produce cada vez que se
   * avanza una posición en la función de progreso
   */
  std::function<void(double)> *onProgress;

  /*!
   * \brief Manejador del evento que se ejecuta al inicializar
   */
  std::function<void(void)> *onInitialize;

  /*!
   * \brief Manejador del evento que se ejecuta al terminar
   */
  std::function<void(void)> *onTerminate;

  /*!
   * \brief Escala
   */
  double mScale;

  //typedef ProgressEvents Listener;

  //std::list<Listener *> mListener;

public:

  /*!
   * \brief Constructora por defecto
   */
  Progress() : mProgress(0.), mMinimun(0.), mMaximun(100.), mPercent(-1), mMsg("")
  {
    onProgress = NULL;
    onInitialize = NULL;
    onTerminate = NULL;
    updateScale();
  }

  /*!
   * \brief Constructora
   */
  Progress(double min, double max, std::string msg = "") : mProgress(0.), mMinimun(min), mMaximun(max), mPercent(-1), mMsg(msg)
  {
    onProgress = NULL;
    onInitialize = NULL;
    onTerminate = NULL;
    updateScale();
  }

  /*!
   * \brief Destructora
   */
  virtual ~Progress() {}

  /*!
   * \brief Operador de llamada. Se llama cada vez que se quiera avanzar en
   * la función de progreso
   */
  bool operator() (double increment = 1.);

  /*!
   * \brief Inicializa el progreso
   * \param min Valor minimo
   * \param max
   * \param msg
   */
  void init(double min, double max, std::string msg = "");

  /*!
   * \brief Restablece los valores al inicio
   */
  void restart();

  /*!
   * \brief Establece el manejador del evento OnInitialize
   * \param initializeFunction Función que se llama al inicializar
   */
  void setOnInitializeListener(std::function<void(void)> &initializeFunction);

  /*!
   * \brief Establece el manejador del evento OnProgress
   * \param progressFunction Función de control del progreso
   */
  void setOnProgressListener(std::function<void(double)> &progressFunction);
  
  /*!
   * \brief Establece el manejador del evento OnTerminate
   * \param terminateFunction Función que se llama al terminar
   */
  void setOnTerminateListener(std::function<void(void)> &terminateFunction);

  //void addListener(Listener *listener) 
  //{ 
  //  mListener.push_front( listener );
  //}

protected:

  /*!
   * \brief initialize
   */
  void initialize();

  /*!
   * \brief
   */
  virtual void update();

  /*!
   * \brief updateScale
   */
  void updateScale();

  /*!
   * \brief terminate
   */
  void terminate();
};


/*!
 * \brief Barra de progreso de consola
 */
class I3D_EXPORT ProgressBar : public Progress
{
private:

  /*!
   * \brief bCustomConsole
   */
  bool bCustomConsole;

  /*!
   * \brief Longitud de la barra de progreso
   */
  const int mSize = 50;

public:

  /*!
   * \brief Constructora
   * \param customConsole
   */
  ProgressBar(bool customConsole = true) 
    : Progress(), bCustomConsole(customConsole) {}

  /*!
   * \brief Constructora
   * \param min Valor mínimo
   * \param max
   * \param customConsole
   */
  ProgressBar(double min, double max, bool customConsole = true) 
    : Progress(min, max), bCustomConsole(customConsole) {}

  /*!
   * \brief Destructora
   */
  ~ProgressBar() {}

  //... warning C4512: 'I3D::ProgressBar' : no se pudo generar el operador de asignaciones
  //    Este warning aparece debido a que mSize es constante. impido la asignación que por
  //    otra parte tampoco me interesa 

  ProgressBar &operator=(const ProgressBar &pb) = delete;

private:

  /*!
   * \brief Actualiza la barra de progreso
   */
  void update() override;
};

/*!
 * \brief Progreso en porcentaje
 */
class I3D_EXPORT ProgressPercent : public Progress
{
private:

  /*!
   * \brief bCustomConsole
   */
  bool bCustomConsole;

public:

  /*!
   * \brief Constructora
   * \param customConsole
   */
  ProgressPercent(bool customConsole = false) 
    : Progress(), bCustomConsole(customConsole) {}

  /*!
   * \brief Constructora
   * \param min Valor mínimo
   * \param max Valor máximo
   * \param customConsole
   */
  ProgressPercent(double min, double max, bool customConsole = false) 
    : Progress(min, max), bCustomConsole(customConsole) {}

  /*!
   * \brief Destructora
   */
  ~ProgressPercent() {}

private:

  /*!
   * \brief Actualiza la barra de progreso
   */
  void update() override;
};

} // End namespace I3D

#endif // I3D_CONSOLE_H
