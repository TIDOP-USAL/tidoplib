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

/*! \addtogroup core
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
   * \brief Constructora privada
   */
  Log();

public:

  /*!
   * \brief Destructora
   */
  ~Log() override;

  Log(const Log &) = delete;
  Log(Log &&) = delete;
  void operator=(const Log &) = delete;
  void operator=(Log &&) = delete;

  /*!
   * \brief Singleton para obtener una referencia única
   */
  static Log &instance();

  /*!
   * \brief Niveles de log activados
   * \return Flag con los niveles de mensajes aceptados por el log
   * \see EnumFlags
   */
  static EnumFlags<MessageLevel> logLevel();

  /*!
   * \brief Establece el nivel de log
   *
   * Se pueden combinar varios niveles de log:
   *
   * \code
   * Log log;
   * log.setMessageLevel(MessageLevel::msg_warning | MessageLevel::msg_error);
   * \endcode
   *
   * \param[in] level Niveles de log.
   */
  static void setMessageLevel(MessageLevel level);
#ifdef TL_ENABLE_DEPRECATED_METHODS
  static void setLogLevel(MessageLevel level);
#endif

  /*!
   * \brief Establece el nombre del fichero log
   * \param[in] file fichero log
   */
  void setLogFile(const std::string &file);

  /*!
   * \brief Escribe una linea en el log
   * \param[in] message Mensaje que se escribe en el log
   */
  void write(const std::string &message);

#ifdef TL_MESSAGE_HANDLER

  static void pauseListener();
  static void resumeListener();

protected:

  /*!
   * \brief Mensaje de depuración
   * \param message Mensaje que se escribe en el log
   * \param date Fecha y hora del mensaje
   */
  void onMsgDebug(const std::string &message, 
                  const std::string &date) override;

  /*!
   * \brief Mensaje de información
   * \param message Mensaje que se escribe en el log
   * \param date Fecha y hora del mensaje
   */
  void onMsgInfo(const std::string &message, 
                 const std::string &date) override;

  /*!
   * \brief Mensaje de advertencia
   * \param message Mensaje que se escribe en el log
   * \param date Fecha y hora del mensaje
   */
  void onMsgWarning(const std::string &message, 
                    const std::string &date) override;

  /*!
   * \brief Mensaje de error
   * \param message Mensaje que se escribe en el log
   * \param date Fecha y hora del mensaje
   */
  void onMsgError(const std::string &message, 
                  const std::string &date) override;

  /*!
   * \brief Escribe una linea en el log
   * \param message Mensaje que se escribe en el log
   * \param date Fecha y hora del mensaje
   */
  void _write(const std::string &message, 
              const std::string &date);

#endif // TL_MESSAGE_HANDLER


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

#ifdef TL_MESSAGE_HANDLER
  static bool sPauseListener;
#endif

};


/*! \} */ // end of Log

/*! \} */ // end of core

} // End namespace tl

#endif // TL_CORE_LOG_H
