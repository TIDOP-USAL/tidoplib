#ifndef I3D_VIDEO_STREAM_H
#define I3D_VIDEO_STREAM_H

#include <ctime>
#include <list>

#include "core/config.h"

#ifdef HAVE_OPENCV
#include "opencv2/highgui/highgui.hpp"

#include "core/defs.h"

namespace I3D
{

class I3D_EXPORT VideoStreamEvents;

/*!
 * \brief Clase para el manejo de video
 */
class I3D_EXPORT VideoStream
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
  typedef VideoStreamEvents Listener;

protected:

  cv::Mat mFrame;

  /*!
   * \brief Tamaño original de los frames del video
   */
  cv::Size mSize;

  /*!
   * \brief Tamaño de los frames del video despues de recortar
   */
  cv::Size mFrameSize;

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
  VideoStream::Resolution mResolutionFrame;

  /*!
   * \brief Tipo de salto de frames aplicado
   * \see skip_video
   */
  VideoStream::Skip sv;

  /*!
   * \brief Salto de frames o de milisegundos con que se cargan los frames
   */
  int mSkip;

  /*!
   * \brief start
   */
  //std::clock_t start; Para prueba de comportamiento del video en tiempo real

  /*!
   * \brief vs Estado de ejecución del video
   * \see video_status
   */
  VideoStream::Status vs;

  std::list<Listener *> events;
  
  double mfps;

private:

  cv::VideoCapture mVideoCapture;

public:

  /*!
   * \brief Constructora I3DVideoStream
   */
  VideoStream() : mSize(0, 0) 
  {
    init();
  }

  /*!
   * \brief Constructora I3DVideoStream
   * \param[in] file Video
   */
  VideoStream(const char *file) : mSize(0, 0)
  {
    init();
    open(file);
  }

  /*!
   * \brief Destructora I3DVideoStream
   */
  ~VideoStream(){}

  /*!
   *
   */
  void addListener(Listener *listener);

  /*!
   * \brief Frames por segundo del video
   * \return Frames por segundo
   */
  virtual double fps() { return mfps/*mVideoCapture.get(CV_CAP_PROP_FPS)*/; }

  /*!
   * \brief Frame de video actual
   * \return
   */
  virtual double getCurrentFrame() { return mVideoCapture.get(CV_CAP_PROP_POS_FRAMES); }

  /*!
   * \brief Número de frames del video
   * \return
   */
  virtual double getFrameCount() { return mVideoCapture.get(CV_CAP_PROP_FRAME_COUNT); }

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
  virtual bool isOpened() { return mVideoCapture.isOpened(); }

  /*!
   * \brief Comprueba si esta activado el salto de frames borrosos
   * \return Verdadero si esta activado
   */
  bool isSkipBlurryFrames() { return bSkipBlurryFrames;  }

  /*!
   * \brief NextFrame
   * \param[out] vf Frame de video
   * \return Verdadero si encuentra frame
   */
  bool nextFrame(cv::Mat *vf);

  /*!
   * \brief Siguiente frame del video
   * \param[out] vf Frame de video
   * \param[in] skip Tipo de salto de video aplicado
   * \param[in] nskip Número de salto
   * \return Verdadero si encuentra frame
   */
  bool nextFrame(cv::Mat *vf, VideoStream::Skip skip, int nskip);

  /*!
   * \brief Abre el video
   * \param[in] name Nombre del video
   * \return verdadero si el video se ha abierto
   */
  virtual bool open(const char *name);

  /*!
   * \brief Lee un frame de video
   * \param[out] vf Frame de video
   * \return Falso si no encuentra ningún frame
   */
  virtual bool read(cv::Mat *vf);

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
  virtual bool setPosFrame(double nframe);

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
  //void stop() { vs = VideoStream::Status::STOPPED; }

//  /*!
//   * \brief Retrasa el video
//   */
  //void Rewind();

