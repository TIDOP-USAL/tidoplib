#ifndef I3D_IMG_PROCESSING_H
#define I3D_IMG_PROCESSING_H

#include <list>
#include <memory>
#include <functional>

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

/*!
 * \brief Tipos de procesos de imagen
 */
enum class process_type {
  BILATERAL,          /*!< Filtro bilateral. */
  BLUR,               /*!< Filtro desenfoque. */
  BOX_FILTER,         /*!<  */
  FILTER_2D,          /*!<  */
  GAUSSIAN_BLUR,      /*!< Desenfoque gaussiano. */
  LAPLACIAN,          /*!< Laplaciano de una imagen */
  MEDIAN_BLUR,        /*!<  */
  SOBEL,              /*!< Operador Sobel. */
  CANNY,              /*!< Detector de bordes canny. */
  NORMALIZE,          /*!< Normalización. */
  BINARIZE,           /*!< Binarización. */
  EQUALIZE_HIST,      /*!< Equalización del histograma. */
  FUNCTION_PROCESS,   /*!< Proceso que ejecuta una función */
  /* Operaciones morfológicas */
  MORPH_DILATION,     /*!< Operacion morfologica de dilatación. */
  MORPH_EROTION,      /*!< Operacion morfologica de erosión. */
  MORPH_OPENING,      /*!< Operacion morfologica de apertura. */
  MORPH_CLOSING,      /*!< Operacion morfologica de cierre. */
  MORPH_GRADIENT,     /*!< Operacion morfologica  */
  MORPH_TOPHAT,       /*!< Operacion morfologica  */
  MORPH_BLACKHAT,     /*!< Operacion morfologica  */
  /* Transformación de imagen */
  RESIZE              /*!< Redimensiona la imagen */
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
   * \return Error. Si los procesos se ejecutan correctamente devuelve 0.
   */
  virtual int execute(const cv::Mat &matIn, cv::Mat *matOut) const = 0 ;

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
  void add(std::shared_ptr<ImgProcessing> ip);

  /*!
   * \brief Ejecuta la lista de procesos
   * \param[in] matIn Imagen de entrada
   * \param[out] matOut Imagen de salida
   * \return Error. Si los procesos se ejecutan correctamente devuelve 0.
   */
  int execute(const cv::Mat &matIn, cv::Mat *matOut) const;

  /*!
   * \brief Limpia la lista de procesos
   */
  void clear() { mProcessList.clear(); }

};

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Filtro bilateral
 * El filtro bilateral es un filtro no lineal que es muy eficaz en la eliminación 
 * de ruido manteniendo los bordes afilados. El valor de intensidad en cada pixel 
 * de la imagen es reemplazado por una media ponderada de los valores de intensidad 
 * de los píxeles cercanos.
 */
class I3D_EXPORT BilateralFilter : public ImgProcessing
{
private:

  /*!
   * \brief d
   */
  int mDiameter;

  /*!
   * \brief Núcleo gama para suavizar las diferencias en las intensidades
   */
  double mSigmaColor;

  /*!
   * \brief Núcleo espacial para suavizar las diferencias de coordenadas
   */
  double mSigmaSpace;

  /*!
   * \brief Tipo de borde
   * Tipo de borde cv::BorderTypes
   */
  int mBorderType;

public:

  /*!
   * \brief Constructora de la clase BilateralFilter
   * \param[in] diameter
   * \param[in] sigmaColor Núcleo gama para suavizar las diferencias en las intensidades
   * \param[in] sigmaSpace Núcleo espacial para suavizar las diferencias de coordenadas
   * \param[in] borderType
   */
  BilateralFilter(int diameter, double sigmaColor, double sigmaSpace, int borderType = cv::BORDER_DEFAULT)
    : ImgProcessing(process_type::BILATERAL), mDiameter(diameter), mSigmaColor(sigmaColor), mSigmaSpace(sigmaSpace), mBorderType(borderType) {}

  /*!
   * \brief Aplica el filtro bilateral a una imagen
   * \param[in] matIn Imagen de entrada.
   * \param[out] matOut Imagen de salida.
   * \return Error. Si el proceso se ejecuta correctamente devuelve 0.
   */
  int execute(const cv::Mat &matIn, cv::Mat *matOut) const override;

