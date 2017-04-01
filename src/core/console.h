#ifndef I3D_CONSOLE_H
#define I3D_CONSOLE_H

#include <functional>
#include <map>
#include <vector>
#include <list>
#include <memory>
#include <iostream>
#include <sstream>

#include "core/defs.h"
#include "core/utils.h"
#include "core/messages.h"

namespace I3D
{

/* ---------------------------------------------------------------------------------- */

/*! \addtogroup utilities
 *  \{
 */

/*! \defgroup Console Utilidades de consola
 *
 *  \{
 */

/*!
 * \brief Clase para gestionar la configuración de la ventana de comandos
 *
 * Permite modificar el color e intensidad de caracter y de fondo,
 * poner la consola en modo Unicode y cambiar el modo de consola (entrada,
 * salida, error)
 */
class I3D_EXPORT Console : public MessageManager::Listener
{
public:

  /*!
   * \brief Valores de intensidad de color
   */
  enum class Intensity : int8_t{
    NORMAL,  /*!< Normal */
    BRIGHT   /*!< Brillante */
  };

  /*!
   * \brief Tipos de color de fondo y caracter.
   */
  enum class Color : int8_t {
    BLACK,    /*!< Negro */
    RED,      /*!< Rojo */
    GREEN,    /*!< Verde */
    YELLOW,   /*!< Amarillo */
    BLUE,     /*!< Azul */
    MAGENTA,  /*!< Magenta */
    CYAN,     /*!< Cian */
    WHITE     /*!< Blanco */
  };

  /*!
   * \brief Modo de consola
   */
  enum class Mode : int8_t {
    INPUT,          /*!< Consola en modo entrada */
    OUTPUT,         /*!< Consola en modo salida */
    OUTPUT_ERROR    /*!< Consola en modo salida de errores */
  };

private:

#ifdef WIN32

  // Consola de Windows

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
   * \brief Configuración de la consola al iniciar.
   *
   * La configuración inicial se recupera al salir o
   * con el método reset
   */
  WORD mOldColorAttrs;

#else

  // Consola Linux

  /*!
   * \brief mStream
   */
  FILE *mStream;

  /*!
   * \brief mCommand
   */
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
  
  /*!
   * \brief Nivel de información de los mensajes
   *
   * Por defecto MSG_ERROR
   * \see MessageLevel
   */
  static MessageLevel sLevel;

public:

  /*!
   * \brief Constructora por defecto
   */
  Console();

  /*!
   * \brief Constructora de copia
   * \param[in] mode Modo de consola
   * \see Mode
   */
  Console(Console::Mode mode);

  /*!
   * Destructora
   * Se recuperan las opciones por defecto de la consola
   */
  ~Console();

  /*!
   * \brief Recupera los valores iniciales
   */
  void reset();

  /*!
   * \brief Establece el color de caracter
   * \param[in] foreColor Color de caracter
   * \param[in] intensity Intensidad. El valor por defecto es Intensity::NORMAL
   * \see Console::Color, Console::Intensity
   */
  void setConsoleForegroundColor(Console::Color foreColor, Console::Intensity intensity = Console::Intensity::NORMAL);

  /*!
   * \brief Establece el color de fondo
   * \param[in] backColor Color de fondo
   * \param[in] intensity Intensidad. El valor por defecto es Intensity::NORMAL
   */
  void setConsoleBackgroundColor(Console::Color backColor, Console::Intensity intensity = Console::Intensity::NORMAL);

  /*!
   * \brief Establece la consola como modo Unicode
   */
  void setConsoleUnicode();

  void setLogLevel(MessageLevel level);

  /*!
   * \brief Imprime un mensaje en la consola
   * \param[in] msg Mensaje
   */
  void printMessage(const char *msg);

  /*!
   * \brief Imprime un mensaje de error en la consola
   * \param[in] msg Mensaje
   */
  void printErrorMessage(const char *msg);

protected:

  /*!
   * \brief onMsgDebug
   * \param msg
   * \param date
   */
  void onMsgDebug(const char *msg, const char *date) override;

