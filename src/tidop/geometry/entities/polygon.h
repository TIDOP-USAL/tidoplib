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

#pragma once

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

    using size_type = typename PolygonHole<Point_t>::size_type;

public:

    PolygonHole();
    explicit PolygonHole(size_type size);
    PolygonHole(std::initializer_list<Point_t> pointList);
    ~PolygonHole() override = default;

};



/*!
 * \brief 2D polygon class
 */
template<typename Point_t>
class Polygon
  : public Entity,
    public Entities2D<Point_t>
{

public:

    using size_type = typename Polygon<Point_t>::size_type;

protected:

    std::vector<PolygonHole<Point_t>> polygonHoles;

public:

    Polygon();
    explicit Polygon(size_type size);
    Polygon(const Polygon &polygon);
    Polygon(Polygon &&polygon) TL_NOEXCEPT;
    explicit Polygon(const std::vector<Point_t> &points);
    Polygon(std::initializer_list<Point_t> pointList);

    ~Polygon() override = default;

    /*!
     * \brief Polygon Area
     * http://mathworld.wolfram.com/PolygonArea.html
     *
     */
    auto area() const -> double;

    /*!
     * \brief Check if a point is inside the polygon
     * \param[in] point Point
     */
    auto isInner(const Point_t &point) const -> bool;

    /*!
     * \brief Polygon perimeter
     */
    auto length() const -> double;

    /*!
     * \brief Copy assignment operator
     */
    auto operator = (const Polygon<Point_t> &polygon) -> Polygon<Point_t> &;

    /*!
     * \brief Move assignment operator
     */
    auto operator = (Polygon<Point_t> &&polygon) TL_NOEXCEPT -> Polygon<Point_t> &;

    /*!
     * \brief Window
     */
    auto window() const -> Window<Point_t>;

    auto holes() const -> size_t;
    auto hole(size_t id) const -> PolygonHole<Point_t>;
    void addHole(const PolygonHole<Point_t> &polygonHole);

};


using PolygonI = Polygon<Point<int>>;
using PolygonD = Polygon<Point<double>>;
using PolygonF = Polygon<Point<float>>;



/* ---------------------------------------------------------------------------------- */


template<typename Point3_t>
class Polygon3DHole
    : public Entities3D<Point3_t>
{

public:

    using size_type = typename Polygon3DHole<Point3_t>::size_type;

public:

    Polygon3DHole();
    explicit Polygon3DHole(size_type size);
    Polygon3DHole(std::initializer_list<Point3_t> pointList);
    ~Polygon3DHole() override = default;

};



/*!
 * \brief 3D polygon class
 *
 */
template<typename Point3_t>
class Polygon3D
  : public Entity,
    public Entities3D<Point3_t>
{

public:

    using size_type = typename Polygon3D<Point3_t>::size_type;

protected:

    std::vector<Polygon3DHole<Point3_t>> polygonHoles;

public:

    Polygon3D();
    explicit Polygon3D(size_type size);
    Polygon3D(const Polygon3D &polygon);
    Polygon3D(Polygon3D &&polygon) TL_NOEXCEPT;
    explicit Polygon3D(const std::vector<Point3_t> &points);
    Polygon3D(std::initializer_list<Point3_t> listPoints);

    /*!
     * \brief Polygon perimeter
     */
    auto length() const -> double;

    /*!
     * \brief Copy assignment operator
     */
    auto operator = (const Polygon3D<Point3_t> &polygon) -> Polygon3D<Point3_t> &;

    /*!
     * \brief Move assignment operator
     */
    auto operator = (Polygon3D<Point3_t> &&polygon) TL_NOEXCEPT -> Polygon3D<Point3_t> &;

    /*!
     * \brief Bounding Box
     */
    auto boundingBox() const -> BoundingBox<Point3_t>;

    auto holes() const -> size_t;
    auto hole(size_t id) const -> Polygon3DHole<Point3_t>;
    void addHole(const Polygon3DHole<Point3_t> &polygonHole);


};

using Polygon3dI = Polygon3D<Point3<int>>;
using Polygon3dD = Polygon3D<Point3<double>>;
using Polygon3dF = Polygon3D<Point3<float>>;


/* ---------------------------------------------------------------------------------- */

