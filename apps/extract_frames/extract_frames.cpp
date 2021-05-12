/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by Esteban Ruiz de Oña Crespo                       *
 *                                                                        *
 * This file is part of TidopLib                                          *
 *                                                                        *
 * TidopLib is free software: you can redistribute it and/or modify       *
 * it under the terms of the GNU Lesser General Public License as         *
 * published by the Free Software Foundation, either version 3 of the     *
 * License, or (at your option) any later version.                        *
 *                                                                        *
 * TidopLib is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU Lesser General Public License for more details.                    *
 *                                                                        *
 * You should have received a copy of the GNU Lesser General Public       *
 * License along with Foobar. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/
 
#include <stdio.h>

// Cabeceras tidopLib
#include <tidop/core/console.h>
#include <tidop/core/messages.h>
#include <tidop/core/path.h>
#include <tidop/experimental/video/videoio.h>


using namespace tl;

#ifdef HAVE_VLD
#include <vld.h>
#endif


class VideoHelper 
  : public VideoOpenCV::Listener
{

public:

  /*!
   * \brief mCurrentPosition Posición actual del video
   */
  double mCurrentPosition;

  /*!
   * \brief mOutPath Ruta donde se guarda el log e información de salida
   */
  std::string mOutPath;
  
  int mStep;
  
public:

  VideoHelper(const std::string &path) 
    : mOutPath(path),
      mCurrentPosition(0),
	    mStep(20)
  { 
  }

  ~VideoHelper() {}

  void setStep(int step)
  {
	  mStep = step;
  }

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

};

void VideoHelper::onFinish()
{
  VideoOpenCV::Listener::onFinish();
}

void VideoHelper::onInitialize()
{
  VideoOpenCV::Listener::onInitialize();
  //cv::namedWindow("Video", cv::WINDOW_NORMAL);
}

void VideoHelper::onPause()
{
  VideoOpenCV::Listener::onPause(); 
}

void VideoHelper::onPositionChange(double position) 
{ 
  VideoOpenCV::Listener::onPositionChange(position);
  mCurrentPosition = position;
}

void VideoHelper::onRead(cv::Mat &frame) 
{
  VideoOpenCV::Listener::onRead(frame);
  std::string path = mOutPath;
  int pos = static_cast<int>(mCurrentPosition);
  if (pos % mStep == 0) {
    path.append("/frame").append(std::to_string(pos)).append(".jpg");
    cv::imwrite(path, frame);
  }

  //cv::imshow("Video", frame);
}

void VideoHelper::onResume() 
{ 
  VideoOpenCV::Listener::onResume();
}

void VideoHelper::onShow(cv::Mat &frame) 
{ 
  //VideoOpenCV::Listener::onShow(frame);
}

void VideoHelper::onStop()
{ 
  VideoOpenCV::Listener::onStop(); 
}


/*!
 * read_video: 
 *
 *
 * uso:
 */
int main(int argc, char** argv)
{

  // Consola
  Console &console = Console::instance();
  console.setTitle("Extract frames from video");
  console.setMessageLevel(MessageLevel::msg_verbose);
  console.setConsoleUnicode();
  MessageManager::instance().addListener(&console);

  Path video;
  Path image_path;
  int step = 20;
  
  Command cmd("extract_frames", "Extract frames from video");
  cmd.push_back(std::make_shared<ArgumentPathRequired>("video", 'v', "Video del cual se quieren extraer los fotogramas", &video));
  cmd.push_back(std::make_shared<ArgumentPathRequired>("images", 'i', "Ruta en la que se guardan las imágenes", &image_path));
  cmd.push_back(std::make_shared<ArgumentIntegerOptional>("step", 's', "Número de frames que se saltan. Por defecto 20", &step));
  
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

  VideoOpenCV video_cv(video.toString());
  if (video_cv.isOpened()) {
      
    image_path.createDirectories();

    VideoHelper videoHelper(image_path.toString());
    videoHelper.setStep(step);
    video_cv.addListener(&videoHelper);
    //video_cv.setFramePerSeconds(30);
    video_cv.run();

  } else {
    msgError("No se ha podido cargar el video: %s", video.toString().c_str());
    return 1;
  }

  return 0;
}
