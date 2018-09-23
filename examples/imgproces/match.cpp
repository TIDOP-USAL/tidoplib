#include <stdio.h>

// Cabeceras tidopLib
#include <tidop/core/console.h>
#include <tidop/core/messages.h>
#include <tidop/img/imgio.h>
#include <tidop/img_process/matching.h>

using namespace TL;

#ifdef HAVE_VLD
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

  std::string img1;
  std::string img2;

  Command cmd("Read Image", "Lectura de una imagen");
  cmd.push_back(std::make_shared<ArgumentStringRequired>("img1", "Imagen 1", &img1));
  cmd.push_back(std::make_shared<ArgumentStringRequired>("img2", "Imagen 2", &img2));

  // Parseo de los argumentos y comprobación de los mismos
  Command::Status status = cmd.parse(argc, argv);
  if (status == Command::Status::PARSE_ERROR ) {
    return 1;
  } else if (status == Command::Status::SHOW_HELP) {
    return 0;
  } else if (status == Command::Status::SHOW_LICENCE) {
    return 0;
  } else if (status == Command::Status::SHOW_VERSION) {
    return 0;
  }

  // Consola
  Console &console = Console::getInstance();
  console.setTitle("Read Image");
  console.setLogLevel(MessageLevel::MSG_VERBOSE);
  console.setConsoleUnicode();
  MessageManager::getInstance().addListener(&console);

  TL::RasterGraphics image1;
  cv::Mat mat_1;
  if (image1.open(img1, TL::RasterGraphics::Mode::Read) == TL::RasterGraphics::Status::OPEN_OK) {
    msgInfo("Numero de bandas: %i", image1.getBands());
    msgInfo("Profundidad de color: %i", image1.getColorDepth());
    msgInfo("Dimensiones de la imagen: %ix%i", image1.getCols(), image1.getRows());
    image1.read(&mat_1, geometry::WindowI());
  } else {
    msgError("Error al abrir la imagen: %s", img1.c_str());
  }

  TL::RasterGraphics image2;
  cv::Mat mat_2;
  if (image2.open(img2, TL::RasterGraphics::Mode::Read) == TL::RasterGraphics::Status::OPEN_OK) {
    msgInfo("Numero de bandas: %i", image2.getBands());
    msgInfo("Profundidad de color: %i", image2.getColorDepth());
    msgInfo("Dimensiones de la imagen: %ix%i", image2.getCols(), image2.getRows());
    image2.read(&mat_2, geometry::WindowI());
  } else {
    msgError("Error al abrir la imagen: %s", img2.c_str());
  }

  Chrono chrono;


  chrono.run();
  Features2D feat1(cv::xfeatures2d::SIFT::create(), cv::xfeatures2d::SIFT::create());
  feat1.detectKeyPoints(mat_1);
  msgInfo("%i", feat1.getKeyPoints().size());
  chrono.stop();

  chrono.run();
  Features2D feat2(cv::xfeatures2d::SIFT::create(), cv::xfeatures2d::SIFT::create());
  feat2.detectKeyPoints(mat_2);
  chrono.stop();

  chrono.run();
  Matching match;
  match.match(feat1.getDescriptors(), feat2.getDescriptors());
  chrono.stop();

  return 0;
}
