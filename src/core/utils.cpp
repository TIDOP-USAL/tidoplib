#include "utils.h"

#include "core/messages.h"
#include "core/console.h"
#include "core/exception.h"

#if defined WIN32
#include <windows.h>
#include <atlstr.h>
#endif

#if defined __linux__ || defined __GNUC__
#include <unistd.h>
#include <sys/stat.h>
#include <libgen.h>
#include <dirent.h>
#endif

#include <iostream>
#include <sstream>
#include <chrono>
#include <vector>
#include <cstring>
#include <exception>
#include <functional>

// Paralelismo
#if defined HAVE_OMP
#  include <omp.h>  // OpenMP
#elif defined (HAVE_PPL) && ( defined (_MSC_VER) && _MSC_VER >= 1600)
#  define I3D_MSVS_CONCURRENCY
#  include <ppl.h>  // Parallel Patterns Library (PPL)
#else
#  include <thread>
#endif


namespace I3D
{

/* ---------------------------------------------------------------------------------- */

const char *getRunfile()
{
  static char runfile[I3D_MAX_PATH];
#if defined WIN32
  ::GetModuleFileNameA(NULL, runfile, I3D_MAX_PATH);
#else
   char szTmp[32];
   sprintf(runfile, "/proc/%d/exe", getpid());
   int len = readlink(szTmp, runfile, I3D_MAX_PATH);
   if(len >= 0)
     runfile[len] = '\0';
#endif
  return runfile;
}

//bool GetAppVersion(std::string *LibName, std::string *CompanyName, std::string *ProductName, std::string *FileVersion,
//  std::string *Copyright, std::string *FileDescription)
//{
//#if defined WIN32
//  DWORD dwHandle, dwLen;
//  UINT BufLen;
//  LPTSTR lpData;
//  unsigned short *LangCharSet;
//  dwLen = GetFileVersionInfoSizeA(LibName->c_str(), &dwHandle);
//  if (!dwLen)   return false;
//  lpData = (LPTSTR)malloc(dwLen);
//  if (!lpData)   return false;
//  if (!GetFileVersionInfoA(LibName->c_str(), dwHandle, dwLen, lpData)) {
//    free(lpData);
//    return false;
//  }
//
//  if (VerQueryValueA(lpData, "\\VarFileInfo\\Translation", (void **)&LangCharSet, &BufLen)) {
//    LPVOID lpBuffer = NULL;
//    char  Prefix[128];
//    sprintf(Prefix, "\\StringFileInfo\\%04x%04x\\", LangCharSet[0], LangCharSet[1]);
//    if (CompanyName && VerQueryValueA(lpData, (Prefix + std::string("CompanyName")).c_str(), &lpBuffer, &BufLen))
//      *CompanyName = (char *)lpBuffer;
//    if (ProductName && VerQueryValueA(lpData, (Prefix + std::string("ProductName")).c_str(), &lpBuffer, &BufLen))
//      *ProductName = (char *)lpBuffer;
//    if (FileVersion && VerQueryValueA(lpData, (Prefix + std::string("FileVersion")).c_str(), &lpBuffer, &BufLen))
//      *FileVersion = (char *)lpBuffer;
//    if (Copyright && VerQueryValueA(lpData, (Prefix + std::string("LegalCopyright")).c_str(), &lpBuffer, &BufLen))
//      *Copyright = (char *)lpBuffer;
//    if (FileDescription && VerQueryValueA(lpData, (Prefix + std::string("FileDescription")).c_str(), &lpBuffer, &BufLen))
//      *FileDescription = (char *)lpBuffer;
//    free(lpData);
//    return true;
//  }
//  free(lpData);
//  return false;
//  
//#else
//  //TODO: ver para Linux
//#endif
//}

bool isDirectory(const char *path)
{
#if defined WIN32
  DWORD ftyp = GetFileAttributesA(path);
  if (ftyp == INVALID_FILE_ATTRIBUTES)
    return false;
  if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
    return true;
#else
  struct stat sb;
  if (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode))
    return true;

#endif
  return false;
}

bool isFile(const char *file)
{
  FILE *fp = std::fopen(file, "rb");
  if (!fp) {
    //msgError("%i: %s", errno, strerror(errno));
    return false;
  } else {
    std::fclose(fp);
    return true;
  } 
}

int createDir(const char *path)
{
  int i_ret = 0;
  if (isDirectory(path)) return 1;

  std::vector<std::string> splitPath;
  I3D::split(path, splitPath, "\\");
  if (splitPath.size() == 1)
    I3D::split(path, splitPath, "/");

  std::string _path = "";
  try {
    for (size_t i = 0; i < splitPath.size(); i++) {
      _path += splitPath[i];
      _path += "\\";
      if (!isDirectory(_path.c_str())) {

#ifdef _MSC_VER

        if (!CreateDirectoryA(_path.c_str(), NULL)) {
          i_ret = -1;
        }

#else

        std::string mkdir = "mkdir \"";
        mkdir.append(_path);
        mkdir.append("\"");
        system(mkdir.c_str());

#endif
      }
    }
  } catch (std::exception &e) {
    msgError(e.what());
    i_ret = -1;
  }

  return i_ret;
}

