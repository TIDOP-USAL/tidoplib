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

#include "tidop/core/base/TextEncoding.h"

#include <codecvt>

namespace tl
{

auto fromLocalEncoding(const std::string& local) -> std::wstring
{
#ifdef TL_OS_WINDOWS
    int len = MultiByteToWideChar(CP_ACP, 0, local.c_str(), -1, nullptr, 0);
    std::wstring wide(len - 1, L'\0'); // -1 para quitar el '\0' agregado
    MultiByteToWideChar(CP_ACP, 0, local.c_str(), -1, &wide[0], len);
    return wide;
#else
    // Assume local encoding is UTF-8
    return fromUtf8(local);
#endif
}

auto toUtf8(const std::wstring& wstr) -> std::string
{
#if TL_CPP_VERSION< 20
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.to_bytes(wstr);
#else
    std::string str;
    mbstate_t state = {};
    const wchar_t *src = wstr.data();
    size_t len = std::wcsrtombs(nullptr, &src, 0, &state);
    if (len == static_cast<size_t>(-1)) return {};
    str.resize(len);
    std::wcsrtombs(&str[0], &src, len, &state);
    return str;
#endif
}

auto fromUtf8(const std::string& utf8str) -> std::wstring
{
#if TL_CPP_VERSION< 20
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(utf8str);
#else
    std::wstring wstr;
    mbstate_t state = {};
    const char *src = utf8str.data();
    size_t len = std::mbsrtowcs(nullptr, &src, 0, &state);
    if (len == static_cast<size_t>(-1)) return {};
    wstr.resize(len);
    std::mbsrtowcs(&wstr[0], &src, len, &state);
    return wstr;
#endif
}

auto toLocal8Bit(const std::wstring& wstr) -> std::string
{
#ifdef TL_OS_WINDOWS
    if (wstr.empty()) return {};

    int size_needed = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string result(size_needed, '\0');
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, &result[0], size_needed, nullptr, nullptr);
    if (!result.empty() && result.back() == '\0') result.pop_back();
    return result;
#else
    return toUtf8(wstr);
#endif
}

auto fromLocal8Bit(const std::string& str) -> std::wstring
{
#ifdef TL_OS_WINDOWS
    if (str.empty()) return {};

    int size_needed = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
    if (size_needed <= 0) return {};

    std::wstring wstr(size_needed, 0);
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, &wstr[0], size_needed);
    if (!wstr.empty() && wstr.back() == L'\0') wstr.pop_back();
    return wstr;
#else
    return fromUtf8(str);
#endif
}

} // End namespace tl

