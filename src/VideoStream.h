#pragma once
#ifndef I3D_VIDEO_STREAM_H
#define I3D_VIDEO_STREAM_H

#include <ctime>

#include "opencv2/highgui/highgui.hpp"

#include "core\defs.h"
#include "core\events.h"

namespace I3D
{

// forward declarations
class I3D_EXPORT VideoStream;

/*!
 * \brief Clase para la visualización de video
 */
class I3D_EXPORT VideoWindow
{
private:
  /*!
   * \brief Nombre de la ventana de video
   */
  std::string wname;

  /*!
   * \brief Añade una barra de posición del video
   */
  bool bPosTrackBar;

  /*!
   * \brief flags de ventana de OpenCV
   */
  int flags;

  /*!
   * \brief Número de frames del video
   */
  double nFrames;

  /*!
   * \brief Refefencia al video que se esta reproduciendo
   */
  VideoStream *video;

public:
  /*!
   * \brief Constructora por defecto  de la clase VideoWindow
   */
  VideoWindow() 
    : wname("Video"), flags(1), bPosTrackBar(true), nFrames(0), video(0) 
  {
    cv::namedWindow(wname, flags);
  }

  /*!
   * \brief Constructora de la clase VideoWindow
   * \param[in] wname Nombre de la ventana de video
   * \param[in] flags
   * \param[in] bPos Muestra slider con posición del video
   */
  VideoWindow(const char* wname, int flags = CV_WINDOW_AUTOSIZE, bool bPos = true)
    : wname(wname), flags(flags), bPosTrackBar(bPos), nFrames(0), video(0) 
  { 
    cv::namedWindow(wname, flags);
  }

  /*!
   * \brief Destructora de la clase I3DVideoCtrls
   */
  ~VideoWindow();

  /*!
   * \brief Añade un trackbar a la ventana de video
   * \param[in] trackbarname Nombre del trackbar
   * \param[in] value Puntero opcional a una variable entera cuyo valor representa la posición del slider.
   * \param[in] count Posición máxima del slider
   * \param[in] onChange Manejador del evento onChange.
   * \param[in] userdata Puntero para pasar parametros al manejador del evento onChange. 
   */
  void addTrackbar(const char *trackbarname, int *value, int count, cv::TrackbarCallback onChange = 0, void* userdata = 0);

  /*!
   * \brief Devuelve una referencia al nombre de la ventana
   */
  const std::string &getName() const {
    return wname;
  }

  /*!
   * \brief Cambia la posición en el trackbar
   * \param[in] pos Nueva posición
   */
  void SetTrackbarPos(double pos);

  /*!
   * \brief Añade una referencia a un objeto VideoStream
   */
  void SetVideo(VideoStream *vs);

private:

