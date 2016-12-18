#include <stdio.h>

#include "opencv2/core/core.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"

// Cabeceras tidopLib
#include "core/console.h"
#include "core/messages.h"
#include "videostream.h"
#include "img_processing.h"
#include "geometric_entities/window.h"

using namespace cv;
using namespace I3D;


/*!
 * Clase auxiliar para manejar los frames de video. Heredada 
 * de VideoStream::Listener e implementa los métodos que controlan
 * la ejecución de los eventos
 */
class VideoHelper : public VideoStream::Listener
{
public:

  /*!
   * \brief mCurrentPosition Posición actual del video
   */
  double mCurrentPosition;

  /*!
   * \brief mOutPath Ruta donde se guardan las imagenes de salida
   */
  std::string mOutPath;

  /*!
   * \brief mExtFile Extensión de las imagenes de salida
   */
  std::string mExtFile;

  /*!
   * \brief pProgress Barra de progreso
   */
  ProgressBar *pProgress;

public:

  VideoHelper(const std::string &path, const std::string &ext) 
    : mOutPath(path), mExtFile(ext), mCurrentPosition(0) { }

  ~VideoHelper() {}

  /*!
   * \brief onFinish
   */
  void onFinish() override;

  /*!
   * \brief onInitialize
   */
  void onInitialize() override;

  /*!
   * \brief onPause
   */
  void onPause() override;

  /*!
   * \brief onPositionChange
   * \param position Posición del video
   */
  void onPositionChange(double position) override;

  /*!
   * \brief onRead
   * \param frame Frame leido
   */
  void onRead(cv::Mat &frame) override;

  /*!
   * \brief onResume
   */
  void onResume() override;

  /*!
   * \brief onShow
   * \param frame Frame que se muestra
   */
  void onShow(cv::Mat &frame) override;

  /*!
   * \brief onStop
   */
  void onStop() override;

  /*!
   * \brief Establece una función de progreso
   * \param progress
   */
  void setProgressBar( ProgressBar *progress);

};

void VideoHelper::onFinish()
{
  VideoStream::Listener::onFinish();
}

void VideoHelper::onInitialize()
{
  VideoStream::Listener::onInitialize();

  if (mOutPath.empty()) {
    char path[I3D_MAX_DRIVE + I3D_MAX_DIR];
    getFileDriveDir(getRunfile(),path);
    mOutPath = path;
  } else {
    createDir(mOutPath.c_str());
  }
}

void VideoHelper::onPause()
{
  VideoStream::Listener::onPause(); 
}

void VideoHelper::onPositionChange(double position) 
{ 
  VideoStream::Listener::onPositionChange(position);
  mCurrentPosition = position;
}

void VideoHelper::onRead(cv::Mat &frame) 
{
  VideoStream::Listener::onRead(frame);
  WindowI wOut;
  char buffer[I3D_MAX_PATH];
  sprintf_s(buffer, "%s\\frame%05i.%s", mOutPath.c_str(), cvRound(mCurrentPosition), mExtFile.c_str());
  cv::imwrite(buffer, frame);
  if (pProgress)(*pProgress)();
}

void VideoHelper::onResume() 
{ 
  VideoStream::Listener::onResume();
}

void VideoHelper::onShow(cv::Mat &frame) 
{ 
  VideoStream::Listener::onShow(frame);
}

void VideoHelper::onStop()
{ 
  VideoStream::Listener::onStop(); 
}

void VideoHelper::setProgressBar(ProgressBar *progress)
{
  pProgress = progress;
}

/*!
 * save_video_frames: Guarda los frames de video como imagen
 *
 * video    << Video
 * out_path >> Directorio de salida donde se guardan las imagenes
 * ext      << Extensión del formato de imagen de salida
 *
 * uso:
 * save_video_frames --video=[value] --out_path=[value] --ext=[value]
 */
int main(int argc, char** argv)
{

  CmdParser cmdParser("save_video_frames", "Guarda los frames de video como imagen");
  cmdParser.addParameter("video", "Video");
  cmdParser.addParameter("out_path", "Directorio de salida donde se guardan las imagenes");
  cmdParser.addParameter("ext", "Extensión del formato de imagen de salida", true, "*.png");
  cmdParser.parse(argc, argv);
  if (cmdParser.parse(argc, argv) == CmdParser::MSG::PARSE_ERROR ) {
    cmdParser.printHelp();
    return 0;
  }
  std::string video = cmdParser.getValue<std::string>("video");
  std::string out_path = cmdParser.getValue<std::string>("out_path");
  std::string ext = cmdParser.getValue<std::string>("ext");

  //Configuración de log y mensajes por consola
  char logfile[I3D_MAX_PATH];
  if (changeFileExtension(getRunfile(), "log", logfile) == 0) {
    Message::setMessageLogFile(logfile);
    Message::setMessageLevel(MessageLevel::MSG_INFO);
  }

  // Lectura de video
  VideoStream strmVideo(video.c_str());
  if (!strmVideo.isOpened()) {
    printInfo("No se ha podido cargar el video: %s", video.c_str());
    return 0;
  }
  
  strmVideo.setSkipFrames(1);

  // Barra de progreso
  ProgressBar progress_bar;
  progress_bar.init(0, strmVideo.getFrameCount());

  VideoHelper videoHelper(out_path, ext);
  videoHelper.setProgressBar(&progress_bar);
  strmVideo.addListener(&videoHelper);

  strmVideo.run(); 

  return 0;
}
