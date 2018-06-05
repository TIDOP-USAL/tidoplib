#include "core/process.h"

#include "core/messages.h"
#include "core/console.h"
#include "core/exception.h"

#if defined WIN32
#include <windows.h>
#include <atlstr.h>
#endif

/* ---------------------------------------------------------------------------------- */
/*          PROCESOS Y BATCH                                                          */
/* ---------------------------------------------------------------------------------- */


namespace TL
{

unsigned long Process::sProcessCount = 0;

Process::Process(Process *parentProcess) 
  : mStatus(Status::START),
    mParentProcess(parentProcess),
    mListeners(0),
    mProcessId(0),
    mProcessName("")
{
  if (mParentProcess == nullptr) {
    mProcessId = ++sProcessCount;
  }
}

Process::~Process()
{
  if (mStatus == Status::RUNNING || mStatus == Status::PAUSE || mStatus == Status::PAUSING) {
    stop();
  }
  mStatus = Status::FINALIZED;
}

void Process::addListener(Listener *listener)
{ 
  if (mParentProcess == nullptr) {
    mListeners.push_back(listener);
  }
}

void Process::pause()
{
  mStatus = Status::PAUSING;
}

void Process::removeListener(Listener *listener)
{
  if (!mListeners.empty()) {
    mListeners.remove(listener);
  }
}

void Process::reset()
{
  mStatus = Status::START;
}

void Process::resume()
{
  if (mStatus == Status::PAUSE || mStatus == Status::PAUSING) {
    mStatus = Status::RUNNING;
  }
  resumeTriggered();
}

TL_DISABLE_WARNING(4100)
Process::Status Process::run(Progress *progressBar)
{
  if (mStatus != Status::FINALIZED) // Util para saltar procesos ya realizados.
    runTriggered();
  return mStatus;
}
TL_ENABLE_WARNING(4100)

void Process::stop()
{
  //TODO: porque no estoy llamando a stopTriggered()???
  if (mStatus == Status::RUNNING) {
    mStatus = Status::STOPPED;
  } else if (mStatus == Status::PAUSE || mStatus == Status::PAUSING) {
    mStatus = Status::STOPPED;
  }
}

Process::Status Process::getStatus()
{
  return mStatus;
}

void Process::setStatus(Process::Status status)
{
  mStatus = status;
}

void Process::endTriggered()
{
  mStatus = Status::FINALIZED;
  if (!mListeners.empty()) {
    for (auto &lst : mListeners) {
      lst->onEnd(getProcessId());
    }
  }
}

void Process::pauseTriggered()
{
  mStatus = Status::PAUSE;
  if (!mListeners.empty()) {
    for (auto &lst : mListeners) {
      lst->onPause(getProcessId());
    }
  }
}

void Process::resumeTriggered()
{
  mStatus = Status::RUNNING;
  if (!mListeners.empty()) {
    for (auto &lst : mListeners) {
      lst->onResume(getProcessId());
    }
  }
}

void Process::runTriggered()
{
  mStatus = Status::RUNNING;
  if (!mListeners.empty()) {
    for (auto &lst : mListeners) {
      lst->onRun(getProcessId());
    }
  }
}

void Process::startTriggered()
{
  mStatus = Status::START;
  if (!mListeners.empty()) {
    for (auto &lst : mListeners) {
      lst->onStart(getProcessId());
    }
  }
}

void Process::stopTriggered()
{
  mStatus = Status::STOPPED;
  if (!mListeners.empty()) {
    for (auto &lst : mListeners) {
      lst->onStop(getProcessId());
    }
  }
}

void Process::errorTriggered()
{
  mStatus = Status::FINALIZED_ERROR;
  if (!mListeners.empty()) {
    for (auto &lst : mListeners) {
      lst->onError(getProcessId());
    }
  }
}


uint64_t Process::getProcessId() const
{
  return mProcessId;
}

std::string Process::getProcessName() const
{
  return mProcessName;
}

void Process::processCountReset()
{
  sProcessCount = 0;
}

Process::Status Process::checkStatus()
{
  if (mStatus == Status::PAUSING) {
    pauseTriggered();
    while (mStatus == Status::PAUSE);
  } else if (mStatus == Status::STOPPED) {
    // Se fuerza la terminaci�n
    stopTriggered();
  }
  return mStatus;
}


/* ---------------------------------------------------------------------------------- */

#ifdef WIN32
DWORD CmdProcess::sPriority = NORMAL_PRIORITY_CLASS;
#endif

CmdProcess::CmdProcess(const std::string &cmd, Process *parentProcess) 
  : Process(parentProcess),
    mCmd(cmd)
{
#ifdef WIN32
  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);
  ZeroMemory(&pi, sizeof(pi));
#endif
}