  /*!
   * \brief Init
   */
  void Init();
};



///*!
// * \brief Tipos salto de frames
// */
//enum class skip_video{
//  NOT_SKIP,            /*!< Frame de video uno a uno. */
//  SKIP_FRAMES,         /*!< Carga un frame de cada n. */
//  SKIP_MILLISECONDS    /*!< Carga un frame cada n milisegundos. */
//};
//
///*!
// * \brief Tipo de resolución del frame de video.
// */
//enum class res_frame {
//  ORIGINAL_FRAME,   /*!< Frame del tamaño original del video. */
//  RESIZE_FRAME,     /*!< Redimensiona el frame de video. */
//  CROP_FRAME        /*!< Recorta el frame de video. */
//};
//
///*!
// * \brief Estado del video
// */
//enum class video_status {
//  START,      /*!< Inicio */
//  RUNNING,    /*!< Video ejecutandose */
//  PAUSE,      /*!< Video en pausa */
//  STOPPED,    /*!< Video detenido */
//  FINALIZED   /*!< Video finalizado */
//};
//
//// Eventos
//typedef void(*ReadCallback)(void *, void* userdata);
//typedef void(*PositionChangeCallback)(double pos, void *userdata);
//typedef void(*ShowCallback)(cv::Mat *frame, void *userdata);

///*!
// * \brief Clase para el manejo de video
// */
//class I3D_EXPORT VideoStream : private cv::VideoCapture
//{
//private:
//  /*!
//   * \brief Tamaño de los frames del video
//   */
//  cv::Size size;
//
//  /*!
//   * \brief Región de recorte del video
//   */
//  cv::Rect rframe;
//
//  /*!
//   * \brief Desecha los frames borrosos
//   */
//  bool bSkipBlurryFrames;
//
//  /*!
//   * \brief Mantiene la proporción al redimensionar una imagen
//   */
//  bool bKeepRatio; 
//
//  /*!
//   * \brief Tipo de resolución del frame de video.
//   * \see res_frame
//   */
//  res_frame resFrame;
//
//  /*!
//   * \brief Tipo de salto de frames aplicado
//   * \see skip_video
//   */
//  skip_video sv;
//
//  /*!
//   * \brief Salto de frames o de milisegundos con que se cargan los frames
//   */
//  int skip;
//
//  /*!
//   * \brief start
//   */
//  //std::clock_t start; Para prueba de comportamiento del video en tiempo real
//
//  /*!
//   * \brief Puntero al manejador definido para el evento Read
//   */
//  ReadCallback ReadEvent;
//
//  /*!
//   * \brief Puntero al manejador definido para el evento PositionChange
//   */
//  PositionChangeCallback PositionChangeEvent;
//
//  /*!
//   * \brief Puntero al manejador definido para el evento ShowEvent
//   */
//  ShowCallback ShowEvent;
//
//  /*!
//   * \brief UserDataReadEvent
//   */
//  void *UserDataReadEvent;
//
//  /*!
//   * \brief UserDataPositionChangeEvent
//   */
//  void *UserDataPositionChangeEvent;
//
//  /*!
//   * \brief UserDataShowEvent
//   */
//  void *UserDataShowEvent;
//
//  /*!
//   * \brief vs Estado de ejecución del video
//   * \see video_status
//   */
//  video_status vs;
//
//public:
//
//  /*!
//   * \brief Constructora I3DVideoStream
//   */
//  VideoStream( );
//
//  /*!
//   * \brief Constructora I3DVideoStream
//   * \param[in] file Video
//   */
//  VideoStream(const char *file);
//
//  /*!
//   * \brief Destructora I3DVideoStream
//   */
//  ~VideoStream(){}
//
//  /*!
//   * \brief Frames por segundo del video
//   * \return Frames por segundo
//   */
//  double fps() { return get(CV_CAP_PROP_FPS); }
//
//  /*!
//   * \brief Frame de video actual
//   * \return
//   */
//  double getCurrentFrame() { return get(CV_CAP_PROP_POS_FRAMES); }
//
//  /*!
//   * \brief Número de frames del video
//   * \return
//   */
//  double getFrameCount() { return get(CV_CAP_PROP_FRAME_COUNT); }
//
//  /*!
//   * \brief Devuelve el ancho y alto de los frames de salida
//   * \return Tamaño de los frames de salida
//   */
//  cv::Size getFrameSize();
//
//  /*!
//   * \brief Ancho y alto del video
//   * \return Tamaño del video
//   */
//  cv::Size getSize();
//
//  /*!
//  * \brief Comprueba si el video está abierto
//  * \return Verdadero si el video está abierto
//  */
//  bool isOpened() { return cv::VideoCapture::isOpened(); }
//
//  /*!
//   * \brief NextFrame
//   * \param[in] vf Frame de video
//   * \return Verdadero si encuentra frame
//   */
//  bool nextFrame(cv::Mat &vf);
//
//  /*!
//   * \brief Siguiente frame del video
//   * \param[in] vf Frame de video
//   * \param[in] skip Tipo de salto de video aplicado
//   * \param[in] nskip Número de salto
//   * \return Verdadero si encuentra frame
//   */
//  bool nextFrame(cv::Mat &vf, skip_video skip, int nskip);
//
//  /*!
//   * \brief Abre el video
//   * \param[in] name Nombre del video
//   * \return verdadero si el video se ha abierto
//   */
//  bool open(const char *name);
//
//  /*!
//   * \brief Lee un frame de video
//   * \param[in] vf Frame de video
//   * \return Falso si no encuentra ningún frame
//   */
//  bool read(cv::Mat &vf);
//
//  /*!
//   * \brief Establece la región de recorte del video
//   * \param[in] rf Región de recorte del frame
//   * \param[in] keepRatio Mantiene las dimensiones
//   */
//  void setCropRect(cv::Rect rf, bool keepRatio = true);
//
//  /*!
//   * \brief Permite redimensionar la salida del video
//   * \param[in] sz Tamaño de salida de video
//   * \param[in] rf Redimensión, recorte o tamaño original del video
//   * \param[in] keepRatio Mantiene las dimensiones
//   */
//  void setFrameSize(cv::Size sz, res_frame rf = res_frame::RESIZE_FRAME, bool keepRatio = true);
//
//  /*!
//   * \brief Establece la posición en el video según el número de frame
//   * \param[in] nframe
//   * \return
//   */
//  bool setPosFrame(double nframe);
//
//  /*!
//   * \brief Comprueba si esta activado el salto de frames borrosos
//   * \return Verdadero si esta activado
//   */
//  bool isSkipBlurryFrames() { return bSkipBlurryFrames;  }
//
//  /*!
//   * \brief Activa el salto de frames borrosos
//   * \param[in] Verdadero para activar el salto de frames borrosos
//   */
//  void setSkipBlurryFrames(bool sbf) { bSkipBlurryFrames = sbf; }
//
//  /*!
//   * \brief Establece el salto de frames en la lectura del video
//   * \param frames Número de frames que salta
//   */
//  void setSkipFrames(int frames);
//
//  /*!
//   * \brief Establece el número de milisegundos que salta el video.
//   * \param[in] ms Número de milisegundos que salta
//   */
//  void setSkipMillisecond(int ms);
//
//  // Controles de ejecución del video
//
////  /*!
////   * \brief Adelanta el video
////   */
////  void Forward();
//
//  /*!
//   * \brief Rausa el video
//   */
//  void pause() { vs = video_status::PAUSE; }
//
//  /*!
//   * \brief Continua la ejecución del video
//   */
//  void resume() { if (vs == video_status::PAUSE) vs = video_status::RUNNING; }
//
//  /*!
//   * \brief Arranca el video
//   *
//   * Ejecuta el video desde el principio. Se puede interactuar con el video
//   * mediante las teclas:
//   * - Tecla esc: Terminar la ejecución del video.
//   * - Tecla +: Aumenta el salto entre frames.
//   * - Tecla -: Disminuye el salto entre frames.
//   * - Barra espaciadora: Pausa/reanuda la ejecución del video.
//   */
//  void run();
//
//  /*!
//   * \brief Disminuye el salto de frames
//   */
//  void skipDown();
//
//  /*!
//   * \brief Aumenta el salto de frames
//   */
//  void skipUp();
//
//  /*!
//   * \brief Detiene la ejecución del video
//   */
//  void stop() { vs = video_status::STOPPED; }
//
////  /*!
////   * \brief Retrasa el video
////   */
//  //void Rewind();
//
//  // eventos
//
//  /*!
//   * \brief Establece un escuchador para el evento OnRead
//   * \param[in] ev_r Función que escucha el evento
//   * \param[in] userdata Puntero a datos de usuario
//   */
//  void setReadListener(ReadCallback ev_r = 0, void *userdata = 0);
//
//  /*!
//   * \brief Establece un escuchador para el evento OnPositionChange
//   * \param[in] ev_pc Función que escucha el evento
//   * \param[in] userdata Puntero a datos de usuario
//   */
//  void setPositionChangeListener( PositionChangeCallback ev_pc = 0, void *userdata = 0 );
//
//  /*!
//   * \brief Establece un escuchador para el evento OnShow
//   * \param[in] ev_s Función que escucha el evento
//   * \param[in] userdata Puntero a datos de usuario
//   */
//  void setShowListener(ShowCallback ev_s = 0, void *userdata = 0);
//
//  /*!
//   * \brief Lanza el evento Show
//   * \param[in] vf Frame de video
//   * \return Si hay algún listener para recogerlo devuelve verdadero
//   */
//  bool show(cv::Mat &vf);
//
//  // Métodos privados
//private:
//  /*!
//   * \brief Recorta el frame de video
//   * \param[in] vf Frame de video
//   */
//  void cropFrame(cv::Mat &vf);
//
//  /*!
//   * \brief Detecta imagenes borrosas con la varianza del laplaciano
//   * \param[in] src Imagen en la que se comprueba si la imagen es borrosa
//   * \return verdadero si la imagen esta borrosa.
//   */
//  bool isImageBlurry(const cv::Mat& src);
//
//  /*!
//   * \brief Redimensiona el frame de video
//   * \param[in] vf Frame de video
//   */
//  void resizeFrame(cv::Mat &vf);
//
//  /*!
//  * \brief Salto de n frames
//  * \param[in] vf Frame de video
//  * \param[in] frames Número de frames que salta
//  * \return Verdadero mientras encuentre otro frame
//  */
//  bool skipFrames(cv::Mat &vf, int frames);
//
//  /*!
//  * \brief Salto de n milisegundos
//  * \param[in] vf Frame de video
//  * \param[in] ms Número de milisegundos que salta
//  * \return Verdadero mientras encuentre otro frame
//  */
//  bool skipMillisecond(cv::Mat &vf, int ms);
//
//  void init();
//};

class Read : public Event
{
public:
  Read() : Event("Read")
  {}

