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
 * License along with TidopLib. If not, see <http://www.gnu.org/licenses>.*
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#ifndef TL_CORE_MESSAGES_H
#define TL_CORE_MESSAGES_H

#include "tidop/config.h"
#include "tidop/core/defs.h"

#include <iostream>
#include <memory>
#include <list>
#include <mutex>

#include "tidop/core/flags.h"

namespace tl
{


/*! \addtogroup core
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
  msg_error   = 1 << 1,                               /*!< Errores. */
  msg_warning = 1 << 2,                               /*!< Warnings */
  msg_info    = 1 << 3,                               /*!< Otra información. */
  msg_trace   = 1 << 4,
  msg_verbose = msg_error | msg_warning | msg_info    /*!< Todos los mensajes. */
};
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
#if CPP_VERSION >= 17
    virtual void onMsgDebug(std::string_view message, 
                            std::string_view date) = 0;
#else
    virtual void onMsgDebug(const std::string &message, 
                            const std::string &date) = 0;
#endif

    /*!
     * \brief Mensaje de información
     * \param message Mensaje que recibe el escuchador
     * \param date Fecha y hora en la que se emite el mensaje
     */
#if CPP_VERSION >= 17
     virtual void onMsgInfo(std::string_view message, 
                            std::string_view date) = 0;
#else
     virtual void onMsgInfo(const std::string &message, 
                            const std::string &date) = 0;
#endif

    /*!
     * \brief Mensaje de advertencia
     * \param message Mensaje que recibe el escuchador
     * \param date Fecha y hora en la que se emite el mensaje
     */
#if CPP_VERSION >= 17
    virtual void onMsgWarning(std::string_view message, 
                              std::string_view date) = 0;
#else
    virtual void onMsgWarning(const std::string &message, 
                              const std::string &date) = 0;
#endif

    /*!
     * \brief Mensaje de error
     * \param message Mensaje que recibe el escuchador
     * \param date Fecha y hora en la que se emite el mensaje
     */
#if CPP_VERSION >= 17
    virtual void onMsgError(std::string_view message, 
                            std::string_view date) = 0;
#else
    virtual void onMsgError(const std::string &message, 
                            const std::string &date) = 0;
#endif
  };

  /*!
   * \brief Clase que representa un mensaje individual
   */
  class TL_EXPORT Message
  {

  public:

    //Message();

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
    ~Message() = default;

    /*!
     * \brief Devuelve la fecha y hora del mensaje
     * \return Fecha y hora del mensaje
     */
    std::string date() const;

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

  TL_DISABLE_COPY(MessageManager)
  TL_DISABLE_MOVE(MessageManager)

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

  //friend class App;
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


/*! \} */ // end of core


} // End namespace tl

#endif // TL_CORE_MESSAGES_H
