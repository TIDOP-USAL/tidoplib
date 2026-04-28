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

#include <vector>
#include <random>
#include <iomanip>

#include "tidop/core/base/exception.h"
#include "tidop/core/base/type_conversions.h"
#include "tidop/math/math.h"
#include "tidop/math/base/data.h"
#include "tidop/math/algebra/BaseExpr.h"
#include "tidop/math/algebra/expr/VecBinaryExpr.h"
#include "tidop/math/algebra/expr/VecScalarExpr.h"
#include "tidop/math/algebra/expr/VecUnaryMinusExpr.h"
#include "tidop/math/algebra/expr/MatVecMulExpr.h"
#include "tidop/math/base/simd.h"
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
 * \tparam VectorDerived A template parameter representing the derived vector class.
 *
 * The `VectorBase` class serves as a base for vector classes, defining fundamental
 * operations between vectors and between vectors and scalars. It provides a foundation
 * for creating vector types with customizable behavior.
 *
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
    VectorBase();

    /*!
     * \brief Calculates the magnitude (or length) of the vector.
     * \return The magnitude of the vector.
     */
    auto module() const -> double
    {
        return sqrt(this->squaredNorm());
    }

    auto norm() const -> double
    {
        return this->module();
    }

    auto squaredNorm() const -> double
    {
        return this->dotProduct(this->derived());
    }

    /*!
     * \brief Normalizes the vector, making its magnitude equal to 1.
     */
    void normalize()
        requires std::is_floating_point_v<value_type>
    {
        double m = this->module();
        if (m > std::numeric_limits<value_type>::epsilon()) {
            *this /= static_cast<value_type>(m);
        }
    }

    /*!
     * \brief Computes the dot product with another vector.
     * \param[in] vector Another vector to compute the dot product with.
     * \tparam OtherDerived The type of the other vector.
     * \return The dot product as a double.
     */
    template<VectorExpr Expr>
    auto dotProduct(const Expr &expr) const -> double
    {
        auto &derived = this->derived();

        TL_ASSERT(derived.size() == expr.size(), "Different vector size");

        double dot = 0.0;
        size_t i = 0;

        auto eval_derived = make_evaluator(derived);
        auto eval_expr = make_evaluator(expr);

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

        for (; i < derived.size(); ++i) {
            dot += static_cast<double>(eval_derived.coeff(i)) * static_cast<double>(eval_expr.coeff(i));
        }

        return dot;
    }

    template<VectorExpr Expr>
    auto cross(const Expr &expr) const -> Vector<value_type, 3>
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

    auto sum() const -> value_type
    {
        value_type summation{};

        auto &derived = this->derived();
        auto eval = make_evaluator(derived);

        size_t i = 0;

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

        for (; i < derived.size(); ++i) {
            summation += eval.coeff(i);
        }

        return summation;
    }

    /* Unary arithmetic operators */

    /*!
     * \brief Unary plus operator.
     * \return A copy of the vector.
     */
    auto operator+() const noexcept -> const Derived&
    {
        return this->derived();
    }

    /*!
     * \brief Unary minus operator.
     * \return A negated copy of the vector.
     */
    auto operator-() const
    {
        return VecUnaryMinusExpr<Derived>(this->derived());
    }

    /* Binary arithmetic operators */

    /*!
     * \brief Adds two vectors.
     * \param[in] vector2 The vector to add.
     * \return The sum of the vectors.
     */
	template<VectorExpr Expr>
    auto operator +(const Expr &expr) const
    {
        return VecBinaryExpr<Derived, Expr, AddOp>(this->derived(), expr);
    }
    
    /*!
     * \brief Subtracts one vector from another.
     * \param[in] vector2 The vector to subtract.
     * \return The difference of the vectors.
     */
    template<VectorExpr Expr>
    auto operator -(const Expr &expr) const
    {
        return VecBinaryExpr<Derived, Expr, SubOp>(this->derived(), expr);
    }
	
    /*!
     * \brief Multiplies two vectors element-wise.
     * \param[in] vector2 The vector to multiply.
     * \return The product of the vectors.
     */
    template<VectorExpr Expr>
    auto cwiseProduct(const Expr &expr) const
    {
        return VecBinaryExpr<Derived, Expr, MulOp>(this->derived(), expr);
    }

    /*!
     * \brief Divides two vectors element-wise.
     * \param[in] vector2 The vector to divide by.
     * \return The quotient of the vectors.
     */
    template<VectorExpr Expr>
    auto cwiseDiv(const Expr &expr) const
    {
        return VecBinaryExpr<Derived, Expr, DivOp>(this->derived(), expr);
    }

    /*!
     * \brief Multiplies the vector by a scalar.
     * \param[in] scalar The scalar to multiply by.
     * \return The scaled vector.
     */
    auto operator*(value_type scalar) const
    {
        return VecScalarExpr<Derived, value_type, MulOp>(this->derived(), scalar);
    }

    /*!
     * \brief Divides the vector by a scalar.
     * \param[in] scalar The scalar to divide by.
     * \return The scaled vector.
     */
    auto operator/(value_type scalar) const
    {
        return VecScalarExpr<Derived, value_type, DivOp>(this->derived(), scalar);
    }

    /*!
     * \brief Adds another vector to this vector.
     * \param[in] vector The vector to add.
     * \tparam OtherDerived The type of the other vector.
     * \return A reference to this vector.
     */
    template<VectorExpr Expr>
        requires (vector_traits<Derived>::is_mutable)
    auto operator+=(const Expr &expr)
    {
        std::move(this->derived()) = this->derived() + expr;

        return this->derived();
    }

    /*!
     * \brief Subtracts another vector from this vector.
     * \param[in] vector The vector to subtract.
     * \tparam OtherDerived The type of the other vector.
     * \return A reference to this vector.
     */
    template<VectorExpr Expr>
        requires (vector_traits<Derived>::is_mutable)
    auto operator-=(const Expr &expr) -> Derived &
    {
        std::move(this->derived()) = this->derived() - expr;

        return this->derived();
    }
	
    /*!
     * \brief Multiplies this vector by another vector element-wise.
     * \param[in] vector The vector to multiply by.
     * \tparam OtherDerived The type of the other vector.
     * \return A reference to this vector.
     */
    template<VectorExpr Expr>
        requires (vector_traits<Derived>::is_mutable)
    auto cwiseProductInPlace(const Expr &expr) -> Derived &
    {
        std::move(this->derived()) = this->derived().cwiseProduct(expr);
        return this->derived();
    }

    /*!
     * \brief Divides this vector by another vector element-wise.
     * \param[in] vector The vector to divide by.
     * \tparam OtherDerived The type of the other vector.
     * \return A reference to this vector.
     */
    template<VectorExpr Expr>
        requires (vector_traits<Derived>::is_mutable)
    auto cwiseDivInPlace(const Expr &expr) -> Derived &
    {
        std::move(this->derived()) = this->derived().cwiseDiv(expr);
        return this->derived();
    }

    /*!
     * \brief Multiplies the vector by a scalar.
     * \param[in] scalar The scalar to multiply by.
     * \return A reference to this vector.
     */
    template<typename Scalar>
        requires (vector_traits<Derived>::is_mutable &&
                  std::is_convertible_v<Scalar, value_type>)
    auto operator *=(Scalar scalar) -> Derived &
    {
        std::move(this->derived()) = this->derived() * scalar;

        return this->derived();
    }

    /*!
     * \brief Divides the vector by a scalar.
     * \param[in] scalar The scalar to divide by.
     * \return A reference to this vector.
     */
    template<typename Scalar>
        requires (vector_traits<Derived>::is_mutable &&
                  std::is_convertible_v<Scalar, value_type>)
    auto operator /=(Scalar scalar) -> Derived &
    {
        std::move(this->derived()) = this->derived() / scalar;

        return this->derived();
    }
	
    constexpr auto eval() const
    {
        if constexpr (vector_traits<Derived>::is_plain) {
            return this->derived();
        } else {
            using T = std::remove_cv_t<typename vector_traits<Derived>::value_type>;
            constexpr size_t Size = vector_traits<Derived>::size;
            return Vector<T, Size>(this->derived());
        }
    }

};





