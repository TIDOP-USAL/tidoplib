#include <stdio.h>
#include <memory>
#include <iomanip>

#include <tidop/core/console.h>
#include <tidop/core/messages.h>
#include <tidop/geospatial/crs.h>
#include <tidop/img/imgreader.h>
#include <tidop/vect/vectwriter.h>
#include <tidop/graphic/layer.h>
#include <tidop/graphic/entities/polygon.h>
#include <tidop/experimental/datamodel.h>
#include <tidop/experimental/camera.h>
#include <tidop/experimental/photo.h>
#include <tidop/geospatial/footprint.h>
#include <tidop/geospatial/ortho.h>

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

  fs::path app_path = argv[0];
  std::string cmd_name = app_path.stem().string();

  // Consola
  Console &console = Console::instance();
  console.setTitle(cmd_name);
  console.setConsoleUnicode();
  console.setFontHeight(14);
  console.setLogLevel(MessageLevel::msg_verbose);
  MessageManager::instance().addListener(&console);

  std::string bundle_file;
  std::string image_list;
  std::string image_path;
  std::string crs;
  std::string mdt;
  std::string ortho_path;

  Command cmd(cmd_name, "Huella de vuelo");
  cmd.push_back(std::make_shared<ArgumentStringRequired>("bundle_file", 'b', "Fichero bundle", &bundle_file));
  cmd.push_back(std::make_shared<ArgumentStringRequired>("image_list", 'i', "Listado de imagenes", &image_list));
  cmd.push_back(std::make_shared<ArgumentStringOptional>("image_path", 'p', "Ruta de imagenes", &image_path));
  cmd.push_back(std::make_shared<ArgumentStringRequired>("crs", 'c', "Código EPSG", &crs));
  cmd.push_back(std::make_shared<ArgumentStringRequired>("mdt", 'm', "Modelo digital del terreno", &mdt));
  cmd.push_back(std::make_shared<ArgumentStringRequired>("ortho_path", 'o', "Ruta ortofotos", &ortho_path));

  cmd.addExample(cmd_name + " --bundle_file bundle.rd.out --image_list bundle.rd.out.list.txt --crs EPSG:25830 -- mdt mdt.tif");

  Command::Status status = cmd.parse(argc, argv);
  if (status == Command::Status::parse_error ) {
    return 1;
  } else if (status == Command::Status::show_help) {
    return 0;
  } else if (status == Command::Status::show_licence) {
    return 0;
  } else if (status == Command::Status::show_version) {
    return 0;
  }

  try {

    /// Carga de imagenes 

    if (!fs::exists(image_list)) throw std::exception("Image list not found");

    std::vector<std::string> images;

    std::ifstream ifs;
    ifs.open(image_list, std::ifstream::in);
    if (ifs.is_open()) {

      std::string line;

      while (std::getline(ifs, line)) {
        
        if (fs::exists(line)) {
          images.push_back(line);
        } else {
          std::string image = std::string(image_path).append("\\").append(line);
          
          if (fs::exists(image)) {
            images.push_back(image);
          } else {
            std::string err = "Image not found: ";
            err.append(image);
            throw std::exception(err.c_str());
          }

        }
        

      }

      ifs.close();
    }

    /// Fin carga de imagenes 

    std::vector<experimental::Photo> photos;
    

    /// Lectura de fichero bundle

    std::unique_ptr<ImageReader> imageReader;

    if (!fs::exists(bundle_file)) throw std::exception("Bundle file not found");

    ifs.open(bundle_file, std::ifstream::in);
    if (ifs.is_open()) {
    
      std::string line;
      std::getline(ifs, line); // Salto primera linea

      std::getline(ifs, line);

      std::stringstream ss(line);

      int camera_count;
      int feature_count;
      ss >> camera_count >> feature_count;

      TL_ASSERT(camera_count == images.size(), "ERROR");

      for (size_t i = 0; i < camera_count; i++) {
      //while (std::getline(ifs, line)) {
        
        imageReader = ImageReaderFactory::createReader(images[i]);
        imageReader->open();
        int width = 0;
        int height = 0;
        if (imageReader->isOpen()) {
          height = imageReader->rows();
          width = imageReader->cols();
        }


        std::getline(ifs, line);
        ss.str(line);
        ss.clear();

        double focal;
        double k1;
        double k2;
        ss >> focal >> k1 >> k2;

        TL_TODO("¿Necesito algo de Camera o sólo de Calibration?")
        experimental::Camera camera;
        //camera.setMake("SONY");
        //camera.setModel("ILCE-6000");
        //camera.setFocal(16);
        camera.setHeight(height);
        camera.setWidth(width);
        camera.setType("Radial");
        //camera.setSensorSize(23.5);
        std::shared_ptr<experimental::Calibration> calibration = experimental::CalibrationFactory::create(camera.type());
        calibration->setParameter(tl::experimental::Calibration::Parameters::focal, focal);
        calibration->setParameter(tl::experimental::Calibration::Parameters::cx, width / 2);        
        calibration->setParameter(tl::experimental::Calibration::Parameters::cy, height / 2);
        calibration->setParameter(tl::experimental::Calibration::Parameters::k1, k1);
        calibration->setParameter(tl::experimental::Calibration::Parameters::k2, k2);
        camera.setCalibration(calibration);

        std::getline(ifs, line);
        ss.str(line);
        ss.clear();
        double r00;
        double r01;
        double r02;
        ss >> r00 >> r01 >> r02;
        std::getline(ifs, line);
        ss.str(line);
        ss.clear();
        double r10;
        double r11;
        double r12;
        ss >> r10 >> r11 >> r12;
        std::getline(ifs, line);
        ss.str(line);
        ss.clear();
        double r20;
        double r21;
        double r22;
        ss >> r20 >> r21 >> r22;

        math::RotationMatrix<double> rotation_matrix;
        rotation_matrix.at(0, 0) = r00;
        rotation_matrix.at(0, 1) = r01;
        rotation_matrix.at(0, 2) = r02;
        rotation_matrix.at(1, 0) = r10;
        rotation_matrix.at(1, 1) = r11;
        rotation_matrix.at(1, 2) = r12;
        rotation_matrix.at(2, 0) = r20;
        rotation_matrix.at(2, 1) = r21;
        rotation_matrix.at(2, 2) = r22;

        std::getline(ifs, line);
        ss.str(line);
        ss.clear();
        double tx;
        double ty;
        double tz;
        ss >> tx >> ty >> tz;
        
        //Point3D principal_point(tx, ty, tz);
        //Point3D principal_point(-5.7208 + 272021.61, -17.8296 + 4338369.137, 0.166741 + 314.874);
        Point3D principal_point;

        // Paso de la transformación de mundo a imagen a imagen mundo

        math::RotationMatrix<double> rotation_transpose = rotation_matrix.transpose();

        principal_point.x = -(rotation_transpose.at(0, 0) * tx +
                              rotation_transpose.at(0, 1) * ty +
                              rotation_transpose.at(0, 2) * tz) + 272021.61;
        principal_point.y = -(rotation_transpose.at(1, 0) * tx +
                              rotation_transpose.at(1, 1) * ty +
                              rotation_transpose.at(1, 2) * tz) + 4338369.137;
        principal_point.z = -(rotation_transpose.at(2, 0) * tx +
                              rotation_transpose.at(2, 1) * ty +
                              rotation_transpose.at(2, 2) * tz) + 314.874;


        experimental::Photo::Orientation orientation(principal_point, rotation_matrix);
        experimental::Photo photo(images[i]);
        photo.setCamera(camera);
        photo.setOrientation(orientation);
        if (images[i].compare("C:\\Users\\esteban\\Documents\\Inspector\\Projects\\Madrigalejo\\images\\image_2020-08-04 12_45_42.jpg") == 0)
          photos.push_back(photo);
      }

      ifs.close();
    }

    /// Fin lectura de fichero bundle

    //Footprint footprint(mdt);
    //footprint.run(photos, footprint_file);
    
    Orthorectification ortho(mdt);
    ortho.run(photos, ortho_path);

  } catch (const std::exception &e) {
    msgError(e.what());
  } 

  return 0;
}
