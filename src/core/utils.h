#ifndef I3D_UTILS_H
#define I3D_UTILS_H

#include <vector>
#include <iostream>
#include <fstream>

#include "opencv2/core/core.hpp"

#include "core/defs.h"

namespace I3D
{

/*! 
 * \defgroup utilities Utilidades
 *
 * Utilidades de proposito general como manipulación de cadenas, información de la 
 * aplicación etc 
 * \{
 */

/* ---------------------------------------------------------------------------------- */
/*                           Información de la aplicación                             */
/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Obtiene el path (directorio + nombre + extensión) de la aplicación
 * que esta corriendo
 * \return path de la aplicación
 */
I3D_EXPORT const char *getRunfile();


//bool getAppVersion(char *libName, std::string *companyName, std::string *productName,
//                   std::string *fileVersion, std::string *copyright, std::string *fileDescription);

/*!
 * \brief Comprueba si existe un directorio
 * \param[in] path Ruta del directorio
 * \return true si existe.
 */
I3D_EXPORT bool isDirectory(const char *path);

/*!
 * \brief Crea un directorio
 * \param[in] path Ruta del directorio
 * \return Error = -1, creado = 0 y existente = 1
 */
I3D_EXPORT int createDir(const char *path);

/*!
 * \brief Crea un directorio
 * \param[in] path Ruta del directorio
 * \param[in] confirm Pide confirmación para borrar el archivo
 * \return Error
 */
I3D_EXPORT int deleteDir(const char *path, bool confirm = false);


/* ---------------------------------------------------------------------------------- */
/*                             Operaciones con cadenas                                */
/* ---------------------------------------------------------------------------------- */

/*! \defgroup stringOper Operaciones con cadenas
 *  
 * \{
 */

/*!
 * \brief Separa una cadena en un array de enteros
 * \param[in] cad Cadena de texto que contiene una lista de numeros
 * \param[out] vOut Vector con los números extraidos
 * \param[in] chs Caracter separador de la cadena. Si se omite toma por defecto ","
 * \return (1) error
 *
 * <h4>Ejemplo</h4>
 * \code
 * std::string aux = "1102,3654";
 * std::vector<int> coord;
 * if ( splitToNumbers(aux, coord) == 0 ){
 *   ...
 * }
 * \endcode
 */
I3D_EXPORT int splitToNumbers(const std::string &cad, std::vector<int> &vOut, const char *chs = ",");

/*!
 * \brief Separa una cadena en un array de dobles
 * \param[in] cad Cadena de texto que contiene una lista de numeros
 * \param[out] vOut Vector con los números extraidos
 * \param[in] chs Caracter separador de la cadena. Si se omite toma por defecto ","
 * \return (1) error
 *
 * <h4>Ejemplo</h4>
 * \code
 * std::string aux = "1102.52,3654.95";
 * std::vector<double> coord;
 * if ( splitToNumbers(aux, coord) == 0 ){
 *   ...
 * }
 * \endcode
 */
I3D_EXPORT int splitToNumbers(const std::string &cad, std::vector<double> &vOut, const  char *chs = ",");

/*!
 * \brief Reemplaza una cadena por otra en un texto.
 * \param[in] str Cadena original
 * \param[in] str_old Cadena a remplazar
 * \param[out] str_new Nueva cadena
 *
 * <h4>Ejemplo</h4>
 * \code
 * std::string str = "Hola mundo";
 * replaceString(str, " ", "_");
 * \endcode
 */
I3D_EXPORT void replaceString(std::string *str, const std::string &str_old, const std::string &str_new);

/*!
 * \brief Optiene el directorio de un archivo
 * \param[in] path Ruta del archivo
 * \param[out] dir Directorio del archivo
 * \return (1) error
 *
 * <h4>Ejemplo</h4>
 * \code
 * char dir[I3D_MAX_DIR];
 * if (getFileDir("c:\temp\file.txt", dir) == 0) {
 * ...
 * }
 * \endcode
 */
I3D_EXPORT int getFileDir(const char *path, char *dir);

/*!
 * \brief Optiene la unidad de disco de un archivo
 * \param[in] path Ruta del archivo
 * \param[out] drive Unidad de disco
 * \return (1) error
 *
 * <h4>Ejemplo</h4>
 * \code
 * char drive[I3D_MAX_DRIVE];
 * if (getFileDrive("c:\temp\file.txt", drive) == 0) {
 * ...
 * }
 * \endcode
 */
I3D_EXPORT int getFileDrive(const char *path, char *drive);

/*!
 * \brief Optiene la extensión de un archivo
 * \param[in] path Ruta del archivo
 * \param[out] ext Extensión del archivo
 * \return (1) error
 *
 * <h4>Ejemplo</h4>
 * \code
 * char ext[I3D_MAX_EXT];
 * if (getFileName("c:\temp\file.txt", ext) == 0) {
 * ...
 * }
 * \endcode
 */
I3D_EXPORT int getFileExtension(const char *path, char *ext);

/*!
 * \brief Optiene el nombre de un archivo
 * \param[in] path Ruta del archivo
 * \param[out] name Nombre del archivo
 * \return (1) error
 *
 * <h4>Ejemplo</h4>
 * \code
 * char name[I3D_MAX_FNAME];
 * if (getFileName("c:\temp\file.txt", name) == 0) {
 * ...
 * }
 * \endcode
 */
I3D_EXPORT int getFileName(const char *path, char *name);

/*!
 * \brief Optiene el directorio de un archivo incluyendo la letra de la unidad
 * \param[in] path Ruta del archivo
 * \param[out] driveDir Unidad de disco y directorio del archivo
 * \return (1) error
 *
 * <h4>Ejemplo</h4>
 * \code
 * char driveDir[I3D_MAX_PATH];
 * if (getFileDriveDir("c:\temp\file.txt", driveDir) == 0) {
 * ...
 * }
 * \endcode
 */
I3D_EXPORT int getFileDriveDir(const char *path, char *driveDir);

/*!
 * \brief Cambia el nombre de un archivo
 * \param[in] path Ruta del archivo
 * \param[in] newName Nuevo nombre
 * \param[out] pathOut Ruta del archivo modificada
 * \return (1) error
 *
 * <h4>Ejemplo</h4>
 * \code
 * char new_name[I3D_MAX_PATH];
 * if (changeFileName("c:\temp\old_name.txt", "new_name", new_name) == 0) {
 * ...
 * }
 * \endcode
 */
I3D_EXPORT int changeFileName(const char *path, const char *newName, char *pathOut);

/*!
 * \brief Cambia la extensión de un archivo
 * \param[in] path Ruta del archivo
 * \param[in] newExt Nueva extensión
 * \param[out] pathOut Ruta del archivo modificada
 * \return (1) error
 *
 * <h4>Ejemplo</h4>
 * \code
 * char logfile[I3D_MAX_PATH];
 * if (changeFileExtension(getRunfile(), "log", logfile) == 0) {
 * ...
 * }
 * \endcode
 */
I3D_EXPORT int changeFileExtension(const char *path, const char *newExt, char *pathOut);

/*!
 * \brief Cambia el nombre y la extensión de un archivo
 * \param[in] path Ruta del archivo
 * \param[in] newNameExt Nuevo nombre incluyendo la extensión
 * \param[out] pathOut Ruta del archivo modificada
 * \return (1) error
 *
 * <h4>Ejemplo</h4>
 * \code
 * char logfile[I3D_MAX_FNAME + I3D_MAX_EXT];
 * if (changeFileNameAndExtension(getRunfile(), "error.log", logfile) == 0) {
 * ...
 * }
 * \endcode
 */
I3D_EXPORT int changeFileNameAndExtension(const char *path, char *newNameExt, char *pathOut);

/*!
 * \brief Separa una cadena
 *
 * \param[in] in Cadena de entrada
 * \param[out] out vector con las cadenas resultantes
 * \param[in] chs cadena de separación. Si se omite toma por defecto ","
 * \return (1) error
 *
 * <h4>Ejemplo</h4>
 * \code
 * char *in = "cadena1,cadena2";
 * std::vector<std::string> out;
 *
 * if ( split(in, out, ",") == 0 ){
 * ...
 * }
 * \endcode
 */
I3D_EXPORT int split(const std::string &in, std::vector<std::string> &out, const char *chs = ",");

/*! \} */ // end of stringOper

/* ---------------------------------------------------------------------------------- */
/*                              Operaciones con vectores                              */
/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Convierte una matriz de OpenCV en un vector
 * \param[in] m Matriz de entrada
 * \param[out] av Vector de salida
 */
template<typename T>
I3D_EXPORT void cvMatToVector(const cv::Mat &m, std::vector<T> *av)
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
I3D_EXPORT void sortVector(std::vector<T> *v)
{
  std::sort(v->begin(), v->end());
}

/*!
 * \brief Ordena un vector de mayor a menor
 * \param[in] v Vector
 */
template<typename T>
I3D_EXPORT void sortVectorInv(std::vector<T> *v)
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
I3D_EXPORT int operator==(const std::vector<T> &v, const T t)
{
  sortVector(v);
  std::pair<typename std::vector<T>::iterator, typename std::vector<T>::iterator> bounds;
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
I3D_EXPORT int operator!=(const std::vector<T> &v, const T t)
{
  sortVector(v);
  std::pair<typename std::vector<T>::iterator, typename std::vector<T>::iterator> bounds;
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
I3D_EXPORT int operator>=(const std::vector<T> &v, const T t)
{
  sortVector(v);
  typename std::vector<T>::iterator upOrEq;
  upOrEq = std::lower_bound(v.begin(), v.end(), t);
  return (v.end() - upOrEq);
}

template<typename T>
I3D_EXPORT int operator<=(const std::vector<T> &v, const T t)
{
  sortVector(v);
  typename std::vector<T>::iterator lowOrEq;
  lowOrEq = std::upper_bound(v.begin(), v.end(), t);
  return (lowOrEq - v.begin());
}

template<typename T>
I3D_EXPORT int operator> (const std::vector<T> &v, const T t)
{
  sortVector(v);
  typename std::vector<T>::iterator up;
  up = std::upper_bound(v.begin(), v.end(), t);
  return (v.end() - up);
}

template<typename T>
I3D_EXPORT int operator< (const std::vector<T> &v, const T t)
{
  sortVector(v);
  typename std::vector<T>::iterator low;
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
I3D_EXPORT int sortMatRows(const cv::Mat &in, cv::Mat *out, cv::Mat *idx);

/*!
 * \brief Ordena los valores de una matriz de mayor a menor por columnas
 * \param[in] in
 * \param[out] out
 * \param[out] idx
 */
I3D_EXPORT int sortMatCols(const cv::Mat &in, cv::Mat *out, cv::Mat *idx);

/*!
 * \brief Ordena los indices de un vector de menor a mayor
 * Para un vector [10,20,15,5] devuelve [3,0,2,1]. El elemento mas 
 * pequeño esta en la posición 3, el segundo en la posición 0, ...
 * \param[in] v Vector
 * \return Vector con los indices ordenados
 */
template <typename T>
I3D_EXPORT std::vector<int> sortIdx(const std::vector<T> &v)
{
  std::vector<int> idx(v.size());
  iota(idx.begin(), idx.end(), 0);

  sort(idx.begin(), idx.end(), [&v](int i1, int i2) {return v[i1] < v[i2]; });

  return idx;
}

I3D_EXPORT void LoadCameraParams(std::string &file, cv::Size &imageSize, cv::Mat &cameraMatrix, cv::Mat& distCoeffs);

} // End namespace I3D

#endif // I3D_UTILS_H
