#ifndef TL_IMGPROCESS_PROCESSING_H
#define TL_IMGPROCESS_PROCESSING_H

#include "config_tl.h"

#include <list>
#include <memory>
#include <functional>

#ifdef HAVE_OPENCV
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "tidop/core/defs.h"
#include "tidop/graphic/color.h"

namespace tl
{


/*! \defgroup ImgProc Procesado de imagen
 *  Filtrado y preprocesado de imagenes
 *  \{
 */

enum class ProcessExit
{
  success,
  failure
#ifdef TL_ENABLE_DEPRECATED_METHODS
    ,
  SUCCESS = success,
  FAILURE = failure
#endif // TL_ENABLE_DEPRECATED_METHODS
};

/*!
 * \brief Tipos de procesado de imagen
 */
enum class process_type {
  /* Filtros */
  bilateral,          /*!< Filtro bilateral. */
  blur,               /*!< Filtro desenfoque. */
  box_filter,         /*!<  */
  filter_2d,          /*!<  */
  convolution,        /*!<  Convolución */ 
  gaussian_blur,      /*!< Desenfoque gaussiano. */
  laplacian,          /*!< Laplaciano de una imagen */
  median_blur,        /*!<  */
  sobel,              /*!< Operador Sobel. */
  canny,              /*!< Detector de bordes canny. */
  /* Operaciones morfológicas */
  morph_dilation,     /*!< Operacion morfologica de dilatación. */
  morph_erotion,      /*!< Operacion morfologica de erosión. */
  morph_opening,      /*!< Operacion morfologica de apertura. */
  morph_closing,      /*!< Operacion morfologica de cierre. */
  morph_gradient,     /*!< Operacion morfologica  */
  morph_tophat,       /*!< Operacion morfologica  */
  morph_blackhat,     /*!< Operacion morfologica  */
  /* Transformación de imagen */
  resize,             /*!< Redimensiona la imagen */
  resize_canvas,
  normalize,          /*!< Normalización. */
  binarize,           /*!< Binarización. */
  equalize_hist,      /*!< Equalización del histograma. */
  function_process,   /*!< Proceso que ejecuta una función */
  /* Balance de blancos */
  grayworld,
  whitepatch,
  /*  */
  color_conversion,
  correlation
#ifdef TL_ENABLE_DEPRECATED_METHODS
    ,
  BILATERAL = bilateral,
  BLUR = blur,
  BOX_FILTER = box_filter,
  FILTER_2D = filter_2d,
  CONVOLUTION = convolution,
  GAUSSIAN_BLUR = gaussian_blur,
  LAPLACIAN = laplacian,
  MEDIAN_BLUR = median_blur,
  SOBEL = sobel,
  CANNY = canny,
  MORPH_DILATION = morph_dilation,
  MORPH_EROTION = morph_erotion,
  MORPH_OPENING = morph_opening,
  MORPH_CLOSING = morph_closing,
  MORPH_GRADIENT = morph_gradient,
  MORPH_TOPHAT = morph_tophat,
  MORPH_BLACKHAT = morph_blackhat,
  RESIZE = resize,
  RESIZE_CANVAS = resize_canvas,
  NORMALIZE = normalize,
  BINARIZE = binarize,
  EQUALIZE_HIST = equalize_hist,
  FUNCTION_PROCESS = function_process,
  GRAYWORLD = grayworld,
  WHITEPATCH = whitepatch,
  COLOR_CONVERSION = color_conversion,
  CORRELATION = correlation
#endif // TL_ENABLE_DEPRECATED_METHODS
};

/*!
 * \brief Clase para gestionar los procesos de imagen
 * Clase que permite tener una interfaz común para aplicar filtros
 * u otros procesos a una imagen.
 */
class TL_EXPORT ImgProcessing
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
    ok,                       // El proceso se ha ejecutado correctamente
    incorrect_input_data,     // Error de los datos de entrada
    process_error             // Error durante el proceso.
#ifdef TL_ENABLE_DEPRECATED_METHODS
    ,
    OK = ok,
    INCORRECT_INPUT_DATA = incorrect_input_data,
    PROCESS_ERROR = process_error
#endif // TL_ENABLE_DEPRECATED_METHODS
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
  virtual ImgProcessing::Status execute(const cv::Mat &matIn, cv::Mat *matOut) const = 0;

};

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Contenedor de procesos de imagen que permite su ejecución secuencial.
 * \code
 * TL::ImgProcessingList imgprolist;
 * imgprolist.add(std::make_shared<TL::Normalize>(0., 255.));
 * imgprolist.add(std::make_shared<TL::BilateralFilter>(5, 50., 50.));
 * imgprolist.add(std::make_shared<TL::Erotion>(1));
 * imgprolist.add(std::make_shared<TL::Dilate>(1));
 * cv::Mat out;
 * imgprolist.execute(in, &out);
 * \endcode
 */
