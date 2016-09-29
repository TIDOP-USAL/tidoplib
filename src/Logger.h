 #ifndef I3D_LOGGER_H
#define I3D_LOGGER_H

#include <iostream>

#include "core\defs.h"

namespace I3D
{

/*!
 * \brief Niveles de log
 */
enum class LogLevel {
  LOG_DEBUG,      /**< Información extra para depuración. */
  LOG_INFO,       /**< Warnings, errores y otra información. */
  LOG_WARNING,    /**< Warnings y errores. */
  LOG_ERROR       /**< Sólo errores. */
};

//void MessageOutput(LogLevel ll, const char *msg, const char *file, int line, const char *function);

/*!
 * \brief Clase para gestión de ficheros log
 */
class I3D_EXPORT LogMsg
{
private:
  /*!
   * \brief logfile
   */
  static std::string logFile;
  
public:

  /*!
   * \brief Nivel de log
   *
   * Por defecto LOG_ERROR
   * \see LogLevel
   */
  static LogLevel logLevel;

  /*!
   * \brief Constructora LogMsg
   */
  LogMsg();

  /*!
   * \brief Constructora LogMsg
   * \param[in] filename Nombre del fichero log.
   * \param[in] ll Nivel de log
   * \see LogLevel
   */
  LogMsg(const char *filename, LogLevel ll = LogLevel::LOG_ERROR);

  /*!
   * Destructora
   */
  ~LogMsg();

  /*!
  * \brief Escribe una linea en el log
  * \param[in] logline Texto que se va a escribir en el log.
  * \param[in] type Nivel de log
  * \see LogLevel
  */
  static void write(const char *logline, LogLevel type = LogLevel::LOG_ERROR);

  /*!
   * \brief Inicializa el log
   */
  static void init();

  /*!
   * \brief Mensaje de depuración
   * \param[in] msg Mensaje
   */
  static void debugMsg(const char *msg, ... );
  
  /*!
   * \brief DebugMsg
   * \param[in] file
   * \param[in] line
   * \param[in] function
   * \param[in] msg
   */
  static void debugMsg(const char *file, int line, const char *function, char *msg, ...);

  static void debugMsg(const char *msg, const char *file, int line, const char *function);

  /*!
   * \brief Mensaje de información
   * \param[in] msg
   */
  static void infoMsg(const char *msg, ... );

  /*!
   * \brief InfoMsg
   * \param[in] file
   * \param[in] line
   * \param[in] function
   * \param[in] msg
   */
  static void infoMsg(const char *file, int line, const char *function, char *msg, ...);

  static void infoMsg(const char *msg, const char *file, int line, const char *function);

  /*!
   * \brief Mensaje de advertencia
   * \param[in] msg
   */
  static void warningMsg(const char *msg, ... );

  /*!
   * \brief WarningMsg
   * \param[in] file
   * \param[in] line
   * \param[in] function
   * \param[in] msg
   */
  static void warningMsg(const char *file, int line, const char *function, char *msg, ...);

  static void warningMsg(const char *msg, const char *file, int line, const char *function);

  /*!
   * \brief Mensaje de error
   * \param[in] msg
   */
  static void errorMsg(const char *msg, ... );
  
  /*!
   * \brief ErrorMsg
   * \param[in] file
   * \param[in] line
   * \param[in] function
   * \param[in] msg
   */
  static void errorMsg(const char *file, int line, const char *function, char *msg, ...);

  static void errorMsg(const char *msg, const char *file, int line, const char *function);
};

char *_msg(const char *msg, ...);

#ifdef _DEBUG
#  define logDebugMsg( msg, ... )    LogMsg::debugMsg( _msg(msg, __VA_ARGS__), __FILE__, __LINE__, I3D_FUNCTION )
#  define logInfoMsg( msg, ... )     LogMsg::infoMsg( _msg(msg, __VA_ARGS__), __FILE__, __LINE__, I3D_FUNCTION )
#  define logWarningMsg( msg, ... )  LogMsg::warningMsg( _msg(msg, __VA_ARGS__), __FILE__, __LINE__, I3D_FUNCTION )
#  define logErrorMsg( msg, ... )    LogMsg::errorMsg( _msg(msg, __VA_ARGS__), __FILE__, __LINE__, I3D_FUNCTION ) 
#else
#  define logDebugMsg( msg, ... )    LogMsg::debugMsg( _msg(msg, __VA_ARGS__) )
#  define logInfoMsg( msg, ... )     LogMsg::infoMsg( _msg(msg, __VA_ARGS__) )
#  define logWarningMsg( msg, ... )  LogMsg::warningMsg( _msg(msg, __VA_ARGS__) )
#  define logErrorMsg( msg, ... )    LogMsg::errorMsg( _msg(msg, __VA_ARGS__) )
#endif

} // End namespace I3D

#endif // I3D_LOGGER_H
