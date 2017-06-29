#include <vector>
#include <array>

#include <Eigen/SVD>

#include "inscribedCircle.h"


// Cabeceras tidopLib
//#include "core/console.h"
//#include "core/messages.h"
//#include "img/imgio.h"

#include "opencv2/core/core.hpp"
//#include "geometric_entities/bbox.h"
//#include "geometric_entities/lineString.h"
//#include "geometric_entities/polygon.h"

//using namespace I3D;
//using namespace I3D::geometry;




void solveSVD(int nRows, int nCols, double *a, double *b, double *c)
{
  Eigen::MatrixXd A = Eigen::Map<Eigen::MatrixXd>(a, nCols, nRows);
  Eigen::VectorXd B = Eigen::Map<Eigen::VectorXd>(b, nRows);
  Eigen::VectorXd C = A.transpose().jacobiSvd(Eigen::ComputeFullU | Eigen::ComputeFullV).solve(B);
  std::memcpy(c, C.data(), nCols*sizeof(double));
}



int main(int argc, char** argv)
{
  //std::vector<cv::Point3d> vertex {
  //  cv::Point3d(-2.8728270531, 4.2343444824, -0.6537694335),
  //  cv::Point3d(-2.8656525612, 4.2345046997, -0.6240240932),
  //  cv::Point3d(-2.8581471443, 4.2335939407, -0.5940701962),
  //  cv::Point3d(-2.8550608158, 4.2323269844, -0.5827135444),
  //  cv::Point3d(-2.8490288258, 4.2204918861, -0.5706148148),
  //  cv::Point3d(-2.8400762081, 4.2021245956, -0.5535204411),
  //  cv::Point3d(-2.8349173069, 4.1911664009, -0.5440745354),
  //  cv::Point3d(-2.8207349777, 4.1592531204, -0.5200322866),
  //  cv::Point3d(-2.8182916641, 4.1536045074, -0.5160524845),
  //  cv::Point3d(-2.8157482147, 4.1438803673, -0.5160569549),
  //  cv::Point3d(-2.8180198669, 4.1341729164, -0.5358891487),
  //  cv::Point3d(-2.8244142532, 4.1259770393, -0.5710852742),
  //  cv::Point3d(-2.8840534687, 4.0826535225, -0.8636367321),
  //  cv::Point3d(-2.8907699585, 4.0822782516, -0.8917269707),
  //  cv::Point3d(-2.8906064034, 4.1782650948, -0.7875338197),
  //  cv::Point3d(-2.8890004158, 4.2041006088, -0.7530517578),
  //  cv::Point3d(-2.8867816925, 4.2140669823, -0.7331582308),
  //  cv::Point3d(-2.8821890354, 4.2241053581, -0.7034013271),
  //  cv::Point3d(-2.8747916222, 4.2333650589, -0.6629241109)
  //};

  std::vector<pcl::PointXYZ> vertex {
    pcl::PointXYZ(-2.8728270531, 4.2343444824, -0.6537694335),
    pcl::PointXYZ(-2.8656525612, 4.2345046997, -0.6240240932),
    pcl::PointXYZ(-2.8581471443, 4.2335939407, -0.5940701962),
    pcl::PointXYZ(-2.8550608158, 4.2323269844, -0.5827135444),
    pcl::PointXYZ(-2.8490288258, 4.2204918861, -0.5706148148),
    pcl::PointXYZ(-2.8400762081, 4.2021245956, -0.5535204411),
    pcl::PointXYZ(-2.8349173069, 4.1911664009, -0.5440745354),
    pcl::PointXYZ(-2.8207349777, 4.1592531204, -0.5200322866),
    pcl::PointXYZ(-2.8182916641, 4.1536045074, -0.5160524845),
    pcl::PointXYZ(-2.8157482147, 4.1438803673, -0.5160569549),
    pcl::PointXYZ(-2.8180198669, 4.1341729164, -0.5358891487),
    pcl::PointXYZ(-2.8244142532, 4.1259770393, -0.5710852742),
    pcl::PointXYZ(-2.8840534687, 4.0826535225, -0.8636367321),
    pcl::PointXYZ(-2.8907699585, 4.0822782516, -0.8917269707),
    pcl::PointXYZ(-2.8906064034, 4.1782650948, -0.7875338197),
    pcl::PointXYZ(-2.8890004158, 4.2041006088, -0.7530517578),
    pcl::PointXYZ(-2.8867816925, 4.2140669823, -0.7331582308),
    pcl::PointXYZ(-2.8821890354, 4.2241053581, -0.7034013271),
    pcl::PointXYZ(-2.8747916222, 4.2333650589, -0.6629241109)
  };

  //cv::Point3d center3D;
  pcl::PointXYZ center3D;
  
  poleOfInaccessibility3D(vertex.begin(), vertex.end(), &center3D);
  double _radius = distPointToPolygon(center3D, vertex.begin(), vertex.end());

  exit(EXIT_SUCCESS);
}



