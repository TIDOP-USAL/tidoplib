/*
 Clases y funciones experimentales
*/

#ifndef I3D_EXPERIMENTAL_H
#define I3D_EXPERIMENTAL_H

#include <vector>
#include <memory>
#include <ctime>

#include "core/defs.h" // Para quitar warnings de sfm

#ifdef HAVE_OPENCV
#include "opencv2/core/core.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/xphoto.hpp"

//#ifdef HAVE_GDAL
//#include "gdal_priv.h"
//#include "cpl_conv.h" // for CPLMalloc()
//#endif

#ifdef I3D_ENABLE_OPENCV_SFM
I3D_SUPPRESS_WARNINGS
#include <opencv2/sfm/conditioning.hpp>
#include <opencv2/sfm/fundamental.hpp>
#include <opencv2/sfm/io.hpp>
#include <opencv2/sfm/numeric.hpp>
#include <opencv2/sfm/projection.hpp>
#include <opencv2/sfm/triangulation.hpp>
#if CERES_FOUND
#include <opencv2/sfm/reconstruct.hpp>
#include <opencv2/sfm/simple_pipeline.hpp>
#endif
#include "libmv/simple_pipeline/tracks.h"
I3D_DEFAULT_WARNINGS
#endif
#endif // HAVE_OPENCV

#include "matching.h"
#include "img_process/img_processing.h"
#include "graphic_entities/color.h"

namespace I3D
{

namespace EXPERIMENTAL
{


// Clase para matching robusto. Necesitaria que tener una clase matching 
// virtual donde se definan todos los métodos comunes
// con esta clase sustituire a nRobustViewMatching y tendre mayor control. 

#ifdef HAVE_OPENCV

/*!
 * \brief Matching robusto
 */
class RobustMatching {

private:

  /*!
   * \brief Descriptor
   */
  cv::Ptr<cv::DescriptorMatcher> mDescriptorMatcher;

  /*!
   * \brief Máxima tolerancia entre el primer y segundo NN
   */
  float mRatio;

  //std::vector<std::vector<cv::DMatch> > mMatches;

public:

  /*!
   * \brief Constructora por defecto
   */
  RobustMatching()
  {
    mDescriptorMatcher = cv::makePtr<cv::BFMatcher>((int)cv::NORM_HAMMING, false);
    mRatio = 0.8f;
  }

  /*!
   * \brief Constructora
   */
  RobustMatching( const cv::Ptr<cv::DescriptorMatcher> &dm) : mDescriptorMatcher(dm)
  {
    mRatio = 0.8f;
  }

  /*!
   * \brief destructora
   */
  virtual ~RobustMatching() {}

  /*!
   * \brief Establece el matcher
   * \param[in] matcher 
   */
  void setDescriptorMatcher(const cv::Ptr<cv::DescriptorMatcher> &matcher) {  mDescriptorMatcher = matcher; }

  /*!
   * \brief Establece el valor de ratio para el test
   * \param[in] ratio 
   */
  void setRatio( float ratio) { mRatio = ratio; }

  // Clear matches for which NN ratio is > than threshold
  // return the number of removed points
  // (corresponding entries being cleared,
  // i.e. size will be 0)
  /*!
   * \brief 
   * \param[in] matches 
   */
  int ratioTest(std::vector<std::vector<cv::DMatch> > &matches);

  /*!
   * \brief test de simetría
   * Busqueda de matches simétricos
   * \param[in] matches1 
   * \param[in] matches2 
   * \param[out] symMatches 
   */
  void symmetryTest( const std::vector<std::vector<cv::DMatch> >& matches1,
                     const std::vector<std::vector<cv::DMatch> >& matches2,
                     std::vector<cv::DMatch>& symMatches );

  /*!
   * \brief test de simetría
   * Busqueda de matches simétricos
   * \param[in] matches
   * \param[out] symMatches 
   */
  void symmetryTest( const std::vector<std::vector<cv::DMatch> >& matches, std::vector<std::vector<cv::DMatch>> *symMatches );

