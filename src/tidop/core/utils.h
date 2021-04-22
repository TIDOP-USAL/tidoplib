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

#ifndef TL_CORE_UTILS_H
#define TL_CORE_UTILS_H

#include "config_tl.h"

#include <iostream>
#include <fstream>
#include <map>
#include <regex>
#include <list>
#include <numeric>





#include "tidop/core/defs.h"

namespace tl
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
TL_EXPORT const char *getRunfile();


/* ---------------------------------------------------------------------------------- */
/*                    Operaciones con directorios y archivos                          */
/* ---------------------------------------------------------------------------------- */


/*!
 * \brief Comprueba si existe un directorio
 * \param[in] path Ruta del directorio
 * \return true si existe.
 */
TL_EXPORT bool isDirectory(const std::string &path);

/*!
 * \brief Comprueba si existe el fichero
 * \param[in] file Fichero
 * \return true si existe.
 */
TL_EXPORT bool isFile(const std::string &file);

/*!
 * \brief Crea un directorio
 * \param[in] path Ruta del directorio
 * \return Error = -1, creado = 0 y existente = 1
 */
TL_EXPORT int createDir(const std::string &path);

/*!
 * \brief Borra un directorio
 * \param[in] path Ruta del directorio
 * \param[in] confirm Pide confirmación para borrar el archivo
 * \return Error
 */
TL_EXPORT int deleteDir(const std::string &path, bool confirm = false);

//TL_EXPORT int move(const std::string &in, const std::string &out);


/*!
 * \brief Optiene el directorio de un archivo
 * \param[in] path Ruta del archivo
 * \param[out] dir Directorio del archivo
 * \param[in] size Tamaño de dir
 * \return (1) error
 *
 * <h4>Ejemplo</h4>
 * \code
 * char dir[TL_MAX_DIR];
 * if (getFileDir("c:\temp\file.txt", dir, TL_MAX_DIR) == 0) {
 * ...
 * }
 * \endcode
 */
TL_EXPORT int getFileDir(const char *path, char *dir, int size);

/*!
 * \brief Optiene la unidad de disco de un archivo
 * \param[in] path Ruta del archivo
 * \param[out] drive Unidad de disco
 * \param[in] size Tamaño de drive
 * \return (1) error
 *
 * <h4>Ejemplo</h4>
 * \code
 * char drive[TL_MAX_DRIVE];
 * if (getFileDrive("c:\temp\file.txt", drive, TL_MAX_DRIVE) == 0) {
 * ...
 * }
 * \endcode
 */
TL_EXPORT int getFileDrive(const char *path, char *drive, int size);

#ifdef TL_ENABLE_DEPRECATED_METHODS
/*!
 * \brief Optiene la extensión de un archivo
 * \param[in] path Ruta del archivo
 * \param[out] ext Extensión del archivo
 * \param[in] size Tamaño de ext
 * \return (1) error
 *
 * <h4>Ejemplo</h4>
 * \code
 * char ext[TL_MAX_EXT];
 * if (getFileExtension("c:\temp\file.txt", ext, TL_MAX_EXT) == 0) {
 * ...
 * }
 * \endcode
 * \deprecated Usar en su lugar boost::filesystem::extension()
 */
TL_DEPRECATED("boost::filesystem::extension()", "2.0")
TL_EXPORT int getFileExtension(const char *path, char *ext, int size);

/*!
 * \brief Optiene el nombre de un archivo
 * \param[in] path Ruta del archivo
 * \param[out] name Nombre del archivo
 * \param[in] size Tamaño de name
 * \return (1) error
 *
 * <h4>Ejemplo</h4>
 * \code
 * char name[TL_MAX_FNAME];
 * if (getFileName("c:\temp\file.txt", name, TL_MAX_FNAME) == 0) {
 * ...
 * }
 * \endcode
 * \deprecated Usar en su lugar boost::filesystem::extension()
 */
TL_DEPRECATED("boost::filesystem::filename()", "2.0")
TL_EXPORT int getFileName(const char *path, char *name, int size);
#endif // TL_ENABLE_DEPRECATED_METHODS