int deleteDir(const char *path, bool confirm)
{
  int i_ret = 0;
  try {
    if (isDirectory(path)) {
      std::string delDir = "rmdir /s ";
      if (!confirm) delDir += "/q ";
      std::string str = path;
      replaceString(&str, "/", "\\");
      delDir += str;
      system(delDir.c_str());
      i_ret = 0;
    } else {
      i_ret = 1;
    }
  } catch (std::exception &e) {
    msgError(e.what());
    i_ret = -1;
  }
  return i_ret;
}

int move(const char *in, const char *out)
{
#ifdef WIN32

  size_t len = strlen(in) + 1;  
  wchar_t * w_in = new wchar_t[len];  
  size_t convertedChars = 0;  
  mbstowcs_s(&convertedChars, w_in, len, in, _TRUNCATE);

  len = strlen(out) + 1;  
  wchar_t * w_out = new wchar_t[len];  
  convertedChars = 0;  
  mbstowcs_s(&convertedChars, w_out, len, out, _TRUNCATE);

  if (!MoveFileEx(w_in, w_out, MOVEFILE_COPY_ALLOWED | MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH)) {
    //printf("MoveFileEx failed with error %d\n", GetLastError());
    msgError("%s", formatWindowsErrorMsg(GetLastError()).c_str());
    return 1;
  } else {
    //tprintf(TEXT("%s has been moved to %s\n"), argv[1], argv[2]);
    return 0;
  }

#else

#endif
}

int getFileDir(const char *path, char *dir, int size)
{
#ifdef _MSC_VER
  return _splitpath_s(path, NULL, NULL, dir, size, NULL, NULL, NULL, NULL);
#else
  char *dirc = (char *)malloc(size);//strdup(path);
  if (dirc) dir = dirname(dirc);
  return (dir) ? 0 : 1;
#endif
}

int getFileDrive(const char *path, char *drive, int size)
{
  int r_err = 0;
#ifdef _MSC_VER
  r_err = _splitpath_s(path, drive, size, NULL, NULL, NULL, NULL, NULL, NULL);
#else

#endif
  return r_err;
}

int getFileExtension(const char *path, char *ext, int size)
{
  int r_err = 0;
#ifdef _MSC_VER
  r_err = _splitpath_s(path, NULL, NULL, NULL, NULL, NULL, NULL, ext, size);
#else

#endif
  return r_err;
}

int getFileName(const char *path, char *name, int size)
{
#ifdef _MSC_VER
  return _splitpath_s(path, NULL, NULL, NULL, NULL, name, size, NULL, NULL);
#elif defined __GNUC__

#else
  char *basec = (char *)malloc(size);
  if (basec) name = basename(basec);
  return (name) ? 0 : 1;
#endif
}

int getFileDriveDir(const char *path, char *drivedir, int size)
{
  int r_err = 0;
  char drive[I3D_MAX_DRIVE];
  char dir[I3D_MAX_DIR];
#ifdef _MSC_VER
  r_err = _splitpath_s(path, drive, I3D_MAX_DRIVE, dir, I3D_MAX_DIR, NULL, NULL, NULL, NULL);
  strcpy_s(drivedir, size, drive);
  strcat_s(drivedir, size, dir);
#else
  char *dirc = (char *)malloc(size);
  if (dirc) drivedir = dirname(dirc);
  return (drivedir) ? 0 : 1;
#endif
  return r_err;
}

int changeFileName(const char *path, const char *newName, char *pathOut, int size)
{
  int r_err = 0;
  char drive[I3D_MAX_DRIVE];
  char dir[I3D_MAX_DIR];
  char ext[I3D_MAX_EXT];
#ifdef _MSC_VER

  r_err = _splitpath_s(path, drive, I3D_MAX_DRIVE, dir, I3D_MAX_DIR, NULL, NULL, ext, I3D_MAX_EXT);
  if (r_err == 0)
    r_err = _makepath_s(pathOut, size, drive, dir, newName, ext);
#else

#endif
  return r_err;
}

int changeFileExtension(const char *path, const char *newExt, char *pathOut, int size)
{
  int r_err = 0;
  char drive[I3D_MAX_DRIVE];
  char dir[I3D_MAX_DIR];
  char fname[I3D_MAX_FNAME];
#ifdef _MSC_VER

  r_err = _splitpath_s(path, drive, I3D_MAX_DRIVE, dir, I3D_MAX_DIR, fname, I3D_MAX_FNAME, NULL, NULL);
  if (r_err == 0)
    r_err = _makepath_s(pathOut, size, drive, dir, fname, newExt);
#else

#endif
  return r_err;
}