  /*!
   * \brief onMsgVerbose
   * \param msg
   * \param date
   */
  void onMsgVerbose(const char *msg, const char *date) override;

  /*!
   * \brief onMsgInfo
   * \param msg
   * \param date
   */
  void onMsgInfo(const char *msg, const char *date) override;

  /*!
   * \brief onMsgWarning
   * \param msg
   * \param date
   */
  void onMsgWarning(const char *msg, const char *date) override;

  /*!
   * \brief onMsgError
   * \param msg
   * \param date
   */
  void onMsgError(const char *msg, const char *date) override;

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

//enum class ArgType : int8_t {
//  OPTION,
//  PARAMETER,
//  PARAMETER_OPTIONS
//};

/*!
 * \brief The CmdArgument struct
 */
struct I3D_EXPORT CmdArgument
{
public:

  /*!
   * \brief Tipo de argumento
   */
  enum class Type : int8_t {
    OPTION,             /*!< Opción */
    PARAMETER,          /*!< Parámetro */
    PARAMETER_OPTIONS   /*!< */
  };

protected:

  /*!
   * \brief Nombre del argumento
   */
  std::string mName;

  /*!
   * \brief Descripción del argumento
   */
  std::string mDescription;

  /*!
   * \brief Argumento opcional u obligatorio
   */
  bool bOptional;

public:

  /*!
   * \brief Constructor
   * \param[in] name Nombre de argumento
   * \param[in] description Descripción
   * \param[in] optional Párametro obligatorio u opcional. Por defecto es obligatorio.
   */
  CmdArgument(const char *name, const char *description, bool optional = false)
    : mName(name), mDescription(description), bOptional(optional) {}

  virtual ~CmdArgument(){}

  CmdArgument(CmdArgument const&) = delete;
  void operator=(CmdArgument const&) = delete;

  /*!
   * \brief Devuelve el nombre del argumento
   * \return Nombre del argumento
   */
  const std::string &getName() const { return mName; }

  /*!
   * \brief Devuelve la descripción del argumento
   * \return Descripción
   */
  const std::string &getDescription() const { return mDescription; }

  /*!
   * \brief Devuelve si el argumento es opcional
   * \return
   */
  bool isOptional() const { return bOptional; }

  /*!
   * \brief Tipo
   * \return
   */
  virtual Type getType() const = 0;
};


/*!
 * \brief Opción de un comando
 */
struct I3D_EXPORT CmdOption : public CmdArgument
{
private:

  /*!
   * \brief Valor de la opción (verdadero o falso)
   */
  bool mValue;

public:

  /*!
   * \brief Constructora de la clase CmdOption
   * \param[in] name Nombre de la opción
   * \param[in] description Descripción de la opción
   */
  CmdOption(const char *name, const char *description)
    : CmdArgument(name, description, true), mValue(false) {}

  /*!
   * \brief getType
   * \return
   */
  CmdArgument::Type getType() const override { return CmdArgument::Type::OPTION; }

  /*!
   * \brief Comprueba si la opción esta activada
   * \return Verdadero si esta activada y falso en caso contrario
   */
  bool isActive() const { return mValue; }

  /*!
   * \brief Establece si esta activada o no
   * \param[in] option
   * \deprecated Use I3D::CmdOption::setActive en su lugar
   */
  I3D_DEPRECATED("Use setActive(bool active) en su lugar")
  void setOption(bool option) { mValue = option; }

  /*!
   * \brief Establece si esta activada o no
   * \param[in] option
   */
  void setActive(bool active) { mValue = active; }
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
   * \brief Constructora de la clase CmdParameter
   * \param[in] name Nombre del parámetro
   * \param[in] description Descripción del parámetro
   * \param[in] optional El parametro es opcional. Por defecto no es opcional
   * \param[in] defValue Valor por defecto
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
  CmdArgument::Type getType() const override { return CmdArgument::Type::PARAMETER; }

  /*!
   * \brief Devuelve el valor del parámetro
   * \return Valor del parámetro
   */
  std::string getValue() const { return mDefValue; }

