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

auto Path::operator=(const Path &path)  -> Path&
{
    mPath = std::make_unique<internal::Path>(*path.mPath);

    return *this;
}

auto Path::operator=(Path &&path) TL_NOEXCEPT  -> Path&
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

auto Path::toString() const -> std::string
{
    return mPath->ref().string();
}

auto Path::toWString() const -> std::wstring
{
    return mPath->ref().wstring();
}

auto Path::fileName() const -> Path
{
    return Path(mPath->ref().filename().native());
}

auto Path::baseName() const -> Path
{
    return Path(mPath->ref().stem().native());
}

auto Path::extension() const -> Path
{
    return Path(mPath->ref().extension().native());
}

auto Path::parentPath() const -> Path
{
    Path parent_path(mPath->ref().parent_path().native());
    return parent_path;
}

auto Path::isDirectory() const -> bool
{
    return mPath->isDirectory();
}

auto Path::isFile() const -> bool
{
    return mPath->isFile();
}

auto Path::empty() const -> bool
{
    return mPath->empty();
}

auto Path::exists() const -> bool
{
    return mPath->exists();
}

auto Path::list(const std::string &extension) -> std::list<Path>
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

auto Path::list(const std::regex &filter) -> std::list<Path>
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

auto Path::replaceFileName(const std::string &fileName) -> Path&
{
    fs::path &_path = mPath->ref();

    if (_path.has_filename()) {
        _path.remove_filename();
        _path.append(fileName);
    }

    return *this;
}

auto Path::replaceFileName(const std::wstring &fileName) -> Path&
{
    fs::path &_path = mPath->ref();

    if (_path.has_filename()) {
        _path.remove_filename();
        _path.append(fileName);
    }

    return *this;
}

auto Path::replaceFileName(const Path &fileName) -> Path&
{
    return replaceFileName(fileName.toWString());
}

auto Path::replaceBaseName(const std::string &baseName) -> Path&
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

auto Path::replaceBaseName(const std::wstring &baseName) -> Path&
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

auto Path::replaceBaseName(const Path &baseName) -> Path&
{
    return replaceBaseName(baseName.toWString());
}

auto Path::replaceExtension(const std::string &extension) -> Path&
{
    mPath->ref().replace_extension(extension);
    return *this;
}

auto Path::replaceExtension(const std::wstring &extension) -> Path&
{
    mPath->ref().replace_extension(extension);
    return *this;
}

auto Path::replaceExtension(const Path &extension) -> Path&
{
    return replaceExtension(extension.toString());
}

auto Path::append(const std::string &text) -> Path&
{
    mPath->ref().append(text);
    return *this;
}

auto Path::append(const std::wstring &text) -> Path&
{
    mPath->ref().append(text);
    return *this;
}

auto Path::append(const Path &text) -> Path&
{
    return append(text.toWString());
}

auto Path::createDirectory() const -> bool
{
    return fs::create_directory(mPath->ref());
}

auto Path::createDirectories() const -> bool
{
    return fs::create_directories(mPath->ref());
}

void Path::removeDirectory() const
{
    fs::remove_all(mPath->ref());
}

void Path::normalize()
{
    mPath->ref().make_preferred();
}

void Path::clear()
{
    mPath = std::make_unique<internal::Path>("");
}

/* Static methods */

auto Path::exists(const Path &path) -> bool
{
    return path.exists();
}

auto Path::tempPath() -> Path
{
    Path temp_path(fs::temp_directory_path().string());
    return temp_path;
}

auto Path::tempDirectory() -> Path
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

auto Path::compare(const Path &path) const -> int
{
    return mPath->ref().compare(path.toString());
}

auto tl::Path::equivalent(const Path &path) const -> bool
{
    return fs::equivalent(mPath->ref(), path.toWString());
}

auto Path::createDirectory(const Path &directory) -> bool
{
    return fs::create_directory(directory.toWString());
}

auto Path::createDirectory(const std::string &directory) -> bool
{
    return fs::create_directory(directory);
}

auto Path::createDirectory(const std::wstring &directory) -> bool
{
    return fs::create_directory(directory);
}

auto Path::createDirectories(const Path &directory) -> bool
{
    return fs::create_directories(directory.toWString());
}

auto Path::createDirectories(const std::string &directory) -> bool
{
    return fs::create_directories(directory);
}

auto Path::createDirectories(const std::wstring &directory) -> bool
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

auto Path::hash(const Path &path) -> size_t
{
    return fs::hash_value(path.mPath->ref());
}

void tl::Path::copy(const Path &from, const Path &to)
{
    fs::copy(from.toWString(), to.toWString());
}

/* Override operators */

auto Path::operator==(const Path& path) const -> bool
{
    return this->compare(path) == 0;
}

auto Path::operator!=(const Path& path) const -> bool
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

auto TemporalDir::path() const -> Path
{
    return mPath;
}



std::ostream &operator<< (std::ostream &os, const Path &path)
{
    os << path.toString() << std::flush;
    return os;
}

} // End namespace tl

