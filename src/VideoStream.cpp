#include "VideoStream.h"

#include "core\messages.h"
#include "utils.h"

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

void OnPositionChange(double pos, void *vw)
{
  VideoWindow *_vw = (VideoWindow *)vw;
  _vw->SetTrackbarPos(pos);
}

void OnFrameReadImage(cv::Mat *frame, void *vw)
{
  VideoWindow *_vw = (VideoWindow *)vw;
  if (_vw && frame && !frame->empty() )
    cv::imshow(_vw->getName(), *frame);
}

//VideoWindow::VideoWindow(const char*_wname, int _flags, bool bPos)
//{
//  wname = _wname;
//  bPosTrackBar = bPos;
//  flags = _flags;
//  video = 0;
//  nFrames = 0;
//  Init();
//}

VideoWindow::~VideoWindow()
{
  cv::destroyWindow(wname);
}

//Se necesita OpenCV compilado con soporte de QT
//void I3DVideoWindow::addButton(const char *name, int type, cv::ButtonCallback onChange, void* userdata, bool buttonState)
//{
//  cv::createButton(name, onChange, &userdata, type, buttonState);
//}

void VideoWindow::addTrackbar(const char *trackbarname, int* value, int count, cv::TrackbarCallback onChange, void *userdata)
{
  cv::createTrackbar(trackbarname, wname, value, count, onChange, userdata);
}

void VideoWindow::SetTrackbarPos(double pos)
{
  //... La posición del trackbar se pasa en entero. para videos muy largos podria desbordarse
  if ( nFrames >= pos )
    cv::setTrackbarPos("Frame", wname, cvRound(pos));
}

void VideoWindow::SetVideo( VideoStream *vs )
{
  video = vs;
  Init();
}

void VideoWindow::Init( )
{
  if (video) {
    nFrames = video->getFrameCount();
    int trackbarPos = 0;
    cv::createTrackbar("Frame", wname, &trackbarPos, cvRound(nFrames), onTrackbarPositionChange, video);
    video->setPositionChangeListener( (PositionChangeCallback)OnPositionChange, this);
    video->setShowListener((ShowCallback)OnFrameReadImage,this);
  }
}

/* ---------------------------------------------------------------------------------- */

