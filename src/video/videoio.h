#ifndef I3D_VIDEO_IO_H
#define I3D_VIDEO_IO_H

#include <memory>
#include <array>
#include <mutex>

#include "core/config.h"
#include "core/defs.h"
#include "core/utils.h" 
#include "graphic_entities/color.h"
#include "geometric_entities/point.h"

#ifdef HAVE_OPENCV
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

namespace I3D
{





class I3D_EXPORT VrtVideo : public File
{

public:

  //TODO: No puedo llamarlo Status por File...
  /*!
   * \brief Estado del video
   */
  enum class VideoStatus {
    START,      /*!< Inicio */
    RUNNING,    /*!< Video ejecutandose */
    PAUSE,      /*!< Video en pausa */
    STOPPED,    /*!< Video detenido */
    FINALIZED   /*!< Video finalizado */
  };

  /*!
   * \brief Tipos salto de frames
   */
  enum class Skip {
    NOT_SKIP,            /*!< Frame de video uno a uno. */
    SKIP_FRAMES,         /*!< Carga un frame de cada n. */
    SKIP_MILLISECONDS    /*!< Carga un frame cada n milisegundos. */
  };

  /*!
   * \brief Tipo de visualización del fotograma de video.
   */
  enum class View {
    ORIGINAL_FRAME,   /*!< Frame del tamaño original del video. */
    RESIZE_FRAME,     /*!< Redimensiona el frame de video. */
    CROP_FRAME        /*!< Recorta el frame de video. */
  };

  /*!
   * \brief Eventos
   */
  class Listener
  {
  protected:

    VrtVideo *mVideo;
  
  public:

    Listener() {}

    ~Listener() {}

    virtual void onFinish() = 0;
    virtual void onInitialize() = 0;
    virtual void onPause() = 0;
    virtual void onPositionChange(double position) = 0;
    virtual void onRead(cv::Mat &frame) = 0;
    virtual void onResume() = 0;
    virtual void onShow(cv::Mat &frame) = 0;
    virtual void onStop() = 0;
  
    void setVideoStream(VrtVideo *video) 
    { 
      mVideo = video; 
    }

  };

protected:
  
  /*!
   * \brief Resolución horizontal del video
   */
  int mWidth;

  /*!
   * \brief Resolución vertical del video
   */
  int mHeight;

  /*!
   * \brief Número de bandas o canales del video
   */
  int mBands;

  /*!
   * \brief Tamaño de los frames del video despues de recortar
   */
  //cv::Size mFrameSize;

  /*!
   * \brief Región de recorte del video
   */
  //cv::Rect rframe;
  geometry::WindowI mWindow;

  /*!
   * \brief Mantiene la proporción al redimensionar una imagen
   */
  bool bKeepRatio; 

  /*!
   * \brief Tipo de resolución del frame de video.
   * \see res_frame
   */
  VrtVideo::View mViewMode;

  /*!
   * \brief Tipo de salto de frames aplicado
   * \see skip_video
   */
  VrtVideo::Skip mSkipMode;

  /*!
   * \brief Salto de frames o de milisegundos con que se cargan los frames
   */
  int mSkip;

  double mFramesPerSecond;

  /*!
   * \brief vs Estado de ejecución del video
   * \see video_status
   */
  VrtVideo::VideoStatus mStatus;

  std::list<Listener *> mEvents;

  cv::Mat mFrame;

  int mCodec;

  bool bGray;

public:

  /*!
   * \brief Constructora
   */
  VrtVideo();

  VrtVideo(const char *file, Mode mode = Mode::Read);

  /*!
   * \brief Constructor de copia
   */
  VrtVideo(const VrtVideo &video);

  /*!
   * \brief Destructora
   */
  virtual ~VrtVideo();

  /*!
   *
   */
  void addListener(Listener *listener);

  /*!
   * \brief Crea un video
   * \return
   */
  virtual Status create(int width, int height, int codec, int fps, bool color = true) = 0;

  /*!
   * \brief Devuelve la resolución horizontal del video
   * \return Número de filas de la imagen
   */
  virtual int getWidth() const;

  /*!
   * \brief Devuelve la resolución Vertical del video
   * \return Número de columnas de la imagen
   */
  virtual int getHeight() const;

  /*!
   * \brief Devuelve el número de canales o bandas de la imagen
   * \return Número de bandas de la imagen
   */
  virtual int getBands() const;

  /*!
   * \brief Frames por segundo del video
   * \return Frames por segundo
   */
  virtual double getFramePerSeconds() const;

  /*!
   * \brief Frame de video actual
   * \return
   */
  virtual double getCurrentFrame() const = 0;

  /*!
   * \brief Número de frames del video
   * \return
   */
  virtual double getFrameCount() const = 0;
  
  /*!
   * \brief Comprueba si el video está abierto
   * \return Verdadero si el video está abierto
   */
  virtual bool isOpened() const = 0;

  virtual void setFramePerSeconds(double fps) = 0;

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
  virtual void run();

    /*!
   * \brief lee el siguiente frame
   */
  virtual void read() = 0;

  /*!
   * \brief Detiene la ejecución del video
   */
  virtual void stop() = 0;

