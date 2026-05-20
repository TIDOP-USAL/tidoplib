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

#include "tidop/core/base/Exception.h"
#include "tidop/math/algebra/eval/MatVecMulEval.h"
#include "tidop/math/algebra/eval/VecBinaryEval.h"
#include "tidop/math/algebra/eval/VecScalarEval.h"
#include "tidop/math/algebra/eval/VectorEval.h"
#include "tidop/math/algebra/eval/VecUnaryMinusEval.h"
#include "tidop/math/algebra/eval/MatrixColEval.h"
#include "tidop/math/algebra/eval/MatrixRowEval.h"
#include "tidop/math/algebra/eval/MatrixDiagonalEval.h"

#include <type_traits>

/// \cond

namespace tl
{

namespace detail
{

template<typename Vector_t, VectorExpr Expr>
auto assign(Vector_t &dst, const Expr &expr) -> Vector_t &
{
    using CleanExpr = std::remove_cvref_t<Expr>;
    using value_type = typename vector_traits<Vector_t>::value_type;

    constexpr size_t vector_size = vector_traits<Vector_t>::size;
    constexpr size_t expr_size = vector_traits<Expr>::size;
    if constexpr (vector_size != DynamicData && expr_size != DynamicData) {
        static_assert(vector_size == expr_size, "Vector sizes must match for static vectors");
    }

    // Resize
    if constexpr (vector_size == DynamicData) {
        if (dst.size() != expr.size()) {
            dst.resize(expr.size());
        }
    } else {
        TL_ASSERT(expr.size() == dst.size(), "Static vector cannot be resized");
    }

    // Check alias
    if (expr.aliases(dst.data())) {
        Vector_t tmp = expr;
        return assign(dst, tmp);
    }

    if constexpr (is_matvec_product_v<CleanExpr>) {

        const auto &mat = expr.lhs();
        const auto &vec = expr.rhs();

        detail::mat_vec_mul(mat, vec, dst);
        return dst;
    }
        
    // Evaluator
    Evaluator<CleanExpr> eval(expr);

    size_t size = dst.size();
    size_t i = 0;

#ifdef TL_HAVE_SIMD_INTRINSICS
    if constexpr (vector_traits<CleanExpr>::has_contiguous_memory) {
        constexpr size_t packed_size = PackedTraits<Packed<value_type>>::size;
        const size_t max_size = size - (size % packed_size);

        for (; i < max_size; i += packed_size) {
            Packed<value_type> result_packet = eval.packet(i);
            result_packet.storeUnaligned(&dst[i]);
        }
    }
#endif

    for (; i < size; ++i) {
        dst[i] = eval.coeff(i);
    }

    return dst;
}

template<typename Row, typename Expr>
void assign_row(Row &dst, const Expr &expr)
{
    TL_ASSERT(expr.size() == dst.size(), "Row size mismatch");

    using CleanExpr = std::remove_cvref_t<Expr>;
    using value_type = typename vector_traits<Row>::value_type;

    if (expr.aliases(&dst[0])) {
        Vector<value_type> tmp = expr;
        assign_row(dst, tmp);
        return;
    }

    if constexpr (is_matvec_product_v<CleanExpr>) {

        const auto &mat = expr.lhs();
        const auto &vec = expr.rhs();
        detail::mat_vec_mul(mat, vec, dst);

    } else {

        Evaluator<CleanExpr> eval(expr);
        size_t size = dst.size();
        size_t i = 0;

#ifdef TL_HAVE_SIMD_INTRINSICS
        if constexpr (vector_traits<Expr>::has_contiguous_memory) {

            constexpr size_t packed_size = Packed<value_type>::size();
            const size_t max_size = size - (size % packed_size);

            for (; i < max_size; i += packed_size) {
                auto result_packet = eval.packet(i);
                result_packet.storeUnaligned(&dst[i]);
            }
        }
#endif
            
        for (; i < size; ++i) {
            dst[i] = eval.coeff(i);
        }

    }
}

template<typename Col, typename Expr>
void assign_col(Col &dst, const Expr &expr)
{
    TL_ASSERT(expr.size() == dst.size(), "Column size mismatch");

    using CleanExpr = std::remove_cvref_t<Expr>;
    using value_type = typename vector_traits<Col>::value_type;

    if (expr.aliases(&dst[0])) {
        Vector<value_type> tmp = expr;
        assign_col(dst, tmp);
        return;
    }

    if constexpr (is_matvec_product_v<CleanExpr>) {

        const auto &mat = expr.lhs();
        const auto &vec = expr.rhs();
        detail::mat_vec_mul(mat, vec, dst);

    } else {
                
        Evaluator<CleanExpr> eval(expr);

        for (size_t i = 0; i < dst.size(); ++i) {
            dst[i] = eval.coeff(i);
        }
    }
}

template<typename Diag, typename Expr>
void assign_diagonal(Diag &dst, const Expr &expr)
{
    TL_ASSERT(expr.size() == dst.size(), "Diagonal size mismatch");

    using CleanExpr = std::remove_cvref_t<Expr>;
    using value_type = typename vector_traits<Diag>::value_type;

    if (expr.aliases(&dst[0])) {
        Vector<value_type> tmp = expr;
        assign_diagonal(dst, tmp);
        return;
    }

    if constexpr (is_matvec_product_v<CleanExpr>) {

        const auto &mat = expr.lhs();
        const auto &vec = expr.rhs();
        detail::mat_vec_mul(mat, vec, dst);

    } else {

        Evaluator<CleanExpr> eval(expr);

        for (size_t i = 0; i < dst.size(); ++i) {
            dst[i] = eval.coeff(i);
        }
    }
}

} // namespace detail

} // namespace tl

/// \endcond