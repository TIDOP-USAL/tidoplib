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

#include "tidop/core/defs.h"
#include "tidop/core/utils.h"
#include "tidop/core/messages.h"
#include "tidop/core/exception.h"
#include "tidop/core/licence.h"
#include "tidop/core/path.h"

namespace tl
{

/* ---------------------------------------------------------------------------------- */

/*! \addtogroup utilities
 *  \{
 */

/*! \defgroup Console Utilidades de consola
 *  
 * Utilidades para aplicaciones en modo consola que comprenden la apariencia de 
 * la consola (tamaño de texto, color, etc), parseo de comandos y barra de progreso 
 * para procesos
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
    normal,            /*!< Normal */
    bright             /*!< Brillante */
#ifdef TL_ENABLE_DEPRECATED_METHODS
    ,
    NORMAL = normal,
    BRIGHT = bright
#endif // TL_ENABLE_DEPRECATED_METHODS
  };

  /*!
   * \brief Tipos de color de fondo y caracter.
   */
  enum class Color : int8_t 
  {
    black,    /*!< Negro */
    red,      /*!< Rojo */
    green,    /*!< Verde */
    yellow,   /*!< Amarillo */
    blue,     /*!< Azul */
    magenta,  /*!< Magenta */
    cyan,     /*!< Cian */
    white     /*!< Blanco */
#ifdef TL_ENABLE_DEPRECATED_METHODS
    ,
    BLACK   = black,    /*!< Negro */
    RED     = red,      /*!< Rojo */
    GREEN   = green,    /*!< Verde */
    YELLOW  = yellow,   /*!< Amarillo */
    BLUE    = blue,     /*!< Azul */
    MAGENTA = magenta,  /*!< Magenta */
    CYAN    = cyan,     /*!< Cian */
    WHITE   = white     /*!< Blanco */
#endif
  };

  /*!
   * \brief Modo de consola
   */
  enum class Mode : int8_t 
  {
    input,          /*!< Consola en modo entrada */
    output,         /*!< Consola en modo salida */
    output_error    /*!< Consola en modo salida de errores */
#ifdef TL_ENABLE_DEPRECATED_METHODS
    ,
    INPUT        = input,          /*!< Consola en modo entrada */
    OUTPUT       = output,         /*!< Consola en modo salida */
    OUTPUT_ERROR = output_error    /*!< Consola en modo salida de errores */
#endif
  };

private:

  /// Constructora privada ya que es un Singleton
  Console();

public:

#ifdef TL_MESSAGE_HANDLER
  ~Console() override;
#else
  ~Console();
#endif

  /// Se invalida la copia y la asignación
  Console(const Console &) = delete;
  Console(Console &&) = delete;
  Console &operator = (const Console &) = delete;
  Console &operator = (Console &&) = delete;

  /*!
   * \brief Singleton que devuelve una referencia unica de un objeto Console
   */
  static Console &instance();

  /*!
   * \brief Niveles de mensaje activados
   * \return Flag con los niveles de mensajes aceptados por la consola
   * \see EnumFlags
   */
  static EnumFlags<MessageLevel> messageLevel();

  /*!
   * \brief Establece los niveles de mensajes que se muestran por consola
   * \param[in] level Nivel de log
   * \see MessageLevel
   */
  static void setMessageLevel(MessageLevel level);
  #ifdef TL_ENABLE_DEPRECATED_METHODS
  static void setLogLevel(MessageLevel level);
  #endif

  /*!
   * \brief Imprime un mensaje en la consola
   * \param[in] message Mensaje que se muestra por consola
   */
  void printMessage(const std::string &message);

  /*!
   * \brief Imprime un mensaje de error en la consola
   * \param[in] message Mensaje que se muestra por consola
   */
  void printErrorMessage(const std::string &message);

  /*!
   * \brief Recupera los valores iniciales
   */
  void reset();

  /*!
   * \brief Establece el color de fondo
   * \param[in] backColor Color de fondo
   * \param[in] intensity Intensidad. El valor por defecto es Intensity::NORMAL
   */
  void setConsoleBackgroundColor(Console::Color backColor, 
                                 Console::Intensity intensity = Console::Intensity::normal);

  /*!
   * \brief Establece el color de caracter
   * \param[in] foreColor Color de caracter
   * \param[in] intensity Intensidad. El valor por defecto es Intensity::NORMAL
   * \see Console::Color, Console::Intensity
   */
  void setConsoleForegroundColor(Console::Color foreColor, 
                                 Console::Intensity intensity = Console::Intensity::normal);

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
   * \brief Establece el título de la consola
   * \param[in] title Titulo de la consola
   */
  void setTitle(const std::string &title);

