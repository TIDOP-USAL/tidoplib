/****************************************************************************
 *                                                                          *
 *  This file is part of TidopLib and can not be copied and/or distributed  *
 *  without the express permision of ITOS3D ENGINEERING S.L                 *  
 *                                                                          *
 *  Contact: http://www.itos3d.com                                          *
 *                                                                          *
 *--------------------------------------------------------------------------*
 *                                                                          *
 *  Copyright (C) 2018, ITOS3D ENGINEERING S.L - All rights reserved        *
 *                                                                          *
 ****************************************************************************/

#ifndef TL_CORE_CONSOLE_H
#define TL_CORE_CONSOLE_H

#include "config_tl.h"

#include <functional>
#include <map>
#include <vector>
#include <list>
#include <memory>
#include <iostream>
#include <sstream>
#if defined (__clang__) || defined (__GNUG__)
#include <cxxabi.h>
#endif

// filesystem
#if (__cplusplus >= 201703L)
#include <filesystem>
namespace fs = std::filesystem;
#else
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#endif

#include "tidop/core/defs.h"
#include "tidop/core/utils.h"
#include "tidop/core/messages.h"
#include "tidop/core/exception.h"

namespace TL
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
class TL_EXPORT Console 
#ifdef TL_MESSAGE_HANDLER
  : public MessageManager::Listener
