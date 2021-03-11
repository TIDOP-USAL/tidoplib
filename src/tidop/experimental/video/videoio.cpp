/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by Esteban Ruiz de O�a Crespo                       *
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
 
#include "videoio.h"

#include "tidop/geometry/entities/window.h"

#ifdef HAVE_OPENCV
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"


#include <ctime>

namespace tl
{

VrtVideo::VrtVideo() 
  : File(), 
    mWidth(0), 
    mHeight(0), 
    mBands(0), 
    mWindow(),
    bKeepRatio(true),
    mViewMode(VrtVideo::View::original_frame),
    mSkipMode(VrtVideo::Skip::not_skip),
    mSkip(0),
    mFramesPerSecond(0),
    mStatus(VrtVideo::VideoStatus::start),
    mEvents(0),
    bGray(false)
{

}

VrtVideo::VrtVideo(const std::string &file, Mode mode) 
  : File(file, mode), 
    mWidth(0), 
    mHeight(0), 
    mBands(0), 
    mWindow(),
    bKeepRatio(true),
    mViewMode(VrtVideo::View::original_frame),
    mSkipMode(VrtVideo::Skip::not_skip),
    mSkip(0),
    mFramesPerSecond(0),
    mStatus(VrtVideo::VideoStatus::start),
    mEvents(0),
    bGray(false)
{
}

VrtVideo::VrtVideo(const VrtVideo &video) 
  : File(video), 
    mWidth(video.mWidth), 
    mHeight(video.mHeight), 
    mBands(video.mBands), 
    mWindow(video.mWindow),
    bKeepRatio(video.bKeepRatio),
    mViewMode(video.mViewMode),
    mSkipMode(video.mSkipMode),
    mSkip(video.mSkip),
    mFramesPerSecond(video.mFramesPerSecond),
    mStatus(video.mStatus),
    mEvents(video.mEvents),
    bGray(video.bGray)
{

}

VrtVideo::~VrtVideo()
{

}

void VrtVideo::addListener(Listener *listener)
{ 
  mEvents.push_back(listener);
}

int VrtVideo::width() const
{ 
  return mWidth; 
}


int VrtVideo::height() const 
{
  return mHeight;
}

int VrtVideo::bands() const 
{
  return mBands; 
}

double VrtVideo::framePerSeconds() const
{ 
  return mFramesPerSecond; 
}

void VrtVideo::run()
{
  onInitialize();
  int delay = static_cast<int>(1000. / mFramesPerSecond);
  char c;
  while (mStatus != VideoStatus::stopped && mStatus != VideoStatus::finalized) {
    if (mStatus != VideoStatus::pause) {
      read();
      if (mStatus == VideoStatus::finalized) break;
      double currentFrame = this->currentFrame();
      onPositionChange( currentFrame /*mVideoCapture.get(CV_CAP_PROP_POS_FRAMES)*/);
      onShow();  
    }
    
    c = (char) cv::waitKey(delay);
    if (c == 27) stop();                // - Tecla esc -> Terminar la ejecuci�n del video.
    //else if (c == 43) skipDown();       // - Tecla + -> Aumenta el salto entre frames.
    //else if (c == 45) skipUp();         // - Tecla - -> Disminuye el salto entre frames.
    else if (c == 32) {                 // - Barra espaciadora -> Pausa/reanuda la ejecuci�n del video.
      if (mStatus == VideoStatus::running) pause();
      else resume();
    }
  }
  finish();
}


void VrtVideo::onFinish() {
  msgDebug("Fin captura de video");
  if (!mEvents.empty()) {
    for (auto ev : mEvents) {
      ev->onFinish();
    }
  }
}

void VrtVideo::onInitialize() {
  msgDebug("Inicio captura video");
  mStatus = VideoStatus::running;
  if (!mEvents.empty()) {
    for (auto ev : mEvents) {
      ev->onInitialize();
    }
  }
}

void VrtVideo::onPause() 
{ 
  if (!mEvents.empty()) {
    for (auto ev : mEvents) {
      ev->onPause();
    }
  }
}

void VrtVideo::onPositionChange(double position)
{ 
  if (!mEvents.empty()) {
    for (auto ev : mEvents) {
      ev->onPositionChange(position);
    }
  }
}

void VrtVideo::onRead()
{ 
  if (!mEvents.empty()) {
    for (auto ev : mEvents) {
      ev->onRead(mFrame);
    }
  }
}

void VrtVideo::onResume() 
{ 
  if (!mEvents.empty()) {
    for (auto ev : mEvents) {
      ev->onResume();
    }
  }
}

void VrtVideo::onShow()
{ 
  if (!mEvents.empty()) {
    for (auto ev : mEvents) {
      ev->onShow(mFrame);
    }
  }
}

void VrtVideo::onStop()
{ 
  if (!mEvents.empty()) {
    for (auto ev : mEvents) {
      ev->onStop();
    }
  }
}

void VrtVideo::Listener::onFinish()
{
  msgDebug("Fin captura de video");
}

void VrtVideo::Listener::onInitialize()
{
  msgDebug("Inicio captura video");
}

void VrtVideo::Listener::onPause()
{
  msgDebug("Video pausado");
}

void VrtVideo::Listener::onPositionChange(double position) 
{ 
  msgDebug("Posici�n: %i", static_cast<int>(position));
}

TL_DISABLE_WARNING(4100)
void VrtVideo::Listener::onRead(cv::Mat &frame) 
{ 

}

void VrtVideo::Listener::onResume() 
{ 

}

void VrtVideo::Listener::onShow(cv::Mat &frame) 
{ 

}
TL_ENABLE_WARNING(4100)

void VrtVideo::Listener::onStop()
{ 
  msgDebug("Video detenido");
}

/* ---------------------------------------------------------------------------------- */

VideoOpenCV::VideoOpenCV() 
  : VrtVideo()
{

}

VideoOpenCV::VideoOpenCV(const std::string &file, Mode mode) 
  : VrtVideo(file, mode)
{
  open(file, mode);
}

VideoOpenCV::VideoOpenCV(const VideoOpenCV &video)
  : VrtVideo(video)
{

}

VideoOpenCV::~VideoOpenCV()
{
  close();
}

void VideoOpenCV::close()
{
  mVideoCapture.release();
  mVideoWriter.release();
}

VideoOpenCV::Status VideoOpenCV::open(const std::string &file, VideoOpenCV::Mode mode, FileOptions *options)
{
  //close();

  mFile = file;

  // Era redundante ya que el m�todo VideoCapture::open() hace lo mismo 
  //if (mVideoCapture.isOpened())
  //mVideoCapture.release();

  mMode = mode;
  bool bOpen = true;
  if (mMode == Mode::read) {
    bOpen = mVideoCapture.open(file);
  } else if (mMode == Mode::update) {
    bOpen = mVideoCapture.open(file);
  } else if (mMode == Mode::create) {

  }

  if (!bOpen) 
    return Status::open_fail;
  else {
    if (mMode == Mode::create) {

    } else {
      mWidth = static_cast<int>(mVideoCapture.get(cv::CAP_PROP_FRAME_WIDTH));
      mHeight = static_cast<int>(mVideoCapture.get(cv::CAP_PROP_FRAME_HEIGHT));
      msgInfo("Video size: %ix%i", mWidth, mHeight);
      //mSize = mFrameSize = cv::Size(width, height);
      mFramesPerSecond = mVideoCapture.get(cv::CAP_PROP_FPS);
      msgInfo("Framerate: %f", mFramesPerSecond);
      mCodec = static_cast<int>(mVideoCapture.get(cv::CAP_PROP_FOURCC));
      char c[] = {(char)(mCodec & 0XFF) , (char)((mCodec & 0XFF00) >> 8),(char)((mCodec & 0XFF0000) >> 16),(char)((mCodec & 0XFF000000) >> 24), 0};
      msgInfo("codec: %f", c);
    }
    return Status::open_ok;
  }
}

VideoOpenCV::Status VideoOpenCV::create(int width, int height, int codec, int fps, bool color) 
{
  if (mMode == Mode::read) {
    msgError("Utilice el modo Create para abrir el archivo");
    return Status::failure; 
  }
  if (mVideoWriter.open(mFile, -1, fps, cv::Size(width, height), color)) {
    mWidth = static_cast<int>(mVideoWriter.get(cv::CAP_PROP_FRAME_WIDTH));
    mHeight = static_cast<int>(mVideoWriter.get(cv::CAP_PROP_FRAME_HEIGHT));
    msgInfo("Video size: %ix%i", mWidth, mHeight);
    //mSize = mFrameSize = cv::Size(width, height);
    mFramesPerSecond = mVideoWriter.get(cv::CAP_PROP_FPS);
    msgInfo("Framerate: %f", mFramesPerSecond);
    mCodec = static_cast<int>(mVideoWriter.get(cv::CAP_PROP_FOURCC));
    char c[] = {(char)(mCodec & 0XFF) , (char)((mCodec & 0XFF00) >> 8),(char)((mCodec & 0XFF0000) >> 16),(char)((mCodec & 0XFF000000) >> 24), 0};
    msgInfo("codec: %f", c);
    return Status::success;
  } else {
    msgError("Could not open the output video for write: %s", mFile.c_str());
    return Status::failure;
  }
}

VideoOpenCV::Status VideoOpenCV::createCopy(const std::string &fileOut)
{
  return Status::success;
}

double VideoOpenCV::currentFrame() const
{ 
  return mVideoCapture.get(cv::CAP_PROP_POS_FRAMES); 
}

double VideoOpenCV::frameCount() const
{
  return mVideoCapture.get(cv::CAP_PROP_FRAME_COUNT);
}

bool VideoOpenCV::isOpened() const
{
  return mVideoCapture.isOpened();
}


void VideoOpenCV::setFramePerSeconds(double fps)
{
  mFramesPerSecond = fps;
  mVideoCapture.set(cv::CAP_PROP_FPS, fps);
}

//void VideoOpenCV::run()
//{
//  onInitialize();
//  int delay = static_cast<int>(100. / mFramesPerSecond);
//  char c;
//  while (mStatus != VideoStatus::STOPPED && mStatus != VideoStatus::FINALIZED) {
//    if (mStatus != VideoStatus::PAUSE) {
//      read();
//      if (mStatus == VideoStatus::FINALIZED) break;
//      double currentFrame = getCurrentFrame();
//      onPositionChange( currentFrame /*mVideoCapture.get(CV_CAP_PROP_POS_FRAMES)*/);
//      onShow();  
//    }
//    
//    c = (char) cv::waitKey(delay);
//    if (c == 27) stop();                // - Tecla esc -> Terminar la ejecuci�n del video.
//    //else if (c == 43) skipDown();       // - Tecla + -> Aumenta el salto entre frames.
//    //else if (c == 45) skipUp();         // - Tecla - -> Disminuye el salto entre frames.
//    else if (c == 32) {                 // - Barra espaciadora -> Pausa/reanuda la ejecuci�n del video.
//      if (mStatus == VideoStatus::RUNNING) pause();
//      else resume();
//    }
//  }
//  finish();
//}

void VideoOpenCV::read()
{ 
  //if (!nextFrame(NULL)) {
  if (!_read()){
    finish();
  } else {
    onRead();
  }
}

void VideoOpenCV::stop() 
{ 
  mStatus = VideoStatus::stopped;
  mVideoCapture.release();
  onStop();
}

void VideoOpenCV::pause() 
{ 
  if (mStatus == VideoStatus::running) {
    mStatus = VideoStatus::pause;
    onPause();
  }
}

void VideoOpenCV::resume() 
{ 
  if (mStatus == VideoStatus::pause) {
    mStatus = VideoStatus::running;
    onResume();
  }
}

void VideoOpenCV::finish() 
{ 
  mStatus = VideoStatus::finalized;
  onFinish();
}

void VideoOpenCV::update()
{
  //mWidth = static_cast<int>(mVideoCapture.get(cv::CAP_PROP_FRAME_WIDTH));
  //mHeight = static_cast<int>(mVideoCapture.get(cv::CAP_PROP_FRAME_HEIGHT));
  //msgInfo("Video size: %ix%i", mWidth, mHeight);
  ////mSize = mFrameSize = cv::Size(width, height);
  //mfps = mVideoCapture.get(cv::CAP_PROP_FPS);
  //msgInfo("Framerate: %f", mfps);
  //mCodec = static_cast<int>(mVideoCapture.get(cv::CAP_PROP_FOURCC));
  ////mCodec = {(char)(ex & 0XFF) , (char)((ex & 0XFF00) >> 8),(char)((ex & 0XFF0000) >> 16),(char)((ex & 0XFF000000) >> 24), 0};
}

static std::clock_t start;

bool VideoOpenCV::_read()
{
  bool bret = false;
  double posframe;
  //if (mVideoCapture.get(cv::CAP_PROP_POS_FRAMES) == 0.0) {
  //  start = std::clock();
  //} else {
  //  int ms = std::clock() - start;
  //  msgInfo("real-time %.2lf", ms / 1000.);
  ////  mVideoCapture.set(cv::CAP_PROP_POS_MSEC, ms);
  //}
  try {
    if ((bret = mVideoCapture.read(mFrame))){
      posframe = mVideoCapture.get(cv::CAP_PROP_POS_MSEC);
      //msgInfo("time %.2lf", posframe / 1000.);
      if (bGray)
        cv::cvtColor(mFrame, mFrame, cv::COLOR_BGR2GRAY); 
    } else {
      // No ha podido leer el frame. Tratamos de seguir leyendo mientras no lleguemos al final del video.
      if (mFrame.empty() && mVideoCapture.isOpened()) {
        posframe = mVideoCapture.get(cv::CAP_PROP_POS_FRAMES);
        double duration = mVideoCapture.get(cv::CAP_PROP_FRAME_COUNT);
        char c;
          while (duration > posframe && bret == false) {
            posframe++;
            msgError("Error al cargar el frame %.lf", posframe);
            mVideoCapture.set(cv::CAP_PROP_POS_FRAMES, posframe);
            bret = mVideoCapture.read(mFrame);
            c = (char)cv::waitKey(1);
            if (c == 27) return false;
          }
        //}
      }
    }
  } catch (std::exception &e) {
    msgError("Error de lectura: %s", e.what());
  }

  //if (bret) {
  //  if (mResolutionFrame == Resolution::RESIZE_FRAME) {
  //    resizeFrame();
  //  }
  //  else if (mResolutionFrame == Resolution::CROP_FRAME) {
  //    cropFrame();
  //  }
  //}
  //if (vf) mFrame.copyTo(*vf);
  return bret;
}


/* ---------------------------------------------------------------------------------- */

ImagePresentation::ImagePresentation() 
  : VrtVideo(),
    mImages(0),
    mCurrentFrame(0)
{

}

ImagePresentation::ImagePresentation(const std::string &file, Mode mode) 
  : VrtVideo(file, mode),
    mImages(0),
    mCurrentFrame(0)
{
  open(file, mode);
}


ImagePresentation::ImagePresentation(const ImagePresentation &video)
  : VrtVideo(video),
    mImages(video.mImages),
    mCurrentFrame(video.mCurrentFrame)
{

}

ImagePresentation::~ImagePresentation()
{
  close();
}

void ImagePresentation::close()
{

}

ImagePresentation::Status ImagePresentation::open(const std::string &file, ImagePresentation::Mode mode, FileOptions *options)
{
  close();

  mFile = file;

  // Sacar a metodo miembro
  std::ifstream read_handler;
  


  mMode = mode;
  bool bOpen = true;
  if (mMode == Mode::read) {
    read_handler.open(mFile, std::ifstream::in);
    bOpen = read_handler.is_open();
    msgError("Unable to read file: %s", mFile.c_str());
  } else if (mMode == Mode::update) {

  } else if (mMode == Mode::create) {

  }

  if (!bOpen) 
    return Status::open_fail;
  else {
    if (mMode == Mode::create) {

    } else {
      std::vector<std::string> out;
      while (!read_handler.eof()) {
        std::string line;
        read_handler >> line;
        if ( !line.empty() )
          mImages.push_back(line);
      }
      if (mImages.size() == 0) bIsOpened = false;
      else {
        cv::Mat aux = cv::imread(mImages[0]);
        bIsOpened = !aux.empty();
        mWidth = aux.cols;
        mHeight = aux.rows;
      }
    }
    return Status::open_ok;
  }
}

ImagePresentation::Status ImagePresentation::create(int width, int height, int codec, int fps, bool color) 
{
  //if (mOpenMode == Mode::Read) {
  //  msgError("Utilice el modo Create para abrir el archivo");
  //  return Status::FAILURE; 
  //}
  //if (mVideoWriter.open(mName, -1, fps, cv::Size(width, height), color)) {
  //  mWidth = static_cast<int>(mVideoWriter.get(cv::CAP_PROP_FRAME_WIDTH));
  //  mHeight = static_cast<int>(mVideoWriter.get(cv::CAP_PROP_FRAME_HEIGHT));
  //  msgInfo("Video size: %ix%i", mWidth, mHeight);
  //  //mSize = mFrameSize = cv::Size(width, height);
  //  mFramesPerSecond = mVideoWriter.get(cv::CAP_PROP_FPS);
  //  msgInfo("Framerate: %f", mFramesPerSecond);
  //  mCodec = static_cast<int>(mVideoWriter.get(cv::CAP_PROP_FOURCC));
  //  char c[] = {(char)(mCodec & 0XFF) , (char)((mCodec & 0XFF00) >> 8),(char)((mCodec & 0XFF0000) >> 16),(char)((mCodec & 0XFF000000) >> 24), 0};
  //  msgInfo("codec: %f", c);
  //  return Status::SUCCESS;
  //} else {
  //  msgError("Could not open the output video for write: %s", mName.c_str());
    return Status::failure;
  //}
}

ImagePresentation::Status ImagePresentation::createCopy(const std::string &fileOut)
{
  return Status::success;
}

double ImagePresentation::currentFrame() const
{ 
  return mCurrentFrame; 
}

double ImagePresentation::frameCount() const
{
  return static_cast<double>(mImages.size());
}

bool ImagePresentation::isOpened() const
{
  return bIsOpened;
}

void ImagePresentation::setFramePerSeconds(double fps)
{
  mFramesPerSecond = fps;
}


//void ImagePresentation::run()
//{
  //onInitialize();
  //int delay = static_cast<int>(100. / mFramesPerSecond);
  //char c;
  //while (mStatus != VideoStatus::STOPPED && mStatus != VideoStatus::FINALIZED) {
  //  if (mStatus != VideoStatus::PAUSE) {
  //    read();
  //    if (mStatus == VideoStatus::FINALIZED) break;
  //    double currentFrame = getCurrentFrame();
  //    onPositionChange( currentFrame /*mVideoCapture.get(CV_CAP_PROP_POS_FRAMES)*/);
  //    onShow();  
  //  }
  //  
  //  c = (char) cv::waitKey(delay);
  //  if (c == 27) stop();                // - Tecla esc -> Terminar la ejecuci�n del video.
  //  //else if (c == 43) skipDown();       // - Tecla + -> Aumenta el salto entre frames.
  //  //else if (c == 45) skipUp();         // - Tecla - -> Disminuye el salto entre frames.
  //  else if (c == 32) {                 // - Barra espaciadora -> Pausa/reanuda la ejecuci�n del video.
  //    if (mStatus == VideoStatus::RUNNING) pause();
  //    else resume();
  //  }
  //}
  //finish();
//}

void ImagePresentation::read()
{ 
  //if (!nextFrame(NULL)) {
  if (!_read()){
    finish();
  } else {
    onRead();
  }
}

void ImagePresentation::stop() 
{ 
  mStatus = VideoStatus::stopped;

  onStop();
}

void ImagePresentation::pause() 
{ 
  if (mStatus == VideoStatus::running) {
    mStatus = VideoStatus::pause;
    onPause();
  }
}

void ImagePresentation::resume() 
{ 
  if (mStatus == VideoStatus::pause) {
    mStatus = VideoStatus::running;
    onResume();
  }
}

void ImagePresentation::finish() 
{ 
  mStatus = VideoStatus::finalized;
  onFinish();
}

void ImagePresentation::update()
{
  //mWidth = static_cast<int>(mVideoCapture.get(cv::CAP_PROP_FRAME_WIDTH));
  //mHeight = static_cast<int>(mVideoCapture.get(cv::CAP_PROP_FRAME_HEIGHT));
  //msgInfo("Video size: %ix%i", mWidth, mHeight);
  ////mSize = mFrameSize = cv::Size(width, height);
  //mfps = mVideoCapture.get(cv::CAP_PROP_FPS);
  //msgInfo("Framerate: %f", mfps);
  //mCodec = static_cast<int>(mVideoCapture.get(cv::CAP_PROP_FOURCC));
  ////mCodec = {(char)(ex & 0XFF) , (char)((ex & 0XFF00) >> 8),(char)((ex & 0XFF0000) >> 16),(char)((ex & 0XFF000000) >> 24), 0};
}

bool ImagePresentation::_read()
{
  bool b_ret = false;
  if (mCurrentFrame < mImages.size()) {
    std::string imgFile = mImages[static_cast<unsigned int>(mCurrentFrame)];
    mFrame = cv::imread(imgFile);
    if (mFrame.empty()) {
      // No ha podido leer el frame. Tratamos de seguir leyendo mientras no lleguemos al final del video.
      double duration = frameCount();
      char c;
      while (duration > mCurrentFrame && b_ret == false) {
        msgError("No se ha podido cargar la imagen: %s", imgFile.c_str());
        mCurrentFrame++;
        imgFile = mImages[static_cast<unsigned int>(mCurrentFrame)];
        mFrame = cv::imread(imgFile);
        b_ret = !mFrame.empty();
        c = (char)cv::waitKey(1);
        if (c == 27) return false;
      }
    } else 
      b_ret = true;
    
    //if (b_ret) {
    //  msgInfo("Imagen: %s", imgFile.c_str());
    //  
    //  if (mResolutionFrame == Resolution::RESIZE_FRAME) {
    //    resizeFrame();
    //  } else if (mResolutionFrame == Resolution::CROP_FRAME) {
    //    cropFrame();
    //  }
    //}
    mCurrentFrame++;
  }
  return b_ret;
}



} // End namespace tl


#endif // HAVE_OPENCV