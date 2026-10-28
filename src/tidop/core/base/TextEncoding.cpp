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
#include <limits>
#include <stdexcept>

namespace tl
{

auto fromLocalEncoding(const std::string& local) -> std::wstring
{
#ifdef TL_OS_WINDOWS
    return fromLocal8Bit(local);
#else
    // Assume local encoding is UTF-8
    return fromUtf8(local);
#endif
}

auto toUtf8(const std::wstring& wstr) -> std::string
{
#ifdef TL_OS_WINDOWS
    if (wstr.empty()) return {};
    if (wstr.size() > static_cast<size_t>(std::numeric_limits<int>::max())) {
        throw std::length_error("Wide string is too large to convert to UTF-8");
    }

    int size_needed = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, wstr.data(),
                                          static_cast<int>(wstr.size()), nullptr, 0, nullptr, nullptr);
    if (size_needed == 0) {
        throw std::range_error("Invalid wide string");
    }

    std::string utf8(size_needed, '\0');
    int converted = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, wstr.data(),
                                        static_cast<int>(wstr.size()), &utf8[0], size_needed, nullptr, nullptr);
    if (converted != size_needed) {
        throw std::range_error("Failed to convert wide string to UTF-8");
    }
    return utf8;
#else
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(wstr);
#endif
}

auto fromUtf8(const std::string& utf8str) -> std::wstring
{
#ifdef TL_OS_WINDOWS
    if (utf8str.empty()) return {};
    if (utf8str.size() > static_cast<size_t>(std::numeric_limits<int>::max())) {
        throw std::length_error("UTF-8 string is too large to convert");
    }

    int size_needed = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, utf8str.data(),
                                          static_cast<int>(utf8str.size()), nullptr, 0);
    if (size_needed == 0) {
        throw std::range_error("Invalid UTF-8 string");
    }

    std::wstring wide(size_needed, L'\0');
    int converted = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, utf8str.data(),
                                        static_cast<int>(utf8str.size()), &wide[0], size_needed);
    if (converted != size_needed) {
        throw std::range_error("Failed to convert UTF-8 string");
    }
    return wide;
#else
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(utf8str);
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
