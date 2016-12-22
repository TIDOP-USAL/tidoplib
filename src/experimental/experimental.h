/*
 Clases y funciones experimentales
*/

#ifndef I3D_EXPERIMENTAL_H
#define I3D_EXPERIMENTAL_H

#include <vector>
#include <memory>

#include "opencv2/core/core.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/xphoto.hpp"

#include "core/defs.h" // Para quitar warnings de sfm
#include "graphic_entities/color.h"

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
#include "libmv\simple_pipeline\tracks.h"
I3D_DEFAULT_WARNINGS
#endif

#include "matching.h"

namespace I3D
{

namespace EXPERIMENTAL
{


// Clase para matching robusto. Necesitaria que tener una clase matching 
// virtual donde se definan todos los métodos comunes
// con esta clase sustituire a nRobustViewMatching y tendre mayor control. 

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

#endif



/*!
 * \brief Clase base virtual para algoritmos de lineas
 *
 */
class LineAlgorithms /*: public std::iterator<std::bidirectional_iterator_tag, int>*/  //deberia ser un iterador
{
public:

  enum class Type
  {
    BRESENHAM,
    DDA
  };

protected:

  Type mType;

  cv::Point mPt1;

  cv::Point mPt2;

  int mStepX;

  int mStepY;

  cv::Point mPos;

  int dx;

  int dy;

public:

  LineAlgorithms(Type type) : mType(type) {}

  LineAlgorithms(Type type, const cv::Point &pt1, const cv::Point &pt2) 
    : mType(type), mPt1(pt1), mPt2(pt2)
  {
    dx = pt2.x - pt1.x;
    dy = pt2.y - pt1.y;
  }

  virtual ~LineAlgorithms() {}

  /*!
   * \brief Determina la posición actual
   * \return Posición actual
   */
  virtual cv::Point position(int id = -1) = 0;

};

/*!
 * \brief Algoritmo de Bresenham para líneas
 *
 * Un algoritmo preciso y efectivo para la generación de líneas de rastreo, 
 * desarrollado por Bresenham (1965), convierte mediante rastreo las líneas 
 * utilizando solo cálculos incrementales con enteros que se pueden adaptar para
 * desplegar también curvas.
 * El algoritmo busca cual de dos pixeles es el que esta mas cerca según la
 * trayectoria de la línea.
 */
class BresenhamLine : public LineAlgorithms
{
public:

  int mCount;

public:

  BresenhamLine(const cv::Point &pt1, const cv::Point &pt2) 
    : LineAlgorithms(LineAlgorithms::Type::BRESENHAM, pt1, pt2) 
  {
    init();
  }

  ~BresenhamLine() {}

  /*!
   * \brief Punto actual
   */
  cv::Point &operator*();

  /*!
   * \brief Incrementa una posición
   */
  BresenhamLine &operator ++();
  
  /*!
   * \brief Incrementa una posición
   */
  BresenhamLine operator ++(int);

  /*!
   * \brief Decrementa una posición
   */
  //BresenhamLine &operator --();
  
  /*!
   * \brief Decrementa una posición
   */
  //BresenhamLine operator --(int);

  bool operator==(const BresenhamLine& bl) {return mPt2==bl.mPos;}
  bool operator!=(const BresenhamLine& bl) {return mPt2!=bl.mPos;}

  BresenhamLine &begin();

  BresenhamLine &end();

  /*!
   * \brief Determina la posición actual o la posición correspondiente al indice
   * El indice es la coordenada x o y del punto en función de que dx > dy o dx < dy
   * \param[in] id Indice del punto
   * \return Posición actual
   */
  cv::Point position(int id = -1) override;

  int size() const;

private:

  void init();

  void _next(int *max, int *min, int dMax, int dMin, int endMax, int stepMax, int stepMin);

};



/*!
 * \brief Algoritmo DDA (analizador diferenciador digital)
 *
 * El algoritmo DDA (Digital Differential Analyzer) es un algoritmo 
 * de conversion de rastreo que se basa en el calculo ya sea de Dy 
 * o Dx por medio de una de las ecuaciones:
 * \f$ Dy = m * Dx \f$<BR>
 * \f$ Dx = Dy / m \f$<BR>
 * Se efectúa un muestreo de la línea en intervalos unitarios en una 
 * coordenada y se determina los valores enteros correspondientes mas  
 * próximos a la trayectoria de la línea para la otra coordenada.
 */
class DDA : public LineAlgorithms
{
public:

  DDA(const cv::Point &pt1, const cv::Point &pt2)
    : LineAlgorithms(LineAlgorithms::Type::DDA, pt1, pt2)
  {
    init();
  }

  ~DDA() {}

