#include <iostream>
#include <string>

#include "opencv2/calib3d.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/sfm.hpp"
#include <opencv2/viz.hpp>
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/ximgproc/disparity_filter.hpp"

#include <pcl/common/common_headers.h>
#include <pcl/io/pcd_io.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <boost/thread/thread.hpp>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/point_types.h>

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
#include "core\console.h"
#include "core\messages.h"

using namespace I3D;
using namespace cv;
using namespace cv::ximgproc;
//using namespace std;
using namespace cv::xfeatures2d;

//static const char *img[] = { 
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-09-26-11h52m50s015.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-09-26-11h53m12s688.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-09-26-11h53m38s082.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-09-26-11h53m48s544.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-09-26-11h53m55s447.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-09-26-11h54m06s036.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-09-26-11h54m33s634.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-09-26-11h54m39s630.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-09-26-11h54m44s427.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-09-26-11h54m49s920.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-09-26-11h54m55s996.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-09-26-11h55m03s330.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-09-26-11h55m11s494.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-09-26-11h55m18s254.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-09-26-11h55m25s110.png"
//};

//static const char *img[] = { 
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h11m44s431.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h11m53s105.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h12m07s446.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h12m15s221.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h12m21s600.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h12m28s542.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h12m35s938.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h12m43s663.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h12m48s203.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h12m52s344.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h12m56s457.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h13m03s100.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h13m11s196.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h13m19s768.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h13m26s062.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h13m32s039.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h13m36s354.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h13m40s321.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h13m46s479.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h13m55s357.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h14m02s369.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h14m12s194.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h14m17s234.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h14m21s161.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h14m24s341.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h14m27s741.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h14m37s087.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h14m41s486.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h14m45s173.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h14m48s469.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h14m52s337.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h14m56s968.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h15m01s982.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h15m06s788.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h15m10s386.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h15m14s985.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h15m18s594.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h15m23s648.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h15m29s253.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h15m32s387.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h15m36s342.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h15m40s528.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h15m44s332.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h15m50s835.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h15m54s609.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h15m58s859.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h16m02s144.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h16m08s162.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h16m12s877.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h16m16s574.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h16m24s337.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h16m29s934.png",
//  "C:\\Users\\Tido\\Pictures\\vlcsnap-2016-10-24-13h16m33s578.png"
//};

