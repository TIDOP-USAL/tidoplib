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

template<typename LHS, typename RHS>
class MatVecMulExpr 
  : public VectorBase<MatVecMulExpr<LHS, RHS>>
{

private:

    const LHS &mMat;
    const RHS &mVec;

public:

    using value_type = std::remove_cv_t<typename matrix_traits<LHS>::value_type>;

    MatVecMulExpr(const LHS &mat, const RHS &vec)
      : mMat(mat), 
        mVec(vec)
    {
        TL_ASSERT(mat.cols() == vec.size(), "Matrix-Vector mismatch");
    }

    constexpr auto size() const noexcept -> size_t { return mMat.rows(); }

    const LHS &lhs() const { return mMat; }
    const RHS &rhs() const { return mVec; }

    auto aliases(const void *ptr) const -> bool
    {
        return mMat.aliases(ptr) || mVec.aliases(ptr);
    }

    auto operator[](size_t r) const -> value_type
    {
        value_type sum = 0;
        size_t cols = mMat.cols();

        for (size_t c = 0; c < cols; ++c) {
            sum += mMat(r, c) * mVec[c];
        }

        return sum;
    }

//#ifdef TL_HAVE_SIMD_INTRINSICS
//    auto packet(size_t r) const
//    {
//        // ⚠️ IMPORTANTE:
//        // MatVec no es fácilmente vectorizable por filas completas
//        // normalmente NO implementas packet aquí
//        // (Eigen tampoco lo hace así)
//
//        // Puedes dejarlo sin implementar o fallback
//        return Packet<value_type>::zero();
//    }
//#endif

};

} // End namespace tl