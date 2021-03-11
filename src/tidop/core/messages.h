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

#ifndef TL_CORE_MESSAGES_H
#define TL_CORE_MESSAGES_H

#include "config_tl.h"
#include "tidop/core/defs.h"

#include <iostream>
#include <memory>
#include <list>
#include <mutex>

#include "tidop/core/utils.h"
#include "tidop/core/flags.h"

namespace tl
{


/*! \addtogroup utilities
 *  \{
 */

/*! \defgroup Messages Gestión de mensajes
 *
 *  \{
 */


/*!
 * \brief Niveles de información de los mensajes
 */
enum class MessageLevel : int8_t {
  msg_debug   = 1 << 0,                               /*!< Información extra para depuración. */
  msg_error   = 1 << 1,                               /*!< Sólo errores. */
  msg_warning = 1 << 2,                               /*!< Warnings */
  msg_info    = 1 << 3,                               /*!< Otra información. */
  msg_verbose = msg_error | msg_warning | msg_info    /*!< Todos los mensajes. */
#ifdef TL_ENABLE_DEPRECATED_METHODS
  ,
  MSG_DEBUG   = msg_debug,                            /*!< Información extra para depuración. */
  MSG_ERROR   = msg_error,                            /*!< Sólo errores. */
  MSG_WARNING = msg_warning,                          /*!< Warnings */
  MSG_INFO    = msg_info,                             /*!< Otra información. */
  MSG_VERBOSE = msg_verbose                           /*!< Todos los mensajes. */
#endif
};

/// Se permiten operaciones a nivel de bit para el enum MessageOutput
ALLOW_BITWISE_FLAG_OPERATIONS(MessageLevel);

#ifdef TL_MESSAGE_HANDLER

struct _msgProperties {
  const char *normal;
  const char *extend;
};


/*!
 * \brief Clase para gestionar los mensajes de la librería
 *
 * Proporciona las herramientas necesarias para la impresión de mensajes por
 * consola, en un fichero log y la comunicación con otras librerias o aplicaciones
 */
class TL_EXPORT MessageManager
{

public:

  /*!
   * \brief Interfaz que deben implementar las clases para comunicarse con
   * el gestor de mensajes
   *
   * Las clases que implementen este listener y se subcriban al gestor de
   * mensajes mediante el método addListener() recibiran los diferentes 
   * mensajes (Depuración, error, etc) que se emitan desde la librería.
   */
	class TL_EXPORT Listener
  {

  public:

    /*!
     * \brief Constructora
     * \param[in] add Si add es true se añade directamente al gestor de mensajes
     */
    Listener(bool add = true);

    /*!
     * \brief destructora
     */
    virtual ~Listener() = 0;

    /*!
     * \brief Mensaje de depuración
     * \param message Mensaje que recibe el escuchador
     * \param date Fecha y hora en la que se emite el mensaje
     */
    virtual void onMsgDebug(const std::string &message, 
                            const std::string &date) = 0;

    /*!
     * \brief Mensaje de información
     * \param message Mensaje que recibe el escuchador
     * \param date Fecha y hora en la que se emite el mensaje
     */
    virtual void onMsgInfo(const std::string &message, 
                           const std::string &date) = 0;

    /*!
     * \brief Mensaje de advertencia
     * \param message Mensaje que recibe el escuchador
     * \param date Fecha y hora en la que se emite el mensaje
     */
    virtual void onMsgWarning(const std::string &message, 
                              const std::string &date) = 0;

    /*!
     * \brief Mensaje de error
     * \param message Mensaje que recibe el escuchador
     * \param date Fecha y hora en la que se emite el mensaje
     */
    virtual void onMsgError(const std::string &message, 
                            const std::string &date) = 0;

  };

  /*!
   * \brief Clase que representa un mensaje individual
   */
  class TL_EXPORT Message
  {

  public:
    
    /*!
     * \brief Constructora
     *
     * Mediante una cadena con formato y una lista variable de argumentos se
     * permite formatear los mensajes de una forma rapida.
     *
     * \param[in] message Cadena con formato que se rellenará para componer el mensaje
     *
     * <b>Ejemplo</b>
     *
     * \code
     *   Message message("Cadena de ejemplo nº%i", 1); // Da como resultado "Cadena de ejemplo nº1"
     * \endcode
     *
     */
    Message(const char *message, ...);
   
    /*!
     * \brief Destructora
     */
    ~Message() { }

