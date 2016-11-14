#include "cloud_points.h"

#include <stdio.h>

#include "opencv2/calib3d.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/optflow.hpp"
I3D_SUPPRESS_WARNINGS
#include "opencv2/sfm.hpp"
I3D_DEFAULT_WARNINGS
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/ximgproc/disparity_filter.hpp"

I3D_DISABLE_WARNING(4512 4324 4702)
#include <pcl/common/common_headers.h>
#include <pcl/io/pcd_io.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <boost/thread/thread.hpp>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/point_types.h>
I3D_ENABLE_WARNING(4512 4324 4702)

//#include <xercesc/parsers/XercesDOMParser.hpp>
//#include <xercesc/dom/DOM.hpp>
//#include <xercesc/sax/HandlerBase.hpp>
//#include <xercesc/util/XMLString.hpp>
//#include <xercesc/util/PlatformUtils.hpp>

// Cabeceras tidopLib
#include "core\console.h"
#include "core\messages.h"
#include "videostream.h"
#include "geometric_entities\segment.h"
#include "linedetector.h"
#include "img_processing.h"
#include "matching.h"
#include "transform.h"
#include "experimental\experimental.h"

using namespace cv;
using namespace optflow;
using namespace I3D;
using namespace cv::ximgproc;
using namespace cv::xfeatures2d;
//using namespace xercesc;

