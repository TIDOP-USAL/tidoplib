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
 * License along with Foobar. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#include "tidop/core/utils.h"

#include "tidop/core/messages.h"
#include "tidop/core/console.h"
#include "tidop/core/exception.h"


#if defined __linux__ || defined __GNUC__
#include <unistd.h>
#include <sys/stat.h>
#include <libgen.h>
#include <dirent.h>
#endif

#define __STDC_WANT_LIB_EXT1__ 1
#include <ctime>
#include <cstring>

//// Paralelismo
//#if defined HAVE_OMP
//#  include <omp.h>  // OpenMP
//#elif defined (HAVE_PPL) && ( defined (_MSC_VER) && _MSC_VER >= 1600)
//#  define TL_MSVS_CONCURRENCY
//#  include <ppl.h>  // Parallel Patterns Library (PPL)
//#else
//#  include <thread>
//#endif

//TODO: Incluir filesystem. Se simplificarian bastantes cosas
// filesystem
#if (__cplusplus >= 201703L)
//C++17
//http://en.cppreference.com/w/cpp/filesystem
#include <filesystem>
#else
//Boost
//http://www.boost.org/doc/libs/1_66_0/libs/filesystem/doc/index.htm
#include <boost/filesystem.hpp>
#endif
#include <boost/algorithm/string.hpp>
#include "concurrency.h"

#if (__cplusplus >= 201703L)
namespace fs = std::filesystem;
#else
namespace fs = boost::filesystem;
#endif