/*------------------------------------------------------------------------*/
/* VectorBase implementation                                              */
/*------------------------------------------------------------------------*/


template<typename Derived>
VectorBase<Derived>::VectorBase()
{
}

//template<typename Derived>
//template<typename D, enable_if_vector_t<D>>
//auto VectorBase<Derived>::module() const -> double
//{
//    return sqrt(this->squaredNorm());
//}
//
//template<typename Derived>
//template<typename D, enable_if_vector_t<D>>
//auto VectorBase<Derived>::norm() const -> double
//{
//    return this->module();
//}
//
//template<typename Derived>
//template<typename D, enable_if_vector_t<D>>
//auto VectorBase<Derived>::squaredNorm() const -> double
//{
//    return this->dotProduct(this->derived());
//}
//
//template<typename Derived>
//template<typename D>
//auto VectorBase<Derived>::normalize() -> std::enable_if_t<is_vector<D>::value &&
//                                         std::is_floating_point_v<T>, void>
//{
//    double m = this->module();
//    if (m > std::numeric_limits<T>::epsilon()) {
//        *this /= static_cast<T>(m);
//    }
//}
//
//template<typename Derived>
//template<typename OtherDerived, typename D, enable_if_vector_t<D>>
//auto VectorBase<Derived>::dotProduct(const OtherDerived &vector) const -> double
//{
//    auto &derived = this->derived();
//
//    TL_ASSERT(derived.size() == vector.size(), "Different vector size");
//
//    double dot = 0.0;
//    size_t i = 0;
//
//#ifdef TL_HAVE_SIMD_INTRINSICS
//    using Scalar = std::remove_cv_t<T>;
//    Packed<Scalar> packed_a;
//    Packed<Scalar> packed_b;
//    Packed<Scalar> packed_result(0);
//
//    constexpr size_t packed_size = packed_a.size();
//    size_t max_vector = (derived.size() / packed_size) * packed_size;
//
//    if (this->properties.isEnabled(Derived::Properties::contiguous_memory) &&
//        vector.properties.isEnabled(OtherDerived::Properties::contiguous_memory)) {
//
//        for (; i < max_vector; i += packed_size) {
//
//            packed_a.loadUnaligned(&derived[i]);
//            packed_b.loadUnaligned(&vector[i]);
//
//            packed_result += packed_a * packed_b;
//
//        }
//
//        dot += static_cast<double>(packed_result.sum());
//    }
//#endif
//
//    for (; i < derived.size(); ++i) {
//        dot += static_cast<double>(derived[i]) * static_cast<double>(vector[i]);
//    }
//
//    return dot;
//}
//
//template<typename Derived>
//template<typename OtherDerived, typename D, enable_if_vector_t<D>>
//auto VectorBase<Derived>::cross(const OtherDerived &other) const -> result_type
//{
//    static_assert(dimensions == 3, "The cross product is only defined for 3 dimensions.");
//
//    result_type res{};
//    const auto &a = this->derived();
//    const auto &b = other.derived();
//
//    res[0] = a[1] * b[2] - a[2] * b[1];
//    res[1] = a[2] * b[0] - a[0] * b[2];
//    res[2] = a[0] * b[1] - a[1] * b[0];
//
//    return res;
//}
//
//template<typename Derived>
//auto VectorBase<Derived>::sum() const -> T
//{
//    T summation{};
//
//    auto &derived = this->derived();
//    size_t i = 0;
//
//#ifdef TL_HAVE_SIMD_INTRINSICS
//    using Scalar = std::remove_cv_t<T>;
//    Packed<Scalar> packed_a;
//    constexpr size_t packed_size = packed_a.size();
//    size_t max_vector = (derived.size() / packed_size) * packed_size;
//    
//    if (this->properties.isEnabled(Derived::Properties::contiguous_memory)) {
//
//        for (; i < max_vector; i += packed_size) {
//            packed_a.loadUnaligned(&derived[i]);
//            summation += packed_a.sum();
//        }
//
//    }
//
//#endif
//
//    for (; i < derived.size(); ++i) {
//        summation += derived[i];
//    }
//
//    return summation;
//}


