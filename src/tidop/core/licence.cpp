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
 * License along with Foobar. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#include "licence.h"

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

bool Licence::empty() const
{
  return mProductName.empty() && mText.empty();
}

std::string Licence::productName() const
{
  return mProductName;
}

void Licence::setProductName(const std::string &productName)
{
  mProductName = productName;
}
std::string Licence::text() const
{
  return mText;
}

void Licence::setText(const std::string &text)
{
  mText = text;
}

std::string Licence::version() const
{
  return mVersion;
}

void Licence::setVersion(const std::string &version)
{
  mVersion = version;
}

std::string Licence::autor() const
{
  return mAutor;
}

void Licence::setAutor(const std::string &autor)
{
  mAutor = autor;
}

std::string Licence::autorEmail() const
{
  return mEmail;
}

void Licence::setAutorEmail(const std::string &autorEmail)
{
  mEmail = autorEmail;
}

std::string Licence::url() const
{
  return mUrl;
}

void Licence::setUrl(const std::string &url)
{
  mUrl = url;
}



AppLicence::AppLicence()
  : mThirdPartyLicences(0)
{
}

AppLicence::iterator AppLicence::begin() TL_NOEXCEPT
{
  return mThirdPartyLicences.begin();
}

AppLicence::const_iterator AppLicence::begin() const TL_NOEXCEPT
{
  return mThirdPartyLicences.cbegin();
}

AppLicence::iterator AppLicence::end() TL_NOEXCEPT
{
  return mThirdPartyLicences.end();
}

AppLicence::const_iterator AppLicence::end() const TL_NOEXCEPT
{
  return mThirdPartyLicences.cend();
}

void AppLicence::push_back(const Licence &licence)
{
  mThirdPartyLicences.push_back(licence);
}

void AppLicence::push_back(Licence &&licence) TL_NOEXCEPT
{
  mThirdPartyLicences.push_back(std::forward<Licence>(licence));
}

void AppLicence::clear() TL_NOEXCEPT
{
  mThirdPartyLicences.clear();
}

bool AppLicence::empty() const TL_NOEXCEPT
{
  return mThirdPartyLicences.empty();
}

size_t AppLicence::size() const TL_NOEXCEPT
{
  return mThirdPartyLicences.size();
}

AppLicence::iterator AppLicence::erase(AppLicence::const_iterator first,
                                       AppLicence::const_iterator last)
{
  return mThirdPartyLicences.erase(first, last);
}

} // End namespace tl
