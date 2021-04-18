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
                 std::string type)
  : mProductName(std::move(productName)),
    mType(std::move(type))
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

void Licence::setType(const std::string &type)
{
  mType = type;
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