//VideoStream::VideoStream( )
//{
//  init();
//}
//
//VideoStream::VideoStream( const char *file )
//{
//  init();
//  open(file);
//}
//
//cv::Size VideoStream::getFrameSize()
//{
//  if (size!=cv::Size(0,0)) return size;
//  else return getSize();
//}
//
//cv::Size VideoStream::getSize()
//{
//  cv::Size sz(0, 0);
//  if (isOpened()){
//    int width =  (int)get(CV_CAP_PROP_FRAME_WIDTH);
//    int height = (int)get(CV_CAP_PROP_FRAME_HEIGHT);
//    sz = cv::Size(width, height);
//  }
//  return sz;
//}
//
//bool VideoStream::nextFrame(cv::Mat &vf)
//{
//  return nextFrame(vf,sv,skip);
//}
//
//bool VideoStream::nextFrame(cv::Mat &vf, skip_video skip, int nskip)
//{
//  bool bret = false;
//  if (skip == skip_video::SKIP_FRAMES)             bret = skipFrames(vf, nskip);
//  else if (skip == skip_video::SKIP_MILLISECONDS)  bret = skipMillisecond(vf, nskip);
//  else                                 bret = read(vf);
//  return bret;
//}
//
//bool VideoStream::open(const char *name)
//{
//  cv::VideoCapture::open(name);
//  if (!isOpened()) return false;
//  else return true;
//}
//
//bool VideoStream::read(cv::Mat &vf)
//{
//  //... Prueba de comportamiento con un video en tiempo real 
////#if defined(_DEBUG)
////  if (get(CV_CAP_PROP_POS_FRAMES) == 0.0) {
////    start = std::clock();
////  } else {
////    int ms = std::clock() - start;
////    set(CV_CAP_PROP_POS_MSEC, ms);
////    int fps = cvRound(get(CV_CAP_PROP_POS_FRAMES));
////    printf("Cargado frame: %i\r", fps);
////  }
////#endif
//  //
//  bool bret = false;
//  if (bret = cv::VideoCapture::read(vf)){
//    while (bSkipBlurryFrames && isImageBlurry(vf)) {
//      bret = cv::VideoCapture::read(vf);
//      if (bret == false) break;
//    }
//  } else {
//    // No ha podido leer el frame. Tratamos de seguir leyendo mientras no lleguemos al final del video.
//    if (vf.empty()) {
//      double posframe = get(CV_CAP_PROP_POS_FRAMES);
//      double duration = get(CV_CAP_PROP_FRAME_COUNT);
//      char c;
//      while (duration > posframe && bret == false) {
//        posframe++;
//        logPrintError("Error al cargar el frame %.lf", posframe);
//        set(CV_CAP_PROP_POS_FRAMES, posframe);
//        bret = cv::VideoCapture::read(vf);
//        c = (char)cv::waitKey(1);
//        if (c == 27) return false;
//      }
//    }
//  }
//  if (bret) {
//    if (resFrame == res_frame::RESIZE_FRAME) {
//      resizeFrame(vf);
//    }
//    else if (resFrame == res_frame::CROP_FRAME) {
//      cropFrame(vf);
//    }
//  }
//  return bret;
//}
//
//void VideoStream::setCropRect(cv::Rect rf, bool keepRatio)
//{
//  rframe = rf;
//  size  = rframe.size();
//  bKeepRatio = keepRatio;
//  resFrame = res_frame::CROP_FRAME;
//}
//
//void VideoStream::setFrameSize(cv::Size sz, res_frame rf, bool keepRatio)
//{
//  size = sz;
//  bKeepRatio = keepRatio;
//  resFrame = (size == getSize()) ? res_frame::ORIGINAL_FRAME : rf;
//}
//
//bool VideoStream::setPosFrame(double nframe)
//{ 
//  double duration = get(CV_CAP_PROP_FRAME_COUNT);
//  if (nframe > duration) return false;
//  set(CV_CAP_PROP_POS_FRAMES, nframe);
//  return true;
//}
//
//void VideoStream::setSkipFrames(int frames)
//{
//  sv = skip_video::SKIP_FRAMES;
//  skip = frames;
//}
//
//void VideoStream::setSkipMillisecond(int ms)
//{
//  sv = skip_video::SKIP_MILLISECONDS;
//  skip = ms;
//}
//
///*********************************************************/
///*                   Controles del video                 */
///*********************************************************/
//
//void VideoStream::run()
//{
//  logPrintDebug("Inicio captura video");
//  vs = video_status::RUNNING;
//  int delay = 100 / cvRound(fps());
//  char c;
//  while (vs != video_status::STOPPED && vs != video_status::FINALIZED) {
//    if (vs != video_status::PAUSE) {
//      if (ReadEvent == NULL ) {
//        cv::Mat vf;
//        if (!nextFrame(vf)) vs = video_status::FINALIZED;
//        if ( ShowEvent )
//          ShowEvent( &vf, UserDataShowEvent );
//      } else {
//        ReadEvent( this, UserDataReadEvent);
//      }
//    }
//    if ( PositionChangeEvent != NULL )
//      PositionChangeEvent(get(CV_CAP_PROP_POS_FRAMES),UserDataPositionChangeEvent);
//    c = (char)cv::waitKey(delay);
//    if (c == 27) vs = video_status::STOPPED;       // - Tecla esc -> Terminar la ejecución del video.
//    else if (c == 43) skipDown();       // - Tecla + -> Aumenta el salto entre frames.
//    else if (c == 45) skipUp();         // - Tecla - -> Disminuye el salto entre frames.
//    else if (c == 32) {                 // - Barra espaciadora -> Pausa/reanuda la ejecución del video.
//      if (vs == video_status::RUNNING) pause();
//      else resume();
//    }
//  }
//  vs = video_status::FINALIZED;
//  logPrintDebug("Fin captura de video");
//}
//
//void VideoStream::skipDown()
//{
//  if (sv == skip_video::NOT_SKIP) sv = skip_video::SKIP_FRAMES;
//  if (sv == skip_video::SKIP_FRAMES) {
//    if (skip == -1){
//      skip = 1;
//    } else if ( skip != 16 ) {
//      if ( skip > 0 ) skip *= 2;
//      else skip /= 2;
//    }
//  } else if ( sv == skip_video::SKIP_MILLISECONDS ) {
//    int n = cvRound(skip/fps());
//    if (n == -1){
//      skip = 1;
//    } else if ( n != 16 ) {
//      if ( n > 0 ) skip *= 2;
//      else skip /= 2;
//    }
//  }
//}
//
//void VideoStream::skipUp()
//{
//  if (sv == skip_video::NOT_SKIP) sv = skip_video::SKIP_FRAMES;
//  if (sv == skip_video::SKIP_FRAMES) {
//    if (skip == 1){
//      skip = -2;
//    } else if ( skip != -16 ) {
//      if ( skip < 0 ) skip *= 2;
//      else skip /= 2;
//    }
//  } else if ( sv == skip_video::SKIP_MILLISECONDS ) {
//    int n = cvRound(skip/fps());
//    if (n == 1){
//      skip = -2;
//    } else if ( n != -16 ) {
//      if ( n < 0 ) skip *= 2;
//      else skip /= 2;
//    }
//  }
//}
//
///*********************************************************/
///*                       Eventos                         */
///*********************************************************/
//
//void VideoStream::setReadListener( ReadCallback ev_r, void* userdata )
//{
//  ReadEvent = ev_r;
//  UserDataReadEvent = userdata;
//}
//
//void VideoStream::setPositionChangeListener(PositionChangeCallback ev_pc, void* userdata)
//{
//  PositionChangeEvent = ev_pc;
//  UserDataPositionChangeEvent = userdata;
//}
//
//void VideoStream::setShowListener(ShowCallback ev_s, void *userdata)
//{
//  ShowEvent = ev_s;
//  UserDataShowEvent = userdata;
//}
//
//bool VideoStream::show(cv::Mat &vf)
//{
//  if (ShowEvent && !vf.empty() ) {
//    ShowEvent(&vf, UserDataShowEvent);
//    return true;
//  } else return false;
//}
//
///*********************************************************/
///*                Métodos privados                       */
///*********************************************************/
//
//void VideoStream::cropFrame(cv::Mat &vf)
//{
//  cv::Size szf;
//  if ( rframe == cv::Rect()){
//    cv::Size szf = getFrameSize();
//    cv::Size szv = getSize();
//    int dw = szv.width - szf.width;
//    int dh = szv.height - szf.height;
//    if (dw > 0){
//      rframe.x = dw / 2;
//      rframe.width = szf.width;
//    }
//    if (dh > 0){
//      rframe.y = dh / 2;
//      rframe.height = szf.height;
//    } else {
//      rframe.height = szv.height;
//    }
//  }
//  vf = vf(rframe);
//}
//
//bool VideoStream::isImageBlurry(const cv::Mat& src)
//{
//  return (laplacianVariance(src) < 70);
//}
//
//void VideoStream::resizeFrame(cv::Mat &vf)
//{
//  cv::Size szv = getSize();
//  cv::Size sz(getFrameSize());
//  if (bKeepRatio) {
//    float scale = (float)szv.width / (float)sz.width;
//    sz.height = int(szv.height / scale);
//  }
//  resize(vf, vf, sz, 0, 0, cv::INTER_NEAREST); //... Ver tipo de interpolación
//}
//
//bool VideoStream::skipFrames(cv::Mat &vf, int frames)
//{
//  double posframe = get(CV_CAP_PROP_POS_FRAMES) + frames - 1.; // -1 porque read ya suma uno
//  set(CV_CAP_PROP_POS_FRAMES, posframe); 
//  return read(vf);
//}
//
//bool VideoStream::skipMillisecond(cv::Mat &vf, int ms)
//{
//  double posms = get(CV_CAP_PROP_POS_MSEC);
//  set(CV_CAP_PROP_POS_MSEC, posms + ms);
//  double posframe = get(CV_CAP_PROP_POS_FRAMES) - 1.; // -1 porque read ya suma un
//  set(CV_CAP_PROP_POS_FRAMES, posframe);
//  return read(vf);
//}
//
//void VideoStream::init()
//{
//  size = cv::Size(0, 0);
//  bSkipBlurryFrames = false;
//  resFrame = res_frame::ORIGINAL_FRAME;
//  bKeepRatio = true;
//  sv = skip_video::NOT_SKIP;
//  skip = 1;
//  vs = video_status::START;
//  ReadEvent = NULL;
//  PositionChangeEvent = NULL;
//  ShowEvent = NULL;
//}



