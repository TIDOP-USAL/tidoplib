/************************************************************************
 *                                                                      *
 * Copyright (C) 2020 by Tidop Research Group                           *
 *                                                                      *
 * This file is part of TidopLib                                        *
 *                                                                      *
 * TidopLib is free software: you can redistribute it and/or modify     *
 * it under the terms of the GNU General Public License as published by *
 * the Free Software Foundation, either version 3 of the License, or    *
 * (at your option) any later version.                                  *
 *                                                                      *
 * TidopLib is distributed in the hope that it will be useful,          *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.      *
 *                                                                      *
 * @license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>                *
 *                                                                      *
 ************************************************************************/

#include "licence.h"

namespace tl
{


Licence::Licence()
  : mProductName(),
    mType()
{
}

Licence::Licence(const std::string &productName, 
                 const std::string &type)
  : mProductName(productName),
    mType(type)
{
    
}

bool Licence::empty() const
{
  return mProductName.empty() && mType.empty();
}

std::string Licence::productName() const
{
  return mProductName;
}

void Licence::setProductName(const std::string &productName)
{
  mProductName = productName;
}
std::string Licence::type() const
{
  return mType;
}

void Licence::setType(const std::string &text)
{
  mType = text;
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

const std::list<Licence> *AppLicence::thirdPartyLicences() const
{
  return &mThirdPartyLicences;
}


} // End namespace tl