#endif
{
public:

  /*!
   * \brief Valores de intensidad de color
   */
  enum class Intensity : int8_t
  {
    NORMAL,  /*!< Normal */
    BRIGHT   /*!< Brillante */
  };

  /*!
   * \brief Tipos de color de fondo y caracter.
   */
  enum class Color : int8_t 
  {
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
  enum class Mode : int8_t 
  {
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
  HANDLE mHandle;
  
  /*!
   * \brief Configuración de la consola al iniciar.
   *
   * La configuración inicial se recupera al salir o
   * con el método reset
   */
  WORD mOldColorAttrs;

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

  //TODO: Por terminar
  CONSOLE_FONT_INFOEX mIniFont;

  CONSOLE_FONT_INFOEX mCurrentFont;

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
   * \brief Color de caracteres
   */
  int mForeColor;

  /*!
   * \brief Color de fondo
   */
  int mBackColor;

  int mBold;

#endif
  
  /*!
   * \brief Nivel de información de los mensajes
   *
   * Por defecto MSG_ERROR
   * \see MessageLevel
   */
  static EnumFlags<MessageLevel> sLevel;

  /*!
   * \brief Objeto unico para la consola
   */
  static std::unique_ptr<Console> sObjConsole;

  static std::mutex mtx;

private:

  /*!
   * \brief Constructora por defecto
   */
  Console();

public:

  /*!
   * Destructora
   * Se recuperan las opciones por defecto de la consola
   */
  ~Console() override;

  Console(Console const&) = delete;
  void operator=(Console const&) = delete;

  /*!
   * \brief Singleton que devuelve una referencia unica de un objeto Console
   */
  static Console &getInstance();

  /*!
   * \brief Niveles de mensaje activados
   * \return Flag con los niveles de mensajes aceptados por la consola
   * \see EnumFlags
   */
  EnumFlags<MessageLevel> getMessageLevel() const;

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

  /*!
   * \brief Recupera los valores iniciales
   */
  void reset();

  /*!
   * \brief Establece el color de fondo
   * \param[in] backColor Color de fondo
   * \param[in] intensity Intensidad. El valor por defecto es Intensity::NORMAL
   */
  void setConsoleBackgroundColor(Console::Color backColor, Console::Intensity intensity = Console::Intensity::NORMAL);

  /*!
   * \brief Establece el color de caracter
   * \param[in] foreColor Color de caracter
   * \param[in] intensity Intensidad. El valor por defecto es Intensity::NORMAL
   * \see Console::Color, Console::Intensity
   */
  void setConsoleForegroundColor(Console::Color foreColor, Console::Intensity intensity = Console::Intensity::NORMAL);

  /*!
   * \brief Establece la consola como modo Unicode
   */
  void setConsoleUnicode();

  /*!
   * \brief Establece la fuente como negrita
   * \param[in] bBold 
   */
  void setFontBold(bool bBold);  

  /*!
   * \brief Establece el tamaño de la fuente
   * \param[in] size Tamaño de la fuente
   */
  void setFontHeight(int16_t size);

  /*!
   * \brief Establece los niveles de mensajes que se muestran por consola
   * \param[in] level Nivel de log
   * \see MessageLevel
   */
  void setLogLevel(MessageLevel level);

  /*!
   * \brief Establece el título de la consola
   * \param[in] title Titulo de la consola
   */
  void setTitle(const char *title);

protected:

#ifdef TL_MESSAGE_HANDLER  

  /*!
   * \brief onMsgDebug
   * \param msg
   * \param date
   */
  void onMsgDebug(const char *msg, const char *date) override;

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

#endif // TL_MESSAGE_HANDLER 

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


/*!
 * \brief The Argument class
 */
class TL_EXPORT Argument
{

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
   * \brief Nombre corto del argumento (Opcional)
   * Es un único caracter
   */
  char mShortName;

public:

  /*!
   * \brief Constructora argumento
   * \param[in] name Nombre del argumento
   * \param[in] description Descripción del argumento
   */
  Argument(const std::string &name, const std::string &description);

  /*!
   * \brief Constructora argumento
   * \param[in] shortName Nombre corto del argumento
   * \param[in] description Descripción del argumento
   */
  Argument(const char &shortName, const std::string &description);

  /*!
   * \brief Constructora argumento
   * \param[in] name Nombre del argumento
   * \param[in] shortName Nombre corto del argumento
   * \param[in] description Descripción del argumento
   */
  Argument(const std::string &name, const char &shortName, const std::string &description);

  /*!
   * \brief Constructora de copia
   * \param[in] argument Objeto que se copia
   */
  Argument(const Argument &argument);

  /*!
   * \brief Destructora
   */
  virtual ~Argument() = 0;

  /*!
   * \brief Devuelve el nombre del argumento
   * \return Nombre del argumento
   */
  std::string name() const;

  /*!
   * \brief Establece el nombre del argumento
   * \param[in] name Nombre del argumento
   */
  void setName(const std::string &name);

  /*!
   * \brief Devuelve la descripción del argumento
   * \return Descripción del argumento
   */
  std::string description() const;

  /*!
   * \brief Establece la descripción del argumento
   * \param[in] description Descripción del argumento
   */
  void setDescription(const std::string &description);

  /*!
   * \brief Devuelve el nombre corto del argumento
   * \return Nombre corto
   */
  char shortName() const;

  /*!
   * \brief Establece el nombre corto del argumento
   * \param[in] shortName Nombre corto
   */
  void setShortName(const char &shortName);

  /*!
   * \brief Devuelve una cadena de texto con el tipo del argumento
   * \return
   */
  virtual std::string typeName() const = 0;

  /*!
   * \brief Comprueba si el argumento es obligatorio
   * \return true si es obligatorio
   */
  virtual bool isRequired() const = 0;

  /*!
   * \brief Convierte el valor del argumento a cadena de texto
   * \return Cadena de texto con el valor del argumento
   */
  virtual std::string toString() const = 0;

  /*!
   * \brief Establece el valor del argumento a partir de una cadena de texto
   * \param[in] value Valor del argumento como cadena de texto
   */
  virtual void fromString(const std::string &value) = 0;
};


/*!
 * \brief Template para gestionar diferentes tipos de argumentos
 */
template <typename T, bool required = true>
class Argument_
  : public Argument
{

protected:

  /*!
   * \brief Valor del argumento
   */
  T *mValue;

public:

  /*!
   * \brief Constructora argumento
   * \param[in] name Nombre del argumento
   * \param[in] description Descripción del argumento
   * \param[in,out] value Valor del argumento. En el caso de argumentos opcionales establece el valor por defecto
   */
  Argument_(const std::string &name, const std::string &description, T *value);

  /*!
   * \brief Constructora argumento
   * \param[in] shortName Nombre corto del argumento
   * \param[in] description Descripción del argumento
   * \param[in,out] value Valor del argumento. En el caso de argumentos opcionales establece el valor por defecto
   */
  Argument_(const char &shortName, const std::string &description, T *value);

  /*!
   * \brief Constructora argumento
   * \param[in] name Nombre del argumento
   * \param[in] shortName Nombre corto del argumento
   * \param[in] description Descripción del argumento
   * \param[in,out] value Valor del argumento. En el caso de argumentos opcionales establece el valor por defecto
   */
  Argument_(const std::string &name, const char &shortName, const std::string &description, T *value);

  /*!
   * \brief Constructora de copia
   * \param[in] argument Objeto que se copia
   */
  Argument_(const Argument_ &argument);

  /*!
   * \brief Destructora
   */
  virtual ~Argument_() override {}

  /*!
   * \brief Devuelve una cadena de texto con el tipo del argumento
   * \return
   */
  std::string typeName() const override;

  /*!
   * \brief Comprueba si el argumento es obligatorio
   * \return true si es obligatorio
   */
  bool isRequired() const override;

  /*!
   * \brief Convierte el valor del argumento a cadena de texto
   * \return Cadena de texto con el valor del argumento
   */
  std::string toString() const override;

  /*!
   * \brief Establece el valor del argumento a partir de una cadena de texto
   * \param[in] value Valor del argumento como cadena de texto
   */
  void fromString(const std::string &value) override;

  /*!
   * \brief Valor del argumento
   * \return Valor del argumento
   */
  T value() const;

  /*!
   * \brief Establece el valor del argumento
   * \param[in] value Valor del argumento
   */
  virtual void setValue(const T &value);
};


/* Definición de unos alias para los tipos mas frecuentes */

typedef Argument_<int, true> ArgumentIntegerRequired;
typedef Argument_<int, false> ArgumentIntegerOptional;
typedef Argument_<double, true> ArgumentDoubleRequired;
typedef Argument_<double, false> ArgumentDoubleOptional;
typedef Argument_<float, true> ArgumentFloatRequired;
typedef Argument_<float, false> ArgumentFloatOptional;
typedef Argument_<bool, true> ArgumentBooleanRequired;
typedef Argument_<bool, false> ArgumentBooleanOptional;
typedef Argument_<std::string, true> ArgumentStringRequired;
typedef Argument_<std::string, false> ArgumentStringOptional;
typedef Argument_<fs::path, true> ArgumentPathRequired;
typedef Argument_<fs::path, false> ArgumentPathOptional;


/* Implementación */

template<typename T, bool required> inline
Argument_<T, required>::Argument_(const std::string &name,
                                  const std::string &description,
                                  T *value)
  : Argument(name, description),
    mValue(value)
{
}

template<typename T, bool required> inline
Argument_<T, required>::Argument_(const char &shortName,
                                  const std::string &description,
                                  T *value)
  : Argument(shortName, description),
    mValue(value)
{
}

template<typename T, bool required> inline
Argument_<T, required>::Argument_(const std::string &name,
                                  const char &shortName,
                                  const std::string &description,
                                  T *value)
  : Argument(name, shortName, description),
    mValue(value)
{
}

template<typename T, bool required> inline
Argument_<T, required>::Argument_(const Argument_ &argument)
  : Argument(argument),
    mValue(argument.mValue)
{
}

template<typename T, bool required> inline
std::string Argument_<T, required>::typeName() const
{
  /// https://ideone.com/sqFWir
  std::string type_name = typeid(T).name();
#if defined (__clang__) || defined (__GNUG__)
  int status;
  char *demangled_name = abi::__cxa_demangle(type_name.c_str(), nullptr, nullptr, &status);
  if (status == 0){
    type_name = demangled_name;
    std::free(demangled_name);
  }
#endif
  return type_name;
}

template<> inline
std::string Argument_<std::string, true>::typeName() const
{
  return "std::string";
}
template<> inline
std::string Argument_<std::string, false>::typeName() const
{
  return "std::string";
}

template<> inline
std::string Argument_<fs::path, true>::typeName() const
{
  return "path";
}
template<> inline
std::string Argument_<fs::path, false>::typeName() const
{
  return "path";
}

template<typename T, bool required> inline
bool Argument_<T, required>::isRequired() const
{
  return required;
}

template<typename T, bool required> inline
std::string Argument_<T, required>::toString() const
{
  std::string val;
  if(typeid(T) == typeid(bool)) {
    val = *mValue ? "true" : "false";
  } else if (std::is_integral<T>::value) {
    val = std::to_string(*mValue);
  } else if (std::is_floating_point<T>::value){
    val = std::to_string(*mValue);
  } else if(typeid(T) == typeid(std::string)) {
    val = *mValue;
  }
  return val;
}

template<> inline
std::string Argument_<std::string, true>::toString() const
{
  return *mValue;
}

template<> inline
std::string Argument_<std::string, false>::toString() const
{
  return *mValue;
}

template<> inline
std::string Argument_<fs::path, true>::toString() const
{
  return mValue->string();
}

template<> inline
std::string Argument_<fs::path, false>::toString() const
{
  return mValue->string();
}

template<typename T, bool required> inline
void Argument_<T, required>::fromString(const std::string &value)
{
  if(typeid(T) == typeid(bool)) {
    if (value == "true" || value == "1")
      *mValue = true;
  } else if (std::is_integral<T>::value) {
    *mValue = stringToInteger(value);
  } else if (std::is_floating_point<T>::value) {
    *mValue = std::stod(value);
  }
}

///TODO: especialización de plantilla para salir del paso...
template<> inline
void Argument_<std::string, true>::fromString(const std::string &value)
{
  *mValue = value;
}

template<> inline
void Argument_<std::string, false>::fromString(const std::string &value)
{
  *mValue = value;
}

template<> inline
void Argument_<fs::path, true>::fromString(const std::string &value)
{
  *mValue = value;
}

template<> inline
void Argument_<fs::path, false>::fromString(const std::string &value)
{
  *mValue = value;
}


template<typename T, bool required> inline
T Argument_<T, required>::value() const
{
    return *mValue;
}

template<typename T, bool required> inline
void Argument_<T, required>::setValue(const T &value)
{
    *mValue = value;
}


/*!
 * \brief Argumento lista de opciones
 */
template <typename T, bool required = true>
class ArgumentList_
  : public Argument_<T, required>
{

protected:

  std::vector<T> mValues;
  size_t *mIdx;

public:

  /*!
   * \brief Constructora argumento lista de opciones
   * \param[in] name Nombre del argumento
   * \param[in] description Descripción del argumento
   * \param[in] values Vector con los posibles valores que puede tomar el argumento
   * \param[in,out] idx Indice del valor que toma el argumento. En el caso de argumentos opcionales establece el valor por defecto
   */
  ArgumentList_(const std::string &name, const std::string &description, std::vector<T> &values, size_t *idx);

  /*!
   * \brief Constructora argumento lista de opciones
   * \param[in] shortName Nombre corto del argumento
   * \param[in] description Descripción del argumento
   * \param[in] values Vector con los posibles valores que puede tomar el argumento
   * \param[in,out] idx Indice del valor que toma el argumento. En el caso de argumentos opcionales establece el valor por defecto
   */
  ArgumentList_(const char &shortName, const std::string &description, std::vector<T> &values, size_t *idx);

  /*!
   * \brief Constructora argumento lista de opciones
   * \param[in] name Nombre del argumento
   * \param[in] shortName Nombre corto del argumento
   * \param[in] description Descripción del argumento
   * \param[in] values Vector con los posibles valores que puede tomar el argumento
   * \param[in,out] idx Indice del valor que toma el argumento. En el caso de argumentos opcionales establece el valor por defecto
   */
  ArgumentList_(const std::string &name, const char &shortName, const std::string &description, std::vector<T> &values, size_t *idx);

  /*!
   * \brief Constructora de copia
   * \param[in] argument Objeto que se copia
   */
  //ArgumentList_(const ArgumentList_ &argumentList);

  /*!
   * \brief Destructora
   */
  virtual ~ArgumentList_() override {}

  void fromString(const std::string &value) override;

  void setValue(const T &value) override;
};


/* Definición de unos alias para los tipos mas frecuentes */

typedef ArgumentList_<int, true> ArgumentListIntegerRequired;
typedef ArgumentList_<int, false> ArgumentListIntegerOptional;
typedef ArgumentList_<double, true> ArgumentListDoubleRequired;
typedef ArgumentList_<double, false> ArgumentListDoubleOptional;
typedef ArgumentList_<float, true> ArgumentListFloatRequired;
typedef ArgumentList_<float, false> ArgumentListFloatOptional;
typedef ArgumentList_<bool, true> ArgumentListBooleanRequired;
typedef ArgumentList_<bool, false> ArgumentListBooleanOptional;
typedef ArgumentList_<std::string, true> ArgumentListStringRequired;
typedef ArgumentList_<std::string, false> ArgumentListStringOptional;
typedef ArgumentList_<fs::path, true> ArgumentListPathRequired;
typedef ArgumentList_<fs::path, false> ArgumentListPathOptional;


/* Implementación */

//template<typename T, bool required> inline
//ArgumentList_<T, required>::ArgumentList_(const std::string &name,
//                                          const std::string &description,
//                                          const std::vector<T> &values, /// Si values esta vacio...
//                                          size_t *idx)
//  : Argument_<T, required>(name, description, &values[*idx >= 0 && *idx < values.size() ? *idx : 0]),
//    mValues(values),
//    mIdx(idx)
//{
//  this->setName(name);
//  this->setDescription(description);
//}

template<typename T, bool required> inline
ArgumentList_<T, required>::ArgumentList_(const std::string &name,
                                          const std::string &description,
                                          std::vector<T> &values,
                                          size_t *idx)
  : Argument_<T, required>(name, description, &values[*idx >= 0 && *idx < values.size() ? *idx : 0]),
    mValues(values),
    mIdx(idx)
{
}

template<typename T, bool required> inline
ArgumentList_<T, required>::ArgumentList_(const char &shortName,
                                          const std::string &description,
                                          std::vector<T> &values,
                                          size_t *idx)
  : Argument_<T, required>(shortName, description, &values[*idx >= 0 && *idx < values.size() ? *idx : 0]),
    mValues(values),
    mIdx(idx)
{
}

template<typename T, bool required> inline
ArgumentList_<T, required>::ArgumentList_(const std::string &name,
                                          const char &shortName,
                                          const std::string &description,
                                          std::vector<T> &values,
                                          size_t *idx)
  : Argument_<T, required>(name, shortName, description, &values[*idx >= 0 && *idx < values.size() ? *idx : 0]),
    mValues(values),
    mIdx(idx)
{
}

template<typename T, bool required> inline
void ArgumentList_<T, required>::fromString(const std::string &value)
{
  T prev_value = this->value();
  Argument_<T, required>::fromString(value);
  T curr_value = this->value();
  size_t idx = 0;
  bool bFind = false;
  for(auto &_value : mValues){
    if (curr_value == _value){
      bFind = true;
      break;
    }
    idx++;
  }
  if (bFind){
    *mIdx = idx;
  } else {
    Argument_<T, required>::setValue(prev_value);
  }
}

template<typename T, bool required> inline
void ArgumentList_<T, required>::setValue(const T &value)
{
  for(auto &_value : mValues){
    if (value == _value){
      Argument_<T, required>::setValue(value);
      return;
    }
  }
}


//class ArgumentValidator
//{
//public:
//
//  ArgumentValidator() {}
//
//  virtual bool validate() = 0;
//};

template <typename T, typename Enable = void>
class ArgumentValidator;

template <typename T>
class ArgumentValidator<T, typename std::enable_if<std::is_arithmetic<T>::value && typeid(T) != typeid(bool)>::type>
{
private:

  T mMin;
  T mMax;

public:

  ArgumentValidator()
    : mMin(),
      mMax(std::numeric_limits<T>().max())
  {}

  bool validate(T value)
  {
    if (value > mMin && value < mMax)
      return true;
  }

  void setRange(T min, T max)
  {
    mMin = min;
    mMax = max;
  }

  T min() const
  {
    return mMin;
  }

  T max() const
  {
    return mMax;
  }
};


/*!
 * \brief Clase para la gestión de comandos por consola
 * La clase establece los argumentos del comando, parsea la entrada del comando
 * y comprueba que el mismo sea correcto
 *
 * La estructura de un comando es:
 *
 * cmdName [--param1|-p] [Value] [--option|-o]
 *
 * Ejemplo de uso:
 *
 * \code
 *  std::string file;
 *  bool bOpt;
 *  int val;
 *  double val_d = 0.5;
 *
 *  Command cmd(name, "Ejemplo de aplicación de consola");
 *  cmd.push_back(std::make_shared<ArgumentStringRequired>("file", 'f', "Ejemplo de parámetro obligatorio. Ruta de un fichero.", &file));
 *  cmd.push_back(std::make_shared<ArgumentIntegerRequired>("int", 'i', "Valor entero obligatorio", &val));
 *  cmd.push_back(std::make_shared<ArgumentBooleanOptional>("bool", 'b', "boolean", &bOpt));
 *  cmd.push_back(std::make_shared<ArgumentDoubleOptional>("double", "Parámetro doble opcional. Si se omite se toma el valor por defecto", &val_d));
 *
 *  // Parseo de los argumentos y comprobación de los mismos
 *  Command::Status status = cmd.parse(argc, argv);
 *  if (status == Command::Status::PARSE_ERROR ) {
 *    return 1;
 *  } else if (status == Command::Status::SHOW_HELP) {
 *    return 0;
 *  } else if (status == Command::Status::SHOW_LICENCE) {
 *    return 0;
 *  } else if (status == Command::Status::SHOW_VERSION) {
 *    return 0;
 *  }
 * \endcode
 */
class TL_EXPORT Command
{

public:

  /*!
   * \brief Estado de salida del parseo del comando
   */
  enum class Status
  {
    PARSE_SUCCESS,  /*!< El parseo se ejecuto correctamente */
    PARSE_ERROR,    /*!< Ocurrio un error al ejecutar el comando */
    SHOW_HELP,      /*!< Se pasa como parametro: help. Muestra la ayuda del programa */
    SHOW_VERSION,   /*!< Se pasa como parametro: version. Se muestra la versión del programa */
    SHOW_LICENCE    /*!< Se pasa como parametro: licence. Se muestra la información de licencia */
  };

  /*!
   * \brief Allocator
   */
  typedef std::list<std::shared_ptr<Argument>>::allocator_type allocator_type;

  /*!
   * \brief value_type
   */
  typedef std::list<std::shared_ptr<Argument>>::value_type value_type;

  /*!
   * \brief Tipo entero sin signo (por lo general size_t)
   */
  typedef std::list<std::shared_ptr<Argument>>::size_type size_type;

  /*!
   * \brief Tipo entero con signo (por lo general ptrdiff_t)
   */
  typedef std::list<std::shared_ptr<Argument>>::difference_type difference_type;

  /*!
   * \brief std::allocator_traits<Allocator>::pointer
   */
  typedef std::list<std::shared_ptr<Argument>>::pointer pointer;

  /*!
   * \brief std::allocator_traits<Allocator>::const_pointer
   */
  typedef std::list<std::shared_ptr<Argument>>::const_pointer const_pointer;

  /*!
   * \brief value_type&
   */
  typedef std::list<std::shared_ptr<Argument>>::reference reference;

  /*!
   * \brief const value_type&
   */
  typedef std::list<std::shared_ptr<Argument>>::const_reference const_reference;

  /*!
   * \brief Iterador de acceso aleatorio
   */
  typedef std::list<std::shared_ptr<Argument>>::iterator iterator;

  /*!
   * \brief Iterador constante de acceso aleatorio
   */
  typedef std::list<std::shared_ptr<Argument>>::const_iterator const_iterator;


private:

  /*!
   * \brief Nombre del comando
   */
  std::string mName;

  /*!
   * \brief Descripción del comando
   */
  std::string mDescription;

  /*!
   * \brief Listado de los argumentos del comando
   */
  std::list<std::shared_ptr<Argument>> mCmdArgs;

  /*!
   * \brief Listado de los argumentos por defecto comando
   * Comandos como ayuda [-h | --help] o versión [--version]
   */
  //std::list<std::shared_ptr<Argument>> mDefaultArgs;

  /*!
   * \brief Versión del programa
   */
  std::string mVersion;

public:

  /*!
   * \brief Constructora por defecto
   */
  Command();

  /*!
   * \brief Constructor de copia
   * \param[in] command Objeto que se copia
   */
  Command(const Command &command);

  /*!
   * \brief Constructor comando tipo POSIX
   * \param[in] name Nombre del comando
   * \param[in] description Descripción del comando
   */
  Command(const std::string &name, const std::string &description);

  /*!
   * \brief Constructora de lista
   * \param[in] name Nombre del comando
   * \param[in] description Descripción del comando
   * \param[in] arguments listado de argumentos
   */
  Command(const std::string &name, const std::string &description, std::initializer_list<std::shared_ptr<TL::Argument>> arguments);

  /*!
   * \brief Devuelve el nombre del comando
   * \return Nombre del comando
   */
  std::string name() const;

  /*!
   * \brief Establece el nombre del comando
   * \param[in] name Nombre del comando
   */
  void setName(const std::string &name);

  /*!
   * \brief Devuelve la descripción del comando
   * \return Descripción del comando
   */
  std::string description() const;

  /*!
   * \brief Establece la descripción del comando
   * \param[in] description Descripción del comando
   */
  void setDescription(const std::string &description);

  /*!
   * \brief Versión del programa
   * \return
   */
  std::string version() const;

  /*!
   * \brief Establece la versión del programa
   * \param[in] version Versión del programa
   */
  void setVersion(const std::string &version);

  /*!
   * \brief parsea los argumentos de entrada
   * \param[in] argc
   * \param[in] argv
   * \return Devuelve el estado. PARSE_ERROR en caso de error y PARSE_SUCCESS cuando el parseo se ha hecho correctamente
   * \see CmdParser::Status
   */
  Status parse(int argc, const char* const argv[]);

  /*!
   * \brief Devuelve un iterador al inicio
   * \return Iterador al primer elemento
   */
  iterator begin();

  /*!
   * \brief Devuelve un iterador constante al inicio
   * \return Iterador al primer elemento
   */
  const_iterator begin() const;

  /*!
   * \brief Devuelve un iterador al siguiente elemento después del último argumento
   * Este elemento actúa como un marcador de posición, intentar acceder a él resulta en un comportamiento no definido
   * \return Iterador al siguiente elemento después del último argumento
   */
  iterator end();

  /*!
   * \brief Devuelve un iterador constante al siguiente elemento después del último argumento
   * Este elemento actúa como un marcador de posición, intentar acceder a él resulta en un comportamiento no definido
   * \return Iterador al siguiente elemento después del último argumento
   */
  const_iterator end() const;

  /*!
   * \brief Agrega un argumento mediante copia al final
   * \param[in] arg Argumento que se añade
   */
  void push_back(const std::shared_ptr<Argument> &arg);

  /*!
   * \brief Agrega un argumento mediante movimiento al final
   * \param[in] arg Argumento que se añade
   */
  void push_back(std::shared_ptr<Argument> &&arg) TL_NOEXCEPT;

  /*!
   * \brief Elimina los argumentos
   */
  void clear();

  /*!
   * \brief Comprueba si no hay argumentos
   * \return true si el contenedor está vacío y false en caso contrario
   */
  bool empty() const;

  /*!
   * \brief Devuelve el tamaño del contenedor
   * \return Tamaño
   */
  size_type size() const;

  /*!
   * \brief Asignación de copia
   */
  Command& operator=(const Command& command);

  /*!
   * \brief Asignación de movimiento
   */
  Command& operator=(Command &&command) TL_NOEXCEPT;

  /*!
   * \brief Elimina el intervalo
   */
  iterator erase(const_iterator first, const_iterator last);

  /*!
   * \brief Muestra la ayuda en la consola
   */
  void showHelp() const;

  /*!
   * \brief Muestra la versión en la consola
   */
  void showVersion() const;

  /*!
   * \brief Muestra la licencia en la consola
   */
  void showLicence() const;

protected:

  void init();

};


/* ---------------------------------------------------------------------------------- */


/*!
 * \brief Función objeto de progreso
 */
class TL_EXPORT Progress
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

  //TODO: quitar los manejadores de eventos. Mejor una clase virtual pura y
  //      crear una clase hija si hace falta

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

  static std::mutex sMutex;

public:

  /*!
   * \brief Constructora por defecto
   */
  Progress();


  /*!
   * \brief Constructora de la clase Progress
   * \param[in] min Valor mínimo
   * \param[in] max Valor máximo
   * \param[in] msg Mensaje opcional con información del proceso.
   */
  Progress(double min, double max, const std::string &msg = "");

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
  virtual void init(double min, double max, const std::string &msg = "");

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

  void setMinimun(double min);

  void setMaximun(double max);

  /*!
   * \brief updateScale
   */
  void updateScale();

protected:

  /*!
   * \brief initialize
   */
  void initialize();

  /*!
   * \brief
   */
  virtual void updateProgress() = 0;

  /*!
   * \brief terminate
   */
  virtual void terminate() = 0;
};


/*!
 * \brief Barra de progreso de consola
 */
class TL_EXPORT ProgressBar 
  : public Progress
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
  ProgressBar(bool customConsole = true);

  /*!
   * \brief Constructora
   * \param min Valor mínimo
   * \param max Valor máximo
   * \param customConsole Si este valor esta activado la barra de progreso se muestra en color
   */
  ProgressBar(double min, double max, bool customConsole = true);

  /*!
   * \brief Destructora
   */
  ~ProgressBar() override {}

  //... warning C4512: 'TL::ProgressBar' : no se pudo generar el operador de asignaciones
  //    Este warning aparece debido a que mSize es constante. impido la asignación que por
  //    otra parte tampoco me interesa

  ProgressBar &operator=(const ProgressBar &pb) = delete;

private:

  /*!
   * \brief Actualiza la barra de progreso
   */
  void updateProgress() override;

  /*!
   * \brief terminate
   */
  void terminate() override;
};

/*!
 * \brief Progreso en porcentaje
 */
class TL_EXPORT ProgressPercent 
  : public Progress
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
  ProgressPercent(bool customConsole = false);

  /*!
   * \brief Constructora
   * \param min Valor mínimo
   * \param max Valor máximo
   * \param customConsole
   */
  ProgressPercent(double min, double max, bool customConsole = false);

  /*!
   * \brief Destructora
   */
  ~ProgressPercent() override {}

private:

  /*!
   * \brief Actualiza la barra de progreso
   */
  void updateProgress() override;

  /*!
   * \brief terminate
   */
  void terminate() override;
};








