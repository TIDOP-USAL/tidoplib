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

#pragma once

#include "tidop/config.h"

#include <memory>
#include <mutex>

#include "tidop/core/defs.h"
#include "tidop/core/path.h"

class GDALDriver;

namespace tl
{

/*! \addtogroup core
 *  \{
 */

#ifdef TL_HAVE_GDAL

/*!
 * \brief Class to register GDAL drivers
 *
 */
class TL_EXPORT RegisterGdal
{

private:

  RegisterGdal() = default;

public:

  ~RegisterGdal() = default;

  TL_DISABLE_COPY(RegisterGdal)
  TL_DISABLE_MOVE(RegisterGdal)

  static void init();

};

GDALDriver *gdalDriver(const tl::Path &filename);
bool driverAvailable(const tl::Path &filename);

#endif // TL_HAVE_GDAL

/*! \} */ // end of core

} // End namespace tl

