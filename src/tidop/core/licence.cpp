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

namespace TL
{


Licence::Licence()
  : mProductName(),
    mText()
{

}

Licence::Licence(const std::string &productName, const std::string &text)
    : mProductName(productName),
      mText(text)
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


} // End namespace TL
