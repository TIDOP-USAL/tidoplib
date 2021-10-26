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

#include "tidop/core/exception.h"
#include "tidop/core/path.h"

#include <locale>
#include <codecvt>
#include <utility>

//TODO: mirar
//https://en.cppreference.com/w/cpp/error/nested_exception
//https://en.cppreference.com/w/cpp/error/throw_with_nested
//https://stackoverflow.com/questions/37227300/why-doesnt-c-use-stdnested-exception-to-allow-throwing-from-destructor/37227893#37227893
//https://github.com/GPMueller/mwe-cpp-exception
//https://www.boost.org/doc/libs/1_65_1/doc/html/stacktrace/getting_started.html#stacktrace.getting_started.how_to_print_current_call_stack



namespace tl
{


Exception::Exception(std::string error) TL_NOEXCEPT
  : mError(std::move(error)),
    mFile(""),
    mLine(-1),
    mFunction("")
{
}

Exception::Exception(std::string error, 
                     const std::string &file, 
                     int line, 
                     std::string function ) TL_NOEXCEPT
  : mError(std::move(error)),
    mLine(line), 
    mFunction(std::move(function))
{
  mFile = Path(file).fileName();
  messagef();
}

const char *Exception::what() const TL_NOEXCEPT
{
  return mMessage.c_str();
}

std::string tl::Exception::file() const
{
  return mFile;
}

std::string tl::Exception::function() const
{
  return mFunction;
}

int tl::Exception::line() const
{
  return mLine;
}

void Exception::messagef()
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




/* ---------------------------------------------------------------------------------- */




Exception makeException(const std::string &error, 
                         const std::string &file, 
                         int line, 
                         const std::string &function)
{
  return Exception(error, file, line, function);
}


void printException(const std::exception &e, int level)
{
  if (level == 0) {
    std::string err = std::string(level, ' ') + "exception: " + e.what();
    tl::MessageManager::release(err, tl::MessageLevel::msg_error);
    tl::MessageManager::release("Trace:", tl::MessageLevel::msg_error);
  } else {
    std::string err = ">>" + std::string(level, ' ') + "" + e.what();
    tl::MessageManager::release(err, tl::MessageLevel::msg_error);
  }

  try {
    std::rethrow_if_nested(e);
  } catch (const std::exception &e) {
    printException(e, level + 1);
  } catch (...) {
  }
}


/* ---------------------------------------------------------------------------------- */


#ifdef WIN32
std::string formatWindowsErrorMsg(DWORD errorCode)
{
  DWORD flags = FORMAT_MESSAGE_FROM_SYSTEM
    | FORMAT_MESSAGE_IGNORE_INSERTS
    | FORMAT_MESSAGE_MAX_WIDTH_MASK;
  
  TCHAR errorMessage[1024] = TEXT("");

  FormatMessage(flags,
                NULL,
                errorCode,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                errorMessage,
                sizeof(errorMessage)/sizeof(TCHAR),
                NULL);

  std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
  std::string strError = converter.to_bytes(errorMessage);

  return strError;
}
#endif


} // End namespace tl
