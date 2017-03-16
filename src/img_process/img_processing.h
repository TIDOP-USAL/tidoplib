#ifndef I3D_IMG_PROCESSING_H
#define I3D_IMG_PROCESSING_H

#include <list>
#include <memory>
#include <functional>

#include "core/config.h"

#ifdef HAVE_OPENCV
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "core/defs.h"
#include "graphic_entities/color.h"

namespace I3D
{


/*! \defgroup ImgProc Procesos de imagen
 *  Filtrado y preprocesado de imagenes
 *  \{
 */

enum class ProcessExit
{
  SUCCESS,
  FAILURE
};

/*!
 * \brief Tipos de procesos de imagen
 */
enum class process_type {
  /* Filtros */
  BILATERAL,          /*!< Filtro bilateral. */
  BLUR,               /*!< Filtro desenfoque. */
  BOX_FILTER,         /*!<  */
  FILTER_2D,          /*!<  */
  GAUSSIAN_BLUR,      /*!< Desenfoque gaussiano. */
  LAPLACIAN,          /*!< Laplaciano de una imagen */
  MEDIAN_BLUR,        /*!<  */
  SOBEL,              /*!< Operador Sobel. */
  CANNY,              /*!< Detector de bordes canny. */
  /* Operaciones morfológicas */
  MORPH_DILATION,     /*!< Operacion morfologica de dilatación. */
  MORPH_EROTION,      /*!< Operacion morfologica de erosión. */
  MORPH_OPENING,      /*!< Operacion morfologica de apertura. */
  MORPH_CLOSING,      /*!< Operacion morfologica de cierre. */
  MORPH_GRADIENT,     /*!< Operacion morfologica  */
  MORPH_TOPHAT,       /*!< Operacion morfologica  */
  MORPH_BLACKHAT,     /*!< Operacion morfologica  */
  /* Transformación de imagen */
  RESIZE,             /*!< Redimensiona la imagen */
  RESIZE_CANVAS,
  NORMALIZE,          /*!< Normalización. */
  BINARIZE,           /*!< Binarización. */
  EQUALIZE_HIST,      /*!< Equalización del histograma. */
  FUNCTION_PROCESS,   /*!< Proceso que ejecuta una función */
  /* Balance de blancos */
  GRAYWORLD,
  WHITEPATCH,
  /* Balance de blancos */
  COLOR_CONVERSION
};

/*!
 * \brief Clase para gestionar los procesos de imagen
 * Clase que permite tener una interfaz común para aplicar filtros
 * u otros procesos a una imagen.
 */
class I3D_EXPORT ImgProcessing
{
public:

  /*!
   * \brief Tipo de proceso de imagen
   * \see process_type
   */
  process_type type;

  /*
   * \brief Mensajes de estado del proceso
   */
  enum class Status
  {
    OK,                       // El proceso se ha ejecutado correctamente
    INCORRECT_INPUT_DATA,     // Error de los datos de entrada
    PROCESS_ERROR             // Error durante el proceso.
  };

public:

  /*!
   * \brief Constructora de la clase
   * \param[in] _type Tipo de proceso.
   * \see process_type
   */
  ImgProcessing( process_type _type ) : type(_type) {}

  /*!
   * \brief Destuctora
   */
  virtual ~ImgProcessing(){ }

  /*!
   * \brief Ejecuta el proceso
   * Metodo virtual puro que deben implementar las clases hijas
   * \param[in] matIn Imagen de entrada
   * \param[out] matOut Imagen de salida
   * \return Si los procesos se ejecutan correctamente devuelve ImgProcessing::Status::OK. 
   * \see ImgProcessing::Status
   */
  virtual ImgProcessing::Status execute(const cv::Mat &matIn, cv::Mat *matOut) const = 0 ;

};

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Contenedor de procesos de imagen que permite su ejecución secuencial.
 * \code
 * I3D::ImgProcessingList imgprolist;
 * imgprolist.add(std::make_shared<I3D::Normalize>(0., 255.));
 * imgprolist.add(std::make_shared<I3D::BilateralFilter>(5, 50., 50.));
 * imgprolist.add(std::make_shared<I3D::Erotion>(1));
 * imgprolist.add(std::make_shared<I3D::Dilate>(1));
 * cv::Mat out;
 * imgprolist.execute(in, &out);
 * \endcode
 */
class I3D_EXPORT ImgProcessingList
{
private:

