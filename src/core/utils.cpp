#include "utils.h"

#include "core\messages.h"

#include <windows.h>
#include <vector>

namespace I3D
{

/* ---------------------------------------------------------------------------------- */

char *getRunfile()
{
  static char runfile[I3D_MAX_PATH];
  ::GetModuleFileNameA(NULL, runfile, sizeof(runfile));
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
  DWORD ftyp = GetFileAttributesA(path);
  if (ftyp == INVALID_FILE_ATTRIBUTES)
    return false;
  if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
    return true;
  return false;
}

int createDir(const char *path)
{
  if (isDirectory(path)) return 1;
  std::vector<std::string> splitPath;
  split(path,splitPath,"\\");
  if (splitPath.size() == 1) 
    split(path,splitPath,"/");
  
  std::string _path = "";
  for (int i = 0; i < splitPath.size(); i++) {
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

int splitToNumbers(const std::string &cad, std::vector<int> &vOut, char *chs)
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
      } else throw std::exception("Split string to numbers fail\n");
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
      } else throw std::exception("Split string to numbers fail\n");
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
  std::size_t end;
  while (ini != std::string::npos) {
    end = ini + str_old.size();
    str->replace(ini, str_old.size(), str_new);
    ini = str->find(str_old, str_new.size() + ini);
  }
}

int getFileDir(const char *path, char *dir)
{
  return _splitpath_s(path, NULL, NULL, dir, I3D_MAX_DIR, NULL, NULL, NULL, NULL);
}

int getFileDrive(const char *path, char *drive)
{
  int r_err;
  r_err = _splitpath_s(path, drive, I3D_MAX_DRIVE, NULL, NULL, NULL, NULL, NULL, NULL);
  return r_err;
}

int getFileExtension(const char *path, char *ext)
{
  int r_err;
  r_err = _splitpath_s(path, NULL, NULL, NULL, NULL, NULL, NULL, ext, I3D_MAX_EXT);
  return r_err;
}

int getFileName(const char *path, char *name)
{
  int r_err;
  r_err = _splitpath_s(path, NULL, NULL, NULL, NULL, name, I3D_MAX_FNAME, NULL, NULL);
  return r_err;
}

int getFileDriveDir(const char *path, char *drivedir)
{
  int r_err;
  char drive[I3D_MAX_DRIVE];
  char dir[I3D_MAX_DIR];
  r_err = _splitpath_s(path, drive, I3D_MAX_DRIVE, dir, I3D_MAX_DIR, NULL, NULL, NULL, NULL);
  strcpy_s(drivedir, I3D_MAX_DRIVE + I3D_MAX_DIR, drive);
  strcat_s(drivedir, I3D_MAX_DRIVE + I3D_MAX_DIR, dir);
  return r_err;
}

int changeFileName(const char *path, char *newName, char *pathOut)
{
  char drive[I3D_MAX_DRIVE];
  char dir[I3D_MAX_DIR];
  char ext[I3D_MAX_EXT];

  int r_err = _splitpath_s(path, drive, I3D_MAX_DRIVE, dir, I3D_MAX_DIR, NULL, NULL, ext, I3D_MAX_EXT);
  if (r_err == 0 )
    r_err = _makepath_s(pathOut, I3D_MAX_PATH, drive, dir, newName, ext);
  return r_err;
}

int changeFileExtension(const char *path, char *newExt, char *pathOut)
{
  char drive[I3D_MAX_DRIVE];
  char dir[I3D_MAX_DIR];
  char fname[I3D_MAX_FNAME];

  int r_err = _splitpath_s(path, drive, I3D_MAX_DRIVE, dir, I3D_MAX_DIR, fname, I3D_MAX_FNAME, NULL, NULL);
  if (r_err == 0)
    r_err = _makepath_s(pathOut, I3D_MAX_PATH, drive, dir, fname, newExt);
  return r_err;
}

int changeFileNameAndExtension(const char *path, char *newNameExt, char *pathOut)
{
  char drive[I3D_MAX_DRIVE];
  char dir[I3D_MAX_DIR];

  int r_err = _splitpath_s(path, drive, I3D_MAX_DRIVE, dir, I3D_MAX_DIR, NULL, NULL, NULL, NULL);
  if (r_err == 0){
    std::vector<std::string> nameext;
    split(newNameExt, nameext, ".");
    r_err = _makepath_s(pathOut, I3D_MAX_PATH, drive, dir, nameext[0].c_str(), nameext[1].c_str());
  }
  return r_err;
}

int split(const std::string &in, std::vector<std::string> &out, char *chs)
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

} // End namespace I3D
