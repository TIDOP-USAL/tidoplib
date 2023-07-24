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

#include "tidop/core/path.h"

#include "tidop/core/console.h"
#include "tidop/core/exception.h"

// filesystem
#if (CPP_VERSION >= 17)
#include <filesystem>
#include <functional> 
#else
#include <boost/filesystem.hpp>
#include <boost/functional/hash.hpp> 
#endif

#include <ostream>


#if (CPP_VERSION >= 17)
namespace fs = std::filesystem;
#else
namespace fs = boost::filesystem;
#endif

namespace tl
{

/// \cond

namespace internal
{

class Path
{

public:

    Path() = default;

    explicit Path(const fs::path &path)
        : mPath(path.native())
    {
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

    const fs::path &ref() const
    {
        return mPath;
    }

    fs::path &ref()
    {
        return mPath;
    }

private:

    fs::path mPath;

};

} // namespace internal

/// \endcond



Path::Path()
  : mPath(new internal::Path())
{
}

Path::Path(const std::string &path)
  : mPath(new internal::Path(path))
{
}

Path::Path(const std::wstring &path)
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

void Path::setPath(const std::wstring &path)
{
    mPath = std::make_unique<internal::Path>(path);
}

std::string Path::toString() const
{
    return mPath->ref().string();
}

std::wstring Path::toWString() const
{
    return mPath->ref().wstring();
}

Path Path::fileName() const
{
    return Path(mPath->ref().filename().native());
}

Path Path::baseName() const
{
    return Path(mPath->ref().stem().native());
}

Path Path::extension() const
{
    return Path(mPath->ref().extension().native());
}

Path Path::parentPath() const
{
    Path parent_path(mPath->ref().parent_path().native());
    return parent_path;
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

std::list<Path> Path::list(const std::string &extension)
{
    std::list<Path> list;

    fs::directory_iterator it_end;

    for (fs::directory_iterator it(mPath->ref()); it != it_end; ++it) {

        if (!fs::is_regular_file(it->status())) continue;

        fs::path _path = it->path();

        std::string extension_found = it->path().extension().string();

        if (compareInsensitiveCase(extension_found, extension)) {

            list.emplace_back(it->path().filename().string());
        }
    }

    return list;
}

std::list<Path> Path::list(const std::regex &filter)
{
    std::list<Path> list;

    fs::directory_iterator it_end;

    for (fs::directory_iterator it(mPath->ref()); it != it_end; ++it) {


        if (!fs::is_regular_file(it->status())) continue;

        std::smatch what;

        std::string fname = it->path().filename().string();

        if (!std::regex_match(fname, what, filter)) continue;

        list.emplace_back(fname);
    }

    return list;
}

Path &Path::replaceFileName(const std::string &fileName)
{
    fs::path &_path = mPath->ref();

    if (_path.has_filename()) {
        _path.remove_filename();
        _path.append(fileName);
    }

    return *this;
}

Path &Path::replaceFileName(const std::wstring &fileName)
{
    fs::path &_path = mPath->ref();

    if (_path.has_filename()) {
        _path.remove_filename();
        _path.append(fileName);
    }

    return *this;
}

Path &Path::replaceFileName(const Path &fileName)
{
    return replaceFileName(fileName.toWString());
}

Path &Path::replaceBaseName(const std::string &baseName)
{
    fs::path &_path = mPath->ref();

    if (_path.has_filename()) {
        std::string ext = _path.extension().string();
        std::string file_name = baseName + ext;
        _path.remove_filename();
        _path.append(file_name);
    }

    return *this;
}

Path &Path::replaceBaseName(const std::wstring &baseName)
{
    fs::path &_path = mPath->ref();

    if (_path.has_filename()) {
        std::wstring ext = _path.extension().wstring();
        std::wstring file_name = baseName + ext;
        _path.remove_filename();
        _path.append(file_name);
    }

    return *this;
}

Path &Path::replaceBaseName(const Path &baseName)
{
    return replaceBaseName(baseName.toWString());
}

Path &Path::replaceExtension(const std::string &extension)
{
    mPath->ref().replace_extension(extension);
    return *this;
}

Path &Path::replaceExtension(const std::wstring &extension)
{
    mPath->ref().replace_extension(extension);
    return *this;
}

Path &Path::replaceExtension(const Path &extension)
{
    return replaceExtension(extension.toString());
}

Path &Path::append(const std::string &text)
{
    mPath->ref().append(text);
    return *this;
}

Path &Path::append(const std::wstring &text)
{
    mPath->ref().append(text);
    return *this;
}

Path &Path::append(const Path &text)
{
    return append(text.toWString());
}

bool Path::createDirectory() const
{
    return fs::create_directory(mPath->ref());
}

bool Path::createDirectories() const
{
    return fs::create_directories(mPath->ref());
}

void Path::removeDirectory() const
{
    fs::remove_all(mPath->ref());
}

void tl::Path::normalize()
{
    mPath->ref().make_preferred();
}

void Path::clear()
{
    mPath = std::make_unique<internal::Path>("");
}

/* Static methods */

bool Path::exists(const Path &path)
{
    return path.exists();
}

Path Path::tempPath()
{
    Path temp_path(fs::temp_directory_path().string());
    return temp_path;
}

Path Path::tempDirectory()
{
    std::string temp_path;

#ifdef WIN32
    temp_path = std::tmpnam(nullptr);
#else
    temp_path = fs::temp_directory_path().string();
    temp_path.append("/tlXXXXXX");
    std::vector<char> c_path(temp_path.begin(), temp_path.end());
    c_path.push_back('\0');
    int file_descriptor = mkstemp(c_path.data());
    if (file_descriptor != -1) {
        temp_path.assign(c_path.begin(), c_path.end() - 1);
    } else {
        temp_path = "";
        Message::warning("Failed to create temporary directory");
    }
#endif

    return Path(temp_path);
}

int Path::compare(const Path &path) const
{
    return mPath->ref().compare(path.toString());
}

bool tl::Path::equivalent(const Path &path) const
{
    return fs::equivalent(mPath->ref(), path.toWString());
}

bool Path::createDirectory(const Path &directory)
{
    return fs::create_directory(directory.toWString());
}

bool Path::createDirectory(const std::string &directory)
{
    return fs::create_directory(directory);
}

bool Path::createDirectory(const std::wstring &directory)
{
    return fs::create_directory(directory);
}

bool Path::createDirectories(const Path &directory)
{
    return fs::create_directories(directory.toWString());
}

bool Path::createDirectories(const std::string &directory)
{
    return fs::create_directories(directory);
}

bool Path::createDirectories(const std::wstring &directory)
{
    return fs::create_directories(directory);
}

void Path::removeDirectory(const Path &directory)
{
    fs::remove_all(directory.toWString());
}

void Path::removeDirectory(const std::string &directory)
{
    fs::remove_all(directory);
}

void Path::removeDirectory(const std::wstring &directory)
{
    fs::remove_all(directory);
}

void Path::removeFile(const Path &file)
{
    fs::remove(file.toWString());
}

size_t tl::Path::hash(const Path &path)
{
    return fs::hash_value(path.mPath->ref());
}

void tl::Path::copy(const Path &from, const Path &to)
{
    fs::copy(from.toWString(), to.toWString());
}

/* Override operators */

bool Path::operator==(const Path& path) const
{
    return this->compare(path) == 0;
}

bool Path::operator!=(const Path& path) const
{
    return this->compare(path) != 0;
}


TemporalDir::TemporalDir(bool autoRemove)
  : bAutoRemove(autoRemove),
    mPath(Path::tempDirectory())
{
    mPath.createDirectories();
}

TemporalDir::~TemporalDir()
{
    if (bAutoRemove && mPath.exists())
        mPath.removeDirectory();
}

Path TemporalDir::path() const
{
    return mPath;
}



std::ostream &operator<< (std::ostream &os, const Path &path)
{
    os << path.toString() << std::flush;
    return os;
}

} // End namespace tl

