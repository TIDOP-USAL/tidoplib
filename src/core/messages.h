/* 
  Hay que añadir la posibilidad de añadir un handler personalizado para 
  facilitar la inclusión de la librería en otros sistemas.
*/

#ifndef I3D_MESSAGES_H
#define I3D_MESSAGES_H

#include <iostream>
#include <memory>
#include <list>
#include <mutex>

#include "core/defs.h"

#include "core/utils.h"
#include "core/flags.h"

namespace I3D
{

#ifdef I3D_MESSAGE_HANDLER

/*!
 * \brief Niveles de información de los mensajes
 */
enum class MessageLevel : int8_t {
  MSG_DEBUG,      /*!< Información extra para depuración. */
  MSG_VERBOSE,    /*!< Todos los mensajes. */
  MSG_INFO,       /*!< Warnings, errores y otra información. */
  MSG_WARNING,    /*!< Warnings y errores. */
  MSG_ERROR       /*!< Sólo errores. */
};

enum class MessageOutput : int8_t {
  MSG_CONSOLE = 0x01,
  MSG_LOG = 0x02
};

//Se permiten operaciones a nivel de bit para el enum MessageOutput
ALLOW_BITWISE_FLAG_OPERATIONS(MessageOutput);

/*!
 * \brief Message
 * \deprecated{ Usar I3D::MessageManager en combinación con I3D::Log y I3D::Console}
 */
class I3D_EXPORT Message
{
private:
  static std::unique_ptr<Message> sObjMessage;

  /*!
   * \brief logfile
   */
  static std::string sLogFile;

  /*!
   * \brief Nivel de información de los mensajes
   *
   * Por defecto MSG_ERROR
   * \see MessageLevel
   */
  static MessageLevel sLevel;

  /*!
   * \brief Tipo de salida de los mensajes
   *
   * Por defecto la salida es por consola (MSG_CONSOLE). Se puede cambiar la salida a 
   * un fichero Log (MSG_LOG) o  combinar ambas ( MSG_CONSOLE | MSG_LOG )
   * \see MessageOutput
   */
  static EnumFlags<MessageOutput> sOutput;

  /*!
   * \brief sLastMessage
   */
  static std::string sLastMessage;

  /*!
   * \brief Plantilla para el formateo de fecha y hora de los mensajes del log.
   *
   * Por defecto la plantilla es:
   * \code
   * std::string Message::timeLogTemplate = "%d/%b/%Y %H:%M:%S";
   * \endcode
   * \see setTimeLogFormat
   */
  static std::string sTimeLogFormat;

  static std::mutex mtx;

private:

  /*!
   * \brief Constructora Message
   */
  Message();

public:

  /*!
   * Destructora
   */
  ~Message() {}

  Message(Message const&) = delete;
  
  void operator=(Message const&) = delete;

  /*!
   * \brief Singleton para obtener una referencia única
   */
  static Message &get();

  /*!
   * \brief Devuelve la cadena de texto con el mensaje
   * \return Mensaje
   */
  const char *getMessage() const;

  /*!
   * \brief message
   * \param[in] msg
   * \return
   */
  I3D_DEPRECATED("MessageManager::release()")
  static Message &message(const char *msg, ...);

  /*!
   * \brief Imprime un mensaje con las opciones establecidas
   */
  static void print();

  /*!
   * \brief Imprime un mensaje
   * \param[in] level Nivel del mensaje
   * \param[in] output Tipo de salida
   * \see MessageLevel, MessageOutput
   */
  static void print(const MessageLevel &level, const MessageOutput &output);

  /*!
   * \brief Imprime un mensaje
   * \param[in] level Nivel del mensaje
   * \param[in] output Tipo de salida
   * \param[in] file Nombre del fichero
   * \param[in] line Número de línea
   * \param[in] function Nombre de función
   */
  static void print(const MessageLevel &level, /*EnumFlags<MessageOutput> flag*/ const MessageOutput &output, const char *file, int line, const char *function);
 
