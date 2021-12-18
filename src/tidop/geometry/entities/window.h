/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by Esteban Ruiz de Oña Crespo                       *
 *                                                                        *
 * This file is part of TidopLib                                          *
 *                                                                        *
 * TidopLib is free software: you can redistribute it and/or modify       *
 * it under the terms of the GNU Lesser General Public License as         *
 * published by the Free Software Foundation, either version 3 of the     *
 * License, or (at your option) any later version.                        *
 *                                                                        *
 * TidopLib is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU Lesser General Public License for more details.                    *
 *                                                                        *
 * You should have received a copy of the GNU Lesser General Public       *
 * License along with Foobar. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#ifndef TL_GEOMETRY_WINDOW_H
#define TL_GEOMETRY_WINDOW_H

#include "config_tl.h"

#include <algorithm>
#include <typeinfo>
#include <cmath>
#include <utility>

//#ifdef HAVE_OPENCV
//#include "opencv2/core/core.hpp"
//#endif // HAVE_OPENCV

#include "tidop/core/defs.h"
#include "tidop/geometry/entities/entity.h"
#include "tidop/geometry/entities/point.h"
#include "tidop/geometry/size.h"

namespace tl
{

/*! \addtogroup GeometricEntities
 *  \{
 */


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
  using value_type = Point_t;

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
   * \param[in] window Objeto Window que se copia
   */
  Window(const Window &window);

  /*!
   * \brief Constructor de movimiento Window
   */
  Window(Window &&window) TL_NOEXCEPT;

  /*!
   * \brief Constructor Window
   * \param[in] pt1 Primer punto
   * \param[in] pt2 Segundo punto
   */
  Window(Point_t pt1, 
         Point_t pt2);

  /*!
   * \brief Constructor Window
   * \param[in] pt Punto central
   * \param[in] width Ancho de la ventana
   * \param[in] szy Alto de la ventana
   */
  template<typename T> Window(const Point_t &pt, 
                              T width, T height);

  /*!
   * \brief Constructor Window
   * \param[in] pt Punto central
   * \param[in] side Tamaño del lado de una ventana cuadrada
   */
  template<typename T> 
  Window(const Point_t &pt, T side);

  /*!
   * \brief Constructor Window
   * Crea una ventana que envuelve a un conjunto de puntos
   *
   * \param[in] vector vector de puntos
   */
  Window(const std::vector<Point_t> &window);

  /*!
   * \brief 
   * \param[in] vector Ventana
   */
  template<typename Point_t2> Window(const std::vector<Point_t2> &window);

  ~Window() override = default;

  /*!
   * \brief Sobrecarga del operador  de asignación
   * \param[in] window Ventana que se asigna
   * \return Referencia a la ventana
   */
  Window &operator = (const Window &window);

  /*!
   * \brief Sobrecarga del operador de asignación de movimiento
   * \param[in] window Ventana que se mueve
   * \return Referencia a la ventana
   */
  Window &operator = (Window &&window) TL_NOEXCEPT;

  /*!
   * \brief Sobrecarga del operador 'igual que'
   * \param[in] window Ventana con la que se compara
   * \return true si ambas ventanas son iguales
   */
  bool operator == (const Window &window) const;

  /*!
   * \brief Conversión a una ventana de un tipo diferente
   */
  template<typename Point_t2> operator Window<Point_t2>() const;

  /*!
   * \brief Devuelve el ancho de la ventana
   * \return Ancho
   */
  typename Point_t::value_type width() const;

  /*!
   * \brief Devuelve el alto de la ventana
   * \return Alto
   */
  typename Point_t::value_type height() const;

  /*!
   * \brief Devuelve centro de la ventana
   * \return Centro de la ventana
   */
  Point_t center() const;