static const char *img[] = { 
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h08m50s737.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h08m56s325.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h09m00s431.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h09m06s731.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h09m11s371.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h09m17s941.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h09m23s339.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h09m27s417.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h09m32s852.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h09m37s075.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h09m42s135.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h09m46s148.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h09m49s401.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h09m55s406.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h10m00s108.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h10m06s073.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h10m10s846.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h10m15s090.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h10m22s543.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h10m27s912.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h10m32s618.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h10m38s468.png"/*,
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h10m43s198.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h10m47s583.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h10m51s120.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h10m55s684.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h11m00s384.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h11m05s135.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h11m10s380.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h11m14s882.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h11m19s333.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h11m25s689.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h11m30s900.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h11m36s461.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h11m41s292.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h11m45s853.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h11m50s941.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h11m56s580.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h12m02s091.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h12m07s431.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h12m13s234.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h12m18s170.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h12m31s624.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h12m38s811.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h12m42s327.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h12m45s971.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h12m50s577.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h12m54s214.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h12m59s020.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h13m02s555.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h13m05s897.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h13m09s239.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h13m12s847.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h13m17s068.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h13m21s036.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h13m24s880.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h13m29s836.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h13m33s386.png",
  "C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\vlcsnap-2016-10-24-16h13m39s127.png"*/
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

bool camera_pov = false;

void keyboard_callback(const viz::KeyboardEvent &event, void* cookie)
{
  if ( event.action == 0 &&!event.symbol.compare("s") )
    camera_pov = !camera_pov;
}

int getdir(const std::string _filename, std::vector<std::string> &files)
{
  ifstream myfile(_filename.c_str());
  if (!myfile.is_open()) {
    cout << "Unable to read file: " << _filename << endl;
    exit(0);
  } else {
    size_t found = _filename.find_last_of("/\\");
    std::string line_str, path_to_file = _filename.substr(0, found);
    while ( getline(myfile, line_str) )
      files.push_back(path_to_file+std::string("/")+line_str);
  }
  return 1;
}

int main(int argc, char** argv)
{
  ProgressBar progress_bar;

  Message::setMessageLevel(MessageLevel::MSG_INFO);

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

  //progress_bar.init(0, size, "Detección de key points y descriptores");
  //for (int i = 0; i < size; i++) {
  //  cv::Mat image  = imread(img[i] ,IMREAD_COLOR);

  //  // Se detectan los key points y los descriptores
  //  int nft = features.detectKeyPoints(image);
  //  features.calcDescriptor(image);
  //  char out[_MAX_PATH];
  //  changeFileExtension(img[i], "xml", out);

  //  // Salvamos key points y los descriptores
  //  features.save(out);
  //  progress_bar();
  //}

  // 2. Calculamos matching entre todas las imagenes e identificamos puntos homologos
  //... Por ahora buscar sólo entre fotos consecutivas
  //... Posteriormente buscar en todas las fotos para darle mayor robustez
  
  std::vector<std::vector<int> > idx_pass_points;
  // Puntos de paso
  std::vector<std::vector<Vec2d> > pass_points;
  std::vector<DMatch> matches;
  
  char out[I3D_MAX_PATH];
  char buf[I3D_MAX_PATH];
  char name1[I3D_MAX_PATH];
  char name2[I3D_MAX_PATH];

  /*
  for (int i = 1; i < size - 1; i++) {
    Features2D ft1, ft2;
    changeFileExtension(img[i-1], "xml", out);
    ft1.read(out);
    changeFileExtension(img[i], "xml", out);
    ft2.read(out);

    match.match(ft1, ft2, &matches);

    // Esto hay que hacerlo mejor
    std::vector<DMatch> good_matches;
    //match.getGoodMatches(&good_matches, 0.05);
    match.getGoodMatches(ft1, ft2, &good_matches, 0.1);

    getFileName(img[i-1], name1);
    getFileName(img[i], name2);

    sprintf_s(buf, 500, "%i matches seleccionados entre %s y %s", good_matches.size(), name1, name2);

    progress_bar.init(0, static_cast<double>(good_matches.size()), buf);

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

        std::vector<Vec2d> v_points(size,Vec2d(-1,-1));
        cv::Point2f point1 = ft1.getKeyPoint(idx1).pt;
        cv::Point2f point2 = ft2.getKeyPoint(idx2).pt;
        v_points[i - 1] = Vec2d(point1.x, point1.y);
        v_points[i] = Vec2d(point2.x, point2.y);
        pass_points.push_back(v_points);
      }
      progress_bar();
    }
  }
  */

  for (int i = 0; i < size - 1; i++) {
    for (int j = i+1; j < size; j++) {
      Features2D ft1, ft2;
      changeFileExtension(img[i], "xml", out);
      ft1.read(out);
      changeFileExtension(img[j], "xml", out);
      ft2.read(out);

      match.match(ft1, ft2, &matches);

      // Esto hay que hacerlo mejor
      std::vector<DMatch> good_matches;
      //match.getGoodMatches(&good_matches, 0.5);
      match.getGoodMatches(ft1, ft2, &good_matches, 0.8);

      // drawing the results
      cv::Mat img_matches, img1, img2;
      img1 = cv::imread(img[i]);
      img2 = cv::imread(img[j]);
      cv::drawMatches(img1, ft1.getKeyPoints(), img2, ft2.getKeyPoints(), good_matches, img_matches, Scalar::all(-1), Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);


      getFileName(img[i], name1);
      getFileName(img[j], name2);

      sprintf_s(buf, 500, "%i matches seleccionados entre %s y %s", good_matches.size(), name1, name2);

      progress_bar.init(0, static_cast<double>(good_matches.size()), buf);

      std::vector<Point2f> pts1;
      std::vector<Point2f> pts2;
      int idx1, idx2;
      for (size_t igm = 0; igm < good_matches.size(); igm++) {
        idx1 = good_matches[igm].queryIdx;
        idx2 = good_matches[igm].trainIdx;

        //Busqueda de si ya esta añadido ese punto
        bool addNew = true;
        for (int k = 0; k < idx_pass_points.size(); k++) {
          if (idx_pass_points[k][i] == idx1 ) {
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

          std::vector<Vec2d> v_points(size,Vec2d(-1,-1));
          cv::Point2f point1 = ft1.getKeyPoint(idx1).pt;
          cv::Point2f point2 = ft2.getKeyPoint(idx2).pt;
          v_points[i] = Vec2d(point1.x, point1.y);
          v_points[j] = Vec2d(point2.x, point2.y);
          pass_points.push_back(v_points);
        }
        progress_bar();
      }
    }
  }

  printInfo("Número de puntos de paso encontrados: %i", pass_points.size());


  // Calculo de la posición de la camara
  // http://docs.opencv.org/3.1.0/d5/dab/tutorial_sfm_trajectory_estimation.html
  // En primer lugar en el ejemplo parsea un archivo y lo almacena como std::vector<Mat> points2d;
  // Seria interesante leer y escribir este archivo
  // Despues establece ls matriz de calibración de la camara.
  // Luego llama a la función reconstruct(points2d, Rs_est, ts_est, K, points3d_estimated, is_projective);

  // Se necesita el modulo SFM(Structure from Motion) de OpenCV
  // Hay que volver a compilar con las dependencias Eigen, GLog, GFlags y Ceres Solver.
  std::vector<Mat> points2d;
  for (int i = 0; i < size; ++i) {
    Mat_<double> frame(2, pass_points.size());
    for (int j = 0; j < pass_points.size(); ++j)
    {
      frame(0,j) = pass_points[j][i][0];
      frame(1,j) = pass_points[j][i][1];
    }
    points2d.push_back(Mat(frame));
  }

  Matx33d K = cameraMatrix;

  std::vector<Mat> Rs_est, ts_est, points3d_estimated;

  // Parse the image paths
  std::vector<std::string> images_paths;
  getdir( argv[1], images_paths );

  try {
    bool is_projective = true;
    cv::sfm::reconstruct(points2d, Rs_est, ts_est, K, points3d_estimated, is_projective);
    //cv::sfm::reconstruct(images_paths, Rs_est, ts_est, K, points3d_estimated, is_projective);
  } catch (cv::Exception &e) {
    printError(e.what());
  } catch (std::exception &e) {
    printError(e.what());
  }
  
    // Print output

  cout << "\n----------------------------\n" << endl;
  cout << "Reconstruction: " << endl;
  cout << "============================" << endl;
  cout << "Estimated 3D points: " << points3d_estimated.size() << endl;
  cout << "Estimated cameras: " << Rs_est.size() << endl;
  cout << "Refined intrinsics: " << endl << K << endl << endl;

  cout << "3D Visualization: " << endl;
  cout << "============================" << endl;

  /// Create 3D windows
  viz::Viz3d window_est("Estimation Coordinate Frame");
             window_est.setBackgroundColor(); // black by default
             window_est.registerKeyboardCallback(&keyboard_callback);
    // Create the pointcloud
  cout << "Recovering points  ... ";

  // recover estimated points3d
  std::vector<Vec3f> point_cloud_est;
  for (int i = 0; i < points3d_estimated.size(); ++i)
    point_cloud_est.push_back(Vec3f(points3d_estimated[i]));

  cout << "[DONE]" << endl;


  /// Recovering cameras
  cout << "Recovering cameras ... ";

  std::vector<Affine3d> path_est;
  for (size_t i = 0; i < Rs_est.size(); ++i)
    path_est.push_back(Affine3d(Rs_est[i],ts_est[i]));

  //cout << "[DONE]" << endl;

  ///// Add cameras
  //cout << "Rendering Trajectory  ... ";

  ///// Wait for key 'q' to close the window
  //cout << endl << "Press:                       " << endl;
  //cout <<         " 's' to switch the camera pov" << endl;
  //cout <<         " 'q' to close the windows    " << endl;


  //if ( path_est.size() > 0 )
  //{
  //  // animated trajectory
  //  //int idx = 0, forw = -1, n = static_cast<int>(path_est.size());

  //  while(!window_est.wasStopped())
  //  {
  //    /// Render points as 3D cubes
  //    for (size_t i = 0; i < point_cloud_est.size(); ++i)
  //    {
  //      Vec3d point = point_cloud_est[i];
  //      Affine3d point_pose(Mat::eye(3,3,CV_64F), point);

  //      char buffer[50];
  //      sprintf (buffer, "%d", static_cast<int>(i));

  //      viz::WCube cube_widget(Point3f(0.1,0.1,0.0), Point3f(0.0,0.0,-0.1), true, viz::Color::blue());
  //                 cube_widget.setRenderingProperty(viz::LINE_WIDTH, 2.0);
  //      window_est.showWidget("Cube"+string(buffer), cube_widget, point_pose);
  //    }

  //    Affine3d cam_pose = path_est[0/*idx*/];

  //    viz::WCameraPosition cpw(0.25); // Coordinate axes
  //    viz::WCameraPosition cpw_frustum(K, 0.3, viz::Color::yellow()); // Camera frustum

  //    if ( camera_pov )
  //      window_est.setViewerPose(cam_pose);
  //    else
  //    {
  //      // render complete trajectory
  //      window_est.showWidget("cameras_frames_and_lines_est", viz::WTrajectory(path_est, viz::WTrajectory::PATH, 1.0, viz::Color::green()));

  //      window_est.showWidget("CPW", cpw, cam_pose);
  //      window_est.showWidget("CPW_FRUSTUM", cpw_frustum, cam_pose);
  //    }

  //    // update trajectory index (spring effect)
  //    //forw *= (idx==n || idx==0) ? -1: 1; idx += forw;

  //    // frame rate 1s
  //    window_est.spinOnce(1, true);
  //    window_est.removeAllWidgets();
  //  }

  //}


  // Mapa de disparidad para cada imagen
  int max_disp = 160;
  double lambda = 8000.;
  double sigma = 1.5;
  int wsize = 3;
  Mat left_for_matcher, right_for_matcher;
  Mat left_disp,right_disp;
  Mat filtered_disp;
  Rect ROI;
  Ptr<DisparityWLSFilter> wls_filter;
  double matching_time, filtering_time;

  //cv::Ptr<cv::FeatureDetector> fd = SURF::create();
  //cv::Ptr<cv::DescriptorExtractor> de = SURF::create();
  Features2D featuresL(fd, de);
  Features2D featuresR(fd, de);
  //Matching match(cv::DescriptorMatcher::create("FlannBased"));

  for (int i = 1; i < images_paths.size(); i++) {
    
    String left_im = images_paths[i-1];
    String right_im = images_paths[i];
    
    // Se cargan las imagenes
    Mat left  = imread(left_im ,IMREAD_COLOR);
    if ( left.empty() )
    {
        cout<<"Cannot read image file: "<<left_im;
        return -1;
    }

    Mat right = imread(right_im,IMREAD_COLOR);
    if ( right.empty() )
    {
        cout<<"Cannot read image file: "<<right_im;
        return -1;
    }

    Mat conf_map = Mat(left.rows,left.cols,CV_8U);
    conf_map = Scalar(255);

    Mat rleft, rright, map1, map2;
    Mat optCameraMat = getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, imageSize, 1, imageSize, 0);
    initUndistortRectifyMap(cameraMatrix, distCoeffs, Mat(), optCameraMat, imageSize, CV_16SC2, map1, map2);
    remap(left, left, map1, map2, INTER_LINEAR);
    remap(right, right, map1, map2, INTER_LINEAR);

    // Se aplica una mascara a la zona del apoyo
    cv::Mat maskL = cv::Mat::zeros(left.size(),CV_8U);
    cv::Mat maskR = cv::Mat::zeros(right.size(),CV_8U);
    //cv::rectangle(maskL, windowToCvRect(wL), cv::Scalar(1,1,1),cv::FILLED );
    //cv::rectangle(maskR, windowToCvRect(wR), cv::Scalar(1,1,1),cv::FILLED );
    //cv::Mat left_mask, right_mask;
    //cv::bitwise_and(left, left, left_mask, maskL);
    //cv::bitwise_and(right, right, right_mask, maskR);
    //left_mask.copyTo(left);
    //right_mask.copyTo(right);
    left_for_matcher  = left.clone();
    right_for_matcher = right.clone();

    //else if(algo=="sgbm") {
      Ptr<StereoSGBM> left_matcher  = StereoSGBM::create(0,max_disp,wsize);
      left_matcher->setP1(24*wsize*wsize);
      left_matcher->setP2(96*wsize*wsize);
      left_matcher->setPreFilterCap(63);
      left_matcher->setMode(StereoSGBM::MODE_SGBM_3WAY);
      wls_filter = createDisparityWLSFilter(left_matcher);
      Ptr<StereoMatcher> right_matcher = createRightMatcher(left_matcher);

      matching_time = (double)getTickCount();
      left_matcher-> compute(left_for_matcher, right_for_matcher,left_disp);
      right_matcher->compute(right_for_matcher,left_for_matcher, right_disp);
      matching_time = ((double)getTickCount() - matching_time)/getTickFrequency();
    //}

      //if (algo != "elas") {
        //! [filtering]
        wls_filter->setLambda(lambda);
        wls_filter->setSigmaColor(sigma);
        filtering_time = (double)getTickCount();
        wls_filter->filter(left_disp, left, filtered_disp, right_disp);
        filtering_time = ((double)getTickCount() - filtering_time) / getTickFrequency();
        //! [filtering]
        //conf_map = wls_filter->getConfidenceMap();

        // Get the ROI that was used in the last filter call:
        //ROI = wls_filter->getROI();
        //if (!no_downscale)
        //{
        //  // upscale raw disparity and ROI back for a proper comparison:
        //  resize(left_disp, left_disp, Size(), 2.0, 2.0);
        //  left_disp = left_disp*2.0;
        //  ROI = Rect(ROI.x * 2, ROI.y * 2, ROI.width * 2, ROI.height * 2);
        //}
      //}

    // Se detectan los key points y los descriptores
    //int nft = featuresL.detectKeyPoints(left);
    //featuresL.calcDescriptor(left);
    //nft = featuresR.detectKeyPoints(right);
    //featuresR.calcDescriptor(right);
  
    //std::vector<DMatch> matches;
    //match.match(featuresL, featuresR, &matches);
  
    //std::vector<DMatch> good_matches;
    //match.getGoodMatches(&good_matches,0.08);

    //std::vector<Point2f> ptsL;
    //std::vector<Point2f> ptsR;
    //for (size_t igm = 0; igm < good_matches.size(); igm++) {
    //  ptsL.push_back(featuresL.getKeyPoint(good_matches[igm].queryIdx).pt);
    //  ptsR.push_back(featuresR.getKeyPoint(good_matches[igm].trainIdx).pt);
    //}

    //I3D::Translate<Point2f> trfTranslate(wL.pt1.x,wL.pt1.y);
    //std::vector<Point2f> ptsLOut, ptsROut;
    //trfTranslate.transform(ptsL, &ptsLOut);
    //trfTranslate.setTranslation(wR.pt1.x, wR.pt1.y);
    //trfTranslate.transform(ptsR, &ptsROut);

    cv::Mat R, T;
    //// Lo mismo por OpenCV??
    //cv::Mat essentialMat = findEssentialMat(ptsLOut, ptsROut, cameraMatrix);
    //cv::recoverPose(essentialMat, ptsLOut, ptsROut, cameraMatrix, R, T);

    //... La matriz de rotación y las translaciones ya las tengo calculadas
    R = Rs_est[i];
    T = ts_est[i];

    cv::Mat R1, R2, P1, P2, Q;
    cv::stereoRectify(cameraMatrix, distCoeffs, cameraMatrix, distCoeffs, imageSize, R, T, R1, R2, P1, P2, Q);
    
    cv::Size size = left.size();

    cv::Mat_<cv::Vec3f> XYZ(size/*wL.getHeight(),wL.getWidth()*/);
    //cv::reprojectImageTo3D(filtered_disp,XYZ,Q);  //... Mejor no usar para poder filtrar la nube de puntos.
    cv::Mat_<double> vec_tmp(4,1);
  
    //for(int y=0; y < filtered_disp.rows; ++y) {
    //    for(int x=0; x < filtered_disp.cols; ++x) {
    for(int y=0; y < size.height; ++y) {
      for(int x=0; x < size.width; ++x) {
        vec_tmp(0)=x; 
        vec_tmp(1)=y; 
        vec_tmp(2)=filtered_disp.at<__int16>(y,x); 
        if(vec_tmp(2)==0) continue;
        vec_tmp(3)=1;
        vec_tmp = Q*vec_tmp;
        vec_tmp /= vec_tmp(3);
        if(abs(vec_tmp(0))>10 || abs(vec_tmp(1))>10 || abs(vec_tmp(2))>10) continue;
        cv::Vec3f &point = XYZ.at<cv::Vec3f>(y,x);
        point[0] = vec_tmp(0);
        point[1] = vec_tmp(1);
        point[2] = vec_tmp(2);
      }
    }

    // Crea nube de puntos
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr point_cloud_ptr(new pcl::PointCloud<pcl::PointXYZRGB>);

    //uchar pr, pg, pb;
    for (int i = 0; i < size.height; i++) {
      //uchar* rgb_ptr = left_for_matcher.ptr<uchar>(i);
      for (int j = 0; j < size.width; j++) {

        pcl::PointXYZRGB point;
        point.x = XYZ.at<cv::Vec3f>(i,j)[0];
        point.y = XYZ.at<cv::Vec3f>(i,j)[1];
        point.z = XYZ.at<cv::Vec3f>(i,j)[2];
        if (point.z < 1.0 || point.z > 0.5) {
          point.rgb = *left_for_matcher.ptr<float>(i,j);
          point_cloud_ptr->points.push_back(point);
        }
      }
    }
    point_cloud_ptr->width = (int)point_cloud_ptr->points.size();
    point_cloud_ptr->height = 1;


    std::unique_ptr<pcl::visualization::PCLVisualizer> viewer (new pcl::visualization::PCLVisualizer ("3D Viewer"));
    viewer->setBackgroundColor(0, 0, 0);
    pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGB> rgb(point_cloud_ptr);
    viewer->addPointCloud<pcl::PointXYZRGB> (point_cloud_ptr, rgb, "reconstruction");
    viewer->setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, "reconstruction");
    viewer->addCoordinateSystem( 1 );
    viewer->initCameraParameters();

    while (!viewer->wasStopped())
    {
      viewer->spinOnce(10); //100
      boost::this_thread::sleep(boost::posix_time::microseconds(100000));
    }

    pcl::io::savePCDFileASCII(std::string("D:\\Desarrollo\\datos\\test_pcd2.pcd"), *point_cloud_ptr);

  }


    

  return 0;
}
