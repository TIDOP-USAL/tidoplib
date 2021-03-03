/************************************************************************
 *                                                                      *
 * Copyright (C) 2020 by Tidop Research Group                           *
 *                                                                      *
 * This file is part of TidopLib                                        *
 *                                                                      *
 * TidopLib is free software: you can redistribute it and/or modify     *
 * it under the terms of the GNU General Public License as published by *
 * the Free Software Foundation, either version 3 of the License, or    *
 * (at your option) any later version.                                  *
 *                                                                      *
 * TidopLib is distributed in the hope that it will be useful,          *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.      *
 *                                                                      *
 * @license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>                *
 *                                                                      *
 ************************************************************************/

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
  : public ImageProcess
{

public:

  /*!
   * \brief Constructora de la clase BilateralFilter
   * \param[in] diameter 
   * \param[in] sigmaColor Núcleo gama para suavizar las diferencias en las intensidades
   * \param[in] sigmaSpace Núcleo espacial para suavizar las diferencias de coordenadas
   * \param[in] borderType
   */
  BilateralFilter(int diameter, 
                  double sigmaColor, 
                  double sigmaSpace, 
                  int borderType = cv::BORDER_DEFAULT);

  /*!
   * \brief Aplica el filtro bilateral a una imagen
   * \param[in] matIn Imagen de entrada.
   * \param[out] matOut Imagen de salida.
   */
  void run(const cv::Mat &matIn, cv::Mat &matOut) const override;

  /*!
   * \brief Establece los parámetros del filtro bilateral
   * \param[in] diameter
   * \param[in] sigmaColor Núcleo gama para suavizar las diferencias en las intensidades
   * \param[in] sigmaSpace Núcleo espacial para suavizar las diferencias de coordenadas
   * \param[in] borderType
   */
  void setParameters(int diameter, 
                     double sigmaColor, 
                     double sigmaSpace, 
                     int borderType = cv::BORDER_DEFAULT);

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

};

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Filtro de desenfoque
 * Provoca un suavizado en la imagen resultante
 */
class TL_EXPORT Blur 
  : public ImageProcess
{

public:

  /*!
   * \brief Constructora de la clase
   * \param[in] ksize Tamaño del kernel
   * \param[in] anchor Punto de anclaje
   * \param[in] borderType Tipo de borde
   */
  Blur(const cv::Size &ksize, 
       const cv::Point &anchor = cv::Point(-1, -1), 
       int borderType = cv::BORDER_DEFAULT);

  /*!
   * \brief Aplica el filtro de desenfoque.
   * \param[in] matIn Imagen de entrada.
   * \param[out] matOut Imagen de salida.
   */
  void run(const cv::Mat &matIn, cv::Mat &matOut) const override;

  /*!
   * \brief Establece los parámetros del filtro
   * \param[in] ksize Tamaño del kernel
   * \param[in] anchor Punto de anclaje 
   * \param[in] borderType Tipo de borde
   */
  void setParameters(const cv::Size ksize, 
                     const cv::Point &anchor = cv::Point(-1, -1), 
                     int borderType = cv::BORDER_DEFAULT);

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
};

/* ---------------------------------------------------------------------------------- */

/*!
* \brief Filtro de desenfoque
* Provoca un suavizado en la imagen resultante
*/
class TL_EXPORT BoxFilter 
  : public ImageProcess
{

public:

  /*!
   * \brief Constructora de la clase
   * \param[in] ddepth the output image depth (-1 to use src.depth()).
   * \param[in] ksize Tamaño del kernel
   * \param[in] anchor Punto de anclaje
   * \param[in] normalize Normalizar
   * \param[in] borderType Tipo de borde
   */
  BoxFilter(int ddepth, 
            const cv::Size &ksize, 
            const cv::Point &anchor = cv::Point(-1, -1), 
            bool normalize = true, 
            int borderType = cv::BORDER_DEFAULT);

  /*!
   * \brief Aplica el filtro de desenfoque.
   * \param[in] matIn Imagen de entrada.
   * \param[out] matOut Imagen de salida.
   */
  void run(const cv::Mat &matIn, cv::Mat &matOut) const override;

  /*!
   * \brief Establece los parámetros del filtro
   * \param[in] ddepth the output image depth (-1 to use src.depth()).
   * \param[in] ksize Tamaño del kernel
   * \param[in] anchor Punto de anclaje
   * \param[in] normalize 
   * \param[in] borderType Tipo de borde
   */
  void setParameters(int ddepth, 
                     const cv::Size &ksize, 
                     const cv::Point &anchor = cv::Point(-1, -1), 
                     bool normalize = true, 
                     int borderType = cv::BORDER_DEFAULT);

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


};

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Aplica un filtrado mediante una matriz de convolución
 */
