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


///*!
// * \brief Tipos de entidades
// */
//enum class entity_type {
//  POINT_2D,                   /*!< Punto 2D */
//  POINT_3D,                   /*!< Punto 3D */
//  LINESTRING_2D,              /*!< Polilinea 2D */
//  LINESTRING_3D,              /*!< Polilinea 3D */
//  POLYGON_2D,                 /*!< Poligono 2D */
//  POLYGON_3D,                 /*!< Poligono 3D */
//  SEGMENT_2D,                 /*!< Segmento 2D */
//  SEGMENT_3D,                 /*!< Segmento 3D */
//  WINDOW,                     /*!< Ventana */
//  BBOX,                       /*!< BBOX */
//  MULTIPOINT_2D,              /*!< Multipunto 2D */
//  MULTIPOINT_3D,              /*!< Multipunto 3D */
//  MULTILINE_2D,
//  MULTILINE_3D,
//  MULTIPOLYGON_2D,
//  MULTIPOLYGON_3D
//};
//
///*!
// * \brief Clase base para entidades geometricas
// */
//template<typename T>
//class I3D_EXPORT Entity_
//{
//
//public:
//
//  /*!
//   * \brief type
//   */
//  typedef T value_type;
//
//protected:
//
//  /*!
//   * \brief Tipo de entidad
//   * \see entity_type
//   */
//  entity_type mEntityType;
//
//public:
//
//  /*!
//   * \brief Constructora
//   * \param[in] type Tipo de entidad
//   */
//  Entity_(entity_type type) : mEntityType(type) {}
//
//  /*!
//   * \brief Destructora
//   */
//  virtual ~Entity_() {}
//
//  /*!
//   * \brief Devuelve el tipo de entidad 
//   */
//  entity_type getType() const { return mEntityType; }
//};
//
//
//template<typename Point_t>
//class I3D_EXPORT EntityPoints_ : public Entity_<Point_t>
//{
//public:
//
//  /*!
//   * \brief type
//   */
//  typedef Point_t value_type;
//
//  /*!
//   * \brief Tipo de dato basico. int, float o double
//   */
//  typedef typename Point_t::value_type sub_type;
//
//protected:
//
//  /*!
//   * \brief Conjunto de puntos
//   */
//  std::vector<Point_t> mPoints;
//
//public:
//
//  /*!
//   * \brief Constructora por defecto
//   * \param[in] type Tipo de entidad
//   */
//  EntityPoints_(entity_type type);
//
//  /*!
//   * \brief Constructor que reserva tamaño para n puntos
//   * \param[in] type Tipo de entidad
//   * \param[in] size Tamaños que se reserva
//   */
//  EntityPoints_(entity_type type, int size);
//
//  /*!
//   * \brief Constructor de copia
//   * \param[in] type Tipo de entidad
//   * \param[in] EntityPoints_ Objeto EntityPoints_ que se copia
//   */
//  EntityPoints_(entity_type type, const EntityPoints_ &EntityPoints_);
//
//  /*!
//   * \brief Constructor
//   * \param[in] type Tipo de entidad
//   * \param[in] vPoint vector de puntos
//   */
//  EntityPoints_(entity_type type, const std::vector<Point_t> &vPoint);
//
//  /*!
//   * \brief Constructor lista de inicialización
//   * \param[in] type Tipo de entidad
//   * \param[in] listPoints Inicializador de lista con los puntos
//   */
//  EntityPoints_(entity_type type, std::initializer_list<Point_t> listPoints);
//
//  //~EntityPoints_();
//
//  /*!
//   * \brief Sobrecarga del operador de asignación
//   * \param[in] EntityPoints_ EntityPoints_ que se asigna
//   * \return Referencia al EntityPoints_
//   */
//  EntityPoints_ &operator = (const EntityPoints_ &EntityPoints_);
//
//  /*!
//   * \brief Añade un punto a la colección
//   * \param[in] point Punto que se añade
//   */
//  virtual void add(const Point_t &point) = 0;
//
//  /*!
//   * \brief Devuelve los puntos de la colección
//   * \return vector de puntos
//   */
//  const std::vector<Point_t> &getPoints() const { return mPoints; }
//
//  /*!
//   * \brief Devuelve los puntos que esta dentro de una ventana
//   * \param[in] w Ventana
//   * \return Puntos que entran dentro de la ventana
//   */
//  std::vector<Point_t> getPointsInWindow(const Window<sub_type> &w) const;
//
//  /*!
//   * \brief Número de puntos de la colección
//   * \return Número de puntos
//   */
//  size_t getSize() const { return mPoints.size(); }
//
//  /*!
//   * \brief Ventana envolvente
//   * \return Ventana envolvente de los puntos
//   */
//  Window<sub_type> getWindow() const;
//
//  /*!
//   * \brief resize
//   * \param[in] size nuevo tamaño
//   */
//  void resize(int size) { mPoints.resize(size); }
//
//  /*!
//   * \brief Operador de indexación sobrecargado
//   * \param[in] id indice del elemento
//   * \return Point_t
//   */
//  Point_t &operator[](size_t id) { return mPoints[id];  }
//
//  /*!
//   * \brief Iterador al inicio
//   */
//  virtual typename std::vector<Point_t>::iterator begin();
//
//  /*!
//   * \brief Iterador constante al inicio
//   */
//  virtual typename std::vector<Point_t>::const_iterator begin() const;
//
//  /*!
//   * \brief Iterador al final
//   */
//  virtual typename std::vector<Point_t>::iterator end();
//
//  /*!
//   * \brief Iterador constante al final
//   */
//  virtual typename std::vector<Point_t>::const_iterator end() const;
//};
//
//
//template<typename Point_t> inline
//EntityPoints_<Point_t>::EntityPoints_(entity_type type) 
//  : Entity<Point_t>(type), mPoints(0) {}
//
//template<typename Point_t> inline
//EntityPoints_<Point_t>::EntityPoints_(entity_type type, int size) 
//  : Entity<Point_t>(type), mPoints(size) {}
//
//template<typename Point_t> inline
//EntityPoints_<Point_t>::EntityPoints_(entity_type type, const EntityPoints_ &EntityPoints_) 
//  : Entity<Point_t>(type), mPoints(EntityPoints_.mPoints) {}
//
//template<typename Point_t> inline
//EntityPoints_<Point_t>::EntityPoints_(entity_type type, const std::vector<Point_t> &vPoint) 
//  : Entity<Point_t>(type), mPoints(vPoint) {}
//
//template<typename Point_t> inline
//EntityPoints_<Point_t>::EntityPoints_(entity_type type, std::initializer_list<Point_t> EntityPoints_)
//  : Entity<Point_t>(type), mPoints(EntityPoints_) {}
//
//template<typename Point_t> inline
//EntityPoints_<Point_t> &EntityPoints_<Point_t>::operator = (const EntityPoints_ &EntityPoints_)
//{
//  if (this != &EntityPoints_) {
//    this->mPoints = EntityPoints_.mPoints;
//    this->mEntityType = EntityPoints_.mEntityType;
//  }
//  return *this;
//}
//
//template<typename Point_t> inline
//std::vector<Point_t> EntityPoints_<Point_t>::getPointsInWindow(const Window<sub_type> &w) const
//{ 
//  std::vector<Point_t> r_points(mPoints.size());
//  int j = 0;
//  for (size_t i = 0; i < mPoints.size(); i++) {
//    if (w.containsPoint(mPoints[i])) {
//      r_points[i] = mPoints[i];
//      j++;
//    }
//  }
//  r_points.resize(j);
//  return r_points;
//}
//
//template<typename Point_t> inline
//Window<typename EntityPoints_::sub_type> EntityPoints_<Point_t>::getWindow() const
//{
//  Window<sub_type> w;
//  for (size_t i = 0; i < mPoints.size(); i++) {
//    if (w.pt1.x > mPoints[i].x) w.pt1.x = mPoints[i].x;
//    if (w.pt1.y > mPoints[i].y) w.pt1.y = mPoints[i].y;
//    if (w.pt2.x < mPoints[i].x) w.pt2.x = mPoints[i].x;
//    if (w.pt2.y < mPoints[i].y) w.pt2.y = mPoints[i].y;
//  }
//  return w;
//}
//
//template<typename Point_t> inline 
//typename std::vector<Point_t>::iterator EntityPoints_<Point_t>::begin() 
//{
//  return mPoints.begin();
//}
//
//template<typename Point_t> inline 
//typename std::vector<Point_t>::const_iterator EntityPoints_<Point_t>::begin() const 
//{
//  return mPoints.cbegin();
//}
//
//template<typename Point_t> inline 
//typename std::vector<Point_t>::iterator EntityPoints_<Point_t>::end()
//{
//  return mPoints.end();
//}
//
//template<typename Point_t> inline 
//typename std::vector<Point_t>::const_iterator EntityPoints_<Point_t>::end() const 
//{
//  return mPoints.cend();
//}
//
//
//




} // End namespace EXPERIMENTAL

} // End namespace I3D

#endif // I3D_EXPERIMENTAL_H