  /*!
   * \brief Establece los parámetros del filtro bilateral
   * \param[in] diameter
   * \param[in] sigmaColor Núcleo gama para suavizar las diferencias en las intensidades
   * \param[in] sigmaSpace Núcleo espacial para suavizar las diferencias de coordenadas
   * \param[in] borderType
   */
  void setParameters(int diameter, double sigmaColor, double sigmaSpace, int borderType = cv::BORDER_DEFAULT);
};

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Filtro de desenfoque
 * Provoca un suavizado en la imagen resultante
 */
class I3D_EXPORT Blur : public ImgProcessing
{
private:

  /*!
   * \brief Tamaño del kernel
   */
  cv::Size mKernelSize;

  /*!
   * \brief Punto de anclaje.
   * Por defecto es el centro del kernel
   */
  cv::Point mAnchor;

  /*!
   * \brief Tipo de borde
   * Tipo de borde cv::BorderTypes
   */
  int mBorderType;

public:

  /*!
   * \brief Constructora de la clase
   * \param[in] ksize Tamaño del kernel
   * \param[in] anchor Punto de anclaje
   * \param[in] borderType Tipo de borde
   */
  Blur(cv::Size ksize, cv::Point anchor = cv::Point(-1, -1), int borderType = cv::BORDER_DEFAULT)
    : ImgProcessing(process_type::BLUR), mKernelSize(ksize), mAnchor(anchor), mBorderType(borderType) {}

  /*!
   * \brief Aplica el filtro de desenfoque.
   * \param[in] matIn Imagen de entrada.
   * \param[out] matOut Imagen de salida.
   * \return Error. Si el proceso se ejecuta correctamente devuelve 0.
   */
  int execute(const cv::Mat &matIn, cv::Mat *matOut) const override;

  /*!
   * \brief Establece los parámetros del filtro
   * \param[in] ksize Tamaño del kernel
   * \param[in] anchor Punto de anclaje 
   * \param[in] borderType Tipo de borde
   */
  void setParameters(cv::Size ksize, cv::Point anchor = cv::Point(-1, -1), int borderType = cv::BORDER_DEFAULT);

};

/* ---------------------------------------------------------------------------------- */

/*!
* \brief Filtro de desenfoque
* Provoca un suavizado en la imagen resultante
*/
class I3D_EXPORT BoxFilter : public ImgProcessing
{
private:

  int mDepth;

  /*!
   * \brief Tamaño del kernel
   */
  cv::Size mKernelSize;

  /*!
   * \brief Punto de anclaje.
   * Por defecto es el centro del kernel
   */
  cv::Point mAnchor;

  bool mNormalize;

  /*!
   * \brief Tipo de borde
   * Tipo de borde cv::BorderTypes
   */
  int mBorderType;

public:

  /*!
   * \brief Constructora de la clase
   * \param[in] ddepth the output image depth (-1 to use src.depth()).
   * \param[in] ksize Tamaño del kernel
   * \param[in] anchor Punto de anclaje
   * \param[in] normalize Normalizar
   * \param[in] borderType Tipo de borde
   */
  BoxFilter(int ddepth, cv::Size ksize, cv::Point anchor = cv::Point(-1, -1), bool normalize = true, int borderType = cv::BORDER_DEFAULT)
    : ImgProcessing(process_type::BOX_FILTER), mDepth(ddepth), mKernelSize(ksize), mAnchor(anchor), mNormalize(normalize), mBorderType(borderType) {}

  /*!
   * \brief Aplica el filtro de desenfoque.
   * \param[in] matIn Imagen de entrada.
   * \param[out] matOut Imagen de salida.
   * \return Error. Si el proceso se ejecuta correctamente devuelve 0.
   */
  int execute(const cv::Mat &matIn, cv::Mat *matOut) const override;

  /*!
   * \brief Establece los parámetros del filtro
   * \param[in] ddepth the output image depth (-1 to use src.depth()).
   * \param[in] ksize Tamaño del kernel
   * \param[in] anchor Punto de anclaje
   * \param[in] normalize 
   * \param[in] borderType Tipo de borde
   */
  void setParameters(int ddepth, cv::Size ksize, cv::Point anchor = cv::Point(-1, -1), bool normalize = true, int borderType = cv::BORDER_DEFAULT);

};

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Aplica un filtrado mediante una matriz de convolución
 */
class I3D_EXPORT Filter2D : public ImgProcessing
{
private:

  /*!
   * \brief Profundidad de la imagen de destino
   *  ddepth=-1: Misma profundidad que la imagen de origen
   */
  int mDepth;

  /*!
   * \brief kernel Matriz de convolución
   */
  cv::Mat mKernel;

