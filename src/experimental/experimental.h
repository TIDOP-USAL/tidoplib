/*
 Clases y funciones experimentales
*/

#ifndef I3D_EXPERIMENTAL_H
#define I3D_EXPERIMENTAL_H

#include <vector>
#include <memory>

#include "opencv2/core/core.hpp"
#include "opencv2/core/utility.hpp"

#include "matching.h"

namespace I3D
{

namespace EXPERIMENTAL
{



class RobustMatcher {
public:
  RobustMatcher() : ratio_(0.8f)
  {
    // ORB is the default feature
    detector_ = cv::ORB::create();
    extractor_ = cv::ORB::create();

    // BruteFroce matcher with Norm Hamming is the default matcher
    matcher_ = cv::makePtr<cv::BFMatcher>((int)cv::NORM_HAMMING, false);

  }
  virtual ~RobustMatcher();

  // Set the feature detector
  void setFeatureDetector(const cv::Ptr<cv::FeatureDetector>& detect) {  detector_ = detect; }

  // Set the descriptor extractor
  void setDescriptorExtractor(const cv::Ptr<cv::DescriptorExtractor>& desc) { extractor_ = desc; }

  // Set the matcher
  void setDescriptorMatcher(const cv::Ptr<cv::DescriptorMatcher>& match) {  matcher_ = match; }

  // Compute the keypoints of an image
  void computeKeyPoints( const cv::Mat& image, std::vector<cv::KeyPoint>& keypoints);

  // Compute the descriptors of an image given its keypoints
  void computeDescriptors( const cv::Mat& image, std::vector<cv::KeyPoint>& keypoints, cv::Mat& descriptors);

  // Set ratio parameter for the ratio test
  void setRatio( float rat) { ratio_ = rat; }

  // Clear matches for which NN ratio is > than threshold
  // return the number of removed points
  // (corresponding entries being cleared,
  // i.e. size will be 0)
  int ratioTest(std::vector<std::vector<cv::DMatch> > &matches);

  // Insert symmetrical matches in symMatches vector
  void symmetryTest( const std::vector<std::vector<cv::DMatch> >& matches1,
                     const std::vector<std::vector<cv::DMatch> >& matches2,
                     std::vector<cv::DMatch>& symMatches );

  // Match feature points using ratio and symmetry test
  void robustMatch( const cv::Mat& frame, std::vector<cv::DMatch>& good_matches,
                      std::vector<cv::KeyPoint>& keypoints_frame,
                      const cv::Mat& descriptors_model );

 // Match feature points using ratio test
 void fastRobustMatch( const cv::Mat& frame, std::vector<cv::DMatch>& good_matches,
                       std::vector<cv::KeyPoint>& keypoints_frame,
                       const cv::Mat& descriptors_model );

private:
  // pointer to the feature point detector object
  cv::Ptr<cv::FeatureDetector> detector_;
  // pointer to the feature descriptor extractor object
  cv::Ptr<cv::DescriptorExtractor> extractor_;
  // pointer to the matcher object
  cv::Ptr<cv::DescriptorMatcher> matcher_;
  // max ratio between 1st and 2nd NN
  float ratio_;
};















class Reconstruction3D
{

  std::vector<cv::Mat> points2d;

  std::vector<std::string> mImagesPaths;


  /*!
   * \brief keyPoints
   */
  std::vector<std::vector<cv::KeyPoint>> mKeyPoints;

  /*!
   * \brief Descriptores
   */
  std::vector<cv::Mat> mDescriptor;

  /*!
   * \brief Detector de caracteristicas
   */
  cv::Ptr<cv::FeatureDetector> mFeatureDetector;

  /*!
   * \brief Extractor de descriptores
   */
  cv::Ptr<cv::DescriptorExtractor> mDescriptorExtractor;

  std::unique_ptr<Features2D> mFeature2D;

  Matching mMatcher;

  cv::FlannBasedMatcher mMatcherFB;

public:

  /*!
   * \brief Cosntructora por defecto
   */
  Reconstruction3D() { 
    mFeature2D = std::make_unique<Features2D>(cv::ORB::create(10000), cv::xfeatures2d::DAISY::create());
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
  Reconstruction3D(const cv::Ptr<cv::FeatureDetector> &fd, const cv::Ptr<cv::DescriptorExtractor> &de) 
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

  int ratioTest(std::vector<std::vector<cv::DMatch> > &matches, float ratio = 0.7f);
  int ratioTest2(std::vector<std::vector<cv::DMatch> > &matches, float ratio = 0.8f);
  void symmetryTest(const std::vector<std::vector<cv::DMatch> >& matches1,
                    const std::vector<std::vector<cv::DMatch> >& matches2,
                    std::vector<cv::DMatch>& symMatches);
  void saveTracks(const std::string &filename, std::vector<std::vector<cv::Vec2d> > passPoints);
};


} // End namespace EXPERIMENTAL

} // End namespace I3D

#endif // I3D_EXPERIMENTAL_H