  /*!
   * \brief Matching robusto
   * 
   * \param[in] descriptor1
   * \param[in] descriptor2
   * \param[out] pMatches12 
   * \param[out] pMatches21 
   */
  void robustMatch(const cv::Mat &descriptor1, const cv::Mat &descriptor2, std::vector<std::vector<cv::DMatch>> *pMatches12,  std::vector<std::vector<cv::DMatch>> *pMatches21);

  /*!
   * \brief Matching robusto
   * 
   * \param[in] descriptor1
   * \param[in] descriptor2
   * \param[out] pMatches
   */
  void fastRobustMatch(const cv::Mat &descriptor1, const cv::Mat &descriptor2, std::vector<std::vector<cv::DMatch>> *pMatches);
};













#ifdef I3D_ENABLE_OPENCV_SFM

class Reconstruction3D
{


  /*!
   * \brief keyPoints
   */
  std::vector<std::vector<cv::KeyPoint>> mKeyPoints;

  /*!
   * \brief Descriptores
   */
  std::vector<cv::Mat> mDescriptor;

  /*!
   * \brief imagenes
   */
  std::vector<std::string> mImagesPaths;


  std::vector<cv::Mat> points2d;

  std::unique_ptr<Features2D> mFeature2D;

  ///*!
  // * \brief Detector de caracteristicas
  // */
  //cv::Ptr<cv::FeatureDetector> mFeatureDetector;

  ///*!
  // * \brief Extractor de descriptores
  // */
  //cv::Ptr<cv::DescriptorExtractor> mDescriptorExtractor;

  Matching mMatcher;

  cv::FlannBasedMatcher mMatcherFB;

  //Para sfm
  cv::Ptr<cv::sfm::BaseSFM> reconstruction;
  bool bRefinement;


  /*!
   * \brief Matching robusto
   */
  std::shared_ptr<RobustMatching> mRobustMatching;

  // Intentar escribir directamente la estructura de libmv
  libmv::Tracks tracks;

public:

  /*!
   * \brief Cosntructora por defecto
   */
  Reconstruction3D() { 
    mFeature2D = std::make_unique<Features2D>(cv::ORB::create(10000), cv::xfeatures2d::DAISY::create());
    mRobustMatching = std::make_shared<RobustMatching>();
    init();
  }

  /*!
   * \brief Constructora Features2D
   * Se le pasan como parametro un detector de caracteristicas y un extractor de descriptores
   * \param[in] fd Detector de caracteristicas
   * \param[in] de Extractor de descriptores
   *
   * Detectores soportados: (Probados)
   * - cv::ORB::create();
   * - cv::xfeatures2d::SIFT::create();
   * - cv::xfeatures2d::SURF::create();
   *
   * Descriptores:
   * - cv::xfeatures2d::DAISY::create();
   * - cv::xfeatures2d::SIFT::create();
   * - cv::xfeatures2d::SURF::create();
   */
  Reconstruction3D(const cv::Ptr<cv::FeatureDetector> &fd, const cv::Ptr<cv::DescriptorExtractor> &de, const std::shared_ptr<RobustMatching> robustMatching) 
    : mRobustMatching(robustMatching)
  { 
    mFeature2D = std::make_unique<Features2D>(fd, de);
  }

  /*!
   * \brief Destructora
   */
  ~Reconstruction3D() {}

  /*!
   * \brief A partir de un conjunto de imagenes obtiene sus keypoints y descriptores. Opcionalmente 
   * puede guardarlos para su uso posterior
   * \param[in] imagesPaths Imagenes que se cargan. Se toma el nombre de archivo y se carga su xml asociado
   * \param[in] bSave Salva keypoints y descriptores en un xml con igual nombre que la imagen
   */
  void getKeyPointAndDescriptor(const std::vector<std::string> &imagesPaths, bool bSave = true);

  /*!
   * \brief carga puntos y descriptores asociados a un set de imagenes
   * \param[in] imagesPaths Imagenes que se cargan. Se toma el nombre de archivo y se carga su xml asociado
   */
  void loadKeyPointAndDescriptor(const std::vector<std::string> &imagesPaths);

