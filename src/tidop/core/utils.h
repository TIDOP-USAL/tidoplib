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

#include <vector>
#include <string>
#include <numeric>
#include <sstream>
#include <string>

#include "tidop/core/defs.h"
#include "tidop/core/exception.h"
#include "tidop/core/common.h"

namespace tl
{

/*!
 * \addtogroup core
 *
 * \{
 */

#if CPP_VERSION >= 17
TL_EXPORT bool compareInsensitiveCase(std::string_view source,
                                      std::string_view compare);
#else
TL_EXPORT bool compareInsensitiveCase(const std::string &source,
                                      const std::string &compare);
#endif

/*!
 * \brief Performs a cast from one numeric type to another, rounding if necessary.
 * \tparam T1 The type to cast to.
 * \tparam T2 The type of the number to cast.
 * \param[in] number The number to be cast.
 * \return The number cast to type T1, rounded if necessary.
 */
template<typename T1, typename T2>
auto numberCast(T2 number) -> enableIfIntegral<T1, T1>
{
    return static_cast<T1>(std::round(number));
}

template<typename T1, typename T2>
auto numberCast(T2 number) -> enableIfFloating<T1, T1>
{
    return static_cast<T1>(number);
}

template<typename T1, typename T2>
auto numberCast(T2 /*b*/) -> enableIfNotArithmetic<T1, T1>
{
    //En linux me sale siempre el error aunque no se llame a la función.
    //TL_COMPILER_WARNING("Invalid conversion. It isn't an arithmetic type.")
    throw Exception("Invalid conversion. It isn't an arithmetic type.", __FILE__, __LINE__, TL_FUNCTION);
    return T1{0};
}


/*!
 * \brief Checks if a value is within a specified range.
 * \param[in] value The value to be checked.
 * \param[in] min The minimum value of the range.
 * \param[in] max The maximum value of the range.
 * \return True if the value is within the range defined by min and max, false otherwise.
 */
template <typename T>
auto isInRange(T value, T min, T max) -> bool
{
    return (value < max) && (value > min);
}


template <typename T>
auto convertStringTo(const std::string &str) -> std::enable_if_t<std::is_arithmetic<T>::value && !std::is_same<T, bool>::value,T>
{
    T value{};

    std::istringstream ss(str);
    ss >> value;

    return value;
} 

template <typename T>
auto convertStringTo(const std::string &str) -> enableIfBool<T,T>
{
    T value = (compareInsensitiveCase(str, "true") || str == "1");

    return value;
}

template <typename T>
auto convertStringTo(const std::string &/*str*/) -> enableIfNotArithmetic<T,T>
{
    //En linux me sale siempre el error aunque no se llame a la función.
    //TL_COMPILER_WARNING("Invalid conversion. It isn't an arithmetic type.")
    throw Exception("Invalid conversion. It isn't an arithmetic type.", __FILE__, __LINE__, TL_FUNCTION);
    return T{0};
}


/* ---------------------------------------------------------------------------------- */
/*                             Operaciones con cadenas                                */
/* ---------------------------------------------------------------------------------- */

/*! \defgroup stringOper Operaciones con cadenas
 *  
 * \{
 */


template <typename T>
auto split(const std::string& string,
           char separator = ',') -> std::enable_if_t<std::is_same<T, std::string>::value, std::vector<T>>
{
    std::vector<T> out;

    std::stringstream ss(string);
    std::string  item{};
    while (std::getline(ss, item, separator)) {
        out.push_back(item);
    }

    return out;
}

template <typename T>
auto split(const std::string& string,
           char separator = ',') -> std::enable_if_t<std::is_arithmetic<T>::value && 
                                                     !std::is_same<T, bool>::value,
	                                                 std::vector<T>>
{
    std::vector<T> out;

    std::stringstream ss(string);
    std::string item{};
    while (std::getline(ss, item, separator)) {
        out.push_back(convertStringTo<T>(item));
    }

    return out;
}


/// https://stackoverflow.com/a/217605

inline void ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
        }));
}

// trim from end (in place)
inline void rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
        }).base(), s.end());
}

// trim from both ends (in place)
inline void trim(std::string &s)
{
    rtrim(s);
    ltrim(s);
}

// trim from start (copying)
inline std::string ltrim_copy(std::string s)
{
    ltrim(s);
    return s;
}

// trim from end (copying)
inline std::string rtrim_copy(std::string s)
{
    rtrim(s);
    return s;
}

// trim from both ends (copying)
inline std::string trim_copy(std::string s)
{
    trim(s);
    return s;
}

/*!
 * \brief Reemplaza una cadena por otra en un texto.
 * \param[in] str Cadena original
 * \param[in] str_old Cadena a remplazar
 * \param[out] str_new Nueva cadena
 *
 * <h4>Ejemplo</h4>
 * \code
 * std::string str = "Hola mundo";
 * replaceString(str, " ", "_");
 * \endcode
 */
TL_EXPORT void replaceString(std::string *str, const std::string &str_old, const std::string &str_new);


/*! \} */ // end of stringOper

#ifdef TL_ENABLE_DEPRECATED_METHODS
/*!
 * \brief Convierte un número a una cadena de texto
 * \param[in] number Numero
 * \return Cadena de texto
 */
template <typename T> inline
TL_DEPRECATED("std::to_string()", "2.0")
std::string numberToString(T number)
{
    std::ostringstream ss;
    ss << number;
    return ss.str();
}
#endif // TL_ENABLE_DEPRECATED_METHODS


/*!
 * \brief Convierte una cadena de texto a un número
 * \param[in] text Texto
 * \return número
 */
template <typename T> inline 
T stringToNumber(const std::string &text)
{
    T number{};
    return (std::istringstream(text) >> number) ? number : 0;
}

enum class Base : int8_t
{
    octal       =  8,
    decimal     = 10,
    hexadecimal = 16
};

/*!
 * \brief Convierte una cadena a un número entero.
 *
 * La cadena puede tener un número en base octal, decimal o hexadecimal
 * \param text Cadena de texto
 * \param base Base en la cual esta el número
 * \return Número
 * \see Base
 */
TL_EXPORT int stringToInteger(const std::string &text, Base base = Base::decimal);


/*!
 * \brief Ordena los indices de un vector de menor a mayor
 * Para un vector [10,20,15,5] devuelve [3,0,2,1]. El elemento mas
 * pequeño esta en la posición 3, el segundo en la posición 0, ...
 * \param[in] v Vector
 * \return Vector con los indices ordenados
 */
template <typename T> inline 
std::vector<int> sortIdx(const std::vector<T> &v)
{
    std::vector<int> idx(v.size());
    std::iota(idx.begin(), idx.end(), 0);
    
    sort(idx.begin(), idx.end(), [&v](int i1, int i2) {return v[i1] < v[i2]; });
    
    return idx;
}

#ifdef TL_OS_WINDOWS

auto stringToWString(const std::string &string) -> std::wstring;

auto wstringToString(const std::wstring &wideString) -> std::string;

#endif // TL_OS_WINDOWS

/*! \} */ // end of core


} // End namespace tl

