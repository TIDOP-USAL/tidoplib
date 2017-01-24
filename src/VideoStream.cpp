#include "VideoStream.h"

#include "core/messages.h"
#include "core/utils.h"
#include "core/mathutils.h"

#include "opencv2/imgproc/imgproc.hpp"

/* ---------------------------------------------------------------------------------- */

namespace I3D
{

// Escuchadores de eventos de video

void onTrackbarPositionChange(int _pos, void *vs)
{
  VideoStream *_vs = (VideoStream *)vs;
  _vs->setPosFrame(_pos);
}

/* ---------------------------------------------------------------------------------- */

void VideoStream::addListener(Listener *listener)
{ 
  events.push_back(listener);
  //listener->setVideoStream(this);
}

cv::Size VideoStream::getFrameSize()
{
  return mFrameSize;
}

cv::Size VideoStream::getSize()
{
  //cv::Size sz(0, 0);
  //if (isOpened()){
  //  int width =  (int)mVideoCapture.get(CV_CAP_PROP_FRAME_WIDTH);
  //  int height = (int)mVideoCapture.get(CV_CAP_PROP_FRAME_HEIGHT);
  //  sz = cv::Size(width, height);
  //}
  //return sz;
  return mSize;
}

bool VideoStream::nextFrame(cv::Mat *vf)
{
  return nextFrame(vf, sv, mSkip);
}

bool VideoStream::nextFrame(cv::Mat *vf, VideoStream::Skip skip, int nskip)
{
  bool bret = false;
  if (skip == VideoStream::Skip::SKIP_FRAMES)             bret = skipFrames(vf, nskip);
  else if (skip == VideoStream::Skip::SKIP_MILLISECONDS)  bret = skipMillisecond(vf, nskip);
  else                                                    bret = read(vf);
  return bret;
}

bool VideoStream::open(const char *name)
{
  mVideoCapture.open(name);
  if (! mVideoCapture.isOpened()) return false;
  else {
    //cv::Size sz(0, 0);
    int width =  (int)mVideoCapture.get(CV_CAP_PROP_FRAME_WIDTH);
    int height = (int)mVideoCapture.get(CV_CAP_PROP_FRAME_HEIGHT);
    mSize = mFrameSize = cv::Size(width, height);
    mfps = mVideoCapture.get(CV_CAP_PROP_FPS);
    return true;
  }
}

bool VideoStream::read(cv::Mat *vf)
{
  //... Prueba de comportamiento con un video en tiempo real 
//#if defined(_DEBUG)
//  if (get(CV_CAP_PROP_POS_FRAMES) == 0.0) {
//    start = std::clock();
//  } else {
//    int ms = std::clock() - start;
//    set(CV_CAP_PROP_POS_MSEC, ms);
//    int fps = cvRound(get(CV_CAP_PROP_POS_FRAMES));
//    printf("Cargado frame: %i\r", fps);
//  }
//#endif
  //
  bool bret = false;
  if ((bret = mVideoCapture.read(mFrame))){
    while (bSkipBlurryFrames && isImageBlurry(mFrame)) {
      bret = mVideoCapture.read(mFrame);
      if (bret == false) break;
    }
  } else {
    // No ha podido leer el frame. Tratamos de seguir leyendo mientras no lleguemos al final del video.
    if (mFrame.empty()) {
      double posframe = mVideoCapture.get(CV_CAP_PROP_POS_FRAMES);
      double duration = mVideoCapture.get(CV_CAP_PROP_FRAME_COUNT);
      char c;
      while (duration > posframe && bret == false) {
        posframe++;
        logPrintError("Error al cargar el frame %.lf", posframe);
        mVideoCapture.set(CV_CAP_PROP_POS_FRAMES, posframe);
        bret = mVideoCapture.read(mFrame);
        c = (char)cv::waitKey(1);
        if (c == 27) return false;
      }
    }
  }
  if (bret) {
    if (mResolutionFrame == Resolution::RESIZE_FRAME) {
      resizeFrame();
    }
    else if (mResolutionFrame == Resolution::CROP_FRAME) {
      cropFrame();
    }
  }
  if (vf) mFrame.copyTo(*vf);
  return bret;
}

void VideoStream::run()
{
  onInitialize();
  int delay = 100 / cvRound(fps());
  char c;
  while (vs != Status::STOPPED && vs != Status::FINALIZED) {
    if (vs != Status::PAUSE) {
      onRead();
      if (vs == Status::FINALIZED) break;
      double currentFrame = getCurrentFrame();
      onPositionChange( currentFrame /*mVideoCapture.get(CV_CAP_PROP_POS_FRAMES)*/);
      onShow();  
    }
    
    c = (char)cv::waitKey(delay);
    if (c == 27) onStop();              // - Tecla esc -> Terminar la ejecución del video.
    else if (c == 43) skipDown();       // - Tecla + -> Aumenta el salto entre frames.
    else if (c == 45) skipUp();         // - Tecla - -> Disminuye el salto entre frames.
    else if (c == 32) {                 // - Barra espaciadora -> Pausa/reanuda la ejecución del video.
      if (vs == Status::RUNNING) onPause();
      else onResume();
    }
  }
  onFinish();
}

void VideoStream::setCropRect(cv::Rect rf, bool keepRatio)
{
  rframe = rf;
  mFrameSize  = rframe.size();
  bKeepRatio = keepRatio;
  mResolutionFrame = Resolution::CROP_FRAME;
}

void VideoStream::setFrameSize(cv::Size sz, Resolution rf, bool keepRatio)
{
  mFrameSize = sz;
  bKeepRatio = keepRatio;
  mResolutionFrame = (mFrameSize == mSize) ? Resolution::ORIGINAL_FRAME : rf;
}

bool VideoStream::setPosFrame(double nframe)
{ 
  double duration = mVideoCapture.get(CV_CAP_PROP_FRAME_COUNT);
  if (nframe > duration) return false;
  mVideoCapture.set(CV_CAP_PROP_POS_FRAMES, nframe);
  return true;
}

void VideoStream::setSkipFrames(int frames)
{
  sv = Skip::SKIP_FRAMES;
  mSkip = frames;
}

void VideoStream::setSkipMillisecond(int ms)
{
  sv = Skip::SKIP_MILLISECONDS;
  mSkip = ms;
}


void VideoStream::skipDown()
{
  if (sv == Skip::NOT_SKIP) sv = Skip::SKIP_FRAMES;
  if (sv == Skip::SKIP_FRAMES) {
    if (mSkip == -1){
      mSkip = 1;
    } else if ( mSkip != 16 ) {
      if ( mSkip > 0 ) mSkip *= 2;
      else mSkip /= 2;
    }
  } else if ( sv == Skip::SKIP_MILLISECONDS ) {
    int n = cvRound(mSkip/fps());
    if (n == -1){
      mSkip = 1;
    } else if ( n != 16 ) {
      if ( n > 0 ) mSkip *= 2;
      else mSkip /= 2;
    }
  }
}

void VideoStream::skipUp()
{
  if (sv == Skip::NOT_SKIP) sv = Skip::SKIP_FRAMES;
  if (sv == Skip::SKIP_FRAMES) {
    if (mSkip == 1){
      mSkip = -2;
    } else if ( mSkip != -16 ) {
      if ( mSkip < 0 ) mSkip *= 2;
      else mSkip /= 2;
    }
  } else if ( sv == Skip::SKIP_MILLISECONDS ) {
    int n = cvRound(mSkip/fps());
    if (n == 1){
      mSkip = -2;
    } else if ( n != -16 ) {
      if ( n < 0 ) mSkip *= 2;
      else mSkip /= 2;
    }
  }
}

/*********************************************************/
/*                Métodos privados                       */
/*********************************************************/

void VideoStream::cropFrame()
{
  if ( rframe == cv::Rect()){
    int dw = mSize.width - mFrameSize.width;
    int dh = mSize.height - mFrameSize.height;
    if (dw > 0){
      rframe.x = dw / 2;
      rframe.width = mFrameSize.width;
    }
    if (dh > 0){
      rframe.y = dh / 2;
      rframe.height = mFrameSize.height;
    } else {
      rframe.height = mSize.height;
    }
  }
  mFrame = mFrame(rframe);
}

bool VideoStream::isImageBlurry(const cv::Mat& src)
{
  return (laplacianVariance(src) < 70);
}

void VideoStream::resizeFrame()
{
  if (bKeepRatio) {
    float scale = (float)mSize.width / (float)mFrameSize.width;
    mFrameSize.height = int(mSize.height / scale);
  }
  resize(mFrame, mFrame, mFrameSize, 0, 0, cv::INTER_NEAREST); //... Ver tipo de interpolación
}

bool VideoStream::skipFrames(cv::Mat *vf, int frames)
{
  double posframe = mVideoCapture.get(CV_CAP_PROP_POS_FRAMES) + frames - 1.; // -1 porque read ya suma uno
  mVideoCapture.set(CV_CAP_PROP_POS_FRAMES, posframe); 
  return read(vf);
}

bool VideoStream::skipMillisecond(cv::Mat *vf, int ms)
{
  double posms = mVideoCapture.get(CV_CAP_PROP_POS_MSEC);
  mVideoCapture.set(CV_CAP_PROP_POS_MSEC, posms + ms);
  double posframe = mVideoCapture.get(CV_CAP_PROP_POS_FRAMES) - 1.; // -1 porque read ya suma un
  mVideoCapture.set(CV_CAP_PROP_POS_FRAMES, posframe);
  return read(vf);
}

void VideoStream::init()
{
  mSize = cv::Size(0, 0);
  bSkipBlurryFrames = false;
  mResolutionFrame = Resolution::ORIGINAL_FRAME;
  bKeepRatio = true;
  sv = Skip::NOT_SKIP;
  mSkip = 1;
  vs = Status::START;
  mfps = 30;
}

void VideoStream::onFinish() {
  printDebug("Fin captura de video");
  vs = Status::FINALIZED;
  if (!events.empty()) {
    for (auto ev : events) {
      ev->onFinish();
    }
  }
}

void VideoStream::onInitialize() {
  printDebug("Inicio captura video");
  vs = Status::RUNNING;
  if (!events.empty()) {
    for (auto ev : events) {
      ev->onInitialize();
    }
  }
}

void VideoStream::onPause() 
{ 
  if (vs == Status::RUNNING) {
    vs = VideoStream::Status::PAUSE;
    if (!events.empty()) {
      for (auto ev : events) {
        ev->onPause();
      }
    }
  }
}

void VideoStream::onPositionChange(double position)
{ 
  if (!events.empty()) {
    for (auto ev : events) {
      ev->onPositionChange(position);
    }
  }
}

void VideoStream::onRead()
{ 
  if (!nextFrame(NULL)) vs = Status::FINALIZED;
  else if (!events.empty()) {
    for (auto ev : events) {
      ev->onRead(mFrame);
    }
  }
}

void VideoStream::onResume() 
{ 
  if (vs == Status::PAUSE) vs = Status::RUNNING; 
  if (!events.empty()) {
    for (auto ev : events) {
      ev->onResume();
    }
  }
}

void VideoStream::onShow()
{ 
  if (!events.empty()) {
    for (auto ev : events) {
      ev->onShow(mFrame);
    }
  }
}

void VideoStream::onStop()
{ 
  vs = Status::STOPPED;
  if (!events.empty()) {
    for (auto ev : events) {
      ev->onStop();
    }
  }
}

/* ---------------------------------------------------------------------------------- */

bool ImagesStream::open(const char *name)
{
  std::ifstream read_handler(name, std::ifstream::in);
  if (!read_handler.is_open()) {
    printError("Unable to read file: %s", name);
    bIsOpened = false;
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
    }
  }
  return bIsOpened;
}

