#include "experimental.h"

#include "opencv2/highgui/highgui.hpp"

namespace I3D
{

namespace EXPERIMENTAL
{

void Reconstruction3D::getKeyPointAndDescriptor(std::vector<std::string> &images_paths, bool bSave) 
{
  int size = images_paths.size();
  mKeyPoints.resize(size);
  mDescriptor.resize(size);
  for (int i = 0; i < size; i++) {
    cv::Mat image = cv::imread(images_paths[i].c_str(), cv::IMREAD_GRAYSCALE);

    std::vector<cv::KeyPoint> keyPoint;
    cv::Mat descriptor;
    // Se detectan los key points y los descriptores
    int nft = mFeature2D->detectKeyPoints(image, &keyPoint);
    mFeature2D->calcDescriptor(image, NULL, &descriptor);

    mKeyPoints[i] = keyPoint;
    mDescriptor[i] = descriptor;

    if (bSave) {
      char out[_MAX_PATH];
      changeFileExtension(images_paths[i].c_str(), "xml", out);

      // Salvamos key points y los descriptores
      mFeature2D->save(out);
    }
  }
}

void Reconstruction3D::loadKeyPointAndDescriptor(std::vector<std::string> &images_paths)
{
  int size = images_paths.size();
  mKeyPoints.resize(size);
  mDescriptor.resize(size);

  char out[I3D_MAX_PATH];

  for (int i = 0; i < size; i++) {
    changeFileExtension(images_paths[i].c_str(), "xml", out);
    mFeature2D->read(out);
    mKeyPoints[i] = mFeature2D->getKeyPoints();
    mDescriptor[i] = mFeature2D->getDescriptors();
  }
}

} // End namespace EXPERIMENTAL

} // End namespace I3D


  ///*!
  // * \brief keyPoints
  // */
  //std::vector<std::vector<cv::KeyPoint>> mKeyPoints;

  ///*!
  // * \brief Descriptores
  // */
  //std::vector<cv::Mat> mDescriptor;
