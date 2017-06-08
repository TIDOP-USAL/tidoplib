#include <stdio.h>

// Cabeceras tidopLib
#include "core/console.h"
#include "core/messages.h"
#include "img/imgio.h"

#include "opencv2/core/core.hpp"
#include "geometric_entities/bbox.h"

using namespace I3D;

/*!
 * read_image: 
 *
 *
 * uso:
 */
int main(int argc, char** argv)
{

  //char name[I3D_MAX_FNAME];
  //getFileName(getRunfile(), name, I3D_MAX_FNAME);

  //CmdParser cmdParser(name, "");
  //cmdParser.addParameter("points", "Fichero de puntos");
  //CmdParser::Status status = cmdParser.parse(argc, argv);
  //if (status == CmdParser::Status::PARSE_ERROR ) {
  //  exit(EXIT_FAILURE);
  //} else if (status == CmdParser::Status::PARSE_HELP) {
  //  exit(EXIT_SUCCESS);
  //}

  //std::string points = cmdParser.getValue<Path>("points").toString();


  //// Fichero de log
  //Log &log = Log::getInstance();
  ////Configuración de log y mensajes por consola
  //char logfile[I3D_MAX_PATH];
  //if (changeFileExtension(getRunfile(), "log", logfile, I3D_MAX_PATH) == 0) {
  //  log.setLogFile(logfile);
  //}
  //log.setLogLevel(MessageLevel::MSG_INFO);

  //// Consola
  //Console console;
  //console.setLogLevel(MessageLevel::MSG_INFO);
  //console.setConsoleUnicode();

  ////Configuración de mensajes
  //MessageManager &msg_h = MessageManager::getInstance();
  //msg_h.addListener(&log);
  //msg_h.addListener(&console);


  I3D::Polygon<int> polygon{
    PointI(100, 144),
    PointI(157, 93),
    PointI(245, 83),
    PointI(333, 56),
    PointI(399, 82),
    PointI(457, 117),
    PointI(465, 158),
    PointI(433, 225),
    PointI(369, 235),
    PointI(242, 264),
    PointI(171, 227),
    PointI(118, 206) 
  };

  std::vector<cv::Point> points{
    cv::Point(100, 144),
    cv::Point(157, 93),
    cv::Point(245, 83),
    cv::Point(333, 56),
    cv::Point(399, 82),
    cv::Point(457, 117),
    cv::Point(465, 158),
    cv::Point(433, 225),
    cv::Point(369, 235),
    cv::Point(242, 264),
    cv::Point(171, 227),
    cv::Point(118, 206)
  };

  PointI center;

  poleOfInaccessibility(polygon, &center);
  double radius = distPointToPolygon(center, polygon);

  //PointI center2;
  //_poleOfInaccessibility(points, &center2);
  //double radius2 = distPointToPolygon(center2, polygon);

  cv::Mat canvas = cv::Mat::zeros(400, 600, CV_8U);

  cv::Mat aux(points);
  const cv::Point *pts = (const cv::Point*) aux.data;
  int npts = aux.rows;
  cv::fillPoly(canvas, &pts, &npts, 1, cv::Scalar(255, 255, 255) );
  //cv::Mat m_out;
  //cv::distanceTransform(canvas, m_out, DIST_L2, 5);
  //double max_val;
  //cv::Point center;

  //cv::minMaxLoc(m_out, NULL, &max_val, NULL, &center);
  cv::Mat canvas_rgb;
  cvtColor(canvas, canvas_rgb, CV_GRAY2BGR);
  cv::line(canvas_rgb, center, center, cv::Scalar(0, 0, 255), 2); 
  cv::circle(canvas_rgb, center, radius, cv::Scalar(255, 0, 0), 2);




  // Lo mismo en 3d

  I3D::Polygon3D<double> polygon3d {
    Point3D(-2.8728270531, 4.2343444824, -0.6537694335),
    Point3D(-2.8656525612, 4.2345046997, -0.6240240932),
    Point3D(-2.8581471443, 4.2335939407, -0.5940701962),
    Point3D(-2.8550608158, 4.2323269844, -0.5827135444),
    Point3D(-2.8490288258, 4.2204918861, -0.5706148148),
    Point3D(-2.8400762081, 4.2021245956, -0.5535204411),
    Point3D(-2.8349173069, 4.1911664009, -0.5440745354),
    Point3D(-2.8207349777, 4.1592531204, -0.5200322866),
    Point3D(-2.8182916641, 4.1536045074, -0.5160524845),
    Point3D(-2.8157482147, 4.1438803673, -0.5160569549),
    Point3D(-2.8180198669, 4.1341729164, -0.5358891487),
    Point3D(-2.8244142532, 4.1259770393, -0.5710852742),
    Point3D(-2.8840534687, 4.0826535225, -0.8636367321),
    Point3D(-2.8907699585, 4.0822782516, -0.8917269707),
    Point3D(-2.8906064034, 4.1782650948, -0.7875338197),
    Point3D(-2.8890004158, 4.2041006088, -0.7530517578),
    Point3D(-2.8867816925, 4.2140669823, -0.7331582308),
    Point3D(-2.8821890354, 4.2241053581, -0.7034013271),
    Point3D(-2.8747916222, 4.2333650589, -0.6629241109)
  };

  Point3D center3d;
  poleOfInaccessibility(polygon3d, &center3d);
  double radius3d = distPointToPolygon(center3d, polygon3d);

  exit(EXIT_SUCCESS);
}