protected:

#ifdef TL_MESSAGE_HANDLER  

  /*!
   * \brief onMsgDebug
   * \param msg
   * \param date
   */
  void onMsgDebug(const std::string &message, 
                  const std::string &date) override;

  /*!
   * \brief onMsgInfo
   * \param msg
   * \param date
   */
  void onMsgInfo(const std::string &message, 
                 const std::string &date) override;

  /*!
   * \brief onMsgWarning
   * \param msg
   * \param date
   */
  void onMsgWarning(const std::string &message, 
                    const std::string &date) override;

  /*!
   * \brief onMsgError
   * \param msg
   * \param date
   */
  void onMsgError(const std::string &message, 
                  const std::string &date) override;

#endif // TL_MESSAGE_HANDLER 

private:

#ifdef WIN32

  /*!
   * \brief Inicializa la consola guardando la configuración  actual.
   * \param handle
   */
  void init(DWORD handle);

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

#ifdef TL_ENABLE_DEPRECATED_METHODS

public:

  /*!
  * \brief Singleton que devuelve una referencia unica de un objeto Console
  * \deprecated Use 'instance()'  en su lugar
  */
  TL_DEPRECATED("Console::instance", "2.0")
    static Console &getInstance();

  /*!
  * \brief Niveles de mensaje activados
  * \return Flag con los niveles de mensajes aceptados por la consola
  * \see EnumFlags
  * \deprecated Use 'messageLevel()'  en su lugar
  */
  TL_DEPRECATED("Console::messageLevel", "2.0")
    EnumFlags<MessageLevel> getMessageLevel() const;

#endif // TL_ENABLE_DEPRECATED_METHODS

private:

#ifdef WIN32

  /* Consola de Windows */

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
  WORD mBackgroundColor;

  TL_TODO("Por terminar")
  CONSOLE_FONT_INFOEX mIniFont;
  CONSOLE_FONT_INFOEX mCurrentFont;

#else

  /* Consola Linux */

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
  int mBackgroundColor;
  int mBold;

#endif
  
  static EnumFlags<MessageLevel> sLevel;
  static std::unique_ptr<Console> sObjConsole;
  static std::mutex mtx;

};


/* ---------------------------------------------------------------------------------- */


/*!
 * \brief The Argument class
 */
class TL_EXPORT Argument
{

public:

  /*!
   * \brief Constructora argumento
   * \param[in] name Nombre del argumento
   * \param[in] description Descripción del argumento
   */
  Argument(std::string name, std::string description);

  /*!
   * \brief Constructora argumento
   * \param[in] shortName Nombre corto del argumento
   * \param[in] description Descripción del argumento
   */
  Argument(const char &shortName, std::string description);

  /*!
   * \brief Constructora argumento
   * \param[in] name Nombre del argumento
   * \param[in] shortName Nombre corto del argumento
   * \param[in] description Descripción del argumento
   */
  Argument(std::string name, const char &shortName, std::string description);

  /*!
   * \brief Constructora de copia
   * \param[in] argument Objeto que se copia
   */
  Argument(const Argument &argument);

  /*!
   * \brief Constructora de movimiento
   * \param[in] argument Objeto que se mueve
   */
  Argument(Argument &&argument) TL_NOEXCEPT;

  /*!
   * \brief Destructora
   */
  virtual ~Argument() = default;

  Argument &operator = (const Argument &arg);
  Argument &operator = (Argument &&arg) TL_NOEXCEPT;

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
   * \brief Establece el valor del argumento a partir de una cadena de texto
   * \param[in] value Valor del argumento como cadena de texto
   */
  virtual void fromString(const std::string &value) = 0;

  /*!
   * \brief Comprueba si el valor pasado al argumento es valido
   * \return
   */
  virtual bool isValid() = 0;

private:

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

};



/* ---------------------------------------------------------------------------------- */




