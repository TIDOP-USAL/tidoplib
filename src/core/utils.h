#ifndef I3D_UTILS_H
#define I3D_UTILS_H

#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include <functional>
#include <memory>
#include <map>
#include <thread>
#include <mutex>

#include "core/config.h"

#ifdef HAVE_OPENCV
#include "opencv2/core/core.hpp"
#endif // HAVE_OPENCV

#include "core/defs.h"
#include "core/datamodel.h"

namespace I3D
{

class Progress;

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

/*!
 * \brief Información de la aplicación
 */
//bool getAppVersion(std::string *libName, std::string *companyName, std::string *productName,
//                   std::string *fileVersion, std::string *copyright, std::string *fileDescription);

/*!
 * \brief Comprueba si existe un directorio
 * \param[in] path Ruta del directorio
 * \return true si existe.
 */
I3D_EXPORT bool isDirectory(const char *path);

/*!
 * \brief Comprueba si existe el fichero
 * \param[in] file Fichero
 * \return true si existe.
 */
I3D_EXPORT bool isFile(const char *file);

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

/*!
 * \brief Optiene el directorio de un archivo
 * \param[in] path Ruta del archivo
 * \param[out] dir Directorio del archivo
 * \param[in] size Tamaño de dir
 * \return (1) error
 *
 * <h4>Ejemplo</h4>
 * \code
 * char dir[I3D_MAX_DIR];
 * if (getFileDir("c:\temp\file.txt", dir, I3D_MAX_DIR) == 0) {
 * ...
 * }
 * \endcode
 */
I3D_EXPORT int getFileDir(const char *path, char *dir, int size);

/*!
 * \brief Optiene la unidad de disco de un archivo
 * \param[in] path Ruta del archivo
 * \param[out] drive Unidad de disco
 * \param[in] size Tamaño de drive
 * \return (1) error
 *
 * <h4>Ejemplo</h4>
 * \code
 * char drive[I3D_MAX_DRIVE];
 * if (getFileDrive("c:\temp\file.txt", drive, I3D_MAX_DRIVE) == 0) {
 * ...
 * }
 * \endcode
 */
I3D_EXPORT int getFileDrive(const char *path, char *drive, int size);

/*!
 * \brief Optiene la extensión de un archivo
 * \param[in] path Ruta del archivo
 * \param[out] ext Extensión del archivo
 * \param[in] size Tamaño de ext
 * \return (1) error
 *
 * <h4>Ejemplo</h4>
 * \code
 * char ext[I3D_MAX_EXT];
 * if (getFileExtension("c:\temp\file.txt", ext, I3D_MAX_EXT) == 0) {
 * ...
 * }
 * \endcode
 */
I3D_EXPORT int getFileExtension(const char *path, char *ext, int size);

/*!
 * \brief Optiene el nombre de un archivo
 * \param[in] path Ruta del archivo
 * \param[out] name Nombre del archivo
 * \param[in] size Tamaño de name
 * \return (1) error
 *
 * <h4>Ejemplo</h4>
 * \code
 * char name[I3D_MAX_FNAME];
 * if (getFileName("c:\temp\file.txt", name, I3D_MAX_FNAME) == 0) {
 * ...
 * }
 * \endcode
 */
I3D_EXPORT int getFileName(const char *path, char *name, int size);

/*!
 * \brief Optiene el directorio de un archivo incluyendo la letra de la unidad
 * \param[in] path Ruta del archivo
 * \param[out] driveDir Unidad de disco y directorio del archivo
 * \param[in] size Tamaño de driveDir
 * \return (1) error
 *
 * <h4>Ejemplo</h4>
 * \code
 * char driveDir[I3D_MAX_PATH];
 * if (getFileDriveDir("c:\temp\file.txt", driveDir, I3D_MAX_PATH) == 0) {
 * ...
 * }
 * \endcode
 */
I3D_EXPORT int getFileDriveDir(const char *path, char *driveDir, int size);

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
 * char new_name[I3D_MAX_PATH];
 * if (changeFileName("c:\temp\old_name.txt", "new_name", new_name, I3D_MAX_PATH) == 0) {
 * ...
 * }
 * \endcode
 */
I3D_EXPORT int changeFileName(const char *path, const char *newName, char *pathOut, int size);

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
 * char logfile[I3D_MAX_PATH];
 * if (changeFileExtension(getRunfile(), "log", logfile, I3D_MAX_PATH) == 0) {
 * ...
 * }
 * \endcode
 */
I3D_EXPORT int changeFileExtension(const char *path, const char *newExt, char *pathOut, int size);

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
 * char logfile[I3D_MAX_FNAME + I3D_MAX_EXT];
 * if (changeFileNameAndExtension(getRunfile(), "error.log", logfile, I3D_MAX_FNAME + I3D_MAX_EXT) == 0) {
 * ...
 * }
 * \endcode
 */
I3D_EXPORT int changeFileNameAndExtension(const char *path, const char *newNameExt, char *pathOut, int size);


I3D_EXPORT void directoryList(const char *directory, std::list<std::string> *dirList);

I3D_EXPORT void fileList(const char *directory, std::list<std::string> *fileList, const char *wildcard = "");

//TODO: Incluir lo anterior en la clase Path para darle un acceso de mas alto nivel a las funciones anteriores

/*!
 * \brief The Path class
 */
class I3D_EXPORT Path
{
private:

 /*!
  * \brief mPos
  */
  int mPos;

  /*!
   * \brief mPath
   */
  std::vector<std::string> mPath;

  /*!
   * \brief Directorio o fichero
   */
  //bool bDir;

#if defined WIN32

  /*!
   * \brief Unidad de disco
   */
  std::string mDrive;

#endif

public:

  /*!
   * \brief Constructor por defecto
   */
  Path();

  /*!
   * \brief Constructor
   * \param path Ruta
   */
  Path(const std::string &path);

  /*!
   * \brief Constructor de copia
   * \param path Ruta
   */
  Path(const Path &path);

  /*!
   * \brief Destructora
   */
  ~Path()
  {
  }

  /*!
   * \brief Parsea una cadena
   * \param path
   */
  void parse(const std::string &path);

  /*!
   * \brief getDrive
   * \return
   */
  const char *getDrive();

  /*!
   * \brief Sube una posición en el path
   */
  void up();

  /*!
   * \brief Baja una posición
   */
  void down();
  
  /*!
   * \brief currentPath
   * \return
   */
  std::vector<std::string> currentPath();

  /*!
   * \brief convierte el path a una cadena
   * \return
   */
  std::string toString();

  //TODO: Un path no solo apunta a un directorio con lo cual habría que añadir utilidades para imagenes
  // bool isDirectory();
  // bool isFile();
  // void listFiles();  // Listar ficheros o directorios. Posibilidad de filtrar con comodines (tipo de archivo, solo directorios, etc)
  // void createDir();
};




/* ---------------------------------------------------------------------------------- */



class I3D_EXPORT Process
{

public:
  
  /*!
   * \brief Estados del proceso
   */
  enum class Status {
    START,             /*!< Inicio */
    RUNNING,           /*!< Corriendo */
    PAUSING,           /*!< Pausando */
    PAUSE,             /*!< Pausado */
    STOPPED,           /*!< Detenido por el usuario*/
    FINALIZED,         /*!< Finalizado */
    FINALIZED_ERROR    /*!< Terminado con error */
  };

  /*!
   * \brief Interfaz que se debe implementar para recibir los eventos del proceso
   *
   * Las clases que implementen este listener y se subcriban mediante el método 
   * addListener() recibiran los diferentes eventos que se emitan desde el proceso.
   */
  class Listener
  {
  public:

    /*!
     * \brief Constructora
     */
    Listener()
    {
    }

    /*!
     * \brief destructora
     */
    virtual ~Listener()
    {
    }

    /*!
     * \brief Evento pausa
     * \param id Identificador del proceso
     */
    virtual void onPause(unsigned long id) = 0;

    /*!
     * \brief Evento reanudación
     * \param id Identificador del proceso
     */
    virtual void onResume(unsigned long id) = 0;