  /*!
   * \brief Lista de procesos
   */
  std::list<std::shared_ptr<ImgProcessing>> mProcessList;

public:

  /*!
   * \brief Constructora por defecto
   */
  ImgProcessingList(){}

  /*!
   * \brief Constructor de copia
   * \param[in] imgProcList ImgProcessingList que se copia
   */
  ImgProcessingList(const ImgProcessingList &imgProcList) : mProcessList(imgProcList.mProcessList) {}

  /*!
   * \brief Constructor de lista
   * \param[in] Listado de procesos
   */
  ImgProcessingList(std::initializer_list<std::shared_ptr<ImgProcessing>> imgProcList) : mProcessList(imgProcList) {}

  /*!
   * \brief Destructora
   */
  ~ImgProcessingList(){}

public:

  /*!
   * \brief Añade un nuevo proceso a la lista
   * \param[in] ip Proceso que se añade
   */
  void add(const std::shared_ptr<ImgProcessing> &ip);

  /*!
   * \brief Ejecuta la lista de procesos
   * \param[in] matIn Imagen de entrada
   * \param[out] matOut Imagen de salida
   * \return Si los procesos se ejecutan correctamente devuelve ProcessExit::SUCCESS. 
   * En caso contrario devuelve ProcessExit::FAILURE
   * \see ProcessExit
   */
  ProcessExit execute(const cv::Mat &matIn, cv::Mat *matOut) const;

  /*!
   * \brief Limpia la lista de procesos
   */
  void clear() { mProcessList.clear(); }

};

/* ---------------------------------------------------------------------------------- */


/*! \addtogroup Filters
 *  
 *  \{
 */

/* ---------------------------------------------------------------------------------- */
/*                          Operaciones morfologicas                                  */
/* ---------------------------------------------------------------------------------- */

/*! \defgroup MorphOper Operaciones morfológicas
 *  
 *  \{
 */

/*!
 * \brief Clase base para las operaciones morfológicas
 *
 * Operaciones morfologicas básicas:<BR>
 * \li Erosion (Erotion)
 * \li Dilatacion (Dilate)
 * 
 * Operaciones morfologicas avanzadas (cv::morphologyEx):
 * \li Apertura (Opening) = erosion + dilation
 * \li Cierre (Closing) = dilatación + erosión
 * \li Morphological Gradient = dilatación - erosión
 * \li TopHat = src - apertura(src)
 * \li BlackHat = cierre(src) - src
 */
class morphologicalOperation : public ImgProcessing
{
protected:

  /*!
   * \brief Forma del elemento estructurante
   */
  cv::MorphShapes mShapes;

  /*!
   * \brief dilationSize
   */
  int mSize;

  /*!
   * \brief Punto de anclaje.
   * Por defecto es el centro del kernel
   */
  cv::Point mAnchor;

  /*!
   * \brief Número de veces que se aplica la dilatación
   */
  int mIterations;

  /*!
   * \brief Método de extrapolación de pixeles
   */
  int mBorderType;

  /*!
   * \brief Valor de borde en el caso de un borde constante.
   */
  cv::Scalar mBorderValue;

public:

  /*!
   * \brief Constructora de la clase
   * \param[in] type Tipo de proceso.
   * \see process_type
   */
  morphologicalOperation(process_type type) 
    : ImgProcessing(type) {}

  /*!
   * \brief Destuctora
   */
  virtual ~morphologicalOperation() {}