namespace tl
{

/* ---------------------------------------------------------------------------------- */

const char *getRunfile()
{
  static char runfile[TL_MAX_PATH];
#ifdef WIN32
  ::GetModuleFileNameA(NULL, runfile, TL_MAX_PATH);
#else
   char szTmp[32];
   sprintf(runfile, "/proc/%d/exe", getpid());
   int len = readlink(szTmp, runfile, TL_MAX_PATH);
   if(len >= 0)
     runfile[len] = '\0';
#endif
  return runfile;
}

bool isDirectory(const char *path)
{
#ifdef WIN32
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
bool isDirectory(const std::string &path)
{
  return isDirectory(path.c_str());
}

bool isFile(const char *file)
{
#ifdef WIN32
  FILE *fp;
  errno_t err = fopen_s(&fp, file, "rb");
  if (err != 0) {
    return false;
  } else {
    std::fclose(fp);
    return true;
  }
#else
  struct stat buffer;
  return (stat(file, &buffer) == 0);
#endif
}
bool isFile(const std::string &file)
{
  return isFile(file.c_str());
}

int createDir(const char *path)
{
  int i_ret = 0;
  if (isDirectory(path)) return 1;

  std::vector<std::string> splitPath;
  split(path, splitPath, "\\");
  if (splitPath.size() == 1)
    split(path, splitPath, "/");

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
        if(system(mkdir.c_str())==0)
          i_ret = -1;
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
      if(system(delDir.c_str()))
        i_ret = 0;
      else i_ret = 1;
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

  delete w_in;
  delete w_out;

#else
  ///TODO: Completar
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

#ifdef _MSC_VER
int getFileDrive(const char *path, char *drive, int size)
{
  int r_err = 0;
  r_err = _splitpath_s(path, drive, size, NULL, NULL, NULL, NULL, NULL, NULL);
  return r_err;
}

int getFileExtension(const char *path, char *ext, int size)
{
  int r_err = 0;
  r_err = _splitpath_s(path, NULL, NULL, NULL, NULL, NULL, NULL, ext, size);
  return r_err;
}
#endif

int getFileName(const char *path, char *name, int size)
{
#ifdef _MSC_VER
  return _splitpath_s(path, NULL, NULL, NULL, NULL, name, size, NULL, NULL);
#elif defined __GNUC__
  ///TODO: Completar
#else
  char *basec = (char *)malloc(size);
  if (basec) name = basename(basec);
  return (name) ? 0 : 1;
#endif
}

int getFileDriveDir(const char *path, char *drivedir, int size)
{
  int r_err = 0;
  char drive[TL_MAX_DRIVE];
  char dir[TL_MAX_DIR];
#ifdef _MSC_VER
  r_err = _splitpath_s(path, drive, TL_MAX_DRIVE, dir, TL_MAX_DIR, NULL, NULL, NULL, NULL);
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
  char drive[TL_MAX_DRIVE];
  char dir[TL_MAX_DIR];
  char ext[TL_MAX_EXT];
#ifdef _MSC_VER

  r_err = _splitpath_s(path, drive, TL_MAX_DRIVE, dir, TL_MAX_DIR, NULL, NULL, ext, TL_MAX_EXT);
  if (r_err == 0)
    r_err = _makepath_s(pathOut, size, drive, dir, newName, ext);
#else
  ///TODO: Completar
#endif
  return r_err;
}

#ifdef TL_ENABLE_DEPRECATED_METHODS
int changeFileExtension(const char *path, const char *newExt, char *pathOut, int size)
{
  int r_err = 0;
  char drive[TL_MAX_DRIVE];
  char dir[TL_MAX_DIR];
  char fname[TL_MAX_FNAME];
#ifdef _MSC_VER

  r_err = _splitpath_s(path, drive, TL_MAX_DRIVE, dir, TL_MAX_DIR, fname, TL_MAX_FNAME, NULL, NULL);
  if (r_err == 0)
    r_err = _makepath_s(pathOut, size, drive, dir, fname, newExt);
#else
  ///TODO: Completar
#endif
  return r_err;
}
#endif // TL_ENABLE_DEPRECATED_METHODS

int changeFileNameAndExtension(const char *path, const char *newNameExt, char *pathOut, int size)
{
  int r_err = 0;
  char drive[TL_MAX_DRIVE];
  char dir[TL_MAX_DIR];
#ifdef _MSC_VER

  r_err = _splitpath_s(path, drive, TL_MAX_DRIVE, dir, TL_MAX_DIR, NULL, NULL, NULL, NULL);
  if (r_err == 0) {
    std::vector<std::string> nameext;
    split(newNameExt, nameext, ".");
    r_err = _makepath_s(pathOut, size, drive, dir, nameext[0].c_str(), nameext[1].c_str());
  }
#else
  ///TODO: Completar
#endif
  return r_err;
}

void directoryList(const char *directory, std::list<std::string> *dirList)
{
#ifdef _MSC_VER
  WIN32_FIND_DATAA findData;
  HANDLE hFind;

  char dir[TL_MAX_PATH];
  strcpy_s(dir, TL_MAX_PATH, directory);
  strcat_s(dir, TL_MAX_PATH, "/*");

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

#ifdef TL_ENABLE_DEPRECATED_METHODS
TL_DISABLE_WARNING(TL_WARNING_DEPRECATED)
void fileList(const char *directory, std::list<std::string> *fileList, const char *wildcard)
{
#ifdef _MSC_VER
  WIN32_FIND_DATAA findData;
  HANDLE hFind;

  //char dir[TL_MAX_PATH];
  //strcpy(dir, directory);
  //strcat(dir, "\\"); // TODO: el tema de las barras aunque a efectos practicos de igual no queda elegante el mezclar los dos tipos de barras...
  //strcat(dir, wildcard);
  //Path _path(directory);
  fs::path _path(directory);
  msgInfo(_path.string().c_str());
  _path.append(wildcard);
  msgInfo(_path.string().c_str());
  hFind = FindFirstFileA(/*dir*/ fs::path(directory).append(wildcard).string().c_str(), &findData);

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
   ///TODO: Completar
#endif
}
TL_ENABLE_WARNING(TL_WARNING_DEPRECATED)
#endif // TL_ENABLE_DEPRECATED_METHODS

/// https://stackoverflow.com/questions/1257721/can-i-use-a-mask-to-iterate-files-in-a-directory-with-boost
void fileList(const std::string &directory, std::list<std::string> *fileList, const std::regex &filter)
{

  fs::directory_iterator itr_end;
  for (fs::directory_iterator it(directory); it != itr_end; ++it) {
    // Skip if not a file
    if (!fs::is_regular_file(it->status())) continue;

    std::smatch what;
    std::string fname = it->path().filename().string();
    if (!std::regex_match(fname, what, filter)) continue;

    // File matches, store it
    if (fileList)
      fileList->push_back(it->path().filename().string());
  }

}

void fileListByExt(const std::string &directory, std::list<std::string> *fileList, const std::string &ext)
{

  fs::directory_iterator itr_end;
  for (fs::directory_iterator it(directory); it != itr_end; ++it) {
    // Skip if not a file
    if (!fs::is_regular_file(it->status())) continue;

    fs::path _path = it->path();

    TL_TODO("Compare es case sensitive");
    std::string _ext = it->path().extension().string();

    if (it->path().extension().compare(ext) == 0) {
      // File matches, store it
      if (fileList)
        fileList->push_back(it->path().filename().string());
    }
  }

}




/* ---------------------------------------------------------------------------------- */
/*                             Operaciones con cadenas                                */
/* ---------------------------------------------------------------------------------- */

int splitToNumbers(const std::string &cad, std::vector<int> &vOut, const char *chs)
{
  int r_err = 0;
  char *dup = strdup(cad.c_str()); // -> warning C4996: 'strdup': The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: _strdup
  //char *dup = _strdup(cad.c_str());
  vOut.resize(0);

  try {
    char *token = strtok(dup, chs);
    //char *context = NULL;
    //char *token = strtok_s(dup, chs, &context);
    while (token != nullptr) {
      char *pEnd;
      int number = strtol(token, &pEnd, 10);
      if (*pEnd == 0) {
        vOut.push_back(number);
        token = strtok(nullptr, chs);
        //token = strtok_s(dup, chs, &context);
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
  //char *dup = _strdup(cad.c_str());
  vOut.resize(0);

  try {
    char *token = strtok(dup, chs);
    //char *context = NULL;
    //char *token = strtok_s(dup, chs, &context);
    while (token != NULL) {
      //vOut.push_back(atof(token));
      char *pEnd;
      double number = strtod(token, &pEnd);
      if (*pEnd == 0) {
        vOut.push_back(number);
        token = strtok(nullptr, chs);
        //token = strtok_s(dup, chs, &context);
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
  dup = _strdup(in.c_str());
#endif
  try {
#ifdef __STDC_LIB_EXT1__
    char *context = nullptr;
    char *token = strtok_s(dup, chs, &context);
    while (token != nullptr) {
      out.push_back(std::string(token));
      token = strtok_s(nullptr, chs, &context);
    }
#else
    char *token = strtok(dup, chs);
    while (token != nullptr) {
      out.push_back(std::string(token));
      token = strtok(nullptr, chs);
    }
#endif
  } catch (std::exception &e) {
    msgError(e.what());
    r_err = 1;
  }
  free(dup);
  return r_err;
}

int stringToInteger(const std::string &text, Base base)
{
  std::istringstream ss(text);
  switch (base) {
  case Base::octal:
    ss.setf(std::ios_base::oct, std::ios::basefield);
    break;
  case Base::decimal:
    ss.setf(std::ios_base::dec, std::ios::basefield);
    break;
  case Base::hexadecimal:
    ss.setf(std::ios_base::hex, std::ios::basefield);
    break;
  default:
    break;
  }
  int number;
  return ss >> number ? number : 0;
}

/* ---------------------------------------------------------------------------------- */

//int readToString(const char *file, std::string *text)
//{
//  //TODO: Forma muy sencilla para salir de paso por ahora
//  std::ifstream in(file);
//  *text = static_cast<std::stringstream const&>(std::stringstream() << in.rdbuf()).str();
//  //TODO: Por ahora se devuelve 0. Cuando se controlen los errores se devolvera 1 en caso de error
//  return 0;
//}
//
//VrtTemplate::VrtTemplate() : mText("")
//{}
//
//VrtTemplate::VrtTemplate(const char *text) : mText(text)
//{}
//
//VrtTemplate::~VrtTemplate()
//{}
//
//int VrtTemplate::read(const char *file)
//{
//  return readToString(file, &mText);
//}
//
//int VrtTemplate::replace(std::string *output) const
//{
//  int i_ret = 0;
//  output->append(mText);
//  size_t ini = output->find("<#");
//  size_t end;
//  while (ini != std::string::npos) {
//    end = output->find(">", ini);
//    size_t size = end - ini + 1;
//    std::string tagString = output->substr(ini + 2, end - (ini + 2));
//    std::string replaceText;
//    replaceTag(tagString, &replaceText);
//    output->replace(ini, size, replaceText);
//    ini = output->find("<#", replaceText.size());
//  }
//  return i_ret;
//}
//
//void VrtTemplate::setTemplate(const char *templ)
//{
//  mText = templ;
//}
//
//
//HtmlTemplate::HtmlTemplate()
//  : VrtTemplate(),
//  mTagValues()
//{}
//
//HtmlTemplate::HtmlTemplate(const char *text, const std::map<std::string, std::string> &tag_values)
//  : VrtTemplate(text),
//  mTagValues(tag_values)
//{
//
//}
//
//HtmlTemplate::~HtmlTemplate()
//{
//
//}
//
//void HtmlTemplate::replaceTag(const std::string &tag, std::string *replaceText) const
//{
//  std::map<std::string, std::string>::const_iterator it = mTagValues.find(tag);
//  if (it != mTagValues.end())
//    *replaceText = it->second;
//  else
//    *replaceText = "";
//}
//




} // End namespace tl

