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

#ifndef TL_CORE_EXCEPTION_H
#define TL_CORE_EXCEPTION_H

#include "config_tl.h"

#include <exception>
#include <string>

#include "tidop/core/defs.h"
#include "tidop/core/messages.h"
#include "tidop/core/path.h"

namespace tl
{

/*! \addtogroup core
 *  \{
 */


/*!
 * Clase para el manejo de excepciones
 */
class Exception
  : public std::exception
{

public:

  explicit Exception(std::string error) TL_NOEXCEPT
    : mError(std::move(error)),
      mFile(""),
      mLine(-1),
      mFunction("")
  {
  }

  explicit Exception(std::string error, 
                     const std::string &file, 
                     int line, 
                     std::string function) TL_NOEXCEPT
    : mError(std::move(error)),
      mLine(line),
      mFunction(std::move(function))
  {
    mFile = Path(file).fileName().toString();
    messagef();
  }

  ~Exception() TL_NOEXCEPT override = default;

  /*!
   * \brief Descripción del error
   */
  const char *what() const TL_NOEXCEPT override
  {
    return mMessage.c_str();
  }

  /*!
   * \brief Fichero fuente donde se ha producido el error
   */
  std::string file() const
  {
    return mFile;
  }

  /*!
   * \brief Nombre de la función donde se ha producido el error
   */
  std::string function() const
  {
    return mFunction;
  }

  /*!
   * \brief Número de línea donde se ha producido el error
   */
  int line() const
  {
    return mLine;
  }

private:

  void messagef()
  {
    char buf[1000];
    if (mLine == -1) {
      mMessage = mError;
    } else {
#if defined _MSC_VER
      sprintf_s(buf, 1000, "%s (%s:%u, %s)", mError.c_str(), mFile.c_str(), mLine, mFunction.c_str());
#else
      snprintf(buf, 1000, "%s (%s:%u, %s)", mError.c_str(), mFile.c_str(), mLine, mFunction.c_str());
#endif
      mMessage = std::string(buf);
    }
  }

private:

  std::string mError;
  std::string mFile;
  int mLine;
  std::string mFunction;
  std::string mMessage;
};




TL_EXPORT Exception makeException(const std::string &error, 
                                  const std::string &file = std::string(), 
                                  int line= -1, 
                                  const std::string &function = std::string());


TL_EXPORT void printException(const std::exception &e, int level = 0);



//#ifdef WIN32
//
///*!
// * \brief formatWindowsErrorMsg
// */
//TL_EXPORT std::string formatWindowsErrorMsg(DWORD errorCode);
//
//#endif

/*! \} */ // end of core

} // fin namespace tl


/*! \addtogroup core
 *  \{
 */

/*!
 * \brief Macro para crear una excepción
 */
#define TL_ERROR(...) tl::makeException(tl::MessageManager::Message(__VA_ARGS__).message(), __FILE__, __LINE__, TL_FUNCTION)

/*!
 * \brief Macro para lanzar una excepción
 */
#define TL_THROW_EXCEPTION(...) throw tl::makeException(tl::MessageManager::Message(__VA_ARGS__).message(), __FILE__, __LINE__, TL_FUNCTION)

 /*!
  * \brief Macro para lanzar una excepción
  */
#define TL_THROW_EXCEPTION_WITH_NESTED(...) std::throw_with_nested(tl::makeException(tl::MessageManager::Message(__VA_ARGS__).message(), __FILE__, __LINE__, TL_FUNCTION))

#define TL_ASSERT(EXPRESSION, ...) if(!(EXPRESSION)) TL_THROW_EXCEPTION( "Assertion '" #EXPRESSION "' failed. " __VA_ARGS__)

/*! \} */ // end of core

#endif // TL_CORE_EXCEPTION_H
