#if defined( __USE_QT__ ) || defined( __QT_PRJ__ )
#include <QString>
#include <QStringList>
#endif

#include <windows.h>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "utils.h"
#include "Logger.h"
#include "geometric_entities\segment.h"

namespace I3D
{

/* ---------------------------------------------------------------------------------- */

char *getRunfile()
{
  static char runfile[_MAX_PATH];
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
  int iret = 0;
  if (isDirectory(path)) iret = 1;
  else {
    std::string mkdir = "mkdir ";
    mkdir += "\"";
    mkdir += path;
    mkdir += "\"";
    try {
      system(mkdir.c_str());
    } catch (std::exception &e) {
      printf(e.what());
      return -1;
    }
  }
  return iret;
}
/* ---------------------------------------------------------------------------------- */

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
    printf(e.what());
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
    printf(e.what());
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
  return _splitpath_s(path, NULL, NULL, dir, _MAX_DIR, NULL, NULL, NULL, NULL);
}

int getFileDrive(const char *path, char *drive)
{
  int r_err;
  r_err = _splitpath_s(path, drive, _MAX_DRIVE, NULL, NULL, NULL, NULL, NULL, NULL);
  return r_err;
}

int getFileExtension(const char *path, char *ext)
{
  int r_err;
  r_err = _splitpath_s(path, NULL, NULL, NULL, NULL, NULL, NULL, ext, _MAX_EXT);
  return r_err;
}

int getFileName(const char *path, char *name)
{
  int r_err;
  r_err = _splitpath_s(path, NULL, NULL, NULL, NULL, name, _MAX_FNAME, NULL, NULL);
  return r_err;
}

int getFileDriveDir(const char *path, char *drivedir)
{
  int r_err;
  char drive[_MAX_DRIVE];
  char dir[_MAX_DIR];
  r_err = _splitpath_s(path, drive, _MAX_DRIVE, dir, _MAX_DIR, NULL, NULL, NULL, NULL);
  strcpy_s(drivedir, _MAX_DRIVE + _MAX_DIR, drive);
  strcat_s(drivedir, _MAX_DRIVE + _MAX_DIR, dir);
  return r_err;
}

int changeFileName(const char *path, char *newName, char *pathOut)
{
  char drive[_MAX_DRIVE];
  char dir[_MAX_DIR];
  char ext[_MAX_EXT];

  int r_err = _splitpath_s(path, drive, _MAX_DRIVE, dir, _MAX_DIR, NULL, NULL, ext, _MAX_EXT);
  if (r_err == 0 )
    r_err = _makepath_s(pathOut, _MAX_PATH, drive, dir, newName, ext);
  return r_err;
}

int changeFileExtension(const char *path, char *newExt, char *pathOut)
{
  char drive[_MAX_DRIVE];
  char dir[_MAX_DIR];
  char fname[_MAX_FNAME];

  int r_err = _splitpath_s(path, drive, _MAX_DRIVE, dir, _MAX_DIR, fname, _MAX_FNAME, NULL, NULL);
  if (r_err == 0)
    r_err = _makepath_s(pathOut, _MAX_PATH, drive, dir, fname, newExt);
  return r_err;
}

int changeFileNameAndExtension(const char *path, char *newNameExt, char *pathOut)
{
  char drive[_MAX_DRIVE];
  char dir[_MAX_DIR];

  int r_err = _splitpath_s(path, drive, _MAX_DRIVE, dir, _MAX_DIR, NULL, NULL, NULL, NULL);
  if (r_err == 0){
    std::vector<std::string> nameext;
    split(newNameExt, nameext, ".");
    r_err = _makepath_s(pathOut, _MAX_PATH, drive, dir, nameext[0].c_str(), nameext[1].c_str());
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
    printf(e.what());
    r_err = 1;
  }
  free(dup);
  return r_err;
}

/* ---------------------------------------------------------------------------------- */

double laplacianVariance(const cv::Mat& src)
{
  cv::Mat laplacian;
  cv::Laplacian(src, laplacian, CV_64F);
  cv::Scalar mean, stddev;
  cv::meanStdDev(laplacian, mean, stddev);
  return (stddev.val[0] * stddev.val[0]);
}

//
//double module(const cv::Point &v)
//{
//  return sqrt(v.x * v.x + v.y*v.y);
//}
//
//
//double vectorAngle(const cv::Point &v1, const cv::Point &v2)
//{
//  if (v1 == cv::Point(0,0) || v2 == cv::Point(0,0))
//    return 0.0;
//  return acos((v1.x*v2.x + v1.y*v2.y) / (module(v1) * module(v2)));
//}

//double vectorAngleOX(const cv::Point &v)
//{
//  double angle = 0.0;
//  if (!(v.x == 0 && v.y == 0))
//    angle = atan2(v.y, v.x);
//  return angle; 
//}
//
//double vectorAngleOY(const cv::Point &v)
//{
//  double angle = 0.0;
//  if (!(v.x == 0 && v.y == 0))
//    angle = atan2(v.x, v.y);
//  return angle;
//}
//
//double azimut(const cv::Point &pt1, const cv::Point &pt2)
//{
//  double azimut = 0.;
//  cv::Point v = pt2 - pt1;
//  if (v.x == 0 && v.y == 0) return azimut;
//  azimut = atan2(v.x, v.y);
//  if (azimut < 0.) azimut += CV_PI * 2.;
//  return azimut;
//}

// Mediana

double computeMedian(const std::vector<double> &input) {
  size_t size = input.size();
  if (size % 2 == 0)
    return (input[size / 2 - 1] + input[size / 2]) / 2;
  else
    return input[size / 2];
}

