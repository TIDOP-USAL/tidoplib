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

#include <string>

#include "tidop/core/defs.h"
#include "tidop/core/path.h"
#include "tidop/core/log.h"
#include "tidop/core/console.h"

namespace tl
{

class Console;
class Log;

/*! \addtogroup core
 *  \{
 */

/*!
 * \brief Application information
 */
class TL_EXPORT App
{

private:

    App();

public:

    ~App() = default;

    TL_DISABLE_COPY(App)
    TL_DISABLE_MOVE(App)

    /*!
     * \brief Application instance
     */
    static auto instance() -> App&;

    /*!
     * \brief Path to the executable
     */
    auto path() const -> Path;

    /*!
     * \brief Application version
     */
    auto version() const -> std::string;

    static auto console() -> Console&;
    static auto log() -> Log&;

private:

    void init();

};

/*! \} */ // end of core

} // namespace tl