VideoStream::VideoStream( )
{
  init();
}

VideoStream::VideoStream( const char *file )
{
  init();
  open(file);
}

cv::Size VideoStream::getFrameSize()
{
  if (size!=cv::Size(0,0)) return size;
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

bool VideoStream::nextFrame(cv::Mat &vf)
{
  return nextFrame(vf,sv,skip);
}

bool VideoStream::nextFrame(cv::Mat &vf, VideoStream::Skip skip, int nskip)
{
  bool bret = false;
  if (skip == VideoStream::Skip::SKIP_FRAMES)             bret = skipFrames(vf, nskip);
  else if (skip == VideoStream::Skip::SKIP_MILLISECONDS)  bret = skipMillisecond(vf, nskip);
  else                                 bret = read(vf);
  return bret;
}

bool VideoStream::open(const char *name)
{
  mVideoCapture.open(name);
  if (!isOpened()) return false;
  else return true;
}

bool VideoStream::read(cv::Mat &vf)
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
  if (bret = mVideoCapture.read(vf)){
    while (bSkipBlurryFrames && isImageBlurry(vf)) {
      bret = mVideoCapture.read(vf);
      if (bret == false) break;
    }
  } else {
    // No ha podido leer el frame. Tratamos de seguir leyendo mientras no lleguemos al final del video.
    if (vf.empty()) {
      double posframe = mVideoCapture.get(CV_CAP_PROP_POS_FRAMES);
      double duration = mVideoCapture.get(CV_CAP_PROP_FRAME_COUNT);
      char c;
      while (duration > posframe && bret == false) {
        posframe++;
        logPrintError("Error al cargar el frame %.lf", posframe);
        mVideoCapture.set(CV_CAP_PROP_POS_FRAMES, posframe);
        bret = mVideoCapture.read(vf);
        c = (char)cv::waitKey(1);
        if (c == 27) return false;
      }
    }
  }
  if (bret) {
    if (resFrame == Resolution::RESIZE_FRAME) {
      resizeFrame(vf);
    }
    else if (resFrame == Resolution::CROP_FRAME) {
      cropFrame(vf);
    }
  }
  return bret;
}

