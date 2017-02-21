/*
 Clases y funciones experimentales
*/

#ifndef I3D_EXPERIMENTAL_H
#define I3D_EXPERIMENTAL_H

#include <vector>
#include <memory>

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


/*!
 * \brief Niveles de información de los mensajes
 */
enum class MessageLevel : int8_t {
  MSG_DEBUG,      /*!< Información extra para depuración. */
  MSG_VERBOSE,    /*!< Información extra para depuración. */
  MSG_INFO,       /*!< Warnings, errores y otra información. */
  MSG_WARNING,    /*!< Warnings y errores. */
  MSG_ERROR       /*!< Sólo errores. */
};

class _Message
{

public:

  class Listener
  {
  protected:
    std::shared_ptr<_Message> mMessage;
  public:
    Listener()
    {
    }

    ~Listener()
    {
    }

    virtual void onMsgDebug(const char *msg, char mDate[64]) = 0;
    virtual void onMsgVerbose(const char *msg, char mDate[64]) = 0;
    virtual void onMsgInfo(const char *msg, char mDate[64]) = 0;
    virtual void onMsgWarning(const char *msg, char mDate[64]) = 0;
    virtual void onMsgError(const char *msg, char mDate[64]) = 0;

  };

private:

  static std::unique_ptr<_Message> sObjMessage;

  std::list<Listener *> mListeners;

  /*!
   * \brief Nivel de información de los mensajes
   *
   * Por defecto MSG_ERROR
   * \see MessageLevel
   */
  static MessageLevel sLevel;

  /*!
   * \brief sLastMessage
   */
  static std::string sLastMessage;

private:

  /*!
   * \brief Constructora Message
   */
  _Message();

public:
  ~_Message();

  void addListener(Listener *listener);

  _Message(_Message const&) = delete;
  
  void operator=(_Message const&) = delete;

  /*!
   * \brief Singleton para obtener una referencia única
   */
  static _Message &get();

  /*!
   * \brief Devuelve la cadena de texto con el mensaje
   * \return Mensaje
   */
  const char *getMessage() const;

  /*!
   * \brief message
   * \param[in] msg
   * \return
   */
  static _Message &message(const char *msg, ...);

  /*!
   * \brief Imprime un mensaje con las opciones establecidas
   */
  static void print();

  /*!
   * \brief Imprime un mensaje
   * \param[in] level Nivel del mensaje
   * \see MessageLevel
   */
  static void print(const MessageLevel &level);

  /*!
   * \brief Imprime un mensaje
   * \param[in] level Nivel del mensaje
   * \param[in] file Nombre del fichero
   * \param[in] line Número de línea
   * \param[in] function Nombre de función
   */
  static void print(const MessageLevel &level, const char *file, int line, const char *function);
 

  /*!
   * \brief Inicializa los manejadores para las librerias externas
   */
  static void initExternalHandlers();

protected:

  void onDebug(const char *msg, char mDate[64]);
  void onVerbose(const char *msg, char mDate[64]);
  void onInfo(const char *msg, char mDate[64]);
  void onWarning(const char *msg, char mDate[64]);
  void onError(const char *msg, char mDate[64]);

private:

  static std::string messageOutput(const MessageLevel &msgLevel);

  static std::string messageOutput(const MessageLevel &msgLevel, const char *file, int line, const char *function);

  static void _print(const MessageLevel &level, const std::string &msgOut);

};


class Log : public _Message::Listener
{
private:

  /*!
   * \brief logfile
   */
  static std::string sLogFile;

  char mDate[64];

  std::string mLevel;

  std::string mMessage;

  /*!
   * \brief Plantilla para el formateo de fecha y hora de los mensajes del log.
   *
   * Por defecto la plantilla es:
   * \code
   * std::string Message::timeLogTemplate = "%d/%b/%Y %H:%M:%S";
   * \endcode
   * \see setTimeLogFormat
   */
  static std::string sTimeLogFormat;

public:

  Log();
  
  ~Log();

  void onMsgDebug(const char *msg, char mDate[64]) override
  {

  }

  void onMsgVerbose(const char *msg, char mDate[64]) override
  {

  }

  void onMsgInfo(const char *msg, char mDate[64]) override
  {

  }

  void onMsgWarning(const char *msg, char mDate[64]) override
  {

  }

  void onMsgError(const char *msg, char mDate[64]) override
  {

  }

};




} // End namespace EXPERIMENTAL

} // End namespace I3D

#endif // I3D_EXPERIMENTAL_H
