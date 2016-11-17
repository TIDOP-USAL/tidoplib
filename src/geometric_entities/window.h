#ifndef I3D_WINDOW_H
#define I3D_WINDOW_H

#include <algorithm>

#include "opencv2/core/core.hpp"

#include "core/defs.h"

namespace I3D
{

/*! \defgroup GeometricEntities Entidades geométricas
 *  Puntos, lineas, ...
 *  \{
 */


// Operaciones con ventanas
/*!
 * \brief Comprueba si dos ventanas intersectan
 * \param[in] w1 Ventana 1
 * \param[in] w2 Ventana 2
 * \return Verdadero si intersectan
 */
template<typename T1, typename T2>
bool I3D_EXPORT intersectWindows(T1 w1, T2 w2)
{
  return w1.pt2.x >= w2.pt1.x && w1.pt2.y >= w2.pt1.y &&  w1.pt1.x <= w2.pt2.x && w1.pt1.y <= w2.pt2.y;
}

/*!
 * \brief Ventana interseción
 * \param[in] w1 Ventana 1
 * \param[in] w2 Ventana 2
 * \return Ventana interseción
 */
template<typename T>
T I3D_EXPORT windowIntersection(T w1, T w2)
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
template<typename T>
T I3D_EXPORT joinWindow(T w1, T w2)
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
template<typename T1, typename T2>
T1 I3D_EXPORT expandWindow(T1 w, T2 szx, T2 szy)
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
template<typename T1, typename T2>
T1 I3D_EXPORT expandWindow(T1 w, T2 sz)
{
  return expandWindow(w, sz, sz);
}

/*!
* \brief The Window class
*/
template<typename T>
class I3D_EXPORT Window
{
public:

  /*!
   * \brief type
   */
  typedef T type;

  /*!
   * \brief Punto 1
   */
  cv::Point_<T> pt1;

  /*!
   * \brief Punto 2
   */
  cv::Point_<T> pt2;

public:

  /*!
   * \brief Constructor por defecto
   */
  Window();

  /*!
   * \brief Constructor de copia
   */
  Window(const Window &w);

  /*!
   * \brief Constructor de movimiento Window
   */
  //Window(Window &&w);

  /*!
   * \brief Constructor Window
   * \param[in] pt1 Primer punto
   * \param[in] pt2 Segundo punto
   */
  Window(const cv::Point_<T> &pt1, const cv::Point_<T> &pt2);

  /*!
   * \brief Constructor Window
   * \param[in] pt Punto central
   * \param[in] sxx Ancho de la ventana
   * \param[in] szy Alto de la ventana
   */
  Window(cv::Point_<T> &pt, T sxx, T szy);

  /*!
   * \brief Constructor Window
   * \param[in] pt Punto central
   * \param[in] sz Alto y ancho de la ventana
   */
  Window(cv::Point_<T> &pt, T sz);

  /*!
   * \brief Constructor Window
   * Crea una ventana que envuelve a un conjunto de puntos
   *
   * \param[in] v vector de puntos
   */
  Window(std::vector<cv::Point_<T>> v);

  /*!
   * \brief Constructor de copia con una ventana de otro tipo
   * \param[in] v Ventana
   */
  template<typename T2> Window(std::vector<cv::Point_<T2>> v);

  //~Window();

  /*!
   * \brief Sobrecarga del operador  de asignación
   * \param[in] w Ventana que se asigna
   * \return Referencia a la ventana
   */
  Window &operator = (const Window &w);

  /*!
   * \brief Sobrecarga del operador 'igual que'
   * \param[in] w Ventana con la que se compara
   * \return true si ambas ventanas son iguales
   */
  bool operator == (const Window &w) const;

  /*!
   * \brief Conversión a una ventana de un tipo diferente
   */
  template<typename T2> operator Window<T2>() const;

  /*!
   * \brief Devuelve el ancho de la ventana
   * \return Ancho
   */
  T getWidth() const { return pt2.x - pt1.x; }

  /*!
   * \brief Devuelve el alto de la ventana
   * \return Alto
   */
  T getHeight() const { return pt2.y - pt1.y; }