  /*!
   * \brief Pausa la ejecución del video
   */
  virtual void pause() = 0;

  /*!
   * \brief Continua la ejecución del video
   */
  virtual void resume() = 0;

  /*!
   * \brief Finaliza la ejecución del video
   */
  virtual void finish() = 0;

protected:

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

};



/*!
 * \brief Ficheros de video que son gestionados por OpenCV
 */
class I3D_EXPORT VideoOpenCV : public VrtVideo
{

private:

  /*!
   * \brief Captura de video
   */
  cv::VideoCapture mVideoCapture;

  /*!
   * \brief Escritura de video
   */
  cv::VideoWriter mVideoWriter;

public:

  /*!
   * \brief Constructora
   */
  VideoOpenCV();

  VideoOpenCV(const char *file, Mode mode = Mode::Read);
  /*!
   * \brief Constructor de copia
   * \param gdalRaster Objeto VideoOpenCV
   */
  VideoOpenCV(const VideoOpenCV &video);

  /*!
   * \brief Destructora
   */
  virtual ~VideoOpenCV();

  /*!
   * \brief Cierra el video
   */
  void close() override;

  /*!
   * \brief Abre un video
   * \param[in] file Nombre del fichero
   * \param[in] mode Modo de apertura
   * \return
   * \see Mode
   */
  Status open(const char *file, Mode mode = Mode::Read) override;

  /*!
   * \brief Crea una imagen
   * \param[in] width Ancho de la imagen
   * \param[in] height Alto de la imagen
   * \param[in] codec Codec de video usado
   * \param[in] fps Número de fotogramas por segundo
   * \param[in] color Si es true el video se genera en color. En caso contrario en b/n
   * \return 
   */
  Status create(int width, int height, int codec, int fps, bool color = true) override;

  /*!
   * \brief Guarda una copia con otro nonbre
   */
  Status createCopy(const char *fileOut) override;

  /*!
   * \brief Frame de video actual
   * \return
   */
  virtual double getCurrentFrame() const override;

  /*!
   * \brief Número de frames del video
   * \return
   */
  virtual double getFrameCount() const override;

  /*!
   * \brief Comprueba si el video está abierto
   * \return Verdadero si el video está abierto
   */
  virtual bool isOpened() const;

  virtual void setFramePerSeconds(double fps) override;

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
  //virtual void run() override;

    /*!
   * \brief lee el siguiente frame
   */
  virtual void read() override;

  /*!
   * \brief Detiene la ejecución del video
   */
  virtual void stop() override;

  /*!
   * \brief Pausa la ejecución del video
   */
  virtual void pause() override;

  /*!
   * \brief Continua la ejecución del video
   */
  virtual void resume() override;

  /*!
   * \brief Finaliza la ejecución del video
   */
  virtual void finish() override;

protected:

  virtual void update();

  virtual bool _read();

};



// Clase para la lectura de un listado de imagenes en un archivo. Para reemplazar a ImagesStream

class I3D_EXPORT ImagePresentation : public VrtVideo
{
public:

  double mCurrentFrame;

  bool bIsOpened;

  std::vector<std::string> mImages;

public:

  ImagePresentation();
  ImagePresentation(const char *file, Mode mode = Mode::Read);
  ImagePresentation(const ImagePresentation &imagePresentation);
  ~ImagePresentation();

  /*!
   * \brief Cierra el video
   */
  void close() override;

  /*!
   * \brief Abre un video
   * \param[in] file Nombre del fichero
   * \param[in] mode Modo de apertura
   * \return
   * \see Mode
   */
  Status open(const char *file, Mode mode = Mode::Read) override;

  /*!
   * \brief Crea una imagen
   * \param[in] width Ancho de la imagen
   * \param[in] height Alto de la imagen
   * \param[in] codec Codec de video usado
   * \param[in] fps Número de fotogramas por segundo
   * \param[in] color Si es true el video se genera en color. En caso contrario en b/n
   * \return 
   */
  Status create(int width, int height, int codec, int fps, bool color = true) override;

  /*!
   * \brief Guarda una copia con otro nonbre
   */
  Status createCopy(const char *fileOut) override;

  /*!
   * \brief Frame de video actual
   * \return
   */
  virtual double getCurrentFrame() const override;

  /*!
   * \brief Número de frames del video
   * \return
   */
  virtual double getFrameCount() const override;

  /*!
   * \brief Comprueba si el video está abierto
   * \return Verdadero si el video está abierto
   */
  virtual bool isOpened() const;

  virtual void setFramePerSeconds(double fps) override;

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
  //virtual void run() override;

    /*!
   * \brief lee el siguiente frame
   */
  virtual void read() override;

  /*!
   * \brief Detiene la ejecución del video
   */
  virtual void stop() override;

  /*!
   * \brief Pausa la ejecución del video
   */
  virtual void pause() override;

  /*!
   * \brief Continua la ejecución del video
   */
  virtual void resume() override;

  /*!
   * \brief Finaliza la ejecución del video
   */
  virtual void finish() override;

protected:

  virtual void update();

  virtual bool _read();
};


} // End namespace I3D

#endif // HAVE_OPENCV

#endif // I3D_VIDEO_IO_H