int changeFileNameAndExtension(const char *path, const char *newNameExt, char *pathOut, int size)
{
  int r_err = 0;
  char drive[I3D_MAX_DRIVE];
  char dir[I3D_MAX_DIR];
#ifdef _MSC_VER

  r_err = _splitpath_s(path, drive, I3D_MAX_DRIVE, dir, I3D_MAX_DIR, NULL, NULL, NULL, NULL);
  if (r_err == 0) {
    std::vector<std::string> nameext;
    split(newNameExt, nameext, ".");
    r_err = _makepath_s(pathOut, size, drive, dir, nameext[0].c_str(), nameext[1].c_str());
  }
#else

#endif
  return r_err;
}

void directoryList(const char *directory, std::list<std::string> *dirList)
{
#ifdef _MSC_VER
  WIN32_FIND_DATAA findData;
  HANDLE hFind;

  char dir[I3D_MAX_PATH];
  strcpy(dir, directory);
  strcat(dir, "/*");

  hFind = FindFirstFileA(dir, &findData);
  if (hFind == INVALID_HANDLE_VALUE) {
    msgError("FindFirstFile failed (%d)\n", GetLastError());
    return;
  } else { 
    while (FindNextFileA(hFind, &findData) != 0) {
      if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
        if (strcmp(findData.cFileName, "..") == 0) continue;
        dirList->push_back(findData.cFileName);
      }
    }
    FindClose(hFind);
  }
#else
  //TODO: Sin probar...
  DIR *dir = opendir(directory);

  struct dirent *entry = readdir(dir);

  while (entry != NULL) {
    if (entry->d_type == DT_DIR)
      dirList->push_back(entry->d_name);
      
    entry = readdir(dir);
  }
  closedir(dir);
#endif
}

void fileList(const char *directory, std::list<std::string> *fileList, const char *wildcard)
{
#ifdef _MSC_VER
  WIN32_FIND_DATAA findData;
  HANDLE hFind;

  char dir[I3D_MAX_PATH];
  strcpy(dir, directory);
  strcat(dir, "\\"); // TODO: el tema de las barras aunque a efectos practicos de igual no queda elegante el mezclar los dos tipos de barras...
  strcat(dir, wildcard);

  hFind = FindFirstFileA(dir, &findData);
  if (hFind == INVALID_HANDLE_VALUE) {
    msgError("FindFirstFile failed (%d)\n", GetLastError());
    return;
  } else { 
    fileList->push_back(findData.cFileName);
    while (FindNextFileA(hFind, &findData) != 0) {
      fileList->push_back(findData.cFileName);
    }
    FindClose(hFind);
  }
#else

#endif
}

/* ---------------------------------------------------------------------------------- */

Path::Path() 
  : mPos(0), 
    mPath(0),
    mFileName(""),
    mFileExtension(""),
    bFile(false)
{
}

Path::Path(const std::string &path) 
  : mPos(0), 
    mPath(0),
    mFileName(""),
    mFileExtension(""),
    bFile(false)
{
  parse(path);
}

Path::Path(const Path &path) 
  : mPos(path.mPos), 
    mPath(path.mPath),
    mFileName(path.mFileName),
    mFileExtension(path.mFileExtension),
    bFile(path.bFile)
{ 
}

Path &Path::operator=(const Path &path) 
{
  mPos = path.mPos;
  mPath = path.mPath;
  mFileName = path.mFileName;
  mFileExtension = path.mFileExtension;
  bFile = path.bFile;
  return *this;
}

void Path::parse(const std::string &path)
{
//  char name[I3D_MAX_FNAME];
//  char drive[I3D_MAX_DRIVE];
//  char dir[I3D_MAX_DIR];
//  char ext[I3D_MAX_EXT];
//#ifdef _MSC_VER
//  int r_err = _splitpath_s(path, drive, I3D_MAX_DRIVE, dir, I3D_MAX_DIR, name, I3D_MAX_FNAME, ext, I3D_MAX_EXT);
//
//#endif

  // Se comprueba si es un fichero
  char name[I3D_MAX_FNAME];
  if (getFileName(path.c_str(), name, I3D_MAX_FNAME) == 0) {
    mFileName = name;
    bFile = true;
  }

  // Extensión
  char ext[I3D_MAX_EXT];
  if (getFileExtension(path.c_str(), ext, I3D_MAX_EXT) == 0) {
    mFileExtension = ext;
  }
  
  char drive[I3D_MAX_DRIVE];
  if (getFileDrive(path.c_str(), drive, I3D_MAX_DRIVE) == 0) {
    mDrive = drive;
  }

  char dir[I3D_MAX_DIR];
  if (getFileDir(path.c_str(), dir, I3D_MAX_DIR) == 0) {
    
  }

  split(dir, mPath, "/\\");
  mPos = static_cast<int>(mPath.size());
  if (mPath.size() == 0) return;

  // rutas relativas
  if (mPath[0] == std::string("..")) {
    char dir[I3D_MAX_DIR];
    getFileDriveDir(getRunfile(), dir, I3D_MAX_DIR);
    //std::string runFilePath = getRunfile();
    Path runPath(dir);
    int i = 0;
    for (; mPath[i] == std::string(".."); i++) {
      runPath.down();
    }

    std::vector<std::string> current = runPath.currentPath();
    for (int j = i; j < mPath.size(); j++)
      current.push_back(mPath[j]);
    mPath = current;
    mPos = static_cast<int>(mPath.size());
  } else if (mPath[0] == std::string(".")) {
    char dir[I3D_MAX_DIR];
    getFileDriveDir(getRunfile(), dir, I3D_MAX_DIR);
    Path runPath(dir);
    std::vector<std::string> current = runPath.currentPath();
    for (int j = 1; j < mPath.size(); j++)
      current.push_back(mPath[j]);
    mPath = current;
    mPos = static_cast<int>(mPath.size());
  }
}