    /*!
     * \brief Devuelve la fecha y hora del mensaje
     * \return Fecha y hora del mensaje
     */
    std::string date() const;

    /*!
     * \brief file
     * \return
     */
    std::string file() const;

    /*!
     * \brief function
     * \return
     */
    std::string function() const;
    
    /*!
     * \brief Nivel del mensaje
     * \return Devuelve el nivel de mensaje establecido
     */
    MessageLevel level() const;

#ifdef TL_ENABLE_DEPRECATED_METHODS
     
    /*!
     * \brief Devuelve la fecha y hora del mensaje
     * \return Fecha y hora del mensaje
     * \deprecated Use 'MessageManager::Message::date()' en su lugar 
     */
    TL_DEPRECATED("MessageManager::Message::date()", "2.0")
    const char *getDate() const;

    /*!
     * \brief getFile
     * \return
     * \deprecated Use 'MessageManager::Message::file()' en su lugar 
     */
    TL_DEPRECATED("MessageManager::Message::file()", "2.0")
    const char *getFile() const;

    /*!
     * \brief getFunction
     * \return
     * \deprecated Use 'MessageManager::Message::function()' en su lugar 
     */
    TL_DEPRECATED("MessageManager::Message::function()", "2.0")
    const char *getFunction() const;
    /*!
     * \brief Nivel del mensaje
     * \return Devuelve el nivel de mensaje establecido
     * \deprecated Use 'MessageManager::Message::level()' en su lugar
     */
    TL_DEPRECATED("MessageManager::Message::level()", "2.0")
      MessageLevel getLevel() const;

    /*!
     * \brief getLine
     * \return
     * \deprecated Use 'MessageManager::Message::line()' en su lugar 
     */
    TL_DEPRECATED("MessageManager::Message::line()", "2.0")
    int getLine() const;

    /*!
     * \brief Devuelve el mensaje como cadena de texto
     * \return Mensaje
     * \deprecated Use 'MessageManager::Message::message()' en su lugar
     */
    TL_DEPRECATED("MessageManager::Message::message()", "2.0")
      const char *getMessage() const;

#endif // TL_ENABLE_DEPRECATED_METHODS
    
    /*!
     * \brief getLine
     * \return
     */
    int line() const;

    /*!
     * \brief Devuelve el mensaje como cadena de texto
     * \return Mensaje
     */
    std::string message() const;

    /*!
     * \brief setTimeLogFormat
     * \code
     * Message::setTimeLogFormat( "%d - %b - %Y (%H:%M:%S)" );
     * \endcode
     * \param[in] timeTemplate
     */
    static void setTimeLogFormat(const std::string &timeTemplate);

    void setMessageLevel(const MessageLevel &level);

    /*!
     * \brief Añade la localización del mensaje (fichero, número de línea
     * y función) y el nivel de información de los mensajes
     * \param[in] level Nivel de información del mensaje
     * \param[in] file Nombre del fichero desde el cual se lanza el mensaje
     * \param[in] line Número de línea del mensaje
     * \param[in] function Nombre de la función desde la que se lanza el mensaje
     */
    void setMessageProperties(const MessageLevel &level, 
                              const std::string &file, 
                              int line, 
                              const std::string &function);
    
  private:

    /*!
     * \brief Mensaje
     */
    std::string mMessage;

    /*!
     * \brief Fecha y hora en la que se emitio el mensaje
     */
    std::string mDate;

    /*!
     * \brief Nivel del mensaje
     */
    MessageLevel mLevel;

    /*!
     * \brief Fichero en el cual se genera el mensaje
     */
    std::string mFile;

    /*!
     * \brief Número de línea donde se ha generado el mensaje
     */
    int mLine;

    /*!
     * \brief Nombre de la función donde se genera el mensaje
     */
    std::string mFunction;

    /*!
     * \brief Plantilla para el formateo de fecha y hora de los mensajes.
     *
     * Por defecto la plantilla es:
     * \code
     * std::string Message::timeLogTemplate = "%d/%b/%Y %H:%M:%S";
     * \endcode
     * \see setTimeLogFormat
     */
    static std::string sTimeLogFormat;

  };

private:

  /*!
   * \brief Constructora privada MessageManager
   */
  MessageManager();

public:

  /*!
   * \brief Destructora
   */
  ~MessageManager();