  /*!
   * \brief Punto actual
   */
  cv::Point &operator*()
  {
    return mPos;
  }

  /*!
   * \brief Incrementa una posición
   */
  DDA &operator ++()
  {
    if (dx > dy) {
      _next(&mPos.x, &mPos.y, dx, dy, mPt2.x, mStepX, mStepY);
    } else {
      _next(&mPos.y, &mPos.x, dy, dx, mPt2.y, mStepY, mStepX);
    }
    return *this;
  }
  
  /*!
   * \brief Incrementa una posición
   */
  DDA operator ++(int) 
  {
    DDA it = *this;
    ++(*this);
    return it;
  }

  virtual cv::Point position(int id = -1) override 
  {
    if (id == -1) {
      return mPos;
    } else {
      //if (dx > dy) {
      //  _next(&mPos.x, &mPos.y, dx, dy, mPt2.x, mStepX, mStepY);
      //} else {
      //  _next(&mPos.y, &mPos.x, dy, dx, mPt2.y, mStepY, mStepX);
      //}
    }
  }

private:

  void init()
  {
    if (dy < 0) {
      dy = -dy;
      mStepY = -1;
    } else {
      mStepY = 1;
    }

    if (dx < 0) {
      dx = -dx;
      mStepX = -1;
    } else {
      mStepX = 1;
    }

    //if (dx > dy) {
    //  mCount = dx + 1;
    //} else {
    //  mCount = dy + 1;
    //}
  }

  void _next(int *max, int *min, int dMax, int dMin, int endMax, int stepMax, int stepMin) 
  {
    float m = (float)dMin / (float)dMax;
    float b = *min - m * *max;
    while (*max != endMax) {
      *max += dx;
      *min = I3D_ROUND_TO_INT(m * *max + b);
    }
  }
};

/*!
 * Algoritmo de trazado de lineas en el que se acumula los incrementos desde el origen
 *
 */
//class Line : public LineAlgorithms
//{
//
//public:
//
//  DDA(const cv::Point &pt1, const cv::Point &pt2)
//    : LineAlgorithms(LineAlgorithms::Type::DDA, pt1, pt2)
//  {
//    init();
//  }
//
//  ~DDA() {}
//
//  /*!
//   * \brief Punto actual
//   */
//  cv::Point &operator*()
//  {
//    return mPos;
//  }
//
//  /*!
//   * \brief Incrementa una posición
//   */
//  DDA &operator ++()
//  {
//    if (dx > dy) {
//      _next(&mPos.x, &mPos.y, dx, dy, mPt2.x, mStepX, mStepY);
//    } else {
//      _next(&mPos.y, &mPos.x, dy, dx, mPt2.y, mStepY, mStepX);
//    }
//    return *this;
//  }
//  
//  /*!
//   * \brief Incrementa una posición
//   */
//  DDA operator ++(int) 
//  {
//    DDA it = *this;
//    ++(*this);
//    return it;
//  }
//
//  virtual cv::Point position(int id = -1) override 
//  {
//    if (id == -1) {
//      return mPos;
//    } else {
//      //if (dx > dy) {
//      //  _next(&mPos.x, &mPos.y, dx, dy, mPt2.x, mStepX, mStepY);
//      //} else {
//      //  _next(&mPos.y, &mPos.x, dy, dx, mPt2.y, mStepY, mStepX);
//      //}
//    }
//  }
//
//private:
//
//  void init()
//  {
//    if (dy < 0) {
//      dy = -dy;
//      mStepY = -1;
//    } else {
//      mStepY = 1;
//    }
//
//    if (dx < 0) {
//      dx = -dx;
//      mStepX = -1;
//    } else {
//      mStepX = 1;
//    }
//
//    //if (dx > dy) {
//    //  mCount = dx + 1;
//    //} else {
//    //  mCount = dy + 1;
//    //}
//  }
//
//  void _next(int *max, int *min, int dMax, int dMin, int endMax, int stepMax, int stepMin) 
//  {
//    float m = (float)dMin / (float)dMax;
//    float b = *min - m * *max;
//    while (*max != endMax) {
//      *max += dx;
//      *min = I3D_ROUND_TO_INT(m * *max + b);
//    }
//  }
//};





// Balance de blancos
// https://courses.cs.washington.edu/courses/cse467/08au/labs/l5/whiteBalance.pdf

// 10.4.1 Gray World
//   Lo implementa OpenCV como cv::xphoto::GrayworldWB
// 10.4.2 White Patch
//
// 10.4.3 Iterative White Balancing  
//
// 10.4.4 Illuminant Voting
//
// 10.4.5 Color by Correlation










} // End namespace EXPERIMENTAL

} // End namespace I3D

#endif // I3D_EXPERIMENTAL_H