/*!
 * \brief Template para gestionar diferentes tipos de argumentos
 */
template <typename T, bool required = true>
class Argument_
  : public Argument
{

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
   * \brief Constructora de movimiento
   * \param[in] argument Objeto que se mueve
   */
  Argument_(Argument_ &&argument) TL_NOEXCEPT;

  /*!
   * \brief Destructora
   */
  ~Argument_() override = default;

  Argument_ &operator = (const Argument_ &argument);
  Argument_ &operator = (Argument_ &&arg) TL_NOEXCEPT;

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
  //std::string toString() const override;

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

  bool isValid() override;

protected:

  void setValid(bool valid);

private:

  T *mValue;
  bool bValid;
};


/* Definición de unos alias para los tipos mas frecuentes */

using ArgumentIntegerRequired = Argument_<int, true>;
using ArgumentIntegerOptional = Argument_<int, false>;
using ArgumentDoubleRequired = Argument_<double, true>;
using ArgumentDoubleOptional = Argument_<double, false>;
using ArgumentFloatRequired = Argument_<float, true>;
using ArgumentFloatOptional = Argument_<float, false>;
using ArgumentBooleanRequired = Argument_<bool, true>;
using ArgumentBooleanOptional = Argument_<bool, false>;
using ArgumentStringRequired = Argument_<std::string, true>;
using ArgumentStringOptional = Argument_<std::string, false>;
using ArgumentPathRequired = Argument_<Path, true>;
using ArgumentPathOptional = Argument_<Path, false>;

/* Implementación */

template<typename T, bool required> inline
Argument_<T, required>::Argument_(const std::string &name,
                                  const std::string &description,
                                  T *value)
  : Argument(name, description),
    mValue(value),
    bValid(true)
{
}

template<typename T, bool required> inline
Argument_<T, required>::Argument_(const char &shortName,
                                  const std::string &description,
                                  T *value)
  : Argument(shortName, description),
    mValue(value),
    bValid(true)
{
}

template<typename T, bool required> inline
Argument_<T, required>::Argument_(const std::string &name,
                                  const char &shortName,
                                  const std::string &description,
                                  T *value)
  : Argument(name, shortName, description),
    mValue(value),
    bValid(true)
{
}

template<typename T, bool required> inline
Argument_<T, required>::Argument_(const Argument_ &argument)
  : Argument(argument),
    mValue(argument.mValue),
    bValid(argument.bValid)
{
}

template<typename T, bool required> inline
Argument_<T, required>::Argument_(Argument_ &&argument) TL_NOEXCEPT
  : Argument(std::forward<Argument>(argument)),
    mValue(std::move(argument.mValue)),
    bValid(std::move(argument.bValid))
{
}

template<typename T, bool required> inline
Argument_<T, required> &Argument_<T, required>::operator=(const Argument_ &argument)
{
  if (this != &argument){
    Argument::operator=(argument);
    this->mValue = argument.mValue;
    this->bValid = argument.bValid;
  }
  return *this;
}