  MessageManager(const MessageManager &) = delete;
  MessageManager(MessageManager &&) = delete;
  MessageManager operator=(const MessageManager &) = delete;
  MessageManager operator=(MessageManager &&) = delete;

  /*!
   * \brief Singleton para obtener una referencia
   */
  static MessageManager &instance();

  /*!
   * \brief Añade un escuchador de mensajes
   * \param[in] listener Objeto escuchador
   */
  void addListener(Listener *listener);

  /*!
   * \brief Inicializa los manejadores para las librerias externas
   */
  //static void initExternalHandlers();

  /*!
   * \brief Pausa el envio de mensajes
   */
  static void pause();

  /*!
   * \brief Lanza un mensaje para que aquellos objetos que estén subscritos lo reciban
   * \param[in] message Mensaje que se lanza
   * \param[in] level Nivel de log
   * \param[in] file Fichero
   * \param[in] line Linea
   * \param[in] function Función
   */
  static void release(const std::string &message, 
                      const MessageLevel &level, 
                      const std::string &file = std::string(), 
                      int line = -1, 
                      const std::string &function = std::string());

  /*!
   * \brief Lanza un mensaje para que aquellos objetos que estén subscritos lo reciban
   * \param[in] message Mensaje que se lanza
   */
  static void release(const Message &message);

  /*!
   * \brief Quita un escuchador de mensajes
   * \param[in] listener Objeto escuchador
   */
  void removeListener(Listener *listener);

  /*!
   * \brief Reanuda el envio de mensajes
   */
  static void resume();

protected:

  void onDebug(const std::string &message, const std::string &date);
  void onInfo(const std::string &message, const std::string &date);
  void onWarning(const std::string &message, const std::string &date);
  void onError(const std::string &message, const std::string &date);

private:

  static _msgProperties messageProperties(MessageLevel msgLevel);

#ifdef TL_ENABLE_DEPRECATED_METHODS

public:

  /*!
   * \brief Singleton para obtener una referencia
   * \deprecated Use 'instance()'  en su lugar
   */
  TL_DEPRECATED("MessageManager::instance", "2.0")
    static MessageManager &getInstance();
#endif // TL_ENABLE_DEPRECATED_METHODS

private:

  /*!
   * \brief sObjMessage
   */
  static std::unique_ptr<MessageManager> sObjMessage;

  /*!
   * \brief Lista con los escuchadores subscritos al gestor de mensajes
   */
  std::list<Listener *> mListeners;

  static bool sStopHandler;

  static std::mutex sMutex;

};

#ifdef _DEBUG
#  define msgDebug(...)    tl::MessageManager::release(tl::MessageManager::Message(__VA_ARGS__).message(), tl::MessageLevel::msg_debug, __FILE__, __LINE__, TL_FUNCTION)
#  define msgInfo(...)     tl::MessageManager::release(tl::MessageManager::Message(__VA_ARGS__).message(), tl::MessageLevel::msg_info, __FILE__, __LINE__, TL_FUNCTION)
#  define msgWarning(...)  tl::MessageManager::release(tl::MessageManager::Message(__VA_ARGS__).message(), tl::MessageLevel::msg_warning, __FILE__, __LINE__, TL_FUNCTION)
#  define msgError(...)    tl::MessageManager::release(tl::MessageManager::Message(__VA_ARGS__).message(), tl::MessageLevel::msg_error, __FILE__, __LINE__, TL_FUNCTION)
#else
#  define msgDebug(...)    tl::MessageManager::release(tl::MessageManager::Message(__VA_ARGS__).message(), tl::MessageLevel::msg_debug)
#  define msgInfo(...)     tl::MessageManager::release(tl::MessageManager::Message(__VA_ARGS__).message(), tl::MessageLevel::msg_info)
#  define msgWarning(...)  tl::MessageManager::release(tl::MessageManager::Message(__VA_ARGS__).message(), tl::MessageLevel::msg_warning)
#  define msgError(...)    tl::MessageManager::release(tl::MessageManager::Message(__VA_ARGS__).message(), tl::MessageLevel::msg_error)
#endif

#else  // End TL_MESSAGE_HANDLER

/// No se utiliza el manejador de mensajes

#  define msgDebug(...)
#  define msgInfo(...)
#  define msgWarning(...)
#  define msgError(...)

#endif

/*! \} */ // end of Messages


/*! \} */ // end of utilities


} // End namespace tl

#endif // TL_CORE_MESSAGES_H
