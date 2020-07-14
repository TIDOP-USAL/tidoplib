#include <stdio.h>

// Cabeceras tidopLib
#include <tidop/core/console.h>
#include <tidop/core/messages.h>
#include <tidop/img/imgio.h>
#include <tidop/imgprocess/matching.h>

using namespace tl;

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

  //std::string img1;
  //std::string img2;

  //Command cmd("Read Image", "Lectura de una imagen");
  //cmd.push_back(std::make_shared<ArgumentStringRequired>("img1", "Imagen 1", &img1));
  //cmd.push_back(std::make_shared<ArgumentStringRequired>("img2", "Imagen 2", &img2));

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

  //RasterGraphics image1;
  //cv::Mat mat_1;
  //if (image1.open(img1, RasterGraphics::Mode::read) == RasterGraphics::Status::open_ok) {
  //  msgInfo("Numero de bandas: %i", image1.getBands());
  //  msgInfo("Profundidad de color: %i", image1.getColorDepth());
  //  msgInfo("Dimensiones de la imagen: %ix%i", image1.getCols(), image1.getRows());
  //  image1.read(&mat_1, WindowI());
  //} else {
  //  msgError("Error al abrir la imagen: %s", img1.c_str());
  //}

  //RasterGraphics image2;
  //cv::Mat mat_2;
  //if (image2.open(img2, RasterGraphics::Mode::read) == RasterGraphics::Status::open_ok) {
  //  msgInfo("Numero de bandas: %i", image2.getBands());
  //  msgInfo("Profundidad de color: %i", image2.getColorDepth());
  //  msgInfo("Dimensiones de la imagen: %ix%i", image2.getCols(), image2.getRows());
  //  image2.read(&mat_2, WindowI());
  //} else {
  //  msgError("Error al abrir la imagen: %s", img2.c_str());
  //}

  //Chrono chrono;


  //chrono.run();
  //Features2D feat1(cv::xfeatures2d::SIFT::create(), cv::xfeatures2d::SIFT::create());
  //feat1.detectKeyPoints(mat_1);
  //msgInfo("KeyPoints image 1: %i", feat1.getKeyPoints().size());
  //chrono.stop();

  //chrono.run();
  //Features2D feat2(cv::xfeatures2d::SIFT::create(), cv::xfeatures2d::SIFT::create());
  //feat2.detectKeyPoints(mat_2);
  //msgInfo("KeyPoints image 2: %i", feat2.getKeyPoints().size());
  //chrono.stop();

  //chrono.run();
  //Matching match;
  //match.match(feat1.getDescriptors(), feat2.getDescriptors());
  //msgInfo("Matching %i", match.getMatches()->size());
  //chrono.stop();

  return 0;
}