  /*!
   * \brief Punto de anclaje.
   * Por defecto es el centro del kernel
   */
  cv::Point mAnchor;

  /*!
   * \brief Valor opcional añadido a los píxeles filtrados
   */
  double mDelta;

  /*!
   * \brief Método de extrapolación (cv::BorderTypes)
   */
  int mBorderType;

public:

  /*!
   * \brief Constructora clase Filter2D
   * \param[in] ddepth Profundidad de la imagen de destino. ddepth=-1: Misma profundidad que la imagen de origen
   * \param[in] kernel Matriz de convolución
   * \param[in] anchor Punto de anclaje. Por defecto es el centro del kernel
   * \param[in] delta Valor opcional añadido a los píxeles filtrados
   * \param[in] borderType Método de extrapolación
   */
  Filter2D(int ddepth, cv::Mat kernel, cv::Point anchor = cv::Point(-1, -1), double delta = 0, int borderType = cv::BORDER_CONSTANT)
    : ImgProcessing(process_type::FILTER_2D), mDepth(ddepth), mKernel(kernel), mAnchor(anchor), mDelta(delta), mBorderType(borderType) {}

  /*!
   * \brief Ejecuta el proceso
   * \param[in] matIn Imagen de entrada
   * \param[out] matOut Imagen de salida
   * \return Error. Si los procesos se ejecutan correctamente devuelve 0.
   */
  int execute(const cv::Mat &matIn, cv::Mat *matOut) const override;

  /*!
   * \brief Establece los parámetros del filtro de convolución
   * \param[in] ddepth Profundidad de la imagen de destino. ddepth=-1: Misma profundidad que la imagen de origen
   * \param[in] kernel Matriz de convolución
   * \param[in] anchor Punto de anclaje. Por defecto es el centro del kernel
   * \param[in] delta Valor opcional añadido a los píxeles filtrados
   * \param[in] borderType Método de extrapolación
   */
  void setParameters(int ddepth, cv::Mat kernel, cv::Point anchor = cv::Point(-1, -1), double delta = 0, int borderType = cv::BORDER_CONSTANT);
};


/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Desenfoque gaussiano
 */
class I3D_EXPORT GaussianBlur : public ImgProcessing
{
private:

  /*!
   * \brief Tamaño del kernel
   */
  cv::Size mKernelSize;

  /*!
   * \brief Desviación estándar del kernel en la dirección X.
   */
  double mSigmaX;

  /*!
   * \brief Desviación estándar del kernel en la dirección Y.
   */
  double mSigmaY;

  /*!
   * \brief Método de extrapolación (cv::BorderTypes)
   */
  int mBorderType;

public:

  /*!
   * \brief GaussianBlur
   * \param[in] size Tamaño del kernel
   * \param[in] sigmaX Desviación estándar del kernel en la dirección X.
   * \param[in] sigmaY Desviación estándar del kernel en la dirección Y.
   * \param[in] borderType Método de extrapolación (cv::BorderTypes)
   */
  GaussianBlur(cv::Size size, double sigmaX, double sigmaY = 0., int borderType = 4)
    : ImgProcessing(process_type::GAUSSIAN_BLUR), mKernelSize(size), mSigmaX(sigmaX), mSigmaY(sigmaY), mBorderType(borderType) {}

  /*!
   * \brief Ejecuta el proceso
   * \param[in] matIn Imagen de entrada
   * \param[out] matOut Imagen de salida
   * \return Error. Si los procesos se ejecutan correctamente devuelve 0.
   */
  int execute(const cv::Mat &matIn, cv::Mat *matOut) const override;

  /*!
   * \brief Establece los parámetros
   * \param[in] kSize Tamaño del kernel
   * \param[in] sigmaX Desviación estándar del kernel en la dirección X
   * \param[in] sigmaY Desviación estándar del kernel en la dirección Y
   * \param[in] borderType Método de extrapolación (cv::BorderTypes)
   */
  void setParameters(cv::Size kSize, double sigmaX, double sigmaY = 0, int borderType = 4);

};

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Laplaciano
 * Calcula el laplaciano de un imagen
 */
class I3D_EXPORT Laplacian : public ImgProcessing
{
private:

  /*!
   * \brief Profundidad deseada de la imagen de destino
   */
  int mDepth;

  /*!
   * \brief Apertura del filtro
   */
  int mKernelsize;
  
  /*!
   * \brief Factor de escala opcional para los valores calculados por el laplaciano.
   * Por defecto no se aplica escala.
   */
  double mScale;
  
