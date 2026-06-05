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

/*! \file VectorBase.h
 * \brief Base class for vector operations with expression templates.
 *
 * This file defines the `VectorBase` class, which serves as the foundation for all
 * vector types in the library. It provides common vector operations such as
 * addition, subtraction, scalar multiplication, dot product, cross product,
 * normalization, and element‑wise operations. The class uses expression templates
 * to enable lazy evaluation and avoid temporary objects, resulting in efficient code.
 *
 * \ingroup Vector
 * \see tl::Vector, tl::VectorExpr, tl::BaseExpr
 */

#pragma once

#include <vector>
#include <random>
#include <iomanip>

#include "tidop/core/base/Exception.h"
#include "tidop/core/base/TypeConversions.h"
#include "tidop/math/base/Data.h"
#include "tidop/math/algebra/BaseExpr.h"
#include "tidop/math/algebra/expr/DiagonalMatrixExpr.h"
#include "tidop/math/algebra/expr/VecBinaryExpr.h"
#include "tidop/math/algebra/expr/VecScalarExpr.h"
#include "tidop/math/algebra/expr/VecUnaryMinusExpr.h"
#include "tidop/math/algebra/expr/MatVecMulExpr.h"
#include "tidop/math/base/Simd.h"
#include "tidop/math/base/Concepts.h"
#include "tidop/math/base/Traits.h"
#include "tidop/math/algebra/vector/detail/VecEqual.h"

namespace tl
{

/*! \addtogroup Vector
 *  \{
 */

