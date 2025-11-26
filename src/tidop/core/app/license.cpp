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

#include "tidop/core/app/license.h"

#include <utility>

namespace tl
{


License::License() = default;

License::License(std::string productName, 
                 std::string license)
  : mProductName(std::move(productName)),
    mLicense(std::move(license))
{
    
}

auto License::productName() const -> std::string
{
    return mProductName;
}

auto License::setProductName(const std::string &productName) -> void
{
    mProductName = productName;
}

auto License::licenseName() const -> std::string
{
    return mLicense;
}

void License::setLicenseName(const std::string &license)
{
    mLicense = license;
}

auto License::licenseText() const -> std::string
{
    return mLicenseText;
}

auto License::setLicenseText(const std::string &text) -> void
{
    mLicenseText = text;
}

auto License::version() const -> std::string
{
    return mVersion;
}

auto License::setVersion(const std::string &version) -> void
{
    mVersion = version;
}

auto License::author() const -> std::string
{
    return mAuthor;
}

void License::setAuthor(const std::string &author)
{
    mAuthor = author;
}

auto License::authorEmail() const -> std::string
{
    return mEmail;
}

auto License::setAuthorEmail(const std::string &authorEmail) -> void
{
    mEmail = authorEmail;
}

auto License::url() const -> std::string
{
    return mUrl;
}

auto License::setUrl(const std::string &url) -> void
{
    mUrl = url;
}

auto License::empty() const -> bool
{
    return mProductName.empty() && mLicenseText.empty();
}




AppLicense::AppLicense()
  : mThirdPartyLicenses(0)
{
}

auto AppLicense::begin() TL_NOEXCEPT -> iterator
{
    return mThirdPartyLicenses.begin();
}

auto AppLicense::begin() const TL_NOEXCEPT -> const_iterator
{
    return mThirdPartyLicenses.cbegin();
}

auto AppLicense::end() TL_NOEXCEPT -> iterator
{
    return mThirdPartyLicenses.end();
}

auto AppLicense::end() const TL_NOEXCEPT -> const_iterator
{
    return mThirdPartyLicenses.cend();
}

void AppLicense::push_back(const License &license)
{
    mThirdPartyLicenses.push_back(license);
}

void AppLicense::push_back(License &&license) TL_NOEXCEPT
{
    mThirdPartyLicenses.push_back(std::move(license));
}

void AppLicense::clear() TL_NOEXCEPT
{
    mThirdPartyLicenses.clear();
}

auto AppLicense::erase(const_iterator first,
                       const_iterator last) -> iterator
{
    return mThirdPartyLicenses.erase(first, last);
}

auto AppLicense::empty() const TL_NOEXCEPT -> bool
{
    return mThirdPartyLicenses.empty();
}

auto AppLicense::size() const TL_NOEXCEPT -> size_t
{
    return mThirdPartyLicenses.size();
}



} // End namespace tl
