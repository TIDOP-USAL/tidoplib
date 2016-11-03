#ifndef I3D_UTILS_H
#define I3D_UTILS_H

#include <vector>
#include <iostream>
#include <fstream>

#include "core\defs.h"

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

/*! \} */ // end of stringOper

//class Path
//{
//private:
//  int mPos;
//  std::vector<std::string> mPath;
//
//public:
//
//  Path() : mPos(0), mPath(0)
//  {
//  }
//
//  Path(std::string path)
//  {
//    parse(path);
//  }
//
//  ~Path()
//  {
//  }
//
//  void parse(std::string path)
//  {
//    split(path,mPath,"\\");
//    if (mPath.size() == 1) 
//      split(path,mPath,"/");
//    mPos = mPath.size();
//
//    for (int i = 0; mPath[i] == std::string(".."); i++) {
//      std::string run_file_path = getRunfile();
//      Path runPath(run_file_path);
//      runPath.down();
//    }
//    
//    std::string current = runPath.currentPath();
//  }
//
//  std::string getDrive() 
//  {
//    return mPath[0];
//  }
//
//  void up() 
//  {
//    if (mPos < mPath.size())
//    mPos++;
//  }
//
//  void down() 
//  {
//    if (mPos != 0)
//      mPos--;
//  }
//  
//  std::string currentPath() 
//  {
//    std::string _path;
//    for (int i = 0; i < mPos; i++) {
//      _path += mPath[i];
//      _path += "\\";
//    }
//  }
//};









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
