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

/*!
 * \file Exception.h
 * \brief Custom exception handling with detailed error context
 *
 * This module provides a comprehensive exception system for error handling with detailed
 * information about where errors occur (file, line, function). It supports exception
 * chaining through nested exceptions and provides convenient macros for throwing
 * and asserting exceptions.
 *
 * ### Classes
 *
 * - \ref Exception - Custom exception with file/line/function information
 *
 * ### Functions
 *
 * - \ref makeException - Factory function to create Exception objects
 * - \ref printException - Print exception chain to console
 *
 * ### Macros
 *
 * - \ref TL_ERROR - Create exception with automatic context
 * - \ref TL_THROW_EXCEPTION - Throw exception with automatic context
 * - \ref TL_THROW_EXCEPTION_WITH_NESTED - Throw nested exception
 * - \ref TL_ASSERT - Assert with exception on failure
 *
 * ### Example Usage
 *
 * \code{.cpp}
 * // Simple throw
 * if (value < 0) {
 *     TL_THROW_EXCEPTION("Invalid value: {}", value);
 * }
 * 
 * // With assertion
 * TL_ASSERT(ptr != nullptr, "Pointer cannot be null");
 * 
 * // Catch and print chain
 * try {
 *     // ... code that throws
 * } catch (const std::exception &e) {
 *     printException(e);
 * }
 * \endcode
 *
 * \see Exception, TL_THROW_EXCEPTION, TL_ASSERT
 */

#pragma once

#include "tidop/config.h"

#include <exception>
#include <string>

#include "tidop/core/base/Path.h"
#include "tidop/core/app/Message.h"

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
     * \brief Constructor with error message only.
     *
     * Creates an exception with just an error message, without file, line, 
     * or function context.
     *
     * \param[in] error The error message describing the exception
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
     * \brief Formats the error message with detailed context information.
     *
     * Constructs a formatted error message that includes file, line number, and function
     * information if available. If line is -1, only the error message is used.
     *
     * ### Format
     *
     * With context: `"error_message (filename:line, function_name)"`
     * 
     * Without context: `"error_message"`
     *
     * \internal
     */
    void messagef()
    {
        if (mLine == -1) {
            mMessage = mError;
        } else {
            mMessage = tl::format("{} ({}:{}, {})", mError, mFile, mLine, mFunction);
        }
    }


};


/*!
 * \brief Creates an exception object without throwing.
 *
 * Factory function that creates an Exception with formatted error message
 * and optional context information (file, line, function).
 *
 * ### Example Usage
 *
 * \code{.cpp}
 * auto exc = makeException("File not found: data.txt", __FILE__, __LINE__, __FUNCTION__);
 * // Process exception without throwing
 * \endcode
 *
 * \param[in] error The error message to include in the exception
 * \param[in] file The source file where error occurred (default: empty)
 * \param[in] line The line number where error occurred (default: -1)
 * \param[in] function The function name where error occurred (default: empty)
 * \return Exception object with formatted message
 *
 * \see TL_ERROR, TL_THROW_EXCEPTION
 */
TL_EXPORT Exception makeException(const std::string &error, 
                                  const std::string &file = std::string(), 
                                  int line= -1, 
                                  const std::string &function = std::string());

/*!
 * \brief Prints the complete exception chain to the console.
 *
 * Outputs all exceptions in a nested exception chain with indentation showing
 * the hierarchy. Each exception displays its error message with context information.
 * Particularly useful for debugging exception chains created with TL_THROW_EXCEPTION_WITH_NESTED.
 *
 * ### Example Output
 *
 * \code
 * Exception trace:
 * >>High-level error message (file.cpp:42, functionName)
 *  >>Intermediate error (file.cpp:35, otherFunction)
 *   >>Root cause error (file.cpp:20, rootFunction)
 * \endcode
 *
 * \param[in] e The exception whose chain will be printed
 *
 * ### Notes
 *
 * - Output is sent to Message::error()
 * - Automatically handles exception hierarchy
 * - Safe for any std::exception subclass
 *
 * \see Exception, TL_THROW_EXCEPTION_WITH_NESTED
 */