  /*!
   * \brief Establece el valor del perámetro
   * \param[in] value Valor del parámetro
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
   * \brief Constructora de la clase CmdParameterOptions
   * \param[in] name Nombre del parámetro
   * \param[in] options Valores que puede tomar el parámetro
   * \param[in] description Descripción del parámetro
   * \param[in] optional El parametro es opcional. Por defecto no es opcional
   * \param[in] defValue Valor por defecto
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
  CmdArgument::Type getType() const override;

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

/*!
 * \brief Parser de los argumentos de entrada  de la consola
 *
 * La estructura de un comando es:
 *
 * cmdName --param1=[Value] --param2=[Value] --par_op=["Value1" o "Value2"] -opt
 *
 * Ejemplo de uso:
 *
 * \code
 * CmdParser cmdParser("cmdName", "Descripción del comando");
 * cmdParser.addParameter("param1", "Descripción del primer parámetro");
 * cmdParser.addParameter("param2", "Descripción del segundo parámetro. Este es opcional", true);
 * cmdParser.addParameterOption("par_op", "Value1,Value2", "Parámetro con una lista de valores predetermidados", true, "Value1"); // "Value1" es el valor por defecto
 * cmdParser.addOption("opt", "Opción");
 *
 * // Se parsea el comando y si da error se cierra la aplicación
 * if ( cmdParser.parse(argc, argv) == CmdParser::Status::PARSE_ERROR ) {
 *   exit(EXIT_FAILURE);
 * }
 * \endcode
 */
class I3D_EXPORT CmdParser
{
public:

  /*!
   * \brief Estado de salida del parseo del comando
   */
  enum class Status
  {
    PARSE_SUCCESS,  /*!< El parseo se ejecuto correctamente */
    PARSE_ERROR,    /*!< Ocurrio un error al ejecutar el comando */
    PARSE_HELP      /*!< Se pasa como parametro: help */
  };

private:

  /*!
   * \brief Nombre del comando
   */
  std::string mCmdName;

  /*!
   * \brief Descripción del comando
   */
  std::string mCmdDescription;

  /*!
   * \brief Listado de los argumentos del comando
   */
  std::list<std::shared_ptr<CmdArgument>> mCmdArgs;

  //... Seria conveniente tener un mensaje de help y otro de copyright

public:

  /*!
   * \brief Constructora por defecto
   */
  CmdParser() {}

  /*!
   * \brief Constructor CmdParser
   * \param[in] name Nombre del comando
   * \param[in] description Descripción del comando
   */
  CmdParser(const char *name, const char *description)
    : mCmdName(name), mCmdDescription(description) {}

  /*!
   * \brief Constructor de lista
   * \param[in] name Nombre del comando
   * \param[in] description Descripción del comando
   * \param[in] cmd_args Lista de argumentos
   */
  CmdParser(const char *name, const char *description, std::initializer_list<std::shared_ptr<CmdArgument>> cmd_args)
    : mCmdName(name), mCmdDescription(description), mCmdArgs(cmd_args) {}

  /*!
   * \brief Destructora
   */
  ~CmdParser(){}

  /*!
   * \brief Añade un parámetro
   * \param[in] name Nombre del parámetro
   * \param[in] description Descripción del parámetro
   * \param[in] optional Parametro opcional o no. Por defecto es requerido
   * \param[in] defValue Valor por defecto
   */
  void addParameter(const char *name, const char *description, bool optional = false, const char *defValue = "");

  /*!
   * \brief Añade un parámetro
   * \param[in] name Nombre del parámetro
   * \param[in] options Lista separada por comas de los valores que puede tomar el parámetro
   * \param[in] description Descripción del parámetro
   * \param[in] optional Parametro opcional o no. Por defecto es requerido
   * \param[in] defValue Valor por defecto
   */
  void addParameterOption(const char *name, const char *options, const char *description, bool optional = false, const char *defValue = "");

  /*!
   * \brief Añade una opción
   * \param[in] name Nombre de la opción
   * \param[in] description Descripción de la opción
   */
  void addOption(const char *name, const char *description);