static void LoadCameraParams(std::string &file, Size &imageSize, Mat &cameraMatrix, Mat& distCoeffs)
{
  FileStorage fs(file, FileStorage::READ);
  fs["image_width"] >> imageSize.width;
  fs["image_height"] >> imageSize.height;
  fs["camera_matrix"] >> cameraMatrix;
  fs["distortion_coefficients"] >> distCoeffs;
  fs.release();
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


void getTowerImageAndWindow(const char * file, std::vector<std::string> &images_paths, std::vector<WindowI> &windows) 
{
  std::ifstream read_handler(file, std::ifstream::in);
  if (!read_handler.is_open()) {
    consolePrintInfo("Unable to read file: %s", file);
  } else {
    std::vector<std::string> out;
    while (!read_handler.eof()) {
      std::string line;
      read_handler >> line;
      if ( !line.empty() && split(line, out, "|") == 0) {
        images_paths.push_back(out[0]);
        std::vector<int> coord;
        if (splitToNumbers(out[1], coord, ";") == 0) {
          windows.push_back(WindowI(cv::Point(coord[0], coord[1]), cv::Point(coord[2], coord[3])));
        }
      }
    }
  }
}

namespace I3D
{

void DetectTransmissionTower::detectGroupLines(const cv::Mat &img, std::vector<ldGroupLines> *linesGroup)
{
  cv::Mat imageGrayProcces;
  if (img.channels() != 1)
    cvtColor(img, imageGrayProcces, CV_BGR2GRAY);
  
  // Se aplican procesos previos a las imagenes
  imgprolist.execute(img, &imageGrayProcces);

  pLineDetector->run(imageGrayProcces);
  groupLinesByDist(pLineDetector->getLines(), linesGroup, 10);

  // Se eliminan los grupos con pocas lineas
  delLinesGroupBySize(linesGroup,5);
}

void DetectTransmissionTower::computeOpticalFlow(const cv::Mat &img1, const cv::Mat &img2, cv::Mat_<cv::Point2f> *flow)
{
  *flow = cv::Mat(img1.size[0], img1.size[1], CV_32FC2);
  algorithm->calc(img1, img2, *flow);
}

bool DetectTransmissionTower::run(const cv::Mat &img1, const cv::Mat &img2, cv::Mat *imgout, WindowI *wOut ) 
{ 
  cv::Size sz = img1.size(); 
  img1.copyTo(*imgout);
  
  ////Ventana en la cual se van a buscar los apoyos
  //WindowI ws(cv::Point(0, 0), cv::Point(sz.width, sz.height));
  //ws = expandWindow(ws, -100, 0);

  cv::Mat image1, image2;

  // Conversión a escala de grises
  cvtColor(img1, image1, CV_BGR2GRAY);
  cvtColor(img2, image2, CV_BGR2GRAY);
  
  std::vector<ldGroupLines> linesGroup1, linesGroup2;
  detectGroupLines(image1, &linesGroup1);
  detectGroupLines(image2, &linesGroup2);

  if (linesGroup1.size() > 0 && linesGroup2.size() > 0) {
    //determinación del flujo óptico
    cv::Mat_<cv::Point2f> flow;
    computeOpticalFlow(image1, image2, &flow);
    image1.release();
    image2.release();

    // Se comparan todos los grupos de lineas dos a dos
    for (int iplg = 0; iplg < linesGroup1.size(); iplg++) {
      WindowI wprev = linesGroup1[iplg].getBbox();
      for (int iclg = 0; iclg < linesGroup2.size(); iclg++) {
        WindowI wcur = linesGroup2[iclg].getBbox();

        // Se comprueba la ventana actual contiene el centro de la ventana previa
        if (wcur.containsPoint(wprev.getCenter())) {

          // Se obtiene la magnitud a partir del flujo óptico.
          cv::Mat magnitude;
          getMagnitude(flow, &magnitude);

          if (bool bTower = isTower(imgout, linesGroup1[iplg], magnitude)) {
            *wOut = linesGroup1[iplg].getBbox();
            return bTower;
          }
        }
      }
    }
  }
  return false;
}

void DetectTransmissionTower::getMagnitude(const cv::Mat_<cv::Point2f> &flow, cv::Mat *magnitude) 
{
  cv::Mat flow_split[2];
  split(flow, flow_split);
  cv::Mat angle; //... Anque no necesito el angulo tengo que pasarselo
  // Magnitud y angulo del desplazamiento. Sólo interesa la magnitud
  cartToPolar(flow_split[0], flow_split[1], *magnitude, angle, true);

}

bool DetectTransmissionTower::isTower(cv::Mat *imgout, const ldGroupLines &linesGroup1, const cv::Mat &magnitude) {

  WindowI wprev = linesGroup1.getBbox();

  // Normalizamos la magnitud para ver si hay valores mas altos en la zona de la posible torre
  cv::Scalar mean, stdv, meanc, stdvc;
  cv::Mat normMag, candidatenorm;
  cv::normalize(magnitude, normMag, 0, 1, NORM_MINMAX);
  cv::meanStdDev(normMag, mean, stdv);
  normMag.colRange(wprev.pt1.x, wprev.pt2.x).rowRange(wprev.pt1.y, wprev.pt2.y).copyTo(candidatenorm);
  cv::meanStdDev(candidatenorm, meanc, stdvc);

  if (mean[0] < meanc[0] && meanc[0] > 0.5 && stdvc[0] > 0.1 /*&& (mean[0] + stdv[0]) < meanc[0]*/) {

    c = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));

    // Buscamos los máximos en la zona
    std::vector<Point> pMax;
    cv::Mat idx;
    cv::sortIdx(candidatenorm, idx, CV_SORT_EVERY_ROW + CV_SORT_DESCENDING);
    for (int ir = 0; ir < candidatenorm.rows; ir++) {
      int id;
      for (int ic = 0; ic < 5; ic++) {
        id = idx.at<int>(ir, ic);
        float cmax = candidatenorm.at<float>(ir, id);
        if (cmax != 0. && cmax > mean[0] + stdv[0]) {
          cv::Point paux(wprev.pt1.x + id, wprev.pt1.y + ir);
          pMax.push_back(paux);
          //if (bDrawRegressionLine)
            //line(imgout, paux, paux, c, 1, cv::LINE_8);
        } else break;
      }
    }

    printVerbose("Frame %i - cols: %i - rows: %i", static_cast<int>(prevFrame), candidatenorm.cols, candidatenorm.rows);
    printVerbose("Frame %i - N points max: %i", static_cast<int>(prevFrame), pMax.size());
    if (pMax.size() > 200 ) {
      // Recta de regresión para los máximos
      double m = 0.;
      double b = 0.;
      regressionLinearXY(pMax, &m, &b);
      
      cv::Point pt1(I3D_ROUND_TO_INT(b), 0);
      cv::Point pt2(I3D_ROUND_TO_INT(m * magnitude.rows + b), magnitude.rows);
            
      //if (bDrawRegressionLine) {
      //  //Se pinta la recta de regresión
      //  line(imgout, pt1, pt2, cv::Scalar(0, 0, 150), 1, cv::LINE_8);
      //}
            

      //Buscamos sólo cuando la torre este mas vertical
      Line rr(pt1, pt2);
      double ang = rr.angleOY();
      if (ang > I3D_PI / 2)  ang = ang - I3D_PI;
      else if (ang < -I3D_PI / 2) ang = ang + I3D_PI;
      // tolerancia de inclinación del eje del apoyo respecto a la vertical -> 0.1
      if (ang <= 0.2 && ang >= -0.2) {
        printVerbose("Frame %i - Angulo: %f", static_cast<int>(prevFrame), ang);

         // Busqueda del máximo valor de desplazamiento
        std::vector<Point> vMagnitudes;
        cv::Point ptMax;
        float maxval = I3D_FLOAT_MIN;
        for (int ir = 0; ir < normMag.rows; ir++) {
          cv::Point pt(I3D_ROUND_TO_INT(m * ir + b), ir);
          if (pt.x >= 0 && pt.x < normMag.cols) {
            float mg = magnitude.at<float>(pt);
            if (mg > maxval){
              maxval = mg;
              ptMax = pt;
            }
            if (mg > mean[0])
              vMagnitudes.push_back(cv::Point(I3D_ROUND_TO_INT(mg), ir));
          }
        }
       
       
        // Aplicamos douglas-Peucker
        std::vector<Point> lmag;
        approxPolyDP(vMagnitudes, lmag, 3, false);
        //if (bDrawDP) {
        //  if (lmag.size() >= 3) {
        //    const cv::Point *cpts2 = (const cv::Point*) Mat(lmag).data;
        //    int npts = Mat(lmag).rows;
        //    cv::polylines(imgout, &cpts2, &npts, 1, false, Scalar(255, 255, 0), 2);
        //  }
        //}
       
        //El punto máximo tiene que estar en la parte superior
        if (abs(wprev.pt1.y - ptMax.y) <= 200) {
       
          //Rectangulo envolvente de la torre            
          //cv::rectangle(imgout, wprev.pt1, wprev.pt2, cv::Scalar(0, 255, 0), 1);
       
          //if (bDrawLines){
          //  for (int il = 0; il < linesGroup1.getSize(); il++) {
          //    cv::Point pt1 = linesGroup1[il].pt1;
          //    cv::Point pt2 = linesGroup1[il].pt2;
          //    line(imgout, pt1, pt2, cv::Scalar(0, 255, 0), 1, cv::LINE_8);
          //  }
          //}
          
       
          // Ajustar el BBOX mejor  
          printInfo("Torre detectada: Frame %i", static_cast<int>(prevFrame));
          //if (bSaveImages) {
          //  char buffer[50];
          //  sprintf_s(buffer, "Apoyo_%05i.jpg", cvRound(prevFrame));
          //  cv::imwrite(buffer, *imgout);
          //}
          return true; // Devolvemos que hemos encontrado una torre
        }
      } else printVerbose("Frame %i rechazado por angulo de recta de regresion mayor al limite. angulo=%f", I3D_ROUND_TO_INT(prevFrame), ang);
    }
  } else {
    printInfo("Torre rechazada: Frame %i", static_cast<int>(prevFrame) );
    return false;
  }
  return false;
}

} // End namespace I3D