CmdProcess::~CmdProcess()
{
#ifdef WIN32
  // Se cierran procesos e hilos 
  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);
#endif
}

TL_DISABLE_WARNING(4100)
Process::Status CmdProcess::run(Progress *progressBar)
{
  Process::run();

#ifdef WIN32
  size_t len = strlen(mCmd.c_str());
  std::wstring wCmdLine(len, L'#');
  //mbstowcs(&wCmdLine[0], mCmd.c_str(), len);
  size_t outSize;
  mbstowcs_s(&outSize, &wCmdLine[0], len, mCmd.c_str(), len);

  LPWSTR cmdLine = (LPWSTR)wCmdLine.c_str();
  if (!CreateProcess(L"C:\\WINDOWS\\system32\\cmd.exe",
    cmdLine,                      // Command line
    NULL,                         // Process handle not inheritable
    NULL,                         // Thread handle not inheritable
    FALSE,                        // Set handle inheritance to FALSE
    CREATE_NO_WINDOW | sPriority, // Flags de creación
    NULL,                         // Use parent's environment block
    NULL,                         // Use parent's starting directory 
    &si,                          // Pointer to STARTUPINFO structure
    &pi)) {                       // Pointer to PROCESS_INFORMATION structure

    msgError("CreateProcess failed (%d) %s", GetLastError(), formatErrorMsg(GetLastError()).c_str());
    return Process::Status::FINALIZED_ERROR;
  }

  DWORD ret = WaitForSingleObject(pi.hProcess, INFINITE);
  if (ret == WAIT_FAILED) {
    msgError("Error (%d: %s) when executing the command: %s", GetLastError(), formatErrorMsg(GetLastError()).c_str(), mCmd.c_str());
    return Process::Status::FINALIZED_ERROR;
  } else if (ret == WAIT_OBJECT_0) {
    msgInfo("Command executed: %s", mCmd.c_str());
    //return Process::Status::FINALIZED;
  } else if (ret == WAIT_ABANDONED) {
    msgError("Error (%d: %s) when executing the command: %s", GetLastError(), formatErrorMsg(GetLastError()).c_str(), mCmd.c_str());
    return Process::Status::FINALIZED_ERROR;
  } else if (ret == WAIT_TIMEOUT) {
    msgError("Error (%d: %s) when executing the command: %s", GetLastError(), formatErrorMsg(GetLastError()).c_str(), mCmd.c_str());
    return Process::Status::FINALIZED_ERROR;
  } /*else {
    msgInfo("Comando ejecutado: %s", mCmd.c_str());
    return Process::Status::FINALIZED;
  }*/
  DWORD exitCode;
  if (GetExitCodeProcess(pi.hProcess, &exitCode) == 0) {
    msgError("Error (%d: %s) when executing the command: %s", GetLastError(), formatErrorMsg(GetLastError()).c_str(), mCmd.c_str());
    return Process::Status::FINALIZED_ERROR;
  }
  return Process::Status::FINALIZED;
#endif
}
TL_ENABLE_WARNING(4100)

void CmdProcess::setPriority(int priority)
{
#ifdef WIN32
  if (priority == 0) {
    sPriority = REALTIME_PRIORITY_CLASS;
  } else if (priority == 1) {
    sPriority = HIGH_PRIORITY_CLASS;
  } else if (priority == 2) {
    sPriority = ABOVE_NORMAL_PRIORITY_CLASS;
  } else if (priority == 3) {
    sPriority = NORMAL_PRIORITY_CLASS;
  } else if (priority == 4) {
    sPriority = BELOW_NORMAL_PRIORITY_CLASS;
  } else if (priority == 5) {
    sPriority = IDLE_PRIORITY_CLASS;
  }
#endif
}

#ifdef WIN32
std::string CmdProcess::formatErrorMsg(DWORD errorCode)
{
  DWORD flags = FORMAT_MESSAGE_FROM_SYSTEM
    | FORMAT_MESSAGE_IGNORE_INSERTS
    | FORMAT_MESSAGE_MAX_WIDTH_MASK;
  
  TCHAR errorMessage[1024] = TEXT("");

  FormatMessage(flags,
                NULL,
                errorCode,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                errorMessage,
                sizeof(errorMessage)/sizeof(TCHAR),
                NULL);

  std::string strError = CW2A(errorMessage);
  return strError;
}
#endif

/* ---------------------------------------------------------------------------------- */


