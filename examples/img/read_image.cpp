#include <stdio.h>

// Cabeceras tidopLib
#include <tidop/core/console.h>
#include <tidop/core/messages.h>
#include <tidop/img/imgreader.h>
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

  Command cmd("read_image", "Lectura de una imagen");
  cmd.push_back(std::make_shared<ArgumentStringRequired>("img", 'i', "Lectura de una imagen", &img));

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
  console.setTitle("Read Image");
  console.setLogLevel(MessageLevel::msg_verbose);
  console.setConsoleUnicode();
  MessageManager::instance().addListener(&console);

  Chrono chrono("Image read");
  chrono.run();  // Se inicia el cronometro

  try {
    std::unique_ptr<ImageReader> imageReader = ImageReaderFactory::createReader(img);

    imageReader->open();
    if (imageReader->isOpen()) {

      msgInfo("Numero de bandas: %i", imageReader->channels());
      msgInfo("Profundidad de color: %i", imageReader->depth());
      msgInfo("Dimensiones de la imagen: %ix%i", imageReader->cols(), imageReader->rows());
      
      std::shared_ptr<ImageMetadata> image_metadata = imageReader->metadata();
      std::map<std::string, std::string> metadata = image_metadata->activeMetadata();
      std::string name;
      std::string value;
      msgInfo("Metadatos:");
      for (auto it = metadata.begin(); it != metadata.end(); it++) {
        name = it->first;
        value = it->second;
        msgInfo("  %s: %s", name.c_str(), value.c_str());
      }

      cv::Mat bmp = imageReader->read(RectI(100,100,500,500));

      imageReader->close();
    } else {
      msgError("Error al abrir la imagen: %s", img.c_str());
    }
  } catch (const std::exception &e) {
    msgError(e.what());
  }

  chrono.stop();

  return 0;
}
