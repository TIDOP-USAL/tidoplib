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

#ifndef TL_CORE_UTILS_GDAL_H
#define TL_CORE_UTILS_GDAL_H

#include "config_tl.h"

#include "tidop/core/defs.h"

#include <memory>
#include <mutex>

namespace tl
{


#ifdef HAVE_GDAL

/*!
 * \brief Clase para registrar los drivers de GDAL
 *
 */
class TL_EXPORT RegisterGdal
{
private:

  static std::unique_ptr<RegisterGdal> sRegisterGdal;
  static std::mutex sMutex;

  /*!
   * \brief Constructor privado
   */
  RegisterGdal() {}

public:

  ~RegisterGdal() {}

  // Se impide la copia y asignación
  RegisterGdal(RegisterGdal const&) = delete;
  void operator=(RegisterGdal const&) = delete;

  /*!
   * \brief Método para iniciar GDAL una unica vez
   */
  static void init();

};

#endif // HAVE_GDAL


} // End namespace tl

#endif // TL_CORE_UTILS_GDAL_H
