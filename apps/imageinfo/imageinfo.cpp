#include <stdio.h>

// Cabeceras tidopLib
#include <tidop/core/console.h>
#include <tidop/core/messages.h>
#include <tidop/img/imgreader.h>
#include <tidop/img/metadata.h>
#include <tidop/imgprocess/filters.h>

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
      //int w = imageReader->cols();
      //int h = imageReader->rows();
      //double scale = 1.;
      //if (w > h) {
      //  scale =  200. / static_cast<double>(w);
      //} else {
      //  scale = 200. / static_cast<double>(h);
      //}

      //cv::Mat bmp = imageReader->read(scale, scale);



      //cv::Mat temperatures(bmp.size(), CV_32F);

      //parallel_for(static_cast<size_t>(0), static_cast<size_t>(bmp.rows), [&](size_t row) {
      //
      //  int r = static_cast<int>(row);
      //  const ushort *rgb_ptr = bmp.ptr<ushort>(r);
      //
      //  for (int c = 0; c < bmp.cols; c++) {
      //    int temp = static_cast<int>(rgb_ptr[c]);
      //    temperatures.at<float>(r, c) = 0.04f * temp - 273.15f;
      //  }
      //});
      

      

      //tl::ImagingProcesses imagingProcesses;
      //imagingProcesses.push_back(std::make_shared<GaussianBlur>(cv::Size(3,3), 0));
      //imagingProcesses.push_back(std::make_shared<Binarize>());

      //cv::Mat hot_spot;
      //imagingProcesses.run(temperatures, hot_spot);

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