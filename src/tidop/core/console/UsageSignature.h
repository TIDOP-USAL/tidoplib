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

#include <vector>
#include <memory>

#include "tidop/core/console/Argument.h"

namespace tl
{

/*! \addtogroup Console
 *  \{
 */

/*!
 * \struct UsageSignature
 * \brief Represents a valid usage pattern for a command
 *
 * A `UsageSignature` defines a specific combination of arguments that is considered
 * a valid way to invoke a command. Each signature can include:
 *
 * - A list of **required** arguments (must be present to match the signature)
 * - A list of **optional** arguments (may or may not be present)
 * - An optional **description** (displayed in the help message next to the usage line)
 *
 * When multiple signatures are defined for a command, the parser will check that
 * the user input matches at least one of them. The help message will list each signature
 * as a separate `Usage:` line.
 *
 * ### Example
 * \code{.cpp}
 * auto arg_input = Argument::make<std::string>("input", "Input file");
 * auto arg_output = Argument::make<std::string>("output", "Output file");
 * auto arg_verbose = Argument::make<bool>("verbose", 'v', "Verbose output", false);
 *
 * UsageSignature sig1({arg_input, arg_output}, {}, "Basic input-output usage");
 * UsageSignature sig2({arg_input}, {arg_verbose}, "Alternative with verbose flag");
 * cmd.addUsage(sig1).addUsage(sig2);
 * \endcode
 *
 * \see Command::addUsage
 */
struct UsageSignature
{
    std::vector<Argument::Ptr> required;   /*!< Required arguments for this signature */
    std::vector<Argument::Ptr> optional;   /*!< Optional arguments for this signature */
    std::string description;               /*!< Description shown in help output */


    UsageSignature() = default;

    /*!
     * \brief Constructs a usage signature with required and optional arguments
     *
     * \param[in] req Required arguments
     * \param[in] opt Optional arguments (default: empty)
     * \param[in] desc Description (default: empty)
     */   
    UsageSignature(const std::vector<Argument::Ptr> &req,
                   const std::vector<Argument::Ptr> &opt = {},
                   const std::string &desc = "")
      : required(req),
        optional(opt), 
        description(desc)
    {
    }
};

/*! \} */

} // namespace tl

