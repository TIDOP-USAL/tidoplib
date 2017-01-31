#include <windows.h>
#include <memory>

// Cabeceras de OpenCV
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/calib3d.hpp"

// Cabeceras propias
#include "feature_detection/linedetector.h"
#include "VideoStream.h"
#include "core/utils.h"
#include "matching.h"
#include "fourier.h"
#include "img_process/img_processing.h"
#include "img_process/filters.h"
#include "transform.h"
#include "core/console.h"
#include "core/messages.h"

using namespace I3D;
using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

const std::string keys =
"{@imput          |       | imagen o video de entrada }"
"{si saveimage    | true  | Guarda imagenes }"
"{dl  DrawLines   | false | Dibuja lineas  }"
"{drl  DrawRegLine | false | Dibuja recta de regresion  }"
;

VideoStream::Skip sf = VideoStream::Skip::NOT_SKIP;
int SkpFrames = 2;
int SkpSeconds = -1;


// De momento emulo el video
//const char *imgpath = "D:\\Esteban\\Ingenio3000\\Deteccion_Conductores_2016-04-21\\Frames_video_termico_MAT\\";
//const char *imgpath = "D:\\Esteban\\recortes_termico\\";

//const char *imgpath = "D:\\Desarrollo\\datos\\Frames_video_termico_tif\\";
//const char *imgpath = "D:\\Desarrollo\\datos\\recortes_termico\\";
//static const char *img[] = { "NEW00011200.tif",
//                             "NEW00011201.tif",
//                             "NEW00011202.tif",
//                             "NEW00011203.tif",
//                             "NEW00011204.tif",
//                             "NEW00011205.tif",
//                             "NEW00011206.tif",
//                             "NEW00011207.tif",
//                             "NEW00011208.tif",
//                             "NEW00011209.tif",
//                             "NEW00011210.tif",
//                             "NEW00011211.tif",
//                             "NEW00011212.tif",
//                             "NEW00011213.tif",
//                             "NEW00011214.tif"};

//static const char *img[] = {  "Captura1.png",
//                              "Captura2.png",
//                              "Captura3.png",
//                              "Captura4.png",
//                              "Captura5.png",
//                              "Captura6.png",
//                              "Captura7.png",
//                              "Captura8.png",
//                              "Captura9.png",
//                              "Captura10.png" };


//Prueba con detectores de lineas. El método de fourier no parece que vaya a ser muy robusto
LD_TYPE ls = LD_TYPE::HOUGHP;

