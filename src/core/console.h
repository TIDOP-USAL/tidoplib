#ifndef I3D_CONSOLE_H
#define I3D_CONSOLE_H

#if defined WIN32
// Para que no den problemas std::numeric_limits<T>().max() 
#define NOMINMAX 
# include <windows.h>
#endif
#include <functional>
#include <map>
#include <vector>
#include <list>
#include <memory>

#include "defs.h"
#include "utils.h"

//Namespace:
using namespace std;

namespace I3D
{

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Clase para gestionar la configuración de la ventana de comandos
 */
class I3D_EXPORT Console
{
public:

  /*!
   * \brief Tipos de color de fondo y caracter.
   */
  enum class Color : int8_t {
    BLACK,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    YELLOW,
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
   * \brief Color de caracteres
   */
  WORD mForeColor;

  /*!
   * \brief Color de fondo
   */
  WORD mBackColor;

  /*!
   * \brief Configuración de la consola al iniciar. Se recupera al salir o con el método reset
   */
  WORD mOldColorAttrs;

#else
  //... Código para otros sistemas operativos
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
  void setConsoleForegroundColor(Console::Color foreColor);

  /*!
   * \brief Establece el color de fondo
   * \param backColor Color
   */
  void setConsoleBackgroundColor(Console::Color backColor);

  /*!
   * \brief setConsoleUnicode
   */
  void setConsoleUnicode();

private:

  /*!
   * \brief Inicializa la consola guardando la configuración  actual.
   * \param handle
   */
  void init( DWORD handle );

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
  virtual ArgType getType() = 0;
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
  ArgType getType() { return ArgType::OPTION; }

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
  ArgType getType() { return ArgType::PARAMETER; }

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
    : CmdArgument(name, description, optional) 
  {
    mDefValue = defValue;
    split(options, mOptions, ",");
  }

  /*!
   * \brief getType
   * \return
   */
  ArgType getType() { return ArgType::PARAMETER_OPTIONS; }

  /*!
   * \brief getValue
   * \return
   */
  template<typename T>
  std::string getValue() const { return mDefValue; }

  /*!
   * \brief setValue
   * \param value
   */
  void setValue(std::string value) { mDefValue = value; }
};

/*!
 * \brief Parseo de los argumentos de entrada  de la consola
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

public:

  /*!
   * \brief Constructora por defecto
   */
  CmdParser() {}

  CmdParser(char *name, char *description) 
    : mCmdName(name), mCmdDescription(description) {}

  /*!
   * \brief Constructor de lista
   * \param[in] Listado argumentos
   */
  CmdParser(char *name, char *description, std::initializer_list<std::shared_ptr<CmdArgument>> cmd_args) 
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
   * \brief
   */
  void clear() { mCmdArgs.clear(); }

  /*!
   * \brief Comprueba si se ha parseado correctamente el comando
   */
  //bool validate() { return true; }

    // Solución de openCV
    //template <typename T>
    //T get(const String& name, bool space_delete = true) const
    //{
    //    T val = T();
    //    getByName(name, space_delete, ParamType<T>::type, (void*)&val);
    //    return val;
    //}
//void CommandLineParser::getByName(const String& name, bool space_delete, int type, void* dst) const
//{
//    try
//    {
//        for (size_t i = 0; i < impl->data.size(); i++)
//        {
//            for (size_t j = 0; j < impl->data[i].keys.size(); j++)
//            {
//                if (name == impl->data[i].keys[j])
//                {
//                    String v = impl->data[i].def_value;
//                    if (space_delete)
//                        v = cat_string(v);
//
//                    // the key was neither specified nor has it a default value
//                    if((v.empty() && type != Param::STRING) || v == noneValue) {
//                        impl->error = true;
//                        impl->error_message = impl->error_message + "Missing parameter: '" + name + "'\n";
//                        return;
//                    }
//
//                    from_str(v, type, dst);
//                    return;
//                }
//            }
//        }
//    }
//    catch (Exception& e)
//    {
//        impl->error = true;
//        impl->error_message = impl->error_message + "Parameter '"+ name + "': " + e.err + "\n";
//        return;
//    }
//
//    CV_Error_(Error::StsBadArg, ("undeclared key '%s' requested", name.c_str()));
//}