#if defined WIN32
const char *Path::getDrive()
{
  //TODO: Esto sólo en Windows...
  //return mPath[0].c_str();
  return mDrive.c_str();
}
#endif

void Path::up()
{
  if (mPos < mPath.size())
    mPos++;
}

void Path::down()
{
  if (mPos != 0)
    mPos--;
}

std::vector<std::string> Path::currentPath()
{
  std::vector<std::string> cur_path;
  for (int i = 0; i < mPos; i++) {
    cur_path.push_back(mPath[i]);
  }
  return cur_path;
}


std::string Path::toString()
{
  std::string _path;
  for (int i = 0; i < mPos; i++) {
    _path.append(mPath[i]);
    if (i < mPos - 1) _path.append("\\");
  }
  return _path;
}

std::list<std::string> Path::files(const std::string &wildcard)
{
  std::list<std::string> files;
  fileList(toString().c_str(), &files, wildcard.c_str());
  return files;
}

std::list<std::string> Path::dirs()
{
  std::list<std::string> dirs;
  directoryList(toString().c_str(), &dirs);
  return dirs;
}

bool Path::isDirectory()
{
  return !bFile;
}

bool Path::isFile()
{
  return bFile;
}

void Path::createDir()
{
  I3D::createDir(toString().c_str());
}

void Path::deleteDir()
{
  I3D::deleteDir(toString().c_str());
}

Path &Path::append(const std::string &dir)
{
  return Path(toString().append("\\").append(dir));
}

/* ---------------------------------------------------------------------------------- */
/*          PROCESOS Y BATCH                                                          */
/* ---------------------------------------------------------------------------------- */

unsigned long Process::sProcessCount = 0;

Process::Process(Process *parentProcess) 
  : mStatus(Status::START),
    mParentProcess(parentProcess),
    mListeners(0) 
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

I3D_DISABLE_WARNING(4100)
Process::Status Process::run(Progress *progressBar)
{
  if (mStatus != Status::FINALIZED) // Util para saltar procesos ya realizados.
    runTriggered();
  return mStatus;
}
I3D_ENABLE_WARNING(4100)

void Process::stop()
{
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

void Process::processCountReset()
{
  sProcessCount = 0;
}

/* ---------------------------------------------------------------------------------- */

DWORD CmdProcess::sPriority = NORMAL_PRIORITY_CLASS;

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

I3D_DISABLE_WARNING(4100)
Process::Status CmdProcess::run(Progress *progressBar)
{
  Process::run();

#ifdef WIN32
  size_t len = strlen(mCmd.c_str());
  std::wstring wCmdLine(len, L'#');
  mbstowcs(&wCmdLine[0], mCmd.c_str(), len);
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
    msgError("Error (%d: %s) al ejecutar el comando: %s", GetLastError(), formatErrorMsg(GetLastError()).c_str(), mCmd.c_str());
    return Process::Status::FINALIZED_ERROR;
  } else if (ret == WAIT_OBJECT_0) {
    msgInfo("Comando ejecutado: %s", mCmd.c_str());
    //return Process::Status::FINALIZED;
  } else if (ret == WAIT_ABANDONED) {
    msgError("Error (%d: %s) al ejecutar el comando: %s", GetLastError(), formatErrorMsg(GetLastError()).c_str(), mCmd.c_str());
    return Process::Status::FINALIZED_ERROR;
  } else if (ret == WAIT_TIMEOUT) {
    msgError("Error (%d: %s) al ejecutar el comando: %s", GetLastError(), formatErrorMsg(GetLastError()).c_str(), mCmd.c_str());
    return Process::Status::FINALIZED_ERROR;
  } /*else {
    msgInfo("Comando ejecutado: %s", mCmd.c_str());
    return Process::Status::FINALIZED;
  }*/
  DWORD exitCode;
  if (GetExitCodeProcess(pi.hProcess, &exitCode) == 0) {
    msgError("Error (%d: %s) al ejecutar el comando: %s", GetLastError(), formatErrorMsg(GetLastError()).c_str(), mCmd.c_str());
    return Process::Status::FINALIZED_ERROR;
  }
  return Process::Status::FINALIZED;
#endif
}
I3D_ENABLE_WARNING(4100)

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
//I3D_ENABLE_WARNING(4100)

/* ---------------------------------------------------------------------------------- */

BatchProcess::BatchProcess()
  : mStatus(Status::START),
    mProcessList(0),
    mCurrentProcess(0),
    _thread()
{}

