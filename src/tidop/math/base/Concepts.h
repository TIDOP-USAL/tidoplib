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

#include <concepts>
#include <cstddef>

#include "tidop/math/base/Traits.h"
#include "tidop/core/base/Concepts.h"

namespace tl
{

template<typename T>
concept LinearExpr = requires(const T & a) 
{
    typename T::value_type;
};

template<typename T>
concept MatrixExpr = LinearExpr<T> && 
                     requires(const T &a, size_t i, size_t j) 
{
    { a.rows() } noexcept -> std::same_as<size_t>;
    { a.cols() } noexcept -> std::same_as<size_t>;
    { a(i, j) } -> std::convertible_to<typename T::value_type>;
};

template<typename T>
concept VectorExpr = LinearExpr<T> && 
                     requires(const T & a, size_t i) 
{
    { a.size() } noexcept -> std::same_as<size_t>;
    { a[i] } -> std::convertible_to<typename T::value_type>;
    typename T::is_vector_expr_tag;
};

template<typename T>
concept DenseMatrix = MatrixExpr<T> && requires(T a)
{
    { a.data() };
};

template<typename T>
concept StaticMatrix = MatrixExpr<T> && (T::rows_at_compile_time != DynamicData);

template<typename T>
concept DynamicMatrix = MatrixExpr<T> && (T::rows_at_compile_time == DynamicData);

template<typename T>
concept StaticVector = VectorExpr<T> && (T::size_at_compile_time != DynamicData);

template<typename T>
concept DynamicVector = VectorExpr<T> && (T::size_at_compile_time == DynamicData);


} // namespace tl