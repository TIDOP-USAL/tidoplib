#include "utils.h"

#include "core/messages.h"

#if defined WIN32
#include <windows.h>
#endif

#if defined __linux__ || defined __GNUC__
#include <unistd.h>
#include <sys/stat.h>
#include <libgen.h>
#endif

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
        printError(e.what());
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
      printError(e.what());
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
    _path += "\\";
  }
  return _path;
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
    printError(e.what());
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
    printError(e.what());
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
    printError(e.what());
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
  int rows;
  int cols;
  int type;
  std::fread(&rows, sizeof(int), 1,fp);
  std::fread(&cols, sizeof(int), 1,fp);
  std::fread(&type, sizeof(int), 1,fp);
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
  int rows = data.rows;
  int cols = data.cols;
  int type = data.type();
  std::fwrite(&data.rows, sizeof(int), 1,fp);
  std::fwrite(&data.cols, sizeof(int), 1,fp);
  std::fwrite(&type, sizeof(int), 1,fp);
  //Cuerpo
  std::fwrite(data.data, sizeof(float), rows*cols, fp);
  std::fclose(fp);
  return 0;
}

#endif // HAVE_OPENCV

/* ---------------------------------------------------------------------------------- */
/*                         Concurrencia, hilos y multiproceso                         */
/* ---------------------------------------------------------------------------------- */

// Añadir cancelación para poder detener los procesos en caso de error

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
    //double cyan, magenta, yellow, key;
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


Chrono::Chrono(bool writeMsg) : mTimeIni(0), mAccumulated(0), mStatus(Chrono::Status::START), bWriteMsg(writeMsg)
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
    return mAccumulated;  
    if (bWriteMsg) consolePrintVerbose("Chrono paused");
  }
}

void Chrono::reset()
{
  mTimeIni = 0;
  mAccumulated = 0;
  mStatus = Status::START;
  if (bWriteMsg) consolePrintVerbose("Chrono reset");
}

void Chrono::resume()
{
  if (mStatus == Status::PAUSE) {
    mTimeIni = getTickCount();
    mStatus = Status::RUNNING;
    if (bWriteMsg) consolePrintVerbose("Chrono resume");
  }
}

uint64_t Chrono::run()
{
  mTimeIni = getTickCount();
  mAccumulated = 0;
  mStatus = Status::RUNNING;
  if (bWriteMsg) consolePrintVerbose("Chrono run");
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
  if (bWriteMsg) consolePrintInfo("Time %f (seconds)", time / 1000.);
  return time;
}

/* ---------------------------------------------------------------------------------- */
/*                        Algoritmos de trazado de lineas                             */
/* ---------------------------------------------------------------------------------- */

PointI &BresenhamLine::operator*()
{
  return mPos;
}

BresenhamLine &BresenhamLine::operator ++()
{
  if (dx > dy) {
    _next(&mPos.x, &mPos.y, mPt2.x, mStepX, mStepY);
  } else {
    _next(&mPos.y, &mPos.x, mPt2.y, mStepY, mStepX);
  }
  return *this;
}

BresenhamLine BresenhamLine::operator ++(int)
{
  BresenhamLine it = *this;
  ++(*this);
  return it;
}

BresenhamLine &BresenhamLine::operator --()
{
  if (dx > dy) {
    _next(&mPos.x, &mPos.y, mPt2.x, -mStepX, -mStepY);
  } else {
    _next(&mPos.y, &mPos.x, mPt2.y, -mStepY, -mStepX);
  }
  return *this;
}

BresenhamLine BresenhamLine::operator --(int)
{
  BresenhamLine it = *this;
  --(*this);
  return it;
}

BresenhamLine BresenhamLine::begin()
{
  BresenhamLine it = *this;
  it.mPos = mPt1;
  return it;
}

BresenhamLine BresenhamLine::end()
{
  BresenhamLine it = *this;
  it.mPos = mPt2;
  return it;
}

PointI BresenhamLine::position(int id)
{
  if (id == -1) {
    return mPos;
  } else {
    BresenhamLine it = begin();
    for ( int i = 0; i < id; i++ ) ++it;//++(*this);
    return it.mPos;
  }
}