/*!
 * Clase auxiliar para manejar los frames de video. Heredada 
 * de VideoStream::Listener e implementa los métodos que controlan
 * la ejecución de los eventos
 */
class VideoHelper : public VideoStream::Listener
{
public:

  double mCurrentPosition;

  cv::Mat mFramePrev;
  cv::Mat out;

  DetectTransmissionTower *mDetectTower;

  std::string outPath;
  std::string outFile;

  std::vector<std::string> framesSaved;
  std::vector<WindowI> windowsSaved;

  //DOMImplementation *impl;

public:

  VideoHelper(DetectTransmissionTower *detectTower) 
  {
    mDetectTower = detectTower;
    mCurrentPosition = 0.;
    outPath = "";
    outFile = "TowerDetected.txt";
  }

  ~VideoHelper() {}

  void onFinish() override;

  void onInitialize() override;

  void onPause() override;

  void onPositionChange(double position) override;

  void onRead(cv::Mat &frame) override;

  void onResume() override;

  void onShow(cv::Mat &frame) override;

  void onStop() override;

  void write();
};

void VideoHelper::onFinish()
{
  VideoStream::Listener::onFinish();

  write();
}

void VideoHelper::onInitialize()
{
  VideoStream::Listener::onInitialize();

  if (outPath.empty()) {
    char path[I3D_MAX_DRIVE + I3D_MAX_DIR];
    getFileDriveDir(getRunfile(),path);
    outPath = path;
  } else {
    createDir(outPath.c_str());
  }
}