void VideoStream::setCropRect(cv::Rect rf, bool keepRatio)
{
  rframe = rf;
  size  = rframe.size();
  bKeepRatio = keepRatio;
  resFrame = Resolution::CROP_FRAME;
}

void VideoStream::setFrameSize(cv::Size sz, Resolution rf, bool keepRatio)
{
  size = sz;
  bKeepRatio = keepRatio;
  resFrame = (size == getSize()) ? Resolution::ORIGINAL_FRAME : rf;
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
  skip = frames;
}

void VideoStream::setSkipMillisecond(int ms)
{
  sv = Skip::SKIP_MILLISECONDS;
  skip = ms;
}

/*********************************************************/
/*                   Controles del video                 */
/*********************************************************/

void VideoStream::run()
{
  logPrintDebug("Inicio captura video");
  vs = Status::RUNNING;
  int delay = 100 / cvRound(fps());
  char c;
  while (vs != Status::STOPPED && vs != Status::FINALIZED) {
    if (vs != Status::PAUSE) {
      if (ReadEvent == NULL ) {
        cv::Mat vf;
        if (!nextFrame(vf)) vs = Status::FINALIZED;
        if ( ShowEvent )
          ShowEvent( &vf, UserDataShowEvent );
      } else {
        ReadEvent( this, UserDataReadEvent);
      }
    }
    if (PositionChangeEvent != NULL) {
      PositionChangeEvent(mVideoCapture.get(CV_CAP_PROP_POS_FRAMES),UserDataPositionChangeEvent);
    }
    notifyOnPositionChange(mVideoCapture.get(CV_CAP_PROP_POS_FRAMES));

    c = (char)cv::waitKey(delay);
    if (c == 27) vs = Status::STOPPED;       // - Tecla esc -> Terminar la ejecución del video.
    else if (c == 43) skipDown();       // - Tecla + -> Aumenta el salto entre frames.
    else if (c == 45) skipUp();         // - Tecla - -> Disminuye el salto entre frames.
    else if (c == 32) {                 // - Barra espaciadora -> Pausa/reanuda la ejecución del video.
      if (vs == Status::RUNNING) pause();
      else resume();
    }
  }
  vs = Status::FINALIZED;
  logPrintDebug("Fin captura de video");
}