/*!
 * \brief Optiene el directorio de un archivo incluyendo la letra de la unidad
 * \param[in] path Ruta del archivo
 * \param[out] driveDir Unidad de disco y directorio del archivo
 * \param[in] size Tamaño de driveDir
 * \return (1) error
 *
 * <h4>Ejemplo</h4>
 * \code
 * char driveDir[TL_MAX_PATH];
 * if (getFileDriveDir("c:\temp\file.txt", driveDir, TL_MAX_PATH) == 0) {
 * ...
 * }
 * \endcode
 */
TL_EXPORT int getFileDriveDir(const char *path, char *driveDir, int size);

/*!
 * \brief Cambia el nombre de un archivo
 * \param[in] path Ruta del archivo
 * \param[in] newName Nuevo nombre
 * \param[out] pathOut Ruta del archivo modificada
 * \param[in] size Tamaño de pathOut
 * \return (1) error
 *
 * <h4>Ejemplo</h4>
 * \code
 * char new_name[TL_MAX_PATH];
 * if (changeFileName("c:\temp\old_name.txt", "new_name", new_name, TL_MAX_PATH) == 0) {
 * ...
 * }
 * \endcode
 */
TL_EXPORT int changeFileName(const char *path, const char *newName, char *pathOut, int size);

#ifdef TL_ENABLE_DEPRECATED_METHODS
/*!
 * \brief Cambia la extensión de un archivo
 * \param[in] path Ruta del archivo
 * \param[in] newExt Nueva extensión
 * \param[out] pathOut Ruta del archivo modificada
 * \param[in] size Tamaño de pathOut
 * \return (1) error
 *
 * <h4>Ejemplo</h4>
 * \code
 * char logfile[TL_MAX_PATH];
 * if (changeFileExtension(getRunfile(), "log", logfile, TL_MAX_PATH) == 0) {
 * ...
 * }
 * \endcode
 * \deprecated Usar en su lugar boost::filesystem::replace_extension() o std::filesystem::replace_extension()
 */
TL_DEPRECATED("filesystem::replace_extension()", "2.0")
TL_EXPORT int changeFileExtension(const char *path, const char *newExt, char *pathOut, int size);
#endif // TL_ENABLE_DEPRECATED_METHODS

/*!
 * \brief Cambia el nombre y la extensión de un archivo
 * \param[in] path Ruta del archivo
 * \param[in] newNameExt Nuevo nombre incluyendo la extensión
 * \param[out] pathOut Ruta del archivo modificada
 * \param[in] size Tamaño de pathOut
 * \return (1) error
 *
 * <h4>Ejemplo</h4>
 * \code
 * char logfile[TL_MAX_FNAME + TL_MAX_EXT];
 * if (changeFileNameAndExtension(getRunfile(), "error.log", logfile, TL_MAX_FNAME + TL_MAX_EXT) == 0) {
 * ...
 * }
 * \endcode
 */
TL_EXPORT int changeFileNameAndExtension(const char *path, const char *newNameExt, char *pathOut, int size);


TL_EXPORT void directoryList(const char *directory, std::list<std::string> *dirList);

#ifdef TL_ENABLE_DEPRECATED_METHODS
TL_DEPRECATED("fileList(const std::string &directory, std::list<std::string> *fileList, const std::regex &wildcard)", "2.0")
TL_EXPORT void fileList(const char *directory, std::list<std::string> *fileList, const char *wildcard = "");
#endif // TL_ENABLE_DEPRECATED_METHODS

/*!
 * \brief Devuelve el listado de archivos de un directorio
 * \param[in] directory Directorio que se quiere listar
 * \param[out] fileList Listado de archivos
 * \param[in] filter Filtro de busqueda
 */
TL_EXPORT void fileList(const std::string &directory, std::list<std::string> *fileList, const std::regex &filter);