  /*!
   * \brief Lanza el evento Show
   * \param[in] vf Frame de video
   * \return Si hay algún listener para recogerlo devuelve verdadero
   */
  //bool show(cv::Mat &vf);

protected:

  /*!
   * \brief Recorta el frame de video
   */
  void cropFrame();

  /*!
   * \brief Detecta imagenes borrosas con la varianza del laplaciano
   * \param[in] src Imagen en la que se comprueba si la imagen es borrosa
   * \return verdadero si la imagen esta borrosa.
   */
  bool isImageBlurry(const cv::Mat& src);

  void init();

  void onFinish();

  void onInitialize();

  /*!
   * \brief Rausa el video
   */
  void onPause();

  /*!
   * \brief 
   */
  void onPositionChange(double position);

  /*!
   * \brief 
   */
  void onRead();

  /*!
   * \brief Continua la ejecución del video
   */
  void onResume();

  /*!
   * \brief 
   */
  void onShow();

  /*!
   * \brief Detiene la ejecución del video
   */
  void onStop();

  /*!
   * \brief Redimensiona el frame de video
   */
  void resizeFrame();

  /*!
   * \brief Salto de n frames
   * \param[in] vf Frame de video
   * \param[in] frames Número de frames que salta
   * \return Verdadero mientras encuentre otro frame
   */
  bool skipFrames(cv::Mat *vf, int frames);

  /*!
   * \brief Salto de n milisegundos
   * \param[in] vf Frame de video
   * \param[in] ms Número de milisegundos que salta
   * \return Verdadero mientras encuentre otro frame
   */
  bool skipMillisecond(cv::Mat *vf, int ms);

  // No me convence usar friend pero...
  friend class VideoStreamEvents;
};



class ImagesStream : public VideoStream
{
private:

  std::vector<std::string> mImages;

  //double mfps;

  double mCurrentFrame;

  bool bIsOpened;

public:

  /*!
   * \brief Constructora ImagesStream
   */
  ImagesStream() : /*VideoStream(),*/ mImages(0), mCurrentFrame(0) 
  {
    init();
    mfps = 1.;
  }

  /*!
   * \brief Constructora I3DVideoStream
   * \param[in] file Video o fichero de texto con un listado de imagenes
   */
  ImagesStream(const char *file) : /*VideoStream(file),*/ mCurrentFrame(0)  
  {
    init();
    open(file);
    mfps = 1.;
  }

  /*!
   * \brief Destructora
   */
  ~ImagesStream()
  {
  }

  /*!
   * \brief Frames por segundo del video
   * \return Frames por segundo
   */
  //double fps() override { return mfps; }

  /*!
   * \brief Frame de video actual
   * \return
   */
  double getCurrentFrame() override { return mCurrentFrame; }

  /*!
   * \brief Número de frames del video
   * \return
   */
  double getFrameCount() override { return static_cast<double>(mImages.size()); }

  /*!
  * \brief Comprueba si el video está abierto
  * \return Verdadero si el video está abierto
  */
  bool isOpened() override { return bIsOpened; }

  /*!
   * \brief Abre el listado de imagenes
   * \param[in] name Fichero con el listado de imagenes
   * \return verdadero si el video se ha abierto
   */
  bool open(const char *name) override;

  /*!
   * \brief Lee un frame de video
   * \param[out] vf Frame de video
   * \return Falso si no encuentra ningún frame
   */
  bool read(cv::Mat *vf) override;

  /*!
   * \brief Establece la posición en el video según el número de frame
   * \param[in] nframe
   * \return
   */
  bool setPosFrame(double nframe) override;

  ///*!
  // * \brief Salto de n frames
  // * \param[in] vf Frame de video
  // * \param[in] frames Número de frames que salta
  // * \return Verdadero mientras encuentre otro frame
  // */
  //virtual bool skipFrames(cv::Mat *vf, int frames);

