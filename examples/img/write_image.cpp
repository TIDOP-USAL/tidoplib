#include <stdio.h>

// Cabeceras tidopLib
#include <tidop/core/console.h>
#include <tidop/core/messages.h>
#include <tidop/img/imgwriter.h>
#include <tidop/img/metadata.h>

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

  std::string img;
  int rows;
  int cols;
  int channels;

  Command cmd("write_image", "Escritura de una imagen aleatoria");
  cmd.push_back(std::make_shared<ArgumentStringRequired>("img", 'i', "ruta de la imagen", &img));
  cmd.push_back(std::make_shared<ArgumentIntegerRequired>("rows", 'r', "Filas de la image", &rows));
  cmd.push_back(std::make_shared<ArgumentIntegerRequired>("cols", 'c', "Columnas de la image", &cols));
  cmd.push_back(std::make_shared<ArgumentIntegerRequired>("bands", 'b', "Número de bandas o canales", &channels));

  // Parseo de los argumentos y comprobación de los mismos
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

  // Consola
  Console &console = Console::instance();
  console.setTitle("Write Image");
  console.setLogLevel(MessageLevel::msg_verbose);
  console.setConsoleUnicode();
  MessageManager::instance().addListener(&console);

  try {
    std::unique_ptr<ImageWriter> imageWriter = ImageWriterFactory::createWriter(img);
    imageWriter->open();
    imageWriter->create(rows, cols, channels, DataType::TL_8U);
    if (imageWriter->isOpen()) {

      msgInfo("Numero de bandas: %i", imageWriter->channels());
      msgInfo("Profundidad de color: %i", imageWriter->depth());
      msgInfo("Dimensiones de la imagen: %ix%i", imageWriter->cols(), imageWriter->rows());
      
      cv::Mat bmp(rows, cols, CV_MAKETYPE(CV_8U, channels));
      double low = -500.0;
      double high = +500.0;
      cv::randu(bmp, cv::Scalar(low), cv::Scalar(high));
      imageWriter->write(bmp, WindowI(PointI(20,20), PointI(70,70)));
      cv::Rect
      imageWriter->close();
    } else {
      msgError("Error al escribir la imagen: %s", img.c_str());
    }
  } catch (const std::exception &e) {
    msgError(e.what());
  }

  return 0;
}