template<typename T, bool required> inline
Argument_<T, required> &Argument_<T, required>::operator=(Argument_ &&argument) TL_NOEXCEPT
{
  if (this != &argument){
    Argument::operator=(std::forward<Argument>(argument));
    this->mValue = std::move(argument.mValue);
    this->bValid = std::move(argument.bValid);
  }
  return *this;
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
std::string Argument_<Path, true>::typeName() const
{
  return "Path";
}
template<> inline
std::string Argument_<Path, false>::typeName() const
{
  return "Path";
}


template<typename T, bool required> inline
bool Argument_<T, required>::isRequired() const
{
  return required;
}

//template<typename T, bool required> inline
//std::string Argument_<T, required>::toString() const
//{
//  std::string val;
//  if(typeid(T) == typeid(bool)) {
//    val = *mValue ? "true" : "false";
//  } else if (std::is_integral<T>::value) {
//    val = std::to_string(*mValue);
//  } else if (std::is_floating_point<T>::value){
//    val = std::to_string(*mValue);
//  } else if(typeid(T) == typeid(std::string)) {
//    val = *mValue;
//  }
//  return val;
//}
//
//template<> inline
//std::string Argument_<std::string, true>::toString() const
//{
//  return *mValue;
//}
//
//template<> inline
//std::string Argument_<std::string, false>::toString() const
//{
//  return *mValue;
//}
//
//
//#if (__cplusplus >= 201703L)
//template<> inline
//std::string Argument_<std::filesystem::path, true>::toString() const
//{
//  return mValue->string();
//}
//
//template<> inline
//std::string Argument_<std::filesystem::path, false>::toString() const
//{
//  return mValue->string();
//}
//#else
//template<> inline
//std::string Argument_<boost::filesystem::path, true>::toString() const
//{
//  return mValue->string();
//}
//
//template<> inline
//std::string Argument_<boost::filesystem::path, false>::toString() const
//{
//  return mValue->string();
//}
//#endif

template<typename T, bool required> inline
void Argument_<T, required>::fromString(const std::string &value)
{
  if(typeid(T) == typeid(bool)) {
    if (value == "true" || value == "1")
      *mValue = true;
    else
      *mValue = false;
  } else if (std::is_integral<T>::value) {
    *mValue = stringToInteger(value);
  } else if (std::is_floating_point<T>::value) {
    *mValue = std::stod(value);
  } else {
    /// No se ha podido obtener el valor
    bValid = false;
  }
}


template<> inline
void Argument_<std::string, true>::fromString(const std::string &value)
{
  *mValue = value;
  bValid = true;
}

template<> inline
void Argument_<std::string, false>::fromString(const std::string &value)
{
  *mValue = value;
  bValid = true;
}

template<> inline
void Argument_<Path, true>::fromString(const std::string &value)
{
  *mValue = value;
  bValid = true;
}

template<> inline
void Argument_<Path, false>::fromString(const std::string &value)
{
  *mValue = value;
  bValid = true;
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
  bValid = true;
}

template<typename T, bool required> inline
bool Argument_<T, required>::isValid()
{
  TL_TODO("Incluir clase ArgumentValidator")
  return bValid;
}

template<typename T, bool required>
void Argument_<T, required>::setValid(bool valid)
{
  bValid = valid;
}



/*!
 * \brief Argumento lista de opciones
 */
template <typename T, bool required = true>
class ArgumentList_
  : public Argument_<T, required>
{

public:

  /*!
   * \brief Constructora argumento lista de opciones
   * \param[in] name Nombre del argumento
   * \param[in] description Descripción del argumento
   * \param[in] values Vector con los posibles valores que puede tomar el argumento
   * \param[in,out] idx Indice del valor que toma el argumento. En el caso de argumentos opcionales establece el valor por defecto
   */
  ArgumentList_(const std::string &name,
                const std::string &description,
                std::vector<T> &values, size_t *idx);

  /*!
   * \brief Constructora argumento lista de opciones
   * \param[in] shortName Nombre corto del argumento
   * \param[in] description Descripción del argumento
   * \param[in] values Vector con los posibles valores que puede tomar el argumento
   * \param[in,out] idx Indice del valor que toma el argumento. En el caso de argumentos opcionales establece el valor por defecto
   */
  ArgumentList_(const char &shortName,
                const std::string &description,
                std::vector<T> &values,
                size_t *idx);

  /*!
   * \brief Constructora argumento lista de opciones
   * \param[in] name Nombre del argumento
   * \param[in] shortName Nombre corto del argumento
   * \param[in] description Descripción del argumento
   * \param[in] values Vector con los posibles valores que puede tomar el argumento
   * \param[in,out] idx Indice del valor que toma el argumento. En el caso de argumentos opcionales establece el valor por defecto
   */
  ArgumentList_(const std::string &name,
                const char &shortName,
                const std::string &description,
                std::vector<T> &values,
                size_t *idx);

  /*!
   * \brief Constructora de copia
   * \param[in] argument Objeto que se copia
   */
  ArgumentList_(const ArgumentList_ &argumentList);

  /*!
   * \brief Destructora
   */
  ~ArgumentList_() override = default;

  void fromString(const std::string &value) override;

  void setValue(const T &value) override;

private:

  std::vector<T> mValues;
  size_t *mIdx;
};


/* Definición de unos alias para los tipos mas frecuentes */

using ArgumentListIntegerRequired = ArgumentList_<int, true>;
using ArgumentListIntegerOptional = ArgumentList_<int, false>;
using ArgumentListDoubleRequired = ArgumentList_<double, true>;
using ArgumentListDoubleOptional = ArgumentList_<double, false>;
using ArgumentListFloatRequired = ArgumentList_<float, true>;
using ArgumentListFloatOptional = ArgumentList_<float, false>;
using ArgumentListBooleanRequired = ArgumentList_<bool, true>;
using ArgumentListBooleanOptional = ArgumentList_<bool, false>;
using ArgumentListStringRequired = ArgumentList_<std::string, true>;
using ArgumentListStringOptional = ArgumentList_<std::string, false>;
using ArgumentListPathRequired = ArgumentList_<Path, true>;
using ArgumentListPathOptional = ArgumentList_<Path, false>;


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
ArgumentList_<T, required>::ArgumentList_(const ArgumentList_ &argumentList)
  : Argument_<T, required>(argumentList),
    mValues(argumentList.mValues),
    mIdx(argumentList.mIdx)
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
    this->setValid(true);
  } else {
    Argument_<T, required>::setValue(prev_value);
    this->setValid(false);
  }
}