  /*!
   * \brief Devuelve centro de la ventana
   * \return Centro de la ventana
   */
  cv::Point_<T> getCenter() const;

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
  template<typename T2> bool containsPoint(const cv::Point_<T2> &pt) const;

  /*!
   * \brief La ventana contiene la ventana
   */
  template<typename T2> bool containsWindow(const Window<T2> &w) const;

};

// Definición de métodos

template<typename T> inline
Window<T>::Window() : pt1(std::numeric_limits<T>().max(), std::numeric_limits<T>().max()), 
                      pt2(-std::numeric_limits<T>().max(), -std::numeric_limits<T>().max()) {}

template<typename T> inline
Window<T>::Window(const Window &w) : pt1(w.pt1), pt2(w.pt2) {}

//template<typename T> inline
//Window<T>::Window(Window &&w)
//{
//  pt1 = std::move(w.pt1);
//  pt2 = std::move(w.pt2);
//}

template<typename T> inline
Window<T>::Window(const cv::Point_<T> &_pt1, const cv::Point_<T> &_pt2)
{
  pt1.x = std::min(_pt1.x, _pt2.x);
  pt1.y = std::min(_pt1.y, _pt2.y);
  pt2.x = std::max(_pt1.x, _pt2.x);
  pt2.y = std::max(_pt1.y, _pt2.y);
}

template<typename T> inline
Window<T>::Window(cv::Point_<T> &_pt, T sxx, T szy)
{ 
  if (typeid(T) == typeid(int)) {
    // Prefiero hacer la conversión a entero para evitar que OpenCV 
    // lo haga mediante cvRound 
    int sxx_2 = I3D_ROUND_TO_INT(sxx / 2);
    int szy_2 = I3D_ROUND_TO_INT(szy / 2);
    int dx = static_cast<int>(sxx) % 2;
    int dy = static_cast<int>(szy) % 2;
    pt1 = cv::Point_<T>(_pt.x - sxx_2, _pt.y - szy_2);
    pt2 = cv::Point_<T>(_pt.x + sxx_2 + dx, _pt.y + szy_2 + dy);
  } else {
    // Quito el warning que da cuando es una ventana de enteros. En ese caso nunca pasara por aqui.
    I3D_DISABLE_WARNING(4244)
    pt1 = cv::Point_<T>(_pt.x - sxx / 2., _pt.y - szy / 2.);
    pt2 = cv::Point_<T>(_pt.x + sxx / 2., _pt.y + szy / 2.);
    I3D_ENABLE_WARNING(4244)
  }
}

template<typename T> inline
Window<T>::Window(cv::Point_<T> &_pt, T sz) : Window<T>::Window(_pt, sz, sz) {}

template<typename T> inline
Window<T>::Window(std::vector<cv::Point_<T>> v)
  : pt1(std::numeric_limits<T>().max(), std::numeric_limits<T>().max()), 
    pt2(-std::numeric_limits<T>().max(), -std::numeric_limits<T>().max())
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

template<typename T> template<typename T2> inline
Window<T>::Window(std::vector<cv::Point_<T2>> v)
  : pt1(std::numeric_limits<T>().max(), std::numeric_limits<T>().max()), 
    pt2(-std::numeric_limits<T>().max(), -std::numeric_limits<T>().max())
{
  if (v.size() >= 2) {
    for (size_t i = 0; i < v.size(); i++) {
      if (typeid(T) == typeid(int)) {
        if (pt1.x > v[i].x) pt1.x = I3D_ROUND_TO_INT(v[i].x);
        if (pt1.y > v[i].y) pt1.y = I3D_ROUND_TO_INT(v[i].y);
        if (pt2.x < v[i].x) pt2.x = I3D_ROUND_TO_INT(v[i].x);
        if (pt2.y < v[i].y) pt2.y = I3D_ROUND_TO_INT(v[i].y);
      } else {
        if (pt1.x > v[i].x) pt1.x = static_cast<T>(v[i].x);
        if (pt1.y > v[i].y) pt1.y = static_cast<T>(v[i].y);
        if (pt2.x < v[i].x) pt2.x = static_cast<T>(v[i].x);
        if (pt2.y < v[i].y) pt2.y = static_cast<T>(v[i].y);
      }
    }
  }
}

template<typename T> inline
Window<T> &Window<T>::operator = (const Window &w)
{
  if (this != &w) {
    pt1 = w.pt1;
    pt2 = w.pt2;
  }
  return *this;
}

template<typename T> inline
bool Window<T>::operator == (const Window &w) const
{
  return (pt1 == w.pt1 && pt2 == w.pt2);
}

template<typename T> template<typename T2> inline
Window<T>::operator Window<T2>() const
{
  // El saturate_cast de OpenCV al hacer cast a int parece que no funciona muy bien.
  // 0.5 lo convierte a 0. 2.5 lo convierte a 2. Sin embargo 1.5 lo convierte a 2...
  //cv::Point_<T2> _pt1 = pt1;
  //cv::Point_<T2> _pt2 = pt2;
  //return Window<T2>(_pt1, _pt2);
  //habria que crear un cast personalizado para transformar a entero. 
  if (typeid(T2) == typeid(int)) {
    //Dos posibles soluciones. Ver cual es mas eficiente
    cv::Point_<T2> _pt1(I3D_ROUND_TO_INT(pt1.x), I3D_ROUND_TO_INT(pt1.y));
    cv::Point_<T2> _pt2(I3D_ROUND_TO_INT(pt2.x), I3D_ROUND_TO_INT(pt2.y));
    return Window<T2>(_pt1, _pt2);
  } else {
    cv::Point_<T2> _pt1 = pt1;
    cv::Point_<T2> _pt2 = pt2;
    return Window<T2>(_pt1, _pt2);
  }
}

template<typename T> inline
cv::Point_<T> Window<T>::getCenter() const
{
  if (typeid(T) == typeid(int)) {
    return cv::Point_<T>(static_cast<int>(std::floor((pt1.x + pt2.x) / 2)), static_cast<int>(std::floor((pt1.y + pt2.y) / 2)));
  } else {
    I3D_DISABLE_WARNING(4244)
    return cv::Point_<T>((pt1.x + pt2.x) / 2., (pt1.y + pt2.y) / 2.);
    I3D_ENABLE_WARNING(4244)
  }
}

template<typename T> inline
bool Window<T>::isEmpty() const 
{ 
  return (   pt1.x == std::numeric_limits<T>().max() 
          && pt1.y == std::numeric_limits<T>().max() 
          && pt2.x == -std::numeric_limits<T>().max() 
          && pt2.y == -std::numeric_limits<T>().max()); 
};

template<typename T> template<typename T2> inline
bool Window<T>::containsPoint(const cv::Point_<T2> &pt) const
{
  cv::Point_<T> _pt = pt;
  return ((pt2.x >= _pt.x) && (pt2.y >= _pt.y) &&
    (pt1.x <= _pt.x) && (pt1.y <= _pt.y));
}

template<typename T> template<typename T2> inline
bool Window<T>::containsWindow(const Window<T2> &w) const
{
  Window<T> w2 = w;
  return pt1.x <= w2.pt1.x && pt1.y <= w2.pt1.y && pt2.x >= w2.pt2.x && pt2.y >= w2.pt2.y;
}

typedef Window<int> WindowI;
typedef Window<double> WindowD;
typedef Window<float> WindowF;


template<typename T>
Window<T> moveWindow(const Window<T> &w, T dx, T dy)
{
  Window<T> w_return = w;
  cv::Point_<T> t(dx, dy);
  w_return.pt1 += t;
  w_return.pt2 += t;
  return w_return;
}



/*!
 * \brief Convierte una ventana a un Rect de OpenCV
 * \param[in] w Ventana que se va a expandir
 * \return Ventana resultante
 */
cv::Rect I3D_EXPORT windowToCvRect(WindowI w);


/*! \} */ // end of GeometricEntities

} // End namespace I3D

#endif // I3D_WINDOW_H
