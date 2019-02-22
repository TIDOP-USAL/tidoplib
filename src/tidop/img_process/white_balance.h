#ifndef TL_IMGPROCESS_WHITE_BALANCE_H
#define TL_IMGPROCESS_WHITE_BALANCE_H

#include "config_tl.h"

#ifdef HAVE_OPENCV
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/xphoto/white_balance.hpp"

#include "tidop/core/defs.h"
#include "tidop/graphic/color.h"
#include "tidop/img_process/img_processing.h"

namespace tl
{

// Balance de blancos
// https://courses.cs.washington.edu/courses/cse467/08au/labs/l5/whiteBalance.pdf


/*! \addtogroup ImgProc
 *  \{
 */


/*! \defgroup WhiteBalance Balance de blancos
 *
 *  \{
 */

/* ---------------------------------------------------------------------------------- */

#if defined CV_VERSION_MAJOR && CV_VERSION_MAJOR >= 3
#  if defined CV_VERSION_MINOR && CV_VERSION_MINOR >= 2

/*!
 * \brief Balance de blancos  Gray World
 */
class TL_EXPORT Grayworld : public ImgProcessing
{
private:

  /*!
   * \brief Tamaño del kernel
   */
  cv::Ptr<cv::xphoto::GrayworldWB> wb;

public:

  /*!
   * \brief Constructora Gray World
   */
  Grayworld()
    : ImgProcessing(process_type::grayworld)
  {
    wb = cv::xphoto::createGrayworldWB();
  }

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
  void setParameters();

};

#  endif // CV_VERSION_MINOR
#endif // CV_VERSION_MAJOR

/* ---------------------------------------------------------------------------------- */

/*!
 * \brief Balance de blancos White Patch
 *
 * Asume que los valores máximos de color en los tres canales de la imagen es el
 * color del blanco bajo la luz de la escena, entonces se elimina el efecto de
 * esa luz y se impone una luz blanca
 * Estimación del color de la luz de la escena de la imagen:
 * Rmax(I): Valor máximo del canal rojo de la imagen I
 * Gmax(I): Valor máximo del canal verde de la imagen I
 * Bmax(I): Valor máximo del canal azul de la imagen I
 * Para una luz blanca de (255,255,255) se elimina la luz de la escena y se
 * introduce la luz blanca:
 * (R, G, B) -> ((255/Rmax(I))*R, (255/Gmax(I))*G, (255/Gmax(I))*G)
 */
class TL_EXPORT WhitePatch : public ImgProcessing
{
private:

  graph::Color mWhite;

public:

  /*!
   * \brief Constructora WhitePatch
   * \param[in] white Luz blanca. Por defecto (255, 255, 255)
   */
  WhitePatch(const graph::Color &white = graph::Color(graph::Color::Name::white))
    : ImgProcessing(process_type::whitepatch), 
      mWhite(white) { }

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
   * \param[in] white Luz blanca. Por defecto (255, 255, 255)
   */
  void setParameters(const graph::Color &white = graph::Color(graph::Color::Name::white));

};

/*! \} */ // end of WhiteBalance

/*! \} */ // end of ImgProc

} // End namespace TL

#endif // HAVE_OPENCV

#endif // TL_IMGPROCESS_WHITE_BALANCE_H