void VideoHelper::onPause()
{
  VideoStream::Listener::onPause(); 
}

void VideoHelper::onPositionChange(double position) 
{ 
  VideoStream::Listener::onPositionChange(position);
  mDetectTower->prevFrame = position;
  mCurrentPosition = position;
}

void VideoHelper::onRead(cv::Mat &frame) 
{
  VideoStream::Listener::onRead(frame);
  if (!mFramePrev.empty()) {
    WindowI wOut;
    bool bTower = mDetectTower->run(mFramePrev, frame, &out, &wOut);
    frame.copyTo(mFramePrev);
    if (bTower) {
      char buffer[I3D_MAX_PATH];
      sprintf_s(buffer, "%s\\Apoyo_%05i.png", outPath.c_str(), I3D_ROUND_TO_INT(mCurrentPosition));
      cv::imwrite(buffer, out);
      
      framesSaved.push_back(std::string(buffer));
      windowsSaved.push_back(wOut);
    }
  } else {
    frame.copyTo(mFramePrev);
  }
}

void VideoHelper::onResume() 
{ 
  VideoStream::Listener::onResume();
}

void VideoHelper::onShow(cv::Mat &frame) 
{ 
  VideoStream::Listener::onShow(frame);
}

void VideoHelper::onStop()
{ 
  VideoStream::Listener::onStop(); 
}

void VideoHelper::write()
{ 
  char buffer[I3D_MAX_PATH];
  sprintf_s(buffer, "%s\\%s", outPath.c_str(), outFile.c_str());
  std::string name = outPath + "\\TowerDetected.txt";
  std::remove(buffer);
  std::ofstream hWrite(buffer);
  for (size_t i = 0; i < framesSaved.size(); ++i) {
    
    if (hWrite.is_open()) {
      hWrite << framesSaved[i].c_str() << "|" << windowsSaved[i].pt1.x <<  ";" << windowsSaved[i].pt1.y <<  ";" << windowsSaved[i].pt2.x <<  ";" << windowsSaved[i].pt2.y << "\n";
    }
  }
  hWrite.close();
}

