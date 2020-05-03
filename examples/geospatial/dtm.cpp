
/* Cabeceras TidopLib */
#include <tidop/core/console.h>
#include <tidop/core/messages.h>
#include <tidop/geometry/entities/point.h>
#include <tidop/geospatial/dtm.h>
#include <tidop/img/imgio.h>


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
  console.setTitle(cmd_name.c_str());                           // Titulo de la ventana de consola
  console.setLogLevel(MessageLevel::msg_verbose);
  MessageManager::instance().addListener(&console);


  fs::path cloud_path;
  fs::path dtm_path;
  size_t id = 0;
  std::vector<std::string> interpolation_list {
    "LINEAR",
    "NEAREST"
  };

  Command cmd(cmd_name, "Generación de un MDT a partir de una nube de puntos");
  cmd.push_back(std::make_shared<ArgumentPathRequired>("cloud_path", "Fichero nube de puntos", &cloud_path));
  cmd.push_back(std::make_shared<ArgumentPathRequired>("dtm_path",  "Modelo digital del terreno", &dtm_path));
  cmd.push_back(std::make_shared<ArgumentListStringOptional>('i', "Modo de interpolación", interpolation_list, &id));


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

  Chrono chrono("Lectura de nube de puntos");
  chrono.run();
  /// Lectura de la nube de puntos
  //std::vector<Point3D> in;
  MultiPoint3D<Point3D> cloud_point;
  std::ifstream read_handler(cloud_path.string().c_str(), std::ifstream::in);
  if (read_handler.is_open()) {
    std::string aux;
    read_handler >> aux; // Cabecera
    while (!read_handler.eof()){
      read_handler >> aux;
      std::vector<double> coord;
      splitToNumbers(aux, coord);
      cloud_point.push_back(Point3D(coord[0], coord[1], coord[2]));
    }
  }


  chrono.stop();
  chrono.reset();

  /// Se divide la ventana en trozos mas pequeños

  double resolution = 0.02; /// Resolución 2 micras
  BoundingBox<Point3D> box = cloud_point.boundingBox();
  double width_mm = box.width();
  double height_mm = box.height();
  //double depth_mm = box.depth();

  int width_px = TL_ROUND_TO_INT(width_mm / resolution);
  int height_px = TL_ROUND_TO_INT(height_mm / resolution);
  /// Si la imagen de salida es muy grande hay que separarla en bloques
  int max_img_block = 1048576;
  int block = max_img_block / width_px;
  if (block < 1)
    block = 1;
  int dh_img = height_px / block;

  std::vector<WindowI> mWindowImage(static_cast<size_t>(block));
  for(int r = 0; r < block; r++){
    mWindowImage[static_cast<size_t>(r)].pt1.x = 0;
    mWindowImage[static_cast<size_t>(r)].pt1.y = r * dh_img;
    mWindowImage[static_cast<size_t>(r)].pt2.x = width_px;
    if (r == block - 1) {
      mWindowImage[static_cast<size_t>(r)].pt2.y = height_px;
    } else {
      mWindowImage[static_cast<size_t>(r)].pt2.y = mWindowImage[static_cast<size_t>(r)].pt1.y + dh_img - 1;
    }
  }

  /// Se crean las subventanas de la nube de puntos

  chrono.setMessage("Se crean las subventanas de la nube de puntos");
  chrono.run();

  size_t n_points = cloud_point.size();
  size_t max_points_block = 100000;  /// Hacer pruebas y medir tiempos

  size_t cloud_blocks = n_points > max_points_block*1.5 ? n_points / max_points_block : 1;
  double scale_area = (width_mm * height_mm) / cloud_blocks;
  double scale_lineal = sqrt(scale_area);
  size_t block_cols = static_cast<size_t>(width_mm / scale_lineal);
  size_t block_rows = static_cast<size_t>(height_mm / scale_lineal);

  std::vector<std::vector<MultiPoint3D<Point3D>>> mGrid(block_rows, std::vector<MultiPoint3D<Point3D>>(block_cols));
  std::vector<std::vector<WindowD>> mWindow(block_rows, std::vector<WindowD>(block_cols));

  for(size_t r = 0; r < block_rows; r++){
    for(size_t c = 0; c < block_cols; c++){
      BoundingBox<Point3D> box2;
      box2.pt1.x = box.pt1.x + c * scale_lineal;
      box2.pt1.y = box.pt1.y + r * scale_lineal;
      box2.pt1.z = box.pt1.z;
      box2.pt2.x = box.pt1.x + (c+1) * scale_lineal;
      box2.pt2.y = box.pt1.y + (r+1) * scale_lineal;
      box2.pt2.z = box.pt2.z;
      //mGrid[r][c] = cloud_point.entitiesInBox(box2);
      mWindow[r][c] = WindowD(static_cast<PointD>(box2.pt1), static_cast<PointD>(box2.pt2));
    }
  }

  chrono.stop();
  chrono.reset();


  chrono.setMessage("segmentación de la nube de puntos");
  chrono.run();

  for (size_t i = 0; i < cloud_point.size(); i++) {
    bool bFind = false;
    for(size_t r = 0; r < block_rows; r++){
      for(size_t c = 0; c < block_cols; c++){
        if (mWindow[r][c].containsPoint(static_cast<PointD>(cloud_point[i]))){
          mGrid[r][c].push_back(cloud_point[i]);
          bFind = true;
          break;
        }
      }
      if (bFind) break;
    }
  }

  cloud_point.clear();

  chrono.stop();

  RasterGraphics dtm;
  dtm.open(dtm_path.string().c_str(), RasterGraphics::Mode::create);
  dtm.create(height_px, width_px, 1, DataType::TL_32F);

  Affine<PointD> trf(box.pt1.x, box.pt1.y, resolution, resolution, 0.0);

  PointD pt_dtm;
  PointD pt_cloud;
  for(size_t r = 0; r < static_cast<size_t>(height_px); r++){
    pt_dtm.y = static_cast<double>(r);
    for(size_t c = 0; c <static_cast<size_t>(width_px); c++){
      pt_dtm.x = static_cast<double>(c);
      trf.transform(pt_dtm, &pt_cloud);

      if (mWindow[r][c].containsPoint(pt_cloud)){

      }
    }
  }

  return 0;
}