 /// \cond

template<typename T, size_t Size>
class Vector;

/// \endcond

/*!
 * \class VectorBase
 * \brief Base class for vector operations.
 *
 * \tparam Derived The derived vector type (e.g., `Vector<T, Size>`).
 *
 * This class implements common vector arithmetic and geometric operations.
 * It is designed as a CRTP base, so the actual vector storage is defined in
 * the derived class (usually `Vector`). The operations are implemented using
 * expression templates, which defer evaluation and eliminate unnecessary
 * temporaries.
 * 
 * ### Example
 * \code
 * Vector<double, 3> v1 = {1, 2, 3};
 * Vector<double, 3> v2 = {4, 5, 6};
 * Vector<double, 3> sum = v1 + v2;
 * 
 * double dot = v1.dotProduct(v2);
 * v1.normalize();
 * \endcode
 *
 * \note The derived class must provide `size()`, `operator[]`, `begin()`, `end()`,
 *       and a `packet(i)` method for SIMD optimizations.
 */
template<typename Derived>
class VectorBase
  : public BaseExpr<Derived>
{

private:

    static constexpr size_t Size = vector_traits<Derived>::size;

public:

    using derived_type = Derived;
    using value_type = typename vector_traits<Derived>::value_type;
    static constexpr size_t dimensions = Size;
    using is_vector_expr_tag = void;

public:

    /*!
     * \brief Default constructor.
     */
    constexpr VectorBase() = default;

    /*!
     * \brief Computes the Euclidean norm (magnitude) of the vector.
     * \return The Euclidean norm as `double`.
     */
    [[nodiscard]]
    auto module() const -> double;

    /*!
     * \brief Alias for `module()`.
     * \return The Euclidean norm as `double`.
     */
    [[nodiscard]]
    auto norm() const -> double;

    /*!
     * \brief Computes the squared Euclidean norm.
     * \return The squared norm (dot product with itself) as `double`.
     */
    [[nodiscard]]
    constexpr auto squaredNorm() const -> double;

    /*!
     * \brief Normalizes the vector in place (makes it unit length).
     * \note Only available for floating‑point types. If the vector is already zero,
     *       no operation is performed.
     */
    void normalize() requires std::is_floating_point_v<value_type>;

    /*!
     * \brief Computes the dot product with another vector (or expression).
     * \tparam Expr A type satisfying `VectorExpr`.
     * \param[in] expr The vector expression to dot with.
     * \return The dot product as `double`.
     * \throws Exception if the sizes differ.
     */
    template<VectorExpr Expr>
    [[nodiscard]]
    constexpr auto dotProduct(const Expr &expr) const -> double;

    /*!
     * \brief Computes the cross product of two 3‑dimensional vectors.
     * \tparam Expr A type satisfying `VectorExpr`.
     * \param[in] expr The other vector.
     * \return A new `Vector<value_type, 3>` containing the cross product.
     * \note Only defined for vectors with dimension 3.
     */
    template<VectorExpr Expr>
    [[nodiscard]]
    constexpr auto cross(const Expr &expr) const -> Vector<value_type, 3>;

    /*!
     * \brief Computes the sum of all elements.
     * \return The sum as `value_type`.
     */
    [[nodiscard]]
    constexpr auto sum() const -> value_type;

    /*!
     * \brief Converts the vector into a diagonal matrix expression.
     *
     * Returns an expression that represents a square diagonal matrix whose diagonal
     * entries are the elements of this vector. Off‑diagonal entries are zero.
     * The expression is evaluated lazily; evaluation occurs when the expression is
     * assigned to a concrete matrix or when `.eval()` is explicitly called.
     *
     * The resulting matrix has size `N x N`, where `N` is the size of the vector.
     *
     * \return A `DiagonalMatrixExpr<Derived>` representing the diagonal matrix.
     *
     * ### Example
     * \code
     * Vector<double, 3> v = {1.0, 2.0, 3.0};
     * auto diag_expr = v.asDiagonal();        // expression, not evaluated yet
     * Matrix<double, 3, 3> D = diag_expr;     // evaluated -> diag(1,2,3)
     *
     * // Works with dynamic vectors and expressions:
     * Vector<double> w = {4,5,6};
     * Matrix<double> M = (w * 2).asDiagonal(); // M = diag(8,10,12)
     * \endcode
     *
     * \note The returned expression is lightweight and does not allocate memory
     *       until evaluated. It is intended for temporary use in matrix operations.
     */
    [[nodiscard]]
    constexpr auto asDiagonal() const -> DiagonalMatrixExpr<Derived>;

    /* Unary arithmetic operators */

    /*!
     * \brief Unary plus operator.
     * \return A const reference to the vector itself.
     */
    [[nodiscard]]
    constexpr auto operator+() const noexcept -> const Derived&;

    /*!
     * \brief Unary minus operator.
     * \return An expression representing the negated vector.
     */
    [[nodiscard]]
    constexpr auto operator-() const;

    /* Binary arithmetic operators */

    /*!
     * \brief Vector addition.
     * \tparam Expr A type satisfying `VectorExpr`.
     * \param[in] expr The vector to add.
     * \return An expression representing the element‑wise sum.
     */
	template<VectorExpr Expr>
    [[nodiscard]]
    constexpr auto operator +(const Expr &expr) const;
    
    /*!
     * \brief Vector subtraction.
     * \tparam Expr A type satisfying `VectorExpr`.
     * \param[in] expr The vector to subtract.
     * \return An expression representing the element‑wise difference.
     */
    template<VectorExpr Expr>
    [[nodiscard]]
    constexpr auto operator -(const Expr &expr) const;
	
    /*!
     * \brief Element‑wise multiplication (Hadamard product).
     * \tparam Expr A type satisfying `VectorExpr`.
     * \param[in] expr The vector to multiply by.
     * \return An expression representing the element‑wise product.
     */
    template<VectorExpr Expr>
    [[nodiscard]]
    constexpr auto cwiseProduct(const Expr &expr) const;

    /*!
     * \brief Element‑wise division.
     * \tparam Expr A type satisfying `VectorExpr`.
     * \param[in] expr The vector to divide by.
     * \return An expression representing the element‑wise quotient.
     */
    template<VectorExpr Expr>
    [[nodiscard]]
    constexpr auto cwiseDiv(const Expr &expr) const;

    /*!
     * \brief Scalar multiplication.
     * \param[in] scalar The scalar value.
     * \return An expression representing the scaled vector.
     */
    [[nodiscard]]
    constexpr auto operator*(value_type scalar) const;

    /*!
     * \brief Scalar division.
     * \param[in] scalar The scalar divisor.
     * \return An expression representing the vector divided by the scalar.
     */
    [[nodiscard]]
    constexpr auto operator/(value_type scalar) const;

    /*!
     * \brief Compound addition assignment.
     * \tparam Expr A type satisfying `VectorExpr`.
     * \param[in] expr The vector to add.
     * \return Reference to the modified derived vector.
     * \note Only available if the derived type is mutable.
     */
    template<VectorExpr Expr>
        requires (vector_traits<Derived>::is_mutable)
    auto operator+=(const Expr &expr);

    /*!
     * \brief Compound subtraction assignment.
     * \tparam Expr A type satisfying `VectorExpr`.
     * \param[in] expr The vector to subtract.
     * \return Reference to the modified derived vector.
     * \note Only available if the derived type is mutable.
     */
    template<VectorExpr Expr>
        requires (vector_traits<Derived>::is_mutable)
    auto operator-=(const Expr &expr) -> Derived &;
	
    /*!
     * \brief In‑place element‑wise multiplication.
     * \tparam Expr A type satisfying `VectorExpr`.
     * \param[in] expr The vector to multiply by.
     * \return Reference to the modified derived vector.
     * \note Only available if the derived type is mutable.
     */
    template<VectorExpr Expr>
        requires (vector_traits<Derived>::is_mutable)
    auto cwiseProductInPlace(const Expr &expr) -> Derived &;

    /*!
     * \brief In‑place element‑wise division.
     * \tparam Expr A type satisfying `VectorExpr`.
     * \param[in] expr The vector to divide by.
     * \return Reference to the modified derived vector.
     * \note Only available if the derived type is mutable.
     */
    template<VectorExpr Expr>
        requires (vector_traits<Derived>::is_mutable)
    auto cwiseDivInPlace(const Expr &expr) -> Derived &;

    /*!
     * \brief Scalar multiplication assignment.
     * \tparam Scalar Numeric type convertible to `value_type`.
     * \param[in] scalar The scalar multiplier.
     * \return Reference to the modified derived vector.
     */
    template<typename Scalar>
    auto operator *=(Scalar scalar) -> Derived& 
        requires (vector_traits<Derived>::is_mutable &&
                  std::is_convertible_v<Scalar, value_type>);

    /*!
     * \brief Scalar division assignment.
     * \tparam Scalar Numeric type convertible to `value_type`.
     * \param[in] scalar The scalar divisor.
     * \return Reference to the modified derived vector.
     */
    template<typename Scalar>
    auto operator /=(Scalar scalar) -> Derived &
        requires (vector_traits<Derived>::is_mutable &&
                  std::is_convertible_v<Scalar, value_type>);
	
    /*!
     * \brief Evaluates the expression and returns a concrete vector.
     * \return A concrete `Vector` object (static or dynamic as appropriate).
     */
    constexpr auto eval() const;

};




/* VectorBase implementation */

template<typename Derived>
auto VectorBase<Derived>::module() const -> double
{
    return sqrt(this->squaredNorm());
}

template<typename Derived>
auto VectorBase<Derived>::norm() const -> double
{
    return this->module();
}

template<typename Derived>
constexpr auto VectorBase<Derived>::squaredNorm() const -> double
{
    return this->dotProduct(this->derived());
}

template<typename Derived>
void VectorBase<Derived>::normalize() requires std::is_floating_point_v<value_type>
{
    double m = this->module();
    if (m > std::numeric_limits<value_type>::epsilon()) {
        *this /= static_cast<value_type>(m);
    }
}

template<typename Derived>
constexpr auto VectorBase<Derived>::sum() const -> value_type
{
    value_type summation{};

    auto &derived = this->derived();
    auto eval = make_evaluator(derived);

    size_t i = 0;

    auto run_scalar_loop = [&]() constexpr {
        for (; i < derived.size(); ++i) {
            summation += eval.coeff(i);
        }
    };

    // Tiempo de compilación
    if (std::is_constant_evaluated()) {
        run_scalar_loop();
        return summation;
    }

#ifdef TL_HAVE_SIMD_INTRINSICS
    using Scalar = std::remove_cv_t<value_type>;
    constexpr size_t packed_size = PackedTraits<Packed<Scalar>>::size;
    size_t max_vector = (derived.size() / packed_size) * packed_size;

    if constexpr (vector_traits<Derived>::has_contiguous_memory) {

        for (; i < max_vector; i += packed_size) {
            summation += eval.packet(i).sum();
        }

    }

#endif

    run_scalar_loop();

    return summation;
}

template<typename Derived>
constexpr auto VectorBase<Derived>::asDiagonal() const -> DiagonalMatrixExpr<Derived>
{
    return DiagonalMatrixExpr<Derived>(this->derived());
}

template<typename Derived>
template<VectorExpr Expr>
constexpr auto VectorBase<Derived>::dotProduct(const Expr &expr) const -> double
{
    auto &derived = this->derived();

    TL_ASSERT(derived.size() == expr.size(), "Different vector size");

    double dot = 0.0;
    size_t i = 0;

    auto eval_derived = make_evaluator(derived);
    auto eval_expr = make_evaluator(expr);

    auto run_scalar_loop = [&]() constexpr {
        for (; i < derived.size(); ++i) {
            dot += static_cast<double>(eval_derived.coeff(i)) * static_cast<double>(eval_expr.coeff(i));
        }
    };

    // Tiempo de compilación
    if (std::is_constant_evaluated()) {
        run_scalar_loop();
        return dot;
    }

#ifdef TL_HAVE_SIMD_INTRINSICS
    using Scalar = std::remove_cv_t<value_type>;
    Packed<Scalar> packed_result(0);

    constexpr size_t packed_size = PackedTraits<Packed<Scalar>>::size;
    size_t max_vector = (derived.size() / packed_size) * packed_size;

    if constexpr (vector_traits<Derived>::has_contiguous_memory &&
                  vector_traits<Expr>::has_contiguous_memory) {

        for (; i < max_vector; i += packed_size) {
            packed_result += eval_derived.packet(i) * eval_expr.packet(i);
        }

        dot += static_cast<double>(packed_result.sum());
    }
#endif

    //for (; i < derived.size(); ++i) {
    //    dot += static_cast<double>(eval_derived.coeff(i)) * static_cast<double>(eval_expr.coeff(i));
    //}
    run_scalar_loop();

    return dot;
}

template<typename Derived>
template<VectorExpr Expr>
constexpr auto VectorBase<Derived>::cross(const Expr &expr) const -> Vector<value_type, 3>
{
    static_assert(dimensions == 3, "The cross product is only defined for 3 dimensions.");

    Vector<value_type, 3> res{};
    Vector<value_type, 3> a = this->derived();
    Vector<value_type, 3> b = expr;

    res[0] = a[1] * b[2] - a[2] * b[1];
    res[1] = a[2] * b[0] - a[0] * b[2];
    res[2] = a[0] * b[1] - a[1] * b[0];

    return res;
}

/* Unary arithmetic operators */

template<typename Derived>
constexpr auto VectorBase<Derived>::operator+() const noexcept -> const Derived&
{
    return this->derived();
}

template<typename Derived>
constexpr auto VectorBase<Derived>::operator-() const
{
    return VecUnaryMinusExpr<Derived>(this->derived());
}

/* Binary arithmetic operators */

template<typename Derived>
template<VectorExpr Expr>
constexpr auto VectorBase<Derived>::operator+(const Expr &expr) const
{
    return VecBinaryExpr<Derived, Expr, AddOp>(this->derived(), expr);
}

template<typename Derived>
template<VectorExpr Expr>
constexpr auto VectorBase<Derived>::operator -(const Expr &expr) const
{
    return VecBinaryExpr<Derived, Expr, SubOp>(this->derived(), expr);
}

template<typename Derived>
template<VectorExpr Expr>
constexpr auto VectorBase<Derived>::cwiseProduct(const Expr &expr) const
{
    return VecBinaryExpr<Derived, Expr, MulOp>(this->derived(), expr);
}

template<typename Derived>
template<VectorExpr Expr>
constexpr auto VectorBase<Derived>::cwiseDiv(const Expr &expr) const
{
    return VecBinaryExpr<Derived, Expr, DivOp>(this->derived(), expr);
}

template<typename Derived>
constexpr auto VectorBase<Derived>::operator*(value_type scalar) const
{
    return VecScalarExpr<Derived, value_type, MulOp>(this->derived(), scalar);
}

template<typename Derived>
constexpr auto VectorBase<Derived>::operator/(value_type scalar) const
{
    return VecScalarExpr<Derived, value_type, DivOp>(this->derived(), scalar);
}

template<typename Derived>
template<VectorExpr Expr>
    requires (vector_traits<Derived>::is_mutable)
auto VectorBase<Derived>::operator+=(const Expr &expr)
{
    std::move(this->derived()) = this->derived() + expr;

    return this->derived();
}

template<typename Derived>
template<VectorExpr Expr>
    requires (vector_traits<Derived>::is_mutable)
auto VectorBase<Derived>::operator-=(const Expr &expr) -> Derived &
{
    std::move(this->derived()) = this->derived() - expr;

    return this->derived();
}

template<typename Derived>
template<VectorExpr Expr>
    requires (vector_traits<Derived>::is_mutable)
auto VectorBase<Derived>::cwiseProductInPlace(const Expr &expr) -> Derived &
{
    std::move(this->derived()) = this->derived().cwiseProduct(expr);
    return this->derived();
}

template<typename Derived>
template<VectorExpr Expr>
    requires (vector_traits<Derived>::is_mutable)
auto VectorBase<Derived>::cwiseDivInPlace(const Expr &expr) -> Derived &
{
    std::move(this->derived()) = this->derived().cwiseDiv(expr);
    return this->derived();
}

template<typename Derived>
template<typename Scalar>
auto VectorBase<Derived>::operator *=(Scalar scalar) -> Derived &
    requires (vector_traits<Derived>::is_mutable &&
              std::is_convertible_v<Scalar, value_type>)
{
    std::move(this->derived()) = this->derived() * scalar;

    return this->derived();
}

template<typename Derived>
template<typename Scalar>
auto VectorBase<Derived>::operator /=(Scalar scalar) -> Derived &
    requires (vector_traits<Derived>::is_mutable &&
              std::is_convertible_v<Scalar, value_type>)
{
    std::move(this->derived()) = this->derived() / scalar;

    return this->derived();
}

template<typename Derived>
constexpr auto VectorBase<Derived>::eval() const
{
    if constexpr (vector_traits<Derived>::is_plain) {
        return this->derived();
    } else {
        using T = std::remove_cv_t<typename vector_traits<Derived>::value_type>;
        constexpr size_t Size = vector_traits<Derived>::size;
        return Vector<T, Size>(this->derived());
    }
}


template<MatrixExpr LHS, VectorExpr RHS>
[[nodiscard]]
constexpr auto operator*(const LHS &mat, const RHS &vec)
{
    return MatVecMulExpr<LHS, RHS>(mat, vec);
}

template<VectorExpr RHS, MatrixExpr LHS>
[[nodiscard]]
constexpr auto operator*(const RHS &vec, const LHS &mat)
{
    return MatVecMulExpr<LHS, RHS>(mat, vec);
}


template<typename DerivedL, typename DerivedR>
[[nodiscard]]
constexpr auto operator==(const VectorBase<DerivedL> &lhs,
                          const VectorBase<DerivedR> &rhs) -> bool
{
    static_assert(!vector_traits<DerivedL>::is_expression &&
        !vector_traits<DerivedR>::is_expression,
        "Vector expressions cannot be compared directly.");

    return detail::vector_equal(lhs.derived(), rhs.derived());
}

template<typename DerivedL, typename DerivedR>
[[nodiscard]]
constexpr auto operator!=(const VectorBase<DerivedL> &lhs,
                          const VectorBase<DerivedR> &rhs) -> bool
{
    return !(lhs == rhs);
}

template<VectorExpr Expr>
auto operator<<(std::ostream &os, const Expr &expr) -> std::ostream &
{
    for (size_t i = 0; i < expr.size(); i++) {
        os << expr[i] << "\n";
    }
    return os;
}


/*! \} */


} // End namespace tl