//int main(int argc, char** argv)
//{
//
//
//  //char name[I3D_MAX_FNAME];
//  //getFileName(getRunfile(), name, I3D_MAX_FNAME);
//
//  //CmdParser cmdParser(name, "");
//  //cmdParser.addParameter("points", "Fichero de puntos");
//  //CmdParser::Status status = cmdParser.parse(argc, argv);
//  //if (status == CmdParser::Status::PARSE_ERROR ) {
//  //  exit(EXIT_FAILURE);
//  //} else if (status == CmdParser::Status::PARSE_HELP) {
//  //  exit(EXIT_SUCCESS);
//  //}
//
//  //std::string points = cmdParser.getValue<Path>("points").toString();
//
//
//  //// Fichero de log
//  //Log &log = Log::getInstance();
//  ////Configuración de log y mensajes por consola
//  //char logfile[I3D_MAX_PATH];
//  //if (changeFileExtension(getRunfile(), "log", logfile, I3D_MAX_PATH) == 0) {
//  //  log.setLogFile(logfile);
//  //}
//  //log.setLogLevel(MessageLevel::MSG_INFO);
//
//  // Consola
//  Console console;
//  console.setLogLevel(MessageLevel::MSG_VERBOSE);
//  console.setConsoleUnicode();
//
//  //Configuración de mensajes
//  MessageManager &msg_h = MessageManager::getInstance();
//  //msg_h.addListener(&log);
//  msg_h.addListener(&console);
//
//
//
//
//
//  //geometry::Polygon<PointI> polygon{
//  //  PointI(100, 144),
//  //  PointI(157, 93),
//  //  PointI(245, 83),
//  //  PointI(333, 56),
//  //  PointI(399, 82),
//  //  PointI(457, 117),
//  //  PointI(465, 158),
//  //  PointI(433, 225),
//  //  PointI(369, 235),
//  //  PointI(242, 264),
//  //  PointI(171, 227),
//  //  PointI(118, 206) 
//  //};
//
//  //std::vector<cv::Point> points{
//  //  cv::Point(100, 144),
//  //  cv::Point(157, 93),
//  //  cv::Point(245, 83),
//  //  cv::Point(333, 56),
//  //  cv::Point(399, 82),
//  //  cv::Point(457, 117),
//  //  cv::Point(465, 158),
//  //  cv::Point(433, 225),
//  //  cv::Point(369, 235),
//  //  cv::Point(242, 264),
//  //  cv::Point(171, 227),
//  //  cv::Point(118, 206)
//  //};
//
//  //PointI center;
//  //poleOfInaccessibility(polygon, &center);
//  //double radius = distPointToPolygon(center, polygon);
//
//  std::vector<cv::Point> points{
//    cv::Point(12.7173,	723.4344),
//    cv::Point(15.0836,	725.3743),
//    cv::Point(17.5414,	727.246 ),
//    cv::Point(18.5374,	727.8854),
//    cv::Point(20.3303,	727.7669),
//    cv::Point(22.9782,	727.474 ),
//    cv::Point(24.4978,	727.2503),
//    cv::Point(28.6463,	726.3747),
//    cv::Point(29.3584,	726.2018),
//    cv::Point(30.0367,	725.4599),
//    cv::Point(29.1288,	723.4344),
//    cv::Point(26.8875,	720.5285),
//    cv::Point(6.52680,	698.2653),
//    cv::Point(4.3078,	696.4163),
//    cv::Point(5.93840,	710.4889),
//    cv::Point(6.89190,	714.6938),
//    cv::Point(7.78660,	716.7431),
//    cv::Point(9.4646,	719.4371),
//    cv::Point(12.0538,	722.7664)
//  };
//
//  geometry::Polygon<PointD> polygon{
//    PointD(-2.872827, 4.234344),
//    PointD(-2.849164, 4.253743),
//    PointD(-2.824586, 4.272460),
//    PointD(-2.814626, 4.278854),
//    PointD(-2.796697, 4.277669),
//    PointD(-2.770218, 4.274740),
//    PointD(-2.755022, 4.272503),
//    PointD(-2.713537, 4.263747),
//    PointD(-2.706416, 4.262018),
//    PointD(-2.699633, 4.254599),
//    PointD(-2.708712, 4.234344),
//    PointD(-2.731125, 4.205285),
//    PointD(-2.934732, 3.982653),
//    PointD(-2.956922, 3.964163),
//    PointD(-2.940616, 4.104889),
//    PointD(-2.931081, 4.146938),
//    PointD(-2.922134, 4.167431),
//    PointD(-2.905354, 4.194371),
//    PointD(-2.879462, 4.227664)
//  };
//  
//  PointD center;
//  poleOfInaccessibility(polygon, &center);
//  double radius = distPointToPolygon(center, polygon);
//
//  //poleOfInaccessibility2D(polygon.begin(), polygon.end(), &center);
//
//  //double radius = I3D_DOUBLE_MAX;
//  //double dist;
//  //for (size_t i = 0; i < polygon.size(); i++) {
//  //  if (i == polygon.size() - 1) {
//  //    dist = distPointToSegment(center, Segment<PointD>(polygon[i], polygon[0]));
//  //  } else {
//  //    dist = distPointToSegment(center, Segment<PointD>(polygon[i], polygon[i+1]));
//  //  }
//  //  if (dist < radius) radius = dist;
//  //}
//
//  //PointI center2;
//  //_poleOfInaccessibility(points, &center2);
//  //double radius2 = distPointToPolygon(center2, polygon);
//
//  cv::Mat canvas = cv::Mat::zeros(1000, 1000, CV_8U);
//
//  cv::Mat aux(points);
//  const cv::Point *pts = (const cv::Point*) aux.data;
//  int npts = aux.rows;
//  cv::fillPoly(canvas, &pts, &npts, 1, cv::Scalar(255, 255, 255) );
//  //cv::Mat m_out;
//  //cv::distanceTransform(canvas, m_out, DIST_L2, 5);
//  //double max_val;
//  //cv::Point center;
//
//  //cv::minMaxLoc(m_out, NULL, &max_val, NULL, &center);
//  cv::Mat canvas_rgb;
//  cvtColor(canvas, canvas_rgb, CV_GRAY2BGR);
//  cv::line(canvas_rgb, center, center, cv::Scalar(0, 0, 255), 2);
//
//  //double radius = I3D_DOUBLE_MAX;
//  //double dist;
//  //for (int i = 0; i < points.size(); i++) {
//  //  if (i == points.size() - 1) {
//  //    dist = distPointToSegment((PointI &)center, SegmentI((PointI &)points[i], (PointI &)points[0]));
//  //  } else {
//  //    dist = distPointToSegment((PointI &)center, SegmentI((PointI &)points[i], (PointI &)points[i+1]));
//  //  }
//  //  if (dist < radius) radius = dist;
//  //}
//
//    
//  cv::circle(canvas_rgb, center, static_cast<int>(radius), cv::Scalar(255, 0, 0), 2);
//
//  // Lo mismo en 3d
//
//  Polygon3D<Point3D> polygon3d {
//    Point3D(-2.8728270531, 4.2343444824, -0.6537694335),
//    Point3D(-2.8656525612, 4.2345046997, -0.6240240932),
//    Point3D(-2.8581471443, 4.2335939407, -0.5940701962),
//    Point3D(-2.8550608158, 4.2323269844, -0.5827135444),
//    Point3D(-2.8490288258, 4.2204918861, -0.5706148148),
//    Point3D(-2.8400762081, 4.2021245956, -0.5535204411),
//    Point3D(-2.8349173069, 4.1911664009, -0.5440745354),
//    Point3D(-2.8207349777, 4.1592531204, -0.5200322866),
//    Point3D(-2.8182916641, 4.1536045074, -0.5160524845),
//    Point3D(-2.8157482147, 4.1438803673, -0.5160569549),
//    Point3D(-2.8180198669, 4.1341729164, -0.5358891487),
//    Point3D(-2.8244142532, 4.1259770393, -0.5710852742),
//    Point3D(-2.8840534687, 4.0826535225, -0.8636367321),
//    Point3D(-2.8907699585, 4.0822782516, -0.8917269707),
//    Point3D(-2.8906064034, 4.1782650948, -0.7875338197),
//    Point3D(-2.8890004158, 4.2041006088, -0.7530517578),
//    Point3D(-2.8867816925, 4.2140669823, -0.7331582308),
//    Point3D(-2.8821890354, 4.2241053581, -0.7034013271),
//    Point3D(-2.8747916222, 4.2333650589, -0.6629241109)
//  };
//
//
//  
//  //std::array<double, 4> plane;
//  //double N = nPointsPlaneLS(polygon3d.getPoints(), plane);
//
//  //// Proyectar polilinea en plano
//  //Polygon3D<double> poligon2;
//  //for (int i = 0; i < polygon3d.getSize(); i++) {
//  //  Point3<double> pt;
//  //  projectPointToPlane(polygon3d[i], plane, &pt);
//  //  poligon2.add(pt);
//  //}
//
//
//  //Point3D center3d;
//  //poleOfInaccessibility(polygon3d, &center3d);
//
//  std::vector<Point3D> vertex {
//    Point3D(-2.8728270531, 4.2343444824, -0.6537694335),
//    Point3D(-2.8656525612, 4.2345046997, -0.6240240932),
//    Point3D(-2.8581471443, 4.2335939407, -0.5940701962),
//    Point3D(-2.8550608158, 4.2323269844, -0.5827135444),
//    Point3D(-2.8490288258, 4.2204918861, -0.5706148148),
//    Point3D(-2.8400762081, 4.2021245956, -0.5535204411),
//    Point3D(-2.8349173069, 4.1911664009, -0.5440745354),
//    Point3D(-2.8207349777, 4.1592531204, -0.5200322866),
//    Point3D(-2.8182916641, 4.1536045074, -0.5160524845),
//    Point3D(-2.8157482147, 4.1438803673, -0.5160569549),
//    Point3D(-2.8180198669, 4.1341729164, -0.5358891487),
//    Point3D(-2.8244142532, 4.1259770393, -0.5710852742),
//    Point3D(-2.8840534687, 4.0826535225, -0.8636367321),
//    Point3D(-2.8907699585, 4.0822782516, -0.8917269707),
//    Point3D(-2.8906064034, 4.1782650948, -0.7875338197),
//    Point3D(-2.8890004158, 4.2041006088, -0.7530517578),
//    Point3D(-2.8867816925, 4.2140669823, -0.7331582308),
//    Point3D(-2.8821890354, 4.2241053581, -0.7034013271),
//    Point3D(-2.8747916222, 4.2333650589, -0.6629241109)
//  };
//
//  Point3D _center3d;
//  //poleOfInaccessibility(vertex.begin(), vertex.end(), &_center3d);
//
//  //std::vector<Point3D> vertex {
//  //  Point3D(0.02, 1000.0, 71.9),
//  //  Point3D(0.02, 1100.0, 63.1),
//  //  Point3D(0.02, 1200.0, 53.8),
//  //  Point3D(0.02, 1300.0, 45.3),
//  //  Point3D(0.10, 1000.0, 74.6),
//  //  Point3D(0.10, 1100.0, 65.5),
//  //  Point3D(0.18, 1200.0, 59.2),
//  //  Point3D(0.18, 1300.0, 49.9)
//  //};
//
//  //std::array<double, 4> plane;
//  //double N = nPointsPlaneLS(vertex, plane);
//  //double kk = plane[0] / plane[2];
//  //double N2 = sqrt(31.15*31.15 + -0.08*-0.08 + -1*-1);
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
//
//
//
//
//
//  //Resolución del problema proyectando los puntos en un plano horizontal y trabajando en 2D
//  
//  //TODO: sacar a función
//  std::array<Point3D, 2> box;
//  box[0].x = box[0].y = box[0].z = std::numeric_limits<double>().max();
//  box[1].x = box[1].y = box[1].z = -std::numeric_limits<double>().max();
//  auto temp = vertex.begin(); 
//  while (temp != vertex.end()) {
//    if (box[0].x > temp->x) box[0].x = temp->x;
//    if (box[0].y > temp->y) box[0].y = temp->y;
//    if (box[0].z > temp->z) box[0].z = temp->z;
//    if (box[1].x < temp->x) box[1].x = temp->x;
//    if (box[1].y < temp->y) box[1].y = temp->y;
//    if (box[1].z < temp->z) box[1].z = temp->z;
//    *temp++;
//  }
// 
//  //Point3D point_tmp;
//  
//  //Point3D tmp;
//  
//  // Se determina el plano de mejor ajuste
//  std::array<double, 4> plane;
//  nPointsPlaneLS(vertex.begin(), vertex.end(), plane, true);
//
//  // Máxima distancia de los puntos al plano
//  double max_dist = I3D_DOUBLE_MIN;
//
//  temp = vertex.begin();
//  while (temp != vertex.end()) {
//    Point3D pt;
//    double distance = distantePointToPlane(*temp, plane);
//    if (distance > max_dist) max_dist = distance;
//    *temp++;
//  }
//
//
//  // Proyectar polilinea en el plano de mejor ajuste
//  std::vector<Point3D> poligon_plane;
//  temp = vertex.begin();;
//  while (temp != vertex.end()) {
//    Point3D pt;
//    projectPointToPlane(*temp, plane, &pt);
//    msgInfo("%f %f %f", pt.x, pt.y, pt.z);
//    poligon_plane.push_back(pt);
//    *temp++;
//  }
//
//  // Se toma como pia el centro de la caja envolvente
//  Point3D pia((box[0].x + box[1].x) / 2., (box[0].y + box[1].y) / 2., (box[0].z + box[1].z) / 2.);
//
//  // Plano horizontal (1,1,0)
//  std::array<double, 4> plane_z;
//  plane_z[0] = 0;
//  plane_z[1] = 0;
//  plane_z[2] = 1;
//  plane_z[3] = -plane_z[0] * pia.x - plane_z[1] * pia.y - plane_z[2] * pia.z;
//
//  geometry::Polygon<PointD> poligon_z;
//  //std::vector<PointD> poligon_z;
//
//  for (int i = 0; i < poligon_plane.size(); i++){
//    Point3D pt;
//    projectPointToPlane(poligon_plane[i], plane_z, &pt);
//    //msgInfo("%f %f %f", pt.x, pt.y, pt.z);
//    poligon_z.add(PointD(pt.x, pt.y));
//  }
//
//  PointD center2D;
//
//  poleOfInaccessibility(poligon_z, &center2D);
//  
//  Point3D _center3D(center2D.x, center2D.y, pia.z);
//
//
//  ////Proyectar el punto al revés
//  Point3D center3D;
//  projectPointToPlane(_center3D, plane_z, &center3D, Point3D(plane[0], plane[1], plane[2]));
//
//  poleOfInaccessibility3D(vertex.begin(), vertex.end(), &center3D);
//  double _radius = distPointToPolygon(center3D, polygon3d);
//
//  exit(EXIT_SUCCESS);
//}