  ~Read()
  {}

private:

};

class PositionChange : public Event
{
public:
  PositionChange() : Event("PositionChange")
  {}

  ~PositionChange()
  {}

private:

};

class Show : public Event
{
public:
  Show() : Event("Show")
  {}

  ~Show()
  {}

private:

};

class VideoStreamEvents
{

public:
  VideoStreamEvents()
  {}

  ~VideoStreamEvents()
  {}

  virtual void onRead() = 0;
  virtual void onPositionChange() = 0;
  virtual void onShow() = 0;
};


//class ObserverRead : public Observer
//{
//public:
//  ObserverRead()
//  {
//  }
//
//  virtual ~ObserverRead()
//  {
//  }
//
//  virtual void operator()() = 0;
//};
//
//class ObserverPositionChange : public Observer
//{
//public:
//  ObserverPositionChange()
//  {
//  }
//
//  virtual ~ObserverPositionChange()
//  {
//  }
//
//  virtual void operator()(double position) = 0;
//};
//
//class ObserverShow : public Observer
//{
//public:
//  ObserverShow()
//  {
//  }
//
//  virtual ~ObserverShow()
//  {
//  }
//
//  virtual void operator()() = 0;
//};

// Eventos
typedef void(*ReadCallback)(void *, void* userdata);
typedef void(*PositionChangeCallback)(double pos, void *userdata);
typedef void(*ShowCallback)(cv::Mat *frame, void *userdata);


/*!
 * \brief Clase para el manejo de video
 */
class I3D_EXPORT VideoStream /*: public Subject*/
{

public:

  /*!
   * \brief Tipos salto de frames
   */
  enum class Skip {
    NOT_SKIP,            /*!< Frame de video uno a uno. */
    SKIP_FRAMES,         /*!< Carga un frame de cada n. */
    SKIP_MILLISECONDS    /*!< Carga un frame cada n milisegundos. */
  };

  /*!
   * \brief Tipo de resolución del frame de video.
   */
  enum class Resolution {
    ORIGINAL_FRAME,   /*!< Frame del tamaño original del video. */
    RESIZE_FRAME,     /*!< Redimensiona el frame de video. */
    CROP_FRAME        /*!< Recorta el frame de video. */
  };

  /*!
   * \brief Estado del video
   */
  enum class Status {
    START,      /*!< Inicio */
    RUNNING,    /*!< Video ejecutandose */
    PAUSE,      /*!< Video en pausa */
    STOPPED,    /*!< Video detenido */
    FINALIZED   /*!< Video finalizado */
  };

  /*!
   * \brief Eventos
   */
  //static const int ON_READ = 0;
  //static const int ON_POSITION_CHANGE = 1;
  //static const int ON_SHOW = 2;

  typedef VideoStreamEvents Listener;

  //typedef ObserverPositionChange OnPositionChange;
  //typedef ObserverRead OnRead;
  //typedef ObserverShow OnShow;

private:

  cv::VideoCapture mVideoCapture;

  /*!
   * \brief Tamaño de los frames del video
   */
  cv::Size size;

  /*!
   * \brief Región de recorte del video
   */
  cv::Rect rframe;

  /*!
   * \brief Desecha los frames borrosos
   */
  bool bSkipBlurryFrames;

  /*!
   * \brief Mantiene la proporción al redimensionar una imagen
   */
  bool bKeepRatio; 

  /*!
   * \brief Tipo de resolución del frame de video.
   * \see res_frame
   */
  VideoStream::Resolution resFrame;

  /*!
   * \brief Tipo de salto de frames aplicado
   * \see skip_video
   */
  VideoStream::Skip sv;

  /*!
   * \brief Salto de frames o de milisegundos con que se cargan los frames
   */
  int skip;

  /*!
   * \brief start
   */
  //std::clock_t start; Para prueba de comportamiento del video en tiempo real