/*
//http://rosettacode.org/wiki/Bitmap/Bresenham's_line_algorithm#C
void Line( const float x1, const float y1, const float x2, const float y2, const Color& color )
{
  // Bresenham's line algorithm
  const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
  if(steep)
  {
    std::swap(x1, y1);
    std::swap(x2, y2);
  }

  if(x1 > x2)
  {
    std::swap(x1, x2);
    std::swap(y1, y2);
  }

  const float dx = x2 - x1;
  const float dy = fabs(y2 - y1);

  float error = dx / 2.0f;
  const int ystep = (y1 < y2) ? 1 : -1;
  int y = (int)y1;

  const int maxX = (int)x2;

  for(int x=(int)x1; x<maxX; x++)
  {
    if(steep)
    {
        SetPixel(y,x, color);
    }
    else
    {
        SetPixel(x,y, color);
    }

    error -= dy;
    if(error < 0)
    {
        y += ystep;
        error += dx;
    }
  }
}
*/

void BresenhamLine::init()
{
  if (dy < 0) {
    dy = -dy;
    mStepY = -1;
  } else {
    mStepY = 1;
  }

  if (dx < 0) {
    dx = -dx;
    mStepX = -1;
  } else {
    mStepX = 1;
  }

  if (dx > dy) {
    mCount = dx + 1;
  } else {
    mCount = dy + 1;
  }

  if (dx > dy) {
    p = 2 * dy - dx;
    incE = 2 * dy;
    incNE = 2 * (dy - dx);
  } else {
    p = 2 * dx - dy;
    incE = 2 * dx;
    incNE = 2 * (dx - dy);
  }
}

void BresenhamLine::_next(int *max, int *min, /*int dMax, int dMin,*/ int endMax, int stepMax, int stepMin)
{
  if (*max < endMax) {
    *max += stepMax;
    if (p < 0) {
      p += incE;
    } else {
      *min += stepMin;
      p += incNE;
    }
  }
}

int BresenhamLine::size() const
{
  return mCount;
}

std::vector<PointI> BresenhamLine::getPoints()
{
  std::vector<PointI> pts;
  while ( mPos != mPt2) {
    this->operator++();
    pts.push_back(mPos);
  }
  return pts;
}

/* ---------------------------------------------------------------------------------- */

PointI &DDA::operator*()
{
  return mPos;
}


DDA &DDA::operator ++()
{
  if (dx > dy) {
    _next(&mPos.x, &mPos.y, dy, mPt2.x, mStepX);
  } else {
    _next(&mPos.y, &mPos.x, dx, mPt2.y, mStepY);
  }
  return *this;
}

DDA DDA::operator ++(int)
{
  DDA it = *this;
  ++(*this);
  return it;
}

DDA &DDA::operator --()
{
  if (dx > dy) {
    _next(&mPos.x, &mPos.y, dy, mPt2.x, mStepX);
  } else {
    _next(&mPos.y, &mPos.x, dx, mPt2.y, mStepY);
  }
  return *this;
}

DDA DDA::operator --(int)
{
  DDA it = *this;
  --(*this);
  return it;
}

DDA DDA::begin()
{
  DDA it = *this;
  it.mPos = mPt1;
  return it;
}

DDA DDA::end()
{
  DDA it = *this;
  it.mPos = mPt2;
  return it;
}

PointI DDA::position(int id)
{
  if (id == -1) {
    return mPos;
  } else {
    DDA it = begin();
    for ( int i = 0; i < id; i++ ) ++it;
    return it.mPos;
  }
}

void DDA::init()
{
  if (dx > dy) {
    m = (float)dy / (float)dx;
    b = mPt1.y - m * mPt1.x;
  } else {
    m = (float)dx / (float)dy;
    b = mPt1.x - m * mPt1.y;
  }

  if (dy < 0) {
    dy = -dy;
    mStepY = -1;
  } else {
    mStepY = 1;
  }

  if (dx < 0) {
    dx = -dx;
    mStepX = -1;
  } else {
    mStepX = 1;
  }
}

void DDA::_next(int *max, int *min, int dMin, int endMax, int step)
{
  if (*max < endMax) {
    *max += step;
    if ( dMin != 0)
      *min = I3D_ROUND_TO_INT(m * *max + b);
  }
}

int DDA::size() const
{
  return mCount;
}

std::vector<PointI> DDA::getPoints()
{
  std::vector<PointI> pts;
  while ( mPos != mPt2) {
    this->operator++();
    pts.push_back(mPos);
  }
  return pts;
}

/* ---------------------------------------------------------------------------------- */

/* ---------------------------------------------------------------------------------- */
} // End namespace I3D