BatchProcess::BatchProcess(const BatchProcess &batchProcess)
  : mStatus(Status::START),
    mProcessList(batchProcess.mProcessList),
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

  auto f_aux = [&](I3D::Progress *progress_bar_total, I3D::Progress *progress_bar_partial) {
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
        process->run(progress_bar_partial);
        //if (process->run(progress_bar_partial) == Process::Status::FINALIZED_ERROR) {
        //  return Status::FINALIZED_ERROR;
        //} else {
          if (progress_bar_total) (*progress_bar_total)();
        //}
      }
    }
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
  msgInfo("Proceso %i en pausa", id);
}

void BatchProcess::onResume(uint64_t id)
{
  msgInfo("Proceso %i reanudado", id);
}

void BatchProcess::onRun(uint64_t id)
{
  msgInfo("Proceso %i corriendo", id);
}

void BatchProcess::onStart(uint64_t id)
{
  msgInfo("Proceso %i preparado", id);
}

void BatchProcess::onStop(uint64_t id)
{
  msgInfo("Proceso %i detenido", id);
}

void BatchProcess::onEnd(uint64_t id)
{
  msgInfo("Proceso %i finalizado", id);
}

void BatchProcess::onError(uint64_t id)
{
  msgInfo("Proceso %i. Error al procesar", id);
}

/* ---------------------------------------------------------------------------------- */
/*                             Operaciones con cadenas                                */
/* ---------------------------------------------------------------------------------- */

int splitToNumbers(const std::string &cad, std::vector<int> &vOut, const char *chs)
{
  int r_err = 0;
  char *dup = strdup(cad.c_str());
  vOut.resize(0);

  try {
    char *token = strtok(dup, chs);
    while (token != NULL) {
      char *pEnd;
      int number = strtol(token, &pEnd, 10);
      if (*pEnd == 0) {
        vOut.push_back(number);
        token = strtok(NULL, chs);
      } else 
        throw std::runtime_error("Split string to numbers fail");
    }
  } catch (std::exception &e) {
    vOut.resize(0);
    msgError(e.what());
    r_err = 1;
  }

  free(dup);
  return r_err;
}

int splitToNumbers(const std::string &cad, std::vector<double> &vOut, const char *chs)
{
  int r_err = 0;
  char *dup = strdup(cad.c_str());
  vOut.resize(0);

  try {
    char *token = strtok(dup, chs);
    while (token != NULL) {
      //vOut.push_back(atof(token));
      char *pEnd;
      double number = strtod(token, &pEnd);
      if (*pEnd == 0) {
        vOut.push_back(number);
        token = strtok(NULL, chs);
      } else 
        throw std::runtime_error("Split string to numbers fail");
    }
  } catch (std::exception &e) {
    vOut.resize(0);
    msgError(e.what());
    r_err = 1;
  }

  free(dup);
  return r_err;
}

void replaceString(std::string *str, const std::string &str_old, const std::string &str_new)
{
  std::size_t ini = str->find(str_old);
  //std::size_t end;
  while (ini != std::string::npos) {
    //end = ini + str_old.size();
    str->replace(ini, str_old.size(), str_new);
    ini = str->find(str_old, str_new.size() + ini);
  }
}

int split(const std::string &in, std::vector<std::string> &out, const char *chs)
{
  out.resize(0);
  int r_err = 0;
  char *dup;
#ifdef __GNUC__
  size_t len = strlen(in.c_str())+1;
  char *s = (char *)malloc(len);
  if (!s) return 1;
  dup = (char *)memcpy(s, in.c_str(), len);
#else
  dup = strdup(in.c_str());
#endif
  try {
    char *token = strtok(dup, chs);
    while (token != NULL) {
      out.push_back(std::string(token));
      token = strtok(NULL, chs);
    }
  } catch (std::exception &e) {
    msgError(e.what());
    r_err = 1;
  }
  free(dup);
  return r_err;
}

int stringToInteger(const std::string &text, I3D::Base base)
{
  std::istringstream ss(text);
  switch (base) {
  case I3D::Base::OCTAL:
    ss.setf(std::ios_base::oct, std::ios::basefield);
    break;
  case I3D::Base::DECIMAL:
    ss.setf(std::ios_base::dec, std::ios::basefield);
    break;
  case I3D::Base::HEXADECIMAL:
    ss.setf(std::ios_base::hex, std::ios::basefield);
    break;
  default:
    break;
  }
  int number;
  return ss >> number ? number : 0;
}

/* ---------------------------------------------------------------------------------- */
/*                Utilidades de carga y guardado para OpenCV                          */
/* ---------------------------------------------------------------------------------- */

#ifdef HAVE_OPENCV

void loadCameraParams(const std::string &file, cv::Size &imageSize, cv::Mat &cameraMatrix, cv::Mat& distCoeffs)
{
  cv::FileStorage fs(file, cv::FileStorage::READ);
  fs["image_width"] >> imageSize.width;
  fs["image_height"] >> imageSize.height;
  fs["camera_matrix"] >> cameraMatrix;
  fs["distortion_coefficients"] >> distCoeffs;
  fs.release();
}