/*!
 * \brief Devuelve el listado de archivos de un directorio filtrando por un tipo de archivo
 * \param[in] directory Directorio que se quiere listar
 * \param[out] fileList Listado de archivos
 * \param[in] ext Extensión de archivo (incluyendo el punto)
 *
 * <h4>Ejemplo</h4>
 * \code
 * std::list<std::string> fileList;
 * fileListByExt("D:\\dir", &fileList, ".ext"); 
 * \endcode
 */
TL_EXPORT void fileListByExt(const std::string &directory, std::list<std::string> *fileList, const std::string &ext);



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
TL_EXPORT int splitToNumbers(const std::string &cad, std::vector<int> &vOut, const char *chs = ",");

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
TL_EXPORT int splitToNumbers(const std::string &cad, std::vector<double> &vOut, const  char *chs = ",");

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
TL_EXPORT void replaceString(std::string *str, const std::string &str_old, const std::string &str_new);


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
TL_EXPORT int split(const std::string &in, std::vector<std::string> &out, const char *chs = ",");

/*! \} */ // end of stringOper

#ifdef TL_ENABLE_DEPRECATED_METHODS
/*!
 * \brief Convierte un número a una cadena de texto
 * \param[in] number Numero
 * \return Cadena de texto
 */
template <typename T> inline
TL_DEPRECATED("std::to_string()", "2.0")
std::string numberToString(T number)
{
  std::ostringstream ss;
  ss << number;
  return ss.str();
}
#endif // TL_ENABLE_DEPRECATED_METHODS


/*!
 * \brief Convierte una cadena de texto a un número
 * \param[in] text Texto
 * \return número
 */
template <typename T> inline 
T stringToNumber(const std::string &text)
{
  T number{};
  return (std::istringstream(text) >> number) ? number : 0;
}

enum class Base : int8_t
{
  octal       =  8,
  decimal     = 10,
  hexadecimal = 16
#ifdef TL_ENABLE_DEPRECATED_METHODS
  ,
  OCTAL       =  8,
  DECIMAL     = 10,
  HEXADECIMAL = 16
#endif
};

/*!
 * \brief Convierte una cadena a un número entero.
 *
 * La cadena puede tener un número en base octal, decimal o hexadecimal
 * \param text Cadena de texto
 * \param base Base en la cual esta el número
 * \return Número
 * \see Base
 */
TL_EXPORT int stringToInteger(const std::string &text, Base base = Base::decimal);


/*!
 * \brief Ordena los indices de un vector de menor a mayor
 * Para un vector [10,20,15,5] devuelve [3,0,2,1]. El elemento mas
 * pequeño esta en la posición 3, el segundo en la posición 0, ...
 * \param[in] v Vector
 * \return Vector con los indices ordenados
 */
template <typename T> inline 
std::vector<int> sortIdx(const std::vector<T> &v)
{
  std::vector<int> idx(v.size());
  std::iota(idx.begin(), idx.end(), 0);

  sort(idx.begin(), idx.end(), [&v](int i1, int i2) {return v[i1] < v[i2]; });

  return idx;
}

TL_EXPORT bool compareInsensitiveCase(const std::string &source,
                                      const std::string &compare);

/* ---------------------------------------------------------------------------------- */
/*                               Plantillas html y xml                                */
/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Clase virtual para la sustitución de etiquetas en textos, ficheros html o xml.
 */
