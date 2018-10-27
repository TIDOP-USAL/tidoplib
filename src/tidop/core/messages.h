/****************************************************************************
 *                                                                          *
 *  This file is part of TidopLib and can not be copied and/or distributed  *
 *  without the express permision of ITOS3D ENGINEERING S.L                 *
 *                                                                          *
 *  Contact: http://www.itos3d.com                                          *
 *           http://tidop.usal.es                                           *
 *                                                                          *
 *--------------------------------------------------------------------------*
 *                                                                          *
 *  Copyright (C) 2018, ITOS3D ENGINEERING S.L - All rights reserved        *
 *                                                                          *
 ****************************************************************************/

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
  MSG_DEBUG =   1 << 0,    /*!< Información extra para depuración. */
  MSG_ERROR =   1 << 1,    /*!< Sólo errores. */
  MSG_WARNING = 1 << 2,    /*!< Warnings */
  MSG_INFO =    1 << 3,    /*!< Otra información. */
  MSG_VERBOSE = MSG_ERROR | MSG_WARNING | MSG_INFO    /*!< Todos los mensajes. */
};

//Se permiten operaciones a nivel de bit para el enum MessageOutput
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
  protected:

    //std::shared_ptr<MessageManager> mMessage;

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
     * \param msg Mensaje que recibe el escuchador
     * \param date Fecha y hora en la que se emite el mensaje
     */
    virtual void onMsgDebug(const char *msg, const char *date) = 0;

    /*!
     * \brief Mensaje verbose
     * \param msg Mensaje que recibe el escuchador
     * \param date Fecha y hora en la que se emite el mensaje
     */
    //virtual void onMsgVerbose(const char *msg, const char *date) = 0;

    /*!
     * \brief Mensaje de información
     * \param msg Mensaje que recibe el escuchador
     * \param date Fecha y hora en la que se emite el mensaje
     */
    virtual void onMsgInfo(const char *msg, const char *date) = 0;

    /*!
     * \brief Mensaje de advertencia
     * \param msg Mensaje que recibe el escuchador
     * \param date Fecha y hora en la que se emite el mensaje
     */
    virtual void onMsgWarning(const char *msg, const char *date) = 0;

    /*!
     * \brief Mensaje de error
     * \param msg Mensaje que recibe el escuchador
     * \param date Fecha y hora en la que se emite el mensaje
     */
    virtual void onMsgError(const char *msg, const char *date) = 0;

  };

  /*!
   * \brief Clase que representa un mensaje individual
   */
  class TL_EXPORT Message
  {
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

  public:
    
    /*!
     * \brief Constructora
     *
     * Mediante una cadena con formato y una lista variable de argumentos se
     * permite formatear los mensajes de una forma rapida.
     *
     * \param[in] msg Cadena con formato que se rellenará para componer el mensaje
     *
     * <b>Ejemplo</b>
     *
     * \code
     *   Message msg("Cadena de ejemplo nº%i", 1); // Da como resultado "Cadena de ejemplo nº1"
     * \endcode
     *
     */
    Message(const char *msg, ...);
   
    /*!
     * \brief Destructora
     */
    ~Message() { }

    /*!
     * \brief Devuelve la fecha y hora del mensaje
     * \return Fecha y hora del mensaje
     */
    const char *getDate() const;

    /*!
     * \brief getFile
     * \return
     */
    const char *getFile() const;

    /*!
     * \brief getFunction
     * \return
     */
    const char *getFunction() const;

    /*!
     * \brief Nivel del mensaje
     * \return Devuelve el nivel de mensaje establecido
     */
    MessageLevel getLevel() const;

    /*!
     * \brief getLine
     * \return
     */
    int getLine() const;

    /*!
     * \brief Devuelve el mensaje como cadena de texto
     * \return Mensaje
     */
    const char *getMessage() const;

    /*!
     * \brief setTimeLogFormat
     * \code
     * Message::setTimeLogFormat( "%d - %b - %Y (%H:%M:%S)" );
     * \endcode
     * \param[in] timeTemplate
     */
    static void setTimeLogFormat(const char *timeTemplate);

    void setMessageLevel(const MessageLevel &level);

    /*!
     * \brief Añade la localización del mensaje (fichero, número de línea
     * y función) y el nivel de información de los mensajes
     * \param[in] level Nivel de información del mensaje
     * \param[in] file Nombre del fichero desde el cual se lanza el mensaje
     * \param[in] line Número de línea del mensaje
     * \param[in] function Nombre de la función desde la que se lanza el mensaje
     */
    void setMessageProperties(const MessageLevel &level, const char *file, int line, const char *function);
  
private:

    /*!
     * \brief messageOutput
     * \param msgLevel
     * \return
     */
    //std::string messageOutput(const MessageLevel &msgLevel);

    /*!
     * \brief messageOutput
     * \param msgLevel
     * \param file
     * \param line
     * \param function
     * \return
     */
    //std::string messageOutput(const MessageLevel &msgLevel, const char *file, int line, const char *function);

  };

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

  MessageManager(MessageManager const&) = delete;
  void operator=(MessageManager const&) = delete;

  /*!
   * \brief Singleton para obtener una referencia única
   */
  static MessageManager &getInstance();

  /*!
   * \brief Añade un escuchador de mensajes
   * \param[in] listener Objeto escuchador
   */
  void addListener(Listener *listener);

  /*!
   * \brief Inicializa los manejadores para las librerias externas
   */
  static void initExternalHandlers();

  /*!
   * \brief Pausa el envio de mensajes
   */
  static void pause();

  /*!
   * \brief Lanza un mensaje para que aquellos objetos que estén subscritos lo reciban
   * \param[in] msg Mensaje que se lanza
   * \param[in] level
   * \param[in] file 
   * \param[in] line
   * \param[in] function
   */
  static void release(const char *msg, const MessageLevel &level, const char *file = nullptr, int line = -1, const char *function = nullptr);

  /*!
   * \brief Lanza un mensaje para que aquellos objetos que estén subscritos lo reciban
   * \param[in] msg Mensaje que se lanza
   */
  static void release(const Message &msg);

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

  void onDebug(const char *msg, const char *date);
  void onInfo(const char *msg, const char *date);
  void onWarning(const char *msg, const char *date);
  void onError(const char *msg, const char *date);

