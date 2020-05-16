#ifndef TL_IMGPROCESS_FILTERS_H
#define TL_IMGPROCESS_FILTERS_H

#include "config_tl.h"

#ifdef HAVE_OPENCV
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include "tidop/core/defs.h"
#include "tidop/imgprocess/imgprocess.h"

namespace tl
{

/*! \addtogroup ImgProc
 *  \{
 */


/*! \defgroup Filters Filtros de imagen
 *  
 * Tipos de filtros:
 * 
 * - Suavizado
 * - Realce
 * - Eliminación de ruido
 * - Detección de bordes
 *
 * <h3>Filtros de paso bajo (suavizado de la imagen)</h3>
 * 
 * - Filtro de media: reemplaza cada píxel por el valor medio de sus contiguos
 * - Mediana: sustituye por el valor de la mediana de los píxeles vecinos (normalmente se comporta mejor que el de promedio).
 * - Gaussiano: aproximación a la distribución gaussiana.
 *
 *
 * <h3>Filtros de realce de la imagen</h3>
 *
 *
 * <h3>Filtros de eliminación de ruido:</h3>
 * 
 * El ruido es la variación aleatoria del brillo o el color en las imágenes digitales producido por el dispositivo de entrada.
 *
 * <h4>Tipos de ruido:</h4>
 *
 * - Ruido Impulsional o "Sal y Pimienta": los píxeles de la imagen son muy diferentes en color o intensidad a los píxeles circundantes. Generalmente, este tipo de ruido, afectará a una pequeña cantidad de píxeles de la imagen.
 * - Ruido Gaussiano: En el ruido de tipo gaussiano, todos y cada uno de los píxeles que componen la imagen se ven afectados de acuerdo con una distribución normal o gaussiana.
 * - Ruido uniforme: Toma valores en un determinado intervalo de forma equiprobable. Se da en un menor número de situaciones reales.
 *
 * <h3>Filtros de detección de bordes</h3>
 *
 * 
 *  \{
 */

/*!
 * \brief Filtro bilateral
 * El filtro bilateral es un filtro no lineal que es muy eficaz en la eliminación 
 * de ruido manteniendo los bordes afilados. El valor de intensidad en cada pixel 
 * de la imagen es reemplazado por una media ponderada de los valores de intensidad 
 * de los píxeles cercanos.
 */
class TL_EXPORT BilateralFilter 
  : public ImgProcessing
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
  BilateralFilter(int diameter, double sigmaColor, double sigmaSpace, int borderType = cv::BORDER_DEFAULT);

  /*!
   * \brief Aplica el filtro bilateral a una imagen
   * \param[in] matIn Imagen de entrada.
   * \param[out] matOut Imagen de salida.
   * \return Si los procesos se ejecutan correctamente devuelve ImgProcessing::Status::OK. 
   * \see ImgProcessing::Status
   */
  ImgProcessing::Status execute(const cv::Mat &matIn, cv::Mat *matOut) const override;

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
class TL_EXPORT Blur 
  : public ImgProcessing
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
  Blur(cv::Size ksize, cv::Point anchor = cv::Point(-1, -1), int borderType = cv::BORDER_DEFAULT);

  /*!
   * \brief Aplica el filtro de desenfoque.
   * \param[in] matIn Imagen de entrada.
   * \param[out] matOut Imagen de salida.
   * \return Si los procesos se ejecutan correctamente devuelve ProcessExit::SUCCESS. 
   * \return Si los procesos se ejecutan correctamente devuelve ImgProcessing::Status::OK. 
   * \see ImgProcessing::Status
   */
  ImgProcessing::Status execute(const cv::Mat &matIn, cv::Mat *matOut) const override;

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
class TL_EXPORT BoxFilter 
  : public ImgProcessing
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
  BoxFilter(int ddepth, cv::Size ksize, cv::Point anchor = cv::Point(-1, -1), bool normalize = true, int borderType = cv::BORDER_DEFAULT);

