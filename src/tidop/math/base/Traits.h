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

namespace tl
{

template<typename T, size_t _size> class Vector;
namespace internal
{
template<typename T, size_t _size> class MatrixRow;
template<typename T, size_t _size> class MatrixCol;
}

template<typename D>
struct VectorTraits;

template<typename T, size_t _size>
struct VectorTraits<Vector<T, _size>>
{
    using value_type = T;
    static constexpr size_t size = _size;
    using result_type = Vector<T, _size>;
};

template<typename T, size_t _size>
struct VectorTraits<internal::MatrixRow<T, _size>>
{
    using value_type = T;
    static constexpr size_t size = _size;
    using result_type = Vector<T, _size>;
};

template<typename T, size_t _size>
struct VectorTraits<internal::MatrixCol<T, _size>> 
{
    using value_type = T;
    static constexpr size_t size = _size;
    using result_type = Vector<T, _size>;
};



template<typename D>
struct is_vector : std::false_type {};

template<typename T, size_t _size>
struct is_vector<Vector<T, _size>> : std::true_type {};

template<typename T, size_t _size>
struct is_vector<internal::MatrixRow<T, _size>> : std::true_type {};

template<typename T, size_t _size>
struct is_vector<internal::MatrixCol<T, _size>> : std::true_type {};

template<typename D>
using enable_if_vector_t = std::enable_if_t<is_vector<D>::value, int>;


template<typename T>
struct is_point : std::false_type {};

template<typename T, size_t _size>
struct is_point<Vector<T, _size>> : std::false_type {};

template<typename T, size_t _size>
struct is_point<internal::MatrixRow<T, _size>> : std::false_type {};

template<typename T, size_t _size>
struct is_point<internal::MatrixCol<T, _size>> : std::false_type {};

template<typename D>
using enable_if_point_t = std::enable_if_t<is_point<D>::value, int>;

}