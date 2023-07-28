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

#include "tidop/config.h"
#include "tidop/core/defs.h"

#include <string>
#include <list>


namespace tl
{

/*! \addtogroup core
 *  \{
 */

/*!
 * \brief Licencia asociada a una programa o librería
 */
class TL_EXPORT Licence
{

private:

    std::string mProductName;
    std::string mText;
    std::string mVersion;
    std::string mAutor;
    std::string mEmail;
    std::string mUrl;

public:

    Licence();
    Licence(std::string name,
            std::string text);

    auto productName() const -> std::string;
    auto setProductName(const std::string &productName) -> void;
    auto text() const -> std::string;
    auto setText(const std::string &text) -> void;
    auto version() const -> std::string;
    auto setVersion(const std::string &version) -> void;
    auto autor() const -> std::string;
    auto setAutor(const std::string &autor) -> void;
    auto autorEmail() const -> std::string;
    auto setAutorEmail(const std::string &autorEmail) -> void;
    auto url() const -> std::string;
    auto setUrl(const std::string &url) -> void;

    auto empty() const -> bool;

};


/*!
 * \brief Licencia de una aplicación
 *
 * Contiene la información de licencia del programa y las licencias
 * de terceros
 */
class TL_EXPORT AppLicence
    : public Licence
{

public:

    /*!
     * \brief Allocator
     */
    using allocator_type = std::list<Licence>::allocator_type;

    /*!
     * \brief value_type
     */
    using value_type = std::list<Licence>::value_type;

    /*!
     * \brief Tipo entero sin signo (por lo general size_t)
     */
    using size_type = std::list<Licence>::size_type;

    /*!
     * \brief Tipo entero con signo (por lo general ptrdiff_t)
     */
    using difference_type = std::list<Licence>::difference_type;

    /*!
     * \brief std::allocator_traits<Allocator>::pointer
     */
    using pointer = std::list<Licence>::pointer;

    /*!
     * \brief std::allocator_traits<Allocator>::const_pointer
     */
    using const_pointer = std::list<Licence>::const_pointer;

    /*!
     * \brief value_type&
     */
    using reference = std::list<Licence>::reference;

    /*!
     * \brief const value_type&
     */
    using const_reference = std::list<Licence>::const_reference;

    /*!
     * \brief Iterador de acceso aleatorio
     */
    using iterator = std::list<Licence>::iterator;

    /*!
     * \brief Iterador constante de acceso aleatorio
     */
    using const_iterator = std::list<Licence>::const_iterator;

private:

    std::list<Licence> mThirdPartyLicences;

public:

    AppLicence();

    auto begin() TL_NOEXCEPT -> iterator;
    auto begin() const TL_NOEXCEPT -> const_iterator;
    auto end() TL_NOEXCEPT -> iterator;
    auto end() const TL_NOEXCEPT -> const_iterator;
    auto push_back(const Licence &licence) -> void;
    auto push_back(Licence &&licence) TL_NOEXCEPT -> void;
    auto clear() TL_NOEXCEPT -> void;
    auto empty() const TL_NOEXCEPT -> bool;
    auto size() const TL_NOEXCEPT -> size_type;
    auto erase(const_iterator first, const_iterator last) -> iterator;

};

/*! \} */ // end of core

} // End namespace tl