    /*!
     * \brief Evento proceso corriendo
     * \param id Identificador del proceso
     */
    virtual void onRun(unsigned long id) = 0;

    /*!
     * \brief Evento inicio procesos
     * \param id Identificador del proceso
     */
    virtual void onStart(unsigned long id) = 0;

    /*!
     * \brief Evento detención
     * \param id Identificador del proceso
     */
    virtual void onStop(unsigned long id) = 0;

    /*!
     * \brief 
     * \param id Identificador del proceso
     */
    virtual void onEnd(unsigned long id) = 0;

    /*!
     * \brief 
     * \param id Identificador del proceso
     */
    virtual void onError(unsigned long id) = 0;
  };

protected:

  Status mStatus;

  /*!
   * \brief Lista con los escuchadores subscritos al gestor de eventos
   */
  std::list<Listener *> mListeners;

  /*!
   * \brief Número de procesos
   */
  static unsigned long sProcessCount;

  /*!
   * \brief Identificador del proceso 
   */    
  unsigned long mProcessId;

  Process *mParentProcess;

public:

  /*!
   * \brief Constructora
   */
  Process(Process *parentProcess = nullptr);

  /*!
   * \brief Destructora
   */
  virtual ~Process();

  /*!
   * \brief Añade un escuchador de eventos
   * \param[in] listener Objeto escuchador
   */
  void addListener(Listener *listener);

  /*!
   * \brief Pausa el proceso
   */
  virtual void pause();

  /*!
   * \brief Quita un escuchador de mensajes
   * \param[in] listener Objeto escuchador
   */
  void removeListener(Listener *listener);

  /*!
   * \brief Reinicia el proceso
   */
  virtual void reset();

  /*!
   * \brief Continua ejecutando el proceso
   */
  virtual void resume();

  /*!
   * \brief Arranca el proceso
   * Aunque es virtual pura se define el comportamiento por defecto.
   * Desde la implementación del método en la clase hija se debe incluir 
   * Process::run() o establecer directamente mStatus = Status::RUNNING
   * al inicio del método para establecer que el proceso esta corriendo.
   */
  virtual Status run(Progress *progressBar = NULL) = 0;

  /*!
   * \brief Detiene el proceso
   */
  virtual void stop();

  /*!
   * \brief Devuelve el proceso como una linea de comandos
   */
  //virtual std::string toString() = 0;

  /*!
   * \brief Devuelve el estado actual de la ejecución 
   */
  Status getStatus();
  
  void setStatus(Status status);

  uint64_t getProcessId() const;

  /*!
   * \brief Establece el contador de procesos a cero
   */
  static void processCountReset();

protected:

  // Eventos que se lanzan
  void endTriggered();
  void pauseTriggered();
  void resumeTriggered();
  void runTriggered();
  void startTriggered();
  void stopTriggered();
  void errorTriggered();
};



/* ---------------------------------------------------------------------------------- */

//TODO: Pendiente para Linux
class I3D_EXPORT CmdProcess : public Process
{
protected:

#ifdef WIN32
  STARTUPINFO si;
  PROCESS_INFORMATION pi;

#endif
  std::string mCmd;

public:

  CmdProcess(const std::string &cmd, Process *parentProcess = nullptr);
  ~CmdProcess();

  virtual Process::Status run(Progress *progressBar = NULL) override;

private:

};


/* ---------------------------------------------------------------------------------- */


///*!
// * \brief Wrapper de una función para ejecutarla como un proceso.
// * 
// */
//class I3D_EXPORT FunctionProcess : public Process
//{
//
//private:
//
//  /*!
//   * \brief Función
//   */
//  std::function<void()> f;
//
//public:
//
//  /*!
//   * \brief Constructora
//   * \param[in] f Función de la forma std::function<void(const cv::Mat &,cv::Mat *)>
//   */
//  FunctionProcess(std::function<void()> f);
//
//  ~FunctionProcess();
//
//  virtual Process::Status run(Progress *progressBar = NULL) override;
//
//};


/* ---------------------------------------------------------------------------------- */

class BatchProcess :  public Process::Listener
{
public:

