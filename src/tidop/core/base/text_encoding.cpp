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

#include "tidop/core/base/text_encoding.h"

#include <codecvt>

namespace tl
{

auto fromLocalEncoding(const std::string& local) -> std::wstring
{
    int len = MultiByteToWideChar(CP_ACP, 0, local.c_str(), -1, nullptr, 0);
    std::wstring wide(len - 1, L'\0'); // -1 para quitar el '\0' agregado
    MultiByteToWideChar(CP_ACP, 0, local.c_str(), -1, &wide[0], len);
    return wide;
}

auto toUtf8(const std::wstring& wstr) -> std::string
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.to_bytes(wstr);
}

auto fromUtf8(const std::string& utf8str) -> std::wstring
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(utf8str);
}

auto toLocal8Bit(const std::wstring& wstr) -> std::string
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

auto fromLocal8Bit(const std::string& str) -> std::wstring
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

