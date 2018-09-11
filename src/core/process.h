#ifndef TL_CORE_PROCESS_H
#define TL_CORE_PROCESS_H

#include <list>
#include <functional>
#include <memory>
#include <thread>
#include <mutex>

#include "config_tl.h"

#include "core/defs.h"


namespace TL
{

class Progress;

/*!
 * \addtogroup utilities Utilidades
 *
 * Utilidades de proposito general como manipulación de cadenas, información de la
 * aplicación etc
 * \{
 */


/* ---------------------------------------------------------------------------------- */

class TL_EXPORT Process
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
    virtual void onPause(uint64_t id) = 0;

    /*!
     * \brief Evento reanudación
     * \param id Identificador del proceso
     */
    virtual void onResume(uint64_t id) = 0;

    /*!
     * \brief Evento proceso corriendo
     * \param id Identificador del proceso
     */
    virtual void onRun(uint64_t id) = 0;

    /*!
     * \brief Evento inicio procesos
     * \param id Identificador del proceso
     */
    virtual void onStart(uint64_t id) = 0;

    /*!
     * \brief Evento detención
     * \param id Identificador del proceso
     */
    virtual void onStop(uint64_t id) = 0;

    /*!
     * \brief 
     * \param id Identificador del proceso
     */
    virtual void onEnd(uint64_t id) = 0;

    /*!
     * \brief 
     * \param id Identificador del proceso
     */
    virtual void onError(uint64_t id) = 0;
  };

protected:

  Status mStatus;
  Process *mParentProcess;

  /*!
   * \brief Lista con los escuchadores subscritos al gestor de eventos
   */
  std::list<Listener *> mListeners;

  /*!
   * \brief Identificador del proceso 
   */    
  unsigned long mProcessId;

  /*!
   * \brief Nombre del proceso 
   */  
  std::string mProcessName;

private:

  /*!
   * \brief Número de procesos
   */
  static unsigned long sProcessCount;

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
  virtual Status run(Progress *progressBar = nullptr) = 0;

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
  
  /*!
   * \brief Establece el estado
   * \param[in] status
   */
  void setStatus(Status status);

  /*!
   * \brief Devuelve el identificador del proceso
   * \return
   */
  uint64_t getProcessId() const;

  /*!
   * \brief Devuelve el nombre del proceso 
   */
  std::string getProcessName() const;

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

  /*!
   * \brief Chequeo del estado del proceso
   * 
   * Este método puede llamarse para comprobar el estado actual del proceso
   * y actuar en consecuencia (poner el proceso en pausa, reanudarlo, finalizar)
   *
   */
  virtual Status checkStatus();
};



/* ---------------------------------------------------------------------------------- */

//TODO: Pendiente para Linux
class TL_EXPORT CmdProcess : public Process
{
public:

#ifdef WIN32
  //// Añadir prioridad https://msdn.microsoft.com/en-us/library/windows/desktop/ms683211(v=vs.85).aspx
  enum class PRIORITY
  {

    REALTIME = REALTIME_PRIORITY_CLASS,
    HIGH = HIGH_PRIORITY_CLASS,
    ABOVE_NORMAL = ABOVE_NORMAL_PRIORITY_CLASS,
    NORMAL = NORMAL_PRIORITY_CLASS,
    BELOW_NORMAL = BELOW_NORMAL_PRIORITY_CLASS,
    IDLE = IDLE_PRIORITY_CLASS
  };
#endif

protected:

#ifdef WIN32
  STARTUPINFO si;
  PROCESS_INFORMATION pi;
  static DWORD sPriority;
#endif
  std::string mCmd;

public:

  CmdProcess(const std::string &cmd, Process *parentProcess = nullptr);
  ~CmdProcess() override;

  virtual Process::Status run(Progress *progressBar = nullptr) override;

  /*!
   * \brief Establece la prioridad del proceso
   * \param[in] priority
   */
  static void setPriority(int priority);

private:

#ifdef WIN32
  std::string formatErrorMsg(DWORD errorCode);
#endif
};


/* ---------------------------------------------------------------------------------- */


///*!
// * \brief Wrapper de una función para ejecutarla como un proceso.
// * 
// */
//class TL_EXPORT FunctionProcess : public Process
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

  /*!
   * \brief Interfaz que se debe implementar para recibir los eventos del batch
   *
   * Las clases que implementen este listener y se subcriban mediante el método 
   * addListener() recibiran los diferentes eventos que se emitan desde el batch.
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
     * \brief 
     */
    virtual void onEnd() = 0;

    /*!
     * \brief 
     */
    virtual void onError() = 0;
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

  /*!
   * \brief Lista con los escuchadores subscritos al gestor de eventos
   */
  std::list<Listener *> mListeners;

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
   * \param[in] procList Listado de procesos
   */
  BatchProcess(std::initializer_list<std::shared_ptr<Process>> procList);

  /*!
   * \brief Destructora
   */
  ~BatchProcess() override;

  /*!
   * \brief Añade un nuevo proceso a la lista
   * \param[in] process Proceso que se añade
   */
  void add(const std::shared_ptr<Process> &process);

  /*!
   * \brief Añade un escuchador de eventos
   * \param[in] listener Objeto escuchador
   */
  void addListener(Listener *listener);

  /*!
   * \brief Quita un escuchador de mensajes
   * \param[in] listener Objeto escuchador
   */
  void removeListener(Listener *listener);

  /*!
   * \brief Limpia la lista de procesos
   * \deprecated Use reset() en su lugar
   */
  TL_DEPRECATED("BatchProcess::reset()")
  void clear();

  /*!
   * \brief Elimina el proceso
   * \param[in] id Identificador del proceso que se quiere eliminar
   */
  void remove(uint64_t id);

  /*!
   * \brief Elimina el proceso
   * \param[in] process Proceso que se va a eliminar
   */
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
  Status run(Progress *progressBarTotal = nullptr, Progress *progressBarPartial = nullptr);

  /*!
   * \brief Corre los procesos en otro hilo de ejecución
   * \param[in] progressBarTotal Barra de progreso total
   * \param[in] progressBarPartial Barra de progreso parcial
   */
  Status run_async(Progress *progressBarTotal = nullptr, Progress *progressBarPartial = nullptr);

  /*!
   * \brief Detiene los procesos
   */
  void stop();

  /*!
   * \brief Inicia el contador
   */
  void initCounter(); 

protected:
  
  virtual void onPause(uint64_t id) override;
  virtual void onResume(uint64_t id) override;
  virtual void onRun(uint64_t id) override;
  virtual void onStart(uint64_t id) override;
  virtual void onStop(uint64_t id) override;
  virtual void onEnd(uint64_t id) override;
  virtual void onError(uint64_t id) override;

private:

  // Eventos que se lanzan
  void endTriggered();
  void errorTriggered();

};


/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Clase para separar en bloques un proceso
 *
 * Por bloque se entiende una tarea particular del proceso que se
 * ejecuta sin comprobación del estado de ejecución del batch.
 * Al finalizar un bloque se comprueba el estado del batch poniendolo 
 * a pausa o obligando a su terminación. Si se indica una barra de proceso
 * también se avanza una posición
 *
 */
//class ProcessBlock
//{
//
//private:
//
//  int mCount;
//  TL::Progress *mProgressBar;
//
//public:
//
//  ProcessBlock(int nBlocks, TL::Progress *progressBar = nullptr);
//  ~ProcessBlock();
//  
//  void next();
//
//private:
//
//};



/*! \} */ // end of utilities



/* ---------------------------------------------------------------------------------- */

} // End namespace TL

#endif // TL_CORE_PROCESS_H