class TL_EXPORT ImgProcessingList
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
  ImgProcessingList(const ImgProcessingList &imgProcList)
    : mProcessList(imgProcList.mProcessList) {}

  /*!
   * \brief Constructor de lista
   * \param[in] imgProcList Listado de procesos
   */
  ImgProcessingList(std::initializer_list<std::shared_ptr<ImgProcessing>> imgProcList)
    : mProcessList(imgProcList) {}

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
class morphologicalOperation 
  : public ImgProcessing
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
   * \param[in] borderValue
   */
  void setParameters(int size, cv::MorphShapes type = cv::MORPH_RECT,
    cv::Point anchor = cv::Point(-1, -1), int iterations = 1, 
    int borderType = cv::BORDER_CONSTANT, const cv::Scalar &borderValue = cv::morphologyDefaultBorderValue());

};


/*!
 * \brief Operacion morfologica de dilatación.
 */
class TL_EXPORT Dilate : public morphologicalOperation
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
  Dilate(int size, cv::MorphShapes type = cv::MORPH_RECT, cv::Point anchor = cv::Point(-1, -1),
         int iterations = 1, int borderType = cv::BORDER_CONSTANT, 
         const cv::Scalar &borderValue = cv::morphologyDefaultBorderValue())
    : morphologicalOperation(process_type::morph_dilation)
  {
    setParameters(size, type, anchor, iterations, borderType, borderValue );
  }

};

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Operacion morfologica de erosión
 */
class TL_EXPORT Erotion : public morphologicalOperation
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
  Erotion(int size, cv::MorphShapes type = cv::MORPH_RECT, cv::Point anchor = cv::Point(-1, -1),
    int iterations = 1, int borderType = cv::BORDER_CONSTANT,
    const cv::Scalar &borderValue = cv::morphologyDefaultBorderValue())
    : morphologicalOperation(process_type::morph_erotion) 
  {
    setParameters(size, type, anchor, iterations, borderType, borderValue);
  }

};

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Operación morfológica de apertura
 * Esta operación consite en aplicar una erosion de la imagen
 * seguida de una dilatación
 */
class TL_EXPORT Opening : public morphologicalOperation
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
  Opening(int size, cv::MorphShapes type = cv::MORPH_RECT, cv::Point anchor = cv::Point(-1, -1),
    int iterations = 1, int borderType = cv::BORDER_CONSTANT,
    const cv::Scalar &borderValue = cv::morphologyDefaultBorderValue())
    : morphologicalOperation(process_type::morph_opening)
  {
    setParameters(size, type, anchor, iterations, borderType, borderValue);
  }

};

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Operación morfológica de apertura
 * Esta operación consite en aplicar una dilatación de la imagen
 * seguida de una erosión
 */