  /*!
   * \brief Establece el nivel de información de los mensajes
   * \param[in] level Nombre del fichero log.
   * \see MessageLevel
   */
  static void setMessageLevel(const MessageLevel &level);

  /*!
   * \brief Establece la salida del mensaje
   * \param[in] output Salida del mensaje
   * \see MessageOutput
   */
  static void setMessageOutput( const MessageOutput &output );

  /*!
   * \brief Establece la salida del mensaje
   * \param[in] output Salida del mensaje
   * \see MessageOutput
   */
  static void setMessageLogFile( const std::string logfile );

  /*!
   * \brief setTimeLogFormat
   * \code
   * Message::setTimeLogFormat( "%d - %b - %Y (%H:%M:%S)" );
   * \endcode
   * \param[in] timeTemplate
   */
  static void setTimeLogFormat( const std::string timeTemplate);

  /*!
   * \brief Nivel de información de los mensajes
   * \return Nivel de información de los mensajes
   * \see MessageLevel
   */
  MessageLevel getMessageLevel() const;

  /*!
   * \brief Nivel de información de los mensajes
   * \return Nivel de información de los mensajes
   * \see MessageOutput
   */
  EnumFlags<MessageOutput> getMessageOutput() const;

  /*!
   * \brief Inicializa los manejadores para las librerias externas
   */
  static void initExternalHandlers();

private:

  static std::string messageOutput(const MessageLevel &msgLevel);

  static std::string messageOutput(const MessageLevel &msgLevel, const char *file, int line, const char *function);

