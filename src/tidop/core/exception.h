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
