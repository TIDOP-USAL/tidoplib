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

#pragma once

#include "tidop/config.h"

#include <exception>
#include <string>

#include "tidop/core/defs.h"
#include "tidop/core/path.h"
#include "tidop/core/msg/message.h"

namespace tl
{

/*! \addtogroup core
 *  \{
 */


/*!
 * \brief Exception handling class
 */
class Exception
  : public std::exception
{
private:

    std::string mError;
    std::string mFile;
    int mLine;
    std::string mFunction;
    std::string mMessage;

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
     * \brief Error description
     */
    auto what() const TL_NOEXCEPT -> const char* override
    {
        return mMessage.c_str();
    }

    /*!
     * \brief Source file where the error occurred
     */
    auto file() const -> std::string
    {
        return mFile;
    }

    /*!
     * \brief Name of the function where the error occurred
     */
    auto function() const -> std::string
    {
        return mFunction;
    }

    /*!
     * \brief Line number where the error occurred
     */
    auto line() const -> int
    {
        return mLine;
    }

private:

    void messagef()
    {
        if (mLine == -1) {
            mMessage = mError;
        } else {
            mMessage = Message::format("{} ({}:{}, {})", mError, mFile, mLine, mFunction);
        }
    }


};




TL_EXPORT Exception makeException(const std::string &error, 
                                  const std::string &file = std::string(), 
                                  int line= -1, 
                                  const std::string &function = std::string());


TL_EXPORT void printException(const std::exception &e, int level = 0);



/*! \} */ // end of core

} // fin namespace tl


/*! \addtogroup core
 *  \{
 */

/*!
 * \brief Macro para crear una excepción
 */
#define TL_ERROR(...) tl::makeException(tl::Message::format(__VA_ARGS__), __FILE__, __LINE__, TL_FUNCTION)

/*!
 * \brief Macro para lanzar una excepción
 */
#define TL_THROW_EXCEPTION(...) throw tl::makeException(tl::Message::format(__VA_ARGS__), __FILE__, __LINE__, TL_FUNCTION)

 /*!
  * \brief Macro para lanzar una excepción
  */
#define TL_THROW_EXCEPTION_WITH_NESTED(...) std::throw_with_nested(tl::makeException(tl::Message::format(__VA_ARGS__), __FILE__, __LINE__, TL_FUNCTION))

#define TL_ASSERT(EXPRESSION, ...) if(!(EXPRESSION)) TL_THROW_EXCEPTION( "Assertion '" #EXPRESSION "' failed. " __VA_ARGS__)

/*! \} */ // end of core