double computeTempMAD(const std::vector<double> &input, const double median)
{
  std::vector<double> inp = input;
  for (size_t i = 0; i < inp.size(); ++i) {
    inp[i] = abs(inp[i] - median);
  }
  sort(inp.begin(), inp.end());
  return computeMedian(inp)*1.4826;
}

bool isOutlier(const double temp, const double median, const double mad)
{
  if ((abs(temp - median) / mad)>2) {
    return true;
  } else {
    return false;
  }
}

double regressionLinearYX(const std::vector<cv::Point2i> &pts, double *m, double *b)
{
  double corr = 0.0;
  double sx = 0, sy = 0, sx2 = 0, sy2 = 0, sxy = 0;
  size_t n = pts.size();
  if (n >= 2) {
    for (int i = 0; i < n; i++) {
      sx += pts[i].x;
      sy += pts[i].y;
      sx2 += pts[i].x * pts[i].x;
      sy2 += pts[i].y * pts[i].y;
      sxy += pts[i].x * pts[i].y;
    }
    double den = (n*sx2 - sx*sx);
    if (den) {
      // Línea no vertical
      *m = (n*sxy - sy*sx) / (n*sx2 - sx*sx);
      *b = (sy - *m*sx) / n;
      den = sy2 - sy*sy / n;
      corr = sqrt(*m * (sxy - sx*sy / n) / (sy2 - sy*sy / n));
    }
  }
  return(corr);
}

double regressionLinearXY(const std::vector<cv::Point2i> &pts, double *m, double *b)
{
  double corr = 0.0;
  double sx = 0, sy = 0, sx2 = 0, sy2 = 0, sxy = 0;
  int n = static_cast<int>(pts.size());
  if (n >= 2) {
    for (int i = 0; i < n; i++)
      sx += pts[i].x, sy += pts[i].y, sx2 += pts[i].x * pts[i].x, sy2 += pts[i].y * pts[i].y, sxy += pts[i].x * pts[i].y;
    double den = (n*sy2 - sy*sy);
    if (den) {
      *m = (n*sxy - sy*sx) / (n*sy2 - sy*sy);
      *b = (sx - *m*sy) / n;
      den = sx2 - sx*sx / n;
      corr = sqrt(*m * (sxy - sx*sy / n) / (sx2 - sx*sx / n));
    }
  }
  return(corr);
}

/* ---------------------------------------------------------------------------------- */

int sortMatRows(const cv::Mat &in, cv::Mat *out, cv::Mat *idx)
{
  int iret = 1;
  if (in.empty() || in.channels() != 1) iret = -1;
  try {
    cv::sortIdx(in, *idx, CV_SORT_EVERY_ROW + CV_SORT_ASCENDING);
    cv::sort(in, *out, CV_SORT_EVERY_ROW + CV_SORT_ASCENDING);
  } catch (std::exception &e) {
    printf(e.what());
    iret = -1;
  }
  return iret;
}

int sortMatCols(const cv::Mat &in, cv::Mat *out, cv::Mat *idx )
{
  int iret = 1;
  if (in.empty() || in.channels() != 1) iret = -1;
  try {
    cv::sortIdx(in, *idx, CV_SORT_EVERY_COLUMN + CV_SORT_ASCENDING);
    cv::sort(in, *out, CV_SORT_EVERY_COLUMN + CV_SORT_ASCENDING);
  } catch (std::exception &e) {
    printf(e.what());
    iret = -1;
  }
  return iret;
}



/* ---------------------------------------------------------------------------------- */



double degreesToDecimalDegrees(int degrees, int minutes, int seconds)
{
  return isNegative(degrees) * (abs(degrees) + minutes / 60. + seconds / 3600.);
}

double degreesToRadians(int degrees, int minutes, int seconds)
{
  return decimalDegreesToRadians(degreesToDecimalDegrees(degrees, minutes, seconds));
}

double degreesToGradians(int degrees, int minutes, int seconds)
{
  return decimalDegreesToGradians(degreesToDecimalDegrees(degrees, minutes, seconds));
}

void decimalDegreesToDegrees(double decimalDegrees, int *degrees, int *minutes, int *seconds)
{
  *degrees = static_cast<int>(decimalDegrees);
  double min = fabs(decimalDegrees - *degrees) * 60.;
  *minutes = static_cast<int>(min);
  *seconds = static_cast<int>(round(fabs(min - *minutes) * 60.));
}

double decimalDegreesToRadians(double decimalDegrees)
{
  return decimalDegrees * I3D_PI / 180.;
}

double decimalDegreesToGradians(double decimalDegrees)
{
  return decimalDegrees * 200. / 180.;
}

void radiansToDegrees(double rad, int *degrees, int *minutes, int *seconds)
{
  decimalDegreesToDegrees(radiansToDecimalDegrees(rad), degrees, minutes, seconds);
}

double radiansToDecimalDegrees(double radians)
{ 
  return radians * 180. / I3D_PI;
}

double radiansToGradians(double radians)
{
  return radians * 200. / I3D_PI;
}

void gradiansToDegrees(double gradians, int *degrees, int *minutes, int *seconds)
{
  decimalDegreesToDegrees(gradiansToDecimalDegrees(gradians), degrees, minutes, seconds);
}

double gradiansToDecimalDegrees(double gradians)
{
  return gradians * 180. / 200.;
}

double gradiansToRadians(double gradians)
{
  return gradians * I3D_PI / 200;
}




} // End namespace I3D
