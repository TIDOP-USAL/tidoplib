#include "VideoStream.h"

#include "core\messages.h"
#include "core\utils.h"
#include "core\mathutils.h"

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

VideoStream::VideoStream( )
{
  init();
}

VideoStream::VideoStream( const char *file )
{
  init();
  open(file);
}

void VideoStream::addListener(Listener *listener)
{ 
  events.push_back(listener);
  //listener->setVideoStream(this);
}

cv::Size VideoStream::getFrameSize()
{
  if (mSize!=cv::Size(0,0)) return mSize;
  else return getSize();
}

cv::Size VideoStream::getSize()
{
  cv::Size sz(0, 0);
  if (isOpened()){
    int width =  (int)mVideoCapture.get(CV_CAP_PROP_FRAME_WIDTH);
    int height = (int)mVideoCapture.get(CV_CAP_PROP_FRAME_HEIGHT);
    sz = cv::Size(width, height);
  }
  return sz;
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
  if (!isOpened()) return false;
  else return true;
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
  if (bret = mVideoCapture.read(mFrame)){
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
      onPositionChange(mVideoCapture.get(CV_CAP_PROP_POS_FRAMES));
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
  mSize  = rframe.size();
  bKeepRatio = keepRatio;
  mResolutionFrame = Resolution::CROP_FRAME;
}

void VideoStream::setFrameSize(cv::Size sz, Resolution rf, bool keepRatio)
{
  mSize = sz;
  bKeepRatio = keepRatio;
  mResolutionFrame = (mSize == getSize()) ? Resolution::ORIGINAL_FRAME : rf;
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
  cv::Size szf;
  if ( rframe == cv::Rect()){
    cv::Size szf = getFrameSize();
    cv::Size szv = getSize();
    int dw = szv.width - szf.width;
    int dh = szv.height - szf.height;
    if (dw > 0){
      rframe.x = dw / 2;
      rframe.width = szf.width;
    }
    if (dh > 0){
      rframe.y = dh / 2;
      rframe.height = szf.height;
    } else {
      rframe.height = szv.height;
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
  cv::Size szv = getSize();
  cv::Size sz(getFrameSize());
  if (bKeepRatio) {
    float scale = (float)szv.width / (float)sz.width;
    sz.height = int(szv.height / scale);
  }
  resize(mFrame, mFrame, sz, 0, 0, cv::INTER_NEAREST); //... Ver tipo de interpolación
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

void VideoStreamEvents::onRead(cv::Mat &frame) 
{ 

}

void VideoStreamEvents::onResume() 
{ 

}

void VideoStreamEvents::onShow(cv::Mat &frame) 
{ 

}

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
//void I3DVideoWindow::addButton(const char *name, int type, cv::ButtonCallback onChange, void* userdata, bool buttonState)
//{
//  cv::createButton(name, onChange, &userdata, type, buttonState);
//}

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