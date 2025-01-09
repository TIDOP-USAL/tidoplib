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

#include "tidop/core/app/licence.h"

#include <utility>

namespace tl
{


Licence::Licence() = default;

Licence::Licence(std::string productName, 
                 std::string text)
  : mProductName(std::move(productName)),
    mText(std::move(text))
{
    
}

auto Licence::productName() const -> std::string
{
    return mProductName;
}

auto Licence::setProductName(const std::string &productName) -> void
{
    mProductName = productName;
}

auto Licence::text() const -> std::string
{
    return mText;
}

auto Licence::setText(const std::string &text) -> void
{
    mText = text;
}

auto Licence::version() const -> std::string
{
    return mVersion;
}

auto Licence::setVersion(const std::string &version) -> void
{
    mVersion = version;
}

auto Licence::autor() const -> std::string
{
    return mAutor;
}

auto Licence::setAutor(const std::string &autor) -> void
{
    mAutor = autor;
}

auto Licence::autorEmail() const -> std::string
{
    return mEmail;
}

auto Licence::setAutorEmail(const std::string &autorEmail) -> void
{
    mEmail = autorEmail;
}

auto Licence::url() const -> std::string
{
    return mUrl;
}

auto Licence::setUrl(const std::string &url) -> void
{
    mUrl = url;
}

auto Licence::empty() const -> bool
{
    return mProductName.empty() && mText.empty();
}




AppLicence::AppLicence()
  : mThirdPartyLicences(0)
{
}

auto AppLicence::begin() TL_NOEXCEPT -> iterator
{
    return mThirdPartyLicences.begin();
}

auto AppLicence::begin() const TL_NOEXCEPT -> const_iterator
{
    return mThirdPartyLicences.cbegin();
}

auto AppLicence::end() TL_NOEXCEPT -> iterator
{
    return mThirdPartyLicences.end();
}

auto AppLicence::end() const TL_NOEXCEPT -> const_iterator
{
    return mThirdPartyLicences.cend();
}

auto AppLicence::push_back(const Licence &licence) -> void
{
    mThirdPartyLicences.push_back(licence);
}

auto AppLicence::push_back(Licence &&licence) TL_NOEXCEPT -> void
{
    mThirdPartyLicences.push_back(std::forward<Licence>(licence));
}

auto AppLicence::clear() TL_NOEXCEPT -> void
{
    mThirdPartyLicences.clear();
}

auto AppLicence::empty() const TL_NOEXCEPT -> bool
{
    return mThirdPartyLicences.empty();
}

auto AppLicence::size() const TL_NOEXCEPT -> size_t
{
    return mThirdPartyLicences.size();
}

auto AppLicence::erase(const_iterator first,
                       const_iterator last) -> iterator
{
    return mThirdPartyLicences.erase(first, last);
}

} // End namespace tl
