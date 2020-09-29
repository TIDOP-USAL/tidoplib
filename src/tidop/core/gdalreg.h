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