  /*!
   * \brief Calculo del matching entre todas las imagenes
   * \param[out] points2d Puntos de paso encontrados entre todas las fotos. Si un punto no existe en una foto es (-1,-1)
   */
  void multiImageMatching(std::vector<cv::Mat> &points2d);

  //int ratioTest(std::vector<std::vector<cv::DMatch> > &matches, float ratio = 0.7f);
  //int ratioTest2(std::vector<std::vector<cv::DMatch> > &matches, float ratio = 0.8f);
  //void symmetryTest(const std::vector<std::vector<cv::DMatch> >& matches1,
  //                  const std::vector<std::vector<cv::DMatch> >& matches2,
  //                  std::vector<cv::DMatch>& symMatches);
  void saveTracks(const std::string &filename, std::vector<std::vector<cv::Vec2d> > passPoints);

  // Tomado de sfm
  void reconstruct(std::vector<std::string> &images, std::vector<cv::Mat> &Rs_est, std::vector<cv::Mat> &ts_est, cv::Matx33d &K, std::vector<cv::Mat> &points3d_estimated);

  void init(bool refinement = true);
};

#endif // I3D_ENABLE_OPENCV_SFM

#endif // HAVE_OPENCV



/* ---------------------------------------------------------------------------------- */







/*!
 * \brief Niveles de información de los mensajes
 */
enum class MessageLevel : int8_t {
  MSG_DEBUG,      /*!< Información extra para depuración. */
  MSG_VERBOSE,    /*!< Todos los mensajes. */
  MSG_INFO,       /*!< Warnings, errores y otra información. */
  MSG_WARNING,    /*!< Warnings y errores. */
  MSG_ERROR       /*!< Sólo errores. */
};

/*!
 * \brief Clase para gestionar los mensajes de la librería
 *
 * Proporciona las herramientas necesarias para la impresión de mensajes por
 * consola, en un fichero log y la comunicación con otras librerias o aplicaciones
 */
class MessageManager
{

public:

  /*!
   * \brief Interfaz que deben implementar las clases para comunicarse con
   * el gestor de mensajes
   *
   * Las clases que implementen este listener y se subcriban al gestor de
   * mensajes mediante el método addListener() recibiran las diferentes 
   * mensajes (Depuración, error, etc) que se emitan desde la librería.
   */
  class Listener
  {
  protected:

    //std::shared_ptr<MessageManager> mMessage;

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
    ~Listener()
    {
    }

    /*!
     * \brief Mensaje de depuración
     * \param msg Mensaje que recibe el escuchador
     * \param date Fecha y hora en la que se emite el mensaje
     */
    virtual void onMsgDebug(const char *msg, const char *date) = 0;

    /*!
     * \brief Mensaje verbose
     * \param msg Mensaje que recibe el escuchador
     * \param date Fecha y hora en la que se emite el mensaje
     */
    virtual void onMsgVerbose(const char *msg, const char *date) = 0;

    /*!
     * \brief Mensaje de información
     * \param msg Mensaje que recibe el escuchador
     * \param date Fecha y hora en la que se emite el mensaje
     */
    virtual void onMsgInfo(const char *msg, const char *date) = 0;

    /*!
     * \brief Mensaje de advertencia
     * \param msg Mensaje que recibe el escuchador
     * \param date Fecha y hora en la que se emite el mensaje
     */
    virtual void onMsgWarning(const char *msg, const char *date) = 0;

    /*!
     * \brief Mensaje de error
     * \param msg Mensaje que recibe el escuchador
     * \param date Fecha y hora en la que se emite el mensaje
     */
    virtual void onMsgError(const char *msg, const char *date) = 0;

  };

  /*!
   * \brief Clase que representa un mensaje individual
   */
  class Message
  {
  private:

    /*!
     * \brief Mensaje
     */
    std::string mMessage;

    /*!
     * \brief Fecha y hora en la que se emitio el mensaje
     */
    std::string mDate;

    /*!
     * \brief Nivel del mensaje
     */
    MessageLevel mLevel;

    /*!
     * \brief Fichero en el cual se genera el mensaje
     */
    std::string mFile;

    /*!
     * \brief Número de línea donde se ha generado el mensaje
     */
    int mLine;

