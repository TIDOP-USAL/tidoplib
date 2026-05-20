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
#include "tidop/core/task/Task.h"

namespace tl
{

class Progress;

/*!
 * \addtogroup Task
 * \{
 */

/*!
 * \class Process
 * \brief Executes an external command as a process.
 *
 * The Process class provides a cross-platform abstraction for launching and
 * managing external processes. It supports configurable priority levels and,
 * on Windows, optional redirection and asynchronous reading of the process
 * output through pipes.
 *
 * \note Thread-safe. Internally synchronized when managing process execution.
 */
class TL_EXPORT Process
  : public TaskBase
{

public:

    /*!
     * \enum Priority
     * \brief Defines priority levels available for process execution.
     *
     * The values correspond to platform-specific priority constants.
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
    STARTUPINFO mStartUpInfo{};
    PROCESS_INFORMATION mProcessInformation{};
    SECURITY_ATTRIBUTES mSecurityAttributes{};
    HANDLE mThreadHandle = nullptr;
    HANDLE mPipeReadHandle = nullptr;
    HANDLE mPipeWriteHandle = nullptr;

#endif

public:

    /*!
     * \brief Constructs a Process object with a command and an optional priority.
     * \param[in] commandText The command to execute.
     * \param[in] priority The desired process priority (default: Priority::normal).
     */
    explicit Process(std::string commandText,
                     Priority priority = Priority::normal);
    /*!
     * \brief Destructor.
     *
     * Ensures that any system handles used during process management
     * are properly released.
     */
    ~Process() override;
   
    /*!
     * \brief Returns the current priority of the process.
     * \return The priority level.
     */
    auto priority() const -> Priority;
   
    /*!
     * \brief Sets a new priority level for the process.
     * \param[in] priority The desired priority.
     *
     * \note The priority is applied when the process is launched.
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

    /*!
     * \brief Reads from the process output pipe asynchronously.
     *
     * This function is executed in a dedicated system thread and reads data
     * from the process output stream until no more data is available.
     *
     * \param[in] lpParam Pointer to the Process instance.
     * \return Thread exit code.
     *
     * \note Windows only.
     */
    static DWORD WINAPI readFromPipe(LPVOID lpParam);

#endif

private:

// TaskBase
  
    void execute(Progress *progressBar = nullptr) override;
  
};


/*! \} */

} // End namespace tl

