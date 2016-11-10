/*
 Clases y funciones experimentales
*/

#ifndef I3D_FLAGS_H
#define I3D_FLAGS_H

#include <vector>
#include <memory>

#include "opencv2/core/core.hpp"
#include "opencv2/core/utility.hpp"

#include "matching.h"

namespace I3D
{

namespace EXPERIMENTAL
{

class Reconstruction3D
{
  //std::vector<std::string> images_paths;
  std::vector<cv::Mat> points2d;

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

  cv::FlannBasedMatcher matcherA;

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

  void getKeyPointAndDescriptor(std::vector<std::string> &images_paths, bool bSave = true);

  void loadKeyPointAndDescriptor(std::vector<std::string> &images_paths);

private:

};


} // End namespace EXPERIMENTAL

} // End namespace I3D

#endif // I3D_FLAGS_H
