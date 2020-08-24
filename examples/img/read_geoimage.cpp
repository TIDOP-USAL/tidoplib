#include <stdio.h>

// Cabeceras tidopLib
#include <tidop/core/console.h>
#include <tidop/core/messages.h>
//#include <tidop/img/imgio.h>

using namespace tl;


/*!
 * read_image: 
 *
 *
 * uso:
 */
int main(int argc, char** argv)
{

  //std::string img;

  //Command cmd("Read Image", "Lectura de una imagen con georeferencia");
  //cmd.push_back(std::make_shared<ArgumentStringRequired>("img", 'i', "Imagen georeferenciada", &img));

  //// Parseo de los argumentos y comprobación de los mismos
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
  //console.setTitle("Read Image");
  //console.setLogLevel(MessageLevel::msg_verbose);
  //console.setConsoleUnicode();
  //MessageManager::instance().addListener(&console);

  //GeoRasterGraphics geo_image;
  //if (geo_image.open(img) == GeoRasterGraphics::Status::open_ok) {
  //  msgInfo("Numero de bandas: %i", geo_image.getBands());
  //  msgInfo("Profundidad de color: %i", geo_image.getColorDepth());
  //  msgInfo("Dimensiones de la imagen: %ix%i", geo_image.getCols(), geo_image.getRows());
  //} else {
  //  msgError("Error al abrir la imagen: %s", img.c_str());
  //}

  //std::array<double, 6> geoTransform = geo_image.georeference();

  //Affine<PointD> affine;
  //affine.setParameters(-geoTransform[1],
  //                     geoTransform[2],
  //                     geoTransform[4],
  //                     geoTransform[5],
  //                     geoTransform[0],
  //                     geoTransform[3]);

  //PointD pt_out = affine.transform(PointD(376711.5, 4808209.5));

  //PointD pt_out2 = affine.transform(PointD(100, 100), transform_order::inverse);
  //geo_image.close();

  return 0;
}
