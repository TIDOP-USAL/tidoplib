#ifndef I3D_MATCHING_H
#define I3D_MATCHING_H

#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"

#include "core/defs.h"
#include "core/utils.h"
#include "geometric_entities/window.h"

namespace I3D
{

/*!
 * \brief The Features2D class
 */
class I3D_EXPORT Features2D
{
private:

  /*!
   * \brief keyPoints
   */
  std::vector<cv::KeyPoint> mKeyPoints;

  /*!
   * \brief Descriptores
   */
  cv::Mat mDescriptor;

  /*!
   * \brief Detector de caracteristicas
   */
  cv::Ptr<cv::FeatureDetector> mFeatureDetector;

  /*!
   * \brief Extractor de descriptores
   */
  cv::Ptr<cv::DescriptorExtractor> mDescriptorExtractor;

public:

  /*!
   * \brief Constructora por defecto Features2D
   */
  Features2D(){}

  /*!
   * \brief Constructora Features2D
   * Se le pasan como parametro un detector de caracteristicas y un extractor de descriptores
   * \param[in] _fd Detector de caracteristicas
   * \param[in] _de Extractor de descriptores
   *
   * Detectores soportados:
   * - cv::AgastFeatureDetector::create()
   * - cv::AKAZE::create();
   * - cv::BRISK::create();
   * - cv::FastFeatureDetector::create();
   * - cv::GFTTDetector::create();
   * - cv::KAZE::create();
   * - cv::MSER::create();
   * - cv::ORB::create();
   * - cv::SimpleBlobDetector::create();
   * - cv::xfeatures2d::BriefDescriptorExtractor::create();
   * - cv::xfeatures2d::DAISY::create();
   * - cv::xfeatures2d::FREAK::create();
   * - cv::xfeatures2d::LATCH::create();
   * - cv::xfeatures2d::LUCID::create();
   * - cv::xfeatures2d::MSDDetector::create();
   * - cv::xfeatures2d::SIFT::create();
   * - cv::xfeatures2d::StarDetector::create();
   * - cv::xfeatures2d::SURF::create();
   *
   * Descriptores:
   * - cv::BRISK::create();
   * - cv::ORB::create();
   * - cv::AKAZE::create();
   * - cv::xfeatures2d::SIFT::create();
   * - cv::xfeatures2d::SURF::create();
   */
  Features2D(const cv::Ptr<cv::FeatureDetector> &fd, const cv::Ptr<cv::DescriptorExtractor> &de) 
    : mFeatureDetector(fd), mDescriptorExtractor(de) { }

  /*!
   * \brief Destructora Features2D
   */
  ~Features2D();

  /*!
   * \brief Establece el detector de caracteristicas
   * \param[in] fd cv::FeatureDetector
   */
  void setFeatureDetector(const cv::Ptr<cv::FeatureDetector> &fd) { mFeatureDetector = fd; }

  /*!
   * \brief Establece el extractor de descriptores
   * \param[in] de cv::DescriptorExtractor
   */
  void setDescriptorExtractor(const cv::Ptr<cv::DescriptorExtractor> &de) { mDescriptorExtractor = de; }

  /*!
   * \brief DetectKeyPoints
   * \param[in] img Imagen en  la que se quieren detectar los key points
   * \param[out] keyPoints Si existe este parámetro recibe una copia de los keyPoints
   * \param[in] mask Mascara opcional
   * \return Número de Key points detectados
   */
  int detectKeyPoints(const cv::Mat &img, std::vector<cv::KeyPoint> *keyPoints = NULL, const cv::InputArray &mask = cv::noArray());

  /*!
   * \brief Calcula los Descriptores
   * \param[in] img Imagen en  la que se quieren extraer los detectores
   * \param[out] keyPoints Si se pasa como NULL utiliza los keyPoints internos
   * \param[in] descriptor Si existe este parámetro recibe una copia de los descriptores
   */
  void calcDescriptor(const cv::Mat &img, std::vector<cv::KeyPoint> *keyPoints = NULL, cv::Mat *descriptor = NULL);

  /*!
   * \brief Guarda en un fichero los keypoints y los descriptores
   * \param[in] fname Nombre del archivo
   */
  void save( const char *fname ) const;

  /*!
   * \brief Lectura de un fichero con los keypoints y descriptores
   * \param[in] fname Nombre del archivo
   */
  void read( const char *fname );

  /*!
   * \brief Devuelve los KeyPoints
   * \return KeyPoints
   */
  const std::vector<cv::KeyPoint> &getKeyPoints() const { return mKeyPoints; }