void reconstruct(std::vector<std::string> &images_paths, std::vector<cv::Mat> &points2d)
{
  std::vector<std::vector<int> > idx_pass_points;
  // Puntos de paso
  std::vector<std::vector<cv::Vec2d> > pass_points;
  std::vector<cv::DMatch> matches;
  
  char out[I3D_MAX_PATH];
  char buf[I3D_MAX_PATH];
  char name1[I3D_MAX_PATH];
  char name2[I3D_MAX_PATH];

  cv::Ptr<cv::FeatureDetector> fdORB = cv::ORB::create(10000);
  cv::Ptr<cv::DescriptorExtractor> deDAISY = cv::xfeatures2d::DAISY::create();
  //Features2D ft1(fdORB, deDAISY);
  //Features2D ft2(fdORB, deDAISY);
  Matching match(cv::DescriptorMatcher::create("FlannBased"));
  cv::Ptr<cv::Feature2D> fd = cv::ORB::create(10000);
  cv::Ptr<cv::Feature2D> de = cv::xfeatures2d::DAISY::create();
  Features2D feature1(fd, de);
  Features2D feature2(fd, de);
  cv::FlannBasedMatcher matcherA;


  size_t size = images_paths.size();
  for (int i = 0; i < size - 1; i++) {
    for (int j = i+1; j < size; j++) {
      //Features2D ft1, ft2;
      //changeFileExtension(images_paths[i].c_str(), "xml", out);
      //ft1.read(out);
      //changeFileExtension(images_paths[j].c_str(), "xml", out);
      //ft2.read(out);
      cv::Mat image1  = imread(images_paths[i].c_str(), IMREAD_GRAYSCALE);
      cv::Mat image2  = imread(images_paths[j].c_str(), IMREAD_GRAYSCALE);
      //ft1.detectKeyPoints(image1);
      //ft1.calcDescriptor(image1);
      //ft2.detectKeyPoints(image2);
      //ft2.calcDescriptor(image2);
      //match.match(ft1, ft2, &matches);

      //std::vector<cv::DMatch> good_matches;
      ////match.getGoodMatches(&good_matches, 0.5);
      //match.getGoodMatches(ft1, ft2, &good_matches, 0.1);

      //// drawing the results
      //cv::Mat img_matches;
      cv::Mat img1 = cv::imread(images_paths[i].c_str());
      cv::Mat img2 = cv::imread(images_paths[j].c_str());
      //cv::drawMatches(img1, ft1.getKeyPoints(), img2, ft2.getKeyPoints(), good_matches, img_matches, Scalar::all(-1), Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

      int nft = feature1.detectKeyPoints(image1);
      feature1.calcDescriptor(image1);
      nft = feature2.detectKeyPoints(image2);
      feature2.calcDescriptor(image2);
      
      //matcherA.add(std::vector<cv::Mat>(1, feature2.getDescriptors()));
      std::vector <std::vector<cv::DMatch>> matchesA;
      matcherA.knnMatch(feature1.getDescriptors(), feature2.getDescriptors(), matchesA, 2);

      std::vector<cv::DMatch> goodMatchesA;
      int max_track_number = 0;
      for (size_t iMatch = 0; iMatch < matchesA.size(); ++iMatch) {
        float distance0 = matchesA[iMatch][0].distance;
        float distance1 = matchesA[iMatch][1].distance;
        if (distance0 < 0.8 * distance1) {
          goodMatchesA.push_back(matchesA[iMatch][0]);
        }
      }

      cv::Mat img_matchesA;
      cv::drawMatches(img1, feature1.getKeyPoints(), img2, feature2.getKeyPoints(), goodMatchesA, img_matchesA, Scalar::all(-1), Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);


      getFileName(images_paths[i].c_str(), name1);
      getFileName(images_paths[j].c_str(), name2);

      sprintf_s(buf, 500, "%i matches seleccionados entre %s y %s", goodMatchesA.size(), name1, name2);

      //progress_bar.init(0, static_cast<double>(good_matches.size()), buf);

      std::vector<Point2f> pts1;
      std::vector<Point2f> pts2;
      int idx1, idx2;
      for (size_t igm = 0; igm < goodMatchesA.size(); igm++) {
        idx1 = goodMatchesA[igm].queryIdx;
        idx2 = goodMatchesA[igm].trainIdx;

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
          cv::Point2f point1 = feature1.getKeyPoint(idx1).pt;
          cv::Point2f point2 = feature2.getKeyPoint(idx2).pt;
          v_points[i] = Vec2d(point1.x, point1.y);
          v_points[j] = Vec2d(point2.x, point2.y);
          pass_points.push_back(v_points);
        }
        //progress_bar();
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
  
  for (int i = 0; i < size; ++i) {
    Mat_<double> frame(2, pass_points.size());
    for (int j = 0; j < pass_points.size(); ++j)
    {
      frame(0,j) = pass_points[j][i][0];
      frame(1,j) = pass_points[j][i][1];
    }
    points2d.push_back(Mat(frame));
  }
  //int nPoints = static_cast<int>(pass_points.size());
  //for (int i = 0; i < nPoints; ++i) {
  //  Mat_<double> frame(2, size);
  //  for (int j = 0; j < size; ++j)
  //  {
  //    frame(0,j) = pass_points[i][j][0];
  //    frame(1,j) = pass_points[i][j][1];
  //  }
  //  points2d.push_back(Mat(frame));
  //}
}


int main(int argc, char** argv)
{

  CmdParser cmdParser("cloud_points", "Calculo de nubes de puntos a partir de imagenes de video");
  cmdParser.addParameter("video", "Video en el que vamos a buscar las torres");
  cmdParser.addParameter("out_path", "Directorio de salida");
  cmdParser.parse(argc, argv);
  if (cmdParser.parse(argc, argv) == CmdParser::MSG::PARSE_ERROR ) {
    cmdParser.printHelp(); // Esto habria que hacerlo directamente en cmdParser.parse y mostrar un mensaje de error por consola
    return 0;
  }
  std::string video = cmdParser.getValue<std::string>("video");
  std::string out_path = cmdParser.getValue<std::string>("out_path");

  // Barra de progreso
  ProgressBar progress_bar;

  //Configuración de log y mensajes por consola
  char logfile[I3D_MAX_PATH];
  if (changeFileExtension(getRunfile(), "log", logfile) == 0) {
    Message::setMessageLogFile(logfile);
    Message::setMessageLevel(MessageLevel::MSG_INFO);
  }

  // Lectura de video
  VideoStream strmVideo(video.c_str());
  if (!strmVideo.isOpened()) {
    printInfo("No se ha podido cargar el video: %s", video.c_str());
    return 0;
  }

  strmVideo.setSkipFrames(3);

  //VideoWindow vc("Cloud Points", WINDOW_AUTOSIZE);
  //vc.setVideo(&strmVideo);

  // Se crea el detector
  double angle = 0;
  double tol = 0.3;
  LD_TYPE ls = LD_TYPE::HOUGHP;
  std::unique_ptr<LineDetector> oLD;
  cv::Scalar ang_tol(angle, tol);
  if (ls == LD_TYPE::HOUGH)            oLD = std::make_unique<ldHouh>(150, ang_tol);
  else if (ls == LD_TYPE::HOUGHP)      oLD = std::make_unique<ldHouhP>(75, ang_tol, 50., 30.);
  else if (ls == LD_TYPE::HOUGH_FAST)  oLD = std::make_unique<ldHouhFast>();
  else if (ls == LD_TYPE::LSD)         oLD = std::make_unique<ldLSD>(ang_tol);
  else {
    printError("No se ha seleccionado ningún detector de lineas.");
    return 0;
  }

  // Detección de apoyos
  DetectTransmissionTower detectTower(oLD.get());
  VideoHelper videoHelper(&detectTower);
  strmVideo.addListener(&videoHelper);
  videoHelper.outPath = out_path;

  //strmVideo.run(); // Lo comento para que no busque por ahora la zona de las torres

  // Se cargan datos de calibración de la cámara
  //bool bCalibratedCamera = true;
  //if (bCalibratedCamera) {
  //  // Si hay datos de calibración de la camara se corrige de distorsión
  //}
  Size imageSize;
  Mat cameraMatrix, distCoeffs;
  std::string file = "D://Esteban//Ingenio3000//Imagenes_Para_Calibracion_GoPro//video_1280x720//out_camera_data.xml";
  //std::string file = "D://Desarrollo//datos//TORRE_3D//calib.xml";

  LoadCameraParams(file, imageSize, cameraMatrix, distCoeffs);
  // Supongo que ya tengo los grupos de imagenes correspondientes a cada torre.

  std::vector<Mat> Rs_est, ts_est, points3d_estimated;
  Matx33d K = cameraMatrix;
  //float f  = 1174.86, cx = 625.55, cy = 344.6;

  //Matx33d K = Matx33d( f, 0, cx,
  //                     0, f, cy,
  //                     0, 0,  1);

  std::vector<std::string> images_paths;
  std::vector<WindowI> windows;
  std::string file2 = out_path + "//TowerDetected.txt";
  getTowerImageAndWindow(file2.c_str(), images_paths, windows );
  //getdir( std::string("D:/Desarrollo/datos/TORRE_3D/pruebas/frames/frames.txt"), images_paths );
  //getdir( std::string("C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\dataset_files.txt"), images_paths );

  //std::vector<std::string> images_paths = videoHelper.framesSaved;

  //LoadImages(std::string("D://Desarrollo//datos//cloud_points//Villaseca//TowerDetect01.xml"), images_paths, windows);
  //std::vector<Mat> points2d;
  cv::Ptr<cv::FeatureDetector> fdORB = cv::ORB::create(10000);
  cv::Ptr<cv::DescriptorExtractor> deDAISY = cv::xfeatures2d::DAISY::create();
  I3D::EXPERIMENTAL::Reconstruction3D reconstruct(fdORB, deDAISY);
  // Se obtienen los keypoints y descriptores
  //reconstruct.getKeyPointAndDescriptor(images_paths, true);
  // Los cargo para probar aunque no lo necesitaria
  reconstruct.loadKeyPointAndDescriptor(images_paths);
  // Se calcula el matching de todas las imagenes
  std::vector<cv::Mat> points2d;
  reconstruct.multiImageMatching(points2d);
  try {
    //reconstruct(images_paths, points2d);
    cv::sfm::reconstruct(points2d, Rs_est, ts_est, K, points3d_estimated,true);
    //bool is_projective = true;
    //cv::sfm::reconstruct(images_paths, Rs_est, ts_est, K, points3d_estimated, is_projective);
  } catch (cv::Exception &e) {
    printError(e.what());
  } catch (std::exception &e) {
    printError(e.what());
  }

  std::vector<Vec3f> point_cloud_est;
  for (int i = 0; i < points3d_estimated.size(); ++i)
    point_cloud_est.push_back(Vec3f(points3d_estimated[i]));


  /* ---------------------------------------------------------------------------------- */


  // guardamos las orientaciones y desplazamiento de las camaras.
  std::vector<Affine3d> path_est;
  std::remove("D:\\Desarrollo\\datos\\TORRE_3D\\pruebas\\frames\\camaras.txt");
  std::ofstream hPos("D:\\Desarrollo\\datos\\TORRE_3D\\pruebas\\frames\\camaras.txt",std::ofstream::app);
  for (size_t i = 0; i < Rs_est.size(); ++i) {
    
    if (hPos.is_open()) {
      hPos << "Camara " << i << ": " << "\n";
      hPos << "R=[";
      cv::Mat_<float> mAux = Rs_est[i];
      for (int r = 0; r < mAux.rows; r++) {
        for (int c = 0; c < mAux.cols; c++) {
          if (!(r == 0 && c == 0))
            hPos << ";";
          //cv::Mat_<float> mAux = Rs_est[i];
          float aux = mAux[r][c];
          hPos << aux;
        }
      }
      hPos << "]\n";
      hPos << "T=[";
      mAux = ts_est[i];
      for (int r = 0; r < mAux.rows; r++) {
        if (r != 0 )
          hPos << ";";
        float aux = mAux[r][0];
        hPos << aux;
      }
      hPos << "]\n";
    }
    path_est.push_back(Affine3d(Rs_est[i],ts_est[i]));
  }
  hPos.close();
    

  /* ---------------------------------------------------------------------------------- */

  // Guardamos nube de puntos
  // Crea nube de puntos
  pcl::PointCloud<pcl::PointXYZ>::Ptr point_cloud_ptr(new pcl::PointCloud<pcl::PointXYZ>);
  int nPoints = point_cloud_est.size();
  point_cloud_ptr->points.resize(nPoints);
  for (int i = 0; i < nPoints; i++) {
    point_cloud_ptr->points[i].x = point_cloud_est[i][0];
    point_cloud_ptr->points[i].y = point_cloud_est[i][1];
    point_cloud_ptr->points[i].z = point_cloud_est[i][2];
  }
  point_cloud_ptr->width = nPoints;
  point_cloud_ptr->height = 1;
  std::string pcd_file = "D:\\Desarrollo\\datos\\TORRE_3D\\pruebas\\frames\\test_pcd.pcd";
  pcl::io::savePCDFileASCII(pcd_file, *point_cloud_ptr);

  std::unique_ptr<pcl::visualization::PCLVisualizer> viewer (new pcl::visualization::PCLVisualizer ("3D Viewer"));
    viewer->setBackgroundColor(0, 0, 0);
    pcl::visualization::PointCloudColorHandlerRandom<pcl::PointXYZ> rgb(point_cloud_ptr);
    viewer->addPointCloud<pcl::PointXYZ> (point_cloud_ptr, rgb, "reconstruction");
    viewer->setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, "reconstruction");
    viewer->addCoordinateSystem( 1 );
    viewer->initCameraParameters();

    while (!viewer->wasStopped())
    {
      viewer->spinOnce(10); //100
      boost::this_thread::sleep(boost::posix_time::microseconds(100000));
    }

  /* ---------------------------------------------------------------------------------- */

/*  // Calculo de los mapas de disparidad

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

  cv::Ptr<cv::FeatureDetector> fd = SURF::create();
  cv::Ptr<cv::DescriptorExtractor> de = SURF::create();

  Features2D featuresL(fd, de);
  Features2D featuresR(fd, de);

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

    left_for_matcher  = left.clone();
    right_for_matcher = right.clone();

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


    wls_filter->setLambda(lambda);
    wls_filter->setSigmaColor(sigma);
    filtering_time = (double)getTickCount();
    wls_filter->filter(left_disp, left, filtered_disp, right_disp);
    filtering_time = ((double)getTickCount() - filtering_time) / getTickFrequency();

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

    cv::Mat_<cv::Vec3f> XYZ(filtered_disp.rows,filtered_disp.cols);
    //cv::reprojectImageTo3D(filtered_disp,XYZ,Q);  //... Mejor no usar para poder filtrar la nube de puntos.
    cv::Mat_<double> vec_tmp(4,1);
  
    cv::Mat dispFilteredDif = left_disp - filtered_disp;
    cv::Mat dispMask = cv::Mat::ones(filtered_disp.size(), CV_8U);
    dispMask.setTo(0, abs(dispFilteredDif) > 10);

    cv::Mat disparity;
    cv::bitwise_and(left_disp, left_disp, disparity, dispMask);

    WindowI wL = windows[i-1];
    WindowI wR = windows[i];

    for(int y=0; y < wL.getHeight(); ++y) {
      for(int x=0; x < wL.getWidth(); ++x) {
        vec_tmp(0) = x;
        vec_tmp(1) = y;
        vec_tmp(2) = disparity.at<__int16>(wL.pt1.y + y, wL.pt1.x + x);
        if (vec_tmp(2) == 0) continue;
        vec_tmp(3) = 1;
        vec_tmp = Q*vec_tmp;
        vec_tmp /= vec_tmp(3);
        if (abs(vec_tmp(0))>10 || abs(vec_tmp(1))>10 || abs(vec_tmp(2)) > 10) continue;
        cv::Vec3f &point = XYZ.at<cv::Vec3f>(wL.pt1.y + y, wL.pt1.x + x);
        point[0] = vec_tmp(0);
        point[1] = vec_tmp(1);
        point[2] = vec_tmp(2);
      }
    }

    // Crea nube de puntos
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr point_cloud_ptr(new pcl::PointCloud<pcl::PointXYZRGB>);

    for (int i = 0; i < wL.getHeight(); i++) {
      for (int j = 0; j < wL.getWidth(); j++) {        

        pcl::PointXYZRGB point;
        point.x = XYZ.at<cv::Vec3f>(wL.pt1.y + i, wL.pt1.x + j)[0];
        point.y = XYZ.at<cv::Vec3f>(wL.pt1.y + i, wL.pt1.x + j)[1];
        point.z = XYZ.at<cv::Vec3f>(wL.pt1.y + i, wL.pt1.x + j)[2];
        if (point.z < 1.0 || point.z > 0.5) {
          point.rgb = *left_for_matcher.ptr<float>(wL.pt1.y + i, wL.pt1.x + j);
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

    std::string cp = "D:\\Desarrollo\\datos\\TORRE_3D\\cp";
    cp += std::to_string(i);
    cp += ".pcd";
    pcl::io::savePCDFileASCII(cp, *point_cloud_ptr);

  }
*/

  return 0;
}
