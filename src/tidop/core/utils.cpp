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

#include "tidop/core/utils.h"

#include "tidop/core/exception.h"

#ifdef TL_HAVE_BOOST
#include <boost/algorithm/string.hpp>
#elif  CPP_VERSION >= 14
#include <algorithm>
#endif

#include <codecvt>


namespace tl
{

/* ---------------------------------------------------------------------------------- */
/*                             Operaciones con cadenas                                */
/* ---------------------------------------------------------------------------------- */


void replaceString(std::string *str, const std::string &str_old, const std::string &str_new)
{
    std::size_t ini = str->find(str_old);
    while (ini != std::string::npos) {
        str->replace(ini, str_old.size(), str_new);
        ini = str->find(str_old, str_new.size() + ini);
    }
}

int stringToInteger(const std::string &text, Base base)
{
    std::istringstream ss(text);
    switch (base) {
    case Base::octal:
        ss.setf(std::ios_base::oct, std::ios::basefield);
        break;
    case Base::decimal:
        ss.setf(std::ios_base::dec, std::ios::basefield);
        break;
    case Base::hexadecimal:
        ss.setf(std::ios_base::hex, std::ios::basefield);
        break;
    }
    int number;
    return ss >> number ? number : 0;
}

#if CPP_VERSION >= 17
bool compareInsensitiveCase(std::string_view source, std::string_view compare)
#else
bool compareInsensitiveCase(const std::string &source, const std::string &compare)
#endif
{
#ifdef TL_HAVE_BOOST
    return boost::iequals(source, compare);
#elif CPP_VERSION >= 14
    //https://stackoverflow.com/questions/11635/case-insensitive-string-comparison-in-c
    return std::equal(source.begin(), source.end(),
                      compare.begin(), compare.end(),
                      [](char a, char b) {
                          return tolower(a) == tolower(b);
                      });
#else

    unsigned int sz = source.size();
    if (compare.size() != sz)
        return false;
    for (unsigned int i = 0; i < sz; ++i)
        if (tolower(source[i]) != tolower(compare[i]))
            return false;
    return true;
#endif
}

#ifdef TL_OS_WINDOWS

auto stringToWString(const std::string &string) -> std::wstring
{
    if (string.empty()) return L"";

    auto size = MultiByteToWideChar(CP_UTF8, 0, string.data(), static_cast<int>(string.size()), nullptr, 0);
    TL_ASSERT(size > 0, "MultiByteToWideChar() failed: {}", size);

    std::wstring wide_string(size, 0);

    MultiByteToWideChar(CP_UTF8, 0, string.data(), static_cast<int>(string.size()), &wide_string[0], size);

    return wide_string;
}

auto wstringToString(const std::wstring &wideString) -> std::string
{
    if (wideString.empty()) return "";

    const auto size = WideCharToMultiByte(CP_UTF8, 0, wideString.data(), static_cast<int>(wideString.size()), nullptr, 0, nullptr, nullptr);
    TL_ASSERT(size > 0, "WideCharToMultiByte() failed: {}", size);

    std::string _string(size, 0);
    WideCharToMultiByte(CP_UTF8, 0, wideString.data(), static_cast<int>(wideString.size()), &_string[0], size, nullptr, nullptr);

    return _string;
}
#endif // TL_OS_WINDOWS

std::wstring fromLocalEncoding(const std::string& local)
{
    int len = MultiByteToWideChar(CP_ACP, 0, local.c_str(), -1, nullptr, 0);
    std::wstring wide(len - 1, L'\0'); // -1 para quitar el '\0' agregado
    MultiByteToWideChar(CP_ACP, 0, local.c_str(), -1, &wide[0], len);
    return wide;
}

std::string toUtf8(const std::wstring &wstr)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.to_bytes(wstr);
}

std::wstring fromUtf8(const std::string &utf8str)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(utf8str);
}

std::string toLocal8Bit(const std::wstring &wstr)
{
#ifdef _WIN32
    if (wstr.empty()) return {};

    int size_needed = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string result(size_needed, '\0');
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, &result[0], size_needed, nullptr, nullptr);
    if (!result.empty() && result.back() == '\0') result.pop_back();
    return result;
#else
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.to_bytes(wstr);
#endif
}

std::wstring fromLocal8Bit(const std::string &str)
{
#ifdef _WIN32
    if (str.empty()) return {};

    int size_needed = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
    if (size_needed <= 0) return {};

    std::wstring wstr(size_needed, 0);
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, &wstr[0], size_needed);
    if (!wstr.empty() && wstr.back() == L'\0') wstr.pop_back();
    return wstr;
#else
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(str);
#endif
}


} // End namespace tl

