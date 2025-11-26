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

/*!
 * \file path.h
 * \brief File and directory path manipulation utilities
 *
 * This module provides comprehensive path handling utilities for working with file
 * and directory paths in a platform-independent way. It wraps std::filesystem (C++17)
 * or Boost.Filesystem for older standards, providing a consistent interface across
 * Windows and Unix-like systems.
 *
 * ### Classes
 *
 * - \ref Path - Represents and manipulates file/directory paths
 * - \ref TemporalDir - RAII wrapper for temporary directories
 *
 * ### Features
 *
 * - Cross-platform path handling (Windows/Linux/macOS)
 * - UTF-8 and local encoding support
 * - Path component manipulation (filename, extension, parent, etc.)
 * - Directory operations (create, remove, list contents)
 * - File operations (copy, remove, comparison)
 * - Temporary directory management with auto-cleanup
 * - C++17 and C++20 support with fallback to Boost.Filesystem
 *
 * ### Example Usage
 *
 * \code{.cpp}
 * #include "tidop/core/base/path.h"
 *
 * // Create and manipulate paths
 * tl::Path path("/home/user/documents/file.txt");
 * std::string name = path.fileName().toString();      // "file.txt"
 * std::string base = path.baseName().toString();      // "file"
 * std::string ext = path.extension().toString();      // ".txt"
 * tl::Path parent = path.parentPath();                // "/home/user/documents"
 *
 * // Work with directories
 * tl::Path dir("/data");
 * if (!dir.exists()) {
 *     dir.createDirectories();
 * }
 *
 * // List files with filter
 * auto files = dir.list(".txt");
 *
 * // Use temporary directory with auto-cleanup
 * {
 *     tl::TemporalDir temp;
 *     auto tempPath = temp.path();
 *     // Use temp directory
 * }  // Auto-removed on scope exit
 * \endcode
 *
 * \see tl::Path, tl::TemporalDir
 */

#pragma once

#include "tidop/config.h"