    /*!
     * \brief Nombre de la función donde se genera el mensaje
     */
    std::string mFunction;

    /*!
     * \brief Plantilla para el formateo de fecha y hora de los mensajes.
     *
     * Por defecto la plantilla es:
     * \code
     * std::string Message::timeLogTemplate = "%d/%b/%Y %H:%M:%S";
     * \endcode
     * \see setTimeLogFormat
     */
    static std::string sTimeLogFormat;

  public:
    
    /*!
     * \brief Constructora
     *
     * Mediante una cadena con formato y una lista variable de argumentos se
     * permite formatear los mensajes de una forma rapida.
     *
     * \param[in] msg Cadena con formato que se rellenará para componer el mensaje
     *
     * <b>Ejemplo</b>
     *
     * \code
     *   Message msg("Cadena de ejemplo nº%i", 1); // Da como resultado "Cadena de ejemplo nº1"
     * \endcode
     *
     */
    Message(const char *msg, ...)
      : mLevel(MessageLevel::MSG_ERROR),
        mFile(""), mLine(-1),
        mFunction("")
    {
      try {
        char date[64];
        std::time_t now = std::time(NULL);
        std::tm *_tm = std::localtime(&now);

        if (_tm) {
          std::strftime(date, sizeof(date), sTimeLogFormat.c_str()/*"%d/%b/%Y %H:%M:%S"*/, _tm);
        } else {
          strcpy(date, "NULL");
        }
        mDate = date;

        char buf[500];
        memset(buf, 0, sizeof(buf));
        std::string aux(msg);
        I3D::replaceString(&aux, "% ", "%% ");
        va_list args;
        va_start(args, msg);
#ifdef _MSC_VER
        vsnprintf_s(buf, _countof(buf), _TRUNCATE, aux.c_str(), args);
#else
        vsnprintf(buf, sizeof(buf), aux.c_str(), args);
#endif
        va_end(args);
        mMessage = buf;
      } catch (...) {

        // Por evitar un error en la constructora... 
      }
    }
   
    /*!
     * \brief Destructora
     */
    ~Message()
    {
    }

    /*!
     * \brief Devuelve el mensaje como cadena de texto
     * \return Mensaje
     */
    const char *getMessage() const
    {
      return mMessage.c_str();
    }

    /*!
     * \brief Devuelve la fecha y hora del mensaje
     * \return Fecha y hora del mensaje
     */
    const char *getDate() const
    {
      return mDate.c_str();
    }

    /*!
     * \brief getLevel
     * \return
     */
    MessageLevel getLevel() const
    {
      return mLevel;
    }

    /*!
     * \brief getFile
     * \return
     */
    std::string getFile() const
    {
      return mFile;
    }

    /*!
     * \brief getLine
     * \return
     */
    int getLine() const
    {
      return mLine;
    }

    /*!
     * \brief getFunction
     * \return
     */
    std::string getFunction() const
    {
      return mFunction;
    }

    /*!
     * \brief setTimeLogFormat
     * \code
     * Message::setTimeLogFormat( "%d - %b - %Y (%H:%M:%S)" );
     * \endcode
     * \param[in] timeTemplate
     */
    static void setTimeLogFormat( const char *timeTemplate)
    {
      sTimeLogFormat = timeTemplate;
    }

    void setMessageLevel(const MessageLevel &level)
    {
      mLevel = level;
    }

    /*!
     * \brief Añade la locacización del mensaje (fichero, número de línea
     * y función) y el nivel de información de los mensajes
     * \param[in] level Nivel de información del mensaje
     * \param[in] file Nombre del fichero desde el cual se lanza el mensaje
     * \param[in] line Número de línea del mensaje
     * \param[in] function Nombre de la función desde la que se lanza el mensaje
     */
    void setMessageProperties(const MessageLevel &level, const char *file, int line, const char *function)
    {
      mLevel = level;
      mLine = line;
      mFile = file;
      mFunction = function;
    }

  private:

    /*!
     * \brief messageOutput
     * \param msgLevel
     * \return
     */
    //std::string messageOutput(const MessageLevel &msgLevel);