/* ---------------------------------------------------------------------------------- */

#ifdef TL_SHOW_DEPRECATED

/* Deprecated class */



// http://www.gnu.org/software/libc/manual/html_node/Argument-Syntax.html#Argument-Syntax

// POSIX
// Los argumentos son opciones si comienzan con un guión (‘-’).
// Múltiples opciones pueden seguir un delimitador de guión en un único token si las opciones no tienen argumentos.
// Asi '-abc' es equivalente a '-a -b -c'.
// Option names are single alphanumeric characters (as for isalnum; see Classification of Characters).
// Ciertas opciones requieren un argumento. Por ejemplo, el '-o' comando del comando ld requiere un argumento
// nombre-archivo de salida.
// Una opción y su argumento pueden o no pueden aparecer como fichas separadas. (En otras palabras, el espacio en
// blanco que los separa es opcional.) Por lo tanto, '-o foo' y '-ofoo' son equivalentes.
//
// GNU añade opciones de larga duración a estas convenciones. Las opciones largas consisten en '-' seguido de un
// nombre hecho de caracteres alfanuméricos y guiones. Los nombres de opciones son por lo general de una a tres
// palabras de largo, con guiones para separar las palabras. Los usuarios pueden abreviar los nombres de las opciones,
// siempre y cuando las abreviaturas son únicos.
//
// Para especificar un argumento para una larga, debe escribirse '--name = valor'. Esta sintaxis permite una opción a
// largo para aceptar un argumento que es en sí opcional.

