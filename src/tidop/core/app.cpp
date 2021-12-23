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

#include "app.h"

#if defined __linux__ || defined __GNUC__
#include <unistd.h>
#endif

namespace tl
{

std::unique_ptr<App> App::sObjApp;
std::mutex App::mtx;

App::App()
{
  init();
}

App &App::instance()
{
  if (sObjApp == nullptr) {
    std::lock_guard<std::mutex> lck(App::mtx);
    if (sObjApp == nullptr) {
      sObjApp.reset(new App());
    }
  }
  return *sObjApp;
}

tl::Path App::path() const
{
  static char runfile[TL_MAX_PATH];

#ifdef WIN32
  ::GetModuleFileNameA(NULL, runfile, TL_MAX_PATH);
#else
  char szTmp[32];
  sprintf(runfile, "/proc/%d/exe", getpid());
  int len = readlink(szTmp, runfile, TL_MAX_PATH);
  if (len >= 0)
    runfile[len] = '\0';
#endif

  return tl::Path(runfile);
}

std::string App::version() const
{
  return std::string();
}

void App::init()
{
}

} // namespace tl