bool ImagesStream::read(cv::Mat *vf)
{
  if (mCurrentFrame < mImages.size()) {
    std::string imgFile = mImages[static_cast<unsigned int>(mCurrentFrame)];
    mFrame = cv::imread(imgFile);
    printInfo("Imagen: %s", imgFile.c_str());
    if (mResolutionFrame == Resolution::RESIZE_FRAME) {
      resizeFrame();
    } else if (mResolutionFrame == Resolution::CROP_FRAME) {
      cropFrame();
    }
    if (vf) mFrame.copyTo(*vf);
    mCurrentFrame++;
    return true;
  }
  return false;
}

bool ImagesStream::setPosFrame(double nframe)
{ 
  if (nframe > mImages.size()) return false;
  mCurrentFrame = nframe;
  return true;
}

//bool ImagesStream::skipFrames(cv::Mat *vf, int frames)
//{
//  //double posframe = mVideoCapture.get(CV_CAP_PROP_POS_FRAMES) + frames - 1.; // -1 porque read ya suma uno
//  //mVideoCapture.set(CV_CAP_PROP_POS_FRAMES, posframe); 
//  return read(vf);
//}
//
//bool ImagesStream::skipMillisecond(cv::Mat *vf, int ms)
//{
//  //double posms = mVideoCapture.get(CV_CAP_PROP_POS_MSEC);
//  //mVideoCapture.set(CV_CAP_PROP_POS_MSEC, posms + ms);
//  //double posframe = mVideoCapture.get(CV_CAP_PROP_POS_FRAMES) - 1.; // -1 porque read ya suma un
//  //mVideoCapture.set(CV_CAP_PROP_POS_FRAMES, posframe);
//  return read(vf);
//}
/* ---------------------------------------------------------------------------------- */

