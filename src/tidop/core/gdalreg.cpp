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

#include "tidop/core/gdalreg.h"

#include "tidop/core/messages.h"
#include "tidop/core/console.h"
#include "tidop/core/exception.h"

#ifdef TL_HAVE_GDAL
TL_DISABLE_WARNINGS
#include "gdal.h"
TL_DEFAULT_WARNINGS
#endif // TL_HAVE_GDAL


namespace tl
{

#ifdef TL_HAVE_GDAL




void RegisterGdal::init()
{
  static std::once_flag init_flag;

  std::call_once(init_flag, []() {
    GDALAllRegister(); 
  });
}

#endif


} // End namespace tl