template <typename Point_t>
class MultiPolygon
  : public Entity,
    public Entities2D<Polygon<Point_t>>
{

public:

    using size_type = typename MultiPolygon<Point_t>::size_type;

public:

    MultiPolygon();
    explicit MultiPolygon(size_type size);
    MultiPolygon(const MultiPolygon &multiPolygon);
    MultiPolygon(MultiPolygon &&multiPolygon) TL_NOEXCEPT;

    ~MultiPolygon() override = default;

    /*!
     * \brief Copy assignment operator
     */
    auto operator =(const MultiPolygon& multiPolygon) -> MultiPolygon<Point_t>&;

    /*!
     * \brief Move assignment operator
     */
    auto operator =(MultiPolygon&& multiPolygon) TL_NOEXCEPT -> MultiPolygon<Point_t>&;

    auto window() const -> Window<Point_t>;
};




/* ---------------------------------------------------------------------------------- */



template <typename Point3_t>
class MultiPolygon3D
  : public Entity,
    public Entities3D<Polygon3D<Point3_t>>
{
public:

    using size_type = typename MultiPolygon3D<Point3_t>::size_type;

public:

    MultiPolygon3D();
    MultiPolygon3D(size_type size);
    MultiPolygon3D(const MultiPolygon3D &multiPolygon);
    MultiPolygon3D(MultiPolygon3D &&multiPolygon) TL_NOEXCEPT;

    ~MultiPolygon3D() override = default;

    /*!
     * \brief Copy assignment operator
     */
    auto operator =(const MultiPolygon3D& multiPolygon) -> MultiPolygon3D<Point3_t>&;

    /*!
     * \brief Move assignment operator
     */
    auto operator =(MultiPolygon3D&& multiPolygon) TL_NOEXCEPT -> MultiPolygon3D<Point3_t>&;

    /*!
     * \brief Bounding box
     */
    auto boundingBox() const -> BoundingBox<Point3_t>;
};





/* PolygonHole */


template<typename Point_t>
PolygonHole<Point_t>::PolygonHole() 
  : Entities2D<Point_t>()
{
}

template<typename Point_t>
PolygonHole<Point_t>::PolygonHole(size_type size)
  : Entities2D<Point_t>(size)
{
}

template<typename Point_t>
PolygonHole<Point_t>::PolygonHole(std::initializer_list<Point_t> pointList)
  : Entities2D<Point_t>(pointList)
{
}



/* Polygon */

template<typename Point_t>
Polygon<Point_t>::Polygon() 
  : Entity(Type::polygon2d),
    Entities2D<Point_t>(),
    polygonHoles(0)
{
}

template<typename Point_t>
Polygon<Point_t>::Polygon(size_type size)
  : Entity(Type::polygon2d),
    Entities2D<Point_t>(size),
    polygonHoles(0)
{
}

template<typename Point_t>
Polygon<Point_t>::Polygon(const Polygon &polygon) 
  : Entity(polygon), 
    Entities2D<Point_t>(polygon),
    polygonHoles(polygon.polygonHoles)
{
}

template<typename Point_t>
Polygon<Point_t>::Polygon(Polygon &&polygon) TL_NOEXCEPT
  : Entity(std::forward<Entity>(polygon)), 
    Entities2D<Point_t>(std::forward<Entities2D<Point_t>>(polygon)),
    polygonHoles(std::forward<std::vector<PolygonHole<Point_t>>>(polygon.polygonHoles))
{
}


template<typename Point_t>
Polygon<Point_t>::Polygon(const std::vector<Point_t> &points) 
  : Entity(Type::polygon2d),
    Entities2D<Point_t>(points),
    polygonHoles(0)
{
}

template<typename Point_t>
Polygon<Point_t>::Polygon(std::initializer_list<Point_t> pointList) 
  : Entity(Type::polygon2d),
    Entities2D<Point_t>(pointList),
    polygonHoles(0)
{
}