class TL_EXPORT Closing : public morphologicalOperation
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
  Closing(int size, cv::MorphShapes type = cv::MORPH_RECT, cv::Point anchor = cv::Point(-1, -1),
    int iterations = 1, int borderType = cv::BORDER_CONSTANT,
    const cv::Scalar &borderValue = cv::morphologyDefaultBorderValue())
    : morphologicalOperation(process_type::morph_closing)
  {
    setParameters(size, type, anchor, iterations, borderType, borderValue);
  }

};

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Operación morfológica gradiente
 * Gradient = dilation - erosion
 * 
 */
class TL_EXPORT Gradient : public morphologicalOperation
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
  Gradient(int size, cv::MorphShapes type = cv::MORPH_RECT, cv::Point anchor = cv::Point(-1, -1),
    int iterations = 1, int borderType = cv::BORDER_CONSTANT,
    const cv::Scalar &borderValue = cv::morphologyDefaultBorderValue())
    : morphologicalOperation(process_type::morph_gradient)
  {
    setParameters(size, type, anchor, iterations, borderType, borderValue);
  }
};

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Operación morfológica Top Hat
 * Es la diferencia entre una imagen y el resultado de aplicar una operación de 
 * apertura sobre la misma imagen
 */
class TL_EXPORT TopHat : public morphologicalOperation
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
  TopHat(int size, cv::MorphShapes type = cv::MORPH_RECT, cv::Point anchor = cv::Point(-1, -1),
    int iterations = 1, int borderType = cv::BORDER_CONSTANT,
    const cv::Scalar &borderValue = cv::morphologyDefaultBorderValue())
    : morphologicalOperation(process_type::morph_tophat)
  {
    setParameters(size, type, anchor, iterations, borderType, borderValue);
  }

};

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Operación morfológica Black Hat
 * Esta operación consite en aplicar un cierre a una imagen y restar el resultado por la
 * la imagen original
 */
class TL_EXPORT BlackHat : public morphologicalOperation
{
public:

  /*!
   * \brief Constructora clase Closing
   * \param[in] size
   * \param[in] shapes
   * \param[in] anchor Punto de anclaje. Por defecto es el centro del kernel
   * \param[in] iterations
   * \param[in] borderType Método de extrapolación
   * \param[in] borderValue
   */
  BlackHat(int size, cv::MorphShapes shapes = cv::MORPH_RECT, cv::Point anchor = cv::Point(-1, -1),
    int iterations = 1, int borderType = cv::BORDER_CONSTANT,
    const cv::Scalar &borderValue = cv::morphologyDefaultBorderValue())
    : morphologicalOperation(process_type::morph_blackhat)
  {
    setParameters(size, shapes, anchor, iterations, borderType, borderValue);
  }

};

/*! \} */ // end of MorphOper

/*! \} */ // end of Filters


/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Clase Normalize
 */
class TL_EXPORT Normalize : public ImgProcessing
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
    : ImgProcessing(process_type::normalize), mLowRange(lowRange), mUpRange(upRange) {}

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
class TL_EXPORT Binarize 
  : public ImgProcessing
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
  Binarize(double thresh = 0., double maxVal = 0., bool bInverse = false);

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
  void setInverse(bool inverse = true);

  /*!
   * \brief GetInverse
   * \return
   */
  bool getInverse() const;

};

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Ecualización del histograma.
 * Mejora del contraste de la imagen mediante la ecualización del histograma
 */
class TL_EXPORT EqualizeHistogram 
  : public ImgProcessing
{

public:

  /*!
   * \brief Constructora de la clase
   */
  EqualizeHistogram();

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
 * TL::ImgProcessingList imgprolist{ fProcess1, fProcess2 };
 * \endcode
 */
class TL_EXPORT FunctionProcess : public ImgProcessing
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
    : ImgProcessing(process_type::function_process), f(f) {}

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

/*! \} */ // end of ImgProc

} // End namespace TL

#endif // HAVE_OPENCV

#endif // TL_IMGPROCESS_PROCESSING_H