  /*!
   * \brief Comprueba si la ventana esta vacia
   * \return True si la ventana es nula
   */
  bool isEmpty() const;
  bool isValid() const;
  void normalized();

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
    pt1(std::numeric_limits<typename Point_t::value_type>().max(), 
        std::numeric_limits<typename Point_t::value_type>().max()), 
    pt2(-std::numeric_limits<typename Point_t::value_type>().max(), 
        -std::numeric_limits<typename Point_t::value_type>().max()) 
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
Window<Point_t>::Window(Window &&window) TL_NOEXCEPT
  : Entity(std::forward<Entity>(window)), 
    pt1(std::move(window.pt1)), 
    pt2(std::move(window.pt2)) 
{
}

template<typename Point_t> inline
Window<Point_t>::Window(Point_t pt1,
                        Point_t pt2)
  : Entity(Entity::Type::window), 
    pt1(std::move(pt1)), 
    pt2(std::move(pt2)) 
{
}
    
template<typename Point_t> template<typename T> inline
Window<Point_t>::Window(const Point_t &pt,
                        T width, T height) 
  : Entity(Entity::Type::window)
{ 
  typename Point_t::value_type two{2};
  auto half_width = width / two;
  auto half_height = height / two;

  if (std::is_integral<typename Point_t::value_type>::value) {

    int dx = static_cast<int>(width) % 2;
    int dy = static_cast<int>(height) % 2;

    this->pt1 = Point_t(pt.x - half_width, 
                        pt.y - half_height);
    this->pt2 = Point_t(pt.x + half_width + dx, 
                        pt.y + half_height + dy);
  } else {
    this->pt1 = Point_t(pt.x - half_width, 
                        pt.y - half_height);
    this->pt2 = Point_t(pt.x + half_width, 
                        pt.y + half_height);
  }
}

template<typename Point_t> template<typename T> inline
Window<Point_t>::Window(const Point_t &pt, 
                        T side) 
  : Entity(Entity::Type::window)
{ 
  typename Point_t::value_type two{2};
  auto half_side = side / two;

  if (std::is_integral<typename Point_t::value_type>::value) {
    int dxy = static_cast<int>(side) % 2;
    this->pt1 = Point_t(pt.x - half_side,
                        pt.y - half_side);
    this->pt2 = Point_t(pt.x + half_side + dxy,
                        pt.y + half_side + dxy);
  } else {
    this->pt1 = Point<T>(pt.x - half_side, 
                         pt.y - half_side);
    this->pt2 = Point<T>(pt.x + half_side, 
                         pt.y + half_side);
  }
}

template<typename Point_t> inline
Window<Point_t>::Window(const std::vector<Point_t> &window)
  : Entity(Entity::Type::window),
    pt1(std::numeric_limits<typename Point_t::value_type>().max(), 
    std::numeric_limits<typename Point_t::value_type>().max()), 
    pt2(-std::numeric_limits<typename Point_t::value_type>().max(), 
    -std::numeric_limits<typename Point_t::value_type>().max())
{
  if (window.size() >= 2) {
    for (size_t i = 0; i < window.size(); i++) {
      if (pt1.x > window[i].x) pt1.x = window[i].x;
      if (pt1.y > window[i].y) pt1.y = window[i].y;
      if (pt2.x < window[i].x) pt2.x = window[i].x;
      if (pt2.y < window[i].y) pt2.y = window[i].y;
    }
  }
}

template<typename Point_t> template<typename Point_t2> inline
Window<Point_t>::Window(const std::vector<Point_t2> &window)
  : Entity(Entity::Type::window),
    pt1(std::numeric_limits<typename Point_t::value_type>().max(), 
    std::numeric_limits<typename Point_t::value_type>().max()), 
    pt2(-std::numeric_limits<typename Point_t::value_type>().max(), 
    -std::numeric_limits<typename Point_t::value_type>().max())
{
  if (window.size() >= 2) {
    for (size_t i = 0; i < window.size(); i++) {
      if (std::is_integral<typename Point_t::value_type>::value) {
        if (pt1.x > window[i].x) pt1.x = static_cast<typename Point_t::value_type>(std::round(window[i].x));
        if (pt1.y > window[i].y) pt1.y = static_cast<typename Point_t::value_type>(std::round(window[i].y));
        if (pt2.x < window[i].x) pt2.x = static_cast<typename Point_t::value_type>(std::round(window[i].x));
        if (pt2.y < window[i].y) pt2.y = static_cast<typename Point_t::value_type>(std::round(window[i].y));
      } else {
        if (pt1.x > window[i].x) pt1.x = static_cast<typename Point_t::value_type>(window[i].x);
        if (pt1.y > window[i].y) pt1.y = static_cast<typename Point_t::value_type>(window[i].y);
        if (pt2.x < window[i].x) pt2.x = static_cast<typename Point_t::value_type>(window[i].x);
        if (pt2.y < window[i].y) pt2.y = static_cast<typename Point_t::value_type>(window[i].y);
      }
    }
  }
}

template<typename Point_t> inline
Window<Point_t> &Window<Point_t>::operator = (const Window &window)
{
  if (this != &window) {
    Entity::operator = (window);
    this->pt1 = window.pt1;
    this->pt2 = window.pt2;
  }
  return *this;
}

template<typename Point_t> inline
Window<Point_t> &Window<Point_t>::operator = (Window &&window) TL_NOEXCEPT
{
  if (this != &window) {
    Entity::operator = (std::forward<Entity>(window));
    this->pt1 = std::move(window.pt1);
    this->pt2 = std::move(window.pt2);
  }
  return *this;
}

template<typename Point_t> inline
bool Window<Point_t>::operator == (const Window &window) const
{
  return (pt1 == window.pt1 && pt2 == window.pt2);
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
typename Point_t::value_type Window<Point_t>::width() const 
{ 
  return this->isEmpty() ? 
    static_cast<typename Point_t::value_type>(0) : 
    pt2.x - pt1.x; 
}

template<typename Point_t> inline
typename Point_t::value_type Window<Point_t>::height() const 
{ 
  return this->isEmpty() ?
    static_cast<typename Point_t::value_type>(0) :
    pt2.y - pt1.y;
}

template<typename Point_t> inline
Point_t Window<Point_t>::center() const
{
  typename Point_t::value_type two{2};

  return Point_t((pt1.x + pt2.x) / two, 
                 (pt1.y + pt2.y) / two);
}

template<typename Point_t> inline
bool Window<Point_t>::isEmpty() const 
{ 
  return (pt1.x == std::numeric_limits<typename Point_t::value_type>().max() && 
          pt1.y == std::numeric_limits<typename Point_t::value_type>().max() && 
          pt2.x == -std::numeric_limits<typename Point_t::value_type>().max() && 
          pt2.y == -std::numeric_limits<typename Point_t::value_type>().max());
}

template<typename Point_t>
inline bool tl::Window<Point_t>::isValid() const
{
  return this->width() > static_cast<typename Point_t::value_type>(0) && 
         this->height() > static_cast<typename Point_t::value_type>(0);
}

template<typename Point_t>
inline void tl::Window<Point_t>::normalized()
{
  if (!this->isValid()) {
    if (this->pt1.x > this->pt2.x) std::swap(this->pt1.x, this->pt2.x);
    if (this->pt1.y > this->pt2.y) std::swap(this->pt1.y, this->pt2.y);
  }
}

template<typename Point_t> template<typename Point_t2> inline
bool Window<Point_t>::containsPoint(const Point_t2 &pt) const
{
  Point_t _pt(pt.x, pt.y);
  return ((pt2.x >= _pt.x) && (pt2.y >= _pt.y) &&
          (pt1.x <= _pt.x) && (pt1.y <= _pt.y));
}

template<typename Point_t> template<typename Point_t2> inline
bool Window<Point_t>::containsWindow(const Window<Point_t2> &w) const
{
  Window<Point_t> w2 = w;
  return pt1.x <= w2.pt1.x && 
         pt1.y <= w2.pt1.y && 
         pt2.x >= w2.pt2.x && 
         pt2.y >= w2.pt2.y;
}

using WindowI = Window<Point<int> >;
using WindowD = Window<Point<double> >;
using WindowF = Window<Point<float> >;



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

template<typename Point_t> static inline
bool operator == (const Window<Point_t> &window1, const Window<Point_t> &window2)
{
  return (window1.pt1 == window2.pt1 && 
          window1.pt2 == window2.pt2);
}

template<typename Point_t> static inline
bool operator != (const Window<Point_t> &window1, const Window<Point_t> &window2)
{
  return (window1.pt1 != window2.pt1 || 
          window1.pt2 != window2.pt2);
}

template<typename It> inline
Window<typename std::iterator_traits<It>::value_type> boundingWindow(It begin, It end)
{
  using Point_t = typename std::iterator_traits<It>::value_type;
  Window<Point_t> window;
  
  while(begin != end) {
    if (window.pt1.x > begin->x) window.pt1.x = begin->x;
    if (window.pt1.y > begin->y) window.pt1.y = begin->y;
    if (window.pt2.x < begin->x) window.pt2.x = begin->x;
    if (window.pt2.y < begin->y) window.pt2.y = begin->y;
    ++begin;
  }

  return window;
}

/*! \} */ // end of GeometricEntities

} // End namespace TL

#endif // TL_GEOMETRY_WINDOW_H