class TL_EXPORT Convolution 
  : public ImageProcess
{

public:

  /*!
   * \brief Constructora clase Convolution
   * \param[in] ddepth Profundidad de la imagen de destino. ddepth=-1: Misma profundidad que la imagen de origen
   * \param[in] kernel Matriz de convolución
   * \param[in] anchor Punto de anclaje. Por defecto es el centro del kernel
   * \param[in] delta Valor opcional añadido a los píxeles filtrados
   * \param[in] borderType Método de extrapolación
   */
  Convolution(int ddepth, 
              const cv::Mat &kernel, 
              const cv::Point &anchor = cv::Point(-1, -1), 
              double delta = 0, 
              int borderType = cv::BORDER_CONSTANT);

  /*!
   * \brief Ejecuta el proceso
   * \param[in] matIn Imagen de entrada
   * \param[out] matOut Imagen de salida
   */
  void run(const cv::Mat &matIn, cv::Mat &matOut) const override;

  /*!
   * \brief Establece los parámetros del filtro de convolución
   * \param[in] ddepth Profundidad de la imagen de destino. ddepth=-1: Misma profundidad que la imagen de origen
   * \param[in] kernel Matriz de convolución
   * \param[in] anchor Punto de anclaje. Por defecto es el centro del kernel
   * \param[in] delta Valor opcional añadido a los píxeles filtrados
   * \param[in] borderType Método de extrapolación
   */
  void setParameters(int ddepth, 
                     const cv::Mat &kernel, 
                     const cv::Point &anchor = cv::Point(-1, -1), 
                     double delta = 0, 
                     int borderType = cv::BORDER_CONSTANT);

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
};

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Desenfoque gaussiano
 */
class TL_EXPORT GaussianBlur 
  : public ImageProcess
{

public:

  /*!
   * \brief GaussianBlur
   * \param[in] kernelSize Tamaño del kernel
   * \param[in] sigmaX Desviación estándar del kernel en la dirección X.
   * \param[in] sigmaY Desviación estándar del kernel en la dirección Y.
   * \param[in] borderType Método de extrapolación (cv::BorderTypes)
   */
  GaussianBlur(const cv::Size &kernelSize, 
               double sigmaX, 
               double sigmaY = 0., 
               int borderType = 4);

  /*!
   * \brief Ejecuta el proceso
   * \param[in] matIn Imagen de entrada
   * \param[out] matOut Imagen de salida
   */
  void run(const cv::Mat &matIn, cv::Mat &matOut) const override;

  /*!
   * \brief Establece los parámetros
   * \param[in] kernelSize Tamaño del kernel
   * \param[in] sigmaX Desviación estándar del kernel en la dirección X
   * \param[in] sigmaY Desviación estándar del kernel en la dirección Y
   * \param[in] borderType Método de extrapolación (cv::BorderTypes)
   */
  void setParameters(const cv::Size &kernelSize, 
                     double sigmaX, 
                     double sigmaY = 0, 
                     int borderType = 4);

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

};

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Laplaciano
 * Calcula el laplaciano de un imagen
 */
