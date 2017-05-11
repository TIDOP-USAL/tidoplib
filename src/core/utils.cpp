#include "utils.h"

#include "core/messages.h"
#include "core/console.h"

#if defined WIN32
#include <windows.h>
#endif

#if defined __linux__ || defined __GNUC__
#include <unistd.h>
#include <sys/stat.h>
#include <libgen.h>
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

//bool GetAppVersion(char *LibName, std::string *CompanyName, std::string *ProductName, std::string *FileVersion,
//  std::string *Copyright, std::string *FileDescription)
//{
//  DWORD dwHandle, dwLen;
//  UINT BufLen;
//  LPTSTR lpData;
//  unsigned short *LangCharSet;
//  dwLen = GetFileVersionInfoSize(LibName, &dwHandle);
//  if (!dwLen)   return false;
//  lpData = (LPTSTR)malloc(dwLen);
//  if (!lpData)   return false;
//  if (!GetFileVersionInfo(LibName, dwHandle, dwLen, lpData)) {
//    free(lpData);
//    return false;
//  }
//
//  if (VerQueryValue(lpData, "\\VarFileInfo\\Translation", (void **)&LangCharSet, &BufLen)) {
//    LPVOID lpBuffer = NULL;
//    char  Prefix[128];
//    sprintf(Prefix, "\\StringFileInfo\\%04x%04x\\", LangCharSet[0], LangCharSet[1]);
//    if (CompanyName && VerQueryValue(lpData, (Prefix + std::string("CompanyName")).c_str(), &lpBuffer, &BufLen))
//      *CompanyName = (char *)lpBuffer;
//    if (ProductName && VerQueryValue(lpData, (Prefix + std::string("ProductName")).c_str(), &lpBuffer, &BufLen))
//      *ProductName = (char *)lpBuffer;
//    if (FileVersion && VerQueryValue(lpData, (Prefix + std::string("FileVersion")).c_str(), &lpBuffer, &BufLen))
//      *FileVersion = (char *)lpBuffer;
//    if (Copyright && VerQueryValue(lpData, (Prefix + std::string("LegalCopyright")).c_str(), &lpBuffer, &BufLen))
//      *Copyright = (char *)lpBuffer;
//    if (FileDescription && VerQueryValue(lpData, (Prefix + std::string("FileDescription")).c_str(), &lpBuffer, &BufLen))
//      *FileDescription = (char *)lpBuffer;
//    free(lpData);
//    return true;
//  }
//  free(lpData);
//  return false;
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

int createDir(const char *path)
{
  if (isDirectory(path)) return 1;
  std::vector<std::string> splitPath;
  I3D::split(path, splitPath, "\\");
  if (splitPath.size() == 1)
    I3D::split(path, splitPath, "/");

  std::string _path = "";
  for (size_t i = 0; i < splitPath.size(); i++) {
    _path += splitPath[i];
    _path += "\\";
    if (!isDirectory(_path.c_str())) {
      std::string mkdir = "mkdir \"";
      mkdir += _path;
      mkdir += "\"";
      try {
        system(mkdir.c_str());
      } catch (std::exception &e) {
        msgError(e.what());
        return -1;
      }
    }
  }
  return 0;
}

int deleteDir(const char *path, bool confirm)
{
  if (isDirectory(path)) {
    std::string delDir = "rmdir /s ";
    if (!confirm) delDir += "/q ";
    std::string str = path;
    replaceString(&str, "/", "\\");
    delDir += str;
    try {
      system(delDir.c_str());
    } catch (std::exception &e) {
      msgError(e.what());
      return -1;
    }
    return 0;
  } else return 1;
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
  if (r_err == 0 )
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
  if (r_err == 0){
    std::vector<std::string> nameext;
    split(newNameExt, nameext, ".");
    r_err = _makepath_s(pathOut, size, drive, dir, nameext[0].c_str(), nameext[1].c_str());
  }
#else

#endif
  return r_err;
}

/* ---------------------------------------------------------------------------------- */

void Path::parse(const std::string &path)
{
  split(path, mPath, "/\\");
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


std::string Path::getDrive() 
{
  return mPath[0];
}

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
    _path += mPath[i];
    if (i < mPos-1) _path += "\\"; 
  }
  return _path;
}

/* ---------------------------------------------------------------------------------- */



/* ---------------------------------------------------------------------------------- */

CmdProcess::CmdProcess(const std::string &cmd) : Process(), mCmd(cmd)
{
  ZeroMemory( &si, sizeof(si) );
  si.cb = sizeof(si);
  ZeroMemory( &pi, sizeof(pi) );
}

CmdProcess::~CmdProcess()
{
  // Se cierran procesos e hilos 
  CloseHandle( pi.hProcess );
  CloseHandle( pi.hThread );
}