  /*!
   * \brief Estados de Batch
   */
  enum class Status {
    START,             /*!< Inicio */
    RUNNING,           /*!< Corriendo */
    PAUSING,           /*!< Pausando */
    PAUSE,             /*!< Pausado */
    STOPPED,           /*!< Detenido */
    FINALIZED,         /*!< Finalizado */
    FINALIZED_ERROR    /*!< Terminado con error */
  };

protected:

  /*!
   * \brief Estado del batch
   */
  Status mStatus;

  /*!
   * \brief Lista de procesos
   */
  std::list<std::shared_ptr<Process>> mProcessList;

  std::thread _thread;
  std::mutex mtx;
  Process *mCurrentProcess;

public:  

  /*!
   * \brief Constructora por defecto
   */
  BatchProcess();

  /*!
   * \brief Constructor de copia
   * \param[in] batchProcess Procesos que se copia
   */
  BatchProcess(const BatchProcess &batchProcess);

  /*!
   * \brief Constructor de lista
   * \param[in] Listado de procesos
   */
  BatchProcess(std::initializer_list<std::shared_ptr<Process>> procList);

  /*!
   * \brief Destructora
   */
  ~BatchProcess();

  /*!
   * \brief Añade un nuevo proceso a la lista
   * \param[in] process Proceso que se añade
   */
  void add(const std::shared_ptr<Process> &process);

  /*!
   * \brief Limpia la lista de procesos
   * \deprecated Use reset() en su lugar
   */
  I3D_DEPRECATED("BatchProcess::reset()")
  void clear();

  void remove(uint64_t id);
  void remove(const std::shared_ptr<Process> &process);

  /*!
   * \brief Comprueba si esta corriendo
   */
  bool isRunning() const;

  /*!
   * \brief Pausa los procesos
   */
  void pause();

  /*!
   * \brief Reinicio los procesos
   */
  void reset();

  /*!
   * \brief Continua corriendo los procesos
   */
  void resume();

  /*!
   * \brief Corre los procesos
   * \param[in] progressBarTotal Barra de progreso total
   * \param[in] progressBarPartial Barra de progreso parcial
   */
  Status run(Progress *progressBarTotal = NULL, Progress *progressBarPartial = NULL);

  /*!
   * \brief Corre los procesos en otro hilo de ejecución
   * \param[in] progressBarTotal Barra de progreso total
   * \param[in] progressBarPartial Barra de progreso parcial
   */
  Status run_async(Progress *progressBarTotal = NULL, Progress *progressBarPartial = NULL);

  /*!
   * \brief Detiene los procesos
   */
  void stop();

protected:
  