//enum class ArgType : int8_t {
//  OPTION,
//  PARAMETER,
//  PARAMETER_OPTIONS
//};

/*!
 * \brief Clase base para la gestión de argumentos en comandos de consola
 * \deprecated
 */
class TL_EXPORT TL_DEPRECATED("Argument") CmdArgument
{
public:

  /*!
   * \brief Tipo de argumento
   */
  enum class Type : int8_t {
    OPTION,             /*!< Opción */
    PARAMETER,          /*!< Parámetro */
    PARAMETER_OPTIONS   /*!< Lista de opciones*/
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
  CmdArgument(const char *name, const char *description, bool optional = false);

  virtual ~CmdArgument(){}

  CmdArgument(CmdArgument const&) = delete;
  void operator=(CmdArgument const&) = delete;

  /*!
   * \brief Devuelve la descripción del argumento
   * \return Descripción
   */
  const char *getDescription() const;

  /*!
   * \brief Devuelve el nombre del argumento
   * \return Nombre del argumento
   */
  const char *getName() const;

  /*!
   * \brief Tipo
   * \return
   */
  virtual Type getType() const = 0;

  /*!
   * \brief Devuelve si el argumento es opcional
   * \return verdadero si el argumento es opcional
   */
  bool isOptional() const;

};


/*!
 * \brief Opción de un comando
 * \deprecated
 */
class TL_EXPORT  CmdOption : public CmdArgument
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
  CmdOption(const char *name, const char *description);