/* Unary arithmetic operators */

//template<typename Derived>
//template<typename D, enable_if_vector_t<D>>
//auto VectorBase<Derived>::operator+() const -> Derived
//{
//    return this->derived();
//}
//
//template<typename Derived>
//template<typename D, enable_if_vector_t<D>>
//auto VectorBase<Derived>::operator-() const -> Derived
//{
//    static_assert(std::is_signed<T>::value, "Requires signed type");
//
//    using Scalar = std::remove_cv_t<T>;
//
//    auto result = this->derived();
//    size_t i = 0;
//
//#ifdef TL_HAVE_SIMD_INTRINSICS
//    Packed<Scalar> packed_value;
//    constexpr size_t packed_size = packed_value.size();
//    size_t max_vector = (result.size() / packed_size) * packed_size;
//
//    if (this->properties.isEnabled(Derived::Properties::contiguous_memory)) {
//        for (; i < max_vector; i += packed_size) {
//            packed_value.loadUnaligned(&result[i]);
//            packed_value = - packed_value;
//            packed_value.storeUnaligned(&result[i]);
//        }
//    }
//#endif
//
//    // Procesa los elementos restantes de manera escalar
//    for (; i < result.size(); ++i) {
//        result[i] = -result[i];
//    }
//
//    return result;
//}
//
///* Binary arithmetic operators */
//
//template<typename Derived>
//template<typename D, enable_if_vector_t<D>>
//auto VectorBase<Derived>::operator+(const Derived &vector2) const -> result_type
//{
//    result_type vector = this->derived();
//    vector += vector2;
//    return vector;
//}
//
//template<typename Derived>
//template<typename D, enable_if_vector_t<D>>
//auto VectorBase<Derived>::operator-(const Derived &vector2) const -> result_type
//{
//    result_type vector = this->derived();
//    vector -= vector2;
//    return vector;
//}
//
//template<typename Derived>
//template<typename D, enable_if_vector_t<D>>
//auto VectorBase<Derived>::cwiseProduct(const Derived &vector) const -> result_type
//{
//    result_type result = this->derived();
//    result.cwiseProductInPlace(vector);
//    return result;
//}
//
//template<typename Derived>
//template<typename D, enable_if_vector_t<D>>
//auto VectorBase<Derived>::cwiseDiv(const Derived &vector) const -> result_type
//{
//    result_type result = this->derived();
//    result.cwiseDivInPlace(vector);
//    return result;
//}
//
//template<typename Derived>
//template<typename D, enable_if_vector_t<D>>
//auto VectorBase<Derived>::operator*(T scalar) const -> result_type
//{
//    result_type vector = this->derived();
//    vector *= scalar;
//    return vector;
//}