//FunctionProcess::FunctionProcess(std::function<void()> f) 
//  : Process(),
//    f(f)
//{
//
//}
//
//FunctionProcess::~FunctionProcess()
//{
//  // Se cierran procesos e hilos 
//  CloseHandle(pi.hProcess);
//  CloseHandle(pi.hThread);
//}
//
//Process::Status FunctionProcess::run(Progress *progressBar)
//{
//  Process::run();
//  size_t len = strlen(mCmd.c_str());
//  std::wstring wCmdLine(len, L'#');
//  mbstowcs(&wCmdLine[0], mCmd.c_str(), len);
//  LPWSTR cmdLine = (LPWSTR)wCmdLine.c_str();
//  if (!CreateProcess(L"C:\\WINDOWS\\system32\\cmd.exe", cmdLine, NULL,
//    NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
//    printf("CreateProcess failed (%d).\n", GetLastError());
//    return Process::Status::FINALIZED_ERROR;
//  }
//
//  // Wait until child process exits.
//  if (WaitForSingleObject(pi.hProcess, INFINITE) == WAIT_FAILED) {
//    msgError("Error al ejecutar el comando: %s", mCmd.c_str());
//    return Process::Status::FINALIZED_ERROR;
//  } else {
//    msgInfo("Comando ejecutado: %s", mCmd.c_str());
//    return Process::Status::FINALIZED;
//  }
//
//  if (matIn.empty()) return ImgProcessing::Status::INCORRECT_INPUT_DATA;
//  try {
//    f(matIn, matOut);
//  } catch (cv::Exception &e){
//    msgError(e.what());
//    return Process::Status::FINALIZED;
//  }
//  return ImgProcessing::Status::OK;
//}
//TL_ENABLE_WARNING(4100)

/* ---------------------------------------------------------------------------------- */

BatchProcess::BatchProcess()
  : mStatus(Status::START),
    mProcessList(0),
    mListeners(0),
    mCurrentProcess(0),
    _thread()
{}

BatchProcess::BatchProcess(const BatchProcess &batchProcess)
  : mStatus(Status::START),
    mProcessList(batchProcess.mProcessList),
    mListeners(batchProcess.mListeners),
    mCurrentProcess(0),
    _thread()
{
  for (auto process : mProcessList) {
    process->addListener(this);
  }
}

BatchProcess::BatchProcess(std::initializer_list<std::shared_ptr<Process>> procList)
  : mStatus(Status::START),
    mProcessList(procList),
    mCurrentProcess(0),
    _thread()
{
  for (auto process : mProcessList) {
    process->addListener(this);
  }
}

BatchProcess::~BatchProcess()
{
  if (mStatus == Status::RUNNING || mStatus == Status::PAUSE || mStatus == Status::PAUSING) {
    stop();
  }
  mStatus = Status::FINALIZED;  
}

void BatchProcess::add(const std::shared_ptr<Process> &process)
{
  mProcessList.push_back(process);
  process->addListener(this);
}

void BatchProcess::addListener(Listener *listener)
{ 
  mListeners.push_back(listener);
}

void BatchProcess::removeListener(Listener *listener)
{
  if (!mListeners.empty()) {
    mListeners.remove(listener);
  }
}

void BatchProcess::clear()
{
  //mProcessList.clear();
  //mStatus = Status::START;
  reset();
}

void BatchProcess::remove(uint64_t id)
{
  for (std::list<std::shared_ptr<Process>>::iterator it = mProcessList.begin(); it != mProcessList.end(); it++) {
    if ((*it)->getProcessId() == id) {
      remove(*it);
      break;
    }
  }
}

void BatchProcess::remove(const std::shared_ptr<Process> &process)
{
  process->removeListener(this);
  mProcessList.remove(process);
}


bool BatchProcess::isRunning() const
{
  return (mStatus == Status::RUNNING || mStatus == Status::PAUSING || mStatus == Status::PAUSE);
}

void BatchProcess::pause()
{
  mStatus = Status::PAUSING;
  if (mCurrentProcess) {
    mCurrentProcess->pause();
  }
}

void BatchProcess::reset()
{
  //TODO: Si esta corriendo no se puede hacer un reset
  if (mStatus == Status::RUNNING) {
    msgWarning("No se puede hacer un reset mientras el batch esta corriendo. Utilice el método stop() para cancelar los procesos");
  } else {
    mStatus = Status::START;
    mProcessList.clear();
    Process::processCountReset();
  }
}

void BatchProcess::resume()
{
  if (mStatus == Status::PAUSE || mStatus == Status::PAUSING) {
    mStatus = Status::RUNNING;
    if (mCurrentProcess) 
      mCurrentProcess->resume();
  }
}

