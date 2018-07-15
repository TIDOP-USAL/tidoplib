#ifndef TL_GRAPHIC_CANVAS_H
#define TL_GRAPHIC_CANVAS_H

#include "config_tl.h"

#include "core/defs.h"
#include "graphic/color.h"
#include "graphic/styles.h"
#include "graphic/entities/point.h"
#include "graphic/entities/linestring.h"
#include "graphic/entities/polygon.h"

namespace TL
{

template<typename T> class Transform;


/*! \defgroup GraphicEntities Entidades gráficas
 *  Puntos, lineas, ...
 *  \{
 */

namespace graph
{

/*!
 * \brief Clase Canvas
 */
class TL_EXPORT Canvas
{

public:

  /*!
   * \brief Constructora canvas
   */
  Canvas();

  virtual ~Canvas();

  /*!
   * \brief Ancho del canvas
   * \return Ancho en pixeles
   */
  virtual int getWidth() const = 0;

  /*!
   * \brief Alto del canvas
   * \return Alto del canvas en pixeles
   */
  virtual int getHeight() const = 0;
   
  /*!
   * \brief Establece el ancho del canvas
   * \param width Ancho del canvas
   */
  virtual void setWidth(int width) = 0;

  /*!
   * \brief Establece el alto del canvas
   * \param height Alto
   */
  virtual void setHeight(int height) = 0;

  /*!
   * \brief Establece el tamaño del canvas
   * \param width Ancho
   * \param height Alto
   */
  virtual void setSize(int width, int height) = 0;

  /*!
   * \brief Establece el color de fondo del canvas
   * \param color Color
   * \see Color
   */
  virtual void setBackgroundColor(const TL::Color &color) = 0;

  /*!
   * \brief Dibuja un punto en el canvas
   * \param point Punto
   */
  virtual void drawPoint(const GPoint &point) = 0;

  /*!
   * \brief Dibuja una polilinea en el canvas
   * \param lineString Polilinea
   */
  virtual void drawLineString(const GLineString &lineString) = 0;

  /*!
   * \brief Dibuja un poligono en el canvas
   * \param polygon Poligono
   */
  virtual void drawPolygon(const GPolygon &polygon) = 0;

};

#ifdef HAVE_OPENCV

class TL_EXPORT CanvasCV : public Canvas
{

private:

  int mWidth;
  int mHeight;
  TL::Color mBgColor;
  cv::Mat mCanvas;

public:

  /*!
   * \brief CanvasCV
   */
  CanvasCV();

  /*!
   * \brief Constructor de copia
   * \param[in] canvas Objeto canvas que se copia
   */
  CanvasCV(const CanvasCV &canvas);

  ~CanvasCV();

  int getWidth() const override;
  int getHeight() const override;
  void setWidth(int width) override;
  void setHeight(int height) override;
  void setSize(int width, int height) override;
  void setBackgroundColor(const TL::Color &color) override;
  void drawPoint(const GPoint &point) override;
  void drawLineString(const GLineString &lineString) override;
  void drawPolygon(const GPolygon &polygon) override;

  /*!
   * \brief operador asignación
   * \param[in] canvas Objeto canvas que se copia por asignación
   * \return
   */
  CanvasCV &operator =(const CanvasCV &canvas);

private:

  void update();
};

#endif // HAVE_OPENCV

} // Fin namespace graph

/*! \} */ // Fin GraphicEntities

} // Fin namespace TL

#endif // TL_GRAPHIC_CANVAS_H
