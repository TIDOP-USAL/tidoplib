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
class VectorBase;


template<typename Expr>
class VecUnaryMinusExpr 
  : public VectorBase<VecUnaryMinusExpr<Expr>>
{

private:

    const Expr &mExpr;

public:

    using value_type = typename vector_traits<Expr>::value_type;

public:

    explicit VecUnaryMinusExpr(const Expr &expr)
      : mExpr(expr) 
    {}

    constexpr auto size() const noexcept -> size_t { return mExpr.size(); }

    auto expr() const -> const Expr & { return mExpr; }

    auto aliases(const void *ptr) const -> bool
    {
        return mExpr.aliases(ptr);
    }

};

} // End namespace tl