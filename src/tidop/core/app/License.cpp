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

#include "tidop/core/app/License.h"

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

auto License::productName() const noexcept -> std::string
{
    return mProductName;
}

void License::setProductName(std::string productName) noexcept
{
    mProductName = std::move(productName);
}

auto License::licenseName() const noexcept -> std::string
{
    return mLicense;
}

void License::setLicenseName(std::string license) noexcept
{
    mLicense = std::move(license);
}

auto License::licenseText() const noexcept -> std::string
{
    return mLicenseText;
}

void License::setLicenseText(std::string text) noexcept
{
    mLicenseText = std::move(text);
}

auto License::version() const noexcept -> std::string
{
    return mVersion;
}

void License::setVersion(std::string version) noexcept
{
    mVersion = std::move(version);
}

auto License::author() const noexcept -> std::string
{
    return mAuthor;
}

void License::setAuthor(std::string author) noexcept
{
    mAuthor = std::move(author);
}

auto License::authorEmail() const noexcept -> std::string
{
    return mEmail;
}

void License::setAuthorEmail(std::string authorEmail) noexcept
{
    mEmail = std::move(authorEmail);
}

auto License::url() const noexcept -> std::string
{
    return mUrl;
}

void License::setUrl(std::string url) noexcept
{
    mUrl = std::move(url);
}

auto License::empty() const noexcept -> bool
{
    return mProductName.empty() && mLicenseText.empty();
}




AppLicense::AppLicense()
  : mThirdPartyLicenses(0)
{
}

auto AppLicense::begin() noexcept -> iterator
{
    return mThirdPartyLicenses.begin();
}

auto AppLicense::begin() const noexcept -> const_iterator
{
    return mThirdPartyLicenses.cbegin();
}

auto AppLicense::end() noexcept -> iterator
{
    return mThirdPartyLicenses.end();
}

auto AppLicense::end() const noexcept -> const_iterator
{
    return mThirdPartyLicenses.cend();
}

void AppLicense::push_back(const License &license)
{
    mThirdPartyLicenses.push_back(license);
}

void AppLicense::push_back(License &&license)
{
    mThirdPartyLicenses.push_back(std::move(license));
}

void AppLicense::clear() noexcept
{
    mThirdPartyLicenses.clear();
}

auto AppLicense::erase(const_iterator first,
                                 const_iterator last) -> iterator
{
    return mThirdPartyLicenses.erase(first, last);
}

auto AppLicense::empty() const noexcept -> bool
{
    return mThirdPartyLicenses.empty();
}

auto AppLicense::size() const noexcept -> size_t
{
    return mThirdPartyLicenses.size();
}



} // End namespace tl