//template<typename Derived>
//template<typename Scalar, typename>
//auto VectorBase<Derived>::operator*(Scalar scalar) const -> result_type
//{
//    result_type result = this->derived();
//    result *= scalar;
//    return result;
//}

//template<typename Derived>
//template<typename D, enable_if_vector_t<D>>
//auto VectorBase<Derived>::operator/(T scalar) const -> result_type
//{
//    result_type vector = this->derived();
//    vector /= scalar;
//    return vector;
//}
//
//template<typename Derived>
//template<typename OtherDerived, typename>
//auto VectorBase<Derived>::operator+=(const OtherDerived &vector) -> Derived &
//{
//    auto &derived = this->derived();
//
//    TL_ASSERT(derived.size() == vector.size(), "Different size vectors");
//
//    size_t i{0};
//
//#ifdef TL_HAVE_SIMD_INTRINSICS
//
//    Packed<T> packed_a;
//    Packed<T> packed_b;
//
//    constexpr size_t packed_size = packed_a.size();
//    size_t max_vector = (derived.size() / packed_size) * packed_size;
//
//    if (this->properties.isEnabled(Derived::Properties::contiguous_memory) &&
//        vector.properties.isEnabled(OtherDerived::Properties::contiguous_memory)) {
//
//        for (; i < max_vector; i += packed_size) {
//
//            packed_a.loadUnaligned(&derived[i]);
//            packed_b.loadUnaligned(&vector[i]);
//
//            packed_a += packed_b;
//            packed_a.storeUnaligned(&derived[i]);
//
//        }
//    }
//
//#endif
//
//    for (; i < derived.size(); ++i) {
//        derived[i] += vector[i];
//    }
//
//    return derived;
//}
//
//template<typename Derived>
//template<typename OtherDerived, typename>
//auto VectorBase<Derived>::operator-=(const OtherDerived &vector) -> Derived &
//{
//    auto &derived = this->derived();
//
//    TL_ASSERT(derived.size() == vector.size(), "");
//
//    size_t i{0};
//
//#ifdef TL_HAVE_SIMD_INTRINSICS
//
//    Packed<T> packed_a;
//    Packed<T> packed_b;
//
//    constexpr size_t packed_size = packed_a.size();
//
//    size_t max_vector = (derived.size() / packed_size) * packed_size;
//
//    if (this->properties.isEnabled(Derived::Properties::contiguous_memory) &&
//        vector.properties.isEnabled(OtherDerived::Properties::contiguous_memory)) {
//
//        for (; i < max_vector; i += packed_size) {
//
//            packed_a.loadUnaligned(&derived[i]);
//            packed_b.loadUnaligned(&vector[i]);
//            packed_a -= packed_b;
//            packed_a.storeUnaligned(&derived[i]);
//
//        }
//    }
//
//#endif
//
//    for (; i < derived.size(); ++i) {
//        derived[i] -= vector[i];
//    }
//
//    return derived;
//}
//
//template<typename Derived>
//template<typename OtherDerived, typename>
//auto VectorBase<Derived>::cwiseProductInPlace(const OtherDerived &vector) -> Derived &
//{
//    auto &derived = this->derived();
//
//    TL_ASSERT(derived.size() == vector.size(), "");
//
//    size_t i{0};
//
//#ifdef TL_HAVE_SIMD_INTRINSICS
//
//    Packed<T> packed_a;
//    Packed<T> packed_b;
//
//    constexpr size_t packed_size = packed_a.size();
//
//    if (this->properties.isEnabled(Derived::Properties::contiguous_memory) &&
//        vector.properties.isEnabled(OtherDerived::Properties::contiguous_memory)) {
//
//        size_t max_vector = (derived.size() / packed_size) * packed_size;
//        for (; i < max_vector; i += packed_size) {
//
//            packed_a.loadUnaligned(&derived[i]);
//            packed_b.loadUnaligned(&vector[i]);
//            packed_a *= packed_b;
//            packed_a.storeUnaligned(&derived[i]);
//
//        }
//    }
//
//#endif
//
//    for (; i < derived.size(); ++i) {
//        derived[i] *= vector[i];
//    }
//
//    return derived;
//}
//
//template<typename Derived>
//template<typename OtherDerived, typename>
//auto VectorBase<Derived>::cwiseDivInPlace(const OtherDerived &vector) -> Derived &
//{
//    auto &derived = this->derived();
//
//    TL_ASSERT(derived.size() == vector.size(), "");
//
//    size_t i{0};
//
//#ifdef TL_HAVE_SIMD_INTRINSICS
//
//    Packed<T> packed_a;
//    Packed<T> packed_b;
//
//    constexpr size_t packed_size = packed_a.size();
//    size_t max_vector = (derived.size() / packed_size) * packed_size;
//
//    if (this->properties.isEnabled(Properties::contiguous_memory) &&
//        vector.properties.isEnabled(OtherDerived::Properties::contiguous_memory)) {
//
//        for (; i < max_vector; i += packed_size) {
//
//            packed_a.loadUnaligned(&derived[i]);
//            packed_b.loadUnaligned(&vector[i]);
//            packed_a /= packed_b;
//            packed_a.storeUnaligned(&derived[i]);
//
//        }
//    }
//
//#endif
//
//    for (; i < derived.size(); ++i) {
//        derived[i] /= vector[i];
//    }
//
//    return derived;
//}
//
//template<typename Derived>
//template<typename Scalar, typename D, typename>
//auto VectorBase<Derived>::operator*=(Scalar scalar) -> Derived &
//{
//    auto &derived = this->derived();
//
//    for (size_t i = 0; i < derived.size(); ++i) {
//        derived[i] = numberCast<T>(derived[i] * scalar);
//    }
//    return derived;
//}
//
//template<typename Derived>
//template<typename D, enable_if_vector_t<D>>
//auto VectorBase<Derived>::operator*=(T scalar) -> Derived &
//{
//    auto &derived = this->derived();
//
//    size_t i{0};
//
//#ifdef TL_HAVE_SIMD_INTRINSICS
//
//    Packed<T> packed_a;
//    Packed<T> packed_b(scalar);
//
//    constexpr size_t packed_size = packed_a.size();
//    size_t max_vector = (derived.size() / packed_size) * packed_size;
//
//    if (this->properties.isEnabled(Properties::contiguous_memory)) {
//        for (; i < max_vector; i += packed_size) {
//
//            packed_a.loadUnaligned(&derived[i]);
//            packed_a *= packed_b;
//            packed_a.storeUnaligned(&derived[i]);
//
//        }
//    }
//
//#endif
//
//    for (; i < derived.size(); i++) {
//        derived[i] *= scalar;
//    }
//
//    return derived;
//}
//
//template<typename Derived>
//template<typename D, enable_if_vector_t<D>>
//auto VectorBase<Derived>::operator/=(T scalar) -> Derived &
//{
//    auto &derived = this->derived();
//
//    TL_ASSERT(scalar != consts::zero<T>, "Division by zero");
//
//    size_t i{0};
//
//#ifdef TL_HAVE_SIMD_INTRINSICS
//
//    if constexpr (!std::is_integral_v<T>) {
//
//        Packed<T> packed_a;
//        Packed<T> packed_b(scalar);
//
//        constexpr size_t packed_size = packed_a.size();
//        size_t max_vector = (derived.size() / packed_size) * packed_size;
//
//        if (this->properties.isEnabled(Properties::contiguous_memory)) {
//            for (; i < max_vector; i += packed_size) {
//
//                packed_a.loadUnaligned(&derived[i]);
//                packed_a /= packed_b;
//                packed_a.storeUnaligned(&derived[i]);
//
//            }
//        }
//    }
//#endif
//
//    for (; i < derived.size(); ++i) {
//        derived[i] /= scalar;
//    }
//
//    return derived;
//}
//
//template<typename Derived>
//template<typename Scalar, typename D, typename>
//auto VectorBase<Derived>::operator/=(Scalar scalar) -> Derived &
//{
//    auto &derived = this->derived();
//
//    for (size_t i = 0; i < derived.size(); ++i) {
//        derived[i] = numberCast<T>(derived[i] / scalar);
//    }
//
//    return derived;
//}

