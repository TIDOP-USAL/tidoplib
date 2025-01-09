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

#pragma once

#include "tidop/config.h"

#include <regex>
#include <list>
#include <memory>


#include "tidop/core/base/defs.h"


namespace tl
{

namespace internal
{
class Path;
}

/*! \addtogroup Base
 *  \{
 */

/*!
 * \class Path
 * \brief Represents a file or directory path.
 *
 * This class provides various utilities for handling file and directory paths, such as checking existence,
 * creating directories, and retrieving components like file names and extensions.
 * 
 * Example:
 * \code
 * Path path("/usr/local/bin");
 * if (path.exists()) {
 *     auto fileName = path.fileName();
 *     auto extension = path.extension();
 * }
 * \endcode
 */
class TL_EXPORT Path
{

private:

    std::unique_ptr<internal::Path> mPath;

public:

    /*!
     * \brief Default constructor.
     */
    Path();

    /*!
     * \brief Constructs a Path object from a string.
     * \param path The path as a std::string.
     */
    Path(const std::string &path);

    /*!
     * \brief Constructs a Path object from a wide string.
     * \param path The path as a std::wstring.
     */
    Path(const std::wstring &path);
    Path(const Path &path);
    Path(Path &&path) TL_NOEXCEPT;
    ~Path();

    auto operator = (const Path &path) -> Path&;
    auto operator = (Path &&path) TL_NOEXCEPT -> Path&;

    void setPath(const std::string &path);
    void setPath(const std::wstring &path);

    /*!
     * \brief Converts the path to a std::string.
     */
    auto toString() const -> std::string;

    /*!
     * \brief Converts the path to a std::wstring.
     */
    auto toWString() const -> std::wstring;

    //auto toLocal8Bit() const -> std::string;

    /*!
     * \brief Returns the file name component of the path.
     */
    auto fileName() const -> Path;

    /*!
     * \brief Returns the base name of the file without extension.
     */
    auto baseName() const -> Path;

    /*!
     * \brief Returns the file extension.
     */
    auto extension() const -> Path;

    /*!
     * \brief Returns the parent directory path.
     */
    auto parentPath() const -> Path;

    /*!
     * \brief Returns the absolute version of the path.
     */
    auto absolutePath() const -> Path;

    //TODO: Deberian ser métodos estáticos

    /*!
     * \brief Lists all files in the directory with a specific extension.
     * \param[in] extension The file extension to filter by.
     * \return A list of paths.
     */
    auto list(const std::string &extension) -> std::list<Path>;

    /*!
     * \brief Lists all files in the directory matching a regex filter.
     * \param[in] filter The regex filter.
     * \return A list of paths.
     */
    auto list(const std::regex &filter) -> std::list<Path>;

    /*!
     * \brief Checks if the path is a directory.
     */
    auto isDirectory() const -> bool;

    /*!
     * \brief Checks if the path is a file.
     */
    auto isFile() const -> bool;

    /*!
     * \brief Checks if the path is absolute.
     */
    auto isAbsolutePath() const -> bool;

    /*!
     * \brief Checks if the path is empty.
     */
    auto empty() const -> bool;

    /*!
     * \brief Checks if the path exists.
     */
    auto exists() const -> bool;

    /*!
     * \brief Replaces the file name component of the path.
     */
    auto replaceFileName(const std::string &fileName) -> Path&;

    /*!
     * \brief Replaces the file name component of the path with a wide string.
     */
    auto replaceFileName(const std::wstring &fileName) -> Path&;

    /*!
     * \brief Replaces the file name component of the path with another Path.
     */
    auto replaceFileName(const Path &fileName) -> Path&;

    /*!
     * \brief Replaces the base name component of the path.
     */
    auto replaceBaseName(const std::string &baseName) -> Path&;

    /*!
     * \brief Replaces the base name component of the path with a wide string.
     */
    auto replaceBaseName(const std::wstring &baseName) -> Path&;

    /*!
     * \brief Replaces the base name component of the path with another Path.
     */
    auto replaceBaseName(const Path &baseName) -> Path&;

    /*!
     * \brief Replaces the file extension.
     */
    auto replaceExtension(const std::string &extension) -> Path&;

    /*!
     * \brief Replaces the file extension with a wide string.
     */
    auto replaceExtension(const std::wstring &extension) -> Path&;