template<typename Point_t>
auto Polygon<Point_t>::isInner(const Point_t &point) const -> bool
{
    Window<Point_t> w = this->window();
    // Comprueba si esta dentro de la ventana envolvente.
    if (w.containsPoint(point) == false) return false;

    // Se comprueba si el punto es uno de los vertices
    for (int i = 0; i < this->size(); i++) {
        // Por ahora se devuelve true. Lo suyo sería indicar que es un vertice.
        if (this->at(i) == point) return true;
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

    for (size_t i = 0, j = 1; i < this->size(); i++, j++) {
        if (j == this->size()) j = 0;


        Segment<Point_t> segment(this->at(i), this->at(j));

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
    if (bVertex) {
        std::vector<int> order;
        int vertex_prev = 0;
        int vertex_next = 0;
        for (size_t i = 0; i < vertex_id.size(); i++) {
            // Se comprueban los puntos anterior y siguiente
            if (vertex_id[i] == 0) vertex_prev = static_cast<int>(this->size()) - 1;
            else vertex_prev = vertex_id[i] - 1;

            if (vertex_id[i] == this->size() - 1) vertex_next = 0;
            else vertex_next = vertex_id[i] + 1;

            if (this->at(vertex_prev).y == sPointH.pt1.y) {
                continue;
            } else {
                if (this->at(vertex_next).y == sPointH.pt1.y) {
                    int prev = isLeft(sPointH.pt1, sPointH.pt2, this->at(vertex_prev));
                    if (vertex_next == this->size() - 1) vertex_next = 0;
                    else vertex_next = vertex_next + 1;
                    int next = isLeft(sPointH.pt1, sPointH.pt2, this->at(vertex_next));
                    if (prev == next) nIntersection -= 2;
                    else nIntersection--;
                } else {
                    int prev = isLeft(sPointH.pt1, sPointH.pt2, this->at(vertex_prev));
                    int next = isLeft(sPointH.pt1, sPointH.pt2, this->at(vertex_next));
                    if (prev == next) nIntersection -= 2;
                    else nIntersection--;
                }

            }

        }
    }
    if (nIntersection < 0) return false;
    if (nIntersection % 2 == 0) return false;
    
    return true;

    //// Probar con OpenCV
    //std::vector<cv::Point2f> points;
    //for (int i = 0; i < size(); i++) {
    //  points.push_back(cv::Point2f(at(i).x, at(i).y));
    //}
    //double i_pol_test = cv::pointPolygonTest(mEntities, cv::Point2f(point.x, point.y), false);
    //if (i_pol_test == -1) return false;
    //else return true;

}

template<typename Point_t>
auto Polygon<Point_t>::length() const -> double
{
    double perimeter = 0.;

    for (size_t i = 1; i < this->size(); i++) {
        perimeter += distance(this->at(i - 1), this->at(i));
    }

    return perimeter;
}

template<typename Point_t>
auto Polygon<Point_t>::area() const -> double
{
    TL_TODO("Si el poligono es complejo hay que determinarla de otra forma. Primero hay que ver que sea complejo")
    double area = 0.;

    for (size_t i = 0; i < this->size(); i++) {
        if (i == this->size() - 1)
            area += crossProduct(this->at(i), this->at(0));
        else
            area += crossProduct(this->at(i), this->at(i + 1));
    }

    return std::abs(area / 2.);
}

template<typename Point_t>
auto Polygon<Point_t>::operator = (const Polygon<Point_t> &polygon) -> Polygon<Point_t> &
{
    if (this != &polygon) {
        Entity::operator = (polygon);
        Entities2D<Point_t>::operator = (polygon);
        polygonHoles = polygon.polygonHoles;
    }

    return *this;
}

template<typename Point_t>
auto Polygon<Point_t>::operator = (Polygon<Point_t> &&polygon) TL_NOEXCEPT -> Polygon<Point_t> &
{
    if (this != &polygon) {
        Entity::operator = (std::forward<Entity>(polygon));
        Entities2D<Point_t>::operator = (std::forward<Entities2D<Point_t>>(polygon));
        polygonHoles = std::forward<std::vector<PolygonHole<Point_t>>>(polygon.polygonHoles);
    }

    return *this;
}

template<typename Point_t>
auto Polygon<Point_t>::window() const -> Window<Point_t>
{
    Window<Point_t> w;

    for (size_t i = 0; i < this->size(); i++) {
        if (w.pt1.x > this->at(i).x) w.pt1.x = this->at(i).x;
        if (w.pt1.y > this->at(i).y) w.pt1.y = this->at(i).y;
        if (w.pt2.x < this->at(i).x) w.pt2.x = this->at(i).x;
        if (w.pt2.y < this->at(i).y) w.pt2.y = this->at(i).y;
    }

    return w;
}

template<typename Point_t>
auto Polygon<Point_t>::holes() const -> size_t
{
    return polygonHoles.size();
}

template<typename Point_t>
auto tl::Polygon<Point_t>::hole(size_t id) const -> PolygonHole<Point_t>
{
    return polygonHoles.at(id);
}

template<typename Point_t>
void Polygon<Point_t>::addHole(const PolygonHole<Point_t> &polygonHole)
{
    polygonHoles.push_back(polygonHole);
}




template<typename Point3_t>
Polygon3DHole<Point3_t>::Polygon3DHole() 
  : Entities3D<Point3_t>()
{
}

template<typename Point3_t>
Polygon3DHole<Point3_t>::Polygon3DHole(size_type size)
  : Entities3D<Point3_t>(size)
{
}

template<typename Point3_t>
Polygon3DHole<Point3_t>::Polygon3DHole(std::initializer_list<Point3_t> pointList)
  : Entities3D<Point3_t>(pointList)
{
}



template<typename Point3_t>
Polygon3D<Point3_t>::Polygon3D() 
  : Entity(Type::polygon3d),
    Entities3D<Point3_t>(),
    polygonHoles(0) 
{
}

template<typename Point3_t>
Polygon3D<Point3_t>::Polygon3D(size_type size)
  : Entity(Type::polygon3d),
    Entities3D<Point3_t>(size),
    polygonHoles(0) 
{
}

template<typename Point3_t>
Polygon3D<Point3_t>::Polygon3D(const Polygon3D &polygon) 
  : Entity(polygon), 
    Entities3D<Point3_t>(polygon),
    polygonHoles(polygon.polygonHoles)
{
}

template<typename Point3_t>
Polygon3D<Point3_t>::Polygon3D(Polygon3D &&polygon) TL_NOEXCEPT
  : Entity(std::forward<Entity>(polygon)), 
    Entities3D<Point3_t>(std::forward<Entities3D<Point3_t>>(polygon)),
    polygonHoles(std::forward<std::vector<Polygon3DHole<Point3_t>>>(polygon.polygonHoles))
{
}

template<typename Point3_t>
Polygon3D<Point3_t>::Polygon3D(const std::vector<Point3_t> &points) 
  : Entity(Type::polygon3d),
    Entities3D<Point3_t>(points),
    polygonHoles(0)
{
}

template<typename Point3_t>
Polygon3D<Point3_t>::Polygon3D(std::initializer_list<Point3_t> listPoints) 
  : Entity(Type::polygon3d),
    Entities3D<Point3_t>(listPoints),
    polygonHoles(0) 
{
}

template<typename Point3_t>
auto Polygon3D<Point3_t>::length() const -> double
{
    double perimeter = 0.;

    for (size_t i = 1; i < this->size(); i++) {
        perimeter += distance(this->at(i - 1), this->at(i));
    }

    return perimeter;
}

template<typename Point3_t>
auto Polygon3D<Point3_t>::operator = (const Polygon3D<Point3_t> &polygon) -> Polygon3D<Point3_t> &
{
    if (this != &polygon) {
        Entity::operator = (polygon);
        Entities3D<Point3_t>::operator = (polygon);
        polygonHoles = polygon.polygonHoles;
    }

    return *this;
}

template<typename Point3_t>
auto Polygon3D<Point3_t>::operator = (Polygon3D<Point3_t> &&polygon) TL_NOEXCEPT -> Polygon3D<Point3_t> &
{
    if (this != &polygon) {
        Entity::operator = (std::forward<Entity>(polygon));
        Entities3D<Point3_t>::operator = (std::forward<Entities3D<Point3_t>>(polygon));
        polygonHoles = std::forward<std::vector<Polygon3DHole<Point3_t>>>(polygon.polygonHoles);
    }

    return *this;
}

template<typename Point3_t>
auto Polygon3D<Point3_t>::boundingBox() const -> BoundingBox<Point3_t>
{
    BoundingBox<Point3_t> bounding_box;

    for (size_t i = 0; i < this->size(); i++) {
        if (bounding_box.pt1.x > this->at(i).x) bounding_box.pt1.x = this->at(i).x;
        if (bounding_box.pt1.y > this->at(i).y) bounding_box.pt1.y = this->at(i).y;
        if (bounding_box.pt1.z > this->at(i).z) bounding_box.pt1.z = this->at(i).z;
        if (bounding_box.pt2.x < this->at(i).x) bounding_box.pt2.x = this->at(i).x;
        if (bounding_box.pt2.y < this->at(i).y) bounding_box.pt2.y = this->at(i).y;
        if (bounding_box.pt2.z < this->at(i).z) bounding_box.pt2.z = this->at(i).z;
    }

    return bounding_box;
}

template<typename Point3_t>
auto Polygon3D<Point3_t>::holes() const -> size_t
{
    return polygonHoles.size();
}

template<typename Point3_t>
auto Polygon3D<Point3_t>::hole(size_t id) const -> Polygon3DHole<Point3_t>
{
    return polygonHoles.at(id);
}

template<typename Point3_t>
void Polygon3D<Point3_t>::addHole(const Polygon3DHole<Point3_t> &polygonHole)
{
    polygonHoles.push_back(polygonHole);
}





template <typename Point_t>
MultiPolygon<Point_t>::MultiPolygon()
  : Entity(Type::multipolygon2d),
    Entities2D<Polygon<Point_t>>()
{
}

template<typename Point_t>
MultiPolygon<Point_t>::MultiPolygon(size_type size)
  : Entity(Type::multipolygon2d),
    Entities2D<Polygon<Point_t>>(size) 
{
}

template<typename Point_t>
MultiPolygon<Point_t>::MultiPolygon(const MultiPolygon &multiPolygon) 
  : Entity(multiPolygon), 
    Entities2D<Polygon<Point_t>>(multiPolygon) 
{
}

template<typename Point_t>
MultiPolygon<Point_t>::MultiPolygon(MultiPolygon &&multiPolygon) TL_NOEXCEPT
  : Entity(std::forward<Entity>(multiPolygon)), 
    Entities2D<Polygon<Point_t>>(std::forward<Entities2D<Polygon<Point_t>>>(multiPolygon)) 
{
}

template<typename Point_t>
auto MultiPolygon<Point_t>::operator = (const MultiPolygon &multiPolygon) -> MultiPolygon<Point_t> &
{
    if (this != &multiPolygon) {
        Entity::operator = (multiPolygon);
        Entities2D<Polygon<Point_t>>::operator = (multiPolygon);
    }

    return *this;
}

template<typename Point_t>
auto MultiPolygon<Point_t>::operator = (MultiPolygon &&multiPolygon) TL_NOEXCEPT -> MultiPolygon<Point_t> &
{
    if (this != &multiPolygon) {
        Entity::operator = (std::forward<Entity>(multiPolygon));
        Entities2D<Polygon<Point_t>>::operator = (std::forward<Entities2D<Polygon<Point_t>>>(multiPolygon));
    }

    return *this;
}

template<typename Point_t>
auto MultiPolygon<Point_t>::window() const -> Window<Point_t>
{
    Window<Point_t> w;

    for (size_t i = 0; i < this->size(); i++) {
        w = joinWindow(w, this->at(i).window());
    }

    return w;
}




template <typename Point3_t>
MultiPolygon3D<Point3_t>::MultiPolygon3D()
  : Entity(Type::multipoygon3d),
    Entities3D<Polygon3D<Point3_t>>()
{
}

template<typename Point3_t>
MultiPolygon3D<Point3_t>::MultiPolygon3D(size_type size)
  : Entity(Type::multipoygon3d),
    Entities3D<Polygon3D<Point3_t>>(size) 
{
}

template<typename Point3_t>
MultiPolygon3D<Point3_t>::MultiPolygon3D(const MultiPolygon3D &multiPolygon) 
  : Entity(multiPolygon), 
    Entities3D<Polygon3D<Point3_t>>(multiPolygon) 
{
}

template<typename Point3_t>
MultiPolygon3D<Point3_t>::MultiPolygon3D(MultiPolygon3D &&multiPolygon) TL_NOEXCEPT
  : Entity(std::forward<Entity>(multiPolygon)), 
    Entities3D<Polygon3D<Point3_t>>(std::forward<MultiPolygon3D<Point3_t>>(multiPolygon)) 
{
}

template<typename Point3_t>
auto MultiPolygon3D<Point3_t>::operator = (const MultiPolygon3D &multiPolygon) -> MultiPolygon3D<Point3_t> &
{
    if (this != &multiPolygon) {
        Entity::operator = (multiPolygon);
        Entities3D<Polygon3D<Point3_t>>::operator = (multiPolygon);
    }

    return *this;
}

template<typename Point3_t>
auto MultiPolygon3D<Point3_t>::operator = (MultiPolygon3D &&multiPolygon) TL_NOEXCEPT -> MultiPolygon3D<Point3_t> &
{
    if (this != &multiPolygon) {
        Entity::operator = (std::forward<Entity>(multiPolygon));
        Entities3D<Polygon3D<Point3_t>>::operator = (std::forward<Entities3D<Polygon3D<Point3_t>>>(multiPolygon));
    }

    return *this;
}


template<typename Point3_t>
auto MultiPolygon3D<Point3_t>::boundingBox() const -> BoundingBox<Point3_t>
{
    BoundingBox<Point3_t> bounding_box;

    for (size_t i = 0; i < this->size(); i++) {
        bounding_box = joinBoundingBoxes(bounding_box, this->at(i).boundingBox());
    }

    return bounding_box;
}


/*! \} */ // end of geometry

} // End namespace tl