  ~CmdOption() {}

  /*!
   * \brief getType
   * \return
   */
  CmdArgument::Type getType() const override;

  /*!
   * \brief Comprueba si la opción esta activada
   * \return Verdadero si esta activada y falso en caso contrario
   */
  bool isActive() const;

  /*!
   * \brief Establece si esta activada o no
   * \param[in] option
   * \deprecated Use TL::CmdOption::setActive en su lugar
   */
  TL_DEPRECATED("CmdOption::setActive")
  void setOption(bool option) { mValue = option; }

  /*!
   * \brief Establece si esta activada o no
   * \param[in] active
   */
  void setActive(bool active);
};

/*!
 * \brief Parametro de un comando de consola
 * \deprecated
 */
class TL_EXPORT CmdParameter : public CmdArgument
{
private:

  /*!
   * \brief Valor del parámetro
   */
  std::string mValue;

public:

  /*!
   * \brief Constructora de la clase CmdParameter
   * \param[in] name Nombre del parámetro
   * \param[in] description Descripción del parámetro
   * \param[in] optional El parametro es opcional. Por defecto no es opcional
   * \param[in] defValue Valor por defecto
   */
  CmdParameter(const char *name, const char *description, bool optional = false, const char *defValue = "");

  ~CmdParameter() {}