class TL_EXPORT Laplacian 
  : public ImageProcess
{

public:

  /*!
   * \brief Laplacian
   * \param[in] ddepth Profundidad deseada de la imagen de destino
   * \param[in] ksize Apertura del filtro
   * \param[in] scale Factor de escala opcional para los valores calculados por el laplaciano. Por defecto no se aplica escala.
   * \param[in] delta Valor opcional añadido a los píxeles filtrados
   * \param[in] bordertype Método de extrapolación (cv::BorderTypes)
   */
  Laplacian(int ddepth, 
            int ksize, 
            double scale = 1, 
            double delta = 0, 
            int bordertype = cv::BORDER_DEFAULT);

  /*!
   * \brief Ejecuta el proceso
   * \param[in] matIn Imagen de entrada
   * \param[out] matOut Imagen de salida
   */
  void run(const cv::Mat &matIn, cv::Mat &matOut) const override;

  /*!
   * \brief Establece los parámetros
   * \param[in] ddepth Profundidad deseada de la imagen de destino
   * \param[in] ksize Apertura del filtro
   * \param[in] scale Factor de escala opcional para los valores calculados por el laplaciano. Por defecto no se aplica escala.
   * \param[in] delta Valor opcional añadido a los píxeles filtrados
   * \param[in] bordertype Método de extrapolación (cv::BorderTypes)
   */
  void setParameters(int ddepth, 
                     int ksize, 
                     double scale = 1, 
                     double delta = 0, 
                     int bordertype = cv::BORDER_DEFAULT);

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

}; 

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Filtro de media
 * Suaviza una imagen con un filtro de media 
 */
class TL_EXPORT MedianBlur
  : public ImageProcess
{

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
   */
  void run(const cv::Mat &matIn, cv::Mat &matOut) const override;

  /*!
   * \brief Establece los parámetros
   * \param[in] ksize Tamaño del filtro
   */
  void setParameters(int ksize);

private:

  /*!
   * \brief Apertura del filtro
   */
  int mKernelSize;
};

/* ---------------------------------------------------------------------------------- */
/*!
 * \brief Sobel
 */
class TL_EXPORT Sobel
  : public ImageProcess
{

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
  Sobel(int dx, 
        int dy, 
        int ksize = 3, 
        double scale = 1., 
        double delta = 0.,
        int ddepth = CV_16S, 
        double thresh = 50., 
        double maxval = 200.,
        int bordertype = cv::BORDER_DEFAULT);

  /*!
   * \brief Ejecuta el proceso
   * \param[in] matIn Imagen de entrada
   * \param[out] matOut Imagen de salida
   */
  void run(const cv::Mat &matIn, cv::Mat &matOut) const override;

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
  void setParameters(int dx, 
                     int dy, 
                     int ksize = 3, 
                     double scale = 1., 
                     double delta = 0., 
                     int ddepth = CV_16S, 
                     double thresh = 50., 
                     double maxval = 200., 
                     int bordertype = cv::BORDER_DEFAULT);

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

};

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Detector de bordes canny
 */
class TL_EXPORT Canny 
  : public ImageProcess
{

public:

  /*!
   * \brief Constructora
   * \param[in] threshold1
   * \param[in] threshold2
   */
  Canny(double threshold1 = 0.0, 
        double threshold2 = 0.0);

  /*!
   * \brief Ejecuta el proceso
   * \param[in] matIn Imagen de entrada
   * \param[out] matOut Imagen de salida
   */
  void run(const cv::Mat &matIn, cv::Mat &matOut) const override;

  /*!
   * \brief Establece los parámetros para el detector de bordes canny
   * \param[in] threshold1 Primer umbral para el procedimiento de histéresis
   * \param[in] threshold2 Segundo umbral para el procedimiento de histéresis
   */
  void setParameters(double threshold1, 
                     double threshold2);

private:

  /*!
   * \brief Primer umbral para el procedimiento de histéresis
   */
  double mThreshold1;

  /*!
   * \brief Segundo umbral para el procedimiento de histéresis
   */
  double mThreshold2;

};



/*! \} */ // end of Filters

/*! \} */ // end of ImgProc

} // End namespace tl

#endif // HAVE_OPENCV

#endif // TL_IMGPROCESS_FILTERS_H
