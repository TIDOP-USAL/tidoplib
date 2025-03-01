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
#include "tidop/geometry/algorithms/distance.h"

namespace tl
{

/*! \addtogroup GeometricEntities
 *  \{
 */


 /*!
  * \brief LineString (2D Polyline)
  *
  * A class that represents a 2D polyline composed of consecutive points.
  * This class provides methods for managing and manipulating a sequence of 2D points
  * forming a connected polyline.
  *
  * \tparam Point_t The type representing a 2D point.
  */
template <typename Point_t>
class LineString
  : public Entity,
    public Entities2D<Point_t>
{

public:

    using size_type = typename LineString<Point_t>::size_type;

public:

    /*!
     * \brief Default constructor
     *
     * Creates an empty polyline with no points.
     */
    LineString();

    /*!
     * \brief Constructs a polyline with a specified number of points.
     * \param[in] size Number of points to reserve.
     * The points are default-initialized.
     */
    explicit LineString(size_type size);

    /*!
     * \brief Copy constructor
     * \param[in] lineString The LineString object to copy.
     * Creates a new polyline as a deep copy of the given one.
     */
    LineString(const LineString &lineString);

    /*!
     * \brief Move constructor
     * \param[in] lineString The LineString object to move.
     * Transfers ownership of the points from the given polyline,
     * leaving the source object in a valid but unspecified state.
     */
    LineString(LineString &&lineString) TL_NOEXCEPT;

    /*!
     * \brief Constructs a polyline from a vector of points.
     * \param[in] points A vector containing Point_t objects.
     * Initializes the polyline with the provided sequence of points.
     */
    explicit LineString(const std::vector<Point_t> &points);

    /*!
     * \brief Constructs a polyline from an initializer list.
     * \param[in] listPoints An initializer list of Point_t objects.
     */
    LineString(std::initializer_list<Point_t> listPoints);

    ~LineString() override = default;

    /*!
     * \brief Copy assignment operator
     * \param[in] lineString The LineString object to copy.
     * \return Reference to the modified LineString instance.
     */
    auto operator = (const LineString<Point_t> &lineString) -> LineString<Point_t> &;

    /*!
     * \brief Move assignment operator
     * \param[in] lineString The LineString object to move.
     * \return Reference to the modified LineString instance.
     *
     * Transfers ownership of the points from the given polyline.
     */
    auto operator = (LineString<Point_t> &&lineString) TL_NOEXCEPT -> LineString<Point_t> &;

    /*!
     * \brief Calculate the total length of the polyline.
     * \return The total length of the polyline.
     *
     * Computes the sum of the Euclidean distances between consecutive points.
     */
    auto length() const -> double;

    /*!
     * \brief Get the window (bounding box) of the polyline.
     * \return The bounding box window of the polyline.
     *
     * Computes the minimum bounding rectangle that encloses all points
     * of the polyline.
     */
    auto window() const -> Window<Point_t>;

};


using LineStringI = LineString<Point<int> >;
using LineStringF = LineString<Point<float> >;
using LineStringD = LineString<Point<double> >;




/*!
 * \brief 3D LineString
 *
 * A class representing a 3D polyline composed of consecutive points in a three-dimensional space.
 * This class provides methods for managing, manipulating, and analyzing a sequence of 3D points
 * that form a connected polyline.
 *
 * \tparam Point3_t The type representing a 3D point.
 */
template <typename Point3_t>
class LineString3D
  : public Entity,
    public Entities3D<Point3_t>
{

public:

    using size_type = typename LineString3D<Point3_t>::size_type;

public:

    /*!
     * \brief Default constructor
     *
     * Creates an empty 3D polyline with no points.
     */
    LineString3D();

    /*!
     * \brief Constructs a 3D polyline with a specified number of points.
     * \param[in] size Number of points to reserve.
     * The points are default-initialized.
     */
    explicit LineString3D(size_type size);

    /*!
     * \brief Copy constructor
     * \param[in] lineString The LineString3D object to copy.
     */
    LineString3D(const LineString3D &lineString);

    /*!
     * \brief Move constructor
     * \param[in] lineString The LineString3D object to move.
     */
    LineString3D(LineString3D &&lineString) TL_NOEXCEPT;

    /*!
     * \brief Constructs a 3D polyline from a vector of points.
     * \param[in] points A vector containing Point3_t objects.
     * Initializes the polyline with the provided sequence of points.
     */
    explicit LineString3D(const std::vector<Point3_t> &points);

    /*!
     * \brief Constructs a 3D polyline from an initializer list.
     * \param[in] listPoints An initializer list of Point3_t objects.
     */
    LineString3D(std::initializer_list<Point3_t> listPoints);

    ~LineString3D() override = default;

    /*!
     * \brief Copy assignment operator
     * \param[in] lineString The LineString3D object to copy.
     * \return Reference to the modified LineString3D instance.
     */
    auto operator = (const LineString3D<Point3_t> &lineString) -> LineString3D<Point3_t> &;

    /*!
     * \brief Move assignment operator
     * \param[in] lineString The LineString3D object to move.
     * \return Reference to the modified LineString3D instance.
     */
    auto operator = (LineString3D<Point3_t> &&lineString) TL_NOEXCEPT -> LineString3D<Point3_t> &;

    /*!
     * \brief Calculates the total length of the 3D polyline.
     * \return The total length of the polyline.
     *
     * Computes the sum of the Euclidean distances between consecutive points
     * in 3D space.
     */
    auto length() const -> double;

    /*!
     * \brief Computes the bounding box of the 3D polyline.
     * \return The bounding box that encloses the entire polyline.
     *
     * The bounding box is the smallest rectangular cuboid that fully contains
     * all points in the polyline.
     */
    auto boundingBox() const -> BoundingBox<Point3_t>;
};



using LineString3dI = LineString3D<Point3<int>>;
using LineString3dD = LineString3D<Point3<double>>;
using LineString3dF = LineString3D<Point3<float>>;






/* LineString implementation */

template <typename Point_t>
LineString<Point_t>::LineString()
  : Entity(Type::linestring2d), 
    Entities2D<Point_t>() 
{
}

template<typename Point_t>
LineString<Point_t>::LineString(size_type size)
  : Entity(Type::linestring2d), 
    Entities2D<Point_t>(size) 
{
}

template <typename Point_t>
LineString<Point_t>::LineString(const LineString &lineString)
  : Entity(lineString),
    Entities2D<Point_t>(lineString)
{
}

template <typename Point_t>
LineString<Point_t>::LineString(LineString &&lineString) TL_NOEXCEPT
  : Entity(std::forward<Entity>(lineString)),
    Entities2D<Point_t>(std::forward<Entities2D<Point_t>>(lineString))
{
}

template <typename Point_t>
LineString<Point_t>::LineString(const std::vector<Point_t> &points)
  : Entity(Type::linestring2d), 
    Entities2D<Point_t>(points) 
{
}

template <typename Point_t>
LineString<Point_t>::LineString(std::initializer_list<Point_t> listPoints)
  : Entity(Type::linestring2d), 
    Entities2D<Point_t>(listPoints) 
{
}

template<typename Point_t>
auto LineString<Point_t>::length() const -> double
{
    double length = 0.;

    for (size_t i = 1; i < this->size(); i++) {
        length += distance(this->at(i - 1), this->at(i));
    }

    return length;
}

template<typename Point_t>
auto LineString<Point_t>::operator = (const LineString<Point_t> &lineString) -> LineString<Point_t> &
{
    if (this != &lineString) {
        Entity::operator = (lineString);
        Entities2D<Point_t>::operator = (lineString);
    }

    return *this;
}

template<typename Point_t>
auto LineString<Point_t>::operator = (LineString<Point_t> &&lineString) TL_NOEXCEPT -> LineString<Point_t> &
{
    if (this != &lineString) {
        Entity::operator = (std::forward<Entity>(lineString));
        Entities2D<Point_t>::operator = (std::forward<Entities2D<Point_t>>(lineString));
    }

    return *this;
}

template<typename Point_t>
auto LineString<Point_t>::window() const -> Window<Point_t> 
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



/* LineString3D implementation */


template <typename Point3_t>
LineString3D<Point3_t>::LineString3D()
  : Entity(Entity::Type::linestring3d), 
    Entities3D<Point3_t>() 
{
}

template<typename Point3_t>
LineString3D<Point3_t>::LineString3D(size_type size)
  : Entity(Type::linestring3d), 
    Entities3D<Point3_t>(size) 
{
}

template <typename Point3_t>
LineString3D<Point3_t>::LineString3D(const LineString3D &lineString)
  : Entity(lineString), 
    Entities3D<Point3_t>(lineString)
{
}

template <typename Point3_t>
LineString3D<Point3_t>::LineString3D(LineString3D &&lineString) TL_NOEXCEPT
  : Entity(std::forward<Entity>(lineString)), 
    Entities3D<Point3_t>(std::forward<Entities3D<Point3_t>>(lineString))
{
}

template <typename Point3_t>
LineString3D<Point3_t>::LineString3D(const std::vector<Point3_t> &points)
  : Entity(Type::linestring3d), 
    Entities3D<Point3_t>(points) 
{
}

template <typename Point3_t>
LineString3D<Point3_t>::LineString3D(std::initializer_list<Point3_t> listPoints)
  : Entity(Type::linestring3d), 
    Entities3D<Point3_t>(listPoints) 
{
}

template<typename Point3_t>
auto LineString3D<Point3_t>::length() const -> double
{
    double length = 0.;

    for (size_t i = 1; i < this->size(); i++) {
        length += distance(this->at(i - 1), this->at(i));
    }

    return length;
}

template<typename Point3_t>
auto LineString3D<Point3_t>::operator = (const LineString3D &lineString) -> LineString3D<Point3_t> &
{
    if (this != &lineString) {
        Entity::operator = (lineString);
        Entities3D<Point3_t>::operator = (lineString);
    }

    return *this;
}

template<typename Point3_t>
auto LineString3D<Point3_t>::operator = (LineString3D &&lineString) TL_NOEXCEPT -> LineString3D<Point3_t> &
{
    if (this != &lineString) {
        Entity::operator = (std::forward<Entity>(lineString));
        Entities3D<Point3_t>::operator = (std::forward<Entities3D<Point3_t>>(lineString));
    }

    return *this;
}

template<typename Point3_t>
auto LineString3D<Point3_t>::boundingBox() const -> BoundingBox<Point3_t>
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


/*! \} */ 

}