  /*!
   * \brief Ejecuta el proceso
   * Metodo virtual puro que deben implementar las clases hijas
   * \param[in] matIn Imagen de entrada
   * \param[out] matOut Imagen de salida
   * \return Si los procesos se ejecutan correctamente devuelve ImgProcessing::Status::OK. 
   * \see ImgProcessing::Status
   */
  ImgProcessing::Status execute(const cv::Mat &matIn, cv::Mat *matOut) const override;

  /*!
   * \brief Establece los parámetros
   * \param[in] size
   * \param[in] type
   * \param[in] anchor Punto de anclaje. Por defecto es el centro del kernel
   * \param[in] iterations
   * \param[in] borderType Método de extrapolación
   */
  void setParameters(int size, cv::MorphShapes shapes = cv::MORPH_RECT, 
    cv::Point anchor = cv::Point(-1, -1), int iterations = 1, 
    int borderType = cv::BORDER_CONSTANT, const cv::Scalar &borderValue = cv::morphologyDefaultBorderValue());

};


/*!
 * \brief Operacion morfologica de dilatación.
 */
class I3D_EXPORT Dilate : public morphologicalOperation
{

public:

  /*!
   * \brief Constructora
   * \param[in] size
   * \param[in] type
   * \param[in] anchor Punto de anclaje. Por defecto es el centro del kernel
   * \param[in] iterations
   * \param[in] borderType Método de extrapolación
   * \param[in] borderValue 
   */
  Dilate(int size, cv::MorphShapes shapes = cv::MORPH_RECT, cv::Point anchor = cv::Point(-1, -1), 
         int iterations = 1, int borderType = cv::BORDER_CONSTANT, 
         const cv::Scalar &borderValue = cv::morphologyDefaultBorderValue())
    : morphologicalOperation(process_type::MORPH_DILATION)
  {
    setParameters(size, shapes, anchor, iterations, borderType, borderValue );
  }

};

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Operacion morfologica de erosión
 */
class I3D_EXPORT Erotion : public morphologicalOperation
{

public:

  /*!
   * \brief Constructora clase Erotion
   * \param[in] size
   * \param[in] type
   * \param[in] anchor Punto de anclaje. Por defecto es el centro del kernel
   * \param[in] iterations
   * \param[in] borderType Método de extrapolación
   * \param[in] borderValue
   */
  Erotion(int size, cv::MorphShapes shapes = cv::MORPH_RECT, cv::Point anchor = cv::Point(-1, -1), 
    int iterations = 1, int borderType = cv::BORDER_CONSTANT,
    const cv::Scalar &borderValue = cv::morphologyDefaultBorderValue())
    : morphologicalOperation(process_type::MORPH_EROTION) 
  {
    setParameters(size, shapes, anchor, iterations, borderType, borderValue);
  }

};

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Operación morfológica de apertura
 * Esta operación consite en aplicar una erosion de la imagen
 * seguida de una dilatación
 */
class I3D_EXPORT Opening : public morphologicalOperation
{
public:

  /*!
   * \brief Constructora clase Opening
   * \param[in] size
   * \param[in] type
   * \param[in] anchor Punto de anclaje. Por defecto es el centro del kernel
   * \param[in] iterations
   * \param[in] borderType Método de extrapolación
   * \param[in] borderValue
   */
  Opening(int size, cv::MorphShapes shapes = cv::MORPH_RECT, cv::Point anchor = cv::Point(-1, -1),
    int iterations = 1, int borderType = cv::BORDER_CONSTANT,
    const cv::Scalar &borderValue = cv::morphologyDefaultBorderValue())
    : morphologicalOperation(process_type::MORPH_OPENING)
  {
    setParameters(size, shapes, anchor, iterations, borderType, borderValue);
  }

};

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Operación morfológica de apertura
 * Esta operación consite en aplicar una dilatación de la imagen
 * seguida de una erosión
 */
class I3D_EXPORT Closing : public morphologicalOperation
{
public:

  /*!
   * \brief Constructora clase Closing
   * \param[in] size
   * \param[in] type
   * \param[in] anchor Punto de anclaje. Por defecto es el centro del kernel
   * \param[in] iterations
   * \param[in] borderType Método de extrapolación
   * \param[in] borderValue
   */
  Closing(int size, cv::MorphShapes shapes = cv::MORPH_RECT, cv::Point anchor = cv::Point(-1, -1),
    int iterations = 1, int borderType = cv::BORDER_CONSTANT,
    const cv::Scalar &borderValue = cv::morphologyDefaultBorderValue())
    : morphologicalOperation(process_type::MORPH_CLOSING)
  {
    setParameters(size, shapes, anchor, iterations, borderType, borderValue);
  }

};

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Operación morfológica gradiente
 * Gradient = dilation - erosion
 * 
 */
class I3D_EXPORT Gradient : public morphologicalOperation
{
public:

  /*!
   * \brief Constructora clase Closing
   * \param[in] size
   * \param[in] type
   * \param[in] anchor Punto de anclaje. Por defecto es el centro del kernel
   * \param[in] iterations
   * \param[in] borderType Método de extrapolación
   * \param[in] borderValue
   */
  Gradient(int size, cv::MorphShapes shapes = cv::MORPH_RECT, cv::Point anchor = cv::Point(-1, -1),
    int iterations = 1, int borderType = cv::BORDER_CONSTANT,
    const cv::Scalar &borderValue = cv::morphologyDefaultBorderValue())
    : morphologicalOperation(process_type::MORPH_GRADIENT)
  {
    setParameters(size, shapes, anchor, iterations, borderType, borderValue);
  }
};

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Operación morfológica Top Hat
 * Es la diferencia entre una imagen y el resultado de aplicar una operación de 
 * apertura sobre la misma imagen
 */
class I3D_EXPORT TopHat : public morphologicalOperation
{
public:

  /*!
   * \brief Constructora clase Closing
   * \param[in] size
   * \param[in] type
   * \param[in] anchor Punto de anclaje. Por defecto es el centro del kernel
   * \param[in] iterations
   * \param[in] borderType Método de extrapolación
   * \param[in] borderValue
   */
  TopHat(int size, cv::MorphShapes shapes = cv::MORPH_RECT, cv::Point anchor = cv::Point(-1, -1),
    int iterations = 1, int borderType = cv::BORDER_CONSTANT,
    const cv::Scalar &borderValue = cv::morphologyDefaultBorderValue())
    : morphologicalOperation(process_type::MORPH_TOPHAT)
  {
    setParameters(size, shapes, anchor, iterations, borderType, borderValue);
  }

};

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Operación morfológica Black Hat
 * Esta operación consite en aplicar un cierre a una imagen y restar el resultado por la
 * la imagen original
 */
class I3D_EXPORT BlackHat : public morphologicalOperation
{
public:

  /*!
   * \brief Constructora clase Closing
   * \param[in] size
   * \param[in] type
   * \param[in] anchor Punto de anclaje. Por defecto es el centro del kernel
   * \param[in] iterations
   * \param[in] borderType Método de extrapolación
   * \param[in] borderValue
   */
  BlackHat(int size, cv::MorphShapes shapes = cv::MORPH_RECT, cv::Point anchor = cv::Point(-1, -1),
    int iterations = 1, int borderType = cv::BORDER_CONSTANT,
    const cv::Scalar &borderValue = cv::morphologyDefaultBorderValue())
    : morphologicalOperation(process_type::MORPH_BLACKHAT)
  {
    setParameters(size, shapes, anchor, iterations, borderType, borderValue);
  }

};

/*! \} */ // end of MorphOper

/*! \} */ // end of Filters

/* ---------------------------------------------------------------------------------- */
/*             Operaciones que transforman geometricamente la imagen                  */
/* ---------------------------------------------------------------------------------- */

/*! \defgroup imgTransf Operaciones de transformación de imagenes
 *  
 *  \{
 */


/*!
 * \brief Redimensiona una imagen
 */
class I3D_EXPORT Resize : public ImgProcessing
{
private:

