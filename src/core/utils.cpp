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

int splitToNumbers(const std::string &cad, std::vector<double> &vOut, char *chs)
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

int getFileDir(const char *path, char *dir)
{
#ifdef _MSC_VER
  return _splitpath_s(path, NULL, NULL, dir, I3D_MAX_DIR, NULL, NULL, NULL, NULL);
#else
  char *dirc = strdup(path);
  dir = dirname(dirc);
  return (dir) ? 0 : 1;
#endif
}

int getFileDrive(const char *path, char *drive)
{
  int r_err;
#ifdef _MSC_VER
  r_err = _splitpath_s(path, drive, I3D_MAX_DRIVE, NULL, NULL, NULL, NULL, NULL, NULL);
#else

#endif
  return r_err;
}

int getFileExtension(const char *path, char *ext)
{
  int r_err;
#ifdef _MSC_VER
  r_err = _splitpath_s(path, NULL, NULL, NULL, NULL, NULL, NULL, ext, I3D_MAX_EXT);
#else

#endif
  return r_err;
}

int getFileName(const char *path, char *name)
{
  int r_err;
#ifdef _MSC_VER
  r_err = _splitpath_s(path, NULL, NULL, NULL, NULL, name, I3D_MAX_FNAME, NULL, NULL);
#else
  char *basec = strdup(path);
  name = basename(basec);
  return (name) ? 0 : 1;
#endif
  return r_err;
}

int getFileDriveDir(const char *path, char *drivedir)
{
  int r_err;
  char drive[I3D_MAX_DRIVE];
  char dir[I3D_MAX_DIR];
#ifdef _MSC_VER
  r_err = _splitpath_s(path, drive, I3D_MAX_DRIVE, dir, I3D_MAX_DIR, NULL, NULL, NULL, NULL);
  strcpy_s(drivedir, I3D_MAX_DRIVE + I3D_MAX_DIR, drive);
  strcat_s(drivedir, I3D_MAX_DRIVE + I3D_MAX_DIR, dir);
#else

#endif
  return r_err;
}

int changeFileName(const char *path, char *newName, char *pathOut)
{
  int r_err = 0;
  char drive[I3D_MAX_DRIVE];
  char dir[I3D_MAX_DIR];
  char ext[I3D_MAX_EXT];
#ifdef _MSC_VER

  r_err = _splitpath_s(path, drive, I3D_MAX_DRIVE, dir, I3D_MAX_DIR, NULL, NULL, ext, I3D_MAX_EXT);
  if (r_err == 0 )
    r_err = _makepath_s(pathOut, I3D_MAX_PATH, drive, dir, newName, ext);
#else

#endif
  return r_err;
}

int changeFileExtension(const char *path, const char *newExt, char *pathOut)
{
  int r_err = 0;
  char drive[I3D_MAX_DRIVE];
  char dir[I3D_MAX_DIR];
  char fname[I3D_MAX_FNAME];
#ifdef _MSC_VER

  r_err = _splitpath_s(path, drive, I3D_MAX_DRIVE, dir, I3D_MAX_DIR, fname, I3D_MAX_FNAME, NULL, NULL);
  if (r_err == 0)
    r_err = _makepath_s(pathOut, I3D_MAX_PATH, drive, dir, fname, newExt);
#else

#endif
  return r_err;
}

int changeFileNameAndExtension(const char *path, char *newNameExt, char *pathOut)
{
  int r_err = 0;
  char drive[I3D_MAX_DRIVE];
  char dir[I3D_MAX_DIR];
#ifdef _MSC_VER

  r_err = _splitpath_s(path, drive, I3D_MAX_DRIVE, dir, I3D_MAX_DIR, NULL, NULL, NULL, NULL);
  if (r_err == 0){
    std::vector<std::string> nameext;
    split(newNameExt, nameext, ".");
    r_err = _makepath_s(pathOut, I3D_MAX_PATH, drive, dir, nameext[0].c_str(), nameext[1].c_str());
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

} // End namespace I3D