  /*!
   * \brief Valor opcional añadido a los píxeles filtrados
   */
  double mDelta;
  
  /*!
   * \brief Método de extrapolación (cv::BorderTypes)
   */
  int mBorderType;

public:

  /*!
   * \brief Laplacian
   * \param[in] ddepth Profundidad deseada de la imagen de destino
   * \param[in] ksize Apertura del filtro
   * \param[in] scale Factor de escala opcional para los valores calculados por el laplaciano. Por defecto no se aplica escala.
   * \param[in] delta Valor opcional añadido a los píxeles filtrados
   * \param[in] bordertype Método de extrapolación (cv::BorderTypes)
   */
  Laplacian(int ddepth, int ksize, double scale = 1, double delta = 0, int bordertype = cv::BORDER_DEFAULT)
    : ImgProcessing(process_type::LAPLACIAN), mDepth(ddepth), mKernelsize(ksize), mScale(scale), mDelta(delta), mBorderType(bordertype) {}

  /*!
   * \brief Ejecuta el proceso
   * \param[in] matIn Imagen de entrada
   * \param[out] matOut Imagen de salida
   * \return Error. Si los procesos se ejecutan correctamente devuelve 0.
   */
  int execute(const cv::Mat &matIn, cv::Mat *matOut) const override;

  /*!
   * \brief Establece los parámetros
   * \param[in] ddepth Profundidad deseada de la imagen de destino
   * \param[in] ksize Apertura del filtro
   * \param[in] scale Factor de escala opcional para los valores calculados por el laplaciano. Por defecto no se aplica escala.
   * \param[in] delta Valor opcional añadido a los píxeles filtrados
   * \param[in] bordertype Método de extrapolación (cv::BorderTypes)
   */
  void setParameters(int ddepth, int ksize, double scale = 1, double delta = 0, int bordertype = cv::BORDER_DEFAULT);

}; 

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Filtro de media
 * Suaviza una imagen con un filtro de media. 
 */
class I3D_EXPORT MedianBlur : public ImgProcessing
{
private:

  /*!
   * \brief Apertura del filtro
   */
  int mKernelSize;

public:

  /*!
   * \brief Constructora
   * \param ksize Tamaño del filtro
   */
  MedianBlur(int ksize)
    : ImgProcessing(process_type::MEDIAN_BLUR), mKernelSize(ksize) {}

  /*!
   * \brief Ejecuta el proceso
   * \param[in] matIn Imagen de entrada
   * \param[out] matOut Imagen de salida
   * \return Error. Si los procesos se ejecutan correctamente devuelve 0.
   */
  int execute(const cv::Mat &matIn, cv::Mat *matOut) const override;

  /*!
   * \brief Establece los parámetros
   * \param[in] ksize Tamaño del filtro
   */
  void setParameters(int ksize);

};

/* ---------------------------------------------------------------------------------- */
/*!
 * \brief Sobel
 */
class I3D_EXPORT Sobel : public ImgProcessing
{
private:

  /*!
   * \brief dx
   */
  int mDx;

  /*!
   * \brief dy
   */
  int mDy;

  /*!
   * \brief mKernelSize
   */
  int mKernelSize;

  /*!
   * \brief scale
   */
  double mScale;

  /*!
   * \brief delta
   */
  double mDelta;

  /*!
   * \brief mDepth
   */
  int mDepth;

  /*!
   * \brief thresh
   */
  double mThresh;

  /*!
   * \brief maxval
   */
  double mMaxVal;

  /*!
   * \brief bordertype
   */
  int mBorderType;

public:

  /*!
   * \brief Constructora
   * \param[in] dx
   * \param[in] dy
   * \param[in] ksize
   * \param[in] scale
   * \param[in] delta
   * \param[in] ddepth
   * \param[in] thresh
   * \param[in] maxval
   * \param[in] bordertype
   */
  Sobel(int dx, int dy, int ksize = 3, double scale = 1., double delta = 0.,
        int ddepth = CV_16S, double thresh = 50., double maxval = 200.,
        int bordertype = cv::BORDER_DEFAULT)
    : ImgProcessing(process_type::SOBEL),
    mDx(dx), mDy(dy), mKernelSize(ksize), mScale(scale), mDelta(delta), mDepth(ddepth),
    mThresh(thresh), mMaxVal(maxval), mBorderType(bordertype) {}