  /*!
   * \brief Aplica el filtro de desenfoque.
   * \param[in] matIn Imagen de entrada.
   * \param[out] matOut Imagen de salida.
   * \return Si los procesos se ejecutan correctamente devuelve ImgProcessing::Status::OK. 
   * \see ImgProcessing::Status
   */
  ImgProcessing::Status execute(const cv::Mat &matIn, cv::Mat *matOut) const override;

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

#ifdef TL_ENABLE_DEPRECATED_METHODS
/*!
 * \brief Aplica un filtrado mediante una matriz de convolución
 */
class TL_DEPRECATED("Convolution", "2.0") TL_EXPORT Filter2D 
  : public ImgProcessing
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
   * \param[in] kernel Matriz o máscara de convolución
   * \param[in] anchor Punto de anclaje. Por defecto es el centro del kernel
   * \param[in] delta Valor opcional añadido a los píxeles filtrados
   * \param[in] borderType Método de extrapolación
   */
  Filter2D(int ddepth, cv::Mat kernel, cv::Point anchor = cv::Point(-1, -1), double delta = 0, int borderType = cv::BORDER_CONSTANT);

  /*!
   * \brief Ejecuta el proceso
   * \param[in] matIn Imagen de entrada
   * \param[out] matOut Imagen de salida
   * \return Si los procesos se ejecutan correctamente devuelve ImgProcessing::Status::OK. 
   * \see ImgProcessing::Status
   */
  ImgProcessing::Status execute(const cv::Mat &matIn, cv::Mat *matOut) const override;

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

#endif // TL_ENABLE_DEPRECATED_METHODS

/*!
 * \brief Aplica un filtrado mediante una matriz de convolución
 */
class TL_EXPORT Convolution 
  : public ImgProcessing
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
   * \brief Constructora clase Convolution
   * \param[in] ddepth Profundidad de la imagen de destino. ddepth=-1: Misma profundidad que la imagen de origen
   * \param[in] kernel Matriz de convolución
   * \param[in] anchor Punto de anclaje. Por defecto es el centro del kernel
   * \param[in] delta Valor opcional añadido a los píxeles filtrados
   * \param[in] borderType Método de extrapolación
   */
  Convolution(int ddepth, cv::Mat kernel, cv::Point anchor = cv::Point(-1, -1), double delta = 0, int borderType = cv::BORDER_CONSTANT);

  /*!
   * \brief Ejecuta el proceso
   * \param[in] matIn Imagen de entrada
   * \param[out] matOut Imagen de salida
   * \return Si los procesos se ejecutan correctamente devuelve ImgProcessing::Status::OK. 
   * \see ImgProcessing::Status
   */
  ImgProcessing::Status execute(const cv::Mat &matIn, cv::Mat *matOut) const override;

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
class TL_EXPORT GaussianBlur 
  : public ImgProcessing
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
  GaussianBlur(cv::Size size, double sigmaX, double sigmaY = 0., int borderType = 4);

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
class TL_EXPORT Laplacian 
  : public ImgProcessing
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
  Laplacian(int ddepth, int ksize, double scale = 1, double delta = 0, int bordertype = cv::BORDER_DEFAULT);

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
 * Suaviza una imagen con un filtro de media 
 */
class TL_EXPORT MedianBlur
  : public ImgProcessing
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
  MedianBlur(int ksize);

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
   * \param[in] ksize Tamaño del filtro
   */
  void setParameters(int ksize);

};

/* ---------------------------------------------------------------------------------- */
/*!
 * \brief Sobel
 */
class TL_EXPORT Sobel
  : public ImgProcessing
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
        int bordertype = cv::BORDER_DEFAULT);

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
class TL_EXPORT Canny 
  : public ImgProcessing
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
  Canny(double threshold1 = 0.0, double threshold2 = 0.0);

  /*!
   * \brief Ejecuta el proceso
   * \param[in] matIn Imagen de entrada
   * \param[out] matOut Imagen de salida
   * \return Si los procesos se ejecutan correctamente devuelve ImgProcessing::Status::OK. 
   * \see ImgProcessing::Status
   */
  ImgProcessing::Status execute(const cv::Mat &matIn, cv::Mat *matOut) const override;

  /*!
   * \brief Establece los parámetros para el detector de bordes canny
   * \param[in] threshold1 Primer umbral para el procedimiento de histéresis
   * \param[in] threshold2 Segundo umbral para el procedimiento de histéresis
   */
  void setParameters(double threshold1, double threshold2);

};



/*! \} */ // end of Filters

/*! \} */ // end of ImgProc

} // End namespace tl

#endif // HAVE_OPENCV

#endif // TL_IMGPROCESS_FILTERS_H
