/* 
  Hay que añadir la posibilidad de añadir un handler personalizado para 
  facilitar la inclusión de la librería en otros sistemas.
*/

#ifndef I3D_MESSAGES_H
#define I3D_MESSAGES_H

#include <iostream>
#include <memory>
#include <mutex>

#include "utils.h"
#include "core\defs.h"
#include "core\config.h"
#include "core\flags.h"

namespace I3D
{

#ifdef I3D_MESSAGE_HANDLER

/*!
 * \brief Niveles de información de los mensajes
 */
enum class MessageLevel : int8_t {
  MSG_DEBUG,      /**< Información extra para depuración. */
  MSG_VERBOSE,    /**< Información extra para depuración. */
  MSG_INFO,       /**< Warnings, errores y otra información. */
  MSG_WARNING,    /**< Warnings y errores. */
  MSG_ERROR       /**< Sólo errores. */
};

enum class MessageOutput : int8_t {
  MSG_CONSOLE = 0x01,
  MSG_LOG = 0x02
};

//Se permiten operaciones a nivel de bit para el enum MessageOutput
ALLOW_BITWISE_FLAG_OPERATIONS(MessageOutput);

/*!
 * \brief Message
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
   * \brief message
   * \param[in] msg
   * \return
   */
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

private:

  static std::string messageOutput(const MessageLevel &msgLevel);

  static std::string messageOutput(const MessageLevel &msgLevel, const char *file, int line, const char *function);

  static void _print(const MessageLevel &level, const MessageOutput &output, const std::string &msgOut);
};

#ifdef _DEBUG
#  define consolePrintDebug( msg, ...)    Message::message(msg, __VA_ARGS__).print( MessageLevel::MSG_DEBUG, MessageOutput::MSG_CONSOLE, __FILE__, __LINE__, I3D_FUNCTION);
#  define consolePrintVerbose( msg, ...)  Message::message(msg, __VA_ARGS__).print( MessageLevel::MSG_VERBOSE, MessageOutput::MSG_CONSOLE, __FILE__, __LINE__, I3D_FUNCTION);
#  define consolePrintInfo( msg, ...)     Message::message(msg, __VA_ARGS__).print( MessageLevel::MSG_INFO, MessageOutput::MSG_CONSOLE, __FILE__, __LINE__, I3D_FUNCTION);
#  define consolePrintWarning( msg, ...)  Message::message(msg, __VA_ARGS__).print( MessageLevel::MSG_WARNING, MessageOutput::MSG_CONSOLE, __FILE__, __LINE__, I3D_FUNCTION);
#  define consolePrintError( msg, ...)    Message::message(msg, __VA_ARGS__).print( MessageLevel::MSG_ERROR, MessageOutput::MSG_CONSOLE, __FILE__, __LINE__, I3D_FUNCTION);

#  define logPrintDebug( msg, ...)    Message::message(msg, __VA_ARGS__).print( MessageLevel::MSG_DEBUG, MessageOutput::MSG_LOG, __FILE__, __LINE__, I3D_FUNCTION);
#  define logPrintVerbose( msg, ...)  Message::message(msg, __VA_ARGS__).print( MessageLevel::MSG_VERBOSE, MessageOutput::MSG_LOG, __FILE__, __LINE__, I3D_FUNCTION);
#  define logPrintInfo( msg, ...)     Message::message(msg, __VA_ARGS__).print( MessageLevel::MSG_INFO, MessageOutput::MSG_LOG, __FILE__, __LINE__, I3D_FUNCTION);
#  define logPrintWarning( msg, ...)  Message::message(msg, __VA_ARGS__).print( MessageLevel::MSG_WARNING, MessageOutput::MSG_LOG, __FILE__, __LINE__, I3D_FUNCTION);
#  define logPrintError( msg, ...)    Message::message(msg, __VA_ARGS__).print( MessageLevel::MSG_ERROR, MessageOutput::MSG_LOG, __FILE__, __LINE__, I3D_FUNCTION);