  template<typename T>
  T getValue( const char *name) const
  { 
    T t = T();
    void *_value = (void *)&t;

    std::string _name(name);
    for (auto arg : mCmdArgs) {
      if (arg->getType() == ArgType::PARAMETER) {
        if (arg->getName() == _name) {
          std::string value = dynamic_cast<CmdParameter *>(arg.get())->getValue();
          std::stringstream strm_value(value);

          if (typeid(T) == typeid(std::string)) {
            *(std::string *)_value = value;
          } else if (typeid(T) == typeid(int)) {
            strm_value >> *(int *)_value;
            //std::string::size_type sz;
            //*(int *)_value = std::stoi(value,&sz);
          } else if (typeid(T) == typeid(double)) {
            strm_value >> *(double *)_value;
            //std::string::size_type sz;
            //*(double *)value = std::stod(value,&sz);
          } else if (typeid(T) == typeid(float)) {
            strm_value >> *(float *)_value;
            //std::string::size_type sz;
            //*(float *)value = std::stof(value,&sz);
          } else {
            throw exception("Tipo de dato  no permitido");
          }
        }
      }
    }
    return t;
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

  /*!
   * brief Manejador del evento que se produce cada vez que se
   * avanza una posición en la función de progreso
   */
  std::function<void(double)> onProgress;

  /*!
   * \brief Manejador del evento que se ejecuta al inicializar
   */
  std::function<void(void)> onInitialize;

  /*!
   * \brief Manejador del evento que se ejecuta al terminar
   */
  std::function<void(void)> onTerminate;

  /*!
   * \brief Escala
   */
  double mScale;

  /*!
   * brief Mensaje que se puede añadir con información del proceso.
   */
  std::string mMsg;

public:

  /*!
   * \brief Constructora por defecto
   */
  Progress() : mProgress(0.), mMinimun(0.), mMaximun(100.), mPercent(-1), mMsg("")
  {
    updateScale();
  }

  /*!
   * \brief Constructora
   */
  Progress(double min, double max, std::string msg = "") : mProgress(0.), mMinimun(min), mMaximun(max), mPercent(-1), mMsg(msg)
  {
    updateScale();
  }

  /*!
   * \brief Destructora
   */
  virtual ~Progress() {}

  /*!
   * \brief Operador de asignación. Se llama cada vez que se quiera avanzar en
   * la función de progreso
   */
  bool operator()(double increment = 1.);

  /*!
   * \brief Establece el manejador del evento OnProgress
   * \param progressFunction Función de control del progreso
   */
  void setOnProgressListener(std::function<void(double)> &progressFunction);
  
  /*!
   * \brief Establece el manejador del evento OnInitialize
   * \param initializeFunction Función que se llama al inicializar
   */
  void setOnInitializeListener(std::function<void(void)> &initializeFunction);
  
  /*!
   * \brief Establece el manejador del evento OnTerminate
   * \param terminateFunction Función que se llama al terminar
   */
  void setOnTerminateListener(std::function<void(void)> &terminateFunction);

  /*!
   * \brief Restablece los valores al inicio
   */
  void restart();

  /*!
   * \brief Inicializa el progreso
   * \param min Valor minimo
   * \param max
   * \param msg
   */
  void init(double min, double max, std::string msg = "");

protected:

  /*!
   * \brief
   */
  virtual void update();

  /*!
   * \brief initialize
   */
  void initialize();

  /*!
   * \brief terminate
   */
  void terminate();

  /*!
   * \brief updateScale
   */
  void updateScale();
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
