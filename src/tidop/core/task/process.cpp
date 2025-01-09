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


#include "tidop/core/task/process.h"
#include "tidop/core/task/progress.h"

#ifdef TL_OS_LINUX
#include <spawn.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#endif
#include <array>
#include <locale>
#include <codecvt>
#include <utility>
#include <cstring>

#ifdef TL_OS_LINUX
extern char **environ;
#endif

namespace tl
{


#ifdef TL_OS_WINDOWS


constexpr auto process_bufsize = 4096;
HANDLE pipeReadHandle = nullptr;
HANDLE pipeWriteHandle = nullptr;

unsigned long readFromPipe(void *)
{
    DWORD numberOfBytesRead;
    //char buffer[process_bufsize];
    std::array<char, process_bufsize + 1> buffer{};
    int err = 0;

    for (;;) {
        err = ReadFile(pipeReadHandle, buffer.data(), process_bufsize, &numberOfBytesRead, nullptr);
        if (!err || numberOfBytesRead == 0) continue;
        buffer[static_cast<size_t>(numberOfBytesRead)] = '\0';
        std::cout << buffer.data() << std::endl;

        if (!err) break;
    }

    return 0;
}
#endif

Process::Process(std::string commandText,
                 Priority priority)
  : mCommandText(std::move(commandText)),
    outputHandle(false)
#ifdef TL_OS_WINDOWS
    , mThreadHandle(nullptr)
#endif
{
    setPriority(priority);
#ifdef TL_OS_WINDOWS
    ZeroMemory(&mStartUpInfo, sizeof(mStartUpInfo));
    mStartUpInfo.cb = sizeof(mStartUpInfo);
    mStartUpInfo.dwFlags |= STARTF_USESTDHANDLES;

    ZeroMemory(&mSecurityAttributes, sizeof(mSecurityAttributes));
    mSecurityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
    mSecurityAttributes.bInheritHandle = TRUE;
    mSecurityAttributes.lpSecurityDescriptor = nullptr;

    ZeroMemory(&mProcessInformation, sizeof(mProcessInformation));
#endif
}

Process::~Process() = default;

void Process::execute(Progress *)
{
    try {

#ifdef TL_OS_WINDOWS

        if (outputHandle && !createPipe()) return;

        unsigned long priority = static_cast<unsigned long>(mPriority);

        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        std::wstring command = converter.from_bytes(mCommandText);

        bool success = CreateProcess(nullptr,
                                     const_cast<wchar_t *>(command.c_str()),
                                     nullptr,
                                     nullptr,
                                     /*false,*/ true,
                                     CREATE_NO_WINDOW | priority,
                                     nullptr,
                                     nullptr,
                                     &mStartUpInfo,
                                     &mProcessInformation);


        TL_ASSERT(success, "CreateProcess failed ({}) {}", GetLastError(), formatErrorMsg(GetLastError()));

        // Cerrar el extremo de escritura de la tubería del proceso hijo, ya que no lo necesitamos
        if (outputHandle) {
            CloseHandle(pipeWriteHandle);
            mThreadHandle = CreateThread(0, 0, readFromPipe, nullptr, 0, nullptr);
        }

        DWORD ret = WaitForSingleObject(mProcessInformation.hProcess, INFINITE);
        if (ret == WAIT_OBJECT_0) {

            unsigned long exitCode;

            if (GetExitCodeProcess(mProcessInformation.hProcess, &exitCode) == 0) {
                TL_THROW_EXCEPTION("Error ({}: {}) when executing the command: {}",
                                   GetLastError(),
                                   formatErrorMsg(GetLastError()),
                                   mCommandText);
            }

            if (exitCode == 0){
                eventTriggered(Event::Type::task_finalized);
            } else {
                eventTriggered(Event::Type::task_error);
            }

        } else {

            TL_THROW_EXCEPTION("Error ({}: {}) when executing the command: {}",
                               GetLastError(),
                               formatErrorMsg(GetLastError()),
                               mCommandText);

        }

        CloseHandle(mProcessInformation.hProcess);
        CloseHandle(mProcessInformation.hThread);
        if (outputHandle) {
            CloseHandle(mThreadHandle);
            CloseHandle(pipeReadHandle);
        }

#else
        pid_t pid;
        char *command = strdup(mCommandText.data());
        //char *argv[] = {const_cast<char *>("sh"), const_cast<char *>("-c"), mCommandText.data(), nullptr};
        char *argv[] = {const_cast<char *>("sh"), const_cast<char *>("-c"), command, nullptr};

        int status = posix_spawn(&pid, "/bin/sh", nullptr, nullptr, argv, environ);
        
        TL_ASSERT(status == 0, "Error({}: {}) when executing the command : {}", status, strerror(status), mCommandText);

            if (waitpid(pid, &status, 0) != -1) {
                eventTriggered(Event::Type::task_finalized);
                return;
            } else {
                eventTriggered(Event::Type::task_error);
                return;
            }

        //  int posix_spawn(pid_t *pid, const char *path,
        //                  const posix_spawn_file_actions_t *file_actions,
        //                  const posix_spawnattr_t *attrp,
        //                  char *const argv[], char *const envp[]);
          /// Para escribir en un log la salida
          /// https://unix.stackexchange.com/questions/252901/get-output-of-posix-spawn
          
        free(command);

#endif
    } catch (...) {
        TL_THROW_EXCEPTION_WITH_NESTED("");
    }

}


auto Process::priority() const -> Priority
{
#ifdef TL_OS_WINDOWS
    return static_cast<Priority>(GetPriorityClass(mProcessInformation.hProcess));
#else
    return static_cast<Priority>(getpriority(PRIO_PROCESS, getpid()));
#endif
}

void Process::setPriority(Priority priority)
{
#ifdef TL_OS_WINDOWS
    SetPriorityClass(mProcessInformation.hProcess,
                     static_cast<unsigned long>(priority));
#else
    setpriority(PRIO_PROCESS, getpid(), static_cast<int>(priority));
#endif

    mPriority = priority;
}

#ifdef TL_OS_WINDOWS

auto Process::formatErrorMsg(unsigned long errorCode) -> std::string
{
    DWORD flags = FORMAT_MESSAGE_FROM_SYSTEM |
                  FORMAT_MESSAGE_IGNORE_INSERTS |
                  FORMAT_MESSAGE_MAX_WIDTH_MASK;

    TCHAR errorMessage[1024] = TEXT("");

    FormatMessage(flags,
                  nullptr,
                  errorCode,
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                  errorMessage,
                  sizeof(errorMessage) / sizeof(TCHAR),
                  nullptr);

    std::string strError = wstringToString(errorMessage);

    return strError;
}


auto Process::createPipe() -> bool
{
    if (!CreatePipe(&pipeReadHandle, &pipeWriteHandle, &mSecurityAttributes, 0)) {
        Message::error("CreateProcess failed ({}) {}", GetLastError(), formatErrorMsg(GetLastError()));
        eventTriggered(Event::Type::task_error);
        return false;
    }

    if (!SetHandleInformation(pipeReadHandle, HANDLE_FLAG_INHERIT, 0)) {
        Message::error("CreateProcess failed ({}) {}", GetLastError(), formatErrorMsg(GetLastError()));
        eventTriggered(Event::Type::task_error);
        return false;
    }

    mStartUpInfo.hStdError = pipeWriteHandle;
    mStartUpInfo.hStdOutput = pipeWriteHandle;

    return true;
}

#endif


} // End namespace tl


