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

#ifndef TL_CORE_PATH_H
#define TL_CORE_PATH_H

#include "config_tl.h"

#include <regex>
#include <list>
#include <numeric>
#include <memory>


#include "tidop/core/defs.h"


namespace tl
{

namespace internal
{
class Path;
}

/*! \addtogroup core
 *  \{
 */

class TL_EXPORT Path
{

public:

  Path();
  Path(const std::string &path);
  Path(const std::wstring &path);
  Path(const Path &path);
  Path(Path &&path) TL_NOEXCEPT;
  ~Path();

  Path &operator = (const Path &path);
  Path &operator = (Path &&path) TL_NOEXCEPT;

  void setPath(const std::string &path);
  void setPath(const std::wstring &path);

  /*!
   * \brief Convierte el path en una cadena
   * \return
   */
  std::string toString() const;
  std::wstring toWString() const;

  Path fileName() const;
  Path baseName() const;
  Path extension() const;
  Path parentPath() const;
  std::list<Path> list(const std::string &extension);
  std::list<Path> list(const std::regex &filter);

  bool isDirectory() const;
  bool isFile() const;
  bool empty() const;
  bool exists() const;

  Path &replaceFileName(const std::string &fileName);
  Path &replaceFileName(const std::wstring &fileName);
  Path &replaceFileName(const Path &fileName);
  Path &replaceBaseName(const std::string &baseName);
  Path &replaceBaseName(const std::wstring &baseName);
  Path &replaceBaseName(const Path &baseName);
  Path &replaceExtension(const std::string &extension);
  Path &replaceExtension(const std::wstring &extension);
  Path &replaceExtension(const Path &extension);
  Path &append(const std::string &text);
  Path &append(const std::wstring &text);
  Path &append(const Path&text);

  int compare(const Path &path) const;
  bool equivalent(const Path &path) const;
  bool createDirectory() const;
  bool createDirectories() const;
  void removeDirectory() const;

  void clear();

/* Static methods */

  static bool exists(const Path &path);
  static Path tempPath();
  static Path tempDirectory();
  static bool createDirectory(const Path &directory);
  static bool createDirectory(const std::string &directory);
  static bool createDirectory(const std::wstring &directory);
  static bool createDirectories(const Path &directory);
  static bool createDirectories(const std::string &directory);
  static bool createDirectories(const std::wstring &directory);
  static void removeDirectory(const Path &directory);
  static void removeDirectory(const std::string &directory);
  static void removeDirectory(const std::wstring &directory);
  static void removeFile(const Path &file);
  static size_t hash(const Path &path);

  /* Override operators */

  bool operator == (const Path &path) const;
  bool operator != (const Path &path) const;

private:

  std::unique_ptr<internal::Path> mPath;

};


/*!
 * /brief Crea un directorio temporal que por defecto se borra automaticamente
 * 
 */
class TL_EXPORT TemporalDir
{

public:

  /*!
   * \brief
   * \param[in] autoRemove 
   */
  explicit TemporalDir(bool autoRemove = true);
  ~TemporalDir();

  TemporalDir(const TemporalDir &) = delete;
  TemporalDir(TemporalDir &&) = delete;
  TemporalDir &operator=(const TemporalDir &) = delete;
  TemporalDir &operator=(TemporalDir &&) = delete;

  Path path() const;

private:

  bool bAutoRemove;
  Path mPath;

};


/* Override operators */

TL_EXPORT std::ostream &operator<< (std::ostream &os, const Path &path);

/*! \} */ // end of core

} // End namespace tl



#endif // TL_CORE_PATH_H
