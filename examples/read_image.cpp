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
#include "opencv2/features2d.hpp"
#include "opencv2/video/tracking.hpp"

using namespace TL;
using namespace TL::geometry;

using namespace cv;
using namespace cv::xfeatures2d;

#ifdef VISUAL_LEAK_DETECTOR
#include <vld.h>
#endif

/*!
 * read_image: 
 *
 *
 * uso:
 */
int main(int argc, char** argv)
{



  //char name[TL_MAX_FNAME];
  //getFileName(getRunfile(), name, TL_MAX_FNAME);

  //CmdParser cmdParser(name, "");
  //cmdParser.addParameter("img", "Imagen");
  //cmdParser.addParameter("img_out", "Imagen de salida");
  //CmdParser::Status status = cmdParser.parse(argc, argv);
  //if (status == CmdParser::Status::PARSE_ERROR ) {
  //  //exit(EXIT_FAILURE);
  //  return 1;
  //} else if (status == CmdParser::Status::PARSE_HELP) {
  //  //exit(EXIT_SUCCESS);
  //  return 0;
  //}

  //std::string img = cmdParser.getValue<Path>("img").toString();
  //std::string img_out = cmdParser.getValue<Path>("img_out").toString();


  //// Fichero de log
  //Log &log = Log::getInstance();
  ////Configuración de log y mensajes por consola
  //char logfile[TL_MAX_PATH];
  //if (changeFileExtension(getRunfile(), "log", logfile, TL_MAX_PATH) == 0) {
  //  log.setLogFile(logfile);
  //}
  //log.setLogLevel(MessageLevel::MSG_VERBOSE);

  // Consola
  Console console;
  //console.setTitle(name);
  console.setLogLevel(MessageLevel::MSG_VERBOSE);
  console.setConsoleUnicode();
  //console.setFontBold(true);
  //console.setFontHeight(10);
  
  ////Configuración de mensajes
  //MessageManager &msg_h = MessageManager::getInstance();
  //msg_h.addListener(&log);
  //msg_h.addListener(&console);
 
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




  geometry::Polygon<cv::Point2f> pol1{
    cv::Point2f(221.666412, 22.934967),
    cv::Point2f(8478.197266, 22.934967),
    cv::Point2f(8478.197266, 5738.994629),
    cv::Point2f(221.666412, 5738.994629)
  };

  MultiPolygon<cv::Point2f> mask;
  mask.add(pol1);
  mask.add(pol1);
  mask.add(pol1);
  mask.add(pol1);
  mask.add(pol1);
  mask.add(pol1);

  struct match_hugin
  {
    cv::Point2f pt1;
    cv::Point2f pt2;
  };

  std::vector<std::vector<Helmert2D<cv::Point2f>>> trfs(6, std::vector<Helmert2D<cv::Point2f>>(6));
  std::vector<std::vector<std::vector<match_hugin>>> points(6, std::vector<std::vector<match_hugin>>(6));

  std::ifstream read_handler("C:\\Users\\Esteban\\Documents\\Digiflights\\Projects\\Zeiss_grid\\20180314\\mosaic\\1111.pto", std::ifstream::in);
  if (read_handler.is_open()) {
    std::string aux;
    while (std::getline(read_handler, aux)) {
      
      if (aux.empty() == false) {
        std::vector<std::string> line;
        TL::split(aux, line, " ");
        if (line[0].compare("c") == 0) {
          size_t ini = line[1].find("n");
          size_t end = line[1].size();
          std::string string_aux = line[1].substr(ini + 1, end);

          int img1 = stoi(string_aux);

          ini = line[2].find("N");
          end = line[2].size();
          string_aux = line[2].substr(ini + 1, end);
          int img2 = stoi(string_aux);

          match_hugin match;

          ini = line[3].find("x");
          end = line[3].size();
          string_aux = line[3].substr(ini + 1, end);
          match.pt1.x = stod(string_aux);

          ini = line[4].find("y");
          end = line[4].size();
          string_aux = line[4].substr(ini + 1, end);
          match.pt1.y = stod(string_aux);

          ini = line[5].find("X");
          end = line[5].size();
          string_aux = line[5].substr(ini + 1, end);
          match.pt2.x = stod(string_aux);

          ini = line[6].find("Y");
          end = line[6].size();
          string_aux = line[6].substr(ini + 1, end);
          match.pt2.y = stod(string_aux);

          if (mask[img1].isInner(match.pt1) && mask[img2].isInner(match.pt2)) {
            points[img1][img2].push_back(match);
            //msgInfo(aux.c_str());
          } else {
            //msgError(aux.c_str());
          }
        }

      }
    }
  }

  std::vector<double> error;
  double rmse;

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

  double distance = 0.05;
  double confidence = 0.95;

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

  Helmert2D<cv::Point2f> trf;

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

  std::unique_ptr<TL::Features2D> pFeatures1 = std::make_unique<TL::Features2D>(SIFT::create(), SIFT::create());
  std::unique_ptr<TL::Features2D> pFeatures2 = std::make_unique<TL::Features2D>(SIFT::create(), SIFT::create());
  std::unique_ptr<TL::Matching> pMatch = std::make_unique<TL::Matching>(cv::DescriptorMatcher::create("FlannBased"));

  RasterGraphics img1;
  RasterGraphics img2;

  //TL::File::Status err1 = img1.open("C:\\Users\\Esteban\\Documents\\Digiflights\\Projects\\test_grid\\20180515\\corregidas\\TEST1\\TEST10_INTER_CUBIC.tif", TL::File::Mode::Read);
  //TL::File::Status err2 = img2.open("C:\\Users\\Esteban\\Documents\\Digiflights\\Projects\\test_grid\\20180515\\corregidas\\TEST1\\TEST15_INTER_CUBIC.tif", TL::File::Mode::Read);
  TL::File::Status err1 = img1.open("C:\\Users\\Esteban\\Documents\\Digiflights\\Projects\\Zeiss_grid\\20180314\\corregidas\\1111\\TEST070_INTER_CUBIC.tif", TL::File::Mode::Read);
  TL::File::Status err2 = img2.open("C:\\Users\\Esteban\\Documents\\Digiflights\\Projects\\Zeiss_grid\\20180314\\corregidas\\1111\\TEST075_INTER_CUBIC.tif", TL::File::Mode::Read);
  

  if (err1 != TL::File::Status::OPEN_FAIL && err2 != TL::File::Status::OPEN_FAIL) {

    cv::Mat data1;
    img1.read(&data1, WindowI());
    cv::Mat mask1 = Mat::zeros(data1.size(), CV_8U);
    cv::Mat roi1(mask1, cv::Rect(221.666412, 22.934967, 1491.901855-221.666412, 5738.994629-22.934967));
    roi1 = Scalar(255, 255, 255);

    int nft1 = pFeatures1->detectKeyPoints(data1, nullptr, mask1);
    if (nft1 < 5) return 1;
    pFeatures1->calcDescriptor(data1);

    cv::Mat data2;
    img2.read(&data2, WindowI());
    cv::Mat mask2 = Mat::zeros(data2.size(), CV_8U); 
    cv::Mat roi2(mask2, cv::Rect(7207.961426, 22.934967, 8478.197266-7207.961426, 5738.994629-22.934967));
    roi2 = Scalar(255, 255, 255);

    int nft2 = pFeatures2->detectKeyPoints(data2, nullptr, mask2);
    if (nft2 < 5) return 1;
    pFeatures2->calcDescriptor(data2);

    std::vector<DMatch> matches;
    pMatch->match(pFeatures1->getDescriptors(), pFeatures2->getDescriptors(), &matches);

    std::vector<DMatch> good_matches;
    //pMatch->getGoodMatches(&good_matches, 0.05);
    pMatch->getGoodMatches(pFeatures1->getKeyPoints(), pFeatures2->getKeyPoints(), &good_matches, distance, confidence);
    size_t nPoints = good_matches.size();
    std::vector<cv::Point2f> pts1(nPoints);
    std::vector<cv::Point2f> pts2(nPoints);
    for (size_t igm = 0; igm < nPoints; igm++) {
      //pts1[igm] = keyPoints1[good_matches[igm].queryIdx].pt;
      //pts2[igm] = keyPoints2[good_matches[igm].trainIdx].pt;
      pts1[igm] = cv::Point2f(pFeatures1->getKeyPoint(good_matches[igm].queryIdx).pt.x, pFeatures1->getKeyPoint(good_matches[igm].queryIdx).pt.y);
      pts2[igm] = cv::Point2f(pFeatures2->getKeyPoint(good_matches[igm].trainIdx).pt.x, pFeatures2->getKeyPoint(good_matches[igm].trainIdx).pt.y);
    }

    trf.compute(pts1, pts2, &error, &rmse);

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

  }

  return 0;
}