  static void _print(const MessageLevel &level, const MessageOutput &output, const std::string &msgOut);
};

#ifdef _DEBUG
#  define consolePrintDebug(...)    Message::message(__VA_ARGS__).print( MessageLevel::MSG_DEBUG, MessageOutput::MSG_CONSOLE, __FILE__, __LINE__, I3D_FUNCTION);
#  define consolePrintVerbose(...)  Message::message(__VA_ARGS__).print( MessageLevel::MSG_VERBOSE, MessageOutput::MSG_CONSOLE, __FILE__, __LINE__, I3D_FUNCTION);
#  define consolePrintInfo(...)     Message::message(__VA_ARGS__).print( MessageLevel::MSG_INFO, MessageOutput::MSG_CONSOLE, __FILE__, __LINE__, I3D_FUNCTION);
#  define consolePrintWarning(...)  Message::message(__VA_ARGS__).print( MessageLevel::MSG_WARNING, MessageOutput::MSG_CONSOLE, __FILE__, __LINE__, I3D_FUNCTION);
#  define consolePrintError(...)    Message::message(__VA_ARGS__).print( MessageLevel::MSG_ERROR, MessageOutput::MSG_CONSOLE, __FILE__, __LINE__, I3D_FUNCTION);

#  define logPrintDebug(...)    Message::message(__VA_ARGS__).print( MessageLevel::MSG_DEBUG, MessageOutput::MSG_LOG, __FILE__, __LINE__, I3D_FUNCTION);
#  define logPrintVerbose(...)  Message::message(__VA_ARGS__).print( MessageLevel::MSG_VERBOSE, MessageOutput::MSG_LOG, __FILE__, __LINE__, I3D_FUNCTION);
#  define logPrintInfo(...)     Message::message(__VA_ARGS__).print( MessageLevel::MSG_INFO, MessageOutput::MSG_LOG, __FILE__, __LINE__, I3D_FUNCTION);
#  define logPrintWarning(...)  Message::message(__VA_ARGS__).print( MessageLevel::MSG_WARNING, MessageOutput::MSG_LOG, __FILE__, __LINE__, I3D_FUNCTION);
#  define logPrintError(...)    Message::message(__VA_ARGS__).print( MessageLevel::MSG_ERROR, MessageOutput::MSG_LOG, __FILE__, __LINE__, I3D_FUNCTION);

#  define printDebug(...)    Message::message(__VA_ARGS__).print( MessageLevel::MSG_DEBUG, MessageOutput::MSG_LOG | MessageOutput::MSG_CONSOLE, __FILE__, __LINE__, I3D_FUNCTION);
#  define printVerbose(...)  Message::message(__VA_ARGS__).print( MessageLevel::MSG_VERBOSE, MessageOutput::MSG_LOG | MessageOutput::MSG_CONSOLE, __FILE__, __LINE__, I3D_FUNCTION);
#  define printInfo(...)     Message::message(__VA_ARGS__).print( MessageLevel::MSG_INFO, MessageOutput::MSG_LOG | MessageOutput::MSG_CONSOLE, __FILE__, __LINE__, I3D_FUNCTION);
#  define printWarning(...)  Message::message(__VA_ARGS__).print( MessageLevel::MSG_WARNING, MessageOutput::MSG_LOG | MessageOutput::MSG_CONSOLE, __FILE__, __LINE__, I3D_FUNCTION);
#  define printError(...)    Message::message(__VA_ARGS__).print( MessageLevel::MSG_ERROR, MessageOutput::MSG_LOG | MessageOutput::MSG_CONSOLE, __FILE__, __LINE__, I3D_FUNCTION);

#else
#  define consolePrintDebug(...)
#  define consolePrintVerbose(...)  Message::message(__VA_ARGS__).print( MessageLevel::MSG_VERBOSE, MessageOutput::MSG_CONSOLE);
#  define consolePrintInfo(...)     Message::message(__VA_ARGS__).print( MessageLevel::MSG_INFO, MessageOutput::MSG_CONSOLE);
#  define consolePrintWarning(...)  Message::message(__VA_ARGS__).print( MessageLevel::MSG_WARNING, MessageOutput::MSG_CONSOLE);
#  define consolePrintError(...)    Message::message(__VA_ARGS__).print( MessageLevel::MSG_ERROR, MessageOutput::MSG_CONSOLE);

#  define logPrintDebug(...)
#  define logPrintVerbose(...)  Message::message(__VA_ARGS__).print( MessageLevel::MSG_VERBOSE, MessageOutput::MSG_LOG);
#  define logPrintInfo(...)     Message::message(__VA_ARGS__).print( MessageLevel::MSG_INFO, MessageOutput::MSG_LOG);
#  define logPrintWarning(...)  Message::message(__VA_ARGS__).print( MessageLevel::MSG_WARNING, MessageOutput::MSG_LOG);
#  define logPrintError(...)    Message::message(__VA_ARGS__).print( MessageLevel::MSG_ERROR, MessageOutput::MSG_LOG);

#  define printDebug(...)    Message::message(__VA_ARGS__).print( MessageLevel::MSG_DEBUG, MessageOutput::MSG_LOG | MessageOutput::MSG_CONSOLE);
#  define printVerbose(...)  Message::message(__VA_ARGS__).print( MessageLevel::MSG_VERBOSE, MessageOutput::MSG_LOG | MessageOutput::MSG_CONSOLE);
#  define printInfo(...)     Message::message(__VA_ARGS__).print( MessageLevel::MSG_INFO, MessageOutput::MSG_LOG | MessageOutput::MSG_CONSOLE);
#  define printWarning(...)  Message::message(__VA_ARGS__).print( MessageLevel::MSG_WARNING, MessageOutput::MSG_LOG | MessageOutput::MSG_CONSOLE);
#  define printError(...)    Message::message(__VA_ARGS__).print( MessageLevel::MSG_ERROR, MessageOutput::MSG_LOG | MessageOutput::MSG_CONSOLE);

#endif



/*! \addtogroup utilities
 *  \{
 */

/*! \defgroup Messages Gestión de mensajes
 *
 *  \{
 */


/*!
 * \brief Clase para gestionar los mensajes de la librería
 *
 * Proporciona las herramientas necesarias para la impresión de mensajes por
 * consola, en un fichero log y la comunicación con otras librerias o aplicaciones
 */
class MessageManager
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
  class Listener
  {
  protected:

    //std::shared_ptr<MessageManager> mMessage;

  public:

    /*!
     * \brief Constructora
     */
    Listener()
    {
    }

    /*!
     * \brief destructora
     */
    ~Listener()
    {
    }

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
    virtual void onMsgVerbose(const char *msg, const char *date) = 0;

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
  class Message
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
     * \brief Devuelve el mensaje como cadena de texto
     * \return Mensaje
     */
    const char *getMessage() const;

    /*!
     * \brief Devuelve la fecha y hora del mensaje
     * \return Fecha y hora del mensaje
     */
    const char *getDate() const;

    /*!
     * \brief Nivel del mensaje
     * \return Devuelve el nivel de mensaje establecido
     */
    MessageLevel getLevel() const;

    /*!
     * \brief getFile
     * \return
     */
    std::string getFile() const;

    /*!
     * \brief getLine
     * \return
     */
    int getLine() const;

    /*!
     * \brief getFunction
     * \return
     */
    std::string getFunction() const;

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

  /*!
   * \brief Nivel de información de los mensajes
   *
   * Por defecto MSG_ERROR
   * \see MessageLevel
   */
  //static MessageLevel sLevel;

  static bool sStopHandler;

private:

  /*!
   * \brief Constructora privada MessageManager
   */
  MessageManager();

public:


  ~MessageManager();

  /*!
   * \brief Añade un escuchador de mensajes
   * \param[in] listener Objeto escuchador
   */
  void addListener(Listener *listener);

  MessageManager(MessageManager const&) = delete;
  void operator=(MessageManager const&) = delete;

  /*!
   * \brief Singleton para obtener una referencia única
   */
  static MessageManager &getInstance();

  /*!
   * \brief Lanza un mensaje para que aquellos objetos que estén subscritos lo reciban
   * \param[in] msg Mensaje que se lanza
   * \param[in] level
   * \param[in] file 
   * \param[in] line
   * \param[in] function
   */
  static void release(const char *msg, const MessageLevel &level, const char *file = NULL, int line = -1, const char *function = NULL);

  /*!
   * \brief Lanza un mensaje para que aquellos objetos que estén subscritos lo reciban
   * \param[in] msg Mensaje que se lanza
   */
  static void release(const Message &msg);

  /*!
   * \brief Inicializa los manejadores para las librerias externas
   */
  static void initExternalHandlers();

  /*!
   * \brief Pausa el envio de mensajes
   */
  static void pause();

  /*!
   * \brief Reanuda el envio de mensajes
   */
  static void resume();

protected:

  void onDebug(const char *msg, const char *date);
  void onVerbose(const char *msg, const char *date);
  void onInfo(const char *msg, const char *date);
  void onWarning(const char *msg, const char *date);
  void onError(const char *msg, const char *date);

};

#ifdef _DEBUG
#  define msgDebug(...)    MessageManager::release(MessageManager::Message(__VA_ARGS__).getMessage(), MessageLevel::MSG_DEBUG, __FILE__, __LINE__, I3D_FUNCTION);
#  define msgVerbose(...)  MessageManager::release(MessageManager::Message(__VA_ARGS__).getMessage(), MessageLevel::MSG_VERBOSE, __FILE__, __LINE__, I3D_FUNCTION);
#  define msgInfo(...)     MessageManager::release(MessageManager::Message(__VA_ARGS__).getMessage(), MessageLevel::MSG_INFO, __FILE__, __LINE__, I3D_FUNCTION);
#  define msgWarning(...)  MessageManager::release(MessageManager::Message(__VA_ARGS__).getMessage(), MessageLevel::MSG_WARNING, __FILE__, __LINE__, I3D_FUNCTION);
#  define msgError(...)    MessageManager::release(MessageManager::Message(__VA_ARGS__).getMessage(), MessageLevel::MSG_ERROR, __FILE__, __LINE__, I3D_FUNCTION);
#else
#  define msgDebug(...)    MessageManager::release(MessageManager::Message(__VA_ARGS__).getMessage(), MessageLevel::MSG_DEBUG);
#  define msgVerbose(...)  MessageManager::release(MessageManager::Message(__VA_ARGS__).getMessage(), MessageLevel::MSG_VERBOSE);
#  define msgInfo(...)     MessageManager::release(MessageManager::Message(__VA_ARGS__).getMessage(), MessageLevel::MSG_INFO);
#  define msgWarning(...)  MessageManager::release(MessageManager::Message(__VA_ARGS__).getMessage(), MessageLevel::MSG_WARNING);
#  define msgError(...)    MessageManager::release(MessageManager::Message(__VA_ARGS__).getMessage(), MessageLevel::MSG_ERROR);
#endif

/*! \} */ // end of Messages

/* ---------------------------------------------------------------------------------- */

/*! \defgroup Log Fichero log
 *
 *  \{
 */


/*!
 * \brief Clase para gestionar ficheros log
 *
 * Esta clase puede funcionar individualmente o si se subscribe a
 * al gestor de mensajes (MessageManager) recibe automaticamente
 * los mensajes
 */
class Log : public MessageManager::Listener
{

private:

  /*!
   * \brief sObjMessage
   */
  static std::unique_ptr<Log> sObjLog;

  /*!
   * \brief Fichero log
   */
  static std::string sLogFile;

  /*!
   * \brief Nivel de información de los mensajes
   *
   * Por defecto MSG_ERROR
   * \see MessageLevel
   */
  static MessageLevel sLevel;

  //std::string mMessage;

  /*!
   * \brief Plantilla para el formateo de fecha y hora de los mensajes del log.
   *
   * Por defecto la plantilla es:
   * \code
   * std::string MessageManager::timeLogTemplate = "%d/%b/%Y %H:%M:%S";
   * \endcode
   * \see setTimeLogFormat
   */
  static std::string sTimeLogFormat;

  static std::mutex mtx;

private:

  /*!
   * \brief Constructora privada
   */
  Log() {}

public:

  /*!
   * \brief Destructora
   */
  ~Log() {}

  Log(Log const&) = delete;
  void operator=(Log const&) = delete;

  /*!
   * \brief Singleton para obtener una referencia única
   */
  static Log &getInstance();

  void setLogFile(const char* file);

  void setLogLevel(MessageLevel level);

  /*!
   * \brief Escribe una linea en el log
   * \param msg Mensaje que se escribe en el log
   * \param date Fecha y hora del mensaje
   */
  void write(const char *msg);

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

  /*!
   * \brief Escribe una linea en el log
   * \param msg Mensaje que se escribe en el log
   * \param date Fecha y hora del mensaje
   */
  void _write(const char *msg, const char *date);

  //static std::string messageOutput(const MessageLevel &msgLevel, const char *file, int line, const char *function);

};

/*! \} */ // end of Log

/*! \} */ // end of utilities

#else  // End I3D_MESSAGE_HANDLER

// No se utiliza el manejador de mensajes
#  define consolePrintDebug(...)
#  define consolePrintVerbose(...)
#  define consolePrintInfo(...)
#  define consolePrintWarning(...)
#  define consolePrintError(...)

#  define logPrintDebug(...)
#  define logPrintVerbose(...)
#  define logPrintInfo(...)
#  define logPrintWarning(...)
#  define logPrintError(...)

#  define printDebug(...)
#  define printVerbose(...)
#  define printInfo(...)
#  define printWarning(...)
#  define printError(...)

#  define msgDebug(...)
#  define msgVerbose(...)
#  define msgInfo(...)
#  define msgWarning(...)
#  define msgError(...)

#endif


} // End namespace I3D

#endif // I3D_MESSAGES_H
