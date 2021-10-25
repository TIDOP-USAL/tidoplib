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
 * License along with Foobar. If not, see <http://www.gnu.org/licenses/>. *
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

namespace tl
{

/*!
 * Clase para el manejo de excepciones
 */
class TL_EXPORT Exception
  : public std::exception
{

public:

  explicit Exception(std::string error) TL_NOEXCEPT;
  explicit Exception(std::string error, 
                     const std::string &file, 
                     int line, 
                     std::string function) TL_NOEXCEPT;
  ~Exception() TL_NOEXCEPT override = default;

  /*!
   * \brief Descripción del error
   */
  const char *what() const TL_NOEXCEPT override;
  
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
#define TL_ERROR(...) tl::makeException(tl::MessageManager::Message(__VA_ARGS__).message(), __FILE__, __LINE__, TL_FUNCTION)

/*!
 * \brief Macro para lanzar una excepción
 */
#define TL_THROW_ERROR(...) throw tl::makeException(tl::MessageManager::Message(__VA_ARGS__).message(), __FILE__, __LINE__, TL_FUNCTION)



#define TL_ASSERT(EXPRESSION, MESSAGE) if(!(EXPRESSION)) { TL_THROW_ERROR( "Assertion '" #EXPRESSION "' " MESSAGE); }


#endif // TL_CORE_EXCEPTION_H
