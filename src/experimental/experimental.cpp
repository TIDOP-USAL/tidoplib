#include "experimental.h"

#include "core\messages.h"

#include "opencv2/highgui/highgui.hpp"

namespace I3D
{

namespace EXPERIMENTAL
{

void Reconstruction3D::getKeyPointAndDescriptor(const std::vector<std::string> &imagesPaths, bool bSave) 
{
  mImagesPaths = imagesPaths;
  int size = static_cast<int>(mImagesPaths.size());
  mKeyPoints.resize(size);
  mDescriptor.resize(size);
  for (int i = 0; i < size; i++) {
    cv::Mat image = cv::imread(mImagesPaths[i].c_str(), cv::IMREAD_GRAYSCALE);

    std::vector<cv::KeyPoint> keyPoint;
    cv::Mat descriptor;
    // Se detectan los key points y los descriptores
    int nft = mFeature2D->detectKeyPoints(image, &keyPoint);
    mFeature2D->calcDescriptor(image, NULL, &descriptor);

    mKeyPoints[i] = keyPoint;
    mDescriptor[i] = descriptor;

    if (bSave) {
      char out[_MAX_PATH];
      changeFileExtension(mImagesPaths[i].c_str(), "xml", out);

      // Salvamos key points y los descriptores
      mFeature2D->save(out);
    }
  }
}

void Reconstruction3D::loadKeyPointAndDescriptor(const std::vector<std::string> &imagesPaths)
{
  mImagesPaths = imagesPaths;
  int size = static_cast<int>(mImagesPaths.size());
  mKeyPoints.resize(size);
  mDescriptor.resize(size);

  char out[I3D_MAX_PATH];

  for (int i = 0; i < size; i++) {
    changeFileExtension(mImagesPaths[i].c_str(), "xml", out);
    mFeature2D->read(out);
    mKeyPoints[i] = mFeature2D->getKeyPoints();
    mDescriptor[i] = mFeature2D->getDescriptors();
  }
}

//void Reconstruction3D::multiImageMatching(std::vector<std::string> &images_paths) 
//{
//
//}
  
void Reconstruction3D::multiImageMatching( std::vector<cv::Mat> &points2d) 
{
  if (mDescriptor.empty()) return;
  int size = static_cast<int>(mDescriptor.size());
  std::vector<std::vector<cv::Vec2d> > pass_points;
  std::vector<std::vector<cv::Vec2d> > passPointsImage(size);
  for (int i = 0; i < size - 1; i++) {
    for (int j = i+1; j < size; j++) {

#ifdef _DEBUG
      cv::Mat image1 = cv::imread(mImagesPaths[i].c_str());
      cv::Mat image2 = cv::imread(mImagesPaths[j].c_str());
#endif

      mMatcher.match(mDescriptor[i], mDescriptor[j]);

      std::vector<cv::DMatch> good_matches;   
      //mMatcher.getGoodMatches(&good_matches, 0.5);
      mMatcher.getGoodMatches(mKeyPoints[i], mKeyPoints[j], &good_matches, 0.05);

#ifdef _DEBUG
      // drawing the results
      cv::Mat img_matches, img1, img2;
      cv::drawMatches(image1, mKeyPoints[i], image2, mKeyPoints[j], good_matches, img_matches, cv::Scalar::all(-1), cv::Scalar::all(-1), std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
#endif
      char name1[I3D_MAX_PATH];
      char name2[I3D_MAX_PATH];
      char buf[I3D_MAX_PATH];

      getFileName(mImagesPaths[i].c_str(), name1);
      getFileName(mImagesPaths[j].c_str(), name2);

      //sprintf_s(buf, 500, "%i matches seleccionados entre %s y %s", good_matches.size(), name1, name2);
      //progress_bar.init(0, static_cast<double>(good_matches.size()), buf);
      printInfo("%i matches seleccionados entre %s y %s", good_matches.size(), name1, name2);

      std::vector<cv::Point2f> pts1;
      std::vector<cv::Point2f> pts2;
      int idx1, idx2;
      for (size_t igm = 0; igm < good_matches.size(); igm++) {
        idx1 = good_matches[igm].queryIdx;
        idx2 = good_matches[igm].trainIdx;

        //Busqueda de si ya esta añadido ese punto
        bool addNew = true;
        std::vector<std::vector<int> > idx_pass_points;
        for (int k = 0; k < idx_pass_points.size(); k++) {
          if (idx_pass_points[k][i] == idx1) {
            idx_pass_points[k][j] = idx2;
            addNew = false;
          } else if (idx_pass_points[k][j] == idx2) {
            idx_pass_points[k][i] = idx1;
            addNew = false;
          }
        }

        if (addNew) {
          std::vector<int> v(size, -1);
          v[i] = idx1;
          v[j] = idx2;
          idx_pass_points.push_back(v);

          std::vector<cv::Vec2d> v_points(size, cv::Vec2d(-1, -1));
          cv::Point2f point1 = mKeyPoints[i][idx1].pt; //ft1.getKeyPoint(idx1).pt;
          cv::Point2f point2 = mKeyPoints[j][idx2].pt; //ft2.getKeyPoint(idx2).pt;
          v_points[i] = cv::Vec2d(point1.x, point1.y);
          v_points[j] = cv::Vec2d(point2.x, point2.y);
          pass_points.push_back(v_points);
          //passPointsImage[i];
          //passPointsImage[j];
        }
        //  //progress_bar();
      }
    }
  }

  int nPassPoint = static_cast<int>(pass_points.size());

  printInfo("Número de puntos de paso encontrados: %i", nPassPoint);

  // Formato necesario para sfm
  for (int i = 0; i < size; ++i) {
    cv::Mat_<double> frame(2, nPassPoint);
    for (int j = 0; j < nPassPoint; ++j)
    {
      frame(0,j) = pass_points[j][i][0];
      frame(1,j) = pass_points[j][i][1];
    }
    points2d.push_back(cv::Mat(frame));
  }
  //int nPoints = static_cast<int>(pass_points.size());
  //for (int i = 0; i < nPoints; ++i) {
  //  cv::Mat_<double> frame(2, size);
  //  for (int j = 0; j < size; ++j)
  //  {
  //    frame(0,j) = pass_points[i][j][0];
  //    frame(1,j) = pass_points[i][j][1];
  //  }
  //  points2d.push_back(cv::Mat(frame));
  //}
}

/*
Metric

Kilometre1
Metre1000
Decimetre10000
Centimetre100000
Millimetre1000000
Micrometre1000000000
Nanometre1E12
Angstrom1E13

British/American

League 0,21
Mile 0,62 
Furlong 4,97
Chain 49,71
Rod 198,84
Yard 1093,61
Foot 3280,84
Link 4970,97
Hand 9842,52
Inch 39370,08
Line 393700,79
Mil 39370078,74
Thou 39370078,74

Nautical

Sea mile 0,54
Fathom 546,81

*/







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