  /*!
   * \brief Ancho
   */
  int mWidth;

  /*!
   * \brief Alto
   */
  int mHeight;

  double mScaleX;
  
  double mScaleY;

public:

  /*!
   * \brief Constructora
   * \param[in] width
   * \param[in] height
   */
  Resize(int width, int height = 0)
    : ImgProcessing(process_type::RESIZE), mWidth(width), mHeight(height), 
      mScaleX(0.), mScaleY(0.) {}

  /*!
   * \brief Constructora
   * \param[in] scaleX
   * \param[in] scaleY
   */
  Resize(double scaleX, double scaleY = 0.)
    : ImgProcessing(process_type::RESIZE), mWidth(0), mHeight(0), 
      mScaleX(scaleX), mScaleY(scaleY ? scaleY : scaleX) {}

  /*!
   * \brief Ejecuta el proceso.
   * \param[in] matIn Imagen de entrada.
   * \param[out] matOut Imagen de salida.
   * \return Si los procesos se ejecutan correctamente devuelve ImgProcessing::Status::OK. 
   * \see ImgProcessing::Status
   */
  ImgProcessing::Status execute(const cv::Mat &matIn, cv::Mat *matOut) const override;

  /*!
   * \brief Establece los parámetros
   * \param[in] width Ancho.
   * \param[in] height Alto.
   */
  void setParameters( int width, int height );

  /*!
   * \brief Establece los parámetros
   * \param[in] scale Escala en porcentaje que se aplica a la imagen
   */
  void setParameters( double scaleX, double scaleY = 0. );
};

/*!
 * \brief Redimensiona el canvas de una imagen
 *
 * Si se amplia se añaden pixeles del color indicado en las 
 * zonas que no haya imagen. Por defecto el color de fondo es el negro.
 * Si se reduce se recorta la imagen.
 */
class I3D_EXPORT ResizeCanvas : public ImgProcessing
{
public:
  
  /*!
   * \brief Posición del canvas
   */
  enum class Position {
    BOTTOM_CENTER,  
    BOTTOM_LEFT,    
    BOTTOM_RIGHT,   
    CENTER,         
    CENTER_LEFT,    
    CENTER_RIGHT,   
    TOP_CENTER,     
    TOP_LEFT,       
    TOP_RIGHT       
  };

private:

  /*!
   * \brief Ancho
   */
  int mWidth;

  /*!
   * \brief Alto
   */
  int mHeight;

  /*!
   * \brief Coordenadas de la esquina superior izquierda
   */
  cv::Point mTopLeft;

  /*!
   * \brief Color de fondo
   */
  Color mColor;

  /*!
   * \brief Posición
   */
  Position mPosition;

public:

  /*!
   * \brief Constructora
   * \param[in] width Nuevo ancho
   * \param[in] height Nuevo alto
   * \param[in] color Color
   * \param[in] position Posición
   * \see Position
   */
  ResizeCanvas(int width, int height, const Color &color = Color(), const Position &position = Position::TOP_LEFT)
    : ImgProcessing(process_type::RESIZE_CANVAS), mWidth(width), mHeight(height), mColor(color), mPosition(position) { }

  /*!
   * \brief Constructora
   * \param[in] width Nuevo ancho
   * \param[in] height Nuevo alto
   * \param[in] point Coordenadas de la esquina superior izquierda
   * \param[in] color Color
   * \see Position
   */
  ResizeCanvas(int width, int height, const cv::Point &point, const Color &color = Color())
    : ImgProcessing(process_type::RESIZE_CANVAS), mWidth(width), mHeight(height), mTopLeft(point), mColor(color) {}


  /*!
   * \brief Ejecuta el proceso.
   * \param[in] matIn Imagen de entrada.
   * \param[out] matOut Imagen de salida.
   * \return Si los procesos se ejecutan correctamente devuelve ImgProcessing::Status::OK. 
   * \see ImgProcessing::Status
   */
  ImgProcessing::Status execute(const cv::Mat &matIn, cv::Mat *matOut) const override;

