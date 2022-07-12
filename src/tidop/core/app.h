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

#ifndef TL_CORE_APP_H
#define TL_CORE_APP_H

#include "config_tl.h"

#include <string>
#include <memory>
#include <mutex>

#include "tidop/core/defs.h"
#include "tidop/core/path.h"

namespace tl
{

/*! \addtogroup core
 *  \{
 */

/*!
 * \brief Información de la aplicación 
 */
class TL_EXPORT App
{

private:

  App();

public:

  ~App() = default;

  TL_DISABLE_COPY(App)
  TL_DISABLE_MOVE(App)

  /*!
   * \brief Singleton
   */
  static App &instance();

  tl::Path path() const;
  std::string version() const;

private:

  void init();

private:

  static std::unique_ptr<App> sObjApp;
  static std::mutex mtx;
  static std::once_flag sInitFlag;
};

/*! \} */ // end of core

} // namespace tl

#endif // TL_CORE_EVENT_H