  /*!
   * \brief Devuelve un KeyPoint
   * \return KeyPoint
   */
  const cv::KeyPoint &getKeyPoint(int id) const { return mKeyPoints[id]; }

  /*!
   * \brief Devuelve los descriptores
   * \return Descriptores
   */
  const cv::Mat &getDescriptors() const { return mDescriptor;}

  /*!
   * \brief Filtrado de keypoints por ventana
   * Los puntos que estén fuera de la ventana se eliminan
   * \param[in] in Imagen de entrada
   * \param[in] w Ventana
   * \param[out] out Imagen de salida. Recortada según la ventana
   * \param[out] _keyPoints Si existe este parametro se devuelve una copia de los key points
   * \param[out] _descriptor Si existe este parametro se devuelve una copia de los descriptores
   */
  template<typename T> void filter(const cv::Mat &in, const Window<T> &w, cv::Mat *out, std::vector<cv::KeyPoint> *_keyPoints, cv::Mat *_descriptor) {
    Window<T> waux = w;
    for (int j = 0; j < mKeyPoints.size(); j++) {
      if (!waux.containsPoint(mKeyPoints[j].pt)) {
        mKeyPoints.erase(mKeyPoints.begin() + j);
        j--;
      } else mKeyPoints[j].pt = mKeyPoints[j].pt - (cv::Point2f)waux.pt1;
    }
    in.colRange(waux.pt1.x, waux.pt2.x).rowRange(waux.pt1.y, waux.pt2.y).copyTo(*out);
    calcDescriptor(*out, &mKeyPoints, &mDescriptor); // Se recalculan los descriptores
    if (_keyPoints) *_keyPoints = mKeyPoints;
    if (_descriptor) mDescriptor.copyTo(*_descriptor);
  }

};

/*!
 * \brief Clase Matching
 */
class I3D_EXPORT Matching
{
private:

  /*!
   * \brief matches
   */
  std::vector< cv::DMatch > mMatches;

  /*!
   * \brief descriptorMatcher
   */
  cv::Ptr<cv::DescriptorMatcher> mDescriptorMatcher;

public:

  /*!
   * \brief Constructora por defecto de la clase Matching
   */
  Matching()
  {
    mDescriptorMatcher = cv::DescriptorMatcher::create("FlannBased");
  }

  /*!
   * \brief Constructora de la clase Matching
   * \param[in] dm
   */
  Matching(const cv::Ptr<cv::DescriptorMatcher> &dm) : mDescriptorMatcher(dm){}
  
  /*!
   * \brief Destructora de la clase Matching
   */
  //~Matching();

  /*!
  * \brief Match
  * \param[in] descriptor1
  * \param[in] descriptor2
  * \param[out] matches
  * \return Número de matches detectados
  */
  int match(const cv::Mat &descriptor1, const cv::Mat &descriptor2, std::vector< cv::DMatch > *matches = NULL);

  /*!
   * \brief Match
   * \param[in] feat1
   * \param[in] feat2
   * \param[out] matches
   * \return
   */
  int match(const Features2D &feat1, const Features2D &feat2, std::vector< cv::DMatch > *matches = NULL);

  /*!
   * \brief GetMatches
   * \return
   */
  std::vector<cv::DMatch> *getMatches() { return &mMatches; }

  /*!
   * \brief GetGoodMatches
   * \param[out] gm
   * \param[in] ratio
   */
  void getGoodMatches(std::vector< cv::DMatch > *gm, double ratio = 0.05) const;

  void getGoodMatches(const Features2D &feat1, const Features2D &feat2, std::vector< cv::DMatch > *gm, double ratio = 0.1) const;
  void getGoodMatches(const std::vector<cv::KeyPoint> &keyPoints1, const std::vector<cv::KeyPoint> &keyPoints2, std::vector< cv::DMatch > *gm, double ratio) const;

  // http://stackoverflow.com/questions/17967950/improve-matching-of-feature-points-with-opencv
  void getGoodMatches(const Features2D &feat1, const Features2D &feat2, std::vector< cv::DMatch > *gm, double distance, double confidence) const;
  
  void getGoodMatches(const std::vector<cv::KeyPoint> &keyPoints1, const std::vector<cv::KeyPoint> &keyPoints2, std::vector< cv::DMatch > *gm, double distance, double confidence) const;

  /*!
   * \brief SetDescriptorMatcher
   * \param[in] dm
   */
  void setDescriptorMatcher(const cv::Ptr<cv::DescriptorMatcher> &dm){ mDescriptorMatcher = dm; }

};

} // End namespace I3D

#endif // I3D_MATCHING_H
