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

#include "tidop/core/base/defs.h"
#include "tidop/core/base/path.h"
#include "tidop/core/app/log.h"
#include "tidop/core/console.h"

namespace tl
{

class Console;
class Log;

/*! \addtogroup AppManagement
 *  \{
 */

/*!
 * \brief Application information and management.
 * 
 * This class provides methods to access application-related information such as 
 * the executable path, version, and logging mechanisms.
 */
class TL_EXPORT App
{

private:

    /*!
     * \brief Default constructor is private to enforce singleton pattern.
     */
    App();

public:

    ~App() = default;

    TL_DISABLE_COPY(App)
    TL_DISABLE_MOVE(App)

    /*!
     * \brief Get the singleton instance of the application.
     * \return A reference to the App instance.
     */
    static auto instance() -> App&;

    /*!
     * \brief Get the path to the executable.
     * \return The executable path.
     */
    auto path() const -> Path;

    /*!
     * \brief Get the application version.
     * \return The version as a string.
     */
    auto version() const -> std::string;

    /*!
     * \brief Get the console instance.
     * \return A reference to the Console object.
     */
    static auto console() -> Console&;

    /*!
     * \brief Get the log instance.
     * \return A reference to the Log object.
     */
    static auto log() -> Log&;

private:

    void init();

};

/*! \} */

} // namespace tl
