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


#include "tidop/core/defs.h"

#include <string>

#include "tidop/math/angles.h"

namespace tl
{


/*!
 * \addtogroup geospatial
 *
 * \{
 */

class Ellipsoid
{

private:

    std::string name;
    double a; // semi-major axis;
    double b; // semi-minor axis;
    double f; // flattening;

public:

    Ellipsoid(std::string ellipsoidName,
              double semiMajorAxis,
              double inverseFlattening)
      : name(std::move(ellipsoidName)),
        a(semiMajorAxis),
        f(inverseFlattening == 0. ? 0. : 1./inverseFlattening)
    {
        b = a * (1 - f);
    }

    /*!
     * \brief Equatorial radius
     */
    auto semiMajorAxis() const -> double;

    /*!
     * \brief Polar radius
     */
    auto semiMinorAxis() const -> double;

    /*!
     * \brief Flattening
     * \f[ f = \frac{a - b}{a} \f]
     */
    auto flattening() const -> double;

    /*!
     * \brief Inverse flattening
     * \f[ 1/f = \frac{a}{a - b} \f]
     * 
     * https://docs.ogc.org/is/18-010r7/18-010r7.html#66 
     * ISO 19111 allows an oblate ellipsoid to be defined through semi-major
     * axis (a) and either semi-minor axis (b) or inverse flattening (1/f). 
     * If semi-minor axis is used as the second defining parameter the value
     * for inverse flattening to be shown in the WKT string should be calculated 
     * from 1/f  =  a / (a – b).
     *
     * ISO 19111 also allows for the earth model to be a sphere, for which
     * 1/f is infinite. In this document if the earth model is a sphere 
     * <inverse flattening> shall be given an artificial value of zero.
     */
    auto inverseFlattening() const -> double;

    /*!
     * \brief Second flattening
     * \f[ e^{'} = \frac{a - b}{b} \f]
     */
    auto secondFlattening() const -> double;

    /*!
     * \brief Third flattening
     * \f[ n = \frac{a - b}{a + b} \f]
     */
    auto thirdFlattening() const -> double;

    /*!
     * \brief Eccentricity
     * \f[ e = \sqrt{\frac{a^2 - b^2}{a^2}} \f]
     */
    auto eccentricity() const -> double;

    /*!
     * \brief Second eccentricity
     * \f[ e^{'} = \sqrt{\frac{a^2 - b^2}{b^2}} \f]
     */
    auto secondEccentricity() const -> double;

    /*!
     * \brief Third eccentricity
     * \f[ e^{''} = \sqrt{\frac{a^2 - b^2}{a^2 + b^2}} \f]
     */
    auto thirdEccentricity() const -> double;

    /*!
     * \brief Linear eccentricity
     * The linear eccentricity is the distance between the center point of the ellipse and either foci.
     *
     * \f[ E = \sqrt{a^2 - b^2} \f]
     *
     */
    auto linearEccentricity() const -> double;

    /*!
     * \brief Geocentric Radius
     * \param[in] lat Latidude in degrees
     * \return 
     */
    auto geocentricRadius(double lat) const -> double;

    auto primeVertical(double lat) const -> double;
    auto meridionalRadiusOfCurvature(double lat) const -> double;
    auto authalicRadius() const -> double;



    auto geodeticToGeocentricLatitude(double lat, double h = 0.) -> double;

    /*!
     * \brief Parametric latitude or reduced latitude
     */
    auto geodeticToParametricLatitude(double lat) -> double;

    /*!
     * \brief Authalic Latitude
     * \param[in] lat Latitude in degrees
     * \return The authalic latitude corresponding to the given latitude
     */
    auto geodeticToAuthalicLatitude(double lat) const -> double;

};



const auto GRS80 = Ellipsoid("GRS 80", 6378388., 297.);
const auto WGS84 = Ellipsoid("WGS 84", 6378137., 298.257222101);




inline auto Ellipsoid::semiMajorAxis() const -> double
{
    return a;
}

inline auto Ellipsoid::semiMinorAxis() const -> double
{
    return b;
}

inline auto Ellipsoid::flattening() const -> double
{
    return f;
}

inline auto Ellipsoid::inverseFlattening() const -> double
{
    if (f == 0.) return 0.;
    return 1. / f;
}

inline auto Ellipsoid::secondFlattening() const -> double
{
    return (a - b) / b;
}

inline auto Ellipsoid::thirdFlattening() const -> double
{
    return f / (consts::two<double> - f);
}

inline auto Ellipsoid::eccentricity() const -> double
{
    return std::sqrt(f * (2 - f));
}

inline auto Ellipsoid::secondEccentricity() const -> double
{
    auto b_2 = b * b;
    return std::sqrt((a * a - b_2) / (b_2));
}

inline auto Ellipsoid::thirdEccentricity() const -> double
{
    auto a_2 = a * a;
    auto b_2 = b * b;
    return std::sqrt(a_2 - b_2) / std::sqrt(a_2 + b_2);
}

inline auto Ellipsoid::linearEccentricity() const -> double
{
    return std::sqrt(a * a - b * b);
}

inline auto Ellipsoid::geodeticToGeocentricLatitude(double lat, double h) -> double
{   
    double lat_rad = lat * consts::deg_to_rad<double>;
    double geocentric_latitude;
    if (h != 0.) {
        double N = primeVertical(lat);
        geocentric_latitude = atan(tan(lat_rad)*(N*(1-f)*(1-f)+h)/(N+h));
    } else {
        geocentric_latitude = atan((1-f) * (1-f) * tan(lat_rad));
    }
    
    return geocentric_latitude * consts::rad_to_deg<double>;
}

inline auto Ellipsoid::geodeticToParametricLatitude(double lat) -> double
{
    double lat_rad = lat * consts::deg_to_rad<double>;
    double e = eccentricity();

    return atan(sqrt(1-e*e) * tan(lat_rad));
}

inline auto Ellipsoid::geodeticToAuthalicLatitude(double lat) const -> double
{
    return 0.0;
}

inline auto Ellipsoid::geocentricRadius(double lat) const -> double
{
    double lat_rad = lat * consts::deg_to_rad<double>;
    double cos_lat = cos(lat_rad);
    double sin_lat = sin(lat_rad);

    return std::sqrt((std::pow(a * a * cos_lat, 2) + std::pow(b * b * sin_lat, 2)) /
                     (std::pow(a * cos_lat, 2) + std::pow(b * sin_lat, 2)));
        
}

inline auto Ellipsoid::primeVertical(double lat) const -> double
{
    double e = eccentricity();
    double sin_lat = sin(lat * consts::deg_to_rad<double>);
    return a / std::sqrt(1 - e*e * sin_lat*sin_lat);
}

inline auto Ellipsoid::meridionalRadiusOfCurvature(double lat) const -> double
{
    double e = eccentricity();
    double N = primeVertical(lat);
    return (1 - e*e)*std::pow(N,3)/(a*a);
}

inline auto Ellipsoid::authalicRadius() const -> double
{
    double e = eccentricity();
    double second_eccentricity = secondEccentricity();

    return sqrt(a*a/2. + b * b * std::atanh(e) / (2. * e));
}

/*! \} */ // end of geospatial

} // End namespace tl