void BatchProcess::initCounter()
{
  Process::processCountReset();
}

BatchProcess::Status BatchProcess::run(Progress *progressBarTotal, Progress *progressBarPartial)
{
  mStatus = Status::RUNNING;
  if (progressBarTotal) progressBarTotal->init(0., (double)mProcessList.size());
  for (const auto process : mProcessList) {
    if (mStatus == Status::PAUSING) {
      mStatus = Status::PAUSE;
      while (mStatus == Status::PAUSE);
    } else if (mStatus == Status::STOPPED) {
      // Se fuerza la terminación
      return Status::STOPPED;
    } else {
      //if (process->run(progressBarPartial) == Process::Status::FINALIZED_ERROR) {
      //  return Status::FINALIZED_ERROR;
      //} else {
      //  if (progressBarTotal) (*progressBarTotal)();
      //}
      process->run(progressBarPartial);
      if (progressBarTotal) (*progressBarTotal)();
    }
  }
  return (mStatus = Status::FINALIZED);
}

BatchProcess::Status BatchProcess::run_async(Progress *progressBarTotal, Progress *progressBarPartial)
{
  mStatus = Status::RUNNING;

  auto f_aux = [&](TL::Progress *progress_bar_total, TL::Progress *progress_bar_partial) {
    if (progress_bar_total) progress_bar_total->init(0., (double)mProcessList.size());
    for (const auto process : mProcessList) {
      if (progress_bar_total) {
        // Se han añadido nuevos procesos asi que se actualiza
        progress_bar_total->setMaximun((double)mProcessList.size());
        progress_bar_total->updateScale();
      }
      mCurrentProcess = process.get();
      if (mStatus == Status::PAUSING) {
        mStatus = Status::PAUSE;
        while (mStatus == Status::PAUSE);
      } else if (mStatus == Status::STOPPED) {
        // Se fuerza la terminación
        return Status::STOPPED;
      } else {
        //process->run(progress_bar_partial);
        if (process->run(progress_bar_partial) == Process::Status::FINALIZED_ERROR) {
        //  return Status::FINALIZED_ERROR;
          if (progress_bar_partial) progress_bar_partial->restart();
          //TODO: evento de error con el id de proceso
        }
          
        if (progress_bar_total) (*progress_bar_total)();
        
      }
    }
    endTriggered();
    return (mStatus = Status::FINALIZED);
  };

  _thread = std::thread(f_aux, progressBarTotal, progressBarPartial);
  _thread.detach();

  return mStatus;
}


void BatchProcess::stop()
{
  mStatus = Status::STOPPED;
  if (mCurrentProcess) {
    mCurrentProcess->stop();
  }
}

void BatchProcess::onPause(uint64_t id)
{
  msgInfo("Process %i paused", id);
}

void BatchProcess::onResume(uint64_t id)
{
  msgInfo("Process %i resumed", id);
}

void BatchProcess::onRun(uint64_t id)
{
  msgInfo("Process %i running", id);
}

void BatchProcess::onStart(uint64_t id)
{
  msgInfo("Process %i ready", id);
}

void BatchProcess::onStop(uint64_t id)
{
  msgInfo("Process %i stopped", id);
}

void BatchProcess::onEnd(uint64_t id)
{
  msgInfo("Process %i completed", id);
}

void BatchProcess::onError(uint64_t id)
{
  msgInfo("Process %i. Process error", id);
  errorTriggered();
}

void BatchProcess::endTriggered()
{
  mStatus = Status::FINALIZED;
  if (!mListeners.empty()) {
    for (auto &lst : mListeners) {
      lst->onEnd();
    }
  }
}

void BatchProcess::errorTriggered()
{
  mStatus = Status::FINALIZED_ERROR;
  if (!mListeners.empty()) {
    for (auto &lst : mListeners) {
      lst->onError();
    }
  }
}


/* ---------------------------------------------------------------------------------- */


//ProcessBlock::ProcessBlock(int nBlocks, TL::Progress *progressBar)
//  : mCount(nBlocks), 
//    mProgressBar(progressBar)
//{
//  if (mProgressBar) {
//    mProgressBar->init(0, nBlocks);
//  }
//}
//
//ProcessBlock::~ProcessBlock()
//{
//}
//
//void ProcessBlock::next()
//{
//  if (mProgressBar) (*mProgressBar)();
//  if (mStatus == Status::PAUSING) {
//    pauseTriggered();
//    while (mStatus == Status::PAUSE);
//  } else if (mStatus == Status::STOPPED) {
//    // Se fuerza la terminaci�n
//    stopTriggered();
//    return Status::STOPPED;
//  }
//}

} // End namespace TL
