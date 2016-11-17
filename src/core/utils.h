#ifndef I3D_UTILS_H
#define I3D_UTILS_H

#include <vector>
#include <iostream>
#include <fstream>

#include "core/defs.h"

namespace I3D
{

/*! \defgroup utilities Utilidades
 *
 *  Utilidades de proposito general como manipulación de cadenas, información de la 
 * aplicación etc 
 *  \{
 */

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
 * \return Error
 */
int I3D_EXPORT deleteDir(const char *path, bool confirm = false);


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
int I3D_EXPORT splitToNumbers(const std::string &cad, std::vector<int> &vOut, char *chs = ",");

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
int I3D_EXPORT splitToNumbers(const std::string &cad, std::vector<double> &vOut, char *chs = ",");

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
void I3D_EXPORT replaceString(std::string *str, const std::string &str_old, const std::string &str_new);

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
int I3D_EXPORT getFileDir(const char *path, char *dir);

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
int I3D_EXPORT getFileDrive(const char *path, char *drive);

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
int I3D_EXPORT getFileExtension(const char *path, char *ext);

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
int I3D_EXPORT getFileName(const char *path, char *name);

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
int I3D_EXPORT getFileDriveDir(const char *path, char *driveDir);

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
int I3D_EXPORT changeFileName(const char *path, char *newName, char *pathOut);

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
int I3D_EXPORT changeFileExtension(const char *path, char *newExt, char *pathOut);

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
int I3D_EXPORT changeFileNameAndExtension(const char *path, char *newNameExt, char *pathOut);

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
int I3D_EXPORT split(const std::string &in, std::vector<std::string> &out, char *chs = ",");

/*! \} */ // end of stringOper

} // End namespace I3D

#endif // I3D_UTILS_H
