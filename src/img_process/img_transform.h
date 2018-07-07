#ifndef TL_IMGPROCESS_TRANSFORM_H
#define TL_IMGPROCESS_TRANSFORM_H

#include "config_tl.h"

#ifdef HAVE_OPENCV

#include "opencv2/core/core.hpp"

#include "core/defs.h"
#include "img_process/img_processing.h"
#include "geometry/transform.h"

namespace TL
{


/*! \addtogroup ImgProc
 *  \{
 */

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
class TL_EXPORT Resize : public ImgProcessing
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
   * \param scaleX Escala en el eje X
   * \param scaleY Escala en el eje Y. Si se omite se toma la mista escala del eje X
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
class TL_EXPORT ResizeCanvas : public ImgProcessing
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


/*!
 * \brief Aplica una transformación a una imagen
 * \param[in] in Imagen de entrada
 * \param[out] out Imagen de salida
 * \param[in] trf Transformación que se aplica a la imagen
 * \param[in] trfOrder Orden de la transformación. Por defecto transform_order::DIRECT
 */
template<typename Point_t> inline
void transform(cv::Mat in, cv::Mat out, Transform<Point_t> *trf, transform_order trfOrder)
{
  transform_type type = trf->getTransformType();
  switch (type) {
  case TL::transform_type::TRANSLATE:
    Translate<Point_t> transTrf = dynamic_cast<Translate<Point_t>>(trf);
    cv::Mat translateMat( 2, 3, CV_32FC1 );
    translateMat.at<float>(0, 0) = 1.f;
    translateMat.at<float>(0, 1) = 0.f;
    translateMat.at<float>(0, 2) = transTrf.getTranslationX();
    translateMat.at<float>(1, 0) = 0.f;
    translateMat.at<float>(1, 1) = 1.f;
    translateMat.at<float>(1, 2) = transTrf.getTranslationY();
    if (trfOrder == transform_order::DIRECT)
      cv::warpAffine(in, out, translateMat, in.size(), cv::INTER_LINEAR);
    else
      cv::warpAffine(in, out, translateMat.inv(), in.size(), cv::INTER_LINEAR);
    break;
  case TL::transform_type::ROTATION:
    Rotation<Point_t> rotTrf = dynamic_cast<Rotation<Point_t>>(trf);
    cv::Mat rotMat( 2, 3, CV_32FC1 );
    double r1 = cos(rotTrf.getAngle());
    double r2 = sin(rotTrf.getAngle());
    rotMat.at<float>(0, 0) = r1;
    rotMat.at<float>(0, 1) = r2;
    rotMat.at<float>(0, 2) = 0.f;
    rotMat.at<float>(1, 0) = r2;
    rotMat.at<float>(1, 1) = r1;
    rotMat.at<float>(1, 2) = 0.f;
    if (trfOrder == transform_order::DIRECT)
      cv::warpAffine(in, out, rotMat, in.size(), cv::INTER_LINEAR);
    else
      cv::warpAffine(in, out, rotMat.inv(), in.size(), cv::INTER_LINEAR);
    break;
  case TL::transform_type::HELMERT_2D:
    Helmert2D<Point_t> h2dTrf = dynamic_cast<Helmert2D<Point_t>>(trf);
    cv::Mat h2DMat( 2, 3, CV_32FC1 );
    double rotation = h2dTrf.getRotation();
    double scale = h2dTrf.getScale();
    double a = scale * cos(rotation);
    double b = scale * sin(rotation);
    h2DMat.at<float>(0, 0) = a;
    h2DMat.at<float>(0, 1) = b;
    h2DMat.at<float>(0, 2) = h2dTrf.x0;
    h2DMat.at<float>(1, 0) = b;
    h2DMat.at<float>(1, 1) = a;
    h2DMat.at<float>(1, 2) = h2dTrf.y0;
    if (trfOrder == transform_order::DIRECT)
      cv::warpAffine(in, out, h2DMat, in.size(), cv::INTER_LINEAR);
    else
      cv::warpAffine(in, out, h2DMat.inv(), in.size(), cv::INTER_LINEAR);
    break;
  case TL::transform_type::AFFINE:
    Affine<Point_t> affineTrf = dynamic_cast<Affine<Point_t>>(trf);
    double r00, r10, r01, r11;
    affineTrf.getParameters(&r00, &r10, &r01, &r11);
    cv::Mat affMat( 2, 3, CV_32FC1 );
    affMat.at<float>(0, 0) = r00;
    affMat.at<float>(0, 1) = r10;
    affMat.at<float>(0, 2) = affineTrf.x0;
    affMat.at<float>(1, 0) = r01;
    affMat.at<float>(1, 1) = r11;
    affMat.at<float>(1, 2) = affineTrf.y0;
    if (trfOrder == transform_order::DIRECT)
      cv::warpAffine(in, out, affMat, in.size(), cv::INTER_LINEAR);
    else
      cv::warpAffine(in, out, affMat.inv(), in.size(), cv::INTER_LINEAR);
    break;
  case TL::transform_type::PERSPECTIVE:
    TrfPerspective<Point_t> perspTrf = dynamic_cast<TrfPerspective<Point_t>>(trf);
    if (trfOrder == transform_order::DIRECT)
      cv::warpPerspective(in, out, perspTrf.H, in.size(), cv::INTER_LINEAR);
    else
      cv::warpPerspective(in, out, perspTrf.H.inv(), in.size(), cv::INTER_LINEAR);
    break;
  case TL::transform_type::PROJECTIVE:
    break;
  case TL::transform_type::POLYNOMIAL:
    break;
  default:
    break;
  }
}

/*! \} */ // end of imgTransf

/*! \} */ // end of ImgProc

} // End namespace TL

#endif // HAVE_OPENCV

#endif // TL_IMGPROCESS_TRANSFORM_H
