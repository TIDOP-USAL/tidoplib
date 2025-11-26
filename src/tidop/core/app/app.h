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

/*! \file app.h
 *  \brief Application singleton for global configuration and management
 *
 * This module provides a global access point for application-wide resources,
 * including executable path, version information, logging facilities, and
 * console I/O. It implements a thread-safe singleton pattern to guarantee
 * a unique and consistent application context.
 * 
 * ### Classes
 *
 * - \ref tl::App - Singleton for accessing global application services
 *
 * \see tl::Console, tl::Logger
 */


#pragma once

#include "tidop/config.h"
#include "tidop/core/console/console.h"
#include "tidop/core/app/logger.h"

#include <string>

#include "tidop/core/base/path.h"

namespace tl
{


/*! \addtogroup AppManagement
 *  \{
 */

/*!
 * \brief Singleton for application management and resource access.
 * 
 * App provides global access to application information and core resources
 * such as the executable path, logging facilities, and console I/O.
 *
 * ### Usage Example
 *
 * \code{.cpp}
 * // Get application information
 * Path exe_path = App::instance().path();
 * std::string version = App::instance().version();
 *
 * // Access global services
 * App::console().write("Hello World");
 * App::log().info("Application started");
 * \endcode
 *
 * \note Thread-safe singleton. The instance is created on first use and is
 *       safe to access concurrently.
 * \note Non-copyable and non-movable. The App singleton cannot be copied or moved.
 *
 * \see Logger, Console
 */
class TL_EXPORT App
{

private:

    /*!
     * \brief Private constructor for singleton pattern
     *
     * Performs application initialization including singleton setup for
     * console and logging subsystems.
     */
    App();

public:

    ~App() = default;

    TL_DISABLE_COPY(App)
    TL_DISABLE_MOVE(App)

    /*!
     * \brief Get the singleton instance of the application.
     *
     * Thread-safe access to the global App instance. The first call creates
     * the instance; subsequent calls return the same instance.
     *
     * \return Reference to the App singleton instance
     */
    static auto instance() -> App&;

    /*!
     * \brief Get the path to the application executable.
     *
     * Returns the full path to the currently running executable. Platform-specific
     * methods are used to resolve the location at runtime.
     *
     * \return The executable path as a Path object.
     * \see Path
     */
    auto path() const -> Path;

    /*!
     * \brief Get the global console instance.
     *
     * Provides access to the console I/O subsystem for interactive user
     * communication, menus, and command-line argument processing.
     *
     * \return Reference to the Console singleton instance
     *
     * \see Console
     */
    static auto console() -> Console&;

    /*!
     * \brief Get the global logger instance.
     *
     * Provides access to the file-based logging subsystem for recording
     * application events and diagnostics.
     *
     * \return Reference to the Logger singleton instance
     *
     * \see Logger
     */
    static auto log() -> Logger&;

private:

    void init();

};

/*! \} */

} // namespace tl
