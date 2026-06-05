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

namespace tl
{

namespace detail
{

//TODO: Si se usa un expr_traits y vector_traits y matrix_traits heredan de él quedaría mas sencillo

template<MatrixExpr Expr>
constexpr decltype(auto) require_physical_memory(const Expr &expr)
{
    if constexpr (matrix_traits<Expr>::is_plain) {
        return expr;
    } else {
        return expr.eval();
    }
}

template<MatrixExpr Expr>
constexpr decltype(auto) require_linear_access(const Expr &expr) 
{
    if constexpr (matrix_traits<Expr>::has_contiguous_memory) {
        return expr;
    } else {
        return expr.eval();
    }
}


template<VectorExpr Expr>
constexpr decltype(auto) require_physical_memory(const Expr &expr)
{
    if constexpr (vector_traits<Expr>::is_plain) {
        return expr;
    } else {
        return expr.eval();
    }
}

template<VectorExpr Expr>
constexpr decltype(auto) require_linear_access(const Expr &expr)
{
    if constexpr (vector_traits<Expr>::has_contiguous_memory) { 
        return expr;
    } else {
        return expr.eval();
    }
}

} // namespace detail

} // namespace tl