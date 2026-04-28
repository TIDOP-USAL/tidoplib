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
#include "tidop/math/algebra/functors/Add.h"
#include "tidop/math/algebra/functors/Sub.h"

namespace tl
{

template<typename Derived>
class MatrixBase;


template<typename LHS, typename RHS, typename Op>
class MatBinaryExpr
  : public MatrixBase<MatBinaryExpr<LHS, RHS, Op>>
{

private:

    const LHS &mLhs;
    const RHS &mRhs;

public:

    static_assert(std::is_same_v<
        typename matrix_traits<LHS>::value_type,
        typename matrix_traits<RHS>::value_type>,
        "Mixed types not supported");

    using value_type = typename matrix_traits<LHS>::value_type;

public:

    MatBinaryExpr(const LHS &lhs, const RHS &rhs)
      : mLhs(lhs), mRhs(rhs)
    {
        TL_ASSERT(lhs.rows() == rhs.rows() && lhs.cols() == rhs.cols(), "Matrix sizes must match");
    }

    constexpr auto rows() const noexcept -> size_t { return mLhs.rows(); }
    constexpr auto cols() const noexcept -> size_t { return mLhs.cols(); }

    auto lhs() const -> const LHS & { return mLhs; }
    auto rhs() const -> const RHS & { return mRhs; }

    auto aliases(const void *ptr) const -> bool
    {
        return mLhs.aliases(ptr) || mRhs.aliases(ptr);
    }

};


} // End namespace tl