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

#include "tidop/core/base/Path.h"

#include "tidop/core/base/StringUtils.h"
#include "tidop/core/base/TextEncoding.h"


// filesystem
#if (TL_CPP_VERSION>= 17)
#include <filesystem>
#include <functional> 
#else
#include <boost/filesystem.hpp>
#include <boost/functional/hash.hpp> 
#endif
#include <random>
#include <codecvt>
#include <ostream>

#if (TL_CPP_VERSION>= 17)
namespace fs = std::filesystem;
#else
namespace fs = boost::filesystem;
#endif

namespace tl
{

/// \cond

class Path::Impl
{
public:

    Impl() = default;

    explicit Impl(const fs::path &path)
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

/// \endcond



Path::Path()
  : mPath(new Impl)
{
}

Path::Path(const std::string &utf8Path)
  : mPath(new Impl(tl::fromUtf8(utf8Path)))
{
}

Path::Path(const std::wstring &path)
  : mPath(new Impl(path))
{
}

Path::Path(const Path &path)
  : mPath(new Impl(*path.mPath))
{
}

Path::Path(Path &&path) noexcept
  : mPath(std::move(path.mPath))
{
}

Path::~Path()
{
}

auto Path::operator=(const Path &path)  -> Path&
{
    mPath = std::make_unique<Impl>(*path.mPath);

    return *this;
}

auto Path::operator=(Path &&path) noexcept  -> Path&
{
    mPath = std::move(path.mPath);

    return *this;
}

void Path::setPath(const std::string &utf8Path)
{
    mPath = std::make_unique<Impl>(tl::fromUtf8(utf8Path));
}

void Path::setPath(const std::wstring &path)
{
    mPath = std::make_unique<Impl>(path);
}

auto Path::toString() const -> std::string
{
    return mPath->ref().string();
}

auto Path::toWString() const -> std::wstring
{
    return mPath->ref().wstring();
}

auto Path::toUtf8() const -> std::string
{
    return tl::toUtf8(mPath->ref().wstring());
}

auto Path::toLocal8Bit() const -> std::string
{
    return tl::toLocal8Bit(mPath->ref().wstring());
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

auto Path::absolutePath() const -> Path
{
    return Path(fs::absolute(mPath->ref()).native());
}

auto Path::isDirectory() const -> bool
{
    return mPath->isDirectory();
}

auto Path::isFile() const -> bool
{
    return mPath->isFile();
}

auto Path::isAbsolutePath() const -> bool
{
    return mPath->ref().is_absolute();
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

auto Path::replaceFileName(const std::string &utf8FileName) -> Path&
{
    fs::path &_path = mPath->ref();

    if (_path.has_filename()) {
        _path.remove_filename();
        _path.append(tl::fromUtf8(utf8FileName));
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

auto Path::replaceBaseName(const std::string &utf8BaseName) -> Path&
{
    fs::path &_path = mPath->ref();

    if (_path.has_filename()) {
        std::string ext = _path.extension().string();
        std::string file_name = utf8BaseName + ext;
        _path.remove_filename();
        _path.append(tl::fromUtf8(file_name));
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
    mPath->ref().append(tl::fromUtf8(text));
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
    if (mPath->exists()) return true;
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
    mPath = std::make_unique<Impl>("");
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

//auto Path::createTempDirectory() -> Path
//{
//    try {
//        // Generate unique directory in system temp location
//        fs::path tempDir = fs::temp_directory_path() /
//            fs::unique_path("tidop-%%%%-%%%%-%%%%-%%%%");
//        if (fs::create_directories(tempDir)) {
//            return Path(tempDir.string());
//        }
//    } catch (const std::exception &e) {
//        Message::warning("Failed to create temporary directory: {}", e.what());
//    }
//    return Path();
//}
auto Path::createTempDirectory() -> Path
{
    try {

        auto generateUniquePathString = []() {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, 15);
            std::ostringstream ss;
            const std::string pattern = "tidop-%%%%-%%%%-%%%%-%%%%";
            for (char c : pattern) {
                if (c == '%') ss << std::hex << dis(gen);
                else ss << c;
            }
            return ss.str();
        };

        fs::path temp_dir = fs::temp_directory_path() / generateUniquePathString();

        if (fs::create_directories(temp_dir)) {
            return Path(temp_dir.string());
        }
    } catch (const std::exception &e) {
        Message::warning("Failed to create temporary directory: {}", e.what());
    }

    return Path();
}

auto Path::compare(const Path &path) const -> int
{
    return mPath->ref().compare(path.toWString());
}

auto tl::Path::equivalent(const Path &path) const -> bool
{
    return fs::equivalent(mPath->ref(), path.toWString());
}

auto Path::createDirectory(const Path &directory) -> bool
{
    return directory.createDirectory();
}

auto Path::createDirectories(const Path &directory) -> bool
{
    return directory.createDirectories();
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

auto Path::currentPath() -> Path
{
    return Path(fs::current_path().wstring());
}

auto Path::fromLocal8Bit(const std::string &s) -> Path
{
    return Path(tl::fromLocal8Bit(s));
}

/* Override operators */

auto Path::operator==(const Path &path) const -> bool
{
    return this->compare(path) == 0;
}

auto Path::operator!=(const Path &path) const -> bool
{
    return this->compare(path) != 0;
}




TemporalDir::TemporalDir(bool autoRemove)
  : bAutoRemove(autoRemove),
    mPath(Path::createTempDirectory())
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
    os << path.toUtf8() << std::flush;
    return os;
}


} // End namespace tl