  ///*!
  // * \brief Salto de n milisegundos
  // * \param[in] vf Frame de video
  // * \param[in] ms Número de milisegundos que salta
  // * \return Verdadero mientras encuentre otro frame
  // */
  //virtual bool skipMillisecond(cv::Mat *vf, int ms);

};

class VideoStreamEvents
{
protected:

  VideoStream *mVideoStream;
  
public:
  VideoStreamEvents() {}

  ~VideoStreamEvents() {}

  virtual void onFinish() = 0;
  virtual void onInitialize() = 0;
  virtual void onPause() = 0;
  virtual void onPositionChange(double position) = 0;
  virtual void onRead(cv::Mat &frame) = 0;
  virtual void onResume() = 0;
  virtual void onShow(cv::Mat &frame) = 0;
  virtual void onStop() = 0;
  
  void setVideoStream(VideoStream *videoStream) 
  { 
    mVideoStream = videoStream; 
  }

};


/*!
 * \brief Clase para la visualización de video
 */
class I3D_EXPORT VideoWindow : public VideoStream::Listener
{
private:
  /*!
   * \brief Nombre de la ventana de video
   */
  std::string mWindowName;

  /*!
   * \brief flags de ventana de OpenCV
   */
  int mFlags;

  /*!
   * \brief Añade una barra de posición del video
   */
  bool bPosTrackBar;

  /*!
   * \brief Número de frames del video
   */
  double mVideoSize;

  /*!
   * \brief Refefencia al video que se esta reproduciendo
   */
  VideoStream *mVideo;

public:
  /*!
   * \brief Constructora por defecto  de la clase VideoWindow
   */
  VideoWindow() 
    : mWindowName("Video"), mFlags(1), bPosTrackBar(true), mVideoSize(0), mVideo(0) 
  {
    cv::namedWindow(mWindowName, mFlags);
  }

  /*!
   * \brief Constructora de la clase VideoWindow
   * \param[in] wname Nombre de la ventana de video
   * \param[in] flags
   * \param[in] bPos Muestra slider con posición del video
   */
  VideoWindow(const char* wname, int flags = CV_WINDOW_AUTOSIZE, bool bPos = true)
    : mWindowName(wname), mFlags(flags), bPosTrackBar(bPos), mVideoSize(0), mVideo(0) 
  { 
    cv::namedWindow(mWindowName, mFlags);
  }

  /*!
   * \brief Destructora de la clase I3DVideoCtrls
   */
  ~VideoWindow();

  void addButton(const char *name, int type, cv::ButtonCallback onChange, void* userdata, bool buttonState);

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
    return mWindowName;
  }

  void onFinish() override
  {
    VideoStream::Listener::onFinish(); 
  }
     
  void onInitialize() override
  {
    VideoStream::Listener::onInitialize(); 
    cv::namedWindow(mWindowName, mFlags);
  }

  void onPause() override
  {
    VideoStream::Listener::onPause(); 
  }

  void onRead(cv::Mat &frame) override 
  { 
    VideoStream::Listener::onRead(frame);
  }

  void onPositionChange(double position) override
  { 
    VideoStream::Listener::onPositionChange(position);

    //... La posición del trackbar se pasa en entero. para videos muy largos podria desbordarse
    if ( mVideoSize >= position )
      cv::setTrackbarPos("Frame", mWindowName, cvRound(position));
  }

  void onResume() override
  {
    VideoStream::Listener::onResume(); 
  }

  void onShow(cv::Mat &frame) override
  { 
    VideoStream::Listener::onShow(frame);
    if ( !frame.empty() )
      cv::imshow(mWindowName, frame);
  }

  void onStop() override
  { 
    VideoStream::Listener::onStop(); 
  }

  /*!
   * \brief Añade una referencia a un objeto VideoStream
   */
  void setVideo(VideoStream *vs);

private:

  /*!
   * \brief Init
   */
  void init();

};




} // End namespace I3D

#endif // HAVE_OPENCV

#endif // I3D_VIDEO_STREAM_H
