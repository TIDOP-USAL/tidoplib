#include <stdio.h>

// Cabeceras tidopLib
#include "core/console.h"
#include "core/messages.h"
#include "img/imgio.h"
#include "img_process/matching.h"

#include "geometry/entities/point.h"
#include "geometry/transform.h" 

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/video/tracking.hpp"

using namespace TL;
using namespace TL::geometry;

using namespace cv;
using namespace cv::xfeatures2d;

#ifdef VISUAL_LEAK_DETECTOR
#include <vld.h>
#endif


std::vector<std::string> mImagesPaths;
std::vector<std::vector<cv::KeyPoint>> mKeyPoints;
std::vector<cv::Mat> mDescriptor;
std::unique_ptr<Features2D> mFeature2D;
std::vector<cv::Mat> imgs;

void getKeyPointAndDescriptor(const std::vector<std::string> &imagesPaths, std::vector<cv::Rect> &mask, bool bSave, TL::ProgressBar *progress)
{

  mImagesPaths = imagesPaths;
  int size = static_cast<int>(mImagesPaths.size());
  mKeyPoints.resize(size);
  mDescriptor.resize(size);
  //mFeature2D = std::make_unique<Features2D>(SURF::create(), SURF::create());
  RasterGraphics img;
  for (int i = 0; i < size; i++) {
    //cv::Mat image = cv::imread(mImagesPaths[i].c_str(), cv::IMREAD_GRAYSCALE);
    TL::File::Status err = img.open(mImagesPaths[i].c_str(), TL::File::Mode::Read);
    if (err != TL::File::Status::OPEN_FAIL) {
      cv::Mat data;
      img.read(&data, WindowI());
      if (img.getBands() >= 3)
        cv::cvtColor(data, data, CV_RGB2GRAY);

      cv::Mat _mask = Mat::zeros(data.size(), CV_8U);
      cv::Mat roi(_mask, mask[i]);
      roi = Scalar(255, 255, 255);

      std::vector<cv::KeyPoint> keyPoint;
      cv::Mat descriptor;
      mFeature2D->detectKeyPoints(data, &keyPoint, _mask);
      mFeature2D->calcDescriptor(data, NULL, &descriptor);

      mKeyPoints[i] = keyPoint;
      mDescriptor[i] = descriptor;

      if (bSave) {
        char out[TL_MAX_PATH];
        changeFileExtension(mImagesPaths[i].c_str(), "bin", out, TL_MAX_PATH);

        // Salvamos key points y los descriptores
        mFeature2D->save(out);

      }
      img.close();
    }
    if (progress) (*progress)();
  }
}

void loadKeyPointAndDescriptor(const std::vector<std::string> &imagesPaths, TL::ProgressBar *progress)
{
  mImagesPaths = imagesPaths;
  int size = static_cast<int>(mImagesPaths.size());
  char out[TL_MAX_PATH];
  //mFeature2D = std::make_unique<Features2D>(cv::xfeatures2d::SURF::create(), cv::xfeatures2d::SURF::create());
  mKeyPoints.resize(size);
  mDescriptor.resize(size);
  for (int i = 0; i < size; i++) {
    changeFileExtension(mImagesPaths[i].c_str(), "bin", out, TL_MAX_PATH);
    mFeature2D->read(out);
    mKeyPoints[i] = mFeature2D->getKeyPoints();
    mDescriptor[i] = mFeature2D->getDescriptors();
  }
}


/*!
 * read_image: 
 *
 *
 * uso:
 */