//template<typename Derived>
//template<typename OtherDerived>
//void VectorBase<Derived>::set(const OtherDerived &vector)
//{
//    auto &derived = this->derived();
//
//    using OtherT = typename vector_traits<OtherDerived>::value_type;
//
//    if(Size == DynamicData) {
//        derived = Derived(vector.size());
//    }
//
//    TL_ASSERT(derived.size() == vector.size(), "Static vector cannot be resized");
//
//    size_t i{0};
//
//#ifdef TL_HAVE_SIMD_INTRINSICS
//
//    if constexpr (std::is_same_v<T, OtherT>) {
//
//        Packed<T> packed_a;
//        Packed<T> packed_b;
//
//        constexpr size_t packed_size = packed_a.size();
//        size_t max_vector = (derived.size() / packed_size) * packed_size;
//
//        if (this->properties.isEnabled(Properties::contiguous_memory) &&
//            vector.properties.isEnabled(OtherDerived::Properties::contiguous_memory)) {
//
//            for (; i < max_vector; i += packed_size) {
//
//                packed_a.loadUnaligned(&derived[i]);
//                packed_b.loadUnaligned(&vector[i]);
//                packed_a = packed_b;
//                packed_a.storeUnaligned(&derived[i]);
//
//            }
//        }
//    }
//
//#endif
//
//    for (; i < derived.size(); ++i) {
//        derived[i] = vector[i];
//    }
//}

//template<typename Derived>
//auto VectorBase<Derived>::derived() -> Derived &
//{
//    return *static_cast<Derived *>(this);
//}
//
//template<typename Derived>
//auto VectorBase<Derived>::derived() const -> const Derived &
//{
//    return *static_cast<const Derived *>(this);
//}

template<MatrixExpr LHS, VectorExpr RHS>
auto operator*(const LHS &mat, const RHS &vec)
{
    return MatVecMulExpr<LHS, RHS>(mat, vec);
}

template<VectorExpr RHS, MatrixExpr LHS>
auto operator*(const RHS &vec, const LHS &mat)
{
    return MatVecMulExpr<LHS, RHS>(mat, vec);
}


template<typename DerivedL, typename DerivedR>
auto operator==(const VectorBase<DerivedL> &lhs,
                const VectorBase<DerivedR> &rhs) -> bool
{
    static_assert(!vector_traits<DerivedL>::is_expression &&
        !vector_traits<DerivedR>::is_expression,
        "Vector expressions cannot be compared directly.");

    return detail::vector_equal(lhs.derived(), rhs.derived());
}

template<typename DerivedL, typename DerivedR>
auto operator!=(const VectorBase<DerivedL> &lhs,
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

