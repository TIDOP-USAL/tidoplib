#include <stdio.h>
#include <memory>

#include <tidop/core/console.h>
#include <tidop/core/messages.h>
#include <tidop/geometry/entities/point.h>
#include <tidop/geospatial/crs.h>
#include <tidop/geospatial/crstransf.h>
#include <tidop/geospatial/ortho.h>
#include <tidop/geospatial/footprint.h>

// filesystem
#if (__cplusplus >= 201703L)
#include <filesystem>
namespace fs = std::filesystem;
#else
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#endif

using namespace tl;
using namespace geospatial;

int main(int argc, char** argv)
{

  // Consola
  Console &console = Console::instance();
  console.setTitle("");
  console.setConsoleUnicode();
  console.setFontHeight(14);
  console.setLogLevel(MessageLevel::msg_verbose);
  MessageManager::instance().addListener(&console);

  std::string dtm("C:\\Users\\esteban\\Documents\\Inspector\\Projects\\Madrigalejo\\dense\\pmvs\\models\\madrigalejo.tif");
  experimental::Camera camera;
  camera.setMake("SONY");
  camera.setModel("ILCE-6000");
  camera.setFocal(16);
  camera.setHeight(6000);
  camera.setWidth(4000);
  camera.setType("Radial");
  camera.setSensorSize(23.5);
  std::shared_ptr<experimental::Calibration> calibration = experimental::CalibrationFactory::create(camera.type());
  calibration->setParameter(tl::experimental::Calibration::Parameters::focal, 3969.51);
  calibration->setParameter(tl::experimental::Calibration::Parameters::cx, 3000);
  calibration->setParameter(tl::experimental::Calibration::Parameters::cy, 2000);
  calibration->setParameter(tl::experimental::Calibration::Parameters::k1, -0.0442599);
  calibration->setParameter(tl::experimental::Calibration::Parameters::k1, 0.0315184);
  camera.setCalibration(calibration);

  //std::string file = "C:\\Users\\esteban\\Documents\\Inspector\\Projects\\Madrigalejo\\images\\image_2020-08-04 12_37_35.jpg";
  //std::string rectifiedFile = "C:\\Users\\esteban\\Documents\\Inspector\\Projects\\Madrigalejo\\ortho\\image_2020-08-04 12_37_35.jpg";
  std::string file = "C:\\Users\\esteban\\Documents\\Inspector\\Projects\\Madrigalejo\\images\\image_2020-08-04 12_45_42.jpg";
  std::string rectifiedFile = "C:\\Users\\esteban\\Documents\\Inspector\\Projects\\Madrigalejo\\ortho\\image_2020-08-04 12_45_42.jpg";
  std::string footprint_file = "C:\\Users\\esteban\\Documents\\Inspector\\Projects\\Madrigalejo\\footprint.shp";

  math::RotationMatrix<double> rotation_matrix;
  //ori.principal_point = Point3D(-26.4568 + 272021.61, 49.9899 + 4338369.137, -0.184017 + 314.874);
  Point3D principal_point(-5.7208 + 272021.61, -17.8296 + 4338369.137, 0.166741 + 314.874);
  rotation_matrix.at(0, 0) = 0.775278;// 0.766944;
  rotation_matrix.at(0, 1) = -0.625695;// -0.640877;
  rotation_matrix.at(0, 2) = -0.0863097;// -0.0327755;
  rotation_matrix.at(1, 0) = -0.624984;// -0.637979;
  rotation_matrix.at(1, 1) = -0.779693;// -0.766987;
  rotation_matrix.at(1, 2) = 0.0383955;// 0.0686509;
  rotation_matrix.at(2, 0) = -0.091319;// -0.0691352;
  rotation_matrix.at(2, 1) = 0.024175;// -0.0317413;
  rotation_matrix.at(2, 2) = -0.995528;// -0.997102;
  
  //Orthorectification ortho(dtm, camera);
  //ortho.run(file, ori, rectifiedFile);

  experimental::Photo::Orientation orientation(principal_point, rotation_matrix);
  experimental::Photo photo(file);
  photo.setCamera(camera);
  photo.setOrientation(orientation);

  std::vector<experimental::Photo> photos;
  photos.push_back(photo);

  Footprint footprint(dtm);
  footprint.run(photos, footprint_file);

  //fs::path app_path = argv[0];
  //std::string cmd_name = app_path.stem().string();

  //std::string epsg_in;
  //std::string epsg_out;
  //std::string coord;
  //std::string separator = ";";
  //std::string coord_trf;

  //Command cmd(cmd_name, "Ejemplo de transformación de coordenadas");
  //cmd.push_back(std::make_shared<ArgumentStringRequired>("epsg_in", 'i', "Sistema de referencia de entrada", &epsg_in));
  //cmd.push_back(std::make_shared<ArgumentStringRequired>("epsg_out", 'o', "Sistema de referencia de salida", &epsg_out));
  //cmd.push_back(std::make_shared<ArgumentStringRequired>("coord", 'c', "Fichero de texto con las coordenadas separadas por comas o cadena de texto con las coordenadas de un punto", &coord));
  //cmd.push_back(std::make_shared<ArgumentStringOptional>("separator", 's', "Caracter separador de coordenadas. Por defecto ';'", &separator));
  //cmd.push_back(std::make_shared<ArgumentStringOptional>("coord_trf", 't', "Fichero de texto con las coordenadas transformadas", &coord_trf));
  //
  //cmd.addExample(cmd_name + " --epsg_in EPSG:25830 --epsg_out EPSG:4258 --coord 281815.044;4827675.243;123.35");
  //cmd.addExample(cmd_name + " -iEPSG:25830 -oEPSG:4258 --coord utm.txt");


  //Command::Status status = cmd.parse(argc, argv);
  //if (status == Command::Status::parse_error ) {
  //  return 1;
  //} else if (status == Command::Status::show_help) {
  //  return 0;
  //} else if (status == Command::Status::show_licence) {
  //  return 0;
  //} else if (status == Command::Status::show_version) {
  //  return 0;
  //}

  //// Consola
  //Console &console = Console::instance();
  //console.setTitle(cmd_name);
  //console.setConsoleUnicode();
  //console.setFontHeight(14);
  //console.setLogLevel(MessageLevel::msg_verbose);
  //MessageManager::instance().addListener(&console);

  //try {

  //  std::shared_ptr<Crs> epsgIn(new Crs(epsg_in));
  //  std::shared_ptr<Crs> epsgOut(new Crs(epsg_out));
  //  CrsTransform<Point3D> crs(epsgIn, epsgOut);
  //  
  //  std::ofstream ofs;
  //  ofs.open(coord_trf, std::ofstream::out | std::ofstream::trunc);

  //  if (fs::exists(coord)) {
  //  
  //    std::ifstream ifs;
  //    ifs.open(coord, std::ifstream::in);
  //    if (ifs.is_open()) {

  //      std::string line;
  //      while (std::getline(ifs, line)) {
  //        std::vector<double> vector;
  //        splitToNumbers(line, vector, separator.c_str());
  //        Point3D pt_in(vector[0], vector[1], vector[2]);
  //        Point3D pt_out;
  //        crs.transform(pt_in, pt_out);
  //        msgInfo("%lf;%lf;%lf -> %lf;%lf;%lf", vector[0], vector[1], vector[2], pt_out.x, pt_out.y, pt_out.z);

  //        if (ofs.is_open()) {
  //          ofs << vector[0] << separator << vector[1] << separator << vector[2] << separator << " -> "
  //              << pt_out.x << separator << pt_out.y << separator << pt_out.z << std::endl;
  //        }
  //      }
  //      ifs.close();
  //    }

  //  } else {
  //    std::vector<double> point;
  //    splitToNumbers(coord, point, separator.c_str());
  //    Point3D pt_in(point[0], point[1], point[2]);
  //    Point3D pt_out;
  //    crs.transform(pt_in, pt_out);
  //    msgInfo("%lf;%lf;%lf -> %lf;%lf;%lf", point[0], point[1], point[2], pt_out.x, pt_out.y, pt_out.z);

  //    if (ofs.is_open()) {
  //      ofs << point[0] << separator << point[1] << separator << point[2] << separator << " -> "
  //          << pt_out.x << separator << pt_out.y << separator << pt_out.z << std::endl;
  //    }

  //  }

  //  if (ofs.is_open()) ofs.close();

  //} catch (const std::exception &e) {
  //  msgError(e.what());
  //} 

  return 0;
}
