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


template<typename LHS, typename Scalar>
class VecMulScalarExpr
  : public VectorBase<VecMulScalarExpr<LHS, Scalar>>
{

private:

    const LHS &mLhs;
    Scalar mScalar;

public:

    static_assert(std::is_same_v<typename vector_traits<LHS>::value_type,
                  Scalar>, "Mixed types not supported");

    using value_type = Scalar;

public:

    VecMulScalarExpr(const LHS&lhs, Scalar scalar)
      : mLhs(lhs), 
        mScalar(scalar)
    {}

    constexpr auto size() const noexcept -> size_t { return mLhs.size(); }

    auto operator[](size_t i) const -> value_type
    {
        return mLhs[i] * mScalar;
    }

#ifdef TL_HAVE_SIMD_INTRINSICS
    auto packet(size_t i) const
    {
        return mLhs.packet(i) * Packed<value_type>(mScalar);
    }
#endif

    auto aliases(const void *ptr) const -> bool
    {
        return mLhs.aliases(ptr);
    }
};


} // End namespace tl