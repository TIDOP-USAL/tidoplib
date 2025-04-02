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

#include "tidop/core/base/defs.h"
#include "tidop/core/base/path.h"
#include "tidop/core/app/message.h"

namespace tl
{

/*! \addtogroup Base
 *  \{
 */


 /*!
  * \class Exception
  * \brief Custom exception class for handling errors with detailed information.
  *
  * This class extends `std::exception` to provide custom error messages along with
  * additional context like the file, line number, and function where the error occurred.
  * It is useful for debugging and tracing errors in complex systems.
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

    /*!
     * \brief Constructor that initializes the exception with an error message.
     *
     * This constructor is used when the error message is available, but
     * no file, line, or function context is required.
     *
     * \param[in] error The error message describing the exception.
     */
    explicit Exception(std::string error) TL_NOEXCEPT
      : mError(std::move(error)),
        mFile(""),
        mLine(-1),
        mFunction("")
    {
    }

    /*!
     * \brief Constructor that initializes the exception with detailed error information.
     *
     * This constructor captures the error message along with the file, line,
     * and function where the error occurred, and formats the error message.
     *
     * \param[in] error The error message describing the exception.
     * \param[in] file The source file where the error occurred.
     * \param[in] line The line number where the error occurred.
     * \param[in] function The function where the error occurred.
     */
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
     * \brief Returns a description of the error.
     *
     * This method overrides `std::exception::what()` to provide the custom error message.
     *
     * \return A pointer to the error message.
     */
    auto what() const TL_NOEXCEPT -> const char* override
    {
        return mMessage.c_str();
    }

    /*!
     * \brief Returns the name of the file where the error occurred.
     *
     * This method returns the file name (without the path) where the error occurred.
     *
     * \return The name of the file where the error occurred.
     */
    auto file() const TL_NOEXCEPT -> std::string
    {
        return mFile;
    }

    /*!
     * \brief Returns the name of the function where the error occurred.
     *
     * This method returns the name of the function where the error occurred.
     *
     * \return The name of the function where the error occurred.
     */
    auto function() const TL_NOEXCEPT -> std::string
    {
        return mFunction;
    }

    /*!
     * \brief Returns the line number where the error occurred.
     *
     * This method returns the line number in the code where the error occurred.
     *
     * \return The line number where the error occurred.
     */
    auto line() const TL_NOEXCEPT -> int
    {
        return mLine;
    }

private:

    /*!
     * \brief Formats the error message with detailed information.
     *
     * This method formats the error message to include the file, line, and function
     * details, if available. If not, it just uses the error message.
     */
    void messagef()
    {
        if (mLine == -1) {
            mMessage = mError;
        } else {
            mMessage = format("{} ({}:{}, {})", mError, mFile, mLine, mFunction);
        }
    }


};

//class OutOfRangeException 
//  : public Exception
//{
//
//public:
//
//    explicit OutOfRangeException(const std::string &message)
//        : Exception(message)
//    {
//    }
//};

/*!
 * \brief Creates an exception object
 *
 * This function creates an `Exception` object with a formatted error message
 * and additional information such as the source file, line, and function where it occurred.
 *
 * \param[in] error The error message to include in the exception.
 * \param[in] file The source file where the error occurred (optional).
 * \param[in] line The line number where the error occurred (optional).
 * \param[in] function The function name where the error occurred (optional).
 *
 * \return The created `Exception` object.
 */
TL_EXPORT Exception makeException(const std::string &error, 
                                  const std::string &file = std::string(), 
                                  int line= -1, 
                                  const std::string &function = std::string());

/*!
 * \brief Prints the exception trace
 *
 * This function prints the error message of an exception and recursively
 * prints any nested exceptions, providing a trace of the exception chain.
 *
 * \param[e] e The exception object to print.
 */
TL_EXPORT void printException(const std::exception &e);



/*! \} */

} // fin namespace tl


/*! \addtogroup Base
 *  \{
 */

 /*!
  * \brief Macro to create an exception
  *
  * This macro formats the error message and creates an `Exception` object,
  * including the file, line, and function where it was called.
  */
#define TL_ERROR(...) tl::makeException(tl::format(__VA_ARGS__), __FILE__, __LINE__, TL_FUNCTION)

/*!
 * \brief Macro to throw an exception
 *
 * This macro creates an `Exception` object using the formatted error message 
 * and throws it, including the file, line, and function where it was called.
 */
#define TL_THROW_EXCEPTION(...) throw tl::makeException(tl::format(__VA_ARGS__), __FILE__, __LINE__, TL_FUNCTION)

/*!
 * \brief Macro to throw a nested exception
 *
 * This macro creates an `Exception` object and throws it with nested exception 
 * information, including the file, line, and function where it was called.
 */
#define TL_THROW_EXCEPTION_WITH_NESTED(...) \
    std::throw_with_nested( \
        tl::makeException(tl::format(__VA_ARGS__), __FILE__, __LINE__, TL_FUNCTION))

/*!
 * \brief Macro to assert an expression and throw an exception if false
 *
 * This macro checks an expression and if the expression evaluates to false, 
 * it throws an exception with a message that includes the assertion failure details.
 * 
 * \param EXPRESSION The expression to assert.
 * \param ... Optional additional error message parameters.
 */
#define TL_ASSERT(EXPRESSION, ...) if(!(EXPRESSION)) TL_THROW_EXCEPTION( "Assertion '" #EXPRESSION "' failed. " __VA_ARGS__)

/*! \} */