  virtual void onPause(unsigned long id) override;
  virtual void onResume(unsigned long id) override;
  virtual void onRun(unsigned long id) override;
  virtual void onStart(unsigned long id) override;
  virtual void onStop(unsigned long id) override;
  virtual void onEnd(unsigned long id) override;
  virtual void onError(unsigned long id) override;
};



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

/*!
 * \brief Convierte un número a una cadena de texto
 * \param[in] number Numero
 * \return Cadena de texto
 */
template <typename T> inline
I3D_DEPRECATED("std::to_string()")
std::string numberToString(T number)
{
  std::ostringstream ss;
  ss << number;
  return ss.str();
}



/*!
 * \brief Convierte una cadena de texto a un número
 * \param[in] text Texto
 * \return número
 */
template <typename T> inline 
T stringToNumber(const std::string &text)
{
  T number;
  return (std::istringstream(text) >> number) ? number : 0;
}

enum class Base : int8_t
{
  OCTAL       =  8,
  DECIMAL     = 10,
  HEXADECIMAL = 16
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
I3D_EXPORT int stringToInteger(const std::string &text, I3D::Base base = I3D::Base::DECIMAL);

/* ---------------------------------------------------------------------------------- */
/*                              Operaciones con vectores                              */
/* ---------------------------------------------------------------------------------- */

#ifdef HAVE_OPENCV

/*!
 * \brief Convierte una matriz de OpenCV en un vector
 * \param[in] m Matriz de entrada
 * \param[out] av Vector de salida
 */
template<typename T> inline 
void cvMatToVector(const cv::Mat &m, std::vector<T> *av)
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

#endif // HAVE_OPENCV

/*!
 * \brief Ordena un vector de menor a mayor
 * \param[in] v Vector
 */
template<typename T> inline 
void sortVector(std::vector<T> *v)
{
  std::sort(v->begin(), v->end());
}

/*!
 * \brief Ordena un vector de mayor a menor
 * \param[in] v Vector
 */
template<typename T> inline 
void sortVectorInv(std::vector<T> *v)
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
template<typename T> inline 
int operator==(const std::vector<T> &v, const T t)
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
template<typename T> inline 
int operator!=(const std::vector<T> &v, const T t)
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
template<typename T> inline 
int operator>=(const std::vector<T> &v, const T t)
{
  sortVector(v);
  typename std::vector<T>::iterator upOrEq;
  upOrEq = std::lower_bound(v.begin(), v.end(), t);
  return (v.end() - upOrEq);
}

template<typename T> inline 
int operator<=(const std::vector<T> &v, const T t)
{
  sortVector(v);
  typename std::vector<T>::iterator lowOrEq;
  lowOrEq = std::upper_bound(v.begin(), v.end(), t);
  return (lowOrEq - v.begin());
}

template<typename T> inline 
int operator> (const std::vector<T> &v, const T t)
{
  sortVector(v);
  typename std::vector<T>::iterator up;
  up = std::upper_bound(v.begin(), v.end(), t);
  return (v.end() - up);
}

template<typename T> inline 
int operator< (const std::vector<T> &v, const T t)
{
  sortVector(v);
  typename std::vector<T>::iterator low;
  low = std::lower_bound(v.begin(), v.end(), t);
  return (low - v.begin());
}

/* ---------------------------------------------------------------------------------- */



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
  iota(idx.begin(), idx.end(), 0);

  sort(idx.begin(), idx.end(), [&v](int i1, int i2) {return v[i1] < v[i2]; });

  return idx;
}

#ifdef HAVE_OPENCV


/* ---------------------------------------------------------------------------------- */
/*                Utilidades de carga y guardado para OpenCV                          */
/* ---------------------------------------------------------------------------------- */

I3D_EXPORT void loadCameraParams(const std::string &file, cv::Size &imageSize, cv::Mat &cameraMatrix, cv::Mat &distCoeffs);

I3D_EXPORT int loadBinMat(const char *file, cv::Mat *data);

I3D_EXPORT int saveBinMat(const char *file, cv::Mat &data);

#endif // HAVE_OPENCV


/* ---------------------------------------------------------------------------------- */
/*                         Concurrencia, hilos y multiproceso                         */
/* ---------------------------------------------------------------------------------- */

/*!
 * \brief número optimo de hilos
 */
I3D_EXPORT uint32_t getOptimalNumberOfThreads();

/*!
 * \brief Ejecuta una función en paralelo
 * \param[in] ini
 * \param[in] end
 * \param[in] f Función o lambda
 */
I3D_EXPORT void parallel_for(int ini, int end, std::function<void(int)> f);

/*!
 * \brief Ejecuta una función en paralelo
 * Método sobrecargado para trabajar con contenedores
 * \param[in] it_begin
 * \param[in] it_end
 * \param[out] it_out_begin 
 * \param[in] f Función o lambda
 */
template<typename itIn, typename itOut> inline
void parallel_for(itIn it_begin, itIn it_end, itOut *it_out_begin, std::function<void(itIn, itIn, itOut *)> f)
{
//#ifdef I3D_MSVS_CONCURRENCY
  //Concurrency::cancellation_token_source cts;
  //Concurrency::run_with_cancellation_token([ini, end, f]() {
  //  Concurrency::parallel_for(ini, end, f);
  //},cts.get_token());
  //Concurrency::parallel_for(ini, end, f);
//#else

  auto f_aux = [&](itIn ini, itIn end, itOut *out) {
    while (in_first != in_last) {
      f(ini, end, out);
    }
  };

  int num_threads = getOptimalNumberOfThreads();
  std::vector<std::thread> threads(num_threads);
  auto size = std::distance(it_begin, it_end);
  int _size = size / num_threads;
  for (int i = 0; i < num_threads; i++) {
    itIn _ini = i * _size + it_begin;
    itIn _end = _ini + _size;
    itOut _out = i * _size + it_out_begin;
    if (i == num_threads -1) _end = it_end;
    threads[i] = std::thread(f_aux, _ini, it_out_begin, _end);
  }

  for (auto &_thread : threads) _thread.join();
//#endif
}


/* ---------------------------------------------------------------------------------- */
/*                                Medición de tiempo                                  */
/* ---------------------------------------------------------------------------------- */

/*!
 * \brief tiempo actual
 *
 * <h4>Ejemplo</h4>
 * \code
 * uint64_t time_ini = getTickCount();
 * ...
 * double time = (getTickCount() - time_ini) / 1000.;
 * consolePrintInfo("Time %f", time);
 * \endcode
 */
I3D_EXPORT uint64_t getTickCount();

/*!
 * \brief Clase para medir tiempos.
 *
 * Su uso esta pensado para medir tiempos en depuración
 */
class I3D_EXPORT Chrono
{
public:

  /*!
   * \brief Posibles estados del cronómetro
   */
  enum class Status {
    START,      /*!< Inicio */
    RUNNING,    /*!< Corriendo */
    PAUSE,      /*!< Pausado */
    STOPPED,    /*!< Detenido */
    FINALIZED   /*!< Finalizado */
  };

private:
  
  /*!
   * Tiempo de inicio en milisegundos
   */
  uint64_t mTimeIni;

  /*!
   * Tiempo acumulado en milisegundos
   */
  uint64_t mAccumulated;

  /*!
   * \brief Estado del cronómetro
   */
  Status mStatus;

  /*!
   * \brief Mensaje de información opcional
   */
  std::string mMessage;

  /*!
   * \brief Escribe mensajes en log y consola
   */
  bool bWriteMsg;

public:

  /*!
   * \brief Constructor
   */
  Chrono(const char *msg = "", bool writeMsg = true);

  /*!
   * \brief Destructora
   * En la destructora se llama al método stop si este no ha sido llamado
   */
  ~Chrono();

  /*!
   * \brief Pausa el cronometro
   */
  uint64_t pause();

  /*!
   * \brief Reinicio del cronometro
   */
  void reset();

  /*!
   * \brief Continua corriendo el cronometro cuando estaba pausado
   */
  void resume();

  /*!
   * \brief Arranca el cronometro
   */
  uint64_t run();

  /*!
   * \brief Detiene el cronometro
   */
  uint64_t stop();

  void setMessage(const char *msg);

private:

};


/* ---------------------------------------------------------------------------------- */
/*                               Plantillas html y xml                                */
/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Clase virtual para la sustitución de etiquetas en textos, ficheros html o xml.
 */
class I3D_EXPORT VrtTemplate
{

protected:

  /*!
   * \brief Texto con las etiquetas de reemplazo
   */
  std::string mText;

public:

  /*!
   * \brief Constructora por defecto
   */
  VrtTemplate();

  /*!
   * \brief Constructora
   */
  VrtTemplate(const char *text);


  /*!
   * \brief Destructora
   */
  virtual ~VrtTemplate();

  /*!
   * \brief Lee un fichero plantilla
   * \param[in] file Fichero plantilla
   * \return
   */
  virtual int read(const char *file);

  /*!
   * \brief Establece la plantilla
   * \param[in] templ Texto con la plantilla
   * \return
   */
  void setTemplate(const char *templ);

  /*!
   * \brief Remplaza todas las ocurrencias de las etiquetas
   * de plantilla
   * \param[out] output Texto con las etiquetas sustituidas
   * \return
   */
  virtual int replace(std::string *output) const;

