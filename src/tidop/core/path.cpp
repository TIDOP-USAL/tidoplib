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

#include "path.h"

#include "tidop/core/messages.h"
#include "tidop/core/console.h"
#include "tidop/core/exception.h"

// filesystem
#if (__cplusplus >= 201703L)
#include <filesystem>
#else
#include <boost/filesystem.hpp>
#endif
#include <memory>

#if (__cplusplus >= 201703L)
namespace fs = std::filesystem;
#else
namespace fs = boost::filesystem;
#endif

namespace tl
{

namespace internal
{

class Path
{

public:

  Path() = default;

  explicit Path(const std::string &path)
    : mPath(path)
  {
    mPath = mPath.native();
  }

  std::string toString() const
  {
    return mPath.string();
  }

  std::string fileName() const
  {
    return mPath.filename().string();
  }

  std::string baseName() const
  {
    return mPath.stem().string();
  }

  std::string extension() const
  {
    return mPath.extension().string();
  }

  bool isDirectory() const
  {
    return fs::is_directory(mPath);
  }

  bool isFile() const
  {
    return fs::is_regular_file(mPath);
  }

  bool empty() const
  {
    return mPath.empty();
  }

  bool exists() const
  {
    return fs::exists(mPath);
  }

  std::string replaceExtension(const std::string &extension)
  {
    mPath.replace_extension(extension);
    return mPath.string();
  }

  std::string parent()
  {
    fs::path parent_path = mPath.parent_path();
    return parent_path.string();
  }

  void append(const std::string &text)
  {
    mPath.append(text);
  }

private:

  fs::path mPath;

};

} // internal


Path::Path()
  : mPath(new internal::Path())
{
}

Path::Path(const std::string &path)
  : mPath(new internal::Path(path))
{
}

Path::Path(const Path &path)
  : mPath(new internal::Path(*path.mPath))
{

}

Path::Path(Path &&path) TL_NOEXCEPT
  : mPath(std::move(path.mPath))
{
}

Path::~Path() = default;

Path &Path::operator=(const Path &path)
{
  mPath = std::make_unique<internal::Path>(*path.mPath);

  return *this;
}

Path &Path::operator=(Path &&path) TL_NOEXCEPT
{
  mPath = std::move(path.mPath);

  return *this;
}

void Path::setPath(const std::string &path)
{
  mPath = std::make_unique<internal::Path>(path);
}

std::string Path::toString() const
{
  return mPath->toString();
}

std::string Path::fileName() const
{
  return mPath->fileName();
}

std::string Path::baseName() const
{
  return mPath->baseName();
}

std::string Path::extension() const
{
  return mPath->extension();
}

bool Path::isDirectory() const
{
  return mPath->isDirectory();
}

bool Path::isFile() const
{
  return mPath->isFile();
}

bool Path::empty() const
{
  return mPath->empty();
}

bool Path::exists() const
{
  return mPath->exists();
}

Path &Path::replaceExtension(const std::string &extension)
{
  mPath->replaceExtension(extension);
  return *this;
}

Path Path::parentPath() const
{
  Path parent_path(mPath->parent());
  return parent_path;
}

Path &Path::append(const std::string &text)
{
  mPath->append(text);
  return *this;
}

void Path::clear()
{
  mPath = std::make_unique<internal::Path>("");
}

/* Static methods */

bool Path::exists(const std::string &path)
{
  return Path(path).exists();
}

bool Path::exists(const Path &path)
{
  return path.exists();
}

Path Path::tempPath()
{
  std::string temp = fs::temp_directory_path().string();
  return Path(temp);
}

Path Path::tempDirectory()
{
  //Path temp_dir = tempPath();
  std::string dir = std::tmpnam(nullptr);
  //temp_dir.append(dir);
  return Path(dir);
}

bool Path::createDirectory(const Path &directory)
{
  return fs::create_directory(directory.toString());
}

bool Path::createDirectory(const std::string &directory)
{
  return fs::create_directory(directory);
}

bool Path::createDirectories(const Path &directory)
{
  return fs::create_directories(directory.toString());
}

bool Path::createDirectories(const std::string &directory)
{
  return fs::create_directories(directory);
}

//Path tl::Path::tempFile()
//{
//  std::string name = std::tmpnam(nullptr);
//  return Path(name);
//}

//std::list<std::string> Path::files(const std::string &wildcard)
//{
//  std::list<std::string> files;
//  //fileList(toString().c_str(), &files, wildcard.c_str());
//  return files;
//}
//
//std::list<std::string> Path::dirs()
//{
//  std::list<std::string> dirs;
//  directoryList(toString().c_str(), &dirs);
//  return dirs;
//}
//

//
//void Path::createDir()
//{
//  tl::createDir(toString().c_str());
//}
//
//void Path::deleteDir()
//{
//  tl::deleteDir(toString().c_str());
//}

//Path &Path::append(const std::string &dir)
//{
//  //TODO: Si el path es de un fichero no se puede a?adir...
//  mPath.push_back(dir);
//  return *this;
//}





TemporalDir::TemporalDir(bool autoRemove)
  : bAutoRemove(autoRemove),
    mPath(Path::tempDirectory())
{
  Path::createDirectories(mPath);
}

TemporalDir::~TemporalDir()
{
  if (bAutoRemove && mPath.exists())
    fs::remove_all(mPath.toString());
}

Path TemporalDir::path() const
{
  return mPath;
}

} // End namespace tl

