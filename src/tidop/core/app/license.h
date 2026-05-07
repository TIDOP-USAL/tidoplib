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

/*!
 * \file license.h
 * \brief License information management for products and dependencies
 *
 * This module provides comprehensive license management for applications and their dependencies.
 * It enables tracking and documentation of licensing information across an entire software stack.
 *
 * ### Classes
 *
 * - \ref tl::License - Stores license information for a single product or library
 * - \ref tl::AppLicense - Container for application license plus third-party dependencies
 *
 * ### Features
 *
 * - Store detailed license metadata (product name, license type, version, author, contact info)
 * - Manage multiple third-party licenses in a centralized location
 * - STL container compatibility for easy iteration and manipulation
 * - Support for both copy and move semantics for efficient memory usage
 *
 * \see tl::License, tl::AppLicense
 */
 
#pragma once

#include "tidop/config.h"

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
 * Stores comprehensive details about a software license including product name,
 * license type, text, version, author information, and reference URLs.
 * 
 * ### Fields
 *
 * - **Product Name** - The name of the product or library
 * - **%License Name** - The type of license (e.g., "GNU LGPL 3.0")
 * - **%License Text** - Full license text or summary
 * - **Version** - %License version
 * - **Author** - Author/copyright holder name
 * - **Author Email** - Contact email
 * - **URL** - Reference URL (project homepage, license page, etc.)
 *
 * ### Example Usage
 *
 * \code{.cpp}
 * %License tidoplib("TidopLib", "GNU Lesser General Public License v3.0");
 * tidoplib.setAuthor("TIDOP Research Group");
 * tidoplib.setAuthorEmail("tidop@example.com");
 * tidoplib.setVersion("4.0.0");
 * tidoplib.setUrl("https://github.com/TIDOP-USAL/tidoplib");
 * \endcode
 */
class TL_EXPORT License
{

private:

    std::string mProductName; /*!< The name of the product or library. */
    std::string mLicense;     /*!< The license name. */
    std::string mLicenseText; /*!< The license text. */
    std::string mVersion;     /*!< The license version. */
    std::string mAuthor;      /*!< The author of the license. */
    std::string mEmail;       /*!< The email of the author. */
    std::string mUrl;         /*!< A URL associated with the license. */

public:

    /*!
     * \brief Default constructor.
     *
     * Creates an empty license with all fields uninitialized.
     */
    License();

    /*!
     * \brief Constructor with product name and license type.
     *
     * Creates a license with the essential information. Other fields can be
     * set using the respective setter methods.
     *
     * \param[in] productName The product or library name
     * \param[in] license The license name or type (e.g., "GPL-3.0", "MIT")
     *
     * \note Parameters are passed by value and moved into members for efficiency.
     *       This allows the constructor to be called with temporaries:
     *
     * \code{.cpp}
     * License license("MyLib", "MIT");  // Temporary strings, efficiently moved
     * \endcode
     */
    License(std::string name, std::string license);

    /*!
     * \brief Get the product name.
     * \return The product or library name
     */
    auto productName() const -> std::string;

    /*!
     * \brief Set the product name.
     * \param productName The product or library name to set
     */
    void setProductName(const std::string &productName);

    /*!
     * \brief Get the license name or type.
     * \return The license identifier (e.g., "GPL-3.0", "MIT", "Apache-2.0")
     */
    auto licenseName() const -> std::string;

    /*!
     * \brief Set the license name or type.
     * \param[in] license The license identifier to set
     */
    void setLicenseName(const std::string &license);

    /*!
     * \brief Get the full license text.
     * \return The license text or summary
     */
    auto licenseText() const -> std::string;

    /*!
     * \brief Set the full license text.
     * \param[in] text The license text or summary to set
     */
    auto setLicenseText(const std::string &text) -> void;

    /*!
     * \brief Get the license version.
     * \return The version string (e.g., "3.0", "v2.1")
     */
    auto version() const->std::string;

    /*!
     * \brief Set the license version.
     * \param[in] version The version string to set
     */
    auto setVersion(const std::string &version) -> void;

    /*!
     * \brief Get the author or copyright holder name.
     * \return The author's name or organization
     */
    auto author() const -> std::string;

    /*!
     * \brief Set the author or copyright holder name.
     * \param[in] author The author's name or organization to set
     */
    void setAuthor(const std::string &author);

    /*!
     * \brief Get the author's email address.
     * \return The contact email
     */
    auto authorEmail() const->std::string;

    /*!
     * \brief Set the author's email.
     * \param[in] authorEmail The email to set.
     */
    auto setAuthorEmail(const std::string &authorEmail) -> void;

    /*!
     * \brief Get the reference URL.
     * \return URL to the project homepage, repository, or license page
     */
    auto url() const->std::string;

    /*!
     * \brief Set the reference URL.
     * \param[in] url The URL to set
     */
    auto setUrl(const std::string &url) -> void;