#  define printDebug( msg, ...)    Message::message(msg, __VA_ARGS__).print( MessageLevel::MSG_DEBUG, MessageOutput::MSG_LOG | MessageOutput::MSG_CONSOLE, __FILE__, __LINE__, I3D_FUNCTION);
#  define printVerbose( msg, ...)  Message::message(msg, __VA_ARGS__).print( MessageLevel::MSG_VERBOSE, MessageOutput::MSG_LOG | MessageOutput::MSG_CONSOLE, __FILE__, __LINE__, I3D_FUNCTION);
#  define printInfo( msg, ...)     Message::message(msg, __VA_ARGS__).print( MessageLevel::MSG_INFO, MessageOutput::MSG_LOG | MessageOutput::MSG_CONSOLE, __FILE__, __LINE__, I3D_FUNCTION);
#  define printWarning( msg, ...)  Message::message(msg, __VA_ARGS__).print( MessageLevel::MSG_WARNING, MessageOutput::MSG_LOG | MessageOutput::MSG_CONSOLE, __FILE__, __LINE__, I3D_FUNCTION);
#  define printError( msg, ...)    Message::message(msg, __VA_ARGS__).print( MessageLevel::MSG_ERROR, MessageOutput::MSG_LOG | MessageOutput::MSG_CONSOLE, __FILE__, __LINE__, I3D_FUNCTION);

#else
#  define consolePrintDebug( msg, ...)
#  define consolePrintVerbose( msg, ...)  Message::message(msg, __VA_ARGS__).print( MessageLevel::MSG_VERBOSE, MessageOutput::MSG_CONSOLE);
#  define consolePrintInfo( msg, ...)     Message::message(msg, __VA_ARGS__).print( MessageLevel::MSG_INFO, MessageOutput::MSG_CONSOLE);
#  define consolePrintWarning( msg, ...)  Message::message(msg, __VA_ARGS__).print( MessageLevel::MSG_WARNING, MessageOutput::MSG_CONSOLE);
#  define consolePrintError( msg, ...)    Message::message(msg, __VA_ARGS__).print( MessageLevel::MSG_ERROR, MessageOutput::MSG_CONSOLE);

#  define logPrintDebug( msg, ...)
#  define logPrintVerbose( msg, ...)  Message::message(msg, __VA_ARGS__).print( MessageLevel::MSG_VERBOSE, MessageOutput::MSG_LOG);
#  define logPrintInfo( msg, ...)     Message::message(msg, __VA_ARGS__).print( MessageLevel::MSG_INFO, MessageOutput::MSG_LOG);
#  define logPrintWarning( msg, ...)  Message::message(msg, __VA_ARGS__).print( MessageLevel::MSG_WARNING, MessageOutput::MSG_LOG);
#  define logPrintError( msg, ...)    Message::message(msg, __VA_ARGS__).print( MessageLevel::MSG_ERROR, MessageOutput::MSG_LOG);

#  define printDebug( msg, ...)    Message::message(msg, __VA_ARGS__).print( MessageLevel::MSG_DEBUG, MessageOutput::MSG_LOG | MessageOutput::MSG_CONSOLE);
#  define printVerbose( msg, ...)  Message::message(msg, __VA_ARGS__).print( MessageLevel::MSG_VERBOSE, MessageOutput::MSG_LOG | MessageOutput::MSG_CONSOLE);
#  define printInfo( msg, ...)     Message::message(msg, __VA_ARGS__).print( MessageLevel::MSG_INFO, MessageOutput::MSG_LOG | MessageOutput::MSG_CONSOLE);
#  define printWarning( msg, ...)  Message::message(msg, __VA_ARGS__).print( MessageLevel::MSG_WARNING, MessageOutput::MSG_LOG | MessageOutput::MSG_CONSOLE);
#  define printError( msg, ...)    Message::message(msg, __VA_ARGS__).print( MessageLevel::MSG_ERROR, MessageOutput::MSG_LOG | MessageOutput::MSG_CONSOLE);

#endif


#else  // End I3D_MESSAGE_HANDLER

// No se utiliza el manejador de mensajes
#  define consolePrintDebug( msg, ...)
#  define consolePrintVerbose( msg, ...)
#  define consolePrintInfo( msg, ...)
#  define consolePrintWarning( msg, ...)
#  define consolePrintError( msg, ...)

#  define logPrintDebug( msg, ...)
#  define logPrintVerbose( msg, ...)
#  define logPrintInfo( msg, ...)
#  define logPrintWarning( msg, ...)
#  define logPrintError( msg, ...) 

#  define printDebug( msg, ...)
#  define printVerbose( msg, ...)
#  define printInfo( msg, ...)
#  define printWarning( msg, ...)
#  define printError( msg, ...)

#endif  


} // End namespace I3D

#endif // I3D_MESSAGES_H
