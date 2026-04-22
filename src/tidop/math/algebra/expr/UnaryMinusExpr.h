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

#include "tidop/math/base/Traits.h"
#include "tidop/math/base/Concepts.h"

namespace tl
{

template<typename Derived>
class MatrixBase;


template<typename Expr>
class UnaryMinusExpr 
  : public MatrixBase<UnaryMinusExpr<Expr>>
{

private:

    const Expr &mExpr;

public:

    explicit UnaryMinusExpr(const Expr &expr)
      : mExpr(expr) 
    {}

    constexpr auto rows() const noexcept -> size_t { return mExpr.rows(); }
    constexpr auto cols() const noexcept -> size_t { return mExpr.cols(); }

    constexpr auto operator()(size_t r, size_t c) const 
    {
        return -mExpr(r, c);
    }

    constexpr auto operator()(size_t i) const
    {
        return -mExpr(i);
    }

#ifdef TL_HAVE_SIMD_INTRINSICS
    auto packet(size_t i) const 
    {
        return -mExpr.packet(i);
    }
#endif

    auto aliases(const void *ptr) const -> bool
    {
        return mExpr.aliases(ptr);
    }

};

} // End namespace tl