private:

  static _msgProperties messageProperties(MessageLevel msgLevel);

};

#ifdef _DEBUG
#  define msgDebug(...)    tl::MessageManager::release(tl::MessageManager::Message(__VA_ARGS__).getMessage(), tl::MessageLevel::MSG_DEBUG, __FILE__, __LINE__, TL_FUNCTION);
#  define msgInfo(...)     tl::MessageManager::release(tl::MessageManager::Message(__VA_ARGS__).getMessage(), tl::MessageLevel::MSG_INFO, __FILE__, __LINE__, TL_FUNCTION);
#  define msgWarning(...)  tl::MessageManager::release(tl::MessageManager::Message(__VA_ARGS__).getMessage(), tl::MessageLevel::MSG_WARNING, __FILE__, __LINE__, TL_FUNCTION);
#  define msgError(...)    tl::MessageManager::release(tl::MessageManager::Message(__VA_ARGS__).getMessage(), tl::MessageLevel::MSG_ERROR, __FILE__, __LINE__, TL_FUNCTION);
#else
#  define msgDebug(...)    tl::MessageManager::release(tl::MessageManager::Message(__VA_ARGS__).getMessage(), tl::MessageLevel::MSG_DEBUG);
#  define msgInfo(...)     tl::MessageManager::release(tl::MessageManager::Message(__VA_ARGS__).getMessage(), tl::MessageLevel::MSG_INFO);
#  define msgWarning(...)  tl::MessageManager::release(tl::MessageManager::Message(__VA_ARGS__).getMessage(), tl::MessageLevel::MSG_WARNING);
#  define msgError(...)    tl::MessageManager::release(tl::MessageManager::Message(__VA_ARGS__).getMessage(), tl::MessageLevel::MSG_ERROR);
#endif

#else  // End TL_MESSAGE_HANDLER

// No se utiliza el manejador de mensajes

#  define msgDebug(...)
#  define msgInfo(...)
#  define msgWarning(...)
#  define msgError(...)

#endif

/*! \} */ // end of Messages


/*! \} */ // end of utilities


} // End namespace tl

#endif // TL_CORE_MESSAGES_H
