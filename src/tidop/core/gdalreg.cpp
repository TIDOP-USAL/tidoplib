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

#include "tidop/core/gdalreg.h"

#include "tidop/core/messages.h"
#include "tidop/core/console.h"
#include "tidop/core/exception.h"

#ifdef HAVE_GDAL
TL_SUPPRESS_WARNINGS
#include "gdal.h"
TL_DEFAULT_WARNINGS
#endif // HAVE_GDAL


namespace tl
{

#ifdef HAVE_GDAL


std::unique_ptr<RegisterGdal> RegisterGdal::sRegisterGdal;
std::mutex RegisterGdal::sMutex;

void RegisterGdal::init()
{
  if (sRegisterGdal.get() == nullptr) {
    std::lock_guard<std::mutex> lck(RegisterGdal::sMutex);
    if (sRegisterGdal.get() == nullptr) {
      sRegisterGdal.reset(new RegisterGdal());
      GDALAllRegister();
    }
  }
}

#endif


} // End namespace tl