int loadBinMat(const char *file, cv::Mat *data)
{
  FILE *fp = std::fopen(file, "rb");
  if (!fp) {
    return 1;
  }
  int i_ret = 0;
  //cabecera
  int32_t rows;
  int32_t cols;
  int32_t type;
  try {
    std::fread(&rows, sizeof(int32_t), 1, fp);
    std::fread(&cols, sizeof(int32_t), 1, fp);
    std::fread(&type, sizeof(int32_t), 1, fp);
    //Cuerpo
    cv::Mat aux(rows, cols, type);
    std::fread(aux.data, sizeof(float), rows*cols, fp);
    aux.copyTo(*data);
  } catch (std::exception &e) {
    msgError(e.what());
    i_ret = 1;
  }
  std::fclose(fp);
  return i_ret;
}

int saveBinMat(const char *file, cv::Mat &data)
{
  FILE* fp = std::fopen(file, "wb");
  if (!fp) {
    return 1;
  }
  int i_ret = 0;
  //cabecera
  int32_t rows = data.rows;
  int32_t cols = data.cols;
  int32_t type = data.type();
  try {
    std::fwrite(&data.rows, sizeof(int32_t), 1, fp);
    std::fwrite(&data.cols, sizeof(int32_t), 1, fp);
    std::fwrite(&type, sizeof(int32_t), 1, fp);
    //Cuerpo
    std::fwrite(data.data, sizeof(float), rows*cols, fp);
  } catch (std::exception &e) {
    msgError(e.what());
    i_ret = 1;
  }
  std::fclose(fp);
  return i_ret;
}

#endif // HAVE_OPENCV

/* ---------------------------------------------------------------------------------- */
/*                         Concurrencia, hilos y multiproceso                         */
/* ---------------------------------------------------------------------------------- */

// Añadir cancelación para poder detener los procesos en caso de error

uint32_t getOptimalNumberOfThreads()
{
#ifdef HAVE_OMP
  //TODO: Sin probar
  return omp_get_max_threads();
#elif defined I3D_MSVS_CONCURRENCY
  return Concurrency::CurrentScheduler::Get()->GetNumberOfVirtualProcessors();
#else
  uint32_t n_threads = std::thread::hardware_concurrency();
  return n_threads == 0 ? 1 : n_threads;
#endif
}

void parallel_for(int ini, int end, std::function<void(int)> f)
{
  //uint64_t time_ini = getTickCount();
#ifdef HAVE_OMP
  //TODO: Sin probar
  #pragma omp parallel for
  for (size_t i = ini; i < end; i++) {
    f(i);
  }
#elif defined I3D_MSVS_CONCURRENCY
  Concurrency::cancellation_token_source cts;
  //Concurrency::run_with_cancellation_token([ini, end, f]() {
  //  Concurrency::parallel_for(ini, end, f);
  //},cts.get_token());
  Concurrency::parallel_for(ini, end, f);
#else

  auto f_aux = [&](int ini, int end) {
    for (int r = ini; r < end; r++) {
      f(r);
    }
  };

  int num_threads = getOptimalNumberOfThreads();
  std::vector<std::thread> threads(num_threads);

  int size = (end - ini) / num_threads;
  for (int i = 0; i < num_threads; i++) {
    int _ini = i * size + ini;
    int _end = _ini + size;
    if (i == num_threads -1) _end = end;
    threads[i] = std::thread(f_aux, _ini, _end);
  }

  for (auto &_thread : threads) _thread.join();
#endif
  //double time = (getTickCount() - time_ini) / 1000.;
  //printf("Time %f", time);
}


// Añadir método para ejecutar código de forma asincrona
// std::async
// Concurrency::task<T> (PPL)

/* ---------------------------------------------------------------------------------- */
/*                                 Medición de tiempos                                */
/* ---------------------------------------------------------------------------------- */

uint64_t getTickCount()
{
#if defined _MSC_VER
  return GetTickCount64();
#else
  return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
#endif
}


Chrono::Chrono(const char *msg, bool writeMsg)
  : mTimeIni(0),
    mAccumulated(0),
    mStatus(Chrono::Status::START),
    mMessage(msg),
    bWriteMsg(writeMsg)
{
  //run();
}

Chrono::~Chrono()
{
  if (mStatus == Status::RUNNING || mStatus == Status::PAUSE) {
    stop();
  }
  mStatus = Status::FINALIZED;
}

uint64_t Chrono::pause()
{
  if (mStatus == Status::RUNNING) {
    mAccumulated += getTickCount() - mTimeIni;
    mStatus = Status::PAUSE;
    if (bWriteMsg) msgInfo("Chrono paused");
  }
  return mAccumulated;
}

void Chrono::reset()
{
  mTimeIni = 0;
  mAccumulated = 0;
  mStatus = Status::START;
  mMessage = "";
  if (bWriteMsg) msgInfo("Chrono reset");
}