int CmdProcess::run(Progress *progressBar)
{
  size_t len = strlen(mCmd.c_str());
  std::wstring wCmdLine(len, L'#');
  mbstowcs(&wCmdLine[0], mCmd.c_str(), len);
  LPWSTR cmdLine = (LPWSTR)wCmdLine.c_str();
  if ( !CreateProcess(L"C:\\WINDOWS\\system32\\cmd.exe", cmdLine, NULL, 
                      NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi ) ) {
    printf( "CreateProcess failed (%d).\n", GetLastError() );
    return 1;
  }

  // Wait until child process exits.
  WaitForSingleObject( pi.hProcess, INFINITE );
  return 0;
}

/* ---------------------------------------------------------------------------------- */

BatchProcess::BatchProcess() 
{

}

BatchProcess::~BatchProcess()
{

}

void BatchProcess::add(const std::shared_ptr<Process> &process)
{
  mProcessList.push_back(process);
}

void BatchProcess::clear()
{ 
  mProcessList.clear();
}

int BatchProcess::run(Progress *progressBarTotal, Progress *progressBarPartial)
{
  if (progressBarTotal) progressBarTotal->init(0., (double)mProcessList.size());
  for (const auto process : mProcessList) {
    if (process->run())
      return 1;
    else {
      if (progressBarTotal) (*progressBarTotal)();
    }
  }
  return 0;
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
    while (token != NULL){
      char *pEnd;
      int number = strtol(token, &pEnd, 10);
      if (*pEnd == 0) {
        vOut.push_back(number);
        token = strtok(NULL, chs);
      } else throw std::runtime_error("Split string to numbers fail\n");
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
    while (token != NULL){
      //vOut.push_back(atof(token));
      char *pEnd;
      double number = strtod(token, &pEnd);
      if (*pEnd == 0) {
        vOut.push_back(number);
        token = strtok(NULL, chs);
      } else throw std::runtime_error("Split string to numbers fail\n");
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
    while (token != NULL){
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

void loadCameraParams(std::string &file, cv::Size &imageSize, cv::Mat &cameraMatrix, cv::Mat& distCoeffs)
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
  FILE* fp = std::fopen(file, "rb");
  if(!fp) {
    return 1;
  }
  //cabecera
  int32_t rows;
  int32_t cols;
  int32_t type;
  std::fread(&rows, sizeof(int32_t), 1,fp);
  std::fread(&cols, sizeof(int32_t), 1,fp);
  std::fread(&type, sizeof(int32_t), 1,fp);
  //Cuerpo
  cv::Mat aux(rows, cols, type);
  std::fread(aux.data, sizeof(float), rows*cols, fp);
  std::fclose(fp);
  aux.copyTo(*data);
  return 0;
}

int saveBinMat(const char *file, cv::Mat &data)
{
  FILE* fp = std::fopen(file, "wb");
  if(!fp) {
    return 1;
  }
  //cabecera
  int32_t rows = data.rows;
  int32_t cols = data.cols;
  int32_t type = data.type();
  std::fwrite(&data.rows, sizeof(int32_t), 1,fp);
  std::fwrite(&data.cols, sizeof(int32_t), 1,fp);
  std::fwrite(&type, sizeof(int32_t), 1,fp);
  //Cuerpo
  std::fwrite(data.data, sizeof(float), rows*cols, fp);
  std::fclose(fp);
  return 0;
}

#endif // HAVE_OPENCV

/* ---------------------------------------------------------------------------------- */
/*                         Concurrencia, hilos y multiproceso                         */
/* ---------------------------------------------------------------------------------- */

// A�adir cancelaci�n para poder detener los procesos en caso de error

uint32_t getOptimalNumberOfThreads()
{
#ifdef I3D_MSVS_CONCURRENCY
  return Concurrency::CurrentScheduler::Get()->GetNumberOfVirtualProcessors();
#else
  uint32_t n_threads = std::thread::hardware_concurrency();
  return n_threads == 0 ? 1 : n_threads;
#endif
}

void parallel_for(int ini, int end, std::function<void(int)> f) {
  //uint64_t time_ini = getTickCount();
#ifdef I3D_MSVS_CONCURRENCY
  //Concurrency::cancellation_token_source cts;
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

// A�adir m�todo para ejecutar c�digo de forma asincrona
// std::async
// Concurrency::task<T> (PPL)

/* ---------------------------------------------------------------------------------- */
/*                                 Medici�n de tiempos                                */
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
  run();
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
    if (bWriteMsg) msgVerbose("Chrono paused");
  }
  return mAccumulated;
}

void Chrono::reset()
{
  mTimeIni = 0;
  mAccumulated = 0;
  mStatus = Status::START;
  mMessage = "";
  if (bWriteMsg) msgVerbose("Chrono reset");
}

void Chrono::resume()
{
  if (mStatus == Status::PAUSE) {
    mTimeIni = getTickCount();
    mStatus = Status::RUNNING;
    if (bWriteMsg) msgVerbose("Chrono resume");
  }
}

uint64_t Chrono::run()
{
  mTimeIni = getTickCount();
  mAccumulated = 0;
  mStatus = Status::RUNNING;
  if (bWriteMsg) msgVerbose("Chrono run");
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
} // End namespace I3D