  /*!
   * \brief Establece los parámetros
   * \param[in] width Ancho
   * \param[in] height Alto
   * \param[in] color Color
   * \param[in] position Posición
   * \see Position
   */
  void setParameters( int width, int height, const Color &color = Color(), const Position &position = Position::TOP_LEFT);

  //void update();
};


/* ---------------------------------------------------------------------------------- */

/*! \} */ // end of imgTransf



/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Clase Normalize
 */
class I3D_EXPORT Normalize : public ImgProcessing
{
private:

  /*!
   * \brief Rango inferior
   */
  double mLowRange;

  /*!
   * \brief Rango superior
   */
  double mUpRange;

public:

  /*!
   * \brief Constructora de la clase Normalize
   * \param lowRange
   * \param upRange
   */
  Normalize(double lowRange, double upRange)
    : ImgProcessing(process_type::NORMALIZE), mLowRange(lowRange), mUpRange(upRange) {}

  /*!
   * \brief Ejecuta el proceso
   * \param[in] matIn Imagen de entrada
   * \param[out] matOut Imagen de salida
   * \return Si los procesos se ejecutan correctamente devuelve ImgProcessing::Status::OK. 
   * \see ImgProcessing::Status
   */
  ImgProcessing::Status execute(const cv::Mat &matIn, cv::Mat *matOut) const override;

  /*!
   * \brief Establece los parámetros
   * \param[in] lowRange Rango inferior
   * \param[in] upRange Rango superior
   */
  void setParameters(double lowRange, double upRange);
};

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Clase Binarize
 * Convierte una imagen a binaria
 */
class I3D_EXPORT Binarize : public ImgProcessing
{
private:
  
  /*!
   * \brief Umbral
   */
  double mThresh;

  /*!
   * \brief Valor máximo
   */
  double mMaxVal;

  /*!
   * \brief bInverse
   */
  bool bInverse;

public:

  /*!
   * \brief Constructora de la clase Binarize
   * Si thresh y maxVal son 0 se calculan internamente a partir de la media y desviación típica.
   * \param thresh Umbral
   * \param maxVal Valor máximo
   * \param bInverse Binarización inversa
   */
  Binarize(double thresh = 0., double maxVal = 0., bool bInverse = false)
    : ImgProcessing(process_type::BINARIZE), mThresh(thresh), mMaxVal(maxVal), bInverse(bInverse) {}

  /*!
   * \brief Ejecuta el proceso
   * \param[in] matIn Imagen de entrada
   * \param[out] matOut Imagen de salida
   * \return Si los procesos se ejecutan correctamente devuelve ImgProcessing::Status::OK. 
   * \see ImgProcessing::Status
   */
  ImgProcessing::Status execute(const cv::Mat &matIn, cv::Mat *matOut) const override;

  /*!
   * \brief Establece los parámetros
   * \param[in] thresh Umbral
   * \param[in] maxVal Valor máximo
   * \param[in] bInverse Binarización inversa
   */
  void setParameters(double thresh, double maxVal, bool bInverse = false);

  /*!
   * \brief Binarización inversa
   * \param[in] inverse
   */
  void setInverse(bool inverse = true) { bInverse = inverse; }

  /*!
   * \brief GetInverse
   * \return
   */
  bool getInverse() const { return bInverse; }

};

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Ecualización del histograma.
 * Mejora del contraste de la imagen mediante la ecualización del histograma
 */
class I3D_EXPORT EqualizeHistogram : public ImgProcessing
{

public:

  /*!
   * \brief Constructora de la clase
   */
  EqualizeHistogram()
    : ImgProcessing(process_type::EQUALIZE_HIST) {}

  //~EqualizeHistogram();

