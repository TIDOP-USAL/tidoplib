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
//C++17
//http://en.cppreference.com/w/cpp/filesystem
#include <filesystem>
#else
//Boost
//http://www.boost.org/doc/libs/1_66_0/libs/filesystem/doc/index.htm
#include <boost/filesystem.hpp>
#endif

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

  Path()
    : mPath{}
  {
  }

  Path(const std::string &path)
    : mPath(path)
  {
  }

  ~Path()
  {
  }

  std::string toString() const
  {
    return mPath.string();
  }

  std::string extension() const
  {
    return mPath.extension().string();
  }

  bool Path::isDirectory() const
  {
    return fs::is_directory(mPath);
  }

  bool Path::isFile() const
  {
    return fs::is_regular_file(mPath);
  }

  bool Path::empty() const
  {
    return mPath.empty();
  }

private:

  fs::path mPath;

};

} // internal


Path::Path()
  : mPath(new internal::Path())
  //  mPos(0),
  //  mPath(0),
  //  mFileName(""),
  //  mFileExtension(""),
  //  bFile(false)
{
}

Path::Path(const std::string &path)
  : mPath(new internal::Path(path))
//  //: mPos(0),
//  //  mPath(0),
//  //  mFileName(""),
//  //  mFileExtension(""),
//  //  bFile(false)
{
}

//Path::Path(const Path &path)
//  //: mPos(path.mPos),
//  //  mPath(path.mPath),
//  //  mFileName(path.mFileName),
//  //  mFileExtension(path.mFileExtension),
//  //  bFile(path.bFile)
//{
//}

Path::~Path()
{
  if (mPath) {
    delete mPath;
    mPath = nullptr;
  }
}

//Path &Path::operator=(const Path &path)
//{
//  //mPos = path.mPos;
//  //mPath = path.mPath;
//  //mFileName = path.mFileName;
//  //mFileExtension = path.mFileExtension;
//  //bFile = path.bFile;
//  //return *this;
//}

//void Path::parse(const std::string &path)
//{
////  char name[TL_MAX_FNAME];
////  char drive[TL_MAX_DRIVE];
////  char dir[TL_MAX_DIR];
////  char ext[TL_MAX_EXT];
////#ifdef _MSC_VER
////  int r_err = _splitpath_s(path, drive, TL_MAX_DRIVE, dir, TL_MAX_DIR, name, TL_MAX_FNAME, ext, TL_MAX_EXT);
////
////#endif
//
//  // Se comprueba si es un fichero
//  //char name[TL_MAX_FNAME];
//  //if (getFileName(path.c_str(), name, TL_MAX_FNAME) == 0) {
//  //  mFileName = name;
//  //  bFile = true;
//  //}
//
//  // Extensi?n
//  //char ext[TL_MAX_EXT];
//  //if (getFileExtension(path.c_str(), ext, TL_MAX_EXT) == 0) {
//  //  mFileExtension = ext;
//  //}
//
//  //char drive[TL_MAX_DRIVE];
//  //if (getFileDrive(path.c_str(), drive, TL_MAX_DRIVE) == 0) {
//  //  mDrive = drive;
//  //}
//
//  //char dir[TL_MAX_DIR];
//  //if (getFileDir(path.c_str(), dir, TL_MAX_DIR) == 0) {
//  //
//  //}
//
//  split(path, mPath, "/\\");
//  mPos = static_cast<int>(mPath.size());
//  if (mPath.size() == 0) return;
//
//  // rutas relativas
//  if (mPath[0] == std::string("..")) {
//    char dir[TL_MAX_DIR];
//    getFileDriveDir(getRunfile(), dir, TL_MAX_DIR);
//    //std::string runFilePath = getRunfile();
//    Path runPath(dir);
//    int i = 0;
//    for (; mPath[i] == std::string(".."); i++) {
//      runPath.down();
//    }
//
//    std::vector<std::string> current = runPath.currentPath();
//    for (int j = i; j < mPath.size(); j++)
//      current.push_back(mPath[j]);
//    mPath = current;
//    mPos = static_cast<int>(mPath.size());
//  } else if (mPath[0] == std::string(".")) {
//    char dir[TL_MAX_DIR];
//    getFileDriveDir(getRunfile(), dir, TL_MAX_DIR);
//    Path runPath(dir);
//    std::vector<std::string> current = runPath.currentPath();
//    for (int j = 1; j < mPath.size(); j++)
//      current.push_back(mPath[j]);
//    mPath = current;
//    mPos = static_cast<int>(mPath.size());
//  }
//}

//#if defined WIN32
//const char *Path::getDrive()
//{
//  //TODO: Esto s?lo en Windows...
//  //return mPath[0].c_str();
//  return mDrive.c_str();
//}
//#endif

//void Path::up()
//{
//  if (mPos < mPath.size())
//    mPos++;
//}
//
//void Path::down()
//{
//  if (mPos != 0)
//    mPos--;
//}

//std::vector<std::string> Path::currentPath()
//{
//  std::vector<std::string> cur_path;
//  for (int i = 0; i < mPos; i++) {
//    cur_path.push_back(mPath[i]);
//  }
//  return cur_path;
//}


std::string Path::toString() const
{
  return mPath->toString();
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


} // End namespace tl

