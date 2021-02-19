/****************************************************************************
 *                                                                          *
 *  This file is part of TidopLib and can not be copied and/or distributed  *
 *  without the express permision of ITOS3D ENGINEERING S.L                 *
 *                                                                          *
 *  Contact: http://www.itos3d.com                                          *
 *           http://tidop.usal.es                                           *
 *                                                                          *
 *--------------------------------------------------------------------------*
 *                                                                          *
 *  Copyright (C) 2018, ITOS3D ENGINEERING S.L - All rights reserved        *
 *                                                                          *
 ****************************************************************************/

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
