/****************************************************************************
 *                                                                          *
 *  This file is part of TidopLib and can not be copied and/or distributed  *
 *  without the express permision of ITOS3D ENGINEERING S.L                 *
 *                                                                          *
 *  Contact: http://www.itos3d.com                                          *
 *           http://tidop.usal.es                                           *
 *                                                                          *
 *--------------------------------------------------------------------------*
 *                                                                          *
 *  Copyright (C) 2018, ITOS3D ENGINEERING S.L - All rights reserved        *
 *                                                                          *
 ****************************************************************************/


#ifndef TL_GEOM_WINDOW_H
#define TL_GEOM_WINDOW_H

#include "config_tl.h"

#include <algorithm>
#include <typeinfo>
#include <cmath>

#ifdef HAVE_OPENCV
#include "opencv2/core/core.hpp"
#endif // HAVE_OPENCV

#include "tidop/core/defs.h"
#include "tidop/geometry/entities/entity.h"
#include "tidop/geometry/entities/point.h"

namespace tl
{

/*! \addtogroup GeometricEntities
 *  \{
 */

namespace geometry
{

/* Operaciones con ventanas */

/*!
 * \brief Comprueba si dos ventanas intersectan
 * \param[in] w1 Ventana 1
 * \param[in] w2 Ventana 2
 * \return Verdadero si intersectan
 */
template<typename T1, typename T2> inline 
bool intersectWindows(const T1 &w1, const T2 &w2)
{
  return (w1.pt2.x >= w2.pt1.x &&
          w1.pt2.y >= w2.pt1.y &&
          w1.pt1.x <= w2.pt2.x &&
          w1.pt1.y <= w2.pt2.y);
}

/*!
 * \brief Ventana interseción
 * \param[in] w1 Ventana 1
 * \param[in] w2 Ventana 2
 * \return Ventana interseción
 */
template<typename T> inline 
T windowIntersection(const T &w1, const T &w2)
{
  T w;
  if (intersectWindows(w1, w2)) {
    w.pt1.x = std::max(w1.pt1.x, w2.pt1.x);
    w.pt1.y = std::max(w1.pt1.y, w2.pt1.y);
    w.pt2.x = std::min(w1.pt2.x, w2.pt2.x);
    w.pt2.y = std::min(w1.pt2.y, w2.pt2.y);
  }
  return w;
}

/*!
 * \brief Unión de dos ventanas
 * \param[in] w1 Ventana 1
 * \param[in] w2 Ventana 2
 * \return Ventana unión
 */
template<typename T> inline 
T joinWindow(const T &w1, const T &w2)
{
  T w;
  w.pt1.x = std::min(w1.pt1.x, w2.pt1.x);
  w.pt1.y = std::min(w1.pt1.y, w2.pt1.y);
  w.pt2.x = std::max(w1.pt2.x, w2.pt2.x);
  w.pt2.y = std::max(w1.pt2.y, w2.pt2.y);
  return w;
}

/*!
 * \brief Expande una ventana según la cantidad indicada para x e y
 * \param[in] w Ventana que se va a expandir
 * \param[in] szx Aumento en x
 * \param[in] szy Aumento en y
 * \return Ventana resultante
 */
template<typename T1, typename T2> inline 
T1 expandWindow(const T1 &w, T2 szx, T2 szy)
{
  T1 _w;
  _w.pt1.x = w.pt1.x - szx;
  _w.pt1.y = w.pt1.y - szy;
  _w.pt2.x = w.pt2.x + szx;
  _w.pt2.y = w.pt2.y + szx;
  return _w;
}


/*!
 * \brief Expande una ventana
 * \param[in] w Ventana que se va a expandir
 * \param[in] sz Cantidad en que se expande
 * \return Ventana resultante
 */
template<typename T1, typename T2> inline 
T1 expandWindow(const T1 &w, T2 sz)
{
  return expandWindow(w, sz, sz);
}

template<typename Point_t, typename T> inline
Window<Point_t> moveWindow(const Window<Point_t> &w, T dx, T dy)
{
  Window<Point_t> w_return = w;
  Point<T> t(dx, dy);
  w_return.pt1 += t;
  w_return.pt2 += t;
  return w_return;
}


/*!
* \brief The Window class
*/
template<typename Point_t>
class Window
  : public Entity
{
public:

  /*!
   * \brief type
   */
  typedef Point_t value_type;

  /*!
   * \brief Punto 1
   */
  Point_t pt1;

  /*!
   * \brief Punto 2
   */
  Point_t pt2;

public:

  /*!
   * \brief Constructor por defecto
   */
  Window();

  /*!
   * \brief Constructor de copia
   * \param[in] w Objeto Window que se copia
   */
  Window(const Window &w);

  /*!
   * \brief Constructor de movimiento Window
   */
  Window(Window &&w);

  /*!
   * \brief Constructor Window
   * \param[in] pt1 Primer punto
   * \param[in] pt2 Segundo punto
   */
  Window(const Point_t &pt1, const Point_t &pt2);

  /*!
   * \brief Constructor Window
   * \param[in] pt Punto central
   * \param[in] sxx Ancho de la ventana
   * \param[in] szy Alto de la ventana
   */
  template<typename T> Window(const Point_t &pt, T sxx, T szy);

  /*!
   * \brief Constructor Window
   * \param[in] pt Punto central
   * \param[in] sz Alto y ancho de la ventana
   */
  template<typename T> 
  Window(const Point_t &pt, T sz);

  /*!
   * \brief Constructor Window
   * Crea una ventana que envuelve a un conjunto de puntos
   *
   * \param[in] v vector de puntos
   */
  Window(const std::vector<Point_t> &v);

  /*!
   * \brief 
   * \param[in] v Ventana
   */
  template<typename Point_t2> Window(const std::vector<Point_t2> &v);

  //~Window();

  /*!
   * \brief Sobrecarga del operador  de asignación
   * \param[in] w Ventana que se asigna
   * \return Referencia a la ventana
   */
  Window &operator = (const Window &w);

  /*!
   * \brief Sobrecarga del operador de asignación de movimiento
   * \param[in] w Ventana que se mueve
   * \return Referencia a la ventana
   */
  Window &operator = (Window &&w);

  /*!
   * \brief Sobrecarga del operador 'igual que'
   * \param[in] w Ventana con la que se compara
   * \return true si ambas ventanas son iguales
   */
  bool operator == (const Window &w) const;

  /*!
   * \brief Conversión a una ventana de un tipo diferente
   */
  template<typename Point_t2> operator Window<Point_t2>() const;

  /*!
   * \brief Devuelve el ancho de la ventana
   * \return Ancho
   */
  typename Point_t::value_type getWidth() const { return pt2.x - pt1.x; }

  /*!
   * \brief Devuelve el alto de la ventana
   * \return Alto
   */
  typename Point_t::value_type getHeight() const { return pt2.y - pt1.y; }

  /*!
   * \brief Devuelve centro de la ventana
   * \return Centro de la ventana
   */
  Point_t getCenter() const;

  /*!
   * \brief Comprueba si la ventana esta vacia
   * \return True si la ventana es nula
   */
  bool isEmpty() const;

  /*!
   * \brief Comprueba si un punto está contenido dentro de la ventana
   * \param[in] pt Punto
   * \return True si el punto esta dentro de la ventana
   */
  template<typename Point_t2> bool containsPoint(const Point_t2 &pt) const;

  /*!
   * \brief La ventana contiene la ventana
   */
  template<typename Point_t2> bool containsWindow(const Window<Point_t2> &w) const;

};

// Definición de métodos

template<typename Point_t> inline
Window<Point_t>::Window() 
  : Entity(Entity::Type::window), 
    pt1(std::numeric_limits<typename Point_t::value_type>().max(), std::numeric_limits<typename Point_t::value_type>().max()), 
    pt2(-std::numeric_limits<typename Point_t::value_type>().max(), -std::numeric_limits<typename Point_t::value_type>().max()) 
{
}

template<typename Point_t> inline
Window<Point_t>::Window(const Window &w) 
  : Entity(Entity::Type::window), 
    pt1(w.pt1), 
    pt2(w.pt2) 
{
}

template<typename Point_t> inline
Window<Point_t>::Window(Window &&w) 
  : Entity(std::forward<Entity>(w)), 
    pt1(w.pt1), 
    pt2(w.pt2) 
{
}

template<typename Point_t> inline
Window<Point_t>::Window(const Point_t &_pt1, const Point_t &_pt2) 
  : Entity(Entity::Type::window) 
{
  pt1.x = std::min(_pt1.x, _pt2.x);
  pt1.y = std::min(_pt1.y, _pt2.y);
  pt2.x = std::max(_pt1.x, _pt2.x);
  pt2.y = std::max(_pt1.y, _pt2.y);
}

template<typename Point_t> template<typename T> inline
Window<Point_t>::Window(const Point_t &_pt, T sxx, T szy) 
  : Entity(Entity::Type::window)
{ 
  if (typeid(typename Point_t::value_type) == typeid(int)) {
    // Prefiero hacer la conversión a entero para evitar que OpenCV 
    // lo haga mediante cvRound 
    int sxx_2 = TL_ROUND_TO_INT(sxx / 2);
    int szy_2 = TL_ROUND_TO_INT(szy / 2);
    int dx = static_cast<int>(sxx) % 2;
    int dy = static_cast<int>(szy) % 2;
    pt1 = Point_t(_pt.x - sxx_2, _pt.y - szy_2);
    pt2 = Point_t(_pt.x + sxx_2 + dx, _pt.y + szy_2 + dy);
  } else {
    // Quito el warning que da cuando es una ventana de enteros. En ese caso nunca pasara por aqui.
    TL_DISABLE_WARNING(TL_WARNING_C4244)
    pt1 = Point_t(_pt.x - sxx / 2., _pt.y - szy / 2.);
    pt2 = Point_t(_pt.x + sxx / 2., _pt.y + szy / 2.);
    TL_ENABLE_WARNING(TL_WARNING_C4244)
  }
}

template<typename Point_t> template<typename T> inline
Window<Point_t>::Window(const Point_t &_pt, T sz) 
  : Entity(Entity::Type::window)
{ 
  if (std::is_integral<typename Point_t::value_type>::value) {
  //if (typeid(typename Point_t::value_type) == typeid(int)) {
    int sz_2 = TL_ROUND_TO_INT(sz / 2);
    int dxy = static_cast<int>(sz) % 2;
    pt1 = Point_t(_pt.x - sz_2, _pt.y - sz_2);
    pt2 = Point_t(_pt.x + sz_2 + dxy, _pt.y + sz_2 + dxy);
  } else {
    // Quito el warning que da cuando es una ventana de enteros. En ese caso nunca pasara por aqui.
    TL_DISABLE_WARNING(TL_WARNING_C4244)
    T sz_2 = sz / 2;
    pt1 = Point<T>(_pt.x - sz_2, _pt.y - sz_2);
    pt2 = Point<T>(_pt.x + sz_2, _pt.y + sz_2);
    TL_ENABLE_WARNING(TL_WARNING_C4244)
  }
}

template<typename Point_t> inline
Window<Point_t>::Window(const std::vector<Point_t> &v)
  : Entity(Entity::Type::window),
    pt1(std::numeric_limits<typename Point_t::value_type>().max(), 
    std::numeric_limits<typename Point_t::value_type>().max()), 
    pt2(-std::numeric_limits<typename Point_t::value_type>().max(), 
    -std::numeric_limits<typename Point_t::value_type>().max())
{
  if (v.size() >= 2) {
    for (size_t i = 0; i < v.size(); i++) {
      if (pt1.x > v[i].x) pt1.x = v[i].x;
      if (pt1.y > v[i].y) pt1.y = v[i].y;
      if (pt2.x < v[i].x) pt2.x = v[i].x;
      if (pt2.y < v[i].y) pt2.y = v[i].y;
    }
  }
}

template<typename Point_t> template<typename Point_t2> inline
Window<Point_t>::Window(const std::vector<Point_t2> &v)
  : Entity(Entity::Type::window),
    pt1(std::numeric_limits<typename Point_t::value_type>().max(), 
    std::numeric_limits<typename Point_t::value_type>().max()), 
    pt2(-std::numeric_limits<typename Point_t::value_type>().max(), 
    -std::numeric_limits<typename Point_t::value_type>().max())
{
  if (v.size() >= 2) {
    for (size_t i = 0; i < v.size(); i++) {
      if (typeid(typename Point_t::value_type) == typeid(int)) {
        if (pt1.x > v[i].x) pt1.x = TL_ROUND_TO_INT(v[i].x);
        if (pt1.y > v[i].y) pt1.y = TL_ROUND_TO_INT(v[i].y);
        if (pt2.x < v[i].x) pt2.x = TL_ROUND_TO_INT(v[i].x);
        if (pt2.y < v[i].y) pt2.y = TL_ROUND_TO_INT(v[i].y);
      } else {
        if (pt1.x > v[i].x) pt1.x = static_cast<typename Point_t::value_type>(v[i].x);
        if (pt1.y > v[i].y) pt1.y = static_cast<typename Point_t::value_type>(v[i].y);
        if (pt2.x < v[i].x) pt2.x = static_cast<typename Point_t::value_type>(v[i].x);
        if (pt2.y < v[i].y) pt2.y = static_cast<typename Point_t::value_type>(v[i].y);
      }
    }
  }
}

template<typename Point_t> inline
Window<Point_t> &Window<Point_t>::operator = (const Window &w)
{
  if (this != &w) {
    this->mEntityType = w.mEntityType;
    this->pt1 = w.pt1;
    this->pt2 = w.pt2;
  }
  return *this;
}

template<typename Point_t> inline
Window<Point_t> &Window<Point_t>::operator = (Window &&w)
{
  if (this != &w) {
    this->mEntityType = std::move(w.mEntityType);
    this->pt1 = std::move(w.pt1);
    this->pt2 = std::move(w.pt2);
  }
  return *this;
}

template<typename Point_t> inline
bool Window<Point_t>::operator == (const Window &w) const
{
  return (pt1 == w.pt1 && pt2 == w.pt2);
}

template<typename Point_t> template<typename Point_t2> inline
Window<Point_t>::operator Window<Point_t2>() const
{
  Window<Point_t2> w;
  if (std::is_integral<typename Point_t2::value_type>::value) {
    w.pt1.x = static_cast<typename Point_t2::value_type>(std::round(pt1.x));
    w.pt1.y = static_cast<typename Point_t2::value_type>(std::round(pt1.y));
    w.pt2.x = static_cast<typename Point_t2::value_type>(std::round(pt2.x));
    w.pt2.y = static_cast<typename Point_t2::value_type>(std::round(pt2.y));
  } else {
    w.pt1.x = static_cast<typename Point_t2::value_type>(pt1.x);
    w.pt1.y = static_cast<typename Point_t2::value_type>(pt1.y);
    w.pt2.x = static_cast<typename Point_t2::value_type>(pt2.x);
    w.pt2.y = static_cast<typename Point_t2::value_type>(pt2.y);
  }
  return w;
}

template<typename Point_t> inline
Point_t Window<Point_t>::getCenter() const
{
  if (std::is_integral<typename Point_t::value_type>::value) {
    return Point_t(static_cast<typename Point_t::value_type>(std::round((pt1.x + pt2.x) / 2)), 
                   static_cast<typename Point_t::value_type>(std::round((pt1.y + pt2.y) / 2)));
  } else {
    TL_DISABLE_WARNING(TL_WARNING_C4244)
    return Point_t((pt1.x + pt2.x) / 2., (pt1.y + pt2.y) / 2.);
    TL_ENABLE_WARNING(TL_WARNING_C4244)
  }
}

template<typename Point_t> inline
bool Window<Point_t>::isEmpty() const 
{ 
  return (   pt1.x == std::numeric_limits<typename Point_t::value_type>().max() 
          && pt1.y == std::numeric_limits<typename Point_t::value_type>().max() 
          && pt2.x == -std::numeric_limits<typename Point_t::value_type>().max() 
          && pt2.y == -std::numeric_limits<typename Point_t::value_type>().max()); 
}

template<typename Point_t> template<typename Point_t2> inline
bool Window<Point_t>::containsPoint(const Point_t2 &pt) const
{
  //Point<typename Point_t::value_type> _pt = pt;
  Point_t _pt(pt.x, pt.y);
  return ((pt2.x >= _pt.x) && (pt2.y >= _pt.y) &&
    (pt1.x <= _pt.x) && (pt1.y <= _pt.y));
}

template<typename Point_t> template<typename Point_t2> inline
bool Window<Point_t>::containsWindow(const Window<Point_t2> &w) const
{
  Window<Point_t> w2 = w;
  return pt1.x <= w2.pt1.x && pt1.y <= w2.pt1.y && pt2.x >= w2.pt2.x && pt2.y >= w2.pt2.y;
}

typedef Window<Point<int>> WindowI;
typedef Window<Point<double>> WindowD;
typedef Window<Point<float>> WindowF;


#ifdef HAVE_OPENCV

/*!
 * \brief Convierte una ventana a un Rect de OpenCV
 * \param[in] w Ventana
 * \return cv::Rect
 */
//TL_EXPORT cv::Rect windowToCvRect(WindowI w);
template<typename T> inline
cv::Rect_<T> windowToCvRect(const Window<Point<T>> &w)
{
  return cv::Rect_<T>(point_cast<cv::Point_<T>>(w.pt1), point_cast<cv::Point_<T>>(w.pt2));
}

/*!
 * \brief Convierte una Rect de OpenCV a un objeto WindowI
 * \param[in] rect cv::Rect
 * \return Ventana
 */
//TL_EXPORT WindowI cvRectToWindow(cv::Rect rect);
template<typename T> inline
Window<T> cvRectToWindow(const cv::Rect_<T> &rect)
{
  Point<T> pt1(rect.tl().x,rect.tl().y);
  Point<T> pt2(rect.br().x,rect.br().y);
  return Window<T>(pt1, pt2);
}
#endif // HAVE_OPENCV



} // End namespace geometry


/*! \} */ // end of GeometricEntities

} // End namespace TL

#endif // TL_GEOM_WINDOW_H
