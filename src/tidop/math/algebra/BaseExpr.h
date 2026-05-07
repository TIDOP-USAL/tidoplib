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

/*! \file BaseExpr.h
 * \brief Base class for expression templates in algebraic operations.
 *
 * This file defines the `BaseExpr` class, which serves as the foundation for
 * expression templates used in linear algebra operations. It provides the CRTP
 * (Curiously Recurring Template Pattern) infrastructure to allow derived expression
 * classes to access the underlying derived object and to perform polymorphic
 * behavior without virtual functions.
 *
 * \ingroup Algebra
 * \see tl::VectorBase, tl::VectorExpr
 */

#pragma once

namespace tl
{

/*! \addtogroup Algebra
 *  \{
 */

/*!
 * \class BaseExpr
 * \brief CRTP base class for expression templates.
 */
template<typename Derived>
class BaseExpr
{

public:

    using derived_type = Derived;

public:

    /*!
     * \brief Casts the base object to a mutable reference of the derived type.
     * \return Reference to the derived object.
     */
    constexpr auto derived() -> derived_type &
    {
        return static_cast<Derived &>(*this);
    }

    /*!
     * \brief Casts the base object to a const reference of the derived type.
     * \return Const reference to the derived object.
     */
    constexpr auto derived() const -> const derived_type &
    {
        return static_cast<const Derived &>(*this);
    }

    /*!
     * \brief Checks whether the expression aliases a given memory address.
     *
     * \param[in] ptr Pointer to a memory location.
     * \return `true` if the expression (or any of its components) uses data stored
     *         at address `ptr`; `false` otherwise.
     *
     * This method is primarily used to avoid aliasing issues during in‑place
     * operations (e.g., `v = v + w`). It delegates the check to the derived type.
     */
    auto aliases(const void *ptr) const -> bool 
    { 
        return derived().aliases(ptr);
    }

    /*!
     * \brief Evaluates the expression and returns a concrete vector.
     * \return The evaluated concrete vector (or a copy of a plain vector).
     *
     * This method triggers the evaluation of the expression template and
     * returns a fully computed `Vector` object. For plain vectors (already
     * concrete), it simply returns a copy.
     */
    constexpr auto eval() const
    {
        return derived().eval();
    }

};


/*! \} */

} // End namespace tl