  /*!
   * \brief Reemplaza una etiqueta por su valor
   * \param tag Etiqueta que se sustituye
   * \param replaceText Texto que sustituye a la etiqueta
   */
  virtual void replaceTag(const std::string &tag, std::string *replaceText) const = 0;

};


class I3D_EXPORT HtmlTemplate : public VrtTemplate
{

protected:

  std::map<std::string, std::string> mTagValues;

public:

  /*!
   * \brief Constructora por defecto
   */
  HtmlTemplate();

  /*!
   * \brief Constructora
   */
  HtmlTemplate(const char *text, const std::map<std::string, std::string> &tag_values);

  /*!
   * \brief Destructora
   */
  ~HtmlTemplate();

  /*!
   * \brief Reemplaza una etiqueta por su valor
   * \param tag Etiqueta que se sustituye
   * \param replaceText Texto que sustituye a la etiqueta
   */
  void replaceTag(const std::string &tag, std::string *replaceText) const override;
};


/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Clase base para manejo de ficheros.
 *
 * Esta clase define la interfaz básica para lectura, creación y escritura de ficheros
 */
class I3D_EXPORT File
{
public:

  //TODO: Revisar los modos. Igual es mejor utilizar flags
  /*!
   * \brief Modos de apertura de ficheros
   */
  enum class Mode : int8_t
  {
    Read,      /*!< Lectura */
    Update,    /*!< Lectura y escritura. */
    Create     /*!< Creación */
  };

  /*!
   * \brief Estado
   */
  enum class Status : int8_t
  {
    OPEN_OK,
    OPEN_FAIL,
    SAVE_OK,
    SUCCESS,
    FAILURE
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

  File(const char *file, Mode mode = Mode::Update) : mFile(file), mMode(mode) { }

  /*!
   * \brief Destructora
   */
  virtual ~File(){}

  /*!
   * \brief Cierra el fichero
   */
  virtual void close() = 0;

  /*!
   * \brief Abre un fichero
   * \param[in] file Fichero
   * \param[in] mode Modo de apertura
   * \return
   * \see Mode
   */
  virtual Status open(const char *file, Mode mode = Mode::Update) = 0;

  /*!
   * \brief Guarda una copia con otro nonbre
   */
  virtual Status createCopy(const char *fileOut) = 0;

};

//Es un fichero y es un modelo de datos con lo cual tendria que heredar de forma publica tambien de 
// DataTable
class I3D_EXPORT Csv : public File/*, private DataTable*/
{
private:

  std::fstream fs;

public:

  /*!
   * \brief Constructora
   */
  Csv();

  Csv(const char *file, Mode mode = Mode::Update);

  Csv(const Csv &csv);

  /*!
   * \brief Destructora
   */
  ~Csv();

  /*!
   * \brief Cierra el fichero csv
   */
  void close() override;

  /*!
   * \brief Crea el fichero
   */
  Status create(const std::string &header);
  
  /*!
   * \brief Crea el fichero
   */  
  //Status create(const DataTable &dataTable);

  /*!
   * \brief Guarda una copia con otro nonbre
   */
  Status createCopy(const char *fileOut) override;

  /*!
   * \brief Abre un fichero csv
   * \param[in] file Nombre del fichero
   * \param[in] mode Modo de apertura
   * \return
   * \see Mode
   */
  Status open(const char *file, Mode mode = Mode::Read) override;

  /*!
   * \brief Lee un registro de la tabla
   */
  //TableRegister *read(int id);

  /*!
   * \brief Escribe una linea en el fichero
   * \return
   */
  //Status write(std::shared_ptr<TableRegister> _register);
  Status write(const std::vector<std::string> &_register);

  /*!
   * \brief carga todo el fichero a memoria
   */
  //Status load();

  /*!
   * \brief Salva el fichero cuando se esta trabajando en memoria
   */
  Status save();

private:

  /*!
   * \brief Lee la cabecera
   */
  //Status readHeader();

};





//TODO: funcion para conversión entre tipos basicos en templates para evitar warnings

} // End namespace I3D

#endif // I3D_UTILS_H
