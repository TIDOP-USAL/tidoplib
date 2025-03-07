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
#include "tidop/core/base/defs.h"

#ifdef TL_HAVE_FMT
#include <fmt/format.h>
#else
#include <format>
#endif

namespace tl
{


/*! \addtogroup Base
 *  \{
 */

/*!
 * \brief Format a message string with arguments.
 * \tparam Args Argument types.
 * \param[in] s Format string.
 * \param[in] args Arguments for formatting.
 * \return Formatted string.
 */
template<typename... Args>
auto format(FORMAT_NAMESPACE format_string<Args...> s, Args&&... args) -> std::string
{
    return FORMAT_NAMESPACE vformat(s.get(), FORMAT_NAMESPACE make_format_args(args...));
}


/*! \} */


} // End namespace tl
