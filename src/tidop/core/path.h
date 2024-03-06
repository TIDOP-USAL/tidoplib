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

#include "tidop/config.h"

#include <regex>
#include <list>
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

private:

    std::unique_ptr<internal::Path> mPath;

public:

    Path();
    Path(const std::string &path);
    Path(const std::wstring &path);
    Path(const Path &path);
    Path(Path &&path) TL_NOEXCEPT;
    ~Path();

    auto operator = (const Path &path) -> Path&;
    auto operator = (Path &&path) TL_NOEXCEPT -> Path&;

    void setPath(const std::string &path);
    void setPath(const std::wstring &path);

    /*!
     * \brief Converts the path to a std::string
     */
    auto toString() const -> std::string;

    /*!
     * \brief Converts the path to a std::wstring
     */
    auto toWString() const -> std::wstring;

    auto fileName() const -> Path;
    auto baseName() const -> Path;
    auto extension() const -> Path;
    auto parentPath() const -> Path;
    auto absolutePath() const -> Path;
    //TODO: Deberian ser métodos estáticos
    auto list(const std::string &extension) -> std::list<Path>;
    auto list(const std::regex &filter) -> std::list<Path>;

    auto isDirectory() const -> bool;
    auto isFile() const -> bool;
    auto isAbsolutePath() const -> bool;
    auto empty() const -> bool;
    auto exists() const -> bool;

    auto replaceFileName(const std::string &fileName) -> Path&;
    auto replaceFileName(const std::wstring &fileName) -> Path&;
    auto replaceFileName(const Path &fileName) -> Path&;
    auto replaceBaseName(const std::string &baseName) -> Path&;
    auto replaceBaseName(const std::wstring &baseName) -> Path&;
    auto replaceBaseName(const Path &baseName) -> Path&;
    auto replaceExtension(const std::string &extension) -> Path&;
    auto replaceExtension(const std::wstring &extension) -> Path&;
    auto replaceExtension(const Path &extension) -> Path&;
    auto append(const std::string &text) -> Path&;
    auto append(const std::wstring &text) -> Path&;
    auto append(const Path &text) -> Path&;

    auto compare(const Path &path) const -> int;
    auto equivalent(const Path &path) const -> bool;
    auto createDirectory() const -> bool;
    auto createDirectories() const -> bool;
    void removeDirectory() const;
    void normalize();

    void clear();

    /* Static methods */

    static auto exists(const Path &path) -> bool;
    static auto tempPath() -> Path;
    static auto tempDirectory() -> Path;
    static auto createDirectory(const Path &directory) -> bool;
    static auto createDirectory(const std::string &directory) -> bool;
    static auto createDirectory(const std::wstring &directory) -> bool;
    static auto createDirectories(const Path &directory) -> bool;
    static auto createDirectories(const std::string &directory) -> bool;
    static auto createDirectories(const std::wstring &directory) -> bool;
    static void removeDirectory(const Path &directory);
    static void removeDirectory(const std::string &directory);
    static void removeDirectory(const std::wstring &directory);
    static void removeFile(const Path &file);
    static auto hash(const Path &path) -> size_t;
    static void copy(const Path &from, const Path &to);
    static auto currentPath() -> Path;

    /* Override operators */

    auto operator == (const Path &path) const -> bool;
    auto operator != (const Path &path) const -> bool;

};


/*!
 * /brief Creates a temporary directory that is automatically deleted by destructor.
 * 
 */
class TL_EXPORT TemporalDir
{
    
private:

    bool bAutoRemove;
    Path mPath;

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

    auto path() const -> Path;

};


/* Override operators */

TL_EXPORT std::ostream &operator<< (std::ostream &os, const Path &path);

/*! \} */ // end of core




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



#endif // TL_CORE_PATH_H