int main(int argc, char *argv[])
{

  int ipto = 1; //Punto que queremos seguir
  const char *imgpath = "C:\\Desarrollo\\datos\\capturas_deteccion_conductores\\";

  static const char *img[] = {
    /*"captura001.jpg",
    "captura002.jpg",
    "captura003.jpg",
    "captura004.jpg",
    "captura005.jpg",
    "captura006.jpg",
    "captura007.jpg",
    "captura009.jpg",
    "captura010.jpg",
    "captura012.jpg",
    "captura013.jpg",
    "captura014.jpg",
    "captura015.jpg",
    "capturas016.jpg",
    "capturas018.jpg",
    "capturas019.jpg",
    "capturas021.jpg",
    "capturas023.jpg",
    "capturas024.jpg",
    "capturas025.jpg",
    "capturas026.jpg",
    "capturas027.jpg",
    "capturas028.jpg",
    "capturas029.jpg",
    "capturas030.jpg",
    "capturas031.jpg",
    "capturas032.jpg",
    "capturas033.jpg",
    "capturas034.jpg",
    "capturas035.jpg",
    "capturas036.jpg",
    "capturas037.jpg",*/
    "capturas051.jpg",
    "capturas052.jpg",
    "capturas053.jpg",
    "capturas054.jpg",
    "capturas055.jpg",
    "capturas056.jpg",
    "capturas057.jpg",
    "capturas058.jpg",
    "capturas059.jpg",
    "capturas060.jpg",
    "capturas061.jpg",
    "capturas062.jpg",
    "capturas063.jpg",
    "capturas064.jpg",
    "capturas065.jpg",
    "capturas066.jpg",
    "capturas067.jpg",
    "capturas068.jpg",
    "capturas069.jpg",
    "capturas070.jpg",
    "capturas071.jpg",
    "capturas072.jpg",
    "capturas073.jpg",
    "capturas074.jpg",
    "capturas075.jpg",
    "capturas076.jpg",
    "capturas077.jpg",
    "capturas078.jpg",
    "capturas079.jpg",
    "capturas080.jpg",
    "capturas081.jpg",
    "capturas082.jpg",
    "capturas083.jpg",
    "capturas084.jpg",
    "capturas085.jpg",
    "capturas086.jpg",
    "capturas087.jpg",
    "capturas088.jpg",
    "capturas089.jpg",
    "capturas090.jpg",
    "capturas091.jpg",
    "capturas092.jpg",
    "capturas093.jpg",
    "capturas094.jpg",
    "capturas095.jpg",
    "capturas096.jpg",
    "capturas097.jpg",
    "capturas098.jpg",
    "capturas099.jpg",
    "capturas100.jpg",
    "capturas101.jpg",
    "capturas102.jpg",
    "capturas103.jpg",
    "capturas104.jpg",
    "capturas105.jpg",
    "capturas106.jpg",
    "capturas107.jpg",
    "capturas108.jpg",
    "capturas109.jpg",
    "capturas110.jpg",
    "capturas111.jpg",
    "capturas112.jpg",
    "capturas113.jpg",
    "capturas114.jpg",
    "capturas115.jpg",
    "capturas116.jpg",
    "capturas117.jpg",
    "capturas118.jpg",
    "capturas119.jpg",
    "capturas120.jpg",
    "capturas121.jpg",
    "capturas122.jpg",
    "capturas123.jpg",
    "capturas124.jpg",
    "capturas125.jpg",
    "capturas126.jpg",
    "capturas127.jpg",
    "capturas128.jpg",
    "capturas129.jpg",
    "capturas130.jpg",
    "capturas131.jpg",
    "capturas132.jpg",
    "capturas133.jpg",
    "capturas134.jpg",
    "capturas135.jpg",
    "capturas136.jpg",
    "capturas137.jpg",
    "capturas138.jpg",
    "capturas139.jpg",
    "capturas140.jpg",
    "capturas141.jpg",
    "capturas142.jpg",
    "capturas143.jpg",
    "capturas144.jpg",
    "capturas145.jpg",
    "capturas146.jpg",
    "capturas147.jpg",
    "capturas148.jpg",
    "capturas149.jpg",
    "capturas150.jpg",
    "capturas151.jpg",
    "capturas152.jpg",
    "capturas153.jpg",
    "capturas154.jpg",
    "capturas155.jpg",
    "capturas156.jpg",
    "capturas157.jpg",
    "capturas158.jpg",
    "capturas159.jpg",
    "capturas160.jpg",
    "capturas161.jpg",
    "capturas162.jpg",
    "capturas163.jpg",
    "capturas164.jpg",
    "capturas165.jpg",
    "capturas166.jpg",
    "capturas167.jpg",
    "capturas168.jpg",
    "capturas169.jpg",
    "capturas170.jpg",
    "capturas171.jpg",
    "capturas172.jpg",
    "capturas173.jpg",
    "capturas174.jpg",
    "capturas175.jpg",
    "capturas176.jpg",
    "capturas177.jpg",
    "capturas178.jpg",
    "capturas179.jpg",
    "capturas180.jpg",
    "capturas181.jpg",
    "capturas182.jpg",
    "capturas183.jpg",
    "capturas184.jpg",
    "capturas185.jpg",
    "capturas186.jpg",
    "capturas187.jpg",
    "capturas188.jpg",
    "capturas189.jpg",
    "capturas190.jpg",
    "capturas191.jpg",
    "capturas192.jpg",
    "capturas193.jpg",
    "capturas194.jpg",
    "capturas195.jpg",
    "capturas196.jpg",
    "capturas197.jpg",
    "capturas198.jpg",
    "capturas199.jpg",
    "capturas200.jpg" };

  std::vector<PointI> ptsPrev;

  ptsPrev.push_back(PointI(320, 132)); // Punto 1. OK
  ptsPrev.push_back(PointI(320, 161)); // Punto 2. OK
  ptsPrev.push_back(PointI(320, 206)); // Punto 3. OK
  ptsPrev.push_back(PointI(320, 242)); // Punto 4. OK
  ptsPrev.push_back(PointI(320, 288)); // Punto 5. OK
  ptsPrev.push_back(PointI(320, 314)); // Punto 6. OK

  cv::Mat current_frame;              // Ventana de video actual 
  cv::Mat crop_frame;                 // Ventana recortada del frame actual                  
  cv::Point ptPrev, ptCur;
  WindowI wPrev, wCur;
  cv::Mat descriptorPrev;
  cv::Mat descriptorCur;

  Line conductor_line_prev;           // Linea que representa el conductor en la imagen previa         
  double conductor_direction = 0.0;   // Dirección del conductor 

  char logfile[I3D_MAX_PATH];
  int err = changeFileNameAndExtension(getRunfile(), "TrackLines.log", logfile, I3D_MAX_PATH);
  if (err == 0) {
        //LogMsg log(logfile, LogLevel::LOG_DEBUG);
    Message::setMessageLogFile(logfile);
    Message::setMessageLevel(MessageLevel::MSG_DEBUG);
  }

  std::unique_ptr<LineDetector> oLD;
  cv::Scalar ang_tol(CV_PI / 2, 0.25);
  if (ls == LD_TYPE::HOUGH)            oLD = std::make_unique<ldHouh>(150, ang_tol);
  else if (ls == LD_TYPE::HOUGHP)      oLD = std::make_unique<ldHouhP>(75, ang_tol, 50., 30.);
  else if (ls == LD_TYPE::HOUGH_FAST)  oLD = std::make_unique<ldHouhFast>();
  else if (ls == LD_TYPE::LSD)         oLD = std::make_unique<ldLSD>(ang_tol);
  else {
    logPrintError("No se ha seleccionado ningún detector de lineas.");
    return 0;
  }

  //Procesos que se aplican a las imagenes
  I3D::ImgProcessingList imgprolist;
  imgprolist.add(std::make_shared<I3D::Normalize>(0., 255.));
  //imgprolist.add(std::make_shared<I3D::GaussianBlur>(cv::Size(3, 3), 1., 1.));
  imgprolist.add(std::make_shared<I3D::BilateralFilter>(5, 50., 50.));
  imgprolist.add(std::make_shared<I3D::Erotion>(1));
  imgprolist.add(std::make_shared<I3D::Dilate>(1));
  //imgprolist.add(std::make_shared<I3D::Binarize>(0., 0., true));
  if (ls == LD_TYPE::LSD) {
    imgprolist.add(std::make_shared<I3D::Sobel>(1, 0, 3, 1., 0.));
  } else {
    imgprolist.add(std::make_shared<I3D::Canny>());
  }
  

 
  //cv::AgastFeatureDetector::create()
  //cv::AKAZE::create();
  //cv::BRISK::create();
  //cv::FastFeatureDetector::create();
  //cv::GFTTDetector::create();
  //cv::KAZE::create();
  //cv::MSER::create();
  //cv::ORB::create();
  //cv::SimpleBlobDetector::create();
  //cv::xfeatures2d::BriefDescriptorExtractor::create();
  //cv::xfeatures2d::DAISY::create();
  //cv::xfeatures2d::FREAK::create();
  //cv::xfeatures2d::LATCH::create();
  //cv::xfeatures2d::LUCID::create();
  //cv::xfeatures2d::MSDDetector::create();
  //cv::xfeatures2d::SIFT::create();
  //cv::xfeatures2d::StarDetector::create();
  //cv::xfeatures2d::SURF::create();

  // Se crea el detector de features
  cv::Ptr<cv::FeatureDetector> fd = SURF::create();

  //cv::BRISK::create();
  //cv::ORB::create();
  //cv::AKAZE::create();
  //cv::xfeatures2d::SIFT::create();
  //cv::xfeatures2d::SURF::create();
  // Se crea el extractor de descriptores
  cv::Ptr<cv::DescriptorExtractor> de = SURF::create();
  Features2D featuresPrev(fd, de);
  Features2D featuresCur(fd, de);

  // Se crea el matcher de descriptores
  
  //- `BruteForce` (it uses L2)
  //- `BruteForce-L1`
  //- `BruteForce-Hamming`
  //- `BruteForce-Hamming(2)`
  //- `FlannBased`
  Matching match(cv::DescriptorMatcher::create("FlannBased"));

  // Línea que vamos a seguir
  //int iTrack = 0;
  // Columnas en las que vamos a determinar la posición de los conductores mediante la transformada de fourier.
  std::vector<int> col_fourier;

  //Listado con los puntos obtenidos por la transformada de fourier
  std::vector<std::vector<cv::Point>> pts_fourier;
  bool bEstimate = false;
  for (int i = 0; i < sizeof(img) / sizeof(img[0]); i++) {
    //... Simulación de la carga del video
    std::string img1 = std::string(imgpath) + img[i];
    logPrintInfo("Frame: %s", img1.c_str());
    current_frame = cv::imread(img1, cv::IMREAD_GRAYSCALE);
    if (current_frame.empty()) {
      logPrintError("No se puede cargar frame: %s", img1.c_str());
    } else {
      ////... Prueba a reducir la resolución
      //cv::Size szImg(current_frame.size().width / 2, current_frame.size().height / 2);
      //cv::resize(current_frame, current_frame, szImg);
      ////...

      cv::Mat out;
      if (wPrev.isEmpty()) {
        // Se determina el punto en la primera foto

        /////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Partimos de un punto obtenido de la detección de los aisladores
        //... Suponemos la posición conocida

		  PointI ptPrev = ptsPrev[ipto];
		  //ptPrev = cv::Point(320, 144); // Punto 1. OK
		  //ptPrev = cv::Point(320, 171); // Punto 2. OK
		  //ptPrev = cv::Point(320, 217); // Punto 3. OK
		  //ptPrev = cv::Point(320, 243); // Punto 4. OK
		  //ptPrev = cv::Point(320, 288); // Punto 5. OK
		  //ptPrev = cv::Point(320, 314); // Punto 6. OK

        //ptPrev = cv::Point(320, 60); // Punto 1. OK
        //ptPrev = cv::Point(320, 91); // Punto 2
        //ptPrev = cv::Point(320, 136); // Punto 3
        //ptPrev = cv::Point(320, 149); // Punto 4
        //ptPrev = cv::Point(320, 195); // Punto 5
        //ptPrev = cv::Point(320, 221); // Punto 6

        //log.InfoMsg("Punto inicial (%i, %i)", 385, 136);
        //ptPrev = cv::Point(385, 136); // Punto 1. 
        //ptPrev = cv::Point(380, 172); // Punto 2
        //ptPrev = cv::Point(378, 218); // Punto 3
        //ptPrev = cv::Point(412, 245); // Punto 4
        //ptPrev = cv::Point(400, 300); // Punto 5
        //ptPrev = cv::Point(397, 331); // Punto 6
        
        //ptPrev = cv::Point(320, 132); // Punto 1. OK
        //ptPrev = cv::Point(320, 161); // Punto 2. OK
        //ptPrev = cv::Point(320, 206); // Punto 3. OK
        //ptPrev = cv::Point(320, 242); // Punto 4. OK
        //ptPrev = cv::Point(320, 288); // Punto 5. OK
        //ptPrev = cv::Point(320, 314); // Punto 6. OK

        /////////////////////////////////////////////////////////////////////////////////////////////////////////

        // Obtenemos la ventana envolvente del punto que queremos seguir
        wPrev = WindowI(ptPrev, 200);

        // Nos aseguramos que la ventana no se sale de los límites.
        wPrev = windowIntersection(wPrev, WindowI(cv::Point(0, 0), cv::Point(current_frame.cols, current_frame.rows)));
        // Cortamos el frame actual quedandonos con la ventana del punto
        current_frame.colRange(wPrev.pt1.x, wPrev.pt2.x).rowRange(wPrev.pt1.y, wPrev.pt2.y).copyTo(crop_frame);

        // Se detectan los key points y los descriptores
        int nft = featuresPrev.detectKeyPoints(crop_frame);
        featuresPrev.calcDescriptor(crop_frame);
        logPrintInfo("Número de features detectado: %i", nft);

        cv::Mat mOut;
        // Procesado previo a la imagen para la detección de lineas
        if (imgprolist.execute(crop_frame, &mOut) == ProcessExit::FAILURE) exit(EXIT_FAILURE);
        if (oLD->run(mOut) == LineDetector::Exit::FAILURE) exit(EXIT_FAILURE);
        oLD->drawLines(crop_frame);
        logPrintInfo("Número de lineas detectado: %i", oLD->getLines().size());
        if (!oLD->getLines().empty()) {
          vector<Line> lines;
          Translate<PointI> trf(wPrev.pt1.x, wPrev.pt1.y);
          //trf.transform(oLD->getLines(), &lines);
          transform(oLD->getLines(), &lines, &trf, transform_order::DIRECT);

          cvtColor(current_frame, out, CV_GRAY2BGR);
          for (size_t i = 0; i < lines.size(); i++) {
            cv::line(out, lines[i].pt1, lines[i].pt2, cv::Scalar(0, 255, 255), 1);
          }

          //Busqueda de la recta mas próxima.
          double minDist = I3D_DOUBLE_MAX;
          double dist = 0.0;
          size_t iminl = 0;
          for (size_t i = 0; i < lines.size(); i++) {
            dist = distPointToSegment(ptPrev, lines[i]);
            if (dist < minDist) {
              minDist = dist;
              iminl = i;
            }
          }

          conductor_line_prev = oLD->getLines()[iminl];
          conductor_direction = conductor_line_prev.angleOX();

        }
        logPrintInfo("Pendiente recta: %f", conductor_direction);

      } else {

        // Se determinan puntos y descriptores en la imagen actual
        //... No calcular con toda la ventana. Calcular una ventana estimada
        int nft = featuresCur.detectKeyPoints(current_frame);
        featuresCur.calcDescriptor(current_frame);
        logPrintInfo("Número de features detectado: %i", nft);

        // Cálculo de matching
        std::vector<DMatch> matches;
        match.match(featuresPrev, featuresCur, &matches);

        //BFMatcher matcher(NORM_HAMMING);
        //std::vector<std::vector<DMatch>> matches;
        //cv::Mat desc1 = featuresPrev.getDescriptors();
        //cv::Mat desc2 = featuresCur.getDescriptors();
        //matcher.knnMatch(desc1, desc2, matches, 2);

        //vector<KeyPoint> matched1, matched2, inliers1, inliers2;
        //vector<DMatch> good_matches;
        //for (size_t i = 0; i < matches.size(); i++) {
        //  DMatch first = matches[i][0];
        //  float dist1 = matches[i][0].distance;
        //  float dist2 = matches[i][1].distance;

        //  if (dist1 < 0.3 * dist2) {
        //    cv::Point2f pfmatch1 = featuresPrev.getKeyPoint(first.queryIdx);
        //    cv::Point2f pfmatch2 = featuresPrev.getKeyPoint(first.trainIdx);
        //    matched1.push_back(pfmatch1);
        //    matched2.push_back(pfmatch2);
        //  }
        //}
        //////////////////////

        // Nos quedamos con los mejores puntos
        std::vector<DMatch> good_matches;
        match.getGoodMatches(&good_matches,0.04);

        std::vector<PointF> ptsprev;
        std::vector<PointF> ptscur;
        for (size_t igm = 0; igm < good_matches.size(); igm++) {
          ptsprev.push_back(featuresPrev.getKeyPoint(good_matches[igm].queryIdx).pt);
          ptscur.push_back(featuresCur.getKeyPoint(good_matches[igm].trainIdx).pt);
        }
        logPrintInfo("Número de matches: %i", good_matches.size());

        // drawing the results
        //cv::Mat img_matches;
        //cv::drawMatches(crop_frame, featuresPrev.getKeyPoints(), current_frame, featuresCur.getKeyPoints(), good_matches, img_matches, Scalar::all(-1), Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);


        // Calculo de la transformación proyectiva
        TrfPerspective<PointF> trfPerps;
        if (trfPerps.compute(ptsprev, ptscur)) {
          // Se transforma la línea del conductor para determinar la posición en la imagen actual
          std::vector<PointF> l_in = { conductor_line_prev.pt1, conductor_line_prev.pt2 };
          std::vector<PointF> l_out;
          trfPerps.transform(l_in, &l_out);
          Line line_proj(l_out[0], l_out[1]);

          cvtColor(current_frame, out, CV_GRAY2BGR);

          // Linea en la imagen actual 
          line(out, line_proj.pt1, line_proj.pt2, Scalar(0, 255, 0), 1, LINE_AA);

          // Linea vertical de centro de pantalla
          line(out, cv::Point(current_frame.cols / 2, 0), cv::Point(current_frame.cols / 2, current_frame.rows), Scalar(0, 255, 0), 1, LINE_AA);

          // Intersección de línea de centro con línea proyectada para determinar el punto del conductor. Este punto se considera una primera aproximación
          PointI pt_intersect;
          intersectLines(line_proj, Line(cv::Point(current_frame.cols / 2, 0), cv::Point(current_frame.cols / 2, current_frame.rows)), &pt_intersect);
          cv::line(out, pt_intersect, pt_intersect, Scalar(255, 0, 0), 3, LINE_AA);

          // Repetido...

          // Obtenemos la ventana envolvente del punto
          wCur = WindowI(pt_intersect, 200);
          wCur = windowIntersection(wCur, WindowI(cv::Point(0, 0), cv::Point(current_frame.cols, current_frame.rows)));
          // Aplicamos un filtrado de features por ventana para eliminar las que esten fuera
          featuresCur.filter(current_frame, wCur, &crop_frame, NULL, NULL);

          cv::Mat mOut;
          // Procesado previo a la imagen para la detección de lineas
          imgprolist.execute(crop_frame, &mOut);
          
          if (oLD->run(mOut, cv::Scalar(line_proj.angleOY(), 0.25)) == LineDetector::Exit::FAILURE) exit(EXIT_FAILURE);

          logPrintInfo("Número de lineas detectado: %i", oLD->getLines().size());
          if (!oLD->getLines().empty()) {

            //for (size_t i = 0; i < oLD.lines.size(); i++) {
            //  cv::line(out, oLD.lines[i].pt1, oLD.lines[i].pt2, cv::Scalar(0, 255, 255), 1);
            //}
            //Busqueda de la recta mas próxima.
            double minDist = I3D_DOUBLE_MAX;
            double dist = 0.0;
            size_t iminl = 0;
            for (size_t i = 0; i < oLD->getLines().size(); i++) {
              dist = distPointToSegment(pt_intersect - wCur.pt1, oLD->getLines()[i]);
              if (dist < minDist) {
                minDist = dist;
                iminl = i;
              }
            }
            // Si es mayor no encuentra línea. Mejor mantener el anterior
            if (minDist < 5.) {
              conductor_line_prev = oLD->getLines()[iminl];
              conductor_direction = conductor_line_prev.angleOX();
              intersectLines(line_proj, Line(cv::Point(current_frame.cols / 2, 0), cv::Point(current_frame.cols / 2, current_frame.rows)), &pt_intersect);
              cv::line(out, pt_intersect, pt_intersect, Scalar(0, 0, 255), 3, LINE_AA);
              logPrintInfo("Punto conductor (%i, %i)", pt_intersect.x, pt_intersect.y);
              bEstimate = false;
            } else {
              bEstimate = true;
              logPrintInfo("Ningún conductor detectado próximo al punto. Posición estimada (%i, %i)", pt_intersect.x, pt_intersect.y);
            }
          } else {
            logPrintInfo("Conductores no encontrados. Punto determinado a partir de la proyección de la línea previa (%i, %i)", pt_intersect.x, pt_intersect.y);
          }
          logPrintInfo("Pendiente recta: %f", conductor_direction);


          cv::namedWindow("TrackLines", WINDOW_AUTOSIZE);
          cv::imshow("TrackLines", out);
          cv::waitKey(1);

          char buffer[50];
          sprintf_s(buffer, "TrackLines_%i_%02i.jpg", ipto, i);
          cv::imwrite(buffer, out);

          // Asignamos valores previos para la siguiente iteración
          ptPrev = ptCur;
          wPrev = wCur;
          featuresPrev = featuresCur;
        }  
      }
    }
  }

  return 0;
}






