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
//#include "tidop/core/app.h"

#include <iostream>
#include <memory>
#include <list>
#include <mutex>
#include <fstream>
#if CPP_VERSION >= 20
#include <format>
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
  ~Log() override;

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
    //friend class App;

private:

    Log2()
    {
    }

public:

    ~Log2()
    {
    }

    TL_DISABLE_COPY(Log2)
    TL_DISABLE_MOVE(Log2)

    static Log2 &instance()
    {
        static Log2 log;
        return log;
    }

    void open(const std::string &file)
    {
        _stream.open(file, std::ofstream::app);
    }

    void close()
    {
        _stream.close();
    }

    bool isOpen() const
    {
        return _stream.is_open();
    }

    Log2 &operator <<(Level level)
    {
        switch(level) {
            case Level::debug:
                _stream << "Debug:   ";
                break;
            case Level::info:
                _stream << "Info:    ";
                break;
            case Level::warning:
                _stream << "Warning: ";
                break;
            case Level::error:
                _stream << "Error:   ";
                break;
        }

        return *this;
    }

    Log2 &operator <<(decltype(std::endl<char, std::char_traits<char>>) _endl)
    {
        _stream << _endl;
        return *this;
    }

    template<typename T>
    Log2 &operator <<(T value)
    {
	    _stream << value;
	    return *this;
    }

    static Log2 &debug()
    {
        auto &log = Log2::instance();
        log << Level::debug;
        return log;
    }

    static Log2 &info()
    {
        auto &log = Log2::instance();
        log << Level::info;
        return log;
    }

    static Log2 &warning()
    {
        auto &log = Log2::instance();
        log << Level::warning;
        return log;
    }

    static Log2 &error()
    {
        auto &log = Log2::instance();
        log << Level::error;
        return log;
    }


#if CPP_VERSION >= 17
    static void debug(std::string_view message)
#else
    static void debug(const std::string &message)
#endif
    {
      Log2::instance() << Level::debug << message << std::endl;
    }

#if CPP_VERSION >= 17
    static void info(std::string_view message)
#else
    static void info(const std::string &message)
#endif
    {
      Log2::instance() << Level::info << message << std::endl;
    }

#if CPP_VERSION >= 17
    static void warning(std::string_view message)
#else
    static void warning(const std::string &message)
#endif
    {
      Log2::instance() << Level::warning << message << std::endl;
    }

#if CPP_VERSION >= 17
    static void error(std::string_view message)
#else
    static void error(const std::string &message)
#endif
    {
      Log2::instance() << Level::error << message << std::endl;
    }

#if CPP_VERSION >= 20

    template<typename... Args>
    static void debug(std::format_string<Args...> s, Args&&... args)
    {
        Log2::instance() << Level::debug << std::vformat(s.get(), std::make_format_args(args...)) << std::endl;
    }

    template<typename... Args>
    static void info(std::format_string<Args...> s, Args&&... args)
    {
        Log2::instance() << Level::info << std::vformat(s.get(), std::make_format_args(args...)) << std::endl;
    }

    template<typename... Args>
    static void warning(std::format_string<Args...> s, Args&&... args)
    {
        Log2::instance() << Level::warning << std::vformat(s.get(), std::make_format_args(args...)) << std::endl;
    }

    template<typename... Args>
    static void error(std::format_string<Args...> s, Args&&... args)
    {
        Log2::instance() << Level::error << std::vformat(s.get(), std::make_format_args(args...)) << std::endl;
    }

#endif

};



/*! \} */ // end of Log

/*! \} */ // end of core

} // End namespace tl

#endif // TL_CORE_LOG_H
