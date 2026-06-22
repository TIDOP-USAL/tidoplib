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


#include "tidop/core/task/Process.h"
#include "tidop/core/task/Progress.h"
#include "tidop/core/base/Exception.h"
#include "tidop/core/base/StringUtils.h"
#include "tidop/core/base/TextEncoding.h"

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
//HANDLE pipeReadHandle = nullptr;
//HANDLE pipeWriteHandle = nullptr;
//
//unsigned long readFromPipe(void *)
//{
//    DWORD bytesRead;
//    std::array<char, process_bufsize + 1> buffer{};
//    int err = 0;
//
//    while (true) {
//        err = ReadFile(pipeReadHandle, buffer.data(), process_bufsize, &bytesRead, nullptr);
//        if (!err || bytesRead == 0) break;
//        std::cout << std::string(buffer.data(), bytesRead);
//    }
//
//    return 0;
//}
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

Process::~Process()
{
#ifdef TL_OS_WINDOWS
    if (mThreadHandle) {
        WaitForSingleObject(mThreadHandle, INFINITE);
        CloseHandle(mThreadHandle);
        mThreadHandle = nullptr;
    }

    if (mProcessInformation.hProcess) {
        CloseHandle(mProcessInformation.hProcess);
        mProcessInformation.hProcess = nullptr;
    }

    if (mProcessInformation.hThread) {
        CloseHandle(mProcessInformation.hThread);
        mProcessInformation.hThread = nullptr;
    }

    if (mPipeReadHandle) {
        CloseHandle(mPipeReadHandle);
        mPipeReadHandle = nullptr;
    }

    if (mPipeWriteHandle) {
        CloseHandle(mPipeWriteHandle);
        mPipeWriteHandle = nullptr;
    }
#endif
}

void Process::execute(Progress *)
{
    try {

#ifdef TL_OS_WINDOWS

        if (outputHandle && !createPipe()) {
            throw std::runtime_error("Failed to create pipe.");
        }

        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        std::wstring command = converter.from_bytes(mCommandText);

        bool success = CreateProcess(nullptr,
                                     const_cast<wchar_t *>(command.c_str()),
                                     nullptr,
                                     nullptr,
                                     /*false,*/ true,
                                     CREATE_NO_WINDOW | static_cast<unsigned long>(mPriority),
                                     nullptr,
                                     nullptr,
                                     &mStartUpInfo,
                                     &mProcessInformation);


        TL_ASSERT(success, "CreateProcess failed ({}) {}", GetLastError(), formatErrorMsg(GetLastError()));

        // Cerrar el extremo de escritura de la tubería del proceso hijo, ya que no lo necesitamos
        if (outputHandle) {
            CloseHandle(mPipeWriteHandle);

            // Crear hilo lector que usará mPipeReadHandle del objeto
            mThreadHandle = CreateThread(nullptr,
                                         0,
                                         &Process::readFromPipe,
                                         this,       // lpParam -> será reinterpretado como Process*
                                         0,
                                         nullptr);

            if (!mThreadHandle) {
                Message::error("CreateThread failed ({}) {}", GetLastError(), formatErrorMsg(GetLastError()));
            }
        }

        DWORD ret = WaitForSingleObject(mProcessInformation.hProcess, INFINITE);
        if (ret == WAIT_OBJECT_0) {

            unsigned long exitCode;

            if (GetExitCodeProcess(mProcessInformation.hProcess, &exitCode)) {
                if (exitCode == 0) {
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

        } else {

            TL_THROW_EXCEPTION("Error ({}: {}) when executing the command: {}",
                               GetLastError(),
                               formatErrorMsg(GetLastError()),
                               mCommandText);

        }

        if (mThreadHandle) {
            // Espera a que el hilo lector termine de leer toda la salida
            WaitForSingleObject(mThreadHandle, INFINITE);
            CloseHandle(mThreadHandle);
            mThreadHandle = nullptr;
        }

        if (mPipeReadHandle) {
            CloseHandle(mPipeReadHandle);
            mPipeReadHandle = nullptr;
        }

#else
        pid_t pid;
        std::unique_ptr<char[]> command(strdup(mCommandText.data()));
        char *argv[] = {const_cast<char *>("sh"), const_cast<char *>("-c"), command.get(), nullptr};

        int status = posix_spawn(&pid, "/bin/sh", nullptr, nullptr, argv, environ);
        
        TL_ASSERT(status == 0, "Error({}: {}) when executing the command : {}", status, strerror(status), mCommandText);

        if (waitpid(pid, &status, 0) != -1) {
            eventTriggered(Event::Type::task_finalized);
        } else {
            eventTriggered(Event::Type::task_error);
        }

        //  int posix_spawn(pid_t *pid, const char *path,
        //                  const posix_spawn_file_actions_t *file_actions,
        //                  const posix_spawnattr_t *attrp,
        //                  char *const argv[], char *const envp[]);
          /// Para escribir en un log la salida
          /// https://unix.stackexchange.com/questions/252901/get-output-of-posix-spawn

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

    std::string strError = toUtf8(errorMessage);

    return strError;
}


auto Process::createPipe() -> bool
{
    // Usar los miembros del objeto
    if (!CreatePipe(&mPipeReadHandle, &mPipeWriteHandle, &mSecurityAttributes, 0)) {
        Message::error("CreateProcess failed ({}) {}", GetLastError(), formatErrorMsg(GetLastError()));
        eventTriggered(Event::Type::task_error);
        return false;
    }

    // Evitar que el proceso hijo herede el extremo de lectura
    if (!SetHandleInformation(mPipeReadHandle, HANDLE_FLAG_INHERIT, 0)) {
        Message::error("CreateProcess failed ({}) {}", GetLastError(), formatErrorMsg(GetLastError()));
        eventTriggered(Event::Type::task_error);
        return false;
    }

    // Usar extremos de escritura para stdout/stderr del hijo
    mStartUpInfo.hStdError = mPipeWriteHandle;
    mStartUpInfo.hStdOutput = mPipeWriteHandle;

    return true;
}

DWORD WINAPI Process::readFromPipe(LPVOID lpParam)
{
    auto *self = reinterpret_cast<Process *>(lpParam);
    if (!self) return 1;

    DWORD bytesRead = 0;
    std::array<char, process_bufsize + 1> buffer{};

    for (;;) {
        BOOL ok = ReadFile(self->mPipeReadHandle,
                           buffer.data(),
                           static_cast<DWORD>(process_bufsize),
                           &bytesRead,
                           nullptr);
        if (!ok || bytesRead == 0) {
            break;
        }
        
        std::cout.write(buffer.data(), bytesRead);
        std::cout.flush();
    }

    return 0;
}

#endif


} // End namespace tl


