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

#ifndef TL_CORE_PATH_H
#define TL_CORE_PATH_H

#include "config_tl.h"

#include <iostream>
#include <fstream>
#include <map>
#include <regex>
#include <list>
#include <numeric>





#include "tidop/core/defs.h"

namespace tl
{

namespace internal
{
class Path;
}

/*!
 * \defgroup utilities Utilidades
 *
 * Utilidades de proposito general como manipulación de cadenas, información de la
 * aplicación etc
 * \{
 */

class TL_EXPORT Path
{

public:

  Path();
  Path(const std::string &path);
  //Path(const Path &path);
  //Path(Path &&path) TL_NOEXCEPT;
  ~Path();

  //Path &operator = (const Path &path);
  //Path &operator = (Path &&path) TL_NOEXCEPT;

//  /*!
//   * \brief Parsea una cadena obteniendo los directorios de la ruta, unidad de disco y nombre si es un fichero
//   * \param path Ruta
//   * 
//   */
//  void parse(const std::string &path);
//
//#if defined WIN32
//
//  /*!
//   * \brief getDrive
//   * \return
//   */
//  const char *getDrive();
//
//#endif
//
//  /*!
//   * \brief Sube una posición en el path
//   */
//  void up();
//
//  /*!
//   * \brief Baja una posición
//   */
//  void down();
//  
//  /*!
//   * \brief currentPath
//   * \return
//   */
//  std::vector<std::string> currentPath();

  /*!
   * \brief Convierte el path en una cadena
   * \return
   */
  std::string toString() const;
  
  std::string extension() const;
  bool isDirectory() const;
  bool isFile() const;
  bool empty() const;

//  
//  void createDir();
//  void deleteDir();
//
//  /*!
//   * \brief
//   */
//  std::list<std::string> files(const std::string &wildcard);  // Listar ficheros o directorios. Posibilidad de filtrar con comodines (tipo de archivo, solo directorios, etc)
//
//  std::list<std::string> dirs();
//
//  Path &append(const std::string &dir);

private:

  internal::Path *mPath;

};


/*! \} */ // end of utilities


} // End namespace tl

#endif // TL_CORE_PATH_H
