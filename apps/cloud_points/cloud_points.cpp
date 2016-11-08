#include "cloud_points.h"

#include <stdio.h>

#include "opencv2/calib3d.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/optflow.hpp"
#include "opencv2/sfm.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/ximgproc/disparity_filter.hpp"

#include <pcl/common/common_headers.h>
#include <pcl/io/pcd_io.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <boost/thread/thread.hpp>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/point_types.h>
	
	
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
  delLinesGroupBySize(linesGroup,10);
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
  
  //Ventana en la cual se van a buscar los apoyos
  WindowI ws(cv::Point(0, 0), cv::Point(sz.width, sz.height));
  ws = expandWindow(ws, -100, 0);

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
      if (ang <= 0.1 && ang >= -0.1) {
        printVerbose("Frame %i - Angulo: %f", static_cast<int>(prevFrame), ang);

         // Busqueda del máximo valor de desplazamiento
        std::vector<Point> vMagnitudes;
        cv::Point ptMax;
        float maxval = I3D_FLOAT_MIN;
        for (int ir = 0; ir < normMag.rows; ir++) {
          cv::Point pt(cvRound(m * ir + b), ir);
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
      } else printVerbose("Frame %i rechazado por angulo de recta de regresion mayor al limite. angulo=%f", cvRound(prevFrame), ang);
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

public:

  VideoHelper(DetectTransmissionTower *detectTower) 
  {
    mDetectTower = detectTower;
    mCurrentPosition = 0.;
    outPath = "";
    outFile = "TowerDetected.xml";
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
  outFile = outPath + "\\TowerDetected.txt";
  // Para limpiar el fichero se carga sin la opción append
  cv::FileStorage fs(outFile.c_str(), cv::FileStorage::WRITE | cv::FileStorage::FORMAT_XML);
  fs.release();
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
      sprintf_s(buffer, "%s\\Apoyo_%05i.jpg", outPath.c_str(), cvRound(mCurrentPosition));
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
  std::string name = outPath + "\\TowerDetected.xml";
  //cv::FileStorage fs(outFile.c_str(), cv::FileStorage::WRITE | cv::FileStorage::APPEND | cv::FileStorage::FORMAT_XML);
  //if (fs.isOpened()) {
  //  fs << "Towers";
  //  fs << "{";
  //  for (size_t i = 0; i < framesSaved.size(); i++) {

  //  fs << "Tower";
  //  fs << "{" << "Frame" << framesSaved[i];
  //  fs << "Bbox";
  //  fs << "{" << "X1" << windowsSaved[i].pt1.x;
  //  fs << "Y1" << windowsSaved[i].pt1.y;
  //  fs << "X2" << windowsSaved[i].pt2.x;
  //  fs << "y2" << windowsSaved[i].pt2.y << "}";
  //  fs << "}";
  //  }
  //  fs << "}";
  //}
  //fs.release();

  ////... FileStorage no es lo suficientemente flexible. O eso parece
  ////    De momento en fichero de texto.
  //try {
  //  XMLPlatformUtils::Initialize();
  //} catch (const XMLException &e) {
  //  char* message = XMLString::transcode(e.getMessage());
  //  printError(message);
  //  return;
  //}

  //XercesDOMParser *parser = new XercesDOMParser();
  //parser->setValidationScheme(XercesDOMParser::Val_Never);
  //parser->setDoNamespaces(true);    // optional

  //XMLPlatformUtils::Terminate();
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

  strmVideo.setSkipFrames(1);

  //VideoWindow vc("Cloud Points", WINDOW_AUTOSIZE);
  //vc.setVideo(&strmVideo);

  // Se crea el detector
  double angle = 0;
  double tol = 0.25;
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
  //std::string file = "D://Esteban//Ingenio3000//Imagenes_Para_Calibracion_GoPro//video_1280x720//out_camera_data.xml";
  std::string file = "D://Desarrollo//datos//TORRE_3D//calib.xml";

  LoadCameraParams(file, imageSize, cameraMatrix, distCoeffs);
  // Supongo que ya tengo los grupos de imagenes correspondientes a cada torre.

  std::vector<Mat> Rs_est, ts_est, points3d_estimated;
  Matx33d K = cameraMatrix;
  //float f  = 1174.86, cx = 625.55, cy = 344.6;

  //Matx33d K = Matx33d( f, 0, cx,
  //                     0, f, cy,
  //                     0, 0,  1);

  //std::vector<std::string> images_paths{
  //  "D://Desarrollo//datos//cloud_points//Villaseca//Apoyo_00389.jpg",
  //  "D://Desarrollo//datos//cloud_points//Villaseca//Apoyo_00393.jpg",
  //  "D://Desarrollo//datos//cloud_points//Villaseca//Apoyo_00394.jpg",
  //  "D://Desarrollo//datos//cloud_points//Villaseca//Apoyo_00395.jpg",
  //  "D://Desarrollo//datos//cloud_points//Villaseca//Apoyo_00396.jpg",
  //  "D://Desarrollo//datos//cloud_points//Villaseca//Apoyo_00397.jpg",
  //  "D://Desarrollo//datos//cloud_points//Villaseca//Apoyo_00398.jpg",
  //  "D://Desarrollo//datos//cloud_points//Villaseca//Apoyo_00399.jpg"
  //};
  std::vector<std::string> images_paths;
  //getdir( std::string("D:/Desarrollo/datos/TORRE_3D/pruebas/frames/frames.txt"), images_paths );
  getdir( std::string("C:\\Users\\Tido\\Pictures\\Torres_Pasillo_illescas\\dataset_files.txt"), images_paths );

  //std::vector<std::string> images_paths = videoHelper.framesSaved;

  //std::vector<WindowI> windows{
  //  WindowI(cv::Point(767, 47), cv::Point(845, 719)),
  //  WindowI(cv::Point(633, 44), cv::Point(769, 719)),
  //  WindowI(cv::Point(623, 86), cv::Point(757, 718)),
  //  WindowI(cv::Point(604, 72), cv::Point(740, 719)),
  //  WindowI(cv::Point(608, 69), cv::Point(730, 719)),
  //  WindowI(cv::Point(582, 85), cv::Point(724, 719)),
  //  WindowI(cv::Point(572, 58), cv::Point(711, 719)),
  //  WindowI(cv::Point(563, 76), cv::Point(704, 718))
  //};


  std::vector<WindowI> windows = videoHelper.windowsSaved;

  //LoadImages(std::string("D://Desarrollo//datos//cloud_points//Villaseca//TowerDetect01.xml"), images_paths, windows);

  try {
    //cv::sfm::reconstruct(points2d, Rs_est, ts_est, K, points3d_estimated,true);
    bool is_projective = true;
    cv::sfm::reconstruct(images_paths, Rs_est, ts_est, K, points3d_estimated, is_projective);
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

  for (int i = 0; i < point_cloud_est.size(); i++) {
    pcl::PointXYZ point;
    point.x = point_cloud_est[i][0];
    point.y = point_cloud_est[i][1];
    point.z = point_cloud_est[i][2];
    point_cloud_ptr->points.push_back(point);
  }
  point_cloud_ptr->width = (int)point_cloud_ptr->points.size();
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