void Chrono::resume()
{
  if (mStatus == Status::PAUSE) {
    mTimeIni = getTickCount();
    mStatus = Status::RUNNING;
    if (bWriteMsg) msgInfo("Chrono resume");
  }
}

uint64_t Chrono::run()
{
  mTimeIni = getTickCount();
  mAccumulated = 0;
  mStatus = Status::RUNNING;
  if (bWriteMsg) msgInfo("Chrono run");
  return mTimeIni;
}

uint64_t Chrono::stop()
{
  uint64_t time;
  if (mStatus == Status::RUNNING) {
    time = getTickCount() - mTimeIni + mAccumulated;
    mStatus = Status::STOPPED;
  } else if (mStatus == Status::PAUSE) {
    // Puede estar pausado y querer terminar
    mStatus = Status::STOPPED;
    time = mAccumulated;
  } else
    time = 0;
  if (bWriteMsg) msgInfo("%s Time %f (seconds)", mMessage.c_str(), time / 1000.);
  return time;
}

void Chrono::setMessage(const char *msg)
{
  mMessage = msg;
}


/* ---------------------------------------------------------------------------------- */

int readToString(const char *file, std::string *text)
{
  //TODO: Forma muy sencilla para salir de paso por ahora
  std::ifstream in(file);
  *text = static_cast<std::stringstream const&>(std::stringstream() << in.rdbuf()).str();
  //TODO: Por ahora se devuelve 0. Cuando se controlen los errores se devolvera 1 en caso de error
  return 0;
}

VrtTemplate::VrtTemplate() : mText("")
{}

VrtTemplate::VrtTemplate(const char *text) : mText(text)
{}

VrtTemplate::~VrtTemplate()
{}

int VrtTemplate::read(const char *file)
{
  return readToString(file, &mText);
}

int VrtTemplate::replace(std::string *output) const
{
  int i_ret = 0;
  output->append(mText);
  size_t ini = output->find("<#");
  size_t end;
  while (ini != std::string::npos) {
    end = output->find(">", ini);
    size_t size = end - ini + 1;
    std::string tagString = output->substr(ini + 2, end - (ini + 2));
    std::string replaceText;
    replaceTag(tagString, &replaceText);
    output->replace(ini, size, replaceText);
    ini = output->find("<#", replaceText.size());
  }
  return i_ret;
}

void VrtTemplate::setTemplate(const char *templ)
{
  mText = templ;
}


HtmlTemplate::HtmlTemplate()
  : VrtTemplate(),
  mTagValues()
{}

HtmlTemplate::HtmlTemplate(const char *text, const std::map<std::string, std::string> &tag_values)
  : VrtTemplate(text),
  mTagValues(tag_values)
{

}

HtmlTemplate::~HtmlTemplate()
{

}

void HtmlTemplate::replaceTag(const std::string &tag, std::string *replaceText) const
{
  std::map<std::string, std::string>::const_iterator it = mTagValues.find(tag);
  if (it != mTagValues.end())
    *replaceText = it->second;
  else
    *replaceText = "";
}


/* ---------------------------------------------------------------------------------- */

Csv::Csv()
  : File()
{
}

Csv::Csv(const char *file, Mode mode)
  : File(file, mode)
{
  open(file, mode);
}

Csv::Csv(const Csv &csv)
  : File(csv)
{
}

Csv::~Csv()
{
  close();
}

void Csv::close()
{
  if (fs.is_open()) 
    fs.close();
}

//Csv::Status Csv::create(std::shared_ptr<TableHeader> tableHeader)
//{
//  if (!fs.is_open()) {
//    msgError("No se ha abierto el archivo");
//    return Status::FAILURE; 
//  }
//
//  if (mMode != Mode::Create) { 
//    msgError("Utilice el modo 'Create' para abrir el archivo");
//    return Status::FAILURE; 
//  }
//  
//  setName(File::mName.c_str());
//  setTableHeader(tableHeader);
//
//  size_t size = getFieldCount();
//  for (size_t i = 0; i < size; i++) {
//    fs << tableHeader->getTableHeaderField(i)->getName();
//    if (i != size -1) fs << ";";
//  }
//  fs << std::endl;
//
//  return Status::SUCCESS;
//}

Csv::Status Csv::create(const std::string &header)
{
  if (!fs.is_open()) {
    msgError("No se ha abierto el archivo %s", mFile.c_str());
    return Status::FAILURE; 
  }

  if (mMode != Mode::Create) { 
    msgError("Utilice el modo 'Create' al abrir el archivo");
    return Status::FAILURE; 
  }
  
  //setName(File::mName.c_str());
  

  std::vector<std::string> out;
  if (split(header, out, ";") == 0) {
    size_t size = out.size();
    for (size_t i = 0; i < size; i++) {
      fs << out[i];
      if (i != size - 1) fs << ";";
    }
    fs << std::endl;
    return Status::SUCCESS;
  } else 
    return Status::FAILURE; 
}

