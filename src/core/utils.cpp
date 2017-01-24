#include "utils.h"

#include "core/messages.h"
#if defined WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/stat.h>
#include <libgen.h>
#endif

#include <vector>
#include <cstring>
#include <exception>
#include <thread>

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

int changeFileNameAndExtension(const char *path, char *newNameExt, char *pathOut, int size)
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

int split(const std::string &in, std::vector<std::string> &out, const char *chs)
{ 
  out.resize(0);
  int r_err = 0;
  char *dup = strdup(in.c_str());
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

/* ---------------------------------------------------------------------------------- */

void LoadCameraParams(std::string &file, cv::Size &imageSize, cv::Mat &cameraMatrix, cv::Mat& distCoeffs)
{
  cv::FileStorage fs(file, cv::FileStorage::READ);
  fs["image_width"] >> imageSize.width;
  fs["image_height"] >> imageSize.height;
  fs["camera_matrix"] >> cameraMatrix;
  fs["distortion_coefficients"] >> distCoeffs;
  fs.release();
}

/* ---------------------------------------------------------------------------------- */

I3D_EXPORT unsigned int getOptimalNumberOfThreads()
{
  unsigned int n_threads = std::thread::hardware_concurrency();
  return n_threads == 0 ? 1 : n_threads;
}

/* ---------------------------------------------------------------------------------- */



cv::Point &BresenhamLine::operator*()
{
  return mPos;
}

BresenhamLine &BresenhamLine::operator ++()
{
  if (dx > dy) {
    _next(&mPos.x, &mPos.y, /*dx, dy,*/ mPt2.x, mStepX, mStepY);
  } else {
    _next(&mPos.y, &mPos.x, /*dy, dx,*/ mPt2.y, mStepY, mStepX);
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
    _next(&mPos.x, &mPos.y, /*dx, dy,*/ mPt2.x, -mStepX, -mStepY);
  } else {
    _next(&mPos.y, &mPos.x, /*dy, dx,*/ mPt2.y, -mStepY, -mStepX);
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

cv::Point BresenhamLine::position(int id)
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

std::vector<cv::Point> BresenhamLine::getPoints()
{
  std::vector<cv::Point> pts;
  while ( mPos != mPt2) {
    this->operator++();
    pts.push_back(mPos);
  }
  return pts;
}

/* ---------------------------------------------------------------------------------- */

//DDA::DDA()
//{}
//
//DDA::~DDA()
//{}

cv::Point &DDA::operator*()
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

cv::Point DDA::position(int id) 
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

std::vector<cv::Point> DDA::getPoints()
{
  std::vector<cv::Point> pts;
  while ( mPos != mPt2) {
    this->operator++();
    pts.push_back(mPos);
  }
  return pts;
}

/* ---------------------------------------------------------------------------------- */

/* ---------------------------------------------------------------------------------- */
} // End namespace I3D
