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
 * License along with TidopLib. If not, see <http://www.gnu.org/licenses>.*
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#ifndef TL_GEOMETRY_POLYGON_H
#define TL_GEOMETRY_POLYGON_H

#include "config_tl.h"

#include "tidop/core/defs.h"
#include "tidop/core/utils.h"

#include "tidop/geometry/entities/entities2d.h"
#include "tidop/geometry/entities/entities3d.h"
#include "tidop/geometry/entities/point.h"
#include "tidop/geometry/entities/segment.h"
#include "tidop/geometry/algorithms.h"


namespace tl
{

/*! \addtogroup geometry
 *  \{
 */


template<typename Point_t>
class PolygonHole
  : public Entities2D<Point_t>
{

public:

  PolygonHole();
  PolygonHole(typename PolygonHole<Point_t>::size_type size);
  ~PolygonHole() override {}

};

template<typename Point_t> inline
PolygonHole<Point_t>::PolygonHole() 
  : Entities2D<Point_t>()
{
}

template<typename Point_t> inline
PolygonHole<Point_t>::PolygonHole(typename PolygonHole<Point_t>::size_type size)
  : Entities2D<Point_t>(size)
{
}

/*!
 * \brief Clase poligono 2D
 *
 * Se han definido los siguientes alias para facilitar el acceso:
 * \code
 * typedef Polygon<Point<int>> PolygonI;
 * typedef Polygon<Point<double>> PolygonD;
 * typedef Polygon<Point<float>> PolygonF;
 * \endcode
 */
template<typename Point_t>
class Polygon 
  : public Entity, 
    public Entities2D<Point_t>
{

public:

  /*!
   * \brief Constructora por defecto
   */
  Polygon();

  /*!
   * \brief Constructor que establece el tamaño del poligono
   */
  Polygon(typename Polygon<Point_t>::size_type size);

  /*!
   * \brief Constructor de copia
   * \param[in] polygon Objeto Polygon que se copia
   */
  Polygon(const Polygon &polygon);

  /*!
   * \brief Constructor de movimiento
   * \param[in] polygon Objeto Polygon que se mueve
   */
  Polygon(Polygon &&polygon) TL_NOEXCEPT;

  /*!
   * \brief Constructor
   * \param[in] points
   */
  Polygon(const std::vector<Point_t> &points);

  /*!
   * \brief Constructor lista de inicialización
   * \param[in] pointList Inicializador de lista con los puntos
   */
  Polygon(std::initializer_list<Point_t> pointList);

  /*!
   * \brief Destructora
   */
  ~Polygon() override = default;

  /*!
   * \brief Area de un poligono simple
   * http://mathworld.wolfram.com/PolygonArea.html
   * 
   * \return Area
   */
  double area() const;

  /*!
   * \brief Comprueba si un punto esta dentro del poligono
   * \param[in] point Punto
   */
  bool isInner(const Point_t &point) const;
  
  /*!
   * \brief Perimetro del poligono
   * \return Perimetro del poligono
   */
  double length() const;

  /*!
   * \brief Operador de asignación
   * \param[in] polygon Objeto Polygon que se mueve
   */
  Polygon<Point_t> &operator = (const Polygon<Point_t> &polygon);
    
  /*!
   * \brief Operador de asignación de movimiento
   * \param[in] polygon Objeto Polygon que se mueve
   */
  Polygon<Point_t> &operator = (Polygon<Point_t> &&polygon) TL_NOEXCEPT;

  /*!
   * \brief Ventana envolvente
   * \return Ventana envolvente de los puntos
   */
  Window<Point_t> window() const;

  size_t holes() const;
  PolygonHole<Point_t> hole(size_t id) const;
  void addHole(const PolygonHole<Point_t> &polygonHole);

protected:

  /*!
   * \brief Anillos interiores
   */
  std::vector<PolygonHole<Point_t>> mPolygonHole;

};

// Definición de métodos

template<typename Point_t> inline
Polygon<Point_t>::Polygon() 
  : Entity(Entity::Type::polygon2d),
    Entities2D<Point_t>(),
    mPolygonHole(0)
{
}

template<typename Point_t> inline
Polygon<Point_t>::Polygon(typename Polygon<Point_t>::size_type size)
  : Entity(Entity::Type::polygon2d),
    Entities2D<Point_t>(size),
    mPolygonHole(0)
{
}

template<typename Point_t> inline
Polygon<Point_t>::Polygon(const Polygon &polygon) 
  : Entity(polygon), 
    Entities2D<Point_t>(polygon),
    mPolygonHole(polygon.mPolygonHole)
{
}

template<typename Point_t> inline
Polygon<Point_t>::Polygon(Polygon &&polygon) TL_NOEXCEPT
  : Entity(std::forward<Entity>(polygon)), 
    Entities2D<Point_t>(std::forward<Entities2D<Point_t>>(polygon)),
    mPolygonHole(std::forward<std::vector<PolygonHole<Point_t>>>(polygon.mPolygonHole))
{
}


template<typename Point_t> inline
Polygon<Point_t>::Polygon(const std::vector<Point_t> &points) 
  : Entity(Entity::Type::polygon2d),
    Entities2D<Point_t>(points),
    mPolygonHole(0)
{
}

template<typename Point_t> inline
Polygon<Point_t>::Polygon(std::initializer_list<Point_t> pointList) 
  : Entity(Entity::Type::polygon2d),
    Entities2D<Point_t>(pointList),
    mPolygonHole(0)
{
}

template<typename Point_t> inline
bool Polygon<Point_t>::isInner(const Point_t &point) const
{
  Window<Point_t> w = this->window();
  // Comprueba si esta dentro de la ventana envolvente.
  if (w.containsPoint(point) == false) return false;

  // Se comprueba si el punto es uno de los vertices
  for (int i = 0; i < this->mEntities.size(); i++) {
    // Por ahora se devuelve true. Lo suyo sería indicar que es un vertice.
    if (this->mEntities[i] == point) return true;
  }

  Segment<Point_t> sPointH(point, Point_t(w.pt2.x, point.y));

  // Comprueba si corta una recta que pase por el punto corta en un numero 
  // par al poligono. La recta va desde el punto a la parte inferior, superior,
  // derecha o izquierda de la ventana.
  //... Se suma uno porque Mat comienza en cero. Se podria hacer una conversion a cv::Rect 
  //cv::Mat aux(w.height()+1, w.width()+1, CV_8UC3);
  //cv::line(aux, point-w.pt1, point-w.pt1, Color(Color::NAME::Red).get<cv::Scalar>(), 1);
  int nIntersection = 0;
  bool bVertex = false;
  std::vector<int> vertex_id;

  for (size_t i = 0, j = 1; i < this->mEntities.size(); i++, j++) {
    if (j == this->mEntities.size()) j = 0;


    Segment<Point_t> segment(this->mEntities[i], this->mEntities[j]);
    
    // El punto es colineal con el segmento y esta dentro del mismo.
    if (distPointToSegment(point, segment) == 0) return true;

    Point_t ptp;
    //if (projectPointInSegment(segment, point, &ptp) == 2) return true; // esta en la linea

      
    if (point.y == segment.pt1.y) {
      vertex_id.push_back(static_cast<int>(i));
      bVertex = true;
    }

    if (point.y == segment.pt1.y && point.y == segment.pt2.y) {
      // los dos segmentos forman parte de la misma linea horizontal
      // como no va a aparecer intersección...
      //nIntersection += 2;
    } else {
      nIntersection += intersectSegments(segment, sPointH, &ptp);
    }

  }

  // hay vertices
  // Si hay vertices hay que estudiar mas en detalle si el punto esta dentro o fuera
  if (bVertex == true) {
    std::vector<int> order;
    int vertex_prev = 0;
    int vertex_next = 0;
    for (size_t i = 0; i < vertex_id.size(); i++) {
      // Se comprueban los puntos anterior y siguiente
      if (vertex_id[i] == 0) vertex_prev = static_cast<int>(this->mEntities.size()) - 1;
      else vertex_prev = vertex_id[i] - 1;

      if (vertex_id[i] == this->mEntities.size() - 1) vertex_next = 0;
      else vertex_next = vertex_id[i] + 1;

      if (this->mEntities[vertex_prev].y == sPointH.pt1.y) {
        continue;
      } else {
        if (this->mEntities[vertex_next].y == sPointH.pt1.y) {
          int prev = isLeft(sPointH.pt1, sPointH.pt2, this->mEntities[vertex_prev]);
          if (vertex_next == this->mEntities.size() - 1) vertex_next = 0;
          else vertex_next = vertex_next + 1;
          int next = isLeft(sPointH.pt1, sPointH.pt2, this->mEntities[vertex_next]);
          if (prev == next) nIntersection -= 2;
          else nIntersection--;
        } else {
          int prev = isLeft(sPointH.pt1, sPointH.pt2, this->mEntities[vertex_prev]);
          int next = isLeft(sPointH.pt1, sPointH.pt2, this->mEntities[vertex_next]);
          if (prev == next) nIntersection -= 2;
          else nIntersection--;
        }

      }

    }
  }
  if (nIntersection < 0) return false;
  if (nIntersection % 2 == 0) return false;
  else return true;

  //// Probar con OpenCV
  //std::vector<cv::Point2f> points;
  //for (int i = 0; i < mEntities.size(); i++) {
  //  points.push_back(cv::Point2f(mEntities[i].x, mEntities[i].y));
  //}
  //double i_pol_test = cv::pointPolygonTest(mEntities, cv::Point2f(point.x, point.y), false);
  //if (i_pol_test == -1) return false;
  //else return true;

}

template<typename Point_t> inline
double Polygon<Point_t>::length()  const
{
  double perimeter = 0.;
  for (size_t i = 1; i < this->mEntities.size(); i++) {
    perimeter += distance(this->mEntities[i-1], this->mEntities[i]);
  }
  return perimeter;
}

template<typename Point_t> inline
double Polygon<Point_t>::area() const
{
  TL_TODO("Si el poligono es complejo hay que determinarla de otra forma. Primero hay que ver que sea complejo");
  double area = 0.;
  for (size_t i = 0; i < this->mEntities.size(); i++) {
    if (i == this->mEntities.size()-1)
      area += crossProduct(this->mEntities[i], this->mEntities[0]);
    else
      area += crossProduct(this->mEntities[i], this->mEntities[i+1]);
  }

  return std::abs(area / 2.);
}

template<typename Point_t> inline
Polygon<Point_t> &Polygon<Point_t>::operator = (const Polygon<Point_t> &polygon)
{
  if (this != &polygon) {
    Entity::operator = (polygon);
    Entities2D<Point_t>::operator = (polygon);
    mPolygonHole = polygon.mPolygonHole;
  }
  return *this;
}

template<typename Point_t> inline
Polygon<Point_t> &Polygon<Point_t>::operator = (Polygon<Point_t> &&polygon) TL_NOEXCEPT
{
  if (this != &polygon) {
    Entity::operator = (std::forward<Entity>(polygon));
    Entities2D<Point_t>::operator = (std::forward<Entities2D<Point_t>>(polygon));
    mPolygonHole = std::forward<std::vector<PolygonHole<Point_t>>>(polygon.mPolygonHole);
  }
  return *this;
}

template<typename Point_t> inline
Window<Point_t> Polygon<Point_t>::window() const
{
  Window<Point_t> w;
  for (size_t i = 0; i < this->mEntities.size(); i++) {
    if (w.pt1.x > this->mEntities[i].x) w.pt1.x = this->mEntities[i].x;
    if (w.pt1.y > this->mEntities[i].y) w.pt1.y = this->mEntities[i].y;
    if (w.pt2.x < this->mEntities[i].x) w.pt2.x = this->mEntities[i].x;
    if (w.pt2.y < this->mEntities[i].y) w.pt2.y = this->mEntities[i].y;
  }
  return w;
}

template<typename Point_t>
inline size_t tl::Polygon<Point_t>::holes() const
{
  return mPolygonHole.size();
}

template<typename Point_t>
inline PolygonHole<Point_t> tl::Polygon<Point_t>::hole(size_t id) const
{
  return mPolygonHole.at(id);
}

template<typename Point_t> inline 
void Polygon<Point_t>::addHole(const PolygonHole<Point_t> &polygonHole)
{
  mPolygonHole.push_back(polygonHole);
}


typedef Polygon<Point<int>> PolygonI;
typedef Polygon<Point<double>> PolygonD;
typedef Polygon<Point<float>> PolygonF;



/* ---------------------------------------------------------------------------------- */


template<typename Point3_t>
class Polygon3DHole
  : public Entities3D<Point3_t>
{

public:

  Polygon3DHole();
  Polygon3DHole(typename Polygon3DHole<Point3_t>::size_type size);
  ~Polygon3DHole() override {}

};

template<typename Point3_t> inline
Polygon3DHole<Point3_t>::Polygon3DHole() 
  : Entities3D<Point3_t>()
{
}

template<typename Point3_t> inline
Polygon3DHole<Point3_t>::Polygon3DHole(typename Polygon3DHole<Point3_t>::size_type size)
  : Entities3D<Point3_t>(size)
{
}

/*!
 * \brief Clase poligono 3D
 *
 * Se han definido los siguientes alias para facilitar el acceso:
 *
 * typedef Polygon3D<int> Polygon3dI;
 * typedef Polygon3D<double> Polygon3dD;
 * typedef Polygon3D<float> Polygon3dF;
 *
 */
template<typename Point3_t>
class Polygon3D 
  : public Entity, 
    public Entities3D<Point3_t>
{

public:

  /*!
   * \brief Constructora por defecto
   */
  Polygon3D();

  /*!
   * \brief Constructor que establece el tamaño del poligono
   */
  Polygon3D(typename Polygon3D<Point3_t>::size_type size);

  /*!
   * \brief Constructor de copia
   * \param[in] polygon Objeto Polygon3D que se copia
   */
  Polygon3D(const Polygon3D &polygon);
    
  /*!
   * \brief Constructor de movimiento
   * \param[in] polygon Objeto Polygon3D que se mueve 
   */
  Polygon3D(Polygon3D &&polygon) TL_NOEXCEPT;

  /*!
   * \brief Constructor
   * \param[in] points 
   */
  Polygon3D(const std::vector<Point3_t> &points);

  /*!
   * \brief Constructor lista de inicialización
   * \param[in] listPoints Inicializador de lista con los puntos
   */
  Polygon3D(std::initializer_list<Point3_t> listPoints);

  /*!
   * \brief Perimetro del poligono
   * \return Perimetro del poligono
   */
  double length() const;

  /*!
   * \brief Operador de asignación
   * \param[in] polygon Objeto Polygon3D que se copia
   */
  Polygon3D<Point3_t> &operator = (const Polygon3D<Point3_t> &polygon);
   
  /*!
   * \brief Operador de asignación
   * \param[in] polygon Objeto Polygon3D que se mueve 
   */
  Polygon3D<Point3_t> &operator = (Polygon3D<Point3_t> &&polygon) TL_NOEXCEPT;

  /*!
   * \brief Caja envolvente
   * \return Caja envolvente del polígono
   */
  BoundingBox<Point3_t> boundingBox() const;

  size_t holes() const;
  Polygon3DHole<Point3_t> hole(size_t id) const;
  void addHole(const Polygon3DHole<Point3_t> &polygonHole);

protected:

  /*!
   * \brief Anillos interiores
   */
  std::vector<Polygon3DHole<Point3_t>> mPolygonHole;

};

template<typename Point3_t> inline
Polygon3D<Point3_t>::Polygon3D() 
  : Entity(Entity::Type::polygon3d),
    Entities3D<Point3_t>(),
    mPolygonHole(0) 
{
}

template<typename Point3_t> inline
Polygon3D<Point3_t>::Polygon3D(typename Polygon3D<Point3_t>::size_type size)
  : Entity(Entity::Type::polygon3d),
    Entities3D<Point3_t>(size),
    mPolygonHole(0) 
{
}

template<typename Point3_t> inline
Polygon3D<Point3_t>::Polygon3D(const Polygon3D &polygon) 
  : Entity(polygon), 
    Entities3D<Point3_t>(polygon),
    mPolygonHole(polygon.mPolygonHole)
{
}

template<typename Point3_t> inline
Polygon3D<Point3_t>::Polygon3D(Polygon3D &&polygon) TL_NOEXCEPT
  : Entity(std::forward<Entity>(polygon)), 
    Entities3D<Point3_t>(std::forward<Entities3D<Point3_t>>(polygon)),
    mPolygonHole(std::forward<std::vector<Polygon3DHole<Point3_t>>>(polygon.mPolygonHole))
{
}

template<typename Point3_t> inline
Polygon3D<Point3_t>::Polygon3D(const std::vector<Point3_t> &points) 
  : Entity(Entity::Type::polygon3d),
    Entities3D<Point3_t>(points),
    mPolygonHole(0)
{
}

template<typename Point3_t> inline
Polygon3D<Point3_t>::Polygon3D(std::initializer_list<Point3_t> listPoints) 
  : Entity(Entity::Type::polygon3d),
    Entities3D<Point3_t>(listPoints),
    mPolygonHole(0) 
{
}

template<typename Point3_t> inline
double Polygon3D<Point3_t>::length()  const
{
  double perimeter = 0.;
  for (size_t i = 1; i < this->mEntities.size(); i++) {
    perimeter += distance(this->mEntities[i-1], this->mEntities[i]);
  }
  return perimeter;
}

template<typename Point3_t> inline
Polygon3D<Point3_t> &Polygon3D<Point3_t>::operator = (const Polygon3D<Point3_t> &polygon)
{
  if (this != &polygon) {
    Entity::operator = (polygon);
    Entities3D<Point3_t>::operator = (polygon);
    mPolygonHole = polygon.mPolygonHole;
  }
  return *this;
}

template<typename Point3_t> inline
Polygon3D<Point3_t> &Polygon3D<Point3_t>::operator = (Polygon3D<Point3_t> &&polygon) TL_NOEXCEPT
{
  if (this != &polygon) {
    Entity::operator = (std::forward<Entity>(polygon));
    Entities3D<Point3_t>::operator = (std::forward<Entities3D<Point3_t>>(polygon));
    mPolygonHole = std::forward<std::vector<Polygon3DHole<Point3_t>>>(polygon.mPolygonHole);
  }
  return *this;
}

template<typename Point3_t> inline
BoundingBox<Point3_t> Polygon3D<Point3_t>::boundingBox() const
{
  BoundingBox<Point3_t> bounding_box;
  for (size_t i = 0; i < this->mEntities.size(); i++) {
    if (bounding_box.pt1.x > this->mEntities[i].x) bounding_box.pt1.x = this->mEntities[i].x;
    if (bounding_box.pt1.y > this->mEntities[i].y) bounding_box.pt1.y = this->mEntities[i].y;
    if (bounding_box.pt1.z > this->mEntities[i].z) bounding_box.pt1.z = this->mEntities[i].z;
    if (bounding_box.pt2.x < this->mEntities[i].x) bounding_box.pt2.x = this->mEntities[i].x;
    if (bounding_box.pt2.y < this->mEntities[i].y) bounding_box.pt2.y = this->mEntities[i].y;
    if (bounding_box.pt2.z < this->mEntities[i].z) bounding_box.pt2.z = this->mEntities[i].z;
  }
  return bounding_box;
}

template<typename Point3_t> inline 
size_t Polygon3D<Point3_t>::holes() const
{
  return mPolygonHole.size();
}

template<typename Point3_t> inline 
Polygon3DHole<Point3_t> Polygon3D<Point3_t>::hole(size_t id) const
{
  return mPolygonHole.at(id);
}

template<typename Point3_t> inline 
void Polygon3D<Point3_t>::addHole(const Polygon3DHole<Point3_t> &polygonHole)
{
  mPolygonHole.push_back(polygonHole);
}


typedef Polygon3D<Point3<int>> Polygon3dI;
typedef Polygon3D<Point3<double>> Polygon3dD;
typedef Polygon3D<Point3<float>> Polygon3dF;


/* ---------------------------------------------------------------------------------- */

template <typename Point_t>
class MultiPolygon
  : public Entity,
    public Entities2D<Polygon<Point_t>>
{

public:

  /*!
   * \brief Constructora por defecto
   */
  MultiPolygon();

  /*!
   * \brief Constructor que reserva tamaño para n poligonos
   */
  MultiPolygon(typename MultiPolygon<Point_t>::size_type size);

  /*!
   * \brief Constructor de copia
   * \param[in] multiPolygon Objeto MultiPolygon que se copia
   */
  MultiPolygon(const MultiPolygon &multiPolygon);

  /*!
   * \brief Constructor de movimiento
   * \param[in] multiPolygon Objeto MultiPolygon que se mueve
   */
  MultiPolygon(MultiPolygon &&multiPolygon) TL_NOEXCEPT;

  /*!
   * \brief Destructora
   */
  ~MultiPolygon() {}

  /*!
   * \brief Operador de asignación
   * \return lineString Objeto que se asigna
   */
  MultiPolygon<Point_t> &operator = (const MultiPolygon &multiPolygon);
  
  /*!
   * \brief Operador de asignación de movimiento
   * \return lineString Objeto que se mueve
   */
  MultiPolygon<Point_t> &operator = (MultiPolygon &&multiPolygon) TL_NOEXCEPT;

  Window<Point_t> window() const;
};

template <typename Point_t>
MultiPolygon<Point_t>::MultiPolygon()
  : Entity(Entity::Type::multipolygon2d),
    Entities2D<Polygon<Point_t>>()
{
}

template<typename Point_t> inline
MultiPolygon<Point_t>::MultiPolygon(typename MultiPolygon<Point_t>::size_type size)
  : Entity(Entity::Type::multipolygon2d),
    Entities2D<Polygon<Point_t>>(size) 
{
}

template<typename Point_t> inline
MultiPolygon<Point_t>::MultiPolygon(const MultiPolygon &multiPolygon) 
  : Entity(multiPolygon), 
    Entities2D<Polygon<Point_t>>(multiPolygon) 
{
}

template<typename Point_t> inline
MultiPolygon<Point_t>::MultiPolygon(MultiPolygon &&multiPolygon) TL_NOEXCEPT
  : Entity(std::forward<Entity>(multiPolygon)), 
    Entities2D<Polygon<Point_t>>(std::forward<Entities2D<Polygon<Point_t>>>(multiPolygon)) 
{
}

template<typename Point_t> inline
MultiPolygon<Point_t> &MultiPolygon<Point_t>::operator = (const MultiPolygon &multiPolygon)
{
  if (this != &multiPolygon) {
    Entity::operator = (multiPolygon);
    Entities2D<Polygon<Point_t>>::operator = (multiPolygon);
  }
  return *this;
}

template<typename Point_t> inline
MultiPolygon<Point_t> &MultiPolygon<Point_t>::operator = (MultiPolygon &&multiPolygon) TL_NOEXCEPT
{
  if (this != &multiPolygon) {
    Entity::operator = (std::forward<Entity>(multiPolygon));
    Entities2D<Polygon<Point_t>>::operator = (std::forward<Entities2D<Polygon<Point_t>>>(multiPolygon));
  }
  return *this;
}

template<typename Point_t> inline
Window<Point_t> MultiPolygon<Point_t>::window() const
{
  Window<Point_t> w;
  for (size_t i = 0; i < this->mEntities.size(); i++) {
    w = joinWindow(w, this->mEntities[i].window());
  }
  return w;
}

/* ---------------------------------------------------------------------------------- */

template <typename Point3_t>
class MultiPolygon3D
  : public Entity,
    public Entities3D<Polygon3D<Point3_t>>
{

public:

  /*!
   * \brief Constructora por defecto
   */
  MultiPolygon3D();
    
  /*!
   * \brief Constructor que reserva tamaño para n poligonos
   */
  MultiPolygon3D(typename MultiPolygon3D<Point3_t>::size_type size);

  /*!
   * \brief Constructor de copia
   * \param[in] multiPolygon Objeto MultiPolygon que se copia
   */
  MultiPolygon3D(const MultiPolygon3D &multiPolygon);

  /*!
   * \brief Constructor de movimiento
   * \param[in] multiPolygon Objeto MultiPolygon que se mueve
   */
  MultiPolygon3D(MultiPolygon3D &&multiPolygon) TL_NOEXCEPT;

  /*!
   * \brief Destructora
   */
  ~MultiPolygon3D() {}

  /*!
   * \brief Operador de asignación
   * \return lineString Objeto que se asigna
   */
  MultiPolygon3D<Point3_t> &operator = (const MultiPolygon3D &multiPolygon);

  /*!
   * \brief Operador de asignación
   * \return lineString Objeto que se asigna
   */
  MultiPolygon3D<Point3_t> &operator = (MultiPolygon3D &&multiPolygon) TL_NOEXCEPT;

  /*!
   * \brief Caja envolvente
   * \return Caja envolvente de los poligonos
   */
  BoundingBox<Point3_t> boundingBox() const;
};

template <typename Point3_t>
MultiPolygon3D<Point3_t>::MultiPolygon3D()
  : Entity(Entity::Type::multipoygon3d),
    Entities3D<Polygon3D<Point3_t>>()
{
}

template<typename Point3_t> inline
MultiPolygon3D<Point3_t>::MultiPolygon3D(typename MultiPolygon3D<Point3_t>::size_type size)
  : Entity(Entity::Type::multipoygon3d),
    Entities3D<Polygon3D<Point3_t>>(size) 
{
}

template<typename Point3_t> inline
MultiPolygon3D<Point3_t>::MultiPolygon3D(const MultiPolygon3D &multiPolygon) 
  : Entity(multiPolygon), 
    Entities3D<Polygon3D<Point3_t>>(multiPolygon) 
{
}

template<typename Point3_t> inline
MultiPolygon3D<Point3_t>::MultiPolygon3D(MultiPolygon3D &&multiPolygon) TL_NOEXCEPT
  : Entity(std::forward<Entity>(multiPolygon)), 
    Entities3D<Polygon3D<Point3_t>>(std::forward<MultiPolygon3D<Point3_t>>(multiPolygon)) 
{
}

template<typename Point3_t> inline
MultiPolygon3D<Point3_t> &MultiPolygon3D<Point3_t>::operator = (const MultiPolygon3D &multiPolygon)
{
  if (this != &multiPolygon) {
    Entity::operator = (multiPolygon);
    Entities3D<Polygon3D<Point3_t>>::operator = (multiPolygon);
  }
  return *this;
}

template<typename Point3_t> inline
MultiPolygon3D<Point3_t> &MultiPolygon3D<Point3_t>::operator = (MultiPolygon3D &&multiPolygon) TL_NOEXCEPT
{
  if (this != &multiPolygon) {
    Entity::operator = (std::forward<Entity>(multiPolygon));
    Entities3D<Polygon3D<Point3_t>>::operator = (std::forward<Entities3D<Polygon3D<Point3_t>>>(multiPolygon));
  }
  return *this;
}


template<typename Point3_t> inline
BoundingBox<Point3_t> MultiPolygon3D<Point3_t>::boundingBox() const
{
  BoundingBox<Point3_t> bounding_box;
  for (size_t i = 0; i < this->mEntities.size(); i++) {
    bounding_box = joinBoundingBoxes(bounding_box, this->mEntities[i].boundingBox());
  }
  return bounding_box;
}


/*! \} */ // end of geometry

} // End namespace tl

#endif // TL_GEOMETRY_POLYGON_H