//Csv::Status Csv::create(const DataTable &dataTable)
//{
//  if (!fs.is_open()) {
//    msgError("No se ha abierto el archivo");
//    return Status::FAILURE; 
//  }
//
//  if (mMode != Mode::Create) { 
//    msgError("Utilice el modo 'Create' para abrir el archivo");
//    return Status::FAILURE; 
//  }
//
//  size_t size = dataTable.getFieldCount();
//
//  // Cabecera
//
//  const TableHeader *header = dataTable.getTableHeader();
//  for (size_t i = 0; i < size; i++) {
//    fs << header->getTableHeaderField(i)->getName();
//    if (i != size -1) fs << ";";
//  }
//  fs << std::endl;
//
//  // datos
//  for (auto &reg : dataTable) {
//    size_t size = getFieldCount();
//    for (size_t i = 0; i < size; i++) {
//      fs << reg->getValue(i);
//      if (i != size -1) fs << ";";
//    }
//    fs << std::endl;
//  }
//
//  return Status::SUCCESS;
//}

Csv::Status Csv::createCopy(const char *fileOut)
{
  Csv csv;
  csv.open(fileOut, Mode::Create);
  //csv.create(std::make_shared<TableHeader>(getTableHeader()));
  return Status::FAILURE;
}

Csv::Status Csv::open(const char *file, Csv::Mode mode)
{
  close();
  
  mFile = file;
  mMode = mode;

  char ext[I3D_MAX_EXT];
  if (getFileExtension(mFile.c_str(), ext, I3D_MAX_EXT) != 0) return Status::OPEN_FAIL;
  if (strcmpi(ext, ".csv") != 0) return Status::OPEN_FAIL;

  std::ios_base::openmode _mode;
  switch (mMode) {
  case Mode::Read:
    _mode = std::fstream::in;
    break;
  case Mode::Update:
    _mode = std::fstream::in | std::fstream::out | std::fstream::app;
    break;
  case Mode::Create:
    _mode = std::fstream::out | std::fstream::trunc;
    break;
  default:
    _mode = std::fstream::in | std::fstream::out;
    break;
  }

  fs.open(file, _mode);

  if (fs.is_open()) {
    if (mMode == Mode::Create) {
      char dir[I3D_MAX_PATH];
      if ( getFileDriveDir(file, dir, I3D_MAX_PATH) == 0 )
        if ( createDir(dir) == -1) return Status::OPEN_FAIL;
    }
    return Status::OPEN_OK;
  } else {
    msgError("File open failed: %s", std::strerror(errno));
    return Status::OPEN_FAIL;
  }
}



//TableRegister *Csv::read(int id)
//{
//  return getRegister(id).get();
//}

//Csv::Status Csv::readHeader()
//{
//  return Status::SUCCESS;
//}

//Csv::Status Csv::write(std::shared_ptr<TableRegister> _register)
//{
//  addRegister(_register);
//
//  TableRegister *reg = getRegister(size()-1).get();
//  size_t size = getFieldCount();
//  for (size_t i = 0; i < size; i++) {
//    fs << reg->getValue(i);
//    if (i != size -1) fs << ";";
//  }
//  fs << std::endl;
//  return Status::SUCCESS;
//}

Csv::Status Csv::write(const std::vector<std::string> &_register)
{
  size_t size = _register.size();
  for (size_t i = 0; i < size; i++) {
    fs << _register[i];
    if (i != size -1) fs << ";";
  }
  fs << std::endl;
  return Status::SUCCESS;
}

//Csv::Status Csv::load(std::shared_ptr<TableRegister> _register)
//{
//
//  return Status::SUCCESS;
//}
//
//Csv::Status Csv::save()
//{
//
//  return Status::SUCCESS;
//}


//void compressFile(const char *file, const char *zip)
//{
//  std::string cmd;
//  std::string pto_gen_cmd("/c \"C:\\Desarrollo\\Libs_sources\\lzma1701\\bin\\x64\\7za.exe\" a ");
//  pto_gen_cmd.append(zip).append(" ").append(file);
//
//  CmdProcess process(cmd);
//  CmdProcess::Status status = process.run();
//}


//CompressFile::CompressFile()
//  : File()
//{
//
//}
//
//CompressFile::CompressFile(const char *file, Mode mode)
//  : File(file, mode)
//{
//  open(file, mode);
//}
//
//CompressFile::CompressFile(const CompressFile &compressFile)
//  : File(compressFile)
//{
//
//}
//
//CompressFile::~CompressFile()
//{
//  close();
//}
//
//void CompressFile::close()
//{
//
//}
//
//CompressFile::Status CompressFile::createCopy(const char *fileOut)
//{
//
//  return Status::FAILURE;
//}
//
//CompressFile::Status CompressFile::open(const char *file, CompressFile::Mode mode)
//{
//  close();
//  
//  mFile = file;
//  mMode = mode;
//
//  switch (mMode) {
//  case Mode::Read:
//
//    break;
//  case Mode::Update:
//
//    break;
//  case Mode::Create:
//
//    break;
//  default:
//
//    break;
//  }
//
//  return Status::FAILURE;
//}
//
//void CompressFile::addFile(const char *file)
//{
//
//}



} // End namespace I3D
