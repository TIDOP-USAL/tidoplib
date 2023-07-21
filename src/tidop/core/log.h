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


#include "tidop/config.h"
#include "tidop/core/defs.h"

#include <iostream>
#include <memory>
#include <list>
#include <mutex>
#include <fstream>
#if CPP_VERSION >= 20
#include <format>
#else
#include <fmt/format.h>
#endif

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
#ifdef TL_MESSAGE_HANDLER
  ~Log() override;
#else
  ~Log();
#endif

  TL_DISABLE_COPY(Log)
  TL_DISABLE_MOVE(Log)

#ifdef TL_ENABLE_DEPRECATED_METHODS

  /*!
   * \brief Singleton para obtener una referencia única
   */
  TL_DEPRECATED("App::log", "3.0")
  static Log &instance();

#endif // TL_ENABLE_DEPRECATED_METHODS

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
#if CPP_VERSION >= 17
  void onMsgDebug(std::string_view message, 
                  std::string_view date) override;
#else
  void onMsgDebug(const std::string &message, 
                  const std::string &date) override;
#endif

  /*!
   * \brief Mensaje de información
   * \param message Mensaje que se escribe en el log
   * \param date Fecha y hora del mensaje
   */
#if CPP_VERSION >= 17
  void onMsgInfo(std::string_view message, 
                 std::string_view date) override;
#else
  void onMsgInfo(const std::string &message, 
                 const std::string &date) override;
#endif

  /*!
   * \brief Mensaje de advertencia
   * \param message Mensaje que se escribe en el log
   * \param date Fecha y hora del mensaje
   */
#if CPP_VERSION >= 17
  void onMsgWarning(std::string_view message, 
                    std::string_view date) override;
#else
  void onMsgWarning(const std::string &message, 
                    const std::string &date) override;
#endif

  /*!
   * \brief Mensaje de error
   * \param message Mensaje que se escribe en el log
   * \param date Fecha y hora del mensaje
   */
#if CPP_VERSION >= 17
  void onMsgError(std::string_view message, 
                  std::string_view date) override;
#else
  void onMsgError(const std::string &message, 
                  const std::string &date) override;
#endif

  /*!
   * \brief Escribe una linea en el log
   * \param message Mensaje que se escribe en el log
   * \param date Fecha y hora del mensaje
   */
#if CPP_VERSION >= 17
  void _write(std::string_view message, 
              std::string_view date);
#else
  void _write(const std::string &message, 
              const std::string &date);
#endif

#endif // TL_MESSAGE_HANDLER


private:

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

  friend class App;

};




class Log2
{

private:

    std::ofstream _stream;
    static std::mutex mtx;
    static EnumFlags<MessageLevel> messageLevelFlags;

private:

    Log2() = default;

public:

    ~Log2() = default;

    TL_DISABLE_COPY(Log2)
    TL_DISABLE_MOVE(Log2)

    static Log2 &instance();

    void open(const std::string &file);
    void close();
    bool isOpen() const;

    Log2 &operator <<(Level level);
    Log2 &operator <<(decltype(std::endl<char, std::char_traits<char>>) _endl);

    template<typename T>
    Log2 &operator <<(T value)
    {
	    _stream << value;
	    return *this;
    }

    /*!
     * \brief Message levels
     * \return Flag with message levels activated
     * \see EnumFlags
     */
    static auto messageLevel() -> EnumFlags<MessageLevel>;

    /*!
     * \brief Sets the message level
     *
     * Several log levels can be combined:
     *
     * \code
     * Log log;
     * log.setMessageLevel(MessageLevel::msg_warning | MessageLevel::msg_error);
     * \endcode
     *
     * \param[in] level Message level.
     */
    static void setMessageLevel(MessageLevel level);

    static Log2 &debug();
    static Log2 &info();
    static Log2 &succes();
    static Log2 &warning();
    static Log2 &error();


#if CPP_VERSION >= 17
    static void debug(std::string_view message);
#else
    static void debug(const std::string &message);
#endif

#if CPP_VERSION >= 17
    static void info(std::string_view message);
#else
    static void info(const std::string &message);
#endif

#if CPP_VERSION >= 17
    static void succes(std::string_view message);
#else
    static void succes(const std::string &message);
#endif

#if CPP_VERSION >= 17
    static void warning(std::string_view message);
#else
    static void warning(const std::string &message);
#endif

#if CPP_VERSION >= 17
    static void error(std::string_view message);
#else
    static void error(const std::string &message);
#endif

#if CPP_VERSION >= 20 || defined(TL_HAVE_FMT)

    template<typename... Args>
    static void debug(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        if (Log2::instance().isOpen())
            Log2::debug(FORMAT_NAMESPACE vformat(s.get(), FORMAT_NAMESPACE make_format_args(args...)));
    }

    template<typename... Args>
    static void info(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        if (Log2::instance().isOpen())
            Log2::info(FORMAT_NAMESPACE vformat(s.get(), FORMAT_NAMESPACE make_format_args(args...)));
    }

    template<typename... Args>
    static void succes(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        if (Log2::instance().isOpen())
            Log2::succes(FORMAT_NAMESPACE vformat(s.get(), FORMAT_NAMESPACE make_format_args(args...)));
    }

    template<typename... Args>
    static void warning(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        if (Log2::instance().isOpen())
            Log2::warning(FORMAT_NAMESPACE vformat(s.get(), FORMAT_NAMESPACE make_format_args(args...)));
    }

    template<typename... Args>
    static void error(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args)
    {
        if (Log2::instance().isOpen())
            Log2::error(FORMAT_NAMESPACE vformat(s.get(), FORMAT_NAMESPACE make_format_args(args...)));
    }

#endif

};



/*! \} */ // end of Log

/*! \} */ // end of core

} // End namespace tl

#endif // TL_CORE_LOG_H