  /*!
   * \brief Puntero al manejador definido para el evento Read
   */
  ReadCallback ReadEvent;

  /*!
   * \brief Puntero al manejador definido para el evento PositionChange
   */
  PositionChangeCallback PositionChangeEvent;

  /*!
   * \brief Puntero al manejador definido para el evento ShowEvent
   */
  ShowCallback ShowEvent;

  /*!
   * \brief UserDataReadEvent
   */
  void *UserDataReadEvent;

  /*!
   * \brief UserDataPositionChangeEvent
   */
  void *UserDataPositionChangeEvent;

  /*!
   * \brief UserDataShowEvent
   */
  void *UserDataShowEvent;

  /*!
   * \brief vs Estado de ejecución del video
   * \see video_status
   */
  VideoStream::Status vs;

  Listener *events;

public:

  /*!
   * \brief Constructora I3DVideoStream
   */
  VideoStream( );

  /*!
   * \brief Constructora I3DVideoStream
   * \param[in] file Video
   */
  VideoStream(const char *file);

  /*!
   * \brief Destructora I3DVideoStream
   */
  ~VideoStream(){}

  /*!
   * \brief Frames por segundo del video
   * \return Frames por segundo
   */
  double fps() { return mVideoCapture.get(CV_CAP_PROP_FPS); }

  /*!
   * \brief Frame de video actual
   * \return
   */
  double getCurrentFrame() { return mVideoCapture.get(CV_CAP_PROP_POS_FRAMES); }

  /*!
   * \brief Número de frames del video
   * \return
   */
  double getFrameCount() { return mVideoCapture.get(CV_CAP_PROP_FRAME_COUNT); }

  /*!
   * \brief Devuelve el ancho y alto de los frames de salida
   * \return Tamaño de los frames de salida
   */
  cv::Size getFrameSize();

  /*!
   * \brief Ancho y alto del video
   * \return Tamaño del video
   */
  cv::Size getSize();

  /*!
  * \brief Comprueba si el video está abierto
  * \return Verdadero si el video está abierto
  */
  bool isOpened() { return mVideoCapture.isOpened(); }

  /*!
   * \brief NextFrame
   * \param[in] vf Frame de video
   * \return Verdadero si encuentra frame
   */
  bool nextFrame(cv::Mat &vf);

  /*!
   * \brief Siguiente frame del video
   * \param[in] vf Frame de video
   * \param[in] skip Tipo de salto de video aplicado
   * \param[in] nskip Número de salto
   * \return Verdadero si encuentra frame
   */
  bool nextFrame(cv::Mat &vf, VideoStream::Skip skip, int nskip);

  /*!
   * \brief Abre el video
   * \param[in] name Nombre del video
   * \return verdadero si el video se ha abierto
   */
  bool open(const char *name);

  /*!
   * \brief Lee un frame de video
   * \param[in] vf Frame de video
   * \return Falso si no encuentra ningún frame
   */
  bool read(cv::Mat &vf);

  /*!
   * \brief Establece la región de recorte del video
   * \param[in] rf Región de recorte del frame
   * \param[in] keepRatio Mantiene las dimensiones
   */
  void setCropRect(cv::Rect rf, bool keepRatio = true);

  /*!
   * \brief Permite redimensionar la salida del video
   * \param[in] sz Tamaño de salida de video
   * \param[in] rf Redimensión, recorte o tamaño original del video
   * \param[in] keepRatio Mantiene las dimensiones
   */
  void setFrameSize(cv::Size sz, VideoStream::Resolution rf = VideoStream::Resolution::RESIZE_FRAME, bool keepRatio = true);

  /*!
   * \brief Establece la posición en el video según el número de frame
   * \param[in] nframe
   * \return
   */
  bool setPosFrame(double nframe);

  /*!
   * \brief Comprueba si esta activado el salto de frames borrosos
   * \return Verdadero si esta activado
   */
  bool isSkipBlurryFrames() { return bSkipBlurryFrames;  }

  /*!
   * \brief Activa el salto de frames borrosos
   * \param[in] Verdadero para activar el salto de frames borrosos
   */
  void setSkipBlurryFrames(bool sbf) { bSkipBlurryFrames = sbf; }

  /*!
   * \brief Establece el salto de frames en la lectura del video
   * \param frames Número de frames que salta
   */
  void setSkipFrames(int frames);