  /*!
   * \brief getType
   * \return
   */
  CmdArgument::Type getType() const override;

  /*!
   * \brief Devuelve el valor del parámetro
   * \return Valor del parámetro
   */
  const char *getValue() const;

  /*!
   * \brief Establece el valor del parámetro
   * \param[in] value Valor del parámetro
   */
  void setValue(const std::string &value);
};


/*!
 * \brief Un parametro que toma un valor de una lista predefinida de opciones
 * \deprecated
 */
struct TL_EXPORT CmdParameterOptions : public CmdArgument
{

private:

  /*!
   * \brief mDefValue
   */
  std::string mValue;

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
  CmdParameterOptions(const char *name, const char *options, const char *description, bool optional = false, const char *defValue = "");

  ~CmdParameterOptions() {}

  /*!
   * \brief Indice de opción
   * \return Indice
   */
  int getIndex(const std::string &value) const;

  /*!
   * \brief Indice de opción seleccionada
   * \return Indice
   */
  int getIndex() const;

  /*!
   * \brief listado de opciones
   */
  std::vector<std::string> getOptions() const;

  /*!
   * \brief getType
   * \return
   */
  CmdArgument::Type getType() const override;

  /*!
   * \brief getValue
   * \return
   */
  const char *getValue() const;

  /*!
   * \brief setValue
   * \param value
   */
  void setValue(const std::string &value);

};