    /*!
     * \brief messageOutput
     * \param msgLevel
     * \param file
     * \param line
     * \param function
     * \return
     */
    //std::string messageOutput(const MessageLevel &msgLevel, const char *file, int line, const char *function);

  };

private:

  /*!
   * \brief sObjMessage
   */
  static std::unique_ptr<MessageManager> sObjMessage;

  /*!
   * \brief Lista con los escuchadores subscritos al gestor de mensajes
   */
  std::list<Listener *> mListeners;

  /*!
   * \brief Nivel de información de los mensajes
   *
   * Por defecto MSG_ERROR
   * \see MessageLevel
   */
  static MessageLevel sLevel;

private:

  /*!
   * \brief Constructora privada MessageManager
   */
  MessageManager();

public:


  ~MessageManager();

  /*!
   * \brief Añade un escuchador de mensajes
   * \param[in] listener Objeto escuchador
   */
  void addListener(Listener *listener);

  MessageManager(MessageManager const&) = delete;
  void operator=(MessageManager const&) = delete;

  /*!
   * \brief Singleton para obtener una referencia única
   */
  static MessageManager &getInstance();

  /*!
   * \brief message
   * \param[in] msg
   * \return
   */
  //static MessageManager &message(const char *msg, ...);

  /*!
   * \brief Lanza un mensaje para que aquellos objetos que estén subscritos lo reciban
   * \param[in] msg Mensaje que se lanza
   * \param[in] level
   * \param[in] file 
   * \param[in] line
   * \param[in] function
   */
  static void release(const char *msg, const MessageLevel &level, const char *file = NULL, int line = -1, const char *function = NULL);

  /*!
   * \brief Lanza un mensaje para que aquellos objetos que estén subscritos lo reciban
   * \param[in] msg Mensaje que se lanza
   */
  static void release(const Message &msg);

  /*!
   * \brief Inicializa los manejadores para las librerias externas
   */
  static void initExternalHandlers();

protected:

  void onDebug(const char *msg, const char *date);
  void onVerbose(const char *msg, const char *date);
  void onInfo(const char *msg, const char *date);
  void onWarning(const char *msg, const char *date);
  void onError(const char *msg, const char *date);

};

#ifdef _DEBUG
#  define msgDebug(...)    EXPERIMENTAL::MessageManager::release(EXPERIMENTAL::MessageManager::Message(__VA_ARGS__).getMessage(), EXPERIMENTAL::MessageLevel::MSG_DEBUG, __FILE__, __LINE__, I3D_FUNCTION);
#  define msgVerbose(...)  EXPERIMENTAL::MessageManager::release(EXPERIMENTAL::MessageManager::Message(__VA_ARGS__).getMessage(), EXPERIMENTAL::MessageLevel::MSG_VERBOSE, __FILE__, __LINE__, I3D_FUNCTION);
#  define msgInfo(...)     EXPERIMENTAL::MessageManager::release(EXPERIMENTAL::MessageManager::Message(__VA_ARGS__).getMessage(), EXPERIMENTAL::MessageLevel::MSG_INFO, __FILE__, __LINE__, I3D_FUNCTION);
#  define msgWarning(...)  EXPERIMENTAL::MessageManager::release(EXPERIMENTAL::MessageManager::Message(__VA_ARGS__).getMessage(), EXPERIMENTAL::MessageLevel::MSG_WARNING, __FILE__, __LINE__, I3D_FUNCTION);
#  define msgError(...)    EXPERIMENTAL::MessageManager::release(EXPERIMENTAL::MessageManager::Message(__VA_ARGS__).getMessage(), EXPERIMENTAL::MessageLevel::MSG_ERROR, __FILE__, __LINE__, I3D_FUNCTION);
#else
#  define msgDebug(...)    EXPERIMENTAL::MessageManager::release(EXPERIMENTAL::MessageManager::Message(__VA_ARGS__).getMessage(), EXPERIMENTAL::MessageLevel::MSG_DEBUG);
#  define msgVerbose(...)  EXPERIMENTAL::MessageManager::release(EXPERIMENTAL::MessageManager::Message(__VA_ARGS__).getMessage(), EXPERIMENTAL::MessageLevel::MSG_VERBOSE);
#  define msgInfo(...)     EXPERIMENTAL::MessageManager::release(EXPERIMENTAL::MessageManager::Message(__VA_ARGS__).getMessage(), EXPERIMENTAL::MessageLevel::MSG_INFO);
#  define msgWarning(...)  EXPERIMENTAL::MessageManager::release(EXPERIMENTAL::MessageManager::Message(__VA_ARGS__).getMessage(), EXPERIMENTAL::MessageLevel::MSG_WARNING);
#  define msgError(...)    EXPERIMENTAL::MessageManager::release(EXPERIMENTAL::MessageManager::Message(__VA_ARGS__).getMessage(), EXPERIMENTAL::MessageLevel::MSG_ERROR);
#endif

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Clase para gestionar ficheros log
 *
 * Esta clase puede funcionar individualmente o si se subscribe a
 * al gestor de mensajes (MessageManager) recibe automaticamente
 * los mensajes
 */
class Log : public MessageManager::Listener
{

private:

  /*!
   * \brief sObjMessage
   */
  static std::unique_ptr<Log> sObjLog;

  /*!
   * \brief Fichero log
   */
  static std::string sLogFile;

  static MessageLevel mLevel;

  //std::string mMessage;

  /*!
   * \brief Plantilla para el formateo de fecha y hora de los mensajes del log.
   *
   * Por defecto la plantilla es:
   * \code
   * std::string MessageManager::timeLogTemplate = "%d/%b/%Y %H:%M:%S";
   * \endcode
   * \see setTimeLogFormat
   */
  static std::string sTimeLogFormat;

private:

  /*!
   * \brief Constructora privada
   */
  Log() {}

public:

  /*!
   * \brief Destructora
   */
  ~Log() {}

  Log(Log const&) = delete;
  void operator=(Log const&) = delete;

  /*!
   * \brief Singleton para obtener una referencia única
   */
  static Log &getInstance();

  void setLogFile(const char* file);

  void setLogLevel(MessageLevel level);

  /*!
   * \brief Escribe una linea en el log
   * \param msg Mensaje que se escribe en el log
   * \param date Fecha y hora del mensaje
   */
  void write(const char *msg);

protected:

  /*!
   * \brief onMsgDebug
   * \param msg
   * \param date
   */
  void onMsgDebug(const char *msg, const char *date) override;

  /*!
   * \brief onMsgVerbose
   * \param msg
   * \param date
   */
  void onMsgVerbose(const char *msg, const char *date) override;

  /*!
   * \brief onMsgInfo
   * \param msg
   * \param date
   */
  void onMsgInfo(const char *msg, const char *date) override;

  /*!
   * \brief onMsgWarning
   * \param msg
   * \param date
   */
  void onMsgWarning(const char *msg, const char *date) override;

  /*!
   * \brief onMsgError
   * \param msg
   * \param date
   */
  void onMsgError(const char *msg, const char *date) override;

  /*!
   * \brief Escribe una linea en el log
   * \param msg Mensaje que se escribe en el log
   * \param date Fecha y hora del mensaje
   */
  void _write(const char *msg, const char *date);

  //static std::string messageOutput(const MessageLevel &msgLevel, const char *file, int line, const char *function);

};


/*!
 * \brief Clase para gestionar la configuración de la ventana de comandos
 *
 * Permite modificar el color e intensidad de caracter y de fondo,
 * poner la consola en modo Unicode y cambiar el modo de consola (entrada,
 * salida, error)
 */
class I3D_EXPORT Console : public MessageManager::Listener
{
public:

  /*!
   * \brief Valores de intensidad de color
   */
  enum class Intensity : int8_t{
    NORMAL,  /*!< Normal */
    BRIGHT   /*!< Brillante */
  };

  /*!
   * \brief Tipos de color de fondo y caracter.
   */
  enum class Color : int8_t {
    BLACK,    /*!< Negro */
    RED,      /*!< Rojo */
    GREEN,    /*!< Verde */
    YELLOW,   /*!< Amarillo */
    BLUE,     /*!< Azul */
    MAGENTA,  /*!< Magenta */
    CYAN,     /*!< Cian */
    WHITE     /*!< Blanco */
  };