int main(int argc, char** argv)
{



  char name[TL_MAX_FNAME];
  getFileName(getRunfile(), name, TL_MAX_FNAME);

  CmdParser cmdParser(name, "Reconstrucción de la imagen completa");
  cmdParser.addParameter("img", "directorio de imagenes");
  cmdParser.addParameter("out", "directorio de salida de los datos");
  cmdParser.addOption("loadFeat", "Carga los puntos de paso y descriptores existentes");
  CmdParser::Status status = cmdParser.parse(argc, argv);
  if (status == CmdParser::Status::PARSE_ERROR ) {
    return 1;
  } else if (status == CmdParser::Status::PARSE_HELP) {
    return 0;
  }

  std::string img = cmdParser.getValue<std::string>("img");
  std::string out = cmdParser.getValue<std::string>("out");
  bool bLoadFeat = cmdParser.hasOption("loadFeat");

  double distance = 0.005;
  double confidence = 0.99;


  // Consola
  Console console;
  console.setTitle(name);
  console.setLogLevel(MessageLevel::MSG_VERBOSE);
  console.setConsoleUnicode();

  // Fichero de log
  Log &log = Log::getInstance();
  char logfile[TL_MAX_PATH];
  sprintf(logfile, "%s//%s.log", out.c_str(), name );
  log.setLogFile(logfile);
  log.setLogLevel(MessageLevel::MSG_VERBOSE);

  TL::ProgressBar progress(0, 6);

  std::vector<std::vector<Helmert2D<cv::Point2f>>> trfs(6, std::vector<Helmert2D<cv::Point2f>>(6));
   
  struct match_hugin
  {
    cv::Point2f pt1;
    cv::Point2f pt2;
  };

  std::vector<std::vector<std::vector<match_hugin>>> points_hugin(6, std::vector<std::vector<match_hugin>>(6));

  std::vector<cv::Rect> mask;
  mask.push_back(cv::Rect(481.787018, 460.636444, 7412.415527-481.787018, 5501.093750-460.636444));
  mask.push_back(cv::Rect(407.998505, 305.946014, 7332.963379-407.998505, 5342.284180-305.946014));
  mask.push_back(cv::Rect(336.406189, 146.727249, 7257.174805-336.406189, 5180.013672-146.727249));
  mask.push_back(cv::Rect(900.143799, 94.900253, 8452.798828-900.143799, 5130.003418-94.900253));
  mask.push_back(cv::Rect(971.339844, 178.972061, 8529.369141-971.339844, 5217.658203-178.972061));
  mask.push_back(cv::Rect(1045.293335, 398.668915, 8609.169922-1045.293335, 5441.253418-398.668915));

  std::list<std::string> fileList;
  fileListByExt(img, &fileList, ".tif"); 

  std::vector<std::string> files;
  for (auto &file : fileList) {
    files.push_back(std::string(img).append("\\").append(file));
  }

  std::vector<std::vector<int> > idx_pass_points;
  std::vector<std::vector<cv::Vec2d> > pass_points;
  std::vector<cv::DMatch> matches;
  cv::Ptr<cv::Feature2D> fd = cv::xfeatures2d::SURF::create(); //cv::ORB::create(1000);
  cv::Ptr<cv::Feature2D> de = cv::xfeatures2d::SURF::create(); //cv::xfeatures2d::DAISY::create();
  Features2D feature1(fd, de);
  Features2D feature2(fd, de);
  mFeature2D = std::make_unique<Features2D>(fd, de);
  cv::FlannBasedMatcher matcherA;

  if (1/*bLoadFeat*/) {
    loadKeyPointAndDescriptor(files, &progress);
  } else {
    getKeyPointAndDescriptor(files, mask, true, &progress);
    return 0;
  }

  //std::unique_ptr<TL::Features2D> pFeatures1 = std::make_unique<TL::Features2D>(SIFT::create(), SIFT::create());
  //std::unique_ptr<TL::Features2D> pFeatures2 = std::make_unique<TL::Features2D>(SIFT::create(), SIFT::create());
  std::unique_ptr<TL::Matching> pMatch = std::make_unique<TL::Matching>(cv::DescriptorMatcher::create("FlannBased"));

  progress.restart();
  progress.init(0, files.size());


  std::shared_ptr<RobustMatching> mRobustMatching = std::make_shared<RobustMatching>(cv::FlannBasedMatcher::create());

  // matching cruzado
  for (int i = 0; i < files.size(); ++i) {
    for (int j = i + 1; j < files.size(); ++j) {

      msgInfo("Imagen 1: %s", files[i].c_str());
      msgInfo("Imagen 2: %s", files[j].c_str());

      std::vector <std::vector<cv::DMatch>> robus_matches;
      //matcherA.knnMatch(mDescriptor[i], mDescriptor[j], robus_matches, 2);
      mRobustMatching->fastRobustMatch(mDescriptor[i], mDescriptor[j], &robus_matches);
      std::vector<cv::DMatch> symMatches;
      mRobustMatching->symmetryTest(robus_matches, &symMatches);

      //cv::Mat img_matches;
      //cv::Mat image1 = cv::imread(files[j].c_str(), cv::IMREAD_GRAYSCALE);
      //cv::Mat image2 = cv::imread(files[i].c_str(), cv::IMREAD_GRAYSCALE); 
      //RasterGraphics img;
      //img.open(files[i].c_str());
      //Window<cv::Point2f> w1(cv::Point2f(0, 0), cv::Point2f(img.getCols(), img.getRows()));
      //Window<cv::Point2f> w2(w1);
      //img.close();
      //cv::drawMatches(image1, mKeyPoints[j], image2, mKeyPoints[i], symMatches, img_matches, cv::Scalar::all(-1), cv::Scalar::all(-1), std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
      //image1.release();
      //image2.release();
      //img_matches.release();
 
      size_t nPoints = symMatches.size();
      std::vector<cv::Point2f> pts1(nPoints);
      std::vector<cv::Point2f> pts2(nPoints);
      for (size_t igm = 0; igm < nPoints; igm++) {
        pts1[igm] = mKeyPoints[i][symMatches[igm].queryIdx].pt;
        pts2[igm] = mKeyPoints[j][symMatches[igm].trainIdx].pt;
      }
            
      msgInfo("Número de puntos despues de filtrado por match simetricos: %i", nPoints);
      
      // Nos quedamos con los mejores puntos
      std::vector<DMatch> good_matches;
      //pMatch->getGoodMatches(mKeyPoints[j], mKeyPoints[i], &good_matches, distance, confidence);
      //pMatch->getGoodMatches(mKeyPoints[i], mKeyPoints[j], &symMatches, 0.05);
      if (!symMatches.empty()) {
    
        //getGoodMatches(gm, 0.05);
        std::vector<cv::DMatch> match_aux = symMatches;
        sortVector(&match_aux);
        double min_dist = match_aux.front().distance;
    
        for (size_t i = 0; i < match_aux.size(); i++) {
          if (match_aux[i].distance <= std::max(2 * min_dist, 0.05)) {
            good_matches.push_back(match_aux[i]);
          }
        }

        size_t nPoints = good_matches.size();
        std::vector<cv::Point2f> pts1_2(nPoints);
        std::vector<cv::Point2f> pts2_2(nPoints);

        for (size_t igm = 0; igm < nPoints; igm++) {
          pts1_2[igm] = mKeyPoints[i][good_matches[igm].queryIdx].pt;
          pts2_2[igm] = mKeyPoints[j][good_matches[igm].trainIdx].pt;
        }

        // Se calcula una transformación perspectiva y se van limpiando puntos
        //... Hay que revisar el valor de error medio cuadrático que se pone y
        //    la forma de ver la convergencia de la solución
        TrfPerspective<cv::Point2f> trfPerps;
        std::vector<double> err;
        int it = 0;
        double rmse = trfPerps.rootMeanSquareError(pts1_2, pts2_2, &err);
        if (rmse == TL_DOUBLE_MAX) continue;
        while ( rmse > 0.1 && it < 30) {
          for (size_t i = 0, j = 0; i < nPoints; i++) {
            if (sqrt(err[i]) > 2*rmse) {
              pts1_2.erase(pts1_2.begin() + j);
              pts2_2.erase(pts2_2.begin() + j);
              good_matches.erase(good_matches.begin() + j);
            } else j++;
          }
          if (nPoints == pts1_2.size()) break;
          else nPoints = pts1_2.size();
          rmse = trfPerps.rootMeanSquareError(pts1_2, pts2_2, &err);
          if (rmse == TL_DOUBLE_MAX) break;
          it++;
          msgInfo("Iteración %i: err = %f", it, rmse);
        }

      } else continue;

      nPoints = good_matches.size();
      std::vector<cv::Point2f> pts1_3(nPoints);
      std::vector<cv::Point2f> pts2_3(nPoints);
      for (size_t igm = 0; igm < nPoints; igm++) {
        pts1_3[igm] = mKeyPoints[i][good_matches[igm].queryIdx].pt;
        pts2_3[igm] = mKeyPoints[j][good_matches[igm].trainIdx].pt;
      }
            
      msgInfo("Número de puntos despues de filtrado 2: %i", nPoints);


      std::vector<double> error;
      double rmse;
      Helmert2D<cv::Point2f> trf;
      trf.compute(pts2_3, pts1_3, &error, &rmse);
      if (rmse > 100) continue; // No hay correspondecia de imagenes

      double mult_rmse = 2.;
      int iter = 0;
      std::vector<cv::Point2f> pts1_f3(pts1_3);
      std::vector<cv::Point2f> pts2_f3(pts2_3);
      while (rmse > 0.5) {

        size_t n = pts1_f3.size();
        for (size_t k = 0, l = 0; k < n; k++) {
          if (sqrt(error[k]) > rmse /** mult_rmse*/) {
            pts1_f3.erase(pts1_f3.begin() + l);
            pts2_f3.erase(pts2_f3.begin() + l);
        //    //points[i][j].erase(points[i][j].begin() + l);
          } else l++;
        }
        if (pts1_f3.empty() || pts1_f3.size() < 5 || iter == 30 /* Por si acaso */) break;
        
        if (n == pts1_f3.size()) {
          // No se ha eliminado ningun punto
          mult_rmse -= 0.25;
        } else {
          std::vector<double> error3;
          double rmse2;
          Helmert2D<cv::Point2f> trf3;
          trf3.compute(pts1_f3, pts2_f3, &error3, &rmse2);
          if (rmse2 < rmse) {
            rmse = rmse2;
            trf = trf3;
            nPoints = pts1_f3.size();
            //points[i][j].resize(points1.size());
            //for (size_t k = 0; k < points1.size(); k++) {
            //  points[i][j][k].pt1 = points1[k];
            //  points[i][j][k].pt2 = points2[k];
            //}
          }
        }
        iter++;

      }

      msgInfo("Número de puntos despues de filtrado 3: %i", nPoints);

      msgInfo("rmse: %f", rmse);
      msgInfo("%f", trf.getScale());
      msgInfo("0");
      msgInfo("0");
      msgInfo("%f", -trf.getScale());
      msgInfo("%f", trf.tx);
      msgInfo("%f", -trf.ty);

    }
  }

  //// matching cruzado
  //for (int i = 0; i < files.size(); ++i) {
  //  for (int j = i+1; j < files.size(); ++j) {

  //    msgInfo("Imagen 1: %s", files[i].c_str());
  //    msgInfo("Imagen 2: %s", files[j].c_str());
  //    char name1[TL_MAX_FNAME];
  //    char name2[TL_MAX_FNAME];
  //    char match_name[TL_MAX_FNAME + TL_MAX_EXT];
  //    getFileName(files[i].c_str(), name1, TL_MAX_FNAME);
  //    getFileName(files[j].c_str(), name2, TL_MAX_FNAME);
  //    std::string name = std::string(name1).append("-").append(name2).append(".bin");
  //    changeFileNameAndExtension(files[i].c_str(), name.c_str(), match_name, TL_MAX_FNAME + TL_MAX_EXT);

  //    
  //    if (isFile(match_name)) {
  //      pMatch->load(match_name);
  //      msgInfo("Número de puntos matching: %i", pMatch->getMatches()->size());
  //    } else {
  //      std::vector<cv::DMatch> matches;
  //      try {
  //        pMatch->match( mDescriptor[i], mDescriptor[j], &matches);
  //        pMatch->save(match_name);
  //        msgInfo("Número de puntos matching: %i", matches.size());
  //      } catch (std::exception &e) {
  //        msgError("%s", e.what());
  //        exit(EXIT_FAILURE);
  //      }
  //    }
  //    

  //    // Nos quedamos con los mejores puntos
  //    std::vector<DMatch> good_matches;
  //    //pMatch->getGoodMatches(mKeyPoints[j], mKeyPoints[i], &good_matches, distance, confidence);
  //    pMatch->getGoodMatches(mKeyPoints[i], mKeyPoints[j], &good_matches, 0.05);
  //    size_t nPoints = good_matches.size();
  //    std::vector<cv::Point2f> pts1(nPoints);
  //    std::vector<cv::Point2f> pts2(nPoints);
  //    for (size_t igm = 0; igm < nPoints; igm++) {
  //      pts1[igm] = mKeyPoints[i][good_matches[igm].queryIdx].pt;
  //      pts2[igm] = mKeyPoints[j][good_matches[igm].trainIdx].pt;
  //    }
  //          
  //    msgInfo("Número de puntos despues de filtrado: %i", nPoints);

  //    //cv::Mat img_matches;
  //    //cv::Mat image1 = cv::imread(files[j].c_str(), cv::IMREAD_GRAYSCALE);
  //    //cv::Mat image2 = cv::imread(files[i].c_str(), cv::IMREAD_GRAYSCALE); 
  //    //RasterGraphics img;
  //    //img.open(files[i].c_str());
  //    //Window<cv::Point2f> w1(cv::Point2f(0, 0), cv::Point2f(img.getCols(), img.getRows()));
  //    //Window<cv::Point2f> w2(w1);
  //    //img.close();
  //    //cv::drawMatches(image1, mKeyPoints[j], image2, mKeyPoints[i], good_matches, img_matches, cv::Scalar::all(-1), cv::Scalar::all(-1), std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
  //    //image1.release();
  //    //image2.release();
  //    //img_matches.release();

  //    std::vector<double> error;
  //    double rmse;
  //    Helmert2D<cv::Point2f> trf;
  //    trf.compute(pts2, pts1, &error, &rmse);
  //    if (rmse > 100) continue; // No hay correspondecia de imagenes
  //    msgInfo("rmse: %f", rmse);
  //    msgInfo("%f", trf.getScale());
  //    msgInfo("0");
  //    msgInfo("0");
  //    msgInfo("%f", -trf.getScale());
  //    msgInfo("%f", trf.tx);
  //    msgInfo("%f", -trf.ty);

  //    ////Ventana intersección y segundo filtrado
  //    //Window<cv::Point2f> w2_trf;
  //    //TL::transform(w2, &w2_trf, &trf);
  //    //Window<cv::Point2f> w1i = windowIntersection(w1, w2_trf);
  //    //Window<cv::Point2f> w2i;
  //    //TL::transform(w1i, &w2i, &trf, transform_order::INVERSE);
  //    //w2i = windowIntersection(w1, w2i);

  //    //std::vector<cv::Point2f> pts1_f2;
  //    //std::vector<cv::Point2f> pts2_f2;
  //    //cv::Point2f pt1;
  //    //cv::Point2f pt2;
  //    //for (size_t igm = 0; igm < nPoints; igm++) {
  //    //  pt1 = mKeyPoints[i][good_matches[igm].queryIdx].pt;
  //    //  pt2 = mKeyPoints[j][good_matches[igm].trainIdx].pt;
  //    //  if (w1i.containsPoint(pt1) && w2i.containsPoint(pt2)) {
  //    //    pts1_f2.push_back(pt1);
  //    //    pts2_f2.push_back(pt2);
  //    //    //match_hugin match;
  //    //    //match.pt1 = pt1;
  //    //    //match.pt2 = pt2;
  //    //    //points_hugin[i][j].push_back(match);
  //    //    //msgInfo("c n%i N%i x%f y%f X%f Y%f t0", i, j, pt1.x, pt1.y, pt2.x, pt2.y);
  //    //  }
  //    //}


  //    //std::vector<double> error2;
  //    //double rmse2;
  //    //trfs[i][j].compute(pts2_f2, pts1_f2, &error2, &rmse2);

  //    //msgInfo("%f", trfs[i][j].getScale());
  //    //msgInfo("0");
  //    //msgInfo("0");
  //    //msgInfo("%f", -trfs[i][j].getScale());
  //    //msgInfo("%f", trfs[i][j].tx);
  //    //msgInfo("%f", -trfs[i][j].ty);














  //    //double mult_rmse = 2.;
  //    //int iter = 0;
  //    //      
  //    //std::vector<cv::Point2f> pts1_f3(pts1_f2);
  //    //std::vector<cv::Point2f> pts2_f3(pts2_f2);
  //    //while (rmse2 > 0.5) {

  //    //  size_t n = pts1_f3.size();
  //    //  for (size_t k = 0, l = 0; k < n; k++) {
  //    //    if (sqrt(error[k]) > rmse /** mult_rmse*/) {
  //    //      pts1_f3.erase(pts1_f3.begin() + l);
  //    //      pts2_f3.erase(pts2_f3.begin() + l);
  //    //  //    //points[i][j].erase(points[i][j].begin() + l);
  //    //    } else l++;
  //    //  }
  //    //  if (pts1_f3.empty() || pts1_f3.size() < 5 || iter == 30 /* Por si acaso */) break;
  //    //  
  //    //  if (n == pts1_f3.size()) {
  //    //    // No se ha eliminado ningun punto
  //    //    mult_rmse -= 0.25;
  //    //  } else {
  //    //    std::vector<double> error3;
  //    //    double rmse3;
  //    //    Helmert2D<cv::Point2f> trf3;
  //    //    trf3.compute(pts1_f3, pts2_f3, &error3, &rmse3);
  //    //    if (rmse3 < rmse2) {
  //    //      rmse2 = rmse3;
  //    //      trfs[i][j] = trf3;
  //    //      //points[i][j].resize(points1.size());
  //    //      //for (size_t k = 0; k < points1.size(); k++) {
  //    //      //  points[i][j][k].pt1 = points1[k];
  //    //      //  points[i][j][k].pt2 = points2[k];
  //    //      //}
  //    //    }
  //    //  }
  //    //  iter++;

  //    //}

  //  }
  //  //progress();
  //}
















  //  std::vector<cv::Point2f> pts1(nPoints);
  //  std::vector<cv::Point2f> pts2(nPoints);
  //  for (size_t igm = 0; igm < nPoints; igm++) {
  //    //pts1[igm] = keyPoints1[good_matches[igm].queryIdx].pt;
  //    //pts2[igm] = keyPoints2[good_matches[igm].trainIdx].pt;
  //    pts1[igm] = cv::Point2f(pFeatures1->getKeyPoint(good_matches[igm].queryIdx).pt.x, pFeatures1->getKeyPoint(good_matches[igm].queryIdx).pt.y);
  //    pts2[igm] = cv::Point2f(pFeatures2->getKeyPoint(good_matches[igm].trainIdx).pt.x, pFeatures2->getKeyPoint(good_matches[igm].trainIdx).pt.y);
  //  }

  //  trf.compute(pts1, pts2, &error, &rmse);




  //std::vector<cv::Point2f> points_g0 {
  //  cv::Point2f(481.787018, 460.636444),
  //  cv::Point2f(481.787018, 1090.693604),
  //  cv::Point2f(481.787018, 1720.750732),
  //  cv::Point2f(481.787018, 2350.807861),
  //  cv::Point2f(481.787018, 2980.864990),
  //  cv::Point2f(481.787018, 3610.922119),
  //  cv::Point2f(481.787018, 4240.979492),
  //  cv::Point2f(481.787018, 4871.036621),
  //  cv::Point2f(481.787018, 5501.093750),
  //  cv::Point2f(1111.844116, 460.636444),
  //  cv::Point2f(1111.844116, 1090.693604),
  //  cv::Point2f(1111.844116, 1720.750732),
  //  cv::Point2f(1111.844116, 2350.807861),
  //  cv::Point2f(1111.844116, 2980.864990),
  //  cv::Point2f(1111.844116, 3610.922119),
  //  cv::Point2f(1111.844116, 4240.979492),
  //  cv::Point2f(1111.844116, 4871.036621),
  //  cv::Point2f(1111.844116, 5501.093750)
  //};


  //std::vector<cv::Point2f> points_g5 {
  //  cv::Point2f(7978.847168, 398.668915),
  //  cv::Point2f(7978.847168, 1028.991943),
  //  cv::Point2f(7978.847168, 1659.315063),
  //  cv::Point2f(7978.847168, 2289.638184),
  //  cv::Point2f(7978.847168, 2919.961182),
  //  cv::Point2f(7978.847168, 3550.284424),
  //  cv::Point2f(7978.847168, 4180.607422),
  //  cv::Point2f(7978.847168, 4810.930664),
  //  cv::Point2f(7978.847168, 5441.253418),
  //  cv::Point2f(8609.169922, 398.668915),
  //  cv::Point2f(8609.169922, 1028.991943),
  //  cv::Point2f(8609.169922, 1659.315063),
  //  cv::Point2f(8609.169922, 2289.638184),
  //  cv::Point2f(8609.169922, 2919.961182),
  //  cv::Point2f(8609.169922, 3550.284424),
  //  cv::Point2f(8609.169922, 4180.607422),
  //  cv::Point2f(8609.169922, 4810.930664),
  //  cv::Point2f(8609.169922, 5441.253418)
  //};

  std::vector<cv::Point2f> points_g0{
    cv::Point2f(232.906406930187, 815.309809587259), 
    cv::Point2f(298.975004246239, 4489.84665475035), 
    cv::Point2f(330.023622105682, 4960.3523149601),  
    cv::Point2f(445.764666092291, 961.356334628711), 
    cv::Point2f(368.026237561441, 1449.43475568496), 
    cv::Point2f(482.407909399638, 3434.67952215645), 
    cv::Point2f(346.676499591767, 3852.46492551363), 
    cv::Point2f(397.147961655557, 4704.07627675425), 
    cv::Point2f(526.287356663216, 1385.916033039), 
    cv::Point2f(626.210986199249, 3446.01368667056), 
    cv::Point2f(531.87314750048,  3670.19713296379),  
    cv::Point2f(628.998844031273, 4953.84242519468)  
  };

  std::vector<cv::Point2f> points_g5{
    cv::Point2f(8034.50596870016, 764.87730763407),
    cv::Point2f(8103.08001973731, 4443.4154827493),
    cv::Point2f(8133.77832089543, 4914.14571751888),
    cv::Point2f(8247.76006287909, 911.043069843037),
    cv::Point2f(8171.11722108742, 1399.91401624347),
    cv::Point2f(8286.85024615041, 3386.59054621383),
    cv::Point2f(8150.9145261656,  3805.24577257408),
    cv::Point2f(8201.423398984,   4657.76513320736),
    cv::Point2f(8329.67411648586, 1336.29443268707),
    cv::Point2f(8430.23941380934, 3398.14519000907),
    cv::Point2f(8336.10652190795, 3622.19627818613),
    cv::Point2f(8433.41670479857, 4907.83242608612)  
  };








  //std::vector<cv::Point2f> points_5_h{
  //  cv::Point2f(1045.293335, 4810.930664),	
  //  cv::Point2f(1675.616333, 4810.930664),	
  //  cv::Point2f(2305.939453, 4810.930664),	
  //  cv::Point2f(2936.262451, 4810.930664),	
  //  cv::Point2f(3566.585693, 4810.930664),	
  //  cv::Point2f(4196.908691, 4810.930664),	
  //  cv::Point2f(4827.231934, 4810.930664),	
  //  cv::Point2f(5457.554688, 4810.930664),
  //  cv::Point2f(6087.877930, 4810.930664),	
  //  cv::Point2f(6718.201172, 4810.930664),	
  //  cv::Point2f(7348.523926, 4810.930664),	
  //  cv::Point2f(7978.847168, 4810.930664),	
  //  cv::Point2f(8609.169922, 4810.930664),
  //  cv::Point2f(1045.293335, 5441.253418),	
  //  cv::Point2f(1675.616333, 5441.253418),	
  //  cv::Point2f(2305.939453, 5441.253418),	
  //  cv::Point2f(2936.262451, 5441.253418),	
  //  cv::Point2f(3566.585693, 5441.253418),	
  //  cv::Point2f(4196.908691, 5441.253418),	
  //  cv::Point2f(4827.231934, 5441.253418),	
  //  cv::Point2f(5457.554688, 5441.253418),	
  //  cv::Point2f(6087.877930, 5441.253418),	
  //  cv::Point2f(6718.201172, 5441.253418),	
  //  cv::Point2f(7348.523926, 5441.253418),	
  //  cv::Point2f(7978.847168, 5441.253418),	
  //  cv::Point2f(8609.169922, 5441.253418)
  //};

  //std::vector<cv::Point2f> points_4_ht{
  //  cv::Point2f(971.339844, 178.972061),
  //  cv::Point2f(1601.175659, 178.972061),
  //  cv::Point2f(2231.011475, 178.972061),
  //  cv::Point2f(2860.847168, 178.972061),
  //  cv::Point2f(3490.682861, 178.972061),
  //  cv::Point2f(4120.518555, 178.972061),
  //  cv::Point2f(4750.354492, 178.972061),
  //  cv::Point2f(5380.190430, 178.972061),
  //  cv::Point2f(6010.025879, 178.972061),
  //  cv::Point2f(6639.861816, 178.972061),
  //  cv::Point2f(7269.697754, 178.972061),
  //  cv::Point2f(7899.533203, 178.972061),
  //  cv::Point2f(8529.369141, 178.972061),

  //  cv::Point2f(971.339844, 808.807800),
  //  cv::Point2f(1601.175659, 808.807800),	
  //  cv::Point2f(2231.011475, 808.807800),	
  //  cv::Point2f(2860.847168, 808.807800),	
  //  cv::Point2f(3490.682861, 808.807800),	
  //  cv::Point2f(4120.518555, 808.807800),	
  //  cv::Point2f(4750.354492, 808.807800),	
  //  cv::Point2f(5380.190430, 808.807800),	
  //  cv::Point2f(6010.025879, 808.807800),	
  //  cv::Point2f(6639.861816, 808.807800),	
  //  cv::Point2f(7269.697754, 808.807800),	
  //  cv::Point2f(7899.533203, 808.807800),	
  //  cv::Point2f(8529.369141, 808.807800)
  //};


  //std::vector<cv::Point2f> points_4_v_no_orientado{
  //  cv::Point2f(7899.533203, 178.972061),
  //  cv::Point2f(7899.533203, 808.807800),
  //  cv::Point2f(7899.533203, 1438.643555),
  //  cv::Point2f(7899.533203, 2068.479248),
  //  cv::Point2f(7899.533203, 2698.315186),
  //  cv::Point2f(7899.533203, 3328.150879),
  //  cv::Point2f(7899.533203, 3957.986572),
  //  cv::Point2f(7899.533203, 4587.822266),
  //  cv::Point2f(7899.533203, 5217.658203),

  //  cv::Point2f(8529.369141, 178.972061),
  //  cv::Point2f(8529.369141, 808.807800),
  //  cv::Point2f(8529.369141, 1438.643555),
  //  cv::Point2f(8529.369141, 2068.479248),
  //  cv::Point2f(8529.369141, 2698.315186),
  //  cv::Point2f(8529.369141, 3328.150879),
  //  cv::Point2f(8529.369141, 3957.986572),
  //  cv::Point2f(8529.369141, 4587.822266),
  //  cv::Point2f(8529.369141, 5217.658203)
  //};

  //std::vector<cv::Point2f> points_1_v{
  //  cv::Point2f(407.998505, 305.946014),
  //  cv::Point2f(407.998505, 935.488281),
  //  cv::Point2f(407.998505, 1565.030518),
  //  cv::Point2f(407.998505, 2194.572754),
  //  cv::Point2f(407.998505, 2824.114990),
  //  cv::Point2f(407.998505, 3453.657227),
  //  cv::Point2f(407.998505, 4083.199463),
  //  cv::Point2f(407.998505, 4712.741699),
  //  cv::Point2f(407.998505, 5342.284180),

  //  cv::Point2f(1037.540771, 305.946014),
  //  cv::Point2f(1037.540771, 935.488281),
  //  cv::Point2f(1037.540771, 1565.030518),
  //  cv::Point2f(1037.540771, 2194.572754),
  //  cv::Point2f(1037.540771, 2824.114990),
  //  cv::Point2f(1037.540771, 3453.657227),
  //  cv::Point2f(1037.540771, 4083.199463),
  //  cv::Point2f(1037.540771, 4712.741699),
  //  cv::Point2f(1037.540771, 5342.284180)

  //};


  //std::vector<cv::Point2f> points_4_hb_no_orientado{
  //  cv::Point2f(971.339844, 4587.822266),	
  //  cv::Point2f(1601.175659, 4587.822266),	
  //  cv::Point2f(2231.011475, 4587.822266),	
  //  cv::Point2f(2860.847168, 4587.822266),	
  //  cv::Point2f(3490.682861, 4587.822266),	
  //  cv::Point2f(4120.518555, 4587.822266),	
  //  cv::Point2f(4750.354492, 4587.822266),	
  //  cv::Point2f(5380.190430, 4587.822266),	
  //  cv::Point2f(6010.025879, 4587.822266),	
  //  cv::Point2f(6639.861816, 4587.822266),	
  //  cv::Point2f(7269.697754, 4587.822266),	
  //  cv::Point2f(7899.533203, 4587.822266),	
  //  cv::Point2f(8529.369141, 4587.822266),

  //  cv::Point2f(971.339844, 5217.658203),	
  //  cv::Point2f(1601.175659, 5217.658203),	
  //  cv::Point2f(2231.011475, 5217.658203),	
  //  cv::Point2f(2860.847168, 5217.658203),	
  //  cv::Point2f(3490.682861, 5217.658203),	
  //  cv::Point2f(4120.518555, 5217.658203),	
  //  cv::Point2f(4750.354492, 5217.658203),	
  //  cv::Point2f(5380.190430, 5217.658203),	
  //  cv::Point2f(6010.025879, 5217.658203),	
  //  cv::Point2f(6639.861816, 5217.658203),	
  //  cv::Point2f(7269.697754, 5217.658203),	
  //  cv::Point2f(7899.533203, 5217.658203),	
  //  cv::Point2f(8529.369141, 5217.658203)
  //};

  //std::vector<cv::Point2f> points_3_ht{
  //  cv::Point2f(900.143799, 94.900253),	
  //  cv::Point2f(1529.531738, 94.900253),	
  //  cv::Point2f(2158.919678, 94.900253),	
  //  cv::Point2f(2788.307373, 94.900253),	
  //  cv::Point2f(3417.695313, 94.900253),	
  //  cv::Point2f(4047.083252, 94.900253),	
  //  cv::Point2f(4676.471191, 94.900253),	
  //  cv::Point2f(5305.858887, 94.900253),	
  //  cv::Point2f(5935.247070, 94.900253),	
  //  cv::Point2f(6564.634766, 94.900253),	
  //  cv::Point2f(7194.022461, 94.900253),	
  //  cv::Point2f(7823.410645, 94.900253),	
  //  cv::Point2f(8452.798828, 94.900253),

  //  cv::Point2f(900.143799, 724.288147),	
  //  cv::Point2f(1529.531738, 724.288147),	
  //  cv::Point2f(2158.919678, 724.288147),	
  //  cv::Point2f(2788.307373, 724.288147),	
  //  cv::Point2f(3417.695313, 724.288147),	
  //  cv::Point2f(4047.083252, 724.288147),	
  //  cv::Point2f(4676.471191, 724.288147),	
  //  cv::Point2f(5305.858887, 724.288147),	
  //  cv::Point2f(5935.247070, 724.288147),	
  //  cv::Point2f(6564.634766, 724.288147),	
  //  cv::Point2f(7194.022461, 724.288147),	
  //  cv::Point2f(7823.410645, 724.288147),	
  //  cv::Point2f(8452.798828, 724.288147)
  //};

  //std::vector<cv::Point2f> points_1_hb_no_orientado{
  //  cv::Point2f(407.998505, 4712.741699),
  //  cv::Point2f(1037.540771, 4712.741699),	
  //  cv::Point2f(1667.083008, 4712.741699),	
  //  cv::Point2f(2296.625244, 4712.741699),	
  //  cv::Point2f(2926.167480, 4712.741699),	
  //  cv::Point2f(3555.709717, 4712.741699),	
  //  cv::Point2f(4185.251953, 4712.741699),	
  //  cv::Point2f(4814.794434, 4712.741699),	
  //  cv::Point2f(5444.336426, 4712.741699),	
  //  cv::Point2f(6073.878906, 4712.741699),	
  //  cv::Point2f(6703.420898, 4712.741699),	
  //  cv::Point2f(7332.963379, 4712.741699),
  //  
  //  cv::Point2f(407.998505, 5342.284180),
  //  cv::Point2f(1037.540771, 5342.284180),	
  //  cv::Point2f(1667.083008, 5342.284180),	
  //  cv::Point2f(2296.625244, 5342.284180),	
  //  cv::Point2f(2926.167480, 5342.284180),	
  //  cv::Point2f(3555.709717, 5342.284180),	
  //  cv::Point2f(4185.251953, 5342.284180),	
  //  cv::Point2f(4814.794434, 5342.284180),	
  //  cv::Point2f(5444.336426, 5342.284180),	
  //  cv::Point2f(6073.878906, 5342.284180),	
  //  cv::Point2f(6703.420898, 5342.284180),	
  //  cv::Point2f(7332.963379, 5342.284180)
  //};

  //std::vector<cv::Point2f> points_2_ht{
  //  cv::Point2f(336.406189, 146.727249),
  //  cv::Point2f(965.566956, 146.727249),
  //  cv::Point2f(1594.727783, 146.727249),
  //  cv::Point2f(2223.888428, 146.727249),
  //  cv::Point2f(2853.049316, 146.727249),
  //  cv::Point2f(3482.209961, 146.727249),
  //  cv::Point2f(4111.371094, 146.727249),
  //  cv::Point2f(4740.531738, 146.727249),
  //  cv::Point2f(5369.692383, 146.727249),
  //  cv::Point2f(5998.853027, 146.727249),
  //  cv::Point2f(6628.014160, 146.727249),
  //  cv::Point2f(7257.174805, 146.727249),
  //                                     
  //  cv::Point2f(336.406189, 775.888000),
  //  cv::Point2f(965.566956, 775.888000),
  //  cv::Point2f(1594.727783, 775.888000),
  //  cv::Point2f(2223.888428, 775.888000),
  //  cv::Point2f(2853.049316, 775.888000),
  //  cv::Point2f(3482.209961, 775.888000),
  //  cv::Point2f(4111.371094, 775.888000),
  //  cv::Point2f(4740.531738, 775.888000),
  //  cv::Point2f(5369.692383, 775.888000),
  //  cv::Point2f(5998.853027, 775.888000),
  //  cv::Point2f(6628.014160, 775.888000),
  //  cv::Point2f(7257.174805, 775.888000)
  //};




  //geometry::Polygon<cv::Point2f> pol1{
  //  cv::Point2f(221.666412, 22.934967),
  //  cv::Point2f(8478.197266, 22.934967),
  //  cv::Point2f(8478.197266, 5738.994629),
  //  cv::Point2f(221.666412, 5738.994629)
  //};

  //MultiPolygon<cv::Point2f> mask;
  //mask.add(pol1);
  //mask.add(pol1);
  //mask.add(pol1);
  //mask.add(pol1);
  //mask.add(pol1);
  //mask.add(pol1);

  //struct match_hugin
  //{
  //  cv::Point2f pt1;
  //  cv::Point2f pt2;
  //};

  //std::vector<std::vector<Helmert2D<cv::Point2f>>> trfs(6, std::vector<Helmert2D<cv::Point2f>>(6));
  //std::vector<std::vector<std::vector<match_hugin>>> points(6, std::vector<std::vector<match_hugin>>(6));

  //std::ifstream read_handler("C:\\Users\\Esteban\\Documents\\Digiflights\\Projects\\Zeiss_grid\\20180314\\mosaic\\1111.pto", std::ifstream::in);
  //if (read_handler.is_open()) {
  //  std::string aux;
  //  while (std::getline(read_handler, aux)) {
  //    
  //    if (aux.empty() == false) {
  //      std::vector<std::string> line;
  //      TL::split(aux, line, " ");
  //      if (line[0].compare("c") == 0) {
  //        size_t ini = line[1].find("n");
  //        size_t end = line[1].size();
  //        std::string string_aux = line[1].substr(ini + 1, end);

  //        int img1 = stoi(string_aux);

  //        ini = line[2].find("N");
  //        end = line[2].size();
  //        string_aux = line[2].substr(ini + 1, end);
  //        int img2 = stoi(string_aux);

  //        match_hugin match;

  //        ini = line[3].find("x");
  //        end = line[3].size();
  //        string_aux = line[3].substr(ini + 1, end);
  //        match.pt1.x = stod(string_aux);

  //        ini = line[4].find("y");
  //        end = line[4].size();
  //        string_aux = line[4].substr(ini + 1, end);
  //        match.pt1.y = stod(string_aux);

  //        ini = line[5].find("X");
  //        end = line[5].size();
  //        string_aux = line[5].substr(ini + 1, end);
  //        match.pt2.x = stod(string_aux);

  //        ini = line[6].find("Y");
  //        end = line[6].size();
  //        string_aux = line[6].substr(ini + 1, end);
  //        match.pt2.y = stod(string_aux);

  //        if (mask[img1].isInner(match.pt1) && mask[img2].isInner(match.pt2)) {
  //          points[img1][img2].push_back(match);
  //          //msgInfo(aux.c_str());
  //        } else {
  //          //msgError(aux.c_str());
  //        }
  //      }

  //    }
  //  }
  //}

  //std::vector<double> error;
  //double rmse;

  //for (size_t i = 0; i < trfs.size(); ++i) {
  //  for (size_t j = 0; j < trfs[0].size(); ++j) {
  //    
  //    if (points[i][j].empty()) continue;
  //    
  //    size_t n = points[i][j].size();

  //    std::vector<cv::Point2f> points1(n);
  //    std::vector<cv::Point2f> points2(n);
  //    for (size_t ip = 0; ip < points[i][j].size(); ip++) {
  //      points1[ip] = points[i][j][ip].pt1;
  //      points2[ip] = points[i][j][ip].pt2;
  //    }

  //    trfs[i][j].compute(points1, points2, &error, &rmse);

  //    double mult_rmse = 2.;
  //    int iter = 0;
  //    while (rmse > 0.5) {

  //      size_t n = points1.size();
  //      for (size_t k = 0, l = 0; k < n; k++) {
  //        if (sqrt(error[k]) > rmse * mult_rmse) {
  //          points1.erase(points1.begin() + l);
  //          points2.erase(points2.begin() + l);
  //          //points[i][j].erase(points[i][j].begin() + l);
  //        } else l++;
  //      }
  //      if (points1.empty() || points1.size() < 5 || iter == 100 /* Por si acaso */) break;
  //      
  //      if (n == points1.size()) {
  //        // No se ha eliminado ningun punto
  //        mult_rmse -= 0.25;
  //      } else {
  //        std::vector<double> error2;
  //        double rmse2;
  //        Helmert2D<cv::Point2f> trf;
  //        trf.compute(points1, points2, &error2, &rmse2);
  //        if (rmse2 < rmse) {
  //          rmse = rmse2;
  //          trfs[i][j] = trf;
  //          points[i][j].resize(points1.size());
  //          for (size_t k = 0; k < points1.size(); k++) {
  //            points[i][j][k].pt1 = points1[k];
  //            points[i][j][k].pt2 = points2[k];
  //          }
  //        }
  //      }
  //      iter++;
  //      //trfs[i][j].compute(points1, points2, &error, &rmse);
  //    }

  //  }
  //}


///////////////////////////////////////////////////////////////////////////////////////////////////////////


//  for (size_t i = 0; i < trfs.size(); ++i) {
//    for (size_t j = 0; j < trfs[0].size(); ++j) {
//
//      if (points[i][j].empty()) continue;
//
//      size_t n = points[i][j].size();
//
//      std::vector<cv::Point2f> points1(n);
//      std::vector<cv::Point2f> points2(n);
//      for (size_t ip = 0; ip < points[i][j].size(); ip++) {
//        points1[ip] = points[i][j][ip].pt1;
//        points2[ip] = points[i][j][ip].pt2;
//      }
//
//      trfs[i][j].compute(points1, points2, &error, &rmse);
//
//      std::vector<uchar> inliers(n, 0);
//      cv::Mat fundamental = cv::findFundamentalMat(cv::Mat(points1), cv::Mat(points2), inliers, CV_FM_RANSAC, distance, confidence);
//
//      std::vector<uchar>::const_iterator itIn = inliers.begin();
//
//     
//      n = inliers.size();
//      int l = 0;
//      for (; itIn != inliers.end(); ++itIn) {
//        if (*itIn) { 
//      //    gm->push_back(*itM);
//          l++;
//        } else {
//          points1.erase(points1.begin() + l);
//          points2.erase(points2.begin() + l);
//          points[i][j].erase(points[i][j].begin() + l);
//        }
//      }
//
//      trfs[i][j].compute(points1, points2, &error, &rmse);
//
//      //for (size_t k = 0, l = 0; k < n; k++) {
//      //  if (inliers[l] != nullptr) {
//      //    points[i][j].erase(points[i][j].begin() + l);
//      //  } else l++;
//      //}
//
//    }
//  }
//
//  //for (size_t i = 0; i < trfs.size(); ++i) {
//  //  for (size_t j = 0; j < trfs[0].size(); ++j) {
//
//  //    if (points[i][j].empty()) continue;
//
//  //    size_t n = points[i][j].size();
//
//  //    std::vector<cv::Point2f> points1(n);
//  //    std::vector<cv::Point2f> points2(n);
//  //    for (size_t ip = 0; ip < points[i][j].size(); ip++) {
//  //      points1[ip] = points[i][j][ip].pt1;
//  //      points2[ip] = points[i][j][ip].pt2;
//  //    }
//
//  //    trfs[i][j].compute(points1, points2, &error, &rmse);
//  //    
//  //    cv::Mat R = cv::estimateRigidTransform(points1, points2, false);
//
//  //    double &a = R.at<double>(0, 0);
//  //    double b = R.at<double>(0, 1);
//  //    double rotation = atan2(b, a);
//  //    double scale = sqrt(a*a + b*b);
//  //    trfs[i][j].setParameters(R.at<double>(0, 2), R.at<double>(1, 2), scale, rotation);
//
//  //    trfs[i][j].compute(points1, points2, &error, &rmse);
//
//  //    //for (size_t k = 0, l = 0; k < n; k++) {
//  //    //  if (inliers[l] != nullptr) {
//  //    //    points[i][j].erase(points[i][j].begin() + l);
//  //    //  } else l++;
//  //    //}
//
//  //  }
//  //}
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  for (size_t i = 0; i < points.size(); i++) {
//    for (size_t j = 0; j < points[i].size(); j++) {
//      if (points[i][j].empty()) continue;
//      for (size_t k = 0; k < points[i][j].size(); k++) {
//        msgInfo("c n%i N%i x%f y%f X%f Y%f t0", i, j, points[i][j][k].pt1.x, points[i][j][k].pt1.y, points[i][j][k].pt2.x, points[i][j][k].pt2.y);
//      }
//    }
//  }
//
//

  ///* ------------------------------------------------------------ */

  //Helmert2D<cv::Point2f> trf;

  //// 0
  //trf.compute(points_g0, points_g5, &error, &rmse);

  //while (rmse > 0.5) {

  //  //rmse = sqrt(sumErr/(mDimensions * (n - mMinPoint)));
  //  size_t n = points_g0.size();
  //  for (size_t i = 0, j = 0; i < n; i++) {
  //    if (sqrt(error[i]) > rmse) {
  //      points_g0.erase(points_g0.begin() + j);
  //      points_g5.erase(points_g5.begin() + j);
  //    } else j++;
  //  }
  //
  //  trf.compute(points_g0, points_g5, &error, &rmse);
  //}

  //// 4
  //trf.compute(points_4_ht, points_5_h, &error, &rmse);
  //
  //std::vector<cv::Point2f> points_4_v_orientado;
  //trf.transform(points_4_v_no_orientado, &points_4_v_orientado);
  //std::vector<cv::Point2f> points_4_hb_orientado;
  //trf.transform(points_4_hb_no_orientado, &points_4_hb_orientado);

  //// 1
  //trf.compute(points_1_v, points_4_v_orientado, &error, &rmse);
  //
  //std::vector<cv::Point2f> points_1_hb_orientado;
  //trf.transform(points_1_hb_no_orientado, &points_1_hb_orientado);

  //// 3
  //trf.compute(points_3_ht, points_4_hb_orientado, &error, &rmse);

  //// 2
  //trf.compute(points_2_ht, points_1_hb_orientado, &error, &rmse);

  //std::unique_ptr<TL::Features2D> pFeatures1 = std::make_unique<TL::Features2D>(SIFT::create(), SIFT::create());
  //std::unique_ptr<TL::Features2D> pFeatures2 = std::make_unique<TL::Features2D>(SIFT::create(), SIFT::create());
  //std::unique_ptr<TL::Matching> pMatch = std::make_unique<TL::Matching>(cv::DescriptorMatcher::create("FlannBased"));

  //RasterGraphics img1;
  //RasterGraphics img2;

  ////TL::File::Status err1 = img1.open("C:\\Users\\Esteban\\Documents\\Digiflights\\Projects\\test_grid\\20180515\\corregidas\\TEST1\\TEST10_INTER_CUBIC.tif", TL::File::Mode::Read);
  ////TL::File::Status err2 = img2.open("C:\\Users\\Esteban\\Documents\\Digiflights\\Projects\\test_grid\\20180515\\corregidas\\TEST1\\TEST15_INTER_CUBIC.tif", TL::File::Mode::Read);
  //TL::File::Status err1 = img1.open("C:\\Users\\Esteban\\Documents\\Digiflights\\Projects\\Zeiss_grid\\20180314\\corregidas\\1111\\TEST070_INTER_CUBIC.tif", TL::File::Mode::Read);
  //TL::File::Status err2 = img2.open("C:\\Users\\Esteban\\Documents\\Digiflights\\Projects\\Zeiss_grid\\20180314\\corregidas\\1111\\TEST075_INTER_CUBIC.tif", TL::File::Mode::Read);
  //

  //if (err1 != TL::File::Status::OPEN_FAIL && err2 != TL::File::Status::OPEN_FAIL) {

  //  cv::Mat data1;
  //  img1.read(&data1, WindowI());
  //  cv::Mat mask1 = Mat::zeros(data1.size(), CV_8U);
  //  cv::Mat roi1(mask1, cv::Rect(221.666412, 22.934967, 1491.901855-221.666412, 5738.994629-22.934967));
  //  roi1 = Scalar(255, 255, 255);

  //  int nft1 = pFeatures1->detectKeyPoints(data1, nullptr, mask1);
  //  if (nft1 < 5) return 1;
  //  pFeatures1->calcDescriptor(data1);

  //  cv::Mat data2;
  //  img2.read(&data2, WindowI());
  //  cv::Mat mask2 = Mat::zeros(data2.size(), CV_8U); 
  //  cv::Mat roi2(mask2, cv::Rect(7207.961426, 22.934967, 8478.197266-7207.961426, 5738.994629-22.934967));
  //  roi2 = Scalar(255, 255, 255);

  //  int nft2 = pFeatures2->detectKeyPoints(data2, nullptr, mask2);
  //  if (nft2 < 5) return 1;
  //  pFeatures2->calcDescriptor(data2);

  //  std::vector<DMatch> matches;
  //  pMatch->match(pFeatures1->getDescriptors(), pFeatures2->getDescriptors(), &matches);

  //  std::vector<DMatch> good_matches;
  //  //pMatch->getGoodMatches(&good_matches, 0.05);
  //  pMatch->getGoodMatches(pFeatures1->getKeyPoints(), pFeatures2->getKeyPoints(), &good_matches, distance, confidence);
  //  size_t nPoints = good_matches.size();
  //  std::vector<cv::Point2f> pts1(nPoints);
  //  std::vector<cv::Point2f> pts2(nPoints);
  //  for (size_t igm = 0; igm < nPoints; igm++) {
  //    //pts1[igm] = keyPoints1[good_matches[igm].queryIdx].pt;
  //    //pts2[igm] = keyPoints2[good_matches[igm].trainIdx].pt;
  //    pts1[igm] = cv::Point2f(pFeatures1->getKeyPoint(good_matches[igm].queryIdx).pt.x, pFeatures1->getKeyPoint(good_matches[igm].queryIdx).pt.y);
  //    pts2[igm] = cv::Point2f(pFeatures2->getKeyPoint(good_matches[igm].trainIdx).pt.x, pFeatures2->getKeyPoint(good_matches[igm].trainIdx).pt.y);
  //  }

  //  trf.compute(pts1, pts2, &error, &rmse);

    //double rmse = trfPerps.rootMeanSquareError(pts1, pts2, &err);

    //rmse = 10;
    //std::vector<cv::Point2f> pts1_orientados;
    //
    //// Primera limpieza de puntos
    //size_t n = pts1.size();
    //trf.transform(pts1, &pts1_orientados);
    //double sumErr = 0.;
    //error.resize(n);
    //for (int i = 0; i < n; i++) {
    //  pts1_orientados[i] -= pts2[i];
    //  error[i] = static_cast<double>(pts1_orientados[i].x * pts1_orientados[i].x + pts1_orientados[i].y * pts1_orientados[i].y);
    //  sumErr += error[i];
    //}
    //rmse = sqrt(sumErr / (2 * (n - 2)));
    //rmse = 10;

    //while (rmse > 0.5) {

    //  //rmse = sqrt(sumErr/(mDimensions * (n - mMinPoint)));
    //  n = pts1.size();
    //  for (size_t i = 0, j = 0; i < n; i++) {
    //    if (sqrt(error[i]) > rmse) {
    //      pts1.erase(pts1.begin() + j);
    //      pts2.erase(pts2.begin() + j);
    //      good_matches.erase(good_matches.begin() + j);
    //    } else j++;
    //  }

    //  trf.compute(pts1, pts2, &error, &rmse);
    //}

    //std::vector<PointF> pts1;
    //std::vector<PointF> pts2;
    //for (size_t igm = 0; igm < good_matches.size(); igm++) {
    //  PointF pt1(pFeatures1->getKeyPoint(good_matches[igm].queryIdx).pt.x, pFeatures1->getKeyPoint(good_matches[igm].queryIdx).pt.y);
    //  PointF pt2(pFeatures2->getKeyPoint(good_matches[igm].trainIdx).pt.x, pFeatures2->getKeyPoint(good_matches[igm].trainIdx).pt.y);
    //  pts1.push_back(pt1);
    //  pts2.push_back(pt2);
    //}

    //
    //std::vector<double> error;
    //double rmse;
    //trf.compute(pts1, pts2, &error, &rmse);

    //cv::Mat img_matches;
    //cv::drawMatches(data1, pFeatures1->getKeyPoints(), data2, pFeatures2->getKeyPoints(), good_matches, img_matches, Scalar::all(-1), Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

  //}

  return 0;
}