  /*!
   * \brief parsea los argumentos de entrada
   * \param[in] argc
   * \param[in] argv
   * \return
   */
  CmdParser::Status parse(int argc, const char* const argv[]);

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
   * \param[in] name Nombre del parámetro
   * \return Valor del parámetro en el tipo indicado
   *
   * Ejemplo de uso:
   *
   * \code
   * std::string filename = cmdParser.getValue<std::string>("img");
   * bool bSaveImages = cmdParser.hasOption("si");
   * int skip_frames = cmdParser.getValue<int>("skip_frames");
   * LD_TYPE ls = cmdParser.getParameterOptionIndex<LD_TYPE>("l_detect");
   * \endcode
   */
  template<typename T>
  T getValue( const char *name) const
  {
    T t = T();
    void *_value = (void *)&t;

    std::string _name(name);
    for (auto arg : mCmdArgs) {
      if (arg->getType() == CmdArgument::Type::PARAMETER 
          || arg->getType() == CmdArgument::Type::PARAMETER_OPTIONS) {
        if (arg->getName() == _name) {
          std::string value = (arg->getType() == CmdArgument::Type::PARAMETER) ? 
            dynamic_cast<CmdParameter *>(arg.get())->getValue() 
            : dynamic_cast<CmdParameterOptions *>(arg.get())->getValue();
          std::stringstream strm_value(value);
          if (typeid(T) == typeid(std::string)) {
            *(std::string *)_value = value;
          } else if (typeid(T) == typeid(int)) {
            strm_value >> *(int *)_value;
          } else if (typeid(T) == typeid(double)) {
            strm_value >> *(double *)_value;
          } else if (typeid(T) == typeid(float)) {
            strm_value >> *(float *)_value;
          } else if (typeid(T) == typeid(Path)) {
            *(Path *)_value = Path(value);
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
   * \param[in] name Nombre del parámetro
   * \return Valor del parámetro en el tipo indicado
   */
  template<typename T>
  T getParameterOptionIndex(const char *name) const
  {
    std::string _name(name);
    for (auto arg : mCmdArgs) {
      if ( arg->getType() == CmdArgument::Type::PARAMETER_OPTIONS ) {
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
   * \brief Mensaje que se puede añadir con información del proceso.
   */
  std::string mMsg;

  /*!
   * \brief Manejador del evento que se produce cada vez que se
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
   * \brief Constructora de la clase Progress
   * \param[min] min Valor mínimo
   * \param[min] max Valor máximo
   * \param[min] msg Mensaje opcional con información del proceso.
   */
  Progress(double min, double max, std::string msg = "") :
    mProgress(0.), mMinimun(min), mMaximun(max), mPercent(-1), mMsg(msg)
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
   * \brief Operador de llamada.
   *
   * Se llama cada vez que se quiera avanzar en la función de progreso
   */
  bool operator() (double increment = 1.);

  /*!
   * \brief Inicializa el progreso
   * \param[in] min Valor minimo
   * \param[in] max Valor máximo
   * \param[in] msg Mensaje opcional con información del proceso.
   */
  void init(double min, double max, std::string msg = "");

  /*!
   * \brief Restablece los valores al inicio
   */
  void restart();

  /*!
   * \brief Establece el manejador del evento OnInitialize
   * \param[in] initializeFunction Función que se llama al inicializar
   */
  void setOnInitializeListener(std::function<void(void)> &initializeFunction);

  /*!
   * \brief Establece el manejador del evento OnProgress
   * \param[in] progressFunction Función de control del progreso
   */
  void setOnProgressListener(std::function<void(double)> &progressFunction);

  /*!
   * \brief Establece el manejador del evento OnTerminate
   * \param[in] terminateFunction Función que se llama al terminar
   */
  void setOnTerminateListener(std::function<void(void)> &terminateFunction);

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
   * \param[in] customConsole
   */
  ProgressBar(bool customConsole = true)
    : Progress(), bCustomConsole(customConsole) {}

  /*!
   * \brief Constructora
   * \param min Valor mínimo
   * \param max Valor máximo
   * \param customConsole Si este valor esta activado la barra de progreso se muestra en color
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
   * \brief Constructora ProgressPercent
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

/*! \} */ // end of Console

/*! \} */ // end of utilities

} // End namespace I3D

#endif // I3D_CONSOLE_H