  /*!
   * \brief Ejecuta el proceso.
   * \param[in] matIn Imagen de entrada.
   * \param[out] matOut Imagen de salida.
   * \return Si los procesos se ejecutan correctamente devuelve ImgProcessing::Status::OK. 
   * \see ImgProcessing::Status
   */
  ImgProcessing::Status execute(const cv::Mat &matIn, cv::Mat *matOut) const override;

};

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Wrapper de una función para ejecutarla como un proceso.
 * 
 * Para permitir una mayor libertad en el procesado de las imagenes mediante
 * ImgProcessingList se permite asociar una función o lambda a un proceso. La
 * función tiene que ser de la forma:
 * <BLOCKQUOTE>std::function<void(const cv::Mat &,cv::Mat *)> </BLOCKQUOTE>
 * o si es una lambda:
 * <BLOCKQUOTE>[](const cv::Mat &in, cv::Mat *out) { ... }</BLOCKQUOTE>
 * <H3>Ejemplo:</H3>
 * \code
 * std::shared_ptr<FunctionProcess> fProcess1 = std::make_shared<FunctionProcess>(
 *   [](const cv::Mat &in, cv::Mat *out) {
 *     in.convertTo(*out, CV_32F);
 * });
 * std::shared_ptr<FunctionProcess> fProcess2 = std::make_shared<FunctionProcess>(
 * [&](const cv::Mat &in, cv::Mat *out) {
 * cv::normalize(in, *out, 0, 255, CV_MINMAX);
 * out->convertTo(*out, CV_8U);
 * });
 *
 * I3D::ImgProcessingList imgprolist{ fProcess1, fProcess2 };
 * \endcode
 */
class I3D_EXPORT FunctionProcess : public ImgProcessing
{
private:

  /*!
   * \brief Función
   */
  std::function<void(const cv::Mat &,cv::Mat *)> f;

public:

  /*!
   * \brief Constructora
   * \param[in] f Función de la forma std::function<void(const cv::Mat &,cv::Mat *)>
   */
  FunctionProcess(std::function<void(const cv::Mat &, cv::Mat *)> f)
    : ImgProcessing(process_type::FUNCTION_PROCESS), f(f) {}

  //~FunctionProcess();

  /*!
   * \brief Ejecuta el proceso.
   * \param[in] matIn Imagen de entrada.
   * \param[out] matOut Imagen de salida.
   * \return Si los procesos se ejecutan correctamente devuelve ImgProcessing::Status::OK. 
   * \see ImgProcessing::Status
   */
  ImgProcessing::Status execute(const cv::Mat &matIn, cv::Mat *matOut) const override;

};


/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Conversión del modo de color.
 * Conversión entre distintos tipos de modos de color
 */
class I3D_EXPORT ColorConversion : public ImgProcessing
{
public:
  
  /*!
   * Modelos de color
   */
  enum class Model
  {
    RGB,
    RGBA,
    CMYK,
    HSL,
    HSV,
    LUMINANCE,
    CHROMATICITY,
  };

private:

  Model mModelIn;
  
  Model mModelOut;

public:

  /*!
   * \brief Constructora de la clase
   */
  ColorConversion(Model modelIn, Model modelOut)
    : ImgProcessing(process_type::COLOR_CONVERSION), mModelIn(modelIn), mModelOut(modelOut) {}

  //~ColorConversion();

  /*!
   * \brief Ejecuta el proceso.
   * \param[in] matIn Imagen de entrada.
   * \param[out] matOut Imagen de salida.
   * \return Si los procesos se ejecutan correctamente devuelve ImgProcessing::Status::OK. 
   * \see ImgProcessing::Status
   */
  ImgProcessing::Status execute(const cv::Mat &matIn, cv::Mat *matOut) const override;

  /*!
   * \brief Establece los parámetros
   * \param[in] modelIn Modelo de color de entrada
   * \param[in] modelOut Modelo de color de salida
   */
  //void setParameters(Model modelIn, Model modelOut);
};

/* ---------------------------------------------------------------------------------- */

/*! \} */ // end of ImgProc

} // End namespace I3D

#endif // HAVE_OPENCV

#endif // I3D_IMG_PROCESSING_H
