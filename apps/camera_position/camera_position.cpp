#include <iostream>
#include <string>
#include "opencv2/calib3d.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core/utility.hpp"
//#include "opencv2/ximgproc/disparity_filter.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"

//#include <pcl/common/common_headers.h>
//#include <pcl/io/pcd_io.h>
//#include <pcl/visualization/pcl_visualizer.h>
//#include <boost/thread/thread.hpp>
//#include <pcl/visualization/cloud_viewer.h>
//#include <pcl/point_types.h>

#include "matching.h"
//#include "elas.h"
//#include "image.h"
//#include "transform.h"
#include "geometric_entities\window.h"

using namespace I3D;
using namespace cv;
//using namespace cv::ximgproc;
//using namespace std;
using namespace cv::xfeatures2d;

static const char *img[] = { 
  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-09-26-11h52m50s015.png",
  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-09-26-11h53m12s688.png",
  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-09-26-11h53m38s082.png",
  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-09-26-11h53m48s544.png",
  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-09-26-11h53m55s447.png",
  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-09-26-11h54m06s036.png",
  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-09-26-11h54m33s634.png",
  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-09-26-11h54m39s630.png",
  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-09-26-11h54m44s427.png",
  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-09-26-11h54m49s920.png",
  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-09-26-11h54m55s996.png",
  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-09-26-11h55m03s330.png",
  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-09-26-11h55m11s494.png",
  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-09-26-11h55m18s254.png",
  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-09-26-11h55m25s110.png"
};

static void LoadCameraParams(std::string &file, Size &imageSize, Mat &cameraMatrix, Mat& distCoeffs)
{
  FileStorage fs(file, FileStorage::READ);
  fs["image_width"] >> imageSize.width;
  fs["image_height"] >> imageSize.height;
  fs["camera_matrix"] >> cameraMatrix;
  fs["distortion_coefficients"] >> distCoeffs;
  fs.release();
}

int main(int argc, char** argv)
{

  cv::Ptr<cv::FeatureDetector> fd = SURF::create();
  cv::Ptr<cv::DescriptorExtractor> de = SURF::create();
  Features2D features(fd, de);
  Matching match(cv::DescriptorMatcher::create("FlannBased"));

  // Se cargan datos de calibración de la cámara
  Size imageSize;
  Mat cameraMatrix, distCoeffs;
  std::string file = "D://Esteban//Ingenio3000//Imagenes_Para_Calibracion_GoPro//video_1280x720//out_camera_data.xml";
  LoadCameraParams(file, imageSize, cameraMatrix, distCoeffs);

  // 1. Carga de las imagenes y determinación de key points y los descriptores
  int size = sizeof(img) / sizeof(img[0]);
  for (int i = 0; i < size; i++) {
    cv::Mat image  = imread(img[i] ,IMREAD_COLOR);

    // Se detectan los key points y los descriptores
    int nft = features.detectKeyPoints(image);
    features.calcDescriptor(image);
    char out[_MAX_PATH];
    changeFileExtension(img[i], "xml", out);

    // Salvamos key points y los descriptores
    features.save(out);
  }

  // 2. Calculamos matching entre todas las imagenes e identificamos puntos homologos
  //... Por ahora buscar sólo entre fotos consecutivas
  //... Posteriormente buscar en todas las fotos para darle mayor robustez
  
  std::vector<std::vector<int> > idx_pass_points;
  // Puntos de paso
  std::vector<std::vector<Vec2d> > pass_points(size);

  std::vector<DMatch> matches;
  
  //// Guarda todos los matches entre todas las fotos
  //std::vector<std::vector<DMatch>> all_matches;

  char out[_MAX_PATH];
  //for (int i = 0; i < size - 1; i++) {
  //  Features2D ft1, ft2;
  //  changeFileExtension(img[i], "xml", out);
  //  ft1.read(out);
  //  for (int j = i; j < size; j++) {
  //    changeFileExtension(img[j], "xml", out);
  //    ft2.read(out);

  //    match.match(ft1, ft2, &matches);

  //    // Esto hay que hacerlo mejor
  //    std::vector<DMatch> good_matches;
  //    match.getGoodMatches(&good_matches, 0.08);
  //    all_matches.push_back(good_matches);
  //  }
  //}

  for (int i = 1; i < size - 1; i++) {
    Features2D ft1, ft2;
    changeFileExtension(img[i-1], "xml", out);
    ft1.read(out);
    changeFileExtension(img[i], "xml", out);
    ft2.read(out);

    match.match(ft1, ft2, &matches);

    // Esto hay que hacerlo mejor
    std::vector<DMatch> good_matches;
    match.getGoodMatches(&good_matches, 0.08);

    std::vector<Point2f> pts1;
    std::vector<Point2f> pts2;
    int idx1, idx2;
    for (size_t igm = 0; igm < good_matches.size(); igm++) {
      idx1 = good_matches[igm].queryIdx;
      idx2 = good_matches[igm].trainIdx;

      //Busqueda de si ya esta añadido ese punto
      bool addNew = true;
      for (int j = 0; j < idx_pass_points.size(); j++) {
        if (idx_pass_points[j][i - 1] == idx1 ) {
          idx_pass_points[j][i] = idx2;
          addNew = false;
        } else if (idx_pass_points[j][i] == idx2) {
          idx_pass_points[j][i - 1] = idx1;
          addNew = false;
        }
      }
      if (addNew) {
        std::vector<int> v(size, -1);
        v[i - 1] = idx1;
        v[i] = idx2;
        idx_pass_points.push_back(v);
      }

      //pts1.push_back(ft1.getKeyPoint(good_matches[igm].queryIdx).pt);
      //pts2.push_back(ft2.getKeyPoint(good_matches[igm].trainIdx).pt);
    }
  }


    //std::vector<Vec2d> track;
    //std::vector<Point2f> pts1;
    //std::vector<Point2f> pts2;
    //for (size_t igm = 0; igm < good_matches.size(); igm++) {
    //  pts1.push_back(ft1.getKeyPoint(good_matches[igm].queryIdx).pt);
    //  pts2.push_back(ft2.getKeyPoint(good_matches[igm].trainIdx).pt);
    //}

  // Calculo de la posición de la camara
  // http://docs.opencv.org/3.1.0/d5/dab/tutorial_sfm_trajectory_estimation.html
  // 

  return 0;
}