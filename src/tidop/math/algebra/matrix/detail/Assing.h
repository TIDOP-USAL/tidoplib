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

#include "tidop/core/base/exception.h"
#include "tidop/math/algebra/eval/MatBinaryEval.h"
#include "tidop/math/algebra/eval/MatScalarEval.h"
#include "tidop/math/algebra/eval/MatMulEval.h"
#include "tidop/math/algebra/eval/MatrixEval.h"
#include "tidop/math/algebra/eval/MatrixBlockEval.h"
#include "tidop/math/algebra/eval/MatUnaryMinusEval.h"
#include "tidop/math/algebra/eval/TransposeEval.h"
#include "tidop/math/algebra/matrix/detail/MulMat.h"

#include <type_traits>

namespace tl
{

namespace detail
{

template<typename Matrix_t, MatrixExpr Expr>
auto assign(Matrix_t &dst, const Expr &expr) -> Matrix_t&
{
    using CleanExpr = std::remove_cvref_t<Expr>;
    using value_type = typename Matrix_t::value_type;

    // TODO: Ver si es mejor hacer Resize despues de alias
    
    // Resize
    if constexpr (matrix_traits<Matrix_t>::rows == DynamicData || matrix_traits<Matrix_t>::cols == DynamicData) {
        if (dst.rows() != expr.rows() || dst.cols() != expr.cols()) {
            dst.resize(expr.rows(), expr.cols());
        }
    } else {
        TL_ASSERT(expr.rows() == dst.rows() && expr.cols() == dst.cols(),
            "Static matrix cannot be resized");
    }

    // Check alias
    if (expr.aliases(dst.data())) {
        Matrix_t tmp = expr;
        return assign(dst, tmp);
    }

    if constexpr (is_matrix_product_v<CleanExpr>) {
        const auto &lhs = expr.lhs();
        const auto &rhs = expr.rhs();
        //dst.fill(0);
        mulmat(lhs, rhs, dst);
        return dst;
    }

    // Evaluator
    Evaluator<CleanExpr> eval(expr);

    size_t size = dst.rows() * dst.cols();
    size_t i = 0;

    // SIMD
#ifdef TL_HAVE_SIMD_INTRINSICS
    if constexpr (matrix_traits<CleanExpr>::has_contiguous_memory) {
        constexpr size_t packed_size = PackedTraits<Packed<value_type>>::size;
        const size_t max_size = size - (size % packed_size);

        for (; i < max_size; i += packed_size) {
            Packed<value_type> result_packet = eval.packet(i);
            result_packet.storeUnaligned(&dst.data()[i]);
        }
    }
#endif

    for (; i < size; ++i) {
        dst(i) = eval.coeff(i);
    }

    return dst;
}


template<typename Block, typename Expr>
void assign_block(Block &dst, const Expr &expr)
{
    using CleanExpr = std::remove_cvref_t<Expr>;

    // Alias
    if (expr.aliases(&dst(0, 0))) {
        Matrix<typename Block::value_type> tmp = expr;
        assign_block(dst, tmp);
        return;
    }

    if constexpr (is_matrix_product_v<CleanExpr>) {

        const auto &lhs = expr.lhs();
        const auto &rhs = expr.rhs();

        // TODO: Aqui no siempre se necesita un temporal, solo cuando hay alias
        Matrix<typename matrix_traits<Block>::value_type> tmp(lhs.rows(), rhs.cols());

        detail::mulmat(lhs, rhs, tmp);

        assign_block(dst, tmp);

    } else {

        Evaluator<CleanExpr> eval(expr);

        for (size_t r = 0; r < dst.rows(); ++r) {
            for (size_t c = 0; c < dst.cols(); ++c) {
                dst(r, c) = eval.coeff(r, c);
            }
        }
    }
}

} // namespace detail

} // namespace tl