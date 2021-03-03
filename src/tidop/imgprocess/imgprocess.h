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

#ifndef TL_IMGPROCESS_PROCESSING_H
#define TL_IMGPROCESS_PROCESSING_H

#include "config_tl.h"

#include <list>
#include <memory>
#include <functional>

#ifdef HAVE_OPENCV
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include "tidop/core/defs.h"
#include "tidop/graphic/color.h"

namespace tl
{


/*! \defgroup ImgProc Procesado de imagen
 *  Filtrado y preprocesado de imagenes
 *  \{
 */

//  Dialnet - TecnicasYAlgoritmosBasicosDeVisionArtificial - 338314.pdf
//- Preprocesado:
//  - Operaciones básicas:
//    - Inversion
//    - Operaciones aritmeticas
//    - Operaciones lógicas
//    - Transformaciones no lineales
//    - Slicing
//    - Clipping
//    - Umbralización
//    - Binarización
//  - Transformaciones geométricas de imágenes
//    - Traslaccion
//    - giro
//    - escalado
//    - espejado
//    - ....
//  - Generadores de ruido
//    - Geneardores de ruido dependientes de la señal 
//    - Generadores de ruido aleatorio
//  - Filtrado
//    - Frecuencia
//    - Espacio
//  - Operaciones basadas en histograma
//    - Transformaciones del histograma
//    - Ecualización del histograma
//- Segmentación
//  - Transformaciones morfologicas
//    - Dilatación
//    - Erosión
//    - Apertura
//    - cierre
//    - Esqueletos
//  - 
 



/*!
 * \brief Clase para gestionar los procesos de imagen
 * Clase que permite tener una interfaz común para aplicar filtros
 * u otros procesos a una imagen.
 */
class TL_EXPORT ImageProcess
{

public:

  /*!
   * \brief Tipos de procesado de imagen
   */
  enum class ProcessType 
  {

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

    thinning,           /*!< Operacion morfologica de adelgazamiento  */

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
  };

public:

  ImageProcess(ProcessType type) : mType(type) {}
  virtual ~ImageProcess() = default;

  /*!
   * \brief Ejecuta el proceso
   * \param[in] matIn Imagen de entrada
   * \param[out] matOut Imagen de salida
   */
  virtual void run(const cv::Mat &matIn, cv::Mat &matOut) const = 0;

  ProcessType type() const { return mType; }

private:

  ProcessType mType;
};


/*!
 * \brief Contenedor de procesos de imagen que permite su ejecución secuencial.
 * \code
 * tl::ImagingProcesses imagingProcesses;
 * imagingProcesses.push_back(std::make_shared<tl::Normalize>(0., 255.));
 * imagingProcesses.push_back(std::make_shared<tl::BilateralFilter>(5, 50., 50.));
 * imagingProcesses.push_back(std::make_shared<tl::Erotion>(1));
 * imagingProcesses.push_back(std::make_shared<tl::Dilate>(1));
 * cv::Mat out;
 * try {
 *   imagingProcesses.execute(in, out);
 * } catch (std::exception &e){
 *  
 * }
 * \endcode
 */
class ImagingProcesses
{

public:

  typedef std::list<std::shared_ptr<ImageProcess>>::iterator iterator;
  typedef std::list<std::shared_ptr<ImageProcess>>::const_iterator const_iterator;

public:

  ImagingProcesses();
  ImagingProcesses(const ImagingProcesses &imagingProcesses);
  ImagingProcesses(std::initializer_list<std::shared_ptr<ImageProcess>> imgProcList);
  ~ImagingProcesses();

  /*!
   * \brief Ejecuta la lista de procesos
   * \param[in] matIn Imagen de entrada
   * \param[out] matOut Imagen de salida
   */
  void run(const cv::Mat &matIn, cv::Mat &matOut) const;

  void push_back(const std::shared_ptr<ImageProcess> &process);
  void pop_back();
  void clear() TL_NOEXCEPT;
  bool empty() const TL_NOEXCEPT;
  iterator begin() TL_NOEXCEPT;
  const_iterator begin() const TL_NOEXCEPT;
  iterator end() TL_NOEXCEPT;
  const_iterator end() const TL_NOEXCEPT;


private:

  /*!
   * \brief Lista de procesos
   */
  std::list<std::shared_ptr<ImageProcess>> mProcessList;

};


/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Clase Normalize
 */
class TL_EXPORT Normalize 
  : public ImageProcess
{

public:

  /*!
   * \brief Constructora de la clase Normalize
   * \param lowRange
   * \param upRange
   */
  Normalize(double lowRange, double upRange);
  ~Normalize() override = default;

  /*!
   * \brief Ejecuta el proceso
   * \param[in] matIn Imagen de entrada
   * \param[out] matOut Imagen de salida
   */
  void run(const cv::Mat &matIn, cv::Mat &matOut) const override;

  /*!
   * \brief Establece el rango inferior
   * \param[in] lowRange Rango inferior
   */
  void setLowRange(double lowRange);

  /*!
   * \brief Establece el rango superior
   * \param[in] upRange Rango superior
   */
  void setUpRange(double upRange);

private:

  /*!
   * \brief Rango inferior
   */
  double mLowRange;

  /*!
   * \brief Rango superior
   */
  double mUpRange;
};

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Clase Binarize
 * Convierte una imagen a binaria
 */
class TL_EXPORT Binarize 
  : public ImageProcess
{

public:

  /*!
   * \brief Constructora de la clase Binarize
   * Si thresh y maxVal son 0 se calculan internamente a partir de la media y desviación típica.
   * \param thresh Umbral
   * \param maxVal Valor máximo
   * \param bInverse Binarización inversa
   */
  Binarize(double thresh = 0., 
           double maxVal = 0., 
           bool bInverse = false);
  ~Binarize() override = default;

  /*!
   * \brief Ejecuta el proceso
   * \param[in] matIn Imagen de entrada
   * \param[out] matOut Imagen de salida
   */
  void run(const cv::Mat &matIn, cv::Mat &matOut) const override;

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
  bool inverse() const;

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
};

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Ecualización del histograma.
 * Mejora del contraste de la imagen mediante la ecualización del histograma
 */
class TL_EXPORT EqualizeHistogram 
  : public ImageProcess
{

public:

  EqualizeHistogram();
  ~EqualizeHistogram() override = default;

  /*!
   * \brief Ejecuta el proceso.
   * \param[in] matIn Imagen de entrada.
   * \param[out] matOut Imagen de salida.
   */
  void run(const cv::Mat &matIn, cv::Mat &matOut) const override;

};

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Wrapper de una función para ejecutarla como un proceso.
 * 
 * Para permitir una mayor libertad en el procesado de las imagenes mediante
 * ImgProcessingList se permite asociar una función o lambda a un proceso. La
 * función tiene que ser de la forma:
 * <BLOCKQUOTE>std::function<void(const cv::Mat &, cv::Mat &)> </BLOCKQUOTE>
 * o si es una lambda:
 * <BLOCKQUOTE>[](const cv::Mat &in, cv::Mat &out) { ... }</BLOCKQUOTE>
 * <H3>Ejemplo:</H3>
 * \code
 * std::shared_ptr<FunctionProcess> fProcess1 = std::make_shared<FunctionProcess>(
 *   [](const cv::Mat &in, cv::Mat &out) {
 *     in.convertTo(out, CV_32F);
 * });
 * std::shared_ptr<FunctionProcess> fProcess2 = std::make_shared<FunctionProcess>(
 * [&](const cv::Mat &in, cv::Mat &out) {
 * cv::normalize(in, out, 0, 255, CV_MINMAX);
 * out->convertTo(out, CV_8U);
 * });
 *
 * tl::ImagingProcesses imgprolist{ fProcess1, fProcess2 };
 * \endcode
 */
class TL_EXPORT FunctionProcess 
  : public ImageProcess
{

public:

  /*!
   * \brief Constructora
   * \param[in] f Función de la forma std::function<void(const cv::Mat &,cv::Mat &)>
   */
  FunctionProcess(std::function<void(const cv::Mat &, cv::Mat &)> f);
  ~FunctionProcess() override = default;

  /*!
   * \brief Ejecuta el proceso.
   * \param[in] matIn Imagen de entrada.
   * \param[out] matOut Imagen de salida.
   */
  void run(const cv::Mat &matIn, cv::Mat &matOut) const override;

private:

  std::function<void(const cv::Mat &, cv::Mat &)> mFunction;
};



/*! \} */ // end of ImgProc

} // End namespace tl

#endif // HAVE_OPENCV

#endif // TL_IMGPROCESS_PROCESSING_H
