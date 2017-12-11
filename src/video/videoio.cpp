#include "video/videoio.h"

#include "geometric_entities/window.h"

#ifdef HAVE_OPENCV
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <ctime>

namespace I3D
{

VrtVideo::VrtVideo() 
  : File(), 
    mWidth(0), 
    mHeight(0), 
    mBands(0), 
    mWindow(),
    bKeepRatio(true),
    mViewMode(VrtVideo::View::ORIGINAL_FRAME),
    mSkipMode(VrtVideo::Skip::NOT_SKIP),
    mSkip(0),
    mFramesPerSecond(0),
    mStatus(VrtVideo::VideoStatus::START),
    mEvents(0),
    bGray(false)
{

}

VrtVideo::VrtVideo(const char *file, Mode mode) 
  : File(file, mode), 
    mWidth(0), 
    mHeight(0), 
    mBands(0), 
    mWindow(),
    bKeepRatio(true),
    mViewMode(VrtVideo::View::ORIGINAL_FRAME),
    mSkipMode(VrtVideo::Skip::NOT_SKIP),
    mSkip(0),
    mFramesPerSecond(0),
    mStatus(VrtVideo::VideoStatus::START),
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

int VrtVideo::getWidth() const
{ 
  return mWidth; 
}


int VrtVideo::getHeight() const 
{
  return mHeight;
}

int VrtVideo::getBands() const 
{
  return mBands; 
}

double VrtVideo::getFramePerSeconds() const
{ 
  return mFramesPerSecond; 
}

void VrtVideo::run()
{
  onInitialize();
  int delay = static_cast<int>(1000. / mFramesPerSecond);
  char c;
  while (mStatus != VideoStatus::STOPPED && mStatus != VideoStatus::FINALIZED) {
    if (mStatus != VideoStatus::PAUSE) {
      read();
      if (mStatus == VideoStatus::FINALIZED) break;
      double currentFrame = getCurrentFrame();
      onPositionChange( currentFrame /*mVideoCapture.get(CV_CAP_PROP_POS_FRAMES)*/);
      onShow();  
    }
    
    c = (char) cv::waitKey(2/*delay*/);
    if (c == 27) stop();                // - Tecla esc -> Terminar la ejecución del video.
    //else if (c == 43) skipDown();       // - Tecla + -> Aumenta el salto entre frames.
    //else if (c == 45) skipUp();         // - Tecla - -> Disminuye el salto entre frames.
    else if (c == 32) {                 // - Barra espaciadora -> Pausa/reanuda la ejecución del video.
      if (mStatus == VideoStatus::RUNNING) pause();
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
  mStatus = VideoStatus::RUNNING;
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
  msgDebug("Posición: %i", static_cast<int>(position));
}

I3D_DISABLE_WARNING(4100)
void VrtVideo::Listener::onRead(cv::Mat &frame) 
{ 

}

void VrtVideo::Listener::onResume() 
{ 

}

void VrtVideo::Listener::onShow(cv::Mat &frame) 
{ 

}
I3D_ENABLE_WARNING(4100)

void VrtVideo::Listener::onStop()
{ 
  msgDebug("Video detenido");
}

/* ---------------------------------------------------------------------------------- */

VideoOpenCV::VideoOpenCV() 
  : VrtVideo()
{

}

VideoOpenCV::VideoOpenCV(const char *file, Mode mode) 
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

VideoOpenCV::Status VideoOpenCV::open(const char *file, VideoOpenCV::Mode mode)
{
  //close();

  mFile = file;

  // Era redundante ya que el método VideoCapture::open() hace lo mismo 
  //if (mVideoCapture.isOpened())
  //mVideoCapture.release();

  mMode = mode;
  bool bOpen = true;
  if (mMode == Mode::Read) {
    bOpen = mVideoCapture.open(file);
  } else if (mMode == Mode::Update) {
    bOpen = mVideoCapture.open(file);
  } else if (mMode == Mode::Create) {

  }

  if (!bOpen) 
    return Status::OPEN_FAIL;
  else {
    if (mMode == Mode::Create) {

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
    return Status::OPEN_OK;
  }
}

VideoOpenCV::Status VideoOpenCV::create(int width, int height, int codec, int fps, bool color) 
{
  if (mMode == Mode::Read) {
    msgError("Utilice el modo Create para abrir el archivo");
    return Status::FAILURE; 
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
    return Status::SUCCESS;
  } else {
    msgError("Could not open the output video for write: %s", mFile.c_str());
    return Status::FAILURE;
  }
}

VideoOpenCV::Status VideoOpenCV::createCopy(const char *fileOut)
{
  return Status::SUCCESS;
}

double VideoOpenCV::getCurrentFrame() const
{ 
  return mVideoCapture.get(cv::CAP_PROP_POS_FRAMES); 
}

double VideoOpenCV::getFrameCount() const
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
//    if (c == 27) stop();                // - Tecla esc -> Terminar la ejecución del video.
//    //else if (c == 43) skipDown();       // - Tecla + -> Aumenta el salto entre frames.
//    //else if (c == 45) skipUp();         // - Tecla - -> Disminuye el salto entre frames.
//    else if (c == 32) {                 // - Barra espaciadora -> Pausa/reanuda la ejecución del video.
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
  mStatus = VideoStatus::STOPPED;
  mVideoCapture.release();
  onStop();
}

void VideoOpenCV::pause() 
{ 
  if (mStatus == VideoStatus::RUNNING) {
    mStatus = VideoStatus::PAUSE;
    onPause();
  }
}

void VideoOpenCV::resume() 
{ 
  if (mStatus == VideoStatus::PAUSE) {
    mStatus = VideoStatus::RUNNING;
    onResume();
  }
}

void VideoOpenCV::finish() 
{ 
  mStatus = VideoStatus::FINALIZED;
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
  //if (mVideoCapture.get(cv::CAP_PROP_POS_FRAMES) == 0.0) {
  //  start = std::clock();
  //} else {
  //  int ms = std::clock() - start;
  //  msgInfo("real-time %.2lf", ms / 1000.);
  ////  mVideoCapture.set(cv::CAP_PROP_POS_MSEC, ms);
  //}
  try {
    if ((bret = mVideoCapture.read(mFrame))){
      double posframe = mVideoCapture.get(cv::CAP_PROP_POS_MSEC);
      //msgInfo("time %.2lf", posframe / 1000.);
      if (bGray)
        cv::cvtColor(mFrame, mFrame, CV_BGR2GRAY); 
    } else {
      // No ha podido leer el frame. Tratamos de seguir leyendo mientras no lleguemos al final del video.
      if (mFrame.empty() && mVideoCapture.isOpened()) {
        double posframe = mVideoCapture.get(cv::CAP_PROP_POS_FRAMES);
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

ImagePresentation::ImagePresentation(const char *file, Mode mode) 
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

ImagePresentation::Status ImagePresentation::open(const char *file, ImagePresentation::Mode mode)
{
  close();

  mFile = file;

  // Sacar a metodo miembro
  std::ifstream read_handler;
  


  mMode = mode;
  bool bOpen = true;
  if (mMode == Mode::Read) {
    read_handler.open(mFile, std::ifstream::in);
    bOpen = read_handler.is_open();
    msgError("Unable to read file: %s", mFile.c_str());
  } else if (mMode == Mode::Update) {

  } else if (mMode == Mode::Create) {

  }

  if (!bOpen) 
    return Status::OPEN_FAIL;
  else {
    if (mMode == Mode::Create) {

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
    return Status::OPEN_OK;
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
    return Status::FAILURE;
  //}
}

ImagePresentation::Status ImagePresentation::createCopy(const char *fileOut)
{
  return Status::SUCCESS;
}

double ImagePresentation::getCurrentFrame() const
{ 
  return mCurrentFrame; 
}

double ImagePresentation::getFrameCount() const
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
  //  if (c == 27) stop();                // - Tecla esc -> Terminar la ejecución del video.
  //  //else if (c == 43) skipDown();       // - Tecla + -> Aumenta el salto entre frames.
  //  //else if (c == 45) skipUp();         // - Tecla - -> Disminuye el salto entre frames.
  //  else if (c == 32) {                 // - Barra espaciadora -> Pausa/reanuda la ejecución del video.
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
  mStatus = VideoStatus::STOPPED;

  onStop();
}

void ImagePresentation::pause() 
{ 
  if (mStatus == VideoStatus::RUNNING) {
    mStatus = VideoStatus::PAUSE;
    onPause();
  }
}

void ImagePresentation::resume() 
{ 
  if (mStatus == VideoStatus::PAUSE) {
    mStatus = VideoStatus::RUNNING;
    onResume();
  }
}

void ImagePresentation::finish() 
{ 
  mStatus = VideoStatus::FINALIZED;
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
      double duration = getFrameCount();
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



} // End namespace I3D


#endif // HAVE_OPENCV