// Aunque son funciones virtuales puras defino su cuerpo.
// En la clase que herede este la definición de los métodos que no se
// podra llamar a estos métodos de la clase padre si no se quiere modificar el 
// comportamiento por defecto. Podria parecer mas lógico que no fuese virtual pura
// y que se sobreescribiesen sólo los métodos que se desee pero al obligar a 
// implementarlos todos me aseguro un mayor control.

void VideoStreamEvents::onFinish()
{
  printDebug("Fin captura de video");
}

void VideoStreamEvents::onInitialize()
{
  printDebug("Inicio captura video");
}

void VideoStreamEvents::onPause()
{
  printDebug("Video pausado");
}

void VideoStreamEvents::onPositionChange(double position) 
{ 
  printDebug("Posición: %i", static_cast<int>(position));
}

I3D_DISABLE_WARNING(4100)
void VideoStreamEvents::onRead(cv::Mat &frame) 
{ 

}

void VideoStreamEvents::onResume() 
{ 

}

void VideoStreamEvents::onShow(cv::Mat &frame) 
{ 

}
I3D_ENABLE_WARNING(4100)

void VideoStreamEvents::onStop()
{ 
  printDebug("Video detenido");
}

/* ---------------------------------------------------------------------------------- */

VideoWindow::~VideoWindow()
{
  cv::destroyWindow(mWindowName);
}

//Se necesita OpenCV compilado con soporte de QT
void VideoWindow::addButton(const char *name, int type, cv::ButtonCallback onChange, void* userdata, bool buttonState)
{
  cv::createButton(name, onChange, &userdata, type, buttonState);
}

void VideoWindow::addTrackbar(const char *trackbarname, int* value, int count, cv::TrackbarCallback onChange, void *userdata)
{
  cv::createTrackbar(trackbarname, mWindowName, value, count, onChange, userdata);
}

void VideoWindow::setVideo( VideoStream *vs )
{
  mVideo = vs;
  init();
  vs->addListener(this);
}

void VideoWindow::init( )
{
  if (mVideo) {
    mVideoSize = mVideo->getFrameCount();
    int trackbarPos = 0;
    cv::createTrackbar("Frame", mWindowName, &trackbarPos, cvRound(mVideoSize), onTrackbarPositionChange, mVideo);
  }
}

/* ---------------------------------------------------------------------------------- */

} // End namespace I3D