  /*!
   * \brief Ejecuta el proceso
   * \param[in] matIn Imagen de entrada
   * \param[out] matOut Imagen de salida
   * \return Error. Si los procesos se ejecutan correctamente devuelve 0.
   */
  int execute(const cv::Mat &matIn, cv::Mat *matOut) const override;

  /*!
   * \brief Establece los parámetros
   * \param[in] dx Orden de la derivada x
   * \param[in] dy Orden de la derivada y
   * \param[in] ksize Tamaño kernel
   * \param[in] scale Factor de escala opcional para los valores de las derivadas calculadas
   * \param[in] delta
   * \param[in] ddepth
   * \param[in] thresh
   * \param[in] maxval
   * \param[in] bordertype
   */
  void setParameters(int dx, int dy, int ksize = 3, double scale = 1., double delta = 0., int ddepth = CV_16S, double thresh = 50., double maxval = 200., int bordertype = cv::BORDER_DEFAULT);

};

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Detector de bordes canny
 */
class I3D_EXPORT Canny : public ImgProcessing
{
private:

  /*!
   * \brief Primer umbral para el procedimiento de histéresis
   */
  double mThreshold1;

  /*!
   * \brief Segundo umbral para el procedimiento de histéresis
   */
  double mThreshold2;

public:

  /*!
   * \brief Constructora
   * \param[in] threshold1
   * \param[in] threshold2
   */
  Canny(double threshold1 = 0.0, double threshold2 = 0.0)
    : ImgProcessing(process_type::CANNY), mThreshold1(threshold1), mThreshold2(threshold2) {}

  /*!
   * \brief Ejecuta el proceso
   * \param[in] matIn Imagen de entrada
   * \param[out] matOut Imagen de salida
   * \return Error. Si los procesos se ejecutan correctamente devuelve 0.
   */
  int execute(const cv::Mat &matIn, cv::Mat *matOut) const override;

  /*!
   * \brief Establece los parámetros para el detector de bordes canny
   * \param[in] threshold1 Primer umbral para el procedimiento de histéresis
   * \param[in] threshold2 Segundo umbral para el procedimiento de histéresis
   */
  void setParameters(double threshold1, double threshold2);

};

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
   * \return Error. Si los procesos se ejecutan correctamente devuelve 0.
   */
  int execute(const cv::Mat &matIn, cv::Mat *matOut) const override;

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
   * \return Error. Si los procesos se ejecutan correctamente devuelve 0.
   */
  int execute(const cv::Mat &matIn, cv::Mat *matOut) const override;

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
   * \return Error. Si los procesos se ejecutan correctamente devuelve 0.
   */
  int execute(const cv::Mat &matIn, cv::Mat *matOut) const override;

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
   * \return Error. Si los procesos se ejecutan correctamente devuelve 0.
   */
  int execute(const cv::Mat &matIn, cv::Mat *matOut) const override;

};

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
   * \return Error. Si los procesos se ejecutan correctamente devuelve 0.
   */
  int execute(const cv::Mat &matIn, cv::Mat *matOut) const override;

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
   * \return Error. Si los procesos se ejecutan correctamente devuelve 0.
   */
  int execute(const cv::Mat &matIn, cv::Mat *matOut) const override;

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
private:

  /*!
   * \brief Ancho
   */
  int mWidth;

  /*!
   * \brief Alto
   */
  int mHeight;

  //Añadir color de fondo, posición, ...
  /*Color mColor;*/

public:

  /*!
   * \brief Constructora
   * \param[in] width Nuevo ancho
   * \param[in] height Nuevo alto
   * \param[in] color Color
   */
  ResizeCanvas(int width, int height/*, const Color &color = Color()*/)
    : ImgProcessing(process_type::RESIZE), mWidth(width), mHeight(height)/*, mColor(color)*/ {}

  /*!
   * \brief Ejecuta el proceso.
   * \param[in] matIn Imagen de entrada.
   * \param[out] matOut Imagen de salida.
   * \return Error. Si los procesos se ejecutan correctamente devuelve 0.
   */
  int execute(const cv::Mat &matIn, cv::Mat *matOut) const override;

  /*!
   * \brief Establece los parámetros
   * \param[in] width Ancho
   * \param[in] height Alto
   * \param[in] color Color
   */
  void setParameters( int width, int height/*, const Color &color*/);

};


/* ---------------------------------------------------------------------------------- */

/*! \} */ // end of imgTransf

/* ---------------------------------------------------------------------------------- */

/*! \} */ // end of ImgProc

} // End namespace I3D

#endif // I3D_IMG_PROCESSING_H