//class TL_EXPORT VrtTemplate
//{
//
//protected:
//
//  /*!
//   * \brief Texto con las etiquetas de reemplazo
//   */
//  std::string mText;
//
//public:
//
//  /*!
//   * \brief Constructora por defecto
//   */
//  VrtTemplate();
//
//  /*!
//   * \brief Constructora
//   */
//  VrtTemplate(const char *text);
//
//
//  /*!
//   * \brief Destructora
//   */
//  virtual ~VrtTemplate();
//
//  /*!
//   * \brief Lee un fichero plantilla
//   * \param[in] file Fichero plantilla
//   * \return
//   */
//  virtual int read(const char *file);
//
//  /*!
//   * \brief Establece la plantilla
//   * \param[in] templ Texto con la plantilla
//   * \return
//   */
//  void setTemplate(const char *templ);
//
//  /*!
//   * \brief Remplaza todas las ocurrencias de las etiquetas
//   * de plantilla
//   * \param[out] output Texto con las etiquetas sustituidas
//   * \return
//   */
//  virtual int replace(std::string *output) const;
//
//  /*!
//   * \brief Reemplaza una etiqueta por su valor
//   * \param tag Etiqueta que se sustituye
//   * \param replaceText Texto que sustituye a la etiqueta
//   */
//  virtual void replaceTag(const std::string &tag, std::string *replaceText) const = 0;
//
//};
//
//
//class TL_EXPORT HtmlTemplate : public VrtTemplate
//{
//
//protected:
//
//  std::map<std::string, std::string> mTagValues;
//
//public:
//
//  /*!
//   * \brief Constructora por defecto
//   */
//  HtmlTemplate();
//
//  /*!
//   * \brief Constructora
//   */
//  HtmlTemplate(const char *text, const std::map<std::string, std::string> &tag_values);
//
//  /*!
//   * \brief Destructora
//   */
//  ~HtmlTemplate();
//
//  /*!
//   * \brief Reemplaza una etiqueta por su valor
//   * \param tag Etiqueta que se sustituye
//   * \param replaceText Texto que sustituye a la etiqueta
//   */
//  void replaceTag(const std::string &tag, std::string *replaceText) const override;
//};
//

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Opciones del formato
 */
class TL_EXPORT FileOptions
{
public:

  FileOptions() {}
  virtual ~FileOptions() = default;

  virtual const char *options() = 0;
};

/*!
 * \brief Clase base para manejo de ficheros.
 *
 * Esta clase define la interfaz básica para lectura, creación y escritura de ficheros
 */
class TL_EXPORT File
{
public:

  //TODO: Revisar los modos. Igual es mejor utilizar flags
  /*!
   * \brief Modos de apertura de ficheros
   */
  enum class Mode : int8_t
  {
    read,      /*!< Lectura */
    update,    /*!< Lectura y escritura. */
    create     /*!< Creación */
#ifdef TL_ENABLE_DEPRECATED_METHODS
    ,
    Read   = read,     /*!< Lectura */
    Update = update,   /*!< Lectura y escritura. */
    Create = create    /*!< Creación */
 #endif
  };

  /*!
   * \brief Estado
   */
  enum class Status : int8_t
  {
    open_ok,
    open_fail,
    save_ok,
    success,
    failure
#ifdef TL_ENABLE_DEPRECATED_METHODS
    ,
    OPEN_OK   = open_ok,
    OPEN_FAIL = open_fail,
    SAVE_OK   = save_ok,
    SUCCESS   = success,
    FAILURE   = failure
#endif
  };

protected:
  
  /*!
   * \brief Fichero
   */
  std::string mFile;

  Mode mMode;

public:

  /*!
   * \brief Constructora
   */
  File() : mFile("") {}

  //File(const char *file, Mode mode = Mode::Update) : mFile(file), mMode(mode) { }
  File(const std::string &file, Mode mode = Mode::update) : mFile(file), mMode(mode) { }

  /*!
   * \brief Destructora
   */
  virtual ~File(){}



  /*!
   * \brief Abre un fichero especificando las opciones del formato
   * \param[in] file Fichero
   * \param[in] mode Modo de apertura
   * \param[in] options Opciones del formato
   * \return
   * \see Mode
   */
  //virtual Status open(const char *file, Mode mode = Mode::Update, FileOptions *options = nullptr) = 0;
  virtual Status open(const std::string &file, Mode mode = Mode::update, FileOptions *options = nullptr) = 0;
 
  /*!
   * \brief Cierra el fichero
   */
  virtual void close() = 0;

  /*!
   * \brief Guarda una copia con otro nonbre
   */
  //virtual Status createCopy(const char *fileOut) = 0;
  virtual Status createCopy(const std::string &fileOut) = 0;
};



/*! \} */ // end of utilities


} // End namespace tl

#endif // TL_CORE_UTILS_H