//int main(int argc, char *argv[])
//{
//  cv::Mat current_frame;              // Ventana de video actual 
//  cv::Mat crop_frame;                 // Ventana recortada del frame actual                  
//  cv::Point ptPrev, ptCur;
//  WindowI wPrev, wCur;
//  cv::Mat descriptorPrev;
//  cv::Mat descriptorCur;
//  
//  Line conductor_line_prev;           // Linea que representa el conductor en la imagen previa         
//  double conductor_direction = 0.0;         // Dirección del conductor 
//
//
//  //BRISK::create();
//  //ORB::create();
//  //MSER::create();
//  //FastFeatureDetector::create();
//  //GFTTDetector::create();
//  //SimpleBlobDetector::create();
//  //KAZE::create();
//  //AKAZE::create();
//  //SIFT::create();
//  //SURF::create();
//
//  // Se crea el detector de features
//  cv::Ptr<cv::FeatureDetector> fd = SURF::create();
//
//  //BRISK::create();
//  //ORB::create();
//  //AKAZE::create();
//  //SIFT::create();
//  //SURF::create();
//
//  // Se crea el extractor de descriptores
//  cv::Ptr<cv::DescriptorExtractor> de = SURF::create();
//  Features2D featuresPrev(fd, de);
//  Features2D featuresCur(fd, de);
//
//  //"BruteForce"
//  //"BruteForce - L1"
//  //"BruteForce - Hamming"
//  //"BruteForce - Hamming(2)"
//  //"FlannBased"
//  // Se crea el matcher de descriptores
//  Matching match(DescriptorMatcher::create("FlannBased"));
//
//  // Línea que vamos a seguir
//  int iTrack = 0;
//  // Columnas en las que vamos a determinar la posición de los conductores mediante la transformada de fourier.
//  std::vector<int> col_fourier;
//
//  //Listado con los puntos obtenidos por la transformada de fourier
//  std::vector<std::vector<cv::Point>> pts_fourier;
//
//  for (int i = 0; i < sizeof(img) / sizeof(img[0]); i++) {
//    //... Simulación de la carga del video
//    std::string img1 = std::string(imgpath) + img[i];
//    current_frame = cv::imread(img1, cv::IMREAD_GRAYSCALE);
//    //...
//
//
//    if (wPrev.isEmpty()) {
//      // Se determina el punto en la primera foto
//
//      /////////////////////////////////////////////////////////////////////////////////////////////////////////
//      // Partimos de un punto obtenido de la detección de los aisladores
//      //... Suponemos la posición conocida
//      //Deteccion_Conductores_2016 - 04 - 21
//      ptPrev = cv::Point(320, 144); // Punto 1. OK
//      //ptPrev = cv::Point(320, 171); // Punto 2
//      //ptPrev = cv::Point(320, 217); // Punto 3
//      //ptPrev = cv::Point(320, 243); // Punto 4
//      //ptPrev = cv::Point(320, 288); // Punto 5       //Llega un punto que no hace bien el matching
//      //ptPrev = cv::Point(320, 314); // Punto 6
//
//      //ptPrev = cv::Point(320, 60); // Punto 1
//      //ptPrev = cv::Point(320, 91); // Punto 2
//      //ptPrev = cv::Point(320, 136); // Punto 3
//      //ptPrev = cv::Point(320, 149); // Punto 4
//      //ptPrev = cv::Point(320, 195); // Punto 5
//      //ptPrev = cv::Point(320, 221); // Punto 6
//      /////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//      // Obtenemos la ventana envolvente del punto que queremos seguir
//      wPrev = WindowI(ptPrev, 200);
//
//      // Nos aseguramos que la ventana no se sale de los límites.
//      wPrev = windowIntersection(wPrev, WindowI(cv::Point(0, 0), cv::Point(current_frame.cols, current_frame.rows)));
//      // Cortamos el frame actual quedandonos con la ventana del punto
//      current_frame.colRange(wPrev.pt1.x, wPrev.pt2.x).rowRange(wPrev.pt1.y, wPrev.pt2.y).copyTo(crop_frame);
//
//      // Se detectan los key points y los descriptores
//      featuresPrev.detectKeyPoints(crop_frame);
//      featuresPrev.calcDescriptor(crop_frame);
//
//      // Se calcula la transformada de fourier para el primer punto y otro punto mas a la derecha (habría que ver mejor la dirección de avance)
//      col_fourier.clear();
//      for (int icf = 0; icf < 20; icf++)
//        col_fourier.push_back(crop_frame.cols / 2 + icf);
//      //col_fourier.push_back(crop_frame.cols / 2);
//      //col_fourier.push_back(crop_frame.cols / 2 + 10);
//      //col_fourier.push_back(crop_frame.cols / 2 + 20);
//      fourierLinesDetection(crop_frame, col_fourier, &pts_fourier/*,&conductor_direction*/);
//
//      // Con los puntos obtenidos con la transformada de fourier determinamos cual es el punto del conductor en la 
//      // columna central y buscamos en las otras dos columnas aquellos puntos que son colineales. A partir de estos
//      // Puntos determinamos la orientación de la recta que utilizaremos en la siguiente iteración para determinar
//      // los puntos mediante la transformada de fourier.
//
//      // Calculo de la línea
//      // ... Ver que se obtienen el mismo número de puntos en cada corte
//      // ... Si no comprobar distancias entre cada punto consecutivo. Tienen que ser similares en cada corte.
//      // ... Comprobar por angulos de 1 a 0 y de 0 a 2
//
//
//
//      //// Se busca el punto mas próximo al punto de inicio del conductor
//      //size_t ipt = pointNearest(pts_fourier[0], ptPrev - wPrev.pt1);
//      ////ptCur = pts_fourier[0][ipt];
//      //// Si la distancia es mayor de 2px probablemente no haya detectado el punto por fourier. 
//      //ptCur = (I3D::distance(pts_fourier[0][ipt], ptPrev - wPrev.pt1) < 2.) ? pts_fourier[0][ipt] : ptPrev - wPrev.pt1;
//
//      //// Puntos derecho en la recta que pasa por 'ptCur' con pendiente 0 (pendiente inicial)
//      //cv::Point pt_right1, pt_right2;
//      //pt_right1.x = col_fourier[1];
//      //pt_right1.y = ptCur.y + tan(conductor_direction)*(col_fourier[1] - ptCur.x);
//      //pt_right2.x = col_fourier[2];
//      //pt_right2.y = ptCur.y + tan(conductor_direction)*(col_fourier[2] - ptCur.x);
//      ////std::vector<cv::Point> pts;
//      ////for (int ir = 1; ir < 10; ir++){
//      ////  ipt = pointNearest(pts_fourier[ir], ptCur);
//      ////  pts.push_back(pts_fourier[ir][ipt]);
//      ////}
//
//
//      //conductor_line_prev.pt1 = ptCur;
//      //size_t iptr1 = pointNearest(pts_fourier[1], pt_right1);
//      ////conductor_line_prev.pt2 = pts_fourier[1][iptr1];
//      //size_t iptr2 = pointNearest(pts_fourier[2], pt_right2);
//      ////conductor_line_prev.pt2 = pts_fourier[2][iptr2];
//      //if (I3D::distance(pts_fourier[1][iptr1], pt_right1) < ((col_fourier[1] - col_fourier[0]) / 5.) + 2) {
//      //  conductor_line_prev.pt2 = pts_fourier[1][iptr1];
//      //} else if (I3D::distance(pts_fourier[2][iptr2], pt_right2) < ((col_fourier[2] - col_fourier[0]) / 5.) + 2) {
//      //  conductor_line_prev.pt2 = pts_fourier[2][iptr2];
//      //}
//
//      //// Se calcula la dirección del conductor
//      //conductor_direction = conductor_line_prev.angleOX();
//
//
//
//
//
//
//
//
//
//
//      int r = ptPrev.y - wPrev.pt1.y;
//      std::vector<cv::Point> pts;
//      for (int iptf = 0; iptf < pts_fourier.size(); iptf++) {
//        size_t ipt = pointNearest(pts_fourier[iptf], cv::Point(col_fourier[iptf], r));
//        pts.push_back(pts_fourier[iptf][ipt]);
//      }
//      double m = 0.;
//      double b = 0.;
//      bool bcontinue = true;
//      while (bcontinue) {
//        // Recta de regresión
//        m = 0.;
//        b = 0.;
//        double corr = RegressionLinearYX(pts, &m, &b);
//        double dist = I3D_DOUBLE_MIN;
//        int idist = 0;
//        for (int ipt = 0; ipt < pts.size(); ipt++) {
//          cv::Point pt_r = cv::Point(pts[ipt].x, m * pts[ipt].x + b);
//          double distaux = I3D::distance(pts[ipt], pt_r);
//          if (distaux > dist){
//            dist = distaux;
//            idist = ipt;
//          }
//        }
//        if (dist > 2.) {
//          pts.erase(pts.begin() + idist);
//        } else
//          bcontinue = false;
//      }
//      conductor_line_prev.pt1 = cv::Point(col_fourier[0], m * col_fourier[0] + b);
//      conductor_line_prev.pt2 = cv::Point(col_fourier[19], m * col_fourier[19] + b);
//      conductor_direction = conductor_line_prev.angleOX();
//
//
//    } else {
//
//      // Se determinan puntos y descriptores en la imagen actual
//      //... No calcular con toda la ventana. Calcular una ventana estimada
//      featuresCur.detectKeyPoints(current_frame);
//      featuresCur.calcDescriptor(current_frame);
//
//      // Cálculo de matching
//      std::vector< DMatch > matches;
//      match.match(featuresPrev, featuresCur, &matches);
//
//      // Nos quedamos con los mejores puntos
//      std::vector< DMatch > good_matches;
//      match.getGoodMatches(&good_matches);
//
//      std::vector<Point2f> ptsprev;
//      std::vector<Point2f> ptscur;
//      for (size_t igm = 0; igm < good_matches.size(); igm++) {
//        ptsprev.push_back(featuresPrev.getKeyPoint(good_matches[igm].queryIdx));
//        ptscur.push_back(featuresCur.getKeyPoint(good_matches[igm].trainIdx));
//      }
//
//      // Calculo de la transformación proyectiva
//      TrfPerspective<Point2f> trfPerps;
//      if ( !trfPerps.compute(ptsprev, ptscur) ) return 0;
//      
//      // Se transforma la línea del conductor para determinar la posición en la imagen actual
//      std::vector<cv::Point2f> l_in = { conductor_line_prev.pt1, conductor_line_prev.pt2 };
//      std::vector<cv::Point2f> l_out;
//      trfPerps.transform(l_in, &l_out);
//      Line line_proj(l_out[0], l_out[1]);
//
//      cv::Mat out;
//      cvtColor(current_frame, out, CV_GRAY2BGR);
//      
//      // Linea en la imagen actual 
//      line(out, line_proj.pt1, line_proj.pt2, Scalar(0, 255, 0), 2, LINE_AA);
//
//      // Linea vertical de centro de pantalla
//      line(out, cv::Point(current_frame.cols / 2, 0), cv::Point(current_frame.cols / 2, current_frame.rows), Scalar(0, 255, 0), 1, LINE_AA);
//
//      // Intersección de línea de centro con línea proyectada para determinar el punto del conductor. Este punto se considera una primera aproximación
//      cv::Point pt_intersect;         
//      intersectLines(line_proj, Line(cv::Point(current_frame.cols / 2, 0), cv::Point(current_frame.cols / 2, current_frame.rows)), &pt_intersect);
//      line(out, pt_intersect, pt_intersect, Scalar(255, 0, 0), 3, LINE_AA);
//
//      // drawing the results
//      //cv::Mat img_matches;
//      //cv::drawMatches(crop_frame, featuresPrev.GetKeyPoints(), current_frame, featuresCur.GetKeyPoints(), good_matches, img_matches, Scalar::all(-1), Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
//
//      // Repetido...
//
//      // Obtenemos la ventana envolvente del punto
//      wCur = WindowI(pt_intersect, 200);
//      wCur = windowIntersection(wCur, WindowI(cv::Point(0, 0), cv::Point(current_frame.cols, current_frame.rows)));
//      // Aplicamos un filtrado de features por ventana para eliminar las que esten fuera
//      featuresCur.filter(current_frame, wCur, &crop_frame, NULL, NULL);
//
//      pts_fourier.clear();
//      col_fourier.clear();
//      //col_fourier.push_back(crop_frame.cols / 2 - 20);
//      //col_fourier.push_back(crop_frame.cols / 2);
//      //col_fourier.push_back(crop_frame.cols / 2 + 20);
//      for (int icf = 0; icf < 21; icf++)
//        col_fourier.push_back(crop_frame.cols / 2 - 10 + icf);
//      // Se vuelve a aplicar la transformada de fourier para determinar el punto del conductor en la columna central
//      if (fourierLinesDetection(crop_frame, col_fourier, &pts_fourier, &conductor_direction)) {
//
//        // Se busca el punto mas próximo al punto obtenido por intersección de la linea del conductor con la columna central
//        //size_t ipt = pointNearest(pts_fourier[1], pt_intersect - wCur.pt1);
//        
//        ////ptCur = pts_fourier[1][ipt];
//        //// Si la distancia es mayor de 2px probablemente no haya detectado el punto por fourier. 
//        //ptCur = ( I3D::distance(pts_fourier[1][ipt], pt_intersect - wCur.pt1) < 2. ) ? pts_fourier[1][ipt] : pt_intersect - wCur.pt1;
//
//        //// Puntos izquierdo y derecho en la recta que pasa por 'ptCur' con pendiente 'conductor_direction'
//        //cv::Point pt_left, pt_right;
//        //double m = tan(conductor_direction);
//        //pt_left.x = col_fourier[0];
//        //pt_left.y = ptCur.y + m*(col_fourier[0] - ptCur.x);
//        //pt_right.x = col_fourier[2];
//        //pt_right.y = ptCur.y + m*(col_fourier[2] - ptCur.x);
//
//        //// Busqueda de puntos colineales para determinar el ángulo del conductor
//        //size_t iptl = pointNearest(pts_fourier[0], pt_left);
//        //size_t iptr = pointNearest(pts_fourier[2], pt_right);
//
//        //conductor_line_prev.pt1 = pts_fourier[0][iptl];
//        //conductor_line_prev.pt2 = pts_fourier[2][iptr];
//
//        //if (isCollinearPoints(ptCur, conductor_line_prev)) {
//        //  // Actualización del angulo del conductor. Si no son puntos colineales mantenemos el ángulo anterior para la siguiente iteración
//        //  conductor_direction = conductor_line_prev.angleOX();
//        //  line(out, conductor_line_prev.pt1 + wCur.pt1, conductor_line_prev.pt2 + wCur.pt1, Scalar(0, 150, 150), 1, LINE_AA);
//        //} else {
//        //  //línea que pasa por ptCur con angulo 'conductor_direction'
//        //  conductor_line_prev.pt1 = pt_left;
//        //  conductor_line_prev.pt2 = pt_right;
//        //}
//        
//
//
//
//        ///////////////////////////////////////////////////////////////////////////////////////////////
//        ptCur = pt_intersect - wCur.pt1;
//        std::vector<cv::Point> pts;
//        for (int iptf = 0; iptf < pts_fourier.size(); iptf++) {
//          cv::Point pt_r;
//          double slope = tan(conductor_direction);
//          pt_r.x = col_fourier[iptf];
//          pt_r.y = ptCur.y + slope*(col_fourier[iptf] - ptCur.x);
//          size_t ipt = pointNearest(pts_fourier[iptf], pt_r);
//          pts.push_back(pts_fourier[iptf][ipt]);
//        }
//        double m = 0.;
//        double b = 0.;
//        bool bcontinue = true;
//        while (bcontinue) {
//          // Recta de regresión
//          m = 0.;
//          b = 0.;
//          double corr = RegressionLinearYX(pts, &m, &b);
//          double dist = I3D_DOUBLE_MIN;
//          int idist = 0;
//          for (int ipt = 0; ipt < pts.size(); ipt++) {
//            cv::Point pt_r = cv::Point(pts[ipt].x, m * pts[ipt].x + b);
//            double distaux = I3D::distance(pts[ipt], pt_r);
//            if (distaux > dist){
//              dist = distaux;
//              idist = ipt;
//            }
//          }
//          if (dist > 2.) {
//            pts.erase(pts.begin() + idist);
//          } else
//            bcontinue = false;
//        }
//        conductor_line_prev.pt1 = cv::Point(col_fourier[0], m * col_fourier[0] + b);
//        conductor_line_prev.pt2 = cv::Point(col_fourier[19], m * col_fourier[19] + b);
//        conductor_direction = conductor_line_prev.angleOX();
//
//        ptCur = cv::Point(col_fourier[10], m * col_fourier[10] + b);
//        ///////////////////////////////////////////////////////////////////////////////////////
//
//
//
//
//
//
//
//
//
//        // Actualizamos el punto del conductor a las coordenadas de la ventana completa 
//        ptCur += wCur.pt1;
//
//        line(out, ptCur, ptCur, Scalar(0, 0, 255), 3, LINE_AA);
//
//        namedWindow("TrackLines", WINDOW_AUTOSIZE);
//        imshow("TrackLines", out);
//        cv::waitKey();
//
//        char buffer[50];
//        sprintf_s(buffer, "TrackLines_2_%02i.jpg", i);
//        cv::imwrite(buffer, out);
//
//        // Asignamos valores previos para la siguiente iteración
//        ptPrev = ptCur;
//        wPrev = wCur;
//        featuresPrev = featuresCur;
//      }
//    }
//  }
//
//  return 0;
//}
