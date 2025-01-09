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
#include "tidop/core/task/task.h"

namespace tl
{

class Progress;

/*!
 * \addtogroup Task
 * \{
 */

/*!
 * \class Process
 * \brief Represents a task that executes an external process.
 *
 * The Process class allows the execution of external processes with configurable priorities
 * and handles platform-specific details for process management.
 */
class TL_EXPORT Process
  : public TaskBase
{

public:

    /*!
     * \enum Priority
     * \brief Defines the priority levels for the process execution.
     */
    enum class Priority
    {
#ifdef TL_OS_WINDOWS
        realtime = REALTIME_PRIORITY_CLASS,         /*!< Real-time priority. */
        high = HIGH_PRIORITY_CLASS,                 /*!< High priority. */
        above_normal = ABOVE_NORMAL_PRIORITY_CLASS, /*!< Above normal priority. */
        normal = NORMAL_PRIORITY_CLASS,             /*!< Normal priority. */
        below_normal = BELOW_NORMAL_PRIORITY_CLASS, /*!< Below normal priority. */
        idle = IDLE_PRIORITY_CLASS                  /*!< Idle priority. */
#else 
        realtime = -20,         /*!< Real-time priority (highest). */
        high = -15,             /*!< High priority. */
        above_normal = -10,     /*!< Above normal priority. */
        normal = 0,             /*!< Normal priority. */
        below_normal = 10,      /*!< Below normal priority. */
        idle = 19               /*!< Idle priority (lowest). */
#endif
    };

private:

    std::string mCommandText; 
    Priority mPriority;
    bool outputHandle;
#ifdef TL_OS_WINDOWS
    STARTUPINFO mStartUpInfo;
    PROCESS_INFORMATION mProcessInformation;
    SECURITY_ATTRIBUTES mSecurityAttributes;
    HANDLE mThreadHandle;
#endif

public:

    /*!
     * \brief Constructs a Process object with a command and priority.
     * \param[in] commandText The command to execute as a process.
     * \param[in] priority The priority of the process (default: Priority::normal).
     */
    explicit Process(std::string commandText,
                     Priority priority = Priority::normal);
    ~Process() override;
   
    /*!
     * \brief Gets the priority of the process.
     * \return The priority of the process.
     */
    auto priority() const -> Priority;
   
    /*!
     * \brief Sets the priority of the process.
     * \param[in] priority The new priority level for the process.
     */
    void setPriority(Priority priority);

#ifdef TL_OS_WINDOWS
private:

    /*!
     * \brief Formats an error message based on a given error code (Windows only).
     * \param[in] errorCode The error code to format.
     * \return A formatted error message as a string.
     */
    auto formatErrorMsg(unsigned long errorCode) -> std::string;

    /*!
     * \brief Creates a pipe for interprocess communication (Windows only).
     * \return True if the pipe was successfully created, otherwise false.
     */
    auto createPipe() -> bool;
#endif

private:

// TaskBase
  
    void execute(Progress *progressBar = nullptr) override;
  
};


/*! \} */

} // End namespace tl