void VideoStream::skipDown()
{
  if (sv == Skip::NOT_SKIP) sv = Skip::SKIP_FRAMES;
  if (sv == Skip::SKIP_FRAMES) {
    if (skip == -1){
      skip = 1;
    } else if ( skip != 16 ) {
      if ( skip > 0 ) skip *= 2;
      else skip /= 2;
    }
  } else if ( sv == Skip::SKIP_MILLISECONDS ) {
    int n = cvRound(skip/fps());
    if (n == -1){
      skip = 1;
    } else if ( n != 16 ) {
      if ( n > 0 ) skip *= 2;
      else skip /= 2;
    }
  }
}

void VideoStream::skipUp()
{
  if (sv == Skip::NOT_SKIP) sv = Skip::SKIP_FRAMES;
  if (sv == Skip::SKIP_FRAMES) {
    if (skip == 1){
      skip = -2;
    } else if ( skip != -16 ) {
      if ( skip < 0 ) skip *= 2;
      else skip /= 2;
    }
  } else if ( sv == Skip::SKIP_MILLISECONDS ) {
    int n = cvRound(skip/fps());
    if (n == 1){
      skip = -2;
    } else if ( n != -16 ) {
      if ( n < 0 ) skip *= 2;
      else skip /= 2;
    }
  }
}

/*********************************************************/
/*                       Eventos                         */
/*********************************************************/

void VideoStream::setReadListener( ReadCallback ev_r, void* userdata )
{
  ReadEvent = ev_r;
  UserDataReadEvent = userdata;
}

void VideoStream::setPositionChangeListener(PositionChangeCallback ev_pc, void* userdata)
{
  PositionChangeEvent = ev_pc;
  UserDataPositionChangeEvent = userdata;
}

void VideoStream::setShowListener(ShowCallback ev_s, void *userdata)
{
  ShowEvent = ev_s;
  UserDataShowEvent = userdata;
}

bool VideoStream::show(cv::Mat &vf)
{
  if (ShowEvent && !vf.empty() ) {
    ShowEvent(&vf, UserDataShowEvent);
    return true;
  } else return false;
}

/*********************************************************/
/*                Métodos privados                       */
/*********************************************************/

void VideoStream::cropFrame(cv::Mat &vf)
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
  vf = vf(rframe);
}

bool VideoStream::isImageBlurry(const cv::Mat& src)
{
  return (laplacianVariance(src) < 70);
}

void VideoStream::resizeFrame(cv::Mat &vf)
{
  cv::Size szv = getSize();
  cv::Size sz(getFrameSize());
  if (bKeepRatio) {
    float scale = (float)szv.width / (float)sz.width;
    sz.height = int(szv.height / scale);
  }
  resize(vf, vf, sz, 0, 0, cv::INTER_NEAREST); //... Ver tipo de interpolación
}

bool VideoStream::skipFrames(cv::Mat &vf, int frames)
{
  double posframe = mVideoCapture.get(CV_CAP_PROP_POS_FRAMES) + frames - 1.; // -1 porque read ya suma uno
  mVideoCapture.set(CV_CAP_PROP_POS_FRAMES, posframe); 
  return read(vf);
}

bool VideoStream::skipMillisecond(cv::Mat &vf, int ms)
{
  double posms = mVideoCapture.get(CV_CAP_PROP_POS_MSEC);
  mVideoCapture.set(CV_CAP_PROP_POS_MSEC, posms + ms);
  double posframe = mVideoCapture.get(CV_CAP_PROP_POS_FRAMES) - 1.; // -1 porque read ya suma un
  mVideoCapture.set(CV_CAP_PROP_POS_FRAMES, posframe);
  return read(vf);
}

void VideoStream::init()
{
  size = cv::Size(0, 0);
  bSkipBlurryFrames = false;
  resFrame = Resolution::ORIGINAL_FRAME;
  bKeepRatio = true;
  sv = Skip::NOT_SKIP;
  skip = 1;
  vs = Status::START;
  ReadEvent = NULL;
  PositionChangeEvent = NULL;
  ShowEvent = NULL;
}

} // End namespace I3D