    /*!
     * \brief Replaces the file extension with another Path.
     */
    auto replaceExtension(const Path &extension) -> Path&;

    /*!
     * \brief Appends a string to the path.
     */
    auto append(const std::string &text) -> Path&;

    /*!
     * \brief Appends a wide string to the path.
     */
    auto append(const std::wstring &text) -> Path&;

    /*!
     * \brief Appends another Path to the path.
     */
    auto append(const Path &text) -> Path&;

    /*!
     * \brief Compares the path with another path.
     * \return 0 if paths are equal, a negative value if this path is less, or a positive value if greater.
     */
    auto compare(const Path &path) const -> int;

    /*!
     * \brief Checks if the path is equivalent to another path.
     */
    auto equivalent(const Path &path) const -> bool;

    /*!
     * \brief Creates a directory.
     */
    auto createDirectory() const -> bool;

    /*!
     * \brief Creates directories along the path.
     */
    auto createDirectories() const -> bool;

    /*!
     * \brief Removes the directory.
     */
    void removeDirectory() const;

    /*!
     * \brief Normalizes the path, resolving any relative components.
     */
    void normalize();

    /*!
     * \brief Clears the path.
     */
    void clear();

    /* Static methods */

    /*!
     * \brief Checks if a path exists.
     */
    static auto exists(const Path &path) -> bool;

    /*!
     * \brief Returns the system's temporary path.
     */
    static auto tempPath() -> Path;

    /*!
     * \brief Returns the system's temporary directory.
     */
    static auto tempDirectory() -> Path;

    /*!
     * \brief Creates a directory at the given path.
     */
    static auto createDirectory(const Path &directory) -> bool;

    /*!
     * \brief Creates directories along the given path.
     */
    static auto createDirectories(const Path &directory) -> bool;

    /*!
     * \brief Removes a directory at the given path.
     */
    static void removeDirectory(const Path &directory);

    /*!
     * \brief Removes a directory given as a std::string.
     */
    static void removeDirectory(const std::string &directory);

    /*!
     * \brief Removes a directory given as a std::wstring.
     */
    static void removeDirectory(const std::wstring &directory);

    /*!
     * \brief Removes a file at the given path.
     */
    static void removeFile(const Path &file);

    /*!
     * \brief Returns a hash of the path.
     */
    static auto hash(const Path &path) -> size_t;

    /*!
     * \brief Copies a file or directory from one path to another.
     */
    static void copy(const Path &from, const Path &to);

    /*!
     * \brief Returns the current working directory path.
     */
    static auto currentPath() -> Path;

    //static auto fromLocal8Bit(const std::string &s) -> Path;

    /* Override operators */

    /*!
     * \brief Equality operator.
     */
    auto operator==(const Path &path) const -> bool;

    /*!
     * \brief Inequality operator.
     */
    auto operator!=(const Path &path) const -> bool;

};


/*!
 * \class TemporalDir
 * \brief Creates a temporary directory that is automatically deleted by the destructor.
 *
 * This class manages a temporary directory, ensuring its removal upon destruction if
 * auto-removal is enabled.
 *
 * Example:
 * \code
 * TemporalDir tempDir;
 * auto path = tempDir.path();
 * // Use the temporary directory
 * \endcode
 */
class TL_EXPORT TemporalDir
{
    
private:

    bool bAutoRemove;   /*!< Indicates whether the directory should be automatically removed. */
    Path mPath;         /*!< The path to the temporary directory. */

public:

    /*!
     * \brief Constructs a TemporalDir object.
     * \param[in] autoRemove Specifies whether to automatically remove the directory on destruction.
     */
    explicit TemporalDir(bool autoRemove = true);

    ~TemporalDir();

    TL_DISABLE_COPY(TemporalDir)
    TL_DISABLE_MOVE(TemporalDir)

    /*!
     * \brief Returns the path to the temporary directory.
     * \return The path to the temporary directory.
     */
    auto path() const -> Path;

};


/* Override operators */

TL_EXPORT std::ostream &operator<< (std::ostream &os, const Path &path);

/*! \} */




//class TL_EXPORT FileStatus
//{
//
//private:
//
//    Path path;
//
//public:
//
//    FileStatus(Path path);
//    ~FileStatus();
//
//    bool isBlock() const;
//
//};



} // End namespace tl
