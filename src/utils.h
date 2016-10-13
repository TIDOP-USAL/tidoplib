#ifndef I3D_UTILS_H
#define I3D_UTILS_H

#include <limits>
#include <numeric>
#include <functional>

#include <iostream>
#include <fstream>


#include "opencv2/core/core.hpp"

#include "core\defs.h"

namespace I3D
{

/* ---------------------------------------------------------------------------------- */
/*                           Información de la aplicación                             */
/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Obtiene el path (directorio + nombre + extensión) de la aplicación
 * que esta corriendo
 * \return path de la aplicación
 */
char I3D_EXPORT *getRunfile();


//bool getAppVersion(char *libName, std::string *companyName, std::string *productName,
//                   std::string *fileVersion, std::string *copyright, std::string *fileDescription);

/*!
 * \brief Comprueba si existe un directorio
 * \param[in] path Ruta del directorio
 * \return true si existe.
 */
bool I3D_EXPORT isDirectory(const char *path);

/*!
 * \brief Crea un directorio
 * \param[in] path Ruta del directorio
 * \return Error = -1, creado = 0 y existente = 1
 */
int I3D_EXPORT createDir(const char *path);

/*!
 * \brief Crea un directorio
 * \param[in] path Ruta del directorio
 * \param[in] confirm Pide confirmación para borrar el archivo
 * \return Error = -1, creado = 0 y existente = 1
 */
int I3D_EXPORT deleteDir(const char *path, bool confirm = false);

/* ---------------------------------------------------------------------------------- */
/*                             Operaciones con cadenas                                */
/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Separa una cadena en un array de enteros
 * \param[in] cad Cadena de texto que contiene una lista de numeros
 * \param[out] vOut Vector con los números extraidos
 * \param[in] chs Caracter separador de la cadena
 * \return (1) error
 */
int I3D_EXPORT splitToNumbers(const std::string &cad, std::vector<int> &vOut, char *chs = ",");

/*!
 * \brief Separa una cadena en un array de dobles
 * \param[in] cad Cadena de texto que contiene una lista de numeros
 * \param[out] vOut Vector con los números extraidos
 * \param[in] chs Caracter separador de la cadena
 * \return (1) error
 */
int I3D_EXPORT splitToNumbers(const std::string &cad, std::vector<double> &vOut, char *chs = ",");

/*!
 * \brief Reemplaza una cadena por otra en un texto.
 * \param[in] str Cadena original
 * \param[in] str_old Cadena a remplazar
 * \param[out] str_new Nueva cadena
 */
void I3D_EXPORT replaceString(std::string *str, const std::string &str_old, const std::string &str_new);

/*!
 * \brief Optiene el directorio de un archivo
 * \param[in] path Ruta del archivo
 * \param[out] dir Directorio del archivo
 * \return (1) error
 */
int I3D_EXPORT getFileDir(const char *path, char *dir);

/*!
 * \brief Optiene la unidad de disco de un archivo
 * \param[in] path Ruta del archivo
 * \param[out] drive Unidad de disco
 * \return (1) error
 */
int I3D_EXPORT getFileDrive(const char *path, char *drive);

/*!
 * \brief Optiene la extensión de un archivo
 * \param[in] path Ruta del archivo
 * \param[out] ext Extensión del archivo
 * \return (1) error
 */
int I3D_EXPORT getFileExtension(const char *path, char *ext);

/*!
 * \brief Optiene el nombre de un archivo
 * \param[in] path Ruta del archivo
 * \param[out] name Nombre del archivo
 * \return (1) error
 */
int I3D_EXPORT getFileName(const char *path, char *name);

/*!
 * \brief Optiene el directorio de un archivo incluyendo la letra de la unidad
 * \param[in] path Ruta del archivo
 * \param[out] driveDir Unidad de disco y directorio del archivo
 * \return (1) error
 */
int I3D_EXPORT getFileDriveDir(const char *path, char *driveDir);

/*!
 * \brief Cambia el nombre de un archivo
 * \param[in] path Ruta del archivo
 * \param[in] newName Nuevo nombre
 * \param[out] pathOut Ruta del archivo modificada
 * \return (1) error
 */
int I3D_EXPORT changeFileName(const char *path, char *newName, char *pathOut);

/*!
 * \brief Cambia la extensión de un archivo
 * \param[in] path Ruta del archivo
 * \param[in] newExt Nueva extensión
 * \param[out] pathOut Ruta del archivo modificada
 * \return (1) error
 */
int I3D_EXPORT changeFileExtension(const char *path, char *newExt, char *pathOut);

/*!
 * \brief Cambia el nombre y la extensión de un archivo
 * \param[in] path Ruta del archivo
 * \param[in] newNameExt Nuevo nombre incluyendo la extensión
 * \param[out] pathOut Ruta del archivo modificada
 * \return (1) error
 */
int I3D_EXPORT changeFileNameAndExtension(const char *path, char *newNameExt, char *pathOut);

/*!
 * \brief Separa una cadena
 *
 * Ejemplo:
 * \code
 * char *in = "cadena1,cadena2";
 * std::vector<std::string> out;
 *
 * if ( split(in, &out, ",") == 0 ){
 * ...
 * }
 * \endcode
 * \param[in] in Cadena de entrada
 * \param[out] out vector con las cadenas resultantes
 * \param[in] chs cadena de separación
 * \return (1) error
 */
int I3D_EXPORT split(const std::string &in, std::vector<std::string> &out, char *chs = ",");

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Varianza del laplaciano
 * \param[in] src
 * \return
 */
double I3D_EXPORT laplacianVariance(const cv::Mat &src);

template<typename T>
inline double I3D_EXPORT module(const cv::Point_<T> &v)
{
  return sqrt(v.x * v.x + v.y*v.y);
}

/*!
 * \brief Devuelve el ángulo entre dos vectores
 * \param[in] v1 Vector 1
 * \param[in] v2 Vector 2
 * \return Ángulo en radianes
 */
template<typename T>
inline double I3D_EXPORT vectorAngle(const cv::Point_<T> &v1, const cv::Point_<T> &v2)
{
  if (v1 == cv::Point_<T>(0,0) || v2 == cv::Point_<T>(0,0))
    return 0.0;
  return acos((v1.x*v2.x + v1.y*v2.y) / (module(v1) * module(v2)));
}

template<typename T>
inline double I3D_EXPORT vectorAngleOX(const cv::Point_<T> &v)
{
  double angle = 0.0;
  if (!(v.x == 0 && v.y == 0))
    angle = atan2(v.y, v.x);
  return angle; 
}

template<typename T>
inline double I3D_EXPORT vectorAngleOY(const cv::Point_<T> &v)
{
  double angle = 0.0;
  if (!(v.x == 0 && v.y == 0))
    angle = atan2(v.x, v.y);
  return angle;
}

template<typename T>
inline double I3D_EXPORT azimut(const cv::Point_<T> &pt1, const cv::Point_<T> &pt2)
{
  double azimut = 0.;
  cv::Point v = pt2 - pt1;
  if (v.x == 0 && v.y == 0) return azimut;
  azimut = atan2(v.x, v.y);
  if (azimut < 0.) azimut += CV_PI * 2.;
  return azimut;
}

double I3D_EXPORT computeMedian(const std::vector<double> &input);

double I3D_EXPORT computeTempMAD(const std::vector<double> &input, const double median);

bool I3D_EXPORT isOutlier(const double temp, const double median, const double mad);

/*!
 * \brief Recta de regresión de Y sobre X
 * La recta de regresión de Y sobre X se utiliza para estimar los valores de la Y a partir de los de la X.
 * La pendiente de la recta es el cociente entre la covarianza y la varianza de la variable X.
 * y = m * x + b
 * \param[in] pts Puntos
 * \param[out] m Pendiente de la recta
 * \param[out] b Ordenada
 * \return coeficiente de correlacción
 */
double I3D_EXPORT regressionLinearYX(const std::vector<cv::Point2i> &pts, double *m, double *b);

/*!
 * \brief Recta de regresión de X sobre Y
 * La recta de regresión de X sobre Y se utiliza para estimar los valores de la X a partir de los de la Y.
 * La pendiente de la recta es el cociente entre la covarianza y la varianza de la variable Y.
 * x = m*y + b
 * \param[in] pts Puntos
 * \param[out] m Pendiente de la recta
 * \param[out] b Ordenada
 * \return coeficiente de correlacción
 */
double I3D_EXPORT regressionLinearXY(const std::vector<cv::Point2i> &pts, double *m, double *b);

/* ---------------------------------------------------------------------------------- */
/*                              Operaciones con vectores                              */
/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Convierte una matriz de OpenCV en un vector
 * \param[in] m Matriz de entrada
 * \param[out] av Vector de salida
 */
template<typename T>
void I3D_EXPORT cvMatToVector(const cv::Mat &m, std::vector<T> *av)
{
  av->resize(m.rows*m.cols);
  if (m.isContinuous()) {
    av->assign((T*)m.datastart, (T*)m.dataend);
  } else {
    for (int i = 0; i < m.rows; ++i) {
      av->insert(av->end(), (T*)m.ptr<uchar>(i), (T*)m.ptr<uchar>(i)+m.cols);
    }
  }
}

/*!
 * \brief Ordena un vector de menor a mayor
 * \param[in] v Vector
 */
template<typename T>
void I3D_EXPORT sortVector(std::vector<T> *v)
{
  std::sort(v->begin(), v->end());
}

/*!
 * \brief Ordena un vector de mayor a menor
 * \param[in] v Vector
 */
template<typename T>
void I3D_EXPORT sortVectorInv(std::vector<T> *v)
{
  std::sort(v->rbegin(),v->rend());
}

/*!
 * \brief Determinar el número de elementos iguales a un número.
 * Sobrecarga del operador == para determinar el número de elementos de un
 * vector que son iguales al valor pasado como parámetro.
 * \param[in] v Vector
 * \param[in] t Valor
 * \return Número de elementos que cumplen la condición
 */
template<typename T>
int operator==(const std::vector<T> &v, const T t)
{
  sortVector(v);
  std::pair<std::vector<T>::iterator, std::vector<T>::iterator> bounds;
  bounds = std::equal_range(v.begin(), v.end(), t);
  int ini = bounds.first - v.begin();
  int end = bounds.second - v.begin();
  return ( end - ini );
}

/*!
 * \brief Determina el número de elementos distintos a un número.
 * Sobrecarga del operador != para determinar el número de elementos de un
 * vector que son distintos al valor pasado como parámetro.
 * \param[in] v Vector
 * \param[in] t Valor
 * \return Número de elementos que cumplen la condición
 */
template<typename T>
int operator!=(const std::vector<T> &v, const T t)
{
  sortVector(v);
  std::pair<std::vector<T>::iterator, std::vector<T>::iterator> bounds;
  bounds = std::equal_range(v.begin(), v.end(), t);
  int r1 = bounds.first - v.begin();
  int r2 = v.end() - bounds.second;
  return (r1 + r2 );
}

/*!
 * \brief operator >=
 * \param[in] v
 * \param[in] t
 * \return
 */
template<typename T>
int operator>=(const std::vector<T> &v, const T t)
{
  sortVector(v);
  std::vector<T>::iterator upOrEq;
  upOrEq = std::lower_bound(v.begin(), v.end(), t);
  return (v.end() - upOrEq);
}

template<typename T>
int operator<=(const std::vector<T> &v, const T t)
{
  sortVector(v);
  std::vector<T>::iterator lowOrEq;
  lowOrEq = std::upper_bound(v.begin(), v.end(), t);
  return (lowOrEq - v.begin());
}

template<typename T>
int operator> (const std::vector<T> &v, const T t)
{
  sortVector(v);
  std::vector<T>::iterator up;
  up = std::upper_bound(v.begin(), v.end(), t);
  return (v.end() - up);
}

template<typename T>
int operator< (const std::vector<T> &v, const T t)
{
  sortVector(v);
  std::vector<T>::iterator low;
  low = std::lower_bound(v.begin(), v.end(), t);
  return (low - v.begin());
}

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Ordena los valores de una matriz de mayor a menor por filas
 * \param[in] in
 * \param[out] out
 * \param[out] idx
 */
int I3D_EXPORT sortMatRows(const cv::Mat &in, cv::Mat *out, cv::Mat *idx);

/*!
 * \brief Ordena los valores de una matriz de mayor a menor por columnas
 * \param[in] in
 * \param[out] out
 * \param[out] idx
 */
int I3D_EXPORT sortMatCols(const cv::Mat &in, cv::Mat *out, cv::Mat *idx);

/*!
 * \brief Ordena los indices de un vector de menor a mayor
 * Para un vector [10,20,15,5] devuelve [3,0,2,1]. El elemento mas 
 * pequeño esta en la posición 3, el segundo en la posición 0, ...
 * \param[in] v Vector
 * \return Vector con los indices ordenados
 */
template <typename T>
std::vector<int> I3D_EXPORT sortIdx(const std::vector<T> &v)
{
  std::vector<int> idx(v.size());
  iota(idx.begin(), idx.end(), 0);

  sort(idx.begin(), idx.end(), [&v](int i1, int i2) {return v[i1] < v[i2]; });

  return idx;
}

/* ---------------------------------------------------------------------------------- */
/*                                Conversión de color                                 */
/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Obtiene la componente azul de un color
 * \param[in] color Color representado como un entero
 * \return Componente azul
 */
inline int I3D_EXPORT getBlue(int color)
{ 
  return(color & 0xFF); 
}

/*!
 * \brief Obtiene la componente verde de un color
 * \param[in] color Color representado como un entero
 * \return Componente verde
 */
inline int I3D_EXPORT getGreen(int color)
{ 
  return((color & 0xFF00) >> 8); 
}

/*!
 * \brief Obtiene la componente roja de un color
 * \param[in] color Color representado como un entero
 * \return Componente roja
 */
inline int I3D_EXPORT getRed(int color)
{ 
  return((color & 0xFF0000) >> 16); 
}

/*!
 * \brief Obtiene el canal alfa de un color
 * \param[in] color Color representado como un entero
 * \return Canal alfa
 */
inline int I3D_EXPORT getAlpha(int color)
{ 
  return((color & 0xFF000000) >> 24); 
}

/*!
 * \brief Convierte un color entero a RGB
 * \param[in] color Color como entero
 * \param[out] red Componente roja
 * \param[out] green Componente verde
 * \param[out] blue Componente azul
 */
inline void I3D_EXPORT intToRGB(int color, int *red, int *green, int *blue)
{
  *red = getRed(color);
  *green = getGreen(color);
  *blue = getBlue(color);
}

/*!
 * \brief Convierte un color RGB a entero
 * \param[in] red Componente roja
 * \param[in] green Componente verde
 * \param[in] blue Componente azul
 * \return Color como entero
 */
inline int I3D_EXPORT rgbToInt(int red, int green, int blue)
{
  return((blue & 0xFF) | ((green << 8) & 0xFF00) | ((red << 16) & 0xFF0000));
}

/*!
 * \brief Convierte un color RGB+alpha a entero
 * \param[in] red Componente roja
 * \param[in] green Componente verde
 * \param[in] blue Componente azul
 * \param[in] alpha Canal alfa
 * \return Color como entero
 */
inline int I3D_EXPORT rgbaToInt(int red, int green, int blue, int alpha)
{
  return((blue & 0xFF) | ((green << 8) & 0xFF00) | ((red << 16) & 0xFF0000) | ((alpha << 24) & 0xFF000000));
}

/* ---------------------------------------------------------------------------------- */
/*                               Conversión de ángulos                                */
/* ---------------------------------------------------------------------------------- */

/*! \defgroup formatConversion Conversión de formatos
*  \{

*/

template<typename T>
int isNegative(T t) 
{
  return t < 0 ? -1 : 1;
}

/*!
 * \brief Conversión de grados sexagesimales a grados sexagesimales en notación decimal
 * \param[in] degrees Grados
 * \param[in] minutes Minutos
 * \param[in] seconds Segundos
 * \return Grados sexagesimales en notación decimal
 */
double I3D_EXPORT degreesToDecimalDegrees(int degrees, int minutes, int seconds);

/*!
 * \brief Conversión de grados sexagesimales a radianes
 * \param[in] degrees Grados
 * \param[in] minutes Minutos
 * \param[in] seconds Segundos
 * \return radianes
 */
double I3D_EXPORT degreesToRadians(int degrees, int minutes, int seconds);

/*!
 * \brief Conversión de grados sexagesimales a grados centesimales
 * \param[in] degrees Grados
 * \param[in] minutes Minutos
 * \param[in] seconds Segundos
 * \return Grados centesimales
 */
double I3D_EXPORT degreesToGradians(int degrees, int minutes, int seconds);

/*!
 * \brief Conversión de grados sexagesimales en notación decimal a grados, minutos y segundos
 * \param[in] decimalDegrees Grados sexagesimales en notación decima
 * \param[out] degrees Puntero a entero que recibe como valor los grados
 * \param[out] minutes Puntero a entero que recibe como valor los minutos
 * \param[out] seconds Puntero a entero que recibe como valor los segundos
 */
void I3D_EXPORT decimalDegreesToDegrees(double decimalDegrees, int *degrees, int *minutes, int *seconds);

/*!
 * \brief Conversión de grados sexagesimales en notación decimal a radianes
 * \param[in] decimalDegrees Grados sexagesimales en notación decima
 * \return Radianes
 */
double I3D_EXPORT decimalDegreesToRadians(double decimalDegrees);

/*!
 * \brief Conversión de grados sexagesimales en notación decimal a grados centesimales
 * \param[in] decimalDegrees Grados sexagesimales en notación decima
 * \return Grados centesimales
 */
double I3D_EXPORT decimalDegreesToGradians(double decimalDegrees);

/*!
 * \brief Conversión de radianes a grados, minutos y segundos
 * \param[in] radians Radianes
 * \param[out] degrees Puntero a entero que recibe como valor los grados
 * \param[out] minutes Puntero a entero que recibe como valor los minutos
 * \param[out] seconds Puntero a entero que recibe como valor los segundos
 */
void I3D_EXPORT radiansToDegrees(double radians, int *degrees, int *minutes, int *seconds);

/*!
 * \brief Conversión de radianes a grados sexagesimales en notación decimal
 * \param[in] radians Radianes
 * \return Grados sexagesimales en notación decimal
 */
double I3D_EXPORT radiansToDecimalDegrees(double radians);

/*!
 * \brief radiansToGradians
 * \param[in] radians Radianes
 * \return Grados centesimales
 */
double I3D_EXPORT radiansToGradians(double radians);

/*!
 * \brief Conversión de grados centesimales a grados, minutos y segundos
 * \param[in] gradians Grados centesimales
 * \param[out] degrees Puntero a entero que recibe como valor los grados
 * \param[out] minutes Puntero a entero que recibe como valor los minutos
 * \param[out] seconds Puntero a entero que recibe como valor los segundos
 */
void I3D_EXPORT gradiansToDegrees(double gradians, int *degrees, int *minutes, int *seconds);

/*!
 * \brief Conversión de grados centesimales a grados sexagesimales en notación decimal
 * \param[in] gradians Grados centesimales
 * \return Grados sexagesimales en notación decimal
 */
double I3D_EXPORT gradiansToDecimalDegrees(double gradians);

/*!
 * \brief Conversión de grados centesimales a radianes
 * \param[in] gradians Grados centesimales
 * \return Radianes
 */
double I3D_EXPORT gradiansToRadians(double gradians);

/*! \} */ // end of formatConversion














//http://stackoverflow.com/questions/21657627/what-is-the-type-signature-of-a-c11-1y-lambda-function/21665705#21665705
//http://stackoverflow.com/questions/21738775/c11-how-to-write-a-wrapper-function-to-make-stdfunction-objects

//// For generic types that are functors, delegate to its 'operator()'
//template <typename T>
//struct function_traits
//  : public function_traits<decltype(&T::operator())>
//{};
//
//// for pointers to member function
//template <typename ClassType, typename ReturnType, typename... Args>
//struct function_traits<ReturnType(ClassType::*)(Args...) const> {
//  enum { arity = sizeof...(Args) };
//  typedef std::function<ReturnType(Args...)> f_type;
//};
//
//// for pointers to member function
//template <typename ClassType, typename ReturnType, typename... Args>
//struct function_traits<ReturnType(ClassType::*)(Args...) > {
//  enum { arity = sizeof...(Args) };
//  typedef std::function<ReturnType(Args...)> f_type;
//};
//
//// for function pointers
//template <typename ReturnType, typename... Args>
//struct function_traits<ReturnType(*)(Args...)>  {
//  enum { arity = sizeof...(Args) };
//  typedef std::function<ReturnType(Args...)> f_type;
//};
//
//template <typename L>
//static typename function_traits<L>::f_type make_function(L l){
//  return (typename function_traits<L>::f_type)(l);
//}
//
////handles bind & multiple function call operator()'s
//template<typename ReturnType, typename... Args, class T>
//auto make_function(T&& t)
//-> std::function<decltype(ReturnType(t(std::declval<Args>()...)))(Args...)>
//{
//  return{ std::forward<T>(t) };
//}
//
////handles explicit overloads
//template<typename ReturnType, typename... Args>
//auto make_function(ReturnType(*p)(Args...))
//-> std::function<ReturnType(Args...)> {
//  return{ p };
//}
//
////handles explicit overloads
//template<typename ReturnType, typename... Args, typename ClassType>
//auto make_function(ReturnType(ClassType::*p)(Args...))
//-> std::function<ReturnType(Args...)> {
//  return{ p };
//}





//
//
//class FileRWHandler
//{
//private:
//  std::ofstream h_write;
//  std::ifstream h_read;
//
//public:
//  FileRWHandler()
//  {
//  }
//
//  ~FileRWHandler()
//  {
//  }
//
//  int load(std::string &fileIn) 
//  { 
//    h_read.open(fileIn, std::ios::in | std::ios::binary);
//    if (h_read.is_open()) {
//      read(this);
//      h_read.close();
//      return 1;
//    } else throw std::exception("Open file error");
//    return 0;
//  }
//
//  int save(std::string &fileOut) 
//  {
//    h_write.open(fileOut, std::ios::out | std::ios::binary);
//    if (h_write.is_open()) {
//      write(this);
//      h_write.close();
//      return 1;
//    }
//    return 0;
//  }
//
//  template <typename T>
//  int  read( T *t) 
//  {
//    try {
//      h_read.read((char *)this, sizeof(T));
//      return 1;
//    } catch (std::exception &e){
//      printf(e.what());
//    }
//    return 0;
//  }
//
//  template <typename T>
//  int write(T *t) 
//  {
//    try {
//      h_write.write((char *)t, sizeof(T));
//      return 1;
//    } catch (std::exception &e){
//      printf(e.what());
//    }
//    return 0;
//  }
//
//  int readUnfix()
//  {
//    return 0;
//  }
//
//  int writeUnfix()
//  {
//    return 0;
//  }
//
//};
//
//class prueba : public FileRWHandler
//{
//public:
//  int entero;
//  float flotante;
//  double doble;
//  std::string cadena;
//
//  prueba() : entero(0), flotante(0), doble(0), cadena("") {}
//  prueba(int entero, float flotante, double doble, std::string cadena) : entero(entero), flotante(flotante), doble(doble), cadena(cadena) {}
//};


//
//http://www.cplusplus.com/forum/general/42160/
//#include <iostream>
//#include <fstream>
//#include <sys/stat.h>
//
//using namespace std;
//
//bool FileExists(std::string File)
//{
//  struct stat FileData;
//  bool Exists = false;
//  int FileInfo;
//  FileInfo = stat(File.c_str(), &FileData);
//  if (!FileInfo)Exists = true;
//  return(Exists);
//}
//
///**
//To make it not use pointers remove the * after T and add a & after (char*).
//*/
//
////SaveObject prototype
//
//template <typename T>
//void SaveObject(T* object, std::string filename);
//
////SaveObject defined
//
//template <typename T>
//void SaveObject(T* object, std::string filename)
//{
//  ofstream out(filename.c_str(), ios::binary);
//  out.write((char *)object, sizeof(T));
//  out.close();
//}
//
////LoadObject prototype
//
//template <typename T>
//bool LoadObject(T* object, std::string filename);
//
////LoadObject defined
//
//template <typename T>
//bool LoadObject(T* object, std::string filename)
//{
//  bool Exists = FileExists(filename);
//  if (Exists)
//  {
//    ifstream in(filename.c_str(), ios::binary);
//    if (in.is_open())
//    {
//      in.read((char *)object, sizeof(T));
//      in.close();
//    }
//  } else
//  {
//    cout << "Unable to find \"" << filename << "\"!" << endl;
//  }
//  return Exists;
//}
//
////Test class
//
//
//class Player
//{
//public:
//  int Level;
//  int Strength;
//  int Wisdom;
//  float Health;
//  float Mana;
//  float Armor;
//  string Class;
//  string Race;
//  string Name;
//
//  Player() {}
//  void Say()
//  {
//    cout << Name << " a level " << Level << " " << Race << " " << Class << endl;
//    cout << "HP: " << Health << '\t' << "MP: " << Mana << '\t' << "AC: " << Armor << endl;
//    cout << "Strength: " << Strength << '\t' << "Wisdom: " << Wisdom << endl;
//  }
//};
//
//int main()
//{
//  //Create some test objects of our class.
//  Player p1;
//  p1.Armor = 5;
//  p1.Class = "Programmer";
//  p1.Health = 12.5;
//  p1.Level = 99;
//  p1.Mana = 7.77;
//  p1.Name = "IGnatus";
//  p1.Race = "Extra Ordinary";
//  p1.Strength = 5;
//  p1.Wisdom = 100;
//
//  p1.Say();
//
//  cout << "---------------" << endl;
//
//  Player p2;
//
//  //Save p1's data, not the values.
//
//  SaveObject<Player>(&p1, "Player1.bin");
//
//  //Load data of p1 into p2.
//
//  if (LoadObject<Player>(&p2, "Player1.bin"))
//    p2.Say();//if all goes well, show it's values.
//
//  return 0;
//}

} // End namespace I3D

#endif // I3D_UTILS_H