TL_EXPORT void printException(const std::exception &e);



/*! \} */

} // fin namespace tl


/*! \addtogroup Base
 *  \{
 */

/*!
 * \brief Macro to create an exception with automatic context.
 *
 * Creates an Exception object with automatic file, line, and function information.
 * Does not throw; useful for returning exceptions or delayed throwing.
 *
 * ### Example Usage
 *
 * \code{.cpp}
 * auto exc = TL_ERROR("Failed to load file: {}", filename);
 * // exc contains: "Failed to load file: data.txt (main.cpp:42, loadData)"
 * \endcode
 *
 * \param ... Format string and arguments (as in std::format)
 * \return Exception object with formatted message and context
 *
 * \see TL_THROW_EXCEPTION, TL_ASSERT
 */
#define TL_ERROR(...) tl::makeException(tl::format(__VA_ARGS__), __FILE__, __LINE__, TL_FUNCTION)

/*!
 * \brief Macro to throw an exception with automatic context.
 *
 * Immediately throws an Exception with automatic file, line, and function information.
 * This is the primary way to throw exceptions in TidopLib.
 *
 * ### Example Usage
 *
 * \code{.cpp}
 * if (value < 0) {
 *     TL_THROW_EXCEPTION("Invalid value: {}. Expected positive number.", value);
 * }
 * \endcode
 *
 * \param ... Format string and arguments (as in std::format)
 * \exception Exception Always throws with formatted message and context
 *
 * \see TL_ERROR, TL_ASSERT, TL_THROW_EXCEPTION_WITH_NESTED
 */
#define TL_THROW_EXCEPTION(...) throw tl::makeException(tl::format(__VA_ARGS__), __FILE__, __LINE__, TL_FUNCTION)

/*!
 * \brief Macro to throw a nested exception preserving the exception chain.
 *
 * Throws an exception while preserving any currently active exception,
 * creating an exception chain. Useful for rethrowing with additional context.
 *
 * ### Example Usage
 *
 * \code{.cpp}
 * try {
 *     // ... code that might throw
 * } catch (const std::exception &e) {
 *     TL_THROW_EXCEPTION_WITH_NESTED("Higher-level operation failed");
 *     // Original exception is preserved in the chain
 * }
 * \endcode
 *
 * \param ... Format string and arguments (as in std::format)
 * \exception Exception Always throws with exception chain preserved
 *
 * \see TL_THROW_EXCEPTION, printException
 */
#define TL_THROW_EXCEPTION_WITH_NESTED(...) \
    std::throw_with_nested( \
        tl::makeException(tl::format(__VA_ARGS__), __FILE__, __LINE__, TL_FUNCTION))

/*!
 * \brief Macro to assert a condition and throw on failure.
 *
 * Evaluates an expression and throws an exception if it evaluates to false.
 * Includes the assertion condition in the error message for easier debugging.
 *
 * ### Example Usage
 *
 * \code{.cpp}
 * void processData(Data* ptr) {
 *     TL_ASSERT(ptr != nullptr, "Data pointer must be valid");
 *     TL_ASSERT(ptr->size() > 0, "Data must contain at least one element");
 *     // ... process data
 * }
 * \endcode
 *
 * \param EXPRESSION The boolean expression to evaluate
 * \param ... Optional format string and arguments for additional error context
 * \exception Exception Throws if EXPRESSION evaluates to false
 *
 * \see TL_THROW_EXCEPTION
 */
#define TL_ASSERT(EXPRESSION, ...) if(!(EXPRESSION)) TL_THROW_EXCEPTION( "Assertion '" #EXPRESSION "' failed. " __VA_ARGS__)

/*! \} */