  /*!
   * \brief Establece el número de milisegundos que salta el video.
   * \param[in] ms Número de milisegundos que salta
   */
  void setSkipMillisecond(int ms);

  // Controles de ejecución del video

//  /*!
//   * \brief Adelanta el video
//   */
//  void Forward();

  /*!
   * \brief Rausa el video
   */
  void pause() { vs = VideoStream::Status::PAUSE; }

  /*!
   * \brief Continua la ejecución del video
   */
  void resume() { if (vs == VideoStream::Status::PAUSE) vs = VideoStream::Status::RUNNING; }

  /*!
   * \brief Arranca el video
   *
   * Ejecuta el video desde el principio. Se puede interactuar con el video
   * mediante las teclas:
   * - Tecla esc: Terminar la ejecución del video.
   * - Tecla +: Aumenta el salto entre frames.
   * - Tecla -: Disminuye el salto entre frames.
   * - Barra espaciadora: Pausa/reanuda la ejecución del video.
   */
  void run();

  /*!
   * \brief Disminuye el salto de frames
   */
  void skipDown();

  /*!
   * \brief Aumenta el salto de frames
   */
  void skipUp();

  /*!
   * \brief Detiene la ejecución del video
   */
  void stop() { vs = VideoStream::Status::STOPPED; }

//  /*!
//   * \brief Retrasa el video
//   */
  //void Rewind();

  // eventos

  /*!
   * \brief Establece un escuchador para el evento OnRead
   * \param[in] ev_r Función que escucha el evento
   * \param[in] userdata Puntero a datos de usuario
   */
  void setReadListener(ReadCallback ev_r = 0, void *userdata = 0);

  /*!
   * \brief Establece un escuchador para el evento OnPositionChange
   * \param[in] ev_pc Función que escucha el evento
   * \param[in] userdata Puntero a datos de usuario
   */
  void setPositionChangeListener( PositionChangeCallback ev_pc = 0, void *userdata = 0 );

  /*!
   * \brief Establece un escuchador para el evento OnShow
   * \param[in] ev_s Función que escucha el evento
   * \param[in] userdata Puntero a datos de usuario
   */
  void setShowListener(ShowCallback ev_s = 0, void *userdata = 0);

  // Nuevo sistema de gestion de eventos
  //void addOnPositionChangeListener(std::shared_ptr<OnPositionChange> onPositionChange) { addObserver(ON_POSITION_CHANGE,onPositionChange); }
  //void addOnReadListener(std::shared_ptr<OnRead> onRead) { addObserver(ON_READ,onRead); }
  //void addOnShowListener(std::shared_ptr<OnShow> onShow) { addObserver(ON_SHOW,onShow); }

  void addListener(Listener *listener) 
  { 
    events = listener;
  }

  /*!
   * \brief Lanza el evento Show
   * \param[in] vf Frame de video
   * \return Si hay algún listener para recogerlo devuelve verdadero
   */
  bool show(cv::Mat &vf);

  // Métodos privados
private:
  /*!
   * \brief Recorta el frame de video
   * \param[in] vf Frame de video
   */
  void cropFrame(cv::Mat &vf);

  /*!
   * \brief Detecta imagenes borrosas con la varianza del laplaciano
   * \param[in] src Imagen en la que se comprueba si la imagen es borrosa
   * \return verdadero si la imagen esta borrosa.
   */
  bool isImageBlurry(const cv::Mat& src);

  /*!
   * \brief Redimensiona el frame de video
   * \param[in] vf Frame de video
   */
  void resizeFrame(cv::Mat &vf);

  /*!
  * \brief Salto de n frames
  * \param[in] vf Frame de video
  * \param[in] frames Número de frames que salta
  * \return Verdadero mientras encuentre otro frame
  */
  bool skipFrames(cv::Mat &vf, int frames);

  /*!
  * \brief Salto de n milisegundos
  * \param[in] vf Frame de video
  * \param[in] ms Número de milisegundos que salta
  * \return Verdadero mientras encuentre otro frame
  */
  bool skipMillisecond(cv::Mat &vf, int ms);

  void init();

  //void notifyOnPositionChange(double position) {
  //  for (const auto obs : mObservers[ON_POSITION_CHANGE])
  //    (*dynamic_cast<OnPositionChange *>(obs.get()))(position);
  //}
  
};





} // End namespace I3D

#endif // I3D_VIDEO_STREAM_H