template<typename T, bool required> inline
void ArgumentList_<T, required>::setValue(const T &value)
{
  bool bFind = false;
  size_t idx = 0;
  for(auto &_value : mValues){
    if (value == _value){
      Argument_<T, required>::setValue(value);
      bFind = true;
      break;
    }
    idx++;
  }
  if (bFind) {
    *mIdx = idx;
    this->setValid(true);
  } else {
    this->setValid(false);
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
class ArgumentValidator<T, typename std::enable_if<std::is_arithmetic<T>::value>::type>
{

public:

  ArgumentValidator()
    : mMin(),
      mMax(std::numeric_limits<T>().max())
  {}

  bool validate(T value)
  {
    return (value > mMin && value < mMax);
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

private:

  T mMin;
  T mMax;
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
 *  if (status == Command::Status::parse_error ) {
 *    return 1;
 *  } else if (status == Command::Status::show_help) {
 *    return 0;
 *  } else if (status == Command::Status::show_licence) {
 *    return 0;
 *  } else if (status == Command::Status::show_version) {
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
    parse_success,  /*!< El parseo se ejecuto correctamente */
    parse_error,    /*!< Ocurrio un error al ejecutar el comando */
    show_help,      /*!< Se pasa como parametro: help. Muestra la ayuda del programa */
    show_version,   /*!< Se pasa como parametro: version. Se muestra la versión del programa */
    show_licence    /*!< Se pasa como parametro: licence. Se muestra la información de licencia */
#ifdef TL_ENABLE_DEPRECATED_METHODS
    ,
    PARSE_SUCCESS = parse_success,  /*!< El parseo se ejecuto correctamente */
    PARSE_ERROR   = parse_error,    /*!< Ocurrio un error al ejecutar el comando */
    SHOW_HELP     = show_help,      /*!< Se pasa como parametro: help. Muestra la ayuda del programa */
    SHOW_VERSION  = show_version,   /*!< Se pasa como parametro: version. Se muestra la versión del programa */
    SHOW_LICENCE  = show_licence    /*!< Se pasa como parametro: licence. Se muestra la información de licencia */
#endif
  };

  /*!
   * \brief Allocator
   */
  using allocator_type = std::list<std::shared_ptr<Argument> >::allocator_type;

  /*!
   * \brief value_type
   */
  using value_type = std::list<std::shared_ptr<Argument> >::value_type;

  /*!
   * \brief Tipo entero sin signo (por lo general size_t)
   */
  using size_type = std::list<std::shared_ptr<Argument> >::size_type;

  /*!
   * \brief Tipo entero con signo (por lo general ptrdiff_t)
   */
  using difference_type = std::list<std::shared_ptr<Argument> >::difference_type;

  /*!
   * \brief std::allocator_traits<Allocator>::pointer
   */
  using pointer = std::list<std::shared_ptr<Argument> >::pointer;

  /*!
   * \brief std::allocator_traits<Allocator>::const_pointer
   */
  using const_pointer = std::list<std::shared_ptr<Argument> >::const_pointer;

  /*!
   * \brief value_type&
   */
  using reference = std::list<std::shared_ptr<Argument> >::reference;

  /*!
   * \brief const value_type&
   */
  using const_reference = std::list<std::shared_ptr<Argument> >::const_reference;

  /*!
   * \brief Iterador de acceso aleatorio
   */
  using iterator = std::list<std::shared_ptr<Argument> >::iterator;

  /*!
   * \brief Iterador constante de acceso aleatorio
   */
  using const_iterator = std::list<std::shared_ptr<Argument> >::const_iterator;

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
  Command(std::string name, std::string description);

  /*!
   * \brief Constructora de lista
   * \param[in] name Nombre del comando
   * \param[in] description Descripción del comando
   * \param[in] arguments listado de argumentos
   */
  Command(std::string name, std::string description, std::initializer_list<std::shared_ptr<Argument>> arguments);

  ~Command() = default;

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
   * \return Devuelve el estado. 'parse_error' en caso de error y 'parse_success' cuando el parseo se ha hecho correctamente
   * \see CmdParser::Status
   */
  Status parse(int argc, const char* const argv[]);

  /*!
   * \brief Devuelve un iterador al inicio
   * \return Iterador al primer elemento
   */
  iterator begin() TL_NOEXCEPT;

  /*!
   * \brief Devuelve un iterador constante al inicio
   * \return Iterador al primer elemento
   */
  const_iterator begin() const TL_NOEXCEPT;

  /*!
   * \brief Devuelve un iterador al siguiente elemento después del último argumento
   * Este elemento actúa como un marcador de posición, intentar acceder a él resulta en un comportamiento no definido
   * \return Iterador al siguiente elemento después del último argumento
   */
  iterator end() TL_NOEXCEPT;

  /*!
   * \brief Devuelve un iterador constante al siguiente elemento después del último argumento
   * Este elemento actúa como un marcador de posición, intentar acceder a él resulta en un comportamiento no definido
   * \return Iterador al siguiente elemento después del último argumento
   */
  const_iterator end() const TL_NOEXCEPT;

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
  void clear() TL_NOEXCEPT;

  /*!
   * \brief Comprueba si no hay argumentos
   * \return true si el contenedor está vacío y false en caso contrario
   */
  bool empty() const TL_NOEXCEPT;

  /*!
   * \brief Devuelve el tamaño del contenedor
   * \return Tamaño
   */
  size_type size() const TL_NOEXCEPT;

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

  /*!
   * \brief Añade un ejemplo de uso
   */
  void addExample(const std::string &example);

  void setLicence(const Licence &licence);

protected:

  void init();

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

  std::list<std::string> mExamples;

  Licence mLicence;
};



/* ---------------------------------------------------------------------------------- */


class TL_EXPORT CommandList
{

public:

  /*!
   * \brief Estado de salida del parseo del comando
   */
  enum class Status
  {
    parse_success,  /*!< El parseo se ejecuto correctamente */
    parse_error,    /*!< Ocurrio un error al ejecutar el comando */
    show_help,      /*!< Se pasa como parametro: help. Muestra la ayuda del programa */
    show_version,   /*!< Se pasa como parametro: version. Se muestra la versión del programa */
    show_licence    /*!< Se pasa como parametro: licence. Se muestra la información de licencia */
#ifdef TL_ENABLE_DEPRECATED_METHODS
    ,
    PARSE_SUCCESS = parse_success,  /*!< El parseo se ejecuto correctamente */
    PARSE_ERROR = parse_error,    /*!< Ocurrio un error al ejecutar el comando */
    SHOW_HELP = show_help,      /*!< Se pasa como parametro: help. Muestra la ayuda del programa */
    SHOW_VERSION = show_version,   /*!< Se pasa como parametro: version. Se muestra la versión del programa */
    SHOW_LICENCE = show_licence    /*!< Se pasa como parametro: licence. Se muestra la información de licencia */
#endif
  };

  /*!
   * \brief Allocator
   */
  using allocator_type = std::list<std::shared_ptr<Command> >::allocator_type;

  /*!
   * \brief value_type
   */
  using value_type = std::list<std::shared_ptr<Command> >::value_type;

  /*!
   * \brief Tipo entero sin signo (por lo general size_t)
   */
  using size_type = std::list<std::shared_ptr<Command> >::size_type;

  /*!
   * \brief Tipo entero con signo (por lo general ptrdiff_t)
   */
  using difference_type = std::list<std::shared_ptr<Command> >::difference_type;

  /*!
   * \brief std::allocator_traits<Allocator>::pointer
   */
  using pointer = std::list<std::shared_ptr<Command> >::pointer;

  /*!
   * \brief std::allocator_traits<Allocator>::const_pointer
   */
  using const_pointer = std::list<std::shared_ptr<Command> >::const_pointer;

  /*!
   * \brief value_type&
   */
  using reference = std::list<std::shared_ptr<Command> >::reference;

  /*!
   * \brief const value_type&
   */
  using const_reference = std::list<std::shared_ptr<Command> >::const_reference;

  /*!
   * \brief Iterador de acceso aleatorio
   */
  using iterator = std::list<std::shared_ptr<Command> >::iterator;

  /*!
   * \brief Iterador constante de acceso aleatorio
   */
  using const_iterator = std::list<std::shared_ptr<Command> >::const_iterator;

public:

  /*!
   * \brief Constructora
   */
  CommandList();

  /*!
   * \brief Constructora
   * \param[in] name Nombre del comando
   * \param[in] description Descripción del comando
   */
  CommandList(std::string name,
              std::string description);
  
  /*!
   * \brief Constructor de copia
   * \param[in] commandList Objeto CommandList que se copia
   */
  CommandList(const CommandList &commandList);

  /*!
   * \brief Constructor de movimiento
   * \param[in] commandList Objeto CommandList que se mueve
   */
  CommandList(CommandList &&commandList) TL_NOEXCEPT;

  /*!
   * \brief Constructora de lista
   * \param[in] name Nombre del comando
   * \param[in] description Descripción del comando
   * \param[in] commands listado de comandos
   */
  CommandList(std::string name,
              std::string description,
              std::initializer_list<std::shared_ptr<Command>> commands);

  ~CommandList() = default;

  /*!
   * \brief Devuelve el nombre del programa
   * \return Nombre del programa
   */
  std::string name() const;

  /*!
   * \brief Establece el nombre del programa
   * \param[in] name Nombre del programa
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
  iterator begin() TL_NOEXCEPT;

  /*!
   * \brief Devuelve un iterador constante al inicio
   * \return Iterador al primer elemento
   */
  const_iterator begin() const TL_NOEXCEPT;

  /*!
   * \brief Devuelve un iterador al siguiente elemento después del último comando
   * Este elemento actúa como un marcador de posición, intentar acceder a él resulta en un comportamiento no definido
   * \return Iterador al siguiente elemento después del último comando
   */
  iterator end() TL_NOEXCEPT;

  /*!
   * \brief Devuelve un iterador constante al siguiente elemento después del último comando
   * Este elemento actúa como un marcador de posición, intentar acceder a él resulta en un comportamiento no definido
   * \return Iterador al siguiente elemento después del último comando
   */
  const_iterator end() const TL_NOEXCEPT;

  /*!
   * \brief Agrega un comando mediante copia al final
   * \param[in] cmd Comando que se añade
   */
  void push_back(const std::shared_ptr<Command> &cmd);

  /*!
   * \brief Agrega un comando mediante movimiento al final
   * \param[in] cmd Comando que se añade
   */
  void push_back(std::shared_ptr<Command> &&cmd) TL_NOEXCEPT;

  /*!
   * \brief Elimina los comandos
   */
  void clear() TL_NOEXCEPT;

  /*!
   * \brief Comprueba si no hay comandos
   * \return true si el contenedor está vacío y false en caso contrario
   */
  bool empty() const TL_NOEXCEPT;

  /*!
   * \brief Devuelve el número de comandos
   * \return Número de comandos
   */
  size_type size() const TL_NOEXCEPT;

  /*!
   * \brief Asignación de copia
   */
  CommandList& operator=(const CommandList& cmdList);

  /*!
   * \brief Asignación de movimiento
   */
  CommandList& operator=(CommandList &&cmdList) TL_NOEXCEPT;

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

  std::string commandName() const;


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
  std::list<std::shared_ptr<Command>> mCommands;

  std::shared_ptr<Command> mCommand;

  /*!
   * \brief Versión del programa
   */
  std::string mVersion;

};



/*! \} */ // end of Console

/*! \} */ // end of utilities


} // End namespace tl


#endif // TL_CORE_CONSOLE_H