    /*!
     * \brief Check if the license is empty.
     *
     * A license is considered empty if its product name is not set.
     * This is the primary identifier for a license.
     *
     * \return True if the license is empty (no product name), false otherwise
     */
    auto empty() const -> bool;

};


/*!
 * \brief Application license information with third-party dependencies.
 *
 * Extends \ref License to include management of licenses for third-party
 * libraries and dependencies used by the application.
 *
 * This class acts as a container for the application's own license plus
 * a collection of licenses from included libraries. It delegates collection
 * operations to an internal `std::list<License>`.
 *
 * ### Typedefs
 *
 * Standard container typedefs are provided for compatibility with STL algorithms:
 * - \ref iterator, const_iterator
 * - \ref value_type, reference, const_reference
 * - \ref size_type, difference_type
 * - \ref allocator_type
 *
 * ### Iterator Support
 *
 * The class provides begin/end iterators for use with range-based for loops
 * and standard algorithms.
 * 
 * ### Example Usage
 *
 * \code{.cpp}
 * AppLicense app_license("MyApp", "GPL-3.0");
 * app_license.setAuthor("My Organization");
 *
 * // Add third-party licenses
 * License opencv("OpenCV", "Apache-2.0");
 * opencv.setUrl("https://opencv.org");
 * app_license.push_back(opencv);
 *
 * License eigen("Eigen", "MPL2");
 * app_license.push_back(std::move(eigen));
 *
 * // Iterate over third-party licenses
 * for (const auto& license : app_license) {
 *     std::cout << license.productName() << ": " << license.licenseName() << '\n';
 * }
 * \endcode
 *
 * \see License
 */
class TL_EXPORT AppLicense
    : public License
{

public:


    using allocator_type = std::list<License>::allocator_type;
    using value_type = std::list<License>::value_type;
    using size_type = std::list<License>::size_type;
    using difference_type = std::list<License>::difference_type;
    using pointer = std::list<License>::pointer;
    using const_pointer = std::list<License>::const_pointer;
    using reference = std::list<License>::reference;
    using const_reference = std::list<License>::const_reference;
    using iterator = std::list<License>::iterator;
    using const_iterator = std::list<License>::const_iterator;

private:

    std::list<License> mThirdPartyLicenses;   /*!< List of third-party licenses */

public:

    /*!
     * \brief Default constructor.
     *
     * Creates an AppLicense with no licenses (empty list).
     * The application's own license can be set using inherited License methods.
     */
    AppLicense();

    /*!
     * \brief Get an iterator to the beginning of the license list.
     * \return Iterator to the first license, or end() if empty
     */
    auto begin() TL_NOEXCEPT -> iterator;

    /*!
     * \brief Get a const iterator to the beginning of the license list.
     * \return Const iterator to the first license, or end() if empty
     */
    auto begin() const TL_NOEXCEPT -> const_iterator;

    /*!
     * \brief Get an iterator to the end of the license list.
     * \return Iterator to one past the last license
     */
    auto end() TL_NOEXCEPT -> iterator;

    /*!
     * \brief Get a const iterator to the end of the license list.
     * \return Const iterator to one past the last license
     */
    auto end() const TL_NOEXCEPT -> const_iterator;

    /*!
     * \brief Add a license to the list.
     *
     * Appends a copy of the given license to the end of the list.
     * Use the move version for better performance with temporary licenses.
     *
     * \param[in] license The license to add
     *
     * \see push_back(License&&)
     */
    void push_back(const License &license);

    /*!
     * \brief Add a license to the list (move).
     *
     * Appends the given license to the end of the list using move semantics.
     * Prefer this for temporary licenses to avoid unnecessary copying.
     *
     * \param[in] license The license to add (will be moved)
     *
     * ### Example
     * \code{.cpp}
     * app_license.push_back(License("OpenCV", "Apache-2.0"));  // Uses move
     * \endcode
     *
     * \see push_back(const License&)
     */
    void push_back(License &&license) TL_NOEXCEPT;

    /*!
     * \brief Remove all licenses from the list.
     *
     * Clears all third-party licenses. The application's own license
     * (from License base class) remains unchanged.
     */
    void clear() TL_NOEXCEPT;

    /*!
     * \brief Erase a range of licenses from the list.
     *
     * Removes all licenses in the range [first, last).
     *
     * \param[in] first Iterator to the first element to erase
     * \param[in] last Iterator to one past the last element to erase
     * \return Iterator to the element following the last erased element
     */
    auto erase(const_iterator first, const_iterator last) -> iterator;
	
    /*!
     * \brief Check if the third-party license list is empty.
     *
     * \return True if no third-party licenses are registered, false otherwise
     *
     * \note This checks only the third-party list, not the application's
     *       own license (from License base class). Use License::empty()
     *       to check the application license.
     */
    auto empty() const TL_NOEXCEPT -> bool;

    /*!
     * \brief Get the number of third-party licenses.
     *
     * \return The number of licenses in the list
     */
    auto size() const TL_NOEXCEPT -> size_type;


};

/*! \} */

} // End namespace tl