#include <regex>
#include <list>
#include <memory>


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
 * ### Example Usage
 * \code{.cpp}
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
     * \param utf8Path The path as a std::string.
     */
    Path(const std::string &utf8Path);

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

    /*!
     * \brief Sets the path from a std::string.
     *
     * Replaces the current path with a new path specified as a UTF-8 encoded string.
     * The new path can be absolute or relative.
     *
     * \param[in] utf8Path The new path as a UTF-8 encoded std::string
     *
     * ### Example
     * \code{.cpp}
     * tl::Path path;
     * path.setPath("/home/user/file.txt");
     * \endcode
     */
    void setPath(const std::string &utf8Path);

    /*!
     * \brief Sets the path from a std::wstring.
     *
     * Replaces the current path with a new path specified as a wide string.
     *
     * \param[in] path The new path as a std::wstring
     */
    void setPath(const std::wstring &path);

    /*!
     * \brief Converts the path to a std::string in system encoding.
     *
     * ### Note
     *
     * On Windows, returns the path in the native Windows encoding.
     * On Unix systems, typically returns UTF-8.
     * For guaranteed UTF-8 output, use toUtf8() instead.
     *
     * \return Path as std::string
     *
     * \see toUtf8(), toLocal8Bit()
     */
    auto toString() const -> std::string;

    /*!
     * \brief Converts the path to a std::wstring.
     *
     * Returns the path as a wide string (wstring).
     *
     * \return Path as std::wstring
     *
     * \see toString(), toUtf8()
     */
    auto toWString() const -> std::wstring;

    /*!
     * \brief Converts the path to UTF-8 encoded string.
     *
     * Ensures the returned path is always in UTF-8 encoding,
     * regardless of the system's native encoding.
     *
     * \return Path as UTF-8 encoded std::string
     *
     * \see toString(), toLocal8Bit()
     */
    auto toUtf8() const -> std::string;

    /*!
     * \brief Converts the path to the system's local 8-bit encoding.
     *
     * Returns the path in the local encoding of the system
     * (typically UTF-8 on Unix, varies on Windows).
     *
     * \return Path as local 8-bit encoded std::string
     *
     * \see toString(), toUtf8()
     */
    auto toLocal8Bit() const -> std::string;

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

    /*!
     * \brief Lists all files in the directory with a specific extension.
     *
     * Searches the current directory (represented by this Path) for all regular files
     * matching the specified extension. The search is case-insensitive.
     *
     * \param[in] extension The file extension to filter by (e.g., ".txt", ".jpg").
     *                      Can include or omit the leading dot.
     * \return A list of Path objects containing only the filenames (not full paths).
     *
     * ### Notes
     *
     * - Only returns regular files, not directories
     * - Extension matching is case-insensitive
     * - Returns only filenames, not full paths
     */
    auto list(const std::string &extension) -> std::list<Path>;

    /*!
     * \brief Lists all files in the directory matching a regex filter.
     *
     * Searches the current directory (represented by this Path) for all regular files
     * whose names match the provided regular expression.
     *
     * \param[in] filter The regex pattern to match filenames against.
     * \return A list of Path objects containing only the filenames matching the filter.
     *
     * ### Example
     * \code{.cpp}
     * tl::Path dir("/data");
     * std::regex pattern(R"(.*\.(txt|log)$)");
     * auto files = dir.list(pattern);
     * \endcode
     *
     * ### Notes
     *
     * - Only returns regular files, not directories
     * - Returns only filenames, not full paths
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
     * \brief Compares this path with another path lexicographically.
     *
     * Performs a string-based lexicographic comparison of the two paths.
     * This is different from equivalent() which checks if paths point to the same file.
     *
     * \param[in] path The path to compare against
     * \return 0 if paths are lexicographically equal,
     *         negative value if this path is less than path,
     *         positive value if this path is greater than path
     *
     * ### Example
     * \code{.cpp}
     * tl::Path path1("/a/file.txt");
     * tl::Path path2("/b/file.txt");
     * int result = path1.compare(path2);
     * if (result < 0) {
     *     // path1 is lexicographically less than path2
     * }
     * \endcode
     *
     * ### Notes
     *
     * - Performs lexicographic (string) comparison, not filesystem equivalence
     * - Case-sensitive on Unix, case-insensitive on Windows (filesystem-dependent)
     * - Does not require paths to exist
     *
     * \see equivalent()
     */
    auto compare(const Path &path) const -> int;

    /*!
     * \brief Checks if this path is equivalent to another path.
     *
     * Determines whether two paths refer to the same file or directory,
     * even if the paths are expressed differently (e.g., with/without symlinks,
     * relative vs absolute, different separators).
     *
     * This uses filesystem equivalence checking, not simple string comparison.
     *
     * \param[in] path The path to compare against
     * \return `true` if both paths refer to the same file/directory, `false` otherwise
     *
     * ### Example
     * \code{.cpp}
     * tl::Path path1("/home/user/file.txt");
     * tl::Path path2("./file.txt");  // From /home/user directory
     * if (path1.equivalent(path2)) {
     *     // They refer to the same file
     * }
     * \endcode
     *
     * \see compare()
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
     * \brief Normalizes the path to use preferred separators.
     *
     * Converts the path to use the preferred path separators for the current platform:
     * - Forward slashes (/) on Unix-like systems
     * - Backslashes (\) on Windows
     *
     * This is useful for ensuring consistent path representation across platforms.
     *
     * ### Example
     * \code{.cpp}
     * tl::Path path("C:\\Users/Documents\\file.txt");
     * path.normalize();
     * // On Windows: "C:\Users\Documents\file.txt"
     * // On Unix: "C:/Users/Documents/file.txt"
     * \endcode
     *
     * \see absolutePath()
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
     * \brief Returns the system's temporary directory path.
     *
     * Returns the path to the system's temporary directory, typically:
     * - Windows: `C:\Users\<username>\AppData\Local\Temp`
     * - Unix: `/tmp`
     *
     * Uses std::filesystem (C++17+) or boost::filesystem for older standards.
     *
     * \return Path to the system temporary directory
     *
     * ### Example
     * \code{.cpp}
     * tl::Path tmpDir = tl::Path::tempPath();
     * \endcode
     *
     * \see createTempDirectory(), TemporalDir
     */
    static auto tempPath() -> Path;

    /*!
     * \brief Creates and returns a new unique temporary directory.
     *
     * Generates a new unique temporary directory in the system's temp location
     * and actually creates it on disk.
     *
     * \return Path to the newly created temporary directory
     *
     * ### Example
     * \code{.cpp}
     * tl::Path tmpDir = tl::Path::createTempDirectory();
     * // tmpDir now exists on disk
     * \endcode
     *
     * \see tempPath(), TemporalDir
     */
    static auto createTempDirectory() -> Path;

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
     * \brief Returns a hash value for the given path.
     *
     * Computes a hash value suitable for use in hash-based containers
     * like std::unordered_map or std::unordered_set.
     *
     * \param[in] path The path to hash
     * \return A size_t hash value
     *
     * ### Example
     * \code{.cpp}
     * std::unordered_set<size_t> pathHashes;
     * pathHashes.insert(tl::Path::hash(somePath));
     * \endcode
     *
     * \see operator==(), operator!=()
     */
    static auto hash(const Path &path) -> size_t;

    /*!
     * \brief Copies a file or directory from one location to another.
     *
     * Recursively copies the source path (file or directory) to the destination.
     * If the source is a directory, all contents are copied recursively.
     *
     * \param[in] from Source path (file or directory)
     * \param[in] to Destination path where to copy to
     *
     * ### Example
     * \code{.cpp}
     * tl::Path::copy(tl::Path("original.txt"), tl::Path("backup.txt"));
     * tl::Path::copy(tl::Path("/src/dir"), tl::Path("/dst/dir"));
     * \endcode
     *
     */
    static void copy(const Path &from, const Path &to);

    /*!
     * \brief Returns the current working directory path.
     *
     * Gets the absolute path of the current working directory of the process.
     *
     * \return Path to the current working directory
     *
     * ### Example
     * \code{.cpp}
     * tl::Path cwd = tl::Path::currentPath();
     * \endcode
     *
     * ### Notes
     *
     * - Returned path is absolute
     * - Current working directory can be changed by OS or other processes
     *
     */
    static auto currentPath() -> Path;

    /*!
     * \brief Creates a Path from a local 8-bit encoded string.
     *
     * Constructs a Path from a string encoded in the system's local 8-bit encoding.
     * Useful for paths obtained from system APIs or environment variables.
     *
     * \param[in] s String in local 8-bit encoding
     * \return Path object
     *
     * ### Example
     * \code{.cpp}
     * const char* envPath = std::getenv("HOME");
     * tl::Path home = tl::Path::fromLocal8Bit(envPath);
     * \endcode
     *
     */
    static auto fromLocal8Bit(const std::string &s) -> Path;

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
 * ### Example Usage
 * \code{.cpp}
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