/* ---------------------------------------------------------------------------------- */

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
 * \deprecated
 */
class TL_EXPORT TL_DEPRECATED("Command") CmdParser
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

public:

  /*!
   * \brief Constructora por defecto
   */
  CmdParser();

  /*!
   * \brief Constructor CmdParser
   * \param[in] name Nombre del comando
   * \param[in] description Descripción del comando
   */
  CmdParser(const char *name, const char *description);

  /*!
   * \brief Constructor de lista
   * \param[in] name Nombre del comando
   * \param[in] description Descripción del comando
   * \param[in] cmd_args Lista de argumentos
   */
  CmdParser(const char *name, const char *description, std::initializer_list<std::shared_ptr<CmdArgument>> cmd_args);

  /*!
   * \brief Destructora
   */
  ~CmdParser() {}

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
   * \return Devuelve el estado. PARSE_ERROR en caso de error y PARSE_SUCCESS cuando el parseo se ha hecho correctamente
   * \see CmdParser::Status
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
          } /*else if (typeid(T) == typeid(Path)) {
            *(Path *)_value = Path(value);
          }*/ else {
            TL_THROW_ERROR("Tipo de dato  no permitido");
            //throw std::runtime_error("Tipo de dato  no permitido");
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
  template<typename T = int>
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



/* End deprecated class */

#endif // TL_SHOW_DEPRECATED


/* ---------------------------------------------------------------------------------- */



/*! \} */ // end of Console

/*! \} */ // end of utilities


} // End namespace TL


#endif // TL_CORE_CONSOLE_H
