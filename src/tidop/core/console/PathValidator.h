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
 * \file PathValidator.h
 * \brief Validator for paths.
 *
 * This file defines the \ref PathValidator class, which checks whether a given
 * path exists and whether it corresponds to a file, a directory, or any type.
 * It is commonly used to validate command‑line arguments that specify input
 * or output paths.
 *
 * \ingroup Validators
 */

#pragma once


#include "tidop/config.h"

#include <memory>

#include "tidop/core/app/Message.h"
#include "tidop/core/base/Format.h"
#include "tidop/core/base/Path.h"
#include "tidop/core/console/Validator.h"

namespace tl
{

/*! \addtogroup Validators
 *  \{
 */

/*!
 * \brief Validator that checks filesystem paths.
 *
 * This validator allows specifying:
 * - Whether the path must exist (\ref mMustExist).
 * - What kind of filesystem entry is expected: any, a regular file, or a directory
 *   (see \ref Type).
 *
 * If the path does not meet the requirements, an error message is emitted via
 * \ref Message::error and validation fails.
 *
 * Typical use cases include validating input file paths, output directories,
 * or ensuring that a path exists before processing.
 *
 * \ingroup Validators
 */
class PathValidator final
  : public ValidatorBase<Path>
{

public:

    /*!
     * \brief Expected type of the path.
     */
    enum class Type : uint8_t
    { 
        any,       ///< Any filesystem entry (file or directory) is accepted.
        file,      ///< The path must be a regular file.
        directory  ///< The path must be a directory.
    };

private:

    Type mType;
    bool mMustExist;

public:

    /*!
     * \brief Constructs a PathValidator.
     * \param[in] type The expected type of the path (default: \ref Type::any).
     * \param[in] mustExist Whether the path must exist (default: `true`).
     */
    PathValidator(Type type = Type::any, bool mustExist = true)
      : mType(type),
        mMustExist(mustExist)
    {
    }

    /*!
     * \brief Validates the given path.
     * \param[in] path The path to check.
     * \return `true` if the path satisfies all conditions; otherwise `false`
     *         and an error message is logged.
     *
     * The validation logic is:
     * - If `mMustExist` is `true` and the path does not exist, fail.
     * - If the path exists and `mType` is `file`, fail if it is not a regular file.
     * - If the path exists and `mType` is `directory`, fail if it is not a directory.
     * - If the path does not exist and `mMustExist` is `false`, the check passes
     *   (no further type validation is performed because the type is unknown).
     */
    auto validate(Path path) -> bool override
    {
        bool exists = path.exists();

        if (mMustExist && !exists) {
            Message::error("Path does not exist: '{}'", path.toString());
            return false;
        }

        if (exists) {
            if (mType == Type::file && !path.isFile()) {
                Message::error("Path is not a file: '{}'", path.toString());
                return false;
            }
            if (mType == Type::directory && !path.isDirectory()) {
                Message::error("Path is not a directory: '{}'", path.toString());
                return false;
            }
        }

        return true;
    }

    /*!
     * \brief Returns a string description of the validation rule.
     * \return A string like "Required: File (Must exist: Yes)".
     */
    [[nodiscard]]
    auto toString() const -> std::string override
    {
        std::string type_str = (mType == Type::file) ? "File" : (mType == Type::directory ? "Directory" : "Path");
        return tl::format("Required: {} (Must exist: {})", type_str, mMustExist ? "Yes" : "No");
    }

    /*!
     * \brief Factory method for creating a shared_ptr to a PathValidator.
     * \param[in] type The expected path type.
     * \param[in] must_exist Whether the path must exist.
     * \return std::shared_ptr<PathValidator>
     */
    static auto create(Type type = Type::any, bool must_exist = true) -> std::shared_ptr<PathValidator>
    {
        return std::make_shared<PathValidator>(type, must_exist);
    }

};


/*! \} */


} // End namespace tl
