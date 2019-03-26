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

#ifndef TL_CORE_LOG_H
#define TL_CORE_LOG_H


#include "config_tl.h"
#include "tidop/core/defs.h"

#include <iostream>
#include <memory>
#include <list>
#include <mutex>

#include "tidop/core/utils.h"
#include "tidop/core/messages.h"
#include "tidop/core/flags.h"

namespace tl
{

/*! \addtogroup utilities
 *  \{
 */


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
class TL_EXPORT Log
#ifdef TL_MESSAGE_HANDLER
  : public MessageManager::Listener
#endif
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
   * Por defecto msg_error
   * \see MessageLevel
   */
  static EnumFlags<MessageLevel> sLevel;

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
  Log();

public:

  /*!
   * \brief Destructora
   */
  ~Log() override;

  /* Se impide la copia y la asignación al ser un singleton */
  Log(Log const&) = delete;
  void operator=(Log const&) = delete;

  /*!
   * \brief Singleton para obtener una referencia única
   */
  static Log &getInstance();

#ifdef TL_ENABLE_DEPRECATED_METHODS
  /*!
   * \brief Niveles de log activados
   * \return Flag con los niveles de mensajes aceptados por el log
   * \see EnumFlags
   * \deprecated Use 'logLevel' en su lugar
   */
  TL_DEPRECATED("logLevel", "2.0")
  EnumFlags<MessageLevel> getLogLevel() const;
#endif // TL_ENABLE_DEPRECATED_METHODS

  /*!
   * \brief Niveles de log activados
   * \return Flag con los niveles de mensajes aceptados por el log
   * \see EnumFlags
   */
  EnumFlags<MessageLevel> logLevel() const;

  /*!
   * \brief Establece el nombre del fichero log
   * \param[in] file fichero log
   */
  void setLogFile(const char* file);

  /*!
   * \brief Establece el nivel de log
   *
   * Se pueden combinar varios niveles de log:
   *
   * \code
   * Log log;
   * log.setLogLevel(MessageLevel::MSG_WARNING | MessageLevel::MSG_ERROR);
   * \endcode
   *
   * \param[in] level Niveles de log.
   */
  void setLogLevel(MessageLevel level);

  /*!
   * \brief Escribe una linea en el log
   * \param[in] msg Mensaje que se escribe en el log
   */
  void write(const char *msg);

protected:

#ifdef TL_MESSAGE_HANDLER

  /*!
   * \brief Mensaje de depuración
   * \param msg Mensaje que se escribe en el log
   * \param date Fecha y hora del mensaje
   */
  void onMsgDebug(const char *msg, const char *date) override;

  /*!
   * \brief Mensaje de información
   * \param msg Mensaje que se escribe en el log
   * \param date Fecha y hora del mensaje
   */
  void onMsgInfo(const char *msg, const char *date) override;

  /*!
   * \brief Mensaje de advertencia
   * \param msg Mensaje que se escribe en el log
   * \param date Fecha y hora del mensaje
   */
  void onMsgWarning(const char *msg, const char *date) override;

  /*!
   * \brief Mensaje de error
   * \param msg Mensaje que se escribe en el log
   * \param date Fecha y hora del mensaje
   */
  void onMsgError(const char *msg, const char *date) override;

  /*!
   * \brief Escribe una linea en el log
   * \param msg Mensaje que se escribe en el log
   * \param date Fecha y hora del mensaje
   */
  void _write(const char *msg, const char *date);

#endif // TL_MESSAGE_HANDLER
};

/*! \} */ // end of Log

/*! \} */ // end of utilities

} // End namespace tl

#endif // TL_CORE_LOG_H
