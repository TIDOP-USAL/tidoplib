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
 * \addtogroup core
 * \{
 */


class TL_EXPORT Process
  : public TaskBase
{

public:

    enum class Priority
    {
#ifdef TL_OS_WINDOWS
        realtime = REALTIME_PRIORITY_CLASS,
        high = HIGH_PRIORITY_CLASS,
        above_normal = ABOVE_NORMAL_PRIORITY_CLASS,
        normal = NORMAL_PRIORITY_CLASS,
        below_normal = BELOW_NORMAL_PRIORITY_CLASS,
        idle = IDLE_PRIORITY_CLASS
#else 
        realtime = -20,
        high = -15,
        above_normal = -10,
        normal = 0,
        below_normal = 10,
        idle = 19
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

    explicit Process(std::string commandText,
                     Priority priority = Priority::normal);
    ~Process() override;
   
   
    auto priority() const -> Priority;
   
    /*!
     * \brief Establece la prioridad del proceso
     * \param[in] priority
     */
    void setPriority(Priority priority);

#ifdef TL_OS_WINDOWS
private:

    auto formatErrorMsg(unsigned long errorCode) -> std::string;
    auto createPipe() -> bool;
#endif

private:

// TaskBase
  
    void execute(Progress *progressBar = nullptr) override;
  
};


/*! \} */ // end of core

} // End namespace tl

