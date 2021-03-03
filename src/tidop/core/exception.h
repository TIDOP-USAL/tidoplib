/************************************************************************
 *                                                                      *
 * Copyright (C) 2020 by Tidop Research Group                           *
 *                                                                      *
 * This file is part of TidopLib                                        *
 *                                                                      *
 * TidopLib is free software: you can redistribute it and/or modify     *
 * it under the terms of the GNU General Public License as published by *
 * the Free Software Foundation, either version 3 of the License, or    *
 * (at your option) any later version.                                  *
 *                                                                      *
 * TidopLib is distributed in the hope that it will be useful,          *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.      *
 *                                                                      *
 * @license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>                *
 *                                                                      *
 ************************************************************************/

#ifndef TL_CORE_EXCEPTION_H
#define TL_CORE_EXCEPTION_H

#include "config_tl.h"

#include <exception>
#include <string>

#include "tidop/core/defs.h"
#include "tidop/core/messages.h"

namespace tl
{

/*!
 * Clase para el manejo de excepciones
 */
class TL_EXPORT Exception
  : public std::exception
{

public:

  explicit Exception(const std::string &error);
  explicit Exception(const std::string &error, 
                     const std::string &file, 
                     int line, 
                     const std::string &function);
  virtual ~Exception() TL_NOEXCEPT override {}

  /*!
   * \brief Descripción del error
   */
  virtual const char *what() const TL_NOEXCEPT override;
  
  /*!
   * \brief Fichero fuente donde se ha producido el error
   */
  std::string file() const;

  /*!
   * \brief Nombre de la función donde se ha producido el error
   */
  std::string function() const;

  /*!
   * \brief Número de línea donde se ha producido el error
   */
  int line() const;

private:

  void messagef();

protected:

  std::string mError;
  std::string mFile;
  int mLine;
  std::string mFunction;
  std::string mMessage;
};




TL_EXPORT Exception make_exception(const std::string &error, 
                                   const std::string &file = std::string(), 
                                   int line= -1, 
                                   const std::string &function = std::string());



#ifdef WIN32

/*!
 * \brief formatWindowsErrorMsg
 */
TL_EXPORT std::string formatWindowsErrorMsg(DWORD errorCode);

#endif


} // fin namespace tl


/*!
 * \brief Macro para crear una excepción
 */
#define TL_ERROR(...) tl::make_exception(tl::MessageManager::Message(__VA_ARGS__).message(), __FILE__, __LINE__, TL_FUNCTION)

/*!
 * \brief Macro para lanzar una excepción
 */
#define TL_THROW_ERROR(...) throw tl::make_exception(tl::MessageManager::Message(__VA_ARGS__).message(), __FILE__, __LINE__, TL_FUNCTION)



#define TL_ASSERT(EXPRESSION, MESSAGE) if(!(EXPRESSION)) { TL_THROW_ERROR( "Assertion '" #EXPRESSION "' " MESSAGE); }


#endif // TL_CORE_EXCEPTION_H