  /*!
   * \brief Modo de consola
   */
  enum class Mode : int8_t {
    INPUT,          /*!< Consola en modo entrada */
    OUTPUT,         /*!< Consola en modo salida */
    OUTPUT_ERROR    /*!< Consola en modo salida de errores */
  };

private:

#ifdef WIN32

  // Consola de Windows

  /*!
   * \brief Manejador de la consola
   */
  HANDLE h;

  /*!
   * \brief Intensidad de caracter
   */
  WORD mForeIntensity;

  /*!
   * \brief Color de caracteres
   */
  WORD mForeColor;

  /*!
   * \brief Intensidad de fondo
   */
  WORD mBackIntensity;

  /*!
   * \brief Color de fondo
   */
  WORD mBackColor;

  /*!
   * \brief Configuración de la consola al iniciar.
   *
   * La configuración inicial se recupera al salir o
   * con el método reset
   */
  WORD mOldColorAttrs;

#else

  // Consola Linux

  /*!
   * \brief mStream
   */
  FILE *mStream;

  /*!
   * \brief mCommand
   */
  char mCommand[13];

  /*!
   * \brief Intensidad de caracter
   */
  int mForeIntensity;

  /*!
   * \brief Color de caracteres
   */
  int mForeColor;

  /*!
   * \brief Intensidad de fondo
   */
  int mBackIntensity;

  /*!
   * \brief Color de fondo
   */
  int mBackColor;

#endif

public:

  /*!
   * \brief Constructora por defecto
   */
  Console();

  /*!
   * \brief Constructora de copia
   * \param[in] mode Modo de consola
   * \see Mode
   */
  Console(Console::Mode mode);

  /*!
   * Destructora
   */
  ~Console();

  /*!
   * \brief Recupera los valores iniciales
   */
  void reset();

  /*!
   * \brief Establece el color de caracter
   * \param[in] foreColor Color de caracter
   * \param[in] intensity Intensidad. El valor por defecto es Intensity::NORMAL
   * \see Console::Color, Console::Intensity
   */
  void setConsoleForegroundColor(Console::Color foreColor, Console::Intensity intensity = Console::Intensity::NORMAL);

  /*!
   * \brief Establece el color de fondo
   * \param[in] backColor Color de fondo
   * \param[in] intensity Intensidad. El valor por defecto es Intensity::NORMAL
   */
  void setConsoleBackgroundColor(Console::Color backColor, Console::Intensity intensity = Console::Intensity::NORMAL);

  /*!
   * \brief Establece la consola como modo Unicode
   */
  void setConsoleUnicode();

  /*!
   * \brief Imprime un mensaje en la consola
   */
  void printMessage(const char *msg);

protected:

  /*!
   * \brief onMsgDebug
   * \param msg
   * \param date
   */
  void onMsgDebug(const char *msg, const char *date) override;

  /*!
   * \brief onMsgVerbose
   * \param msg
   * \param date
   */
  void onMsgVerbose(const char *msg, const char *date) override;

  /*!
   * \brief onMsgInfo
   * \param msg
   * \param date
   */
  void onMsgInfo(const char *msg, const char *date) override;

  /*!
   * \brief onMsgWarning
   * \param msg
   * \param date
   */
  void onMsgWarning(const char *msg, const char *date) override;

  /*!
   * \brief onMsgError
   * \param msg
   * \param date
   */
  void onMsgError(const char *msg, const char *date) override;

private:

#ifdef WIN32

  /*!
   * \brief Inicializa la consola guardando la configuración  actual.
   * \param handle
   */
  void init( DWORD handle );

#else

  /*!
   * \brief Inicializa la consola guardando la configuración  actual.
   */
  void init(FILE *stream);

#endif

  /*!
   * \brief Actualiza la consola
   */
  void update();

};


} // End namespace EXPERIMENTAL

} // End namespace I3D

#endif // I3D_EXPERIMENTAL_H
