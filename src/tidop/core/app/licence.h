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
#include "tidop/core/base/defs.h"

#include <string>
#include <list>


namespace tl
{

/*! \addtogroup AppManagement
 *  \{
 */

/*!
 * \brief Licensing information for a product or library.
 * 
 * This class stores details about the license associated with the application or library,
 * including its name, text, version, author, email, and a URL.
 * 
 * ### Example Usage
 * \code{.cpp}
 * Licence licence("TidopLib", "GNU Lesser General Public License v3.0");
 * licence.setUrl("https://github.com/TIDOP-USAL/tidoplib");
 * licence.setAuthor("TIDOP Research Group");
 * licence.setAuthorEmail("email@email.com");
 * licence.setVersion("4.0.0");
 * \endcode
 */
class TL_EXPORT Licence
{

private:

    std::string mProductName; /*!< The name of the product or library. */
    std::string mLicense;     /*!< The license name. */
    std::string mLicenseText; /*!< The license text. */
    std::string mVersion;     /*!< The license version. */
    std::string mAutor;       /*!< The author of the license. */
    std::string mEmail;       /*!< The email of the author. */
    std::string mUrl;         /*!< A URL associated with the license. */

public:

    /*!
     * \brief Default constructor.
     */
    Licence();

    /*!
     * \brief Constructor with product name and license name.
     * \param[in] name The product name.
     * \param[in] license The license name.
     */
    Licence(std::string name, std::string license);

    /*!
     * \brief Get the product name.
     * \return The product name.
     */
    auto productName() const -> std::string;

    /*!
     * \brief Set the product name.
     * \param productName The product name to set.
     */
    auto setProductName(const std::string &productName) -> void;

    /*!
     * \brief Get the license name.
     * \return The license name.
     */
    auto licenseName() const -> std::string;

    /*!
     * \brief Set the license name.
     * \param[in] license The license name to set.
     */
    void setLicence(const std::string &license);

    /*!
     * \brief Get the license text.
     * \return The license text.
     */
    auto licenseText() const -> std::string;

    /*!
     * \brief Set the license text.
     * \param[in] text The license text to set.
     */
    auto setLicenseText(const std::string &text) -> void;

    /*!
     * \brief Get the license version.
     * \return The version as a string.
     */
    auto version() const->std::string;

    /*!
     * \brief Set the license version.
     * \param[in] version The version to set.
     */
    auto setVersion(const std::string &version) -> void;

    /*!
     * \brief Get the author of the license.
     * \return The author's name.
     */
    auto autor() const->std::string;

    /*!
     * \brief Set the author of the license.
     * \param[in] autor The author's name to set.
     */
    auto setAuthor(const std::string &autor) -> void;

    /*!
     * \brief Get the author's email.
     * \return The author's email.
     */
    auto authorEmail() const->std::string;

    /*!
     * \brief Set the author's email.
     * \param[in] authorEmail The email to set.
     */
    auto setAuthorEmail(const std::string &authorEmail) -> void;

    /*!
     * \brief Get the license URL.
     * \return The URL as a string.
     */
    auto url() const->std::string;

    /*!
     * \brief Set the license URL.
     * \param[in] url The URL to set.
     */
    auto setUrl(const std::string &url) -> void;

    /*!
     * \brief Check if the license object is empty.
     * \return True if empty, false otherwise.
     */
    auto empty() const -> bool;

};


/*!
 * \brief Application license management.
 *
 * This class extends \ref Licence and manages additional licenses for third-party dependencies.
 */
class TL_EXPORT AppLicence
    : public Licence
{

public:


    using allocator_type = std::list<Licence>::allocator_type;
    using value_type = std::list<Licence>::value_type;
    using size_type = std::list<Licence>::size_type;
    using difference_type = std::list<Licence>::difference_type;
    using pointer = std::list<Licence>::pointer;
    using const_pointer = std::list<Licence>::const_pointer;
    using reference = std::list<Licence>::reference;
    using const_reference = std::list<Licence>::const_reference;
    using iterator = std::list<Licence>::iterator;
    using const_iterator = std::list<Licence>::const_iterator;

private:

    std::list<Licence> mThirdPartyLicences;

public:

    /*!
     * \brief Default constructor.
     */
    AppLicence();

    /*!
     * \brief Get an iterator to the beginning of the list.
     * \return An iterator to the beginning.
     */
    auto begin() TL_NOEXCEPT->iterator;

    /*!
     * \brief Get a const iterator to the beginning of the list.
     * \return A const iterator to the beginning.
     */
    auto begin() const TL_NOEXCEPT->const_iterator;

    /*!
     * \brief Get an iterator to the end of the list.
     * \return An iterator to the end.
     */
    auto end() TL_NOEXCEPT->iterator;

    /*!
     * \brief Get a const iterator to the end of the list.
     * \return A const iterator to the end.
     */
    auto end() const TL_NOEXCEPT->const_iterator;

    /*!
     * \brief Add a license to the list.
     * \param[in] licence The license to add.
     */
    auto push_back(const Licence &licence) -> void;

    /*!
     * \brief Add a license to the list (move version).
     * \param[in] licence The license to add.
     */
    auto push_back(Licence &&licence) TL_NOEXCEPT -> void;

    /*!
     * \brief Clear all licenses from the list.
     */
    auto clear() TL_NOEXCEPT -> void;

    /*!
     * \brief Check if the license list is empty.
     * \return True if empty, false otherwise.
     */
    auto empty() const TL_NOEXCEPT -> bool;

    /*!
     * \brief Get the size of the license list.
     * \return The number of licenses in the list.
     */
    auto size() const TL_NOEXCEPT->size_type;

    /*!
     * \brief Erase licenses from the list.
     * \param[in] first Iterator to the first element to erase.
     * \param[in] last Iterator to the last element to erase.
     * \return An iterator to the element following the last erased element.
     */
    auto erase(const_iterator first, const_iterator last) -> iterator;

};

/*! \} */

} // End namespace tl

